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

#include "Garrison.h"
#include "Creature.h"
#include "GameObject.h"
#include "GarrisonMgr.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "VehicleDefines.h"
#include "MiscPackets.h"
#include "InstanceScript.h"
#include "LootPackets.h"

uint32 getSiteLevelIdById(uint32 team, uint8 lvl)
{
    switch (lvl)
    {
        case 1:
            return team == ALLIANCE ? 5 : 258;
        case 2:
            return team == ALLIANCE ? 444 : 445;
        case 3:
            return team == ALLIANCE ? 6 : 259;
    }

    ASSERT(false);
    return 0;
}

Garrison::Garrison(Player* owner) : _owner(owner), _siteLevel(nullptr), _followerActivationsRemainingToday(1), _lastResTaken(0)
{ 
    updateTimer.SetInterval(5 * IN_MILLISECONDS);
    updateTimer.Reset();
}

bool Garrison::LoadFromDB(PreparedQueryResult garrison, PreparedQueryResult blueprints, PreparedQueryResult buildings,
    PreparedQueryResult followers, PreparedQueryResult abilities, PreparedQueryResult missions, PreparedQueryResult shipments)
{
    if (!garrison)
        return false;

    Field* fields = garrison->Fetch();
    _siteLevel = sGarrSiteLevelStore.LookupEntry(fields[0].GetUInt32());
    _followerActivationsRemainingToday = fields[1].GetUInt32();
    _lastResTaken = fields[2].GetUInt32();

    if (!_siteLevel)
        return false;

    if (_siteLevel->SiteID == SITE_ID_GARRISON_ALLIANCE && _owner->GetTeam() == HORDE ||
        _siteLevel->SiteID == SITE_ID_GARRISON_HORDE && _owner->GetTeam() == ALLIANCE)
    {
        _siteLevel = sGarrSiteLevelStore.LookupEntry(getSiteLevelIdById(_owner->GetTeam(), _siteLevel->Level));

        if (!_siteLevel)
            return false;
    }

    InitializePlots();

    if (blueprints)
    {
        do
        {
            fields = blueprints->Fetch();
            if (GarrBuildingEntry const* building = sGarrBuildingStore.LookupEntry(fields[0].GetUInt32()))
                _knownBuildings.insert(building->ID);

        }
        while (blueprints->NextRow());
    }

    if (buildings)
    {
        do
        {
            fields = buildings->Fetch();
            uint32 plotInstanceId = fields[0].GetUInt32();
            uint32 buildingId = fields[1].GetUInt32();
            time_t timeBuilt = time_t(fields[2].GetUInt64());
            bool active = fields[3].GetBool();

            Plot* plot = GetPlot(plotInstanceId);
            if (!plot)
                continue;

            if (!sGarrBuildingStore.LookupEntry(buildingId))
                continue;

            plot->BuildingInfo.PacketInfo = boost::in_place();
            plot->BuildingInfo.PacketInfo->GarrPlotInstanceID = plotInstanceId;
            plot->BuildingInfo.PacketInfo->GarrBuildingID = buildingId;
            plot->BuildingInfo.PacketInfo->TimeBuilt = timeBuilt;
            plot->BuildingInfo.PacketInfo->Active = active;

            if (!plot->BuildingInfo.PacketInfo->Active)
                plot->buildingActivationWaiting = true;
        }
        while (buildings->NextRow());
    }

    //           0             1          2              3          4               5                6             7 
    // SELECT dbId, missionRecID, offerTime, offerDuration, startTime, travelDuration, missionDuration, missionState FROM character_garrison_missions WHERE guid = ?
    if (missions)
    {
        do
        {
            Field* fields = missions->Fetch();

            uint64 dbId = fields[0].GetUInt64();
            uint32 missionRecID = fields[1].GetUInt32();
            uint32 offerTime = fields[2].GetUInt32();
            uint32 offerDuration = fields[3].GetUInt32();

            if (!sGarrMissionStore.LookupEntry(missionRecID))
                continue;

            if (offerTime + offerDuration <= time(nullptr))
                continue;

            _missionIds.insert(missionRecID);
            Mission& mission = _missions[dbId];
            mission.PacketInfo.DbID = dbId;
            mission.PacketInfo.MissionRecID = missionRecID;
            mission.PacketInfo.OfferTime = offerTime;
            mission.PacketInfo.OfferDuration = offerDuration;
            mission.PacketInfo.StartTime = fields[4].GetUInt32();
            mission.PacketInfo.TravelDuration = fields[5].GetUInt32();
            mission.PacketInfo.MissionDuration = fields[6].GetUInt32();
            mission.PacketInfo.MissionState = fields[7].GetUInt32();
        } while (missions->NextRow());
    }

    //           0           1        2      3                4               5   6                7               8       9
    // SELECT dbId, followerId, quality, level, itemLevelWeapon, itemLevelArmor, xp, currentBuilding, currentMission, status FROM character_garrison_followers WHERE guid = ?
    if (followers)
    {
        do
        {
            Field* fields = followers->Fetch();

            uint64 dbId = fields[0].GetUInt64();
            uint32 followerId = fields[1].GetUInt32();
            if (!sGarrFollowerStore.LookupEntry(followerId))
                continue;

            _followerIds.insert(followerId);
            Follower& follower = _followers[dbId];
            follower.PacketInfo.DbID = dbId;
            follower.PacketInfo.GarrFollowerID = followerId;
            follower.PacketInfo.Quality = fields[2].GetUInt32();
            follower.PacketInfo.FollowerLevel = fields[3].GetUInt32();
            follower.PacketInfo.ItemLevelWeapon = fields[4].GetUInt32();
            follower.PacketInfo.ItemLevelArmor = fields[5].GetUInt32();
            follower.PacketInfo.Xp = fields[6].GetUInt32();
            follower.PacketInfo.CurrentBuildingID = fields[7].GetUInt32();
            follower.PacketInfo.CurrentMissionID = fields[8].GetUInt32();
            follower.PacketInfo.FollowerStatus = fields[9].GetUInt32();
            if (!sGarrBuildingStore.LookupEntry(follower.PacketInfo.CurrentBuildingID))
                follower.PacketInfo.CurrentBuildingID = 0;

            if (follower.PacketInfo.CurrentMissionID)
            {
                if (Garrison::Mission* mission = GetMissionByRecID(follower.PacketInfo.CurrentMissionID))
                    mission->CurrentFollowerDBIDs.push_back(follower.PacketInfo.DbID);
                else
                    follower.PacketInfo.CurrentMissionID = 0;
            }
            //if (!sGarrMissionStore.LookupEntry(follower.PacketInfo.CurrentMissionID))
            //    follower.PacketInfo.CurrentMissionID = 0;

        }
        while (followers->NextRow());

        if (abilities)
        {
            do
            {
                Field* fields = abilities->Fetch();
                uint64 dbId = fields[0].GetUInt64();
                GarrAbilityEntry const* ability = sGarrAbilityStore.LookupEntry(fields[1].GetUInt32());

                if (!ability)
                    continue;

                auto itr = _followers.find(dbId);
                if (itr == _followers.end())
                    continue;

                itr->second.PacketInfo.AbilityID.push_back(ability);
            }
            while (abilities->NextRow());
        }
    }

    //SELECT dbId, shipmentID, orderTime FROM character_garrison_shipment WHERE guid = ? ORDER BY `orderTime` ASC
    if (shipments)
    {
        do
        {
            Field* fields = shipments->Fetch();

            uint64 dbId = fields[0].GetUInt64();

            uint32 shipmentID = fields[1].GetUInt16();
            uint32 orderTime = fields[2].GetUInt32();
            PlaceShipment(shipmentID, orderTime, dbId);

        } while (shipments->NextRow());
    }
    return true;
}

