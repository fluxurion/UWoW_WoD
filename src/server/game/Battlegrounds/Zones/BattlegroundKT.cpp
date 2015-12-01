/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "Object.h"
#include "Player.h"
#include "BattlegroundKT.h"
#include "Creature.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "BattlegroundMgr.h"
#include "WorldPacket.h"
#include "Language.h"
#include "MapManager.h"

BattlegroundKT::BattlegroundKT()
{
    StartMessageIds[BG_STARTING_EVENT_FIRST] = 0;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_KT_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD] = LANG_BG_KT_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_KT_HAS_BEGUN;

    _updatePointsTimer = 3 * IN_MILLISECONDS;
    _lastCapturedOrbTeam = TEAM_NONE;
}

BattlegroundKT::~BattlegroundKT()
{ }

void BattlegroundKT::PostUpdateImpl(uint32 diff)
{
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        if (_updatePointsTimer <= diff)
        {
            for (uint8 i = 0; i < MAX_ORBS; ++i)
            {
                ObjectGuid guid = _orbKeepers[i];

                if (guid.IsEmpty() || _playersZone.find(guid) == _playersZone.end())
                    continue;

                auto playerZone = _playersZone[guid];
                if (Player* player = ObjectAccessor::FindPlayer(guid))
                {
                    if (playerZone > KT_ZONE_MAX)
                        continue;
                        
                    TeamId teamID = player->GetBGTeamId();
                    if (teamID >= TEAM_NEUTRAL)
                        continue;

                    m_TeamScores[teamID] += BG_KT_TickPoints[playerZone];

                    if (m_TeamScores[teamID] > BG_KT_MAX_TEAM_SCORE)
                        m_TeamScores[teamID] = BG_KT_MAX_TEAM_SCORE;

                    UpdateWorldState(teamID == TEAM_ALLIANCE ? WorldStates::BG_KT_ORB_POINTS_A : WorldStates::BG_KT_ORB_POINTS_H, m_TeamScores[teamID]);

                    if (m_TeamScores[teamID] == BG_KT_MAX_TEAM_SCORE)
                        EndBattleground(GetTeamByTeamId(teamID));

                    UpdatePlayerScore(player, SCORE_ORB_SCORE, BG_KT_TickPoints[playerZone]);
                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, BG_KT_OBJECTIVE_ORB_COUNT, playerZone + 3);
                    Battleground::SendBattleGroundPoints(player->GetBGTeamId() != TEAM_ALLIANCE, m_TeamScores[player->GetBGTeamId()]);
                }
            }

            _updatePointsTimer = 2 * IN_MILLISECONDS;
        }
        else
            _updatePointsTimer -= diff;
    }
}

void BattlegroundKT::StartingEventCloseDoors()
{
    SpawnBGObject(BG_KT_OBJECT_A_DOOR, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_KT_OBJECT_H_DOOR, RESPAWN_IMMEDIATELY);

    DoorsClose(BG_KT_OBJECT_A_DOOR, BG_KT_OBJECT_H_DOOR);

    for (uint8 i = 0; i < MAX_ORBS; ++i)
        SpawnBGObject(BG_KT_OBJECT_ORB_GREEN + i, RESPAWN_ONE_DAY);
}

void BattlegroundKT::StartingEventOpenDoors()
{
    DoorsOpen(BG_KT_OBJECT_A_DOOR, BG_KT_OBJECT_H_DOOR);

    for (uint8 i = 0; i < MAX_ORBS; ++i)
        SpawnBGObject(BG_KT_OBJECT_ORB_GREEN + i, RESPAWN_IMMEDIATELY);

    // Players that join battleground after start are not eligible to get achievement.
    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, BG_EVENT_START_BATTLE);
    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT2, BG_EVENT_START_BATTLE);
}

void BattlegroundKT::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    PlayerScores[player->GetGUID()] = new BattleGroundKTScore(player->GetGUID(), player->GetBGTeamId());

    player->SendDirectMessage(WorldPackets::Battleground::Init(BG_KT_MAX_TEAM_SCORE).Write());
    Battleground::SendBattleGroundPoints(player->GetBGTeamId() != TEAM_ALLIANCE, m_TeamScores[player->GetBGTeamId()], false, player);
    
    _playersZone[player->GetGUID()] = KT_ZONE_OUT;
}

