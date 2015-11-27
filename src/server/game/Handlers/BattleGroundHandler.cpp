/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "BattlegroundPackets.h"
#include "Bracket.h"
#include "Chat.h"
#include "Common.h"
#include "DisableMgr.h"
#include "Group.h"
#include "Language.h"
#include "Log.h"
#include "NPCPackets.h"
#include "Object.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void WorldSession::HandleBattlemasterHello(WorldPackets::NPC::Hello& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Creature* unit = player->GetMap()->GetCreature(packet.Unit);
    if (!unit)
        return;

    if (!unit->isBattleMaster())                             // it's not battlemaster
        return;

    unit->StopMoving();

    BattlegroundTypeId bgTypeId = sBattlegroundMgr->GetBattleMasterBG(unit->GetEntry());

    if (!player->GetBGAccessByLevel(bgTypeId))
    {
        // temp, must be gossip message...
        SendNotification(LANG_YOUR_BG_LEVEL_REQ_ERROR);
        return;
    }

    sBattlegroundMgr->SendBattlegroundList(player, packet.Unit, bgTypeId);
}

void WorldSession::HandleBattlemasterJoin(WorldPackets::Battleground::Join& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (!sBattlemasterListStore.LookupEntry(packet.QueueID))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: invalid bgtype (%u) received. possible cheater? player guid %u", packet.QueueID, _player->GetGUIDLow());
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, packet.QueueID, nullptr))
    {
        ChatHandler(this).PSendSysMessage(LANG_BG_DISABLED);
        return;
    }

    BattlegroundTypeId bgTypeId = BattlegroundTypeId(packet.QueueID);

    // can do this, since it's battleground, not arena
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, 0);
    BattlegroundQueueTypeId bgQueueTypeIdRandom = BattlegroundMgr::BGQueueTypeId(BATTLEGROUND_RB, 0);

    // ignore if player is already in BG
    if (player->InBattleground())
        return;

    // get bg instance or bg template if instance not found
    Battleground* bg = nullptr;
    //if (instanceId)
    //    bg = sBattlegroundMgr->GetBattlegroundThroughClientInstance(instanceId, bgTypeId);

    if (!bg)
        bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
    if (!bg)
        return;

    // expected bracket entry
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
    if (!bracketEntry)
        return;

    GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE;

    // check queue conditions
    if (!packet.JoinAsGroup)
    {
        if (player->isUsingLfg())
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, ERR_LFG_CANT_USE_BATTLEGROUND);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        if (!player->CanJoinToBattleground())
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        if (player->GetBattlegroundQueueIndex(bgQueueTypeIdRandom) < PLAYER_MAX_BATTLEGROUND_QUEUES)
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, ERR_IN_RANDOM_BG);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        if (player->InBattlegroundQueue() && bgTypeId == BATTLEGROUND_RB)
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, ERR_IN_NON_RANDOM_BG);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        // check if already in queue
        if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
            //player is already in this queue
            return;

        // check if has free queue slots
        if (!player->HasFreeBattlegroundQueueId())
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, ERR_BATTLEGROUND_TOO_MANY_QUEUES);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        BattlegroundQueue& bgQueue = sBattlegroundMgr->m_BattlegroundQueues[bgQueueTypeId];

        GroupQueueInfo* ginfo = bgQueue.AddGroup(player, nullptr, bgTypeId, bracketEntry, 0, false, packet.JoinAsGroup, packet.BlacklistMap);
        uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        uint32 queueSlot = player->AddBattlegroundQueueId(bgQueueTypeId);

        // add joined time data
        player->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

        WorldPackets::Battleground::BattlefieldStatusQueued queued;
        sBattlegroundMgr->BuildBattlegroundStatusQueued(&queued, bg, player, queueSlot, ginfo->JoinTime, avgTime, ginfo->JoinType, false);
        SendPacket(queued.Write());

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, player->GetGUIDLow(), player->GetName());
    } else
    {
        Group* grp = grp = player->GetGroup();
        if (!grp)
            return;

        if (grp->GetLeaderGUID() != player->GetGUID())
            return;

        ObjectGuid errorGuid;
        err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, 0, bg->GetMaxPlayersPerTeam(), false, 0, errorGuid);
        packet.JoinAsGroup = (grp->GetMembersCount() >= bg->GetMinPlayersPerTeam());

        BattlegroundQueue& bgQueue = sBattlegroundMgr->m_BattlegroundQueues[bgQueueTypeId];
        GroupQueueInfo* ginfo = nullptr;
        uint32 avgTime = 0;

        if (!err)
        {
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: the following players are joining as group:");
            ginfo = bgQueue.AddGroup(player, grp, bgTypeId, bracketEntry, 0, false, packet.JoinAsGroup, packet.BlacklistMap);
            avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        }

        for (GroupReference* itr = grp->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            Player* member = itr->getSource();
            if (!member)
                continue;

            if (err)
            {
                WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
                sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, err, &errorGuid);
                member->GetSession()->SendPacket(battlefieldStatus.Write());
                continue;
            }

            // add to queue
            uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

            // add joined time data
            member->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

            WorldPackets::Battleground::BattlefieldStatusQueued queued;
            sBattlegroundMgr->BuildBattlegroundStatusQueued(&queued, bg, player, queueSlot, ginfo->JoinTime, avgTime, ginfo->JoinType, true);
            member->GetSession()->SendPacket(queued.Write());

            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, member->GetGUIDLow(), member->GetName());
        }
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: group end");

    }

    sBattlegroundMgr->ScheduleQueueUpdate(0, 0, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
}

