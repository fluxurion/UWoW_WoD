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

#include "LFGPackets.h"

ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::LFG::RideTicket& ticket)
{
    data >> ticket.RequesterGuid;
    data >> ticket.Id;
    data >> ticket.Type;
    data >> ticket.Time;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::LFG::RideTicket const& ticket)
{
    data << ticket.RequesterGuid;
    data << int32(ticket.Id);
    data << int32(ticket.Type);
    data << uint32(ticket.Time);

    return data;
}

void WorldPackets::LFG::BlackList::Initialize(::lfg::LfgLockMap const& lock, ObjectGuid const& guid /*= ObjectGuid::Empty*/)
{
    PlayerGuid = guid;

    for (auto const& map : lock)
    {
        WorldPackets::LFG::BlackList::BlackListInfo info;
        info.Slot = map.first;
        info.Reason = map.second.status;
        info.SubReason1 = map.second.reqItemLevel;
        info.SubReason2 = map.second.currItemLevel;
        blackListData.push_back(info);
    }
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::LFG::BlackList const& blackList)
{
    data.WriteBit(blackList.PlayerGuid.is_initialized());
    data.FlushBits();

    data << static_cast<uint32>(blackList.blackListData.size());

    if (blackList.PlayerGuid)
        data << *blackList.PlayerGuid;

    for (auto const& map : blackList.blackListData)
    {
        data << map.Slot;
        data << map.Reason;
        data << map.SubReason1;
        data << map.SubReason2;
    }

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::LFG::LFGListBlacklist const& blackList)
{
    data << blackList.ActivityID;
    data << blackList.Reason;

    return data;
}

ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::LFG::LFGListBlacklist& blackList)
{
    data >> blackList.ActivityID;
    data >> blackList.Reason;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::LFG::BootInfo const& boot)
{
    data.WriteBit(boot.VoteInProgress);
    data.WriteBit(boot.VotePassed);
    data.WriteBit(boot.MyVoteCompleted);
    data.WriteBit(boot.MyVote);
    data.WriteBits(boot.Reason.length(), 8);
    data << boot.Target;
    data << boot.TotalVotes;
    data << boot.BootVotes;
    data << boot.TimeLeft;
    data << boot.VotesNeeded;
    data << boot.Reason;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::LFG::ListJoinRequest const& join)
{
    data << join.ActivityID;
    data << join.RequiredItemLevel;
    data.WriteBits(join.Name.length(), 8);
    data.WriteBits(join.Comment.length(), 11);
    data.WriteBits(join.VoiceChat.length(), 8);
    data.WriteString(join.Name);
    data.WriteString(join.Comment);
    data.WriteString(join.VoiceChat);

    return data;
}

ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::LFG::ListJoinRequest& join)
{
    data >> join.ActivityID;
    data >> join.RequiredItemLevel;

    uint32 NameLen = data.ReadBits(8);
    uint32 Comment = data.ReadBits(11);
    uint32 VoiceChat = data.ReadBits(8);

    join.Name = data.ReadString(NameLen);
    join.Comment = data.ReadString(Comment);
    join.VoiceChat = data.ReadString(VoiceChat);

    return data;
}

void WorldPackets::LFG::ShortageReward::Initialize(::Quest const* quest /*= nullptr*/, ::Quest const* bonusQuest /*= nullptr*/, ::Player* player /*= nullptr*/)
{
    if (!quest)
        return;

    Mask = 0;
    RewardMoney = quest->GetRewMoney();
    if (player)
        RewardXP = quest->XPValue(player);

    RewardSpellID = quest->GetRewSpell();

    for (auto const& i : {0, 1, 2, 3})
    {
        uint32 itemId = quest->RewardItemId[i];
        if (!itemId)
            continue;

        WorldPackets::LFG::ShortageReward::PlayerQuestRewardItem item;
        item.ItemID = itemId;
        item.Quantity = quest->RewardItemCount[i];
        Item.push_back(item);
    }

    for (auto const& i : {0, 1, 2, 3})
    {
        uint32 currencyID = quest->RewardCurrencyId[i];
        if (!currencyID)
            return;

        WorldPackets::LFG::ShortageReward::PlayerQuestRewardCurrency currency;
        currency.CurrencyID = currencyID;
        currency.Quantity = quest->RewardCurrencyCount[i] * GetCurrencyPrecision(currencyID);
        Currency.push_back(currency);
    }

    if (!bonusQuest)
        return;

    for (auto const& i : {0, 1, 2, 3})
    {
        uint32 currencyID = bonusQuest->RewardCurrencyId[i];
        if (!currencyID)
            return;

        WorldPackets::LFG::ShortageReward::PlayerQuestRewardCurrency bonusCurrency;
        bonusCurrency.CurrencyID = currencyID;
        bonusCurrency.Quantity = bonusQuest->RewardCurrencyCount[i] * GetCurrencyPrecision(currencyID);
        BonusCurrency.push_back(bonusCurrency);
    }
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::LFG::ShortageReward const& reward)
{
    data << reward.Mask;
    data << reward.RewardMoney;
    data << reward.RewardXP;

    data << static_cast<uint32>(reward.Item.size());
    data << static_cast<uint32>(reward.Currency.size());
    data << static_cast<uint32>(reward.BonusCurrency.size());

    for (auto const& itemreward : reward.Item)
    {
        data << itemreward.ItemID;
        data << itemreward.Quantity;
    }

    for (auto const& currencyreward : reward.Currency)
    {
        data << currencyreward.CurrencyID;
        data << currencyreward.Quantity;
    }

    for (auto const& bonusCurrencyreward : reward.BonusCurrency)
    {
        data << bonusCurrencyreward.CurrencyID;
        data << bonusCurrencyreward.Quantity;
    }

    data.WriteBit(reward.RewardSpellID.is_initialized());
    data.FlushBits();
    if (reward.RewardSpellID)
        data << *reward.RewardSpellID;

    return data;
}

