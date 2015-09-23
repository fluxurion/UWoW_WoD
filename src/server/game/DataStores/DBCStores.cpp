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

#include "DB2Stores.h"
#include "DBCStores.h"
#include "Log.h"
#include "SharedDefines.h"
#include "SpellMgr.h"
#include "DBCfmt.h"
#include "ItemTemplate.h"
#include <iostream>
#include <fstream>

#include <map>

typedef std::map<uint16, uint32> AreaFlagByAreaID;
typedef std::map<uint32, uint32> AreaFlagByMapID;
typedef std::map<uint32, AreaTableEntry const* > AreaEntryMap;

struct WMOAreaTableTripple
{
    WMOAreaTableTripple(int32 r, int32 a, int32 g) : groupId(g), rootId(r), adtId(a)
    { }

    bool operator <(const WMOAreaTableTripple& b) const
    {
        return memcmp(this, &b, sizeof(WMOAreaTableTripple)) < 0;
    }

    // ordered by entropy; that way memcmp will have a minimal medium runtime
    int32 groupId;
    int32 rootId;
    int32 adtId;
};


//DBCStorage<AchievementCriteriaEntry> sAchievementCriteriaStore(AchievementCriteriafmt);
//DBCStorage<GtOCTRegenMPEntry>            sGtOCTRegenMPStore(GtOCTRegenMPfmt);  -- not used currently
//DBCStorage<ItemDisplayInfoEntry>       sItemDisplayInfoStore(ItemDisplayTemplateEntryfmt); -- not used currently
//std::map<uint32,

typedef std::map<uint32, SimpleFactionsList> FactionTeamMap;
typedef std::map<WMOAreaTableTripple, WMOAreaTableEntry const*> WMOAreaInfoByTripple;

static std::unordered_map<uint32, std::vector<CriteriaTreeEntry const*>> sCriteriaTreeList;
static std::unordered_map<uint32, std::vector<ModifierTreeEntry const*>> sModifierTreeList;
static std::unordered_map<uint32, std::list<uint32>> sSpellProcsPerMinuteModEntryList;
static std::unordered_map<uint32, AchievementEntry const*> sAchievementParentList;
static std::unordered_map<uint32, std::list<uint32>> sItemSpecsList;
static std::unordered_map<uint32, uint32> sRevertLearnSpellList;
static std::unordered_map<uint32, uint32> sReversTriggerSpellList;
static AreaEntryMap sAreaEntry;
static AreaFlagByAreaID sAreaFlagByAreaID;
static AreaFlagByMapID sAreaFlagByMapID;
static FactionTeamMap sFactionTeamMap;
static WMOAreaInfoByTripple sWMOAreaInfoByTripple;


CharacterLoadoutItemMap                     sCharacterLoadoutItemMap;
ItemSetSpellsStore                          sItemSetSpellsStore;
MapDifficultyMap                            sMapDifficultyMap;
NameGenVectorArraysMap                      sGenNameVectoArraysMap;
PetFamilySpellsStore                        sPetFamilySpellsStore;
ResearchSiteDataMap                         sResearchSiteDataMap;
SpellCategoryStore                          sSpellCategoryStore;
SpellEffectDiffMap                          sSpellEffectDiffMap;
SpellEffectMap                              sSpellEffectMap;
SpellRestrictionDiffMap                     sSpellRestrictionDiffMap;
SpellSkillingList                           sSpellSkillingList;
std::set<ResearchProjectEntry const*>       sResearchProjectSet;
std::set<uint32>                            sScenarioCriteriaTreeStore;
TalentSpellList                             sTalentSpellList;
TransportAnimationsByEntry                  sTransportAnimationsByEntry;

