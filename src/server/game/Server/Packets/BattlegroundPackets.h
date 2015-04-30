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

        struct StatusHeader
        {
            uint64 QueueID = 0;
            uint8 RangeMin = 0;
            uint8 RangeMax = 0;
            uint8 TeamSize = 0;
            uint32 InstanceID = 0;
            bool RegisteredMatch = 0;
            bool TournamentRules = 0;
            RideTicket TicketData;
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
            RatedInfo() : ServerPacket(SMSG_BATTLEFIELD_RATED_INFO, 32 * BRACKET_TYPE_MAX) { }

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

        ///< CMSG_BATTLEFIELD_MGR_ENTRY_INVITE_RESPONSE
        ///< CMSG_BATTLEFIELD_MGR_QUEUE_INVITE_RESPONSE
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

            RideTicket TicketData;
            bool AcceptedInvite = false;
        };

        class Join final : public ClientPacket
        {
        public:
            Join(WorldPacket&& packet) : ClientPacket(CMSG_BATTLEMASTER_JOIN, std::move(packet)) { }

            void Read() override;

            uint64 QueueID = 0;
            uint8 Roles = 0;
            uint32 BlacklistMap[2];
            bool JoinAsGroup = false;
        };

        class JoinArena final : public ClientPacket
        {
        public:
            JoinArena(WorldPacket&& packet) : ClientPacket(CMSG_BATTLEMASTER_JOIN_ARENA, std::move(packet)) { }

            void Read() override;

            uint8 TeamSizeIndex = 0;
        };

        struct RatingData
        {
            uint32 Prematch[BG_TEAMS_COUNT];
            uint32 Postmatch[BG_TEAMS_COUNT];
            uint32 PrematchMMR[BG_TEAMS_COUNT];
        };

        struct PVPLogDataPlayer
        {
            struct Honor
            {
                uint32 HonorKills = 0;
                uint32 Deaths = 0;
                uint32 ContributionPoints = 0;
            };

            ObjectGuid PlayerGUID;
            uint32 Kills = 0;
            uint32 DamageDone = 0;
            uint32 HealingDone = 0;
            std::vector<uint32> Stats;
            uint32 PrimaryTalentTree = 0;
            bool Faction = false;
            bool IsInWorld = false;
            Optional<Honor> HonorData;
            Optional<uint32> PreMatchRating;
            Optional<uint32> RatingChange;
            Optional<uint32> PreMatchMMR;
            Optional<uint32> MmrChange;
        };

        class PvPLogData final : public ServerPacket
        {
        public:
            PvPLogData() : ServerPacket(SMSG_PVP_LOG_DATA, 32 * BRACKET_TYPE_MAX) { }

            WorldPacket const* Write() override;

            uint8 PlayerCount[BG_TEAMS_COUNT];
            Optional<RatingData> RatingsData;
            Optional<uint8> Winner;
            Optional<std::vector<PVPLogDataPlayer>> Players;
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
            ReportPvPPlayerAFKResult() : ServerPacket(SMSG_REPORT_PVP_PLAYER_AFK_RESULT, 25) { }

            WorldPacket const* Write() override;

            ObjectGuid Offender;
            uint8 NumPlayersIHaveReported = 0;
            uint8 NumBlackMarksOnOffender = 0;
            uint8 Result = 0;
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
            RequestPVPRewardsResponse() : ServerPacket(SMSG_REQUEST_PVP_REWARDS_RESPONSE, 40) { }

            WorldPacket const* Write() override;

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

        struct BattlegroundPlayerPosition
        {
            G3D::Vector2 Pos;
            ObjectGuid Guid;
            uint8 IconID = 0;
            uint8 ArenaSlot = 0;
        };

        class PlayerPositions final : public ServerPacket
        {
        public:
            PlayerPositions() : ServerPacket(SMSG_BATTLEGROUND_PLAYER_POSITIONS, 25) { }

            WorldPacket const* Write() override;

            std::vector<BattlegroundPlayerPosition> FlagCarriers;
        };

        class StatusNone final : public ServerPacket
        {
        public:
            StatusNone() : ServerPacket(SMSG_BATTLEFIELD_STATUS_NONE, 25) { }

            WorldPacket const* Write() override;

            RideTicket Ticket;
        };

        class StatusNeedConfirmation final : public ServerPacket
        {
        public:
            StatusNeedConfirmation() : ServerPacket(SMSG_BATTLEFIELD_STATUS_NEED_CONFIRMATION, 25) { }

            WorldPacket const* Write() override;

            StatusHeader Header;
            uint32 Mapid = 0;
            uint32 Timeout = 0;
            uint8 Role = 0;
        };

        class StatusFailed final : public ServerPacket
        {
        public:
            StatusFailed() : ServerPacket(SMSG_BATTLEFIELD_STATUS_FAILED, 25) { }

            WorldPacket const* Write() override;

            RideTicket Ticket;
            uint64 QueueID = 0;
            uint32 Reason = 0;
            ObjectGuid ClientID;
        };

        class PlayerJoined final : public ServerPacket
        {
        public:
            PlayerJoined() : ServerPacket(SMSG_BATTLEGROUND_PLAYER_JOINED, 25) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
        };

        class PlayerLeft final : public ServerPacket
        {
        public:
            PlayerLeft() : ServerPacket(SMSG_BATTLEGROUND_PLAYER_LEFT, 25) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
        };

        class Points final : public ServerPacket
        {
        public:
            Points() : ServerPacket(SMSG_BATTLEGROUND_POINTS, 25) { }

            WorldPacket const* Write() override;

            uint16 BgPoints = 0;
            bool Team = false;
        };

        class Init final : public ServerPacket
        {
        public:
            Init() : ServerPacket(SMSG_BATTLEGROUND_INIT, 25) { }

            WorldPacket const* Write() override;

            uint16 MaxPoints = 0;
            bool Team = false;
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
            BFMgrQueueRequestResponse() : ServerPacket(SMSG_BF_MGR_QUEUE_REQUEST_RESPONSE, 25) { }

            WorldPacket const* Write() override;

            uint64 QueueID = 0;
            uint32 AreaID = 0;
            uint8 BattleState = 0;
            ObjectGuid FailedPlayerGUID;
            uint8 Result = 0;
            bool LoggingIn = false;
        };

        struct BattlegroundCapturePointInfo
        {
            ObjectGuid Guid;
            G3D::Vector2 Pos;
            uint8 State = 0;
            uint32 CaptureTime = 0;
            uint32 CaptureTotalDuration = 0;
        };

        class UpdateCapturePoint final : public ServerPacket
        {
        public:
            UpdateCapturePoint() : ServerPacket(SMSG_UPDATE_CAPTURE_POINT, 25) { }

            WorldPacket const* Write() override;

            BattlegroundCapturePointInfo CapturePointInfo;
        };

        class MapObjectivesInit final : public ServerPacket
        {
        public:
            MapObjectivesInit() : ServerPacket(SMSG_MAP_OBJECTIVES_INIT, 25) { }

            WorldPacket const* Write() override;

            std::vector<BattlegroundCapturePointInfo> CapturePointInfo;
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
            BFMgrEntryInvite() : ServerPacket(SMSG_BF_MGR_ENTRY_INVITE, 25) { }

            WorldPacket const* Write() override;

            uint64 QueueID = 0;
            uint32 AreaID = 0;
            uint32 ExpireTime = 0;
            bool Remove = false;
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

            StatusHeader Header;
            uint32 Mapid = 0;
            uint32 Timeout = 0;
            uint8 TotalPlayers[2];
            uint8 AwaitingPlayers[2];
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

        class StatusQueued final : public ServerPacket
        {
        public:
            StatusQueued() : ServerPacket(SMSG_BATTLEFIELD_STATUS_QUEUED, 11) { }

            WorldPacket const* Write() override;

            StatusHeader Header;
            uint32 AverageWaitTime = 0;
            uint32 WaitTime = 0;
            bool AsGroup = false;
            bool SuspendedQueue = false;
            bool EligibleForMatchmaking = false;
        };

        class StatusActive final : public ServerPacket
        {
        public:
            StatusActive() : ServerPacket(SMSG_BATTLEFIELD_STATUS_ACTIVE, 14) { }

            WorldPacket const* Write() override;

            StatusHeader Header;
            uint32 Mapid = 0;
            uint32 StartTimer = 0;
            uint32 ShutdownTimer = 0;
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
        //< CMSG_REQUEST_PVP_OPTIONS_ENABLED
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
    }
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Battleground::RideTicket const& ticket);
ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Battleground::RideTicket& ticket);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Battleground::StatusHeader const& header);
ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Battleground::StatusHeader& header);

#endif // BattlegroundPackets_h__
