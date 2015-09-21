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
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "Corpse.h"
#include "Player.h"
#include "SpellAuras.h"
#include "MapManager.h"
#include "Transport.h"
#include "Battleground.h"
#include "WaypointMovementGenerator.h"
#include "InstanceSaveMgr.h"
#include "ObjectMgr.h"
#include "VMapFactory.h"
#include "Vehicle.h"
#include "MovementPackets.h"
#include "AccountMgr.h"

#define MOVEMENT_PACKET_TIME_DELAY 0

void WorldSession::HandleMoveWorldportAckOpcode(WorldPacket& /*recvPacket*/)
{
    HandleMoveWorldportAckOpcode();
}

void WorldSession::HandleMoveWorldportAckOpcode()
{
    if (!GetPlayer()->IsBeingTeleportedFar())
        return;

    GetPlayer()->SetSemaphoreTeleportFar(false);
    if(Unit* mover = _player->m_mover)
        mover->ClearUnitState(UNIT_STATE_JUMPING);

    WorldLocation const loc = GetPlayer()->GetTeleportDest();
    if (!MapManager::IsValidMapCoord(loc))
    {
        LogoutPlayer(false);
        return;
    }

    MapEntry const* mEntry = sMapStore.LookupEntry(loc.GetMapId());
    InstanceTemplate const* mInstance = sObjectMgr->GetInstanceTemplate(loc.GetMapId());

    if (GetPlayer()->m_InstanceValid == false && !mInstance)
        GetPlayer()->m_InstanceValid = true;

    Map* oldMap = GetPlayer()->GetMap();
    if (GetPlayer()->IsInWorld())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Player (Name %s) is still in world when teleported from map %u to new map %u", GetPlayer()->GetName(), oldMap->GetId(), loc.GetMapId());
        oldMap->RemovePlayerFromMap(GetPlayer(), false);
    }

    Map* newMap = sMapMgr->CreateMap(loc.GetMapId(), GetPlayer());
    if (!newMap || !newMap->CanEnter(GetPlayer()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Map %d could not be created for player %d, porting player to homebind", loc.GetMapId(), GetPlayer()->GetGUID().GetCounter());
        GetPlayer()->TeleportTo(GetPlayer()->m_homebindMapId, GetPlayer()->m_homebindX, GetPlayer()->m_homebindY, GetPlayer()->m_homebindZ, GetPlayer()->GetOrientation());
        return;
    }
    else
        GetPlayer()->Relocate(&loc);

    GetPlayer()->ResetMap();
    GetPlayer()->SetMap(newMap);

    GetPlayer()->SendInitialPacketsBeforeAddToMap();
    if (!GetPlayer()->GetMap()->AddPlayerToMap(GetPlayer()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: failed to teleport player %s (%d) to map %d because of unknown reason!", GetPlayer()->GetName(), GetPlayer()->GetGUID().GetCounter(), loc.GetMapId());
        GetPlayer()->ResetMap();
        GetPlayer()->SetMap(oldMap);
        GetPlayer()->TeleportTo(GetPlayer()->m_homebindMapId, GetPlayer()->m_homebindX, GetPlayer()->m_homebindY, GetPlayer()->m_homebindZ, GetPlayer()->GetOrientation());
        return;
    }

    if (_player->InBattleground())
    {
        if (!mEntry->IsBattlegroundOrArena())
        {
            if (Battleground* bg = _player->GetBattleground())
                _player->LeaveBattleground(false);

            _player->SetBattlegroundId(0, BATTLEGROUND_TYPE_NONE);
            _player->SetBGTeam(0);
        }
        else if (Battleground* bg = _player->GetBattleground())
        {
            if (_player->IsInvitedForBattlegroundInstance(_player->GetBattlegroundId()))
                bg->AddPlayer(_player);
        }
    }

    GetPlayer()->SendInitialPacketsAfterAddToMap();

    _player->m_movementInfo.time = getMSTime();
    _player->m_movementInfo.pos.m_positionX = loc.m_positionX;
    _player->m_movementInfo.pos.m_positionY = loc.m_positionY;
    _player->m_movementInfo.pos.m_positionZ = loc.m_positionZ;
    _player->m_movementInfo.pos.m_orientation = loc.m_orientation;

    WorldPackets::Movement::MoveUpdate playerMovement;
    playerMovement.movementInfo = &_player->m_movementInfo;
    _player->SendMessageToSet(playerMovement.Write(), _player);

    if (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
    {
        if (!_player->InBattleground())
        {
            FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(GetPlayer()->GetMotionMaster()->top());
            flight->Initialize(*GetPlayer());
            return;
        }

        GetPlayer()->GetMotionMaster()->MovementExpired();
        GetPlayer()->CleanupAfterTaxiFlight();
    }

    Corpse* corpse = GetPlayer()->GetCorpse();
    if (corpse && corpse->GetType() != CORPSE_BONES && corpse->GetMapId() == GetPlayer()->GetMapId())
    {
        if (mEntry->IsDungeon())
        {
            GetPlayer()->ResurrectPlayer(0.5f, false);
            GetPlayer()->SpawnCorpseBones();
        }
    }

    bool allowMount = !mEntry->IsDungeon() || mEntry->IsBattlegroundOrArena();
    if (mInstance)
    {
        Difficulty diff = GetPlayer()->GetDifficultyID(mEntry);
        if (MapDifficulty const* mapDiff = GetMapDifficultyData(mEntry->MapID, diff))
        {
            if (mapDiff->resetTime)
            {
                if (time_t timeReset = sWorld->getInstanceResetTime(mapDiff->resetTime))
                {
                    uint32 timeleft = uint32(timeReset - time(NULL));
                    GetPlayer()->SendInstanceResetWarning(mEntry->MapID, diff, timeleft);
                }
            }
        }
        allowMount = mInstance->AllowMount;
    }

    if (!allowMount || (GetPlayer()->GetMapId() == 530 && GetPlayer()->GetZoneId() == 0)) //530 - uwow event map
        _player->RemoveAurasByType(SPELL_AURA_MOUNTED);

    uint32 newzone, newarea;
    GetPlayer()->GetZoneAndAreaId(newzone, newarea);
    GetPlayer()->UpdateZone(newzone, newarea);

    if (GetPlayer()->pvpInfo.inHostileArea)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);
    else if (GetPlayer()->IsPvP() && !GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
        GetPlayer()->UpdatePvP(false, false);

    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMoveTeleportAck(WorldPackets::Movement::MoveTeleportAck& packet)
{
    Player* plMover = _player->m_mover->ToPlayer();

    if (!plMover || !plMover->IsBeingTeleportedNear())
        return;

    if (packet.MoverGUID != plMover->GetGUID())
        return;

    plMover->SetSemaphoreTeleportNear(false);

    uint32 old_zone = plMover->GetZoneId();

    WorldLocation const& dest = plMover->GetTeleportDest();

    plMover->UpdatePosition(dest, true);

    uint32 newzone, newarea;
    plMover->GetZoneAndAreaId(newzone, newarea);
    plMover->UpdateZone(newzone, newarea);

    if (old_zone != newzone)
    {
        if (plMover->pvpInfo.inHostileArea)
            plMover->CastSpell(plMover, 2479, true);

        else if (plMover->IsPvP() && !plMover->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
            plMover->UpdatePvP(false, false);
    }

    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();
    GetPlayer()->ProcessDelayedOperations();

    if(Unit* mover = _player->m_mover)
    {
        mover->m_movementInfo.time = getMSTime();
        mover->m_movementInfo.pos.m_positionX = mover->GetPositionX();
        mover->m_movementInfo.pos.m_positionY = mover->GetPositionY();
        mover->m_movementInfo.pos.m_positionZ = mover->GetPositionZ();

        WorldPackets::Movement::MoveUpdate playerMovement;
        playerMovement.movementInfo = &mover->m_movementInfo;
        mover->SendMessageToSet(playerMovement.Write(), mover);

        mover->ClearUnitState(UNIT_STATE_JUMPING);
    }
}

void WorldSession::HandleMovementOpcodes(WorldPackets::Movement::ClientPlayerMovement& packet)
{
    OpcodeClient opcode = packet.GetOpcode();

    uint32 diff = sWorld->GetUpdateTime();
    Unit* mover = _player->m_mover;

    if(!mover || mover == NULL)
        return;

    Player* plrMover = mover->ToPlayer();

    Vehicle *vehMover = mover->GetVehicleKit();
    if (vehMover)
        if (mover->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED))
            if (Unit *charmer = mover->GetCharmer())
                if (charmer->GetTypeId() == TYPEID_PLAYER)
                    plrMover = (Player*)charmer;

    uint8 forvehunit = 0;
    if(plrMover && plrMover->GetTypeId() == TYPEID_PLAYER && plrMover->GetVehicle())
        if(Unit* VehUnit = plrMover->GetVehicle()->GetBase())
            if(VehUnit->HasUnitMovementFlag(MOVEMENTFLAG_CAN_FLY) || VehUnit->HasUnitMovementFlag(MOVEMENTFLAG_FLYING))
                forvehunit = 1;

    if (plrMover && plrMover->IsBeingTeleported())
        return;

    if (plrMover && (plrMover->GetUInt32Value(UNIT_NPC_EMOTESTATE) != 0))
        plrMover->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);

    MovementInfo& movementInfo = packet.movementInfo;
    GetPlayer()->ValidateMovementInfo(&movementInfo);

    if (movementInfo.guid != mover->GetGUID() || !mover->IsInWorld())
        return;

    if (!movementInfo.pos.IsPositionValid())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandleMovementOpcodes: Invalid Position");
        return;
    }

    if(mover->HasAuraType(SPELL_AURA_MOD_POSSESS) || (plrMover && plrMover->HasAuraType(SPELL_AURA_MOD_POSSESS)))
    {
        if (movementInfo.flags & MOVEMENTFLAG_WALKING)
            movementInfo.flags &= ~MOVEMENTFLAG_WALKING;
    }

    if (movementInfo.transport.guid)
    {
        if(World::GetEnableMvAnticheatDebug())
            sLog->outError(LOG_FILTER_NETWORKIO, "HandleMovementOpcodes t_guid %s, opcode[%s]", movementInfo.transport.guid.ToString().c_str(), GetOpcodeNameForLogging(opcode).c_str());

        if (movementInfo.transport.pos.GetPositionX() > 50 || movementInfo.transport.pos.GetPositionY() > 50 || movementInfo.transport.pos.GetPositionZ() > 50)
            return;

        if (!Trinity::IsValidMapCoord(movementInfo.pos.GetPositionX() + movementInfo.transport.pos.GetPositionX(), movementInfo.pos.GetPositionY() + movementInfo.transport.pos.GetPositionY(),
            movementInfo.pos.GetPositionZ() + movementInfo.transport.pos.GetPositionZ(), movementInfo.pos.GetOrientation() + movementInfo.transport.pos.GetOrientation()))
            return;

        if (plrMover)
        {
            if (!plrMover->GetTransport())
            {
                for (MapManager::TransportSet::const_iterator iter = sMapMgr->m_Transports.begin(); iter != sMapMgr->m_Transports.end(); ++iter)
                {
                    if ((*iter)->GetGUID() == movementInfo.transport.guid)
                    {
                        plrMover->m_transport = *iter;
                        (*iter)->AddPassenger(plrMover);
                        break;
                    }
                }

                if (!plrMover->m_transport)
                    if (Map *tempMap = mover->GetMap())
                        if (GameObject *tempTransport = tempMap->GetGameObject(movementInfo.transport.guid))
                            if (tempTransport->IsTransport())
                                plrMover->m_temp_transport = tempTransport;
            }
            else if (plrMover->GetTransport()->GetGUID() != movementInfo.transport.guid)
            {
                bool foundNewTransport = false;
                plrMover->m_transport->RemovePassenger(plrMover);
                for (MapManager::TransportSet::const_iterator iter = sMapMgr->m_Transports.begin(); iter != sMapMgr->m_Transports.end(); ++iter)
                {
                    if ((*iter)->GetGUID() == movementInfo.transport.guid)
                    {
                        foundNewTransport = true;
                        plrMover->m_transport = *iter;
                        (*iter)->AddPassenger(plrMover);
                        break;
                    }
                }

                if (!foundNewTransport)
                {
                    plrMover->m_transport = NULL;
                    plrMover->m_movementInfo.transport.Reset();
                }
            }
        }

        if (!mover->GetTransport() && !mover->GetVehicle())
        {
            GameObject* go = mover->GetMap()->GetGameObject(movementInfo.transport.guid);
            if (!go || go->GetGoType() != GAMEOBJECT_TYPE_TRANSPORT)
                movementInfo.transport.guid.Clear();
        }
    }
    else if (plrMover && (plrMover->m_transport || plrMover->m_temp_transport))
    {
        if (plrMover->m_transport)
        {
            plrMover->m_transport->RemovePassenger(plrMover);
            plrMover->m_transport = NULL;
        }
        plrMover->m_temp_transport = NULL;
        movementInfo.transport.Reset();
    }

    if (plrMover && plrMover->m_movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING_FAR) && !movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING_FAR) && plrMover && !plrMover->isInFlight())
    {
        plrMover->m_anti_JumpCount = 0;
        plrMover->m_anti_JumpBaseZ = 0;
        if(!plrMover->Zliquid_status)
            plrMover->HandleFall(movementInfo);
    }

    if (plrMover && ((movementInfo.flags & MOVEMENTFLAG_SWIMMING) != 0) != plrMover->IsInWater())
        plrMover->SetInWater(!plrMover->IsInWater() || plrMover->GetBaseMap()->IsUnderWater(movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY(), movementInfo.pos.GetPositionZ()));

    if (plrMover && mover)
    {
        Vehicle *veh = mover->GetVehicleKit();
        if (veh && veh->GetBase())
        {
            if (Creature * vehCreature = veh->GetBase()->ToCreature())
            {
                if (!vehCreature->isInAccessiblePlaceFor(vehCreature))
                    plrMover->ExitVehicle();
            }
        }
    }

    /*----------------------*/
    // begin anti cheat
    bool check_passed = true;
    if(World::GetEnableMvAnticheatDebug())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "AC2-%s > time: %d fall-time: %d | xyzo: %f, %f, %fo(%f) flags[%X] flags2[%X] UnitState[%X] opcode[%s] | mover (xyzo): %f, %f, %fo(%f)",
            plrMover->GetName(), movementInfo.time, movementInfo.fallTime, movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY(), movementInfo.pos.GetPositionZ(), movementInfo.pos.GetOrientation(),
            movementInfo.flags, movementInfo.flags2, mover->GetUnitState(), GetOpcodeNameForLogging(static_cast<OpcodeClient>(opcode)).c_str(), mover->GetPositionX(), mover->GetPositionY(), mover->GetPositionZ(), mover->GetOrientation());
    }

    if (plrMover && plrMover->GetTypeId() == TYPEID_PLAYER && !plrMover->HasUnitState(UNIT_STATE_LOST_CONTROL) &&
        !plrMover->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_TAXI_FLIGHT) && 
        mover->GetMotionMaster()->GetCurrentMovementGeneratorType() != POINT_MOTION_TYPE &&
        !(plrMover->m_transport || plrMover->m_temp_transport) && (plrMover->GetMapId() != 578 || plrMover->GetMapId() != 603))
    {
        float speed_plus = 1.5f;
        bool m_anty_check = true;
        bool m_anty_vechicle = plrMover->GetCharmerOrOwnerPlayerOrPlayerItself()->GetVehicle();
        bool m_anty_possess = plrMover->HasAuraType(SPELL_AURA_MOD_POSSESS);
        float delta_check_plus = 0.0f;
        if(m_anty_vechicle || m_anty_possess)
            delta_check_plus = 200.0f;

        if (World::GetEnableMvAnticheat() && plrMover->GetCharmerOrOwnerPlayerOrPlayerItself()->GetSession()->GetSecurity() < SEC_GAMEMASTER)
        {
            //check CX walkmode + time sinc
            if (movementInfo.flags & MOVEMENTFLAG_WALKING && _player->m_anti_MistiCount == 1)
            {
                plrMover->m_anti_MistiCount = 0;
                check_passed = false;
            }
            //end

            // calc time deltas
            int32 cClientTimeDelta = 1500;
            if (plrMover->m_anti_LastClientTime != 0)
            {
                cClientTimeDelta = movementInfo.time - plrMover->m_anti_LastClientTime;
                plrMover->m_anti_DeltaClientTime += cClientTimeDelta;
                plrMover->m_anti_LastClientTime = movementInfo.time;
            }
            else
                plrMover->m_anti_LastClientTime = movementInfo.time;
 
            const uint64 cServerTime = getMSTime();
            uint32 cServerTimeDelta = 1500;
            if (plrMover->m_anti_LastServerTime != 0)
            {
                cServerTimeDelta = cServerTime - plrMover->m_anti_LastServerTime;
                plrMover->m_anti_DeltaServerTime += cServerTimeDelta;
                plrMover->m_anti_LastServerTime = cServerTime;
            }
            else
                plrMover->m_anti_LastServerTime = cServerTime;

            // resync times on client login (first 15 sec for heavy areas)
            if (plrMover->m_anti_DeltaServerTime < 15000 && plrMover->m_anti_DeltaClientTime < 15000)
                plrMover->m_anti_DeltaClientTime = plrMover->m_anti_DeltaServerTime;

            const int32 sync_time = plrMover->m_anti_DeltaClientTime - plrMover->m_anti_DeltaServerTime;

            if(World::GetEnableMvAnticheatDebug())
                sLog->outError(LOG_FILTER_NETWORKIO, "AC2-%s Time > cClientTimeDelta: %d, cServerTime: %d | deltaC: %d - deltaS: %d | SyncTime: %d, opcode[%s]",
                plrMover->GetName(), cClientTimeDelta, cServerTime, plrMover->m_anti_DeltaClientTime, plrMover->m_anti_DeltaServerTime, sync_time, GetOpcodeNameForLogging(static_cast<OpcodeClient>(opcode)).c_str());

            // mistiming checks
            const int32 GetMistimingDelta = abs(int32(World::GetMistimingDelta()));
            if (sync_time > GetMistimingDelta)
            {
                cClientTimeDelta = cServerTimeDelta;
                ++(plrMover->m_anti_MistimingCount);

                const bool bMistimingModulo = plrMover->m_anti_MistimingCount % 50 == 0;

                if (bMistimingModulo)
                {
                    if(World::GetEnableMvAnticheatDebug())
                        sLog->outError(LOG_FILTER_NETWORKIO, "AC2-%s, mistiming exception #%d, mistiming: %dms, opcode[%s]", plrMover->GetName(), plrMover->m_anti_MistimingCount, sync_time, GetOpcodeNameForLogging(static_cast<OpcodeClient>(opcode)).c_str());

                    check_passed = false;
                }                   
            }
            // end mistiming checks

            const uint32 curDest = plrMover->m_taxi.GetTaxiDestination(); // check taxi flight
            if (!curDest)
            {
                // calculating section
                // current speed
                if(plrMover->HasAuraType(SPELL_AURA_FEATHER_FALL))
                    speed_plus = 7.0f;
                if(plrMover->HasAura(19503))
                    speed_plus = 15.0f;
                if(plrMover->HasAura(2983) || plrMover->HasAura(48594) || plrMover->HasAura(56354) || plrMover->HasAura(32720) || plrMover->HasAura(3714))
                    speed_plus = 4.0f;

                float current_speed = mover->GetSpeed(MOVE_RUN) > mover->GetSpeed(MOVE_FLIGHT) ? mover->GetSpeed(MOVE_RUN) : mover->GetSpeed(MOVE_FLIGHT);
                if(current_speed < mover->GetSpeed(MOVE_SWIM))
                    current_speed = mover->GetSpeed(MOVE_SWIM);
                current_speed *= speed_plus + mover->m_TempSpeed;
                bool speed_check = true;

                if(mover->m_anti_JupmTime && mover->m_anti_JupmTime > 0)
                {
                    plrMover->m_anti_LastSpeedChangeTime = movementInfo.time + mover->m_anti_JupmTime;
                    speed_check = false;

                    if(mover->m_anti_JupmTime <= diff)
                    {
                        mover->m_anti_JupmTime = 0;
                        speed_check = true;
                    }
                    else
                        mover->m_anti_JupmTime -= diff;
                }
                // end current speed

                // movement distance
                const float delta_x = (plrMover->m_transport || plrMover->m_temp_transport) ? 0 : mover->GetPositionX() - movementInfo.pos.GetPositionX();
                const float delta_y = (plrMover->m_transport || plrMover->m_temp_transport) ? 0 : mover->GetPositionY() - movementInfo.pos.GetPositionY();
                const float delta_z = (plrMover->m_transport || plrMover->m_temp_transport) ? 0 : mover->GetPositionZ() - movementInfo.pos.GetPositionZ();
                const float real_delta = (plrMover->m_transport || plrMover->m_temp_transport) ? 0 : (pow(delta_x, 2) + pow(delta_y, 2));
                 // end movement distance

                const bool fly_auras = (plrMover->HasAuraType(SPELL_AURA_FLY) || plrMover->HasAuraType(SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED)
                    || plrMover->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) || plrMover->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED)
                    || plrMover->HasAuraType(SPELL_AURA_MOD_MOUNTED_FLIGHT_SPEED_ALWAYS));
                const bool fly_flags = (movementInfo.flags & (MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING | MOVEMENTFLAG_DISABLE_GRAVITY | MOVEMENTFLAG_ASCENDING));
                bool exeption_fly = true;

                if(mover->m_anti_FlightTime && mover->m_anti_FlightTime > 0)
                {
                    if (!fly_auras && fly_flags)
                        exeption_fly = false;

                    if(mover->m_anti_FlightTime <= diff)
                    {
                        mover->m_anti_FlightTime = 0;
                        exeption_fly = true;
                    }
                    else
                        mover->m_anti_FlightTime -= diff;
                }

                float _vmapHeight = 0.0f;
                float _Height = 0.0f;
                //if in fly crash on check VmapHeight
                if(!fly_auras)
                {
                    _vmapHeight = plrMover->GetMap()->GetVmapHeight(movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY(), movementInfo.pos.GetPositionZ());
                    _Height = plrMover->GetMap()->GetHeight(movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY(), movementInfo.pos.GetPositionZ());
                }
                const float ground_Z = movementInfo.pos.GetPositionZ() - _vmapHeight;

                if(cClientTimeDelta == 0)
                    cClientTimeDelta = 1500;
                if (cClientTimeDelta < 0)
                    cClientTimeDelta = 0;
                const float time_delta = cClientTimeDelta < 1500 ? float(cClientTimeDelta)/1000.0f : 1.5f; // normalize time - 1.5 second allowed for heavy loaded server

                const float tg_z = (real_delta != 0 && !fly_auras && !plrMover->Zliquid_status) ? (pow(delta_z, 2) / real_delta) : -99999; // movement distance tangents

                if (current_speed < plrMover->m_anti_Last_HSpeed && plrMover->m_anti_LastSpeedChangeTime == 0)
                    plrMover->m_anti_LastSpeedChangeTime = movementInfo.time + uint32(floor(((plrMover->m_anti_Last_HSpeed / current_speed) * 1500)) + 100); // 100ms above for random fluctuation

                const float allowed_delta = (plrMover->m_transport || plrMover->m_temp_transport) ? 2 : // movement distance allowed delta
                    pow(std::max(current_speed, plrMover->m_anti_Last_HSpeed) * time_delta, 2)
                    + 2                                                                             // minimum allowed delta
                    + (tg_z > 2.2 ? pow(delta_z, 2)/2.37f : 0);                                      // mountain fall allowed delta

                    if(World::GetEnableMvAnticheatDebug())
                        sLog->outError(LOG_FILTER_NETWORKIO, "AC444 out m_anti_JupmTime %u current_speed %f allowed_delta %f real_delta %f fly_auras %u fly_flags %u _vmapHeight %f, _Height %f, ZLiquidStatus %u, opcode[%s]",
                        mover->m_anti_JupmTime, current_speed, allowed_delta, real_delta, fly_auras, fly_flags, _vmapHeight, _Height, plrMover->Zliquid_status, GetOpcodeNameForLogging(static_cast<OpcodeClient>(opcode)).c_str());

                if (movementInfo.time > plrMover->m_anti_LastSpeedChangeTime)
                {
                    plrMover->m_anti_Last_HSpeed = current_speed;                                    // store current speed
                    plrMover->m_anti_Last_VSpeed = -2.3f;
                    plrMover->m_anti_LastSpeedChangeTime = 0;
                }
                // end calculating section

                // speed and teleport hack checks
                if (real_delta > (allowed_delta + delta_check_plus))
                {
                    if(World::GetEnableMvAnticheatDebug())
                        if (real_delta < 4900.0f)
                            sLog->outError(LOG_FILTER_NETWORKIO, "AC2-%s, speed exception | cDelta=%f aDelta=%f | cSpeed=%f lSpeed=%f deltaTime=%f, opcode[%s]", plrMover->GetName(), real_delta, allowed_delta, current_speed, plrMover->m_anti_Last_HSpeed, time_delta, GetOpcodeNameForLogging(static_cast<OpcodeClient>(opcode)).c_str());
                        else
                            sLog->outError(LOG_FILTER_NETWORKIO, "AC2-%s, teleport exception | cDelta=%f aDelta=%f | cSpeed=%f lSpeed=%f deltaTime=%f, opcode[%s]", plrMover->GetName(), real_delta, allowed_delta, current_speed, plrMover->m_anti_Last_HSpeed, time_delta, GetOpcodeNameForLogging(static_cast<OpcodeClient>(opcode)).c_str());

                    if(speed_check || real_delta > 4900.0f)
                        check_passed = false;

                    plrMover->FallGroundAnt();
                }
 
                // Fly hack checks
                if (!fly_auras && (fly_flags || ground_Z > 2.3f) && !forvehunit && exeption_fly && !plrMover->Zliquid_status)
                {
                    if(World::GetEnableMvAnticheatDebug())
                        sLog->outError(LOG_FILTER_NETWORKIO, "AC2-%s, flight exception. {SPELL_AURA_FLY=[%X]} {SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED=[%X]} {SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED=[%X]} {SPELL_AURA_MOD_MOUNTED_FLIGHT_SPEED_ALWAYS=[%X]} {SPELL_AURA_MOD_FLIGHT_SPEED_NOT_STACK=[%X]} {plrMover->GetVehicle()=[%X]} forvehunit=[%X], opcode[%s]",
                            plrMover->GetName(),
                            plrMover->HasAuraType(SPELL_AURA_FLY), plrMover->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED),
                            plrMover->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED), plrMover->HasAuraType(SPELL_AURA_MOD_MOUNTED_FLIGHT_SPEED_ALWAYS),
                            plrMover->HasAuraType(SPELL_AURA_MOD_FLIGHT_SPEED_NOT_STACK), plrMover->GetVehicle(), forvehunit, GetOpcodeNameForLogging(static_cast<OpcodeClient>(opcode)).c_str());

                    //check_passed = false;
                    plrMover->SetCanFly(true);
                    plrMover->SetCanFly(false);
                    //plrMover->FallGroundAnt();
                }

                // Teleport To Plane checks
                if (!plrMover->Zliquid_status && movementInfo.pos.GetPositionZ() < 0.0001f && movementInfo.pos.GetPositionZ() > -0.0001f)
                {
                    if (const Map *map = plrMover->GetMap())
                    {
                        float plane_z = map->GetHeight(movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY(), MAX_HEIGHT) - movementInfo.pos.GetPositionZ();
                        plane_z = (plane_z < -500.0f) ? 0.0f : plane_z; // check holes in height map
                        if (plane_z > 0.1f || plane_z < -0.1f)
                        {
                            if(World::GetEnableMvAnticheatDebug())
                                sLog->outError(LOG_FILTER_NETWORKIO, "AC2-%s, teleport to plane exception. plane_z: %f, opcode[%s]", plrMover->GetName(), plane_z, GetOpcodeNameForLogging(static_cast<OpcodeClient>(opcode)).c_str());

                            if(World::GetEnableMvAnticheatDebug())
                                if (plrMover->m_anti_TeleToPlane_Count > World::GetTeleportToPlaneAlarms())
                                    sLog->outError(LOG_FILTER_NETWORKIO, "AC2-%s, teleport to plane exception. Exception count: %d, opcode[%s]", plrMover->GetName(), plrMover->m_anti_TeleToPlane_Count, GetOpcodeNameForLogging(static_cast<OpcodeClient>(opcode)).c_str());

                            ++(plrMover->m_anti_TeleToPlane_Count);
                            check_passed = false;
                        }
                    }
                }
                else
                    plrMover->m_anti_TeleToPlane_Count = 0;
            }
        }
    }

    if (check_passed)
    {
        movementInfo.time = movementInfo.time + m_clientTimeDelay + MOVEMENT_PACKET_TIME_DELAY;

        movementInfo.guid = mover->GetGUID();
        mover->m_movementInfo = movementInfo;

        WorldPackets::Movement::MoveUpdate playerMovement;
        playerMovement.movementInfo = &mover->m_movementInfo;
        mover->SendMessageToSet(const_cast<WorldPacket*>(playerMovement.Write()), _player);

        if(opcode == CMSG_MOVE_FALL_LAND)
        {
            mover->ClearUnitState(UNIT_STATE_JUMPING);
            mover->m_TempSpeed = 0.0f;
        }

        if (mover->GetVehicle())
        {
            mover->SetOrientation(movementInfo.pos.GetOrientation());
            return;
        }

        mover->UpdatePosition(movementInfo.pos);

        if (plrMover)                                            // nothing is charmed, or player charmed
        {
            plrMover->UpdateFallInformationIfNeed(movementInfo, opcode);

            if (plrMover->m_movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING_FAR))
            {
                float groundZ = plrMover->GetMap()->GetGridMapHeigh(plrMover->GetPositionX(), plrMover->GetPositionY());
                if (groundZ == VMAP_INVALID_HEIGHT_VALUE)
                    groundZ = MAX_MAP_DEPTH;

                if ((groundZ - 500.0f) > movementInfo.pos.GetPositionZ())
                if (!(plrMover->GetBattleground() && plrMover->GetBattleground()->HandlePlayerUnderMap(_player)))
                {
                    // by CyberBrest: Nice! Best regards. Just kill man,  maybe return them to the grave??? 
                    // move to grave, and then kill.
                    plrMover->RepopAtGraveyard();

                    // NOTE: this is actually called many times while falling
                    // even after the player has been teleported away
                    // TODO: discard movement packets after the player is rooted
                    if (plrMover->isAlive())
                    {
                        plrMover->EnvironmentalDamage(DAMAGE_FALL_TO_VOID, GetPlayer()->GetMaxHealth());
                        if (!plrMover->isAlive())
                            plrMover->KillPlayer();
                    }
                }
            }
        }
    }
    else if (plrMover)
    {
        if (plrMover->m_transport)
        {
            plrMover->m_transport->RemovePassenger(plrMover);
            plrMover->m_transport = NULL;
        }
        plrMover->m_temp_transport = NULL;
        ++(plrMover->m_anti_AlarmCount);
        plrMover->SetUnitMovementFlags(0);

        WorldPackets::Movement::MoveUpdate playerMovement;
        playerMovement.movementInfo = &plrMover->m_movementInfo;
        plrMover->SendMessageToSet(playerMovement.Write(), true);

        plrMover->SetCanFly(true);
        plrMover->SetCanFly(false);
        plrMover->FallGroundAnt();
    }
}

