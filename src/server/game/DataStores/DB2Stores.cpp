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
#include "DBCStores.h"
#include "DB2Utility.h"
#include "Log.h"
#include "SharedDefines.h"
#include "SpellMgr.h"
#include "DB2fmt.h"
#include <iostream>
#include <fstream>
#include "DBCStructure.h"

#include <map>
#include <functional>

std::map<uint32 /*curveID*/, std::map<uint32/*index*/, CurvePointEntry const*, std::greater<uint32>>> HeirloomCurvePoints;
DB2Storage<CurvePointEntry>                 sCurvePointStore(CurvePointEntryfmt);
DB2Storage<BroadcastTextEntry>              sBroadcastTextStore(BroadcastTextEntryfmt, &DB2Utilities::HasBroadcastTextEntry, &DB2Utilities::WriteBroadcastTextDbReply);
DB2Storage<HolidaysEntry>                   sHolidaysStore(HolidaysEntryfmt);
DB2Storage<ItemEntry>                       sItemStore(Itemfmt, &DB2Utilities::HasItemEntry, &DB2Utilities::WriteItemDbReply);
DB2Storage<ItemAppearanceEntry>             sItemAppearanceStore(ItemAppearanceEntryfmt);
ItemDisplayIDMap                            sItemDisplayIDMap;
std::unordered_map<uint32 /*itemId | appearanceMod << 24*/, uint32> ItemDisplayMap;
DB2Storage<ItemBonusEntry>                  sItemBonusStore(ItemBonusEntryfmt);
std::unordered_map<uint32 /*bonusListId*/, std::vector<ItemBonusEntry const*>> ItemBonusLists;
DB2Storage<ItemCurrencyCostEntry>           sItemCurrencyCostStore(ItemCurrencyCostfmt);
DB2Storage<ItemExtendedCostEntry>           sItemExtendedCostStore(ItemExtendedCostEntryfmt);
DB2Storage<ItemEffectEntry>                 sItemEffectStore(ItemEffectEntryfmt);
DB2Storage<ItemModifiedAppearanceEntry>     sItemModifiedAppearanceStore(ItemModifiedAppearanceEntryfmt);
DB2Storage<ItemSparseEntry>                 sItemSparseStore (ItemSparsefmt, &DB2Utilities::HasItemSparseEntry, &DB2Utilities::WriteItemSparseDbReply);
DB2Storage<BattlePetSpeciesEntry>           sBattlePetSpeciesStore(BattlePetSpeciesEntryfmt);
DB2Storage<LanguageWordsEntry>              sLanguageWordsStore(LanguageWordsEntryfmt);
std::map<uint32 /*lang id*/, LanguageWordsMap> sLanguageWordsMapStore;
DB2Storage<QuestPackageItem>                sQuestPackageItemStore(QuestPackageItemfmt);
DB2Storage<SpellReagentsEntry>              sSpellReagentsStore(SpellReagentsEntryfmt);
DB2Storage<ItemUpgradeEntry>                sItemUpgradeStore(ItemUpgradeEntryfmt);
DB2Storage<RuleSetItemUpgradeEntry>         sRuleSetItemUpgradeEntryStore(RuleSetItemUpgradeEntryfmt);
DB2Storage<GameObjectsEntry>                sGameObjectsStore(GameObjectsEntryfmt);
DB2Storage<MapChallengeModeEntry>           sMapChallengeModeStore(MapChallengeModeEntryfmt);
DB2Storage<SpellVisualEntry>                sSpellVisualStore(SpellVisualEntryfmt);
DB2Storage<KeyChainEntry>                   sKeyChainStore(KeyChainfmt);
DB2Storage<OverrideSpellDataEntry>          sOverrideSpellDataStore(OverrideSpellDataEntryfmt);
DB2Storage<PhaseGroupEntry>                 sPhaseGroupStore(PhaseGroupEntryfmt);
DB2Storage<SpellAuraRestrictionsEntry>      sSpellAuraRestrictionsStore(SpellAuraRestrictionsEntryfmt);
DB2Storage<SpellCastingRequirementsEntry>   sSpellCastingRequirementsStore(SpellCastingRequirementsEntryfmt);
DB2Storage<SpellClassOptionsEntry>          sSpellClassOptionsStore(SpellClassOptionsEntryfmt);
DB2Storage<SpellMiscEntry>                  sSpellMiscStore(SpellMiscEntryfmt);
DB2Storage<SpellPowerEntry>                 sSpellPowerStore(SpellPowerEntryfmt);
DB2Storage<SpellRuneCostEntry>              sSpellRuneCostStore(SpellRuneCostEntryfmt);
DB2Storage<SpellTotemsEntry>                sSpellTotemsStore(SpellTotemsEntryfmt);
DB2Storage<TaxiNodesEntry>                  sTaxiNodesStore(TaxiNodesEntryfmt);
DB2Storage<TaxiPathEntry>                   sTaxiPathStore(TaxiPathEntryfmt);
DB2Storage<TaxiPathNodeEntry>               sTaxiPathNodeStore(TaxiPathNodeEntryfmt);
TaxiMask                                    sTaxiNodesMask;
TaxiMask                                    sOldContinentsNodesMask;
TaxiMask                                    sHordeTaxiNodesMask;
TaxiMask                                    sAllianceTaxiNodesMask;
TaxiMask                                    sDeathKnightTaxiNodesMask;
TaxiPathSetBySource                         sTaxiPathSetBySource;
TaxiPathNodesByPath                         sTaxiPathNodesByPath;
PhaseGroupContainer sPhasesByGroup;