void Garrison::SaveToDB(SQLTransaction trans)
{
    DeleteFromDB(_owner->GetGUIDLow(), trans);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_GARRISON);
    stmt->setUInt64(0, _owner->GetGUIDLow());
    stmt->setUInt32(1, _siteLevel->ID);
    stmt->setUInt32(2, _followerActivationsRemainingToday);
    stmt->setUInt32(3, _lastResTaken);
    
    trans->Append(stmt);

    for (uint32 building : _knownBuildings)
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_GARRISON_BLUEPRINTS);
        stmt->setUInt64(0, _owner->GetGUIDLow());
        stmt->setUInt32(1, building);
        trans->Append(stmt);
    }

    for (auto const& p : _plots)
    {
        Plot const& plot = p.second;
        if (plot.BuildingInfo.PacketInfo)
        {
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_GARRISON_BUILDINGS);
            stmt->setUInt64(0, _owner->GetGUIDLow());
            stmt->setUInt32(1, plot.BuildingInfo.PacketInfo->GarrPlotInstanceID);
            stmt->setUInt32(2, plot.BuildingInfo.PacketInfo->GarrBuildingID);
            stmt->setUInt64(3, plot.BuildingInfo.PacketInfo->TimeBuilt);
            stmt->setBool(4, plot.BuildingInfo.PacketInfo->Active);
            trans->Append(stmt);
        }
    }

    for (auto const& p : _followers)
    {
        Follower const& follower = p.second;
        uint8 index = 0;
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_GARRISON_FOLLOWERS);
        stmt->setUInt64(index++, follower.PacketInfo.DbID);
        stmt->setUInt64(index++, _owner->GetGUIDLow());
        stmt->setUInt32(index++, follower.PacketInfo.GarrFollowerID);
        stmt->setUInt32(index++, follower.PacketInfo.Quality);
        stmt->setUInt32(index++, follower.PacketInfo.FollowerLevel);
        stmt->setUInt32(index++, follower.PacketInfo.ItemLevelWeapon);
        stmt->setUInt32(index++, follower.PacketInfo.ItemLevelArmor);
        stmt->setUInt32(index++, follower.PacketInfo.Xp);
        stmt->setUInt32(index++, follower.PacketInfo.CurrentBuildingID);
        stmt->setUInt32(index++, follower.PacketInfo.CurrentMissionID);
        stmt->setUInt32(index++, follower.PacketInfo.FollowerStatus);
        trans->Append(stmt);

        uint8 slot = 0;
        for (GarrAbilityEntry const* ability : follower.PacketInfo.AbilityID)
        {
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_GARRISON_FOLLOWER_ABILITIES);
            stmt->setUInt64(0, follower.PacketInfo.DbID);
            stmt->setUInt32(1, ability->ID);
            stmt->setUInt8(2, slot++);
            trans->Append(stmt);
        }
    }

    for (auto const& m : _missions)
    {
        Mission const& mission = m.second;
        uint8 index = 0;

        if (mission.PacketInfo.MissionState == MISSION_STATE_COMPLETED)
            continue;

        if (mission.PacketInfo.OfferTime + mission.PacketInfo.OfferDuration <= time(nullptr))
            continue;

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_GARRISON_MISSIONS);
        stmt->setUInt64(index++, mission.PacketInfo.DbID);
        stmt->setUInt64(index++, _owner->GetGUIDLow());
        stmt->setUInt32(index++, mission.PacketInfo.MissionRecID);
        stmt->setUInt32(index++, mission.PacketInfo.OfferTime);
        stmt->setUInt32(index++, mission.PacketInfo.OfferDuration);
        stmt->setUInt32(index++, mission.PacketInfo.StartTime);
        stmt->setUInt32(index++, mission.PacketInfo.TravelDuration);
        stmt->setUInt32(index++, mission.PacketInfo.MissionDuration);
        stmt->setUInt32(index++, mission.PacketInfo.MissionState);
        trans->Append(stmt);
    }

    for (auto &x : _shipments)
    {
        for (auto shipment : x.second)
        {
            uint8 index = 0;
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_GARRISON_SHIPMENTS);

            stmt->setUInt64(index++, shipment.ShipmentID);
            stmt->setUInt64(index++, _owner->GetGUIDLow());
            stmt->setUInt16(index++, shipment.ShipmentRecID);
            stmt->setUInt32(index++, shipment.CreationTime);

            trans->Append(stmt);
        }
    }
}

void Garrison::DeleteFromDB(ObjectGuid::LowType ownerGuid, SQLTransaction trans)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHARACTER_GARRISON);
    stmt->setUInt64(0, ownerGuid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHARACTER_GARRISON_BLUEPRINTS);
    stmt->setUInt64(0, ownerGuid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHARACTER_GARRISON_BUILDINGS);
    stmt->setUInt64(0, ownerGuid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHARACTER_GARRISON_FOLLOWERS);
    stmt->setUInt64(0, ownerGuid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHARACTER_GARRISON_MISSIONS);
    stmt->setUInt64(0, ownerGuid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHARACTER_GARRISON_SHIPMENTS);
    stmt->setUInt64(0, ownerGuid);
    trans->Append(stmt);
}

bool Garrison::Create(uint32 garrSiteId)
{
    _siteLevel = sGarrisonMgr.GetGarrSiteLevelEntry(garrSiteId, 1);
    if (!_siteLevel)
        return false;

    InitializePlots();

    WorldPackets::Garrison::GarrisonCreateResult garrisonCreateResult;
    garrisonCreateResult.GarrSiteLevelID = _siteLevel->ID;
    _owner->SendDirectMessage(garrisonCreateResult.Write());

    //_owner->GetPhaseMgr().RemoveUpdateFlag(PHASE_UPDATE_FLAG_AREA_UPDATE); update phase send at quest credit.
    _owner->TeleportTo(GetGarrisonMapID(), _owner->GetPositionX(), _owner->GetPositionY(), _owner->GetPositionZ(), _owner->GetOrientation(), TELE_TO_SEAMLESS);
    SendRemoteInfo();
    return true;
}

void Garrison::Delete()
{
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    DeleteFromDB(_owner->GetGUIDLow(), trans);
    CharacterDatabase.CommitTransaction(trans);

    WorldPackets::Garrison::GarrisonDeleteResult garrisonDelete;
    garrisonDelete.Result = GARRISON_SUCCESS;
    garrisonDelete.GarrSiteID = _siteLevel->SiteID;
    _owner->SendDirectMessage(garrisonDelete.Write());
}

void Garrison::InitializePlots()
{
    if (std::vector<GarrSiteLevelPlotInstEntry const*> const* plots = sGarrisonMgr.GetGarrPlotInstForSiteLevel(_siteLevel->ID))
    {
        for (size_t i = 0; i < plots->size(); ++i)
        {
            uint32 garrPlotInstanceId = plots->at(i)->GarrPlotInstanceID;
            GarrPlotInstanceEntry const* plotInstance = sGarrPlotInstanceStore.LookupEntry(garrPlotInstanceId);
            GameObjectsEntry const* gameObject = sGarrisonMgr.GetPlotGameObject(_siteLevel->MapID, garrPlotInstanceId);
            if (!plotInstance || !gameObject)
                continue;

            GarrPlotEntry const* plot = sGarrPlotStore.LookupEntry(plotInstance->GarrPlotID);
            if (!plot)
                continue;

            Plot& plotInfo = _plots[garrPlotInstanceId];
            plotInfo.PacketInfo.GarrPlotInstanceID = garrPlotInstanceId;
            plotInfo.PacketInfo.PlotPos.Relocate(gameObject->Position.X, gameObject->Position.Y, gameObject->Position.Z, 2 * std::acos(gameObject->RotationW));
            plotInfo.PacketInfo.PlotType = plot->PlotType;
            plotInfo.EmptyGameObjectId = gameObject->ID;
            plotInfo.GarrSiteLevelPlotInstId = plots->at(i)->ID;
        }
    }
}

void Garrison::Upgrade()
{
    WorldPackets::Garrison::GarrisonUpgradeResult result;

    uint32 garrLvl = _siteLevel->Level;

    // check for cheting / modification in client
    if (garrLvl == 3)
    {
        result.Result = GARRISON_ERROR_MAX_LEVEL;
        _owner->SendDirectMessage(result.Write());
        return;
    }

    GarrSiteLevelEntry const* newSiteLevel = NULL;
    for (GarrSiteLevelEntry const* v : sGarrSiteLevelStore)
        if (v->SiteID == _siteLevel->SiteID && v->Level == garrLvl + 1)
            newSiteLevel = v;

    if (newSiteLevel->Level == garrLvl)
    {
        result.Result = GARRISON_ERROR_NO_BUILDING;
        _owner->SendDirectMessage(result.Write());
        return;
    }

    // cost taken from old site, not from new
    if (!_owner->HasCurrency(CURRENCY_TYPE_GARRISON_RESOURCES, _siteLevel->UpgradeResourceCost))
    {
        result.Result = GARRISON_ERROR_NOT_ENOUGH_CURRENCY;
        _owner->SendDirectMessage(result.Write());
        return;
    }

    // cost taken from old site, not from new
    if (!_owner->HasEnoughMoney(uint64(_siteLevel->UpgradeMoneyCost) * GOLD))
    {
        result.Result = GARRISON_ERROR_NOT_ENOUGH_GOLD;
        _owner->SendDirectMessage(result.Write());
        return;
    }
    //Set new site level.
    _siteLevel = newSiteLevel;
    InitializePlots();

    switch (_siteLevel->ID)
    {
        //< alliance
        case 444:   // 2 lvl
            _owner->TeleportTo(_siteLevel->MapID, 1779.97f, 199.439f, 70.8068f, 0.3276943f, TELE_TO_SEAMLESS);
            break;
        case 6:     // 3 lvl
            _owner->TeleportTo(_siteLevel->MapID, 1733.25f, 156.995f, 75.40f, 0.79f, TELE_TO_SEAMLESS);
            break;
            //< horde
        case 445:   // 2 lvl
        case 259:   // 3 lvl
            _owner->TeleportTo(_siteLevel->MapID, 5756.79f, 4493.11f, 132.311f, 2.862183f, TELE_TO_SEAMLESS);
            break;
        default:
            _owner->TeleportTo(_siteLevel->MapID, _owner->GetPositionX(), _owner->GetPositionY(), _owner->GetPositionZ(), _owner->GetOrientation(), TELE_TO_SEAMLESS);
            break;
    }

    WorldPackets::Misc::StreamingMovie movie;
    movie.MovieIDs.push_back(_siteLevel->MovieID);
    _owner->SendDirectMessage(movie.Write());

    result.Result = GARRISON_SUCCESS;
    result.GarrSiteLevelID = _siteLevel->ID;
    _owner->SendDirectMessage(result.Write());

    _owner->ModifyCurrency(CURRENCY_TYPE_GARRISON_RESOURCES, -_siteLevel->UpgradeResourceCost, false, true);
    _owner->ModifyMoney(-_siteLevel->UpgradeMoneyCost * GOLD, false);

    // Save us
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    SaveToDB(trans);
    CharacterDatabase.CommitTransaction(trans);
}

void Garrison::Enter() const
{
    WorldLocation loc(_siteLevel->MapID);
    loc.Relocate(_owner);
    _owner->TeleportTo(loc, TELE_TO_SEAMLESS);
}