void WorldSession::HandlePVPLogData(WorldPackets::Battleground::NullCmsg& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Battleground* bg = player->GetBattleground();
    if (!bg)
        return;

    if (bg->isArena())
        return;

    WorldPackets::Battleground::PVPLogData pvpLogData;
    bg->BuildPvPLogDataPacket(pvpLogData);
    SendPacket(pvpLogData.Write());
}

void WorldSession::HandleBattlefieldList(WorldPackets::Battleground::ListClient& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    BattlemasterListEntry const* bl = sBattlemasterListStore.LookupEntry(packet.ListID);
    if (bl)
        sBattlegroundMgr->SendBattlegroundList(player, ObjectGuid::Empty, BattlegroundTypeId(packet.ListID));
}

void WorldSession::HandleBattleFieldPort(WorldPackets::Battleground::Port& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (packet.Ticket.Id > PLAYER_MAX_BATTLEGROUND_QUEUES)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "HandleBattleFieldPort queueSlot %u", packet.Ticket.Id);
        return;
    }

    if (!player || !player->InBattlegroundQueue())
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundHandler: Invalid CMSG_BATTLEFIELD_PORT received from player (Name: %s, GUID: %s), he is not in bg_queue.", player->GetName(), player->GetGUID().ToString().c_str());
        return;
    }

    BattlegroundQueueTypeId bgQueueTypeId = player->GetBattlegroundQueueTypeId(packet.Ticket.Id);
    if (bgQueueTypeId == BATTLEGROUND_QUEUE_NONE)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundHandler: invalid queueSlot (%u) received.", packet.Ticket.Id);
        return;
    }

    if (bgQueueTypeId > MAX_BATTLEGROUND_QUEUE_TYPES)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "HandleBattleFieldPort bgQueueTypeId %u", bgQueueTypeId);
        return;
    }

    BattlegroundQueue& bgQueue = sBattlegroundMgr->m_BattlegroundQueues[bgQueueTypeId];
    BattlegroundTypeId bgTypeId = BattlegroundMgr::BGTemplateId(bgQueueTypeId);

    if (bgTypeId == BATTLEGROUND_TYPE_NONE || bgTypeId >= MAX_BATTLEGROUND_TYPE_ID)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundHandler: invalid bgTypeId (%u) received.", bgTypeId);
        return;
    }

    // we must use temporary variable, because GroupQueueInfo pointer can be deleted in BattlegroundQueue::RemovePlayer() function
    GroupQueueInfo ginfo;
    if (!bgQueue.GetPlayerGroupInfoData(player->GetGUID(), &ginfo))
        return;

    if (!ginfo.IsInvitedToBGInstanceGUID && packet.AcceptedInvite == true)
        return;

    // BGTemplateId returns BATTLEGROUND_AA when it is arena queue.
    // Do instance id search as there is no AA bg instances.
    Battleground* bg = sBattlegroundMgr->GetBattleground(ginfo.IsInvitedToBGInstanceGUID, (bgTypeId == BATTLEGROUND_AA || bgTypeId == BATTLEGROUND_RATED_10_VS_10) ? BATTLEGROUND_TYPE_NONE : bgTypeId);

    // bg template might and must be used in case of leaving queue, when instance is not created yet
    if (!bg && packet.AcceptedInvite == false)
        bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
    if (!bg)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "BattlegroundHandler: bg_template not found for type id %u.", bgTypeId);
        return;
    }

    // get real bg type
    bgTypeId = bg->GetTypeID();

    // expected bracket entry
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
    if (!bracketEntry)
        return;

    //some checks if player isn't cheating - it is not exactly cheating, but we cannot allow it
    if (packet.AcceptedInvite == true && ginfo.JoinType == 0)
    {
        //if player is trying to enter battleground (not arena!) and he has deserter debuff, we must just remove him from queue
        if (!player->CanJoinToBattleground())
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            SendPacket(battlefieldStatus.Write());

            packet.AcceptedInvite = false;
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player %s (%u) has a deserter debuff, do not port him to battleground!", player->GetName(), player->GetGUIDLow());
        }
        //if player don't match battleground max level, then do not allow him to enter! (this might happen when player leveled up during his waiting in queue
        if (player->getLevel() > bg->GetMaxLevel())
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: Player %s (%u) has level (%u) higher than maxlevel (%u) of battleground (%u)! Do not port him to battleground!",
                           player->GetName(), player->GetGUIDLow(), player->getLevel(), bg->GetMaxLevel(), bg->GetTypeID());
            packet.AcceptedInvite = false;
        }
    }

    if (packet.AcceptedInvite)
    {
        if (!player->IsInvitedForBattlegroundQueueType(bgQueueTypeId))
            return;                                 // cheating?

        if (!player->InBattleground())
            player->SetBattlegroundEntryPoint();

        if (!player->isAlive())
        {
            player->ResurrectPlayer(1.0f);
            player->SpawnCorpseBones();
        }

        if (player->isInFlight())
        {
            player->GetMotionMaster()->MovementExpired();
            player->CleanupAfterTaxiFlight();
        }

        WorldPackets::Battleground::BattlefieldStatusActive battlefieldStatus;
        sBattlegroundMgr->BuildBattlegroundStatusActive(&battlefieldStatus, bg, player, packet.Ticket.Id, player->GetBattlegroundQueueJoinTime(bgTypeId), ginfo.JoinType);
        player->SendDirectMessage(battlefieldStatus.Write());

        // remove battleground queue status from BGmgr
        bgQueue.RemovePlayer(player->GetGUID(), false);
        // this is still needed here if battleground "jumping" shouldn't add deserter debuff
        // also this is required to prevent stuck at old battleground after SetBattlegroundId set to new
        if (Battleground* currentBg = player->GetBattleground())
            currentBg->RemovePlayerAtLeave(player->GetGUID(), false, true);

        // set the destination instance id
        player->SetBattlegroundId(bg->GetInstanceID(), bgTypeId);
        // set the destination team
        player->SetBGTeam(ginfo.Team);
        // bg->HandleBeforeTeleportToBattleground(player);
        sBattlegroundMgr->SendToBattleground(player, ginfo.IsInvitedToBGInstanceGUID, bgTypeId);
        // add only in HandleMoveWorldPortAck()
        // bg->AddPlayer(player, team);
        player->OnEnterPvPCombat();

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player %s (%u) joined battle for bg %u, bgtype %u, queue type %u.", _player->GetName(), _player->GetGUIDLow(), bg->GetInstanceID(), bg->GetTypeID(), bgQueueTypeId);
    } else // leave queue
    {
        WorldPackets::Battleground::BattlefieldStatusNone none;
        sBattlegroundMgr->BuildBattlegroundStatusNone(&none, player, packet.Ticket.Id, 0, bg, ginfo.JoinType);
        SendPacket(none.Write());

        WorldPackets::Battleground::BattlefieldStatusFailed failed;
        sBattlegroundMgr->BuildBattlegroundStatusFailed(&failed, bg, player, packet.Ticket.Id, ERR_LEAVE_QUEUE);
        SendPacket(failed.Write());

        if (bg && bg->isRated())
            if (bg->GetStatus() == STATUS_WAIT_JOIN || bg->GetStatus() == STATUS_IN_PROGRESS)
                player->HandleArenaDeserter();

        player->RemoveBattlegroundQueueId(bgQueueTypeId);  // must be called this way, because if you move this call to queue->removeplayer, it causes bugs
        bgQueue.RemovePlayer(player->GetGUID(), true);
        // player left queue, we should update it - do not update Arena Queue
        if (!ginfo.JoinType)
            sBattlegroundMgr->ScheduleQueueUpdate(ginfo.MatchmakerRating, ginfo.JoinType, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player %s (%u) left queue for bgtype %u, queue type %u.", player->GetName(), player->GetGUIDLow(), bg->GetTypeID(), bgQueueTypeId);
    }
}