typedef std::list<std::string> StoreProblemList1;
static std::map<uint32, std::list<uint32> > sPackageItemList;
std::list<uint32> sGameObjectsList;

ItemUpgradeDataMap sItemUpgradeDataMap;
BattlePetSpeciesBySpellIdMap sBattlePetSpeciesBySpellId;
MapChallengeModeEntryMap sMapChallengeModeEntrybyMap;

typedef std::map<uint32 /*hash*/, DB2StorageBase*> DB2StorageMap;
DB2StorageMap DB2Stores;

uint32 DB2FilesCount = 0;

static bool LoadDB2_assert_print(uint32 fsize,uint32 rsize, const std::string& filename)
{
    sLog->outError(LOG_FILTER_GENERAL, "Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).", filename.c_str(), fsize, rsize);

    // ASSERT must fail after function call
    return false;
}

struct LocalDB2Data
{
    LocalDB2Data(LocaleConstant loc) : defaultLocale(loc), availableDb2Locales(0xFFFFFFFF) {}

    LocaleConstant defaultLocale;

    // bitmasks for index of fullLocaleNameList
    uint32 availableDb2Locales;
};

template<class T>
inline void LoadDB2(StoreProblemList1& errlist, DB2Storage<T>& storage, const std::string& db2_path, const std::string& filename, std::string const* customFormat = NULL, std::string const* customIndexName = NULL)
{
    // compatibility format and C++ structure sizes
    ASSERT(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDB2_assert_print(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()), sizeof(T), filename));

    ++DB2FilesCount;

    std::string db2_filename = db2_path + filename;
    SqlDb2 * sql = NULL;
    if (customFormat)
        sql = new SqlDb2(&filename, customFormat, customIndexName, storage.GetFormat());

    if (!storage.Load(db2_filename.c_str(), sql))
    {
        // sort problematic db2 to (1) non compatible and (2) nonexistent
        if (FILE * f = fopen(db2_filename.c_str(), "rb"))
        {
            char buf[100];
            snprintf(buf, 100,"(exist, but have %d fields instead %u) Wrong client version DBC file?", storage.GetFieldCount(), strlen(storage.GetFormat()));
            errlist.push_back(db2_filename + buf);
            fclose(f);
        }
        else
            errlist.push_back(db2_filename);
    }

    DB2Stores[storage.GetHash()] = &storage;
    delete sql;
}

