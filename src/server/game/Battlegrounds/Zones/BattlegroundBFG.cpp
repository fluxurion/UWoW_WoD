/*
 * Copyright (C) 2005-2011 MaNGOS <http://www.getmangos.com/>
 *
 * Copyright (C) 2008-2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010-2011 Project SkyFire <http://www.projectskyfire.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "gamePCH.h"
#include "BattlegroundBFG.h"

BattlegroundBFG::BattlegroundBFG()
{
    m_BuffChange = true;
    BgObjects.resize(GILNEAS_BG_OBJECT_MAX);
    BgCreatures.resize(GILNEAS_BG_ALL_NODES_COUNT);

    StartMessageIds[BG_STARTING_EVENT_FIRST] = LANG_BG_BG_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_BG_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD] = LANG_BG_BG_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_BG_HAS_BEGUN;

    _IsInformedNearVictory = false;

    for (uint8 i = TEAM_ALLIANCE; i < MAX_TEAMS; ++i)
        _teamScores500Disadvantage[i] = false;
}

BattlegroundBFG::~BattlegroundBFG()
{ }

void BattlegroundBFG::PostUpdateImpl(Milliseconds diff)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (GetElapsedTime() >= Minutes(17))
        Battleground::BattlegroundTimedWin();

    int16 teamPoints[MAX_TEAMS] = { };

    for (uint8 i = GILNEAS_BG_NODE_LIGHTHOUSE; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
    {
        if (_capturePoints[i].Timer > Milliseconds(0))
        {
            if (_capturePoints[i].Timer > diff)
                _capturePoints[i].Timer -= diff;
            else
            {
                _capturePoints[i].Timer = Milliseconds(0);
                _capturePoints[i].PrevStatus = _capturePoints[i].Status;
                _capturePoints[i].Status = NODE_STATUS_CAPTURE;

                _NodeOccupied(i, _capturePoints[i].TeamID);

                UpdateCapturePoint(NODE_STATUS_CAPTURE, _capturePoints[i].TeamID, _capturePoints[i].Point);
            }
        }

        for (uint8 team = TEAM_ALLIANCE; team < MAX_TEAMS; ++team)
            if (_capturePoints[i].TeamID == team && _capturePoints[i].Status == NODE_STATUS_CAPTURE)
                ++teamPoints[team];
    }

    for (uint8 team = TEAM_ALLIANCE; team < MAX_TEAMS; ++team)
    {
        int16 points = teamPoints[team];
        if (!points)
            continue;

        _lastTick[team] += diff;
        if (_lastTick[team] > BgBFGTickIntervals[points])
        {
            _lastTick[team] -= BgBFGTickIntervals[points];
            m_TeamScores[team] += BgBFGTickPoints[points];
            _honorScoreTicks[team] += BgBFGTickPoints[points];

            if (_honorScoreTicks[team] >= _HonorTicks)
            {
                RewardHonorToTeam(GetBonusHonorFromKill(1), GetTeamByTeamId(team));
                _honorScoreTicks[team] -= _HonorTicks;
            }

            if (!_IsInformedNearVictory && m_TeamScores[team] > GILNEAS_BG_WARNING_NEAR_VICTORY_SCORE)
            {
                SendBroadcastTextToAll(team == TEAM_ALLIANCE ? 10598 : 10599, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                PlaySoundToAll(BG_SOUND_NEAR_VICTORY);
                _IsInformedNearVictory = true;
            }

            if (m_TeamScores[team] > GILNEAS_BG_MAX_TEAM_SCORE)
                m_TeamScores[team] = GILNEAS_BG_MAX_TEAM_SCORE;

            Battleground::SendBattleGroundPoints(team != TEAM_ALLIANCE, m_TeamScores[team]);

            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::ALLIANCE_RESOUCES : WorldStates::HORDE_RESOUCES, m_TeamScores[team]);

            uint8 otherTeam = (team + 1) % MAX_TEAMS;
            if (m_TeamScores[team] > m_TeamScores[otherTeam] + 500)
                _teamScores500Disadvantage[otherTeam] = true;
        }

        if (m_TeamScores[team] >= GILNEAS_BG_MAX_TEAM_SCORE)
            EndBattleground(GetTeamByTeamId(team));
    }
}

void BattlegroundBFG::StartingEventCloseDoors()
{
    for (uint8 object = BG_BFG_OBJECT_BANNER; object < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++object)
        SpawnBGObject(object, RESPAWN_ONE_DAY);

    for (uint8 i = GILNEAS_BG_NODE_LIGHTHOUSE; i < GILNEAS_BG_DYNAMIC_NODES_COUNT * 3; ++i)
        SpawnBGObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + i, RESPAWN_ONE_DAY);

    DoorsClose(GILNEAS_BG_OBJECT_GATE_A_1, GILNEAS_BG_OBJECT_GATE_H_1);
    DoorsClose(GILNEAS_BG_OBJECT_GATE_A_2, GILNEAS_BG_OBJECT_GATE_H_2);

    _NodeOccupied(GILNEAS_BG_SPIRIT_ALIANCE, TEAM_ALLIANCE);
    _NodeOccupied(GILNEAS_BG_SPIRIT_HORDE, TEAM_HORDE);
}

void BattlegroundBFG::StartingEventOpenDoors()
{
    for (uint8 i = GILNEAS_BG_NODE_LIGHTHOUSE; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
    {
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
        UpdateCapturePoint(NODE_STATUS_NEUTRAL, _capturePoints[i].TeamID, _capturePoints[i].Point, nullptr, true);
    }

    for (uint8 i = GILNEAS_BG_NODE_LIGHTHOUSE; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
        SpawnBGObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + urand(0, 2) + i * 3, RESPAWN_IMMEDIATELY);

    DoorsOpen(GILNEAS_BG_OBJECT_GATE_A_1, GILNEAS_BG_OBJECT_GATE_H_1);
    DoorsOpen(GILNEAS_BG_OBJECT_GATE_A_2, GILNEAS_BG_OBJECT_GATE_H_2);

    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, BG_EVENT_START_BATTLE);
    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT2, BG_EVENT_START_BATTLE);
}

void BattlegroundBFG::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    PlayerScores[player->GetGUID()] = new BattlegroundBFGScore(player->GetGUID(), player->GetBGTeamId());

    player->SendDirectMessage(WorldPackets::Battleground::Init(GILNEAS_BG_MAX_TEAM_SCORE).Write());

    Battleground::SendBattleGroundPoints(player->GetBGTeamId() != TEAM_ALLIANCE, m_TeamScores[player->GetBGTeamId()], false, player);
}

void BattlegroundBFG::RemovePlayer(Player* /*player*/, ObjectGuid /*guid*/, uint32 team)
{ }