DBCStorage<AchievementEntry>                sAchievementStore(Achievementfmt);
DBCStorage<AreaPOIEntry>                    sAreaPOIStore(AreaPOIEntryfmt);
DBCStorage<AreaTableEntry>                  sAreaStore(AreaTableEntryfmt);
DBCStorage<AreaTriggerEntry>                sAreaTriggerStore(AreaTriggerEntryfmt);
DBCStorage<ArmorLocationEntry>              sArmorLocationStore(ArmorLocationfmt);
DBCStorage<AuctionHouseEntry>               sAuctionHouseStore(AuctionHouseEntryfmt);
DBCStorage<BankBagSlotPricesEntry>          sBankBagSlotPricesStore(BankBagSlotPricesEntryfmt);
DBCStorage<BannedAddOnsEntry>               sBannedAddOnsStore(BannedAddOnsfmt);
DBCStorage<BarberShopStyleEntry>            sBarberShopStyleStore(BarberShopStyleEntryfmt);
DBCStorage<BattlemasterListEntry>           sBattlemasterListStore(BattlemasterListEntryfmt);
DBCStorage<CharacterLoadoutItemEntry>       sCharacterLoadoutItemStore(CharacterLoadoutItemfmt);
DBCStorage<CharStartOutfitEntry>            sCharStartOutfitStore(CharStartOutfitEntryfmt);
DBCStorage<CharTitlesEntry>                 sCharTitlesStore(CharTitlesEntryfmt);
DBCStorage<ChatChannelsEntry>               sChatChannelsStore(ChatChannelsEntryfmt);
DBCStorage<ChrClassesEntry>                 sChrClassesStore(ChrClassesEntryfmt);
DBCStorage<ChrPowerTypesEntry>              sChrPowerTypesStore(ChrClassesXPowerTypesfmt);
DBCStorage<ChrRacesEntry>                   sChrRacesStore(ChrRacesEntryfmt);
DBCStorage<ChrSpecializationsEntry>         sChrSpecializationsStore(ChrSpecializationsfmt);
DBCStorage<CinematicSequencesEntry>         sCinematicSequencesStore(CinematicSequencesEntryfmt);
DBCStorage<CreatureDisplayInfoEntry>        sCreatureDisplayInfoStore(CreatureDisplayInfofmt);
DBCStorage<CreatureDisplayInfoExtraEntry>   sCreatureDisplayInfoExtraStore(CreatureDisplayInfoExtrafmt);
DBCStorage<CreatureFamilyEntry>             sCreatureFamilyStore(CreatureFamilyfmt);
DBCStorage<CreatureModelDataEntry>          sCreatureModelDataStore(CreatureModelDatafmt);
DBCStorage<CreatureSpellDataEntry>          sCreatureSpellDataStore(CreatureSpellDatafmt);
DBCStorage<CreatureTypeEntry>               sCreatureTypeStore(CreatureTypefmt);
DBCStorage<CriteriaEntry>                   sCriteriaStore(Criteriafmt);
DBCStorage<CriteriaTreeEntry>               sCriteriaTreeStore(CriteriaTreefmt);
DBCStorage<DestructibleModelDataEntry>      sDestructibleModelDataStore(DestructibleModelDatafmt);
DBCStorage<DifficultyEntry>                 sDifficultyStore(Difficultyfmt);
DBCStorage<DungeonEncounterEntry>           sDungeonEncounterStore(DungeonEncounterfmt);
DBCStorage<DurabilityCostsEntry>            sDurabilityCostsStore(DurabilityCostsfmt);
DBCStorage<DurabilityQualityEntry>          sDurabilityQualityStore(DurabilityQualityfmt);
DBCStorage<EmotesEntry>                     sEmotesStore(EmotesEntryfmt);
DBCStorage<EmotesTextEntry>                 sEmotesTextStore(EmotesTextEntryfmt);
DBCStorage<FactionEntry>                    sFactionStore(FactionEntryfmt);
DBCStorage<FactionTemplateEntry>            sFactionTemplateStore(FactionTemplateEntryfmt);
DBCStorage<GameObjectDisplayInfoEntry>      sGameObjectDisplayInfoStore(GameObjectDisplayInfofmt);
DBCStorage<GameTablesEntry>                 sGameTablesStore(GameTablesFmt);
DBCStorage<GemPropertiesEntry>              sGemPropertiesStore(GemPropertiesEntryfmt);
DBCStorage<GlyphPropertiesEntry>            sGlyphPropertiesStore(GlyphPropertiesfmt);
DBCStorage<GlyphSlotEntry>                  sGlyphSlotStore(GlyphSlotfmt);
DBCStorage<GuildPerkSpellsEntry>            sGuildPerkSpellsStore(GuildPerkSpellsfmt);
DBCStorage<ImportPriceArmorEntry>           sImportPriceArmorStore(ImportPriceArmorfmt);
DBCStorage<ImportPriceQualityEntry>         sImportPriceQualityStore(ImportPriceQualityfmt);
DBCStorage<ImportPriceShieldEntry>          sImportPriceShieldStore(ImportPriceShieldfmt);
DBCStorage<ImportPriceWeaponEntry>          sImportPriceWeaponStore(ImportPriceWeaponfmt);
DBCStorage<ItemArmorQualityEntry>           sItemArmorQualityStore(ItemArmorQualityfmt);
DBCStorage<ItemArmorShieldEntry>            sItemArmorShieldStore(ItemArmorShieldfmt);
DBCStorage<ItemArmorTotalEntry>             sItemArmorTotalStore(ItemArmorTotalfmt);
DBCStorage<ItemBagFamilyEntry>              sItemBagFamilyStore(ItemBagFamilyfmt);
DBCStorage<ItemClassEntry>                  sItemClassStore(ItemClassfmt);
DBCStorage<ItemDamageEntry>                 sItemDamageAmmoStore(ItemDamagefmt);
DBCStorage<ItemDamageEntry>                 sItemDamageOneHandCasterStore(ItemDamagefmt);
DBCStorage<ItemDamageEntry>                 sItemDamageOneHandStore(ItemDamagefmt);
DBCStorage<ItemDamageEntry>                 sItemDamageRangedStore(ItemDamagefmt);
DBCStorage<ItemDamageEntry>                 sItemDamageThrownStore(ItemDamagefmt);
DBCStorage<ItemDamageEntry>                 sItemDamageTwoHandCasterStore(ItemDamagefmt);
DBCStorage<ItemDamageEntry>                 sItemDamageTwoHandStore(ItemDamagefmt);
DBCStorage<ItemDamageEntry>                 sItemDamageWandStore(ItemDamagefmt);
DBCStorage<ItemDisenchantLootEntry>         sItemDisenchantLootStore(ItemDisenchantLootfmt);
DBCStorage<ItemLimitCategoryEntry>          sItemLimitCategoryStore(ItemLimitCategoryEntryfmt);
DBCStorage<ItemPriceBaseEntry>              sItemPriceBaseStore(ItemPriceBasefmt);
DBCStorage<ItemRandomPropertiesEntry>       sItemRandomPropertiesStore(ItemRandomPropertiesfmt);
DBCStorage<ItemRandomSuffixEntry>           sItemRandomSuffixStore(ItemRandomSuffixfmt);
DBCStorage<ItemReforgeEntry>                sItemReforgeStore(ItemReforgefmt);
DBCStorage<ItemSetEntry>                    sItemSetStore(ItemSetEntryfmt);
DBCStorage<ItemSetSpellEntry>               sItemSetSpellStore(ItemSetSpellEntryfmt);
DBCStorage<ItemSpecEntry>                   sItemSpecStore(ItemSpecEntryfmt);
DBCStorage<ItemSpecOverrideEntry>           sItemSpecOverrideStore(ItemSpecOverrideEntryfmt);
DBCStorage<LFGDungeonEntry>                 sLFGDungeonStore(LFGDungeonEntryfmt);
DBCStorage<LiquidTypeEntry>                 sLiquidTypeStore(LiquidTypefmt);
DBCStorage<LockEntry>                       sLockStore(LockEntryfmt);
DBCStorage<MailTemplateEntry>               sMailTemplateStore(MailTemplateEntryfmt);
DBCStorage<MapDifficultyEntry>              sMapDifficultyStore(MapDifficultyEntryfmt); // only for loading
DBCStorage<MapEntry>                        sMapStore(MapEntryfmt);
DBCStorage<ModifierTreeEntry>               sModifierTreeStore(ModifierTreefmt);
DBCStorage<MountCapabilityEntry>            sMountCapabilityStore(MountCapabilityfmt);
DBCStorage<MountTypeEntry>                  sMountTypeStore(MountTypefmt);
DBCStorage<MovieEntry>                      sMovieStore(MovieEntryfmt);
DBCStorage<NameGenEntry>                    sNameGenStore(NameGenfmt);
DBCStorage<PhaseEntry>                      sPhaseStores(PhaseEntryfmt);
DBCStorage<PowerDisplayEntry>               sPowerDisplayStore(PowerDisplayEntryfmt);
DBCStorage<PvPDifficultyEntry>              sPvPDifficultyStore(PvPDifficultyfmt);
DBCStorage<QuestFactionRewEntry>            sQuestFactionRewardStore(QuestFactionRewardfmt);
DBCStorage<QuestPOIBlobEntry>               sQuestPOIBlobStore(QuestPOIBlobfmt);
DBCStorage<QuestPOIPointEntry>              sQuestPOIPointStore(QuestPOIPointfmt);
DBCStorage<QuestSortEntry>                  sQuestSortStore(QuestSortEntryfmt);
DBCStorage<QuestV2Entry>                    sQuestV2Store(QuestV2fmt);
DBCStorage<QuestXPEntry>                    sQuestXPStore(QuestXPfmt);
DBCStorage<RandomPropertiesPointsEntry>     sRandomPropertiesPointsStore(RandomPropertiesPointsfmt);
DBCStorage<ResearchBranchEntry>             sResearchBranchStore(ResearchBranchfmt);
DBCStorage<ResearchProjectEntry>            sResearchProjectStore(ResearchProjectfmt);
DBCStorage<ResearchSiteEntry>               sResearchSiteStore(ResearchSitefmt);
DBCStorage<ScalingStatDistributionEntry>    sScalingStatDistributionStore(ScalingStatDistributionfmt);
DBCStorage<ScenarioEntry>                   sScenarioStore(Scenariofmt);
DBCStorage<ScenarioStepEntry>               sScenarioStepStore(ScenarioStepfmt);
DBCStorage<SkillLineAbilityEntry>           sSkillLineAbilityStore(SkillLineAbilityfmt);
DBCStorage<SkillLineEntry>                  sSkillLineStore(SkillLinefmt);
DBCStorage<SpecializationSpellEntry>        sSpecializationSpellStore(SpecializationSpellsfmt);
DBCStorage<SpellAuraOptionsEntry>           sSpellAuraOptionsStore(SpellAuraOptionsEntryfmt);
DBCStorage<SpellCastTimesEntry>             sSpellCastTimesStore(SpellCastTimefmt);
DBCStorage<SpellCategoriesEntry>            sSpellCategoriesStore(SpellCategoriesEntryfmt);
DBCStorage<SpellCategoryEntry>              sSpellCategoryStores(SpellCategoryEntryfmt);
DBCStorage<SpellCooldownsEntry>             sSpellCooldownsStore(SpellCooldownsEntryfmt);
DBCStorage<SpellDurationEntry>              sSpellDurationStore(SpellDurationfmt);
DBCStorage<SpellEffectEntry>                sSpellEffectStore(SpellEffectEntryfmt);
DBCStorage<SpellEffectScalingEntry>         sSpellEffectScalingStore(SpellEffectScalingEntryfmt);
DBCStorage<SpellEntry>                      sSpellStore(SpellEntryfmt);
DBCStorage<SpellEquippedItemsEntry>         sSpellEquippedItemsStore(SpellEquippedItemsEntryfmt);
DBCStorage<SpellFocusObjectEntry>           sSpellFocusObjectStore(SpellFocusObjectfmt);
DBCStorage<SpellInterruptsEntry>            sSpellInterruptsStore(SpellInterruptsEntryfmt);
DBCStorage<SpellItemEnchantmentConditionEntry> sSpellItemEnchantmentConditionStore(SpellItemEnchantmentConditionfmt);
DBCStorage<SpellItemEnchantmentEntry>       sSpellItemEnchantmentStore(SpellItemEnchantmentfmt);
DBCStorage<SpellLevelsEntry>                sSpellLevelsStore(SpellLevelsEntryfmt);
DBCStorage<SpellProcsPerMinuteEntry>        sSpellProcsPerMinuteStore(SpellProcsPerMinuteEntryfmt);
DBCStorage<SpellProcsPerMinuteModEntry>     sSpellProcsPerMinuteModStore(SpellProcsPerMinuteModEntryfmt);
DBCStorage<SpellRadiusEntry>                sSpellRadiusStore(SpellRadiusfmt);
DBCStorage<SpellRangeEntry>                 sSpellRangeStore(SpellRangefmt);
DBCStorage<SpellScalingEntry>               sSpellScalingStore(SpellScalingEntryfmt);
DBCStorage<SpellShapeshiftEntry>            sSpellShapeshiftStore(SpellShapeshiftEntryfmt);
DBCStorage<SpellShapeshiftFormEntry>        sSpellShapeshiftFormStore(SpellShapeshiftFormfmt);
DBCStorage<SpellTargetRestrictionsEntry>    sSpellTargetRestrictionsStore(SpellTargetRestrictionsEntryfmt);
DBCStorage<StableSlotPricesEntry>           sStableSlotPricesStore(StableSlotPricesfmt);
DBCStorage<SummonPropertiesEntry>           sSummonPropertiesStore(SummonPropertiesfmt);
DBCStorage<TalentEntry>                     sTalentStore(TalentEntryfmt);
DBCStorage<TeamContributionPointsEntry>     sTeamContributionPointsStore(TeamContributionPointsfmt);
DBCStorage<TotemCategoryEntry>              sTotemCategoryStore(TotemCategoryEntryfmt);
DBCStorage<TransportAnimationEntry>         sTransportAnimationStore(TransportAnimationfmt);
DBCStorage<TransportRotationEntry>          sTransportRotationStore(TransportRotationfmt);
DBCStorage<UnitPowerBarEntry>               sUnitPowerBarStore(UnitPowerBarfmt);
DBCStorage<VehicleEntry>                    sVehicleStore(VehicleEntryfmt);
DBCStorage<VehicleSeatEntry>                sVehicleSeatStore(VehicleSeatEntryfmt);
DBCStorage<WMOAreaTableEntry>               sWMOAreaTableStore(WMOAreaTableEntryfmt);
DBCStorage<WorldMapAreaEntry>               sWorldMapAreaStore(WorldMapAreaEntryfmt);
DBCStorage<WorldMapOverlayEntry>            sWorldMapOverlayStore(WorldMapOverlayEntryfmt);
DBCStorage<WorldSafeLocsEntry>              sWorldSafeLocsStore(WorldSafeLocsEntryfmt);

