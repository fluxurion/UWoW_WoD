/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BattlePetPacketsWorld_h__
#define BattlePetPacketsWorld_h__

#include "Packet.h"
#include "LFGPackets.h"
#include "Unit.h"
#include "BattlePetMgr.h"
#include "Player.h"

namespace WorldPackets
{
    namespace BattlePet
    {
        //< SMSG_BATTLE_PET_JOURNAL_LOCK_ACQUIRED
        //< SMSG_PET_BATTLE_FINISHED
        //< SMSG_PET_BATTLE_QUEUE_PROPOSE_MATCH
        //< SMSG_BATTLE_PET_JOURNAL_LOCK_DENIED
        //< SMSG_PET_BATTLE_CHAT_RESTRICTED
        //< SMSG_BATTLE_PET_LICENSE_CHANGED
        //< SMSG_BATTLE_PETS_HEALED
        class NullSMsg final : public ServerPacket
        {
        public:
            NullSMsg(OpcodeServer opcode) : ServerPacket(opcode, 0) { }

            WorldPacket const* Write() override { return &_worldPacket; }
        };

        //< CMSG_BATTLE_PET_REQUEST_JOURNAL
        //< CMSG_BATTLE_PET_REQUEST_JOURNAL_LOCK
        //< CMSG_PET_BATTLE_FINAL_NOTIFY
        //< CMSG_JOIN_PET_BATTLE_QUEUE
        //< CMSG_PET_BATTLE_SCRIPT_ERROR_NOTIFY
        class NullCmsg final : public ClientPacket
        {
        public:
            NullCmsg(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

            void Read() override { }
        };

        struct BattlePetJournalInfo
        {
            ObjectGuid BattlePetGUID;
            uint32 SpeciesID = 0;
            uint32 CreatureID = 0;
            uint32 CollarID = 0;
            uint16 BreedID = 0;
            uint16 Level = 0;
            uint16 Xp = 0;
            uint16 BattlePetDBFlags = 0;
            uint32 Power = 0;
            uint32 Health = 0;
            uint32 MaxHealth = 0;
            uint32 Speed = 0;
            uint8 BreedQuality = 0;
            std::string CustomName;
            ObjectGuid OwnerGuid;
            bool NoRename = false;
        };

        struct BattlePetSlot
        {
            BattlePetJournalInfo Pet;
            uint32 CollarID = 0;
            uint8 SlotIndex = 0;
            bool Locked = false;
        };

        class BattlePetJournal final : public ServerPacket
        {
        public:
            BattlePetJournal() : ServerPacket(SMSG_BATTLE_PET_JOURNAL, 2 + 4 + 4 + 1) { }

            WorldPacket const* Write() override;

            uint16 TrapLevel = 0;
            std::vector<BattlePetSlot> Slots;
            std::vector<BattlePetJournalInfo> Pets;
            bool HasJournalLock = false;
        };

        class Query final : public ClientPacket
        {
        public:
            Query(WorldPacket&& packet) : ClientPacket(CMSG_QUERY_BATTLE_PET_NAME, std::move(packet)) { }

            void Read() override;

            ObjectGuid BattlePetID;
            ObjectGuid UnitGUID;
        };

        class QueryResponse final : public ServerPacket
        {
        public:
            QueryResponse() : ServerPacket(SMSG_QUERY_BATTLE_PET_NAME_RESPONSE, 16 + 4 + 4 + 1 + 2 + 2 + 1) { }

            WorldPacket const* Write() override;

            ObjectGuid BattlePetID;
            uint32 CreatureID = 0;
            time_t Timestamp = time(nullptr);
            bool Allow = false;
            std::string Name;
            std::string DeclinedNames[MAX_DECLINED_NAME_CASES] = { };
            bool HasDeclined = false;
        };

        //< CMSG_BATTLE_PET_DELETE_PET
        //< CMSG_BATTLE_PET_DELETE_PET_CHEAT
        //< CMSG_BATTLE_PET_SUMMON
        //< CMSG_CAGE_BATTLE_PET
        //< CMSG_PET_BATTLE_QUIT_NOTIFY
        class BattlePetGuidRead final : public ClientPacket
        {
        public:
            BattlePetGuidRead(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

            void Read() override;

            ObjectGuid BattlePetGUID;
        };

        class BattlePetDeleted final : public ServerPacket
        {
        public:
            BattlePetDeleted(ObjectGuid battlePetGUID) : ServerPacket(SMSG_BATTLE_PET_DELETED, 16), BattlePetGUID(battlePetGUID) { }