void WorldSession::HandleLeaveBattlefield(WorldPackets::Battleground::NullCmsg& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (Battleground* bg = player->GetBattleground())
        if (player->isInCombat() || bg->isArena())
            if (bg->GetStatus() != STATUS_WAIT_LEAVE)
                return;

    player->LeaveBattleground();
}

void WorldSession::HandleBattlefieldStatus(WorldPackets::Battleground::NullCmsg& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Battleground* bg = nullptr;
    for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
    {
        BattlegroundQueueTypeId bgQueueTypeId = player->GetBattlegroundQueueTypeId(i);
        if (!bgQueueTypeId)
            continue;

        BattlegroundTypeId bgTypeId = BattlegroundMgr::BGTemplateId(bgQueueTypeId);
        uint8 joinType = BattlegroundMgr::BGJoinType(bgQueueTypeId);
        if (bgTypeId == player->GetBattlegroundTypeId())
        {
            bg = player->GetBattleground();
            //i cannot check any variable from player class because player class doesn't know if player is in 2v2 / 3v3 or 5v5 arena
            //so i must use bg pointer to get that information
            if (bg && bg->GetJoinType() == joinType)
            {
                WorldPackets::Battleground::BattlefieldStatusActive battlefieldStatus;
                sBattlegroundMgr->BuildBattlegroundStatusActive(&battlefieldStatus, bg, player, i, player->GetBattlegroundQueueJoinTime(bgTypeId), joinType);
                SendPacket(battlefieldStatus.Write());
                continue;
            }
        }

        //we are sending update to player about queue - he can be invited there!
        //get GroupQueueInfo for queue status
        BattlegroundQueue& bgQueue = sBattlegroundMgr->m_BattlegroundQueues[bgQueueTypeId];
        GroupQueueInfo ginfo;
        if (!bgQueue.GetPlayerGroupInfoData(player->GetGUID(), &ginfo))
            continue;
        if (ginfo.IsInvitedToBGInstanceGUID)
        {
            bg = sBattlegroundMgr->GetBattleground(ginfo.IsInvitedToBGInstanceGUID, bgTypeId);
            if (!bg)
                continue;

            WorldPackets::Battleground::BattlefieldStatusNeedConfirmation battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusNeedConfirmation(&battlefieldStatus, bg, player, i, player->GetBattlegroundQueueJoinTime(bgTypeId), getMSTimeDiff(getMSTime(), ginfo.RemoveInviteTime), joinType);
            SendPacket(battlefieldStatus.Write());
        } else
        {
            bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
            if (!bg)
                continue;

            // expected bracket entry
            PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
            if (!bracketEntry)
                continue;

            uint32 avgTime = bgQueue.GetAverageQueueWaitTime(&ginfo, bracketEntry->GetBracketId());

            WorldPackets::Battleground::BattlefieldStatusQueued queued;
            sBattlegroundMgr->BuildBattlegroundStatusQueued(&queued, bg, player, i, avgTime, player->GetBattlegroundQueueJoinTime(bgTypeId), joinType, false); // how to check group join or no ?
            SendPacket(queued.Write());
        }
    }
}

