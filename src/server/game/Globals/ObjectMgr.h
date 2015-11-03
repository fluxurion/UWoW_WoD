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

#ifndef _OBJECTMGR_H
#define _OBJECTMGR_H

#include "AreaTrigger.h"
#include "Conversation.h"
#include "Log.h"
#include "Object.h"
#include "Bag.h"
#include "Creature.h"
#include "Player.h"
#include "DynamicObject.h"
#include "GameObject.h"
#include "Corpse.h"
#include "QuestDef.h"
#include "ItemTemplate.h"
#include "NPCHandler.h"
#include "DatabaseEnv.h"
#include "Mail.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "ObjectDefines.h"
#include "VehicleDefines.h"
#include <string>
#include <map>
#include <limits>
#include "ConditionMgr.h"
#include <functional>
#include "PhaseMgr.h"
#include <LockedMap.h>
#include "Packets/BattlePayPackets.h"

class Item;
class PhaseMgr;

// GCC have alternative #pragma pack(N) syntax and old gcc version not support pack(push, N), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct PageText
{
    std::string Text;
    uint16 NextPage;
};

// GCC have alternative #pragma pack() syntax and old gcc version not support pack(pop), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

// Benchmarked: Faster than std::unordered_map (insert/find)
typedef std::map<uint32, PageText> PageTextContainer;

// Benchmarked: Faster than std::map (insert/find)
typedef std::unordered_map<uint16, InstanceTemplate> InstanceTemplateContainer;

struct GameTele
{
    float  position_x;
    float  position_y;
    float  position_z;
    float  orientation;
    uint32 mapId;
    std::string name;
    std::wstring wnameLow;
};

typedef std::unordered_map<uint32, GameTele > GameTeleContainer;

enum ScriptsType
{
    SCRIPTS_FIRST = 1,

    SCRIPTS_QUEST_END = SCRIPTS_FIRST,
    SCRIPTS_QUEST_START,
    SCRIPTS_SPELL,
    SCRIPTS_GAMEOBJECT,
    SCRIPTS_EVENT,
    SCRIPTS_WAYPOINT,

    SCRIPTS_LAST
};

enum eScriptFlags
{
    // Talk Flags
    SF_TALK_USE_PLAYER          = 0x1,

    // Emote flags
    SF_EMOTE_USE_STATE          = 0x1,

    // TeleportTo flags
    SF_TELEPORT_USE_CREATURE    = 0x1,

    // KillCredit flags
    SF_KILLCREDIT_REWARD_GROUP  = 0x1,

    // RemoveAura flags
    SF_REMOVEAURA_REVERSE       = 0x1,

    // CastSpell flags
    SF_CASTSPELL_SOURCE_TO_TARGET = 0,
    SF_CASTSPELL_SOURCE_TO_SOURCE = 1,
    SF_CASTSPELL_TARGET_TO_TARGET = 2,
    SF_CASTSPELL_TARGET_TO_SOURCE = 3,
    SF_CASTSPELL_SEARCH_CREATURE  = 4,
    SF_CASTSPELL_TRIGGERED      = 0x1,

    // PlaySound flags
    SF_PLAYSOUND_TARGET_PLAYER  = 0x1,
    SF_PLAYSOUND_DISTANCE_SOUND = 0x2,

    // Orientation flags
    SF_ORIENTATION_FACE_TARGET  = 0x1,
};

struct ScriptInfo
{
    ScriptsType type;
    uint32 id;
    uint32 delay;
    ScriptCommands command;

    union
    {
        struct
        {
            uint32 nData[3];
            float  fData[4];
        } Raw;

        struct                      // SCRIPT_COMMAND_TALK (0)
        {
            uint32 ChatType;        // datalong
            uint32 Flags;           // datalong2
            int32  TextID;          // dataint
        } Talk;

        struct                      // SCRIPT_COMMAND_EMOTE (1)
        {
            uint32 EmoteID;         // datalong
            uint32 Flags;           // datalong2
        } Emote;

        struct                      // SCRIPT_COMMAND_FIELD_SET (2)
        {
            uint32 FieldID;         // datalong
            uint32 FieldValue;      // datalong2
        } FieldSet;

        struct                      // SCRIPT_COMMAND_MOVE_TO (3)
        {
            uint32 Unused1;         // datalong
            uint32 TravelTime;      // datalong2
            int32  Unused2;         // dataint

            float DestX;
            float DestY;
            float DestZ;
        } MoveTo;

        struct                      // SCRIPT_COMMAND_FLAG_SET (4)
                                    // SCRIPT_COMMAND_FLAG_REMOVE (5)
        {
            uint32 FieldID;         // datalong
            uint32 FieldValue;      // datalong2
        } FlagToggle;

        struct                      // SCRIPT_COMMAND_TELEPORT_TO (6)
        {
            uint32 MapID;           // datalong
            uint32 Flags;           // datalong2
            int32  Unused1;         // dataint

            float DestX;
            float DestY;
            float DestZ;
            float Orientation;
        } TeleportTo;

        struct                      // SCRIPT_COMMAND_QUEST_EXPLORED (7)
        {
            uint32 QuestID;         // datalong
            uint32 Distance;        // datalong2
        } QuestExplored;

        struct                      // SCRIPT_COMMAND_KILL_CREDIT (8)
        {
            uint32 CreatureEntry;   // datalong
            uint32 Flags;           // datalong2
        } KillCredit;

        struct                      // SCRIPT_COMMAND_RESPAWN_GAMEOBJECT (9)
        {
            uint32 GOGuid;          // datalong
            uint32 DespawnDelay;    // datalong2
        } RespawnGameobject;

        struct                      // SCRIPT_COMMAND_TEMP_SUMMON_CREATURE (10)
        {
            uint32 CreatureEntry;   // datalong
            uint32 DespawnDelay;    // datalong2
            int32  Unused1;         // dataint

            float PosX;
            float PosY;
            float PosZ;
            float Orientation;
        } TempSummonCreature;

        struct                      // SCRIPT_COMMAND_CLOSE_DOOR (12)
                                    // SCRIPT_COMMAND_OPEN_DOOR (11)
        {
            uint32 GOGuid;          // datalong
            uint32 ResetDelay;      // datalong2
        } ToggleDoor;

                                    // SCRIPT_COMMAND_ACTIVATE_OBJECT (13)

        struct                      // SCRIPT_COMMAND_REMOVE_AURA (14)
        {
            uint32 SpellID;         // datalong
            uint32 Flags;           // datalong2
        } RemoveAura;

        struct                      // SCRIPT_COMMAND_CAST_SPELL (15)
        {
            uint32 SpellID;         // datalong
            uint32 Flags;           // datalong2
            int32  CreatureEntry;   // dataint

            float SearchRadius;
        } CastSpell;

        struct                      // SCRIPT_COMMAND_PLAY_SOUND (16)
        {
            uint32 SoundID;         // datalong
            uint32 Flags;           // datalong2
        } PlaySound;

        struct                      // SCRIPT_COMMAND_CREATE_ITEM (17)
        {
            uint32 ItemEntry;       // datalong
            uint32 Amount;          // datalong2
        } CreateItem;

        struct                      // SCRIPT_COMMAND_DESPAWN_SELF (18)
        {
            uint32 DespawnDelay;    // datalong
        } DespawnSelf;

        struct                      // SCRIPT_COMMAND_LOAD_PATH (20)
        {
            uint32 PathID;          // datalong
            uint32 IsRepeatable;    // datalong2
        } LoadPath;

        struct                      // SCRIPT_COMMAND_CALLSCRIPT_TO_UNIT (21)
        {
            uint32 CreatureEntry;   // datalong
            uint32 ScriptID;        // datalong2
            uint32 ScriptType;      // dataint
        } CallScript;

        struct                      // SCRIPT_COMMAND_KILL (22)
        {
            uint32 Unused1;         // datalong
            uint32 Unused2;         // datalong2
            int32  RemoveCorpse;    // dataint
        } Kill;

        struct                      // SCRIPT_COMMAND_ORIENTATION (30)
        {
            uint32 Flags;           // datalong
            uint32 Unused1;         // datalong2
            int32  Unused2;         // dataint

            float Unused3;
            float Unused4;
            float Unused5;
            float Orientation;
        } Orientation;

        struct                      // SCRIPT_COMMAND_EQUIP (31)
        {
            uint32 EquipmentID;     // datalong
        } Equip;

        struct                      // SCRIPT_COMMAND_MODEL (32)
        {
            uint32 ModelID;         // datalong
        } Model;

                                    // SCRIPT_COMMAND_CLOSE_GOSSIP (33)

        struct                      // SCRIPT_COMMAND_PLAYMOVIE (34)
        {
            uint32 MovieID;         // datalong
        } PlayMovie;

        struct                      // SCRIPT_COMMAND_PLAYSCENE (35)
                                    // SCRIPT_COMMAND_STOPSCENE (36)
        {
            uint32 sceneID;         // datalong
        } PlayScene;
    };

    std::string GetDebugInfo() const;
};

typedef std::multimap<uint32, ScriptInfo> ScriptMap;
typedef std::map<uint32, ScriptMap > ScriptMapMap;
typedef std::multimap<uint32, uint32> SpellScriptsContainer;
typedef std::pair<SpellScriptsContainer::iterator, SpellScriptsContainer::iterator> SpellScriptsBounds;
extern ScriptMapMap sQuestEndScripts;
extern ScriptMapMap sQuestStartScripts;
extern ScriptMapMap sSpellScripts;
extern ScriptMapMap sGameObjectScripts;
extern ScriptMapMap sEventScripts;
extern ScriptMapMap sWaypointScripts;