GameTable<GtNPCManaCostScalerEntry>         sGtNPCManaCostScalerStore(GtNPCManaCostScalerfmt);
GameTable<GtNpcTotalHpEntry>                sGtNpcTotalHpExp1Store(GtNpcTotalHpfmt);
GameTable<GtNpcTotalHpEntry>                sGtNpcTotalHpExp2Store(GtNpcTotalHpfmt);
GameTable<GtNpcTotalHpEntry>                sGtNpcTotalHpExp3Store(GtNpcTotalHpfmt);
GameTable<GtNpcTotalHpEntry>                sGtNpcTotalHpExp4Store(GtNpcTotalHpfmt);
GameTable<GtNpcTotalHpEntry>                sGtNpcTotalHpExp5Store(GtNpcTotalHpfmt);
GameTable<GtNpcTotalHpEntry>                sGtNpcTotalHpStore(GtNpcTotalHpfmt);
GameTable<GtOCTLevelExperienceEntry>        sGtOCTLevelExperienceStore(GtOCTLevelExperiencefmt);
GameTable<GtBarberShopCostBaseEntry>        sGtBarberShopCostBaseStore(GtBarberShopCostBasefmt);
GameTable<GtBattlePetTypeDamageModEntry>    sGtBattlePetTypeDamageModStore(GtBattlePetTypeDamageModfmt);
GameTable<GtChanceToMeleeCritBaseEntry>     sGtChanceToMeleeCritBaseStore(GtChanceToMeleeCritBasefmt);
GameTable<GtChanceToMeleeCritEntry>         sGtChanceToMeleeCritStore(GtChanceToMeleeCritfmt);
GameTable<GtChanceToSpellCritBaseEntry>     sGtChanceToSpellCritBaseStore(GtChanceToSpellCritBasefmt);
GameTable<GtChanceToSpellCritEntry>         sGtChanceToSpellCritStore(GtChanceToSpellCritfmt);
GameTable<GtCombatRatingsEntry>             sGtCombatRatingsStore(GtCombatRatingsfmt);
GameTable<GtItemSocketCostPerLevelEntry>    sGtItemSocketCostPerLevelStore(GtItemSocketCostPerLevelfmt);
GameTable<GtOCTBaseHPByClassEntry>          sGtOCTBaseHPByClassStore(GtOCTBaseHPByClassfmt);
GameTable<GtOCTBaseMPByClassEntry>          sGtOCTBaseMPByClassStore(GtOCTBaseMPByClassfmt);
GameTable<gtOCTHpPerStaminaEntry>           sGtOCTHpPerStaminaStore(GtOCTHpPerStaminafmt);
GameTable<GtOCTRegenHPEntry>                sGtOCTRegenHPStore(GtOCTRegenHPfmt);
GameTable<GtRegenMPPerSptEntry>             sGtRegenMPPerSptStore(GtRegenMPPerSptfmt);
GameTable<GtSpellScalingEntry>              sGtSpellScalingStore(GtSpellScalingfmt);

typedef std::list<std::string> StoreProblemList;

uint32 DBCFileCount = 0;
uint32 GameTableCount = 0;

static bool LoadDBC_assert_print(uint32 fsize, uint32 rsize, const std::string& filename)
{
    sLog->outError(LOG_FILTER_GENERAL, "Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).", filename.c_str(), fsize, rsize);

    // ASSERT must fail after function call
    return false;
}

template<class T>
inline void LoadDBC(uint32& availableDbcLocales, StoreProblemList& errors, DBCStorage<T>& storage, std::string const& dbcPath, std::string const& filename, uint32 defaultLocale, std::string const* customFormat = NULL, std::string const* customIndexName = NULL)
{
    // compatibility format and C++ structure sizes
    ASSERT(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDBC_assert_print(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()), sizeof(T), filename));

    ++DBCFileCount;
    std::string dbcFilename = dbcPath + localeNames[defaultLocale] + '/' + filename;
    SqlDbc * sql = NULL;
    if (customFormat)
        sql = new SqlDbc(&filename, customFormat, customIndexName, storage.GetFormat());

    if (storage.Load(dbcFilename.c_str(), sql))
    {
        for (uint8 i = 0; i < TOTAL_LOCALES; ++i)
        {
            if (!(availableDbcLocales & (1 << i)))
                continue;

            std::string localizedName(dbcPath);
            localizedName.append(localeNames[i]);
            localizedName.push_back('/');
            localizedName.append(filename);

            if (!storage.LoadStringsFrom(localizedName.c_str()))
                availableDbcLocales &= ~(1<<i);             // mark as not available for speedup next checks
        }
    }
    else
    {
        // sort problematic dbc to (1) non compatible and (2) non-existed
        if (FILE* f = fopen(dbcFilename.c_str(), "rb"))
        {
            std::ostringstream stream;
            stream << dbcFilename << " exists, and has " << storage.GetFieldCount() << " field(s) (expected " << strlen(storage.GetFormat()) << "). Extracted file might be from wrong client version or a database-update has been forgotten.";
            std::string buf = stream.str();
            errors.push_back(buf);
            fclose(f);
        }
        else
            errors.push_back(dbcFilename);
    }

    delete sql;
}

template<class T>
inline void LoadGameTable(StoreProblemList& errors, std::string const& tableName, GameTable<T>& storage, std::string const& dbcPath, std::string const& filename)
{
    // compatibility format and C++ structure sizes
    ASSERT(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDBC_assert_print(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()), sizeof(T), filename));

    ++GameTableCount;
    std::string dbcFilename = dbcPath + filename;

    if (storage.Load(dbcFilename.c_str()))
    {
        bool found = false;
        // Find table definition in GameTables.dbc
        for (GameTablesEntry const* gt : sGameTablesStore)
        {
            if (tableName == gt->Name)
            {
                found = true;
                storage.SetGameTableEntry(gt);
                break;
            }
        }

        ASSERT(found, "Game table %s definition not found in GameTables.dbc", tableName.c_str());
    }
    else
    {
        // sort problematic dbc to (1) non compatible and (2) non-existed
        if (FILE* f = fopen(dbcFilename.c_str(), "rb"))
        {
            std::ostringstream stream;
            stream << dbcFilename << " exists, and has " << storage.GetFieldCount() << " field(s) (expected " << strlen(storage.GetFormat()) << "). Extracted file might be from wrong client version or a database-update has been forgotten.";
            std::string buf = stream.str();
            errors.push_back(buf);
            fclose(f);
        }
        else
            errors.push_back(dbcFilename);
    }
}

