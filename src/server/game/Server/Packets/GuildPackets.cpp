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

#include "GuildPackets.h"

void WorldPackets::Guild::QueryGuildInfo::Read()
{
    _worldPacket >> GuildGuid;
    _worldPacket >> PlayerGuid;
}

WorldPacket const* WorldPackets::Guild::QueryGuildInfoResponse::Write()
{
    _worldPacket << GuildGuid;
    _worldPacket.WriteBit(Info.is_initialized());
    _worldPacket.FlushBits();

    if (Info)
    {
        _worldPacket << Info->GuildGUID;
        _worldPacket << uint32(Info->VirtualRealmAddress);
        _worldPacket << uint32(Info->Ranks.size());
        _worldPacket << uint32(Info->EmblemColor);
        _worldPacket << uint32(Info->EmblemStyle);
        _worldPacket << uint32(Info->BorderColor);
        _worldPacket << uint32(Info->BorderStyle);
        _worldPacket << uint32(Info->BackgroundColor);

        for (GuildInfo::GuildInfoRank const& rank : Info->Ranks)
        {
            _worldPacket << uint32(rank.RankID);
            _worldPacket << uint32(rank.RankOrder);

            _worldPacket.WriteBits(rank.RankName.size(), 7);
            _worldPacket.WriteString(rank.RankName);
        }

        _worldPacket.WriteBits(Info->GuildName.size(), 7);
        _worldPacket.WriteString(Info->GuildName);
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Guild::GuildRoster::Write()
{
    _worldPacket << NumAccounts;
    _worldPacket.AppendPackedTime(CreateDate);
    _worldPacket << GuildFlags;
    _worldPacket << uint32(MemberData.size());

    for (GuildRosterMemberData const& member : MemberData)
        _worldPacket << member;

    _worldPacket.WriteBits(WelcomeText.length(), 10);
    _worldPacket.WriteBits(InfoText.length(), 10);
    _worldPacket.FlushBits();

    _worldPacket.WriteString(WelcomeText);
    _worldPacket.WriteString(InfoText);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Guild::GuildRosterUpdate::Write()
{
    _worldPacket << uint32(MemberData.size());

    for (GuildRosterMemberData const& member : MemberData)
        _worldPacket << member;

    return &_worldPacket;
}

void WorldPackets::Guild::GuildUpdateMotdText::Read()
{
    uint32 textLen = _worldPacket.ReadBits(10);
    MotdText = _worldPacket.ReadString(textLen);
}

WorldPacket const* WorldPackets::Guild::GuildCommandResult::Write()
{
    _worldPacket << Result;
    _worldPacket << Command;

    _worldPacket.WriteBits(Name.length(), 8);
    _worldPacket.FlushBits();

    _worldPacket.WriteString(Name);

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Guild::GuildRosterProfessionData const& rosterProfessionData)
{
    data << rosterProfessionData.DbID;
    data << rosterProfessionData.Rank;
    data << rosterProfessionData.Step;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Guild::GuildRosterMemberData const& rosterMemberData)
{
    data << rosterMemberData.Guid;
    data << rosterMemberData.RankID;
    data << rosterMemberData.AreaID;
    data << rosterMemberData.PersonalAchievementPoints;
    data << rosterMemberData.GuildReputation;
    data << rosterMemberData.LastSave;

    for (uint8 i = 0; i < 2; i++)
        data << rosterMemberData.ProfessionData[i];

    data << rosterMemberData.VirtualRealmAddress;
    data << rosterMemberData.Status;
    data << rosterMemberData.Level;
    data << rosterMemberData.ClassID;
    data << rosterMemberData.Gender;

    data.WriteBits(rosterMemberData.Name.length(), 6);
    data.WriteBits(rosterMemberData.Note.length(), 8);
    data.WriteBits(rosterMemberData.OfficerNote.length(), 8);
    data.WriteBit(rosterMemberData.Authenticated);
    data.WriteBit(rosterMemberData.SorEligible);
    data.FlushBits();

    data.WriteString(rosterMemberData.Name);
    data.WriteString(rosterMemberData.Note);
    data.WriteString(rosterMemberData.OfficerNote);

    return data;
}

void WorldPackets::Guild::GuildBankQueryTab::Read()
{
    _worldPacket >> Banker;
    _worldPacket >> TabId;

    FullUpdate = _worldPacket.ReadBit();
}


WorldPacket const* WorldPackets::Guild::GuildBankQueryResults::Write()
{
    _worldPacket << Money;
    _worldPacket << Tab;
    _worldPacket << WithdrawalsRemaining;
    _worldPacket << uint32(TabInfo.size());
    _worldPacket << uint32(ItemInfo.size());

    for (GuildBankTabInfo const& tab : TabInfo)
    {
        _worldPacket << tab.TabIndex;
        _worldPacket.WriteBits(tab.Name.length(), 7);
        _worldPacket.WriteBits(tab.Icon.length(), 9);
        _worldPacket.FlushBits();

        _worldPacket.WriteString(tab.Name);
        _worldPacket.WriteString(tab.Icon);
    }

    for (GuildBankItemInfo const& item : ItemInfo)
    {
        _worldPacket << item.Slot;
        _worldPacket << item.Item;
        _worldPacket << item.Count;
        _worldPacket << item.EnchantmentID;
        _worldPacket << item.Charges;
        _worldPacket << item.OnUseEnchantmentID;
        _worldPacket << uint32(/*item.SocketEnchant.size()*/0);
        _worldPacket << item.Flags;

        /*for (GuildBankItemInfo::GuildBankSocketEnchant const& socketEnchant : item.SocketEnchant)
        {
            _worldPacket << socketEnchant.SocketIndex;
            _worldPacket << socketEnchant.SocketEnchantID;
        }*/

        _worldPacket.WriteBit(item.Locked);
        _worldPacket.FlushBits();
    }

    _worldPacket.WriteBit(FullUpdate);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Guild::LFGuildRecruitData const& recruit)
{
    data << recruit.RecruitGUID;
    data << recruit.RecruitVirtualRealm;
    data << recruit.CharacterClass;
    data << recruit.CharacterGender;
    data << recruit.CharacterLevel;
    data << recruit.ClassRoles;
    data << recruit.PlayStyle;
    data << recruit.Availability;
    data << recruit.SecondsSinceCreated;
    data << recruit.SecondsUntilExpiration;
    data.WriteBits(recruit.Name.length(), 6);
    data.WriteBits(recruit.Comment.length(), 10);
    data.WriteString(recruit.Name);
    data.WriteString(recruit.Comment);

    return data;
}

WorldPacket const* WorldPackets::Guild::LFGuildRecruits::Write()
{
    _worldPacket << static_cast<uint32>(Recruits.size());
    _worldPacket << int32(UpdateTime);
    for (auto const& v : Recruits)
        _worldPacket << v;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Guild::LFGuildPost::Write()
{
    _worldPacket.WriteBit(Post.is_initialized());
    _worldPacket.FlushBits();
    if (Post)
    {
        _worldPacket.WriteBit(Post->Active);
        _worldPacket.WriteBits(Post->Comment.length(), 10);
        _worldPacket.FlushBits();

        _worldPacket << Post->PlayStyle;
        _worldPacket << Post->Availability;
        _worldPacket << Post->ClassRoles;
        _worldPacket << Post->LevelRange;
        _worldPacket << Post->SecondsRemaining;
        _worldPacket.WriteString(Post->Comment);
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Guild::LFGuildBrowse::Write()
{
    _worldPacket << static_cast<uint32>(Browses.size());
    for (auto const& v : Browses)
    {
        _worldPacket.WriteBits(v.GuildName.length(), 7);
        _worldPacket.WriteBits(v.Comment.length(), 10);
        _worldPacket.FlushBits();
        _worldPacket << v.GuildGUID;
        _worldPacket << v.GuildVirtualRealm;
        _worldPacket << v.GuildMembers;
        _worldPacket << v.GuildAchievementPoints;
        _worldPacket << v.PlayStyle;
        _worldPacket << v.Availability;
        _worldPacket << v.ClassRoles;
        _worldPacket << v.LevelRange;
        _worldPacket << v.EmblemStyle;
        _worldPacket << v.EmblemColor;
        _worldPacket << v.BorderStyle;
        _worldPacket << v.BorderColor;
        _worldPacket << v.Background;
        _worldPacket << v.Cached;
        _worldPacket << v.MembershipRequested;
        _worldPacket.WriteString(v.GuildName);
        _worldPacket.WriteString(v.Comment);
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Guild::LFGuildApplication::Write()
{
    _worldPacket << NumRemaining;
    _worldPacket << static_cast<uint32>(Applications.size());
    for (auto const& v : Applications)
    {
        _worldPacket << v.GuildGUID;
        _worldPacket << v.GuildVirtualRealm;
        _worldPacket << v.ClassRoles;
        _worldPacket << v.PlayStyle;
        _worldPacket << v.Availability;
        _worldPacket << v.SecondsSinceCreated;
        _worldPacket.WriteBits(v.GuildName.length(), 7);
        _worldPacket.WriteBits(v.Comment.length(), 10);
        _worldPacket.WriteString(v.GuildName);
        _worldPacket.WriteString(v.Comment);
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Guild::GuildReputationReactionChanged::Write()
{
    _worldPacket << MemberGUID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Guild::GuildNameChanged::Write()
{
    _worldPacket << GuildGUID;
    _worldPacket.WriteBits(GuildName.length(), 7);
    _worldPacket.WriteString(GuildName);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Guild::GuildChallengeUpdated::Write()
{
    for (int8 i = 0; i < 6; i++)
        _worldPacket << CurrentCount[i];

    for (int8 i = 0; i < 6; i++)
        _worldPacket << MaxCount[i];

    for (int8 i = 0; i < 6; i++)
        _worldPacket << Gold[i];

    for (int8 i = 0; i < 6; i++)
        _worldPacket << MaxLevelGold[i];

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Guild::GuildChallengeCompleted::Write()
{
    _worldPacket << ChallengeType;
    _worldPacket << CurrentCount;
    _worldPacket << MaxCount;
    _worldPacket << GoldAwarded;

    return &_worldPacket;
}

void WorldPackets::Guild::LFGuildSetGuildPost::Read()
{
    _worldPacket >> PlayStyle;
    _worldPacket >> Availability;
    _worldPacket >> ClassRoles;
    _worldPacket >> LevelRange;
    Active = _worldPacket.ReadBit();
    Comment = _worldPacket.ReadString(_worldPacket.ReadBits(10));
}

void WorldPackets::Guild::GuildBankDepositMoney::Read()
{
    _worldPacket >> Banker;
    _worldPacket >> Money;
}

void WorldPackets::Guild::GuildBankWithdrawMoney::Read()
{
    _worldPacket >> Banker;
    _worldPacket >> Money;
}

WorldPacket const* WorldPackets::Guild::GuildPermissionsQueryResults::Write()
{
    _worldPacket << RankID;
    _worldPacket << WithdrawGoldLimit;
    _worldPacket << Flags;
    _worldPacket << NumTabs;
    _worldPacket << uint32(Tab.size());

    for (auto const& v : Tab)
    {
        _worldPacket << v.Flags;
        _worldPacket << v.WithdrawItemLimit;
    }

    return &_worldPacket;
}

void WorldPackets::Guild::GuildBankLogQuery::Read()
{
    _worldPacket >> TabId;
}

WorldPacket const* WorldPackets::Guild::GuildBankLogQueryResults::Write()
{
    _worldPacket << TabId;
    _worldPacket << uint32(Entry.size());

    for (GuildBankLogEntry const& logEntry : Entry)
    {
        _worldPacket << logEntry.PlayerGUID;
        _worldPacket << logEntry.TimeOffset;
        _worldPacket << logEntry.EntryType;

        _worldPacket.WriteBit(logEntry.Money.is_initialized());
        _worldPacket.WriteBit(logEntry.ItemID.is_initialized());
        _worldPacket.WriteBit(logEntry.Count.is_initialized());
        _worldPacket.WriteBit(logEntry.OtherTab.is_initialized());
        _worldPacket.FlushBits();

        if (logEntry.Money.is_initialized())
            _worldPacket << *logEntry.Money;

        if (logEntry.ItemID.is_initialized())
            _worldPacket << *logEntry.ItemID;

        if (logEntry.Count.is_initialized())
            _worldPacket << *logEntry.Count;

        if (logEntry.OtherTab.is_initialized())
            _worldPacket << *logEntry.OtherTab;
    }

    _worldPacket.WriteBit(WeeklyBonusMoney.is_initialized());
    _worldPacket.FlushBits();

    if (WeeklyBonusMoney)
        _worldPacket << *WeeklyBonusMoney;

    return &_worldPacket;
}

void WorldPackets::Guild::GuildBankTextQuery::Read()
{
    _worldPacket >> TabId;
}

WorldPacket const* WorldPackets::Guild::GuildBankTextQueryResult::Write()
{
    _worldPacket << TabId;

    _worldPacket.WriteBits(Text.length(), 14);
    _worldPacket.FlushBits();

    _worldPacket.WriteString(Text);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Guild::GuildBankRemainingWithdrawMoney::Write()
{
    _worldPacket << RemainingWithdrawMoney;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Guild::GuildEventLogQueryResults::Write()
{
    _worldPacket.reserve(4 + Entry.size() * 38);

    _worldPacket << uint32(Entry.size());

    for (GuildEventEntry const& entry : Entry)
    {
        _worldPacket << entry.PlayerGUID;
        _worldPacket << entry.OtherGUID;
        _worldPacket << entry.TransactionType;
        _worldPacket << entry.RankID;
        _worldPacket << entry.TransactionDate;
    }

    return &_worldPacket;
}

// GuildNews

void WorldPackets::Guild::GuildQueryNews::Read()
{
    _worldPacket >> GuildGUID;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Guild::GuildNewsEvent const& newsEvent)
{
    data << newsEvent.Id;
    data.AppendPackedTime(newsEvent.CompletedDate);
    data << newsEvent.Type;
    data << newsEvent.Flags;

    for (uint8 i = 0; i < 2; i++)
        data << newsEvent.Data[i];

    data << newsEvent.MemberGuid;
    data << uint32(newsEvent.MemberList.size());

    for (ObjectGuid memberGuid : newsEvent.MemberList)
        data << memberGuid;

    data.WriteBit(newsEvent.Item.is_initialized());
    data.FlushBits();

    if (newsEvent.Item)
        data << *newsEvent.Item;  // WorldPackets::Item::ItemInstance

    return data;
}

WorldPacket const* WorldPackets::Guild::GuildNews::Write()
{
    _worldPacket << NewsEvents;

    return &_worldPacket;
}

void WorldPackets::Guild::GuildNewsUpdateSticky::Read()
{
    _worldPacket >> GuildGUID;
    _worldPacket >> NewsID;

    NewsID = _worldPacket.ReadBit();
}

WorldPacket const* WorldPackets::Guild::PlayerSaveGuildEmblem::Write()
{
    _worldPacket << int32(Error);

    return &_worldPacket;
}