void BattlegroundBFG::HandleAreaTrigger(Player* player, uint32 trigger, bool entered)
{
    switch (trigger)
    {
        case 6448: // Horde Start
        case 6447: // Alliance Start
            if (!entered && GetStatus() == STATUS_WAIT_JOIN)
                player->TeleportTo(GetMapId(), GetTeamStartPosition(player->GetBGTeamId()));
            break;
        default:
            Battleground::HandleAreaTrigger(player, trigger, entered);
            break;
    }
}

void BattlegroundBFG::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    for (uint8 i = GILNEAS_BG_NODE_LIGHTHOUSE; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
        if (GameObject* point = _capturePoints[i].Point) // if player entered the battleground which has already begun
            packet.Worldstates.emplace_back(static_cast<WorldStates>(point->GetGOInfo()->capturePoint.worldState1), _capturePoints[i].Status);

    packet.Worldstates.emplace_back(WorldStates::OCCOPIED_BASES_ALLIANCE, _GetCapturedNodesForTeam(TEAM_ALLIANCE));
    packet.Worldstates.emplace_back(WorldStates::OCCOPIED_BASES_HORDE, _GetCapturedNodesForTeam(TEAM_HORDE));
    packet.Worldstates.emplace_back(WorldStates::ALLIANCE_RESOUCES, m_TeamScores[TEAM_ALLIANCE]);
    packet.Worldstates.emplace_back(WorldStates::HORDE_RESOUCES, m_TeamScores[TEAM_HORDE]);
    packet.Worldstates.emplace_back(WorldStates::MAX_TEAM_RESOUCES, GILNEAS_BG_MAX_TEAM_SCORE);
}