void WorldSession::JoinBracket(uint8 slot)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (player->InBattleground())
        return;

    BattlegroundTypeId bgTypeId = slot < BRACKET_TYPE_RATED_BG ? BATTLEGROUND_AA : BATTLEGROUND_RATED_10_VS_10;

    uint32 arenaRating = 0;
    uint32 matchmakerRating = 0;

    uint8 Jointype = BattlegroundMgr::GetJoinTypeByBracketSlot(slot);

    //check existance
    Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
    if (!bg)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: template bg (all arenas) not found");
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, bgTypeId, nullptr))
    {
        ChatHandler(this).PSendSysMessage(LANG_ARENA_DISABLED);
        return;
    }

    bgTypeId = bg->GetTypeID();
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, Jointype);

    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
    if (!bracketEntry)
        return;

    uint32 avgTime = 0;
    GroupQueueInfo* ginfo;
    GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE;
    Group* grp = player->GetGroup();

    // no group found, error
    if (!grp || grp->GetLeaderGUID() != player->GetGUID())
        return;

    BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

    ObjectGuid errorGuid;
    err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, Jointype, Jointype, true, slot, errorGuid);
    if (!err)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: leader %s join type %u", player->GetName(), Jointype);

        matchmakerRating = grp->GetAverageMMR(BracketType(slot));
        WorldPackets::Battleground::IgnorMapInfo ignore;
        ginfo = bgQueue.AddGroup(player, grp, bgTypeId, bracketEntry, Jointype, true, false, ignore, matchmakerRating);
        avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
    } else
    {
        WorldPackets::Battleground::BattlefieldStatusFailed failed;
        sBattlegroundMgr->BuildBattlegroundStatusFailed(&failed, bg, player, 0, err, &errorGuid);
        SendPacket(failed.Write());

        sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: Join Fail. QueueTypeID: %u, JoinType: %u, Slot: %u, Err %u", bgQueueTypeId, Jointype, slot, err);
        return;
    }

    for (GroupReference* itr = grp->GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        Player* member = itr->getSource();
        if (!member)
            continue;

        // add joined time data
        member->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

        WorldPackets::Battleground::BattlefieldStatusQueued queued;
        sBattlegroundMgr->BuildBattlegroundStatusQueued(&queued, bg, player, member->AddBattlegroundQueueId(bgQueueTypeId), ginfo->JoinTime, avgTime, ginfo->JoinType, true);
        member->GetSession()->SendPacket(queued.Write());

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for arena as group bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, member->GetGUIDLow(), member->GetName());
    }
    sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, Jointype, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
}

