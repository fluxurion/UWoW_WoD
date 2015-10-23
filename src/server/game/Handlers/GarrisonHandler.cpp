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

#include "WorldSession.h"
#include "Garrison.h"
#include "GarrisonPackets.h"

void WorldSession::HandleGetGarrisonInfo(WorldPackets::Garrison::GetGarrisonInfo& /*packet*/)
{
    if (Garrison* garrison = _player->GetGarrison())
        garrison->SendInfo();
}

void WorldSession::HandleGarrisonPurchaseBuilding(WorldPackets::Garrison::GarrisonPurchaseBuilding& packet)
{
    if (!_player->GetNPCIfCanInteractWith(packet.NpcGUID, UNIT_NPC_FLAG_NONE, UNIT_NPC_FLAG2_GARRISON_ARCHITECT))
        return;

    if (Garrison* garrison = _player->GetGarrison())
        garrison->PlaceBuilding(packet.PlotInstanceID, packet.BuildingID);
}

void WorldSession::HandleGarrisonCancelConstruction(WorldPackets::Garrison::GarrisonCancelConstruction& packet)
{
    if (!_player->GetNPCIfCanInteractWith(packet.NpcGUID, UNIT_NPC_FLAG_NONE, UNIT_NPC_FLAG2_GARRISON_ARCHITECT))
        return;

    if (Garrison* garrison = _player->GetGarrison())
        garrison->CancelBuildingConstruction(packet.PlotInstanceID);
}

void WorldSession::HandleGarrisonRequestBlueprintAndSpecializationData(WorldPackets::Garrison::GarrisonRequestBlueprintAndSpecializationData& /*packet*/)
{
    if (Garrison* garrison = _player->GetGarrison())
        garrison->SendBlueprintAndSpecializationData();
}

void WorldSession::HandleGarrisonGetBuildingLandmarks(WorldPackets::Garrison::GarrisonGetBuildingLandmarks& /*packet*/)
{
    if (Garrison* garrison = _player->GetGarrison())
        garrison->SendBuildingLandmarks(_player);
}

void WorldSession::HandleGarrisonMissionBonusRoll(WorldPackets::Garrison::GarrisonMissionBonusRoll& /*packet*/)
{ }

void WorldSession::HandleGarrisonRequestLandingPageShipmentInfo(WorldPackets::Garrison::GarrisonRequestLandingPageShipmentInfo& /*packet*/)
{ }

void WorldSession::HandleGarrisonCheckUpgradeable(WorldPackets::Garrison::GarrisonCheckUpgradeable& /*packet*/)
{
    if (Garrison* garrison = _player->GetGarrison())
        garrison->SendGarrisonUpgradebleResult(_player);
}

void WorldSession::HandleGarrisonStartMission(WorldPackets::Garrison::GarrisonStartMission& packet)
{
    if (!_player->GetNPCIfCanInteractWith(packet.NpcGUID, UNIT_NPC_FLAG_NONE, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC))
        return;

    if (!sGarrMissionStore.LookupEntry(packet.MissionRecID))
        return;

    if (Garrison* garrison = _player->GetGarrison())
    {
        if (Garrison::Mission* mission = garrison->GetMissionByRecID(packet.MissionRecID))
            mission->Start(_player, packet.FollowerDBIDs);
    }
}

void WorldSession::HandleGarrisonCompleteMission(WorldPackets::Garrison::GarrisonCompleteMission& packet)
{
    if (!_player->GetNPCIfCanInteractWith(packet.NpcGUID, UNIT_NPC_FLAG_NONE, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC))
        return;

    if (!sGarrMissionStore.LookupEntry(packet.MissionRecID))
        return;

    /*if (Garrison* garrison = _player->GetGarrison())
    {
        if (Garrison::Mission* mission = garrison->GetMissionByRecID(packet.MissionRecID))
        {
            if (!mission->IsCompleted())
                return;

            mission->Complete();
        }
    }*/
}

void WorldSession::HandleCreateShipment(WorldPackets::Garrison::CreateShipment& /*packet*/)
{ }

void WorldSession::HandleGarrisonOpenMissionNpc(WorldPackets::Garrison::GarrisonOpenMissionNpc& packet)
{
    if (!_player->GetNPCIfCanInteractWith(packet.NpcGUID, UNIT_NPC_FLAG_NONE, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC))
        return;

    if (Garrison* garrison = _player->GetGarrison())
        garrison->OpenMissionNPC(_player);
}

void WorldSession::HandleCompleteAllReadyShipments(WorldPackets::Garrison::CompleteAllReadyShipments& /*packet*/)
{ }

void WorldSession::HandleUpgradeGarrison(WorldPackets::Garrison::UpgradeGarrison& packet)
{
    if (!_player->GetNPCIfCanInteractWith(packet.NpcGUID, UNIT_NPC_FLAG_NONE, UNIT_NPC_FLAG2_GARRISON_ARCHITECT))
        return;

    if (Garrison* garrison = _player->GetGarrison())
        garrison->Upgrade();
}

void WorldSession::HandleTrophyData(WorldPackets::Garrison::TrophyData& packet)
{
    switch (packet.GetOpcode())
    {
        case CMSG_REPLACE_TROPHY:
            break;
        case CMSG_CHANGE_MONUMENT_APPEARANCE:
            break;
        default:
            break;
    }
}

void WorldSession::HandleRevertTrophy(WorldPackets::Garrison::RevertTrophy& /*packet*/)
{ }

void WorldSession::HandleGetTrophyList(WorldPackets::Garrison::GetTrophyList& /*packet*/)
{ }

void WorldSession::HandleGarrisonSetFollowerInactive(WorldPackets::Garrison::GarrisonSetFollowerInactive& /*packet*/)
{ }

void WorldSession::HandleGarrisonRemoveFollowerFromBuilding(WorldPackets::Garrison::GarrisonRemoveFollowerFromBuilding& /*packet*/)
{ }

void WorldSession::HandleGarrisonAssignFollowerToBuilding(WorldPackets::Garrison::GarrisonAssignFollowerToBuilding& /*packet*/)
{ }
