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

#include "DB2Stores.h"
#include "Common.h"
#include "Containers.h"
#include "DBCStores.h"
#include "DB2fmt.h"
#include "Log.h"
#include "World.h"

DB2Storage<AreaGroupEntry>                  sAreaGroupStore("AreaGroup.db2", AreaGroupFormat, HOTFIX_SEL_AREA_GROUP);
DB2Storage<AreaGroupMemberEntry>            sAreaGroupMemberStore("AreaGroupMember.db2", AreaGroupMemberFormat, HOTFIX_SEL_AREA_GROUP_MEMBER);
DB2Storage<BattlePetAbilityEffectEntry>     sBattlePetAbilityEffectStore("BattlePetAbilityEffect.db2", BattlePetAbilityEffectFormat, HOTFIX_SEL_BATTLE_PET_ABILITY_EFFECT);
DB2Storage<BattlePetAbilityEntry>           sBattlePetAbilityStore("BattlePetAbility.db2", BattlePetAbilityFormat, HOTFIX_SEL_BATTLE_PET_ABILITY);
DB2Storage<BattlePetAbilityStateEntry>      sBattlePetAbilityStateStore("BattlePetAbilityState.db2", BattlePetAbilityStateFormat, HOTFIX_SEL_BATTLE_PET_ABILITY_STATE);
DB2Storage<BattlePetAbilityTurnEntry>       sBattlePetAbilityTurnStore("BattlePetAbilityTurn.db2", BattlePetAbilityTurnFormat, HOTFIX_SEL_BATTLE_PET_ABILITY_TURN);
DB2Storage<BattlePetBreedQualityEntry>      sBattlePetBreedQualityStore("BattlePetBreedQuality.db2", BattlePetBreedQualityFormat, HOTFIX_SEL_BATTLE_PET_BREED_QUALITY);
DB2Storage<BattlePetBreedStateEntry>        sBattlePetBreedStateStore("BattlePetBreedState.db2", BattlePetBreedStateFormat, HOTFIX_SEL_BATTLE_PET_BREED_STATE);
DB2Storage<BattlePetEffectPropertiesEntry>  sBattlePetEffectPropertiesStore("BattlePetEffectProperties.db2", BattlePetEffectPropertiesFormat, HOTFIX_SEL_BATTLE_PET_EFFECT_PROPERTIES);
DB2Storage<BattlePetSpeciesEntry>           sBattlePetSpeciesStore("BattlePetSpecies.db2", BattlePetSpeciesFormat, HOTFIX_SEL_BATTLE_PET_SPECIES);
DB2Storage<BattlePetSpeciesStateEntry>      sBattlePetSpeciesStateStore("BattlePetSpeciesState.db2", BattlePetSpeciesStateFormat, HOTFIX_SEL_BATTLE_PET_SPECIES_STATE);
DB2Storage<BattlePetSpeciesXAbilityEntry>   sBattlePetSpeciesXAbilityStore("BattlePetSpeciesXAbility.db2", BattlePetSpeciesXAbilityFormat, HOTFIX_SEL_BATTLE_PET_SPECIES_X_ABILITY);
DB2Storage<BattlePetStateEntry>             sBattlePetStateStore("BattlePetState.db2", BattlePetStateFormat, HOTFIX_SEL_BATTLE_PET_STATE);
DB2Storage<BroadcastTextEntry>              sBroadcastTextStore("BroadcastText.db2", BroadcastTextFormat, HOTFIX_SEL_BROADCAST_TEXT);
DB2Storage<CurrencyTypesEntry>              sCurrencyTypesStore("CurrencyTypes.db2", CurrencyTypesFormat, HOTFIX_SEL_CURRENCY_TYPES);
DB2Storage<CurvePointEntry>                 sCurvePointStore("CurvePoint.db2", CurvePointFormat, HOTFIX_SEL_CURVE_POINT);
DB2Storage<GameObjectsEntry>                sGameObjectsStore("GameObjects.db2", GameObjectsFormat, HOTFIX_SEL_GAME_OBJECTS);
DB2Storage<GarrAbilityEntry>                sGarrAbilityStore("GarrAbility.db2", GarrAbilityFormat, HOTFIX_SEL_GARR_ABILITY);
DB2Storage<GarrBuildingEntry>               sGarrBuildingStore("GarrBuilding.db2", GarrBuildingFormat, HOTFIX_SEL_GARR_BUILDING);
DB2Storage<GarrBuildingPlotInstEntry>       sGarrBuildingPlotInstStore("GarrBuildingPlotInst.db2", GarrBuildingPlotInstFormat, HOTFIX_SEL_GARR_BUILDING_PLOT_INST);
DB2Storage<GarrClassSpecEntry>              sGarrClassSpecStore("GarrClassSpec.db2", GarrClassSpecFormat, HOTFIX_SEL_GARR_CLASS_SPEC);
DB2Storage<GarrFollowerEntry>               sGarrFollowerStore("GarrFollower.db2", GarrFollowerFormat, HOTFIX_SEL_GARR_FOLLOWER);
DB2Storage<GarrFollowerXAbilityEntry>       sGarrFollowerXAbilityStore("GarrFollowerXAbility.db2", GarrFollowerXAbilityFormat, HOTFIX_SEL_GARR_FOLLOWER_X_ABILITY);
DB2Storage<GarrPlotBuildingEntry>           sGarrPlotBuildingStore("GarrPlotBuilding.db2", GarrPlotBuildingFormat, HOTFIX_SEL_GARR_PLOT_BUILDING);
DB2Storage<GarrPlotEntry>                   sGarrPlotStore("GarrPlot.db2", GarrPlotFormat, HOTFIX_SEL_GARR_PLOT);
DB2Storage<GarrPlotInstanceEntry>           sGarrPlotInstanceStore("GarrPlotInstance.db2", GarrPlotInstanceFormat, HOTFIX_SEL_GARR_PLOT_INSTANCE);
DB2Storage<GarrSiteLevelEntry>              sGarrSiteLevelStore("GarrSiteLevel.db2", GarrSiteLevelFormat, HOTFIX_SEL_GARR_SITE_LEVEL);
DB2Storage<GarrSiteLevelPlotInstEntry>      sGarrSiteLevelPlotInstStore("GarrSiteLevelPlotInst.db2", GarrSiteLevelPlotInstFormat, HOTFIX_SEL_GARR_SITE_LEVEL_PLOT_INST);
DB2Storage<HolidaysEntry>                   sHolidaysStore("Holidays.db2", HolidaysFormat, HOTFIX_SEL_HOLIDAYS);
DB2Storage<ItemAppearanceEntry>             sItemAppearanceStore("ItemAppearance.db2", ItemAppearanceFormat, HOTFIX_SEL_ITEM_APPEARANCE);
DB2Storage<ItemBonusEntry>                  sItemBonusStore("ItemBonus.db2", ItemBonusFormat, HOTFIX_SEL_ITEM_BONUS);
DB2Storage<ItemBonusTreeNodeEntry>          sItemBonusTreeNodeStore("ItemBonusTreeNode.db2", ItemBonusTreeNodeFormat, HOTFIX_SEL_ITEM_BONUS_TREE_NODE);
DB2Storage<ItemCurrencyCostEntry>           sItemCurrencyCostStore("ItemCurrencyCost.db2", ItemCurrencyCostFormat, HOTFIX_SEL_ITEM_CURRENCY_COST);
DB2Storage<ItemEffectEntry>                 sItemEffectStore("ItemEffect.db2", ItemEffectFormat, HOTFIX_SEL_ITEM_EFFECT);
DB2Storage<ItemEntry>                       sItemStore("Item.db2", ItemFormat, HOTFIX_SEL_ITEM);
DB2Storage<ItemExtendedCostEntry>           sItemExtendedCostStore("ItemExtendedCost.db2", ItemExtendedCostFormat, HOTFIX_SEL_ITEM_EXTENDED_COST);
DB2Storage<ItemModifiedAppearanceEntry>     sItemModifiedAppearanceStore("ItemModifiedAppearance.db2", ItemModifiedAppearanceFormat, HOTFIX_SEL_ITEM_MODIFIED_APPEARANCE);
DB2Storage<ItemSparseEntry>                 sItemSparseStore("Item-sparse.db2", ItemSparseFormat, HOTFIX_SEL_ITEM_SPARSE);
DB2Storage<ItemUpgradeEntry>                sItemUpgradeStore("ItemUpgrade.db2", ItemUpgradeFormat, HOTFIX_SEL_ITEM_UPGRADE);
DB2Storage<ItemXBonusTreeEntry>             sItemXBonusTreeStore("ItemXBonusTree.db2", ItemXBonusTreeFormat, HOTFIX_SEL_ITEM_X_BONUS_TREE);
DB2Storage<KeyChainEntry>                   sKeyChainStore("KeyChain.db2", KeyChainFormat, HOTFIX_SEL_KEY_CHAIN);
DB2Storage<LanguageWordsEntry>              sLanguageWordsStore("LanguageWords.db2", LanguageWordsFormat, HOTFIX_SEL_LANGUAGE_WORDS);
DB2Storage<MapChallengeModeEntry>           sMapChallengeModeStore("MapChallengeMode.db2", MapChallengeModeFormat, HOTFIX_SEL_MAP_CHALLENGE_MODE);
DB2Storage<MountEntry>                      sMountStore("Mount.db2", MountFormat, HOTFIX_SEL_MOUNT);
DB2Storage<OverrideSpellDataEntry>          sOverrideSpellDataStore("OverrideSpellData.db2", OverrideSpellDataFormat, HOTFIX_SEL_OVERRIDE_SPELL_DATA);
DB2Storage<PhaseGroupEntry>                 sPhaseGroupStore("PhaseXPhaseGroup.db2", PhaseGroupFormat, HOTFIX_SEL_PHASE_GROUP);
DB2Storage<QuestPackageItemEntry>           sQuestPackageItemStore("QuestPackageItem.db2", QuestPackageItemFormat, HOTFIX_SEL_QUEST_PACKAGE_ITEM);
DB2Storage<RuleSetItemUpgradeEntry>         sRuleSetItemUpgradeEntryStore("RulesetItemUpgrade.db2", RuleSetItemUpgradeFormat, HOTFIX_SEL_RULE_SET_ITEM_UPGRADE);
DB2Storage<SoundEntriesEntry>               sSoundEntriesStore("SoundEntries.db2", SoundEntriesFormat, HOTFIX_SEL_SOUND_ENTRIES);
DB2Storage<SpellAuraRestrictionsEntry>      sSpellAuraRestrictionsStore("SpellAuraRestrictions.db2", SpellAuraRestrictionsFormat, HOTFIX_SEL_SPELL_AURA_RESTRICTIONS);
DB2Storage<SpellCastingRequirementsEntry>   sSpellCastingRequirementsStore("SpellCastingRequirements.db2", SpellCastingRequirementsFormat, HOTFIX_SEL_SPELL_CASTING_REQUIREMENTS);
DB2Storage<SpellClassOptionsEntry>          sSpellClassOptionsStore("SpellClassOptions.db2", SpellClassOptionsFormat, HOTFIX_SEL_SPELL_CLASS_OPTIONS);
DB2Storage<SpellMiscEntry>                  sSpellMiscStore("SpellMisc.db2", SpellMiscFormat, HOTFIX_SEL_SPELL_MISC);
DB2Storage<SpellPowerEntry>                 sSpellPowerStore("SpellPower.db2", SpellPowerFormat, HOTFIX_SEL_SPELL_POWER);
DB2Storage<SpellReagentsEntry>              sSpellReagentsStore("SpellReagents.db2", SpellReagentsFormat, HOTFIX_SEL_SPELL_REAGENTS);
DB2Storage<SpellRuneCostEntry>              sSpellRuneCostStore("SpellRuneCost.db2", SpellRuneCostFormat, HOTFIX_SEL_SPELL_RUNE_COST);
DB2Storage<SpellTotemsEntry>                sSpellTotemsStore("SpellTotems.db2", SpellTotemsFormat, HOTFIX_SEL_SPELL_TOTEMS);
DB2Storage<SpellVisualEntry>                sSpellVisualStore("SpellVisual.db2", SpellVisualFormat, HOTFIX_SEL_SPELL_VISUAL);
DB2Storage<TaxiNodesEntry>                  sTaxiNodesStore("TaxiNodes.db2", TaxiNodesFormat, HOTFIX_SEL_TAXI_NODES);
DB2Storage<TaxiPathEntry>                   sTaxiPathStore("TaxiPath.db2", TaxiPathFormat, HOTFIX_SEL_TAXI_PATH);
DB2Storage<TaxiPathNodeEntry>               sTaxiPathNodeStore("TaxiPathNode.db2", TaxiPathNodeFormat, HOTFIX_SEL_TAXI_PATH_NODE);
DB2Storage<ToyEntry>                        sToyStore("Toy.db2", ToyFormat, HOTFIX_SEL_TOY);

