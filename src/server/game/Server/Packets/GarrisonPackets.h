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

#ifndef GarrisonPackets_h__
#define GarrisonPackets_h__

#include "Packet.h"
#include "ObjectGuid.h"
#include "PacketUtilities.h"
#include "DB2Structure.h"

namespace WorldPackets
{
    namespace Garrison
    {
        class GarrisonCreateResult final : public ServerPacket
        {
        public:
            GarrisonCreateResult() : ServerPacket(SMSG_GARRISON_CREATE_RESULT, 4 + 4) { }

            WorldPacket const* Write() override;

            uint32 GarrSiteLevelID = 0;
            uint32 Result = 0;
        };

        class GarrisonDeleteResult final : public ServerPacket
        {
        public:
            GarrisonDeleteResult() : ServerPacket(SMSG_GARRISON_DELETE_RESULT, 4 + 4) { }

            WorldPacket const* Write() override;

            uint32 Result = 0;
            uint32 GarrSiteID = 0;
        };

        class GetGarrisonInfo final : public ClientPacket
        {
        public:
            GetGarrisonInfo(WorldPacket&& packet) : ClientPacket(CMSG_GET_GARRISON_INFO, std::move(packet)) { }

            void Read() override { }
        };

        struct GarrisonPlotInfo
        {
            Position PlotPos;
            uint32 GarrPlotInstanceID = 0;
            uint32 PlotType = 0;
        };

        struct GarrisonBuildingInfo
        {
            uint32 GarrPlotInstanceID = 0;
            uint32 GarrBuildingID = 0;
            time_t TimeBuilt = time_t(0);
            uint32 CurrentGarSpecID = 0;
            time_t TimeSpecCooldown = time_t(2288912640);   // 06/07/1906 18:35:44 - another in the series of magic blizz dates
            bool Active = false;
        };

        struct GarrisonFollower
        {
            uint64 DbID = 0;
            uint32 GarrFollowerID = 0;
            uint32 Quality = 0;
            uint32 FollowerLevel = 0;
            uint32 ItemLevelWeapon = 0;
            uint32 ItemLevelArmor = 0;
            uint32 Xp = 0;
            uint32 CurrentBuildingID = 0;
            uint32 CurrentMissionID = 0;
            std::list<GarrAbilityEntry const*> AbilityID;
            uint32 FollowerStatus = 0;
        };

        struct GarrisonMission
        {
            uint64 DbID = 0;
            uint32 MissionRecID = 0;
            time_t OfferTime = time_t(0);
            uint32 OfferDuration = 0;
            time_t StartTime = time_t(2288912640);
            uint32 TravelDuration = 0;
            uint32 MissionDuration = 0;
            uint32 MissionState = 0;
        };

        struct LandingPageData
        {
            uint64 FollowerDBID = 0;
            uint32 MissionRecID = 0;
            uint32 Unk1 = 0;
            uint32 Unk2 = 0;
        };

        struct Shipment
        {
            uint64 ShipmentID = 0;
            uint32 ShipmentRecID = 0;
            time_t CreationTime = time(0);
            int32 ShipmentDuration = 0;
        };

        class GetGarrisonInfoResult final : public ServerPacket
        {
        public:
            GetGarrisonInfoResult() : ServerPacket(SMSG_GET_GARRISON_INFO_RESULT, 36) { }

            WorldPacket const* Write() override;

            uint32 GarrSiteID = 0;
            uint32 GarrSiteLevelID = 0;
            uint32 FactionIndex = 0;
            uint32 NumFollowerActivationsRemaining = 0;
            std::vector<GarrisonPlotInfo*> Plots;
            std::vector<GarrisonBuildingInfo const*> Buildings;
            std::vector<GarrisonFollower const*> Followers;
            std::vector<GarrisonMission const*> Missions;
            std::vector<int32> ArchivedMissions;
        };

        struct GarrisonRemoteBuildingInfo
        {
            GarrisonRemoteBuildingInfo() : GarrPlotInstanceID(0), GarrBuildingID(0) { }
            GarrisonRemoteBuildingInfo(uint32 plotInstanceId, uint32 buildingId) : GarrPlotInstanceID(plotInstanceId), GarrBuildingID(buildingId) { }

            uint32 GarrPlotInstanceID;
            uint32 GarrBuildingID;
        };

        struct GarrisonRemoteSiteInfo
        {
            uint32 GarrSiteLevelID = 0;
            std::vector<GarrisonRemoteBuildingInfo> Buildings;
        };

        class GarrisonRemoteInfo final : public ServerPacket
        {
        public:
            GarrisonRemoteInfo() : ServerPacket(SMSG_GARRISON_REMOTE_INFO, 4) { }