void WorldSession::HandleForceSpeedChangeAck(WorldPackets::Movement::MovementSpeedAck& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    player->ValidateMovementInfo(&packet.Ack.movementInfo);

    if (player->GetGUID() != packet.Ack.movementInfo.guid)
        return;

    UnitMoveType move_type;

    OpcodeClient opcode = packet.GetOpcode();
    switch (opcode)
    {
        case CMSG_MOVE_FORCE_WALK_SPEED_CHANGE_ACK:        move_type = MOVE_WALK;        break;
        case CMSG_MOVE_FORCE_RUN_SPEED_CHANGE_ACK:         move_type = MOVE_RUN;         break;
        case CMSG_MOVE_FORCE_RUN_BACK_SPEED_CHANGE_ACK:    move_type = MOVE_RUN_BACK;    break;
        case CMSG_MOVE_FORCE_SWIM_SPEED_CHANGE_ACK:        move_type = MOVE_SWIM;        break;
        case CMSG_MOVE_FORCE_SWIM_BACK_SPEED_CHANGE_ACK:   move_type = MOVE_SWIM_BACK;   break;
        case CMSG_MOVE_FORCE_TURN_RATE_CHANGE_ACK:         move_type = MOVE_TURN_RATE;   break;
        case CMSG_MOVE_FORCE_FLIGHT_SPEED_CHANGE_ACK:      move_type = MOVE_FLIGHT;      break;
        case CMSG_MOVE_FORCE_FLIGHT_BACK_SPEED_CHANGE_ACK: move_type = MOVE_FLIGHT_BACK; break;
        case CMSG_MOVE_FORCE_PITCH_RATE_CHANGE_ACK:        move_type = MOVE_PITCH_RATE;  break;
        default:
            return;
    }

    if (player->m_forced_speed_changes[move_type] > 0)
    {
        --player->m_forced_speed_changes[move_type];
        if (player->m_forced_speed_changes[move_type] > 0)
            return;
    }

    if (!player->GetTransport() && std::fabs(player->GetSpeed(move_type) - packet.Speed) > 0.01f)
    {
        if (player->GetSpeed(move_type) > packet.Speed)
            player->SetSpeed(move_type, player->GetSpeedRate(move_type), true);
        else
            player->GetSession()->KickPlayer();
    }
}

