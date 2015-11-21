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

#define MAX_BROADCAST_TEXT_EMOTES 3
#define MAX_HOLIDAY_DURATIONS 10
#define MAX_HOLIDAY_DATES 16
#define MAX_HOLIDAY_FLAGS 10
#define MAX_ITEM_EXT_COST_ITEMS 5
#define MAX_ITEM_EXT_COST_CURRENCIES 5
#define MAX_EFFECT_PROPERTIES 6
#define KEYCHAIN_SIZE 32
#define MAX_OVERRIDE_SPELL 10
#define GO_DBC_DATA_COUNT 8
#define MAX_ITEM_UPDGRADES 5
#define MAX_SPELL_TOTEMS 2
#define MAX_SPELL_REAGENTS 8

#pragma pack(push, 1)

struct AreaGroupEntry
{
    uint32      ID;                                                 // 0
};

struct AreaGroupMemberEntry
{
    uint32      ID;                                                 // 0
    uint32      AreaGroupID;                                        // 1
    uint32      AreaID;                                             // 2
};

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

struct CurvePointEntry
{
    uint32 ID;                                                      // 0
    uint32 CurveID;                                                 // 1
    uint32 Index;                                                   // 2
    float X;                                                        // 3
    float Y;                                                        // 4
};

struct HeirloomEntry
{
    uint32 ID;                                                      // 0
    uint32 ItemID;                                                  // 1
    uint32 Flags;                                                   // 2
    LocalizedString* SourceText;                                    // 3
    uint32 Source;                                                  // 4
    uint32 OldItem[2];                                              // 5-6
    uint32 NextDifficultyItemID;                                    // 7
    uint32 UpgradeItemID[2];                                        // 8-9
    uint32 ItemBonusListID[2];                                      // 10-11
};