void LoadDB2Stores(const std::string& dataPath)
{
    std::string db2Path = dataPath + "dbc/";

    StoreProblemList1 bad_db2_files;

    LoadDB2(bad_db2_files, sBattlePetSpeciesStore,  db2Path, "BattlePetSpecies.db2");
    for (uint32 i = 0; i < sBattlePetSpeciesStore.GetNumRows(); ++i)
    {
        BattlePetSpeciesEntry const* entry = sBattlePetSpeciesStore.LookupEntry(i);
        if (!entry)
            continue;

        // Ruby Droplet DBC fix
        if (entry->CreatureEntry == 73356)
            const_cast<BattlePetSpeciesEntry*>(entry)->spellId = 148050;

        sBattlePetSpeciesBySpellId[entry->CreatureEntry] = entry;
    }

    LoadDB2(bad_db2_files, sCurvePointStore,           db2Path,    "CurvePoint.db2");//19342
    LoadDB2(bad_db2_files, sBroadcastTextStore,        db2Path,    "BroadcastText.db2");//19342
    LoadDB2(bad_db2_files, sHolidaysStore,             db2Path,    "Holidays.db2");
    LoadDB2(bad_db2_files, sItemStore,                 db2Path,    "Item.db2");//19342
    LoadDB2(bad_db2_files, sItemAppearanceStore,       db2Path,    "ItemAppearance.db2");//19342
    LoadDB2(bad_db2_files, sItemBonusStore,            db2Path,    "ItemBonus.db2");//19342
    LoadDB2(bad_db2_files, sItemCurrencyCostStore,     db2Path,    "ItemCurrencyCost.db2");
    LoadDB2(bad_db2_files, sItemSparseStore,           db2Path,    "Item-sparse.db2");//19342
    LoadDB2(bad_db2_files, sItemModifiedAppearanceStore, db2Path,  "ItemModifiedAppearance.db2");//19342
    LoadDB2(bad_db2_files, sItemExtendedCostStore,     db2Path,    "ItemExtendedCost.db2", &CustomItemExtendedCostEntryfmt, &CustomItemExtendedCostEntryIndex);//19342
    LoadDB2(bad_db2_files, sItemEffectStore,           db2Path,    "ItemEffect.db2");//19342
    LoadDB2(bad_db2_files, sLanguageWordsStore,        db2Path,    "LanguageWords.db2");//19342
    for (uint32 i = 0; i < sLanguageWordsStore.GetNumRows(); ++i)
    {
        LanguageWordsEntry const* entry = sLanguageWordsStore.LookupEntry(i);
        if (!entry)
            continue;

        sLanguageWordsMapStore[entry->langId][strlen(entry->word)].push_back(entry->word);
    }

    LoadDB2(bad_db2_files, sKeyChainStore,             db2Path,    "KeyChain.db2");
    LoadDB2(bad_db2_files, sOverrideSpellDataStore,    db2Path,    "OverrideSpellData.db2");
    LoadDB2(bad_db2_files, sPhaseGroupStore,           db2Path,    "PhaseXPhaseGroup.db2");
    LoadDB2(bad_db2_files, sSpellAuraRestrictionsStore, db2Path,   "SpellAuraRestrictions.db2");
    LoadDB2(bad_db2_files, sSpellCastingRequirementsStore, db2Path, "SpellCastingRequirements.db2");
    LoadDB2(bad_db2_files, sSpellClassOptionsStore,    db2Path,    "SpellClassOptions.db2");
    LoadDB2(bad_db2_files, sSpellMiscStore,            db2Path,    "SpellMisc.db2");
    LoadDB2(bad_db2_files, sSpellPowerStore,           db2Path,    "SpellPower.db2");
    LoadDB2(bad_db2_files, sSpellReagentsStore,        db2Path,    "SpellReagents.db2");
    LoadDB2(bad_db2_files, sSpellRuneCostStore,        db2Path,    "SpellRuneCost.db2");
    LoadDB2(bad_db2_files, sSpellTotemsStore,          db2Path,    "SpellTotems.db2");
    LoadDB2(bad_db2_files, sTaxiNodesStore,            db2Path,    "TaxiNodes.db2");
    LoadDB2(bad_db2_files, sTaxiPathStore,             db2Path,    "TaxiPath.db2");
    LoadDB2(bad_db2_files, sTaxiPathNodeStore,         db2Path,    "TaxiPathNode.db2");

    LoadDB2(bad_db2_files, sQuestPackageItemStore,     db2Path,    "QuestPackageItem.db2");
    LoadDB2(bad_db2_files, sGameObjectsStore,          db2Path,    "GameObjects.db2");//19342
    LoadDB2(bad_db2_files, sMapChallengeModeStore,     db2Path,    "MapChallengeMode.db2");
    LoadDB2(bad_db2_files, sSpellVisualStore,          db2Path,    "SpellVisual.db2");
    LoadDB2(bad_db2_files, sItemUpgradeStore,          db2Path,    "ItemUpgrade.db2");
    LoadDB2(bad_db2_files, sRuleSetItemUpgradeEntryStore,db2Path,  "RulesetItemUpgrade.db2");

    for (uint32 i = 0; i < sItemBonusStore.GetNumRows(); ++i)
        if (ItemBonusEntry const* bonus = sItemBonusStore.LookupEntry(i))
            ItemBonusLists[bonus->BonusListID].push_back(bonus);

    for (uint32 i = 0; i < sItemModifiedAppearanceStore.GetNumRows(); ++i)
        if (ItemModifiedAppearanceEntry const* appearanceMod = sItemModifiedAppearanceStore.LookupEntry(i))
            if (ItemAppearanceEntry const* appearance = sItemAppearanceStore.LookupEntry(appearanceMod->AppearanceID))
                ItemDisplayMap[appearanceMod->ItemID | (appearanceMod->AppearanceModID << 24)] = appearance->DisplayID;

    {
        std::set<uint32> scalingCurves;
        for (uint32 i = 0; i < sScalingStatDistributionStore.GetNumRows(); ++i)
            if (ScalingStatDistributionEntry const* ssd = sScalingStatDistributionStore.LookupEntry(i))
                scalingCurves.insert(ssd->ItemLevelCurveID);

        for (uint32 i = 0; i < sCurvePointStore.GetNumRows(); ++i)
            if (CurvePointEntry const* curvePoint = sCurvePointStore.LookupEntry(i))
                if (scalingCurves.count(curvePoint->CurveID))
                    HeirloomCurvePoints[curvePoint->CurveID][curvePoint->Index] = curvePoint;
    }

    for (uint32 i = 0; i < sMapChallengeModeStore.GetNumRows(); ++i)
    {
        MapChallengeModeEntry const* entry = sMapChallengeModeStore.LookupEntry(i);
        if (!entry)
            continue;

        sMapChallengeModeEntrybyMap[entry->map] = entry;
    }

    for (uint32 i = 0; i < sQuestPackageItemStore.GetNumRows(); ++i)
    {
        if (QuestPackageItem const* sp = sQuestPackageItemStore.LookupEntry(i))
            sPackageItemList[sp->packageEntry].push_back(i);
    }

    for (uint32 i = 0; i < sGameObjectsStore.GetNumRows(); ++i)
    {
        if (GameObjectsEntry const* goe = sGameObjectsStore.LookupEntry(i))
            sGameObjectsList.push_back(i);
    }

    for (uint32 i = 0; i < sPhaseGroupStore.GetNumRows(); ++i)
        if (PhaseGroupEntry const* group = sPhaseGroupStore.LookupEntry(i))
            if (PhaseEntry const* phase = sPhaseStores.LookupEntry(group->PhaseID))
                sPhasesByGroup[group->PhaseGroupID].insert(phase->ID);

    for (uint32 i = 0; i < sItemAppearanceStore.GetNumRows(); ++i)
        if (ItemAppearanceEntry const* entry = sItemAppearanceStore.LookupEntry(i))
            sItemDisplayIDMap[entry->FileDataID] = entry->DisplayID;
    
    for (uint32 i = 1; i < sTaxiPathStore.GetNumRows(); ++i)
        if (TaxiPathEntry const* entry = sTaxiPathStore.LookupEntry(i))
            sTaxiPathSetBySource[entry->From][entry->To] = TaxiPathBySourceAndDestination(entry->ID, entry->Cost);
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
            if (SpellEffectEntry const* sInfo = sSpellEffectStore.LookupEntry (i))
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
            uint8  field   = (uint8)((i - 1) / 8);
            uint32 submask = 1 << ((i-1) % 8);

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

    for (uint32 i = 0; i < sRuleSetItemUpgradeEntryStore.GetNumRows(); ++i)
    {
        RuleSetItemUpgradeEntry const* rsiu = sRuleSetItemUpgradeEntryStore.LookupEntry(i);
        if (!rsiu)
            continue;

        ItemUpgradeDataMap::iterator itr = sItemUpgradeDataMap.find(rsiu->itemEntry);
        if (itr != sItemUpgradeDataMap.end())
            continue;

        ItemUpgradeData& data = sItemUpgradeDataMap[rsiu->itemEntry];

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

    // error checks
    if (bad_db2_files.size() >= DB2FilesCount)
    {
        sLog->outError(LOG_FILTER_GENERAL, "\nIncorrect DataDir value in worldserver.conf or ALL required *.db2 files (%d) not found by path: %sdb2", DB2FilesCount, dataPath.c_str());
        exit(1);
    }
    else if (!bad_db2_files.empty())
    {
        std::string str;
        for (std::list<std::string>::iterator i = bad_db2_files.begin(); i != bad_db2_files.end(); ++i)
            str += *i + "\n";

        sLog->outError(LOG_FILTER_GENERAL, "\nSome required *.db2 files (%u from %d) not found or not compatible:\n%s", (uint32)bad_db2_files.size(), DB2FilesCount,str.c_str());
        exit(1);
    }

    // Check loaded DB2 files proper version
    if (!sItemStore.LookupEntry(107499)            ||       // last item added in 5.4.1 17538
        !sItemExtendedCostStore.LookupEntry(5268)  )        // last item extended cost added in 5.4.1 17538
    {
        sLog->outError(LOG_FILTER_GENERAL, "Please extract correct db2 files from client 5.0.5 16057.");
        exit(1);
    }

    sLog->outInfo(LOG_FILTER_GENERAL, ">> Initialized %d DB2 data stores.", DB2FilesCount);
}

std::list<uint32> GetPackageItemList(uint32 packageEntry)
{
    return sPackageItemList[packageEntry];
}

std::list<uint32> GetGameObjectsList()
{
    return sGameObjectsList;
}

ItemUpgradeData const* GetItemUpgradeData(uint32 itemEntry)
{
    switch (itemEntry)
    {
        case 102248: itemEntry = 105277; break;
        case 102249: itemEntry = 98147;  break;
        case 102245: itemEntry = 98335;  break;
        case 102250: itemEntry = 98146;  break;
        case 102247: itemEntry = 98149;  break;
        case 102246: itemEntry = 98150;  break;
        default:
            break;
    }

    ItemUpgradeDataMap::iterator itr = sItemUpgradeDataMap.find(itemEntry);
    if (itr == sItemUpgradeDataMap.end())
        return NULL;

    return &itr->second;
}

uint32 GetItemDisplayID(uint32 appearanceID)
{
    auto itr = sItemDisplayIDMap.find(appearanceID);
    if (itr != sItemDisplayIDMap.end())
        return itr->second;
    return 0;
}

uint32 GetHeirloomItemLevel(uint32 curveId, uint32 level)
{
    // Assuming linear item level scaling for heirlooms
    auto itr = HeirloomCurvePoints.find(curveId);
    if (itr == HeirloomCurvePoints.end())
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

uint32 GetItemDisplayId(uint32 itemId, uint32 appearanceModId)
{
    auto itr = ItemDisplayMap.find(itemId | (appearanceModId << 24));
    if (itr != ItemDisplayMap.end())
        return itr->second;

    // Fall back to unmodified appearance
    if (appearanceModId)
    {
        itr = ItemDisplayMap.find(itemId);
        if (itr != ItemDisplayMap.end())
            return itr->second;
    }

    return 0;
}

std::vector<ItemBonusEntry const*> GetItemBonuses(uint32 bonusListId)
{
    auto itr = ItemBonusLists.find(bonusListId);
    if (itr != ItemBonusLists.end())
        return itr->second;

    return std::vector<ItemBonusEntry const*>();
}

std::set<uint32> const& GetPhasesForGroup(uint32 group)
{
    return sPhasesByGroup[group];
}

LanguageWordsMap const* GetLanguageWordMap(uint32 lang_id)
{
    std::map<uint32, LanguageWordsMap>::const_iterator itr = sLanguageWordsMapStore.find(lang_id);
    return itr != sLanguageWordsMapStore.end() ? &itr->second : NULL;
}

std::vector<std::string> const* GetLanguageWordsBySize(uint32 lang_id, uint32 size)
{
    LanguageWordsMap const* wordMap = GetLanguageWordMap(lang_id);
    if (!wordMap)
        return NULL;

    std::map<uint32, std::vector<std::string> >::const_iterator itr = wordMap->find(size);
    return itr != wordMap->end() ? &itr->second : NULL;
}

DB2StorageBase const* GetDB2Storage(uint32 type)
{
    DB2StorageMap::const_iterator itr = DB2Stores.find(type);
    if (itr != DB2Stores.end())
        return itr->second;

    return NULL;
}