void LoadDBCStores(const std::string& dataPath, uint32 defaultLocale)
{
    uint32 oldMSTime = getMSTime();

    std::string dbcPath = dataPath + "dbc/";

    StoreProblemList bad_dbc_files;
    uint32 availableDbcLocales = 0xFFFFFFFF;

#define LOAD_DBC(store, file) LoadDBC(availableDbcLocales, bad_dbc_files, store, dbcPath, file, defaultLocale)

    //LoadDBC(availableDbcLocales, bad_dbc_files, sAchievementCriteriaStore, "Achievement_Criteria.dbc", &CustomAchievementCriteriafmt, &CustomAchievementCriteriaIndex);
    //LoadDBC(availableDbcLocales, bad_dbc_files, sAreaPOIStore,             "AreaPOI.dbc");
    //LoadDBC(availableDbcLocales, bad_dbc_files, sCinematicSequencesStore,  "CinematicSequences.dbc");
    //LoadDBC(availableDbcLocales, bad_dbc_files, sItemReforgeStore,         "ItemReforge.dbc");
    //LoadDBC(availableDbcLocales, bad_dbc_files, sQuestPOIBlobStore,        "QuestPOIBlob.dbc");
    //LoadDBC(availableDbcLocales, bad_dbc_files, sStableSlotPricesStore,    "StableSlotPrices.dbc");
    //LoadDBC(availableDbcLocales, bad_dbc_files, sTeamContributionPointsStore, "TeamContributionPoints.dbc");
    //LoadDBC(dbcCount, availableDbcLocales, bad_dbc_files, sItemDisplayInfoStore,     "ItemDisplayInfo.dbc");     -- not used currently
    LOAD_DBC(sAchievementStore,                 "Achievement.dbc"/*, &CustomAchievementfmt, &CustomAchievementIndex*/);
    LOAD_DBC(sAreaStore,                        "AreaTable.dbc");
    LOAD_DBC(sAreaTriggerStore,                 "AreaTrigger.dbc");
    LOAD_DBC(sArmorLocationStore,               "ArmorLocation.dbc");
    LOAD_DBC(sAuctionHouseStore,                "AuctionHouse.dbc");
    LOAD_DBC(sBankBagSlotPricesStore,           "BankBagSlotPrices.dbc");
    LOAD_DBC(sBannedAddOnsStore,                "BannedAddOns.dbc");
    LOAD_DBC(sBarberShopStyleStore,             "BarberShopStyle.dbc");
    LOAD_DBC(sBattlemasterListStore,            "BattlemasterList.dbc");
    LOAD_DBC(sCharacterLoadoutItemStore,        "CharacterLoadoutItem.dbc");
    LOAD_DBC(sCharStartOutfitStore,             "CharStartOutfit.dbc");
    LOAD_DBC(sCharTitlesStore,                  "CharTitles.dbc");
    LOAD_DBC(sChatChannelsStore,                "ChatChannels.dbc");
    LOAD_DBC(sChrClassesStore,                  "ChrClasses.dbc");
    LOAD_DBC(sChrPowerTypesStore,               "ChrClassesXPowerTypes.dbc");
    LOAD_DBC(sChrRacesStore,                    "ChrRaces.dbc");
    LOAD_DBC(sChrSpecializationsStore,          "ChrSpecialization.dbc");
    LOAD_DBC(sCreatureDisplayInfoExtraStore,    "CreatureDisplayInfoExtra.dbc");
    LOAD_DBC(sCreatureDisplayInfoStore,         "CreatureDisplayInfo.dbc");
    LOAD_DBC(sCreatureFamilyStore,              "CreatureFamily.dbc");
    LOAD_DBC(sCreatureModelDataStore,           "CreatureModelData.dbc");
    LOAD_DBC(sCreatureSpellDataStore,           "CreatureSpellData.dbc");
    LOAD_DBC(sCreatureTypeStore,                "CreatureType.dbc");
    LOAD_DBC(sCriteriaStore,                    "Criteria.dbc"/*, &CustomCriteriafmt, &CustomCriteriaIndex*/);
    LOAD_DBC(sCriteriaTreeStore,                "CriteriaTree.dbc"); // &CustomCriteriaTreefmt, &CustomCriteriaTreeIndex
    LOAD_DBC(sDestructibleModelDataStore,       "DestructibleModelData.dbc");
    LOAD_DBC(sDifficultyStore,                  "Difficulty.dbc");
    LOAD_DBC(sDungeonEncounterStore,            "DungeonEncounter.dbc");
    LOAD_DBC(sDurabilityCostsStore,             "DurabilityCosts.dbc");
    LOAD_DBC(sDurabilityQualityStore,           "DurabilityQuality.dbc");
    LOAD_DBC(sEmotesStore,                      "Emotes.dbc");
    LOAD_DBC(sEmotesTextStore,                  "EmotesText.dbc");
    LOAD_DBC(sFactionStore,                     "Faction.dbc");
    LOAD_DBC(sFactionTemplateStore,             "FactionTemplate.dbc");
    LOAD_DBC(sGameObjectDisplayInfoStore,       "GameObjectDisplayInfo.dbc");
    LOAD_DBC(sGameTablesStore,                  "GameTables.dbc");
    LOAD_DBC(sGemPropertiesStore,               "GemProperties.dbc");
    LOAD_DBC(sGlyphPropertiesStore,             "GlyphProperties.dbc");
    LOAD_DBC(sGlyphSlotStore,                   "GlyphSlot.dbc");
    LOAD_DBC(sGuildPerkSpellsStore,             "GuildPerkSpells.dbc");
    LOAD_DBC(sImportPriceArmorStore,            "ImportPriceArmor.dbc");
    LOAD_DBC(sImportPriceQualityStore,          "ImportPriceQuality.dbc");
    LOAD_DBC(sImportPriceShieldStore,           "ImportPriceShield.dbc");
    LOAD_DBC(sImportPriceWeaponStore,           "ImportPriceWeapon.dbc");
    LOAD_DBC(sItemArmorQualityStore,            "ItemArmorQuality.dbc");
    LOAD_DBC(sItemArmorShieldStore,             "ItemArmorShield.dbc");
    LOAD_DBC(sItemArmorTotalStore,              "ItemArmorTotal.dbc");
    LOAD_DBC(sItemBagFamilyStore,               "ItemBagFamily.dbc");
    LOAD_DBC(sItemClassStore,                   "ItemClass.dbc");
    LOAD_DBC(sItemDamageAmmoStore,              "ItemDamageAmmo.dbc");
    LOAD_DBC(sItemDamageOneHandCasterStore,     "ItemDamageOneHandCaster.dbc");
    LOAD_DBC(sItemDamageOneHandStore,           "ItemDamageOneHand.dbc");
    LOAD_DBC(sItemDamageRangedStore,            "ItemDamageRanged.dbc");
    LOAD_DBC(sItemDamageThrownStore,            "ItemDamageThrown.dbc");
    LOAD_DBC(sItemDamageTwoHandCasterStore,     "ItemDamageTwoHandCaster.dbc");
    LOAD_DBC(sItemDamageTwoHandStore,           "ItemDamageTwoHand.dbc");
    LOAD_DBC(sItemDamageWandStore,              "ItemDamageWand.dbc");
    LOAD_DBC(sItemDisenchantLootStore,          "ItemDisenchantLoot.dbc"); // &CustomtemDisenchantLootEntryfmt, &CustomtemDisenchantLootEntryIndex
    LOAD_DBC(sItemLimitCategoryStore,           "ItemLimitCategory.dbc");
    LOAD_DBC(sItemPriceBaseStore,               "ItemPriceBase.dbc");
    LOAD_DBC(sItemRandomPropertiesStore,        "ItemRandomProperties.dbc");
    LOAD_DBC(sItemRandomSuffixStore,            "ItemRandomSuffix.dbc");
    LOAD_DBC(sItemSetSpellStore,                "ItemSetSpell.dbc");
    LOAD_DBC(sItemSetStore,                     "ItemSet.dbc");
    LOAD_DBC(sItemSpecOverrideStore,            "ItemSpecOverride.dbc"); // &CustomItemSpecOverrideEntryfmt, &CustomItemSpecOverrideEntryIndex
    LOAD_DBC(sItemSpecStore,                    "ItemSpec.dbc");
    LOAD_DBC(sLFGDungeonStore,                  "LfgDungeons.dbc");
    LOAD_DBC(sLiquidTypeStore,                  "LiquidType.dbc");
    LOAD_DBC(sLockStore,                        "Lock.dbc");
    LOAD_DBC(sMailTemplateStore,                "MailTemplate.dbc");
    LOAD_DBC(sMapDifficultyStore,               "MapDifficulty.dbc"); // &CustomMapDifficultyEntryfmt, &CustomMapDifficultyEntryIndex
    LOAD_DBC(sMapStore,                         "Map.dbc");
    LOAD_DBC(sModifierTreeStore,                "ModifierTree.dbc");
    LOAD_DBC(sMountCapabilityStore,             "MountCapability.dbc");
    LOAD_DBC(sMountTypeStore,                   "MountType.dbc");
    LOAD_DBC(sMovieStore,                       "Movie.dbc");
    LOAD_DBC(sNameGenStore,                     "NameGen.dbc");
    LOAD_DBC(sPhaseStores,                      "Phase.dbc");
    LOAD_DBC(sPowerDisplayStore,                "PowerDisplay.dbc");
    LOAD_DBC(sPvPDifficultyStore,               "PvpDifficulty.dbc");
    LOAD_DBC(sQuestFactionRewardStore,          "QuestFactionReward.dbc");
    LOAD_DBC(sQuestPOIPointStore,               "QuestPOIPoint.dbc");
    LOAD_DBC(sQuestSortStore,                   "QuestSort.dbc");
    LOAD_DBC(sQuestV2Store,                     "QuestV2.dbc");
    LOAD_DBC(sQuestXPStore,                     "QuestXP.dbc");
    LOAD_DBC(sRandomPropertiesPointsStore,      "RandPropPoints.dbc");
    LOAD_DBC(sResearchBranchStore,              "ResearchBranch.dbc");
    LOAD_DBC(sResearchProjectStore,             "ResearchProject.dbc");
    LOAD_DBC(sResearchSiteStore,                "ResearchSite.dbc");
    LOAD_DBC(sScalingStatDistributionStore,     "ScalingStatDistribution.dbc");
    LOAD_DBC(sScenarioStepStore,                "ScenarioStep.dbc");
    LOAD_DBC(sScenarioStore,                    "Scenario.dbc");
    LOAD_DBC(sSkillLineAbilityStore,            "SkillLineAbility.dbc");
    LOAD_DBC(sSkillLineStore,                   "SkillLine.dbc");
    LOAD_DBC(sSpecializationSpellStore,         "SpecializationSpells.dbc");
    LOAD_DBC(sSpellAuraOptionsStore,            "SpellAuraOptions.dbc"/*, &CustomSpellAuraOptionsEntryfmt, &CustomSpellAuraOptionsEntryIndex*/);
    LOAD_DBC(sSpellCastTimesStore,              "SpellCastTimes.dbc");
    LOAD_DBC(sSpellCategoriesStore,             "SpellCategories.dbc");
    LOAD_DBC(sSpellCategoryStores,              "SpellCategory.dbc");
    LOAD_DBC(sSpellCooldownsStore,              "SpellCooldowns.dbc");
    LOAD_DBC(sSpellDurationStore,               "SpellDuration.dbc");
    LOAD_DBC(sSpellEffectScalingStore,          "SpellEffectScaling.dbc");
    LOAD_DBC(sSpellEffectStore,                 "SpellEffect.dbc");
    LOAD_DBC(sSpellEquippedItemsStore,          "SpellEquippedItems.dbc");
    LOAD_DBC(sSpellFocusObjectStore,            "SpellFocusObject.dbc");
    LOAD_DBC(sSpellInterruptsStore,             "SpellInterrupts.dbc");
    LOAD_DBC(sSpellItemEnchantmentConditionStore, "SpellItemEnchantmentCondition.dbc");
    LOAD_DBC(sSpellItemEnchantmentStore,        "SpellItemEnchantment.dbc");
    LOAD_DBC(sSpellLevelsStore,                 "SpellLevels.dbc");
    LOAD_DBC(sSpellProcsPerMinuteModStore,      "SpellProcsPerMinuteMod.dbc");
    LOAD_DBC(sSpellProcsPerMinuteStore,         "SpellProcsPerMinute.dbc");
    LOAD_DBC(sSpellRadiusStore,                 "SpellRadius.dbc");
    LOAD_DBC(sSpellRangeStore,                  "SpellRange.dbc");
    LOAD_DBC(sSpellScalingStore,                "SpellScaling.dbc");
    LOAD_DBC(sSpellShapeshiftFormStore,         "SpellShapeshiftForm.dbc");
    LOAD_DBC(sSpellShapeshiftStore,             "SpellShapeshift.dbc");
    LOAD_DBC(sSpellStore,                       "Spell.dbc"/*, &CustomSpellEntryfmt, &CustomSpellEntryIndex*/);
    LOAD_DBC(sSpellTargetRestrictionsStore,     "SpellTargetRestrictions.dbc");
    LOAD_DBC(sSummonPropertiesStore,            "SummonProperties.dbc");
    LOAD_DBC(sTalentStore,                      "Talent.dbc");
    LOAD_DBC(sTotemCategoryStore,               "TotemCategory.dbc");
    LOAD_DBC(sTransportAnimationStore,          "TransportAnimation.dbc");
    LOAD_DBC(sTransportRotationStore,           "TransportRotation.dbc");
    LOAD_DBC(sUnitPowerBarStore,                "UnitPowerBar.dbc");
    LOAD_DBC(sVehicleSeatStore,                 "VehicleSeat.dbc");
    LOAD_DBC(sVehicleStore,                     "Vehicle.dbc");
    LOAD_DBC(sWMOAreaTableStore,                "WMOAreaTable.dbc");
    LOAD_DBC(sWorldMapAreaStore,                "WorldMapArea.dbc");
    LOAD_DBC(sWorldMapOverlayStore,             "WorldMapOverlay.dbc");
    LOAD_DBC(sWorldSafeLocsStore,               "WorldSafeLocs.dbc");

#undef LOAD_DBC

    // error checks
    if (bad_dbc_files.size() >= DBCFileCount)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Incorrect DataDir value in worldserver.conf or ALL required *.dbc files (%d) not found by path: %sdbc/%s/", DBCFileCount, dataPath.c_str(), localeNames[defaultLocale]);
        exit(1);
    }
    else if (!bad_dbc_files.empty())
    {
        std::string str;
        for (StoreProblemList::iterator i = bad_dbc_files.begin(); i != bad_dbc_files.end(); ++i)
            str += *i + "\n";

        sLog->outError(LOG_FILTER_GENERAL, "Some required *.dbc files (%u from %d) not found or not compatible:\n%s", (uint32)bad_dbc_files.size(), DBCFileCount, str.c_str());
        exit(1);
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Initialized %d DBC data stores in %u ms", DBCFileCount, GetMSTimeDiffToNow(oldMSTime));
}

