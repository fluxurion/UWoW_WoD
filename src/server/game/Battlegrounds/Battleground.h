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

#ifndef __BATTLEGROUND_H
#define __BATTLEGROUND_H

#include "Common.h"
#include "SharedDefines.h"
#include "DBCEnums.h"
#include "ObjectDefines.h"
#include "ByteBuffer.h"
#include "Duration.h"
#include "Packets/WorldStatePackets.h"
#include "Packets/BattlegroundPackets.h"
#include "ArenaScore.h"

class Creature;
class GameObject;
class Group;
class Player;
class WorldPacket;
class BattlegroundMap;

struct PvPDifficultyEntry;
struct WorldSafeLocsEntry;

enum BattlegroundMisc
{
    BG_EVENT_START_BATTLE           = 8563,
};

enum BattlegroundSounds
{
    BG_SOUND_START                              = 3439,
    BG_SOUND_NEAR_VICTORY                       = 8456,
    BG_SOUND_FLAG_RESET                         = 8192,

    BG_SOUND_ALLIANCE_WIN                       = 8455,
    BG_SOUND_HORDE_WIN                          = 8454,
    
    BG_SOUND_CAPTURE_POINT_ASSAULT_HORDE        = 8212,
    BG_SOUND_CAPTURE_POINT_ASSAULT_ALLIANCE     = 8174,

    BG_SOUND_CAPTURE_POINT_CAPTURED_HORDE       = 8213,
    BG_SOUND_CAPTURE_POINT_CAPTURED_ALLIANCE    = 8173,

    BG_SOUND_FLAG_PLACED_ALLIANCE               = 8232,
    BG_SOUND_FLAG_PLACED_HORDE                  = 8333,
};

enum BattlegroundQuests
{
    SPELL_WS_QUEST_REWARD           = 43483,
    SPELL_AB_QUEST_REWARD           = 43484,
    SPELL_AV_QUEST_REWARD           = 43475,
    SPELL_AV_QUEST_KILLED_BOSS      = 23658,
    SPELL_EY_QUEST_REWARD           = 43477,
    SPELL_SA_QUEST_REWARD           = 61213,
    SPELL_AB_QUEST_REWARD_4_BASES   = 24061,
    SPELL_AB_QUEST_REWARD_5_BASES   = 24064
};

enum BattlegroundMarks
{
    SPELL_WS_MARK_LOSER             = 24950,
    SPELL_WS_MARK_WINNER            = 24951,
    SPELL_AB_MARK_LOSER             = 24952,
    SPELL_AB_MARK_WINNER            = 24953,
    SPELL_AV_MARK_LOSER             = 24954,
    SPELL_AV_MARK_WINNER            = 24955,
    SPELL_SA_MARK_WINNER            = 61160,
    SPELL_SA_MARK_LOSER             = 61159,
    ITEM_AV_MARK_OF_HONOR           = 20560,
    ITEM_WS_MARK_OF_HONOR           = 20558,
    ITEM_AB_MARK_OF_HONOR           = 20559,
    ITEM_EY_MARK_OF_HONOR           = 29024,
    ITEM_SA_MARK_OF_HONOR           = 42425
};

enum BattlegroundMarksCount
{
    ITEM_WINNER_COUNT               = 3,
    ITEM_LOSER_COUNT                = 1
};

enum BattlegroundCreatures
{
    BG_CREATURE_ENTRY_A_SPIRITGUIDE      = 13116,           // alliance
    BG_CREATURE_ENTRY_H_SPIRITGUIDE      = 13117,           // horde
};

enum BattlegroundFactions
{
    BG_FACTION_ALLIANCE     = 1732,
    BG_FACTION_HORDE        = 1735,
    BG_FACTION_VILLIAN      = 35,

    BG_MAX_FACTIONS         = 2,
    BF_MAX_FACTIONS         = 3,
};

uint32 const BgFactions[BG_MAX_FACTIONS] = { BG_FACTION_ALLIANCE, BG_FACTION_HORDE };
uint32 const BfFactions[BF_MAX_FACTIONS] = { BG_FACTION_ALLIANCE, BG_FACTION_HORDE, BG_FACTION_VILLIAN };

enum BattlegroundSpells
{
    SPELL_BG_FOCUSED_ASSAULT            = 46392,
    SPELL_BG_BRUTAL_ASSAULT             = 46393,

    SPELL_BG_HORDE_FLAG                 = 23333,
    SPELL_BG_HORDE_FLAG_DROPPED         = 23334,
    SPELL_BG_HORDE_FLAG_PICKED_UP       = 61266,    ///< Fake Spell - Used as a start timer event
    SPELL_BG_HORDE_GOLD_FLAG            = 35774,
    SPELL_BG_HORDE_GREEN_FLAG           = 35775,

