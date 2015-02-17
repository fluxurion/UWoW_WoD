/*
 * Copyright (C) 2011 TrintiyCore <http://www.trinitycore.org/>
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

#ifndef TRINITY_DB2STRUCTURE_H
#define TRINITY_DB2STRUCTURE_H

#include "Common.h"
#include "DBCEnums.h"
#include "Define.h"
#include "Path.h"
#include "Util.h"
//#include "Vehicle.h"
#include "SharedDefines.h"
#include "ItemPrototype.h"

#include <map>
#include <set>
#include <vector>

// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

// Structures used to access raw DB2 data and required packing to portability

#define MAX_BROADCAST_TEXT_EMOTES 3
#define MAX_HOLIDAY_DURATIONS 10
#define MAX_HOLIDAY_DATES 16
#define MAX_HOLIDAY_FLAGS 10

struct BroadcastTextEntry
{
    uint32 ID;
    int32 Language;
    LocalizedString* MaleText;
    LocalizedString* FemaleText;
    uint32 EmoteID[MAX_BROADCAST_TEXT_EMOTES];
    uint32 EmoteDelay[MAX_BROADCAST_TEXT_EMOTES];
    uint32 SoundID;
    uint32 UnkEmoteID;
    uint32 Type;
};

struct HolidaysEntry
{
    uint32      ID;                                         // 0
    uint32      Duration[MAX_HOLIDAY_DURATIONS];            // 1-10
    uint32      Date[MAX_HOLIDAY_DATES];                    // 11-26 (dates in unix time starting at January, 1, 2000)
    uint32      Region;                                     // 27
    uint32      Looping;                                    // 28
    uint32      CalendarFlags[MAX_HOLIDAY_FLAGS];           // 29-38
    //uint32    HolidayNameID;                              // 39 HolidayNames.dbc
    //uint32    HolidayDescriptionID;                       // 40 HolidayDescriptions.dbc
    char*       TextureFilename;                            // 41
    uint32      Priority;                                   // 42
    uint32      CalendarFilterType;                         // 43 (-1 = Fishing Contest, 0 = Unk, 1 = Darkmoon Festival, 2 = Yearly holiday)
    //uint32    Flags;                                      // 44 (0 = Darkmoon Faire, Fishing Contest and Wotlk Launch, rest is 1)
};

struct ItemEntry
{
    uint32      ID;                                         // 0
    uint32      Class;                                      // 1
    uint32      SubClass;                                   // 2
    int32       SoundOverrideSubclass;                      // 3
    int32       Material;                                   // 4
    uint32      InventoryType;                              // 5
    uint32      Sheath;                                     // 6
    uint32      FileDataID;                                 // 7
    uint32      GroupSoundsID;                              // 8
};

struct ItemAppearanceEntry
{
    uint32      ID;                                         // 0 (reference to ItemModifiedAppearance.db2?)
    uint32      DisplayID;                                  // 1
    uint32      FileDataID;                                 // 2
};


struct ItemCurrencyCostEntry
{
    //uint32  Id;
    uint32  ItemId;
};

struct ItemEffectEntry
{
    uint32      ID;                                         // 0
    uint32      ItemID;                                     // 1
    uint32      OrderIndex;                                 // 2
    uint32      SpellID;                                    // 3
    uint32      Trigger;                                    // 4
    uint32      Charges;                                    // 5
    int32       Cooldown;                                   // 6
    uint32      Category;                                   // 7
    int32       CategoryCooldown;                           // 8
};

struct ItemSparseEntry
{
    uint32      ID;                                         // 0
    uint32      Quality;                                    // 1
    uint32      Flags[MAX_ITEM_PROTO_FLAGS];                // 2-4
    float       Unk1;                                       // 5
    float       Unk2;                                       // 6
    uint32      BuyCount;                                   // 7
    uint32      BuyPrice;                                   // 8
    uint32      SellPrice;                                  // 9
    uint32      InventoryType;                              // 10
    int32       AllowableClass;                             // 11
    int32       AllowableRace;                              // 12
    uint32      ItemLevel;                                  // 13
    int32       RequiredLevel;                              // 14
    uint32      RequiredSkill;                              // 15
    uint32      RequiredSkillRank;                          // 16
    uint32      RequiredSpell;                              // 17
    uint32      RequiredHonorRank;                          // 18
    uint32      RequiredCityRank;                           // 19
    uint32      RequiredReputationFaction;                  // 20
    uint32      RequiredReputationRank;                     // 21
    uint32      MaxCount;                                   // 22
    uint32      Stackable;                                  // 23
    uint32      ContainerSlots;                             // 24
    int32       ItemStatType[MAX_ITEM_PROTO_STATS];         // 25 - 34
    uint32      ItemStatValue[MAX_ITEM_PROTO_STATS];        // 35 - 44
    int32       ItemStatUnk1[MAX_ITEM_PROTO_STATS];         // 45 - 54
    int32       ItemStatUnk2[MAX_ITEM_PROTO_STATS];         // 55 - 64
    uint32      ScalingStatDistribution;                    // 65
    uint32      DamageType;                                 // 66
    uint32      Delay;                                      // 67
    float       RangedModRange;                             // 68
    uint32      Bonding;                                    // 69
    char*       Name;                                  // 70
    char*       Name2;                                 // 71
    char*       Name3;                                 // 72
    char*       Name4;                                 // 73
    char*       Description;                           // 74
    uint32      PageText;                                   // 75
    uint32      LanguageID;                                 // 76
    uint32      PageMaterial;                               // 77
    uint32      StartQuest;                                 // 78
    uint32      LockID;                                     // 79
    int32       Material;                                   // 80
    uint32      Sheath;                                     // 81
    uint32      RandomProperty;                             // 82
    uint32      RandomSuffix;                               // 83
    uint32      ItemSet;                                    // 84
    uint32      Area;                                       // 85
    uint32      Map;                                        // 86
    uint32      BagFamily;                                  // 87
    uint32      TotemCategory;                              // 88
    uint32      SocketColor[MAX_ITEM_PROTO_SOCKETS];        // 89-91
    uint32      SocketBonus;                                // 92
    uint32      GemProperties;                              // 93
    float       ArmorDamageModifier;                        // 94
    uint32      Duration;                                   // 95
    uint32      ItemLimitCategory;                          // 96
    uint32      HolidayID;                                  // 97
    float       StatScalingFactor;                          // 98
    uint32      CurrencySubstitutionID;                     // 99
    uint32      CurrencySubstitutionCount;                  // 100
    uint32      ItemNameDescriptionID;                      // 101
};

#define MAX_ITEM_EXT_COST_ITEMS         5
#define MAX_ITEM_EXT_COST_CURRENCIES    5

struct ItemExtendedCostEntry
{
    uint32      ID;                                         // 0 extended-cost entry id
    uint32      RequiredHonorPoints;                        // 1 required honor points
    uint32      RequiredArenaPoints;                        // 2 required arena points
    uint32      RequiredArenaSlot;                          // 3 arena slot restrictions (min slot value)
    uint32      RequiredItem[MAX_ITEM_EXT_COST_ITEMS];      // 4-8 required item id
    uint32      RequiredItemCount[MAX_ITEM_EXT_COST_ITEMS]; // 9-13 required count of 1st item
    uint32      RequiredPersonalArenaRating;                // 14 required personal arena rating
    uint32      ItemPurchaseGroup;                          // 15
    uint32      RequiredCurrency[MAX_ITEM_EXT_COST_CURRENCIES];// 16-20 required curency id
    uint32      RequiredCurrencyCount[MAX_ITEM_EXT_COST_CURRENCIES];// 21-25 required curency count
    uint32      RequiredFactionId;
    uint32      RequiredFactionStanding;
    uint32      RequirementFlags;
    uint32      RequiredAchievement;

    bool IsSeasonCurrencyRequirement(uint32 i) const
    {
        if(i > MAX_ITEM_EXT_COST_CURRENCIES)
            return 0;

        // start from ITEM_EXTENDED_COST_FLAG_SEASON_IN_INDEX_0
        return RequirementFlags & (1 << (i + 1));
    }
};

struct BattlePetAbility
{
    uint32 ID;                  // 0
    uint32 Type;                // 1
    //uint32 someFlags;         // 2
    //uint32 unk;               // 3
    //uint32 unk;               // 4
    //uint32 unk;               // 5
    //string name               // 6
    //string description        // 7
};

struct BattlePetAbilityEffect
{
    uint32 ID;                  // 0
    uint32 AbilityID;           // 1
    //uint32 unk;               // 2
    //uint32 AuraID?;           // 3
    //uint32 unk;               // 4
    //uint32 effectNumber;      // 5
    //uint32 unk;               // 6
    //uint32 hitChance;         // 7
    //uint32 unk;               // 8
    //uint32 unk;               // 9
    //uint32 unk;               // 10
    //uint32 unk;               // 11
};

struct BattlePetAbilityState
{
    uint32 EffectID;            // 0
    uint32 AbilityID;           // 1
    uint32 stateID;             // 2
    //uint32 unk;               // 3
};

struct BattlePetState
{
    uint32 ID;                  // 0
    //uint32 unk;               // 1
    //string name;              // 2
    //uint32 unk;               // 3
};

struct BattlePetSpeciesEntry
{
    uint32 ID;                  // 0
    uint32 CreatureEntry;       // 1
    //uint32 fileDataEntry;     // 2
    uint32 spellId;             // 3
    //uint32 petType;           // 4
    uint32 source;              // 5
    uint32 flags;               // 6
    //string                    // 7
    //string                    // 8
};

struct QuestPackageItem
{
    uint32 ID;
    uint32 packageEntry;
    uint32 ItemID;
    uint32 count;
    //uint32 unk2;
};

struct LanguageWordsEntry
{
    //uint32 Id;                                            // 0
    uint32 langId;                                          // 1
    char* word;                                             // 2
};

typedef std::map<uint32 /*word length*/, std::vector<std::string> > LanguageWordsMap;
LanguageWordsMap const* GetLanguageWordMap(uint32 lang_id);
std::vector<std::string> const* GetLanguageWordsBySize(uint32 lang_id, uint32 size);

