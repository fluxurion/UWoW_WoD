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

#ifndef TRINITY_DBCSTRUCTURE_H
#define TRINITY_DBCSTRUCTURE_H

#include "Common.h"
#include "DBCEnums.h"
#include "Define.h"
#include "Path.h"
#include "Util.h"

#include <map>
#include <set>
#include <vector>

// Structures using to access raw DBC data and required packing to portability

// GCC have alternative #pragma pack(N) syntax and old gcc version not support pack(push, N), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

#define MAX_MAP_DEPTH -5000 // Temporary define until max depth is found somewhere (adt?)
#define MAX_OUTFIT_ITEMS 24
#define MAX_MASTERY_SPELLS 2
#define MAX_PERKS_COUNT 4
#define MAX_CREATURE_SPELL_DATA_SLOT 4
#define MAX_FACTION_RELATIONS 4
#define MAX_ITEM_ENCHANTMENT_EFFECTS 3
#define MAX_ITEM_SET_ITEMS 17
#define MAX_ITEM_SET_SPELLS 8
#define MAX_LOCK_CASE 8
#define MAX_MOUNT_CAPABILITIES 24
#define MAX_SPELL_EFFECTS_DIFF 32
#define MAX_EFFECT_MASK 4294967295
#define MAX_POWERS_FOR_SPELL 4
#define MAX_SHAPESHIFT_SPELLS 8
#define MAX_ITEM_ENCHANTMENT_EFFECTS 3
#define MAX_VEHICLE_SEATS 8
#define MAX_WORLD_MAP_OVERLAY_AREA_IDX 4

struct AchievementEntry
{
    uint32    ID;                                           // 0
    int32    requiredFaction;                               // 1 -1=all, 0=horde, 1=alliance
    int32    mapID;                                         // 2 -1=none
    uint32   parent;                                        // 3 its Achievement parent (can`t start while parent uncomplete, use its Criteria if don`t have own, use its progress on begin)
    char* name;                                             // 4
    //char* description;                                    // 5
    uint32    categoryId;                                   // 6
    uint32    points;                                       // 7 reward points
    //uint32 OrderInCategory;                               // 8
    uint32    flags;                                        // 9
    //uint32    icon;                                       // 10 icon (from SpellIcon.dbc)
    //char* reward;                                         // 11
    uint32 count;                                           // 12 - need this count of completed criterias (own or referenced achievement criterias)
    uint32 refAchievement;                                  // 13 - referenced achievement (counting of all completed criterias)
    uint32 criteriaTree;                                    // 14
};

struct AchievementCategoryEntry
{
    uint32    ID;                                           // 0
    uint32    parentCategory;                               // 1 -1 for main category
    //char* name;                                           // 2
    //uint32    sortOrder;                                  // 3
};

struct CriteriaEntry
{
    uint32  ID;                                            // 0
    uint32  type;                                          // 1
    union
    {
        // ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE          = 0
        // TODO: also used for player deaths..
        struct
        {
            uint32  creatureID;                             // 2
            // uint32  creatureCount;                       // treeCount in CriteriaTree
        } kill_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_BG                 = 1
        struct
        {
            uint32  bgMapID;                                // 2
            // uint32  winCount;                            // treeCount in CriteriaTree
        } win_bg;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ARCHAEOLOGY_PROJECTS = 3
        struct
        {
            uint32  type;                                   // 2
            // uint32  itemCount;                           // treeCount in CriteriaTree
        } archaelogy;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL            = 125
        struct
        {
            uint32  unused;                                 // 2
            // uint32  level;                               // treeCount in CriteriaTree
        } reach_level;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL      = 7
        struct
        {
            uint32  skillID;                                // 2
            // uint32  skillLevel;                          // treeCount in CriteriaTree
        } reach_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT   = 8
        struct
        {
            uint32  linkedAchievement;                      // 2
        } complete_achievement;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT   = 9
        struct
        {
            uint32  unused;                                 // 2
            // uint32  totalQuestCount;                     // treeCount in CriteriaTree
        } complete_quest_count;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY = 10
        struct
        {
            uint32  unused;                                 // 2
            // uint32  numberOfDays;                        // treeCount in CriteriaTree
        } complete_daily_quest_daily;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11
        struct
        {
            uint32  zoneID;                                 // 2
            // uint32  questCount;                          // treeCount in CriteriaTree
        } complete_quests_in_zone;

        // ACHIEVEMENT_CRITERIA_TYPE_CURRENCY = 12
        struct
        {
            uint32 currency;
            // uint32 count;
        } currencyGain;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST   = 14
        struct
        {
            uint32  unused;                                 // 2
            // uint32  questCount;                          // treeCount in CriteriaTree
        } complete_daily_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND  = 15
        struct
        {
            uint32  mapID;                                  // 2
        } complete_battleground;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP           = 16
        struct
        {
            uint32  mapID;                                  // 2
        } death_at_map;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON       = 18
        struct
        {
            uint32  manLimit;                               // 2
        } death_in_dungeon;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID          = 19
        struct
        {
            uint32  groupSize;                              // 2 can be 5, 10 or 25
        } complete_raid;

        // ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE     = 20
        struct
        {
            uint32  creatureEntry;                          // 2
        } killed_by_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING     = 24
        struct
        {
            uint32  unused;                                 // 2
            // uint32  fallHeight;                          // treeCount in CriteriaTree
        } fall_without_dying;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM            = 26
        struct
        {
            uint32 type;                                    // 2, see enum EnviromentalDamage
        } death_from;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST         = 27
        struct
        {
            uint32  questID;                                // 2
            // uint32  questCount;                          // treeCount in CriteriaTree
        } complete_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET        = 28
        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2       = 69
        struct
        {
            uint32  spellID;                                // 2
            // uint32  spellCount;                          // treeCount in CriteriaTree
        } be_spell_target;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL             = 29
        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2            = 110
        struct
        {
            uint32  spellID;                                // 2
            // uint32  castCount;                           // treeCount in CriteriaTree
        } cast_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE
        struct
        {
            uint32 objectiveId;                             // 2
            // uint32 completeCount;                        // treeCount in CriteriaTree
        } bg_objective;

        // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31
        struct
        {
            uint32  areaID;                                 // 2 Reference to AreaTable.dbc
            // uint32  killCount;                           // treeCount in CriteriaTree
        } honorable_kill_at_area;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA              = 32
        struct
        {
            uint32  mapID;                                  // 2 Reference to Map.dbc
        } win_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA             = 33
        struct
        {
            uint32  mapID;                                  // 2 Reference to Map.dbc
        } play_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL            = 34
        struct
        {
            uint32  spellID;                                // 2 Reference to Map.dbc
        } learn_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM               = 36
        struct
        {
            uint32  itemID;                                 // 2
            // uint32  itemCount;                           // treeCount in CriteriaTree
        } own_item;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA        = 37
        struct
        {
            uint32  unused;                                 // 2
            // uint32  count;                               // treeCount in CriteriaTree
        } win_rated_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING    = 38
        struct
        {
            uint32  teamtype;                               // 2 {2, 3, 5}
        } highest_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING      = 39
        struct
        {
            uint32  teamtype;                               // 2 {2, 3, 5}
            // uint32  teamrating;                          // treeCount in CriteriaTree
        } reach_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING = 39
        struct
        {
            uint32 teamtype; // 2 {2, 3, 5}
            // uint32 PersonalRating; // 4
        } highest_personal_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL      = 40
        struct
        {
            uint32  skillID;                                // 2
            // uint32  skillLevel;                          // treeCount in CriteriaTree apprentice=1, journeyman=2, expert=3, artisan=4, master=5, grand master=6
        } learn_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM               = 41
        struct
        {
            uint32  itemID;                                 // 2
            // uint32  itemCount;                           // treeCount in CriteriaTree
        } use_item;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM              = 42
        struct
        {
            uint32  itemID;                                 // 2
            // uint32  itemCount;                           // treeCount in CriteriaTree
        } loot_item;

        // ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA           = 43
        struct
        {
            // TODO: This rank is _NOT_ the index from AreaTable.dbc
            uint32  areaReference;                          // 2
        } explore_area;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK               = 44
        struct
        {
            // TODO: This rank is _NOT_ the index from CharTitles.dbc
            uint32  rank;                                   // 2
        } own_rank;

        // ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT          = 45
        struct
        {
            uint32  unused;                                 // 2
            // uint32  numberOfSlots;                       // treeCount in CriteriaTree
        } buy_bank_slot;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION        = 46
        struct
        {
            uint32  factionID;                              // 2
            // uint32  reputationAmount;                    // treeCount in CriteriaTree Total reputation amount, so 42000 = exalted
        } gain_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION= 47
        struct
        {
            uint32  unused;                                 // 2
            // uint32  numberOfExaltedFactions;             // treeCount in CriteriaTree
        } gain_exalted_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP      = 48
        struct
        {
            uint32 unused;                                  // 2
            // uint32 numberOfVisits;                       // treeCount in CriteriaTree
        } visit_barber;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM        = 49
        // TODO: where is the required itemlevel stored?
        struct
        {
            uint32  itemSlot;                               // 2
            // uint32  count;                               // treeCount in CriteriaTree
        } equip_epic_item;

        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT      = 50
        struct
        {
            uint32  rollValue;                              // 2
            // uint32  count;                               // treeCount in CriteriaTree
        } roll_need_on_loot;
       // ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT      = 51
        struct
        {
            uint32  rollValue;                              // 2
            // uint32  count;                               // treeCount in CriteriaTree
        } roll_greed_on_loot;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS               = 52
        struct
        {
            uint32  classID;                                // 2
            // uint32  count;                               // treeCount in CriteriaTree
        } hk_class;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_RACE                = 53
        struct
        {
            uint32  raceID;                                 // 2
            // uint32  count;                               // treeCount in CriteriaTree
        } hk_race;

        // ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE               = 54
        // TODO: where is the information about the target stored?
        struct
        {
            uint32  emoteID;                                // 2 enum TextEmotes
            // uint32  count;                               // treeCount in CriteriaTree count of emotes, always required special target or requirements
        } do_emote;
        // ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE            = 13
        // ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE           = 55
        // ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS      = 56
        struct
        {
            uint32  unused;                                 // 2
            // uint32  count;                               // treeCount in CriteriaTree
        } healing_done;

        // ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS      = 56
        struct
        {
            uint32  unused;
            // uint32  killCount;
        } get_killing_blow;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM             = 57
        struct
        {
            uint32  itemID;                                 // 2
            // uint32  count;                               // treeCount in CriteriaTree
        } equip_item;

        // ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD= 62
        struct
        {
            uint32  unused;                                 // 2
            // uint32  goldInCopper;                        // treeCount in CriteriaTree
        } quest_reward_money;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY             = 67
        struct
        {
            uint32  unused;                                 // 2
            // uint32  goldInCopper;                        // treeCount in CriteriaTree
        } loot_money;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT         = 68
        struct
        {
            uint32  goEntry;                                // 2
            // uint32  useCount;                            // treeCount in CriteriaTree
        } use_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL       = 70
        // TODO: are those special criteria stored in the dbc or do we have to add another sql table?
        struct
        {
            uint32  unused;                                 // 2
            // uint32  killCount;                           // treeCount in CriteriaTree
        } special_pvp_kill;

        // ACHIEVEMENT_CRITERIA_TYPE_INSTANSE_MAP_ID        = 71
        struct
        {
            uint32  mapID;                                // 2
            // uint32  lootCount;                           // treeCount in CriteriaTree
        } finish_instance;

        // ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT     = 72
        struct
        {
            uint32  goEntry;                                // 2
            // uint32  lootCount;                           // treeCount in CriteriaTree
        } fish_in_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_SCRIPT_EVENT           = 73
        // ACHIEVEMENT_CRITERIA_TYPE_SCRIPT_EVENT_2         = 92
        struct
        {
            uint32 unkValue;
            //uint32 count;
        } script_event;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS = 75
        struct
        {
            uint32  skillLine;                              // 2
            // uint32  spellCount;                          // treeCount in CriteriaTree
        } learn_skillline_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL               = 76
        struct
        {
            uint32  unused;                                 // 2
            // uint32  duelCount;                           // treeCount in CriteriaTree
        } win_duel;

        // ACHIEVEMENT_CRITERIA_TYPE_ADD_BATTLE_PET_JOURNAL = 96
        struct
        {
            uint32  add_pet;
        } battle_pet_journal;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT           = 97
        struct
        {
            uint32  statType;                               // 2 4=spirit, 3=int, 2=stamina, 1=agi, 0=strength
        } highest_stat;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER     = 98
        struct
        {
            uint32  spellSchool;                            // 2
        } highest_spellpower;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING         = 100
        struct
        {
            uint32  ratingType;                             // 2
        } highest_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE              = 109
        struct
        {
            uint32  lootType;                               // 2 3=fishing, 2=pickpocket, 4=disentchant
            // uint32  lootTypeCount;                       // treeCount in CriteriaTree
        } loot_type;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE       = 112
        struct
        {
            uint32  skillLine;                              // 2
            // uint32  spellCount;                          // treeCount in CriteriaTree
        } learn_skill_line;

        // ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL    = 113
        struct
        {
            uint32  unused;                                 // 2
            // uint32  killCount;                           // treeCount in CriteriaTree
        } honorable_kill;
        
        // ACHIEVEMENT_CRITERIA_TYPE_BATTLEPET_LEVEL_UP     = 160
        struct
        {
            uint32  level_up;
        } battlepet_level;

        struct
        {
            uint32  unused;                                 // 2
            // uint32  dungeonsComplete;                    // treeCount in CriteriaTree
        } use_lfg;

        struct
        {
            uint32  field3;                                 //  main requirement
            // uint32  count;                               // treeCount in CriteriaTree main requirement count
        } raw;

        struct
        {
            //ACHIEVEMENT_CRITERIA_TYPE_PLACE_GARRISON_BUILDING = 167
            //ACHIEVEMENT_CRITERIA_TYPE_UPGRADE_GARRISON_BUILDING = 168
            // ACHIEVEMENT_CRITERIA_TYPE_CONSTRUCT_GARRISON_BUILDING = 169
            uint32 GarrBuildingID;
        } garBuild;

        // ACHIEVEMENT_CRITERIA_TYPE_UPGRADE_GARRISON       = 170
        uint32 GarrisonLevel;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GARRISON_MISSION = 174
        uint32 GarrMissionID;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GARRISON_SHIPMENT = 182
        uint32 CharShipmentContainerID;
    };

    uint32  timedCriteriaStartType;                        // 3 m_timer_start_event
    uint32  timedCriteriaMiscId;                           // 4 m_timer_asset_id
    uint32  timeLimit;                                     // 5 m_timer_time
    uint32  timedCriteriaFailType;                         // 6 fail_event
    uint32  timedCriteriaMiscFailId;                       // 7 fail_asset_id
    uint32  ModifyTree;                                    // 8 m_modifier_tree_id
    uint32  operatorFlags;                                 // 9 m_flags
    uint32  worldStateId;                                  // 10 m_eligibility_world_state_ID
    uint32  worldStateValue;                               // 11 m_eligibility_world_state_value
};

struct CriteriaTreeEntry
{
    uint32  ID;                                          // 0
    uint32  criteria;                                    // 1
    uint32  requirement_count;                           // 2
    //uint32  field3;                                    // 3 always 0
    uint32  flags;                                       // 4
    uint32  parent;                                      // 5 m_parent in m_id
    uint32  flags2;                                      // 6
    char*   name;                                        // 7
};

struct ModifierTreeEntry // additional Condition
{
    uint32  ID;                                            // 0 m_ID
    uint32  additionalConditionType;                       // 1 m_type
    uint32  additionalConditionValue;                      // 2 m_asset
    uint32  additionalConditionCount;                       // 3 m_secondaryAsset
    uint32  operatorFlags;                                 // 4 m_operator
    //uint32  m_amount                                     // 5 m_amount
    uint32  parent;                                        // 6 m_parent in m_id
};

struct AreaTableEntry
{
    uint32      ID;                                         // 0
    uint32      mapid;                                      // 1
    uint32      ParentAreaID;                               // 2 if 0 then it's zone, else it's zone id of this area
    uint32      AreaBit;                                    // 3, main index
    uint32      Flags[2];                                   // 4-5,
    //uint32    SoundProviderPref;                          // 6,
    //uint32    SoundProviderPrefUnderwater;                // 7,
    //uint32    AmbienceID;                                 // 8,
    //uint32    ZoneMusic;                                  // 9,
    char*       ZoneName;                                   // 10
    //uint32    IntroSound;                                 // 11
    uint32      ExplorationLevel;                           // 12
    char*       AreaName_lang;                               // 13
    uint32      FactionGroupMask;                           // 14
    uint32      LiquidTypeID[4];                            // 15-18
    //float     AmbientMultiplier;                          // 19
    uint32    MountFlags;                                   // 20
    //uint32    UWIntroMusic;                               // 21
    //uint32    UWZoneMusic;                                // 22
    //uint32    UWAmbience;                                 // 23
    //uint32    WorldPvPID;                                 // 24 World_PVP_Area.dbc
    //uint32    PvPCombastWorldStateID;                     // 25
    uint32    m_wildBattlePetLevelMin;                      // 26
    uint32    m_wildBattlePetLevelMax;                      // 27
    //uint32    WindSettingsID;                             // 28

    // helpers
    bool IsSanctuary() const
    {
        if (mapid == 609)
            return true;
        return (Flags[0] & AREA_FLAG_SANCTUARY) != 0;
    }
};

struct AreaPOIEntry
{
    uint32 id;              //0
    uint32 icon[11];        //1-11
    float x;                //12
    float y;                //13
    uint32 mapId;           //14  m_continentID
    //uint32 m_flags;       //15  m_flags
    uint32 zoneId;          //16  m_areaID
    //char* name;           //17 - name
    //char* name2;          //18 - name2
    uint32 worldState;      //19
    //uint32 m_playerConditionID;//20
    //uint32 m_worldMapLink //21
    //uint32 m_portLocID;   //22
};

struct AreaTriggerEntry
{
    uint32  id;                                             // 0        m_ID
    uint32  mapid;                                          // 1        m_ContinentID
    float   x;                                              // 2        m_x
    float   y;                                              // 3        m_y
    float   z;                                              // 4        m_z
    //uint32  m_phaseUseFlags                               // 5        m_phaseUseFlags
    //uint32  m_phaseID                                     // 6        m_phaseID
    //uint32  m_phaseGroupID                                // 7        m_phaseGroupID
    float   radius;                                         // 8        m_radius
    float   box_x;                                          // 9        m_box_length
    float   box_y;                                          // 10       m_box_width
    float   box_z;                                          // 11       m_box_heigh
    float   box_orientation;                                // 12       m_box_yaw
    //uint32 m_shapeType                                    // 13       m_shapeType
    //uint32 m_shapeID                                      // 14       m_shapeID
    //uint32 m_areaTriggerActionSetID                       // 15       m_areaTriggerActionSetID
    //uint32 m_flags                                        // 16       m_flags
    //uint32 unk                                                        on 6.0.3 19243
};

struct ArmorLocationEntry
{
  uint32    InventoryType;                                  // 0
  float     Value[5];                                       // 1-5 multiplier for armor types (cloth...plate, no armor?)
};

struct AuctionHouseEntry
{
    uint32    houseId;                                      // 0 index
    uint32    faction;                                      // 1 id of faction.dbc for player factions associated with city
    uint32    depositPercent;                               // 2 1/3 from real
    uint32    cutPercent;                                   // 3
    //char*     name;                                       // 4
};

struct BankBagSlotPricesEntry
{
    uint32  ID;
    uint32  price;
};

struct BannedAddOnsEntry
{
    uint32      ID;                                         // 0
    //uint32    NameMD5[4];                                 // 1
    //uint32    VersionMD5[4];                              // 2
    //uint32    LastModified;                               // 3
    //uint32    Flags;                                      // 4
};

struct BarberShopStyleEntry
{
    uint32  Id;                                             // 0
    uint32  type;                                           // 1 value 0 -> hair, value 2 -> facialhair
    //char*   name;                                         // 2        m_DisplayName_lang
    //char*  unk_name;                                      // 3        m_Description_lang
    //float   CostMultiplier;                               // 4        m_Cost_Modifier
    uint32  race;                                           // 5        m_race
    uint32  gender;                                         // 6        m_sex
    uint32  hair_id;                                        // 7        m_data (real ID to hair/facial hair)
};

struct BattlemasterListEntry
{
    uint32      ID;
    int32       MapID[16];
    uint32      InstanceType;
    //uint32      GroupsAllowed;
    char*       NameLang;
    uint32      MaxGroupSize;
    uint32      HolidayWorldState;
    uint32      Minlevel;
    uint32      Maxlevel;
    uint32      RatedPlayers;
    uint32      MinPlayers;
    uint32      MaxPlayers;
    uint32      Flags;
    //uint32      IconFileDataID
    //char*       GametypeLang
    //uint32      CritreriaID;
};

struct CharStartOutfitEntry
{
    //uint32    ID;                                         // 0
    uint8       RaceID;                                     // 1
    uint8       ClassID;                                    // 2
    uint8       GenderID;                                   // 3
    //uint8     OutfitID;                                   // 4
    int32       ItemID[MAX_OUTFIT_ITEMS];                   // 5-28
    uint32      PetDisplayID;                               // 29 Pet Model ID for starting pet
    uint32      PetFamilyID;                                // 30 Pet Family Entry for starting pet
};

struct CharTitlesEntry
{
    uint32      ID;                                         // 0, title ids, for example in Quest::GetRewTitle()
    //uint32    ConditionID;                                // 1
    char*       NameMale_lang;                              // 2 m_name_lang
    char*       NameFemale_lang;                            // 3 m_name1_lang
    uint32      MaskID;                                     // 4 m_mask_ID used in PLAYER_CHOSEN_TITLE and 1<<index in PLAYER__FIELD_KNOWN_TITLES
    //uint32    Flags;                                      // 5
};

struct ChatChannelsEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
    //uint32    FactionGroup                                // 2
    char*     Name_lang;                                    // 3
    //char*     Shortcut_lang;                              // 4
};

struct ChrClassesEntry
{
    uint32      ID;                                         // 0
    uint32      PowerType;                                  // 1
    //char*     PetNameToken                                // 2
    char*       Name_lang;                                  // 3
    //char*     NameFemale_lang;                            // 4
    //char*     NameMale_lang;                              // 5
    //char*     Filename;                                   // 6
    uint32      SpellClassSet;                              // 7
    //uint32    Flags;                                      // 8
    uint32      CinematicSequenceID;                        // 9
    uint32      AttackPowerPerStrength;                     // 10 Attack Power bonus per point of strength
    uint32      AttackPowerPerAgility;                      // 11 Attack Power bonus per point of agility
    uint32      RangedAttackPowerPerAgility;                // 12 Ranged Attack Power bonus per point of agility
    uint32      DefaultSpec;                                  // 13
    //uint32    CreateScreenFileDataID;                     // 14
    //uint32    SelectScreenFileDataID;                     // 15
    //uint32    LowResScreenFileDataID;                     // 16
    //uint32    IconFileDataID;                             // 17
    //uint32    Unk1;                                       // 18
};