    SPELL_BG_ALLIANCE_FLAG              = 23335,
    SPELL_BG_ALLIANCE_FLAG_DROPPED      = 23336,
    SPELL_BG_ALLIANCE_FLAG_PICKED_UP    = 61265,    ///< Fake Spell - Used as a start timer event
    SPELL_BG_ALLIANCE_GOLD_FLAG         = 32724,
    SPELL_BG_ALLIANCE_GREEN_FLAG        = 32725,

    SPELL_BG_WAITING_FOR_RESURRECT      = 2584,
    SPELL_BG_SPIRIT_HEAL_CHANNEL        = 22011,
    SPELL_BG_SPIRIT_HEAL                = 22012,
    SPELL_BG_RESURRECTION_VISUAL        = 24171,
    SPELL_BG_PREPARATION                = 44521,
    SPELL_BG_SPIRIT_HEAL_MANA           = 44535,
    SPELL_BG_RECENTLY_DROPPED_FLAG      = 42792,
    SPELL_BG_AURA_PLAYER_INACTIVE       = 43681,
    SPELL_BG_HONORABLE_DEFENDER_25Y     = 68652,
    SPELL_BG_HONORABLE_DEFENDER_60Y     = 66157,
    SPELL_BG_THE_LAST_STANDING          = 26549,
    SPELL_BG_ARENA_DUMPENING            = 110310,
    SPELL_BATTLE_FATIGUE                = 134735,
    
    SPELL_ARENA_PREPARATION             = 32727,
    SPELL_ARENA_PEREODIC_AURA           = 74410,
    SPELL_ENTERING_BATTLEGROUND         = 91318,
    SPELL_RATED_PVP_TRANSFORM_SUPPRESSION = 182306,
};

static Milliseconds const PositionBroadcastUpdate = Seconds(5);

enum BattlegroundTimeIntervals
{
    RESURRECTION_INTERVAL           = 30000,                // ms
    INVITATION_REMIND_TIME          = 20000,                // ms
    BG_INVITE_ACCEPT_WAIT_TIME      = 90000,                // ms
    ARENA_INVITE_ACCEPT_WAIT_TIME   = 30000,                // ms
    TIME_AUTOCLOSE_BATTLEGROUND     = 120000,               // ms
    MAX_OFFLINE_TIME                = 300,                  // secs
    RESPAWN_ONE_DAY                 = 86400,                // secs
    RESPAWN_IMMEDIATELY             = 0,                    // secs
    BUFF_RESPAWN_TIME               = 180,                  // secs
    BATTLEGROUND_COUNTDOWN_MAX      = 120,                  // secs
    ARENA_COUNTDOWN_MAX             = 60                    // secs
};

enum BattlegroundBuffObjects
{
    BG_OBJECTID_SPEEDBUFF_ENTRY     = 179871,
    BG_OBJECTID_REGENBUFF_ENTRY     = 179904,
    BG_OBJECTID_BERSERKERBUFF_ENTRY = 179905
};

uint32 const Buff_Entries[3] = { BG_OBJECTID_SPEEDBUFF_ENTRY, BG_OBJECTID_REGENBUFF_ENTRY, BG_OBJECTID_BERSERKERBUFF_ENTRY };
Milliseconds const m_messageTimer[4] = {Minutes(2), Minutes(1), Seconds(30), Seconds(0)}; 

enum BattlegroundStatus
{
    STATUS_NONE         = 0,                                // first status, should mean bg is not instance
    STATUS_WAIT_QUEUE   = 1,                                // means bg is empty and waiting for queue
    STATUS_WAIT_JOIN    = 2,                                // this means, that BG has already started and it is waiting for more players
    STATUS_IN_PROGRESS  = 3,                                // means bg is running
    STATUS_WAIT_LEAVE   = 4                                 // means some faction has won BG and it is ending
};

struct BattlegroundPlayer
{
    time_t  OfflineRemoveTime;                              // for tracking and removing offline players from queue after 5 minutes
    uint32  Team;                                           // Player's team
};

struct BattlegroundObjectInfo
{
    BattlegroundObjectInfo() : object(nullptr), timer(0), spellid(0) { }

    GameObject* object;
    int32 timer;
    uint32 spellid;
};

// handle the queue types and bg types separately to enable joining queue for different sized arenas at the same time
enum BattlegroundQueueTypeId
{
    BATTLEGROUND_QUEUE_NONE     = 0,
    BATTLEGROUND_QUEUE_AV       = 1,
    BATTLEGROUND_QUEUE_WS       = 2,
    BATTLEGROUND_QUEUE_AB       = 3,
    BATTLEGROUND_QUEUE_EY       = 4,
    BATTLEGROUND_QUEUE_SA       = 5,
    BATTLEGROUND_QUEUE_IC       = 6,
    BATTLEGROUND_QUEUE_TP       = 7,
    BATTLEGROUND_QUEUE_BFG      = 8,
    BATTLEGROUND_QUEUE_RB       = 9,
    BATTLEGROUND_QUEUE_2v2      = 10,
    BATTLEGROUND_QUEUE_3v3      = 11,
    BATTLEGROUND_QUEUE_5v5      = 12,
    BATTLEGROUND_QUEUE_KT       = 13,
    BATTLEGROUND_QUEUE_CTF3     = 14,
    BATTLEGROUND_QUEUE_SSM      = 15,
    BATTLEGROUND_QUEUE_TV       = 16,
    BATTLEGROUND_QUEUE_RBG      = 17,
    BATTLEGROUND_QUEUE_DG       = 18,

