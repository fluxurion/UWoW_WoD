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

#ifndef GuildPackets_h__
#define GuildPackets_h__

#include "Packet.h"
#include "ObjectGuid.h"

namespace WorldPackets
{
    namespace Guild
    {
        class QueryGuildInfo final : public ClientPacket
        {
        public:
            QueryGuildInfo(WorldPacket&& packet) : ClientPacket(CMSG_QUERY_GUILD_INFO, std::move(packet)) { }

            void Read() override;

            ObjectGuid PlayerGuid;
            ObjectGuid GuildGuid;
        };

        class QueryGuildInfoResponse final : public ServerPacket
        {
        public:
            struct GuildInfo
            {
                ObjectGuid GuildGUID;

                uint32 VirtualRealmAddress = 0;

                std::string GuildName;

                struct GuildInfoRank
                {
                    GuildInfoRank(uint32 id, uint32 order, std::string const& name)
                        : RankID(id), RankOrder(order), RankName(name)
                    { }

                    uint32 RankID;
                    uint32 RankOrder;
                    std::string RankName;

                    bool operator<(GuildInfoRank const& right) const
                    {
                        return RankID < right.RankID;
                    }
                };

                std::set<GuildInfoRank> Ranks;

                uint32 EmblemStyle = 0;
                uint32 EmblemColor = 0;
                uint32 BorderStyle = 0;
                uint32 BorderColor = 0;
                uint32 BackgroundColor = 0;
            };

            QueryGuildInfoResponse();

            WorldPacket const* Write() override;

            ObjectGuid GuildGuid;
            Optional<GuildInfo> Info;
        };

        class GuildGetRoster final : public ClientPacket
        {
        public:
            GuildGetRoster(WorldPacket&& packet) : ClientPacket(CMSG_GUILD_GET_ROSTER, std::move(packet)) { }

            void Read() override { }
        };

        struct GuildRosterProfessionData
        {
            int32 DbID = 0;
            int32 Rank = 0;
            int32 Step = 0;
        };

        struct GuildRosterMemberData
        {
            ObjectGuid Guid;
            int64 WeeklyXP = 0;
            int64 TotalXP = 0;
            int32 RankID = 0;
            int32 AreaID = 0;
            int32 PersonalAchievementPoints = 0;
            int32 GuildReputation = 0;
            int32 GuildRepToCap = 0;
            float LastSave = 0.0f;
            std::string Name;
            uint32 VirtualRealmAddress = 0;
            std::string Note;
            std::string OfficerNote;
            uint8 Status = 0;
            uint8 Level = 0;
            uint8 ClassID = 0;
            uint8 Gender = 0;
            bool Authenticated = false;
            bool SorEligible = false;
            GuildRosterProfessionData ProfessionData[2];
        };

        class GuildRoster final : public ServerPacket
        {
        public:
            GuildRoster() : ServerPacket(SMSG_GUILD_ROSTER) { }

            WorldPacket const* Write() override;

            std::vector<GuildRosterMemberData> MemberData;
            std::string WelcomeText;
            std::string InfoText;
            uint32 CreateDate = 0;
            int32 NumAccounts = 0;
            int32 GuildFlags = 0;
        };

        class GuildRosterUpdate final : public ServerPacket
        {
        public:
            GuildRosterUpdate() : ServerPacket(SMSG_GUILD_ROSTER_UPDATE) { }

            WorldPacket const* Write() override;

            std::vector<GuildRosterMemberData> MemberData;
        };

        class GuildUpdateMotdText final : public ClientPacket
        {
        public:
            GuildUpdateMotdText(WorldPacket&& packet) : ClientPacket(CMSG_GUILD_UPDATE_MOTD_TEXT, std::move(packet)) { }

            void Read() override;

            std::string MotdText;
        };

        class GuildCommandResult final : public ServerPacket
        {
        public:
            GuildCommandResult() : ServerPacket(SMSG_GUILD_COMMAND_RESULT) { }

            WorldPacket const* Write() override;

            std::string Name;
            int32 Result = 0;
            int32 Command = 0;
        };