            WorldPacket const* Write() override;

            std::vector<GarrisonRemoteSiteInfo> Sites;
        };

        class GarrisonPurchaseBuilding final : public ClientPacket
        {
        public:
            GarrisonPurchaseBuilding(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_PURCHASE_BUILDING, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 BuildingID = 0;
            uint32 PlotInstanceID = 0;
        };

        class GarrisonPlaceBuildingResult final : public ServerPacket
        {
        public:
            GarrisonPlaceBuildingResult() : ServerPacket(SMSG_GARRISON_PLACE_BUILDING_RESULT, 4 + 21 + 1) { }

            WorldPacket const* Write() override;
            
            GarrisonBuildingInfo BuildingInfo;
            uint32 Result = 0;
            bool PlayActivationCinematic = false;
        };

        class GarrisonCancelConstruction final : public ClientPacket
        {
        public:
            GarrisonCancelConstruction(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_CANCEL_CONSTRUCTION, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 PlotInstanceID = 0;
        };

        class GarrisonBuildingRemoved final : public ServerPacket
        {
        public:
            GarrisonBuildingRemoved() : ServerPacket(SMSG_GARRISON_BUILDING_REMOVED, 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            uint32 Result = 0;
            uint32 GarrPlotInstanceID = 0;
            uint32 GarrBuildingID = 0;
        };

        class GarrisonLearnBlueprintResult final : public ServerPacket
        {
        public:
            GarrisonLearnBlueprintResult() : ServerPacket(SMSG_GARRISON_LEARN_BLUEPRINT_RESULT, 4 + 4) { }

            WorldPacket const* Write() override;

            uint32 BuildingID = 0;
            uint32 Result = 0;
        };

        class GarrisonUnlearnBlueprintResult final : public ServerPacket
        {
        public:
            GarrisonUnlearnBlueprintResult() : ServerPacket(SMSG_GARRISON_UNLEARN_BLUEPRINT_RESULT, 4 + 4) { }

            WorldPacket const* Write() override;

            uint32 BuildingID = 0;
            uint32 Result = 0;
        };

        class GarrisonRequestBlueprintAndSpecializationData final : public ClientPacket
        {
        public:
            GarrisonRequestBlueprintAndSpecializationData(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_REQUEST_BLUEPRINT_AND_SPECIALIZATION_DATA, std::move(packet)) { }

            void Read() override { }
        };

        class GarrisonRequestBlueprintAndSpecializationDataResult final : public ServerPacket
        {
        public:
            GarrisonRequestBlueprintAndSpecializationDataResult() : ServerPacket(SMSG_GARRISON_REQUEST_BLUEPRINT_AND_SPECIALIZATION_DATA_RESULT, 400) { }

            WorldPacket const* Write() override;

            std::unordered_set<uint32> const* SpecializationsKnown = nullptr;
            std::unordered_set<uint32> const* BlueprintsKnown = nullptr;
        };

        class GarrisonGetBuildingLandmarks final : public ClientPacket
        {
        public:
            GarrisonGetBuildingLandmarks(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_GET_BUILDING_LANDMARKS, std::move(packet)) { }

            void Read() override { }
        };

        struct GarrisonBuildingLandmark
        {
            GarrisonBuildingLandmark() : GarrBuildingPlotInstID(0), Pos() { }
            GarrisonBuildingLandmark(uint32 buildingPlotInstId, Position const& pos) : GarrBuildingPlotInstID(buildingPlotInstId), Pos(pos) { }
            
            Position Pos;
            uint32 GarrBuildingPlotInstID;
        };

        class GarrisonBuildingLandmarks final : public ServerPacket
        {
        public:
            GarrisonBuildingLandmarks() : ServerPacket(SMSG_GARRISON_BUILDING_LANDMARKS, 4) { }

            WorldPacket const* Write() override;

            std::vector<GarrisonBuildingLandmark> Landmarks;
        };

        class GarrisonPlotPlaced final : public ServerPacket
        {
        public:
            GarrisonPlotPlaced() : ServerPacket(SMSG_GARRISON_PLOT_PLACED, 20) { }

            WorldPacket const* Write() override;

            GarrisonPlotInfo* PlotInfo = nullptr;
        };

        class GarrisonPlotRemoved final : public ServerPacket
        {
        public:
            GarrisonPlotRemoved() : ServerPacket(SMSG_GARRISON_PLOT_REMOVED, 4) { }

            WorldPacket const* Write() override;

            uint32 GarrPlotInstanceID = 0;
        };

