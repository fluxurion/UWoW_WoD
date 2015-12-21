/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
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

#include "BattlegroundWS.h"
#include "Creature.h"
#include "GameObject.h"
#include "Language.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "BattlegroundMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"

BattlegroundWS::BattlegroundWS()
{
    BgObjects.resize(BG_WS_OBJECT_MAX);
    BgCreatures.resize(BG_CREATURES_MAX_WS);

    StartMessageIds[BG_STARTING_EVENT_FIRST] = LANG_BG_WS_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_WS_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD] = LANG_BG_WS_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_WS_HAS_BEGUN;

    _flagSpellForceTimer = 0;
    _bothFlagsKept = false;
    _flagDebuffState = 0;

    _reputationCapture = 0;
}

BattlegroundWS::~BattlegroundWS()
{ }

void BattlegroundWS::PostUpdateImpl(uint32 diff)
{
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        if (GetElapsedTime() >= Minutes(17))
            Battleground::BattlegroundTimedWin(2);
        else if (GetElapsedTime() > Minutes(3))
            UpdateWorldState(WorldStates::BG_WS_CURRENT_TIMER, int32(time(nullptr) + std::chrono::duration_cast<Seconds>(Minutes(15) - GetElapsedTime()).count()));

        for (uint8 team = TEAM_ALLIANCE; team < MAX_TEAMS; ++team)
        {
            switch (_flagState[team])
            {
                case BG_WS_FLAG_STATE_WAIT_RESPAWN:
                    _flagsTimer -= diff;
                    if (_flagsTimer <= 0)
                    {
                        _flagsTimer = 0;
                        RespawnFlag(team, true);
                        UpdateWorldState(WorldStates::BG_WS_FLAG_UNKNOWN, 0);
                    }
                    break;
                case BG_WS_FLAG_STATE_ON_GROUND:
                    _flagsDropTimer[team] -= diff;
                    if (_flagsDropTimer[team] < 0)
                    {
                        _flagsDropTimer[team] = 0;

                        RespawnFlag(team, false);

                        if (GameObject* obj = GetBgMap()->GetGameObject(_droppedFlagGUID[team]))
                            obj->Delete();
                        else
                            sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundWS: An error has occurred in PostUpdateImpl: Unknown dropped flag GUID: %u", _droppedFlagGUID[team].GetCounter());

                        _droppedFlagGUID[team].Clear();

                        /// If both flags are kept and 1 of cariers dies and no one clicked on flag set _bothflagskept = false
                        if (_bothFlagsKept)
                        {
                            _bothFlagsKept = false;
                            _flagDebuffState = 0;
                            _flagSpellForceTimer = 0;
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        if (_bothFlagsKept)
        {
            _flagSpellForceTimer += diff;

            if (_flagSpellForceTimer >= 30 * IN_MILLISECONDS && _flagDebuffState < 10)
            {
                _flagDebuffState = _flagSpellForceTimer / MINUTE / IN_MILLISECONDS;
                _flagSpellForceTimer += 30 * IN_MILLISECONDS;

                if (_flagDebuffState <= 5)
                {
                    for (uint8 team = TEAM_ALLIANCE; team < MAX_TEAMS; ++team)
                        if (Player* player = ObjectAccessor::FindPlayer(_flagKeepers[team]))
                            player->CastSpell(player, SPELL_BG_FOCUSED_ASSAULT, true);
                }
                else if (_flagDebuffState == 6)
                {
                    for (uint8 team = TEAM_ALLIANCE; team < MAX_TEAMS; ++team)
                    {
                        if (Player* player = ObjectAccessor::FindPlayer(_flagKeepers[team]))
                        {
                            player->RemoveAurasDueToSpell(SPELL_BG_FOCUSED_ASSAULT);
                            player->CastCustomSpell(SPELL_BG_BRUTAL_ASSAULT, SPELLVALUE_AURA_STACK, _flagDebuffState, player);
                        }
                    }
                }
                else
                    for (uint8 team = TEAM_ALLIANCE; team < MAX_TEAMS; ++team)
                        if (Player* player = ObjectAccessor::FindPlayer(_flagKeepers[team]))
                            player->CastSpell(player, SPELL_BG_BRUTAL_ASSAULT, true);
            }
        }
    }
}

void BattlegroundWS::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    PlayerScores[player->GetGUID()] = new BattlegroundWGScore(player->GetGUID(), player->GetBGTeamId());

    player->SendDirectMessage(WorldPackets::Battleground::Init(BG_WS_MAX_TEAM_SCORE).Write());
    Battleground::SendBattleGroundPoints(player->GetBGTeamId() != TEAM_ALLIANCE, m_TeamScores[player->GetBGTeamId()], false, player);
}

void BattlegroundWS::GetPlayerPositionData(std::vector<WorldPackets::Battleground::PlayerPositions::BattlegroundPlayerPosition>* positions) const
{
    if (Player* player = ObjectAccessor::FindPlayer(_flagKeepers[TEAM_ALLIANCE], true))
    {
        WorldPackets::Battleground::PlayerPositions::BattlegroundPlayerPosition position;
        position.Guid = player->GetGUID();
        position.Pos = player->GetPosition();
        position.IconID = PLAYER_POSITION_ICON_ALLIANCE_FLAG;
        position.ArenaSlot = PLAYER_POSITION_ARENA_SLOT_NONE;
        positions->push_back(position);
    }

    if (Player* player = ObjectAccessor::FindPlayer(_flagKeepers[TEAM_HORDE], true))
    {
        WorldPackets::Battleground::PlayerPositions::BattlegroundPlayerPosition position;
        position.Guid = player->GetGUID();
        position.Pos = player->GetPosition();
        position.IconID = PLAYER_POSITION_ICON_HORDE_FLAG;
        position.ArenaSlot = PLAYER_POSITION_ARENA_SLOT_NONE;
        positions->push_back(position);
    }
}

void BattlegroundWS::StartingEventCloseDoors()
{
    for (uint32 i = BG_WS_OBJECT_DOOR_A_1; i <= BG_WS_OBJECT_DOOR_H_3; ++i)
    {
        DoorClose(i);
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
    }

    for (uint32 i = BG_WS_OBJECT_A_FLAG; i <= BG_WS_OBJECT_BERSERKBUFF_2; ++i)
        SpawnBGObject(i, RESPAWN_ONE_DAY);
}

void BattlegroundWS::StartingEventOpenDoors()
{
    for (uint32 i = BG_WS_OBJECT_DOOR_A_1; i <= BG_WS_OBJECT_DOOR_A_6; ++i)
        DoorOpen(i);
    for (uint32 i = BG_WS_OBJECT_DOOR_H_1; i <= BG_WS_OBJECT_DOOR_H_4; ++i)
        DoorOpen(i);

    for (uint32 i = BG_WS_OBJECT_A_FLAG; i <= BG_WS_OBJECT_BERSERKBUFF_2; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);

    SpawnBGObject(BG_WS_OBJECT_DOOR_A_5, RESPAWN_ONE_DAY);
    SpawnBGObject(BG_WS_OBJECT_DOOR_A_6, RESPAWN_ONE_DAY);
    SpawnBGObject(BG_WS_OBJECT_DOOR_H_3, RESPAWN_ONE_DAY);
    SpawnBGObject(BG_WS_OBJECT_DOOR_H_4, RESPAWN_ONE_DAY);

    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, BG_EVENT_START_BATTLE);
    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT2, BG_EVENT_START_BATTLE);

    UpdateWorldState(WorldStates::BG_WS_ENABLE_TIMER, 1);
    UpdateWorldState(WorldStates::BG_WS_CURRENT_TIMER, int32(time(nullptr) + std::chrono::duration_cast<Seconds>(Minutes(15)).count()));
}