void BattlegroundBFG::UpdateCapturePoint(uint8 type, TeamId teamID, GameObject* node, Player const* player /*= nullptr*/, bool initial /*= false*/)
{
    Battleground::UpdateCapturePoint(type, teamID, node, player, initial);
}

void BattlegroundBFG::_NodeOccupied(uint8 node, TeamId team)
{
    if (node >= GILNEAS_BG_DYNAMIC_NODES_COUNT)
        return;

    if (!AddSpiritGuide(node, BgBfgSpiritGuidePos[node], team))
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Failed to spawn spirit guide! point: %u, team: %u, ", node, GetTeamByTeamId(team));

    UpdateWorldState(_capturePoints[node].Point->GetGOInfo()->capturePoint.worldState1, _capturePoints[node].Status);

    if (_capturePoints[node].Status == NODE_STATUS_CAPTURE)
    {
        if (_capturePoints[node].TeamID == TEAM_ALLIANCE)
            UpdateWorldState(WorldStates::OCCOPIED_BASES_ALLIANCE, _GetCapturedNodesForTeam(TEAM_ALLIANCE));

        if (_capturePoints[node].TeamID == TEAM_HORDE)
            UpdateWorldState(WorldStates::OCCOPIED_BASES_HORDE, _GetCapturedNodesForTeam(TEAM_HORDE));
    }
}

void BattlegroundBFG::_NodeDeOccupied(uint8 node)
{
    if (node >= GILNEAS_BG_DYNAMIC_NODES_COUNT)
        return;

    if (node < GILNEAS_BG_DYNAMIC_NODES_COUNT)
        DelCreature(node + 5);

    RelocateDeadPlayers(BgCreatures[node]);

    if (!BgCreatures[node].IsEmpty())
        DelCreature(node);
}

void BattlegroundBFG::EventPlayerClickedOnFlag(Player* source, GameObject* /*object*/)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint8 i = GILNEAS_BG_NODE_LIGHTHOUSE;
    GameObject* object = GetBgMap()->GetGameObject(BgObjects[i]);
    while ((i < GILNEAS_BG_DYNAMIC_NODES_COUNT) && ((!object) || (!source->IsWithinDistInMap(object, 10))))
    {
        ++i;
        object = GetBgMap()->GetGameObject(BgObjects[i]);
    }

    if (i == GILNEAS_BG_DYNAMIC_NODES_COUNT)
        return;

    TeamId teamID = source->GetBGTeamId();
    if (_capturePoints[i].TeamID == teamID)
        return;

    source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);

    auto pointInfo = object->GetGOInfo()->capturePoint;

    switch (_capturePoints[i].Status)
    {
        case NODE_STATUS_NEUTRAL: // If node is neutral, change to contested
            UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
            UpdateCapturePoint(NODE_STATUS_ASSAULT, teamID, _capturePoints[i].Point, source);

            _capturePoints[i].Timer = Milliseconds(pointInfo.CaptureTime);
            _capturePoints[i].PrevStatus = _capturePoints[i].Status;
            _capturePoints[i].Status = NODE_STATUS_ASSAULT;
            _capturePoints[i].TeamID = teamID;
            break;
        case NODE_STATUS_ASSAULT:
            if (_capturePoints[i].PrevStatus < NODE_STATUS_CAPTURE) // If last state is NOT occupied, change node to enemy-contested
            {
                UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
                UpdateCapturePoint(NODE_STATUS_ASSAULT, teamID, _capturePoints[i].Point, source);

                _capturePoints[i].Timer = Milliseconds(pointInfo.CaptureTime);
                _capturePoints[i].PrevStatus = _capturePoints[i].Status;
                _capturePoints[i].Status = NODE_STATUS_ASSAULT;
                _capturePoints[i].TeamID = teamID;
                break;
            }
            else // If contested, change back to occupied
            {
                UpdatePlayerScore(source, SCORE_BASES_DEFENDED, 1);
                UpdateCapturePoint(NODE_STATUS_CAPTURE, teamID, _capturePoints[i].Point, source);
                _NodeOccupied(i, teamID);

                _capturePoints[i].Timer = Milliseconds(0);
                _capturePoints[i].PrevStatus = _capturePoints[i].Status;
                _capturePoints[i].Status = NODE_STATUS_CAPTURE;
                _capturePoints[i].TeamID = teamID;
                break;
            }
        default: // If node is occupied, change to enemy-contested
        {
            UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
            UpdateCapturePoint(NODE_STATUS_ASSAULT, teamID, _capturePoints[i].Point, source);
            _NodeDeOccupied(i);

            _capturePoints[i].Timer = Milliseconds(pointInfo.CaptureTime);
            _capturePoints[i].PrevStatus = _capturePoints[i].Status;
            _capturePoints[i].Status = NODE_STATUS_ASSAULT;
            _capturePoints[i].TeamID = teamID;
        }
    }
}