void Garrison::Leave() const
{
    if (MapEntry const* map = sMapStore.LookupEntry(_siteLevel->MapID))
    {
        WorldLocation loc(map->ParentMapID);
        loc.Relocate(_owner);
        _owner->TeleportTo(loc, TELE_TO_SEAMLESS);
    }
}

void Garrison::Update(uint32 diff)
{
    updateTimer.Update(diff);
    if (!updateTimer.Passed())
        return;

    updateTimer.Reset();

    Map* map = FindMap();
    if (!map)
        return;

    for (auto& p : _plots)
    {
        if (p.second.buildingActivationWaiting &&
            p.second.BuildingInfo.CanActivate() && p.second.BuildingInfo.PacketInfo && !p.second.BuildingInfo.PacketInfo->Active)
        {
            p.second.buildingActivationWaiting = false;

            if (FinalizeGarrisonPlotGOInfo const* finalizeInfo = sGarrisonMgr.GetPlotFinalizeGOInfo(p.second.PacketInfo.GarrPlotInstanceID))
            {
                Position const& pos2 = finalizeInfo->FactionInfo[GetFaction()].Pos;
                GameObject* finalizer = new GameObject();
                if (finalizer->Create(sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate(), finalizeInfo->FactionInfo[GetFaction()].GameObjectId, map, 1, pos2.GetPositionX(), pos2.GetPositionY(),
                    pos2.GetPositionZ(), pos2.GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 255, GO_STATE_READY))
                {
                    // set some spell id to make the object delete itself after use
                    finalizer->SetSpellId(finalizer->GetGOInfo()->goober.spell);
                    finalizer->SetRespawnTime(0);

                    if (uint16 animKit = finalizeInfo->FactionInfo[GetFaction()].AnimKitId)
                        finalizer->SetAIAnimKitId(animKit);

                    map->AddToMap(finalizer);
                }
                else
                    delete finalizer;
            }
        }
    }

    //! Shipment orders update.
    for (auto data : ShipmentConteiners)
    {
        GameObject *object = map->GetGameObject(data.second);
        if (!object)
            continue;

        bool ready = false;
        for (auto &data : _shipments[data.first])
        {
            if (data.ShipmentDuration > 0)
                data.ShipmentDuration -= updateTimer.GetCurrent() / IN_MILLISECONDS;
            else
            {
                //maybe we need prepare go for spawining?
                ready = true;
                data.ShipmentDuration = 0;
            }
        }

        // go horde non actine = 20508, active 19959
        // go alliance non active 15585, active = 16091
        if (ready)
        {
            uint32 displayID = _owner->GetTeam() == HORDE ? 19959 : 16091;
            if (object->GetDisplayId() != displayID)
                object->SetDisplayId(displayID);

            if (object->GetGoState() == GO_STATE_ACTIVE_ALTERNATIVE)
                object->SetGoState(GO_STATE_READY);
        }
        else if (!object->loot.items.size())
        {
            uint32 displayID = _owner->GetTeam() == HORDE ? 20508 : 15585;
            if (object->GetDisplayId() != displayID)
                object->SetDisplayId(displayID);
            if (object->GetGoState() != (_shipments[data.first].size() ? GO_STATE_ACTIVE_ALTERNATIVE : GO_STATE_READY))
                object->SetGoState(_shipments[data.first].size() ? GO_STATE_ACTIVE_ALTERNATIVE : GO_STATE_READY);
        }
    }
}

GarrisonFactionIndex Garrison::GetFaction() const
{
    return _owner->GetTeam() == HORDE ? GARRISON_FACTION_INDEX_HORDE : GARRISON_FACTION_INDEX_ALLIANCE;
}

std::vector<Garrison::Plot*> Garrison::GetPlots()
{
    std::vector<Plot*> plots;
    plots.reserve(_plots.size());
    for (auto& p : _plots)
        plots.push_back(&p.second);

    return plots;
}

Garrison::Plot* Garrison::GetPlot(uint32 garrPlotInstanceId)
{
    auto itr = _plots.find(garrPlotInstanceId);
    if (itr != _plots.end())
        return &itr->second;

    return nullptr;
}

Garrison::Plot const* Garrison::GetPlot(uint32 garrPlotInstanceId) const
{
    auto itr = _plots.find(garrPlotInstanceId);
    if (itr != _plots.end())
        return &itr->second;

    return nullptr;
}

Garrison::Plot* Garrison::GetPlotWithBuildingType(uint32 BuildingTypeID)
{
    GarrBuildingEntry const* existingBuilding;
    for (auto &p : _plots)
    {
        if (p.second.BuildingInfo.PacketInfo)
        {
            existingBuilding = sGarrBuildingStore.AssertEntry(p.second.BuildingInfo.PacketInfo->GarrBuildingID);
            if (existingBuilding->Type == BuildingTypeID)
                return &p.second;
        }
    }
    return NULL;
}
bool Garrison::LearnBlueprint(uint32 garrBuildingId)
{
    WorldPackets::Garrison::GarrisonLearnBlueprintResult learnBlueprintResult;
    learnBlueprintResult.BuildingID = garrBuildingId;
    learnBlueprintResult.Result = GARRISON_SUCCESS;

    if (!sGarrBuildingStore.LookupEntry(garrBuildingId))
        learnBlueprintResult.Result = GARRISON_ERROR_INVALID_BUILDINGID;
    else if (_knownBuildings.count(garrBuildingId))
        learnBlueprintResult.Result = GARRISON_ERROR_BLUEPRINT_KNOWN;
    else
        _knownBuildings.insert(garrBuildingId);

    _owner->SendDirectMessage(learnBlueprintResult.Write());

    if (learnBlueprintResult.Result = GARRISON_SUCCESS)
        return false;
    else
        return true;
}

void Garrison::UnlearnBlueprint(uint32 garrBuildingId)
{
    WorldPackets::Garrison::GarrisonUnlearnBlueprintResult unlearnBlueprintResult;
    unlearnBlueprintResult.BuildingID = garrBuildingId;
    unlearnBlueprintResult.Result = GARRISON_SUCCESS;

    if (!sGarrBuildingStore.LookupEntry(garrBuildingId))
        unlearnBlueprintResult.Result = GARRISON_ERROR_INVALID_BUILDINGID;
    else if (!_knownBuildings.count(garrBuildingId))
        unlearnBlueprintResult.Result = GARRISON_ERROR_BLUEPRINT_NOT_KNOWN;
    else
        _knownBuildings.erase(garrBuildingId);

    _owner->SendDirectMessage(unlearnBlueprintResult.Write());
}

void Garrison::PlaceBuilding(uint32 garrPlotInstanceId, uint32 garrBuildingId, bool quest/* = false*/)
{
    WorldPackets::Garrison::GarrisonPlaceBuildingResult placeBuildingResult;
    placeBuildingResult.Result = CheckBuildingPlacement(garrPlotInstanceId, garrBuildingId, quest);
    if (placeBuildingResult.Result == GARRISON_SUCCESS)
    {
        placeBuildingResult.BuildingInfo.GarrPlotInstanceID = garrPlotInstanceId;
        placeBuildingResult.BuildingInfo.GarrBuildingID = garrBuildingId;
        placeBuildingResult.BuildingInfo.TimeBuilt = time(nullptr);

        Plot* plot = GetPlot(garrPlotInstanceId);
        plot->buildingActivationWaiting = true;

        uint32 oldBuildingId = 0;
        Map* map = FindMap();
        GarrBuildingEntry const* building = sGarrBuildingStore.AssertEntry(garrBuildingId);
        if (map)
        {
            if (InstanceMap* m = map->ToInstanceMap())
                if (InstanceScript* inst = m->GetInstanceScript())
                    inst->OnPlaceBuilding(_owner, this, garrBuildingId, garrPlotInstanceId, placeBuildingResult.BuildingInfo.TimeBuilt);

            plot->DeleteGameObject(map);
        }

        if (plot->BuildingInfo.PacketInfo)
        {
            oldBuildingId = plot->BuildingInfo.PacketInfo->GarrBuildingID;
            if (sGarrBuildingStore.AssertEntry(oldBuildingId)->Type != building->Type)
                plot->ClearBuildingInfo(_owner);
        }

        // If build by quest - skip building state and spawn building.
        if (quest)
        {
            placeBuildingResult.PlayActivationCinematic = true;
            placeBuildingResult.BuildingInfo.Active = true;
        }
        else
        {
            _owner->ModifyCurrency(building->CostCurrencyID, -building->CostCurrencyAmount, false, true);
            _owner->ModifyMoney(-building->CostMoney * GOLD, false);
        }

        plot->SetBuildingInfo(placeBuildingResult.BuildingInfo, _owner);
        if (map)
        {
            if (GameObject* go = plot->CreateGameObject(map, GetFaction(), this))
                map->AddToMap(go);
        }

        if (oldBuildingId)
        {
            WorldPackets::Garrison::GarrisonBuildingRemoved buildingRemoved;
            buildingRemoved.Result = GARRISON_SUCCESS;
            buildingRemoved.GarrPlotInstanceID = garrPlotInstanceId;
            buildingRemoved.GarrBuildingID = oldBuildingId;
            _owner->SendDirectMessage(buildingRemoved.Write());
        }

        _owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_PLACE_GARRISON_BUILDING, garrBuildingId);
    }

    _owner->SendDirectMessage(placeBuildingResult.Write());
}