void BattlegroundKT::GetPlayerPositionData(std::vector<WorldPackets::Battleground::PlayerPositions::BattlegroundPlayerPosition>* positions) const
{
    for (uint8 i = 0; i < MAX_ORBS; ++i)
    {
        Player* player = ObjectAccessor::FindPlayer(GetFlagPickerGUID(i));
        if (!player)
            continue;

        WorldPackets::Battleground::PlayerPositions::BattlegroundPlayerPosition position;
        position.Guid = player->GetGUID();
        position.Pos = player->GetPosition();
        position.IconID = player->GetBGTeamId() == TEAM_ALLIANCE ? PLAYER_POSITION_ICON_ALLIANCE_FLAG : PLAYER_POSITION_ICON_HORDE_FLAG;
        position.ArenaSlot = i + 2;
        positions->push_back(position);
    }
}

void BattlegroundKT::EventPlayerClickedOnFlag(Player* source, GameObject* object)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (!source->IsWithinDistInMap(object, 10))
        return;

    uint32 index = object->GetEntry() - BG_KT_OBJECT_ORB_1_ENTRY;

    // If this orb is already keeped by a player, there is a problem
    if (index > MAX_ORBS || !_orbKeepers[index].IsEmpty())
        return;

    // Check if the player already have an orb
    for (uint8 i = 0; i < MAX_ORBS; ++i)
        if (_orbKeepers[i] == source->GetGUID())
            return;

    TeamId teamID = source->GetBGTeamId();

    source->CastSpell(source, BG_KT_ORBS_SPELLS[index], true);
    source->CastSpell(source, teamID == TEAM_ALLIANCE ? BG_KT_ALLIANCE_INSIGNIA : BG_KT_HORDE_INSIGNIA, true);

    UpdatePlayerScore(source, SCORE_ORB_HANDLES, 1);

    _orbKeepers[index] = source->GetGUID();
    UpdateWorldState(OrbsWS[index][teamID], 1);
    UpdateWorldState(OrbsIcons[index], 0);
    SpawnBGObject(BG_KT_OBJECT_ORB_GREEN + index, RESPAWN_ONE_DAY);

    if (Creature* aura = GetBGCreature(BG_KT_CREATURE_ORB_AURA_1 + index))
        aura->RemoveAllAuras();

    PlayeCapturePointSound(NODE_STATUS_ASSAULT, teamID);
    SendBroadcastTextToAll(BgKtBroadCastTextOrbPickedUp[index], teamID == TEAM_ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, source);
    source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
}