bool BattlegroundBFG::SetupBattleground()
{
    if (!AddObject(BG_BFG_OBJECT_BANNER, BG_BFG_GO_NODE_BANNER_0, BgBfgNodePosition[0][0], BgBfgNodePosition[0][1], RESPAWN_ONE_DAY) ||
        !AddObject(BG_BFG_OBJECT_BANNER + 1, BG_BFG_GO_NODE_BANNER_1, BgBfgNodePosition[1][0], BgBfgNodePosition[1][1], RESPAWN_ONE_DAY) ||
        !AddObject(BG_BFG_OBJECT_BANNER + 2, BG_BFG_GO_NODE_BANNER_2, BgBfgNodePosition[2][0], BgBfgNodePosition[2][1], RESPAWN_ONE_DAY))
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "BattleForGilneas: Can't Create Some Object");
        return false;
    }

    if (!AddObject(GILNEAS_BG_OBJECT_GATE_A_1, BG_BFG_GO_GATE_A_1, BgBfgDoorPos[0][0], BgBfgDoorPos[0][1], RESPAWN_IMMEDIATELY) ||
        !AddObject(GILNEAS_BG_OBJECT_GATE_A_2, BG_BFG_GO_GATE_A_2, BgBfgDoorPos[1][0], BgBfgDoorPos[1][1], RESPAWN_IMMEDIATELY) ||
        !AddObject(GILNEAS_BG_OBJECT_GATE_H_1, BG_BFG_GO_GATE_H_1, BgBfgDoorPos[2][0], BgBfgDoorPos[2][1], RESPAWN_IMMEDIATELY) ||
        !AddObject(GILNEAS_BG_OBJECT_GATE_H_2, BG_BFG_GO_GATE_H_2, BgBfgDoorPos[3][0], BgBfgDoorPos[3][1], RESPAWN_IMMEDIATELY))
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "BattleForGilneas: Can't Create Doors");
        return false;
    }

    for (uint8 i = GILNEAS_BG_NODE_LIGHTHOUSE; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
    {
        if (GameObject* obj = GetBgMap()->GetGameObject(BgObjects[i]))
            _capturePoints[i].Point = obj;

    //    if (!AddObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + 3 * i, Buff_Entries[0], BgBFGBuffsPos[i][0], BgBFGBuffsPos[i][1], BgBFGBuffsPos[i][2], BgBFGBuffsPos[i][3], 0, 0, sin(BgBFGBuffsPos[i][3] / 2), cos(BgBFGBuffsPos[i][3] / 2), RESPAWN_ONE_DAY)
    //        || !AddObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + 3 * i + 1, Buff_Entries[1], BgBFGBuffsPos[i][0], BgBFGBuffsPos[i][1], BgBFGBuffsPos[i][2], BgBFGBuffsPos[i][3], 0, 0, sin(BgBFGBuffsPos[i][3] / 2), cos(BgBFGBuffsPos[i][3] / 2), RESPAWN_ONE_DAY)
    //        || !AddObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + 3 * i + 2, Buff_Entries[2], BgBFGBuffsPos[i][0], BgBFGBuffsPos[i][1], BgBFGBuffsPos[i][2], BgBFGBuffsPos[i][3], 0, 0, sin(BgBFGBuffsPos[i][3] / 2), cos(BgBFGBuffsPos[i][3] / 2), RESPAWN_ONE_DAY))
    //        sLog->outError(LOG_FILTER_BATTLEGROUND, "BattleForGilneas: Can't Create Buffs");
    }


    return true;
}