void WorldSession::HandleMoveKnockBackAck(WorldPackets::Movement::MovementAckMessage& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    player->ValidateMovementInfo(&packet.Ack.movementInfo);

    if (player->m_mover->GetGUID() != packet.Ack.movementInfo.guid)
        return;

    player->m_movementInfo = packet.Ack.movementInfo;

    WorldPackets::Movement::MoveUpdateKnockBack updateKnockBack;
    updateKnockBack.movementInfo = &player->m_movementInfo;
    player->SendMessageToSet(updateKnockBack.Write(), false);
}

void WorldSession::HandleMovementAckMessage(WorldPackets::Movement::MovementAckMessage& packet)
{
    GetPlayer()->ValidateMovementInfo(&packet.Ack.movementInfo);
}

void WorldSession::HandleSetCollisionHeightAck(WorldPackets::Movement::MoveSetCollisionHeightAck& packet)
{
    GetPlayer()->ValidateMovementInfo(&packet.Data.movementInfo);
}

void WorldSession::HandleSetActiveMover(WorldPackets::Movement::SetActiveMover& packet)
{
    if (Player* player = GetPlayer())
        if (player->IsInWorld())
        {
            if (player->m_mover->GetGUID() != packet.ActiveMover)
                sLog->outError(LOG_FILTER_NETWORKIO, "HandleSetActiveMover: incorrect mover guid: mover is %s  and should be %s",
                packet.ActiveMover.ToString().c_str(), player->m_mover->GetGUID().ToString().c_str());
        }
}

void WorldSession::HandleMoveTimeSkipped(WorldPackets::Movement::MoveTimeSkipped& /*packet*/)
{
    if (Player* player = GetPlayer())
        if (player->m_mover->m_movementInfo.flags & MOVEMENTFLAG_WALKING)
            player->m_anti_MistiCount = 1;
}