void Garrison::CancelBuildingConstruction(uint32 garrPlotInstanceId)
{
    WorldPackets::Garrison::GarrisonBuildingRemoved buildingRemoved;
    buildingRemoved.Result = CheckBuildingRemoval(garrPlotInstanceId);
    if (buildingRemoved.Result == GARRISON_SUCCESS)
    {
        Plot* plot = GetPlot(garrPlotInstanceId);

        buildingRemoved.GarrPlotInstanceID = garrPlotInstanceId;
        buildingRemoved.GarrBuildingID = plot->BuildingInfo.PacketInfo->GarrBuildingID;

        Map* map = FindMap();
        if (map)
            plot->DeleteGameObject(map);

        plot->ClearBuildingInfo(_owner);
        _owner->SendDirectMessage(buildingRemoved.Write());

        GarrBuildingEntry const* constructing = sGarrBuildingStore.AssertEntry(buildingRemoved.GarrBuildingID);
        // Refund construction/upgrade cost
        _owner->ModifyCurrency(constructing->CostCurrencyID, constructing->CostCurrencyAmount, false, true);
        _owner->ModifyMoney(constructing->CostMoney * GOLD, false);

        if (constructing->Level > 1)
        {
            // Restore previous level building
            GarrBuildingEntry const* restored = sGarrisonMgr.GetPreviousLevelBuilding(constructing->Type, constructing->Level);
            ASSERT(restored);

            WorldPackets::Garrison::GarrisonPlaceBuildingResult placeBuildingResult;
            placeBuildingResult.Result = GARRISON_SUCCESS;
            placeBuildingResult.BuildingInfo.GarrPlotInstanceID = garrPlotInstanceId;
            placeBuildingResult.BuildingInfo.GarrBuildingID = restored->ID;
            placeBuildingResult.BuildingInfo.TimeBuilt = time(nullptr);
            placeBuildingResult.BuildingInfo.Active = true;

            plot->SetBuildingInfo(placeBuildingResult.BuildingInfo, _owner);
            _owner->SendDirectMessage(placeBuildingResult.Write());
        }

        if (map)
            if (GameObject* go = plot->CreateGameObject(map, GetFaction(), this))
                map->AddToMap(go);
    }
    else
        _owner->SendDirectMessage(buildingRemoved.Write());
}

void Garrison::ActivateBuilding(uint32 garrPlotInstanceId)
{
    if (Plot* plot = GetPlot(garrPlotInstanceId))
    {
        if (plot->BuildingInfo.CanActivate() && plot->BuildingInfo.PacketInfo && !plot->BuildingInfo.PacketInfo->Active)
        {
            plot->BuildingInfo.PacketInfo->Active = true;

            if (Map* map = FindMap())
            {
                plot->DeleteGameObject(map);
                if (GameObject* go = plot->CreateGameObject(map, GetFaction(), this))
                    map->AddToMap(go);
            }

            WorldPackets::Garrison::GarrisonBuildingActivated buildingActivated;
            buildingActivated.GarrPlotInstanceID = garrPlotInstanceId;
            _owner->SendDirectMessage(buildingActivated.Write());

            _owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CONSTRUCT_GARRISON_BUILDING, plot->BuildingInfo.PacketInfo->GarrBuildingID);
        }
    }
}

void Garrison::AddFollower(uint32 garrFollowerId)
{
    WorldPackets::Garrison::GarrisonAddFollowerResult addFollowerResult;
    GarrFollowerEntry const* followerEntry = sGarrFollowerStore.LookupEntry(garrFollowerId);
    if (_followerIds.count(garrFollowerId) || !followerEntry)
    {
        addFollowerResult.Result = GARRISON_GENERIC_UNKNOWN_ERROR;
        _owner->SendDirectMessage(addFollowerResult.Write());
        return;
    }

    _followerIds.insert(garrFollowerId);
    uint64 dbId = sGarrisonMgr.GenerateFollowerDbId();
    Follower& follower = _followers[dbId];
    follower.PacketInfo.DbID = dbId;
    follower.PacketInfo.GarrFollowerID = garrFollowerId;
    follower.PacketInfo.Quality = follower.RollQuality(followerEntry->Quality);
    follower.PacketInfo.FollowerLevel = followerEntry->Level;
    follower.PacketInfo.ItemLevelWeapon = followerEntry->ItemLevelWeapon;
    follower.PacketInfo.ItemLevelArmor = followerEntry->ItemLevelArmor;
    follower.PacketInfo.AbilityID = sGarrisonMgr.RollFollowerAbilities(followerEntry, follower.PacketInfo.Quality, GetFaction(), true);
    follower.PacketInfo.FollowerStatus = FOLLOWER_STATUS_BASE;

    addFollowerResult.Follower = follower.PacketInfo;
    _owner->SendDirectMessage(addFollowerResult.Write());

    _owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_RECRUIT_GARRISON_FOLLOWER, follower.PacketInfo.DbID);

    if (GarrMissionEntry const* mission = sGarrisonMgr.GetMissionAtFollowerTaking(garrFollowerId))
        AddMission(mission->ID);
}

void Garrison::AddMission(uint32 missionRecID)
{
    WorldPackets::Garrison::GarrisonAddMissionResult addMissionResult;
    GarrMissionEntry const* missionEntry = sGarrMissionStore.LookupEntry(missionRecID);
    if (_missionIds.count(missionRecID) || !missionEntry)
    {
        addMissionResult.Result = GARRISON_GENERIC_UNKNOWN_ERROR;
        _owner->SendDirectMessage(addMissionResult.Write());
        return;
    }

    _missionIds.insert(missionRecID);
    uint64 dbId = sGarrisonMgr.GenerateMissionDbId();
    Mission& mission = _missions[dbId];
    mission.PacketInfo.DbID = dbId;
    mission.PacketInfo.MissionRecID = missionRecID;
    mission.PacketInfo.OfferTime = time(nullptr);
    mission.PacketInfo.OfferDuration = missionEntry->OfferDuration;
    mission.PacketInfo.StartTime = 0;
    mission.PacketInfo.TravelDuration = 0;
    mission.PacketInfo.MissionDuration = missionEntry->MissionDuration;
    mission.PacketInfo.MissionState = MISSION_STATE_AVAILABLE;

    addMissionResult.unk = 1;
    addMissionResult.MissionData = mission.PacketInfo;
    _owner->SendDirectMessage(addMissionResult.Write());
}

Garrison::Follower const* Garrison::GetFollower(uint64 dbId) const
{
    auto itr = _followers.find(dbId);
    if (itr != _followers.end())
        return &itr->second;

    return nullptr;
}

Garrison::Follower* Garrison::GetFollower(uint64 dbId)
{
    auto itr = _followers.find(dbId);
    if (itr != _followers.end())
        return &itr->second;

    return nullptr;
}

Garrison::Follower* Garrison::GetFollowerByID(uint32 entry)
{
    for (auto& v : _followers)
        if (v.second.PacketInfo.GarrFollowerID == entry)
            return &v.second;

    return nullptr;
}

void Garrison::ReTrainFollower(SpellInfo const* spellInfo, uint32 followerID)
{
    auto follower = GetFollowerByID(followerID);
    if (!follower)
        return;

    auto followerEntry = sGarrFollowerStore.LookupEntry(follower->PacketInfo.GarrFollowerID);
    if (!followerEntry)
        return;

    follower->PacketInfo.AbilityID = sGarrisonMgr.RollFollowerAbilities(followerEntry, follower->PacketInfo.Quality, GetFaction(), spellInfo->Effects[0].MiscValue == 1 ? false : true);

    // here should be SMSG_GARRISON_FOLLOWER_CHANGED_ABILITIES
}

Garrison::Mission const* Garrison::GetMission(uint64 dbId) const
{
    auto itr = _missions.find(dbId);
    if (itr != _missions.end())
        return &itr->second;

    return nullptr;
}

Garrison::Mission* Garrison::GetMissionByRecID(uint32 missionRecID)
{
    for (auto& m : _missions)
        if (m.second.PacketInfo.MissionRecID == missionRecID)
            return &m.second;

    return nullptr;
}

void Garrison::SendInfo()
{
    WorldPackets::Garrison::GetGarrisonInfoResult garrisonInfo;
    garrisonInfo.GarrSiteID = _siteLevel->SiteID;
    garrisonInfo.GarrSiteLevelID = _siteLevel->ID;
    garrisonInfo.FactionIndex = GetFaction();
    garrisonInfo.NumFollowerActivationsRemaining = _followerActivationsRemainingToday;
    for (auto& p : _plots)
    {
        Plot& plot = p.second;
        garrisonInfo.Plots.push_back(&plot.PacketInfo);
        if (plot.BuildingInfo.PacketInfo)
            garrisonInfo.Buildings.push_back(plot.BuildingInfo.PacketInfo.get_ptr());
    }

    for (auto const& p : _followers)
        garrisonInfo.Followers.push_back(&p.second.PacketInfo);

    for (auto const& i : _missions)
        garrisonInfo.Missions.push_back(&i.second.PacketInfo);
    
    garrisonInfo.ArchivedMissions.resize(0);

    _owner->SendDirectMessage(garrisonInfo.Write());
}

void Garrison::SendRemoteInfo() const
{
    MapEntry const* garrisonMap = sMapStore.LookupEntry(_siteLevel->MapID);
    if (!garrisonMap || int32(_owner->GetMapId()) != garrisonMap->ParentMapID)
        return;

    WorldPackets::Garrison::GarrisonRemoteInfo remoteInfo;
    remoteInfo.Sites.resize(1);

    WorldPackets::Garrison::GarrisonRemoteSiteInfo& remoteSiteInfo = remoteInfo.Sites[0];
    remoteSiteInfo.GarrSiteLevelID = _siteLevel->ID;
    for (auto const& p : _plots)
        if (p.second.BuildingInfo.PacketInfo)
            remoteSiteInfo.Buildings.emplace_back(p.first, p.second.BuildingInfo.PacketInfo->GarrBuildingID);

    _owner->SendDirectMessage(remoteInfo.Write());
}