#define KEYCHAIN_SIZE   32

struct KeyChainEntry
{
    uint32      Id;
    uint8       Key[KEYCHAIN_SIZE];
};

#define MAX_OVERRIDE_SPELL 10

struct OverrideSpellDataEntry
{
    uint32      ID;                                         // 0
    uint32      SpellID[MAX_OVERRIDE_SPELL];                // 1-10
    //uint32    Flags;                                      // 11
    //char*     PlayerActionbarFileDataID;                  // 12
};

struct PhaseGroupEntry
{
    uint32      ID;
    uint32      PhaseID;
    uint32      PhaseGroupID;
};

struct SpellAuraRestrictionsEntry
{
    //uint32    ID;                                         // 0
    uint32      CasterAuraState;                            // 1
    uint32      TargetAuraState;                            // 2
    uint32      ExcludeCasterAuraState;                     // 3
    uint32      ExcludeTargetAuraState;                     // 4
    uint32      CasterAuraSpell;                            // 5
    uint32      TargetAuraSpell;                            // 6
    uint32      ExcludeCasterAuraSpell;                     // 7
    uint32      ExcludeTargetAuraSpell;                     // 8
};

struct SpellCastingRequirementsEntry
{
    //uint32    ID;                                         // 0
    uint32      FacingCasterFlags;                          // 1
    //uint32    MinFactionID;                               // 1
    //uint32    MinReputation;                              // 3
    uint32      RequiredAreasID;                            // 4
    //uint32    RequiredAuraVision;                         // 5
    uint32      RequiresSpellFocus;                         // 6
};