    MAX_BATTLEGROUND_QUEUE_TYPES
};

enum JoinType : int8
{
    JOIN_TYPE_NONE              = 0,
    JOIN_TYPE_ARENA_2v2         = 2,
    JOIN_TYPE_ARENA_3v3         = 3,
    JOIN_TYPE_ARENA_5v5         = 5,
    JOIN_TYPE_RATED_BG_10v10    = 10,
    JOIN_TYPE_RATED_BG_15v15    = 15,
    JOIN_TYPE_RATED_BG_25v25    = 25,
};

enum BattlegroundType
{
    TYPE_BATTLEGROUND     = 3,
    TYPE_ARENA            = 4
};

enum BattlegroundWinner
{
    WINNER_HORDE            = 0,
    WINNER_ALLIANCE         = 1,
    WINNER_NONE             = 2
};

enum BattlegroundStartingEvents
{
    BG_STARTING_EVENT_NONE  = 0x00,
    BG_STARTING_EVENT_1     = 0x01,
    BG_STARTING_EVENT_2     = 0x02,
    BG_STARTING_EVENT_3     = 0x04,
    BG_STARTING_EVENT_4     = 0x08
};

enum BattlegroundStartingEventsIds
{
    BG_STARTING_EVENT_FIRST     = 0,
    BG_STARTING_EVENT_SECOND    = 1,
    BG_STARTING_EVENT_THIRD     = 2,
    BG_STARTING_EVENT_FOURTH    = 3,

    BG_STARTING_EVENT_COUNT
};

static uint32 const ArenaBroadcastTexts[BG_STARTING_EVENT_COUNT] = {15740, 15741, 15739, 15742};
static uint32 const BattlegroundBroadcastTexts[BG_STARTING_EVENT_COUNT] = {71789, 71790, 71791, 71792};

enum BgNodeStatus
{
    NODE_STATUS_NEUTRAL             = 0,
    NODE_STATUS_ASSAULT             = 1,
    NODE_STATUS_CAPTURE             = 2
};

enum BgNodeStatusIcon
{
    //NODE_STATUS_ICON_NEUTRAL,
    //NODE_STATUS_ICON_ALLIANCE,
    //NODE_STATUS_ICON_CON_HORDE      = 3,
    //NODE_STATUS_ICON_CON_ALLIANCE,
    //NODE_STATUS_ICON_HORDE,

    //NODE_STATUS_ICON_CAPTURED_ALLIANCE = 5, // NODE_STATE_ALLIANCE_CAPTURE

    //NODE_STATUS_ICON_MAX
};

enum GroupJoinBattlegroundResult
{
    ERR_BATTLEGROUND_NONE                           = 0,
    ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS           = 2,        // You cannot join the battleground yet because you or one of your party members is flagged as a Deserter.
    ERR_ARENA_TEAM_PARTY_SIZE                       = 3,        // Incorrect party size for this arena.
    ERR_BATTLEGROUND_TOO_MANY_QUEUES                = 4,        // You can only be queued for 2 battles at once
    ERR_BATTLEGROUND_CANNOT_QUEUE_FOR_RATED         = 5,        // You cannot queue for a rated match while queued for other battles
    ERR_BATTLEDGROUND_QUEUED_FOR_RATED              = 6,        // You cannot queue for another battle while queued for a rated arena match
    ERR_BATTLEGROUND_TEAM_LEFT_QUEUE                = 7,        // Your team has left the arena queue
    ERR_BATTLEGROUND_NOT_IN_BATTLEGROUND            = 8,        // You can't do that in a battleground.
    ERR_BATTLEGROUND_JOIN_XP_GAIN                   = 9,        // wtf, doesn't exist in client...
    ERR_BATTLEGROUND_JOIN_RANGE_INDEX               = 10,       // Cannot join the queue unless all members of your party are in the same battleground level range.
    ERR_BATTLEGROUND_JOIN_TIMED_OUT                 = 11,       // %s was unavailable to join the queue. (uint64 guid exist in client cache)
    //ERR_BATTLEGROUND_JOIN_TIMED_OUT               = 12,       // same as 11
    //ERR_BATTLEGROUND_TEAM_LEFT_QUEUE              = 13,       // same as 7
    ERR_LFG_CANT_USE_BATTLEGROUND                   = 14,       // You cannot queue for a battleground or arena while using the dungeon system.
    ERR_IN_RANDOM_BG                                = 15,       // Can't do that while in a Random Battleground queue.
    ERR_IN_NON_RANDOM_BG                            = 16,       // Can't queue for Random Battleground while in another Battleground queue.
    ERR_BG_DEVELOPER_ONLY                           = 17,
    ERR_BATTLEGROUND_INVITATION_DECLINED            = 18,
    ERR_MEETING_STONE_NOT_FOUND                     = 19,
    ERR_WARGAME_REQUEST_FAILURE                     = 20,
    ERR_BATTLEFIELD_TEAM_PARTY_SIZE                 = 22,
    ERR_NOT_ON_TOURNAMENT_REALM                     = 23,
    ERR_BATTLEGROUND_PLAYERS_FROM_DIFFERENT_REALMS  = 24,
    ERR_LEAVE_QUEUE                                 = 30,       // just leave queue
    ERR_REMOVE_FROM_PVP_QUEUE_GRANT_LEVEL           = 33,
    ERR_REMOVE_FROM_PVP_QUEUE_FACTION_CHANGE        = 34,
    ERR_BATTLEGROUND_JOIN_FAILED                    = 35,
    ERR_BATTLEGROUND_DUPE_QUEUE                     = 43
};

