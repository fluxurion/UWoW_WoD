/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "WorldSession.h"
#include "WorldPacket.h"
#include "DBCStores.h"
#include "Player.h"
#include "Group.h"
#include "LFGMgr.h"
#include "ObjectMgr.h"
#include "GroupMgr.h"
#include "LFGQueue.h"
#include "LFGPackets.h"

void WorldSession::HandleDFJoin(WorldPackets::LFG::DFJoin& packet)
{
    if (packet.Slot.empty())
        return;

    if (!sLFGMgr->isOptionEnabled(lfg::LFG_OPTION_ENABLE_DUNGEON_FINDER | lfg::LFG_OPTION_ENABLE_RAID_BROWSER))
        return;

    sLFGMgr->JoinLfg(GetPlayer(), packet.Roles, packet.Slot, packet.Comment);
}

void WorldSession::HandleDFLeave(WorldPackets::LFG::DFLeave& packet)
{
    Group* group = GetPlayer()->GetGroup();
    ObjectGuid gguid = group ? group->GetGUID() : packet.Ticket.RequesterGuid;

    if (!group || group->GetLeaderGUID() == packet.Ticket.RequesterGuid)
        sLFGMgr->LeaveLfg(gguid);
}

void WorldSession::HandleDFProposalResponse(WorldPackets::LFG::DFProposalResponse& packet)
{
    sLFGMgr->UpdateProposal(packet.Data);
}

void WorldSession::HandleDFSetRoles(WorldPackets::LFG::DFSetRoles& packet)
{
    ObjectGuid guid = GetPlayer()->GetGUID();
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    sLFGMgr->UpdateRoleCheck(group->GetGUID(), guid, packet.RolesDesired);
}

void WorldSession::HandleDFSetComment(WorldPackets::LFG::DFSetComment& packet)
{
    sLFGMgr->SetComment(packet.Ticket.RequesterGuid, packet.Comment);
}

void WorldSession::HandleDFBootPlayerVote(WorldPackets::LFG::DFBootPlayerVote& packet)
{
    sLFGMgr->UpdateBoot(GetPlayer()->GetGUID(), packet.Vote);
}

void WorldSession::HandleDFTeleport(WorldPackets::LFG::DFTeleport& packet)
{
    sLFGMgr->TeleportPlayer(GetPlayer(), packet.TeleportOut, true);
}

void WorldSession::HandleLockInfoRequest(WorldPackets::LFG::LockInfoRequest& /*packet*/)
{
    Player* player = GetPlayer();
    uint8 level = player->getLevel();
    lfg::LfgDungeonSet const& rewardableDungeons = sLFGMgr->GetRewardableDungeons(level, player->GetSession()->Expansion());
    lfg::LfgLockMap const& lock = sLFGMgr->GetLockedDungeons(player->GetGUID());

    WorldPackets::LFG::PlayerInfo info;
    info.BlackListMap.Initialize(lock, player->GetGUID());
    for (auto const& dungeon : rewardableDungeons)
    {
        lfg::LfgReward const* reward = sLFGMgr->GetDungeonReward(dungeon, level);
        if (!reward)
            continue;
    
        Quest const* rewardQuest = sObjectMgr->GetQuestTemplate(reward->firstQuest);
        Quest const* bonusQuest = sObjectMgr->GetQuestTemplate(reward->bonusQuestId);

        bool firstCompletion = rewardQuest ? player->CanRewardQuest(rewardQuest, false) : true;
        if (!firstCompletion)
            rewardQuest = sObjectMgr->GetQuestTemplate(reward->otherQuest);

        WorldPackets::LFG::PlayerDungeonInfo dungeonInfo;
        dungeonInfo.Slot = dungeon;
        dungeonInfo.FirstReward = firstCompletion;
        dungeonInfo.ShortageEligible = true;
        dungeonInfo.CompletionQuantity = 0;
        dungeonInfo.CompletionLimit = 0;
        dungeonInfo.CompletionCurrencyID = 0;
        dungeonInfo.SpecificQuantity = 0;
        dungeonInfo.SpecificLimit = 0;
        dungeonInfo.OverallQuantity = 0;
        dungeonInfo.OverallLimit = 0;
        dungeonInfo.PurseWeeklyQuantity = 0;
        dungeonInfo.PurseWeeklyLimit = 0;
        dungeonInfo.PurseQuantity = 0;
        dungeonInfo.PurseLimit = 0;
        dungeonInfo.Quantity = 0;
        dungeonInfo.CompletedMask = 0;
        dungeonInfo.Reward.Initialize(rewardQuest, bonusQuest, player);
        //std::vector<ShortageReward> ShortageRewards;
        info.Dungeon.push_back(dungeonInfo);
    }

    SendPacket(info.Write());
}

