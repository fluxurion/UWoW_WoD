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

WorldPackets::Guild::QueryGuildInfoResponse::QueryGuildInfoResponse()
    : ServerPacket(SMSG_QUERY_GUILD_INFO_RESPONSE)
{ }

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
        _worldPacket << uint32(Info->EmblemStyle);
        _worldPacket << uint32(Info->EmblemColor);
        _worldPacket << uint32(Info->BorderStyle);
        _worldPacket << uint32(Info->BorderColor);
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
        _worldPacket << uint32(item.SocketEnchant.size());
        _worldPacket << item.Flags;

        for (GuildBankItemInfo::GuildBankSocketEnchant const& socketEnchant : item.SocketEnchant)
        {
            _worldPacket << socketEnchant.SocketIndex;
            _worldPacket << socketEnchant.SocketEnchantID;
        }

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
