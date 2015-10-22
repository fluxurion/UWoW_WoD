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

#ifndef TRINITY_DBCSTORES_H
#define TRINITY_DBCSTORES_H

#include "Common.h"
#include "DBCStore.h"
#include "DBCStructure.h"
#include "SharedDefines.h"

#include <list>

uint32 GetAvailableMinorTalent(uint32 specID, int32 orderIndex);

typedef std::list<uint32> SimpleFactionsList;
SimpleFactionsList const* GetFactionTeamList(uint32 faction);
std::list<uint32> GetItemSpecsList(uint32 ItemID);
void AddSpecdtoItem(uint32 ItemID, uint32 SpecID);
uint32 GetLearnSpell(uint32 trigerSpell);
uint32 GetSpellByTrigger(uint32 trigerSpell);

std::vector<CriteriaTreeEntry const*> const* GetCriteriaTreeList(uint32 parent);
std::vector<ModifierTreeEntry const*> const* GetModifierTreeList(uint32 parent);
std::list<uint32> const* GetSpellProcsPerMinuteModList(uint32 PerMinId);
AchievementEntry const* GetsAchievementByTreeList(uint32 criteriaTree);

char const* GetPetName(uint32 petfamily, uint32 dbclang);
SpellEffectEntry const* GetSpellEffectEntry(uint32 spellId, uint32 effect, uint8 difficulty);
SpellEffectScalingEntry const* GetSpellEffectScalingEntry(uint32 effectId);
SpellTargetRestrictionsEntry const *GetSpellTargetRestrioctions(uint32 spellid, uint16 difficulty);

int32 GetAreaFlagByAreaID(uint32 area_id);                  // -1 if not found
AreaTableEntry const* GetAreaEntryByAreaID(uint32 area_id);
AreaTableEntry const* GetAreaEntryByAreaFlagAndMap(uint32 area_flag, uint32 map_id);
uint32 GetAreaFlagByMapId(uint32 mapid);

WMOAreaTableEntry const* GetWMOAreaTableEntryByTripple(int32 rootid, int32 adtid, int32 groupid);

uint32 GetVirtualMapForMapAndZone(uint32 mapid, uint32 zoneId);
int32 GetMapFromZone(uint32 zoneId);

std::string GetRandomCharacterName(uint8 race, uint8 gender);

enum ContentLevels
{
    CONTENT_1_60    = 0,
    CONTENT_61_70   = 1,
    CONTENT_71_80   = 2,
    CONTENT_81_85   = 3,
    CONTENT_86_90   = 4,
    MAX_CONTENT
};

uint32 GetMaxLevelForExpansion(uint32 expansion);
uint32 GetExpansionForLevel(uint32 level);

ContentLevels GetContentLevelsForMapAndZone(uint32 mapid, uint32 zoneId);

bool IsTotemCategoryCompatiableWith(uint32 itemTotemCategoryId, uint32 requiredTotemCategoryId);

void DeterminaAlternateMapPosition(uint32 mapId, float x, float y, float z, uint32* newMapId = nullptr, DBCPosition2D* newPos = nullptr);

void Zone2MapCoordinates(float &x, float &y, uint32 zone);
void Map2ZoneCoordinates(float &x, float &y, uint32 zone);

typedef std::unordered_map<uint32, std::unordered_map<uint32, MapDifficultyEntry const*>> MapDifficultyMap;
MapDifficultyEntry const* GetDefaultMapDifficulty(uint32 mapID);
MapDifficultyEntry const* GetMapDifficultyData(uint32 mapId, Difficulty difficulty);
MapDifficultyEntry const* GetDownscaledMapDifficultyData(uint32 mapId, Difficulty &difficulty);

uint32 GetLiquidFlags(uint32 liquidType);

PvPDifficultyEntry const* GetBattlegroundBracketByLevel(uint32 mapid, uint32 level);
PvPDifficultyEntry const* GetBattlegroundBracketById(uint32 mapid, BattlegroundBracketId id);

template<class T>
class GameTable
{
public:
    GameTable(char const* format) : _storage(format), _gtEntry(nullptr) { }

    void SetGameTableEntry(GameTablesEntry const* gtEntry) { _gtEntry = gtEntry; }