bool BattlegroundWS::SetupBattleground()
{
    if (!AddObject(BG_WS_OBJECT_A_FLAG, BG_OBJECT_A_FLAG_WS_ENTRY, 1540.423f, 1481.325f, 351.8284f, 3.089233f, 0, 0, 0.9996573f, 0.02617699f, BG_WS_FLAG_RESPAWN_TIME / 1000)
        || !AddObject(BG_WS_OBJECT_H_FLAG, BG_OBJECT_H_FLAG_WS_ENTRY, 916.0226f, 1434.405f, 345.413f, 0.01745329f, 0, 0, 0.008726535f, 0.9999619f, BG_WS_FLAG_RESPAWN_TIME / 1000)

        || !AddObject(BG_WS_OBJECT_SPEEDBUFF_1, BG_OBJECTID_SPEEDBUFF_ENTRY, 1449.93f, 1470.71f, 342.6346f, -1.64061f, 0, 0, 0.7313537f, -0.6819983f, BUFF_RESPAWN_TIME)
        || !AddObject(BG_WS_OBJECT_SPEEDBUFF_2, BG_OBJECTID_SPEEDBUFF_ENTRY, 1005.171f, 1447.946f, 335.9032f, 1.64061f, 0, 0, 0.7313537f, 0.6819984f, BUFF_RESPAWN_TIME)
        || !AddObject(BG_WS_OBJECT_REGENBUFF_1, BG_OBJECTID_REGENBUFF_ENTRY, 1317.506f, 1550.851f, 313.2344f, -0.2617996f, 0, 0, 0.1305263f, -0.9914448f, BUFF_RESPAWN_TIME)
        || !AddObject(BG_WS_OBJECT_REGENBUFF_2, BG_OBJECTID_REGENBUFF_ENTRY, 1110.451f, 1353.656f, 316.5181f, -0.6806787f, 0, 0, 0.333807f, -0.9426414f, BUFF_RESPAWN_TIME)
        || !AddObject(BG_WS_OBJECT_BERSERKBUFF_1, BG_OBJECTID_BERSERKERBUFF_ENTRY, 1320.09f, 1378.79f, 314.7532f, 1.186824f, 0, 0, 0.5591929f, 0.8290376f, BUFF_RESPAWN_TIME)
        || !AddObject(BG_WS_OBJECT_BERSERKBUFF_2, BG_OBJECTID_BERSERKERBUFF_ENTRY, 1139.688f, 1560.288f, 306.8432f, -2.443461f, 0, 0, 0.9396926f, -0.3420201f, BUFF_RESPAWN_TIME)

        || !AddObject(BG_WS_OBJECT_DOOR_A_1, BG_OBJECT_DOOR_A_1_WS_ENTRY, 1503.335f, 1493.466f, 352.1888f, 3.115414f, 0, 0, 0.9999143f, 0.01308903f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_WS_OBJECT_DOOR_A_2, BG_OBJECT_DOOR_A_2_WS_ENTRY, 1492.478f, 1457.912f, 342.9689f, 3.115414f, 0, 0, 0.9999143f, 0.01308903f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_WS_OBJECT_DOOR_A_3, BG_OBJECT_DOOR_A_3_WS_ENTRY, 1468.503f, 1494.357f, 351.8618f, 3.115414f, 0, 0, 0.9999143f, 0.01308903f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_WS_OBJECT_DOOR_A_4, BG_OBJECT_DOOR_A_4_WS_ENTRY, 1471.555f, 1458.778f, 362.6332f, 3.115414f, 0, 0, 0.9999143f, 0.01308903f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_WS_OBJECT_DOOR_A_5, BG_OBJECT_DOOR_A_5_WS_ENTRY, 1492.347f, 1458.34f, 342.3712f, -0.03490669f, 0, 0, 0.01745246f, -0.9998477f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_WS_OBJECT_DOOR_A_6, BG_OBJECT_DOOR_A_6_WS_ENTRY, 1503.466f, 1493.367f, 351.7352f, -0.03490669f, 0, 0, 0.01745246f, -0.9998477f, RESPAWN_IMMEDIATELY)

        || !AddObject(BG_WS_OBJECT_DOOR_H_1, BG_OBJECT_DOOR_H_1_WS_ENTRY, 949.1663f, 1423.772f, 345.6241f, -0.5756807f, -0.01673368f, -0.004956111f, -0.2839723f, 0.9586737f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_WS_OBJECT_DOOR_H_2, BG_OBJECT_DOOR_H_2_WS_ENTRY, 953.0507f, 1459.842f, 340.6526f, -1.99662f, -0.1971825f, 0.1575096f, -0.8239487f, 0.5073641f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_WS_OBJECT_DOOR_H_3, BG_OBJECT_DOOR_H_3_WS_ENTRY, 949.9523f, 1422.751f, 344.9273f, 0.0f, 0, 0, 0, 1, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_WS_OBJECT_DOOR_H_4, BG_OBJECT_DOOR_H_4_WS_ENTRY, 950.7952f, 1459.583f, 342.1523f, 0.05235988f, 0, 0, 0.02617695f, 0.9996573f, RESPAWN_IMMEDIATELY)
        )
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundWS: Failed to spawn some object Battleground not created!");
        return false;
    }

    WorldSafeLocsEntry const* sg = sWorldSafeLocsStore.LookupEntry(WS_GRAVEYARD_MAIN_ALLIANCE);
    if (!sg || !AddSpiritGuide(WS_SPIRIT_MAIN_ALLIANCE, sg->Loc, TEAM_ALLIANCE))
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundWS: Failed to spawn Alliance spirit guide! Battleground not created!");
        return false;
    }

    sg = sWorldSafeLocsStore.LookupEntry(WS_GRAVEYARD_MAIN_HORDE);
    if (!sg || !AddSpiritGuide(WS_SPIRIT_MAIN_HORDE, sg->Loc, TEAM_HORDE))
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundWS: Failed to spawn Horde spirit guide! Battleground not created!");
        return false;
    }

    return true;
}