void WorldSession::HandleLfrJoinOpcode(WorldPacket& recv_data)
{
    uint32 entry;                                          // Raid id to search
    recv_data >> entry;

    //SendLfrUpdateListOpcode(entry);
}

void WorldSession::HandleLfrLeaveOpcode(WorldPacket& recvData)
{
    uint32 dungeonId;                                      // Raid id queue to leave
    recvData >> dungeonId;

    //sLFGMgr->LeaveLfr(GetPlayer(), dungeonId);
}

void WorldSession::HandleDfGetJoinStatus(WorldPackets::LFG::NullCmsg& /*packet*/)
{
    lfg::LfgUpdateData updateData = sLFGMgr->GetLfgStatus(GetPlayer()->GetGUID());

    if (GetPlayer()->GetGroup())
    {
        SendLfgUpdateParty(updateData);
        updateData.dungeons.clear();
        SendLfgUpdatePlayer(updateData);
    }
    else
    {
        SendLfgUpdatePlayer(updateData);
        updateData.dungeons.clear();
        SendLfgUpdateParty(updateData);
    }
}

void WorldSession::SendLfgUpdatePlayer(lfg::LfgUpdateData const& updateData)
{
    sLFGMgr->SendUpdateStatus(GetPlayer(), updateData, false);
}

void WorldSession::SendLfgUpdateParty(lfg::LfgUpdateData const& updateData)
{
    sLFGMgr->SendUpdateStatus(GetPlayer(), updateData, true);
}

void WorldSession::SendLfgRoleChosen(ObjectGuid guid, uint8 roles)
{
    WorldPackets::LFG::RoleChosen chosen;
    chosen.Player = guid;
    chosen.RoleMask = roles;
    chosen.Accepted = roles != 0;
    SendPacket(chosen.Write());
}

void WorldSession::SendLfgRoleCheckUpdate(lfg::LfgRoleCheck const& roleCheck)
{
    bool updateAll = roleCheck.state == lfg::LFG_ROLECHECK_FINISHED || roleCheck.state == lfg::LFG_ROLECHECK_NO_ROLE;

    lfg::LfgDungeonSet dungeons;
    if (roleCheck.rDungeonId)
        dungeons.insert(roleCheck.rDungeonId);
    else
        dungeons = roleCheck.dungeons;

    ObjectGuid guid = _player->GetGUID();

    WorldPackets::LFG::RoleCheckUpdate update;
    update.PartyIndex = 0;
    update.RoleCheckStatus = roleCheck.state;
    update.BgQueueID = guid.GetGUIDLow();
    update.ActivityID = 0;
    update.IsBeginning = roleCheck.state == lfg::LFG_ROLECHECK_INITIALITING;
    update.ShowRoleCheck = roleCheck.state == lfg::LFG_ROLECHECK_FINISHED || roleCheck.state == lfg::LFG_ROLECHECK_ABORTED;

    for (auto const& i : dungeons)
        update.JoinSlots.push_back(sLFGMgr->GetLFGDungeonEntry(i));

    if (updateAll && !roleCheck.roles.empty())
    {
        ObjectGuid memberGuid = guid;
        uint8 roles = roleCheck.roles.find(guid)->second;
        Player* player = ObjectAccessor::FindPlayer(guid);

        WorldPackets::LFG::RoleCheckUpdate::CheckUpdateMember updateMember;
        updateMember.Guid = guid;
        updateMember.RolesDesired = roles;
        updateMember.Level = player ? player->getLevel() : 0;
        updateMember.RoleCheckComplete = roles != 0;
        update.Members.push_back(updateMember);

        for (auto const& i : roleCheck.roles)
        {
            if (i.first == GetPlayer()->GetGUID() || !updateAll)
                continue;

            player = ObjectAccessor::FindPlayer(guid);

            updateMember.Guid = i.first;
            updateMember.RolesDesired = i.second;
            updateMember.Level = player ? player->getLevel() : 0;
            updateMember.RoleCheckComplete = i.second != 0;
            update.Members.push_back(updateMember);
        }
    }

    SendPacket(update.Write());
}

