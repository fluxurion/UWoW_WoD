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

#include "Battleground.h"
#include "BattlegroundTTP.h"
#include "Language.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "WorldPacket.h"

BattlegroundTTP::BattlegroundTTP()
{
    BgObjects.resize(BG_TTP_OBJECT_MAX);

    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_ARENA_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_ARENA_THIRTY_SECONDS;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_ARENA_FIFTEEN_SECONDS;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_ARENA_HAS_BEGUN;
}

BattlegroundTTP::~BattlegroundTTP()
{

}

void BattlegroundTTP::StartingEventCloseDoors()
{
    for (uint32 i = BG_TTP_OBJECT_DOOR_1; i <= BG_TTP_OBJECT_DOOR_2; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);

    UpdateWorldState(static_cast<WorldStates>(8524), 0);
}

void BattlegroundTTP::StartingEventOpenDoors()
{
    for (uint32 i = BG_TTP_OBJECT_DOOR_1; i <= BG_TTP_OBJECT_DOOR_2; ++i)
        DoorOpen(i);
        
    for (uint32 i = BG_TTP_OBJECT_BUFF_1; i <= BG_TTP_OBJECT_BUFF_2; ++i)
        SpawnBGObject(i, 60);
        
    UpdateWorldState(static_cast<WorldStates>(8524), 1);
    UpdateWorldState(static_cast<WorldStates>(8529), int32(time(nullptr) + 1200));
}

void BattlegroundTTP::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    UpdateArenaWorldState();
}

void BattlegroundTTP::RemovePlayer(Player* /*player*/, ObjectGuid /*guid*/, uint32 /*team*/)
{
    if (GetStatus() == STATUS_WAIT_LEAVE)
        return;

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void BattlegroundTTP::HandleKillPlayer(Player* player, Player* killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (!killer)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundTTP: Killer player not found");
        return;
    }

    Battleground::HandleKillPlayer(player, killer);

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void BattlegroundTTP::HandleAreaTrigger(Player* player, uint32 trigger, bool entered)
{
    switch (trigger)
    {
        case 9127:
        case 9126:
            break;
        default:
            Battleground::HandleAreaTrigger(player, trigger, entered);
            break;
    }
}

void BattlegroundTTP::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(static_cast<WorldStates>(0xa11), 1);
    packet.Worldstates.emplace_back(static_cast<WorldStates>(3610), 1);
    packet.Worldstates.emplace_back(static_cast<WorldStates>(8524), (GetStatus() != STATUS_IN_PROGRESS ? 0 : 1));
    packet.Worldstates.emplace_back(static_cast<WorldStates>(8529), int32(time(nullptr) + std::chrono::duration_cast<Seconds>(Minutes(20) - GetArenaMinutesElapsed()).count()));
    UpdateArenaWorldState();
}

void BattlegroundTTP::Reset()
{
    Battleground::Reset();
}

bool BattlegroundTTP::SetupBattleground()
{
    // gates
    if (!AddObject(BG_TTP_OBJECT_DOOR_1, BG_TTP_OBJECT_TYPE_DOOR_1, 501.932f, 633.429f, 380.708f, 0.0262353f, 0, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TTP_OBJECT_DOOR_2, BG_TTP_OBJECT_TYPE_DOOR_2, 632.101f, 633.791f, 380.704f, 3.20989f, 0, 0, 0, 0, RESPAWN_IMMEDIATELY)
    // buffs
        || !AddObject(BG_TTP_OBJECT_BUFF_1, BG_TTP_OBJECT_TYPE_BUFF_1, 566.6805f, 602.2274f, 383.6813f, 3.316144f, 0, 0, -1.f, 0, 120)
        || !AddObject(BG_TTP_OBJECT_BUFF_2, BG_TTP_OBJECT_TYPE_BUFF_2, 566.6563f, 664.566f, 383.6809f, 2.460913f, 0, 0, 0, 1.f, 120))
    {
        sLog->outError(LOG_FILTER_SQL, "BattlegroundTTP: Failed to spawn some object!");
        return false;
    }

    return true;
}