void BattlegroundWS::Reset()
{
    Battleground::Reset();

    for (int8 i = TEAM_ALLIANCE; i < MAX_TEAMS; ++i)
    {
        _flagKeepers[i].Clear();
        _droppedFlagGUID[i].Clear();
        _flagsDropTimer[i] = 0;
        _flagState[i] = BG_WS_FLAG_STATE_ON_BASE;
        m_TeamScores[i] = 0;
    }

    _reputationCapture = 35;
    _flagsTimer = 0;
}

void BattlegroundWS::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(WorldStates::BG_WS_FLAG_CAPTURES_ALLIANCE, m_TeamScores[TEAM_ALLIANCE]);
    packet.Worldstates.emplace_back(WorldStates::BG_WS_FLAG_CAPTURES_HORDE, m_TeamScores[TEAM_HORDE]);
    packet.Worldstates.emplace_back(WorldStates::BG_WS_FLAG_CAPTURES_MAX, BG_WS_MAX_TEAM_SCORE);

    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        for (uint8 team = TEAM_ALLIANCE; team < MAX_TEAMS; ++team)
        {
            switch (_flagState[team])
            {
                case BG_WS_FLAG_STATE_ON_GROUND:
                    packet.Worldstates.emplace_back(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, -1);
                    packet.Worldstates.emplace_back(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_ALLIANCE : WorldStates::BG_WS_FLAG_STATE_HORDE, BG_WS_FLAG_STATE_ON_GROUND);
                    break;
                case BG_WS_FLAG_STATE_ON_PLAYER:
                    packet.Worldstates.emplace_back(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, 1);
                    packet.Worldstates.emplace_back(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_ALLIANCE : WorldStates::BG_WS_FLAG_STATE_HORDE, BG_WS_FLAG_STATE_ON_PLAYER);
                    break;
                default:
                    packet.Worldstates.emplace_back(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, 0);
                    packet.Worldstates.emplace_back(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_ALLIANCE : WorldStates::BG_WS_FLAG_STATE_HORDE, BG_WS_FLAG_STATE_WAIT_RESPAWN);
                    break;
            }
        }

        packet.Worldstates.emplace_back(WorldStates::BG_WS_ENABLE_TIMER, 1);
        packet.Worldstates.emplace_back(WorldStates::BG_WS_CURRENT_TIMER, int32(time(nullptr) + std::chrono::duration_cast<Seconds>(Minutes(15) - GetElapsedTime()).count()));
    }
    else
    {
        packet.Worldstates.emplace_back(WorldStates::BG_WS_ENABLE_TIMER, 0);
        packet.Worldstates.emplace_back(WorldStates::BG_WS_FLAG_UNK_ALLIANCE, 0);
        packet.Worldstates.emplace_back(WorldStates::BG_WS_FLAG_UNK_HORDE, 0);
        packet.Worldstates.emplace_back(WorldStates::BG_WS_FLAG_STATE_HORDE, 1);
        packet.Worldstates.emplace_back(WorldStates::BG_WS_FLAG_STATE_ALLIANCE, 1);
    }
}