struct ChrRacesEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
    uint32      FactionID;                                  // 2 faction template id
    //uint32    ExplorationSoundID;                         // 3
    uint32      MaleDisplayID;                              // 4
    uint32      FemaleDisplayID;                            // 5
    //char*     ClientPrefix;                               // 6
    //uint32    BaseLanguage;                               // 7
    //uint32    CreatureType;                               // 8
    //uint32    ResSicknessSpellID;                         // 9
    //uint32    SplashSoundID;                              // 10
    //char*     ClientFileString;                           // 11
    uint32      CinematicSequenceID;                        // 12
    uint32      TeamID;                                     // 13 m_alliance (0 alliance, 1 horde, 2 neutral)
    char*       Name_lang;                                  // 14
    //char*     NameFemale_lang;                            // 15
    //char*     NameMale_lang;                              // 16
    //char*     FacialHairCustomization[2];                 // 17-18
    //char*     HairCustomization;                          // 19
    //uint32    RaceRelated;                                // 20
    //uint32    UnalteredVisualRaceID;                      // 21
    //uint32    UAMaleCreatureSoundDataID;                  // 22
    //uint32    UAFemaleCreatureSoundDataID;                // 23
    //uint32    CharComponentTextureLayoutID;               // 24
    //uint32    DefaultClassID;                             // 25
    //uint32    CreateScreenFileDataID;                     // 26
    //uint32    SelectScreenFileDataID;                     // 27
    //float     MaleCustomizeOffset[3];                     // 28-30
    //float     FemaleCustomizeOffset[3];                   // 31-33
    //uint32    NeutralRaceID;                              // 34
    //uint32    LowResScreenFileDataID;                     // 35
    //uint32    HighResMaleDisplayID;                       // 36
    //uint32    HighResFemaleDisplayID;                     // 37
    //uint32    CharComponentTexLayoutHiResID;              // 38
    //uint32    Unk;                                        // 39
};

struct ChrPowerTypesEntry
{
   uint32 entry;                                               // 0
   uint32 classId;                                             // 1
   uint32 power;                                               // 2
};

struct ChrSpecializationsEntry
{
    uint32      ID;                                         // 0 Specialization ID
    //char*     BackgroundFile;                             // 1
    uint32      ClassID;                                    // 2
    uint32      MasterySpellID[MAX_MASTERY_SPELLS];         // 3
    uint32      OrderIndex;                                 // 4
    uint32      PetTalentType;                              // 5
    uint32      Role;                                       // 6 (0 - Tank, 1 - Healer, 2 - DPS)
    uint32      SpellIconID;                                // 7
    uint32      RaidBuffs;                                  // 8
    uint32      Flags;                                      // 9
    //char*     Name_lang;                                  // 10
    //char*     Name2_lang;                                 // 11 Same as name_lang?
    //char*     Description_lang;                           // 12
    uint32      PrimaryStatOrder[2];                        // 13-14
};

/* not used
struct CinematicCameraEntry
{
    uint32      id;                                         // 0 index
    char*       filename;                                   // 1
    uint32      soundid;                                    // 2 in SoundEntries.dbc or 0
    float       start_x;                                    // 3
    float       start_y;                                    // 4
    float       start_z;                                    // 5
    float       unk6;                                       // 6 speed?
};
*/

struct CinematicSequencesEntry
{
    uint32      Id;                                         // 0 index
    //uint32      unk1;                                     // 1 always 0
    //uint32      cinematicCamera;                          // 2 id in CinematicCamera.dbc
                                                            // 3-9 always 0
};

struct CreatureDisplayInfoEntry
{
    uint32      ID;                                         // 0
    uint32      ModelID;                                    // 1
    //uint32    SoundID;                                    // 2
    uint32      ExtendedDisplayInfoID;                      // 3
    float       CreatureModelScale;                         // 4
    //uint32    CreatureModelAlpha;                         // 5
    //char*     TextureVariation[3];                        // 6-8
    //char*     PortraitTextureName;                        // 9
    //uint32    PortraitCreatureDisplayInfoID;              // 10
    //uint32    SizeClass;                                  // 11
    //uint32    BloodID;                                    // 12
    //uint32    NPCSoundID;                                 // 13
    //uint32    ParticleColorID;                            // 14
    //uint32    CreatureGeosetData;                         // 15
    //uint32    ObjectEffectPackageID;                      // 16
    //uint32    AnimReplacementSetID;                       // 17
    //uint32    Flags;                                      // 18
    int32       Gender;                                     // 19
    //uint32    StateSpellVisualKitID;                      // 20
};

struct CreatureDisplayInfoExtraEntry
{
    //uint32    ID;                                         // 0
    uint32      DisplayRaceID;                              // 1
    //uint32    DisplaySexID;                               // 2
    //uint32    SkinID;                                     // 3
    //uint32    FaceID;                                     // 4
    //uint32    HairStyleID;                                // 5
    //uint32    HairColorID;                                // 6
    //uint32    FacialHairID;                               // 7
    //uint32    NPCItemDisplay[11];                         // 8-18
    //uint32    Flags;                                      // 19
    //uint32    FileDataID;                                 // 20
    //uint32    Unk;                                        // 21
};
struct CreatureFamilyEntry
{
    uint32  ID;                                             // 0        m_ID
    float   minScale;                                       // 1        m_minScale
    uint32  minScaleLevel;                                  // 2        m_minScaleLevel
    float   maxScale;                                       // 3        m_maxScale
    uint32  maxScaleLevel;                                  // 4        m_maxScaleLevel
    uint32  skillLine[2];                                   // 5-6      m_skillLine
    uint32  petFoodMask;                                    // 7        m_petFoodMask
    int32   petTalentType;                                  // 8        m_petTalentType
                                                            // 9        m_categoryEnumID
    char* Name;                                             // 10       m_name_lang
                                                            // 11       m_iconFile
};

struct CreatureModelDataEntry
{
    uint32 Id;
    //uint32 Flags;
    //char* ModelPath
    //uint32 Unk1;
    //float Scale;                                             // Used in calculation of unit collision data
    //int32 Unk2
    //int32 Unk3
    //uint32 Unk4
    //uint32 Unk5
    //float Unk6
    //uint32 Unk7
    //float Unk8
    //uint32 Unk9
    //uint32 Unk10
    //float CollisionWidth;
    float CollisionHeight;
    float MountHeight;                                       // Used in calculation of unit collision data when mounted
    //float Unks[13]
};

struct CreatureSpellDataEntry
{
    uint32    ID;                                           // 0        m_ID
    uint32    spellId[MAX_CREATURE_SPELL_DATA_SLOT];        // 1-4      m_spells[4]
    //uint32    availability[MAX_CREATURE_SPELL_DATA_SLOT]; // 4-7      m_availability[4]
};

struct CreatureTypeEntry
{
    uint32    ID;                                           // 0        m_ID
    //char*   Name;                                         // 1        m_name_lang
    //uint32    no_expirience;                              // 2        m_flags no exp? critters, non-combat pets, gas cloud.
};

/* not used
struct CurrencyCategoryEntry
{
    uint32    ID;                                           // 0
    uint32    Unk1;                                         // 1        0 for known categories and 3 for unknown one (3.0.9)
    char*   Name[16];                                       // 2-17     name
    //                                                      // 18       string flags
};
*/

struct DestructibleModelDataEntry
{
    uint32  Id;
    uint32  DamagedDisplayId;
    //uint32  DamagedUnk1;
    //uint32  DamagedUnk2;
    //uint32  DamagedUnk3;
    uint32  DestroyedDisplayId;
    //uint32  DestroyedUnk1;
    //uint32  DestroyedUnk2;
    //uint32  DestroyedUnk3;
    //uint32  DestroyedUnk4;
    uint32  RebuildingDisplayId;
    //uint32  RebuildingUnk1;
    //uint32  RebuildingUnk2;
    //uint32  RebuildingUnk3;
    //uint32  RebuildingUnk4;
    uint32  SmokeDisplayId;
    //uint32  SmokeUnk1;
    //uint32  SmokeUnk2;
    //uint32  SmokeUnk3;
    //uint32  SmokeUnk4;
    //uint32  UnkDisplayid;
    //uint32  Unk6;
    //uint32  Unk7;
    //uint32  Unk8;
};

struct DifficultyEntry
{
    uint32      m_ID;                                       // 0
    uint32      FallbackDifficultyID;                       // 1
    uint32      InstanceType;                               // 2
    uint32      m_minPlayers;                               // 3
    uint32      m_maxPlayers;                               // 4
    //int32     OldEnumValue;                               // 5
    uint32      Flags;                                      // 6
    uint32      ToggleDifficultyID;                         // 7
    //uint32    GroupSizeHealthCurveID;                     // 8
    //uint32    GroupSizeDmgCurveID;                        // 9
    //uint32    GroupSizeSpellPointsCurveID;                // 10
    //char const* NameLang;                                 // 11
    uint32    ItemBonusTreeModID;                           // 12
};

struct DungeonEncounterEntry
{
    uint32 id;                                              // 0        unique id
    uint32 mapId;                                           // 1        map id
    uint32 difficulty;                                      // 2        instance mode
    //uint32 orderIndex;                                    // 3
    uint32 encounterIndex;                                  // 4        encounter index for creating completed mask
    char* encounterName;                                    // 5        encounter name
    uint32 creatureDisplayID;                               // 6
    uint32 spellIconID;                                     // 7
    //uint32 flags;                                         // 8
};

struct DurabilityCostsEntry
{
    uint32    Itemlvl;                                      // 0
    uint32    multiplier[29];                               // 1-29
};

struct DurabilityQualityEntry
{
    uint32    Id;                                           // 0
    float     quality_mod;                                  // 1
};

struct EmotesEntry
{
    uint32  Id;                                             // 0
    //char* Name;                                           // 1, internal name
    //uint32 AnimationId;                                   // 2, ref to animationData
    uint32  Flags;                                          // 3, bitmask, may be unit_flags
    uint32  EmoteType;                                      // 4, Can be 0, 1 or 2 (determine how emote are shown)
    uint32  UnitStandState;                                 // 5, uncomfirmed, may be enum UnitStandStateType
    //uint32 SoundId;                                       // 6, ref to soundEntries
    //uint32 unk7                                           // 7
};

struct EmotesTextEntry
{
    uint32  Id;
    uint32  textid;
};

struct FactionEntry
{
    uint32      ID;                                         // 0        m_ID
    int32       reputationListID;                           // 1        m_reputationIndex
    uint32      BaseRepRaceMask[4];                         // 2-5      m_reputationRaceMask
    uint32      BaseRepClassMask[4];                        // 6-9      m_reputationClassMask
    int32       BaseRepValue[4];                            // 10-13    m_reputationBase
    uint32      ReputationFlags[4];                         // 14-17    m_reputationFlags
    uint32      team;                                       // 18       m_parentFactionID
    float       spilloverRateIn;                            // 19       Faction gains incoming rep * spilloverRateIn
    float       spilloverRateOut;                           // 20       Faction outputs rep * spilloverRateOut as spillover reputation
    uint32      spilloverMaxRankIn;                         // 21       The highest rank the faction will profit from incoming spillover
    //uint32    spilloverRank_unk;                          // 22       It does not seem to be the max standing at which a faction outputs spillover ...so no idea
    char* name;                                             // 23       m_name_lang
    //char*     description;                                // 24       m_description_lang
    //uint32    m_expansion                                 // 25
    uint32      m_flags;                                    // 26       5.4.1
    //uint32    m_friendshipRepID                           // 27       5.4.1

