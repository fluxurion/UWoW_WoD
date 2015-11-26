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

#ifndef BattlegroundPackets_h__
#define BattlegroundPackets_h__

#include "Packet.h"
#include "Packets/LFGPackets.h"

namespace WorldPackets
{
    namespace Battleground
    {
        class PVPSeason final : public ServerPacket
        {
        public:
            PVPSeason() : ServerPacket(SMSG_PVP_SEASON, 8) { }

            WorldPacket const* Write() override;

            uint32 PreviousSeason = 0;
            uint32 CurrentSeason = 0;
        };

        struct RideTicket
        {
            ObjectGuid RequesterGuid;
            uint32 Id = 0;
            uint32 Type = 0;
            uint32 Time = 0;
        };

        struct BattlefieldStatusHeader
        {
            RideTicket Ticket;
            uint64 QueueID = 0;
            uint32 InstanceID = 0;
            uint8 RangeMax = 0;
            uint8 RangeMin = 0;
            uint8 TeamSize = 0;
            bool RegisteredMatch = false;
            bool TournamentRules = false;
        };

        struct BracketInfo
        {
            uint32 PersonalRating = 0;
            uint32 Ranking = 0;
            uint32 SeasonPlayed = 0;
            uint32 SeasonWon = 0;
            uint32 WeeklyPlayed = 0;
            uint32 WeeklyWon = 0;
            uint32 BestWeeklyRating = 0;
            uint32 BestSeasonRating = 0;
        };

        class RatedInfo final : public ServerPacket
        {
        public:
            RatedInfo() : ServerPacket(SMSG_RATED_BATTLEFIELD_INFO, 32 * BRACKET_TYPE_MAX) { }

            WorldPacket const* Write() override;

            BracketInfo Info[BRACKET_TYPE_MAX];
        };

        class ListClient final : public ClientPacket
        {
        public:
            ListClient(WorldPacket&& packet) : ClientPacket(CMSG_BATTLEFIELD_LIST, std::move(packet)) { }

            void Read() override;

            uint32 ListID = 0;
        };

        ///< CMSG_BF_MGR_ENTRY_INVITE_RESPONSE
        ///< CMSG_BF_MGR_QUEUE_INVITE_RESPONSE
        class EntryOrQueueInviteResponse final : public ClientPacket
        {
        public:
            EntryOrQueueInviteResponse(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

            void Read() override;

            uint64 QueueID = 0;
            bool AcceptedInvite = false;
        };

        class ExitRequest final : public ClientPacket
        {
        public:
            ExitRequest(WorldPacket&& packet) : ClientPacket(CMSG_BF_MGR_QUEUE_EXIT_REQUEST, std::move(packet)) { }

            void Read() override;

            uint64 QueueID = 0;
        };

        class Port final : public ClientPacket
        {
        public:
            Port(WorldPacket&& packet) : ClientPacket(CMSG_BATTLEFIELD_PORT, std::move(packet)) { }

            void Read() override;

            RideTicket Ticket;
            bool AcceptedInvite = false;
        };

        struct IgnorMapInfo
        {
            IgnorMapInfo() { map[0] = 0, map[1] = 0; }
            uint32 map[2];
        };

        class Join final : public ClientPacket
        {
        public:
            Join(WorldPacket&& packet) : ClientPacket(CMSG_BATTLEMASTER_JOIN, std::move(packet)) { }

            void Read() override;

            uint64 QueueID = 0;
            uint8 Roles = 0;
            IgnorMapInfo BlacklistMap;
            bool JoinAsGroup = false;
        };

        class JoinArena final : public ClientPacket
        {
        public:
            JoinArena(WorldPacket&& packet) : ClientPacket(CMSG_BATTLEMASTER_JOIN_ARENA, std::move(packet)) { }

            void Read() override;

            uint8 TeamSizeIndex = 0;
        };

        class PVPLogData final : public ServerPacket
        {
        public:
            PVPLogData() : ServerPacket(SMSG_PVP_LOG_DATA, 8) { }

            WorldPacket const* Write() override;