    T const* EvaluateTable(int32 row, int32 column) const
    {
        if (row > (int32)_gtEntry->NumRows || column > (int32)_gtEntry->NumColumns)
        {
            sLog->outU(" >> GameTable::EvaluateTable dbc %s request (row: %i, column: %i) max(row: %u, column: %u)", _gtEntry->Name, row, column, _gtEntry->NumRows, _gtEntry->NumColumns);
            return NULL;
        }
        ASSERT(row < (int32)_gtEntry->NumRows);
        ASSERT(column < (int32)_gtEntry->NumColumns);

        return _storage.LookupEntry(_gtEntry->NumRows * column + row);
    }

    inline bool HasEntry(uint32 row, uint32 column) const
    {
        return row < _gtEntry->NumRows && column < _gtEntry->NumColumns;
    }

    char const* GetFormat() const { return _storage.GetFormat(); }
    uint32 GetFieldCount() const { return _storage.GetFieldCount(); }
    bool Load(char const* fileName) { return _storage.Load(fileName, nullptr); }

    uint32 GetTableRowCount() const { return _gtEntry->NumRows; }
    uint32 GetTableColumnCount() const { return _gtEntry->NumColumns; }

private:
    DBCStorage<T> _storage;
    GameTablesEntry const* _gtEntry;
};