enum BattlegroundPlayerPositionConstants
{
    PLAYER_POSITION_ICON_NONE           = 0,
    PLAYER_POSITION_ICON_HORDE_FLAG     = 1,
    PLAYER_POSITION_ICON_ALLIANCE_FLAG  = 2,

    PLAYER_POSITION_ARENA_SLOT_NONE     = 1,
    PLAYER_POSITION_ARENA_SLOT_1        = 2,
    PLAYER_POSITION_ARENA_SLOT_2        = 3,
    PLAYER_POSITION_ARENA_SLOT_3        = 4,
    PLAYER_POSITION_ARENA_SLOT_4        = 5,
    PLAYER_POSITION_ARENA_SLOT_5        = 6
};

/*
This class is used to:
1. Add player to battleground
2. Remove player from battleground
3. some certain cases, same for all battlegrounds
4. It has properties same for all battlegrounds
*/
class Battleground
{
    public:
        Battleground();
        virtual ~Battleground();

        void Update(uint32 diff);

        typedef std::map<ObjectGuid, BattlegroundPlayer> BattlegroundPlayerMap;
        typedef std::map<ObjectGuid, BattlegroundScore*> BattlegroundScoreMap;

        virtual bool SetupBattleground() { return true; }   // must be implemented in BG subclass
        virtual void Reset();                               // resets all common properties for battlegrounds, must be implemented and called in BG subclass
        virtual void StartingEventCloseDoors() { }
        virtual void StartingEventOpenDoors() { }
        virtual void ResetBGSubclass() { }                  // must be implemented in BG subclass
        virtual void DestroyGate(Player* /*player*/, GameObject* /*go*/) { }