void BattlegroundWS::EndBattleground(uint32 winner)
{
    for (uint8 team = TEAM_ALLIANCE; team < MAX_TEAMS; ++team)
        if (Player* player = ObjectAccessor::FindPlayer(_flagKeepers[team]))
        {
            player->RemoveAurasDueToSpell(SPELL_BG_FOCUSED_ASSAULT);
            player->RemoveAurasDueToSpell(SPELL_BG_BRUTAL_ASSAULT);
        }

    UpdateWorldState(WorldStates::BG_WS_FLAG_UNK_ALLIANCE, 0);
    UpdateWorldState(WorldStates::BG_WS_FLAG_UNK_HORDE, 0);
    UpdateWorldState(WorldStates::BG_WS_FLAG_STATE_ALLIANCE, 1);
    UpdateWorldState(WorldStates::BG_WS_FLAG_STATE_HORDE, 1);
    UpdateWorldState(WorldStates::BG_WS_ENABLE_TIMER, 0);

    uint32 realWinner = WINNER_NONE;
    if (winner == TEAM_ALLIANCE)
        realWinner = ALLIANCE;
    else if (winner == TEAM_HORDE)
        realWinner = HORDE;
    else if (winner > WINNER_NONE)
        realWinner = winner;

    Battleground::EndBattleground(realWinner);
}