//extern DBCStorage<GtOCTRegenMPEntry>            sGtOCTRegenMPStore; -- not used currently
//extern DBCStorage<ItemDisplayInfoEntry>      sItemDisplayInfoStore; -- not used currently
//extern DBCStorage<StableSlotPricesEntry>        sStableSlotPricesStore;
extern CharacterLoadoutItemMap                  sCharacterLoadoutItemMap;
extern DBCStorage<AchievementEntry>             sAchievementStore;
extern DBCStorage<AreaPOIEntry>                 sAreaPOIStore;
extern DBCStorage<AreaTableEntry>               sAreaStore;// recommend access using functions
extern DBCStorage<AreaTriggerEntry>             sAreaTriggerStore;
extern DBCStorage<ArmorLocationEntry>           sArmorLocationStore;
extern DBCStorage<AuctionHouseEntry>            sAuctionHouseStore;
extern DBCStorage<BankBagSlotPricesEntry>       sBankBagSlotPricesStore;
extern DBCStorage<BannedAddOnsEntry>            sBannedAddOnsStore;
extern DBCStorage<BarberShopStyleEntry>         sBarberShopStyleStore;
extern DBCStorage<BattlemasterListEntry>        sBattlemasterListStore;
extern DBCStorage<CharStartOutfitEntry>         sCharStartOutfitStore;
extern DBCStorage<CharTitlesEntry>              sCharTitlesStore;
extern DBCStorage<ChatChannelsEntry>            sChatChannelsStore;
extern DBCStorage<ChrClassesEntry>              sChrClassesStore;
extern DBCStorage<ChrPowerTypesEntry>           sChrPowerTypesStore;
extern DBCStorage<ChrRacesEntry>                sChrRacesStore;
extern DBCStorage<ChrSpecializationsEntry>      sChrSpecializationsStore;
extern DBCStorage<CinematicSequencesEntry>      sCinematicSequencesStore;
extern DBCStorage<CreatureDisplayInfoEntry>     sCreatureDisplayInfoStore;
extern DBCStorage<CreatureDisplayInfoExtraEntry> sCreatureDisplayInfoExtraStore;
extern DBCStorage<CreatureFamilyEntry>          sCreatureFamilyStore;
extern DBCStorage<CreatureModelDataEntry>       sCreatureModelDataStore;
extern DBCStorage<CreatureSpellDataEntry>       sCreatureSpellDataStore;
extern DBCStorage<CreatureTypeEntry>            sCreatureTypeStore;
extern DBCStorage<CriteriaEntry>                sCriteriaStore;
extern DBCStorage<CriteriaTreeEntry>            sCriteriaTreeStore;
extern DBCStorage<DestructibleModelDataEntry>   sDestructibleModelDataStore;
extern DBCStorage<DifficultyEntry>              sDifficultyStore;
extern DBCStorage<DungeonEncounterEntry>        sDungeonEncounterStore;
extern DBCStorage<DurabilityCostsEntry>         sDurabilityCostsStore;
extern DBCStorage<DurabilityQualityEntry>       sDurabilityQualityStore;
extern DBCStorage<EmotesEntry>                  sEmotesStore;
extern DBCStorage<EmotesTextEntry>              sEmotesTextStore;
extern DBCStorage<FactionEntry>                 sFactionStore;
extern DBCStorage<FactionTemplateEntry>         sFactionTemplateStore;
extern DBCStorage<GameObjectDisplayInfoEntry>   sGameObjectDisplayInfoStore;
extern DBCStorage<GemPropertiesEntry>           sGemPropertiesStore;
extern DBCStorage<GlyphPropertiesEntry>         sGlyphPropertiesStore;
extern DBCStorage<GlyphSlotEntry>               sGlyphSlotStore;
extern DBCStorage<GtArmorMitigationByLvlEntry>  sGtArmorMitigationByLvlStore;
extern DBCStorage<GuildPerkSpellsEntry>         sGuildPerkSpellsStore;
extern DBCStorage<ImportPriceArmorEntry>        sImportPriceArmorStore;
extern DBCStorage<ImportPriceQualityEntry>      sImportPriceQualityStore;
extern DBCStorage<ImportPriceShieldEntry>       sImportPriceShieldStore;
extern DBCStorage<ImportPriceWeaponEntry>       sImportPriceWeaponStore;
extern DBCStorage<ItemArmorQualityEntry>        sItemArmorQualityStore;
extern DBCStorage<ItemArmorShieldEntry>         sItemArmorShieldStore;
extern DBCStorage<ItemArmorTotalEntry>          sItemArmorTotalStore;
extern DBCStorage<ItemBagFamilyEntry>           sItemBagFamilyStore;
extern DBCStorage<ItemClassEntry>               sItemClassStore;
extern DBCStorage<ItemDamageEntry>              sItemDamageAmmoStore;
extern DBCStorage<ItemDamageEntry>              sItemDamageOneHandCasterStore;
extern DBCStorage<ItemDamageEntry>              sItemDamageOneHandStore;
extern DBCStorage<ItemDamageEntry>              sItemDamageRangedStore;
extern DBCStorage<ItemDamageEntry>              sItemDamageThrownStore;
extern DBCStorage<ItemDamageEntry>              sItemDamageTwoHandCasterStore;
extern DBCStorage<ItemDamageEntry>              sItemDamageTwoHandStore;
extern DBCStorage<ItemDamageEntry>              sItemDamageWandStore;
extern DBCStorage<ItemDisenchantLootEntry>      sItemDisenchantLootStore;
extern DBCStorage<ItemLimitCategoryEntry>       sItemLimitCategoryStore;
extern DBCStorage<ItemPriceBaseEntry>           sItemPriceBaseStore;
extern DBCStorage<ItemRandomPropertiesEntry>    sItemRandomPropertiesStore;
extern DBCStorage<ItemRandomSuffixEntry>        sItemRandomSuffixStore;
extern DBCStorage<ItemReforgeEntry>             sItemReforgeStore;
extern DBCStorage<ItemSetEntry>                 sItemSetStore;
extern DBCStorage<ItemSetSpellEntry>            sItemSetSpellStore;
extern DBCStorage<ItemSpecEntry>                sItemSpecStore;
extern DBCStorage<ItemSpecOverrideEntry>        sItemSpecOverrideStore;
extern DBCStorage<LFGDungeonEntry>              sLFGDungeonStore;
extern DBCStorage<LiquidTypeEntry>              sLiquidTypeStore;
extern DBCStorage<LockEntry>                    sLockStore;
extern DBCStorage<MailTemplateEntry>            sMailTemplateStore;
extern DBCStorage<MapDifficultyEntry>           sMapDifficultyStore; // use GetMapDifficultyData instead
extern DBCStorage<MapEntry>                     sMapStore;
extern DBCStorage<MinorTalentEntry>             sMinorTalentStore;
extern DBCStorage<ModifierTreeEntry>            sModifierTreeStore;
extern DBCStorage<MountCapabilityEntry>         sMountCapabilityStore;
extern DBCStorage<MountTypeEntry>               sMountTypeStore;
extern DBCStorage<MovieEntry>                   sMovieStore;
extern DBCStorage<NameGenEntry>                 sNameGenStore;
extern DBCStorage<PhaseEntry>                   sPhaseStores;
extern DBCStorage<PowerDisplayEntry>            sPowerDisplayStore;
extern DBCStorage<QuestFactionRewEntry>         sQuestFactionRewardStore;
extern DBCStorage<QuestPOIBlobEntry>            sQuestPOIBlobStore;
extern DBCStorage<QuestPOIPointEntry>           sQuestPOIPointStore;
extern DBCStorage<QuestSortEntry>               sQuestSortStore;
extern DBCStorage<QuestXPEntry>                 sQuestXPStore;
extern DBCStorage<RandomPropertiesPointsEntry>  sRandomPropertiesPointsStore;
extern DBCStorage<ResearchBranchEntry>          sResearchBranchStore;
extern DBCStorage<ResearchProjectEntry>         sResearchProjectStore;
extern DBCStorage<ResearchSiteEntry>            sResearchSiteStore;
extern DBCStorage<ScalingStatDistributionEntry> sScalingStatDistributionStore;
extern DBCStorage<ScenarioEntry>                sScenarioStore;
extern DBCStorage<ScenarioStepEntry>            sScenarioStepStore;
extern DBCStorage<SkillLineAbilityEntry>        sSkillLineAbilityStore;
extern DBCStorage<SkillLineEntry>               sSkillLineStore;
extern DBCStorage<SkillRaceClassInfoEntry>      sSkillRaceClassInfoStore;
extern DBCStorage<SpecializationSpellEntry>     sSpecializationSpellStore;
extern DBCStorage<SpellAuraOptionsEntry>        sSpellAuraOptionsStore;
extern DBCStorage<SpellCastTimesEntry>          sSpellCastTimesStore;
extern DBCStorage<SpellCategoriesEntry>         sSpellCategoriesStore;
extern DBCStorage<SpellCategoryEntry>           sSpellCategoryStores;
extern DBCStorage<SpellCooldownsEntry>          sSpellCooldownsStore;
extern DBCStorage<SpellDurationEntry>           sSpellDurationStore;
extern DBCStorage<SpellEffectEntry>             sSpellEffectStore;
extern DBCStorage<SpellEffectScalingEntry>      sSpellEffectScalingStore;
extern DBCStorage<SpellEntry>                   sSpellStore;
extern DBCStorage<SpellEquippedItemsEntry>      sSpellEquippedItemsStore;
extern DBCStorage<SpellFocusObjectEntry>        sSpellFocusObjectStore;
extern DBCStorage<SpellInterruptsEntry>         sSpellInterruptsStore;
extern DBCStorage<SpellItemEnchantmentConditionEntry> sSpellItemEnchantmentConditionStore;
extern DBCStorage<SpellItemEnchantmentEntry>    sSpellItemEnchantmentStore;
extern DBCStorage<SpellLevelsEntry>             sSpellLevelsStore;
extern DBCStorage<SpellProcsPerMinuteEntry>     sSpellProcsPerMinuteStore;
extern DBCStorage<SpellProcsPerMinuteModEntry>  sSpellProcsPerMinuteModStore;
extern DBCStorage<SpellRadiusEntry>             sSpellRadiusStore;
extern DBCStorage<SpellRangeEntry>              sSpellRangeStore;
extern DBCStorage<SpellScalingEntry>            sSpellScalingStore;
extern DBCStorage<SpellShapeshiftEntry>         sSpellShapeshiftStore;
extern DBCStorage<SpellShapeshiftFormEntry>     sSpellShapeshiftFormStore;
extern DBCStorage<SpellTargetRestrictionsEntry> sSpellTargetRestrictionsStore;
extern DBCStorage<SummonPropertiesEntry>        sSummonPropertiesStore;
extern DBCStorage<TalentEntry>                  sTalentStore;
extern DBCStorage<TransportAnimationEntry>      sTransportAnimationStore;
extern DBCStorage<UnitPowerBarEntry>            sUnitPowerBarStore;
extern DBCStorage<VehicleEntry>                 sVehicleStore;
extern DBCStorage<VehicleSeatEntry>             sVehicleSeatStore;
extern DBCStorage<WMOAreaTableEntry>            sWMOAreaTableStore;
extern DBCStorage<WorldMapAreaEntry>            sWorldMapAreaStore; //-- use Zone2MapCoordinates and Map2ZoneCoordinates
extern DBCStorage<WorldMapOverlayEntry>         sWorldMapOverlayStore;
extern DBCStorage<WorldMapTransformsEntry>      sWorldMapTransformsStore;
extern DBCStorage<WorldSafeLocsEntry>           sWorldSafeLocsStore;