            WorldPacket const* Write() override;

            ObjectGuid BattlePetGUID;
        };

        class ModifyName final : public ClientPacket
        {
        public:
            ModifyName(WorldPacket&& packet) : ClientPacket(CMSG_BATTLE_PET_MODIFY_NAME, std::move(packet)) { }

            void Read() override;

            ObjectGuid BattlePetGUID;
            ObjectGuid UnitGUID;
            std::string DeclinedNames[MAX_DECLINED_NAME_CASES] = { };
            std::string Name;
        };

        class SetBattleSlot final : public ClientPacket
        {
        public:
            SetBattleSlot(WorldPacket&& packet) : ClientPacket(CMSG_BATTLE_PET_SET_BATTLE_SLOT, std::move(packet)) { }

            void Read() override;

            ObjectGuid BattlePetGUID;
            uint8 SlotIndex = 0;
        };

        class SetFlags final : public ClientPacket
        {
        public:
            SetFlags(WorldPacket&& packet) : ClientPacket(CMSG_BATTLE_PET_SET_FLAGS, std::move(packet)) { }

            void Read() override;

            ObjectGuid BattlePetGUID;
            uint32 Flags = 0;
            uint8 ControlType = 0;
        };

        class Updates final : public ServerPacket
        {
        public:
            Updates() : ServerPacket(SMSG_BATTLE_PET_UPDATES, 1 + 4) { }

            WorldPacket const* Write() override;

            bool AddedPet = false;
            std::vector<BattlePetJournalInfo> Pets;
        };

        struct ActiveAbility
        {
            uint32 AbilityID = 0;
            uint16 CooldownRemaining = 0;
            uint16 LockdownRemaining = 0;
            uint8 AbilityIndex = 0;
            uint8 Pboid = 0;
        };

        struct EffectTarget
        {
            uint16 Type = 0;
            uint8 Petx = 0;
            uint32 TriggerAbilityID = 0;
            uint32 NewStatValue = 0;
            uint32 BroadcastTextID = 0;
            uint32 Health = 0;
            uint32 AuraInstanceID = 0;
            uint32 AuraAbilityID = 0;
            uint32 RoundsRemaining = 0;
            uint32 CurrentRound = 0;
            uint32 StateID = 0;
            uint32 StateValue = 0;
            uint32 ChangedAbilityID = 0;
            uint32 CooldownRemaining = 0;
            uint32 LockdownRemaining = 0;
        };

        struct Effect
        {
            uint32 AbilityEffectID = 0;
            uint16 Flags = 0;
            uint16 SourceAuraInstanceID = 0;
            uint16 TurnInstanceID = 0;
            uint8 PetBattleEffectType = 0;
            uint8 CasterPBOID = 0;
            uint8 StackDepth = 0;
            std::list<EffectTarget> EffectTargetData;
        };

        struct RoundResult
        {
            uint32 CurRound = 0;
            uint8 NextPetBattleState = 0;
            uint8 NextInputFlags[2] = { };
            uint8 NextTrapStatus[2] = { };
            uint16 RoundTimeSecs[2] = { };
            std::vector<Effect> EffectData;
            std::vector<ActiveAbility> Ability;
            std::vector<uint8> PetXDied;
        };

        //< SMSG_PET_BATTLE_FIRST_ROUND
        //< SMSG_PET_BATTLE_ROUND_RESULT
        //< SMSG_PET_BATTLE_REPLACEMENTS_MADE
        class BattleRound final : public ServerPacket
        {
        public:
            BattleRound(OpcodeServer opcode) : ServerPacket(opcode, 4 + 1 + 1 + 1 + 2 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            RoundResult MsgData;
        };

        struct FinalPet
        {
            ObjectGuid Guid;
            uint16 Level = 0;
            uint16 Xp = 0;
            uint32 Health = 0;
            uint32 MaxHealth = 0;
            uint16 InitialLevel = 0;
            uint8 Pboid = 0;
            bool Captured = false;
            bool Caged = false;
            bool SeenAction = false;
            bool AwardedXP = false;
        };

        struct FinalRound
        {
            bool Abandoned = false;
            bool PvpBattle = false;
            bool Winner[2] = { };
            uint32 NpcCreatureID[2] = { };
            std::vector<FinalPet> Pets;
        };