void BattlegroundWS::HandleKillPlayer(Player* player, Player* killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    EventPlayerDroppedFlag(player);

    Battleground::HandleKillPlayer(player, killer);
}

void BattlegroundWS::HandleAreaTrigger(Player* player, uint32 trigger, bool entered)
{
    switch (trigger)
    {
        case 3646:                                          // Alliance Flag spawn
            if (_flagState[TEAM_HORDE] && !_flagState[TEAM_ALLIANCE] && GetStatus() == STATUS_IN_PROGRESS)
                if (_flagKeepers[TEAM_HORDE] == player->GetGUID())
                    EventPlayerCapturedFlag(player);
            break;
        case 3647:                                          // Horde Flag spawn
            if (_flagState[TEAM_ALLIANCE] && !_flagState[TEAM_HORDE] && GetStatus() == STATUS_IN_PROGRESS)
                if (GetFlagPickerGUID(TEAM_ALLIANCE) == player->GetGUID())
                    EventPlayerCapturedFlag(player);
            break;
        case 8966: // Alliance start loc
        case 8965: // Horde start loc
            if (!entered && GetStatus() == STATUS_WAIT_JOIN)
                player->TeleportTo(GetMapId(), GetTeamStartPosition(player->GetBGTeamId()));
            break;
        default:
            Battleground::HandleAreaTrigger(player, trigger, entered);
            break;
    }
}

bool BattlegroundWS::UpdatePlayerScore(Player* player, uint32 type, uint32 value, bool doAddHonor /*= true*/)
{
    if (!Battleground::UpdatePlayerScore(player, type, value, doAddHonor))
        return false;

    switch (type)
    {
        case SCORE_FLAG_CAPTURES:
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, WS_OBJECTIVE_CAPTURE_FLAG, 1);
            break;
        case SCORE_FLAG_RETURNS:
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, WS_OBJECTIVE_RETURN_FLAG, 1);
            break;
        default:
            break;
    }
    return true;
}

