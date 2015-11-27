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

#ifndef __BATTLEGROUNDWS_H
#define __BATTLEGROUNDWS_H

#include "Battleground.h"
#include "BattlegroundScore.h"

enum BG_WS_TimerOrScore
{
    BG_WS_MAX_TEAM_SCORE                    = 3,
    BG_WS_FLAG_RESPAWN_TIME                 = 23000,
    BG_WS_FLAG_DROP_TIME                    = 10000,
    BG_WS_SPELL_FORCE_TIME                  = 600000,
    BG_WS_SPELL_BRUTAL_TIME                 = 900000
};

enum BG_WS_ObjectTypes
{
    BG_WS_OBJECT_DOOR_A_1       = 0,
    BG_WS_OBJECT_DOOR_A_2       = 1,
    BG_WS_OBJECT_DOOR_A_3       = 2,
    BG_WS_OBJECT_DOOR_A_4       = 3,
    BG_WS_OBJECT_DOOR_A_5       = 4,
    BG_WS_OBJECT_DOOR_A_6       = 5,
    BG_WS_OBJECT_DOOR_H_1       = 6,
    BG_WS_OBJECT_DOOR_H_2       = 7,
    BG_WS_OBJECT_DOOR_H_3       = 8,
    BG_WS_OBJECT_DOOR_H_4       = 9,
    BG_WS_OBJECT_A_FLAG         = 10,
    BG_WS_OBJECT_H_FLAG         = 11,
    BG_WS_OBJECT_SPEEDBUFF_1    = 12,
    BG_WS_OBJECT_SPEEDBUFF_2    = 13,
    BG_WS_OBJECT_REGENBUFF_1    = 14,
    BG_WS_OBJECT_REGENBUFF_2    = 15,
    BG_WS_OBJECT_BERSERKBUFF_1  = 16,
    BG_WS_OBJECT_BERSERKBUFF_2  = 17,
    BG_WS_OBJECT_MAX            = 18
};

enum BG_WS_ObjectEntry
{
    BG_OBJECT_DOOR_A_1_WS_ENTRY          = 179918,
    BG_OBJECT_DOOR_A_2_WS_ENTRY          = 179919,
    BG_OBJECT_DOOR_A_3_WS_ENTRY          = 179920,
    BG_OBJECT_DOOR_A_4_WS_ENTRY          = 179921,
    BG_OBJECT_DOOR_A_5_WS_ENTRY          = 180322,
    BG_OBJECT_DOOR_A_6_WS_ENTRY          = 180322,
    BG_OBJECT_DOOR_H_1_WS_ENTRY          = 179916,
    BG_OBJECT_DOOR_H_2_WS_ENTRY          = 179917,
    BG_OBJECT_DOOR_H_3_WS_ENTRY          = 180322,
    BG_OBJECT_DOOR_H_4_WS_ENTRY          = 180322,
    BG_OBJECT_A_FLAG_WS_ENTRY            = 179830,
    BG_OBJECT_H_FLAG_WS_ENTRY            = 179831,
    BG_OBJECT_A_FLAG_GROUND_WS_ENTRY     = 179785,
    BG_OBJECT_H_FLAG_GROUND_WS_ENTRY     = 179786
};

enum BG_WS_FlagState
{
    BG_WS_FLAG_STATE_ON_BASE                = 0,
    BG_WS_FLAG_STATE_WAIT_RESPAWN,
    BG_WS_FLAG_STATE_ON_PLAYER,
    BG_WS_FLAG_STATE_ON_GROUND,
};

enum BG_WS_Graveyards
{
    WS_GRAVEYARD_FLAGROOM_ALLIANCE = 769,
    WS_GRAVEYARD_FLAGROOM_HORDE    = 770,
    WS_GRAVEYARD_MAIN_ALLIANCE     = 771,
    WS_GRAVEYARD_MAIN_HORDE        = 772
};

enum BG_WS_CreatureTypes
{
    WS_SPIRIT_MAIN_ALLIANCE   = 0,
    WS_SPIRIT_MAIN_HORDE      = 1,

    BG_CREATURES_MAX_WS       = 2
};