TaxiMask                                    sAllianceTaxiNodesMask;
TaxiMask                                    sDeathKnightTaxiNodesMask;
TaxiMask                                    sHordeTaxiNodesMask;
TaxiMask                                    sOldContinentsNodesMask;
TaxiMask                                    sTaxiNodesMask;
TaxiPathDestList                            sTaxiPathDestList;
TaxiPathNodesByPath                         sTaxiPathNodesByPath;
TaxiPathSetBySource                         sTaxiPathSetBySource;

uint32 DB2FilesCount = 0;
typedef std::list<std::string> DB2StoreProblemList;

static bool LoadDB2_assert_print(uint32 fsize, uint32 rsize, std::string const& filename)
{
    sLog->outError(LOG_FILTER_GENERAL, "Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).", filename.c_str(), fsize, rsize);

    // ASSERT must fail after function call
    return false;
}

template<class T>
inline void LoadDB2(uint32& availableDb2Locales, DB2StoreProblemList& errlist, DB2Manager::StorageMap& stores, DB2Storage<T>* storage, std::string const& db2Path, uint32 defaultLocale)
{
    // compatibility format and C++ structure sizes
    ASSERT(DB2FileLoader::GetFormatRecordSize(storage->GetFormat()) == sizeof(T) || LoadDB2_assert_print(DB2FileLoader::GetFormatRecordSize(storage->GetFormat()), sizeof(T), storage->GetFileName().c_str()));

    ++DB2FilesCount;

    if (storage->Load(db2Path + localeNames[defaultLocale] + '/', defaultLocale))
    {
        storage->LoadFromDB();

        for (uint32 i = 0; i < TOTAL_LOCALES; ++i)
        {
            if (availableDb2Locales & (1 << i))
                if (!storage->LoadStringsFrom((db2Path + localeNames[i] + '/'), i))
                    availableDb2Locales &= ~(1 << i);             // mark as not available for speedup next checks

            storage->LoadStringsFromDB(i);
        }
    }
    else
    {
        // sort problematic db2 to (1) non compatible and (2) nonexistent
        if (FILE* f = fopen((db2Path + storage->GetFileName()).c_str(), "rb"))
        {
            std::ostringstream stream;
            stream << storage->GetFileName() << " exists, and has " << storage->GetFieldCount() << " field(s) (expected " << strlen(storage->GetFormat())
                << "). Extracted file might be from wrong client version.";
            std::string buf = stream.str();
            errlist.push_back(buf);
            fclose(f);
        }
        else
            errlist.push_back(storage->GetFileName());
    }

    stores[storage->GetHash()] = storage;
}