struct SpellClassOptionsEntry
{
    uint32      ID;                                         // 0
    uint32      ModalNextSpell;                             // 1
    flag128     SpellClassMask;                             // 2
    uint32      SpellClassSet;                              // 3
};

struct SpellMiscEntry
{
    uint32      ID;                                         // 0
    uint32      Attributes;                                 // 1
    uint32      AttributesEx;                               // 2
    uint32      AttributesEx2;                              // 3
    uint32      AttributesEx3;                              // 4
    uint32      AttributesEx4;                              // 5
    uint32      AttributesEx5;                              // 6
    uint32      AttributesEx6;                              // 7
    uint32      AttributesEx7;                              // 8
    uint32      AttributesEx8;                              // 9
    uint32      AttributesEx9;                              // 10
    uint32      AttributesEx10;                             // 11
    uint32      AttributesEx11;                             // 12
    uint32      AttributesEx12;                             // 13
    uint32      AttributesEx13;                             // 14
    uint32      CastingTimeIndex;                           // 15
    uint32      DurationIndex;                              // 16
    uint32      RangeIndex;                                 // 17
    float       Speed;                                      // 18
    uint32      SpellVisualID[2];                           // 19-20
    uint32      SpellIconID;                                // 21
    uint32      ActiveIconID;                               // 22
    uint32      SchoolMask;                                 // 23
    //float     Unk;                                        // 24
};

