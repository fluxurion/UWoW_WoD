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

#include "BattlegroundTP.h"
#include "Creature.h"
#include "GameObject.h"
#include "Language.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "BattlegroundMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"

BattlegroundTP::BattlegroundTP()
{
    BgObjects.resize(BG_TP_OBJECT_MAX);
    BgCreatures.resize(BG_CREATURES_MAX_TP);

    StartMessageIds[BG_STARTING_EVENT_FIRST] = LANG_BG_TP_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_TP_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD] = LANG_BG_TP_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_TP_HAS_BEGUN;

    _flagSpellForceTimer = 0;
    _bothFlagsKept = false;
    _flagDebuffState = 0;
}

BattlegroundTP::~BattlegroundTP()
{ }

void BattlegroundTP::PostUpdateImpl(uint32 diff)
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
                case BG_TP_FLAG_STATE_WAIT_RESPAWN:
                    _flagsTimer -= diff;
                    if (_flagsTimer <= 0)
                    {
                        _flagsTimer = 0;
                        RespawnFlag(team, true);
                        UpdateWorldState(WorldStates::BG_WS_FLAG_UNKNOWN, 0);
                    }
                    break;
                case BG_TP_FLAG_STATE_ON_GROUND:
                    _flagsDropTimer[team] -= diff;
                    if (_flagsDropTimer[team] < 0)
                    {
                        _flagsDropTimer[team] = 0;
                        RespawnFlag(team, false);

                        if (GameObject* obj = GetBgMap()->GetGameObject(_droppedFlagGUID[team]))
                            obj->Delete();
                        else
                            sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundTP: An error has occurred in PostUpdateImpl: Unknown dropped flag GUID: %u", _droppedFlagGUID[team].GetCounter());

                        _droppedFlagGUID[team].Clear();

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

void BattlegroundTP::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    PlayerScores[player->GetGUID()] = new BattlegroundTPScore(player->GetGUID(), player->GetBGTeamId());

    player->SendDirectMessage(WorldPackets::Battleground::Init(BG_TP_MAX_TEAM_SCORE).Write());
    Battleground::SendBattleGroundPoints(player->GetBGTeamId() != TEAM_ALLIANCE, m_TeamScores[player->GetBGTeamId()], false, player);
}

void BattlegroundTP::StartingEventCloseDoors()
{
    for (uint32 i = BG_TP_OBJECT_DOOR_A_1; i <= BG_TP_OBJECT_DOOR_H_3; ++i)
    {
        DoorClose(i);
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
    }
    for (uint32 i = BG_TP_OBJECT_A_FLAG; i <= BG_TP_OBJECT_BERSERKBUFF_2; ++i)
        SpawnBGObject(i, RESPAWN_ONE_DAY);
}

void BattlegroundTP::StartingEventOpenDoors()
{
    for (uint32 i = BG_TP_OBJECT_DOOR_A_1; i <= BG_TP_OBJECT_DOOR_A_3; ++i)
        DoorOpen(i);
    for (uint32 i = BG_TP_OBJECT_DOOR_H_1; i <= BG_TP_OBJECT_DOOR_H_3; ++i)
        DoorOpen(i);

    for (uint32 i = BG_TP_OBJECT_A_FLAG; i <= BG_TP_OBJECT_BERSERKBUFF_2; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);

    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, BG_EVENT_START_BATTLE);
    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT2, BG_EVENT_START_BATTLE);

    UpdateWorldState(WorldStates::BG_WS_ENABLE_TIMER, 1);
    UpdateWorldState(WorldStates::BG_WS_CURRENT_TIMER, int32(time(nullptr) + std::chrono::duration_cast<Seconds>(Minutes(15)).count()));
}