        class GarrisonAddFollowerResult final : public ServerPacket
        {
        public:
            GarrisonAddFollowerResult() : ServerPacket(SMSG_GARRISON_ADD_FOLLOWER_RESULT, 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 5 * 4 + 4) { }

            WorldPacket const* Write() override;

            GarrisonFollower Follower;
            uint32 Result = 0;
        };

        class GarrisonFollowerChangedItemLevel final : public ServerPacket
        {
        public:
            GarrisonFollowerChangedItemLevel() : ServerPacket(SMSG_GARRISON_FOLLOWER_CHANGED_ITEM_LEVEL, 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 5 * 4) { }

            WorldPacket const* Write() override;

            GarrisonFollower Follower;
        };

        class GarrisonBuildingActivated final : public ServerPacket
        {
        public:
            GarrisonBuildingActivated() : ServerPacket(SMSG_GARRISON_BUILDING_ACTIVATED, 4) { }

            WorldPacket const* Write() override;

            uint32 GarrPlotInstanceID = 0;
        };

        class GarrisonRequestLandingPageShipmentInfo final : public ClientPacket
        {
        public:
            GarrisonRequestLandingPageShipmentInfo(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_REQUEST_LANDING_PAGE_SHIPMENT_INFO, std::move(packet)) { }

            void Read() override { }
        };

        class GarrisonCheckUpgradeable final : public ClientPacket
        {
        public:
            GarrisonCheckUpgradeable(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_CHECK_UPGRADEABLE, std::move(packet)) { }

            void Read() override { }
        };

        class GarrisonMissionBonusRoll final : public ClientPacket
        {
        public:
            GarrisonMissionBonusRoll(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_MISSION_BONUS_ROLL, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 MissionRecID = 0;
        };

        class GarrisonMissionBonusRollResult final : public ServerPacket
        {
        public:
            GarrisonMissionBonusRollResult() : ServerPacket(SMSG_GARRISON_MISSION_BONUS_ROLL_RESULT, 36 + 4 + 4) { }

            WorldPacket const* Write() override;

            GarrisonMission MissionData;
            uint32 MissionRecID = 0;
            uint32 Result = 0;
        };

        class GarrisonStartMission final : public ClientPacket
        {
        public:
            GarrisonStartMission(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_START_MISSION, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            std::vector<uint64> FollowerDBIDs;
            uint32 MissionRecID = 0;
        };

        class GarrisonCompleteMission final : public ClientPacket
        {
        public:
            GarrisonCompleteMission(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_COMPLETE_MISSION, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 MissionRecID = 0;
        };

        class GarrisonAssignFollowerToBuildingResult final : public ServerPacket
        {
        public:
            GarrisonAssignFollowerToBuildingResult() : ServerPacket(SMSG_GARRISON_ASSIGN_FOLLOWER_TO_BUILDING_RESULT, 8 + 4 + 4) { }

            WorldPacket const* Write() override;

            uint64 FollowerDBID = 0;
            int32 Result = 0;
            int32 PlotInstanceID = 0;
        };

        class GarrisonLandingPage final : public ServerPacket
        {
        public:
            GarrisonLandingPage() : ServerPacket(SMSG_GARRISON_LANDINGPAGE_SHIPMENTS, 4) { }

            WorldPacket const* Write() override;

            std::vector<LandingPageData> MsgData;
        };

        class GarrisonAddMissionResult final : public ServerPacket
        {
        public:
            GarrisonAddMissionResult() : ServerPacket(SMSG_GARRISON_ADD_MISSION_RESULT, 36 + 4) { }

            WorldPacket const* Write() override;

            GarrisonMission MissionData;
            uint32 Result = 0;
        };

        class GarrisonUpgradeResult final : public ServerPacket
        {
        public:
            GarrisonUpgradeResult() : ServerPacket(SMSG_GARRISON_UPGRADE_RESULT, 4 + 4) { }

            WorldPacket const* Write() override;

            uint32 GarrSiteLevelID = 0;
            uint32 Result = 0;
        };

        class GarrisonStartMissionResult final : public ServerPacket
        {
        public:
            GarrisonStartMissionResult() : ServerPacket(SMSG_GARRISON_START_MISSION_RESULT, 36 + 4 + 4) { }

            WorldPacket const* Write() override;
            
            GarrisonMission MissionData;
            std::vector<uint64> FollowerDBIDs;
            uint32 Result = 0;
        };

        class GarrisonCompleteMissionResult final : public ServerPacket
        {
        public:
            GarrisonCompleteMissionResult() : ServerPacket(SMSG_GARRISON_COMPLETE_MISSION_RESULT, 36 + 4 + 4 + 1) { }