struct HolidaysEntry
{
    uint32      ID;                                         // 0
    uint32      Duration[MAX_HOLIDAY_DURATIONS];            // 1-10
    uint32      Date[MAX_HOLIDAY_DATES];                    // 11-26 (dates in unix time starting at January, 1, 2000)
    uint32      Region;                                     // 27
    uint32      Looping;                                    // 28
    uint32      CalendarFlags[MAX_HOLIDAY_FLAGS];           // 29-38
    uint32      HolidayNameID;                              // 39 HolidayNames.dbc
    uint32      HolidayDescriptionID;                       // 40 HolidayDescriptions.dbc
    LocalizedString* TextureFilename;                       // 41
    uint32      Priority;                                   // 42
    uint32      CalendarFilterType;                         // 43 (-1 = Fishing Contest, 0 = Unk, 1 = Darkmoon Festival, 2 = Yearly holiday)
    uint32      Flags;                                      // 44 (0 = Darkmoon Faire, Fishing Contest and Wotlk Launch, rest is 1)
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

struct ItemBonusEntry
{
    uint32      ID;                                                 // 0
    uint32      BonusListID;                                        // 1
    uint32      Type;                                               // 2
    int32       Value[2];                                           // 3-4
    uint32      Index;                                              // 5
};

struct ItemBonusTreeNodeEntry
{
    uint32 ID;                                                      // 0
    uint32 BonusTreeID;                                             // 1
    uint32 BonusTreeModID;                                          // 2
    uint32 SubTreeID;                                               // 3
    uint32 BonusListID;                                             // 4
};

struct ItemCurrencyCostEntry
{
    uint32      ID;                                                 // 0
    uint32      ItemId;                                             // 1
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

struct ItemModifiedAppearanceEntry
{
    uint32      ID;                                                 // 0
    uint32      ItemID;                                             // 1
    uint32      AppearanceModID;                                    // 2
    uint32      AppearanceID;                                       // 3
    uint32      IconFileDataID;                                     // 4
    uint32      Index;                                              // 5
};

struct ItemSparseEntry
{
    uint32      ID;                                                 // 0
    uint32      Quality;                                            // 1
    uint32      Flags[MAX_ITEM_PROTO_FLAGS];                        // 2-4
    float       Unk1;                                               // 5
    float       Unk2;                                               // 6
    uint32      BuyCount;                                           // 7
    uint32      BuyPrice;                                           // 8
    uint32      SellPrice;                                          // 9
    uint32      InventoryType;                                      // 10
    int32       AllowableClass;                                     // 11
    int32       AllowableRace;                                      // 12
    uint32      ItemLevel;                                          // 13
    int32       RequiredLevel;                                      // 14
    uint32      RequiredSkill;                                      // 15
    uint32      RequiredSkillRank;                                  // 16
    uint32      RequiredSpell;                                      // 17
    uint32      RequiredHonorRank;                                  // 18
    uint32      RequiredCityRank;                                   // 19
    uint32      RequiredReputationFaction;                          // 20
    uint32      RequiredReputationRank;                             // 21
    uint32      MaxCount;                                           // 22
    uint32      Stackable;                                          // 23
    uint32      ContainerSlots;                                     // 24
    int32       ItemStatType[MAX_ITEM_PROTO_STATS];                 // 25 - 34
    int32       ItemStatValue[MAX_ITEM_PROTO_STATS];                // 35 - 44
    int32       ItemStatAllocation[MAX_ITEM_PROTO_STATS];           // 45 - 54
    float       ItemStatSocketCostMultiplier[MAX_ITEM_PROTO_STATS]; // 55 - 64
    uint32      ScalingStatDistribution;                            // 65
    uint32      DamageType;                                         // 66
    uint32      Delay;                                              // 67
    float       RangedModRange;                                     // 68
    uint32      Bonding;                                            // 69
    LocalizedString* Name;                                          // 70
    LocalizedString* Name2;                                         // 71
    LocalizedString* Name3;                                         // 72
    LocalizedString* Name4;                                         // 73
    LocalizedString* Description;                                   // 74
    uint32      PageText;                                           // 75
    uint32      LanguageID;                                         // 76
    uint32      PageMaterial;                                       // 77
    uint32      StartQuest;                                         // 78
    uint32      LockID;                                             // 79
    int32       Material;                                           // 80
    uint32      Sheath;                                             // 81
    uint32      RandomProperty;                                     // 82
    uint32      RandomSuffix;                                       // 83
    uint32      ItemSet;                                            // 84
    uint32      Area;                                               // 85
    uint32      Map;                                                // 86
    uint32      BagFamily;                                          // 87
    uint32      TotemCategory;                                      // 88
    uint32      SocketColor[MAX_ITEM_PROTO_SOCKETS];                // 89-91
    uint32      SocketBonus;                                        // 92
    uint32      GemProperties;                                      // 93
    float       ArmorDamageModifier;                                // 94
    uint32      Duration;                                           // 95
    uint32      ItemLimitCategory;                                  // 96
    uint32      HolidayID;                                          // 97
    float       StatScalingFactor;                                  // 98
    uint32      CurrencySubstitutionID;                             // 99
    uint32      CurrencySubstitutionCount;                          // 100
    uint32      ItemNameDescriptionID;                              // 101
};

struct ItemExtendedCostEntry
{
    uint32      ID;                                                 // 0 extended-cost entry id
    uint32      RequiredArenaSlot;                                  // 3 arena slot restrictions (min slot value)
    uint32      RequiredItem[MAX_ITEM_EXT_COST_ITEMS];              // 3-6 required item id
    uint32      RequiredItemCount[MAX_ITEM_EXT_COST_ITEMS];         // 7-11 required count of 1st item
    uint32      RequiredPersonalArenaRating;                        // 12 required personal arena rating
    uint32      ItemPurchaseGroup;                                  // 13
    uint32      RequiredCurrency[MAX_ITEM_EXT_COST_CURRENCIES];     // 14-18 required curency id
    uint32      RequiredCurrencyCount[MAX_ITEM_EXT_COST_CURRENCIES];// 19-23 required curency count
    uint32      RequiredFactionId;                                  // 24
    uint32      RequiredFactionStanding;                            // 25
    uint32      RequirementFlags;                                   // 26
    uint32      RequiredAchievement;                                // 27
    uint32      RequiredMoney;                                      // 28