        class SceneObjectFinalRound final : public ServerPacket
        {
        public:
            SceneObjectFinalRound() : ServerPacket(SMSG_PET_BATTLE_FINAL_ROUND, 1 + 1 + 1 + 1 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            FinalRound MsgData;
        };

        struct Locations
        {
            uint32 LocationResult = 0;
            Position BattleOrigin;
            float BattleFacing = 0.0f;
            Position PlayerPositions[2] = { };
        };

        class FinalizeLocation final : public ServerPacket
        {
        public:
            FinalizeLocation(Locations loc) : ServerPacket(SMSG_PET_BATTLE_FINALIZE_LOCATION, 4 + 12 + 4 + 12 * 2), Location(loc) { }

            WorldPacket const* Write() override;

            Locations Location;
        };

        class PVPChallenge final : public ServerPacket
        {
        public:
            PVPChallenge() : ServerPacket(SMSG_PET_BATTLE_PVP_CHALLENGE, 16 + 4 + 12 + 4 + 12 * 2) { }

            WorldPacket const* Write() override;

            ObjectGuid ChallengerGUID;
            Locations Location;
        };

        struct BattleRequest
        {
            ObjectGuid TargetGUID;
            Locations Location;
        };

        class RequestWild final : public ClientPacket
        {
        public:
            RequestWild(WorldPacket&& packet) : ClientPacket(CMSG_PET_BATTLE_REQUEST_WILD, std::move(packet)) { }

            void Read() override;

            BattleRequest Battle;
        };

        class RequestPVP final : public ClientPacket
        {
        public:
            RequestPVP(WorldPacket&& packet) : ClientPacket(CMSG_PET_BATTLE_REQUEST_PVP, std::move(packet)) { }

            void Read() override;

            BattleRequest Battle;
        };

        class RequestFailed final : public ServerPacket
        {
        public:
            RequestFailed() : ServerPacket(SMSG_PET_BATTLE_REQUEST_FAILED, 1) { }

            WorldPacket const* Write() override;

            uint8 Reason = 0;
        };

        class SlotUpdates final : public ServerPacket
        {
        public:
            SlotUpdates() : ServerPacket(SMSG_PET_BATTLE_SLOT_UPDATES, 4 + 1 + 1) { }

            WorldPacket const* Write() override;

            std::vector<BattlePetSlot> Slots;
            bool AutoSlotted = false;
            bool NewSlotUnlocked = false;
        };

        class ReplaceFrontPet final : public ClientPacket
        {
        public:
            ReplaceFrontPet(WorldPacket&& packet) : ClientPacket(CMSG_PET_BATTLE_REPLACE_FRONT_PET, std::move(packet)) { }

            void Read() override;

            uint8 FrontPet = 0;
        };

        struct InputData
        {
            uint8 MoveType = 0;
            uint8 NewFrontPet = 0;
            uint8 DebugFlags = 0;
            uint8 BattleInterrupted = 0;
            uint32 AbilityID = 0;
            uint32 Round = 0;
            bool IgnoreAbandonPenalty = false;
        };

        struct QueueStatus
        {
            uint32 Status = 0;
            std::vector<uint32> SlotResult;
            Optional<uint32> ClientWaitTime;
            Optional<uint32> AverageWaitTime;
            WorldPackets::LFG::RideTicket Ticket;
        };

        class PetBattleQueueStatus final : public ServerPacket
        {
        public:
            PetBattleQueueStatus() : ServerPacket(SMSG_PET_BATTLE_QUEUE_STATUS, 4 + 4 + 1 + 1 + 28) { }

            WorldPacket const* Write() override;

            QueueStatus Msg;
        };

        class QueueProposeMatchResult final : public ClientPacket
        {
        public:
            QueueProposeMatchResult(WorldPacket&& packet) : ClientPacket(CMSG_PET_BATTLE_QUEUE_PROPOSE_MATCH_RESULT, std::move(packet)) { }

            void Read() override;

            bool Accepted = false;
        };

        class LeaveQueue final : public ClientPacket
        {
        public:
            LeaveQueue(WorldPacket&& packet) : ClientPacket(CMSG_LEAVE_PET_BATTLE_QUEUE, std::move(packet)) { }

            void Read() override;

            WorldPackets::LFG::RideTicket Ticket;
        };

        class RequestUpdate final : public ClientPacket
        {
        public:
            RequestUpdate(WorldPacket&& packet) : ClientPacket(CMSG_PET_BATTLE_REQUEST_UPDATE, std::move(packet)) { }