WorldSafeLocsEntry const* BattlegroundWS::GetClosestGraveYard(Player* player)
{
    if (player->GetBGTeam() == ALLIANCE)
        return sWorldSafeLocsStore.LookupEntry(GetStatus() == STATUS_IN_PROGRESS ? WS_GRAVEYARD_MAIN_ALLIANCE : WS_GRAVEYARD_FLAGROOM_ALLIANCE);
    else
        return sWorldSafeLocsStore.LookupEntry(GetStatus() == STATUS_IN_PROGRESS ? WS_GRAVEYARD_MAIN_HORDE : WS_GRAVEYARD_FLAGROOM_HORDE);
}

void BattlegroundWS::EventPlayerDroppedFlag(Player* Source)
{
    TeamId teamID = Source->GetBGTeamId();

    if (GetStatus() != STATUS_IN_PROGRESS)
    {
        if (teamID == TEAM_ALLIANCE)
        {
            if (!IsHordeFlagPickedup())
                return;

            if (GetFlagPickerGUID(TEAM_HORDE) == Source->GetGUID())
            {
                SetHordeFlagPicker(ObjectGuid::Empty);
                Source->RemoveAurasDueToSpell(SPELL_BG_HORDE_FLAG);
            }
        }
        else
        {
            if (!IsAllianceFlagPickedup())
                return;

            if (GetFlagPickerGUID(TEAM_ALLIANCE) == Source->GetGUID())
            {
                SetAllianceFlagPicker(ObjectGuid::Empty);
                Source->RemoveAurasDueToSpell(SPELL_BG_ALLIANCE_FLAG);
            }
        }
        return;
    }

    bool set = false;

    if (teamID == TEAM_ALLIANCE)
    {
        if (!IsHordeFlagPickedup())
            return;

        if (GetFlagPickerGUID(TEAM_HORDE) == Source->GetGUID())
        {
            SetHordeFlagPicker(ObjectGuid::Empty);
            Source->RemoveAurasDueToSpell(SPELL_BG_HORDE_FLAG);
            _flagState[TEAM_HORDE] = BG_WS_FLAG_STATE_ON_GROUND;
            Source->CastSpell(Source, SPELL_BG_HORDE_FLAG_DROPPED, true);
            set = true;
        }
    }
    else
    {
        if (!IsAllianceFlagPickedup())
            return;
        if (GetFlagPickerGUID(TEAM_ALLIANCE) == Source->GetGUID())
        {
            SetAllianceFlagPicker(ObjectGuid::Empty);
            Source->RemoveAurasDueToSpell(SPELL_BG_ALLIANCE_FLAG);
            _flagState[TEAM_ALLIANCE] = BG_WS_FLAG_STATE_ON_GROUND;
            Source->CastSpell(Source, SPELL_BG_ALLIANCE_FLAG_DROPPED, true);
            set = true;
        }
    }

    if (set)
    {
        Source->CastSpell(Source, SPELL_BG_RECENTLY_DROPPED_FLAG, true);
        UpdateFlagState(teamID, 1);

        SendBroadcastTextToAll(teamID == TEAM_ALLIANCE ? 9805 : 9806, teamID == TEAM_ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, Source);
        UpdateWorldState(teamID == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, uint32(-1));

        _flagsDropTimer[Source->GetBGTeamId() ? 0 : 1] = BG_WS_FLAG_DROP_TIME;
    }
}