            struct RatingData
            {
                int32 Prematch[MAX_TEAMS] = { };
                int32 Postmatch[MAX_TEAMS] = { };
                int32 PrematchMMR[MAX_TEAMS] = { };
            };

            struct HonorData
            {
                uint32 HonorKills = 0;
                uint32 Deaths = 0;
                uint32 ContributionPoints = 0;
            };

            struct PlayerData
            {
                ObjectGuid PlayerGUID;
                uint32 Kills = 0;
                uint8 Faction = 0;
                bool IsInWorld = false;
                Optional<HonorData> Honor;
                uint32 DamageDone = 0;
                uint32 HealingDone = 0;
                Optional<uint32> PreMatchRating;
                Optional<int32> RatingChange;
                Optional<uint32> PreMatchMMR;
                Optional<int32> MmrChange;
                std::vector<int32> Stats;
                int32 PrimaryTalentTree = 0;
                uint32 PrimaryTalentTreeNameIndex = 0;
            };

            Optional<uint8> Winner;
            std::vector<PlayerData> Players;
            Optional<RatingData> Ratings;
            int8 PlayerCount[MAX_TEAMS] = { };
        };

        class AreaSpiritHealerQuery final : public ClientPacket
        {
        public:
            AreaSpiritHealerQuery(WorldPacket&& packet) : ClientPacket(CMSG_AREA_SPIRIT_HEALER_QUERY, std::move(packet)) { }

            void Read() override;

            ObjectGuid HealerGuid;
        };

        class AreaSpiritHealerQueue final : public ClientPacket
        {
        public:
            AreaSpiritHealerQueue(WorldPacket&& packet) : ClientPacket(CMSG_AREA_SPIRIT_HEALER_QUEUE, std::move(packet)) { }

            void Read() override;

            ObjectGuid HealerGuid;
        };

        class AreaSpiritHealerTime final : public ServerPacket
        {
        public:
            AreaSpiritHealerTime() : ServerPacket(SMSG_AREA_SPIRIT_HEALER_TIME, 25) { }

            WorldPacket const* Write() override;

            ObjectGuid HealerGuid;
            uint32 TimeLeft = 0;
        };

        class ReportPvPPlayerAFKResult final : public ServerPacket
        {
        public:
            ReportPvPPlayerAFKResult() : ServerPacket(SMSG_REPORT_PVP_PLAYER_AFK_RESULT, 16 + 1 + 1 + 1) { }

            WorldPacket const* Write() override;

            enum ResultCode : uint8
            {
                PVP_REPORT_AFK_SUCCESS = 0,
                PVP_REPORT_AFK_GENERIC_FAILURE = 1,
                PVP_REPORT_AFK_SYSTEM_ENABLED = 5,
                PVP_REPORT_AFK_SYSTEM_DISABLED = 6
            };

            ObjectGuid Offender;
            uint8 NumPlayersIHaveReported = 0;
            uint8 NumBlackMarksOnOffender = 0;
            uint8 Result = PVP_REPORT_AFK_GENERIC_FAILURE;
        };

        class BattlefieldList final : public ServerPacket
        {
        public:
            BattlefieldList() : ServerPacket(SMSG_BATTLEFIELD_LIST, 25) { }

            WorldPacket const* Write() override;

            ObjectGuid BattlemasterGuid;
            uint32 BattlemasterListID = 0;
            uint8 MaxLevel = 0;
            uint8 MinLevel = 0;
            std::vector<uint32> Battlefields;

            bool PvpAnywhere = false;
            bool HasHolidayWinToday = false;
            bool HasRandomWinToday = false;
            bool IsRandomBG = false;
        };

        class PVPOptionsEnabled final : public ServerPacket
        {
        public:
            PVPOptionsEnabled() : ServerPacket(SMSG_PVP_OPTIONS_ENABLED, 25) { }

            WorldPacket const* Write() override;

            bool RatedArenas = false;
            bool ArenaSkirmish = false;
            bool PugBattlegrounds = false;
            bool WargameBattlegrounds = false;
            bool WargameArenas = false;
            bool RatedBattlegrounds = false;
        };

