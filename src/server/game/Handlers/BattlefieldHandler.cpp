/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
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

#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Opcodes.h"

#include "Bracket.h"
#include "Packets/BattlegroundPackets.h"

void WorldSession::SendBfInvitePlayerToWar(uint64 const& queueID, uint32 areaID, uint32 pTime)
{
    WorldPackets::Battleground::BFMgrEntryInvite invite;
    invite.QueueID = queueID;
    invite.AreaID = areaID;
    invite.ExpireTime = time(nullptr) + pTime;
    SendPacket(invite.Write());
}

void WorldSession::SendBfInvitePlayerToQueue(uint64 queueId, int8 battleState)
{
    WorldPackets::Battleground::BFMgrQueueInvite queueInvite;
    queueInvite.QueueID = queueId;
    queueInvite.BattleState = battleState;
    queueInvite.Timeout = 4294967295;
    SendPacket(queueInvite.Write());
}

void WorldSession::SendBfQueueInviteResponse(uint64 queueId, uint32 zoneId, int8 battleStatus, bool canQueue /*= true*/, bool loggingIn /*= false*/)
{
    WorldPackets::Battleground::BFMgrQueueRequestResponse response;
    response.FailedPlayerGUID;
    response.QueueID = queueId;
    response.AreaID = zoneId;
    response.BattleState = battleStatus;
    response.Result = canQueue;
    response.LoggingIn = loggingIn;
    SendPacket(response.Write());
}

void WorldSession::SendBfEntered(uint64 queueID, bool relocated, bool onOffense)
{
    WorldPackets::Battleground::BFMgrEntering entering;
    entering.ClearedAFK = _player->isAFK();
    entering.OnOffense = onOffense;
    entering.Relocated = relocated;
    entering.QueueID = queueID;
    SendPacket(entering.Write());
}

void WorldSession::SendBfLeaveMessage(uint64 queueId, int8 battleState, bool relocated, BFLeaveReason reason /*= BF_LEAVE_REASON_EXITED*/)
{
    WorldPackets::Battleground::BFMgrEjected packet;
    packet.QueueID = queueId;
    packet.Reason = reason;
    packet.BattleState = battleState;
    packet.Relocated = relocated;
    SendPacket(packet.Write());
}

void WorldSession::HandleBfQueueInviteResponse(WorldPackets::Battleground::EntryOrQueueInviteResponse& packet)
{
    if (!packet.AcceptedInvite)
        return;

    Battlefield* bf = sBattlefieldMgr->GetBattlefieldByQueueID(packet.QueueID);
    if (!bf)
        return;
    
    if (packet.AcceptedInvite)
        bf->PlayerAcceptInviteToQueue(_player);
    else if (bf->IsWarTime() || bf->IsOnStartGrouping())
    {
        if (_player->GetZoneId() == bf->GetZoneId())
            bf->KickPlayerFromBattlefield(_player->GetGUID());
    }
}

void WorldSession::HandleBfEntryInviteResponse(WorldPackets::Battleground::EntryOrQueueInviteResponse& packet)
{
    Battlefield* bf = sBattlefieldMgr->GetBattlefieldByQueueID(packet.QueueID);
    if (!bf)
        return;

    if (packet.AcceptedInvite)
        bf->PlayerAcceptInviteToWar(_player);
    else
        if (_player->GetZoneId() == bf->GetZoneId())
            bf->KickPlayerFromBattlefield(_player->GetGUID());
}

void WorldSession::HandleBfQueueRequest(WorldPackets::Battleground::QueueRequest& packet)
{
    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldByQueueID(packet.QueueID))
    {
        if (bf->IsWarTime())
            bf->InvitePlayerToWar(_player);
        else
        {
            uint32 timer = bf->GetTimer() / 1000;
            if (timer < 15 * MINUTE)
                bf->InvitePlayerToQueue(_player);
        }
    }
}

void WorldSession::HandleBfExitQueueRequest(WorldPackets::Battleground::ExitRequest& packet)
{
    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldByQueueID(packet.QueueID))
    {
        bf->AskToLeaveQueue(_player);
        SendBfLeaveMessage(packet.QueueID, bf->GetState(), _player->GetZoneId() == bf->GetZoneId());
    }
}

void WorldSession::HandleBfExitRequest(WorldPacket& recv_data)
{
    Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(_player->GetZoneId());
    if (!bf)
        return;

    //SendBfLeaveMessage(bf->GetBattleId());
    bf->AskToLeaveQueue(_player);
    bf->KickPlayerFromBattlefield(_player->GetGUID());
}

void WorldSession::HandleReportPvPAFK(WorldPacket & recvData)
{
    ObjectGuid playerGuid;
    recvData >> playerGuid;

    if (Player* reportedPlayer = ObjectAccessor::FindPlayer(playerGuid))
        reportedPlayer->ReportedAfkBy(_player);
}

void WorldSession::HandleBfSetPreferedCemetry(WorldPacket & recvData)
{
    uint32 graveID = 0;
    recvData >> graveID;
}