            WorldPacket const* Write() override;
            
            GarrisonMission MissionData;
            uint32 Result = 0;
            uint32 MissionRecID = 0;
            bool Succeeded;
        };

        class GarrisonIsUpgradeableResult final : public ServerPacket
        {
        public:
            GarrisonIsUpgradeableResult() : ServerPacket(SMSG_GARRISON_IS_UPGRADEABLE_RESULT, 4) { }

            WorldPacket const* Write() override;

            uint32 Result = 0;
        };

        class CreateShipment final : public ClientPacket
        {
        public:
            CreateShipment(WorldPacket&& packet) : ClientPacket(CMSG_CREATE_SHIPMENT, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
        };

        class GarrisonOpenMissionNpc final : public ClientPacket
        {
        public:
            GarrisonOpenMissionNpc(WorldPacket&& packet) : ClientPacket(CMSG_OPEN_MISSION_NPC, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
        };

        class CompleteAllReadyShipments final : public ClientPacket
        {
        public:
            CompleteAllReadyShipments(WorldPacket&& packet) : ClientPacket(CMSG_COMPLETE_ALL_READY_SHIPMENTS, std::move(packet)) { }

            void Read() override;

            ObjectGuid GUID;
        };

        class GetShipmentInfoResponse final : public ServerPacket
        {
        public:
            GetShipmentInfoResponse() : ServerPacket(SMSG_GET_SHIPMENT_INFO_RESPONSE, 1 + 4 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;
            
            std::vector<Shipment> Shipments;
            uint32 ShipmentID = 0;
            uint32 MaxShipments = 0;
            uint32 PlotInstanceID = 0;
            bool Success = false;
        };

        class CreateShipmentResponse final : public ServerPacket
        {
        public:
            CreateShipmentResponse() : ServerPacket(SMSG_CREATE_SHIPMENT_RESPONSE, 8 + 4 + 4) { }

            WorldPacket const* Write() override;

            uint64 ShipmentID = 0;
            uint32 ShipmentRecID = 0;
            uint32 Result = 0;
        };

        class OpenShipmentNPCFromGossip final : public ServerPacket
        {
        public:
            OpenShipmentNPCFromGossip() : ServerPacket(SMSG_OPEN_SHIPMENT_NPC_FROM_GOSSIP, 20) { }

            WorldPacket const* Write() override;

            ObjectGuid NpcGUID;
            uint32 CharShipmentContainerID = 0;
        };

        class SetupTrophy final : public ServerPacket
        {
        public:
            SetupTrophy() : ServerPacket(SMSG_GET_DISPLAYED_TROPHY_LIST_RESPONSE, 4) { }

            WorldPacket const* Write() override;

            struct TrophyData
            {
                uint32 Unk1 = 0;
                uint32 Unk2 = 0;
            };

            std::vector<TrophyData> Trophys;
        };

        class UpgradeGarrison final : public ClientPacket
        {
        public:
            UpgradeGarrison(WorldPacket&& packet) : ClientPacket(CMSG_UPGRADE_GARRISON, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
        };

        //< CMSG_REPLACE_TROPHY
        //< CMSG_CHANGE_MONUMENT_APPEARANCE
        class TrophyData final : public ClientPacket
        {
        public:
            TrophyData(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

            void Read() override;

            ObjectGuid TrophyGUID;
            uint32 NewTrophyID = 0;
        };

        class RevertTrophy final : public ClientPacket
        {
        public:
            RevertTrophy(WorldPacket&& packet) : ClientPacket(CMSG_REVERT_MONUMENT_APPEARANCE, std::move(packet)) { }

            void Read() override;

            ObjectGuid TrophyGUID;
        };

        class GetTrophyList final : public ClientPacket
        {
        public:
            GetTrophyList(WorldPacket&& packet) : ClientPacket(CMSG_GET_TROPHY_LIST, std::move(packet)) { }

            void Read() override;

            uint32 TrophyTypeID = 0;
        };

        class GarrisonSetFollowerInactive final : public ClientPacket
        {
        public:
            GarrisonSetFollowerInactive(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_SET_FOLLOWER_INACTIVE, std::move(packet)) { }

            void Read() override;

            uint64 FollowerDBID = 0;
            bool Inactive = false;
        };

        class GarrisonRemoveFollowerFromBuilding final : public ClientPacket
        {
        public:
            GarrisonRemoveFollowerFromBuilding(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_REMOVE_FOLLOWER_FROM_BUILDING, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint64 FollowerDBID = 0;
        };