enum VisibleDistanceType
{
    TYPE_VISIBLE_MAP  = 0,
    TYPE_VISIBLE_ZONE = 1,
    TYPE_VISIBLE_AREA = 2,
    TYPE_VISIBLE_MAX  = 3,
};

typedef std::unordered_map<uint32 /*id*/, float /*distance*/> VisibleDistanceMap;
extern VisibleDistanceMap sVisibleDistance[TYPE_VISIBLE_MAX];
float GetVisibleDistance(uint32 type, uint32 id);

std::string GetScriptsTableNameByType(ScriptsType type);
ScriptMapMap* GetScriptsMapByType(ScriptsType type);
std::string GetScriptCommandName(ScriptCommands command);

struct SpellClickInfo
{
    uint32 spellId;
    uint8 castFlags;
    SpellClickUserTypes userType;

    // helpers
    bool IsFitToRequirements(Unit const* clicker, Unit const* clickee) const;
};

typedef std::multimap<uint32, SpellClickInfo> SpellClickInfoContainer;
typedef std::pair<SpellClickInfoContainer::const_iterator, SpellClickInfoContainer::const_iterator> SpellClickInfoMapBounds;

struct AreaTriggerStruct
{
    uint32 target_mapId;
    float  target_X;
    float  target_Y;
    float  target_Z;
    float  target_Orientation;
};

struct ScenarioData
{
    uint32 ScenarioID;
    uint32 mapId;
    uint32 DifficultyID;
    uint32 WaveCurrent;
    uint32 WaveMax;
    uint32 TimerDuration;
    uint32 CriteriaProgressCount;
    uint32 BonusObjectiveDataCount;
};

typedef std::unordered_map<uint32/*ScenarioID*/, ScenarioData> ScenarioDataMap;
typedef std::unordered_map<uint32/*mapId*/, std::list<ScenarioData* > > ScenarioDataListMap;

struct ConversationCreature
{
    uint32 entry;
    uint32 id;
    uint32 creatureId;
    uint32 creatureGuid;
};

struct ConversationData
{
    uint32 entry;
    uint32 id;
    uint32 textId;
    uint32 unk1;
    uint32 unk2;
    uint32 flags;
};

typedef std::unordered_map<uint32/*entry*/, std::vector<ConversationData> > ConversationDataMap;
typedef std::unordered_map<uint32/*entry*/, std::vector<ConversationCreature> > ConversationCreatureMap;

typedef std::set<ObjectGuid::LowType> CellGuidSet;
typedef std::map<ObjectGuid/*player guid*/, uint32/*instance*/> CellCorpseSet;
struct CellObjectGuids
{
    CellGuidSet creatures;
    CellGuidSet gameobjects;
    CellCorpseSet corpses;
};
typedef std::unordered_map<uint32/*cell_id*/, CellObjectGuids> CellObjectGuidsMap;
typedef std::unordered_map<uint32/*(mapid, spawnMode) pair*/, CellObjectGuidsMap> MapObjectGuids;

// Trinity string ranges
#define MIN_TRINITY_STRING_ID           1                    // 'trinity_string'
#define MAX_TRINITY_STRING_ID           2000000000
#define MIN_DB_SCRIPT_STRING_ID        MAX_TRINITY_STRING_ID // 'db_script_string'
#define MAX_DB_SCRIPT_STRING_ID        2000010000
#define MIN_CREATURE_AI_TEXT_STRING_ID (-1)                 // 'creature_ai_texts'
#define MAX_CREATURE_AI_TEXT_STRING_ID (-1000000)

// Trinity Trainer Reference start range
#define TRINITY_TRAINER_START_REF      200000

struct TrinityStringLocale
{
    StringVector Content;
};

typedef std::map<ObjectGuid, ObjectGuid> LinkedRespawnContainer;
typedef std::unordered_map<ObjectGuid::LowType, CreatureData> CreatureDataContainer;
typedef std::unordered_map<ObjectGuid::LowType, GameObjectData> GameObjectDataContainer;
typedef std::map<TempSummonGroupKey, std::vector<TempSummonData> > TempSummonDataContainer;

struct PersonalLootData
{
    uint32 entry;
    uint8 type;
    uint8 chance;
    uint32 lootspellId;
    uint32 bonusspellId;
    uint32 cooldownid;
    uint8 cooldowntype;
    uint8 respawn;
};

enum PersonalRespawnType
{
    TYPE_NORESPAWN      = 0,
    TYPE_RESPAWN        = 1,
    TYPE_NODESPAWN      = 2
};

typedef std::unordered_map<uint32, CreatureLocale> CreatureLocaleContainer;
typedef std::unordered_map<uint32, GameObjectLocale> GameObjectLocaleContainer;
typedef std::unordered_map<uint32, ItemLocale> ItemLocaleContainer;
typedef std::unordered_map<uint32, QuestTemplateLocale> QuestTemplateLocaleContainer;
typedef std::unordered_map<uint32, QuestObjectivesLocale> QuestObjectivesLocaleContainer;
typedef std::unordered_map<uint32, NpcTextLocale> NpcTextLocaleContainer;
typedef std::unordered_map<uint32, PageTextLocale> PageTextLocaleContainer;
typedef std::unordered_map<int32, TrinityStringLocale> TrinityStringLocaleContainer;
typedef std::unordered_map<uint32, GossipMenuItemsLocale> GossipMenuItemsLocaleContainer;
typedef std::unordered_map<uint32, PointOfInterestLocale> PointOfInterestLocaleContainer;
typedef std::unordered_map<uint32, PersonalLootData> PersonalLootContainer;

typedef std::multiset<uint32> QuestObject;
typedef std::map<uint32, QuestObject> QuestStarter;
typedef std::multimap<uint32, uint32> QuestRelations;
typedef std::pair<QuestRelations::const_iterator, QuestRelations::const_iterator> QuestRelationBounds;

struct PetStats
{
    float hp;
    float ap;
    int32 ap_type;
    float spd;
    int32 school_mask;
    int32 state;
    int32 energy;
    int32 energy_type;
    float armor;
    float damage;
    int32 type;
    int32 maxspdorap;
    int32 haste;
};

struct MailLevelReward
{
    MailLevelReward() : raceMask(0), mailTemplateId(0), senderEntry(0) {}
    MailLevelReward(uint32 _raceMask, uint32 _mailTemplateId, uint32 _senderEntry) : raceMask(_raceMask), mailTemplateId(_mailTemplateId), senderEntry(_senderEntry) {}

    uint32 raceMask;
    uint32 mailTemplateId;
    uint32 senderEntry;
};

typedef std::list<MailLevelReward> MailLevelRewardList;
typedef std::unordered_map<uint8, MailLevelRewardList> MailLevelRewardContainer;

// We assume the rate is in general the same for all three types below, but chose to keep three for scalability and customization
struct RepRewardRate
{
    float quest_rate;                                       // We allow rate = 0.0 in database. For this case, it means that
    float creature_rate;                                    // no reputation are given at all for this faction/rate type.
    float spell_rate;
};

struct ReputationOnKillEntry
{
    uint32 RepFaction1;
    uint32 RepFaction2;
    uint32 ReputationMaxCap1;
    int32 RepValue1;
    uint32 ReputationMaxCap2;
    int32 RepValue2;
    bool IsTeamAward1;
    bool IsTeamAward2;
    bool TeamDependent;
};

struct RepSpilloverTemplate
{
    uint32 faction[MAX_SPILLOVER_FACTIONS];
    float faction_rate[MAX_SPILLOVER_FACTIONS];
    uint32 faction_rank[MAX_SPILLOVER_FACTIONS];
};

struct PointOfInterest
{
    uint32 entry;
    float x;
    float y;
    uint32 icon;
    uint32 flags;
    uint32 data;
    std::string icon_name;
};

struct GossipMenuItems
{
    uint32          MenuId;
    uint32          OptionIndex;
    uint8           OptionIcon;
    std::string     OptionText;
    uint32          OptionType;
    uint32          OptionNpcflag;
    uint32          ActionMenuId;
    uint32          ActionPoiId;
    bool            BoxCoded;
    uint32          BoxMoney;
    std::string     BoxText;
    ConditionList   Conditions;
};

struct GossipMenus
{
    uint32          entry;
    uint32          text_id;
    ConditionList   conditions;
};

typedef std::multimap<uint32, GossipMenus> GossipMenusContainer;
typedef std::pair<GossipMenusContainer::const_iterator, GossipMenusContainer::const_iterator> GossipMenusMapBounds;
typedef std::pair<GossipMenusContainer::iterator, GossipMenusContainer::iterator> GossipMenusMapBoundsNonConst;
typedef std::multimap<uint32, GossipMenuItems> GossipMenuItemsContainer;
typedef std::pair<GossipMenuItemsContainer::const_iterator, GossipMenuItemsContainer::const_iterator> GossipMenuItemsMapBounds;
typedef std::pair<GossipMenuItemsContainer::iterator, GossipMenuItemsContainer::iterator> GossipMenuItemsMapBoundsNonConst;

struct QuestPOIPoint
{
    int32 X;
    int32 Y;

    QuestPOIPoint() : X(0), Y(0) { }
    QuestPOIPoint(int32 _X, int32 _Y) : X(_X), Y(_Y) { }
};