            void Read() override;

            ObjectGuid TargetGUID;
            bool Canceled = false;
        };

        //< SMSG_BATTLE_PET_RESTORED
        //< SMSG_BATTLE_PET_REVOKED
        //< SMSG_BATTLE_PET_CAGE_DATE_ERROR
        class GuidData final : public ServerPacket
        {
        public:
            GuidData(OpcodeServer opcode) : ServerPacket(opcode, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid BattlePetGUID;
        };

        struct BattlePetAura
        {
            int32 AbilityID = 0;
            uint32 InstanceID = 0;
            int32 RoundsRemaining = 0;
            int32 CurrentRound = 0;
            uint8 CasterPBOID = 0;
        };

        struct BattlePetAbility
        {
            int32 AbilityID = 0;
            int16 CooldownRemaining = 0;
            int16 LockdownRemaining = 0;
            int8 AbilityIndex = 0;
            uint8 Pboid = 0;
        };

        struct PetBattlePetUpdate
        {
            BattlePetJournalInfo* JournalInfo = nullptr;
            uint32 NpcTeamMemberID = 0;
            uint16 StatusFlags = 0; // same as Pet.Flags?
            uint8 Slot = 0;
            std::vector<BattlePetAbility> Abilities;
            std::vector<BattlePetAura> Auras;
            std::unordered_map<uint32 /*StateID*/, int32 /*StateValue*/> States;
        };

        struct PetBattlePlayerUpdate
        {
            ObjectGuid CharacterID;
            int32 TrapAbilityID = 0;
            int32 TrapStatus = 0;
            uint16 RoundTimeSecs = 0;
            std::vector<PetBattlePetUpdate> Pets;
            int8 FrontPet = 0;
            uint8 InputFlags = 0;
        };

        struct PetBattleEnviroUpdate
        {
            std::vector<BattlePetAura> Auras;
            std::unordered_map<uint32 /*StateID*/, int32 /*StateValue*/> States;
        };

        struct PetBattleFullUpdate
        {
            uint16 WaitingForFrontPetsMaxSecs = 0;
            uint16 PvpMaxRoundTime = 0;
            int32 CurRound = 0;
            uint32 NpcCreatureID = 0;
            uint32 NpcDisplayID = 0;
            int8 CurPetBattleState = 0;
            uint8 ForfeitPenalty = 0;
            ObjectGuid InitialWildPetGUID;
            bool IsPVP = false;
            bool CanAwardXP = false;
            PetBattlePlayerUpdate Players[2] = { };
            PetBattleEnviroUpdate Enviros[3] = { };
        };

        class PetBattleInitialUpdate final : public ServerPacket
        {
        public:
            PetBattleInitialUpdate() : ServerPacket(SMSG_PET_BATTLE_INITIAL_UPDATE, 124) { }

            WorldPacket const* Write() override;

            PetBattleFullUpdate MsgData;
        };

        class BattlePetError final : public ServerPacket
        {
        public:
            BattlePetError() : ServerPacket(SMSG_BATTLE_PET_ERROR, 8) { }

            WorldPacket const* Write() override;

            uint32 Result = 0;
            int32 CreatureID = 0;
        };

        class PetBattleInput final : public ClientPacket
        {
        public:
            PetBattleInput(WorldPacket&& packet) : ClientPacket(CMSG_PET_BATTLE_INPUT, std::move(packet)) { }

            void Read() override;

            uint8 MoveType = 0;
            int8 NewFrontPet = 0;
            uint8 DebugFlags = 0;
            uint8 BattleInterrupted = 0;
            bool IgnoreAbandonPenalty = false;
            int32 AbilityID = 0;
            int32 Round = 0;
        };
    }
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::ActiveAbility const& activeAbility);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::BattlePetSlot const& petBattleSlot);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::BattlePetJournalInfo const& battlePet);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::EffectTarget const& effectTarget);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::Effect const& effect);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::RoundResult const& roundResult);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::FinalPet const& finalPet);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::FinalRound const& finalRound);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::Locations& locations);
ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::BattlePet::Locations& locations);
ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::BattlePet::InputData& locations);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::PetBattlePlayerUpdate const& update);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::PetBattleEnviroUpdate const& update);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::PetBattleFullUpdate const& update);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::BattlePetAbility const& ability);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePet::BattlePetAura const& aura);

#endif // BattlePetPacketsWorld_h__