        class GarrisonAssignFollowerToBuilding final : public ClientPacket
        {
        public:
            GarrisonAssignFollowerToBuilding(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_ASSIGN_FOLLOWER_TO_BUILDING, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint64 FollowerDBID = 0;
            uint32 PlotInstanceID = 0;
        };

        class GetTrophyListResponse final : public ServerPacket
        {
        public:
            GetTrophyListResponse() : ServerPacket(SMSG_GET_TROPHY_LIST_RESPONSE, 1 + 4) { }

            WorldPacket const* Write() override;

            struct TrophyData
            {
                uint32 TrophyID = 0;
                uint32 Unk1 = 0;
                uint32 Unk2 = 0;
            };

            std::vector<TrophyData> MsgData;
            bool Success = false;
        };

        class ReplaceTrophyResponse final : public ServerPacket
        {
        public:
            ReplaceTrophyResponse() : ServerPacket(SMSG_REPLACE_TROPHY_RESPONSE, 1) { }

            WorldPacket const* Write() override;

            bool Success = false;
        };

        class GarrisonOpenArchitect final : public ServerPacket
        {
        public:
            GarrisonOpenArchitect() : ServerPacket(SMSG_GARRISON_OPEN_ARCHITECT, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid NpcGUID;
        };

        class GarrisonOpenMissionNpcResponse final : public ServerPacket
        {
        public:
            GarrisonOpenMissionNpcResponse() : ServerPacket(SMSG_GARRISON_OPEN_MISSION_NPC, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid NpcGUID;
        };

        class GarrisonRemoveFollowerResult final : public ServerPacket
        {
        public:
            GarrisonRemoveFollowerResult() : ServerPacket(SMSG_GARRISON_REMOVE_FOLLOWER_RESULT, 8 + 4 + 4) { }

            WorldPacket const* Write() override;

            uint64 FollowerDBID = 0;
            int32 Result = 0;
            int32 Destroyed = 0;
        };

        class GarrisonFollowerChangedXP final : public ServerPacket
        {
        public:
            GarrisonFollowerChangedXP() : ServerPacket(SMSG_GARRISON_FOLLOWER_CHANGED_XP) { }

            WorldPacket const* Write() override;
            
            GarrisonFollower Follower;
            GarrisonFollower Follower2;
            int32 Result = 0;
            uint32 Xp = 0;
        };

        class GarrisonNumFollowerActivationsRemaining final : public ServerPacket
        {
        public:
            GarrisonNumFollowerActivationsRemaining() : ServerPacket(SMSG_GARRISON_NUM_FOLLOWER_ACTIVATIONS_REMAINING, 4) { }

            WorldPacket const* Write() override;

            int32 Amount = 0;
        };

        class GarrisonOpenTradeskillNpcResponse final : public ServerPacket
        {
        public:
            GarrisonOpenTradeskillNpcResponse() : ServerPacket(SMSG_GARRISON_OPEN_TRADESKILL_NPC_RESPONSE, 16 + 24) { }

            WorldPacket const* Write() override;

            ObjectGuid GUID;
            int32 SpellID = 0;
            std::vector<int32> SkillLineIDs;
            std::vector<int32> SkillRanks;
            std::vector<int32> SkillMaxRanks;
            std::vector<int32> KnownAbilitySpellIDs;
            std::vector<int32> PlayerConditionIDs;
        };

        class GarrisonRemoveFollowerFromBuildingResult final : public ServerPacket
        {
        public:
            GarrisonRemoveFollowerFromBuildingResult() : ServerPacket(SMSG_GARRISON_REMOVE_FOLLOWER_FROM_BUILDING_RESULT, 12) { }

            WorldPacket const* Write() override;

            uint64 FollowerDBID = 0;
            int32 Result = 0;
        };

        class QueryGarrisonCreatureNameResponse final : public ServerPacket
        {
        public:
            QueryGarrisonCreatureNameResponse() : ServerPacket(SMSG_QUERY_GARRISON_CREATURE_NAME_RESPONSE, 16 + 8 + 1) { }

            WorldPacket const* Write() override;

            ObjectGuid NpcGUID;
            uint64 InqueKey = 0;
            Optional<std::string> Name;
        };

        class GarrisonMissionListUpdate final : public ServerPacket
        {
        public:
            GarrisonMissionListUpdate() : ServerPacket(SMSG_GARRISON_MISSION_LIST_UPDATE, 4 + 4 + 1) { }

            WorldPacket const* Write() override;

            uint32 Result = 0;
            uint32 updatedMissionCount = 0;
            std::vector<uint32> updatedMissionRecIDs;
            bool openMissionNpc;
        };
    }
}

#endif // GarrisonPackets_h__
