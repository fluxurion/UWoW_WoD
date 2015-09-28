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

#include "Common.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "Chat.h"
#include "Language.h"
#include "Log.h"
#include "Player.h"
#include "Object.h"
#include "Opcodes.h"
#include "DisableMgr.h"
#include "Group.h"
#include "Bracket.h"
#include "BattlegroundPackets.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"

void WorldSession::HandleBattlemasterHelloOpcode(WorldPacket & recvData)
{
    ObjectGuid guid;
    recvData >> guid;

    Creature* unit = GetPlayer()->GetMap()->GetCreature(guid);
    if (!unit)
        return;

    if (!unit->isBattleMaster())                             // it's not battlemaster
        return;

    // Stop the npc if moving
    unit->StopMoving();

    BattlegroundTypeId bgTypeId = sBattlegroundMgr->GetBattleMasterBG(unit->GetEntry());

    if (!_player->GetBGAccessByLevel(bgTypeId))
    {
                                                            // temp, must be gossip message...
        SendNotification(LANG_YOUR_BG_LEVEL_REQ_ERROR);
        return;
    }

    SendBattleGroundList(guid, bgTypeId);
}

void WorldSession::SendBattleGroundList(ObjectGuid guid, BattlegroundTypeId bgTypeId)
{
    WorldPacket data;
    sBattlegroundMgr->BuildBattlegroundListPacket(&data, guid, _player, bgTypeId);
    SendPacket(&data);
}

//! 6.0.3
void WorldSession::HandleBattlemasterJoin(WorldPackets::Battleground::Join& packet)
{
    Group* grp = NULL;

    if (!sBattlemasterListStore.LookupEntry(packet.QueueID))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: invalid bgtype (%u) received. possible cheater? player guid %u", packet.QueueID, _player->GetGUID().GetCounter());
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, packet.QueueID, NULL))
    {
        ChatHandler(this).PSendSysMessage(LANG_BG_DISABLED);
        return;
    }

    BattlegroundTypeId bgTypeId = BattlegroundTypeId(packet.QueueID);

    // can do this, since it's battleground, not arena
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, 0);
    BattlegroundQueueTypeId bgQueueTypeIdRandom = BattlegroundMgr::BGQueueTypeId(BATTLEGROUND_RB, 0);

    // ignore if player is already in BG
    if (_player->InBattleground())
        return;

    // get bg instance or bg template if instance not found
    Battleground* bg = NULL;
    //if (instanceId)
    //    bg = sBattlegroundMgr->GetBattlegroundThroughClientInstance(instanceId, bgTypeId);

    if (!bg)
        bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
    if (!bg)
        return;

    // expected bracket entry
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), _player->getLevel());
    if (!bracketEntry)
        return;

    GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE;

    // check queue conditions
    if (!packet.JoinAsGroup)
    {
        if (GetPlayer()->isUsingLfg())
        {
            // player is using dungeon finder or raid finder
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, ERR_LFG_CANT_USE_BATTLEGROUND);
            GetPlayer()->GetSession()->SendPacket(&data);
            return;
        }

        // check Deserter debuff
        if (!_player->CanJoinToBattleground())
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            _player->GetSession()->SendPacket(&data);
            return;
        }

        if (_player->GetBattlegroundQueueIndex(bgQueueTypeIdRandom) < PLAYER_MAX_BATTLEGROUND_QUEUES)
        {
            //player is already in random queue
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, ERR_IN_RANDOM_BG);
            _player->GetSession()->SendPacket(&data);
            return;
        }

        if (_player->InBattlegroundQueue() && bgTypeId == BATTLEGROUND_RB)
        {
            //player is already in queue, can't start random queue
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, ERR_IN_NON_RANDOM_BG);
            _player->GetSession()->SendPacket(&data);
            return;
        }

        // check if already in queue
        if (_player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
            //player is already in this queue
            return;

        // check if has free queue slots
        if (!_player->HasFreeBattlegroundQueueId())
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, ERR_BATTLEGROUND_TOO_MANY_QUEUES);
            _player->GetSession()->SendPacket(&data);
            return;
        }

        BattlegroundQueue& bgQueue = sBattlegroundMgr->m_BattlegroundQueues[bgQueueTypeId];

        GroupQueueInfo* ginfo = bgQueue.AddGroup(_player, NULL, bgTypeId, bracketEntry, 0, false, packet.JoinAsGroup, packet.BlacklistMap);
        uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        uint32 queueSlot = _player->AddBattlegroundQueueId(bgQueueTypeId);

        // add joined time data
        _player->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

        WorldPacket data; // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, _player, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, ginfo->JoinType);
        SendPacket(&data);

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, _player->GetGUID().GetCounter(), _player->GetName());
    }
    else
    {
        grp = _player->GetGroup();

        if (!grp)
            return;

        if (grp->GetLeaderGUID() != _player->GetGUID())
            return;

        err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, 0, bg->GetMaxPlayersPerTeam(), false, 0);
        packet.JoinAsGroup = (grp->GetMembersCount() >= bg->GetMinPlayersPerTeam());

        BattlegroundQueue& bgQueue = sBattlegroundMgr->m_BattlegroundQueues[bgQueueTypeId];
        GroupQueueInfo* ginfo = NULL;
        uint32 avgTime = 0;

        if (!err)
        {
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: the following players are joining as group:");
            ginfo = bgQueue.AddGroup(_player, grp, bgTypeId, bracketEntry, 0, false, packet.JoinAsGroup, packet.BlacklistMap);
            avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        }

        for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* member = itr->getSource();
            if (!member)
                continue;   // this should never happen

            if (err)
            {
                WorldPacket data;
                sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, err);
                member->GetSession()->SendPacket(&data);
                continue;
            }

            // add to queue
            uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

            // add joined time data
            member->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

            WorldPacket data; // send status packet (in queue)
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, member, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, ginfo->JoinType);
            member->GetSession()->SendPacket(&data);

            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, member->GetGUID().GetCounter(), member->GetName());
        }
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: group end");

    }

    sBattlegroundMgr->ScheduleQueueUpdate(0, 0, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
}