void WorldSession::SendLfgJoinResult(lfg::LfgJoinResultData const& joinData)
{
    WorldPackets::LFG::JoinResult result;
    result.Result = joinData.result;
    result.ResultDetail = joinData.state;
    for (auto const& map : joinData.lockmap)
    {
        WorldPackets::LFG::BlackList list;
        list.PlayerGuid = map.first;
        list.Initialize(map.second);
        result.BlackList.push_back(list);
    }

    result.Ticket.RequesterGuid = GetPlayer()->GetGUID();
    result.Ticket.Id = GetPlayer()->GetTeam();
    result.Ticket.Time = time(nullptr);
    result.Ticket.Type = 3;
  
    SendPacket(result.Write());
}

void WorldSession::SendLfgQueueStatus(lfg::LfgQueueStatusData const& queueData)
{
    WorldPackets::LFG::QueueStatus status;
    status.Ticket.RequesterGuid = GetPlayer()->GetGUID();
    status.Ticket.Id = GetPlayer()->GetTeam();
    status.Ticket.Time = time(NULL) - queueData.queuedTime;
    status.Ticket.Type = 3;
    status.AvgWaitTimeMe = queueData.waitTimeAvg;
    status.AvgWaitTime = queueData.waitTime;
    status.QueuedTime = queueData.queuedTime;
    status.Slot = sLFGMgr->GetLFGDungeonEntry(queueData.dungeonId);
    status.AvgWaitTimeByRole[0] = queueData.waitTimeTank;
    status.AvgWaitTimeByRole[1] = queueData.waitTimeHealer;
    status.AvgWaitTimeByRole[2] = queueData.waitTimeDps;
    status.LastNeeded[0] = queueData.queueInfo->tanks;
    status.LastNeeded[1] = queueData.queueInfo->healers;
    status.LastNeeded[2] = queueData.queueInfo->dps;
    SendPacket(status.Write());
}

void WorldSession::SendLfgPlayerReward(lfg::LfgPlayerRewardData const& rewardData)
{
    if (!rewardData.rdungeonEntry || !rewardData.sdungeonEntry)
        return;

    Quest const* quest = nullptr;
    if (rewardData.bonus)
        quest = sObjectMgr->GetQuestTemplate(rewardData.reward->bonusQuestId);
    else if (rewardData.done)
        quest = sObjectMgr->GetQuestTemplate(rewardData.reward->otherQuest);
    else
        quest = sObjectMgr->GetQuestTemplate(rewardData.reward->firstQuest);
    if (!quest)
        return;

    WorldPackets::LFG::PlayerReward playerData;
    playerData.ActualSlot = rewardData.rdungeonEntry;
    playerData.QueuedSlot = rewardData.sdungeonEntry;
    playerData.RewardMoney = quest->GetRewMoney();
    playerData.AddedXP = quest->XPValue(GetPlayer());

    for (auto const& i : {0, 1, 2, 3})
    {
        WorldPackets::LFG::PlayerReward::PlayerRewards reward;

        uint32 itemId = quest->RewardItemId[i];
        if (itemId)
        {
            reward.IsCurrency = false;
            reward.RewardItem = itemId;
            reward.RewardItemQuantity = quest->RewardItemCount[i];
        }
        
        uint32 currency = quest->RewardCurrencyId[i];
        if (currency)
        {
            reward.IsCurrency = true;
            reward.RewardItemQuantity = quest->RewardCurrencyCount[i] * GetCurrencyPrecision(currency);
            reward.BonusCurrency = currency;
        }

        playerData.Players.push_back(reward);
    }
 
    SendPacket(playerData.Write());
}