    bool IsSeasonCurrencyRequirement(uint32 i) const
    {
        if(i > MAX_ITEM_EXT_COST_CURRENCIES)
            return 0;

        // start from ITEM_EXTENDED_COST_FLAG_SEASON_IN_INDEX_0
        return RequirementFlags & (1 << (i + 1));
    }
};

struct BattlePetAbilityEntry
{
    uint32 ID;                  // 0
    uint32 Type;                // 1
    uint32 fileDataEntry;     // 2
    uint32 turnCooldown;        // 3
    uint32 auraAbilityID;       // 4
    uint32 auraDuration;        // 5
    LocalizedString* name;               // 6
    LocalizedString* description;        // 7
};

struct BattlePetAbilityEffectEntry
{
    uint32 ID;                  // 0
    uint32 TurnEntryID;         // 1
    uint32 unk;               // 2
    uint32 AuraId_unk;           // 3
    uint32 propertiesID;        // 4
    uint32 effectIndex;         // 5
    uint32 propertyValues[MAX_EFFECT_PROPERTIES];   // 6 - 11
};

struct BattlePetEffectPropertiesEntry
{
    uint32 ID;                  // 0
    uint32 someFlags;         // 1
    LocalizedString* propertyDescs[MAX_EFFECT_PROPERTIES];     // 2 - 7
    uint32 unk;               // 8
    uint32 unk2;               // 9
    uint32 unk3;               // 10
    uint32 unk4;               // 11
    uint32 unk5;               // 12
    uint32 unk6;               // 13
};

struct BattlePetAbilityTurnEntry
{
    uint32 ID;                  // 0
    uint32 AbilityID;           // 1
    uint32 stateID;           // 2
    uint32 turnIndex;           // 3
    uint32 unk;              // 4
    uint32 procIndex;        // 5
};

struct BattlePetAbilityStateEntry
{
    uint32 ID;                  // 0
    uint32 AbilityID;         // 1
    uint32 stateID;           // 2
    uint32 unk;               // 3
};

struct BattlePetStateEntry
{
    uint32 ID;                  // 0
    uint32 unk;               // 1
    LocalizedString* stateName;            // 2
    uint32 flags;               // 3
};

struct BattlePetSpeciesEntry
{
    uint32 ID;                  // 0
    uint32 CreatureEntry;       // 1
    uint32 IconFileID;          // 2
    uint32 spellId;             // 3
    uint32 petType;             // 4
    int32  source;              // 5
    uint32 flags;               // 6
    LocalizedString* SourceText; // 7
    LocalizedString* Description; // 8
};

struct BattlePetSpeciesStateEntry
{
    uint32 ID;                                                      // 0
    uint32 SpeciesID;                                               // 1
    uint32 State;                                                   // 2
    int32 Value;                                                    // 3
};

struct BattlePetSpeciesXAbilityEntry
{
    uint32 ID;
    uint32 speciesID;
    uint32 abilityID;
    uint32 requiredLevel;
    uint32 rank;
};

struct BattlePetBreedQualityEntry
{
    uint32 ID;
    uint32 quality;
    float qualityModifier;
};

struct BattlePetBreedStateEntry
{
    uint32 ID;
    uint32 breedID;
    uint32 stateID;
    int32 stateModifier;
};

struct GarrAbilityEntry
{
    uint32 ID;                                                      // 0
    uint32 Flags;                                                   // 1
    LocalizedString* Name;                                          // 2
    LocalizedString* Description;                                   // 3
    uint32 IconFileDataID;                                          // 4
    uint32 OtherFactionGarrAbilityID;                               // 5
    uint32 GarrAbilityCategoryID;                                   // 6
};

struct GarrAbilityEffectEntry
{
    uint32 ID;                  // 0
    uint32 Flags;               // 1
    uint32 GarrAbilityID;       // 2
    uint32 Unk1;                // 3
    uint32 GarrMechanicTypeID;  // 4
    uint32 Unk3;                // 5
    float Amount;               // 6
    float Amount2;              // 7
    float Amount3;              // 8
    uint32 Unk4;                // 9
    uint32 Unk5;                // 10
};

struct GarrBuildingEntry
{
    uint32 ID;                                                      // 0
    uint32 HordeGameObjectID;                                       // 1
    uint32 AllianceGameObjectID;                                    // 2
    uint32 Unknown;                                                 // 3
    uint32 Type;                                                    // 4
    uint32 Level;                                                   // 5
    LocalizedString* NameAlliance;                                  // 6
    LocalizedString* NameHorde;                                     // 7
    LocalizedString* Description;                                   // 8
    LocalizedString* Tooltip;                                       // 9
    uint32 BuildDuration;                                           // 10
    uint32 CostCurrencyID;                                          // 11
    int32  CostCurrencyAmount;                                      // 12
    uint32 HordeTexPrefixKitID;                                     // 13
    uint32 AllianceTexPrefixKitID;                                  // 14
    uint32 IconFileDataID;                                          // 15
    uint32 BonusAmount;                                             // 16
    uint32 Flags;                                                   // 17
    uint32 AllianceActivationScenePackageID;                        // 18
    uint32 HordeActivationScenePackageID;                           // 19
    uint32 MaxShipments;                                            // 20
    uint32 FollowerRequiredGarrAbilityID;                           // 21
    uint32 FollowerGarrAbilityEffectID;                             // 22
    int32  CostMoney;                                               // 23
};

struct GarrBuildingPlotInstEntry
{
    uint32 ID;                                                      // 0
    uint32 GarrBuildingID;                                          // 1
    uint32 UiTextureAtlasMemberID;                                  // 2
    uint32 GarrSiteLevelPlotInstID;                                 // 3
    DBCPosition2D LandmarkOffset;                                   // 4-5
};

struct GarrClassSpecEntry
{
    uint32 ID;                                                      // 0
    LocalizedString* NameMale;                                      // 1
    LocalizedString* NameFemale;                                    // 2
    LocalizedString* NameGenderless;                                // 3
    uint32 ClassAtlasID;                                            // 4 UiTextureAtlasMember.db2 ref
    uint32 GarrFollItemSetID;                                       // 5
};

struct GarrFollowerEntry
{
    uint32 ID;                                                      // 0
    uint32 HordeCreatureID;                                         // 1
    uint32 AllianceCreatureID;                                      // 2
    uint32 HordeUiAnimRaceInfoID;                                   // 3
    uint32 AllianceUiAnimRaceInfoID;                                // 4
    uint32 Quality;                                                 // 5
    uint32 HordeGarrClassSpecID;                                    // 6
    uint32 AllianceGarrClassSpecID;                                 // 7
    uint32 HordeGarrFollItemSetID;                                  // 8
    uint32 AllianceGarrFollItemSetID;                               // 9
    uint32 Level;                                                   // 10
    uint32 ItemLevelWeapon;                                         // 11
    uint32 ItemLevelArmor;                                          // 12
    uint32 Unknown1;                                                // 13
    uint32 Flags;                                                   // 14
    LocalizedString* HordeSourceText;                               // 15
    LocalizedString* AllianceSourceText;                            // 16
    int32 Unknown2;                                                 // 17
    int32 Unknown3;                                                 // 18
    uint32 HordePortraitIconID;                                     // 19
    uint32 AlliancePortraitIconID;                                  // 20
};

struct GarrFollowerLevelXPEntry
{
    uint32 ID;
    uint32 Level;
    uint32 NextLevelXP;
    uint32 Unk;
};

struct GarrFollowerQualityEntry
{
    uint32 ID;
    uint32 Quality;
    uint32 NextQualityXP;
    uint32 Unk;
};

struct GarrFollowerXAbilityEntry
{
    uint32 ID;                                                      // 0
    uint32 GarrFollowerID;                                          // 1
    uint32 GarrAbilityID;                                           // 2
    uint32 FactionIndex;                                            // 3
};

struct GarrEncounterEntry
{
    uint32 ID;                                                      // 0
    uint32 CreatureEntry;                                           // 1
    LocalizedString* Name;                                          // 2
    float Mod;                                                      // 3
    float Mod1;                                                     // 4
    uint32 SomeFileDataID;                                          // 5
};

struct GarrEncounterXMechanicEntry
{
    uint32 ID;                  // 0
    uint32 GarrEncounterID;     // 1
    uint32 GarrMechanicID;      // 2
};

struct GarrMechanicEntry
{
    uint32 ID;                  // 0
    uint32 Type;                // 1 (GarrMechanicTypeID)
    float Mod;                  // 2
};

struct GarrMechanicTypeEntry
{
    uint32 ID;
    uint32 Category;
    LocalizedString* Name;
    LocalizedString* Description;
    uint32 SomeFileDataID;
};

struct GarrMissionEntry
{
    uint32 ID;                    // 0
    uint32 ReqLevel;              // 1
    uint32 ReqFollowersItemLevel; // 2
    uint32 GarrMechanicTypeID;    // 3
    uint32 ReqFollowersCount;     // 4
    uint32 Unk1;                  // 5
    uint32 MissionDuration;       // 6
    uint32 OfferDuration;         // 7
    uint32 Unk2;                  // 8
    uint32 GarrMissionTypeID;     // 9
    LocalizedString* Name;        // 10
    LocalizedString* Description; // 11
    LocalizedString* Location;    // 12
    uint32 CriteriaID;            // 13
    uint32 Unk3;                  // 14
    uint32 ReqResourcesCount;     // 15
    uint32 Unk4;                  // 16
    uint32 BaseXP;                // 17
    uint32 BaseChance;            // 18
};

struct GarrMissionRewardEntry
{
    uint32 ID;                  // 0
    uint32 MissionID;           // 1
    uint32 RewardXP;            // 2
    uint32 RewardItemID;        // 3
    uint32 ItemAmount;          // 4
    uint32 CurrencyID;          // 5
    uint32 CurrencyValue;       // 6
    uint32 Unk2;                // 7
    uint32 Unk3;                // 8
    uint32 Unk4;                // 9