bool BattlegroundTP::SetupBattleground()
{
    if (!AddObject(BG_TP_OBJECT_A_FLAG, BG_OBJECT_A_FLAG_TP_ENTRY, 2117.637f, 191.6823f, 44.05199f, 6.021387f, 0, 0, 0.9996573f, 0.02617699f, BG_TP_FLAG_RESPAWN_TIME / 1000)
        || !AddObject(BG_TP_OBJECT_H_FLAG, BG_OBJECT_H_FLAG_TP_ENTRY, 1578.337f, 344.0451f, 2.418409f, 2.792518f, 0, 0, 0.008726535f, 0.9999619f, BG_TP_FLAG_RESPAWN_TIME / 1000)
        /// Buffs
        || !AddObject(BG_TP_OBJECT_SPEEDBUFF_1, BG_OBJECTID_SPEEDBUFF_ENTRY, 1544.55f, 303.852f, 0.692371f, 6.265733f, 0, 0, 0.7313537f, -0.6819983f, BUFF_RESPAWN_TIME) // id 179899 in sniff
        || !AddObject(BG_TP_OBJECT_SPEEDBUFF_2, BG_OBJECTID_SPEEDBUFF_ENTRY, 2175.866f, 226.6215f, 43.76288f, 2.600535f, 0, 0, 0.7313537f, 0.6819984f, BUFF_RESPAWN_TIME)
        || !AddObject(BG_TP_OBJECT_REGENBUFF_1, BG_OBJECTID_REGENBUFF_ENTRY, 1754.163f, 242.125f, -14.13157f, 1.151916f, 0, 0, 0.1305263f, -0.9914448f, BUFF_RESPAWN_TIME) // id 179906 in sniff
        || !AddObject(BG_TP_OBJECT_REGENBUFF_2, BG_OBJECTID_REGENBUFF_ENTRY, 1951.18f, 383.795f, -10.5257f, 4.06662f, 0, 0, 0.333807f, -0.9426414f, BUFF_RESPAWN_TIME)
        || !AddObject(BG_TP_OBJECT_BERSERKBUFF_1, BG_OBJECTID_BERSERKERBUFF_ENTRY, 1932.832f, 226.7917f, -17.05979f, 2.44346f, 0, 0, 0.5591929f, 0.8290376f, BUFF_RESPAWN_TIME)
        || !AddObject(BG_TP_OBJECT_BERSERKBUFF_2, BG_OBJECTID_BERSERKERBUFF_ENTRY, 1737.566f, 435.8455f, -8.086342f, 5.515242f, 0, 0, 0.9396926f, -0.3420201f, BUFF_RESPAWN_TIME) // id 179907 in sniff
        /// Alliance gates
        || !AddObject(BG_TP_OBJECT_DOOR_A_1, BG_OBJECT_DOOR_A_1_TP_ENTRY, 2135.525f, 218.926f, 43.60946f, 5.750861f, 0, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TP_OBJECT_DOOR_A_2, BG_OBJECT_DOOR_A_2_TP_ENTRY, 2156.0f, 219.2059f, 43.6256f, 2.609261f, 0, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TP_OBJECT_DOOR_A_3, BG_OBJECT_DOOR_A_3_TP_ENTRY, 2118.088f, 154.6754f, 43.57089f, 2.609261f, 0, 0, 0, 0, RESPAWN_IMMEDIATELY)
        /// Horde gates
        || !AddObject(BG_TP_OBJECT_DOOR_H_1, BG_OBJECT_DOOR_H_1_TP_ENTRY, 1556.656f, 314.7127f, 1.589001f, 6.178466f, 0, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TP_OBJECT_DOOR_H_2, BG_OBJECT_DOOR_H_2_TP_ENTRY, 1574.605f, 321.2421f, 1.58989f, 6.178466f, 0, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TP_OBJECT_DOOR_H_3, BG_OBJECT_DOOR_H_3_TP_ENTRY, 1558.088f, 372.7654f, 1.723727f, 6.178466f, 0, 0, 0, 0, RESPAWN_IMMEDIATELY)
        )
    {
        sLog->outError(LOG_FILTER_GENERAL, "BattegroundTP: Failed to spawn some objects. Battleground not created!");
        return false;
    }

    for (uint32 i = TP_GRAVEYARD_START_ALLIANCE; i < TP_MAX_GRAVEYARDS; ++i)
    {
        WorldSafeLocsEntry const* grave = sWorldSafeLocsStore.LookupEntry(BG_TP_GraveyardIds[i]);
        if (grave)
        {
            uint8 team = i % 2; ///< If 0 team == TEAM_ALLIANCE else TEAM_HORDE
            if (!AddSpiritGuide(team == TEAM_ALLIANCE ? TP_SPIRIT_ALLIANCE : TP_SPIRIT_HORDE, grave->Loc, TeamId(team)))
            {
                sLog->outError(LOG_FILTER_GENERAL, "BatteGroundTP: Failed to spawn spirit guide id: %u. Battleground not created!", grave->ID);
                return false;
            }
        }
        else
        {
            sLog->outError(LOG_FILTER_GENERAL, "BatteGroundTP: Failed to load spirit guide. Battleground not created!");
            return false;
        }
    }

    return true;
}