void LoadGameTables(const std::string& dataPath, uint32 defaultLocale)
{
    uint32 oldMSTime = getMSTime();

    std::string dbcPath = dataPath + "dbc/" + localeNames[defaultLocale] + '/';

    StoreProblemList bad_dbc_files;

#define LOAD_GT(tableName, store, file) LoadGameTable(bad_dbc_files, tableName, store, dbcPath, file)

    LOAD_GT("BarberShopCostBase",      sGtBarberShopCostBaseStore,      "gtBarberShopCostBase.dbc");
    LOAD_GT("BattlePetTypeDamageMod",  sGtBattlePetTypeDamageModStore,  "gtBattlePetTypeDamageMod.dbc");
    LOAD_GT("ChanceToMeleeCrit",       sGtChanceToMeleeCritStore,       "gtChanceToMeleeCrit.dbc");
    LOAD_GT("ChanceToMeleeCritBase",   sGtChanceToMeleeCritBaseStore,   "gtChanceToMeleeCritBase.dbc");
    LOAD_GT("ChanceToSpellCrit",       sGtChanceToSpellCritStore,       "gtChanceToSpellCrit.dbc");
    LOAD_GT("ChanceToSpellCritBase",   sGtChanceToSpellCritBaseStore,   "gtChanceToSpellCritBase.dbc");
    LOAD_GT("CombatRatings",           sGtCombatRatingsStore,           "gtCombatRatings.dbc");
    LOAD_GT("ItemSocketCostPerLevel",  sGtItemSocketCostPerLevelStore,  "gtItemSocketCostPerLevel.dbc");;
    LOAD_GT("NPCManaCostScaler",       sGtNPCManaCostScalerStore,       "gtNPCManaCostScaler.dbc");
    LOAD_GT("NpcTotalHp",              sGtNpcTotalHpStore,              "gtNpcTotalHp.dbc");
    LOAD_GT("NpcTotalHpExp1",          sGtNpcTotalHpExp1Store,          "gtNpcTotalHpExp1.dbc");
    LOAD_GT("NpcTotalHpExp2",          sGtNpcTotalHpExp2Store,          "gtNpcTotalHpExp2.dbc");
    LOAD_GT("NpcTotalHpExp3",          sGtNpcTotalHpExp3Store,          "gtNpcTotalHpExp3.dbc");
    LOAD_GT("NpcTotalHpExp4",          sGtNpcTotalHpExp4Store,          "gtNpcTotalHpExp4.dbc");
    LOAD_GT("NpcTotalHpExp5",          sGtNpcTotalHpExp5Store,          "gtNpcTotalHpExp5.dbc");
    LOAD_GT("OCTBaseHPByClass",        sGtOCTBaseHPByClassStore,        "gtOCTBaseHPByClass.dbc");
    LOAD_GT("OCTBaseMPByClass",        sGtOCTBaseMPByClassStore,        "gtOCTBaseMPByClass.dbc");
    LOAD_GT("OCTHPPerStamina",         sGtOCTHpPerStaminaStore,         "gtOCTHpPerStamina.dbc");
    LOAD_GT("OCTLevelExperience",      sGtOCTLevelExperienceStore,      "gtOCTLevelExperience.dbc");
    LOAD_GT("RegenMPPerSpt",           sGtRegenMPPerSptStore,           "gtRegenMPPerSpt.dbc");
    LOAD_GT("SpellScaling",            sGtSpellScalingStore,            "gtSpellScaling.dbc");

#undef LOAD_GT

    // error checks
    if (bad_dbc_files.size() >= GameTableCount)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Incorrect DataDir value in worldserver.conf or ALL required *.dbc GameTable files (%d) not found by path: %sdbc/%s/", DBCFileCount, dataPath.c_str(), localeNames[defaultLocale]);
        exit(1);
    }
    else if (!bad_dbc_files.empty())
    {
        std::string str;
        for (StoreProblemList::iterator i = bad_dbc_files.begin(); i != bad_dbc_files.end(); ++i)
            str += *i + "\n";

        sLog->outError(LOG_FILTER_GENERAL, "Some required *.dbc GameTable files (%u from %d) not found or not compatible:\n%s", (uint32)bad_dbc_files.size(), GameTableCount, str.c_str());
        exit(1);
    }

    sLog->outError(LOG_FILTER_GENERAL, ">> Initialized %d DBC GameTables data stores in %u ms", GameTableCount, GetMSTimeDiffToNow(oldMSTime));
}