    bool HasItemReward() const { return RewardItemID != 0; }
    bool HasMoneyReward() const { return CurrencyID == 0 && CurrencyValue > 0; }
    bool HasCurrencyReward() const { return CurrencyID != 0 && CurrencyValue > 0; }
    bool HasFollowerXPReward() const { return RewardXP > 0; }
};

struct GarrMissionXEncounterEntry
{
    uint32 ID;                  // 0
    uint32 GarrMissionID;       // 1
    uint32 GarrEncounterID;     // 2
};

struct GarrPlotEntry
{
    uint32 ID;                                                      // 0
    uint32 GarrPlotUICategoryID;                                    // 1
    uint32 PlotType;                                                // 2
    uint32 Flags;                                                   // 3
    LocalizedString* Name;                                          // 4
    uint32 MinCount;                                                // 5
    uint32 MaxCount;                                                // 6
    uint32 AllianceConstructionGameObjectID;                        // 7
    uint32 HordeConstructionGameObjectID;                           // 8
};

struct GarrPlotBuildingEntry
{
    uint32 ID;                                                      // 0
    uint32 GarrPlotID;                                              // 1
    uint32 GarrBuildingID;                                          // 2
};

struct GarrPlotInstanceEntry
{
    uint32 ID;                                                      // 0
    uint32 GarrPlotID;                                              // 1
    LocalizedString* Name;                                          // 2
};

struct GarrSiteLevelEntry
{
    uint32 ID;                                                      // 0
    uint32 Level;                                                   // 1
    uint32 MapID;                                                   // 2
    uint32 SiteID;                                                  // 3
    uint32 UITextureKitID;                                          // 4
    DBCPosition2D TownHall;                                         // 5-6
    uint32 MovieID;                                                 // 7
    uint32 Level2;                                                  // 8
    int32 UpgradeResourceCost;                                      // 9
    int32 UpgradeMoneyCost;                                         // 10
};

struct GarrSiteLevelPlotInstEntry
{
    uint32 ID;                                                       // 0
    uint32 GarrSiteLevelID;                                          // 1
    uint32 GarrPlotInstanceID;                                       // 2
    DBCPosition2D Landmark;                                          // 3-4
    uint32 Unknown;                                                  // 5
};

struct CreatureDifficultyEntry
{
    uint32      ID;
    uint32      CreatureID;
    uint32      FactionID;
    uint32      Expansion;
    uint32      MinLevel;
    uint32      MaxLevel;
    uint32      Flags[5];
};

struct CurrencyTypesEntry
{
    uint32      ID;                                                 // 0
    uint32      CategoryID;                                         // 1
    LocalizedString*       Name_lang;                                          // 2
    LocalizedString*       InventoryIcon[2];                                   // 3-4
    uint32      SpellWeight;                                        // 5
    uint32      SpellCategory;                                      // 6
    uint32      MaxQty;                                             // 7
    uint32      MaxEarnablePerWeek;                                 // 8
    uint32      Flags;                                              // 9
    uint32      Quality;                                            // 10
    LocalizedString*       Description_lang;                                   // 11