//! 6.0.3
void WorldSession::HandlePVPLogDataOpcode(WorldPacket & /*recvData*/)
{
    Battleground* bg = _player->GetBattleground();
    if (!bg)
        return;

    // Prevent players from sending BuildPvpLogDataPacket in an arena except for when sent in BattleGround::EndBattleGround.
    if (bg->isArena())
        return;

    WorldPacket data;
    sBattlegroundMgr->BuildPvpLogDataPacket(&data, bg);
    SendPacket(&data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent MSG_PVP_LOG_DATA Message");
}

void WorldSession::HandleBattlefieldList(WorldPackets::Battleground::ListClient& packet)
{
    BattlemasterListEntry const* bl = sBattlemasterListStore.LookupEntry(packet.ListID);
    if (!bl)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundHandler: invalid bgtype (%u) with player (Name: %s, GUID: %u) received.", packet.ListID, _player->GetName(), _player->GetGUID().GetCounter());
        return;
    }

    WorldPacket data;
    sBattlegroundMgr->BuildBattlegroundListPacket(&data, ObjectGuid::Empty, _player, BattlegroundTypeId(packet.ListID));
    SendPacket(&data);
}

//! 6.0.3
void WorldSession::HandleBattleFieldPort(WorldPackets::Battleground::Port& packet)
{
	WorldPacket data;

    if (packet.TicketData.Id > PLAYER_MAX_BATTLEGROUND_QUEUES)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "HandleBattleFieldPort queueSlot %u", packet.TicketData.Id);
        return;
    }

    if (!_player || !_player->InBattlegroundQueue())
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundHandler: Invalid CMSG_BATTLEFIELD_PORT received from player (Name: %s, GUID: %s), he is not in bg_queue.", _player->GetName(), _player->GetGUID().ToString().c_str());
        return;
    }

    BattlegroundQueueTypeId bgQueueTypeId = _player->GetBattlegroundQueueTypeId(packet.TicketData.Id);
    if (bgQueueTypeId == BATTLEGROUND_QUEUE_NONE)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundHandler: invalid queueSlot (%u) received.", packet.TicketData.Id);
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
    if (!bgQueue.GetPlayerGroupInfoData(_player->GetGUID(), &ginfo))
    {
        //sLog->outError(LOG_FILTER_NETWORKIO, "BattlegroundHandler: itrplayerstatus not found.");
        return;
    }

    // if packet.AcceptedInvite == 1, then instanceId is required
    if (!ginfo.IsInvitedToBGInstanceGUID && packet.AcceptedInvite == 1)
    {
        //sLog->outError(LOG_FILTER_NETWORKIO, "BattlegroundHandler: instance not found.");
        return;
    }

    // BGTemplateId returns BATTLEGROUND_AA when it is arena queue.
    // Do instance id search as there is no AA bg instances.
    Battleground* bg = sBattlegroundMgr->GetBattleground(ginfo.IsInvitedToBGInstanceGUID, (bgTypeId == BATTLEGROUND_AA || bgTypeId == BATTLEGROUND_RATED_10_VS_10) ? BATTLEGROUND_TYPE_NONE : bgTypeId);

    // bg template might and must be used in case of leaving queue, when instance is not created yet
    if (!bg && packet.AcceptedInvite == 0)
        bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
    if (!bg)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "BattlegroundHandler: bg_template not found for type id %u.", bgTypeId);
        return;
    }

    // get real bg type
    bgTypeId = bg->GetTypeID();

    // expected bracket entry
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), _player->getLevel());
    if (!bracketEntry)
        return;

    //some checks if player isn't cheating - it is not exactly cheating, but we cannot allow it
    if (packet.AcceptedInvite == 1 && ginfo.JoinType == 0)
    {
        //if player is trying to enter battleground (not arena!) and he has deserter debuff, we must just remove him from queue
        if (!_player->CanJoinToBattleground())
        {
            //send bg command result to show nice message
            WorldPacket data2;
            sBattlegroundMgr->BuildStatusFailedPacket(&data2, bg, _player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            _player->GetSession()->SendPacket(&data2);
            packet.AcceptedInvite = 0;
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player %s (%u) has a deserter debuff, do not port him to battleground!", _player->GetName(), _player->GetGUID().GetCounter());
        }
        //if player don't match battleground max level, then do not allow him to enter! (this might happen when player leveled up during his waiting in queue
        if (_player->getLevel() > bg->GetMaxLevel())
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: Player %s (%u) has level (%u) higher than maxlevel (%u) of battleground (%u)! Do not port him to battleground!",
                _player->GetName(), _player->GetGUID().GetCounter(), _player->getLevel(), bg->GetMaxLevel(), bg->GetTypeID());
            packet.AcceptedInvite = 0;
        }
    }

    if (packet.AcceptedInvite)
    {                                        // port to battleground
        if (!_player->IsInvitedForBattlegroundQueueType(bgQueueTypeId))
            return;                                 // cheating?

        if (!_player->InBattleground())
            _player->SetBattlegroundEntryPoint();

        // resurrect the player
        if (!_player->isAlive())
        {
            _player->ResurrectPlayer(1.0f);
            _player->SpawnCorpseBones();
        }
        // stop taxi flight at port
        if (_player->isInFlight())
        {
            _player->GetMotionMaster()->MovementExpired();
            _player->CleanupAfterTaxiFlight();
        }

        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, _player, packet.TicketData.Id, STATUS_IN_PROGRESS, _player->GetBattlegroundQueueJoinTime(bgTypeId), bg->GetElapsedTime(), ginfo.JoinType);
        _player->GetSession()->SendPacket(&data);
        // remove battleground queue status from BGmgr
        bgQueue.RemovePlayer(_player->GetGUID(), false);
        // this is still needed here if battleground "jumping" shouldn't add deserter debuff
        // also this is required to prevent stuck at old battleground after SetBattlegroundId set to new
        if (Battleground* currentBg = _player->GetBattleground())
            currentBg->RemovePlayerAtLeave(_player->GetGUID(), false, true);

        // set the destination instance id
        _player->SetBattlegroundId(bg->GetInstanceID(), bgTypeId);
        // set the destination team
        _player->SetBGTeam(ginfo.Team);
        // bg->HandleBeforeTeleportToBattleground(_player);
        sBattlegroundMgr->SendToBattleground(_player, ginfo.IsInvitedToBGInstanceGUID, bgTypeId);
        // add only in HandleMoveWorldPortAck()
        // bg->AddPlayer(_player, team);

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player %s (%u) joined battle for bg %u, bgtype %u, queue type %u.", _player->GetName(), _player->GetGUID().GetCounter(), bg->GetInstanceID(), bg->GetTypeID(), bgQueueTypeId);
    }
    else // leave queue
    {
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, _player, packet.TicketData.Id, STATUS_NONE, 0, 0, ginfo.JoinType);
        SendPacket(&data);

        sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, packet.TicketData.Id, ERR_LEAVE_QUEUE);
        SendPacket(&data);

        if (bg && bg->isRated())
            if (bg->GetStatus() == STATUS_WAIT_JOIN || bg->GetStatus() == STATUS_IN_PROGRESS)
                _player->HandleArenaDeserter();

        _player->RemoveBattlegroundQueueId(bgQueueTypeId);  // must be called this way, because if you move this call to queue->removeplayer, it causes bugs
        bgQueue.RemovePlayer(_player->GetGUID(), true);
        // player left queue, we should update it - do not update Arena Queue
        if (!ginfo.JoinType)
            sBattlegroundMgr->ScheduleQueueUpdate(ginfo.MatchmakerRating, ginfo.JoinType, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player %s (%u) left queue for bgtype %u, queue type %u.", _player->GetName(), _player->GetGUID().GetCounter(), bg->GetTypeID(), bgQueueTypeId);
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
    WorldPacket data;
    // we must update all queues here
    Battleground* bg = NULL;
    for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
    {
        BattlegroundQueueTypeId bgQueueTypeId = _player->GetBattlegroundQueueTypeId(i);
        if (!bgQueueTypeId)
            continue;
        BattlegroundTypeId bgTypeId = BattlegroundMgr::BGTemplateId(bgQueueTypeId);
        uint8 JoinType = BattlegroundMgr::BGJoinType(bgQueueTypeId);
        if (bgTypeId == _player->GetBattlegroundTypeId())
        {
            bg = _player->GetBattleground();
            //i cannot check any variable from player class because player class doesn't know if player is in 2v2 / 3v3 or 5v5 arena
            //so i must use bg pointer to get that information
            if (bg && bg->GetJoinType() == JoinType)
            {
                // this line is checked, i only don't know if GetElapsedTime() is changing itself after bg end!
                // send status in Battleground
                sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, _player, i, STATUS_IN_PROGRESS, _player->GetBattlegroundQueueJoinTime(bgTypeId), bg->GetElapsedTime(), JoinType);
                SendPacket(&data);
                continue;
            }
        }

        //we are sending update to player about queue - he can be invited there!
        //get GroupQueueInfo for queue status
        BattlegroundQueue& bgQueue = sBattlegroundMgr->m_BattlegroundQueues[bgQueueTypeId];
        GroupQueueInfo ginfo;
        if (!bgQueue.GetPlayerGroupInfoData(_player->GetGUID(), &ginfo))
            continue;
        if (ginfo.IsInvitedToBGInstanceGUID)
        {
            bg = sBattlegroundMgr->GetBattleground(ginfo.IsInvitedToBGInstanceGUID, bgTypeId);
            if (!bg)
                continue;

            // send status invited to Battleground
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, GetPlayer(), i, STATUS_WAIT_JOIN, getMSTimeDiff(getMSTime(), ginfo.RemoveInviteTime), _player->GetBattlegroundQueueJoinTime(bgTypeId), JoinType);
            SendPacket(&data);
        }
        else
        {
            bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
            if (!bg)
                continue;

            // expected bracket entry
            PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), _player->getLevel());
            if (!bracketEntry)
                continue;

            uint32 avgTime = bgQueue.GetAverageQueueWaitTime(&ginfo, bracketEntry->GetBracketId());
            // send status in Battleground Queue
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, GetPlayer(), i, STATUS_WAIT_QUEUE, avgTime, _player->GetBattlegroundQueueJoinTime(bgTypeId), JoinType);
            SendPacket(&data);
        }
    }
}