void WorldSession::SendLfgBootProposalUpdate(lfg::LfgPlayerBoot const& boot)
{
    ObjectGuid guid = GetPlayer()->GetGUID();
    lfg::LfgAnswer playerVote = boot.votes.find(guid)->second;
    uint8 votesNum = 0;
    uint8 agreeNum = 0;
    uint32 secsleft = uint8((boot.cancelTime - time(nullptr)) / 1000);
    for (lfg::LfgAnswerContainer::const_iterator it = boot.votes.begin(); it != boot.votes.end(); ++it)
    {
        if (it->second != lfg::LFG_ANSWER_PENDING)
        {
            ++votesNum;
            if (it->second == lfg::LFG_ANSWER_AGREE)
                ++agreeNum;
        }
    }

    WorldPackets::LFG::BootPlayer bootPlayer;
    bootPlayer.Info.VoteInProgress = boot.inProgress;
    bootPlayer.Info.VotePassed = playerVote == lfg::LFG_ANSWER_DENY;
    bootPlayer.Info.MyVoteCompleted = playerVote != lfg::LFG_ANSWER_PENDING;
    bootPlayer.Info.MyVote = playerVote == lfg::LFG_ANSWER_AGREE;
    bootPlayer.Info.Target = boot.victim;
    bootPlayer.Info.TotalVotes = votesNum;
    bootPlayer.Info.BootVotes = agreeNum;
    bootPlayer.Info.TimeLeft = secsleft;
    bootPlayer.Info.VotesNeeded = boot.votesNeeded;
    bootPlayer.Info.Reason = boot.reason;
    SendPacket(bootPlayer.Write());
}

void WorldSession::SendLfgUpdateProposal(lfg::LfgProposal const& proposal)
{
    ObjectGuid guid = GetPlayer()->GetGUID();
    ObjectGuid gguid = proposal.players.find(guid)->second.group;
    bool silent = !proposal.isNew && gguid == proposal.group;
    uint32 dungeonEntry = proposal.dungeonId;
    if (!silent)
    {
        lfg::LfgDungeonSet const& playerDungeons = sLFGMgr->GetSelectedDungeons(guid);
        if (!playerDungeons.empty() && playerDungeons.find(proposal.dungeonId) == playerDungeons.end())
            dungeonEntry = (*playerDungeons.begin());
    }

    dungeonEntry = sLFGMgr->GetLFGDungeonEntry(dungeonEntry);

    WorldPackets::LFG::ProposalUpdate update;
    update.Ticket.RequesterGuid = guid;
    update.Ticket.Id = GetPlayer()->GetTeam();
    update.Ticket.Time = time(nullptr);
    update.Ticket.Type = 3;
    update.InstanceID = ObjectGuid::Create<HighGuid::RaidGroup>(dungeonEntry).GetGUIDLow();
    update.ProposalID = proposal.id;
    update.Slot = dungeonEntry;
    update.State = proposal.state;
    update.CompletedMask = proposal.encounters;
    update.ValidCompletedMask = proposal.isNew;
    update.ProposalSilent = silent;

    for (auto const& i : proposal.players)
    {
        WorldPackets::LFG::ProposalUpdate::ProposalUpdatePlayer playerData;
        playerData.Roles = i.second.role;
        playerData.Me = i.first == guid;
        playerData.SameParty = i.second.group ? i.second.group == gguid : false;
        playerData.MyParty = i.second.group ? i.second.group == proposal.group : false;
        playerData.Responded = i.second.accept != lfg::LFG_ANSWER_PENDING;
        playerData.Accepted = i.second.accept == lfg::LFG_ANSWER_AGREE;
        update.Players.push_back(playerData);
    }

    SendPacket(update.Write());
}

void WorldSession::SendLfgLfrList(bool update)
{
    //sLog->outDebug(LOG_FILTER_LFG, "SMSG_LFG_LFR_LIST %s update: %u",
    //    GetPlayerName().c_str(), update ? 1 : 0);
    //WorldPacket data(SMSG_LFG_UPDATE_SEARCH, 1);
    //data << uint8(update);                                 // In Lfg Queue?
    //SendPacket(&data);
}

void WorldSession::SendLfgDisabled()
{
    SendPacket(WorldPackets::LFG::NullSmsg(SMSG_LFG_DISABLED).Write());
}

void WorldSession::SendLfgOfferContinue(uint32 dungeonEntry)
{
    sLog->outDebug(LOG_FILTER_LFG, "SMSG_LFG_OFFER_CONTINUE %s dungeon entry: %u",
        GetPlayerName().c_str(), dungeonEntry);
    WorldPacket data(SMSG_LFG_OFFER_CONTINUE, 4);
    data << uint32(dungeonEntry);
    SendPacket(&data);
}

void WorldSession::SendLfgTeleportError(uint8 err)
{
    SendPacket(WorldPackets::LFG::TeleportDenied(err).Write());
}

void WorldSession::HandleBonusFactionID(WorldPackets::LFG::BonusFactionID& packet)
{
    GetPlayer()->SetLfgBonusFaction(packet.FactionID);
}