void Garrison::SendBlueprintAndSpecializationData()
{
    WorldPackets::Garrison::GarrisonRequestBlueprintAndSpecializationDataResult data;
    data.BlueprintsKnown = &_knownBuildings;
    _owner->SendDirectMessage(data.Write());
}

void Garrison::SendBuildingLandmarks(Player* receiver) const
{
    WorldPackets::Garrison::GarrisonBuildingLandmarks buildingLandmarks;
    buildingLandmarks.Landmarks.reserve(_plots.size());

    for (auto const& p : _plots)
    {
        Plot const& plot = p.second;
        if (plot.BuildingInfo.PacketInfo)
            if (uint32 garrBuildingPlotInstId = sGarrisonMgr.GetGarrBuildingPlotInst(plot.BuildingInfo.PacketInfo->GarrBuildingID, plot.GarrSiteLevelPlotInstId))
                buildingLandmarks.Landmarks.emplace_back(garrBuildingPlotInstId, plot.PacketInfo.PlotPos);
    }

    receiver->SendDirectMessage(buildingLandmarks.Write());
}

void Garrison::SendGarrisonUpgradebleResult(Player* receiver) const
{
    //!
    //@TODO worn on checks... exmaple: at first lvl u cant upgrade if u hadnt complete quest line
    // Quest line with rew. item 118215
    WorldPackets::Garrison::GarrisonIsUpgradeableResult result;
    switch (_siteLevel->Level)
    {
        case 1: 
            result.Result = _plots.size() < 2 ? GARRISON_ERROR_NOT_ALLOW_GARRISON_UPGRADE : GARRISON_SUCCESS;
            break;
        case 2: //Requare Alliance Q: 36592, Horde Q: 36567
        default:
            result.Result = GARRISON_ERROR_MAX_LEVEL;
            break;
    }

    if (sWorld->getBoolConfig(CONFIG_DISABLE_GARE_UPGRADE))
        result.Result = GARRISON_ERROR_MAX_LEVEL;

    receiver->SendDirectMessage(result.Write());
}

Map* Garrison::FindMap() const
{
    return sMapMgr->FindMap(_siteLevel->MapID, _owner->GetGUIDLow());
}

GarrisonError Garrison::CheckBuildingPlacement(uint32 garrPlotInstanceId, uint32 garrBuildingId, bool byQuest/*=false*/) const
{
    GarrPlotInstanceEntry const* plotInstance = sGarrPlotInstanceStore.LookupEntry(garrPlotInstanceId);
    Plot const* plot = GetPlot(garrPlotInstanceId);
    if (!plotInstance || !plot)
        return GARRISON_ERROR_INVALID_PLOT;

    GarrBuildingEntry const* building = sGarrBuildingStore.LookupEntry(garrBuildingId);
    if (!building)
        return GARRISON_ERROR_INVALID_BUILDINGID;

    if (!sGarrisonMgr.IsPlotMatchingBuilding(plotInstance->GarrPlotID, garrBuildingId))
        return GARRISON_ERROR_INVALID_PLOT_BUILDING;

    // Cannot place buldings of higher level than garrison level
    if (building->Level > _siteLevel->Level)
        return GARRISON_ERROR_INVALID_BUILDINGID;

    if (building->Flags & GARRISON_BUILDING_FLAG_NEEDS_PLAN)
    {
        if (!_knownBuildings.count(garrBuildingId))
            return GARRISON_ERROR_BLUEPRINT_NOT_KNOWN;
    }
    else if (!byQuest) // Building is built as a quest reward
        return GARRISON_ERROR_INVALID_BUILDINGID;

    // Check all plots to find if we already have this building
    GarrBuildingEntry const* existingBuilding;
    for (auto const& p : _plots)
    {
        if (p.second.BuildingInfo.PacketInfo)
        {
            existingBuilding = sGarrBuildingStore.AssertEntry(p.second.BuildingInfo.PacketInfo->GarrBuildingID);
            if (existingBuilding->Type == building->Type)
                if (p.first != garrPlotInstanceId || existingBuilding->Level + 1 != building->Level)    // check if its an upgrade in same plot
                    return GARRISON_ERROR_BUILDING_EXISTS;
        }
    }

    if (!byQuest && building->CostCurrencyAmount && !_owner->HasCurrency(building->CostCurrencyID, building->CostCurrencyAmount))
        return GARRISON_ERROR_NOT_ENOUGH_CURRENCY;

    if (building->CostMoney && !_owner->HasEnoughMoney(uint64(building->CostMoney) * GOLD))
        return GARRISON_ERROR_NOT_ENOUGH_GOLD;

    // New building cannot replace another building currently under construction
    if (plot->BuildingInfo.PacketInfo)
        if (!plot->BuildingInfo.PacketInfo->Active)
            return GARRISON_ERROR_NO_BUILDING;

    return GARRISON_SUCCESS;
}

GarrisonError Garrison::CheckBuildingRemoval(uint32 garrPlotInstanceId) const
{
    Plot const* plot = GetPlot(garrPlotInstanceId);
    if (!plot)
        return GARRISON_ERROR_INVALID_PLOT;

    if (!plot->BuildingInfo.PacketInfo)
        return GARRISON_ERROR_NO_BUILDING;

    if (plot->BuildingInfo.CanActivate())
        return GARRISON_ERROR_BUILDING_EXISTS;

    return GARRISON_SUCCESS;
}

GameObject* Garrison::Plot::CreateGameObject(Map* map, GarrisonFactionIndex faction, Garrison* garrison)
{
    uint32 entry = EmptyGameObjectId;
    GarrBuildingEntry const* buildingEtry = NULL;
    if (BuildingInfo.PacketInfo)
    {
        GarrPlotInstanceEntry const* plotInstance = sGarrPlotInstanceStore.AssertEntry(PacketInfo.GarrPlotInstanceID);
        GarrPlotEntry const* plot = sGarrPlotStore.AssertEntry(plotInstance->GarrPlotID);
        buildingEtry = sGarrBuildingStore.AssertEntry(BuildingInfo.PacketInfo->GarrBuildingID);
        entry = faction == GARRISON_FACTION_INDEX_HORDE ? plot->HordeConstructionGameObjectID : plot->AllianceConstructionGameObjectID;
        if (BuildingInfo.PacketInfo->Active || !entry)
            entry = faction == GARRISON_FACTION_INDEX_HORDE ? buildingEtry->HordeGameObjectID : buildingEtry->AllianceGameObjectID;
    }

    if (!sObjectMgr->GetGameObjectTemplate(entry))
    {
        sLog->outError(LOG_FILTER_PLAYER, "Garrison attempted to spawn gameobject whose template doesn't exist (%u)", entry);
        return nullptr;
    }

    Position const& pos = PacketInfo.PlotPos;
    GameObject* building = new GameObject();
    if (!building->Create(sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate(), entry, map, 1, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation(),
        0.0f, 0.0f, 0.0f, 0.0f, 255, GO_STATE_READY))
    {
        delete building;
        return nullptr;
    }
    
    if ((building->GetGoType() == GAMEOBJECT_TYPE_GARRISON_BUILDING || building->GetGoType() == GAMEOBJECT_TYPE_GARRISON_PLOT)/* && building->GetGOInfo()->garrisonBuilding.mapID*/)
    {

        if (std::list<GameObjectData> const* goList = sGarrisonMgr.GetGoSpawnBuilding(PacketInfo.GarrPlotInstanceID, BuildingInfo.PacketInfo && BuildingInfo.PacketInfo->Active ? BuildingInfo.PacketInfo->GarrBuildingID : 0))
        for (std::list<GameObjectData>::const_iterator data = goList->begin(); data != goList->end(); ++data)
        {
            if (GarrisonMgr::getFirstMap(map->GetId()) != data->mapid)
                continue;

            // ignore building state object for already build object.
            if (data->building && !BuildingInfo.PacketInfo ||
                (BuildingInfo.PacketInfo && BuildingInfo.PacketInfo->Active == data->building))
                continue;

            GameObject* linkGO = new GameObject();
            if (!linkGO->Create(sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate(), data->id, map, 1, data->posX, data->posY, data->posZ, data->orientation,
                data->rotation0, data->rotation1, data->rotation2, data->rotation3, 255, GO_STATE_READY) ||
                !linkGO->IsPositionValid() || !map->AddToMap(linkGO))
            {
                delete linkGO;
                continue;
            }
            if (buildingEtry && linkGO->GetGOInfo()->type == GAMEOBJECT_TYPE_GARRISON_SHIPMENT)
                garrison->ShipmentConteiners[buildingEtry->Type] = linkGO->GetGUID();
            BuildingInfo.Spawns.insert(linkGO->GetGUID());
        }

        if (std::list<CreatureData> const* npcList = sGarrisonMgr.GetNpcSpawnBuilding(PacketInfo.GarrPlotInstanceID, BuildingInfo.PacketInfo && BuildingInfo.PacketInfo->Active ? BuildingInfo.PacketInfo->GarrBuildingID : 0))
        for (std::list<CreatureData>::const_iterator data = npcList->begin(); data != npcList->end(); ++data)
        {
            if (GarrisonMgr::getFirstMap(map->GetId()) != data->mapid)
                continue;
            // ignore building state object for already build object.
            if (data->building && !BuildingInfo.PacketInfo ||
                (BuildingInfo.PacketInfo && BuildingInfo.PacketInfo->Active == data->building))
                continue;
            Creature* linkNPC = new Creature();
            if (!linkNPC->Create(sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate(), map, 1, data->id, 0, 0, data->posX, data->posY, data->posZ, data->orientation) ||
                !linkNPC->IsPositionValid() || !map->AddToMap(linkNPC))
            {
                delete linkNPC;
                continue;
            }
            if (data->building)
                linkNPC->SetUInt32Value(UNIT_NPC_EMOTESTATE, urand(0, 1) ? 173 : 69);

            BuildingInfo.Spawns.insert(linkNPC->GetGUID());
        }
    }

    BuildingInfo.Guid = building->GetGUID();
    return building;
}