extern GameTable<GtBarberShopCostBaseEntry>     sGtBarberShopCostBaseStore;
extern GameTable<GtBattlePetTypeDamageModEntry> sGtBattlePetTypeDamageModStore;
extern GameTable<GtChanceToMeleeCritBaseEntry>  sGtChanceToMeleeCritBaseStore;
extern GameTable<GtChanceToMeleeCritEntry>      sGtChanceToMeleeCritStore;
extern GameTable<GtChanceToSpellCritBaseEntry>  sGtChanceToSpellCritBaseStore;
extern GameTable<GtChanceToSpellCritEntry>      sGtChanceToSpellCritStore;
extern GameTable<GtCombatRatingsEntry>          sGtCombatRatingsStore;
extern GameTable<GtItemSocketCostPerLevelEntry> sGtItemSocketCostPerLevelStore;
extern GameTable<GtNPCManaCostScalerEntry>      sGtNPCManaCostScalerStore;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp1Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp2Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp3Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp4Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp5Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpStore;
extern GameTable<GtOCTBaseHPByClassEntry>       sGtOCTBaseHPByClassStore;
extern GameTable<GtOCTBaseMPByClassEntry>       sGtOCTBaseMPByClassStore;
extern GameTable<GtOCTHpPerStaminaEntry>        sGtOCTHpPerStaminaStore;
extern GameTable<GtOCTLevelExperienceEntry>     sGtOCTLevelExperienceStore;
extern GameTable<GtRegenMPPerSptEntry>          sGtRegenMPPerSptStore;
extern GameTable<GtSpellScalingEntry>           sGtSpellScalingStore;

