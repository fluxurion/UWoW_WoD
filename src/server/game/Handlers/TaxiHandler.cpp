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
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "Path.h"
#include "WaypointMovementGenerator.h"

//! 6.0.3
void WorldSession::HandleTaxiNodeStatusQueryOpcode(WorldPacket& recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_TAXI_NODE_STATUS_QUERY");

    ObjectGuid guid;
    recvData >> guid;

    SendTaxiStatus(guid);
}

//! 6.0.3
void WorldSession::SendTaxiStatus(ObjectGuid guid)
{
    // cheating checks
    Creature* unit = GetPlayer()->GetMap()->GetCreature(guid);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSession::SendTaxiStatus - Unit (GUID: %u) not found.", guid.GetCounter());
        return;
    }

    uint32 curloc = sObjectMgr->GetNearestTaxiNode(unit->GetPositionX(), unit->GetPositionY(), unit->GetPositionZ(), unit->GetMapId(), GetPlayer()->GetTeam());

    uint8 statu = 0;   // 0 - no, 1 - ready for learning, 2 - learned
    // not found nearest
    if (curloc)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: current location %u ", curloc);
        statu = GetPlayer()->m_taxi.IsTaximaskNodeKnown(curloc) ? 2 : 1;
    }

    WorldPacket data(SMSG_TAXI_NODE_STATUS, 9);
    data << guid;
    data.WriteBits(statu, 2);
    SendPacket(&data);

    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_TAXI_NODE_STATUS");
}

//! 6.0.3
void WorldSession::HandleTaxiQueryAvailableNodes(WorldPacket& recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_TAXI_QUERY_AVAILABLE_NODES");

    ObjectGuid guid;
    recvData >> guid;

    // cheating checks
    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_FLIGHTMASTER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleTaxiQueryAvailableNodes - Unit (GUID: %u) not found or you can't interact with him.", uint32(guid.GetCounter()));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // unknown taxi node case
    if (SendLearnNewTaxiNode(unit))
        return;

    // known taxi node case
    SendTaxiMenu(unit);
}

//! 6.0.3
void WorldSession::SendTaxiMenu(Creature* unit)
{
    // find current node
    uint32 curloc = sObjectMgr->GetNearestTaxiNode(unit->GetPositionX(), unit->GetPositionY(), unit->GetPositionZ(), unit->GetMapId(), GetPlayer()->GetTeam());
    if (!curloc)
        return;

    bool lastTaxiCheaterState = GetPlayer()->isTaxiCheater();
    if (unit->GetEntry() == 29480)
        GetPlayer()->SetTaxiCheater(true); // Grimwing in Ebon Hold, special case. NOTE: Not perfect, Zul'Aman should not be included according to WoWhead, and I think taxicheat includes it.

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_TAXI_NODE_STATUS_QUERY %u ", curloc);

    WorldPacket data(SMSG_SHOW_TAXI_NODES, (4 + 8 + 4 + 8 * 4));
    data.WriteBit(1);                    // has data
    data << uint32(TaxiMaskSize);
    data << unit->GetGUID();
    data << uint32(curloc);

    GetPlayer()->m_taxi.AppendTaximaskTo(data, GetPlayer()->isTaxiCheater());
    SendPacket(&data);

    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_SHOW_TAXI_NODES");

    GetPlayer()->SetTaxiCheater(lastTaxiCheaterState);
}

void WorldSession::SendDoFlight(uint32 mountDisplayId, uint32 path, uint32 pathNode)
{
    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    while (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
        GetPlayer()->GetMotionMaster()->MovementExpired(false);

    if (mountDisplayId)
        GetPlayer()->Mount(mountDisplayId);

    GetPlayer()->GetMotionMaster()->MoveTaxiFlight(path, pathNode);
}

//! 6.0.3
bool WorldSession::SendLearnNewTaxiNode(Creature* unit)
{
    // find current node
    uint32 curloc = sObjectMgr->GetNearestTaxiNode(unit->GetPositionX(), unit->GetPositionY(), unit->GetPositionZ(), unit->GetMapId(), GetPlayer()->GetTeam());

    if (curloc == 0)
        return true;                                        // `true` send to avoid WorldSession::SendTaxiMenu call with one more curlock seartch with same false result.

    if (GetPlayer()->m_taxi.SetTaximaskNode(curloc))
    {
        WorldPacket msg(SMSG_NEW_TAXI_PATH, 0);
        SendPacket(&msg);

        WorldPacket update(SMSG_TAXI_NODE_STATUS, 9);
        update << unit->GetGUID();
        update.WriteBits(2, 2);
        SendPacket(&update);

        return true;
    }
    else
        return false;
}

void WorldSession::SendDiscoverNewTaxiNode(uint32 nodeid)
{
    if (GetPlayer()->m_taxi.SetTaximaskNode(nodeid))
    {
        WorldPacket msg(SMSG_NEW_TAXI_PATH, 0);
        SendPacket(&msg);
    }
}

//! 6.0.3
void WorldSession::HandleActivateTaxiExpressOpcode (WorldPacket & recvData)
{
    ObjectGuid guid;
    uint32 node_count;

    recvData >> guid >> node_count;

    std::vector<uint32> nodes;
    for (uint32 i = 0; i < node_count; ++i)
    {
        uint32 node;
        recvData >> node;
        nodes.push_back(node);
    }

    //recvData.ReadGuidBytes<0>(guid);

    Creature* npc = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_FLIGHTMASTER);
    if (!npc)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleActivateTaxiExpressOpcode - Unit (GUID: %u) not found or you can't interact with it.", uint32(guid.GetCounter()));
        return;
    }

    if (nodes.empty())
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ACTIVATE_TAXI_EXPRESS from %d to %d", nodes.front(), nodes.back());

    GetPlayer()->ActivateTaxiPathTo(nodes, npc);
}