void BattlegroundBFG::Reset()
{
    Battleground::Reset();

    for (int8 i = TEAM_ALLIANCE; i < MAX_TEAMS; ++i)
    {
        m_TeamScores[i] = 0;
        _lastTick[i] = Milliseconds(0);
        _honorScoreTicks[i] = 0;
        _teamScores500Disadvantage[i] = false;
    }

    _IsInformedNearVictory = false;
    _HonorTicks = 330;

    for (uint8 i = GILNEAS_BG_NODE_LIGHTHOUSE; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
    {
        _capturePoints[i].Timer = Milliseconds(0);
        _capturePoints[i].Point = nullptr;
        _capturePoints[i].Status = NODE_STATUS_NEUTRAL;
        _capturePoints[i].PrevStatus = NODE_STATUS_NEUTRAL;
        _capturePoints[i].TeamID = TEAM_NONE;
    }

    for (uint8 i = 0; i < GILNEAS_BG_ALL_NODES_COUNT; ++i)
        if (!BgCreatures[i].IsEmpty())
            DelCreature(i);
}

bool BattlegroundBFG::UpdatePlayerScore(Player* player, uint32 type, uint32 value, bool doAddHonor /*= true*/)
{
    if (!Battleground::UpdatePlayerScore(player, type, value, doAddHonor))
        return false;

    switch (type)
    {
        case SCORE_BASES_ASSAULTED:
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, BG_OBJECTIVE_ASSAULT_BASE, 1);
            break;
        case SCORE_BASES_DEFENDED:
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, BG_OBJECTIVE_DEFEND_BASE, 1);
            break;
        default:
            break;
    }
    return true;
}

void BattlegroundBFG::EndBattleground(uint32 winner)
{
    Battleground::EndBattleground(winner);
}

WorldSafeLocsEntry const* BattlegroundBFG::GetClosestGraveYard(Player* player)
{
    TeamId teamIndex = player->GetBGTeamId();

    std::vector<uint8> nodes;
    for (uint8 i = GILNEAS_BG_NODE_LIGHTHOUSE; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
        if (_capturePoints[i].TeamID == teamIndex + 3)
            nodes.push_back(i);

    WorldSafeLocsEntry const* safeLocksEntry = nullptr;

    if (!nodes.empty())
    {
        float X = player->GetPositionX();
        float Y = player->GetPositionY();

        float mindist = 999999.0f; // Temp Hack
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            WorldSafeLocsEntry const* entry = sWorldSafeLocsStore.LookupEntry(GILNEAS_BG_GraveyardIds[nodes[i]]);
            if (!entry)
                continue;

            float dist = (entry->Loc.X - X) * (entry->Loc.X - X) + (entry->Loc.Y - Y) * (entry->Loc.Y - Y);

            if (mindist > dist)
            {
                mindist = dist;
                safeLocksEntry = entry;
            }
        }
        nodes.clear();
    }

    if (!safeLocksEntry) // If not, place ghost on starting location
        safeLocksEntry = sWorldSafeLocsStore.LookupEntry(GILNEAS_BG_GraveyardIds[teamIndex + 3]);

    return safeLocksEntry;
}

bool BattlegroundBFG::IsAllNodesConrolledByTeam(uint32 teamID) const
{
    uint8 count = 0;
    for (int8 i = GILNEAS_BG_NODE_LIGHTHOUSE; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
        if (_capturePoints[i].TeamID == teamID && _capturePoints[i].Status == NODE_STATUS_CAPTURE)
            ++count;

    return count == GILNEAS_BG_DYNAMIC_NODES_COUNT;
}

uint8 BattlegroundBFG::_GetCapturedNodesForTeam(TeamId teamID)
{
    uint8 nodes = 0;
    for (uint8 i = GILNEAS_BG_NODE_LIGHTHOUSE; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
        if (_capturePoints[i].Status == NODE_STATUS_CAPTURE && _capturePoints[i].TeamID == teamID)
            ++nodes;

    return nodes;
}