    // helpers
    bool CanHaveReputation() const
    {
        return reputationListID >= 0;
    }

    bool CanBeLfgBonus() const { return m_flags & 0x1; }
};

struct FactionTemplateEntry
{
    uint32      ID;                                         // 0        m_ID
    uint32      faction;                                    // 1        m_faction
    uint32      factionFlags;                               // 2        m_flags
    uint32      ourMask;                                    // 3        m_factionGroup
    uint32      friendlyMask;                               // 4        m_friendGroup
    uint32      hostileMask;                                // 5        m_enemyGroup
    uint32      enemyFaction[MAX_FACTION_RELATIONS];        // 6        m_enemies[MAX_FACTION_RELATIONS]
    uint32      friendFaction[MAX_FACTION_RELATIONS];       // 10       m_friend[MAX_FACTION_RELATIONS]
    //-------------------------------------------------------  end structure

    // helpers
    bool IsFriendlyTo(FactionTemplateEntry const& entry) const
    {
        if (ID == entry.ID)
            return true;
        if (entry.faction)
        {
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (enemyFaction[i] == entry.faction)
                    return false;
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (friendFaction[i] == entry.faction)
                    return true;
        }
        return (friendlyMask & entry.ourMask) || (ourMask & entry.friendlyMask);
    }
    bool IsHostileTo(FactionTemplateEntry const& entry) const
    {
        if (ID == entry.ID)
            return false;
        if (entry.faction)
        {
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (enemyFaction[i] == entry.faction)
                    return true;
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (friendFaction[i] == entry.faction)
                    return false;
        }
        return (hostileMask & entry.ourMask) != 0;
    }
    bool IsHostileToPlayers() const { return (hostileMask & FACTION_MASK_PLAYER) !=0; }
    bool IsNeutralToAll() const
    {
        for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
            if (enemyFaction[i] != 0)
                return false;
        return hostileMask == 0 && friendlyMask == 0;
    }
    bool IsContestedGuardFaction() const { return (factionFlags & FACTION_TEMPLATE_FLAG_CONTESTED_GUARD) != 0; }
};

struct GameObjectDisplayInfoEntry
{
    uint32          ID;                                     // 0
    uint32          FileDataID;                             // 1
    //uint32        Sound[10];                              // 2-11
    float   minX;
    float   minY;
    float   minZ;
    float   maxX;
    float   maxY;
    float   maxZ;
    //uint32        ObjectEffectPackageID;                  // 18
    //float         OverrideLootEffectScale;                // 19
    //float         OverrideNameScale;                      // 20
};

struct GameTablesEntry
{
    //uint32 Index;                                         // 0 - not a real field, not counted for columns
    char const* Name;                                       // 1
    uint32 NumRows;                                         // 2
    uint32 NumColumns;                                      // 3
};

struct GemPropertiesEntry
{
    uint32      ID;                                         // 0        m_id
    uint32      spellitemenchantement;                      // 1        m_enchant_id
    //uint32    MaxCountInv;                                // 2
    //uint32    MaxCountItem;                               // 3
    uint32      Type;                                       // 4
    uint32      MinItemLevel;                               // 5
};

struct GlyphPropertiesEntry
{
    uint32      Id;
    uint32      SpellId;
    uint32      TypeFlags;
    uint32      SpellIconID;                                // 3 GlyphIconId (SpellIcon.dbc)
    //uint32    GlyphExclusiveCategoryID;                   // 4
};

struct GlyphSlotEntry
{
    uint32  Id;
    uint32  TypeFlags;
    uint32  Order;
};

struct GtBarberShopCostBaseEntry
{
    //uint32 level;
    float   cost;
};

struct GtCombatRatingsEntry
{
    //uint32 level;
    float    ratio;
};

struct GtChanceToMeleeCritBaseEntry
{
    //uint32 level;
    float    base;
};

struct GtChanceToMeleeCritEntry
{
    //uint32 level;
    float    ratio;
};

struct GtChanceToSpellCritBaseEntry
{
    float    base;
};

struct GtChanceToSpellCritEntry
{
    float    ratio;
};

struct GtNpcTotalHpEntry
{
    float    HP;
};

struct GtArmorMitigationByLvlEntry
{
    float    Armor;
};

struct GtOCTLevelExperienceEntry
{
    float    Data;
};

struct GtOCTRegenHPEntry
{
    float    ratio;
};

struct GtOCTRegenMPEntry
{
    float    ratio;
};

struct GtOCTHpPerStaminaEntry
{
    float    ratio;
};

struct GtRegenHPPerSptEntry
{
    float    ratio;
};

struct GtRegenMPPerSptEntry
{
    float    ratio;
};

struct GtSpellScalingEntry
{
    float value;
};

struct GtOCTBaseHPByClassEntry
{
    float ratio;
};

struct GtOCTBaseMPByClassEntry
{
    float ratio;
};

struct GtBattlePetTypeDamageModEntry
{
    uint32 Id;
    float value;
};

struct GuildPerkSpellsEntry
{
    //uint32 Id;
    uint32 Level;
    uint32 SpellId;
};

// ImportPriceArmor.dbc
struct ImportPriceArmorEntry
{
    uint32 InventoryType;                                   // 1        Id/InventoryType
    float ClothFactor;                                      // 2        Price factor cloth
    float LeatherFactor;                                    // 3        Price factor leather
    float MailFactor;                                       // 4        Price factor mail
    float PlateFactor;                                      // 5        Price factor plate
};

// ImportPriceQuality.dbc
struct ImportPriceQualityEntry
{
    uint32 QualityId;                                       // 1        Quality Id (+1?)
    float Factor;                                           // 2        Price factor
};

// ImportPriceShield.dbc
struct ImportPriceShieldEntry
{
    uint32 Id;                                              // 1        Unk id (only 1 and 2)
    float Factor;                                           // 2        Price factor
};

// ImportPriceWeapon.dbc
struct ImportPriceWeaponEntry
{
    uint32 Id;                                              // 1        Unk id (mainhand - 0, offhand - 1, weapon - 2, 2hweapon - 3, ranged/rangedright/relic - 4)
    float Factor;                                           // 2        Price factor
};

// ItemPriceBase.dbc
struct ItemPriceBaseEntry
{
    uint32 ItemLevel;                                       // 2        Item level (1 - 1000)
    float ArmorFactor;                                      // 3        Price factor for armor
    float WeaponFactor;                                     // 4        Price factor for weapons
};

struct GtItemSocketCostPerLevelEntry
{
    float    ratio;
};

struct GtNPCManaCostScalerEntry
{
    float    ratio;
};

struct ItemReforgeEntry
{
    uint32 Id;
    uint32 SourceStat;
    float SourceMultiplier;
    uint32 FinalStat;
    float FinalMultiplier;
};

// common struct for:
// ItemDamageAmmo.dbc
// ItemDamageOneHand.dbc
// ItemDamageOneHandCaster.dbc
// ItemDamageRanged.dbc
// ItemDamageThrown.dbc
// ItemDamageTwoHand.dbc
// ItemDamageTwoHandCaster.dbc
// ItemDamageWand.dbc
struct ItemDamageEntry
{
    uint32    Id;                                             // 0 item level
    float     DPS[7];                                         // 1-7 multiplier for item quality
    uint32    Id2;                                            // 8 item level
};

struct ItemArmorQualityEntry
{
    uint32      ID;                                           // 0 item level
    float       QualityMod[7];                                // 1-7 multiplier for item quality
    uint32      ItemLevel;                                    // 8 item level
};

struct ItemArmorShieldEntry
{
    uint32      ID;                                           // 0 item level
    uint32      ItemLevel;                                    // 1 item level
    float       Quality[7];                                   // 2-8 quality
};

struct ItemArmorTotalEntry
{
    uint32    Id;                                             // 0 item level
    uint32    Id2;                                            // 1 item level
    float     Value[4];                                       // 2-5 multiplier for armor types (cloth...plate)
};

// ItemClass.dbc
struct ItemClassEntry
{
    uint32    Class;                                          // 1 item class id
    float     PriceFactor;                                    // 2 used to calculate certain prices
  //char*     Name;                                           // 3 class name
};

struct ItemBagFamilyEntry
{
    uint32   ID;                                            // 0
    //char*     name;                                       // 1        m_name_lang
};

struct ItemDisplayInfoEntry
{
    uint32      ID;                                         // 0        m_ID
                                                            // 1        m_modelName[2]
                                                            // 2        m_modelTexture[2]
                                                            // 3        m_inventoryIcon
                                                            // 4        m_geosetGroup[3]
                                                            // 5        m_flags
                                                            // 6        m_spellVisualID
                                                            // 7        m_groupSoundIndex
                                                            // 8        m_helmetGeosetVis[2]
                                                            // 9        m_texture[2]
                                                            // 10       m_itemVisual[8]
                                                            // 11       m_particleColorID
};

struct ItemDisenchantLootEntry
{
    uint32 Id;
    uint32 ItemClass;
    int32 ItemSubClass;
    uint32 ItemQuality;
    uint32 MinItemLevel;
    uint32 MaxItemLevel;
    uint32 RequiredDisenchantSkill;
};

struct ItemLimitCategoryEntry
{
    uint32      ID;                                         // 0 Id
    //char*     name;                                       // 1        m_name_lang
    uint32      maxCount;                                   // 2,       m_quantity max allowed equipped as item or in gem slot
    uint32      mode;                                       // 3,       m_flags 0 = have, 1 = equip (enum ItemLimitCategoryMode)
};

struct ItemRandomPropertiesEntry
{
    uint32    ID;                                           // 0        m_ID
    //char* internalName                                    // 1        m_Name
    uint32    enchant_id[MAX_ITEM_ENCHANTMENT_EFFECTS];     // 2-4      m_Enchantment
                                                            // 5-6      unused
    char* nameSuffix;                                       // 7        m_name_lang
};

struct ItemRandomSuffixEntry
{
    uint32    ID;                                           // 0        m_ID
    char* nameSuffix;                                       // 1        m_name_lang
                                                            // 2        m_internalName
    uint32    enchant_id[5];                                // 3-7      m_enchantment
    uint32    prefix[5];                                    // 8-12     m_allocationPct
};

struct ItemSetEntry
{
    uint32      ID;                                         // 0
    char* name;                                             // 1        m_name_lang
    uint32    itemId[MAX_ITEM_SET_ITEMS];                   // 2-18     m_itemID
    uint32    required_skill_id;                            // 19       m_requiredSkill
    uint32    required_skill_value;                         // 20       m_requiredSkillRank
};

struct ItemSetSpellEntry
{
    uint32      ID;                                         // 0
    uint32      ItemSetID;                                  // 1
    uint32      SpellID;                                    // 2
    uint32      Threshold;                                  // 3
    uint32      ChrSpecID;                                  // 4
};

typedef std::vector<ItemSetSpellEntry const*> ItemSetSpells;
typedef std::unordered_map<uint32, ItemSetSpells> ItemSetSpellsStore;

struct ItemSpecEntry
{
    uint32      ID;                                         // 0
    uint32      MinLevel;                                   // 1
    uint32      MaxLevel;                                   // 2
    uint32      ItemType;                                   // 3
    uint32      PrimaryStat;                                // 4
    uint32      SecondaryStat;                              // 5
    uint32      SpecID;                                     // 6
};

struct ItemSpecOverrideEntry
{
    uint32      ID;                                         // 0
    uint32      ItemID;                                     // 1
    uint32      SpecID;                                     // 2
};