        struct GuildBankItemInfo
        {
            struct GuildBankSocketEnchant
            {
                int32 SocketIndex = 0;
                int32 SocketEnchantID = 0;
            };

            WorldPackets::Item::ItemInstance Item;
            int32 Slot = 0;
            int32 Count = 0;
            int32 EnchantmentID = 0;
            int32 Charges = 0;
            int32 OnUseEnchantmentID = 0;
            int32 Flags = 0;
            bool Locked = false;
            std::vector<GuildBankSocketEnchant> SocketEnchant;
        };

        struct GuildBankTabInfo
        {
            int32 TabIndex = 0;
            std::string Name;
            std::string Icon;
        };

        class GuildBankQueryResults final : public ServerPacket
        {
        public:
            GuildBankQueryResults() : ServerPacket(SMSG_GUILD_BANK_QUERY_RESULTS) { }

            WorldPacket const* Write() override;

            std::vector<GuildBankItemInfo> ItemInfo;
            std::vector<GuildBankTabInfo> TabInfo;
            uint64 Money = 0;
            int32 WithdrawalsRemaining = 0;
            int32 Tab = 0;
            bool FullUpdate = false;
        };

        struct LFGuildRecruitData
        {
            ObjectGuid RecruitGUID;
            int32 RecruitVirtualRealm = 0;
            int32 CharacterClass = 0;
            int32 CharacterGender = 0;
            int32 CharacterLevel = 0;
            int32 ClassRoles = 0;
            int32 PlayStyle = 0;
            int32 Availability = 0;
            int32 SecondsSinceCreated = 0;
            int32 SecondsUntilExpiration = 0;
            std::string Name;
            std::string Comment;
        };

        class LFGuildRecruits final : public ServerPacket
        {
        public:
            LFGuildRecruits() : ServerPacket(SMSG_LF_GUILD_RECRUITS) { }

            WorldPacket const* Write() override;

            std::vector<LFGuildRecruitData> Recruits;
            time_t UpdateTime = time_t(0);
        };

        struct GuildPostData
        {
            int32 PlayStyle = 0;
            int32 Availability = 0;
            int32 ClassRoles = 0;
            int32 LevelRange = 0;
            int32 SecondsRemaining = 0;
            std::string Comment;
            bool Active = false;
        };

        class LFGuildPost final : public ServerPacket
        {
        public:
            LFGuildPost() : ServerPacket(SMSG_LF_GUILD_POST, 4) { }

            WorldPacket const* Write() override;

            Optional<GuildPostData> Post;
        };

        struct LFGuildBrowseData
        {
            ObjectGuid GuildGUID;
            uint32 GuildVirtualRealm = 0;
            int32 GuildMembers = 0;
            int32 GuildAchievementPoints = 0;
            int32 PlayStyle = 0;
            int32 Availability = 0;
            int32 ClassRoles = 0;
            int32 LevelRange = 0;
            int32 EmblemStyle = 0;
            int32 EmblemColor = 0;
            int32 BorderStyle = 0;
            int32 BorderColor = 0;
            int32 Background = 0;
            std::string GuildName;
            std::string Comment;
            int8 Cached = 0;
            int8 MembershipRequested = 0;
        };

        class LFGuildBrowse final : public ServerPacket
        {
        public:
            LFGuildBrowse() : ServerPacket(SMSG_LF_GUILD_BROWSE, 4) { }

            WorldPacket const* Write() override;

            std::vector<LFGuildBrowseData> Browses;
        };

        struct LFGuildApplicationData
        {
            ObjectGuid GuildGUID;
            int32 GuildVirtualRealm = 0;
            int32 ClassRoles = 0;
            int32 PlayStyle = 0;
            int32 Availability = 0;
            int32 SecondsSinceCreated = 0;
            std::string GuildName;
            std::string Comment;
        };

        class LFGuildApplication final : public ServerPacket
        {
        public:
            LFGuildApplication() : ServerPacket(SMSG_LF_GUILD_APPLICATIONS, 4 + 4) { }

            WorldPacket const* Write() override;

            int32 NumRemaining = 0;
            std::vector<LFGuildApplicationData> Applications;
        };