    bool HasPrecision() const   { return Flags & CURRENCY_FLAG_HAS_PRECISION; }
    bool HasSeasonCount() const { return Flags & CURRENCY_FLAG_HAS_SEASON_COUNT; }
    float GetPrecision() const  { return HasPrecision() ? CURRENCY_PRECISION : 1.0f; }
};

struct QuestPackageItemEntry
{
    uint32 ID;                                                      // 0
    uint32 QuestPackageID;                                          // 1
    uint32 ItemID;                                                  // 2
    uint32 ItemCount;                                               // 3
    uint32 FilterType;                                              // 4
};

struct MountEntry
{
    uint32 Id;
    uint32 MountTypeId;
    uint32 DisplayId;
    uint32 Flags;
    LocalizedString* Name;
    LocalizedString* Description;
    LocalizedString* SourceDescription;
    uint32 Source;
    uint32 SpellId;
    uint32 PlayerConditionId;
};

/*struct MountCapabilityEntry
{
    uint32 ID;                                                      // 0
    uint32 Flags;                                                   // 1
    uint32 RequiredRidingSkill;                                     // 2
    uint32 RequiredArea;                                            // 3
    uint32 RequiredAura;                                            // 4
    uint32 RequiredSpell;                                           // 5
    uint32 SpeedModSpell;                                           // 6
    int32  RequiredMap;                                             // 7
};*/

struct MountTypeXCapabilityEntry
{
    uint32 ID;                                                      // 0
    uint32 MountTypeID;                                             // 1
    uint32 OrderIndex;                                              // 2
    uint32 MountCapabilityID;                                       // 3
};

struct LanguageWordsEntry
{
    uint32 Id;                                            // 0
    uint32 langId;                                          // 1
    LocalizedString* word;                                             // 2
};

struct KeyChainEntry
{
    uint32      Id;
    uint8       Key[KEYCHAIN_SIZE];
};

struct OverrideSpellDataEntry
{
    uint32 ID;                                                      // 0
    uint32 SpellID[MAX_OVERRIDE_SPELL];                             // 1-10
    uint32 Flags;                                                   // 11
    uint32 PlayerActionbarFileDataID;                               // 12
};

struct PhaseGroupEntry
{
    uint32      ID;
    uint32      PhaseID;
    uint32      PhaseGroupID;
};

struct SoundEntriesEntry
{
    uint32      ID;                                                 // 0
    uint32      SoundType;                                          // 1
    LocalizedString* Name;                                          // 2
    uint32      FileDataID[20];                                     // 3-22
    uint32      Freq[20];                                           // 23-42
    float       VolumeFloat;                                        // 43
    uint32      Flags;                                              // 44
    float       MinDistance;                                        // 45
    float       DistanceCutoff;                                     // 46
    uint32      EAXDef;                                             // 47
    uint32      SoundEntriesAdvancedID;                             // 48
    float       VolumeVariationPlus;                                // 49
    float       VolumeVariationMinus;                               // 50
    float       PitchVariationPlus;                                 // 51
    float       PitchVariationMinus;                                // 52
    float       PitchAdjust;                                        // 53
    uint32      DialogType;                                         // 54
    uint32      BusOverwriteID;                                     // 55
};

struct SpellAuraRestrictionsEntry
{
    uint32      ID;                                         // 0
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
    uint32      ID;                                         // 0
    uint32      FacingCasterFlags;                          // 1
    uint32      MinFactionID;                               // 1
    uint32      MinReputation;                              // 3
    uint32      RequiredAreasID;                            // 4
    uint32      RequiredAuraVision;                         // 5
    uint32      RequiresSpellFocus;                         // 6
};

struct SpellClassOptionsEntry
{
    uint32      ID;                                         // 0
    uint32      ModalNextSpell;                             // 1
    flag128     SpellClassMask;                             // 2
    uint32      SpellClassSet;                              // 3
};

struct SpellLearnSpellEntry
{
    uint32      ID;                                         // 0
    uint32      LearnSpellID;                               // 1
    uint32      SpellID;                                    // 2
    uint32      OverridesSpellID;                           // 3
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
    float       MultistrikeSpeedMod;                        // 24
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
    uint32      ManaCostAdditional;                         // 7 (All 0 except one spell: 22570)
    uint32      PowerDisplayId;                             // 8
    uint32      UnitPowerBarID;                             // 9
    float       PowerCostPercentage;                        // 10
    float       PowerCostPercentagePerSecond;               // 11
    uint32      RequiredAura;                               // 12
    float       HealthCostPercentage;                       // 13
};

struct SpellReagentsEntry
{
    uint32      Id;                                         // 0
    int32       Reagent[MAX_SPELL_REAGENTS];                // 1-8
    uint32      ReagentCount[MAX_SPELL_REAGENTS];           // 9-16
    uint32      ReagentCurrency;                            // 17
    uint32      ReagentCurrencyCount;                       // 18
};

struct SpellRuneCostEntry
{
    uint32      ID;                                         // 0
    uint32      RuneCost[4];                                // 1-4 (0=blood, 1=frost, 2=unholy)
    uint32      RunePowerGain;                              // 5