struct LFGDungeonEntry
{
    uint32  ID;                                             // 0
    char*   name;                                           // 1
    uint32  minlevel;                                       // 2
    uint32  maxlevel;                                       // 3
    uint32  reclevel;                                       // 4
    uint32  recminlevel;                                    // 5
    uint32  recmaxlevel;                                    // 6
    int32   map;                                            // 7
    uint32  difficulty;                                     // 8
    uint32  flags;                                          // 9
    uint32  type;                                           // 10
    int32   faction;                                        // 11
    //char*   textureFileName;                              // 12 Name lite
    uint32  expansion;                                      // 13
    //uint32 orderIndex;                                    // 14
    uint32 groupId;                                         // 15
    //char* descr                                           // 16 Description
    uint32 random_id;                                       // 17 random id
    uint32 tankNeeded;                                      // 18
    uint32 healerNeeded;                                    // 19
    uint32 dpsNeeded;                                       // 20
    uint32 minTankNeeded;                                   // 21
    uint32 minHealerNeeded;                                 // 22
    uint32 minDpsNeeded;                                    // 23   5.4.1
    uint32 scenarioId;                                      // 24   5.4.1
    uint32 subType;                                         // 25   5.4.1
    uint32 bonusRepAmt;                                     // 26   5.4.1
    //uint32 mentorCharLevel;                               // 27   5.4.1
    //uint32 mentorItemLevel;                               // 28   5.4.1

    // Helpers
    uint32 Entry() const { return ID + (type << 24); }
    bool IsScenario() const { return subType == LFG_SUBTYPE_SCENARIO; }
    bool IsChallenge() const { return difficulty == DIFFICULTY_CHALLENGE; }
    bool IsRaidFinder() const { return difficulty == DIFFICULTY_LFR; }
    bool IsFlex() const { return difficulty == FLEXIBLE_DIFFICULTY; }
    uint32 GetMinGroupSize() const { return minTankNeeded + minHealerNeeded + minDpsNeeded; }
    uint32 GetMaxGroupSize() const { return tankNeeded + healerNeeded + dpsNeeded; }
    bool IsValid() const
    {
        switch (ID)
        {
            case 540:           // Temple of Kotmogu
            case 541:           // Test Scenario PS
            case 645:           // Greenstone Village
            case 697:           // Scenario: Prototype
            case 699:
                return false;
            default:
                break;
        }

        return true;
    }

    LfgType GetInternalType() const
    {
        switch (subType)
        {
            case LFG_SUBTYPE_DUNGEON:
                return LFG_TYPE_DUNGEON;
            case LFG_SUBTYPE_RAID:
            case LFG_SUBTYPE_FLEX:
                return LFG_TYPE_RAID;
            case LFG_SUBTYPE_SCENARIO:
                return LFG_TYPE_SCENARIO;
            default:
                break;
        }

        return LFG_TYPE_DUNGEON;
    }

    bool CanBeRewarded() const
    {
        return type == LFG_TYPE_RANDOM || IsRaidFinder() || IsChallenge() || flags & LFG_FLAG_SEASONAL;
    }

    bool FitsTeam(uint32 team) const
    {
        if (faction == -1)
            return true;
        else if (faction == 0)
            return team == HORDE;
        else
            return team == ALLIANCE;
    }
};


struct LiquidTypeEntry
{
    uint32 Id;
    //char*  Name;
    //uint32 Flags;
    uint32 Type;
    //uint32 SoundId;
    uint32 SpellId;
    //float     MaxDarkenDepth;                             // 6
    //float     FogDarkenIntensity;                         // 7
    //float     AmbDarkenIntensity;                         // 8
    //float     DirDarkenIntensity;                         // 9
    //uint32    LightID;                                    // 10
    //float     ParticleScale;                              // 11
    //uint32    ParticleMovement;                           // 12
    //uint32    ParticleTexSlots;                           // 13
    //uint32    MaterialID;                                 // 14
    //char*     Texture[6];                                 // 15-20
    //uint32    Color[2];                                   // 21-23
    //float     Unk1[18];                                   // 24-41
    //uint32    Unk2[4];                                    // 42-45
    //uint32    Unk3[5];                                    // 46-50
};

struct LockEntry
{
    uint32      ID;                                         // 0        m_ID
    uint32      Type[MAX_LOCK_CASE];                        // 1-8      m_Type
    uint32      Index[MAX_LOCK_CASE];                       // 9-16     m_Index
    uint32      Skill[MAX_LOCK_CASE];                       // 17-24    m_Skill
    uint32      Action[MAX_LOCK_CASE];                      // 25-32    m_Action
};

struct PhaseEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
};

struct MailTemplateEntry
{
    uint32      ID;                                         // 0
    //char*       subject;                                  // 1        m_subject_lang
    char* content;                                          // 2        m_body_lang
};

struct MapEntry
{
    uint32  MapID;                                          // 0
    //char*       internalname;                             // 1 unused
    uint32  InstanceType;                                   // 2
    uint32 flags;                                           // 3
    //uint32        MapType;                                // 4
    //uint32 unk4;                                          // 5 4.0.1
    char* name;                                             // 6        m_MapName_lang
    uint32  linked_zone;                                    // 7        m_areaTableID
    //char*     hordeIntro;                                 // 8        m_MapDescription0_lang
    //char*     allianceIntro;                              // 9        m_MapDescription1_lang
    uint32  multimap_id;                                    // 10       m_LoadingScreenID (LoadingScreens.dbc)
    //float   BattlefieldMapIconScale;                      // 11       m_minimapIconScale
    int32   entrance_map;                                   // 12       m_corpseMapID map_id of entrance map in ghost mode (continent always and in most cases = normal entrance)
    float   entrance_x;                                     // 13       m_corpseX entrance x coordinate in ghost mode  (in most cases = normal entrance)
    float   entrance_y;                                     // 14       m_corpseY entrance y coordinate in ghost mode  (in most cases = normal entrance)
    //uint32  timeOfDayOverride;                            // 15       m_timeOfDayOverride
    uint32  addon;                                          // 16       m_expansionID
    uint32 unk_time;                                        // 17       m_raidOffset
    uint32 maxPlayers;                                      // 18       m_maxPlayers
    int32   ParentMapID;                                    // 19
    //uint32        CosmeticParentMapID                     // 20
    //uint32        TimeOffset                              // 21
    // Helpers
    uint32 Expansion() const { return addon; }

    bool IsDungeon() const { return InstanceType == MAP_INSTANCE || InstanceType == MAP_RAID || InstanceType == MAP_SCENARIO; }
    bool IsNonRaidDungeon() const { return InstanceType == MAP_INSTANCE || InstanceType == MAP_SCENARIO; }
    bool Instanceable() const { return InstanceType == MAP_INSTANCE || InstanceType == MAP_RAID || InstanceType == MAP_BATTLEGROUND || InstanceType == MAP_ARENA || InstanceType == MAP_SCENARIO; }
    bool IsRaid() const { return InstanceType == MAP_RAID; }
    bool IsBattleground() const { return InstanceType == MAP_BATTLEGROUND; }
    bool IsBattleArena() const { return InstanceType == MAP_ARENA; }
    bool IsBattlegroundOrArena() const { return InstanceType == MAP_BATTLEGROUND || InstanceType == MAP_ARENA; }
    bool IsWorldMap() const { return InstanceType == MAP_COMMON; }
    bool IsScenario() const { return InstanceType == MAP_SCENARIO; }

    bool GetEntrancePos(int32 &mapid, float &x, float &y) const
    {
        if (entrance_map < 0)
            return false;
        mapid = entrance_map;
        x = entrance_x;
        y = entrance_y;
        return true;
    }

    bool IsContinent() const
    {
        return MapID == 0 || MapID == 1 || MapID == 530 || MapID == 571 || MapID == 870 || MapID == 1116;
    }

    bool IsDynamicDifficultyMap() const { return (flags & MAP_FLAG_CAN_CHANGE_DIFFICULTY) != 0; }
    bool IsGarrison() const { return (flags & MAP_FLAG_GARRISON) != 0; }

    bool IsDifficultyModeSupported(uint32 difficulty) const;
};

struct MinorTalentEntry
{
    uint32      ID;                                         // 0
    uint32      SpecID;                                     // 1
    uint32      SpellID;                                    // 2
    uint32      OrderIndex;                                 // 3
};

struct MapDifficultyEntry
{
    //uint32    ID;                                         // 0
    uint32      MapID;                                      // 1
    uint32      DifficultyID;                               // 2 (for arenas: arena slot)
    char*       Message_lang;                               // 3 m_message_lang (text showed when transfer to map failed)
    uint32      RaidDuration;                               // 4 m_raidDuration in secs, 0 if no fixed reset time
    uint32      MaxPlayers;                                 // 5 m_maxPlayers some heroic versions have 0 when expected same amount as in normal version
    uint32      LockID;                                     // 6
    uint32      ItemBonusTreeModID;                         // 7
};

struct MountCapabilityEntry
{
    uint32 Id;
    uint32 Flags;
    uint32 RequiredRidingSkill;
    uint32 RequiredArea;
    uint32 RequiredAura;
    uint32 RequiredSpell;
    uint32 SpeedModSpell;
    int32  RequiredMap;
};

struct MountTypeEntry
{
    uint32 Id;
    uint32 MountCapability[MAX_MOUNT_CAPABILITIES];
};

struct MovieEntry
{
    uint32      Id;                                         // 0 index
    //char*       filename;                                 // 1
    //uint32      unk1;                                     // 2        m_volume
    //uint32      unk2;                                     // 3 4.0.0
    //uint32      unk3;                                     // 4 5.0.0
};

struct NameGenEntry
{
    //uint32 id;
    char*  name;
    uint32 race;
    uint32 gender;
};

struct PvPDifficultyEntry
{
    //uint32      id;                                       // 0        m_ID
    uint32      mapId;                                      // 1
    uint32      bracketId;                                  // 2
    uint32      minLevel;                                   // 3
    uint32      maxLevel;                                   // 4

    // helpers
    BattlegroundBracketId GetBracketId() const { return BattlegroundBracketId(bracketId); }
};

struct QuestSortEntry
{
    uint32      id;                                         // 0        m_ID
    //char*       name;                                     // 1        m_SortName_lang
};

struct QuestV2Entry
{
    uint32      ID;                                         // 0
    uint32      UniqueBitFlag;                              // 1
};

struct QuestXPEntry
{
  uint32      id;
  uint32      Exp[10];
};

struct QuestFactionRewEntry
{
  uint32      id;
  int32       QuestRewFactionValue[10];
};

struct RandomPropertiesPointsEntry
{
    //uint32  Id;                                           // 0 hidden key
    uint32    itemLevel;                                    // 1
    uint32    EpicPropertiesPoints[5];                      // 2-6
    uint32    RarePropertiesPoints[5];                      // 7-11
    uint32    UncommonPropertiesPoints[5];                  // 12-16
};

struct ResearchBranchEntry
{
    uint32    ID;                                           // 0 ID
    char* Race;                                             // 1 Archeaology Race (Fossil, Troll, Nerubian)
    //char* AlwaysDwarf;                                    // 2 unk. Always Dwarf.
    uint32    CurrencyID;                                   // 3 CurrencyID
    //                                                      // 4 Icon Path
    uint32    ItemID;                                       // 5 ItemID
};

struct ResearchProjectEntry
{
    uint32    ID;                                           // 0 ID
    char*     Name;                                         // 1 Name of item.
    char*     Description;                                  // 2 Item description
    uint32    rare;                                         // 3
    uint32    branchId;                                     // 4
    uint32    SpellID;                                      // 5 SpellID - Reward
    uint32    Complexity;                                   // 6 Item from ResearchBranch
    //                                                      // 7 Icon Path
    uint32    RequiredCurrencyAmount;                       // 8 Required currency amount for create (should be * 100)