void InitDBCCustomStores()
{
    for (AreaTableEntry const* area : sAreaStore)
    {
        sAreaEntry.insert(AreaEntryMap::value_type(area->ID, area));
        sAreaFlagByAreaID.insert(AreaFlagByAreaID::value_type(uint16(area->ID), area->AreaBit));

        if (area->ParentAreaID == 0)
            sAreaFlagByMapID.insert(AreaFlagByMapID::value_type(area->mapid, area->AreaBit));
    }

    for (AchievementEntry const* as : sAchievementStore)
        if (as->criteriaTree > 0)
            sAchievementParentList[as->criteriaTree] = as;

    for (CriteriaTreeEntry const* ct : sCriteriaTreeStore)
        if (ct->parent > 0)
            sCriteriaTreeList[ct->parent].push_back(ct);

    for (ModifierTreeEntry const* mt : sModifierTreeStore)
        if (mt->parent > 0)
            sModifierTreeList[mt->parent].push_back(mt);

    for (FactionEntry const* faction : sFactionStore)
    {
        if (faction->team)
        {
            SimpleFactionsList &flist = sFactionTeamMap[faction->team];
            flist.push_back(faction->ID);
        }
    }

    for (GameObjectDisplayInfoEntry const* info : sGameObjectDisplayInfoStore)
    {
        if (info->maxX < info->minX)
            std::swap(*(float*)(&info->maxX), *(float*)(&info->minX));
        if (info->maxY < info->minY)
            std::swap(*(float*)(&info->maxY), *(float*)(&info->minY));
        if (info->maxZ < info->minZ)
            std::swap(*(float*)(&info->maxZ), *(float*)(&info->minZ));
    }

    for (ItemSpecOverrideEntry const* isp : sItemSpecOverrideStore)
        sItemSpecsList[isp->ItemID].push_back(isp->SpecID);

    for (MapDifficultyEntry const* entry : sMapDifficultyStore)
    {
        if (!sMapStore.LookupEntry(entry->MapID))
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "DB table `mapdifficulty_dbc` or MapDifficulty.dbc has non-existant map %u.", entry->MapID);
            continue;
        }
        if (entry->DifficultyID && !sDifficultyStore.LookupEntry(entry->DifficultyID))
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "DB table `mapdifficulty_dbc` or MapDifficulty.dbc has non-existant difficulty %u.", entry->DifficultyID);
            continue;
        }
        sMapDifficultyMap[entry->MapID][entry->DifficultyID] = MapDifficulty(entry->DifficultyID, entry->RaidDuration, entry->MaxPlayers, entry->Message_lang[0] > 0);
    }
    sMapDifficultyStore.Clear();

    for (NameGenEntry const* entry : sNameGenStore)
        sGenNameVectoArraysMap[entry->race].stringVectorArray[entry->gender].push_back(std::string(entry->name));
    sNameGenStore.Clear();

    for (PvPDifficultyEntry const* entry : sPvPDifficultyStore)
    {
        if (entry->bracketId > MAX_BATTLEGROUND_BRACKETS)
            ASSERT(false && "Need update MAX_BATTLEGROUND_BRACKETS by DBC data");
    }

    for (ResearchProjectEntry const* rp : sResearchProjectStore)
    {
        if (!rp || !rp->IsVaid())
            continue;

        sResearchProjectSet.insert(rp);
    }

    for (ResearchSiteEntry const* rs : sResearchSiteStore)
    {
        if (!rs || !rs->IsValid())
            continue;

        ResearchSiteData& data = sResearchSiteDataMap[rs->ID];

        data.entry = rs;

        for (QuestPOIPointEntry const* poi : sQuestPOIPointStore)
            if (poi->POIId == rs->POIid)
                data.points.push_back(ResearchPOIPoint(poi->x, poi->y));

        if (data.points.size() == 0)
            sLog->outDebug(LOG_FILTER_SERVER_LOADING, "Research site %u POI %u map %u has 0 POI points in DBC!", rs->ID, rs->POIid, rs->MapID);
    }


    for (ScenarioStepEntry const* entry : sScenarioStepStore)
    {
        if (!entry || !entry->m_criteriaTreeId)
            continue;

        if (!sCriteriaTreeStore.LookupEntry(entry->m_criteriaTreeId))
            continue;

        sScenarioCriteriaTreeStore.insert(entry->m_criteriaTreeId);
    }

    for (uint32 i = 1; i < sSpellStore.GetNumRows(); ++i)
    {
        SpellCategoriesEntry const* spell = sSpellCategoriesStore.LookupEntry(i);
        if (spell && spell->Category)
            sSpellCategoryStore[spell->Category].insert(i);
    }

    for (SkillLineAbilityEntry const* skillLine : sSkillLineAbilityStore)
    {
        SpellEntry const* spellInfo = sSpellStore.LookupEntry(skillLine->spellId);
        if (!spellInfo)
            continue;

        SpellMiscEntry const* spellMisc = sSpellMiscStore.LookupEntry(spellInfo->MiscID);
        if (!spellMisc)
            continue;

        SpellLevelsEntry const* levels = sSpellLevelsStore.LookupEntry(spellInfo->LevelsID);
        if (levels && levels->spellLevel)
            continue;

        if (spellMisc->Attributes & SPELL_ATTR0_PASSIVE)
        {
            for (CreatureFamilyEntry const* cFamily : sCreatureFamilyStore)
            {
                if (skillLine->skillId != cFamily->skillLine[0] && skillLine->skillId != cFamily->skillLine[1])
                    continue;

                if (skillLine->learnOnGetSkill != ABILITY_LEARNED_ON_GET_RACE_OR_CLASS_SKILL)
                    continue;

                sPetFamilySpellsStore[cFamily->ID].insert(spellInfo->ID);
            }
        }
    }

    for (SpellTargetRestrictionsEntry const* restriction : sSpellTargetRestrictionsStore)
        sSpellRestrictionDiffMap[restriction->SpellId].restrictions.insert(restriction);

    for (SpellProcsPerMinuteModEntry const* sppm : sSpellProcsPerMinuteModStore)
        sSpellProcsPerMinuteModEntryList[sppm->ProcsPerMinuteId].push_back(sppm->Id);

    for (SpellEffectEntry const* spellEffect : sSpellEffectStore)
    {
        if (spellEffect->EffectIndex > MAX_SPELL_EFFECTS)
            continue;

        if (spellEffect->DifficultyID)
            sSpellEffectDiffMap[spellEffect->SpellID].effects[MAKE_PAIR16(spellEffect->EffectIndex, spellEffect->DifficultyID)] = spellEffect;
        else
            sSpellEffectMap[spellEffect->SpellID].effects[spellEffect->EffectIndex] = spellEffect;

        if (spellEffect->Effect == SPELL_EFFECT_LEARN_SPELL)
            sRevertLearnSpellList[spellEffect->EffectTriggerSpell] = spellEffect->SpellID;

        if (spellEffect->EffectTriggerSpell)
            sReversTriggerSpellList[spellEffect->EffectTriggerSpell] = spellEffect->SpellID;
    }

    for (SpellEntry const* spell : sSpellStore)
    {
        if (SpellEffectEntry const* spellEffect = spell->GetSpellEffect(EFFECT_1))
            if (spellEffect->Effect == SPELL_EFFECT_SKILL && IsProfessionSkill(spellEffect->EffectMiscValue))
                sSpellSkillingList.push_back(spell);
    }

    for (TalentEntry const* entry : sTalentStore)
        sTalentSpellList.insert(entry->spellId);

    for (TransportAnimationEntry const* entry : sTransportAnimationStore)
        sTransportAnimationsByEntry[entry->TransportID][entry->TimeIndex] = entry;

    for (TransportRotationEntry const* rot : sTransportRotationStore)
    {
        //WoD::ToDo
        //sTransportMgr->AddPathRotationToTransport(rot->TransportID, rot->TimeIndex, rot);
    }

    for (WMOAreaTableEntry const* wmoAreaTableEntry : sWMOAreaTableStore)
        sWMOAreaInfoByTripple.insert(WMOAreaInfoByTripple::value_type(WMOAreaTableTripple(wmoAreaTableEntry->WMOID, wmoAreaTableEntry->NameSet, wmoAreaTableEntry->WMOGroupID), wmoAreaTableEntry));

    for (ItemSetSpellEntry const* itemSetSpell : sItemSetSpellStore)
        sItemSetSpellsStore[itemSetSpell->ItemSetID].push_back(itemSetSpell);

    for (CharacterLoadoutItemEntry const* LoadOutItem : sCharacterLoadoutItemStore)
        sCharacterLoadoutItemMap[LoadOutItem->LoadOutID].push_back(LoadOutItem->ItemID);
}