    bool NoRuneCost() const { return RuneCost[0] == 0 && RuneCost[1] == 0 && RuneCost[2] == 0; }
    bool NoRunicPowerGain() const { return RunePowerGain == 0; }
};

struct ToyEntry
{
    uint32 ID;                                                      // 0
    uint32 ItemID;                                                  // 1
    uint32 Flags;                                                   // 2
    LocalizedString* Description;                                   // 3
    uint32 CategoryFilter;                                          // 4
};

struct PvpItemEntry
{
    uint32 ID;
    uint32 ItemID;
    uint32 BonusIlvl;
};

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
    LocalizedString* Name_lang;                             // 5
    uint32          MountCreatureID[MAX_TEAMS];           // 6-7
    uint32          ConditionID;                            // 8
    uint32          LearnableIndex;                         // 9 - some kind of index only for learnable nodes
    uint32          Flags;                                  // 10
    float           MapOffset[MAX_TEAMS];                 // 11-12
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

struct GameObjectsEntry
{
    uint32 ID;                                                      // 0
    uint32 MapID;                                                   // 1
    uint32 DisplayID;                                               // 2
    DBCPosition3D Position;                                         // 3-5
    float RotationX;                                                // 6
    float RotationY;                                                // 7
    float RotationZ;                                                // 8
    float RotationW;                                                // 9
    float Size;                                                     // 10
    uint32 PhaseUseFlags;                                           // 11
    uint32 PhaseID;                                                 // 12
    uint32 PhaseGroupID;                                            // 13
    uint32 Type;                                                    // 14
    uint32 Data[GO_DBC_DATA_COUNT];                                 // 15-22
    LocalizedString* Name;                                          // 23
};

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
    uint32 unk;               // 1
    uint32 unk2;               // 2
    uint32 unk3;              // 3
    uint32 unk4;               // 4
    uint32 unk5;               // 5
    uint32 unk6;               // 6
    uint32 unk7;               // 7
    uint32 unk8;               // 8
    uint32 unk9;               // 9
    uint32 unk10;               // 10
    uint32 unk11;               // 11
    uint32 unk12;               // 12
    uint32 unk13;               // 13
    uint32 unk14;               // 14
    uint32 unk15;               // 15
    uint32 unk16;               // 16
    uint32 unk17;               // 17
    uint32 unk18;               // 18
    float unk19;                // 19
    float unk20;                // 20
    float unk21;                // 21
    float unk22;                // 22
    float unk23;                // 23
    float unk24;                // 24
    uint32 hostileId;           // 25
    uint32 unk25;               // 26
    uint32 unk26;               // 27
    uint32 unk27;               // 28
    uint32 unk28;               // 29
};

struct ItemXBonusTreeEntry
{
    uint32 ID;                                                      // 0
    uint32 ItemID;                                                  // 1
    uint32 BonusTreeID;                                             // 2
};

struct ItemToBattlePetSpeciesEntry
{
    uint32      ID;                                                 // 0
    uint32      BattlePetSpeciesID;                                 // 1
};

#pragma pack(pop)
   
struct TaxiPathBySourceAndDestination
{
    TaxiPathBySourceAndDestination() : ID(0), price(0) { }
    TaxiPathBySourceAndDestination(uint32 _id, uint32 _price) : ID(_id), price(_price) { }

    uint32    ID;
    uint32    price;
};

typedef std::map<uint32, TaxiPathBySourceAndDestination> TaxiPathSetForSource;
typedef std::map<uint32, TaxiPathSetForSource> TaxiPathSetBySource;

typedef std::vector<TaxiPathNodeEntry const*> TaxiPathNodeList;
typedef std::vector<TaxiPathNodeList> TaxiPathNodesByPath;

#define TaxiMaskSize 217
typedef std::array<uint8, TaxiMaskSize> TaxiMask;

#endif