void BattlegroundTP::Reset()
{
    Battleground::Reset();

    for (int8 i = TEAM_ALLIANCE; i < MAX_TEAMS; ++i)
    {
        _flagKeepers[i].Clear();
        _droppedFlagGUID[i].Clear();
        _flagsDropTimer[i] = 0;
        _flagState[i] = BG_TP_FLAG_STATE_ON_BASE;
        m_TeamScores[i] = 0;
    }

    _flagsTimer = 0;
}

void BattlegroundTP::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(WorldStates::BG_WS_FLAG_CAPTURES_ALLIANCE, m_TeamScores[TEAM_ALLIANCE]);
    packet.Worldstates.emplace_back(WorldStates::BG_WS_FLAG_CAPTURES_HORDE, m_TeamScores[TEAM_HORDE]);
    packet.Worldstates.emplace_back(WorldStates::BG_WS_FLAG_CAPTURES_MAX, BG_TP_MAX_TEAM_SCORE);

    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        for (uint8 team = TEAM_ALLIANCE; team < MAX_TEAMS; ++team)
        {
            switch (_flagState[team])
            {
                case BG_TP_FLAG_STATE_ON_GROUND:
                    packet.Worldstates.emplace_back(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, -1);
                    packet.Worldstates.emplace_back(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_ALLIANCE : WorldStates::BG_WS_FLAG_STATE_HORDE, BG_TP_FLAG_STATE_ON_GROUND);
                    break;
                case BG_TP_FLAG_STATE_ON_PLAYER:
                    packet.Worldstates.emplace_back(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, 1);
                    packet.Worldstates.emplace_back(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_ALLIANCE : WorldStates::BG_WS_FLAG_STATE_HORDE, BG_TP_FLAG_STATE_ON_PLAYER);
                    break;
                default:
                    packet.Worldstates.emplace_back(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, 0);
                    packet.Worldstates.emplace_back(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_ALLIANCE : WorldStates::BG_WS_FLAG_STATE_HORDE, 1);
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

void BattlegroundTP::EndBattleground(uint32 winner)
{
    for (uint8 team = TEAM_ALLIANCE; team < MAX_TEAMS; ++team)
    {
        if (Player* player = ObjectAccessor::FindPlayer(_flagKeepers[team]))
        {
            player->RemoveAurasDueToSpell(SPELL_BG_FOCUSED_ASSAULT);
            player->RemoveAurasDueToSpell(SPELL_BG_BRUTAL_ASSAULT);
        }
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

void BattlegroundTP::HandleKillPlayer(Player* player, Player* killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (player->HasAura(SPELL_BG_HORDE_FLAG) || player->HasAura(SPELL_BG_ALLIANCE_FLAG))
        EventPlayerDroppedFlag(player);

    Battleground::HandleKillPlayer(player, killer);
}

void BattlegroundTP::HandleAreaTrigger(Player* player, uint32 trigger, bool entered)
{
    switch (trigger)
    {
        case 5904:                                          // Alliance Flag spawn
            if (_flagState[TEAM_HORDE] && !_flagState[TEAM_ALLIANCE] && GetStatus() == STATUS_IN_PROGRESS)
                if (_flagKeepers[TEAM_HORDE] == player->GetGUID())
                    EventPlayerCapturedFlag(player);
            break;
        case 5905:                                          // Horde Flag spawn
            if (_flagState[TEAM_ALLIANCE] && !_flagState[TEAM_HORDE] && GetStatus() == STATUS_IN_PROGRESS)
                if (_flagKeepers[TEAM_ALLIANCE] == player->GetGUID())
                    EventPlayerCapturedFlag(player);
            break;
        case 8967: // Alliance start loc
        case 8968: // Horde start loc
            if (!entered && GetStatus() == STATUS_WAIT_JOIN)
                player->TeleportTo(GetMapId(), GetTeamStartPosition(player->GetBGTeamId()));
            break;
        default:
            Battleground::HandleAreaTrigger(player, trigger, entered);
            break;
    }
}

bool BattlegroundTP::UpdatePlayerScore(Player* player, uint32 type, uint32 value, bool doAddHonor /*= true*/)
{
    if (!Battleground::UpdatePlayerScore(player, type, value, doAddHonor))
        return false;

    switch (type)
    {
        case SCORE_FLAG_CAPTURES:
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, TP_OBJECTIVE_CAPTURE_FLAG, 1);
            break;
        case SCORE_FLAG_RETURNS:
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, TP_OBJECTIVE_RETURN_FLAG, 1);
            break;
        default:
            break;
    }

    return true;
}

WorldSafeLocsEntry const* BattlegroundTP::GetClosestGraveYard(Player* player)
{
    if (!player)
        return nullptr;

    TeamId team = player->GetBGTeamId();

    if (GetStatus() != STATUS_IN_PROGRESS)
        return sWorldSafeLocsStore.LookupEntry(BG_TP_GraveyardIds[TP_GRAVEYARD_FLAGROOM_ALLIANCE + team]);

    WorldSafeLocsEntry const* grave_enemy_base = sWorldSafeLocsStore.LookupEntry(BG_TP_GraveyardIds[TP_GRAVEYARD_FLAGROOM_ALLIANCE + (team ^ 1)]);
    WorldSafeLocsEntry const* grave_enemy_middle = sWorldSafeLocsStore.LookupEntry(BG_TP_GraveyardIds[TP_GRAVEYARD_MIDDLE_ALLIANCE + (team ^ 1)]);

    if (player->GetDistance2d(grave_enemy_base->Loc.X, grave_enemy_base->Loc.Y) < player->GetDistance2d(grave_enemy_middle->Loc.X, grave_enemy_middle->Loc.Y))
        return sWorldSafeLocsStore.LookupEntry(BG_TP_GraveyardIds[TP_GRAVEYARD_START_ALLIANCE + team]);

    return sWorldSafeLocsStore.LookupEntry(BG_TP_GraveyardIds[TP_GRAVEYARD_MIDDLE_ALLIANCE + team]);
}

void BattlegroundTP::EventPlayerDroppedFlag(Player* Source)
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
            _flagState[TEAM_HORDE] = BG_TP_FLAG_STATE_ON_GROUND;
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
            _flagState[TEAM_ALLIANCE] = BG_TP_FLAG_STATE_ON_GROUND;
            Source->CastSpell(Source, SPELL_BG_ALLIANCE_FLAG_DROPPED, true);
            set = true;
        }
    }

    if (set)
    {
        Source->CastSpell(Source, SPELL_BG_RECENTLY_DROPPED_FLAG, true);
        UpdateFlagState(Source->GetBGTeam(), BG_TP_FLAG_STATE_WAIT_RESPAWN);

        SendMessageToAll(teamID == TEAM_ALLIANCE ? 9806 : 9805, teamID == TEAM_ALLIANCE ? CHAT_MSG_BG_SYSTEM_HORDE : CHAT_MSG_BG_SYSTEM_ALLIANCE, Source);
        UpdateWorldState(teamID == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_HORDE : WorldStates::BG_WS_FLAG_UNK_ALLIANCE, uint32(-1));

        _flagsDropTimer[teamID ? TEAM_ALLIANCE : TEAM_HORDE] = BG_TP_FLAG_DROP_TIME;
    }
}