extern ItemSetSpellsStore                        sItemSetSpellsStore;
extern MapDifficultyMap                          sMapDifficultyMap;
extern PetFamilySpellsStore                      sPetFamilySpellsStore;
extern ResearchSiteDataMap                       sResearchSiteDataMap;
extern SpellCategoryStore                        sSpellCategoryStore;
extern SpellSkillingList                         sSpellSkillingList;
extern std::set<ResearchProjectEntry const*>     sResearchProjectSet;
extern std::set<uint32>                          sScenarioCriteriaTreeStore;
extern TransportAnimationsByEntry                sTransportAnimationsByEntry;

void LoadDBCStores(std::string const& dataPath, uint32 defaultLocale);
void LoadGameTables(std::string const& dataPath, uint32 defaultLocale);
void InitDBCCustomStores();
bool IsValidDifficulty(uint32 diff, bool isRaid);

uint32 GetQuestUniqueBitFlag(uint32 questId);
std::vector<uint32> GetItemLoadOutItems(uint32 LoadOutID);
AreaTableEntry const* FindAreaEntry(uint32 area);
DungeonEncounterEntry const* GetDungeonEncounterByDisplayID(uint32 displayID);


// SkillRaceClassInfo
typedef std::unordered_multimap<uint32, SkillRaceClassInfoEntry const*> SkillRaceClassInfoMap;
typedef std::pair<SkillRaceClassInfoMap::iterator, SkillRaceClassInfoMap::iterator> SkillRaceClassInfoBounds;
SkillRaceClassInfoEntry const* GetSkillRaceClassInfo(uint32 skill, uint8 race, uint8 class_);

std::vector<SpecializationSpellEntry const*> const* GetSpecializationSpells(uint32 specId);

typedef ChrSpecializationsEntry const* ChrSpecializationByIndexArray[MAX_CLASSES][4];
extern ChrSpecializationByIndexArray             sChrSpecializationByIndexStore;

typedef std::vector<TalentEntry const*> TalentsByPosition[MAX_CLASSES][7][3];
extern TalentsByPosition                         sTalentByPos;

#endif