struct QuestPOI
{
    int32 BlobIndex;
    int32 ObjectiveIndex;
    int32 QuestObjectiveID;
    int32 QuestObjectID;
    int32 MapID;
    int32 WorldMapAreaID;
    int32 Floor;
    int32 Priority;
    int32 Flags;
    int32 WorldEffectID;
    int32 PlayerConditionID;
    int32 UnkWoD1;
    std::vector<QuestPOIPoint> points;

    QuestPOI() : BlobIndex(0), ObjectiveIndex(0), QuestObjectiveID(0), QuestObjectID(0), MapID(0), WorldMapAreaID(0), Floor(0), Priority(0), Flags(0), WorldEffectID(0), PlayerConditionID(0), UnkWoD1(0) { }
    QuestPOI(int32 _BlobIndex, int32 _ObjectiveIndex, int32 _QuestObjectiveID, int32 _QuestObjectID, int32 _MapID, int32 _WorldMapAreaID, int32 _Foor, int32 _Priority, int32 _Flags, int32 _WorldEffectID, int32 _PlayerConditionID, int32 _UnkWoD1) :
        BlobIndex(_BlobIndex), ObjectiveIndex(_ObjectiveIndex), QuestObjectiveID(_QuestObjectiveID), QuestObjectID(_QuestObjectID), MapID(_MapID), WorldMapAreaID(_WorldMapAreaID),
        Floor(_Foor), Priority(_Priority), Flags(_Flags), WorldEffectID(_WorldEffectID), PlayerConditionID(_PlayerConditionID), UnkWoD1(_UnkWoD1) { }
};

typedef std::vector<QuestPOI> QuestPOIVector;
typedef std::unordered_map<uint32, QuestPOIVector> QuestPOIContainer;

struct ScenarioPOIPoint
{
    int32 x;
    int32 y;

    ScenarioPOIPoint() : x(0), y(0) {}
    ScenarioPOIPoint(int32 _x, int32 _y) : x(_x), y(_y) {}
};

struct ScenarioPOI
{
    uint32 BlobID;
    uint32 MapID;
    uint32 WorldMapAreaID;
    uint32 Floor;
    uint32 Priority;
    uint32 Flags;
    uint32 WorldEffectID;
    uint32 PlayerConditionID;
    std::vector<ScenarioPOIPoint> points;

    ScenarioPOI() : BlobID(0), MapID(0), WorldMapAreaID(0), Floor(0), Priority(0), Flags(0), WorldEffectID(0), PlayerConditionID(0) {}
    ScenarioPOI(uint32 _BlobID, uint32 _MapID, uint32 _WorldMapAreaID, uint32 _Floor, uint32 _Priority, uint32 _Flags, uint32 _WorldEffectID, uint32 _PlayerConditionID) :
        BlobID(_BlobID), MapID(_MapID), WorldMapAreaID(_WorldMapAreaID), Floor(_Floor), Priority(_Priority), Flags(_Flags), WorldEffectID(_WorldEffectID), PlayerConditionID(_PlayerConditionID) { }
};

typedef std::vector<ScenarioPOI> ScenarioPOIVector;
typedef std::unordered_map<uint32, ScenarioPOIVector> ScenarioPOIContainer;

struct GraveYardData
{
    uint32 safeLocId;
    uint32 team;
};

typedef std::multimap<uint32, GraveYardData> GraveYardContainer;

typedef std::unordered_map<uint32, VendorItemData> CacheVendorItemContainer;
typedef std::unordered_map<uint32, TrainerSpellData> CacheTrainerSpellContainer;

typedef std::unordered_map<uint8, uint8> ExpansionRequirementContainer;
typedef std::unordered_map<uint32, std::string> RealmNameContainer;

enum SkillRangeType
{
    SKILL_RANGE_LANGUAGE,                                   // 300..300
    SKILL_RANGE_LEVEL,                                      // 1..max skill for level
    SKILL_RANGE_MONO,                                       // 1..1, grey monolite bar
    SKILL_RANGE_RANK,                                       // 1..skill for known rank
    SKILL_RANGE_NONE,                                       // 0..0 always
};

#define MAX_SKILL_STEP 16

struct SkillTiersEntry
{
    uint32      ID;                                         // 0
    uint32      Value[MAX_SKILL_STEP];                      // 1-16
};

SkillRangeType GetSkillRangeType(SkillRaceClassInfoEntry const* rcEntry);

#define MAX_PLAYER_NAME          12                         // max allowed by client name length
#define MAX_INTERNAL_PLAYER_NAME 15                         // max server internal player name length (> MAX_PLAYER_NAME for support declined names)
#define MAX_PET_NAME             12                         // max allowed by client name length
#define MAX_CHARTER_NAME         24                         // max allowed by client name length

bool normalizePlayerName(std::string& name);

struct LanguageDesc
{
    Language lang_id;
    uint32   spell_id;
    uint32   skill_id;
};


#define LANGUAGE_DESC_COUNT 24
extern LanguageDesc lang_description[LANGUAGE_DESC_COUNT];
LanguageDesc const* GetLanguageDescByID(uint32 lang);

struct DungeonEncounter
{
    DungeonEncounter(DungeonEncounterEntry const* _dbcEntry, EncounterCreditType _creditType, uint32 _creditEntry, uint32 _lastEncounterDungeon)
        : dbcEntry(_dbcEntry), creditType(_creditType), creditEntry(_creditEntry), lastEncounterDungeon(_lastEncounterDungeon) { }

    DungeonEncounterEntry const* dbcEntry;
    EncounterCreditType creditType;
    uint32 creditEntry;
    uint32 lastEncounterDungeon;
};

typedef std::list<DungeonEncounter const*> DungeonEncounterList;
typedef std::unordered_map<uint32, DungeonEncounterList> DungeonEncounterContainer;

struct HotfixInfo
{
    uint32 Type;
    uint32 Timestamp;
    uint32 Entry;
};

typedef std::map<uint32, bool> UpdateSkipData;

typedef std::map<uint32, AreaTriggerInfo > AreaTriggerInfoMap;

class PlayerDumpReader;

struct ItemSpecStats
{
    uint32 ItemType;
    uint32 ItemSpecStatTypes[MAX_ITEM_PROTO_STATS];
    uint32 ItemSpecStatCount;
    int32 ItemSpecPrimaryStat;

    ItemSpecStats(ItemEntry const* item, ItemSparseEntry const* sparse) : ItemType(0), ItemSpecStatCount(0), ItemSpecPrimaryStat(-1)
    {
        memset(ItemSpecStatTypes, -1, sizeof(ItemSpecStatTypes));
        ItemSpecPrimaryStat = GetPrimaryStat(sparse);

        if (item->Class == ITEM_CLASS_WEAPON)
        {
            ItemType = 5;
            switch (item->SubClass)
            {
                case ITEM_SUBCLASS_WEAPON_AXE:
                    AddStat(ITEM_SPEC_STAT_ONE_HANDED_AXE);
                    break;
                case ITEM_SUBCLASS_WEAPON_AXE2:
                    AddStat(ITEM_SPEC_STAT_TWO_HANDED_AXE);
                    break;
                case ITEM_SUBCLASS_WEAPON_BOW:
                    AddStat(ITEM_SPEC_STAT_BOW);
                    break;
                case ITEM_SUBCLASS_WEAPON_GUN:
                    AddStat(ITEM_SPEC_STAT_GUN);
                    break;
                case ITEM_SUBCLASS_WEAPON_MACE:
                    AddStat(ITEM_SPEC_STAT_ONE_HANDED_MACE);
                    break;
                case ITEM_SUBCLASS_WEAPON_MACE2:
                    AddStat(ITEM_SPEC_STAT_TWO_HANDED_MACE);
                    break;
                case ITEM_SUBCLASS_WEAPON_POLEARM:
                    AddStat(ITEM_SPEC_STAT_POLEARM);
                    break;
                case ITEM_SUBCLASS_WEAPON_SWORD:
                    AddStat(ITEM_SPEC_STAT_ONE_HANDED_SWORD);
                    break;
                case ITEM_SUBCLASS_WEAPON_SWORD2:
                    AddStat(ITEM_SPEC_STAT_TWO_HANDED_SWORD);
                    break;
                case ITEM_SUBCLASS_WEAPON_STAFF:
                    AddStat(ITEM_SPEC_STAT_STAFF);
                    break;
                case ITEM_SUBCLASS_WEAPON_FIST_WEAPON:
                    AddStat(ITEM_SPEC_STAT_FIST_WEAPON);
                    break;
                case ITEM_SUBCLASS_WEAPON_DAGGER:
                    AddStat(ITEM_SPEC_STAT_DAGGER);
                    break;
                case ITEM_SUBCLASS_WEAPON_THROWN:
                    AddStat(ITEM_SPEC_STAT_THROWN);
                    break;
                case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                    AddStat(ITEM_SPEC_STAT_CROSSBOW);
                    break;
                case ITEM_SUBCLASS_WEAPON_WAND:
                    AddStat(ITEM_SPEC_STAT_WAND);
                    break;
                default:
                    break;
            }
        }
        else if (item->Class == ITEM_CLASS_ARMOR && item->SubClass > 5 && item->SubClass <= 11)
        {
            switch (item->SubClass)
            {
                case ITEM_SUBCLASS_ARMOR_CLOTH:
                    if (sparse->InventoryType != INVTYPE_CLOAK)
                    {
                        ItemType = 1;
                        break;
                    }

                    ItemType = 0;
                    AddStat(ITEM_SPEC_STAT_CLOAK);
                    break;
                case ITEM_SUBCLASS_ARMOR_LEATHER:
                    ItemType = 2;
                    break;
                case ITEM_SUBCLASS_ARMOR_MAIL:
                    ItemType = 3;
                    break;
                case ITEM_SUBCLASS_ARMOR_PLATE:
                    ItemType = 4;
                    break;
                default:
                    ItemType = 6;
                    if (item->SubClass == ITEM_SUBCLASS_ARMOR_SHIELD)
                        AddStat(ITEM_SPEC_STAT_SHIELD);
                    else if (item->SubClass > ITEM_SUBCLASS_ARMOR_SHIELD && item->SubClass <= ITEM_SUBCLASS_ARMOR_RELIC)
                        AddStat(ITEM_SPEC_STAT_RELIC);
                    break;
            }
        }
        else
            ItemType = 0;

        for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
            if (sparse->ItemStatType[i] != -1)
                AddModStat(sparse->ItemStatType[i]);
    }