void BattlegroundTP::EventPlayerClickedOnFlag(Player* source, GameObject* object)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    TeamId team = source->GetBGTeamId();

    if (source->IsWithinDistInMap(object, 10))
    {
        switch (_flagState[team ^ 1])
        {
            case BG_TP_FLAG_STATE_ON_BASE:
            {
                if (BgObjects[BG_TP_OBJECT_A_FLAG + (team ^ 1)] == object->GetGUID())
                {
                    SpawnBGObject(team == TEAM_ALLIANCE ? BG_TP_OBJECT_H_FLAG : BG_TP_OBJECT_A_FLAG, RESPAWN_ONE_DAY);
                    UpdateFlagState(team ^ 1, BG_TP_FLAG_STATE_ON_PLAYER, source->GetGUID());
                    source->CastSpell(source, team == TEAM_ALLIANCE ? SPELL_BG_HORDE_FLAG : SPELL_BG_ALLIANCE_FLAG, true);
                    source->GetAchievementMgr().StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_SPELL_TARGET2, team == TEAM_ALLIANCE ? SPELL_BG_HORDE_FLAG_PICKED_UP : SPELL_BG_ALLIANCE_FLAG_PICKED_UP);

                    if (!_flagKeepers[team].IsEmpty())
                        _bothFlagsKept = true;

                    PlayeCapturePointSound(NODE_STATUS_ASSAULT, team);
                    SendBroadcastTextToAll(team == TEAM_ALLIANCE ? 9807 : 9804, team == TEAM_ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, source);
                }
                break;
            }
            case BG_TP_FLAG_STATE_ON_GROUND:
            {
                if (_droppedFlagGUID[team ^ 1] == object->GetGUID())
                {
                    source->CastSpell(source, team == TEAM_ALLIANCE ? SPELL_BG_HORDE_FLAG : SPELL_BG_ALLIANCE_FLAG, true);
                    UpdateFlagState(team ^ 1, BG_TP_FLAG_STATE_ON_PLAYER, source->GetGUID());

                    if (_flagDebuffState  && _flagDebuffState < 6)
                        source->CastCustomSpell(SPELL_BG_FOCUSED_ASSAULT, SPELLVALUE_AURA_STACK, _flagDebuffState, source);
                    else if (_flagDebuffState >= 6)
                        source->CastCustomSpell(SPELL_BG_BRUTAL_ASSAULT, SPELLVALUE_AURA_STACK, _flagDebuffState, source);

                    _flagsDropTimer[team ^ 1] = 0;

                    PlayeCapturePointSound(NODE_STATUS_ASSAULT, team);
                    SendBroadcastTextToAll(team == TEAM_ALLIANCE ? 9807 : 9804, team == TEAM_ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, source);
                }
                break;
            }
        }

        if (_flagState[team] == BG_TP_FLAG_STATE_ON_GROUND && _droppedFlagGUID[team] == object->GetGUID())
        {
            UpdateFlagState(team, BG_TP_FLAG_STATE_WAIT_RESPAWN);
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

void BattlegroundTP::EventPlayerCapturedFlag(Player* source)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    TeamId team = source->GetBGTeamId();

    source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
    _flagDebuffState = 0;

    SpawnBGObject(BG_TP_OBJECT_A_FLAG, RESPAWN_ONE_DAY);
    SpawnBGObject(BG_TP_OBJECT_H_FLAG, RESPAWN_ONE_DAY);

    source->RemoveAurasDueToSpell(SPELL_BG_FOCUSED_ASSAULT);
    source->RemoveAurasDueToSpell(SPELL_BG_BRUTAL_ASSAULT);

    RewardHonorToTeam(GetBonusHonorFromKill(2), source->GetBGTeam());

    m_TeamScores[team]++;

    UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_CAPTURES_ALLIANCE : WorldStates::BG_WS_FLAG_CAPTURES_HORDE, m_TeamScores[team]);
    UpdateWorldState(WorldStates::BG_WS_FLAG_UNKNOWN, -1);
    UpdateFlagState(team ^ 1, BG_TP_FLAG_STATE_WAIT_RESPAWN);
    UpdatePlayerScore(source, SCORE_FLAG_CAPTURES, 1);
    UpdateWorldState(WorldStates::BG_WS_UNKNOWN, 1);

    m_lastFlagCaptureTeam = source->GetBGTeam();

    PlayeCapturePointSound(NODE_STATUS_CAPTURE, team);
    SendBroadcastTextToAll(team == TEAM_ALLIANCE ? 9801 : 9802, team == TEAM_ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, source);

    source->RemoveAurasDueToSpell(team == TEAM_ALLIANCE ? SPELL_BG_HORDE_FLAG : SPELL_BG_ALLIANCE_FLAG);

    Battleground::SendBattleGroundPoints(team != TEAM_ALLIANCE, m_TeamScores[team]);

    if (m_TeamScores[team] == BG_TP_MAX_TEAM_SCORE)
        EndBattleground(team);
    else
        _flagsTimer = BG_TP_FLAG_RESPAWN_TIME;
}