struct SpellPowerEntry
{
    uint32      ID;                                         // 0
    uint32      SpellID;                                    // 1
    uint32      PowerIndex;                                 // 2
    uint32      PowerType;                                  // 3
    uint32      PowerCost;                                  // 4
    uint32      PowerCostPerLevel;                          // 5
    uint32      PowerCostPerSecond;                         // 6
    //uint32    Unk4;                                       // 7 (All 0 except one spell: 22570)
    uint32    PowerDisplayId;                               // 8
    //uint32    Unk6;                                       // 9
    float       PowerCostPercentage;                        // 10
    float       PowerCostPercentagePerSecond;               // 11
    uint32      RequiredAura;                               // 12
    float       getpercentHp;                               // 13
};

#define MAX_SPELL_REAGENTS 8

// SpellReagents.dbc
struct SpellReagentsEntry
{
    //uint32    Id;                                           // 0        m_ID
    int32     Reagent[MAX_SPELL_REAGENTS];                  // 1-8      m_reagent
    uint32    ReagentCount[MAX_SPELL_REAGENTS];             // 9-16     m_reagentCount
    uint32    ReagentCurrency;                              // 17
    uint32    ReagentCurrencyCount;                         // 18
};

struct SpellRuneCostEntry
{
    uint32      ID;                                         // 0
    uint32      RuneCost[3];                                // 1-3 (0=blood, 1=frost, 2=unholy)
    //uint32    Unk;                                        // 4 (All 0 except for 2 ids: 2510, 2748)
    uint32      RunePowerGain;                              // 5

    bool NoRuneCost() const { return RuneCost[0] == 0 && RuneCost[1] == 0 && RuneCost[2] == 0; }
    bool NoRunicPowerGain() const { return RunePowerGain == 0; }
};

#define MAX_SPELL_TOTEMS 2

struct SpellTotemsEntry
{
    uint32      ID;                                         // 0
    uint32      TotemCategory[MAX_SPELL_TOTEMS];            // 1
    uint32      Totem[MAX_SPELL_TOTEMS];                    // 2
};

struct TaxiNodesEntry
{
    uint32          ID;                                     // 0
    uint32          MapID;                                  // 1
    DBCPosition3D   Pos;                                    // 2-4
    char*           Name_lang;                              // 5
    uint32          MountCreatureID[2];                     // 6-7
    uint32          ConditionID;                            // 8
    uint32          Flags;                                  // 9
    float           MapOffset[2];                           // 10-11
};

struct TaxiPathEntry
{
    uint32          ID;                                     // 0
    uint32          From;                                   // 1
    uint32          To;                                     // 2
    uint32          Cost;                                   // 3
};

struct TaxiPathNodeEntry
{
    uint32          ID;                                     // 0
    uint32          PathID;                                 // 1
    uint32          NodeIndex;                              // 2
    uint32          MapID;                                  // 3
    DBCPosition3D   Loc;                                    // 4-6
    uint32          Flags;                                  // 7
    uint32          Delay;                                  // 8
    uint32          ArrivalEventID;                         // 9
    uint32          DepartureEventID;                       // 10
};

struct ItemUpgradeEntry
{
    uint32 id;                  // 0 rules id from RuleSetItemUpgradeEntry startUpgrade
    uint32 itemUpgradePathId;   // 1 extended-cost entry id
    uint32 levelBonus;          // 2 total level bonus related to non-upgraded item
    uint32 prevUpgradeId;       // 3
    uint32 currencyReqId;       // 4 currency Id
    uint32 currencyReqAmt;      // 5 currency count
};