        class RequestPVPRewardsResponse final : public ServerPacket
        {
        public:
            RequestPVPRewardsResponse() : ServerPacket(SMSG_REQUEST_PVP_REWARDS_RESPONSE, 40 + 50) { }

            WorldPacket const* Write() override;

            WorldPackets::LFG::ShortageReward Reward[2] = { };
            uint32 RewardPointsThisWeek = 0;
            uint32 MaxRewardPointsThisWeek = 0;
            uint32 RatedRewardPointsThisWeek = 0;
            uint32 RatedMaxRewardPointsThisWeek = 0;
            uint32 RandomRewardPointsThisWeek = 0;
            uint32 RandomMaxRewardPointsThisWeek = 0;
            uint32 ArenaRewardPointsThisWeek = 0;
            uint32 ArenaMaxRewardPointsThisWeek = 0;
            uint32 ArenaRewardPoints = 0;
            uint32 RatedRewardPoints = 0;
        };

        class PlayerPositions final : public ServerPacket
        {
        public:
            PlayerPositions() : ServerPacket(SMSG_BATTLEGROUND_PLAYER_POSITIONS, 25) { }

            WorldPacket const* Write() override;

            struct BattlegroundPlayerPosition
            {
                Position Pos;
                ObjectGuid Guid;
                uint8 IconID = 0;
                uint8 ArenaSlot = 0;
            };

            std::vector<BattlegroundPlayerPosition> FlagCarriers;
        };

        class BattlefieldStatusNone final : public ServerPacket
        {
        public:
            BattlefieldStatusNone() : ServerPacket(SMSG_BATTLEFIELD_STATUS_NONE, 25) { }

            WorldPacket const* Write() override;

            RideTicket Ticket;
        };

        class BattlefieldStatusNeedConfirmation final : public ServerPacket
        {
        public:
            BattlefieldStatusNeedConfirmation() : ServerPacket(SMSG_BATTLEFIELD_STATUS_NEED_CONFIRMATION, 25) { }

            WorldPacket const* Write() override;

            BattlefieldStatusHeader Header;
            uint32 Mapid = 0;
            uint32 Timeout = 0;
            uint8 Role = 0;
        };

        class BattlefieldStatusFailed final : public ServerPacket
        {
        public:
            BattlefieldStatusFailed() : ServerPacket(SMSG_BATTLEFIELD_STATUS_FAILED, 25) { }

            WorldPacket const* Write() override;

            RideTicket Ticket;
            uint64 QueueID = 0;
            uint32 Reason = 0;
            ObjectGuid ClientID;
        };

        class PlayerJoined final : public ServerPacket
        {
        public:
            PlayerJoined(ObjectGuid guid) : ServerPacket(SMSG_BATTLEGROUND_PLAYER_JOINED, 20), Guid(guid) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
        };

        class PlayerLeft final : public ServerPacket
        {
        public:
            PlayerLeft(ObjectGuid guid) : ServerPacket(SMSG_BATTLEGROUND_PLAYER_LEFT, 20), Guid(guid) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
        };

        class Points final : public ServerPacket
        {
        public:
            Points() : ServerPacket(SMSG_BATTLEGROUND_POINTS, 3) { }

            WorldPacket const* Write() override;

            uint16 BgPoints = 0;
            bool Team = false; // 0 - alliance; 1 - horde
        };

        class Init final : public ServerPacket
        {
        public:
            Init(uint16 maxPoints) : ServerPacket(SMSG_BATTLEGROUND_INIT, 2), MaxPoints(maxPoints) { }

            WorldPacket const* Write() override;

            uint16 MaxPoints = 0;
        };

        class BFMgrEjected final : public ServerPacket
        {
        public:
            BFMgrEjected() : ServerPacket(SMSG_BF_MGR_EJECTED, 25) { }

            WorldPacket const* Write() override;

            uint64 QueueID = 0;
            uint8 BattleState = 0;
            uint8 Reason = 0;
            bool Relocated = false;
        };

        class BFMgrQueueRequestResponse final : public ServerPacket
        {
        public:
            BFMgrQueueRequestResponse() : ServerPacket(SMSG_BF_MGR_QUEUE_REQUEST_RESPONSE, 16 + 8 + 4 + 1 + 1 + 1) { }