    int32 GetPrimaryStat(ItemSparseEntry const* sparse)
    {
        for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        {
            if (sparse->ItemStatType[i] != -1)
            {
                switch (sparse->ItemStatType[i])
                {
                    case ITEM_MOD_AGILITY:
                        return ITEM_SPEC_STAT_AGILITY;
                    case ITEM_MOD_STRENGTH:
                        return ITEM_SPEC_STAT_STRENGTH;
                    case ITEM_MOD_INTELLECT:
                        return ITEM_SPEC_STAT_INTELLECT;
                }
            }
        }

        return -1;
    }

    void AddStat(ItemSpecStat statType)
    {
        if (ItemSpecStatCount >= MAX_ITEM_PROTO_STATS)
            return;

        for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
            if (ItemSpecStatTypes[i] == uint32(statType))
                return;

        ItemSpecStatTypes[ItemSpecStatCount++] = statType;
    }

    void AddModStat(int32 itemStatType)
    {
        switch (itemStatType)
        {
            /*case ITEM_MOD_AGILITY:
                AddStat(ITEM_SPEC_STAT_AGILITY);
                break;
            case ITEM_MOD_STRENGTH:
                AddStat(ITEM_SPEC_STAT_STRENGTH);
                break;
            case ITEM_MOD_INTELLECT:
                AddStat(ITEM_SPEC_STAT_INTELLECT);
                break;*/
            case ITEM_MOD_SPIRIT:
                AddStat(ITEM_SPEC_STAT_SPIRIT);
                break;
            case ITEM_MOD_DODGE_RATING:
                AddStat(ITEM_SPEC_STAT_DODGE);
                break;
            case ITEM_MOD_PARRY_RATING:
                AddStat(ITEM_SPEC_STAT_PARRY);
                break;
            case ITEM_MOD_CRIT_MELEE_RATING:
            case ITEM_MOD_CRIT_RANGED_RATING:
            case ITEM_MOD_CRIT_SPELL_RATING:
            case ITEM_MOD_CRIT_RATING:
                AddStat(ITEM_SPEC_STAT_CRIT);
                break;
            case ITEM_MOD_HASTE_MELEE_RATING:
            case ITEM_MOD_HASTE_RANGED_RATING:
            case ITEM_MOD_HASTE_SPELL_RATING:
            case ITEM_MOD_HASTE_RATING:
                AddStat(ITEM_SPEC_STAT_HASTE);
                break;
            case ITEM_MOD_HIT_RATING:
                AddStat(ITEM_SPEC_STAT_HIT);
                break;
            case ITEM_MOD_EXTRA_ARMOR:
                AddStat(ITEM_SPEC_STAT_BONUS_ARMOR);
                break;
            case ITEM_MOD_AGI_STR_INT:
                AddStat(ITEM_SPEC_STAT_AGILITY);
                AddStat(ITEM_SPEC_STAT_STRENGTH);
                AddStat(ITEM_SPEC_STAT_INTELLECT);
                break;
            case ITEM_MOD_AGI_STR:
                AddStat(ITEM_SPEC_STAT_AGILITY);
                AddStat(ITEM_SPEC_STAT_STRENGTH);
                break;
            case ITEM_MOD_AGI_INT:
                AddStat(ITEM_SPEC_STAT_AGILITY);
                AddStat(ITEM_SPEC_STAT_INTELLECT);
                break;
            case ITEM_MOD_STR_INT:
                AddStat(ITEM_SPEC_STAT_STRENGTH);
                AddStat(ITEM_SPEC_STAT_INTELLECT);
                break;
       }
    }
};

class ObjectMgr
{
    friend class PlayerDumpReader;

    private:
        ObjectMgr();
        ~ObjectMgr();

    public:
        static ObjectMgr* instance()
        {
            static ObjectMgr instance;
            return &instance;
        }

        typedef std::unordered_map<uint32, Item*> ItemMap;

        typedef std::unordered_map<uint32, Quest*> QuestMap;

        typedef std::unordered_map<uint32, AreaTriggerStruct> AreaTriggerContainer;

        typedef std::unordered_map<uint32, uint32> AreaTriggerScriptContainer;

        typedef std::unordered_map<uint32, AccessRequirement> AccessRequirementContainer;

        typedef std::unordered_map<uint32, RepRewardRate > RepRewardRateContainer;
        typedef std::unordered_map<uint32, ReputationOnKillEntry> RepOnKillContainer;
        typedef std::unordered_map<uint32, RepSpilloverTemplate> RepSpilloverTemplateContainer;

        typedef std::unordered_map<uint32, PointOfInterest> PointOfInterestContainer;

        typedef StringVector ScriptNameContainer;

        typedef std::map<uint32, uint32> CharacterConversionMap;

        GuidSet m_dupeLogMap;

        void AddCharToDupeLog(ObjectGuid const& guid);
        bool IsPlayerInLogList(Player *player);
        void RemoveCharFromDupeList(ObjectGuid const& guid);
        void DumpDupeConstant(Player *player);

        typedef std::list<CurrencyLoot> CurrencysLoot;
        std::list<CurrencyLoot> GetCurrencyLoot(uint32 entry, uint8 type, uint8 spawnMode);

        Player* GetPlayerByLowGUID(ObjectGuid::LowType const& lowguid) const;

        GameObjectTemplate const* GetGameObjectTemplate(uint32 entry);
        GameObjectTemplateContainer const* GetGameObjectTemplates() const { return &_gameObjectTemplateStore; }
        int LoadReferenceVendor(int32 vendor, int32 item, uint8 type, std::set<uint32> *skip_vendors);

        void LoadGameObjectTemplate();
        void AddGameobjectInfo(GameObjectTemplate* goinfo);

        const std::vector<CreatureDifficultyStat>* GetDifficultyStat(uint32 entry) const;
        CreatureDifficultyStat const* GetCreatureDifficultyStat(uint32 entry, uint8 diff) const;

        CreatureTemplate const* GetCreatureTemplate(uint32 entry);
        CreatureTemplateContainer const* GetCreatureTemplates() const { return &_creatureTemplateStore; }
        CreatureModelInfo const* GetCreatureModelInfo(uint32 modelId);
        CreatureModelInfo const* GetCreatureModelRandomGender(uint32* displayID);
        static uint32 ChooseDisplayId(uint32 team, const CreatureTemplate* cinfo, const CreatureData* data = NULL);
        static void ChooseCreatureFlags(const CreatureTemplate* cinfo, uint32& npcflag, uint32& npcflag2, uint32& unit_flags, uint32& dynamicflags, const CreatureData* data = NULL);
        EquipmentInfo const* GetEquipmentInfo(uint32 entry, int8& id);
        CreatureAddon const* GetCreatureAddon(ObjectGuid::LowType const& lowguid);
        CreatureAddon const* GetCreatureTemplateAddon(uint32 entry);
        CreatureEvadeWmoData const* GetCreatureEvadeWmoData(uint32 entry);
        ItemTemplate const* GetItemTemplate(uint32 entry);
        ItemTemplateContainer const* GetItemTemplateStore() const { return &_itemTemplateStore; }

        typedef std::unordered_map<uint32, uint32> CreatureSpellBonusList;
        CreatureSpellBonusList _creatureSpellBonus;

        uint32 GetEntryByBonusSpell(uint32 spellId) const
        {
            CreatureSpellBonusList::const_iterator itr = _creatureSpellBonus.find(spellId);
            if (itr != _creatureSpellBonus.end())
                return itr->second;
            return 0;
        }

        InstanceTemplate const* GetInstanceTemplate(uint32 mapId);

        PetStats const* GetPetStats(uint32 creature_id) const;

        void GetPlayerClassLevelInfo(uint32 class_, uint8 level, uint32& baseHP, uint32& baseMana) const;

        PlayerInfo const* GetPlayerInfo(uint32 race, uint32 class_) const
        {
            if (race >= MAX_RACES)
                return nullptr;

            if (class_ >= MAX_CLASSES)
                return nullptr;

            PlayerInfo const* info = _playerInfo[race][class_];
            if (info->displayId_m == 0 || info->displayId_f == 0)
                return nullptr;

            return info;
        }

        void GetPlayerLevelInfo(uint32 race, uint32 class_, uint8 level, PlayerLevelInfo* info) const;

        /**
        * Retrieves the player name by guid.
        *
        * If the player is online, the name is retrieved immediately otherwise
        * a database query is done.
        *
        * @remark Use sWorld->GetCharacterInfo because it doesn't require a database query when player is offline
        *
        * @param guid player full guid
        * @param name returned name
        *
        * @return true if player was found, false otherwise
        */
        static ObjectGuid GetPlayerGUIDByName(std::string name);
        static bool GetPlayerNameByGUID(ObjectGuid const& guid, std::string &name);
        uint32 GetPlayerTeamByGUID(ObjectGuid const& guid) const;
        static uint32 GetPlayerAccountIdByGUID(ObjectGuid const& guid);
        static uint32 GetPlayerAccountIdByPlayerName(std::string const& name);