    bool IsVaid() const
    {
        return branchId != ARCHAEOLOGY_BRANCH_UNUSED &&
            branchId != ARCHAEOLOGY_BRANCH_NONE;
    }
};

struct ResearchSiteEntry
{
    uint32    ID;                                           // 0 ID
    uint32    MapID;                                        // 1 MapID
    uint32    POIid;                                        // 2
    char*     areaName;                                     // 3 Research site name
    //uint32  flags;                                        // 4 Always 177.

    bool IsValid() const
    {
        return ID != 140 && // template
            ID != 142 &&    // template
            ID != 161 &&    // template
            ID != 471 &&    // vashj'ir
            ID != 473 &&    // vashj'ir
            ID != 475 &&    // vashj'ir
            ID != 949 &&    // template
            ID != 951 &&    // template
            ID != 1023 &&   // not on wowhead
            ID != 1049 &&   // not on wowhead
            ID != 1051 &&   // not on wowhead
            ID != 1284;     // not on wowhead
    }
};

struct QuestPOIBlobEntry
{
    uint32    ID;                                           // 0 ID
    uint32    Count;                                        // 1 m_NumPoints
    uint32    MapID;                                        // 2 m_MapID
    //uint32 unk;                                           // 4 m_WorldMapAreaID
};

struct QuestPOIPointEntry
{
    //uint32    ID;                                           // 0
    int32     x;                                            // 1
    int32     y;                                            // 2
    uint32    POIId;                                        // 4    m_QuestPOIBlobID
};

struct ScalingStatDistributionEntry
{
    uint32      ID;                                         // 0
    uint32      MinLevel;                                   // 1
    uint32      MaxLevel;                                   // 2       m_maxlevel
    uint32      ItemLevelCurveID;                           // 3
};

struct ScenarioEntry
{
    uint32 m_Id;                                            // 0
    char* m_name_lang;                                      // 1
    uint32 m_flags;                                         // 2

    bool IsChallenge() const { return m_flags & SCENARIO_FLAG_CHALLENGE; }
    bool IsProvingGrounds() const { return m_flags & SCENARIO_FLAG_SUPRESS_STAGE_TEXT; }
};

struct ScenarioStepEntry
{
    uint32 m_Id;                                            // 0
    uint32 m_criteriaTreeId;                                // 1
    uint32 m_scenarioId;                                    // 2
    uint32 m_orderIndex;                                    // 3
    //char* m_descriptionLang;                              // 4
    //char* m_title_lang;                                   // 5
    uint32 m_flags;                                         // 6
    uint32 RelatedStep;                                     // 7
    uint32 Supersedes;                                      // 8
    uint32 RewardQuestID;                                   // 9

    bool IsBonusObjective() const { return m_flags & SCENARIO_STEP_FLAG_BONUS_OBJECTIVE; }
};

//struct SkillLineCategoryEntry{
//    uint32    id;                                         // 0      m_ID
//    char*     name[16];                                   // 1-17   m_name_lang
//                                                          // 18 string flag
//    uint32    displayOrder;                               // 19     m_sortIndex
//};

struct SkillRaceClassInfoEntry
{
    //uint32    ID;                                         // 0
    uint32      SkillID;                                    // 1
    int32       RaceMask;                                   // 2
    int32       ClassMask;                                  // 3
    uint32      Flags;                                      // 4
    uint32      Availability;                               // 5
    uint32      MinLevel;                                   // 6
    uint32      SkillTierID;                                // 7
};

//struct SkillTiersEntry{
//    uint32    id;                                         // 0      m_ID
//    uint32    skillValue[16];                             // 1-17   m_cost
//    uint32    maxSkillValue[16];                          // 18-32  m_valueMax
//};

struct SkillLineEntry
{
    uint32 id;                                              // 0	m_ID
    int32 categoryId;                                       // 1	m_categoryID
    char* name;                                             // 2	m_displayName_lang
    //DBCString description;                                // 3	m_description_lang
    uint32 spellIcon;                                       // 4	m_spellIconID
    //DBCString alternateVerb;                              // 5	m_alternateVerb_lang
    uint32 canLink;                                         // 6	m_canLink (prof. with recipes)
    uint32 parentSkillId;                                   // 7	5.0.1
    //uint32                                                // 8    5.4.1
};

struct SkillLineAbilityEntry
{
    uint32      ID;                                         // 0
    uint32      SkillLine;                                  // 1
    uint32      SpellID;                                    // 2
    uint32      RaceMask;                                   // 3
    uint32      ClassMask;                                  // 4
    uint32      MinSkillLineRank;                           // 7
    uint32      SupercedesSpell;                            // 8
    uint32      AquireMethod;                               // 9
    uint32      TrivialSkillLineRankHigh;                   // 10
    uint32      TrivialSkillLineRankLow;                    // 11
    uint32      NumSkillUps;                                // 12
    uint32      UniqueBit;                                  // 13
    uint32      TradeSkillCategoryID;                       // 14
};

// SpecializationSpells.dbc
struct SpecializationSpellEntry
{
    uint32  Id;                      // 0
    uint32  SpecializationEntry;     // 1
    uint32  LearnSpell;              // 2
    uint32  OverrideSpell;           // 3
    //uint32 unk;                    // 4
};

// SpellEffect.dbc
struct SpellEffectEntry
{
    uint32      ID;                                         // 0
    uint32      DifficultyID;                               // 1
    uint32      Effect;                                     // 2
    float       EffectAmplitude;                            // 3
    uint32      EffectAura;                                 // 4
    uint32      EffectAuraPeriod;                           // 5
    uint32      EffectBasePoints;                           // 6
    float       EffectBonusCoefficient;                     // 7
    float       EffectChainAmplitude;                       // 8
    uint32      EffectChainTargets;                         // 9
    uint32      EffectDieSides;                             // 10
    uint32      EffectItemType;                             // 11
    uint32      EffectMechanic;                             // 12
    int32       EffectMiscValue;                            // 13
    int32       EffectMiscValueB;                           // 14
    float       EffectPointsPerResource;                    // 15
    uint32      EffectRadiusIndex;                          // 16
    uint32      EffectRadiusMaxIndex;                       // 17
    float       EffectRealPointsPerLevel;                   // 18
    flag128     EffectSpellClassMask;                       // 19-22
    uint32      EffectTriggerSpell;                         // 23
    float       EffectPosFacing;                            // 24
    uint32      ImplicitTarget[2];                          // 25-26
    uint32      SpellID;                                    // 27
    uint32      EffectIndex;                                // 28
    uint32      EffectAttributes;                           // 29
    float       BonusCoefficientFromAP;                     // 30
};

struct SpellEffectScalingEntry
{
    //uint32    Id;                                         // 0
    float     Coefficient;                                  // 1
    float     Variance;                                     // 2
    float     ResourceCoefficient;                          // 3
    uint32    SpellEffectId;                                // 4
};

// SpellAuraOptions.dbc
struct SpellAuraOptionsEntry
{
    uint32    Id;                                           // 0       m_ID
    //uint32    spellId;                                    // 1       5.0.5
    //uint32    m_difficultyID                              // 2       5.0.5
    uint32    StackAmount;                                  // 3       m_cumulativeAura
    uint32    procChance;                                   // 4       m_procChance
    uint32    procCharges;                                  // 5       m_procCharges
    uint32    procFlags;                                    // 6       m_procTypeMask
    uint32    procTimeRec;                                  // 7       m_procTimeRecovery
    uint32    procPerMinId;                                 // 8       SpellProcsPerMinuteId
};

// SpellProcsPerMinute.dbc
struct SpellProcsPerMinuteEntry
{
    uint32    Id;                                           // 0       m_ID
    float     ppmRate;                                      // 1       m_ppmRate
    //uint32                                                // 2       not use
};

// SpellProcsPerMinuteMod.dbc
struct SpellProcsPerMinuteModEntry
{
    uint32    Id;                                           // 0       m_ID
    //uint32    spellId;                                    // 1       unk
    uint32    specId;                                       // 2       SpecializationId
    float     ppmRateMod;                                   // 3       ppmRate mod for spec
    uint32    ProcsPerMinuteId;                             // 4       SpellProcsPerMinuteId
};

// Spell.dbc
struct SpellEntry
{
    uint32      ID;                                         // 0
    char*       Name_lang;                                  // 1
    char*       NameSubtext_lang;                           // 2
    //char*     Description_lang;                           // 3
    //char*     AuraDescription_lang;                       // 4
    uint32      RuneCostID;                                 // 5
    uint32      SpellMissileID;                             // 6
    uint32      DescriptionVariablesID;                     // 7
    uint32      ScalingID;                                  // 8
    uint32      AuraOptionsID;                              // 9
    uint32      AuraRestrictionsID;                         // 10
    uint32      CastingRequirementsID;                      // 11
    uint32      CategoriesID;                               // 12
    uint32      ClassOptionsID;                             // 13
    uint32      CooldownsID;                                // 14
    uint32      EquippedItemsID;                            // 15
    uint32      InterruptsID;                               // 16
    uint32      LevelsID;                                   // 17
    uint32      ReagentsID;                                 // 18
    uint32      ShapeshiftID;                               // 19
    uint32      TargetRestrictionsID;                       // 20
    uint32      TotemsID;                                   // 21
    uint32      RequiredProjectID;                          // 22
    uint32      MiscID;                                     // 23

    // struct access functions
    SpellEffectEntry const* GetSpellEffect(uint32 eff, uint8 diff = 0) const;
};

// SpellCategories.dbc
struct SpellCategoriesEntry
{
    //uint32    Id;                                         // 0        m_ID
    uint32    Category;                                     // 1        m_category
    uint32    DmgClass;                                     // 153      m_defenseType
    uint32    Dispel;                                       // 2        m_dispelType
    uint32    Mechanic;                                     // 3        m_mechanic
    uint32    PreventionType;                               // 154      m_preventionType
    uint32    StartRecoveryCategory;                        // 145      m_startRecoveryCategory
    uint32    ChargeRecoveryCategory;                       //
};

// SpellCategory.dbc
struct SpellCategoryEntry
{
    uint32 CategoryId;
    uint32 Flags;
    //uint32
    //char* categoryName;
    uint32 chargeCount;
    uint32 chargeRegenTime;
};

typedef std::set<uint32> SpellCategorySet;
typedef std::unordered_map<uint32, SpellCategorySet > SpellCategoryStore;
typedef std::list<const SpellEntry*> SpellSkillingList;
typedef std::set<uint32> PetFamilySpellsSet;
typedef std::unordered_map<uint32, PetFamilySpellsSet > PetFamilySpellsStore;

struct SpellCastTimesEntry
{
    uint32    ID;                                           // 0
    int32     CastTime;                                     // 1
    //float     CastTimePerLevel;                           // 2 unsure / per skill?
    //int32     MinCastTime;                                // 3 unsure
};

struct SpellFocusObjectEntry
{
    uint32    ID;                                           // 0
    //char*     Name;                                       // 1        m_name_lang
};

struct SpellRadiusEntry
{
    uint32    ID;                                           // 0
    float     radiusHostile;                                // 1
    //float   radiusPerLevel                                // 2
    //uint32                                                // 3    new 5.0.5
    float     radiusFriend;                                 // 4
};

struct SpellRangeEntry
{
    uint32    ID;
    float     minRangeHostile;
    float     minRangeFriend;
    float     maxRangeHostile;
    float     maxRangeFriend;                               //friend means unattackable unit here
    uint32    type;
    //char*   Name;                                         // 6-21     m_displayName_lang
    //char*   ShortName;                                    // 23-38    m_displayNameShort_lang
};