void WorldSession::JoinBracket(uint8 slot)
{
    // ignore if we already in BG or BG queue
    if (_player->InBattleground())
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

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, bgTypeId, NULL))
    {
        ChatHandler(this).PSendSysMessage(LANG_ARENA_DISABLED);
        return;
    }

    bgTypeId = bg->GetTypeID();
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, Jointype);

    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), _player->getLevel());
    if (!bracketEntry)
        return;
    
    uint32 avgTime = 0;
    GroupQueueInfo* ginfo;
    GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE;
    Group* grp = _player->GetGroup();

    //! Custom conection for join withoud group
    if (!grp && sBattlegroundMgr->isArenaTesting())
    {
        BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

        WorldPackets::Battleground::IgnorMapInfo ignore;    //empty
        ginfo = bgQueue.AddGroup(_player, NULL, bgTypeId, bracketEntry, Jointype, true, false, ignore, 1500);
        avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId()); 

        uint32 queueSlot = _player->AddBattlegroundQueueId(bgQueueTypeId);
        _player->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

        WorldPacket data; // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, _player, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, Jointype);
        _player->GetSession()->SendPacket(&data);
        sBattlegroundMgr->ScheduleQueueUpdate(1500, Jointype, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
        return;
    }

     // no group found, error
     if (!grp || grp->GetLeaderGUID() != _player->GetGUID())
         return;

    BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

    err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, Jointype, Jointype, true, slot);
    if (!err || (err && sBattlegroundMgr->isArenaTesting()))
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: leader %s join type %u", _player->GetName(), Jointype);

        matchmakerRating = grp->GetAverageMMR(BracketType(slot));
        WorldPackets::Battleground::IgnorMapInfo ignore;
        ginfo = bgQueue.AddGroup(_player, grp, bgTypeId, bracketEntry, Jointype, true, false, ignore, matchmakerRating);
        avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());        
    }
    else
    {
         WorldPacket data;
         sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, err);
         SendPacket(&data);

         sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: Join Fail. QueueTypeID: %u, JoinType: %u, Slot: %u, Err %u", bgQueueTypeId, Jointype, slot, err);
         return;
    }

    for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->getSource();
        if (!member)
            continue;

         // add to queue
        uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

        // add joined time data
        member->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

        WorldPacket data; // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, member, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, Jointype);
        member->GetSession()->SendPacket(&data);

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for arena as group bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, member->GetGUID().GetCounter(), member->GetName());
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
    _player->SendPvpRatedStats();
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
    _player->SendPvpRewards();
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
        sBattlegroundMgr->SendAreaSpiritHealerQueryOpcode(player, bg, packet.HealerGuid);

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId()))
        bf->SendAreaSpiritHealerQueryOpcode(player, packet.HealerGuid);
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