        uint32 GetNearestTaxiNode(float x, float y, float z, uint32 mapid, uint32 team);
        void GetTaxiPath(uint32 source, uint32 destination, uint32 &path, uint32 &cost);
        void GetTaxiPath(uint32 source, uint32 destination, std::vector<uint32>& path, uint32& cost);
        uint32 GetTaxiMountDisplayId(uint32 id, uint32 team, bool allowed_alt_team = false);

        Quest const* GetQuestTemplate(uint32 quest_id) const
        {
            QuestMap::const_iterator itr = _questTemplates.find(quest_id);
            return itr != _questTemplates.end() ? itr->second : NULL;
        }

        QuestMap const& GetQuestTemplates() const { return _questTemplates; }

        uint32 GetQuestForAreaTrigger(uint32 Trigger_ID) const
        {
            QuestAreaTriggerContainer::const_iterator itr = _questAreaTriggerStore.find(Trigger_ID);
            if (itr != _questAreaTriggerStore.end())
                return itr->second;
            return 0;
        }

        bool IsTavernAreaTrigger(uint32 Trigger_ID) const
        {
            return _tavernAreaTriggerStore.find(Trigger_ID) != _tavernAreaTriggerStore.end();
        }

        bool IsGameObjectForQuests(uint32 entry) const
        {
            return _gameObjectForQuestStore.find(entry) != _gameObjectForQuestStore.end();
        }

        NpcText const* GetNpcText(uint32 textID) const;

        WorldSafeLocsEntry const* GetDefaultGraveYard(uint32 team);
        WorldSafeLocsEntry const* GetClosestGraveYard(float x, float y, float z, uint32 MapId, uint32 team);
        bool AddGraveYardLink(uint32 id, uint32 zoneId, uint32 team, bool persist = true);
        void RemoveGraveYardLink(uint32 id, uint32 zoneId, uint32 team, bool persist = false);
        void LoadGraveyardZones();
        GraveYardData const* FindGraveYardData(uint32 id, uint32 zone);

        AreaTriggerStruct const* GetAreaTrigger(uint32 trigger) const
        {
            AreaTriggerContainer::const_iterator itr = _areaTriggerStore.find(trigger);
            if (itr != _areaTriggerStore.end())
                return &itr->second;
            return NULL;
        }

        AccessRequirement const* GetAccessRequirement(uint32 mapid, Difficulty difficulty) const
        {
            AccessRequirementContainer::const_iterator itr = _accessRequirementStore.find(MAKE_PAIR32(mapid, difficulty));
            if (itr != _accessRequirementStore.end())
                return &itr->second;
            return NULL;
        }

        AreaTriggerStruct const* GetGoBackTrigger(uint32 Map) const;
        AreaTriggerStruct const* GetMapEntranceTrigger(uint32 Map) const;

        uint32 GetAreaTriggerScriptId(uint32 trigger_id);
        SpellScriptsBounds GetSpellScriptsBounds(uint32 spell_id);

        RepRewardRate const* GetRepRewardRate(uint32 factionId) const
        {
            RepRewardRateContainer::const_iterator itr = _repRewardRateStore.find(factionId);
            if (itr != _repRewardRateStore.end())
                return &itr->second;

            return NULL;
        }

        ReputationOnKillEntry const* GetReputationOnKilEntry(uint32 id) const
        {
            RepOnKillContainer::const_iterator itr = _repOnKillStore.find(id);
            if (itr != _repOnKillStore.end())
                return &itr->second;
            return NULL;
        }

        RepSpilloverTemplate const* GetRepSpilloverTemplate(uint32 factionId) const
        {
            RepSpilloverTemplateContainer::const_iterator itr = _repSpilloverTemplateStore.find(factionId);
            if (itr != _repSpilloverTemplateStore.end())
                return &itr->second;

            return NULL;
        }

        PointOfInterest const* GetPointOfInterest(uint32 id) const
        {
            PointOfInterestContainer::const_iterator itr = _pointsOfInterestStore.find(id);
            if (itr != _pointsOfInterestStore.end())
                return &itr->second;
            return NULL;
        }

        QuestPOIVector const* GetQuestPOIVector(int32 QuestID)
        {
            QuestPOIContainer::const_iterator itr = _questPOIStore.find(QuestID);
            if (itr != _questPOIStore.end())
                return &itr->second;
            return NULL;
        }

        ScenarioPOIVector const* GetScenarioPOIVector(uint32 criteriaTreeId)
        {
            ScenarioPOIContainer::const_iterator itr = _scenarioPOIStore.find(criteriaTreeId);
            if (itr != _scenarioPOIStore.end())
                return &itr->second;
            return NULL;
        }

        VehicleAccessoryList const* GetVehicleAccessoryList(Vehicle* veh) const;

        DungeonEncounterList const* GetDungeonEncounterList(uint32 mapId, Difficulty difficulty)
        {
            std::unordered_map<uint32, DungeonEncounterList>::const_iterator itr = _dungeonEncounterStore.find(MAKE_PAIR32(mapId, difficulty));
            if (itr != _dungeonEncounterStore.end())
                return &itr->second;
            return NULL;
        }

