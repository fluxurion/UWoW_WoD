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
#include "BattlegroundTV.h"
#include "Language.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "WorldPacket.h"

BattlegroundTV::BattlegroundTV()
{
    BgObjects.resize(BG_TV_OBJECT_MAX);

    for (uint8 i = BG_STARTING_EVENT_FIRST; i < BG_STARTING_EVENT_COUNT; ++i)
    {
        m_broadcastMessages[i] = ArenaBroadcastTexts[i];
        m_hasBroadcasts[i] = true;
    }
}

BattlegroundTV::~BattlegroundTV()
{

}

void BattlegroundTV::StartingEventCloseDoors()
{
    for (uint32 i = BG_TV_OBJECT_DOOR_1; i <= BG_TV_OBJECT_DOOR_2; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);

    UpdateWorldState(static_cast<WorldStates>(8524), 0);
}

void BattlegroundTV::StartingEventOpenDoors()
{
    for (uint32 i = BG_TV_OBJECT_DOOR_1; i <= BG_TV_OBJECT_DOOR_2; ++i)
        DoorOpen(i);

    UpdateWorldState(static_cast<WorldStates>(8524), 1);
    UpdateWorldState(static_cast<WorldStates>(8529), int32(time(nullptr) + 1200));
}

void BattlegroundTV::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);

    UpdateArenaWorldState();
}

void BattlegroundTV::RemovePlayer(Player* /*player*/, ObjectGuid /*guid*/, uint32 /*team*/)
{
    if (GetStatus() == STATUS_WAIT_LEAVE)
        return;

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void BattlegroundTV::HandleKillPlayer(Player* player, Player* killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (!killer)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundTV: Killer player not found");
        return;
    }

    Battleground::HandleKillPlayer(player, killer);

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void BattlegroundTV::HandleAreaTrigger(Player* player, uint32 trigger, bool entered)
{
    switch (trigger)
    {
        case 8451: // Alliance start loc
        case 8452: // Horde start loc
            if (!entered && GetStatus() != STATUS_IN_PROGRESS)
                player->TeleportTo(GetMapId(), GetTeamStartPosition(player->GetTeamId()));
            break;
        default:
            Battleground::HandleAreaTrigger(player, trigger, entered);
            break;
    }
}

void BattlegroundTV::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(static_cast<WorldStates>(6643), 1);
    packet.Worldstates.emplace_back(static_cast<WorldStates>(3610), 1);
    packet.Worldstates.emplace_back(static_cast<WorldStates>(8524), (GetStatus() != STATUS_IN_PROGRESS ? 0 : 1));
    packet.Worldstates.emplace_back(static_cast<WorldStates>(8529), int32(time(nullptr) + std::chrono::duration_cast<Seconds>(Minutes(20) - GetArenaMinutesElapsed()).count()));
    UpdateArenaWorldState();
}

void BattlegroundTV::Reset()
{
    Battleground::Reset();
}

bool BattlegroundTV::SetupBattleground()
{
    if (!AddObject(BG_TV_OBJECT_DOOR_1, BG_TV_OBJECT_TYPE_DOOR_1, -10654.3f, 428.3047f, 23.54276f, 3.141593f, 0, 0, -1.f, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TV_OBJECT_DOOR_2, BG_TV_OBJECT_TYPE_DOOR_2, -10774.61f, 431.2383f, 23.54276f, 0, 0, 0, 0, 1.f, RESPAWN_IMMEDIATELY))
    {
        sLog->outError(LOG_FILTER_SQL, "BattlegroundTV: Failed to spawn some object!");
        return false;
    }

    return true;
}
