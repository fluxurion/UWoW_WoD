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

#ifndef Garrison_h__
#define Garrison_h__

#include "Player.h"
#include "Packets/GarrisonPackets.h"

enum GarrisonFactionIndex
{
    GARRISON_FACTION_INDEX_HORDE    = 0,
    GARRISON_FACTION_INDEX_ALLIANCE = 1
};

enum GarrisonBuildingFlags
{
    GARRISON_BUILDING_FLAG_NEEDS_PLAN   = 0x1
};

enum GarrisonFollowerFlags
{
    GARRISON_FOLLOWER_FLAG_UNIQUE   = 0x1
};

enum GarrisonAbilityFlags
{
    GARRISON_ABILITY_FLAG_TRAIT         = 0x01,
    GARRISON_ABILITY_CANNOT_ROLL        = 0x02,
    GARRISON_ABILITY_HORDE_ONLY         = 0x04,
    GARRISON_ABILITY_ALLIANCE_ONLY      = 0x08,
    GARRISON_ABILITY_FLAG_CANNOT_REMOVE = 0x10,
    GARRISON_ABILITY_FLAG_EXCLUSIVE     = 0x20
};

enum GarrisonError
{
    GARRISON_SUCCESS                        = 0,
    GARRISON_ERROR_INVALID_PLOT             = 1,
    GARRISON_ERROR_INVALID_BUILDINGID       = 2,
    GARRISON_ERROR_INVALID_PLOT_BUILDING    = 7,
    GARRISON_ERROR_NO_BUILDING              = 8,
    GARRISON_ERROR_MAX_LEVEL                = 12,
    GARRISON_ERROR_SPECIALIZATION_KNOWN     = 19,
    GARRISON_ERROR_BLUEPRINT_KNOWN          = 21,
    GARRISON_ERROR_BLUEPRINT_NOT_KNOWN      = 22,
    GARRISON_ERROR_BUILDING_EXISTS          = 24,
    GARRISON_ERROR_NOT_ENOUGH_CURRENCY      = 46,
    GARRISON_ERROR_NOT_ENOUGH_GOLD          = 47,
    GARRISON_ERROR_NOT_ALLOW_GARRISON_UPGRADE = 68,
    GARRISON_GENERIC_UNKNOWN_ERROR          = 255   // custom value for packets whose handlers only check if error != 0
};

enum GarrisonFollowerStatus
{
    FOLLOWER_STATUS_BASE        = 0x00,
    FOLLOWER_STATUS_FAVORITE    = 0x01,
    FOLLOWER_STATUS_EXHAUSTED   = 0x02,
    FOLLOWER_STATUS_INACTIVE    = 0x04
};

enum MissionState
{
    MISSION_STATE_AVAILABLE     = 0,
    MISSION_STATE_IN_PROGRESS   = 1,
    MISSION_STATE_WAITING_BONUS = 2,
    MISSION_STATE_COMPLETED     = 5
};

enum FollowerQuality
{
    FOLLOWER_QUALITY_UNCOMMON  = 2,
    FOLLOWER_QUALITY_RARE      = 3,
    FOLLOWER_QUALITY_EPIC      = 4,
    FOLLOWER_QUALITY_LEGENDARY = 5
};

class GameObject;
class Map;

class Garrison
{
public:
    struct Building
    {
        bool CanActivate() const;

        ObjectGuid Guid;
        GuidUnorderedSet Spawns;
        Optional<WorldPackets::Garrison::GarrisonBuildingInfo> PacketInfo;
    };

    struct Plot
    {
        GameObject* CreateGameObject(Map* map, GarrisonFactionIndex faction);
        void DeleteGameObject(Map* map);
        void ClearBuildingInfo(Player* owner);
        void SetBuildingInfo(WorldPackets::Garrison::GarrisonBuildingInfo const& buildingInfo, Player* owner);

        WorldPackets::Garrison::GarrisonPlotInfo PacketInfo;
        uint32 EmptyGameObjectId = 0;
        uint32 GarrSiteLevelPlotInstId = 0;
        Building BuildingInfo;

        bool buildingActivationWaiting = false;
    };

    struct Follower
    {
        WorldPackets::Garrison::GarrisonFollower PacketInfo;