void Garrison::Plot::DeleteGameObject(Map* map)
{
    if (BuildingInfo.Guid.IsEmpty())
        return;

    for (ObjectGuid const& guid : BuildingInfo.Spawns)
    {
        WorldObject* object = nullptr;
        switch (guid.GetHigh())
        {
            case HighGuid::Creature:
                object = map->GetCreature(guid);
                break;
            case HighGuid::GameObject:
                object = map->GetGameObject(guid);
                break;
            default:
                continue;
        }

        if (object)
            object->AddObjectToRemoveList();
    }

    BuildingInfo.Spawns.clear();

    if (GameObject* oldBuilding = map->GetGameObject(BuildingInfo.Guid))
        oldBuilding->Delete();

    BuildingInfo.Guid.Clear();
}

void Garrison::Plot::ClearBuildingInfo(Player* owner)
{
    WorldPackets::Garrison::GarrisonPlotPlaced plotPlaced;
    plotPlaced.PlotInfo = &PacketInfo;
    owner->SendDirectMessage(plotPlaced.Write());

    BuildingInfo.PacketInfo = boost::none;
}

void Garrison::Plot::SetBuildingInfo(WorldPackets::Garrison::GarrisonBuildingInfo const& buildingInfo, Player* owner)
{
    if (!BuildingInfo.PacketInfo)
    {
        WorldPackets::Garrison::GarrisonPlotRemoved plotRemoved;
        plotRemoved.GarrPlotInstanceID = PacketInfo.GarrPlotInstanceID;
        owner->SendDirectMessage(plotRemoved.Write());
    }

    BuildingInfo.PacketInfo = buildingInfo;
}

bool Garrison::Building::CanActivate() const
{
    if (PacketInfo)
    {
        GarrBuildingEntry const* building = sGarrBuildingStore.AssertEntry(PacketInfo->GarrBuildingID);
        if (PacketInfo->TimeBuilt + building->BuildDuration <= time(nullptr))
            return true;
    }

    return false;
}

uint32 Garrison::Follower::GetItemLevel() const
{
    return (PacketInfo.ItemLevelWeapon + PacketInfo.ItemLevelArmor) / 2;
}

void Garrison::Follower::IncreaseFollowerItemLevel(SpellInfo const* spellInfo, Player* caster)
{
    bool updateInfo = false;

    uint32 bp = spellInfo->Effects[0].BasePoints;
    uint32 mv = spellInfo->Effects[0].MiscValue;
    uint32 mvb = spellInfo->Effects[0].MiscValueB;

    // code for 6.2.2 - delete hardcoded cases by spellID after update
    /*if (mv == 2 && mvb == 1) // increment weapon ilvl
    {
        if (PacketInfo.ItemLevelWeapon < 675)
        {
            PacketInfo.ItemLevelWeapon += bp;
            updateInfo = true;
        }
    }
    else if (mv == 3 && mvb == 1) // increment armor ilvl
    {
        if (PacketInfo.ItemLevelArmor < 675)
        {
            PacketInfo.ItemLevelArmor += bp;
            updateInfo = true;
        }
    }
    else if (mv == 1 && !mvb) // set armor ilvl
    {
        if (PacketInfo.ItemLevelArmor < bp)
        {
            PacketInfo.ItemLevelArmor = bp;
            updateInfo = true;
        }
    }
    else if (!mv && !mvb == 1) // set weapon ilvl
    {
        if (PacketInfo.ItemLevelWeapon < bp)
        {
            PacketInfo.ItemLevelWeapon = bp;
            updateInfo = true;
        }
    }*/

    switch (spellInfo->Id)
    {
        // Description: Equip a follower with an item level $s2 weapon.
        case 168143:
        case 168150:
        case 168389:
            if (PacketInfo.ItemLevelWeapon < mvb)
            {
                PacketInfo.ItemLevelWeapon = mvb;
                updateInfo = true;
            }
            break;
            // Description: Upgrade a follower's weapon by $s1 item levels.
        case 168154:
        case 168155:
        case 168156:
        case 168157:
        case 168158:
        case 168159:
        case 168161:
        case 168395:
            if (PacketInfo.ItemLevelWeapon < mvb)
            {
                PacketInfo.ItemLevelWeapon += bp;
                updateInfo = true;
            }
            break;
            // Description: Upgrade a follower's weapon by 15 item levels, up to a total item level of 655.
        case 168162:
        case 168246:
        case 168151:
            if (PacketInfo.ItemLevelWeapon < 655)
            {
                PacketInfo.ItemLevelWeapon += bp;
                updateInfo = true;
            }
            break;
            // Description: Upgrade a follower's armor by $s1 item levels.
        case 168235:
        case 168236:
        case 168237:
        case 168238:
        case 168239:
        case 168240:
        case 168241:
            if (PacketInfo.ItemLevelArmor < mvb)
            {
                PacketInfo.ItemLevelArmor += bp;
                updateInfo = true;
            }
            break;
            // Description: Equip a follower with a full set of item level $s2 armor.
        case 168242:
        case 168243:
        case 168244:
        case 168245:
            if (PacketInfo.ItemLevelArmor < mvb)
            {
                PacketInfo.ItemLevelArmor = mvb;
                updateInfo = true;
            }
            break;
        case 174914: // wrong spell for effect
            break;
        default:
            break;
    }

    if (updateInfo)
    {
        if (PacketInfo.ItemLevelWeapon > 675)
            PacketInfo.ItemLevelWeapon = 675;

        if (PacketInfo.ItemLevelArmor > 675)
            PacketInfo.ItemLevelArmor = 675;

        WorldPackets::Garrison::GarrisonFollowerChangedItemLevel update;
        update.Follower = PacketInfo;
        caster->SendDirectMessage(update.Write());
    }
}

uint8 Garrison::Follower::RollQuality(uint32 baseQuality)
{
    // 35% - rare, 7% - epic
    uint32 r = urand(0, 100);
    uint8 quality = FOLLOWER_QUALITY_UNCOMMON;
    if (r >= 65 && r < 90)
        quality = FOLLOWER_QUALITY_RARE;
    else if (r >= 93 && r <= 100)
        quality = FOLLOWER_QUALITY_EPIC;

    return quality > baseQuality ? quality : baseQuality;
}

uint32 Garrison::GetAreaIdForTeam(uint32 team)
{
    switch (team)
    {
        case ALLIANCE:
            return 7078;
        case HORDE:
            return 7004;
    }
    return 0;
}

void Garrison::Mission::Start(Player* owner)
{
    if (Garrison* garrison = owner->GetGarrison())
    {
        // check followers
        for (auto f : CurrentFollowerDBIDs)
        {
            if (Garrison::Follower* follower = garrison->GetFollower(f))
                follower->PacketInfo.CurrentMissionID = PacketInfo.MissionRecID;
        }

        PacketInfo.MissionState = MISSION_STATE_IN_PROGRESS;
        PacketInfo.StartTime = time(nullptr);

        WorldPackets::Garrison::GarrisonStartMissionResult missionRes;
        missionRes.MissionData = PacketInfo;
        missionRes.FollowerDBIDs.insert(missionRes.FollowerDBIDs.begin(), CurrentFollowerDBIDs.begin(), CurrentFollowerDBIDs.end());
        owner->SendDirectMessage(missionRes.Write());

        if (GarrMissionEntry const* entry = sGarrMissionStore.LookupEntry(PacketInfo.MissionRecID))
            owner->ModifyCurrency(CURRENCY_TYPE_GARRISON_RESOURCES, entry->ReqResourcesCount);

        garrison->SendMissionListUpdate(false);

        owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_START_GARRISON_MISSION, PacketInfo.MissionRecID);
    }
}

void Garrison::Mission::Complete(Player* owner)
{
    if (Garrison* garrison = owner->GetGarrison())
    {
        // succeeded or failed
        bool bonus = HasBonusRoll();

        if (bonus)
            PacketInfo.MissionState = MISSION_STATE_WAITING_BONUS;
        else
            PacketInfo.MissionState = MISSION_STATE_COMPLETED;

        WorldPackets::Garrison::GarrisonCompleteMissionResult res;
        res.MissionData = PacketInfo;
        res.MissionRecID = PacketInfo.MissionRecID;
        res.Succeeded = bonus ? true : false;
        owner->SendDirectMessage(res.Write());

        for (auto f : CurrentFollowerDBIDs)
        {
            if (Garrison::Follower* follower = garrison->GetFollower(f))
            {
                if (!bonus)
                    follower->PacketInfo.CurrentMissionID = 0;

                if (GarrMissionEntry const* entry = sGarrMissionStore.LookupEntry(PacketInfo.MissionRecID))
                {
                    WorldPackets::Garrison::GarrisonFollowerChangedXP data;
                    data.TotalXp = entry->BaseXP;
                    data.Follower = follower->PacketInfo;

                    follower->GiveXP(entry->BaseXP);

                    data.Follower2 = follower->PacketInfo;
                    owner->SendDirectMessage(data.Write());
                }
            }
        }

        if (!bonus)
        {
            CurrentFollowerDBIDs.clear();
            garrison->SendMissionListUpdate(true);
        }
    }
}

