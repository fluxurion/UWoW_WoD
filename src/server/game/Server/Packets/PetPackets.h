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

#ifndef PetPackets_h__
#define PetPackets_h__

#include "Packet.h"
#include "PacketUtilities.h"
#include "Unit.h"

namespace WorldPackets
{
    namespace PetPackets
    {
        class Spells final : public ServerPacket
        {
        public:
            Spells() : ServerPacket(SMSG_PET_SPELLS_MESSAGE) { } //@TODO

            WorldPacket const* Write() override;

            struct ActionButtons
            {
                uint16 spell16 = 0;
                uint8 spell8 = 0;
                uint8 slot = 0;
            };

            struct Cooldown
            {
                uint32 SpellID = 0;
                int32 Duration = 0;
                int32 CategoryDuration = 0;
                int16 Category = 0;
            };

            struct History
            {
                int32 CategoryID = 0;
                int32 RecoveryTime = 0;
                uint8 ConsumedCharges = 0;
            };

            ObjectGuid PetGUID;
            uint16 CreatureFamily = 0;
            uint16 Specialization = 0;
            uint32 TimeLimit = 0;
            uint32 PetModeAndOrders = 0;
            ActionButtons Buttons[MAX_UNIT_ACTION_BAR_INDEX];
            std::vector<uint32> Actions;
            std::vector<Cooldown> Cooldowns;
            std::vector<History> Historys;
        };

        class Action final : public ClientPacket
        {
        public:
            Action(WorldPacket&& packet) : ClientPacket(CMSG_PET_ACTION, std::move(packet)) { } //@TODO

            void Read() override;

            ObjectGuid PetGUID;
            uint32 ActionData = 0;
            uint8 Slot = 0;
            ObjectGuid TargetGUID;
            G3D::Vector3 ActionPosition;
        };

        class SetPetSpecialization final : public ServerPacket
        {
        public:
            SetPetSpecialization(uint16 specializationID) : ServerPacket(SMSG_SET_PET_SPECIALIZATION, 2), SpecializationID(specializationID) { }

            WorldPacket const* Write() override;

            uint16 SpecializationID = 0;
        };

        class LearnPetSpecializationGroup final : public ClientPacket
        {
        public:
            LearnPetSpecializationGroup(WorldPacket&& packet) : ClientPacket(CMSG_LEARN_PET_SPECIALIZATION_GROUP, std::move(packet)) { }

            void Read() override;

            ObjectGuid PetGUID;
            int32 SpecGroupId = 0;
        };

        //< SMSG_PET_LEARNED_SPELLS
        //< SMSG_PET_UNLEARNED_SPELLS
        class LearnedRemovedSpells final : public ServerPacket
        {
        public:
            LearnedRemovedSpells(OpcodeServer opcode) : ServerPacket(opcode, 4) { }
            LearnedRemovedSpells(OpcodeServer opcode, std::vector<uint32> spellIDs) : ServerPacket(opcode, 4), SpellIDs(spellIDs) { }

            WorldPacket const* Write() override;

            std::vector<uint32> SpellIDs;
        };

        class Guids final : public ServerPacket
        {
        public:
            Guids() : ServerPacket(SMSG_PET_GUIDS, 4) { }

            WorldPacket const* Write() override;

            GuidVector PetGUIDs;
        };

        class DismissCritter final : public ClientPacket
        {
        public:
            DismissCritter(WorldPacket&& packet) : ClientPacket(CMSG_DISMISS_CRITTER, std::move(packet)) { }

            void Read() override;

            ObjectGuid CritterGUID;
        };

        class Sound final : public ServerPacket
        {
        public:
            Sound(ObjectGuid unitGUID, int32 action) : ServerPacket(SMSG_PET_ACTION_SOUND, 16 + 4), UnitGUID(unitGUID), Action(action) { }

            WorldPacket const* Write() override;

            ObjectGuid UnitGUID;
            int32 Action = 0;
        };

        class Mode final : public ServerPacket
        {
        public:
            Mode() : ServerPacket(SMSG_PET_MODE, 16 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid PetGUID;
            uint32 PetModeFlag = 0;

            //! not in jam data
            uint8 _reactState = 0;
            uint8 _commandState = 0;
            uint16 _flag = 0;
        };

        class StopAttack final : public ClientPacket
        {
        public:
            StopAttack(WorldPacket&& packet) : ClientPacket(CMSG_PET_STOP_ATTACK, std::move(packet)) { }

            void Read() override;

            ObjectGuid PetGUID;
        };

        class SetAction final : public ClientPacket
        {
        public:
            SetAction(WorldPacket&& packet) : ClientPacket(CMSG_PET_SET_ACTION, std::move(packet)) { } //@TODO

            void Read() override;

            ObjectGuid PetGUID;
            uint32 Index = 0;
            uint8 Unk = 0;

            uint32 Action = 0;
        };

        struct StableInfo //< Write
        {
            int32 PetSlot = 0;
            int32 PetNumber = 0;
            int32 CreatureID = 0;
            int32 DisplayID = 0;
            int32 ExperienceLevel = 0;
            uint8 PetFlags = 0;
            std::string PetName;
        };

        class StableList final : public ServerPacket
        {
        public:
            StableList() : ServerPacket(SMSG_PET_STABLE_LIST, 16 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid StableMaster;
            std::vector<StableInfo> Stables;
        };

        class Added final : public ServerPacket
        {
        public:
            Added() : ServerPacket(SMSG_PET_ADDED, 20 + 1 + 2) { }

            WorldPacket const* Write() override;

            StableInfo Stable;
        };

        class ActionFeedback final : public ServerPacket
        {
        public:
            ActionFeedback(uint32 spellID, uint8 response) : ServerPacket(SMSG_PET_ACTION_FEEDBACK, 4 + 1), SpellID(spellID), Response(response)  { }

            WorldPacket const* Write() override;

            uint32 SpellID = 0;
            uint8 Response = 0;
        };

        class StableResult final : public ServerPacket
        {
        public:
            StableResult() : ServerPacket(SMSG_PET_STABLE_RESULT, 1) { }

            WorldPacket const* Write() override;

            uint8 Result = 0;
        };

        class TameFailure final : public ServerPacket
        {
        public:
            TameFailure(uint8 result) : ServerPacket(SMSG_PET_TAME_FAILURE, 1), Result(result) { }

            WorldPacket const* Write() override;

            uint8 Result = 0;
        };
    }
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::PetPackets::StableInfo const& info);

#endif // PetPackets_h__