void BattlegroundWS::EventPlayerClickedOnFlag(Player* source, GameObject* object)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    TeamId team = source->GetBGTeamId();

    if (source->IsWithinDistInMap(object, 10))
    {
        switch (_flagState[team ^ 1])
        {
            case BG_WS_FLAG_STATE_ON_BASE:
                if (BgObjects[BG_WS_OBJECT_A_FLAG + (team ^ 1)] == object->GetGUID())
                {
                    SpawnBGObject(team == TEAM_ALLIANCE ? BG_WS_OBJECT_H_FLAG : BG_WS_OBJECT_A_FLAG, RESPAWN_ONE_DAY);
                    UpdateFlagState(team ^ 1, BG_WS_FLAG_STATE_ON_PLAYER, source->GetGUID());
                    source->CastSpell(source, team == TEAM_ALLIANCE ? SPELL_BG_HORDE_FLAG : SPELL_BG_ALLIANCE_FLAG, true);
                    source->GetAchievementMgr().StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_SPELL_TARGET2, team == TEAM_ALLIANCE ? SPELL_BG_HORDE_FLAG_PICKED_UP : SPELL_BG_ALLIANCE_FLAG_PICKED_UP);

                    if (!_flagKeepers[team].IsEmpty())
                        _bothFlagsKept = true;

                    SendBroadcastTextToAll(team == TEAM_ALLIANCE ? 9807 : 9804, team == TEAM_ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, source);
                    PlayeCapturePointSound(NODE_STATUS_ASSAULT, team);
                }
                break;
            case BG_WS_FLAG_STATE_ON_GROUND:
                if (_droppedFlagGUID[team ^ 1] == object->GetGUID())
                {
                    source->CastSpell(source, team == TEAM_ALLIANCE ? SPELL_BG_HORDE_FLAG : SPELL_BG_ALLIANCE_FLAG, true);
                    UpdateFlagState(team ^ 1, BG_WS_FLAG_STATE_ON_PLAYER, source->GetGUID());

                    if (_flagDebuffState  && _flagDebuffState < 6)
                        source->CastCustomSpell(SPELL_BG_FOCUSED_ASSAULT, SPELLVALUE_AURA_STACK, _flagDebuffState, source);
                    else if (_flagDebuffState >= 6)
                        source->CastCustomSpell(SPELL_BG_BRUTAL_ASSAULT, SPELLVALUE_AURA_STACK, _flagDebuffState, source);

                    _flagsDropTimer[team ^ 1] = 0;

                    PlayeCapturePointSound(NODE_STATUS_ASSAULT, team);
                    SendBroadcastTextToAll(team == TEAM_ALLIANCE ? 9807 : 9804, team == TEAM_ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, source);
                }
                break;
            default:
                break;
        }

        if (_flagState[team] == BG_WS_FLAG_STATE_ON_GROUND && _droppedFlagGUID[team] == object->GetGUID())
        {
            UpdateFlagState(team, BG_WS_FLAG_STATE_WAIT_RESPAWN);
            UpdatePlayerScore(source, SCORE_FLAG_RETURNS, 1);

            RespawnFlag(team, false);

            PlaySoundToAll(BG_SOUND_FLAG_RESET);
            SendBroadcastTextToAll(team == TEAM_ALLIANCE ? 9808 : 9809, team == TEAM_ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, source);

            _bothFlagsKept = false;
            _flagSpellForceTimer = 0;
        }
    }

    source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
}

void BattlegroundWS::EventPlayerCapturedFlag(Player* source)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    TeamId team = source->GetBGTeamId();

    source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
    _flagDebuffState = 0;

    SpawnBGObject(BG_WS_OBJECT_A_FLAG, RESPAWN_ONE_DAY);
    SpawnBGObject(BG_WS_OBJECT_H_FLAG, RESPAWN_ONE_DAY);

    source->RemoveAurasDueToSpell(SPELL_BG_FOCUSED_ASSAULT);
    source->RemoveAurasDueToSpell(SPELL_BG_BRUTAL_ASSAULT);

    RewardHonorToTeam(GetBonusHonorFromKill(2), source->GetBGTeam());

    RewardReputationToTeam(890, 889, _reputationCapture, source->GetBGTeam());

    m_TeamScores[team]++;

    UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_CAPTURES_ALLIANCE : WorldStates::BG_WS_FLAG_CAPTURES_HORDE, m_TeamScores[team]);

    UpdateWorldState(WorldStates::BG_WS_FLAG_UNKNOWN, -1);
    UpdateFlagState(team ^ 1, BG_WS_FLAG_STATE_WAIT_RESPAWN);
    UpdatePlayerScore(source, SCORE_FLAG_CAPTURES, 1);
    UpdateWorldState(WorldStates::BG_WS_UNKNOWN, 1);

    m_lastFlagCaptureTeam = source->GetBGTeam();

    PlayeCapturePointSound(NODE_STATUS_CAPTURE, team);
    SendBroadcastTextToAll(team == TEAM_ALLIANCE ? 9801 : 9802, team == TEAM_ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, source);

    source->RemoveAurasDueToSpell(team == TEAM_ALLIANCE ? SPELL_BG_HORDE_FLAG : SPELL_BG_ALLIANCE_FLAG);

    Battleground::SendBattleGroundPoints(team != TEAM_ALLIANCE, m_TeamScores[team]);

    if (m_TeamScores[team] == BG_WS_MAX_TEAM_SCORE)
        EndBattleground(team);
    else
        _flagsTimer = BG_WS_FLAG_RESPAWN_TIME;
}