WorldPacket const* WorldPackets::LFG::PlayerInfo::Write()
{
    _worldPacket << static_cast<uint32>(Dungeon.size());
    _worldPacket << BlackListMap;

    for (PlayerDungeonInfo const& map : Dungeon)
    {
        _worldPacket << map.Slot;
        _worldPacket << map.CompletionQuantity;
        _worldPacket << map.CompletionLimit;
        _worldPacket << map.CompletionCurrencyID;
        _worldPacket << map.SpecificQuantity;
        _worldPacket << map.SpecificLimit;
        _worldPacket << map.OverallQuantity;
        _worldPacket << map.OverallLimit;
        _worldPacket << map.PurseWeeklyQuantity;
        _worldPacket << map.PurseWeeklyLimit;
        _worldPacket << map.PurseQuantity;
        _worldPacket << map.PurseLimit;
        _worldPacket << map.Quantity;
        _worldPacket << map.CompletedMask;

        _worldPacket << static_cast<uint32>(map.ShortageRewards.size());
        _worldPacket << map.Reward;

        for (auto const& shortageRewardMap : map.ShortageRewards)
            _worldPacket << shortageRewardMap;

        _worldPacket.WriteBit(map.FirstReward);
        _worldPacket.WriteBit(map.ShortageEligible);
        _worldPacket.FlushBits();
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::LFG::JoinResult::Write()
{
    _worldPacket << Ticket;
    _worldPacket << static_cast<uint8>(Result);
    _worldPacket << static_cast<uint8>(ResultDetail);;
    _worldPacket << static_cast<uint32>(blackList.size());
    for (auto const& map : blackList)
    {
        _worldPacket << *map.PlayerGuid;
        _worldPacket << static_cast<uint32>(map.blackListData.size());
        for (auto const& slotsMap : map.blackListData)
        {
            _worldPacket << slotsMap.Slot;
            _worldPacket << slotsMap.Reason;
            _worldPacket << slotsMap.SubReason1;
            _worldPacket << slotsMap.SubReason2;
        }
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::LFG::QueueStatusUpdate::Write()
{
    _worldPacket << Ticket;

    _worldPacket << SubType;
    _worldPacket << Reason;

    for (uint8 const id : {0, 1, 2})
        _worldPacket << Needs[id];

    _worldPacket << static_cast<uint32>(Slots.size());
    _worldPacket << RequestedRoles;
    _worldPacket << static_cast<uint32>(SuspendedPlayers.size());

    for (uint32 const& map : Slots)
        _worldPacket << map;

    for (ObjectGuid const& map2 : SuspendedPlayers)
        _worldPacket << map2;

    _worldPacket.WriteBit(IsParty);
    _worldPacket.WriteBit(NotifyUI);
    _worldPacket.WriteBit(Joined);
    _worldPacket.WriteBit(LfgJoined);
    _worldPacket.WriteBit(Queued);
    _worldPacket.WriteBits(Comment.length(), 8);
    _worldPacket.WriteString(Comment);

    return &_worldPacket;
}

void WorldPackets::LFG::LockInfoRequest::Read()
{
    Player = _worldPacket.ReadBit();
    _worldPacket >> PartyIndex;
}

WorldPacket const* WorldPackets::LFG::QueueStatus::Write()
{
    _worldPacket << Ticket;

    _worldPacket << Slot;
    _worldPacket << AvgWaitTime;
    _worldPacket << QueuedTime;

    for (uint8 i = 0; i < 3; i++)
    {
        _worldPacket << AvgWaitTimeByRole[i];
        _worldPacket << LastNeeded[i];
    }

    _worldPacket << AvgWaitTimeMe;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::LFG::ProposalUpdate::Write()
{
    _worldPacket << Ticket;

    _worldPacket << InstanceID;

    _worldPacket << ProposalID;
    _worldPacket << Slot;

    _worldPacket << State;

    _worldPacket << CompletedMask;
    _worldPacket << static_cast<uint32>(Players.size());
    for (auto const& map : Players)
    {
        _worldPacket << map.Roles;
        _worldPacket.WriteBit(map.Me);
        _worldPacket.WriteBit(map.SameParty);
        _worldPacket.WriteBit(map.MyParty);
        _worldPacket.WriteBit(map.Responded);
        _worldPacket.WriteBit(map.Accepted);
        _worldPacket.FlushBits();
    }

    _worldPacket.WriteBit(ValidCompletedMask);
    _worldPacket.WriteBit(ProposalSilent);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::LFG::PlayerReward::Write()
{
    _worldPacket << ActualSlot;
    _worldPacket << QueuedSlot;
    _worldPacket << RewardMoney;
    _worldPacket << AddedXP;

    _worldPacket << static_cast<uint32>(Players.size());
    for (auto const& map : Players)
    {
        _worldPacket << map.RewardItem;
        _worldPacket << map.RewardItemQuantity;
        _worldPacket << map.BonusCurrency;
        _worldPacket.WriteBit(map.IsCurrency);
        _worldPacket.FlushBits();
    }

    return &_worldPacket;
}

void WorldPackets::LFG::DFJoin::Read()
{
    QueueAsGroup = _worldPacket.ReadBit();
    uint32 commentLength = _worldPacket.ReadBits(8);

    _worldPacket >> PartyIndex;
    _worldPacket >> Roles;
    uint32 numDungeons = _worldPacket.read<uint32>();
    for (auto const& i : {0, 1, 2})
        _worldPacket >> Need[i];

    Comment = _worldPacket.ReadString(commentLength);

    for (uint8 i = 0; i < numDungeons; i++)
        Slot.insert(_worldPacket.read<uint32>() & 0xFFFFF);
}

WorldPacket const* WorldPackets::LFG::RoleCheckUpdate::Write()
{
    _worldPacket << PartyIndex;
    _worldPacket << RoleCheckStatus;
    _worldPacket << static_cast<uint32>(JoinSlots.size());
    _worldPacket << BgQueueID;
    _worldPacket << ActivityID;
    _worldPacket << static_cast<uint32>(Members.size());

    for (auto const& map : JoinSlots)
        _worldPacket << map;

    for (auto const& map : Members)
    {
        _worldPacket << map.Guid;
        _worldPacket << map.RolesDesired;
        _worldPacket << map.Level;
        _worldPacket.WriteBit(map.RoleCheckComplete);
        _worldPacket.FlushBits();
    }

    _worldPacket.WriteBit(IsBeginning);
    _worldPacket.WriteBit(ShowRoleCheck);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::LFG::RoleChosen::Write()
{
    _worldPacket << Player;
    _worldPacket << RoleMask;
    _worldPacket.WriteBit(Accepted);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::LFG::PartyInfo::Write()
{
    _worldPacket << static_cast<uint32>(Player.size());

    for (auto const& map : Player)
        _worldPacket << map;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::LFG::BootPlayer::Write()
{
    _worldPacket << Info;

    return &_worldPacket;
}

void WorldPackets::LFG::DFBootPlayerVote::Read()
{
    Vote = _worldPacket.ReadBit();
}

void WorldPackets::LFG::DFProposalResponse::Read()
{
    _worldPacket >> Data.Ticket;
    _worldPacket >> Data.InstanceID;
    _worldPacket >> Data.ProposalID;
    Data.Accepted = _worldPacket.ReadBit();
}

WorldPacket const* WorldPackets::LFG::ListUpdateBlacklist::Write()
{
    _worldPacket << static_cast<uint32>(Blacklist.size());

    for (auto const& map : Blacklist)
        _worldPacket << map;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::LFG::ListUpdateStatus::Write()
{
    _worldPacket << Ticket;
    _worldPacket << Request;
    _worldPacket << uint32(0); // Unk
    _worldPacket << Reason;
    _worldPacket.WriteBit(Listed);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::LFG::TeleportDenied::Write()
{
    _worldPacket.WriteBits(Reason, 4);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

void WorldPackets::LFG::ListInviteResponse::Read()
{
    _worldPacket >> Ticket;
    Accept = _worldPacket.ReadBit();
}

void WorldPackets::LFG::DFTeleport::Read()
{
    TeleportOut = _worldPacket.ReadBit();
}

void WorldPackets::LFG::DFSetRoles::Read()
{
    _worldPacket >> RolesDesired;
    _worldPacket >> PartyIndex;
}

void WorldPackets::LFG::DFLeave::Read()
{
    _worldPacket >> Ticket;
}

void WorldPackets::LFG::ListJoin::Read()
{
    _worldPacket >> Request;
}

void WorldPackets::LFG::ListLeave::Read()
{
    _worldPacket >> Ticket;
}

void WorldPackets::LFG::BonusFactionID::Read()
{
    _worldPacket >> FactionID;
}

void WorldPackets::LFG::DFSetComment::Read()
{
    _worldPacket >> Ticket;
    Comment = _worldPacket.ReadString(_worldPacket.ReadBits(8));
}

void WorldPackets::LFG::DFSearchJoin::Read()
{
    _worldPacket >> Slot;
}

WorldPacket const* WorldPackets::LFG::LFGListJoinResult::Write()
{
    _worldPacket << Ticket;
    _worldPacket << Result;
    _worldPacket << ResultDetail;

    return &_worldPacket;
}

void WorldPackets::LFG::DFSearchLeave::Read()
{
    _worldPacket >> Slot;
}
