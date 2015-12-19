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

#include "GarrisonPackets.h"

WorldPacket const* WorldPackets::Garrison::GarrisonCreateResult::Write()
{
    _worldPacket << uint32(Result);
    _worldPacket << uint32(GarrSiteLevelID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonDeleteResult::Write()
{
    _worldPacket << uint32(Result);
    _worldPacket << uint32(GarrSiteID);

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Garrison::GarrisonPlotInfo& plotInfo)
{
    data << uint32(plotInfo.GarrPlotInstanceID);
    data << plotInfo.PlotPos.PositionXYZOStream();
    data << uint32(plotInfo.PlotType);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Garrison::GarrisonBuildingInfo const& buildingInfo)
{
    data << uint32(buildingInfo.GarrPlotInstanceID);
    data << uint32(buildingInfo.GarrBuildingID);
    data << uint32(buildingInfo.TimeBuilt);
    data << uint32(buildingInfo.CurrentGarSpecID);
    data << uint32(buildingInfo.TimeSpecCooldown);
    data.WriteBit(buildingInfo.Active);
    data.FlushBits();

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Garrison::GarrisonFollower const& follower)
{
    data << uint64(follower.DbID);
    data << uint32(follower.GarrFollowerID);
    data << uint32(follower.Quality);
    data << uint32(follower.FollowerLevel);
    data << uint32(follower.ItemLevelWeapon);
    data << uint32(follower.ItemLevelArmor);
    data << uint32(follower.Xp);
    data << uint32(follower.CurrentBuildingID);
    data << uint32(follower.CurrentMissionID);
    data << static_cast<uint32>(follower.AbilityID.size());
    data << uint32(follower.FollowerStatus);
    for (GarrAbilityEntry const* ability : follower.AbilityID)
        data << uint32(ability->ID);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Garrison::GarrisonMission const& mission)
{
    data << uint64(mission.DbID);
    data << uint32(mission.MissionRecID);
    data << uint32(mission.OfferTime);
    data << uint32(mission.OfferDuration);
    data << uint32(mission.StartTime);
    data << uint32(mission.TravelDuration);
    data << uint32(mission.MissionDuration);
    data << uint32(mission.MissionState);

    return data;
}

WorldPacket const* WorldPackets::Garrison::GetGarrisonInfoResult::Write()
{
    _worldPacket.reserve(4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
        Buildings.size() * sizeof(GarrisonBuildingInfo) +
        Plots.size() * sizeof(GarrisonPlotInfo) +
        Followers.size() * (sizeof(GarrisonFollower) + 5 * 4) +
        Missions.size() * sizeof(GarrisonMission) +
        ArchivedMissions.size() * 4);

    _worldPacket << int32(GarrSiteID);
    _worldPacket << int32(GarrSiteLevelID);
    _worldPacket << int32(FactionIndex);
    _worldPacket << static_cast<uint32>(Buildings.size());
    _worldPacket << static_cast<uint32>(Plots.size());
    _worldPacket << static_cast<uint32>(Followers.size());
    _worldPacket << static_cast<uint32>(Missions.size());
    _worldPacket << static_cast<uint32>(ArchivedMissions.size());
    _worldPacket << int32(NumFollowerActivationsRemaining);

    for (GarrisonBuildingInfo const* building : Buildings)
        _worldPacket << *building;

    for (GarrisonPlotInfo* plot : Plots)
        _worldPacket << *plot;

    for (GarrisonFollower const* follower : Followers)
        _worldPacket << *follower;

    for (GarrisonMission const* mission : Missions)
        _worldPacket << *mission;

    if (!ArchivedMissions.empty())
        _worldPacket.append(ArchivedMissions.data(), ArchivedMissions.size());

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Garrison::GarrisonRemoteBuildingInfo const& building)
{
    data << uint32(building.GarrPlotInstanceID);
    data << uint32(building.GarrBuildingID);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Garrison::LandingPageData const& page)
{
    data << uint32(page.MissionRecID);
    data << uint32(page.FollowerDBID);
    data << uint32(page.Unk1);
    data << uint32(page.Unk2);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Garrison::Shipment const& shipment)
{
    data << shipment.ShipmentRecID;
    data << shipment.ShipmentID;
    data.AppendPackedTime(shipment.CreationTime);
    data << shipment.ShipmentDuration;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Garrison::GarrisonRemoteSiteInfo const& site)
{
    data << uint32(site.GarrSiteLevelID);
    data << static_cast<uint32>(site.Buildings.size());
    for (WorldPackets::Garrison::GarrisonRemoteBuildingInfo const& building : site.Buildings)
        data << building;

    return data;
}

WorldPacket const* WorldPackets::Garrison::GarrisonRemoteInfo::Write()
{
    _worldPacket << static_cast<uint32>(Sites.size());
    for (GarrisonRemoteSiteInfo const& site : Sites)
        _worldPacket << site;

    return &_worldPacket;
}

void WorldPackets::Garrison::GarrisonPurchaseBuilding::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> PlotInstanceID;
    _worldPacket >> BuildingID;
}

WorldPacket const* WorldPackets::Garrison::GarrisonPlaceBuildingResult::Write()
{
    _worldPacket << uint32(Result);
    _worldPacket << BuildingInfo;
    _worldPacket.WriteBit(PlayActivationCinematic);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

void WorldPackets::Garrison::GarrisonCancelConstruction::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> PlotInstanceID;
}

WorldPacket const* WorldPackets::Garrison::GarrisonBuildingRemoved::Write()
{
    _worldPacket << uint32(Result);
    _worldPacket << uint32(GarrPlotInstanceID);
    _worldPacket << uint32(GarrBuildingID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonLearnBlueprintResult::Write()
{
    _worldPacket << uint32(Result);
    _worldPacket << uint32(BuildingID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonUnlearnBlueprintResult::Write()
{
    _worldPacket << uint32(Result);
    _worldPacket << uint32(BuildingID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonRequestBlueprintAndSpecializationDataResult::Write()
{
    _worldPacket << static_cast<uint32>(BlueprintsKnown ? BlueprintsKnown->size() : 0);
    _worldPacket << static_cast<uint32>(SpecializationsKnown ? SpecializationsKnown->size() : 0);
    if (BlueprintsKnown)
        for (uint32 blueprint : *BlueprintsKnown)
            _worldPacket << uint32(blueprint);

    if (SpecializationsKnown)
        for (uint32 specialization : *SpecializationsKnown)
            _worldPacket << uint32(specialization);

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Garrison::GarrisonBuildingLandmark& landmark)
{
    data << uint32(landmark.GarrBuildingPlotInstID);
    data << landmark.Pos.PositionXYZStream();

    return data;
}

WorldPacket const* WorldPackets::Garrison::GarrisonBuildingLandmarks::Write()
{
    _worldPacket << static_cast<uint32>(Landmarks.size());
    for (GarrisonBuildingLandmark& landmark : Landmarks)
        _worldPacket << landmark;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonPlotPlaced::Write()
{
    _worldPacket << *PlotInfo;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonPlotRemoved::Write()
{
    _worldPacket << uint32(GarrPlotInstanceID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonAddFollowerResult::Write()
{
    _worldPacket << uint32(Result);
    _worldPacket << Follower;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonFollowerChangedItemLevel::Write()
{
    _worldPacket << Follower;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonBuildingActivated::Write()
{
    _worldPacket << uint32(GarrPlotInstanceID);

    return &_worldPacket;
}

void WorldPackets::Garrison::GarrisonMissionBonusRoll::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> MissionRecID;
}

WorldPacket const* WorldPackets::Garrison::GarrisonMissionBonusRollResult::Write()
{
    _worldPacket << MissionData;
    _worldPacket << MissionRecID;
    _worldPacket << Result;

    return &_worldPacket;
}

void WorldPackets::Garrison::GarrisonStartMission::Read()
{
    _worldPacket >> NpcGUID;
    uint32 followerCount;
    _worldPacket >> followerCount;
    _worldPacket >> MissionRecID;
    FollowerDBIDs.clear();
    for (uint8 i = 0; i < followerCount; ++i)
    {
        uint32 followerDbId;
        _worldPacket >> followerDbId;
        FollowerDBIDs.push_back(followerDbId);
    }
}

void WorldPackets::Garrison::GarrisonCompleteMission::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> MissionRecID;
}

WorldPacket const* WorldPackets::Garrison::GarrisonAssignFollowerToBuildingResult::Write()
{
    _worldPacket << FollowerDBID;
    _worldPacket << Result;
    _worldPacket << PlotInstanceID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonLandingPage::Write()
{
    _worldPacket << static_cast<uint32>(MsgData.size());
    for (auto const& map : MsgData)
        _worldPacket << map;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonAddMissionResult::Write()
{
    _worldPacket << Result;
    _worldPacket << unk; // 2 == GARRISON_RANDOM_MISSION_ADDED = 904
    _worldPacket << MissionData;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonUpgradeResult::Write()
{
    _worldPacket << Result;
    _worldPacket << GarrSiteLevelID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonStartMissionResult::Write()
{
    _worldPacket << Result;
    _worldPacket << MissionData;
    _worldPacket << static_cast<uint32>(FollowerDBIDs.size());
    for (auto const& map : FollowerDBIDs)
        _worldPacket << map;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonCompleteMissionResult::Write()
{
    _worldPacket << Result;
    _worldPacket << MissionData;
    _worldPacket << MissionRecID;
    _worldPacket.WriteBit(Succeeded);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonIsUpgradeableResult::Write()
{
    _worldPacket << Result;

    return &_worldPacket;
}

void WorldPackets::Garrison::CreateShipment::Read()
{
    _worldPacket >> NpcGUID;
}

void WorldPackets::Garrison::GarrisonRequestShipmentInfo::Read()
{
    _worldPacket >> NpcGUID;
}

void WorldPackets::Garrison::GarrisonOpenMissionNpc::Read()
{
    _worldPacket >> NpcGUID;
}

void WorldPackets::Garrison::CompleteAllReadyShipments::Read()
{
    _worldPacket >> GUID;
}

WorldPacket const* WorldPackets::Garrison::GetShipmentInfoResponse::Write()
{
    _worldPacket.WriteBit(Success);
    _worldPacket.FlushBits();

    _worldPacket << ShipmentID;
    _worldPacket << MaxShipments;

    _worldPacket << static_cast<uint32>(Shipments.size());
    for (auto const& map : Shipments)
        _worldPacket << map;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::CreateShipmentResponse::Write()
{
    _worldPacket << ShipmentID;
    _worldPacket << ShipmentRecID;
    _worldPacket << Result;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::OpenShipmentNPCFromGossip::Write()
{
    _worldPacket << NpcGUID;
    _worldPacket << CharShipmentContainerID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::SetupTrophy::Write()
{
    _worldPacket << uint32(Trophys.size());
    for (auto const& map : Trophys)
    {
        _worldPacket << map.Unk1;
        _worldPacket << map.Unk2;
    }

    return &_worldPacket;
}

void WorldPackets::Garrison::UpgradeGarrison::Read()
{
    _worldPacket >> NpcGUID;
}

void WorldPackets::Garrison::TrophyData::Read()
{
    _worldPacket >> TrophyGUID;
    _worldPacket >> NewTrophyID;
}

void WorldPackets::Garrison::RevertTrophy::Read()
{
    _worldPacket >> TrophyGUID;
}

void WorldPackets::Garrison::GetTrophyList::Read()
{
    _worldPacket >> TrophyTypeID;
}

void WorldPackets::Garrison::GarrisonSetFollowerInactive::Read()
{
    _worldPacket >> FollowerDBID;
    Inactive = _worldPacket.ReadBit();
}

void WorldPackets::Garrison::GarrisonRemoveFollowerFromBuilding::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> FollowerDBID;
}

void WorldPackets::Garrison::GarrisonAssignFollowerToBuilding::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> PlotInstanceID;
    _worldPacket >> FollowerDBID;
}

WorldPacket const* WorldPackets::Garrison::GetTrophyListResponse::Write()
{
    _worldPacket.WriteBit(Success);
    _worldPacket.FlushBits();

    _worldPacket << static_cast<uint32>(MsgData.size());
    for (auto const& map : MsgData)
    {
        _worldPacket << map.TrophyID;
        _worldPacket << map.Unk1;
        _worldPacket << map.Unk2;
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::ReplaceTrophyResponse::Write()
{
    _worldPacket.WriteBit(Success);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonOpenArchitect::Write()
{
    _worldPacket << NpcGUID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonOpenMissionNpcResponse::Write()
{
    _worldPacket << NpcGUID.WriteAsPacked();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonMissionListUpdate::Write()
{
    _worldPacket << Result;
    _worldPacket << updatedMissionCount;
    _worldPacket.WriteBit(!openMissionNpc);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonRemoveFollowerResult::Write()
{
    _worldPacket << FollowerDBID;
    _worldPacket << Result;
    _worldPacket << Destroyed;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonFollowerChangedXP::Write()
{
    _worldPacket << TotalXp;
    _worldPacket << Result;
    _worldPacket << Follower;
    _worldPacket << Follower2;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonNumFollowerActivationsRemaining::Write()
{
    _worldPacket << Amount;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonOpenTradeskillNpcResponse::Write()
{
    _worldPacket << GUID;
    _worldPacket << SpellID;

    _worldPacket << static_cast<uint32>(SkillLineIDs.size());
    _worldPacket << static_cast<uint32>(SkillRanks.size());
    _worldPacket << static_cast<uint32>(SkillMaxRanks.size());
    _worldPacket << static_cast<uint32>(KnownAbilitySpellIDs.size());

    for (int32 const& n : SkillLineIDs)
        _worldPacket << n;

    for (int32 const& b : SkillRanks)
        _worldPacket << b;

    for (int32 const& z : SkillMaxRanks)
        _worldPacket << z;

    for (int32 const& x : KnownAbilitySpellIDs)
        _worldPacket << x;

    _worldPacket << static_cast<uint32>(PlayerConditionIDs.size());
    for (int32 const& v : PlayerConditionIDs)
        _worldPacket << v;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonRemoveFollowerFromBuildingResult::Write()
{
    _worldPacket << FollowerDBID;
    _worldPacket << Result;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::QueryGarrisonCreatureNameResponse::Write()
{
    _worldPacket << InqueKey;
    _worldPacket << NpcGUID;
    _worldPacket.WriteBit(Name.is_initialized());
    _worldPacket.FlushBits();

    if (Name)
    {
        _worldPacket.WriteBits(Name->size(), 8);
        _worldPacket.WriteString(*Name);
    }

    return &_worldPacket;
}