void BattlegroundTP::RemovePlayer(Player* player, ObjectGuid guid, uint32 /* team */)
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
                    sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundTP: An error has occurred in RemovePlayer: player with GUID: %u haven't been found. (_bothflagsKept is TRUE).", _flagKeepers[team]);
            }

            _bothFlagsKept = false;
            _flagDebuffState = 0;
            _flagSpellForceTimer = 0;
        }
    }
}

void BattlegroundTP::RespawnFlag(uint32 team, bool captured)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (captured)
    {
        SpawnBGObject(BG_TP_OBJECT_H_FLAG, RESPAWN_IMMEDIATELY);
        SpawnBGObject(BG_TP_OBJECT_A_FLAG, RESPAWN_IMMEDIATELY);
        SendBroadcastTextToAll(9803, CHAT_MSG_BG_SYSTEM_NEUTRAL);
    }
    else
    {
        SpawnBGObject(BG_TP_OBJECT_A_FLAG + team, RESPAWN_IMMEDIATELY);
        SendBroadcastTextToAll(team == TEAM_ALLIANCE ? 24891 : 24892, CHAT_MSG_BG_SYSTEM_NEUTRAL);
    }

    PlayeCapturePointSound(NODE_STATUS_NEUTRAL, static_cast<TeamId>(team));

    UpdateFlagState(team, BG_TP_FLAG_STATE_ON_BASE);
}