void DB2Manager::LoadStores(std::string const& dataPath, uint32 defaultLocale)
{
    uint32 oldMSTime = getMSTime();

    std::string db2Path = dataPath + "dbc/";

    DB2StoreProblemList bad_db2_files;
    uint32 availableDb2Locales = 0xFF;

#define LOAD_DB2(store) LoadDB2(availableDb2Locales, bad_db2_files, _stores, &store, db2Path, defaultLocale)

    LOAD_DB2(sAreaGroupMemberStore);
    LOAD_DB2(sAreaGroupStore);
    LOAD_DB2(sBattlePetAbilityEffectStore);
    LOAD_DB2(sBattlePetAbilityStateStore);
    //LOAD_DB2(sBattlePetAbilityStore);
    LOAD_DB2(sBattlePetAbilityTurnStore);
    LOAD_DB2(sBattlePetBreedQualityStore);
    LOAD_DB2(sBattlePetBreedStateStore);
    LOAD_DB2(sBattlePetEffectPropertiesStore);
    LOAD_DB2(sBattlePetSpeciesStateStore);
    LOAD_DB2(sBattlePetSpeciesStore);
    LOAD_DB2(sBattlePetSpeciesXAbilityStore);
    LOAD_DB2(sBattlePetStateStore);
    LOAD_DB2(sBroadcastTextStore);
    LOAD_DB2(sCurrencyTypesStore);
    LOAD_DB2(sCurvePointStore);
    LOAD_DB2(sGameObjectsStore);
    LOAD_DB2(sGarrAbilityStore);
    LOAD_DB2(sGarrBuildingStore);
    LOAD_DB2(sGarrBuildingPlotInstStore);
    LOAD_DB2(sGarrClassSpecStore);
    LOAD_DB2(sGarrFollowerStore);
    LOAD_DB2(sGarrFollowerXAbilityStore);
    LOAD_DB2(sGarrPlotBuildingStore);
    LOAD_DB2(sGarrPlotInstanceStore);
    LOAD_DB2(sGarrPlotStore);
    LOAD_DB2(sGarrSiteLevelPlotInstStore);
    LOAD_DB2(sGarrSiteLevelStore);
    LOAD_DB2(sHolidaysStore);
    LOAD_DB2(sItemAppearanceStore);
    LOAD_DB2(sItemBonusStore);
    LOAD_DB2(sItemBonusTreeNodeStore);
    LOAD_DB2(sItemCurrencyCostStore);
    LOAD_DB2(sItemEffectStore);
    LOAD_DB2(sItemExtendedCostStore);
    LOAD_DB2(sItemModifiedAppearanceStore);
    LOAD_DB2(sItemSparseStore);
    LOAD_DB2(sItemStore);
    LOAD_DB2(sItemUpgradeStore);
    LOAD_DB2(sKeyChainStore);
    LOAD_DB2(sItemXBonusTreeStore);
    //LOAD_DB2(sLanguageWordsStore);
    LOAD_DB2(sMapChallengeModeStore);
    LOAD_DB2(sMountStore);
    LOAD_DB2(sOverrideSpellDataStore);
    LOAD_DB2(sPhaseGroupStore);
    LOAD_DB2(sQuestPackageItemStore);
    LOAD_DB2(sRuleSetItemUpgradeEntryStore);
    LOAD_DB2(sSoundEntriesStore);
    LOAD_DB2(sSpellAuraRestrictionsStore);
    LOAD_DB2(sSpellCastingRequirementsStore);
    LOAD_DB2(sSpellClassOptionsStore);
    LOAD_DB2(sSpellMiscStore);
    LOAD_DB2(sSpellPowerStore);
    LOAD_DB2(sSpellReagentsStore);
    LOAD_DB2(sSpellRuneCostStore);
    LOAD_DB2(sSpellTotemsStore);
    LOAD_DB2(sSpellVisualStore);
    LOAD_DB2(sTaxiNodesStore);
    LOAD_DB2(sTaxiPathNodeStore);
    LOAD_DB2(sTaxiPathStore);
    LOAD_DB2(sToyStore);

#undef LOAD_DB2

    InitDB2CustomStores();

    // error checks
    if (bad_db2_files.size() >= DB2FilesCount)
    {
        sLog->outError(LOG_FILTER_GENERAL, "\nIncorrect DataDir value in worldserver.conf or ALL required *.db2 files (%d) not found by path: %sdbc/%s/", DB2FilesCount, dataPath.c_str(), localeNames[defaultLocale]);
        exit(1);
    }
    else if (!bad_db2_files.empty())
    {
        std::string str;
        for (std::list<std::string>::iterator i = bad_db2_files.begin(); i != bad_db2_files.end(); ++i)
            str += *i + "\n";

        sLog->outError(LOG_FILTER_GENERAL, "\nSome required *.db2 files (%u from %d) not found or not compatible:\n%s", (uint32)bad_db2_files.size(), DB2FilesCount, str.c_str());
        exit(1);
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Initialized %d DB2 data stores in %u ms", DB2FilesCount, GetMSTimeDiffToNow(oldMSTime));
}

void DB2Manager::InitDB2CustomStores()
{
    for (BattlePetSpeciesEntry const* entry : sBattlePetSpeciesStore)
    {
        // Ruby Droplet DBC fix
        if (entry->CreatureEntry == 73356)
            const_cast<BattlePetSpeciesEntry*>(entry)->spellId = 148050;

        _battlePetSpeciesBySpellId[entry->CreatureEntry] = entry;
    }

    for (LanguageWordsEntry const* entry : sLanguageWordsStore)
        sLanguageWordsMapStore[entry->langId][strlen(entry->word[DEFAULT_LOCALE].Str[DEFAULT_LOCALE])].push_back(entry->word[DEFAULT_LOCALE].Str[DEFAULT_LOCALE]);

    for (ToyEntry const* toy : sToyStore)
        _toys.push_back(toy->ItemID);

    for (AreaGroupMemberEntry const* areaGroupMember : sAreaGroupMemberStore)
        _areaGroupMembers[areaGroupMember->AreaGroupID].push_back(areaGroupMember->AreaID);

    for (ItemBonusEntry const* bonus : sItemBonusStore)
        _itemBonusLists[bonus->BonusListID].push_back(bonus);

    for (ItemBonusTreeNodeEntry const* bonusTreeNode : sItemBonusTreeNodeStore)
        _itemBonusTrees[bonusTreeNode->BonusTreeID].insert(bonusTreeNode);

    for (ItemModifiedAppearanceEntry const* appearanceMod : sItemModifiedAppearanceStore)
        if (ItemAppearanceEntry const* appearance = sItemAppearanceStore.LookupEntry(appearanceMod->AppearanceID))
            _itemDisplayIDs[appearanceMod->ItemID | (appearanceMod->AppearanceModID << 24)] = appearance->DisplayID;

    {
        std::set<uint32> scalingCurves;
        for (ScalingStatDistributionEntry const* ssd : sScalingStatDistributionStore)
            scalingCurves.insert(ssd->ItemLevelCurveID);

        for (CurvePointEntry const* curvePoint : sCurvePointStore)
            if (scalingCurves.count(curvePoint->CurveID))
                _heirloomCurvePoints[curvePoint->CurveID][curvePoint->Index] = curvePoint;
    }

    for (MapChallengeModeEntry const* entry : sMapChallengeModeStore)
        _mapChallengeModeEntrybyMap[entry->map] = entry;

    for (ItemXBonusTreeEntry const* itemBonusTreeAssignment : sItemXBonusTreeStore)
        _itemToBonusTree[itemBonusTreeAssignment->ItemID].push_back(itemBonusTreeAssignment);

    for (QuestPackageItemEntry const* questPackageItem : sQuestPackageItemStore)
        _questPackages[questPackageItem->QuestPackageID].push_back(questPackageItem);

    for (GameObjectsEntry const* store : sGameObjectsStore)
        sGameObjectsList.push_back(store->ID);

    for (RuleSetItemUpgradeEntry const* rsiu : sRuleSetItemUpgradeEntryStore)
    {
        ItemUpgradeDataMap::iterator itr = _itemUpgradeDataMap.find(rsiu->itemEntry);
        if (itr != _itemUpgradeDataMap.end())
            continue;

        ItemUpgradeData& data = _itemUpgradeDataMap[rsiu->itemEntry];

        uint32 offs = 0;
        uint32 prevUpd = 0;
        for (uint32 j = 0; j < sItemUpgradeStore.GetNumRows(); ++j)
        {
            ItemUpgradeEntry const* ue = sItemUpgradeStore.LookupEntry(j);
            if (!ue)
                continue;

            if (!prevUpd)
            {
                if (ue->id == rsiu->startUpgrade)
                {
                    prevUpd = ue->id;
                    data.upgrade[offs++] = ue;
                    j = 0;
                }
            }
            else if (ue->prevUpgradeId == prevUpd)
            {
                prevUpd = ue->id;
                data.upgrade[offs++] = ue;
                j = 0;
            }
        }
    }


    //! taxi system under this - req full rework since 6.1.0
    for (uint32 i = 1; i < sTaxiPathStore.GetNumRows(); ++i)
        if (TaxiPathEntry const* entry = sTaxiPathStore.LookupEntry(i))
            sTaxiPathSetBySource[entry->From][entry->To] = TaxiPathBySourceAndDestination(entry->ID, entry->Cost);

    // Generate multy-path
    for (TaxiPathSetBySource::iterator itr = sTaxiPathSetBySource.begin(); itr != sTaxiPathSetBySource.end(); ++itr)
        FillPathDestList(itr->first, itr->first);

    //for (TaxiPathDestList::iterator itr = sTaxiPathDestList.begin(); itr != sTaxiPathDestList.end(); ++itr)
    //    for(TaxiDestList::iterator r = itr->second.begin(); r != itr->second.end(); ++r)
    //    {
    //        std::ostringstream ss;
    //        ss << "from:" << itr->first << " to:" << r->first << " - ";
    //        for(TaxiPatchList::iterator b = r->second.begin(); b != r->second.end();++b)
    //            ss << (*b) << " ";
    //        sLog->outU(">>> %s", ss.str().c_str());
    //    }
    uint32 pathCount = sTaxiPathStore.GetNumRows();

    // Calculate path nodes count
    std::vector<uint32> pathLength;
    pathLength.resize(pathCount);                           // 0 and some other indexes not used
    for (uint32 i = 1; i < sTaxiPathNodeStore.GetNumRows(); ++i)
    {
        if (TaxiPathNodeEntry const* entry = sTaxiPathNodeStore.LookupEntry(i))
        {
            if (pathLength[entry->PathID] < entry->NodeIndex + 1)
                pathLength[entry->PathID] = entry->NodeIndex + 1;
        }
    }

    // Set path length
    sTaxiPathNodesByPath.resize(pathCount);                 // 0 and some other indexes not used
    for (uint32 i = 1; i < sTaxiPathNodesByPath.size(); ++i)
        sTaxiPathNodesByPath[i].resize(pathLength[i]);

    // fill data
    for (uint32 i = 1; i < sTaxiPathNodeStore.GetNumRows(); ++i)
        if (TaxiPathNodeEntry const* entry = sTaxiPathNodeStore.LookupEntry(i))
            sTaxiPathNodesByPath[entry->PathID].set(entry->NodeIndex, entry);

    // Initialize global taxinodes mask
    // include existed nodes that have at least single not spell base (scripted) path
    {
        std::set<uint32> spellPaths;
        for (uint32 i = 1; i < sSpellEffectStore.GetNumRows(); ++i)
            if (SpellEffectEntry const* sInfo = sSpellEffectStore.LookupEntry(i))
                if (sInfo->Effect == SPELL_EFFECT_SEND_TAXI)
                    spellPaths.insert(sInfo->EffectMiscValue);

        memset(sTaxiNodesMask, 0, sizeof(sTaxiNodesMask));
        memset(sOldContinentsNodesMask, 0, sizeof(sOldContinentsNodesMask));
        memset(sHordeTaxiNodesMask, 0, sizeof(sHordeTaxiNodesMask));
        memset(sAllianceTaxiNodesMask, 0, sizeof(sAllianceTaxiNodesMask));
        memset(sDeathKnightTaxiNodesMask, 0, sizeof(sDeathKnightTaxiNodesMask));
        for (uint32 i = 1; i < sTaxiNodesStore.GetNumRows(); ++i)
        {
            TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(i);
            if (!node)
                continue;

            TaxiPathSetBySource::const_iterator src_i = sTaxiPathSetBySource.find(i);
            if (src_i != sTaxiPathSetBySource.end() && !src_i->second.empty())
            {
                bool ok = false;
                for (TaxiPathSetForSource::const_iterator dest_i = src_i->second.begin(); dest_i != src_i->second.end(); ++dest_i)
                {
                    if (!dest_i->second.price)
                        continue;

                    // not spell path
                    if (spellPaths.find(dest_i->second.ID) == spellPaths.end())
                    {
                        ok = true;
                        break;
                    }
                }

                if (!ok)
                    continue;
            }

            // valid taxi network node
            uint8  field = (uint8)((i - 1) / 8);
            uint32 submask = 1 << ((i - 1) % 8);

            sTaxiNodesMask[field] |= submask;
            if (node->MountCreatureID[0] && node->MountCreatureID[0] != 32981)
                sHordeTaxiNodesMask[field] |= submask;
            if (node->MountCreatureID[1] && node->MountCreatureID[1] != 32981)
                sAllianceTaxiNodesMask[field] |= submask;
            if (node->MountCreatureID[0] == 32981 || node->MountCreatureID[1] == 32981)
                sDeathKnightTaxiNodesMask[field] |= submask;

            // old continent node (+ nodes virtually at old continents, check explicitly to avoid loading map files for zone info)
            if (node->MapID < 2 || i == 82 || i == 83 || i == 93 || i == 94)
                sOldContinentsNodesMask[field] |= submask;

            // fix DK node at Ebon Hold and Shadow Vault flight master
            if (i == 315 || i == 333)
                ((TaxiNodesEntry*)node)->MountCreatureID[1] = 32981;
        }
    }
}

DB2StorageBase const* DB2Manager::GetStorage(uint32 type) const
{
    StorageMap::const_iterator itr = _stores.find(type);
    if (itr != _stores.end())
        return itr->second;

    return nullptr;
}

void DB2Manager::LoadHotfixData()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = HotfixDatabase.Query("SELECT TableHash, RecordID, `Timestamp`, Deleted FROM hotfix_data");

    if (!result)
        return;

    uint32 count = 0;

    _hotfixData.reserve(result->GetRowCount());

    do
    {
        Field* fields = result->Fetch();

        HotfixNotify info;
        info.TableHash = fields[0].GetUInt32();
        info.Entry = fields[1].GetUInt32();
        info.Timestamp = fields[2].GetUInt32();
        _hotfixData.push_back(info);

        if (fields[3].GetBool())
        {
            auto itr = _stores.find(info.TableHash);
            if (itr != _stores.end())
                itr->second->EraseRecord(info.Entry);
        }

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u hotfix info entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

time_t DB2Manager::GetHotfixDate(uint32 entry, uint32 type) const
{
    time_t ret = 0;
    for (HotfixNotify const& hotfix : _hotfixData)
        if (hotfix.Entry == entry && hotfix.TableHash == type)
            if (time_t(hotfix.Timestamp) > ret)
                ret = time_t(hotfix.Timestamp);

    return ret ? ret : time(NULL);
}


void DB2Manager::FillPathDestList(uint32 from, uint32 prev)
{
    for (TaxiPathSetForSource::iterator itr = sTaxiPathSetBySource[prev].begin(); itr != sTaxiPathSetBySource[prev].end(); ++itr)
    {
        TaxiDestList::iterator list = sTaxiPathDestList[from].find(prev);
        if (list == sTaxiPathDestList[from].end())
            sTaxiPathDestList[from][prev].push_back(prev);                      //start

        // check if we already have it.
        list = sTaxiPathDestList[from].find(itr->first);
        if (list != sTaxiPathDestList[from].end())
        {
            //if current path smaller - skip 
            if (sTaxiPathDestList[from][itr->first].size() <= (sTaxiPathDestList[from][prev].size() + 1))
                continue;
        }

        sTaxiPathDestList[from][itr->first] = sTaxiPathDestList[from][prev];    //copy path from prev.
        sTaxiPathDestList[from][itr->first].push_back(itr->first);              //and add this to end.
        FillPathDestList(from, itr->first);
    }
}

std::vector<uint32> DB2Manager::GetAreasForGroup(uint32 areaGroupId)
{
    auto itr = _areaGroupMembers.find(areaGroupId);
    if (itr != _areaGroupMembers.end())
        return itr->second;

    return std::vector<uint32>();
}

std::list<uint32> DB2Manager::GetGameObjectsList()
{
    return sGameObjectsList;
}

ItemUpgradeData const* DB2Manager::GetItemUpgradeData(uint32 itemEntry)
{
    ItemUpgradeDataMap::iterator itr = _itemUpgradeDataMap.find(itemEntry);
    if (itr == _itemUpgradeDataMap.end())
        return NULL;

    return &itr->second;
}

uint32 DB2Manager::GetHeirloomItemLevel(uint32 curveId, uint32 level) const
{
    // Assuming linear item level scaling for heirlooms
    auto itr = _heirloomCurvePoints.find(curveId);
    if (itr == _heirloomCurvePoints.end())
        return 0;

    auto it2 = itr->second.begin(); // Highest scaling point
    if (level >= it2->second->X)
        return it2->second->Y;

    auto previousItr = it2++;
    for (; it2 != itr->second.end(); ++it2, ++previousItr)
        if (level >= it2->second->X)
            return uint32((previousItr->second->Y - it2->second->Y) / (previousItr->second->X - it2->second->X) * (float(level) - it2->second->X) + it2->second->Y);

    return uint32(previousItr->second->Y);  // Lowest scaling point
}

uint32 DB2Manager::GetItemDisplayId(uint32 itemId, uint32 appearanceModId) const
{
    if (!itemId)
        return 0;

    auto itr = _itemDisplayIDs.find(itemId | (appearanceModId << 24));
    if (itr != _itemDisplayIDs.end())
        return itr->second;

    // Fall back to unmodified appearance
    if (appearanceModId)
    {
        itr = _itemDisplayIDs.find(itemId);
        if (itr != _itemDisplayIDs.end())
            return itr->second;
    }

    return 0;
}

DB2Manager::ItemBonusList const* DB2Manager::GetItemBonusList(uint32 bonusListId) const
{
    auto itr = _itemBonusLists.find(bonusListId);
    if (itr != _itemBonusLists.end())
        return &itr->second;

    return nullptr;
}

DB2Manager::LanguageWordsMap const* DB2Manager::GetLanguageWordMap(uint32 langID)
{
    std::map<uint32, LanguageWordsMap>::const_iterator itr = sLanguageWordsMapStore.find(langID);
    return itr != sLanguageWordsMapStore.end() ? &itr->second : NULL;
}

DB2Manager::LanguageWordsSize const* DB2Manager::GetLanguageWordsBySize(uint32 langID, uint32 size)
{
    LanguageWordsMap const* wordMap = GetLanguageWordMap(langID);
    if (!wordMap)
        return NULL;

    std::map<uint32, DB2Manager::LanguageWordsSize>::const_iterator itr = wordMap->find(size);
    return itr != wordMap->end() ? &itr->second : NULL;
}

BattlePetSpeciesEntry const* DB2Manager::GetBattlePetSpeciesEntry(uint32 creatureEntry)
{
    auto it = _battlePetSpeciesBySpellId.find(creatureEntry);
    if (it != _battlePetSpeciesBySpellId.end())
        return it->second;

    return NULL;
}

std::vector<QuestPackageItemEntry const*> const* DB2Manager::GetQuestPackageItems(uint32 questPackageID) const
{
    auto itr = _questPackages.find(questPackageID);
    if (itr != _questPackages.end())
        return &itr->second;

    return nullptr;
}

MountEntry const* DB2Manager::GetMountById(uint32 id) const
{
    return sMountStore.LookupEntry(id);
}

std::set<uint32> DB2Manager::GetItemBonusTree(uint32 itemId, uint32 itemBonusTreeMod) const
{
    std::set<uint32> bonusListIDs;
    auto itemIdRange = _itemToBonusTree.find(itemId);
    if (itemIdRange == _itemToBonusTree.end())
        return bonusListIDs;

    for (auto itemTreeItr : itemIdRange->second)
    {
        std::set<uint32> listBonus = GetFindBonusTree((*itemTreeItr).BonusTreeID, itemBonusTreeMod);
        bonusListIDs.insert(listBonus.begin(), listBonus.end());
    }

    return bonusListIDs;
}

std::set<uint32> DB2Manager::GetFindBonusTree(uint32 BonusTreeID, uint32 itemBonusTreeMod) const
{
    std::set<uint32> bonusListIDs;

    auto treeItr = _itemBonusTrees.find(BonusTreeID);
    if (treeItr == _itemBonusTrees.end())
        return bonusListIDs;

    for (auto bonusTreeNode : treeItr->second)
    {
        if (bonusTreeNode->BonusTreeModID == itemBonusTreeMod)
        {
            if(bonusTreeNode->BonusListID)
                bonusListIDs.insert(bonusTreeNode->BonusListID);
            else if(bonusTreeNode->SubTreeID)
            {
                std::set<uint32> listBonus = GetFindBonusTree(bonusTreeNode->SubTreeID, itemBonusTreeMod);
                bonusListIDs.insert(listBonus.begin(), listBonus.end());
            }
        }
    }

    return bonusListIDs;
}