// SpellEquippedItems.dbc
struct SpellEquippedItemsEntry
{
    //uint32    Id;                                         // 0        m_ID
    //uint32 spellId;                                       // 1
    //uint32 difficulty;                                    // 2
    int32     EquippedItemClass;                            // 3        m_equippedItemClass (value)
    int32     EquippedItemInventoryTypeMask;                // 4        m_equippedItemInvTypes (mask)
    int32     EquippedItemSubClassMask;                     // 5        m_equippedItemSubclass (mask)
};

// SpellCooldowns.dbc
struct SpellCooldownsEntry
{
    //uint32    Id;                                         // 0        m_ID
    uint32    CategoryRecoveryTime;                         // 31       m_categoryRecoveryTime
    uint32    RecoveryTime;                                 // 30       m_recoveryTime
    uint32    StartRecoveryTime;                            // 146      m_startRecoveryTime
};

// SpellInterrupts.dbc
struct SpellInterruptsEntry
{
    //uint32    Id;                                         // 0       m_ID
    uint32    AuraInterruptFlags;                           // 1       m_auraInterruptFlags
    //uint32                                                // 2       4.0.0
    uint32    ChannelInterruptFlags;                        // 3       m_channelInterruptFlags
    //uint32                                                // 4       4.0.0
    uint32    InterruptFlags;                               // 5       m_interruptFlags
};

// SpellLevels.dbc
struct SpellLevelsEntry
{
    //uint32 Id; // 0 m_ID
    uint32 SpellId; // 1 m_spellId
    //uint32 m_difficultyID; // 2 5.0.1.15589
    uint32 baseLevel; // 3 m_baseLevel
    uint32 maxLevel; // 4 m_maxLevel
    uint32 spellLevel; // 5 m_spellLevel
};

// PowerDisplay.dbc
struct PowerDisplayEntry
{
    uint32    Id;                                           // 0 m_ID
    uint32    PowerType;                                    // 1 m_actualType
    //char* m_globalStringBaseTag;                          // 2 m_globalStringBaseTag
    //uint32    red;                                        // 3 m_red
    //uint32    green;                                      // 4 m_green
    //uint32    blue;                                       // 5 m_blue
};

struct SpellShapeshiftFormEntry
{
    uint32 ID;                                              // 0
    //uint32 m_bonusActionBar;                              // 1 unused
    //char* Name;                                           // 2 unused
    uint32 flags1;                                          // 3
    int32  creatureType;                                    // 4 <=0 humanoid, other normal creature types
    //uint32 m_attackIconID;                                // 5 unused, related to next field
    uint32 attackSpeed;                                     // 6
    uint32 modelID_A;                                       // 7 alliance modelid (0 means no model)
    uint32 modelID_H;                                       // 8 horde modelid (but only for one form)
    //uint32 modelID;                                       // 9 unused always 0
    //uint32 modelID;                                       // 10 unused always 0
    uint32 stanceSpell[MAX_SHAPESHIFT_SPELLS];              // 11-18 spells which appear in the bar after shapeshifting
    //uint32 m_mountTypeID;                                 // 19
    //uint32 m_exitSoundEntriesID;                          // 20
};

// SpellShapeshift.dbc
struct SpellShapeshiftEntry
{
    uint32    Id;                                           // 0 - m_ID
    uint32    StancesNot;                                   // 3 - m_shapeshiftExclude
    // uint32 StancesNot2;                                  // 2 - 3.2.0
    uint32    Stances;                                      // 1 - m_shapeshiftMask
    // uint32 Stances2;                                     // 4 - 3.2.0
    // uint32    StanceBarOrder;                            // 5 - m_stanceBarOrder not used
};

// SpellTargetRestrictions.dbc
struct SpellTargetRestrictionsEntry
{
    uint32 Id; // 0 m_ID
    uint32 SpellId; // 1 m_spellId
    uint32 m_difficultyID; // 2 5.0.1.15589
    float MaxTargetRadius; // 3 m_maxTargetRadius
    //float m_width; // 5 5.0.1.15589
    uint32 MaxAffectedTargets; // 5 m_maxTargets
    uint32 MaxTargetLevel; // 6 m_maxTargetLevel
    uint32 TargetCreatureType; // 7 m_targetCreatureType
    uint32 Targets; // 8 m_targets
};

// SpellScaling.dbc
struct SpellScalingEntry
{
    //uint32    Id;                                         // 0        m_ID
    int32     CastTimeMin;                                  // 1
    int32     CastTimeMax;                                  // 2
    uint32    CastTimeMaxLevel;                             // 3
    int32     ScalingClass;                                 // 4        (index * 100) + charLevel - 1 => gtSpellScaling.dbc
    float     NerfFactor;                                   // 5
    uint32    NerfMaxLevel;                                 // 6
    uint32    MaxScalingLevel;                              // 7
    uint32    ScalesFromItemLevel;                          // 8
};

struct SpellDurationEntry
{
    uint32    ID;
    int32     Duration[3];
};

struct SpellItemEnchantmentEntry
{
    uint32      ID;                                             // 0
    uint32      Charges;                                        // 1
    uint32      Effect[MAX_ITEM_ENCHANTMENT_EFFECTS];           // 2-4
    uint32      EffectPointsMin[MAX_ITEM_ENCHANTMENT_EFFECTS];  // 5-7
    uint32      EffectSpellID[MAX_ITEM_ENCHANTMENT_EFFECTS];    // 8-10
    //char*     Name_lang                                       // 11
    uint32      ItemVisual;                                     // 12
    uint32      Flags;                                          // 13
    uint32      SRCItemID;                                      // 14
    uint32      ConditionID;                                    // 15
    uint32      RequiredSkillID;                                // 16
    uint32      RequiredSkillRank;                              // 17
    uint32      MinLevel;                                       // 18
    uint32      MaxLevel;                                       // 19
    uint32      ItemLevel;                                      // 20
    int32       ScalingClass;                                   // 21
    int32       ScalingClassRestricted;                         // 22
    float       EffectScalingPoints[MAX_ITEM_ENCHANTMENT_EFFECTS];//23-25
};

struct SpellItemEnchantmentConditionEntry
{
    uint32  ID;                                             // 0        m_ID
    uint8   Color[5];                                       // 1-5      m_lt_operandType[5]
    //uint32  LT_Operand[5];                                // 6-10     m_lt_operand[5]
    uint8   Comparator[5];                                  // 11-15    m_operator[5]
    uint8   CompareColor[5];                                // 15-20    m_rt_operandType[5]
    uint32  Value[5];                                       // 21-25    m_rt_operand[5]
    //uint8   Logic[5]                                      // 25-30    m_logic[5]
};

struct StableSlotPricesEntry
{
    uint32 Slot;
    uint32 Price;
};

struct SummonPropertiesEntry
{
    uint32  Id;                                             // 0
    uint32  Category;                                       // 1, 0 - can't be controlled?, 1 - something guardian?, 2 - pet?, 3 - something controllable?, 4 - taxi/mount?
    uint32  Faction;                                        // 2, 14 rows > 0
    uint32  Type;                                           // 3, see enum
    int32   Slot;                                           // 4, 0-6
    uint32  Flags;                                          // 5
};

struct TalentEntry
{
    uint32  Id;                                     // 0
    uint32  SpecID;                                 // 1
    uint32  row;                                    // 2
    uint32  column;                                 // 3
    uint32  spellId;                                // 4
    //uint32  m_flags;                              // 5
    //uint32  m_categoryMask1;                      // 6
    //uint32  m_categoryMask2;                      // 7
    uint32  classId;                                // 8
    uint32  OverridesSpellID;                       // 9
    char*   description;                            // 10

};

struct TeamContributionPointsEntry
{
    //uint32    entry;                                      // 0
    float     value;                                        // 1 (???)
};

struct TotemCategoryEntry
{
    uint32    ID;                                           // 0
    //char*   name;                                         // 1        m_name_lang
    uint32    categoryType;                                 // 2        m_totemCategoryType (one for specialization)
    uint32    categoryMask;                                 // 3        m_totemCategoryMask (compatibility mask for same type: different for totems, compatible from high to low for rods)
};


struct TransportAnimationEntry
{
    //uint32        ID;                                     // 0
    uint32          TransportID;                            // 1
    uint32          TimeIndex;                              // 2
    DBCPosition3D   Pos;                                    // 3-5
    //uint32        SequenceID;                             // 6
};
 
struct TransportRotationEntry
{
    //uint32    ID;                                         // 0
    uint32      TransportID;                                // 1
    uint32      TimeIndex;                                  // 2
    float       X;                                          // 3
    float       Y;                                          // 4
    float       Z;                                          // 5
    float       W;                                          // 6
};

struct UnitPowerBarEntry
{
    uint32 ID;                     // 0
    uint32 MinPower;               // 1
    uint32 MaxPower;               // 2
    uint32 startPower;             // 3
    uint32 centerPower;            // 4
    float regenerationPeace;       // 5
    float regenerationCombat;      // 6
    uint32 barType;                // 7
    //uint32 fileDataID[6];        // 8-13
    //uint32 color[6];             // 14-19
    uint32 flags;                  // 20
    //char* name;
    //char* costString;
    //char* errorString;
    //char* tooltipString;
    // float
    // float
};

struct VehicleEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
    uint32      FlagsB;                                     // 2
    float       TurnSpeed;                                  // 3
    float       PitchSpeed;                                 // 4
    float       PitchMin;                                   // 5
    float       PitchMax;                                   // 6
    uint32      SeatID[MAX_VEHICLE_SEATS];                  // 7-14
    float       MouseLookOffsetPitch;                       // 15
    float       CameraFadeDistScalarMin;                    // 16
    float       CameraFadeDistScalarMax;                    // 17
    float       CameraPitchOffset;                          // 18
    float       FacingLimitRight;                           // 19
    float       FacingLimitLeft;                            // 20
    float       MsslTrgtTurnLingering;                      // 21
    float       MsslTrgtPitchLingering;                     // 22
    float       MsslTrgtMouseLingering;                     // 23
    float       MsslTrgtEndOpacity;                         // 24
    float       MsslTrgtArcSpeed;                           // 25
    float       MsslTrgtArcRepeat;                          // 26
    float       MsslTrgtArcWidth;                           // 27
    float       MsslTrgtImpactRadius[2];                    // 28-29
    //char*     MsslTrgtArcTexture;                         // 30
    //char*     MsslTrgtImpactTexture;                      // 31
    //char*     MsslTrgtImpactModel;                        // 32-33
    float       CameraYawOffset;                            // 34
    uint32      UILocomotionType;                           // 35
    float       MsslTrgtImpactTexRadius;                    // 36
    uint32      VehicleUIIndicatorID;                       // 37
    uint32      PowerDisplayID[3];                          // 38-40
};