        /* achievement req. */
        virtual bool IsAllNodesConrolledByTeam(uint32 /*team*/) const { return false; }
        bool IsTeamScoreInRange(uint32 team, uint32 minScore, uint32 maxScore) const;
        void StartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry);

        /* Battleground */
        // Get methods:
        char const* GetName() const         { return m_Name; }
        uint64 GetQueueID() const { return m_QueueID; }
        BattlegroundTypeId GetTypeID(bool GetRandom = false) const { return GetRandom ? m_RandomTypeID : m_TypeID; }
        BattlegroundBracketId GetBracketId() const { return m_BracketId; }
        uint32 GetInstanceID() const        { return m_InstanceID; }
        BattlegroundStatus GetStatus() const { return m_Status; }
        uint32 GetClientInstanceID() const  { return m_ClientInstanceID; }
        Milliseconds GetElapsedTime() const       { return m_StartTime; }
        Milliseconds GetRemainingTime() const     { return m_EndTime; }
        uint32 GetLastResurrectTime() const { return m_LastResurrectTime; }
        uint32 GetMaxPlayers() const        { return m_MaxPlayers; }
        uint32 GetMinPlayers() const        { return m_MinPlayers; }

        uint32 GetMinLevel() const          { return m_LevelMin; }
        uint32 GetMaxLevel() const          { return m_LevelMax; }

        uint32 GetMaxPlayersPerTeam() const { return m_MaxPlayersPerTeam; }
        uint32 GetMinPlayersPerTeam() const { return m_MinPlayersPerTeam; }

        Milliseconds GetStartDelayTime() const { return m_StartDelayTime; }
        uint8 GetJoinType() const          { return m_JoinType; }
        uint8 GetWinner() const             { return m_Winner; }
        uint32 GetScriptId() const          { return ScriptId; }
        uint32 GetBonusHonorFromKill(uint32 kills) const;
        bool IsRandom() const { return m_IsRandom; }

        // Set methods:
        void SetQueueID(uint64 newID)        { m_QueueID = newID; }
        void SetName(char const* Name)      { m_Name = Name; } 
        void SetTypeID(BattlegroundTypeId TypeID) { m_TypeID = TypeID; }
        void SetRandomTypeID(BattlegroundTypeId TypeID) { m_RandomTypeID = TypeID; }
        //here we can count minlevel and maxlevel for players
        void SetBracket(PvPDifficultyEntry const* bracketEntry);
        void SetInstanceID(uint32 InstanceID) { m_InstanceID = InstanceID; }
        void SetStatus(BattlegroundStatus Status) { m_Status = Status; }
        void SetClientInstanceID(uint32 InstanceID) { m_ClientInstanceID = InstanceID; }
        void SetElapsedTime(Milliseconds Time)      { m_StartTime = Time; }
        void SetRemainingTime(Milliseconds Time)        { m_EndTime = Time; }
        void SetLastResurrectTime(uint32 Time) { m_LastResurrectTime = Time; }
        void SetMaxPlayers(uint32 MaxPlayers) { m_MaxPlayers = MaxPlayers; }
        void SetMinPlayers(uint32 MinPlayers) { m_MinPlayers = MinPlayers; }
        void SetLevelRange(uint32 min, uint32 max) { m_LevelMin = min; m_LevelMax = max; }
        void SetRated(bool state) { m_IsRated = state; }
        void SetTournamentRules(bool state) { m_IsRated = state; }
        void SetJoinType(uint8 type)       { m_JoinType = type; }
        void SetArenaorBGType(bool _isArena) { m_IsArena = _isArena; }
        void SetWinner(uint8 winner)        { m_Winner = winner; }
        void SetScriptId(uint32 scriptId)   { ScriptId = scriptId; }

        void ModifyStartDelayTime(Milliseconds diff) { m_StartDelayTime -= diff; }
        void SetStartDelayTime(Milliseconds Time)    { m_StartDelayTime = Time; }

        void SetMaxPlayersPerTeam(uint32 MaxPlayers) { m_MaxPlayersPerTeam = MaxPlayers; }
        void SetMinPlayersPerTeam(uint32 MinPlayers) { m_MinPlayersPerTeam = MinPlayers; }

        void AddToBGFreeSlotQueue();                        //this queue will be useful when more battlegrounds instances will be available
        void RemoveFromBGFreeSlotQueue();                   //this method could delete whole BG instance, if another free is available

        void DecreaseInvitedCount(uint32 team)      { (team == ALLIANCE) ? --m_InvitedAlliance : --m_InvitedHorde; }
        void IncreaseInvitedCount(uint32 team)      { (team == ALLIANCE) ? ++m_InvitedAlliance : ++m_InvitedHorde; }

        void SetRandom(bool isRandom) { m_IsRandom = isRandom; }
        uint32 GetInvitedCount(uint32 team) const   { return (team == ALLIANCE) ? m_InvitedAlliance : m_InvitedHorde; }
        bool HasFreeSlots() const;
        uint32 GetFreeSlotsForTeam(uint32 Team) const;

        bool isArena() const { return m_IsArena; }
        bool isBattleground() const { return !m_IsArena; }
        bool IsRBG() const { return m_IsRBG; }
        bool isRated() const { return m_IsRated; }
        bool IsTournament() const { return m_IsTournament; }

        BattlegroundPlayerMap const& GetPlayers() const { return m_Players; }
        uint32 GetPlayersSize() const { return m_Players.size(); }
        uint32 GetPlayerScoresSize() const { return PlayerScores.size(); }
        uint32 GetReviveQueueSize() const { return m_ReviveQueue.size(); }

        void AddPlayerToResurrectQueue(ObjectGuid npc_guid, ObjectGuid player_guid);
        void RemovePlayerFromResurrectQueue(ObjectGuid player_guid);
        
        void RelocateDeadPlayers(ObjectGuid guideGuid);

        void StartBattleground();

        GameObject* GetBGObject(uint32 type);
        Creature* GetBGCreature(uint32 type);

        void SetMapId(uint32 MapID) { m_MapId = MapID; }
        uint32 GetMapId() const { return m_MapId; }

        // Map pointers
        void SetBgMap(BattlegroundMap* map) { m_Map = map; }
        BattlegroundMap* GetBgMap() const { if(m_Map) return m_Map; else return nullptr; }
        BattlegroundMap* FindBgMap() const { return m_Map; }

        void SetTeamStartPosition(TeamId teamId, Position const& pos);
        Position const* GetTeamStartPosition(TeamId teamId) const;

        // Packet Transfer
        // method that should fill worldpacket with actual world states (not yet implemented for all battlegrounds!)
        virtual void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& /*packet*/) { }
        void SendPacketToTeam(uint32 TeamID, WorldPacket const* packet, Player* sender = nullptr, bool self = true);
        void SendPacketToAll(WorldPacket const* packet);
        void YellToAll(Creature* creature, const char* text, uint32 language);

        template<class Do>
        void BroadcastWorker(Do& _do);

        void PlaySoundToTeam(uint32 SoundID, uint32 TeamID);
        void PlaySoundToAll(uint32 SoundID, ObjectGuid sourceGuid = ObjectGuid::Empty);
        void CastSpellOnTeam(uint32 SpellID, uint32 TeamID);
        void RemoveAuraOnTeam(uint32 SpellID, uint32 TeamID);
        void RewardHonorToTeam(uint32 Honor, uint32 TeamID);
        void RewardReputationToTeam(uint32 factionIDAlliance, uint32 factionIDHorde, uint32 reputation, uint32 teamID);
        void UpdateWorldState(uint32 Field, uint32 Value);
        void UpdateWorldState(WorldStates variableID, uint32 value);
        void UpdateWorldStateForPlayer(WorldStates variableID, uint32 value, Player* Source);
        virtual void EndBattleground(uint32 winner);
        void PlayerReward(Player* player, bool isWinner, uint8 type, bool firstWeekly, bool firstDaily);

        void SendBattleGroundPoints(bool isHorde, int32 teamScores, bool broadcast = true, Player* player = nullptr);

        void BlockMovement(Player* player);

        void SendWarningToAll(int32 entry, ...);
        void SendMessageToAll(int32 entry, ChatMsg type, Player const* source = nullptr);
        void PSendMessageToAll(int32 entry, ChatMsg type, Player const* source, ...);

        // specialized version with 2 string id args
        void SendMessage2ToAll(int32 entry, ChatMsg type, Player const* source, int32 strId1 = 0, int32 strId2 = 0);
        void SendBroadcastTextToAll(int32 broadcastTextID, ChatMsg type, Unit const* unit = nullptr);

        // Raid Group
        Group* GetBgRaid(uint32 TeamID) const { return TeamID == ALLIANCE ? m_BgRaids[TEAM_ALLIANCE] : m_BgRaids[TEAM_HORDE]; }
        void SetBgRaid(uint32 TeamID, Group* bg_raid);

        void BuildPvPLogDataPacket(WorldPackets::Battleground::PVPLogData& pvpLogData);
        virtual bool UpdatePlayerScore(Player* player, uint32 type, uint32 value, bool doAddHonor = true);

        static TeamId GetTeamIndexByTeamId(uint32 team) { return team == ALLIANCE ? TEAM_ALLIANCE : TEAM_HORDE; }
        static Team GetTeamByTeamId(uint32 teamID) { return teamID == TEAM_ALLIANCE ? ALLIANCE : HORDE; }

        uint32 GetPlayersCountByTeam(uint32 Team) const { return m_PlayersCount[GetTeamIndexByTeamId(Team)]; }
        uint32 GetAlivePlayersCountByTeam(uint32 Team) const;   // used in arenas to correctly handle death in spirit of redemption / last stand etc. (killer = killed) cases
        void UpdatePlayersCountByTeam(uint32 Team, bool remove)
        {
            if (remove)
                --m_PlayersCount[GetTeamIndexByTeamId(Team)];
            else
                ++m_PlayersCount[GetTeamIndexByTeamId(Team)];
        }

        virtual void CheckWinConditions() { }

        // used for rated arena battles
        void SetGroupForTeam(uint32 Team, uint32 GroupId) { m_GroupIds[GetTeamIndexByTeamId(Team)] = GroupId; }
        uint32 GetGroupIdForTeam(uint32 Team) const             { return m_GroupIds[GetTeamIndexByTeamId(Team)]; }
        uint32 GetGroupIdByIndex(uint32 index) const { return m_GroupIds[index]; }
        void SetMatchmakerRating(uint32 Team, uint32 MMR){ m_ArenaTeamMMR[GetTeamIndexByTeamId(Team)] = MMR; }
        uint32 GetMatchmakerRating(uint32 Team) const          { return m_ArenaTeamMMR[GetTeamIndexByTeamId(Team)]; }
        uint32 GetMatchmakerRatingByIndex(uint32 index) const  { return m_ArenaTeamMMR[index]; }
        void BattlegroundTimedWin(uint32 type = 1);

        uint32 GetSameTeamId() { return m_sameBgTeamId; }
        void SetSameTeamId(uint32 team) { m_sameBgTeamId = team; }

        // Triggers handle
        // must be implemented in BG subclass
        virtual void HandleAreaTrigger(Player* /*Source*/, uint32 /*Trigger*/, bool /*entered*/);
        // must be implemented in BG subclass if need AND call base class generic code
        virtual void HandleKillPlayer(Player* player, Player* killer);
        virtual void HandleKillUnit(Creature* /*unit*/, Player* /*killer*/) { }

        // Battleground events
        virtual void UpdateCapturePoint(uint8 type, TeamId teamID, GameObject* node, Player const* player = nullptr, bool initial = false);
        virtual void EventPlayerDroppedFlag(Player* /*player*/) { }
        virtual void EventPlayerClickedOnFlag(Player* /*player*/, GameObject* /*object*/) { }
        void EventPlayerLoggedIn(Player* player);
        void EventPlayerLoggedOut(Player* player);
        virtual void EventPlayerDamagedGO(Player* /*player*/, GameObject* /*go*/, uint32 /*eventType*/) { }
        virtual void EventPlayerUsedGO(Player* /*player*/, GameObject* /*go*/){ }

        void PlayeCapturePointSound(uint8 type, TeamId teamID);

        // this function can be used by spell to interact with the BG map
        virtual void DoAction(uint32 /*action*/, ObjectGuid /*var*/) { }

        virtual void HandlePlayerResurrect(Player* /*player*/) { }

        // Death related
        virtual WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);

        virtual void AddPlayer(Player* player);                // must be implemented in BG subclass

        void AddOrSetPlayerToCorrectBgGroup(Player* player, uint32 team);

        virtual void RemovePlayerAtLeave(ObjectGuid guid, bool Transport, bool SendPacket);
                                                            // can be extended in in BG subclass

        void HandleTriggerBuff(ObjectGuid go_guid);

        void AddNameInNameList(uint32 team, const char* Name) { m_nameList[team].push_back(Name); }

        // TODO: make this protected:
        GuidVector BgObjects;
        GuidVector BgCreatures;

        void SpawnBGObject(uint32 type, uint32 respawntime);

        bool AddObject(uint32 type, uint32 entry, Position pos, Position rotation = { }, uint32 respawnTime = 0);
        bool AddObject(uint32 type, uint32 entry, float x, float y, float z, float o, float rotation0, float rotation1, float rotation2, float rotation3, uint32 respawnTime = 0);

        Creature* AddCreature(uint32 entry, uint32 type, uint32 teamval, Position pos, uint32 respawntime = 0);
        Creature* AddCreature(uint32 entry, uint32 type, uint32 teamval, float x, float y, float z, float o, uint32 respawntime = 0);

        bool DelCreature(uint32 type);
        bool DelObject(uint32 type);
        
        bool AddSpiritGuide(uint32 type, DBCPosition4D loc, TeamId team);
        bool AddSpiritGuide(uint32 type, Position pos, TeamId team);
        bool AddSpiritGuide(uint32 type, float x, float y, float z, float o, uint32 team);
        int32 GetObjectType(ObjectGuid guid);
        
        void DoorsOpen(uint32 type1, uint32 type2);
        void DoorsClose(uint32 type1, uint32 type2);
        void DoorOpen(uint32 type);
        void DoorClose(uint32 type);
        //to be removed
        const char* GetTrinityString(int32 entry);

        bool HandlePlayerUnderMap(Player* player);

        // since arenas can be AvA or Hvh, we have to get the "temporary" team of a player
        uint32 GetPlayerTeam(ObjectGuid guid) const;
        uint32 GetOtherTeam(uint32 teamId) const;
        bool IsPlayerInBattleground(ObjectGuid guid) const;

        bool ToBeDeleted() const { return m_SetDeleteThis; }
        void SetDeleteThis() { m_SetDeleteThis = true; }

        // virtual score-array - get's used in bg-subclasses
        int32 m_TeamScores[MAX_TEAMS];
        ArenaTeamScore _arenaTeamScores[MAX_TEAMS];
        uint32 m_lastFlagCaptureTeam;

        void RewardXPAtKill(Player* killer, Player* victim);

        virtual ObjectGuid GetFlagPickerGUID(int32 /*team*/ = -1) const { return ObjectGuid::Empty; }
        
        void SetRBG(bool enable) { m_IsRBG = enable; }

        void _ProcessPlayerPositionBroadcast(Milliseconds diff);
        virtual void GetPlayerPositionData(std::vector<WorldPackets::Battleground::PlayerPositions::BattlegroundPlayerPosition>* /*positions*/) const { }

        virtual Player* GetPlayer(ObjectGuid guid, bool offlineRemove, const char* context) const;
        virtual Player* GetPlayer(BattlegroundPlayerMap::iterator itr, const char* context);
        virtual Player* GetPlayer(BattlegroundPlayerMap::const_iterator itr, const char* context) const;
        virtual Player* GetPlayerForTeam(uint32 teamId, BattlegroundPlayerMap::const_iterator itr, const char* context) const;
    protected:
        // this method is called, when BG cannot spawn its own spirit guide, or something is wrong, It correctly ends Battleground
        void EndNow();
        void PlayerAddedToBGCheckIfBGIsRunning(Player* player);

        void _ProcessOfflineQueue();
        void _ProcessRessurect(uint32 diff);
        void _ProcessProgress(uint32 diff);
        void _ProcessLeave(uint32 diff);
        virtual void _ProcessJoin(uint32 diff);

        // Scorekeeping
        BattlegroundScoreMap PlayerScores;                // Player scores
        // must be implemented in BG subclass
        virtual void RemovePlayer(Player* /*player*/, ObjectGuid /*guid*/, uint32 /*team*/) { }

        // Player lists, those need to be accessible by inherited classes
        BattlegroundPlayerMap  m_Players;
        // Spirit Guide guid + Player list GUIDS
        std::map<ObjectGuid, GuidVector>  m_ReviveQueue;

        uint8 m_Events;
        // this must be filled in constructors!
        uint32 StartMessageIds[BG_STARTING_EVENT_COUNT];
        uint32 m_broadcastMessages[BG_STARTING_EVENT_COUNT];
        bool m_hasBroadcasts[BG_STARTING_EVENT_COUNT];

        bool   m_BuffChange;
        bool   m_IsRandom;
        BattlegroundTypeId m_TypeID;
        std::map<uint32, std::list<const char*>> m_nameList;
        uint8  m_JoinType;                                 // 2=2v2, 3=3v3, 5=5v5
    private:
        // Battleground
        BattlegroundTypeId m_RandomTypeID;
        uint32 m_InstanceID;                                // Battleground Instance's GUID!
        BattlegroundStatus m_Status;
        uint32 m_ClientInstanceID;                          // the instance-id which is sent to the client and without any other internal use
        Milliseconds m_StartTime;
        uint32 m_ResetStatTimer;
        Milliseconds m_EndTime;                                    // it is set to 120000 when bg is ending and it decreases itself
        Milliseconds m_CountdownTimer;
        uint32 m_LastResurrectTime;
        BattlegroundBracketId m_BracketId;
        bool   m_InBGFreeSlotQueue;                         // used to make sure that BG is only once inserted into the BattlegroundMgr.BGFreeSlotQueue[bgTypeId] deque
        bool   m_SetDeleteThis;                             // used for safe deletion of the bg after end / all players leave
        bool   m_IsArena;
        uint8  m_Winner;                                    // 0=alliance, 1=horde, 2=none
        Milliseconds m_StartDelayTime;
        Milliseconds m_LastPlayerPositionBroadcast;
        bool m_IsRated;
        bool m_IsTournament;
        bool m_PrematureCountDown;
        uint32 m_PrematureCountDownTimer;
        char const* m_Name;
        uint64 m_QueueID;
        uint32 m_sameBgTeamId;

        /* Pre- and post-update hooks */

        /**
         * @brief Pre-update hook.
         *
         * Will be called before battleground update is started. Depending on
         * the result of this call actual update body may be skipped.
         *
         * @param diff a time difference between two worldserver update loops in
         * milliseconds.
         *
         * @return @c true if update must be performed, @c false otherwise.
         *
         * @see Update(), PostUpdateImpl().
         */
        virtual bool PreUpdateImpl(uint32 /* diff */) { return true; };

        /**
         * @brief Post-update hook.
         *
         * Will be called after battleground update has passed. May be used to
         * implement custom update effects in subclasses.
         *
         * @param diff a time difference between two worldserver update loops in
         * milliseconds.
         *
         * @see Update(), PreUpdateImpl().
         */
        virtual void PostUpdateImpl(uint32 /* diff */) { };
        virtual void PostUpdateImpl(Milliseconds /*diff*/) { };

        // Player lists
        GuidVector m_ResurrectQueue;               // Player GUID
        GuidDeque m_OfflineQueue;                  // Player GUID

        // Invited counters are useful for player invitation to BG - do not allow, if BG is started to one faction to have 2 more players than another faction
        // Invited counters will be changed only when removing already invited player from queue, removing player from battleground and inviting player to BG
        // Invited players counters
        uint32 m_InvitedAlliance;
        uint32 m_InvitedHorde;

        // Raid Group
        Group* m_BgRaids[MAX_TEAMS];                   // 0 - alliance, 1 - horde

        // Players count by team
        uint32 m_PlayersCount[MAX_TEAMS];

        // Arena team ids by team
        uint32 m_GroupIds[MAX_TEAMS];

        uint32 m_ArenaTeamMMR[MAX_TEAMS];

        // Limits
        uint32 m_LevelMin;
        uint32 m_LevelMax;
        uint32 m_MaxPlayersPerTeam;
        uint32 m_MaxPlayers;
        uint32 m_MinPlayersPerTeam;
        uint32 m_MinPlayers;

        // Start location
        uint32 m_MapId;
        BattlegroundMap* m_Map;
        Position m_TeamStartPos[MAX_TEAMS];
        uint32 ScriptId;

        bool m_IsRBG;
};

#endif