            WorldPacket const* Write() override;

            ObjectGuid FailedPlayerGUID;
            uint64 QueueID = 0;
            uint32 AreaID = 0;
            uint8 BattleState = 0;
            uint8 Result = 0;
            bool LoggingIn = false;
        };

        struct BattlegroundCapturePointInfoData
        {
            ObjectGuid Guid;
            Position Pos;
            BattleGroundCapturePointState NodeState = NODE_STATE_NONE;
            uint32 CaptureTime = 0;
            uint32 CaptureTotalDuration = 0;
        };

        class BattlegroundCapturePointInfo final : public ServerPacket
        {
        public:
            BattlegroundCapturePointInfo() : ServerPacket(SMSG_BATTLEGROUND_CAPTURE_POINT_INFO, 16 + 8 + 1 + 4 + 4) { }

            WorldPacket const* Write() override;

            BattlegroundCapturePointInfoData Info;
        };

        class MapObjectivesInit final : public ServerPacket
        {
        public:
            MapObjectivesInit() : ServerPacket(SMSG_MAP_OBJECTIVES_INIT, 25) { }

            WorldPacket const* Write() override;

            std::vector<BattlegroundCapturePointInfoData> CapturePointInfo;
        };

        class BFMgrEjectPending final : public ServerPacket
        {
        public:
            BFMgrEjectPending() : ServerPacket(SMSG_BF_MGR_EJECT_PENDING, 9) { }

            WorldPacket const* Write() override;

            uint64 QueueID = 0;
            bool Remove = false;
        };

        class BFMgrEntryInvite final : public ServerPacket
        {
        public:
            BFMgrEntryInvite() : ServerPacket(SMSG_BF_MGR_ENTRY_INVITE, 8 + 4 + 4) { }

            WorldPacket const* Write() override;

            uint64 QueueID = 0;
            uint32 AreaID = 0;
            uint32 ExpireTime = 0;
        };

        class BFMgrStateChanged final : public ServerPacket
        {
        public:
            BFMgrStateChanged() : ServerPacket(SMSG_BF_MGR_STATE_CHANGED, 12) { }

            WorldPacket const* Write() override;

            uint64 QueueID = 0;
            uint32 State = 0;
        };

        class StatusWaitForGroups final : public ServerPacket
        {
        public:
            StatusWaitForGroups() : ServerPacket(SMSG_BATTLEFIELD_STATUS_WAIT_FOR_GROUPS, 12) { }

            WorldPacket const* Write() override;

            BattlefieldStatusHeader Header;
            uint32 Mapid = 0;
            uint32 Timeout = 0;
            uint8 TotalPlayers[MAX_TEAMS] = { };
            uint8 AwaitingPlayers[MAX_TEAMS] = { };
        };

        class BFMgrDropTimerStarted final : public ServerPacket
        {
        public:
            BFMgrDropTimerStarted() : ServerPacket(SMSG_BF_MGR_DROP_TIMER_STARTED, 12) { }

            WorldPacket const* Write() override;

            uint64 QueueID = 0;
            uint32 Time = 0;
        };

        class BFMgrDropTimerCanceled final : public ServerPacket
        {
        public:
            BFMgrDropTimerCanceled() : ServerPacket(SMSG_BF_MGR_DROP_TIMER_CANCELLED, 8) { }

            WorldPacket const* Write() override;

            uint64 QueueID = 0;
        };

        class BFMgrEntering final : public ServerPacket
        {
        public:
            BFMgrEntering() : ServerPacket(SMSG_BF_MGR_ENTERING, 11) { }

            WorldPacket const* Write() override;

            bool ClearedAFK = false;
            bool OnOffense = false;
            bool Relocated = false;
            uint64 QueueID = 0;
        };

        class BattlefieldStatusQueued final : public ServerPacket
        {
        public:
            BattlefieldStatusQueued() : ServerPacket(SMSG_BATTLEFIELD_STATUS_QUEUED, 11) { }