void Garrison::SendMissionListUpdate(bool openMissionNpc) const
{
    WorldPackets::Garrison::GarrisonMissionListUpdate data;
    data.openMissionNpc = openMissionNpc;
    _owner->SendDirectMessage(data.Write());
}

void Garrison::Mission::BonusRoll(Player* owner)
{
    if (Garrison* garrison = owner->GetGarrison())
    {
        PacketInfo.MissionState = MISSION_STATE_COMPLETED;

        WorldPackets::Garrison::GarrisonMissionBonusRollResult res;
        res.MissionData = PacketInfo;
        res.MissionRecID = PacketInfo.MissionRecID;
        owner->SendDirectMessage(res.Write());

        GarrMissionRewardEntry const* entry = sDB2Manager.GetMissionRewardByRecID(PacketInfo.MissionRecID);

        if (!entry)
            return;

        for (auto f : CurrentFollowerDBIDs)
        {
            if (Garrison::Follower* follower = garrison->GetFollower(f))
            {
                follower->PacketInfo.CurrentMissionID = 0;

                if (entry->HasFollowerXPReward())
                {
                    WorldPackets::Garrison::GarrisonFollowerChangedXP data;
                    data.TotalXp = entry->RewardXP;
                    data.Follower = follower->PacketInfo;

                    follower->GiveXP(entry->RewardXP);

                    data.Follower2 = follower->PacketInfo;
                    owner->SendDirectMessage(data.Write());
                }
            }
        }

        CurrentFollowerDBIDs.clear();

        garrison->RewardMission(PacketInfo.MissionRecID);
        garrison->SendMissionListUpdate(true);
    }
}

void Garrison::Follower::GiveXP(uint32 xp)
{
    uint32 curXp = PacketInfo.Xp;
    uint32 nextLvlXP = GetXPForNextUpgrade();
    uint32 newXP = curXp + xp;

    if (PacketInfo.FollowerLevel == 100 && PacketInfo.Quality == FOLLOWER_QUALITY_EPIC)
        newXP = 0;

    // calc level xp
    while (newXP >= nextLvlXP && PacketInfo.FollowerLevel < 100)
    {
        newXP -= nextLvlXP;

        if (PacketInfo.FollowerLevel < 100)
            GiveLevel(PacketInfo.FollowerLevel + 1);

        nextLvlXP = GetXPForNextUpgrade();
    }

    // calc level quality
    while (newXP >= nextLvlXP && PacketInfo.FollowerLevel == 100 && PacketInfo.Quality < FOLLOWER_QUALITY_EPIC)
    {
        newXP -= nextLvlXP;

        if (PacketInfo.Quality < FOLLOWER_QUALITY_EPIC)
            GiveQuality(PacketInfo.Quality + 1);

        nextLvlXP = GetXPForNextUpgrade();
    }

    PacketInfo.Xp = newXP;
}

uint32 Garrison::Follower::GetXPForNextUpgrade()
{
    if (PacketInfo.FollowerLevel < 100)
        return sDB2Manager.GetXPForNextFollowerLevel(PacketInfo.FollowerLevel);
    else if (PacketInfo.FollowerLevel == 100)
        return sDB2Manager.GetXPForNextFollowerQuality(PacketInfo.Quality);

    return 0;
}

bool Garrison::Mission::CanStart(Player* owner)
{
    Garrison* garrison = owner->GetGarrison();

    if (!garrison)
        return false;

    if (!sGarrMissionStore.LookupEntry(PacketInfo.MissionRecID))
        return false;

    if (PacketInfo.MissionState != MISSION_STATE_AVAILABLE)
        return false;

    if (PacketInfo.StartTime != time_t(0))
        return false;

    GarrMissionEntry const* entry = sGarrMissionStore.LookupEntry(PacketInfo.MissionRecID);

    if (!owner->HasCurrency(CURRENCY_TYPE_GARRISON_RESOURCES, entry->ReqResourcesCount))
        return false;

    if (CurrentFollowerDBIDs.empty() || CurrentFollowerDBIDs.size() != entry->ReqFollowersCount)
        return false;

    for (auto f : CurrentFollowerDBIDs)
    {
        Garrison::Follower* follower = garrison->GetFollower(f);

        if (!follower)
            return false;

        if (follower->PacketInfo.CurrentMissionID != 0 || follower->PacketInfo.CurrentBuildingID != 0)
            return false;

        //if (follower->PacketInfo.FollowerStatus == 2)
            //return false;
    }

    return true;
}

bool Garrison::Mission::CanBonusRoll()
{
    if (!sGarrMissionStore.LookupEntry(PacketInfo.MissionRecID))
        return false;

    if (PacketInfo.MissionState != MISSION_STATE_WAITING_BONUS)
        return false;

    if (PacketInfo.StartTime + PacketInfo.MissionDuration > time(nullptr))
        return false;

    return true;
}

float Garrison::Mission::ComputeSuccessChance()
{
    return 100.0f;
}

float Garrison::Mission::CalcChance(float a, float b, float c)
{
    float d = 0.0f;
    if (c >= 0)
        d = ((b - a) * c) + a;
    else
        d = (c + 1) * a;

    return d;
}

uint32 Garrison::Mission::GetDuration(Player* owner)
{
    uint32 duration = PacketInfo.MissionDuration;

    /*if (Garrison* garrison = owner->GetGarrison())
    {
        for (auto f : CurrentFollowerDBIDs)
        {
            if (Garrison::Follower* follower = garrison->GetFollower(f))
            {
                // Epic Mount
                for (GarrAbilityEntry const* entry : follower->PacketInfo.AbilityID)
                {
                    if (entry && entry->ID == 221)
                        duration *= 0.5f;
                }
            }
        }
    }*/

    return duration;
}

void Garrison::RewardMission(uint32 missionRecID)
{
    if (GarrMissionRewardEntry const* entry = sDB2Manager.GetMissionRewardByRecID(missionRecID))
    {
        if (entry->HasMoneyReward())
            _owner->ModifyMoney(entry->CurrencyValue);

        if (entry->HasCurrencyReward())
            _owner->ModifyCurrency(entry->CurrencyID, entry->CurrencyValue);

        if (entry->HasItemReward())
        {
            ItemPosCountVec dest;
            if (_owner->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, entry->RewardItemID, entry->ItemAmount) == EQUIP_ERR_OK)
            {
                if (Item* item = _owner->StoreNewItem(dest, entry->RewardItemID, true, Item::GenerateItemRandomPropertyId(entry->RewardItemID)))
                    _owner->SendNewItem(item, entry->ItemAmount, true, false);
            }
        }
    }

    if (GarrMissionEntry const* mission = sGarrisonMgr.GetNextMissionInQuestLine(missionRecID))
        AddMission(mission->ID);
}

/*
The Garrison Cache next to your Town Hall accumulates  Garrison Resources (GR) 
at a rate of 1 GR every 10 minutes of real time (6 per hour),
which works out to 144 GR every full day (6 x 24hrs = 144 GR).

За 3-е суток и 10 часов будет достигнут лимит в 500 ресурсов.

При покупке и использовании  Торговое соглашение: араккоа-изгои (продается в Танаанских джунглях) лимит склада становится равным 1000 ресурсов. = 6 суток 20 часов.
*/
uint32 Garrison::GetResNumber() const
{
    // ToDo: set get congig
    #define default_resource_num 50
    #define limit_cap 500
    #define min_counter 10

    if (!_lastResTaken)
        return default_resource_num;

    uint32 res = (time(NULL) - _lastResTaken) / (min_counter * MINUTE);
    return res > limit_cap ? limit_cap : res;
}

void Garrison::UpdateResTakenTime()
{
    _lastResTaken = time(NULL);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    SaveToDB(trans);
    CharacterDatabase.CommitTransaction(trans);
}

//! Activate some buildings at finishing quests as complete spells not exist on dbc. For example: spellID 165077
void Garrison::OnQuestReward(uint32 questID)
{
    switch (questID)
    {
        // Open Herb Garden PlotID = 63 & BuildID = 29
        case 36404:
        case 34193:
            PlaceBuilding(63, 29, true);
            break;
        // Open Mines PlotID = 59 & BuildID = 61
        case 35154:
        case 34192:
            PlaceBuilding(59, 61, true);
            break;
        // Open Fishing PlotID = 67 & BuildID = 64
        case 36870:
        case 36612:
            PlaceBuilding(67, 64, true);
            break;
        // Open Pet Menagerie PlotID = 81 & BuildID = 42
        case 36483:
        case 36662:
            PlaceBuilding(81, 42, true);
            break;
        default:
            break;
    }
}

bool Garrison::canAddShipmentOrder(Creature* source)
{
    if (!source->HasFlag(UNIT_FIELD_NPC_FLAGS2, UNIT_NPC_FLAG2_SHIPMENT_ORDER))
        return false;

    GarrShipment const* data = sGarrisonMgr.GetGarrShipment(source->GetEntry(), SHIPMENT_GET_BY_NPC);
    if (!data)
        return false;

    // check if we have aready build building.
    GarrBuildingEntry const* existingBuilding;
    for (auto const& p : _plots)
    {
        if (p.second.BuildingInfo.PacketInfo && p.second.BuildingInfo.PacketInfo->Active)
        {
            existingBuilding = sGarrBuildingStore.AssertEntry(p.second.BuildingInfo.PacketInfo->GarrBuildingID);
            if (existingBuilding->Type == data->cEntry->BuildingType)
                return true;
        }
    }

    return false;
}