void BattlegroundTP::UpdateFlagState(uint32 team, uint32 value, ObjectGuid flagKeeperGUID)
{
    switch (value)
    {
        case BG_TP_FLAG_STATE_WAIT_RESPAWN:
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, 0);
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_HORDE : WorldStates::BG_WS_FLAG_STATE_ALLIANCE, BG_TP_FLAG_STATE_WAIT_RESPAWN);
            break;
        case BG_TP_FLAG_STATE_ON_BASE:
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, 0);
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_HORDE : WorldStates::BG_WS_FLAG_STATE_ALLIANCE, 1);
            break;
        case BG_TP_FLAG_STATE_ON_GROUND:
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, -1);
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_HORDE : WorldStates::BG_WS_FLAG_STATE_ALLIANCE, BG_TP_FLAG_STATE_ON_GROUND);
            break;
        case BG_TP_FLAG_STATE_ON_PLAYER:
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_UNK_ALLIANCE : WorldStates::BG_WS_FLAG_UNK_HORDE, 1);
            UpdateWorldState(team == TEAM_ALLIANCE ? WorldStates::BG_WS_FLAG_STATE_HORDE : WorldStates::BG_WS_FLAG_STATE_ALLIANCE, BG_TP_FLAG_STATE_ON_PLAYER);
            break;
        default:
            break;
    }

    _flagState[team] = value;
    _flagKeepers[team] = flagKeeperGUID;
}