const std::string* GetRandomCharacterName(uint8 race, uint8 gender)
{
    uint32 size = sGenNameVectoArraysMap[race].stringVectorArray[gender].size();
    uint32 randPos = urand(0, size - 1);

    return &sGenNameVectoArraysMap[race].stringVectorArray[gender][randPos];
}

SimpleFactionsList const* GetFactionTeamList(uint32 faction)
{
    FactionTeamMap::const_iterator itr = sFactionTeamMap.find(faction);
    if (itr != sFactionTeamMap.end())
        return &itr->second;

    return NULL;
}

std::list<uint32> GetItemSpecsList(uint32 ItemID)
{
    return sItemSpecsList[ItemID];
}

void AddSpecdtoItem(uint32 ItemID, uint32 SpecID)
{
    sItemSpecsList[ItemID].push_back(SpecID);
}

AchievementEntry const* GetsAchievementByTreeList(uint32 criteriaTree)
{
    UNORDERED_MAP<uint32, AchievementEntry const* >::const_iterator itr = sAchievementParentList.find(criteriaTree);
    if (itr != sAchievementParentList.end())
        return itr->second;
    return 0;
}


uint32 GetLearnSpell(uint32 trigerSpell)
{
    UNORDERED_MAP<uint32, uint32 >::const_iterator itr = sRevertLearnSpellList.find(trigerSpell);
    if (itr != sRevertLearnSpellList.end())
        return itr->second;
    return 0;
}

uint32 GetSpellByTrigger(uint32 trigerSpell)
{
    UNORDERED_MAP<uint32, uint32 >::const_iterator itr = sReversTriggerSpellList.find(trigerSpell);
    if (itr != sReversTriggerSpellList.end())
        return itr->second;
    return 0;
}

std::vector<CriteriaTreeEntry const*> const* GetCriteriaTreeList(uint32 parent)
{
    UNORDERED_MAP<uint32, std::vector<CriteriaTreeEntry const*> >::const_iterator itr = sCriteriaTreeList.find(parent);
    if (itr != sCriteriaTreeList.end())
        return &itr->second;
    return NULL;
}

std::vector<ModifierTreeEntry const*> const* GetModifierTreeList(uint32 parent)
{
    UNORDERED_MAP<uint32, std::vector<ModifierTreeEntry const*> >::const_iterator itr = sModifierTreeList.find(parent);
    if (itr != sModifierTreeList.end())
        return &itr->second;
    return NULL;
}

std::list<uint32> const* GetSpellProcsPerMinuteModList(uint32 PerMinId)
{
    UNORDERED_MAP<uint32, std::list<uint32> >::const_iterator itr = sSpellProcsPerMinuteModEntryList.find(PerMinId);
    if (itr != sSpellProcsPerMinuteModEntryList.end())
        return &itr->second;
    return NULL;
}

char const* GetPetName(uint32 petfamily, uint32 /*dbclang*/)
{
    if (!petfamily)
        return NULL;
    CreatureFamilyEntry const* pet_family = sCreatureFamilyStore.LookupEntry(petfamily);
    if (!pet_family)
        return NULL;
    return pet_family->Name ? pet_family->Name : NULL;
}

SpellEffectEntry const* GetSpellEffectEntry(uint32 spellId, uint32 effect, uint8 difficulty)
{
    if (spellId == 9262) //hack fix Segmentation fault
        return NULL;

    if (difficulty)
    {
        uint16 index = MAKE_PAIR16(effect, difficulty);
        SpellEffectDiffMap::const_iterator itr = sSpellEffectDiffMap.find(spellId);
        if (itr != sSpellEffectDiffMap.end())
        {
            SpellEffectsMap const* effects = &itr->second.effects;
            SpellEffectsMap::const_iterator itrsecond = effects->find(index);
            if (itrsecond != effects->end())
                return itrsecond->second;
        }
    }
    else
    {
        SpellEffectMap::const_iterator itr = sSpellEffectMap.find(spellId);
        if (itr != sSpellEffectMap.end())
            if (itr->second.effects[effect])
                return itr->second.effects[effect];
    }

    return NULL;
}

SpellEffectScalingEntry const* GetSpellEffectScalingEntry(uint32 effectId)
{
    return sSpellEffectScalingStore.LookupEntry(effectId);
}

SpellTargetRestrictionsEntry const *GetSpellTargetRestrioctions(uint32 spellId, uint16 difficulty)
{
    SpellRestrictionDiffMap::const_iterator itr = sSpellRestrictionDiffMap.find(spellId);
    if (itr != sSpellRestrictionDiffMap.end())
    {
        SpellRestrictionMap const* restr = &itr->second.restrictions;
        for (SpellRestrictionMap::const_iterator sr = restr->begin(); sr != restr->end(); ++sr)
        {
            if ((*sr)->m_difficultyID == difficulty)
                return *sr;
        }
    }

    return NULL;
}

TalentSpellPos const* GetTalentSpellPos(uint32 spellId)
{
    return NULL;
    /*TalentSpellPosMap::const_iterator itr = sTalentSpellPosMap.find(spellId);
    if (itr == sTalentSpellPosMap.end())
    return NULL;

    return &itr->second;*/
}

uint32 GetTalentSpellCost(uint32 spellId)
{
    if (TalentSpellPos const* pos = GetTalentSpellPos(spellId))
        return pos->rank + 1;

    return 0;
}

int32 GetAreaFlagByAreaID(uint32 area_id)
{
    AreaFlagByAreaID::iterator i = sAreaFlagByAreaID.find(area_id);
    if (i == sAreaFlagByAreaID.end())
        return -1;

    return i->second;
}

WMOAreaTableEntry const* GetWMOAreaTableEntryByTripple(int32 rootid, int32 adtid, int32 groupid)
{
    WMOAreaInfoByTripple::iterator i = sWMOAreaInfoByTripple.find(WMOAreaTableTripple(rootid, adtid, groupid));
    if (i == sWMOAreaInfoByTripple.end())
        return NULL;
    return i->second;
}

AreaTableEntry const* GetAreaEntryByAreaID(uint32 area_id)
{
    int32 areaflag = GetAreaFlagByAreaID(area_id);
    if (areaflag < 0)
        return NULL;

    return sAreaStore.LookupEntry(areaflag);
}

AreaTableEntry const* GetAreaEntryByAreaFlagAndMap(uint32 area_flag, uint32 map_id)
{
    if (area_flag)
        return sAreaStore.LookupEntry(area_flag);

    if (MapEntry const* mapEntry = sMapStore.LookupEntry(map_id))
        return GetAreaEntryByAreaID(mapEntry->linked_zone);

    return NULL;
}

uint32 GetAreaFlagByMapId(uint32 mapid)
{
    AreaFlagByMapID::iterator i = sAreaFlagByMapID.find(mapid);
    if (i == sAreaFlagByMapID.end())
        return 0;
    else
        return i->second;
}

uint32 GetVirtualMapForMapAndZone(uint32 mapid, uint32 zoneId)
{
    if (mapid != 530 && mapid != 571 && mapid != 732)   // speed for most cases
        return mapid;

    if (WorldMapAreaEntry const* wma = sWorldMapAreaStore.LookupEntry(zoneId))
        return wma->DisplayMapID >= 0 ? wma->DisplayMapID : wma->MapID;

    return mapid;
}