void BattlegroundKT::EventPlayerDroppedFlag(Player* source)
{
    if (!source || GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint8 index = 0;

    for (; index <= MAX_ORBS; ++index)
    {
        if (index == MAX_ORBS)
            return;

        if (_orbKeepers[index] == source->GetGUID())
            break;
    }
    
    TeamId teamID = source->GetBGTeamId();

    source->RemoveAurasDueToSpell(BG_KT_ORBS_SPELLS[index]);
    source->RemoveAurasDueToSpell(BG_KT_ALLIANCE_INSIGNIA);
    source->RemoveAurasDueToSpell(BG_KT_HORDE_INSIGNIA);

    _orbKeepers[index].Clear();
    SpawnBGObject(BG_KT_OBJECT_ORB_GREEN + index, RESPAWN_IMMEDIATELY);

    if (Creature* aura = GetBGCreature(BG_KT_CREATURE_ORB_AURA_1 + index))
        aura->AddAura(BG_KT_ORBS_AURA[index], aura);

    UpdateWorldState(OrbsWS[index][teamID], 0);
    UpdateWorldState(OrbsIcons[index], 1);
    
    PlayeCapturePointSound(NODE_STATUS_ASSAULT, teamID);
    SendBroadcastTextToAll(BgKtBroadCastTextOrbDropped[index], teamID == TEAM_ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, source);
    source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
}

void BattlegroundKT::RemovePlayer(Player* player, ObjectGuid guid, uint32)
{
    if (!player)
        return;

    EventPlayerDroppedFlag(player);
    _playersZone.erase(player->GetGUID());
}

void BattlegroundKT::HandleAreaTrigger(Player* player, uint32 trigger, bool entered)
{
    ObjectGuid sourceGuid = player->GetGUID();
    switch (trigger)
    {
        case 7734: // Out-In trigger
        {
            if (_playersZone.find(sourceGuid) == _playersZone.end())
                return;

            if (_playersZone[sourceGuid] == KT_ZONE_OUT)
                _playersZone[sourceGuid] = KT_ZONE_IN;
            else
                _playersZone[sourceGuid] = KT_ZONE_OUT;
            break;
        }
        case 7735: // Middle-In trigger
        {
            if (_playersZone.find(sourceGuid) == _playersZone.end())
                return;

            if (_playersZone[sourceGuid] == KT_ZONE_IN)
                _playersZone[sourceGuid] = KT_ZONE_MIDDLE;
            else
                _playersZone[sourceGuid] = KT_ZONE_IN;
            break;
        }
        case 8378: // Alliance start loc
        case 8377: // Horde start loc
            if (!entered && GetStatus() == STATUS_WAIT_JOIN)
                player->TeleportTo(GetMapId(), GetTeamStartPosition(player->GetBGTeamId()));
            break;
        default:
            Battleground::HandleAreaTrigger(player, trigger, entered);
            break;
    }
}

bool BattlegroundKT::SetupBattleground()
{
    if (!AddObject(BG_KT_OBJECT_A_DOOR, BG_KT_OBJECT_DOOR_ENTRY, BG_KT_DoorPositions[0][0], BG_KT_DoorPositions[0][1], BG_KT_DoorPositions[0][2], BG_KT_DoorPositions[0][3], 0, 0, sin(BG_KT_DoorPositions[0][3] / 2), cos(BG_KT_DoorPositions[0][3] / 2), RESPAWN_IMMEDIATELY) ||
        !AddObject(BG_KT_OBJECT_H_DOOR, BG_KT_OBJECT_DOOR_ENTRY, BG_KT_DoorPositions[1][0], BG_KT_DoorPositions[1][1], BG_KT_DoorPositions[1][2], BG_KT_DoorPositions[1][3], 0, 0, sin(BG_KT_DoorPositions[1][3] / 2), cos(BG_KT_DoorPositions[1][3] / 2), RESPAWN_IMMEDIATELY) ||
        !AddObject(BG_KT_OBJECT_BERSERKBUFF_1, BG_OBJECTID_BERSERKERBUFF_ENTRY, 1856.635f, 1333.741f, 10.555f, 3.150048f, 0, 0, 0.5591929f, 0.8290376f, BUFF_RESPAWN_TIME) ||
        !AddObject(BG_KT_OBJECT_BERSERKBUFF_2, BG_OBJECTID_BERSERKERBUFF_ENTRY, 1710.284f, 1333.345f, 10.554f, 0.116051f, 0, 0, 0.9396926f, -0.3420201f, BUFF_RESPAWN_TIME) ||
        !AddSpiritGuide(BG_KT_CREATURE_SPIRIT_1, BG_KT_SpiritPositions[0], TEAM_ALLIANCE) ||
        !AddSpiritGuide(BG_KT_CREATURE_SPIRIT_2, BG_KT_SpiritPositions[1], TEAM_HORDE))
        return false;

    for (uint8 i = 0; i < MAX_ORBS; ++i)
    {
        if (!AddObject(BG_KT_OBJECT_ORB_GREEN + i, BG_KT_OBJECT_ORB_1_ENTRY + i, BG_KT_OrbPositions[i][0], BG_KT_OrbPositions[i][1], BG_KT_OrbPositions[i][2], BG_KT_OrbPositions[i][3], 0, 0, sin(BG_KT_OrbPositions[i][3] / 2), cos(BG_KT_OrbPositions[i][3] / 2), RESPAWN_ONE_DAY))
            return false;

        if (Creature* trigger = AddCreature(WORLD_TRIGGER, BG_KT_CREATURE_ORB_AURA_1 + i, TEAM_NEUTRAL, BG_KT_OrbPositions[i][0], BG_KT_OrbPositions[i][1], BG_KT_OrbPositions[i][2], BG_KT_OrbPositions[i][3], RESPAWN_IMMEDIATELY))
            trigger->AddAura(BG_KT_ORBS_AURA[i], trigger);
    }

    return true;
}

void BattlegroundKT::Reset()
{
    Battleground::Reset();
    BgObjects.resize(BG_KT_OBJECT_MAX);
    BgCreatures.resize(BG_KT_CREATURE_MAX);

    for (uint32 i = 0; i < MAX_ORBS; ++i)
        _orbKeepers[i].Clear();

    _lastCapturedOrbTeam = TEAM_NONE;
}

void BattlegroundKT::EndBattleground(uint32 winner)
{
    if (winner == ALLIANCE)
    {
        //RewardHonorToTeam(GetBonusHonorFromKill(m_HonorWinKills), ALLIANCE);
        CastSpellOnTeam(135788, ALLIANCE); // Quest credit "The Lion Roars"
    }

    if (winner == HORDE)
    {
        //RewardHonorToTeam(GetBonusHonorFromKill(m_HonorWinKills), HORDE);
        CastSpellOnTeam(135788, HORDE); // Quest credit "The Lion Roars"
    }

    //RewardHonorToTeam(GetBonusHonorFromKill(m_HonorEndKills), ALLIANCE);
    //RewardHonorToTeam(GetBonusHonorFromKill(m_HonorEndKills), HORDE);

    Battleground::EndBattleground(winner);
}

void BattlegroundKT::HandleKillPlayer(Player *player, Player *killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    EventPlayerDroppedFlag(player);

    Battleground::HandleKillPlayer(player, killer);
}

WorldSafeLocsEntry const* BattlegroundKT::GetClosestGraveYard(Player* player)
{
    if (player->GetBGTeamId() == TEAM_ALLIANCE)
        return sWorldSafeLocsStore.LookupEntry(GetStatus() == STATUS_IN_PROGRESS ? KT_GRAVEYARD_RECTANGLEA1 : KT_GRAVEYARD_RECTANGLEA2);
    else
        return sWorldSafeLocsStore.LookupEntry(GetStatus() == STATUS_IN_PROGRESS ? KT_GRAVEYARD_RECTANGLEH1 : KT_GRAVEYARD_RECTANGLEH2);
}

void BattlegroundKT::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(WorldStates::BG_KT_ICON_GREEN_ORB_ICON, _orbKeepers[BG_KT_OBJECT_ORB_GREEN].IsEmpty() ? 1 : 0);
    packet.Worldstates.emplace_back(WorldStates::BG_KT_ICON_PURPLE_ORB_ICON, _orbKeepers[BG_KT_OBJECT_ORB_GREEN].IsEmpty() ? 1 : 0);
    packet.Worldstates.emplace_back(WorldStates::BG_KT_ICON_ORANGE_ORB_ICON, _orbKeepers[BG_KT_OBJECT_ORB_ORANGE].IsEmpty() ? 1 : 0);
    packet.Worldstates.emplace_back(WorldStates::BG_KT_ICON_BLUE_ORB_ICON, _orbKeepers[BG_KT_OBJECT_ORB_BLUE].IsEmpty() ? 1 : 0);

    packet.Worldstates.emplace_back(WorldStates::BG_KT_ORB_POINTS_A, m_TeamScores[TEAM_ALLIANCE]);
    packet.Worldstates.emplace_back(WorldStates::BG_KT_ORB_POINTS_H, m_TeamScores[TEAM_HORDE]);

    packet.Worldstates.emplace_back(WorldStates::BG_KT_PURPLE_ORB_C, _orbKeepers[BG_KT_OBJECT_ORB_GREEN].IsEmpty());
    packet.Worldstates.emplace_back(WorldStates::BG_KT_GREEN_ORB_C, _orbKeepers[BG_KT_OBJECT_ORB_PURPLE].IsEmpty());
    packet.Worldstates.emplace_back(WorldStates::BG_KT_ORANGE_ORB_C, _orbKeepers[BG_KT_OBJECT_ORB_ORANGE].IsEmpty());
    packet.Worldstates.emplace_back(WorldStates::BG_KT_BLUE_ORB_C, _orbKeepers[BG_KT_OBJECT_ORB_BLUE].IsEmpty());
}