void BattlegroundWS::RemovePlayer(Player* player, ObjectGuid guid, uint32 /*team*/)
{
    if (!player)
        return;

    TeamId team = player->GetBGTeamId();

    if (_flagKeepers[team ^ 1] == guid)
    {
        RespawnFlag(team ^ 1, false);
        player->RemoveAurasDueToSpell(team == TEAM_ALLIANCE ? SPELL_BG_HORDE_FLAG : SPELL_BG_ALLIANCE_FLAG);

        if (_bothFlagsKept)
        {
            for (uint8 teamTemp = TEAM_ALLIANCE; teamTemp < MAX_TEAMS; ++teamTemp)
            {
                if (Player* pl = ObjectAccessor::FindPlayer(_flagKeepers[team]))
                {
                    if (_flagDebuffState && _flagDebuffState < 6)
                        pl->RemoveAurasDueToSpell(SPELL_BG_FOCUSED_ASSAULT);
                    else if (_flagDebuffState >= 6)
                        pl->RemoveAurasDueToSpell(SPELL_BG_BRUTAL_ASSAULT);
                }
                else
                    sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundWS: An error has occurred in RemovePlayer: player with GUID: %u haven't been found. (_bothflagsKept is TRUE).", _flagKeepers[team]);            }

            _bothFlagsKept = false;
            _flagDebuffState = 0;
            _flagSpellForceTimer = 0;
        }
    }
}

void BattlegroundWS::RespawnFlag(uint32 team, bool captured)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (captured)
    {
        SpawnBGObject(BG_WS_OBJECT_H_FLAG, RESPAWN_IMMEDIATELY);
        SpawnBGObject(BG_WS_OBJECT_A_FLAG, RESPAWN_IMMEDIATELY);
        SendBroadcastTextToAll(9803, CHAT_MSG_BG_SYSTEM_NEUTRAL);
    }
    else
    {
        SpawnBGObject(BG_WS_OBJECT_A_FLAG + team, RESPAWN_IMMEDIATELY);
        SendBroadcastTextToAll(team == TEAM_ALLIANCE ? 24891 : 24892, CHAT_MSG_BG_SYSTEM_NEUTRAL);
    }

    PlayeCapturePointSound(NODE_STATUS_NEUTRAL, static_cast<TeamId>(team));

    UpdateFlagState(team, BG_WS_FLAG_STATE_ON_BASE);
}

void BattlegroundWS::UpdateFlagState(uint32 team, uint32 value, ObjectGuid flagKeeperGUID)
{
    switch (value)
    {
        case BG_WS_FLAG_STATE_WAIT_RESPAWN:
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, 0);
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_HORDE : WorldStates::BG_WS_FLAG_STATE_ALLIANCE, BG_WS_FLAG_STATE_WAIT_RESPAWN);
            break;
        case BG_WS_FLAG_STATE_ON_BASE:
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, 0);
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_HORDE : WorldStates::BG_WS_FLAG_STATE_ALLIANCE, 1); // strange
            break;
        case BG_WS_FLAG_STATE_ON_PLAYER:
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, 1);
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_HORDE : WorldStates::BG_WS_FLAG_STATE_ALLIANCE, BG_WS_FLAG_STATE_ON_PLAYER);
            break;
        case BG_WS_FLAG_STATE_ON_GROUND:
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, -1);
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_HORDE : WorldStates::BG_WS_FLAG_STATE_ALLIANCE, BG_WS_FLAG_STATE_ON_GROUND);
            break;
        default:
            break;
    }

    _flagState[team] = value;
    _flagKeepers[team] = flagKeeperGUID;
}