int32 GetMapFromZone(uint32 zoneId)
{
    if (WorldMapAreaEntry const* wma = sWorldMapAreaStore.LookupEntry(zoneId))
        return wma->DisplayMapID >= 0 ? wma->DisplayMapID : wma->MapID;

    return -1;
}

uint32 GetMaxLevelForExpansion(uint32 expansion)
{
    switch (expansion)
    {
        case CONTENT_1_60:
            return 60;
        case CONTENT_61_70:
            return 70;
        case CONTENT_71_80:
            return 80;
        case CONTENT_81_85:
            return 85;
        case CONTENT_86_90:
            return 90;
        default:
            break;

    }
    return 0;
}

uint32 GetExpansionForLevel(uint32 level)
{
    if (level < 60)
        return EXPANSION_CLASSIC;
    else if (level < 70)
        return EXPANSION_THE_BURNING_CRUSADE;
    else if (level < 80)
        return EXPANSION_WRATH_OF_THE_LICH_KING;
    else if (level < 85)
        return EXPANSION_CATACLYSM;
    else if (level < 90)
        return EXPANSION_MISTS_OF_PANDARIA;
    else
        return CURRENT_EXPANSION;
}

bool IsTotemCategoryCompatiableWith(uint32 itemTotemCategoryId, uint32 requiredTotemCategoryId)
{
    if (requiredTotemCategoryId == 0)
        return true;
    if (itemTotemCategoryId == 0)
        return false;

    TotemCategoryEntry const* itemEntry = sTotemCategoryStore.LookupEntry(itemTotemCategoryId);
    if (!itemEntry)
        return false;
    TotemCategoryEntry const* reqEntry = sTotemCategoryStore.LookupEntry(requiredTotemCategoryId);
    if (!reqEntry)
        return false;

    if (itemEntry->categoryType != reqEntry->categoryType)
        return false;

    return (itemEntry->categoryMask & reqEntry->categoryMask) == reqEntry->categoryMask;
}

void Zone2MapCoordinates(float& x, float& y, uint32 zone)
{
    WorldMapAreaEntry const* maEntry = sWorldMapAreaStore.LookupEntry(zone);

    // if not listed then map coordinates (instance)
    if (!maEntry)
        return;

    std::swap(x, y);                                         // at client map coords swapped
    x = x*((maEntry->LocBottom - maEntry->LocTop) / 100) + maEntry->LocTop;
    y = y*((maEntry->LocRight - maEntry->LocLeft) / 100) + maEntry->LocLeft;      // client y coord from top to down
}

void Map2ZoneCoordinates(float& x, float& y, uint32 zone)
{
    WorldMapAreaEntry const* maEntry = sWorldMapAreaStore.LookupEntry(zone);

    // if not listed then map coordinates (instance)
    if (!maEntry)
        return;

    x = (x - maEntry->LocTop) / ((maEntry->LocBottom - maEntry->LocTop) / 100);
    y = (y - maEntry->LocLeft) / ((maEntry->LocRight - maEntry->LocLeft) / 100);    // client y coord from top to down
    std::swap(x, y);                                         // client have map coords swapped
}

MapDifficulty const* GetDefaultMapDifficulty(uint32 mapID)
{
    auto itr = sMapDifficultyMap.find(mapID);
    if (itr == sMapDifficultyMap.end())
        return nullptr;

    if (itr->second.empty())
        return nullptr;

    for (auto& p : itr->second)
    {
        DifficultyEntry const* difficulty = sDifficultyStore.LookupEntry(p.first);
        if (!difficulty)
            continue;

        if (difficulty->Flags & DIFFICULTY_FLAG_DEFAULT)
            return &p.second;
    }

    return &itr->second.begin()->second;
}


MapDifficulty const* GetMapDifficultyData(uint32 mapId, Difficulty difficulty)
{
    auto itr = sMapDifficultyMap.find(mapId);
    if (itr == sMapDifficultyMap.end())
        return nullptr;

    auto diffItr = itr->second.find(difficulty);
    if (diffItr == itr->second.end())
        return nullptr;

    return &diffItr->second;
}

MapDifficulty const* GetDownscaledMapDifficultyData(uint32 mapId, Difficulty &difficulty)
{
    DifficultyEntry const* diffEntry = sDifficultyStore.LookupEntry(difficulty);
    if (!diffEntry)
        return GetDefaultMapDifficulty(mapId);

    uint32 tmpDiff = difficulty;
    MapDifficulty const* mapDiff = GetMapDifficultyData(mapId, Difficulty(tmpDiff));
    while (!mapDiff)
    {
        tmpDiff = diffEntry->FallbackDifficultyID;
        diffEntry = sDifficultyStore.LookupEntry(tmpDiff);
        if (!diffEntry)
            return GetDefaultMapDifficulty(mapId);

        // pull new data
        mapDiff = GetMapDifficultyData(mapId, Difficulty(tmpDiff)); // we are 10 normal or 25 normal
    }

    difficulty = Difficulty(tmpDiff);
    return mapDiff;
}

PvPDifficultyEntry const* GetBattlegroundBracketByLevel(uint32 mapid, uint32 level)
{
    PvPDifficultyEntry const* maxEntry = NULL;              // used for level > max listed level case
    for (PvPDifficultyEntry const* entry : sPvPDifficultyStore)
    {
        // skip unrelated and too-high brackets
        if (entry->mapId != mapid || entry->minLevel > level)
            continue;

        // exactly fit
        if (entry->maxLevel >= level)
            return entry;

        // remember for possible out-of-range case (search higher from existed)
        if (!maxEntry || maxEntry->maxLevel < entry->maxLevel)
            maxEntry = entry;
    }

    return maxEntry;
}

PvPDifficultyEntry const* GetBattlegroundBracketById(uint32 mapid, BattlegroundBracketId id)
{
    for (PvPDifficultyEntry const* entry : sPvPDifficultyStore)
        if (entry->mapId == mapid && entry->GetBracketId() == id)
            return entry;

    return NULL;
}

std::vector<uint32> const* GetTalentTreePrimarySpells(uint32 talentTree)
{
    return NULL;
    /*TalentTreePrimarySpellsMap::const_iterator itr = sTalentTreePrimarySpellsMap.find(talentTree);
    if (itr == sTalentTreePrimarySpellsMap.end())
        return NULL;

    return &itr->second;*/
}

uint32 GetLiquidFlags(uint32 liquidType)
{
    if (LiquidTypeEntry const* liq = sLiquidTypeStore.LookupEntry(liquidType))
        return 1 << liq->Type;

    return 0;
}

float GetCurrencyPrecision(uint32 currencyId)
{
    CurrencyTypesEntry const * entry = sCurrencyTypesStore.LookupEntry(currencyId);

    return entry ? entry->GetPrecision() : 1.0f;
}

ResearchSiteEntry const* GetResearchSiteEntryById(uint32 id)
{
    ResearchSiteDataMap::const_iterator itr = sResearchSiteDataMap.find(id);
    if (itr == sResearchSiteDataMap.end())
        return NULL;

    return itr->second.entry;
}

bool MapEntry::IsDifficultyModeSupported(uint32 difficulty) const
{
    return IsValidDifficulty(difficulty, IsRaid());
}

bool IsValidDifficulty(uint32 diff, bool isRaid)
{
    if (diff == DIFFICULTY_NONE)
        return true;

    switch (diff)
    {
        case DIFFICULTY_NORMAL:
        case DIFFICULTY_HEROIC:
        case DIFFICULTY_N_SCENARIO:
        case DIFFICULTY_HC_SCENARIO:
        case DIFFICULTY_CHALLENGE:
            return !isRaid;
        default:
            break;
    }

    return isRaid;
}

uint32 GetQuestUniqueBitFlag(uint32 questId)
{
    QuestV2Entry const* v2 = sQuestV2Store.LookupEntry(questId);
    if (!v2)
        return 0;

    return v2->UniqueBitFlag;
}

bool IsScenarioCriteriaTree(uint32 criteriaTreeId)
{
    return sScenarioCriteriaTreeStore.find(criteriaTreeId) != sScenarioCriteriaTreeStore.end();
}

std::vector<uint32> GetItemLoadOutItems(uint32 LoadOutID)
{
    return sCharacterLoadoutItemMap[LoadOutID];
}

AreaTableEntry const* FindAreaEntry(uint32 area)
{
    auto data = sAreaEntry.find(area);
    if (data == sAreaEntry.end())
        return NULL;
    return data->second;
}