            WorldPacket const* Write() override;

            BattlefieldStatusHeader Header;
            uint32 AverageWaitTime = 0;
            uint32 WaitTime = 0;
            bool AsGroup = false;
            bool SuspendedQueue = false;
            bool EligibleForMatchmaking = false;
        };

        class BattlefieldStatusActive final : public ServerPacket
        {
        public:
            BattlefieldStatusActive() : ServerPacket(SMSG_BATTLEFIELD_STATUS_ACTIVE, 14) { }

            WorldPacket const* Write() override;

            BattlefieldStatusHeader Header;
            Milliseconds StartTimer = Milliseconds(0);
            Milliseconds ShutdownTimer = Milliseconds(0);
            uint32 Mapid = 0;
            bool ArenaFaction = false;
            bool LeftEarly = false;
        };

        class BFMgrQueueInvite final : public ServerPacket
        {
        public:
            BFMgrQueueInvite() : ServerPacket(SMSG_BF_MGR_QUEUE_INVITE, 31) { }

            WorldPacket const* Write() override;

            uint64 QueueID = 0;
            uint8 BattleState = 0;
            uint32 Timeout = 0;
            uint32 MinLevel = 0;
            uint32 MaxLevel = 0;
            uint32 InstanceID = 0;
            uint32 MapID = 0;
            bool Index = false;
        };

        class ConquestFormulaConstants final : public ServerPacket
        {
        public:
            ConquestFormulaConstants() : ServerPacket(SMSG_CONQUEST_FORMULA_CONSTANTS, 20) { }

            WorldPacket const* Write() override;

            uint32 PvpMinCPPerWeek = 0;
            uint32 PvpMaxCPPerWeek = 0;
            float PvpCPBaseCoefficient = 0.f;
            float PvpCPExpCoefficient = 0.f;
            float PvpCPNumerator = 0.f;
        };

        //< SMSG_BATTLEGROUND_INFO_THROTTLED
        class NullSMsg final : public ServerPacket
        {
        public:
            NullSMsg(OpcodeServer opcode) : ServerPacket(opcode) { }

            WorldPacket const* Write() override { return &_worldPacket; }
        };

        //< CMSG_REQUEST_BATTLEFIELD_STATUS
        //< CMSG_REQUEST_CONQUEST_FORMULA_CONSTANTS
        //< CMSG_REQUEST_RATED_BATTLEFIELD_INFO
        //< CMSG_GET_PVP_OPTIONS_ENABLED
        //< CMSG_BATTLEFIELD_LEAVE
        //< CMSG_PVP_LOG_DATA
        class NullCmsg final : public ClientPacket
        {
        public:
            NullCmsg(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

            void Read() override { }
        };

        class ArenaPrepOpponentSpecializations final : public ServerPacket
        {
        public:
            ArenaPrepOpponentSpecializations() : ServerPacket(SMSG_ARENA_PREP_OPPONENT_SPECIALIZATIONS) { }

            WorldPacket const* Write() override;

            struct OpponentSpecData
            {
                int32 SpecializationID = 0;
                int32 Unk = 0;
                ObjectGuid Guid;
            };

            std::vector<OpponentSpecData> Data;
        };

        class QueueRequest final : public ClientPacket
        {
        public:
            QueueRequest(WorldPacket&& packet) : ClientPacket(CMSG_BF_MGR_QUEUE_REQUEST, std::move(packet)) { }

            void Read() override;

            uint64 QueueID = 0;
        };

        class JoinSkirmish final : public ClientPacket
        {
        public:
            JoinSkirmish(WorldPacket&& packet) : ClientPacket(CMSG_BATTLEMASTER_JOIN_SKIRMISH, std::move(packet)) { }

            void Read() override;

            uint8 Roles = 0;
            uint8 Bracket = 0;
            bool JoinAsGroup = false;
            bool UnkBool = false;
        };
    }
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Battleground::RideTicket const& ticket);
ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Battleground::RideTicket& ticket);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Battleground::BattlefieldStatusHeader const& header);
ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Battleground::BattlefieldStatusHeader& header);

#endif // BattlegroundPackets_h__