enum BG_WS_Objectives
{
    WS_OBJECTIVE_CAPTURE_FLAG   = 42,
    WS_OBJECTIVE_RETURN_FLAG    = 44
};

struct BattlegroundWGScore final : public BattlegroundScore
{
    friend class BattlegroundWS;

    protected:
        BattlegroundWGScore(ObjectGuid playerGuid, TeamId team) : BattlegroundScore(playerGuid, team), FlagCaptures(0), FlagReturns(0) { }

        void UpdateScore(uint32 type, uint32 value) override
        {
            switch (type)
            {
                case SCORE_FLAG_CAPTURES:
                    FlagCaptures += value;
                    break;
                case SCORE_FLAG_RETURNS:
                    FlagReturns += value;
                    break;
                default:
                    BattlegroundScore::UpdateScore(type, value);
                    break;
            }
        }

        void BuildObjectivesBlock(std::vector<int32>& stats) override
        {
            stats.push_back(FlagCaptures);
            stats.push_back(FlagReturns);
        }

        uint32 FlagCaptures;
        uint32 FlagReturns;
};

class BattlegroundWS : public Battleground
{
    friend class BattlegroundMgr;

    public:
        BattlegroundWS();
        ~BattlegroundWS();

        void PostUpdateImpl(uint32 diff) override;

        void AddPlayer(Player* player) override;
        void RemovePlayer(Player* player, ObjectGuid guid, uint32 team) override;
        void GetPlayerPositionData(std::vector<WorldPackets::Battleground::PlayerPositions::BattlegroundPlayerPosition>* positions) const override;

        void StartingEventCloseDoors() override;
        void StartingEventOpenDoors() override;
        bool SetupBattleground() override;
        void Reset() override;
        void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet) override;

        void EndBattleground(uint32 winner) override;

        WorldSafeLocsEntry const* GetClosestGraveYard(Player* player) override;
        void HandleKillPlayer(Player *player, Player *killer) override;

        ObjectGuid GetFlagPickerGUID(int32 team) const override
        {
            if (team == TEAM_ALLIANCE || team == TEAM_HORDE)
                return _flagKeepers[team];

            return ObjectGuid::Empty;
        }

        void SetAllianceFlagPicker(ObjectGuid const& guid) { _flagKeepers[TEAM_ALLIANCE] = guid; }
        void SetHordeFlagPicker(ObjectGuid const& guid) { _flagKeepers[TEAM_HORDE] = guid; }
        bool IsAllianceFlagPickedup() const { return !_flagKeepers[TEAM_ALLIANCE].IsEmpty(); }
        bool IsHordeFlagPickedup() const { return !_flagKeepers[TEAM_HORDE].IsEmpty(); }
        void HandleAreaTrigger(Player* player, uint32 trigger, bool entered) override;
        uint8 GetFlagState(uint32 team) { return _flagState[GetTeamIndexByTeamId(team)]; }
        bool UpdatePlayerScore(Player* player, uint32 type, uint32 value, bool doAddHonor = true) override;
        void SetDroppedFlagGUID(ObjectGuid guid, uint32 TeamID) { _droppedFlagGUID[GetTeamIndexByTeamId(TeamID)] = guid; }

private:
        void UpdateFlagState(uint32 team, uint32 value, ObjectGuid flagKeeperGUID = ObjectGuid::Empty);
        void RespawnFlag(uint32 team, bool captured = false);
        void EventPlayerDroppedFlag(Player* source) override;
        void EventPlayerClickedOnFlag(Player* source, GameObject* object) override;
        void EventPlayerCapturedFlag(Player* source);

        ObjectGuid _flagKeepers[MAX_TEAMS];
        ObjectGuid _droppedFlagGUID[MAX_TEAMS];
        uint8 _flagState[MAX_TEAMS];
        int32 _flagsTimer;
        int32 _flagsDropTimer[MAX_TEAMS];
        int32 _flagSpellForceTimer;
        bool _bothFlagsKept;
        uint8 _flagDebuffState;
        uint32 _reputationCapture;
        int32 _flagPosTimer;
};

#endif