        void LoadCurrencysLoot();
        void LoadQuests();
        void LoadQuestRelations()
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading GO Start Quest Data...");
            LoadGameobjectQuestRelations();
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading GO End Quest Data...");
            LoadGameobjectInvolvedRelations();
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Creature Start Quest Data...");
            LoadCreatureQuestRelations();
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Creature End Quest Data...");
            LoadAreaQuestRelations();
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Area Quest Data...");
            LoadCreatureInvolvedRelations();
        }
        void LoadGameobjectQuestRelations();
        void LoadGameobjectInvolvedRelations();
        void LoadCreatureQuestRelations();
        void LoadCreatureInvolvedRelations();
        void LoadAreaQuestRelations();

        QuestRelations* GetGOQuestRelationMap()
        {
            return &_goQuestRelations;
        }

        QuestRelationBounds GetGOQuestRelationBounds(uint32 go_entry)
        {
            return _goQuestRelations.equal_range(go_entry);
        }

        QuestRelationBounds GetGOQuestInvolvedRelationBounds(uint32 go_entry)
        {
            return _goQuestInvolvedRelations.equal_range(go_entry);
        }

        QuestRelations* GetCreatureQuestRelationMap()
        {
            return &_creatureQuestRelations;
        }
        QuestStarter* GetCreatureQuestStarterMap()
        {
            return &_creatureQuestStarter;
        }

        QuestStarter* GetGOQuestStarterMap()
        {
            return &_goQuestStarter;
        }

        QuestRelationBounds GetCreatureQuestRelationBounds(uint32 creature_entry)
        {
            return _creatureQuestRelations.equal_range(creature_entry);
        }

        QuestRelationBounds GetCreatureQuestInvolvedRelationBounds(uint32 creature_entry)
        {
            return _creatureQuestInvolvedRelations.equal_range(creature_entry);
        }

        QuestRelationBounds GetAreaQuestRelationBounds(uint32 area)
        {
            return _areaQuestRelations.equal_range(area);
        }

        void LoadGameObjectScripts();
        void LoadQuestEndScripts();
        void LoadQuestStartScripts();
        void LoadEventScripts();
        void LoadSpellScripts();
        void LoadWaypointScripts();

        void LoadSpellScriptNames();
        void ValidateSpellScripts();

        bool LoadTrinityStrings(char const* table, int32 min_value, int32 max_value);
        bool LoadTrinityStrings() { return LoadTrinityStrings("trinity_string", MIN_TRINITY_STRING_ID, MAX_TRINITY_STRING_ID); }
        void LoadDbScriptStrings();
        void LoadCreatureClassLevelStats();
        void LoadWorldVisibleDistance();
        void LoadCreatureLocales();
        void LoadCreatureEvaseWmoData();
        void LoadCreatureDifficultyStat();
        void LoadCreatureTemplates();
        void LoadCreatureTemplateAddons();
        void CheckCreatureTemplate(CreatureTemplate const* cInfo);
        void RestructCreatureGUID();
        void RestructGameObjectGUID();
        void LoadTempSummons();
        void LoadCreatures();
        void LoadCreatureAIInstance();
        void LoadLinkedRespawn();
        bool SetCreatureLinkedRespawn(ObjectGuid::LowType const& guid, ObjectGuid::LowType const& linkedGuid);
        void LoadCreatureAddons();
        void LoadCreatureModelInfo();
        void LoadEquipmentTemplates();
        void LoadGameObjectLocales();
        void LoadGameobjects();
        void LoadItemTemplates();
        void LoadItemTemplateAddon();
        void LoadItemScriptNames();
        void LoadItemLocales();
        void LoadQuestTemplateLocale();
        void LoadQuestObjectivesLocale();
        void LoadPageTextLocales();
        void LoadGossipMenuItemsLocales();
        void LoadPointOfInterestLocales();
        void LoadInstanceTemplate();
        void LoadInstanceEncounters();
        void LoadMailLevelRewards();
        void LoadVehicleTemplateAccessories();
        void LoadVehicleAccessories();
        void LoadPersonalLootTemplate();

        void LoadNPCText();

        void LoadAreaTriggerTeleports();
        void LoadAccessRequirements();
        void LoadQuestAreaTriggers();
        void LoadAreaTriggerScripts();
        void LoadTavernAreaTriggers();
        void LoadGameObjectForQuests();

        void LoadPageTexts();
        PageText const* GetPageText(uint32 pageEntry);

        void LoadPlayerInfo();
        void LoadPetStats();
        void LoadExplorationBaseXP();
        void LoadPetNames();
        void LoadPetNumber();
        void LoadCorpses();
        void LoadFishingBaseSkillLevel();

        void LoadSkillTiers();
        void LoadReputationRewardRate();
        void LoadReputationOnKill();
        void LoadReputationSpilloverTemplate();

        void LoadPointsOfInterest();
        void LoadQuestPOI();
        void LoadScenarioPOI();

        void LoadNPCSpellClickSpells();

        void LoadGameTele();

        void LoadGossipMenu();
        void LoadGossipMenuItems();

        void LoadVendors();
        void LoadTrainerSpell();
        void AddSpellToTrainer(uint32 entry, uint32 spell, uint32 spellCost, uint32 reqSkill, uint32 reqSkillValue, uint32 reqLevel);

        void LoadPhaseDefinitions();
        void LoadSpellPhaseInfo();

        void LoadResearchSiteToZoneData();
        void LoadDigSitePositions();

        void LoadAreaTriggerActionsAndData();
        AreaTriggerInfo const* GetAreaTriggerInfo(uint32 entry);

        void LoadScenarioData();

        void LoadConversationData();

        void LoadBannedAddons();

        // Battle Pet System
        void LoadBattlePetXPForLevel();
        void LoadBattlePetBreedsToSpecies();
        void LoadBattlePetNpcTeamMember();

        PhaseDefinitionStore const* GetPhaseDefinitionStore() { return &_PhaseDefinitionStore; }
        SpellPhaseStore const* GetSpellPhaseStore() { return &_SpellPhaseStore; }

        std::string GeneratePetName(uint32 entry);
        uint32 GetBaseXP(uint8 level);
        uint32 GetXPForLevel(uint8 level) const;

        int32 GetFishingBaseSkillLevel(uint32 entry) const
        {
            FishingBaseSkillContainer::const_iterator itr = _fishingBaseForAreaStore.find(entry);
            return itr != _fishingBaseForAreaStore.end() ? itr->second : 0;
        }

        SkillTiersEntry const* GetSkillTier(uint32 skillTierId) const
        {
            auto itr = _skillTiers.find(skillTierId);
            return itr != _skillTiers.end() ? &itr->second : nullptr;
        }

        void ReturnOrDeleteOldMails(bool serverUp);

        CreatureBaseStats const* GetCreatureBaseStats(uint8 level, uint8 unitClass);

        void SetHighestGuids();
        template<HighGuid type>
        ObjectGuidGenerator<type>* GetGenerator();
        uint32 GenerateAuctionID();
        uint64 GenerateEquipmentSetGuid();
        uint32 GenerateMailID();
        uint32 GeneratePetNumber();
        uint64 GenerateVoidStorageItemId();

        typedef std::multimap<int32, uint32> ExclusiveQuestGroups;
        ExclusiveQuestGroups mExclusiveQuestGroups;

        MailLevelReward const* GetMailLevelReward(uint32 level, uint32 raceMask)
        {
            MailLevelRewardContainer::const_iterator map_itr = _mailLevelRewardStore.find(level);
            if (map_itr == _mailLevelRewardStore.end())
                return NULL;

            for (MailLevelRewardList::const_iterator set_itr = map_itr->second.begin(); set_itr != map_itr->second.end(); ++set_itr)
                if (set_itr->raceMask & raceMask)
                    return &*set_itr;

            return NULL;
        }

        CellObjectGuids const& GetCellObjectGuids(uint16 mapid, uint8 spawnMode, uint32 cell_id)
        {
            return _mapObjectGuidsStore[MAKE_PAIR32(mapid, spawnMode)][cell_id];
        }

        std::vector<TempSummonData> const* GetSummonGroup(uint32 summonerId, SummonerType summonerType, uint8 group) const;

        CreatureData const* GetCreatureData(ObjectGuid::LowType const& guid) const
        {
            CreatureDataContainer::const_iterator itr = _creatureDataStore.find(guid);
            if (itr == _creatureDataStore.end()) return NULL;
            return &itr->second;
        }

        CreatureAIInstance const* GetCreatureAIInstaceData(uint32 entry) const
        {
            CreatureAIInstanceContainer::const_iterator itr = _creatureAIInstance.find(entry);
            if (itr == _creatureAIInstance.end()) return NULL;
            return &itr->second;
        }

        const std::vector<DoorData>* GetCreatureAIInstaceDoorData(int32 instanceId) const
        {
            CreatureAIInstanceDoorContainer::const_iterator itr = _creatureAIInstanceDoor.find(instanceId);
            return itr != _creatureAIInstanceDoor.end() ? &(itr->second) : NULL;
        }

        const std::vector<DoorGoData>* GetCreatureAIInstaceGoData(int32 entry) const
        {
            CreatureAIInstanceGoContainer::const_iterator itr = _creatureAIInstanceGo.find(entry);
            return itr != _creatureAIInstanceGo.end() ? &(itr->second) : NULL;
        }

        CreatureData& NewOrExistCreatureData(ObjectGuid::LowType const& guid) { return _creatureDataStore[guid]; }
        void DeleteCreatureData(ObjectGuid::LowType const& guid);

        PersonalLootData const* GetPersonalLootData(uint32 id, uint32 type = 0) const
        {
            PersonalLootContainer::const_iterator itr = _PersonalLootStore[type].find(id);
            if (itr == _PersonalLootStore[type].end()) return NULL;
            return &itr->second;
        }

        PersonalLootData const* GetPersonalLootDataBySpell(uint32 spellId) const
        {
            PersonalLootContainer::const_iterator itr = _PersonalLootBySpellStore.find(spellId);
            if (itr == _PersonalLootBySpellStore.end()) return NULL;
            return &itr->second;
        }

        ObjectGuid GetLinkedRespawnGuid(ObjectGuid const& guid) const
        {
            LinkedRespawnContainer::const_iterator itr = _linkedRespawnStore.find(guid);
            if (itr == _linkedRespawnStore.end()) return ObjectGuid::Empty;
            return itr->second;
        }

        CreatureLocale const* GetCreatureLocale(uint32 entry) const
        {
            CreatureLocaleContainer::const_iterator itr = _creatureLocaleStore.find(entry);
            if (itr == _creatureLocaleStore.end()) return NULL;
            return &itr->second;
        }
        GameObjectLocale const* GetGameObjectLocale(uint32 entry) const
        {
            GameObjectLocaleContainer::const_iterator itr = _gameObjectLocaleStore.find(entry);
            if (itr == _gameObjectLocaleStore.end()) return NULL;
            return &itr->second;
        }
        ItemLocale const* GetItemLocale(uint32 entry) const
        {
            ItemLocaleContainer::const_iterator itr = _itemLocaleStore.find(entry);
            if (itr == _itemLocaleStore.end()) return NULL;
            return &itr->second;
        }
        QuestTemplateLocale const* GetQuestLocale(uint32 entry) const
        {
            QuestTemplateLocaleContainer::const_iterator itr = _questTemplateLocaleStore.find(entry);
            if (itr == _questTemplateLocaleStore.end()) return NULL;
            return &itr->second;
        }
        QuestObjectivesLocale const* GetQuestObjectivesLocale(uint32 entry) const
        {
            QuestObjectivesLocaleContainer::const_iterator itr = _questObjectivesLocaleStore.find(entry);
            if (itr == _questObjectivesLocaleStore.end()) return NULL;
            return &itr->second;
        }
        NpcTextLocale const* GetNpcTextLocale(uint32 entry) const
        {
            NpcTextLocaleContainer::const_iterator itr = _npcTextLocaleStore.find(entry);
            if (itr == _npcTextLocaleStore.end()) return NULL;
            return &itr->second;
        }
        PageTextLocale const* GetPageTextLocale(uint32 entry) const
        {
            PageTextLocaleContainer::const_iterator itr = _pageTextLocaleStore.find(entry);
            if (itr == _pageTextLocaleStore.end()) return NULL;
            return &itr->second;
        }
        GossipMenuItemsLocale const* GetGossipMenuItemsLocale(uint32 entry) const
        {
            GossipMenuItemsLocaleContainer::const_iterator itr = _gossipMenuItemsLocaleStore.find(entry);
            if (itr == _gossipMenuItemsLocaleStore.end()) return NULL;
            return &itr->second;
        }
        PointOfInterestLocale const* GetPointOfInterestLocale(uint32 poi_id) const
        {
            PointOfInterestLocaleContainer::const_iterator itr = _pointOfInterestLocaleStore.find(poi_id);
            if (itr == _pointOfInterestLocaleStore.end()) return NULL;
            return &itr->second;
        }

        GameObjectData const* GetGOData(ObjectGuid::LowType const& guid) const
        {
            GameObjectDataContainer::const_iterator itr = _gameObjectDataStore.find(guid);
            if (itr == _gameObjectDataStore.end()) return NULL;
            return &itr->second;
        }
        GameObjectData& NewGOData(ObjectGuid::LowType const& guid) { return _gameObjectDataStore[guid]; }
        void DeleteGOData(ObjectGuid::LowType const& guid);

        TrinityStringLocale const* GetTrinityStringLocale(int32 entry) const
        {
            TrinityStringLocaleContainer::const_iterator itr = _trinityStringLocaleStore.find(entry);
            if (itr == _trinityStringLocaleStore.end()) return NULL;
            return &itr->second;
        }
        const char *GetTrinityString(int32 entry, LocaleConstant locale_idx) const;
        const char *GetTrinityStringForDBCLocale(int32 entry) const { return GetTrinityString(entry, DBCLocaleIndex); }
        LocaleConstant GetDBCLocaleIndex() const { return DBCLocaleIndex; }
        void SetDBCLocaleIndex(LocaleConstant locale) { DBCLocaleIndex = locale; }

        const std::vector<ConversationData>* GetConversationData(uint32 entry) const;
        const std::vector<ConversationCreature>* GetConversationCreature(uint32 entry) const;

        void AddCorpseCellData(uint32 mapid, uint32 cellid, ObjectGuid player_guid, uint32 instance);
        void DeleteCorpseCellData(uint32 mapid, uint32 cellid, ObjectGuid player_guid);

        // grid objects
        void AddCreatureToGrid(ObjectGuid::LowType const& guid, CreatureData const* data);
        void RemoveCreatureFromGrid(ObjectGuid::LowType const& guid, CreatureData const* data);
        void AddGameobjectToGrid(ObjectGuid::LowType const& guid, GameObjectData const* data);
        void RemoveGameobjectFromGrid(ObjectGuid::LowType const& guid, GameObjectData const* data);
        ObjectGuid::LowType AddGOData(uint32 entry, uint32 map, float x, float y, float z, float o, uint32 spawntimedelay = 0, float rotation0 = 0, float rotation1 = 0, float rotation2 = 0, float rotation3 = 0, uint32 aid = 0);
        ObjectGuid::LowType AddCreData(uint32 entry, uint32 team, uint32 map, float x, float y, float z, float o, uint32 spawntimedelay = 0);
        bool MoveCreData(ObjectGuid::LowType const& guid, uint32 map, Position pos);

        // reserved names
        void LoadReservedPlayersNames();
        bool IsReservedName(const std::string& name) const;

        // name with valid structure and symbols
        static ResponseCodes CheckPlayerName(const std::string& name, bool create = false);
        static PetNameInvalidReason CheckPetName(const std::string& name);
        static bool IsValidCharterName(const std::string& name);

        static bool CheckDeclinedNames(std::wstring w_ownname, DeclinedName const& names);

        GameTele const* GetGameTele(uint32 id) const
        {
            GameTeleContainer::const_iterator itr = _gameTeleStore.find(id);
            if (itr == _gameTeleStore.end()) return NULL;
            return &itr->second;
        }
        GameTele const* GetGameTele(const std::string& name) const;
        GameTeleContainer const& GetGameTeleMap() const { return _gameTeleStore; }
        bool AddGameTele(GameTele& data);
        bool DeleteGameTele(const std::string& name);

        TrainerSpellData const* GetNpcTrainerSpells(uint32 entry) const
        {
            CacheTrainerSpellContainer::const_iterator  iter = _cacheTrainerSpellStore.find(entry);
            if (iter == _cacheTrainerSpellStore.end())
                return NULL;

            return &iter->second;
        }

        VendorItemData const* GetNpcVendorItemList(uint32 entry) const
        {
            CacheVendorItemContainer::const_iterator iter = _cacheVendorItemStore.find(entry);
            if (iter == _cacheVendorItemStore.end())
                return NULL;

            return &iter->second;
        }
        void AddVendorItem(uint32 entry, uint32 item, int32 maxcount, uint32 incrtime, uint32 extendedCost, uint8 type, bool persist = true); // for event
        bool RemoveVendorItem(uint32 entry, uint32 item, uint8 type, bool persist = true); // for event
        bool IsVendorItemValid(uint32 vendor_entry, uint32 id, int32 maxcount, uint32 ptime, uint32 ExtendedCost, uint8 type, Player* player = NULL, std::set<uint32>* skip_vendors = NULL, uint32 ORnpcflag = 0) const;

        void LoadScriptNames();
        ScriptNameContainer &GetScriptNames() { return _scriptNamesStore; }
        const char * GetScriptName(uint32 id) const { return id < _scriptNamesStore.size() ? _scriptNamesStore[id].c_str() : ""; }
        uint32 GetScriptId(const char *name);

        SpellClickInfoMapBounds GetSpellClickInfoMapBounds(uint32 creature_id) const
        {
            return SpellClickInfoMapBounds(_spellClickInfoStore.lower_bound(creature_id), _spellClickInfoStore.upper_bound(creature_id));
        }

        GossipMenusMapBounds GetGossipMenusMapBounds(uint32 uiMenuId) const
        {
            return GossipMenusMapBounds(_gossipMenusStore.lower_bound(uiMenuId), _gossipMenusStore.upper_bound(uiMenuId));
        }

        GossipMenusMapBoundsNonConst GetGossipMenusMapBoundsNonConst(uint32 uiMenuId)
        {
            return GossipMenusMapBoundsNonConst(_gossipMenusStore.lower_bound(uiMenuId), _gossipMenusStore.upper_bound(uiMenuId));
        }

        GossipMenuItemsMapBounds GetGossipMenuItemsMapBounds(uint32 uiMenuId) const
        {
            return GossipMenuItemsMapBounds(_gossipMenuItemsStore.lower_bound(uiMenuId), _gossipMenuItemsStore.upper_bound(uiMenuId));
        }
        GossipMenuItemsMapBoundsNonConst GetGossipMenuItemsMapBoundsNonConst(uint32 uiMenuId)
        {
            return GossipMenuItemsMapBoundsNonConst(_gossipMenuItemsStore.lower_bound(uiMenuId), _gossipMenuItemsStore.upper_bound(uiMenuId));
        }

        // for wintergrasp only
        GraveYardContainer GraveYardStore;

        static void AddLocaleString(std::string const& value, LocaleConstant localeConstant, StringVector& data);
        static inline void GetLocaleString(StringVector const& data, LocaleConstant localeConstant, std::string& value)
        {
            if (data.size() > size_t(localeConstant) && !data[localeConstant].empty())
                value = data[localeConstant];
        }

        CharacterConversionMap FactionChange_Achievements;
        CharacterConversionMap FactionChange_Items;
        CharacterConversionMap FactionChange_Spells;
        CharacterConversionMap FactionChange_Reputation;
        CharacterConversionMap FactionChange_Titles;

        void LoadFactionChangeAchievements();
        void LoadFactionChangeItems();
        void LoadFactionChangeSpells();
        void LoadFactionChangeReputations();
        void LoadFactionChangeTitles();

        ///Temporaire pour la création des Z, a remettre en private après
        GameObjectDataContainer _gameObjectDataStore;

        //Get item count from spawnmode
        uint8 GetCountFromSpawn(uint8 spawnmode, uint32 size)
        {
            switch (spawnmode)
            {
                case DIFFICULTY_NONE:
                case DIFFICULTY_NORMAL:
                case DIFFICULTY_HEROIC:
                    return 1;
                case DIFFICULTY_10_N:
                case DIFFICULTY_10_HC:
                    return 2;
                case DIFFICULTY_25_N:
                case DIFFICULTY_25_HC:
                case DIFFICULTY_40:
                    return 4;
                case DIFFICULTY_LFR:
                case DIFFICULTY_CHALLENGE:
                    return 3;
                default:
                    break;
            }

            return 0;
        }

        // Battle Pets
        const std::vector<uint32>* GetPossibleBreedsForSpecies(uint32 speciesID) const;
        const std::vector<uint32>* GetBattlePetTeamMembers(uint32 creatureEntry) const;
        void LoadRaceAndClassExpansionRequirements();
        void LoadRealmNames();
        void LoadBattlePay();

        std::string GetRealmName(uint32 realm) const;

        ExpansionRequirementContainer const& GetRaceExpansionRequirements() const { return _raceExpansionRequirementStore; }
        uint8 GetRaceExpansionRequirement(uint8 race) const
        {
            auto itr = _raceExpansionRequirementStore.find(race);
            if (itr != _raceExpansionRequirementStore.end())
                return itr->second;
            return EXPANSION_CLASSIC;
        }

        ExpansionRequirementContainer const& GetClassExpansionRequirements() const { return _classExpansionRequirementStore; }
        uint8 GetClassExpansionRequirement(uint8 class_) const
        {
            auto itr = _classExpansionRequirementStore.find(class_);
            if (itr != _classExpansionRequirementStore.end())
                return itr->second;
            return EXPANSION_CLASSIC;
        }

        //Get count from difficulty
        static uint8 GetGuildGroupCountFromDifficulty(uint8 spawnmode)
        {
            switch (spawnmode)
            {
                case DIFFICULTY_NONE:
                case DIFFICULTY_NORMAL:
                case DIFFICULTY_HEROIC:
                case DIFFICULTY_CHALLENGE:
                case DIFFICULTY_HC_SCENARIO:
                case DIFFICULTY_N_SCENARIO:
                    return 3;
                case DIFFICULTY_10_N:
                case DIFFICULTY_10_HC:
                    return 8;
                case DIFFICULTY_25_N:
                case DIFFICULTY_25_HC:
                case FLEXIBLE_DIFFICULTY:
                case DIFFICULTY_LFR:
                    return 20;
                case DIFFICULTY_40:
                    return 32;
                default:
                    break;
            }

            return 5;
        }

        //Get boundType from difficulty
        uint8 GetboundTypeFromDifficulty(uint8 difficulty)
        {
            switch (difficulty)
            {
                case DIFFICULTY_HEROIC:
                    return 1;
                default:
                    return 0;
            }

            return 0;
        }

        bool HasScenarioInMap(uint32 mapId) const
        {
            return _scenarioDataList.find(mapId) != _scenarioDataList.end();
        }

        const ScenarioData* GetScenarioOnMap(uint32 mapId, uint32 scenarioId = 0) const;

        WorldPackets::BattlePay::ProductListResponse productList;
        std::map<uint32, WorldPackets::BattlePay::Product> BattlePayProductMap;
    private:
        // first free id for selected id type
        uint32 _auctionId;
        uint64 _equipmentSetGuid;
        uint32 _itemTextId;
        uint32 _mailId;
        uint32 _hiPetNumber;
        ObjectGuid::LowType _voidItemId;

        // first free low guid for selected guid type
        ObjectGuidGenerator<HighGuid::Player> _playerGuidGenerator;
        ObjectGuidGenerator<HighGuid::Creature> _creatureGuidGenerator;
        ObjectGuidGenerator<HighGuid::Pet> _petGuidGenerator;
        ObjectGuidGenerator<HighGuid::Vehicle> _vehicleGuidGenerator;
        ObjectGuidGenerator<HighGuid::Item> _itemGuidGenerator;
        ObjectGuidGenerator<HighGuid::GameObject> _gameObjectGuidGenerator;
        ObjectGuidGenerator<HighGuid::DynamicObject> _dynamicObjectGuidGenerator;
        ObjectGuidGenerator<HighGuid::Corpse> _corpseGuidGenerator;
        ObjectGuidGenerator<HighGuid::LootObject> _lootObjectGuidGenerator;
        ObjectGuidGenerator<HighGuid::AreaTrigger> _areaTriggerGuidGenerator;
        ObjectGuidGenerator<HighGuid::Transport> _moTransportGuidGenerator;
        ObjectGuidGenerator<HighGuid::BattlePet> _BattlePetGuidGenerator;
        ObjectGuidGenerator<HighGuid::PetBattle> _PetBattleGuidGenerator;
        ObjectGuidGenerator<HighGuid::Conversation> _conversationGuidGenerator;

        QuestMap _questTemplates;

        typedef std::unordered_map<uint32, NpcText> NpcTextContainer;
        typedef std::unordered_map<uint32, uint32> QuestAreaTriggerContainer;
        typedef std::set<uint32> TavernAreaTriggerContainer;
        typedef std::set<uint32> GameObjectForQuestContainer;

        QuestAreaTriggerContainer _questAreaTriggerStore;
        TavernAreaTriggerContainer _tavernAreaTriggerStore;
        GameObjectForQuestContainer _gameObjectForQuestStore;
        NpcTextContainer _npcTextStore;
        AreaTriggerContainer _areaTriggerStore;
        AreaTriggerScriptContainer _areaTriggerScriptStore;
        AccessRequirementContainer _accessRequirementStore;
        DungeonEncounterContainer _dungeonEncounterStore;

        RepRewardRateContainer _repRewardRateStore;
        RepOnKillContainer _repOnKillStore;
        RepSpilloverTemplateContainer _repSpilloverTemplateStore;

        GossipMenusContainer _gossipMenusStore;
        GossipMenuItemsContainer _gossipMenuItemsStore;
        PointOfInterestContainer _pointsOfInterestStore;

        QuestPOIContainer _questPOIStore;
        ScenarioPOIContainer _scenarioPOIStore;

        QuestRelations _goQuestRelations;
        QuestRelations _goQuestInvolvedRelations;
        QuestRelations _creatureQuestRelations;
        QuestRelations _creatureQuestInvolvedRelations;
        QuestRelations _areaQuestRelations;
        CurrencysLoot  _currencysLoot;

        QuestStarter _goQuestStarter;
        QuestStarter _creatureQuestStarter;
        QuestStarter _areaQuestStarter;

        //character reserved names
        typedef std::set<std::wstring> ReservedNamesContainer;
        ReservedNamesContainer _reservedNamesStore;

        GameTeleContainer _gameTeleStore;

        ScriptNameContainer _scriptNamesStore;

        SpellClickInfoContainer _spellClickInfoStore;

        SpellScriptsContainer _spellScriptsStore;

        VehicleAccessoryTemplateContainer _vehicleTemplateAccessoryStore;
        VehicleAccessoryContainer _vehicleAccessoryStore;

        LocaleConstant DBCLocaleIndex;

        PageTextContainer _pageTextStore;
        InstanceTemplateContainer _instanceTemplateStore;

        PhaseDefinitionStore _PhaseDefinitionStore;
        SpellPhaseStore _SpellPhaseStore;

        // Battle Pet System
        typedef std::vector<uint32> BattlePetXPForLevel;
        BattlePetXPForLevel _battlePetXPperLevel;

        typedef std::map<uint32, std::vector<uint32> > BattlePetPossibleBreedsToSpecies;
        BattlePetPossibleBreedsToSpecies _battlePetPossibleBreedsToSpecies;

        typedef std::map<uint32, std::vector<uint32> > BattlePetNpcTeamMembers;
        BattlePetNpcTeamMembers _battlePetNpcTeamMembers;

        uint32 _skipUpdateCount;

    private:
        void LoadScripts(ScriptsType type);
        void CheckScripts(ScriptsType type, std::set<int32>& ids);
        void LoadQuestRelationsHelper(QuestRelations& map, QuestStarter& _map, std::string const& table, bool starter, bool go);
        void PlayerCreateInfoAddItemHelper(uint32 race_, uint32 class_, uint32 itemId, int32 count);

        MailLevelRewardContainer _mailLevelRewardStore;

        CreatureBaseStatsContainer _creatureBaseStatsStore;

        typedef std::map<uint32, PetStats> PetStatsContainer;
        PetStatsContainer _petStatsStore;                            // [creature_id][level]

        void BuildPlayerLevelInfo(uint8 race, uint8 class_, uint8 level, PlayerLevelInfo* plinfo) const;

        PlayerInfo* _playerInfo[MAX_RACES][MAX_CLASSES];

        typedef std::vector<uint32> PlayerXPperLevel;       // [level]
        PlayerXPperLevel _playerXPperLevel;

        typedef std::map<uint32, uint32> BaseXPContainer;          // [area level][base xp]
        BaseXPContainer _baseXPTable;

        typedef std::map<uint32, int32> FishingBaseSkillContainer; // [areaId][base skill level]
        FishingBaseSkillContainer _fishingBaseForAreaStore;
        std::unordered_map<uint32, SkillTiersEntry> _skillTiers;

        typedef std::map<uint32, StringVector> HalfNameContainer;
        HalfNameContainer _petHalfName0;
        HalfNameContainer _petHalfName1;

        MapObjectGuids _mapObjectGuidsStore;
        CreatureDataContainer _creatureDataStore;
        CreatureTemplateContainer _creatureTemplateStore;
        CreatureDifficultyStatContainer _creatureDifficultyStatStore;
        CreatureModelContainer _creatureModelStore;
        CreatureAddonContainer _creatureAddonStore;
        CreatureTemplateAddonContainer _creatureTemplateAddonStore;
        CreatureEvadeWmoDataContainer _creatureEvadeWmoDataStore;
        CreatureAIInstanceContainer _creatureAIInstance;
        CreatureAIInstanceDoorContainer _creatureAIInstanceDoor;
        CreatureAIInstanceGoContainer _creatureAIInstanceGo;
        EquipmentInfoContainer _equipmentInfoStore;
        LinkedRespawnContainer _linkedRespawnStore;
        CreatureLocaleContainer _creatureLocaleStore;
        //GameObjectDataContainer _gameObjectDataStore;
        GameObjectLocaleContainer _gameObjectLocaleStore;
        GameObjectTemplateContainer _gameObjectTemplateStore;
        PersonalLootContainer _PersonalLootStore[MAX_LOOT_COOLDOWN_TYPE];
        PersonalLootContainer _PersonalLootBySpellStore;
        /// Stores temp summon data grouped by summoner's entry, summoner's type and group id
        TempSummonDataContainer _tempSummonDataStore;

        ItemTemplateContainer _itemTemplateStore;
        ItemLocaleContainer _itemLocaleStore;
        QuestTemplateLocaleContainer _questTemplateLocaleStore;
        QuestObjectivesLocaleContainer _questObjectivesLocaleStore;
        NpcTextLocaleContainer _npcTextLocaleStore;
        PageTextLocaleContainer _pageTextLocaleStore;
        TrinityStringLocaleContainer _trinityStringLocaleStore;
        GossipMenuItemsLocaleContainer _gossipMenuItemsLocaleStore;
        PointOfInterestLocaleContainer _pointOfInterestLocaleStore;

        CacheVendorItemContainer _cacheVendorItemStore;
        CacheTrainerSpellContainer _cacheTrainerSpellStore;

        ExpansionRequirementContainer _raceExpansionRequirementStore;
        ExpansionRequirementContainer _classExpansionRequirementStore;
        RealmNameContainer _realmNameStore;

        enum CreatureLinkedRespawnType
        {
            CREATURE_TO_CREATURE,
            CREATURE_TO_GO,         // Creature is dependant on GO
            GO_TO_GO,
            GO_TO_CREATURE,         // GO is dependant on creature
        };

        AreaTriggerInfoMap _areaTriggerData;
        ScenarioDataMap _scenarioData;
        ScenarioDataListMap _scenarioDataList;

        ConversationDataMap _conversationDataList;
        ConversationCreatureMap _conversationCreatureList;
};

#define sObjectMgr ObjectMgr::instance()

// scripting access functions
bool LoadTrinityStrings(char const* table, int32 start_value = MAX_CREATURE_AI_TEXT_STRING_ID, int32 end_value = std::numeric_limits<int32>::min());

#endif