struct RuleSetItemUpgradeEntry
{
    uint32 id;                  // 0 m_ID
    uint32 levelUpd;            // 1 level upgrade
    uint32 startUpgrade;        // 2 start update rules for ItemUpgradeEntry
    uint32 itemEntry;           // 3 Item ID
};

#define GO_DBC_DATA_COUNT 8
struct GameObjectsEntry
{
    uint32 id;                  // 0 m_ID
    uint32 map;                 // 1 mapId
    uint32 displayId;           // 2
    float position_x;           // 3
    float position_y;           // 4
    float position_z;           // 5
    float rotation0;            // 6
    float rotation1;            // 7
    float rotation2;            // 8
    float rotation3;            // 9
    float size;                 // 10
    uint32 unk1;                // 11
    uint32 unk2;                // 12
    uint32 unk3;                // 13
    uint32 type;                // 14
    uint32 data[GO_DBC_DATA_COUNT];             // 15-22
    char* name;                 // 23
};

#define MAX_ITEM_UPDGRADES 3

struct ItemUpgradeData
{
    ItemUpgradeData()
    {
        memset(upgrade, 0, sizeof(upgrade));
    }

    ItemUpgradeEntry const* upgrade[MAX_ITEM_UPDGRADES];
};

struct MapChallengeModeEntry
{
    uint32 id;                  // 0
    uint32 map;                 // 1
    uint32 unk1;                // 2
    uint32 unk2;                // 3
    uint32 season;              // 4
    uint32 bronze;              // 5
    uint32 silver;              // 6
    uint32 gold;                // 7
    uint32 unk3;                // 8
    uint32 unk4;                // 9
};

struct SpellVisualEntry
{
    uint32 ID;                  // 0
    //uint32 unk;               // 1
    //uint32 unk;               // 2
    //uint32 unk?;              // 3
    //uint32 unk;               // 4
    //uint32 unk;               // 5
    //uint32 unk;               // 6
    //uint32 unk;               // 7
    //uint32 unk;               // 8
    //uint32 unk;               // 9
    //uint32 unk;               // 10
    //uint32 unk;               // 11
    //uint32 unk;               // 12
    //uint32 unk;               // 13
    //uint32 unk;               // 14
    //uint32 unk;               // 15
    //uint32 unk;               // 16
    //uint32 unk;               // 17
    //uint32 unk;               // 18
    //float unk;                // 19
    //float unk;                // 20
    //float unk;                // 21
    //float unk;                // 22
    //float unk;                // 23
    //float unk;                // 24
    uint32 hostileId;           // 25
    //uint32 unk;               // 26
    //uint32 unk;               // 27
    //uint32 unk;               // 28
    //uint32 unk;               // 29
};

// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif


typedef std::map<uint32, uint32> ItemDisplayIDMap;

struct TaxiPathBySourceAndDestination
{
    TaxiPathBySourceAndDestination() : ID(0), price(0) { }
    TaxiPathBySourceAndDestination(uint32 _id, uint32 _price) : ID(_id), price(_price) { }

    uint32    ID;
    uint32    price;
};
typedef std::map<uint32, TaxiPathBySourceAndDestination> TaxiPathSetForSource;
typedef std::map<uint32, TaxiPathSetForSource> TaxiPathSetBySource;

struct TaxiPathNodePtr
{
    TaxiPathNodePtr() : i_ptr(NULL) { }
    TaxiPathNodePtr(TaxiPathNodeEntry const* ptr) : i_ptr(ptr) { }
    TaxiPathNodeEntry const* i_ptr;
    operator TaxiPathNodeEntry const& () const { return *i_ptr; }
};

typedef Path<TaxiPathNodePtr, TaxiPathNodeEntry const> TaxiPathNodeList;
typedef std::vector<TaxiPathNodeList> TaxiPathNodesByPath;

#define TaxiMaskSize 114
typedef uint8 TaxiMask[TaxiMaskSize];

typedef std::unordered_map<uint32, std::set<uint32>> PhaseGroupContainer;
#endif