uint32 Garrison::GetPlotInstanceForBuildingType(uint32 type) const
{
    // check if we have aready build building.
    GarrBuildingEntry const* existingBuilding;
    for (auto const& p : _plots)
    {
        if (p.second.BuildingInfo.PacketInfo)
        {
            existingBuilding = sGarrBuildingStore.AssertEntry(p.second.BuildingInfo.PacketInfo->GarrBuildingID);
            if (existingBuilding->Type == type)
                return p.second.BuildingInfo.PacketInfo->GarrPlotInstanceID;
        }
    }
    return 0;
}

void Garrison::OnGossipSelect(WorldObject* source)
{
    if (!source->HasFlag(UNIT_FIELD_NPC_FLAGS2, UNIT_NPC_FLAG2_SHIPMENT_ORDER))
        return;

    GarrShipment const* data = sGarrisonMgr.GetGarrShipment(source->GetEntry(), SHIPMENT_GET_BY_NPC);
    if (!data)
        return;

    //SMSG_OPEN_SHIPMENT_NPC_FROM_GOSSIP
    WorldPackets::Garrison::OpenShipmentNPCFromGossip openShipment;
    openShipment.NpcGUID = source->GetGUID();
    openShipment.CharShipmentContainerID = data->data->ShipmentConteinerID;
    _owner->SendDirectMessage(openShipment.Write());
}

void Garrison::SendShipmentInfo(ObjectGuid const& guid)
{
    GarrShipment const* shipment = sGarrisonMgr.GetGarrShipment(guid.GetEntry(), SHIPMENT_GET_BY_NPC);
    const Garrison::Plot* plot = GetPlotWithBuildingType(shipment->cEntry->BuildingType);

    //SMSG_GET_SHIPMENT_INFO_RESPONSE
    WorldPackets::Garrison::GetShipmentInfoResponse shipmentResponse;
    if (shipment && plot)
    {
        GarrBuildingEntry const* existingBuilding = sGarrBuildingStore.AssertEntry(plot->BuildingInfo.PacketInfo->GarrBuildingID);

        shipmentResponse.ShipmentID = shipment->ShipmentID;
        shipmentResponse.MaxShipments = existingBuilding->MaxShipments;
        shipmentResponse.PlotInstanceID = plot->BuildingInfo.PacketInfo->GarrPlotInstanceID;

        shipmentResponse.Shipments.assign(_shipments[shipment->cEntry->BuildingType].begin(), _shipments[shipment->cEntry->BuildingType].end());
    }
    shipmentResponse.Success = shipment && plot;

    _owner->SendDirectMessage(shipmentResponse.Write());
}

void Garrison::CreateShipment(ObjectGuid const& guid, uint32 count)
{
    GarrShipment const* shipment = sGarrisonMgr.GetGarrShipment(guid.GetEntry(), SHIPMENT_GET_BY_NPC);
    if (!shipment)
        return;

    const Garrison::Plot* plot = GetPlotWithBuildingType(shipment->cEntry->BuildingType);
    if (!plot)
        return;

    CharShipmentEntry const* shipmentEntry = sCharShipmentStore.LookupEntry(shipment->ShipmentID);
    if (!shipmentEntry)
        return;

    for (uint32 i = 0; i < count; ++i)
        _owner->CastSpell(_owner, shipmentEntry->SpellCreation, false);
}

void Garrison::CreateGarrisonShipment(uint32 shipmentID)
{
    uint64 dbID = PlaceShipment(shipmentID, time(NULL));

    //SMSG_CREATE_SHIPMENT_RESPONSE
    WorldPackets::Garrison::CreateShipmentResponse shipmentResponse;
    shipmentResponse.ShipmentRecID = shipmentID;
    shipmentResponse.ShipmentID = dbID;
    shipmentResponse.Result = dbID ? 0 : 1;    //0 - OK

    _owner->SendDirectMessage(shipmentResponse.Write());
}

uint64 Garrison::PlaceShipment(uint32 shipmentID, uint32 placeTime, uint64 dbID/* = 0*/)
{
    CharShipmentEntry const* shipmentEntry = sCharShipmentStore.LookupEntry(shipmentID);
    if (!shipmentEntry)
        return 0;

    CharShipmentConteiner const* shipmentConteinerEntry = sCharShipmentContainerStore.LookupEntry(shipmentEntry->ShipmentConteinerID);
    if (!shipmentConteinerEntry)
        return 0;

    Garrison::Plot* plot = GetPlotWithBuildingType(shipmentConteinerEntry->BuildingType);
    if (!plot)
        return 0;

    GarrBuildingEntry const* existingBuilding = sGarrBuildingStore.AssertEntry(plot->BuildingInfo.PacketInfo->GarrBuildingID);
    if (!existingBuilding)
        return 0;

    if (_shipments[shipmentConteinerEntry->BuildingType].size() >= existingBuilding->MaxShipments)
        return 0;

    uint32 EndShipment = shipmentEntry->TimeForShipment + placeTime;
    uint32 lastFinishTime = 0;

    // find last finishing time.
    for (auto const& x : _shipments[shipmentConteinerEntry->BuildingType])
    {
        uint32 end = x.CreationTime + x.ShipmentDuration;
        if (!lastFinishTime || end > lastFinishTime)
            lastFinishTime = end;
    }

    // last in progress. so out complition is incrased.
    if (lastFinishTime > placeTime)
        EndShipment = lastFinishTime + shipmentEntry->TimeForShipment;

    WorldPackets::Garrison::Shipment _s;
    _s.ShipmentID = dbID ? dbID : sGarrisonMgr.GenerateShipmentDbId();
    _s.ShipmentRecID = shipmentID;
    _s.BuildingTypeID = shipmentConteinerEntry->BuildingType;
    _s.CreationTime = time_t(placeTime);
    _s.ShipmentDuration = EndShipment - ::time(nullptr);

    _shipments[shipmentConteinerEntry->BuildingType].push_back(_s);
    return _s.ShipmentID;
}

void Garrison::SendGarrisonShipmentLandingPage()
{
    WorldPackets::Garrison::GarrisonLandingPage packet;

    for (auto const& x : _shipments)
    {
        for (auto const& shipment : x.second)
            packet.MsgData.push_back(shipment);
    }
    _owner->SendDirectMessage(packet.Write());
}

void Garrison::CompleteShipments(GameObject *go)
{
    CharShipmentConteiner const* shipmentConteinerEntry = sCharShipmentContainerStore.LookupEntry(go->GetGOInfo()->garrisonShipment.ShipmentContainer);
    if (!shipmentConteinerEntry)
        return;
 
    std::map<uint32, uint32> loot_items;
    for (auto const& sh : _shipments[shipmentConteinerEntry->BuildingType])
        if (!sh.ShipmentDuration)
        {
            if (CharShipmentEntry const* shipmentEntry = sCharShipmentStore.LookupEntry(sh.ShipmentRecID))
                ++loot_items[shipmentEntry->ShipmentResultItemID];
        }

    // This is hardcode of Player::SendLoot
    Loot* loot = &go->loot;

    if (loot_items.empty())
    {
        WorldPackets::Loot::LootResponse packet;
        packet.LootObj = loot->GetGUID();
        packet.AcquireReason = LOOT_CORPSE;
        packet.Owner = go->GetGUID();
        packet.AELooting = false;
        loot->BuildLootResponse(packet, _owner, ALL_PERMISSION, ITEM_QUALITY_POOR);

        _owner->SendDirectMessage(packet.Write());
        return;
    }

    loot->shipmentBuildingType = shipmentConteinerEntry->BuildingType;

    // fill loot only once... after we should add only continue.
    bool update = !loot->items.empty();
    if (!update)
    {
        loot->clear();
        loot->objType = 3;

        loot->FillLoot(go->GetEntry(), LootTemplates_Gameobject, _owner, true, false, go);
        loot->AddLooter(_owner->GetGUID());
    }
    
    for (auto data : loot_items)
    {
        //HARDCORE.
        switch (data.first)
        {
            //Mine Shipment
            case 116055:
            {
                loot->AddOrReplaceItem(109119, data.second, false);
                loot->AddOrReplaceItem(109118, data.second, false);
                break;
            }
            //Garden Shipment
            case 116054:
                loot->AddOrReplaceItem(109125, data.second, false);
                loot->AddOrReplaceItem(109124, data.second, false);
                break;
            case 118111:
                loot->AddOrReplaceItem(CURRENCY_TYPE_GARRISON_RESOURCES, data.second * 30, true);
                break;
            default:
                loot->AddOrReplaceItem(data.first, data.second, false);
                break;
        }
    }

    WorldPackets::Loot::LootResponse packet;
    packet.LootObj = loot->GetGUID();
    packet.AcquireReason = LOOT_CORPSE;
    packet.Owner = go->GetGUID();
    packet.AELooting = false;
    loot->BuildLootResponse(packet, _owner, ALL_PERMISSION, ITEM_QUALITY_POOR);

    _owner->SendDirectMessage(packet.Write());

    _owner->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOOTING);
    go->SetLootState(GO_ACTIVATED, _owner);
}

void Garrison::FreeShipmentChest(uint32 shipmentConteinerBuildingType)
{
    ShipmentSet &set = _shipments[shipmentConteinerBuildingType];
    for (ShipmentSet::iterator itr = set.begin(); itr != set.end();)
    {
        if (!itr->ShipmentDuration)
        {
            set.erase(itr++);
        }
        else
            ++itr;
    }
}