struct VehicleSeatEntry
{
    uint32          ID;                                     // 0
    uint32          Flags;                                  // 1
    int32           AttachmentID;                           // 2
    DBCPosition3D   AttachmentOffset;                       // 3-5
    float           EnterPreDelay;                          // 6
    float           EnterSpeed;                             // 7
    float           EnterGravity;                           // 8
    float           EnterMinDuration;                       // 9
    float           EnterMaxDuration;                       // 10
    float           EnterMinArcHeight;                      // 11
    float           EnterMaxArcHeight;                      // 12
    int32           EnterAnimStart;                         // 13
    int32           EnterAnimLoop;                          // 14
    int32           RideAnimStart;                          // 15
    int32           RideAnimLoop;                           // 16
    int32           RideUpperAnimStart;                     // 17
    int32           RideUpperAnimLoop;                      // 18
    float           ExitPreDelay;                           // 19
    float           ExitSpeed;                              // 20
    float           ExitGravity;                            // 21
    float           ExitMinDuration;                        // 22
    float           ExitMaxDuration;                        // 23
    float           ExitMinArcHeight;                       // 24
    float           ExitMaxArcHeight;                       // 25
    int32           ExitAnimStart;                          // 26
    int32           ExitAnimLoop;                           // 27
    int32           ExitAnimEnd;                            // 28
    float           PassengerYaw;                           // 29
    float           PassengerPitch;                         // 30
    float           PassengerRoll;                          // 31
    int32           PassengerAttachmentID;                  // 32
    int32           VehicleEnterAnim;                       // 33
    int32           VehicleExitAnim;                        // 34
    int32           VehicleRideAnimLoop;                    // 35
    int32           VehicleEnterAnimBone;                   // 36
    int32           VehicleExitAnimBone;                    // 37
    int32           VehicleRideAnimLoopBone;                // 38
    float           VehicleEnterAnimDelay;                  // 39
    float           VehicleExitAnimDelay;                   // 40
    uint32          VehicleAbilityDisplay;                  // 41
    uint32          EnterUISoundID;                         // 42
    uint32          ExitUISoundID;                          // 43
    uint32          FlagsB;                                 // 44
    float           CameraEnteringDelay;                    // 45
    float           CameraEnteringDuration;                 // 46
    float           CameraExitingDelay;                     // 47
    float           CameraExitingDuration;                  // 48
    DBCPosition3D   CameraOffset;                           // 49-51
    float           CameraPosChaseRate;                     // 52
    float           CameraFacingChaseRate;                  // 53
    float           CameraEnteringZoom;                     // 54
    float           CameraSeatZoomMin;                      // 55
    float           CameraSeatZoomMax;                      // 56
    uint32          EnterAnimKitID;                         // 57
    uint32          RideAnimKitID;                          // 58
    uint32          ExitAnimKitID;                          // 59
    uint32          VehicleEnterAnimKitID;                  // 60
    uint32          VehicleRideAnimKitID;                   // 61
    uint32          VehicleExitAnimKitID;                   // 62
    uint32          CameraModeID;                           // 63
    uint32          FlagsC;                                 // 64
    uint32          UISkinFileDataID;                       // 65

    bool CanEnterOrExit() const { return Flags & VEHICLE_SEAT_FLAG_CAN_ENTER_OR_EXIT; }
    bool CanSwitchFromSeat() const { return Flags & VEHICLE_SEAT_FLAG_CAN_SWITCH; }
    bool IsUsableByOverride() const { return (Flags & VEHICLE_SEAT_FLAG_UNCONTROLLED)
                                    || (FlagsB & (VEHICLE_SEAT_FLAG_B_USABLE_FORCED | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_2 |
                                        VEHICLE_SEAT_FLAG_B_USABLE_FORCED_3 | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_4)); }
    bool IsEjectable() const { return FlagsB & VEHICLE_SEAT_FLAG_B_EJECTABLE; }
};

struct WMOAreaTableEntry
{
    uint32      ID;                                         // 0 index
    int32       WMOID;                                      // 1 used in root WMO
    int32       NameSet;                                    // 2 used in adt file
    int32       WMOGroupID;                                 // 3 used in group WMO
    //uint32    SoundProviderPref;                          // 4
    //uint32    SoundProviderPrefUnderwater;                // 5
    //uint32    AmbienceID;                                 // 6
    //uint32    ZoneMusic;                                  // 7
    //uint32    IntroSound;                                 // 8
    uint32      Flags;                                      // 9 used for indoor/outdoor determination
    uint32      AreaTableID;                                // 10 link to AreaTableEntry.ID
    char*       AreaName_lang;                              // 11       m_AreaName_lang
    //uint32    UWIntroSound;                               // 12
    //uint32    UWZoneMusic;                                // 13
    //uint32    UWAmbience;                                 // 14
};

struct WorldMapAreaEntry
{
    //uint32    ID;                                         // 0
    uint32      MapID;                                      // 1
    uint32      AreaID;                                     // 2 index (continent 0 areas ignored)
    //char*     AreaName                                    // 3
    float       LocLeft;                                    // 4
    float       LocRight;                                   // 5
    float       LocTop;                                     // 6
    float       LocBottom;                                  // 7
    int32       DisplayMapID;                               // 8 -1 (map_id have correct map) other: virtual map where zone show (map_id - where zone in fact internally)
    //int32     DefaultDungeonFloor;                        // 9 pointer to DungeonMap.dbc (owerride loc coordinates)
    //uint32    ParentWorldMapID;                           // 10
    //uint32    Flags;                                      // 11
    uint32    LevelRangeMin;                              // 12 Minimum recommended level displayed on world map
    uint32    LevelRangeMax;                              // 13 Maximum recommended level displayed on world map
};

struct WorldMapOverlayEntry
{
    uint32      ID;                                         // 0
    //uint32    MapAreaID;                                  // 1 idx in WorldMapArea.dbc
    uint32      AreaID[MAX_WORLD_MAP_OVERLAY_AREA_IDX];     // 2-5
    //char*     TextureName;                                // 6
    //uint32    TextureWidth;                               // 7
    //uint32    TextureHeight;                              // 8
    //uint32    OffsetX;                                    // 9
    //uint32    OffsetY;                                    // 10
    //uint32    HitRectTop;                                 // 11
    //uint32    HitRectLeft;                                // 12
    //uint32    HitRectBottom;                              // 13
    //uint32    HitRectRight;                               // 14
    //uint32    PlayerConditionID;                          // 15
};

struct WorldMapTransformsEntry
{
    //uint32 ID;                                            //
    uint32 MapID;                                           //
    DBCPosition3D RegionMin;                                //
    DBCPosition3D RegionMax;                                //
    uint32 NewMapID;                                        //
    DBCPosition2D RegionOffset;                             //
    //uint32 NewDungeonMapID;                               //
    //uint32 Flags;                                         //
    //uint32 NewAreaID;                                     //
    float RegionScale;                                      //
};

struct WorldSafeLocsEntry
{
    uint32          ID;                                     // 0
    uint32          MapID;                                  // 1
    DBCPosition4D   Loc;                                    // 2 - 5
    //char*         AreaName_lang;                          // 6
};

/*
struct WorldStateSounds
{
    uint32    ID;                                           // 0        Worldstate
    uint32    unk;                                          // 1
    uint32    areaTable;                                    // 2
    uint32    WMOAreaTable;                                 // 3
    uint32    zoneIntroMusicTable;                          // 4
    uint32    zoneIntroMusic;                               // 5
    uint32    zoneMusic;                                    // 6
    uint32    soundAmbience;                                // 7
    uint32    soundProviderPreferences;                     // 8
};
*/

/*
struct WorldStateUI
{
    uint32    ID;                                           // 0
    uint32    map_id;                                       // 1        Can be -1 to show up everywhere.
    uint32    zone;                                         // 2        Can be zero for "everywhere".
    uint32    phaseMask;                                    // 3        Phase this WorldState is avaliable in
    uint32    icon;                                         // 4        The icon that is used in the interface.
    char*     textureFilename;                              // 5
    char*     text;                                         // 6-21     The worldstate text
    char*     description;                                  // 22-38    Text shown when hovering mouse on icon
    uint32    worldstateID;                                 // 39       This is the actual ID used
    uint32    type;                                         // 40       0 = unknown, 1 = unknown, 2 = not shown in ui, 3 = wintergrasp
    uint32    unk1;                                         // 41
    uint32    unk2;                                         // 43
    uint32    unk3;                                         // 44-58
    uint32    unk4;                                         // 59-61    Used for some progress bars.
    uint32    unk7;                                         // 62       Unused in 3.3.5a
};
*/

// GCC have alternative #pragma pack() syntax and old gcc version not support pack(pop), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

struct VectorArray
{
    StringVector stringVectorArray[2];
};

typedef std::unordered_map<uint32, VectorArray> NameGenVectorArraysMap;

struct TalentSpellPos
{
    TalentSpellPos() : talent_id(0), rank(0) {}
    TalentSpellPos(uint16 _talent_id, uint8 _rank) : talent_id(_talent_id), rank(_rank) {}

    uint16 talent_id;
    uint8  rank;
};

typedef std::unordered_map<uint32, TalentSpellPos> TalentSpellPosMap;

struct SpellEffect
{
    SpellEffect()
    {
        for(int i = 0; i < MAX_SPELL_EFFECTS; i++)
        {
            effects[i] = NULL;
        }
    }
    SpellEffectEntry const* effects[MAX_SPELL_EFFECTS];
};

typedef std::unordered_map<uint16, SpellEffectEntry const*> SpellEffectsMap;

struct SpellEffectDiff
{
    SpellEffectsMap effects;
};

typedef std::unordered_map<uint32, SpellEffectDiff> SpellEffectDiffMap;
typedef std::unordered_map<uint32, SpellEffect> SpellEffectMap;

typedef std::set<SpellTargetRestrictionsEntry const*> SpellRestrictionMap;

struct SpellRestrictionDiff
{
    SpellRestrictionMap restrictions;
};

typedef std::unordered_map<uint32, SpellRestrictionDiff> SpellRestrictionDiffMap;
typedef std::unordered_map<uint32 /*frame*/, TransportAnimationEntry const*> TransportAnimationEntryMap;
typedef std::unordered_map<uint32, TransportAnimationEntryMap> TransportAnimationsByEntry;

float GetCurrencyPrecision(uint32 currencyId);

// artifact point
struct ResearchPOIPoint
{
    ResearchPOIPoint() : x(0), y(0) { }
    ResearchPOIPoint(int32 _x, int32 _y) : x(_x), y(_y) { }

    int32 x;
    int32 y;
};

struct DigSitePosition
{
    DigSitePosition() : x(0.0f), y(0.0f) { }
    DigSitePosition(float _x, float _y) : x(_x), y(_y) { }

    float x;
    float y;
};

typedef std::vector<ResearchPOIPoint> ResearchPOIPointVector;
typedef std::vector<DigSitePosition> DigSitePositionVector;

struct ResearchSiteData
{
    ResearchSiteData() : zone(0), level(0xFF), branch_id(0), entry(NULL) { }

    ResearchSiteEntry const* entry;
    uint16 zone;
    uint8 level;
    uint8 branch_id;

    ResearchPOIPointVector points;
    DigSitePositionVector digSites;
};

typedef std::map<uint32 /*site_id*/, ResearchSiteData> ResearchSiteDataMap;
ResearchSiteEntry const* GetResearchSiteEntryById(uint32 id);

struct CharacterLoadoutItemEntry
{
    uint32 ID;
    uint32 LoadOutID;
    uint32 ItemID;
};

typedef std::map<uint32 /*LoadOutID*/, std::vector<uint32>> CharacterLoadoutItemMap;

static uint32 LoadOutIdByClass(uint32 classID)
{
    switch(classID)
    {
        case CLASS_WARRIOR: return 539;
        case CLASS_PALADIN: return 524;
        case CLASS_HUNTER: return 533;
        case CLASS_ROGUE: return 512;
        case CLASS_PRIEST: return 536;
        case CLASS_DEATH_KNIGHT: return 515;
        case CLASS_SHAMAN: return 530;
        case CLASS_MAGE: return 545;
        case CLASS_WARLOCK: return 542;
        case CLASS_MONK: return 527;
        case CLASS_DRUID: return 549;
    }
    return 0;
}
#endif