void WorldSession::HandleBattlemasterJoinArena(WorldPackets::Battleground::JoinArena& packet)
{
    JoinBracket(packet.TeamSizeIndex);
}

void WorldSession::HandleBattlemasterJoinRated(WorldPacket& recvData)
{
    JoinBracket(BRACKET_TYPE_RATED_BG);
}

void WorldSession::HandleRequestRatedInfo(WorldPackets::Battleground::NullCmsg& /*packet*/)
{
    Player* player = GetPlayer();
    if (player)
        player->SendPvpRatedStats();
}

void WorldSession::HandleRequestPvpOptions(WorldPackets::Battleground::NullCmsg& /*packet*/)
{
    WorldPackets::Battleground::PVPOptionsEnabled options;
    options.RatedArenas = true;
    options.ArenaSkirmish = true;
    options.PugBattlegrounds = true;
    options.WargameBattlegrounds = true;
    options.WargameArenas = true;
    options.RatedBattlegrounds = true;
    SendPacket(options.Write());
}

void WorldSession::HandleRequestPvpReward(WorldPacket& recvData)
{
    Player* player = GetPlayer();
    if (player)
        player->SendPvpRewards();
}

//! This is const data used for calc some field for SMSG_RATED_BATTLEFIELD_INFO 
void WorldSession::HandlePersonalRatedInfoRequest(WorldPackets::Battleground::NullCmsg& /*packet*/)
{
    WorldPackets::Battleground::ConquestFormulaConstants constants;
    constants.PvpMinCPPerWeek = 1500;
    constants.PvpMaxCPPerWeek = 3000;
    constants.PvpCPBaseCoefficient = 1511.26f;
    constants.PvpCPExpCoefficient = 1639.28f;
    constants.PvpCPNumerator = 0.00412f;
    SendPacket(constants.Write());
}