        class GuildReputationReactionChanged final : public ServerPacket
        {
        public:
            GuildReputationReactionChanged() : ServerPacket(SMSG_GUILD_REPUTATION_REACTION_CHANGED, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid MemberGUID;
        };

        class GuildNameChanged final : public ServerPacket
        {
        public:
            GuildNameChanged() : ServerPacket(SMSG_GUILD_NAME_CHANGED, 16 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid GuildGUID;
            std::string GuildName;
        };

        class GuildChallengeUpdated final : public ServerPacket
        {
        public:
            GuildChallengeUpdated() : ServerPacket(SMSG_GUILD_CHALLENGE_UPDATE, 4 * 4 * 6) { }

            WorldPacket const* Write() override;

            int32 CurrentCount[6] = { };
            int32 MaxCount[6] = { };
            int32 Gold[6] = { };
            int32 MaxLevelGold[6] = { };
        };

        class GuildChallengeCompleted final : public ServerPacket
        {
        public:
            GuildChallengeCompleted() : ServerPacket(SMSG_GUILD_CHALLENGE_COMPLETED, 4 * 4) { }

            WorldPacket const* Write() override;

            int32 ChallengeType = 0;
            int32 CurrentCount = 0;
            int32 MaxCount = 0;
            int32 GoldAwarded = 0;
        };

        class LFGuildSetGuildPost final : public ClientPacket
        {
        public:
            LFGuildSetGuildPost(WorldPacket&& packet) : ClientPacket(CMSG_LF_GUILD_SET_GUILD_POST, std::move(packet)) { }

            void Read() override;

            uint32 Availability = 0;
            uint32 PlayStyle = 0;
            uint32 ClassRoles = 0;
            uint32 LevelRange = 0;
            std::string Comment;
            bool Active = false;
        };

        class GuildBankDepositMoney final : public ClientPacket
        {
        public:
            GuildBankDepositMoney(WorldPacket&& packet) : ClientPacket(CMSG_GUILD_BANK_DEPOSIT_MONEY, std::move(packet)) { }

            void Read() override;

            ObjectGuid Banker;
            uint64 Money = 0;
        };

        class GuildBankWithdrawMoney final : public ClientPacket
        {
        public:
            GuildBankWithdrawMoney(WorldPacket&& packet) : ClientPacket(CMSG_GUILD_BANK_WITHDRAW_MONEY, std::move(packet)) { }

            void Read() override;

            ObjectGuid Banker;
            uint64 Money = 0;
        };

        class GuildPermissionsQueryResults final : public ServerPacket
        {
        public:
            struct GuildRankTabPermissions
            {
                int32 Flags = 0;
                int32 WithdrawItemLimit = 0;
            };

            GuildPermissionsQueryResults() : ServerPacket(SMSG_GUILD_PERMISSIONS_QUERY_RESULTS, 20) { }

            WorldPacket const* Write() override;

            int32 NumTabs = 0;
            int32 WithdrawGoldLimit = 0;
            int32 Flags = 0;
            uint32 RankID = 0;
            std::vector<GuildRankTabPermissions> Tab;
        };

        class GuildBankRemainingWithdrawMoney final : public ServerPacket
        {
        public:
            GuildBankRemainingWithdrawMoney() : ServerPacket(SMSG_GUILD_BANK_REMAINING_WITHDRAW_MONEY, 8) { }

            WorldPacket const* Write() override;

            int64 RemainingWithdrawMoney = 0;
        };

        class PlayerSaveGuildEmblem final : public ServerPacket
        {
        public:
            PlayerSaveGuildEmblem() : ServerPacket(SMSG_PLAYER_SAVE_GUILD_EMBLEM, 4) { }

            WorldPacket const* Write() override;

            int32 Error = 0;
        };
    }
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Guild::GuildRosterProfessionData const& rosterProfessionData);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Guild::GuildRosterMemberData const& rosterMemberData);
//ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Guild::GuildRankData const& rankData);
//ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Guild::GuildRewardItem const& rewardItem);
//ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Guild::GuildNewsEvent const& newsEvent);

#endif // GuildPackets_h__