//! 6.0.3
void WorldSession::HandleMoveSplineDoneOpcode(WorldPacket& recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_MOVE_SPLINE_DONE");
    recvData.rfinish();

    // in taxi flight packet received in 2 case:
    // 1) end taxi path in far (multi-node) flight
    // 2) switch from one map to other in case multim-map taxi path
    // we need process only (1)

    uint32 curDest = GetPlayer()->m_taxi.GetTaxiDestination();
    if (!curDest)
        return;

    TaxiNodesEntry const* curDestNode = sTaxiNodesStore.LookupEntry(curDest);

    // far teleport case
    if (curDestNode && curDestNode->MapID != GetPlayer()->GetMapId())
    {
        if (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
        {
            // short preparations to continue flight
            FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(GetPlayer()->GetMotionMaster()->top());

            flight->SetCurrentNodeAfterTeleport();
            TaxiPathNodeEntry const& node = flight->GetPath()[flight->GetCurrentNode()];
            flight->SkipCurrentNode();

            GetPlayer()->TeleportTo(curDestNode->MapID, node.Loc.X, node.Loc.Y, node.Loc.Z, GetPlayer()->GetOrientation());
        }
        return;
    }

    uint32 destinationnode = GetPlayer()->m_taxi.NextTaxiDestination();
    if (destinationnode > 0)                              // if more destinations to go
    {
        // current source node for next destination
        uint32 sourcenode = GetPlayer()->m_taxi.GetTaxiSource();

        // Add to taximask middle hubs in taxicheat mode (to prevent having player with disabled taxicheat and not having back flight path)
        if (GetPlayer()->isTaxiCheater())
        {
            if (GetPlayer()->m_taxi.SetTaximaskNode(sourcenode))
            {
                WorldPacket data(SMSG_NEW_TAXI_PATH, 0);
                _player->GetSession()->SendPacket(&data);
            }
        }

        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Taxi has to go from %u to %u", sourcenode, destinationnode);

        uint32 mountDisplayId = sObjectMgr->GetTaxiMountDisplayId(sourcenode, GetPlayer()->GetTeam());

        uint32 path, cost;
        sObjectMgr->GetTaxiPath(sourcenode, destinationnode, path, cost);

        if (path && mountDisplayId)
            SendDoFlight(mountDisplayId, path, 1);        // skip start fly node
        else
            GetPlayer()->m_taxi.ClearTaxiDestinations();    // clear problematic path and next
        return;
    }
    else
        GetPlayer()->m_taxi.ClearTaxiDestinations();        // not destinations, clear source node

    GetPlayer()->CleanupAfterTaxiFlight();
    GetPlayer()->SetFallInformation(0, GetPlayer()->GetPositionZ());
    if (GetPlayer()->pvpInfo.inHostileArea)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);
}

//! 6.0.3
void WorldSession::HandleActivateTaxiOpcode(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ACTIVATE_TAXI");

    ObjectGuid guid;
    std::vector<uint32> nodes;
    nodes.resize(2);

    recvData >> guid >> nodes[1];

    Creature* npc = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_FLIGHTMASTER);
    if (!npc)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleActivateTaxiOpcode - Unit (GUID: %u) not found or you can't interact with it.", uint32(guid.GetCounter()));
        return;
    }

    nodes[0] = sObjectMgr->GetNearestTaxiNode(npc->GetPositionX(), npc->GetPositionY(), npc->GetPositionZ(), npc->GetMapId(), GetPlayer()->GetTeam());
    if (!nodes[0])
        return;

    TaxiDestList::iterator list = sTaxiPathDestList[nodes[0]].find(nodes[1]);
    if (list == sTaxiPathDestList[nodes[0]].end())
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ACTIVATE_TAXI from %d to %d", nodes[0], nodes[1]);

    GetPlayer()->ActivateTaxiPathTo(sTaxiPathDestList[nodes[0]][nodes[1]], npc);
}

//! 6.0.3
void WorldSession::SendActivateTaxiReply(ActivateTaxiReply reply)
{
    WorldPacket data(SMSG_ACTIVATE_TAXI_REPLY, 4);
    data.WriteBits(reply, 4);
    SendPacket(&data);

    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_ACTIVATE_TAXI_REPLY");
}
