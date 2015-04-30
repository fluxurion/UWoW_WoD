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

//This send to player windows for invite player to join the war
//Param1:(m_Guid) the BattleId of Bf
//Param2:(ZoneId) the zone where the battle is (4197 for wg)
//Param3:(time) Time in second that the player have for accept
//! 6.0.3
void WorldSession::SendBfInvitePlayerToWar(uint64 const& guid, uint32 zoneId, uint32 pTime)
{
    //Send packet
    WorldPacket data(SMSG_BF_MGR_ENTRY_INVITE);
    data << guid;
    data << uint32(zoneId);         // Zone Id
    data << uint32(time(NULL) + pTime); // Invite lasts until

    //Sending the packet to player
    SendPacket(&data);
}

//This send invitation to player to join the queue
//Param1:(guid) the guid of Bf
//! 6.0.3
void WorldSession::SendBfInvitePlayerToQueue(uint64 const& guid)
{
    bool warmup = true;

    WorldPacket data(SMSG_BF_MGR_QUEUE_INVITE);
    data << guid;
    data << uint8(1);  //warmup. 1 - WG, 2 - TB
    
    data << int32(-1);  //Timeout
    data << int32(0);   //MinLevel
    data << int32(0);   //MaxLevel
    data << int32(0);   //InstanceID
    data << int32(0);   //MapID

    data.WriteBit(0); // index

    //Sending packet to player
    SendPacket(&data);
}

//This send packet for inform player that he join queue
//Param1:(guid) the guid of Bf
//Param2:(ZoneId) the zone where the battle is (4197 for wg)
//Param3:(CanQueue) if able to queue
//Param4:(Full) on log in is full
//! 6.0.3
void WorldSession::SendBfQueueInviteResponse(uint64 const& guid, uint32 ZoneId, bool CanQueue, bool Full)
{
    ObjectGuid FailedPlayerGuid;

    WorldPacket data(SMSG_BF_MGR_QUEUE_REQUEST_RESPONSE);
    data << guid;
    data << uint32(ZoneId);
    data << uint8(1); // Warmup
    data << FailedPlayerGuid;
    data << uint8((CanQueue ? 1 : 0));  //Accepted          //0 you cannot queue wg     //1 you are queued

    data.WriteBit((Full ? 0 : 1)); // //Logging In        //0 wg full                 //1 queue for upcoming (we may need to swap it)

    SendPacket(&data);
}

//This is call when player accept to join war
//Param1:(guid) the guid of Bf
//! 6.0.3
void WorldSession::SendBfEntered(uint64 const& guid)
{
//    m_PlayerInWar[player->GetTeamId()].insert(player->GetGUID());

    WorldPacket data(SMSG_BF_MGR_ENTERING);

    data.WriteBit(_player->isAFK() ? 1 : 0); //Clear AFK
    data.WriteBit(1); // unk
    data.WriteBit(1); // unk

    data << guid;
    SendPacket(&data);
}

//! 6.0.3
void WorldSession::SendBfLeaveMessage(uint64 const& guid, BFLeaveReason reason)
{
    WorldPacket data(SMSG_BF_MGR_EJECTED);
    data << guid;
    data << uint8(2); // byte BattleStatus
    data << uint8(reason); // byte Reason

    data.WriteBit(0); // relocated

    SendPacket(&data);
}

//Send by client when he click on accept for queue
//! 6.0.3
void WorldSession::HandleBfQueueInviteResponse(WorldPacket & recvData)
{
    uint8 accepted;
    uint64 guid;
    recvData >> guid;
    accepted = recvData.ReadBit();

    //sLog->outError(LOG_FILTER_GENERAL, "HandleQueueInviteResponse: GUID:" UI64FMTD " Accepted:%u", guid, accepted);

    if(!accepted)
        return;

    Battlefield* bf = sBattlefieldMgr->GetBattlefieldByGUID(guid);
    if (!bf)
        return;
    
    if (accepted)
        bf->PlayerAcceptInviteToQueue(_player);
    else if (bf->IsWarTime() || bf->IsOnStartGrouping())
    {
        if (_player->GetZoneId() == bf->GetZoneId())
            bf->KickPlayerFromBattlefield(_player->GetGUID());
    }
}

//Send by client on clicking in accept or refuse of invitation windows for join game
//! 6.0.3
void WorldSession::HandleBfEntryInviteResponse(WorldPacket & recvData)
{
    uint8 accepted;
    uint64 guid;
    recvData >> guid;
    accepted = recvData.ReadBit();

    //sLog->outError(LOG_FILTER_GENERAL, "HandleBattlefieldInviteResponse: GUID:" UI64FMTD " Accepted:%u", guid, accepted);

    Battlefield* bf = sBattlefieldMgr->GetBattlefieldByGUID(guid);
    if (!bf)
        return;

    if (accepted)
        bf->PlayerAcceptInviteToWar(_player);
    else
        if (_player->GetZoneId() == bf->GetZoneId())
            bf->KickPlayerFromBattlefield(_player->GetGUID());
}

//! 6.0.6
void WorldSession::HandleBfQueueRequest(WorldPacket& recvData)
{
    uint64 guid;
    recvData >> guid;

    //sLog->outError(LOG_FILTER_GENERAL, "HandleBfQueueRequest: GUID:" UI64FMTD " ", guid.GetCounter());

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldByGUID(guid))
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

//! 6.0.3
void WorldSession::HandleBfExitQueueRequest(WorldPacket & recvData)
{
    uint64 guid;
    recvData >> guid;

    //sLog->outError(LOG_FILTER_GENERAL, "HandleBfExitQueueRequest: GUID:" UI64FMTD " ", guid.GetCounter());

    SendBfLeaveMessage(guid);

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldByGUID(guid))
        bf->AskToLeaveQueue(_player);
}

void WorldSession::HandleBfExitRequest(WorldPacket& recv_data)
{
    sLog->outError(LOG_FILTER_GENERAL, "HandleBfExitRequest");
    Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(_player->GetZoneId());

    if (!bf)
        return;

    //SendBfLeaveMessage(bf->GetBattleId());
    bf->AskToLeaveQueue(_player);
    bf->KickPlayerFromBattlefield(_player->GetGUID());
}

//! 6.0.3
void WorldSession::HandleReportPvPAFK(WorldPacket & recvData)
{
    ObjectGuid playerGuid;
    recvData >> playerGuid;

    Player* reportedPlayer = ObjectAccessor::FindPlayer(playerGuid);

    if (!reportedPlayer)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "WorldSession::HandleReportPvPAFK: player not found");
        return;
    }

    //sLog->outDebug(LOG_FILTER_BATTLEGROUND, "WorldSession::HandleReportPvPAFK: %s reported %s", _player->GetName(), reportedPlayer->GetName());

    reportedPlayer->ReportedAfkBy(_player);
}

void WorldSession::HandleBfSetPreferedCemetry(WorldPacket & recvData)
{
    uint32 graveID = 0;
    recvData >> graveID;
    //sLog->outError(LOG_FILTER_GENERAL, "HandleBfSetPreferedCemetry: GraveID: %u", graveID);
}