        uint32 GetItemLevel() const;
        void IncreaseFollowerItemLevel(SpellInfo const* spellInfo, Player* caster);
        uint8 RollQuality(uint32 baseQuality);
        void GiveLevel(uint32 level) { PacketInfo.FollowerLevel = level; }
        void GiveQuality(uint32 quality) { PacketInfo.Quality = quality; }
        void GiveXP(uint32 xp);
        uint32 GetXPForNextUpgrade();
    };

    struct Mission
    {
        WorldPackets::Garrison::GarrisonMission PacketInfo;
        std::list<uint64> CurrentFollowerDBIDs;

        void Start(Player* owner);
        bool CanStart(Player* owner);
        void BonusRoll(Player* onwer);
        bool CanBonusRoll();
        void Complete(Player* owner);
        bool HasBonusRoll() { float chance = ComputeSuccessChance(); return roll_chance_f(chance); }
        float ComputeSuccessChance();
        float CalcChance(float a, float b, float c);
        uint32 GetDuration(Player* owner);
    };

    explicit Garrison(Player* owner);

    bool LoadFromDB(PreparedQueryResult garrison, PreparedQueryResult blueprints, PreparedQueryResult buildings,
        PreparedQueryResult followers, PreparedQueryResult abilities, PreparedQueryResult missions);
    void SaveToDB(SQLTransaction trans);
    static void DeleteFromDB(ObjectGuid::LowType ownerGuid, SQLTransaction trans);

    bool Create(uint32 garrSiteId);
    void Delete();
    void Upgrade();

    void Enter() const;
    void Leave() const;

    void Update(uint32 diff);

    GarrisonFactionIndex GetFaction() const;

    // Plots
    std::vector<Plot*> GetPlots();
    Plot* GetPlot(uint32 garrPlotInstanceId);
    Plot const* GetPlot(uint32 garrPlotInstanceId) const;

    // Buildings
    bool LearnBlueprint(uint32 garrBuildingId);
    void UnlearnBlueprint(uint32 garrBuildingId);
    void PlaceBuilding(uint32 garrPlotInstanceId, uint32 garrBuildingId);
    void CancelBuildingConstruction(uint32 garrPlotInstanceId);
    void ActivateBuilding(uint32 garrPlotInstanceId);
    uint32 GetCountOfBluePrints() const { return _knownBuildings.size();  }
    uint32 GetCountOFollowers() const { return _followers.size(); }
    
    // Followers
    void AddFollower(uint32 garrFollowerId);
    Follower const* GetFollower(uint64 dbId) const;
    Follower* GetFollower(uint64 dbId);
    Follower* GetFollowerByID(uint32 entry);
    void ReTrainFollower(SpellInfo const* spellInfo, uint32 followerID);

    // Missions
    void AddMission(uint32 missionRecID);
    Mission const* GetMission(uint64 dbId) const;
    Mission* GetMissionByRecID(uint32 missionRecID);
    void RewardMission(uint32 missionRecID);

    void SendInfo();
    void SendRemoteInfo() const;
    void SendBlueprintAndSpecializationData();
    void SendBuildingLandmarks(Player* receiver) const;
    void SendGarrisonUpgradebleResult(Player* receiver) const;
    void SendMissionListUpdate(bool openMissionNpc) const;

    void ResetFollowerActivationLimit() { _followerActivationsRemainingToday = 1; }

    // Map
    int32 GetGarrisonMapID() const { return _siteLevel ? _siteLevel->MapID : -1; }
    uint8 GetGarrisonLevel() const { return _siteLevel ? _siteLevel->Level : 0; }
    static uint32 GetAreaIdForTeam(uint32 team);

    //  Ressource
    uint32 GetResNumber() const;
    void UpdateResTakenTime();

private:
    Map* FindMap() const;
    void InitializePlots();
    GarrisonError CheckBuildingPlacement(uint32 garrPlotInstanceId, uint32 garrBuildingId) const;
    GarrisonError CheckBuildingRemoval(uint32 garrPlotInstanceId) const;
    Player* _owner;
    GarrSiteLevelEntry const* _siteLevel;
    uint32 _followerActivationsRemainingToday;
    uint32 _lastResTaken;

    std::unordered_map<uint32 /*garrPlotInstanceId*/, Plot> _plots;
    std::unordered_set<uint32 /*garrBuildingId*/> _knownBuildings;
    std::unordered_map<uint64 /*dbId*/, Follower> _followers;
    std::unordered_set<uint32> _followerIds;
    std::unordered_map<uint64 /*dbId*/, Mission> _missions;
    std::unordered_set<uint32> _missionIds;
};

#endif // Garrison_h__