void WorldSession::HandleAreaSpiritHealerQuery(WorldPackets::Battleground::AreaSpiritHealerQuery& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Creature* unit = player->GetMap()->GetCreature(packet.HealerGuid);
    if (!unit)
        return;

    if (!unit->isSpiritService())
        return;

    if (Battleground* bg = player->GetBattleground())
        sBattlegroundMgr->SendAreaSpiritHealerQuery(player, bg, packet.HealerGuid);

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId()))
        bf->SendAreaSpiritHealerQuery(player, packet.HealerGuid);
}

void WorldSession::HandleAreaSpiritHealerQueue(WorldPackets::Battleground::AreaSpiritHealerQueue& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Creature* unit = player->GetMap()->GetCreature(packet.HealerGuid);
    if (!unit)
        return;

    if (!unit->isSpiritService())
        return;

    if (Battleground* bg = player->GetBattleground())
        bg->AddPlayerToResurrectQueue(packet.HealerGuid, player->GetGUID());

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId()))
    {
        bf->RemovePlayerFromResurrectQueue(player->GetGUID());
        bf->AddPlayerToResurrectQueue(packet.HealerGuid, player->GetGUID());
    }
}

void WorldSession::HandleJoinSkirmish(WorldPackets::Battleground::JoinSkirmish& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;
    
    uint8 arenatype = sBattlegroundMgr->GetJoinTypeByBracketSlot(packet.Bracket);
    Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
    if (!bg)
        return;

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, nullptr))
    {
        ChatHandler(this).PSendSysMessage(LANG_ARENA_DISABLED);
        return;
    }

    BattlegroundTypeId bgTypeId = bg->GetTypeID();
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, arenatype);

    if (player->InBattleground())
        return;

    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
    if (!bracketEntry)
        return;

    if (!packet.JoinAsGroup)
    {
        if (player->isUsingLfg())
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, ERR_LFG_CANT_USE_BATTLEGROUND);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        if (!player->CanJoinToBattleground())
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
            return;

        if (!player->HasFreeBattlegroundQueueId())
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, ERR_BATTLEGROUND_TOO_MANY_QUEUES);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        GroupQueueInfo* ginfo = bgQueue.AddGroup(player, nullptr, bgTypeId, bracketEntry, arenatype, false, false, {}, 0);

        uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        uint32 queueSlot = player->AddBattlegroundQueueId(bgQueueTypeId);

        WorldPackets::Battleground::BattlefieldStatusQueued battlefieldStatus;
        sBattlegroundMgr->BuildBattlegroundStatusQueued(&battlefieldStatus, bg, player, queueSlot, ginfo->JoinTime, avgTime, arenatype, false);
        SendPacket(battlefieldStatus.Write());
    }
    else
    {
        Group* grp = player->GetGroup();

        if (!grp)
            return;

        if (grp->GetLeaderGUID() != player->GetGUID())
            return;

        ObjectGuid errorGuid;
        GroupJoinBattlegroundResult err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, 0, bg->GetMaxPlayersPerTeam(), false, 0, errorGuid);

        BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        GroupQueueInfo* ginfo = nullptr;
        uint32 avgTime = 0;

        if (!err)
        {
            ginfo = bgQueue.AddGroup(player, grp, bgTypeId, bracketEntry, arenatype, false, false, {}, 0);
            avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        }

        for (GroupReference* itr = grp->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            Player* member = itr->getSource();
            if (!member)
                continue;

            if (err)
            {
                WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
                sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, err, &errorGuid);
                member->GetSession()->SendPacket(battlefieldStatus.Write());
                continue;
            }

            WorldPackets::Battleground::BattlefieldStatusQueued battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusQueued(&battlefieldStatus, bg, member, member->AddBattlegroundQueueId(bgQueueTypeId), ginfo->JoinTime, avgTime, arenatype, true);
            member->SendDirectMessage(battlefieldStatus.Write());
        }
    }

    sBattlegroundMgr->ScheduleQueueUpdate(0, arenatype, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
}
