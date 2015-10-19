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

#ifndef TRINITY_DB2STORES_H
#define TRINITY_DB2STORES_H

#include "DB2Store.h"
#include "DB2Structure.h"
#include "SharedDefines.h"

#include <array>
#include <boost/regex.hpp>
#include <list>

extern DB2Storage<AreaGroupMemberEntry>             sAreaGroupMemberStore;
extern DB2Storage<BattlePetAbilityEffectEntry>      sBattlePetAbilityEffectStore;
extern DB2Storage<BattlePetAbilityEntry>            sBattlePetAbilityStore;
extern DB2Storage<BattlePetAbilityStateEntry>       sBattlePetAbilityStateStore;
extern DB2Storage<BattlePetAbilityTurnEntry>        sBattlePetAbilityTurnStore;
extern DB2Storage<BattlePetBreedQualityEntry>       sBattlePetBreedQualityStore;
extern DB2Storage<BattlePetBreedStateEntry>         sBattlePetBreedStateStore;
extern DB2Storage<BattlePetEffectPropertiesEntry>   sBattlePetEffectPropertiesStore;
extern DB2Storage<BattlePetSpeciesEntry>            sBattlePetSpeciesStore;
extern DB2Storage<BattlePetSpeciesStateEntry>       sBattlePetSpeciesStateStore;
extern DB2Storage<BattlePetSpeciesXAbilityEntry>    sBattlePetSpeciesXAbilityStore;
extern DB2Storage<BattlePetStateEntry>              sBattlePetStateStore;
extern DB2Storage<BroadcastTextEntry>               sBroadcastTextStore;
extern DB2Storage<CurrencyTypesEntry>               sCurrencyTypesStore;
extern DB2Storage<GameObjectsEntry>                 sGameObjectsStore;
extern DB2Storage<GarrAbilityEntry>                 sGarrAbilityStore;
extern DB2Storage<GarrBuildingEntry>                sGarrBuildingStore;
extern DB2Storage<GarrBuildingPlotInstEntry>        sGarrBuildingPlotInstStore;
extern DB2Storage<GarrClassSpecEntry>               sGarrClassSpecStore;
extern DB2Storage<GarrFollowerEntry>                sGarrFollowerStore;
extern DB2Storage<GarrFollowerXAbilityEntry>        sGarrFollowerXAbilityStore;
extern DB2Storage<GarrMissionEntry>                 sGarrMissionStore;
extern DB2Storage<GarrPlotBuildingEntry>            sGarrPlotBuildingStore;
extern DB2Storage<GarrPlotEntry>                    sGarrPlotStore;
extern DB2Storage<GarrPlotInstanceEntry>            sGarrPlotInstanceStore;
extern DB2Storage<GarrSiteLevelEntry>               sGarrSiteLevelStore;
extern DB2Storage<GarrSiteLevelPlotInstEntry>       sGarrSiteLevelPlotInstStore;
extern DB2Storage<HeirloomEntry>                    sHeirloomStore;
extern DB2Storage<HolidaysEntry>                    sHolidaysStore;
extern DB2Storage<ItemAppearanceEntry>              sItemAppearanceStore;
extern DB2Storage<ItemCurrencyCostEntry>            sItemCurrencyCostStore;
extern DB2Storage<ItemEffectEntry>                  sItemEffectStore;
extern DB2Storage<ItemEntry>                        sItemStore;
extern DB2Storage<ItemExtendedCostEntry>            sItemExtendedCostStore;
extern DB2Storage<ItemSparseEntry>                  sItemSparseStore;
extern DB2Storage<ItemToBattlePetSpeciesEntry>      sItemToBattlePetSpeciesStore;
extern DB2Storage<ItemUpgradeEntry>                 sItemUpgradeStore;
extern DB2Storage<KeyChainEntry>                    sKeyChainStore;
extern DB2Storage<LanguageWordsEntry>               sLanguageWordsStore;
extern DB2Storage<MapChallengeModeEntry>            sMapChallengeModeStore;
extern DB2Storage<OverrideSpellDataEntry>           sOverrideSpellDataStore;
extern DB2Storage<PhaseGroupEntry>                  sPhaseGroupStore;
extern DB2Storage<PvpItemEntry>                     sPvpItemStore;
extern DB2Storage<QuestPackageItemEntry>            sQuestPackageItemStore;
extern DB2Storage<RuleSetItemUpgradeEntry>          sRuleSetItemUpgradeEntryStore;
extern DB2Storage<SoundEntriesEntry>                sSoundEntriesStore;
extern DB2Storage<SpellAuraRestrictionsEntry>       sSpellAuraRestrictionsStore;
extern DB2Storage<SpellCastingRequirementsEntry>    sSpellCastingRequirementsStore;
extern DB2Storage<SpellClassOptionsEntry>           sSpellClassOptionsStore;
extern DB2Storage<SpellLearnSpellEntry>             sSpellLearnSpellStore;
extern DB2Storage<SpellMiscEntry>                   sSpellMiscStore;
extern DB2Storage<SpellPowerEntry>                  sSpellPowerStore;
extern DB2Storage<SpellReagentsEntry>               sSpellReagentsStore;
extern DB2Storage<SpellRuneCostEntry>               sSpellRuneCostStore;
extern DB2Storage<SpellTotemsEntry>                 sSpellTotemsStore;
extern DB2Storage<SpellVisualEntry>                 sSpellVisualStore;
extern DB2Storage<TaxiNodesEntry>                   sTaxiNodesStore;
extern DB2Storage<TaxiPathEntry>                    sTaxiPathStore;

extern TaxiMask                                     sTaxiNodesMask;
extern TaxiMask                                     sOldContinentsNodesMask;
extern TaxiMask                                     sHordeTaxiNodesMask;
extern TaxiMask                                     sAllianceTaxiNodesMask;
extern TaxiPathSetBySource                          sTaxiPathSetBySource;
extern TaxiPathNodesByPath                          sTaxiPathNodesByPath;

void LoadDB2Stores(std::string const& dataPath);

struct HotfixNotify
{
    uint32 TableHash;
    uint32 Timestamp;
    uint32 Entry;
};

typedef std::vector<HotfixNotify> HotfixData;

#define DEFINE_DB2_SET_COMPARATOR(structure) \
    struct structure ## Comparator : public std::binary_function<structure const*, structure const*, bool> \
    { \
        bool operator()(structure const* left, structure const* right) const { return Compare(left, right); } \
        static bool Compare(structure const* left, structure const* right); \
    };

class DB2Manager
{
public:
    //DEFINE_DB2_SET_COMPARATOR(MountTypeXCapabilityEntry);

    typedef std::map<uint32 /*hash*/, DB2StorageBase*> StorageMap;

    typedef std::map<uint32 /*curveID*/, std::map<uint32/*index*/, CurvePointEntry const*, std::greater<uint32>>> HeirloomCurvesContainer;
    typedef std::vector<ItemBonusEntry const*> ItemBonusList;
    typedef std::vector<ItemXBonusTreeEntry const*> ItemXBonusTreeList;
    typedef std::unordered_map<uint32 /*bonusListId*/, ItemBonusList> ItemBonusListContainer;
    typedef std::unordered_map<uint32 /*itemId*/, ItemXBonusTreeList> ItemToBonusTreeContainer;
    typedef std::unordered_map<uint32 /*itemId | appearanceMod << 24*/, uint32> ItemDisplayIdContainer;
    typedef std::unordered_map<uint32, std::set<ItemBonusTreeNodeEntry const*>> ItemBonusTreeContainer;
    typedef std::unordered_map<uint32, std::vector<ItemSpecOverrideEntry const*>> ItemSpecOverridesContainer;
    typedef std::unordered_map<uint32, MountEntry const*> MountContainer;
    //typedef std::set<MountTypeXCapabilityEntry const*, MountTypeXCapabilityEntryComparator> MountTypeXCapabilitySet;
    //typedef std::unordered_map<uint32, MountTypeXCapabilitySet> MountCapabilitiesByTypeContainer;
    typedef std::vector<std::string> LanguageWordsSize;
    typedef std::map<uint32 /*word length*/, LanguageWordsSize> LanguageWordsMap;
    typedef std::unordered_map<uint32 /*areaGroupId*/, std::vector<uint32/*areaId*/>> AreaGroupMemberContainer;
    typedef std::unordered_map<uint32, MapChallengeModeEntry const*> MapChallengeModeEntryMap;
    typedef std::unordered_map<uint32, ItemUpgradeData> ItemUpgradeDataMap;
    typedef std::unordered_map<uint32, std::vector<QuestPackageItemEntry const*>> QuestPackageItemContainer;
    typedef std::vector<uint32> ToyItemIdsContainer;
    typedef std::unordered_map<uint32, HeirloomEntry const*> HeirloomItemsContainer;
    typedef std::multimap<uint32, std::pair<uint32, int32>> BattlePetBreedStateByBreedMap;
    typedef std::multimap<uint32, std::pair<uint32, int32>> BattlePetSpeciesStateBySpecMap;
    typedef std::map<uint32, BattlePetSpeciesEntry const*> BattlePetSpeciesBySpellIdMap;
    typedef std::multimap<uint32, std::pair<uint32, uint8>> BattlePetTurnByAbilityIdMap;
    typedef std::multimap<uint32, BattlePetSpeciesXAbilityEntry const*> BattlePetXAbilityEntryBySpecIdMap;
    typedef std::map<uint32, uint8> BattlePetQualityMultiplierMap;
    typedef std::map<uint32, BattlePetAbilityEffectEntry const*> BattlePetEffectEntryByTurnIdMap;
    typedef std::unordered_map<BattlePetState /*state*/, int32 /*value*/, std::hash<std::underlying_type<BattlePetState>::type> > BattlePetStateMap;
    typedef std::unordered_map<uint16 /*BreedID*/, BattlePetStateMap> BattlePetBreedStates;
    typedef std::unordered_map<uint32 /*SpeciesID*/, BattlePetStateMap> BattlePetSpeciesStates;

    static DB2Manager& Instance()
    {
        static DB2Manager instance;
        return instance;
    }

    void LoadStores(std::string const& dataPath, uint32 defaultLocale);
    void InitDB2CustomStores();
    DB2StorageBase const* GetStorage(uint32 type) const;

    void LoadHotfixData();
    HotfixData const* GetHotfixData() const { return &_hotfixData; }
    time_t GetHotfixDate(uint32 entry, uint32 type) const;

    std::vector<uint32> GetAreasForGroup(uint32 areaGroupId);
    std::list<uint32> GetGameObjectsList();
    ItemUpgradeData const* GetItemUpgradeData(uint32 itemEntry);
    uint32 GetHeirloomItemLevel(uint32 curveId, uint32 level) const;
    uint32 GetItemDisplayId(uint32 itemId, uint32 appearanceModId) const;
    ItemBonusList const* GetItemBonusList(uint32 bonusListId) const;
    LanguageWordsMap const* GetLanguageWordMap(uint32 landID);
    LanguageWordsSize const* GetLanguageWordsBySize(uint32 landID, uint32 size);
    std::vector<QuestPackageItemEntry const*> const* GetQuestPackageItems(uint32 questPackageID) const;
    MountEntry const* GetMount(uint32 spellId) const;
    MountEntry const* GetMountById(uint32 id) const;
    //MountTypeXCapabilitySet const* GetMountCapabilities(uint32 mountType) const;
    std::set<uint32> GetItemBonusTree(uint32 itemId, uint32 itemBonusTreeMod) const;
    std::set<uint32> GetFindBonusTree(uint32 BonusTreeID, uint32 itemBonusTreeMod) const;
    HeirloomEntry const* GetHeirloomByItemId(uint32 itemId) const;
    HeirloomEntry const* GetHeirloomByOldItem(uint32 itemId) const;
    uint8 GetBattlePetSpeciesBySpellID(uint32 entry) const;
    float CalcBattlePetQualityMuliplier(uint8 quality, uint8 level);
    uint32 GetBattlePetTurnByAbility(uint32 abilityID, uint8 turnIndex = 1);
    uint32 GetBattlePetXAbilityEntryBySpec(uint32 speciesID, uint32 customAbility, uint8 rank);
    BattlePetSpeciesEntry const* GetBattlePetSpeciesEntry(uint32 creatureEntry);

    MapChallengeModeEntryMap _mapChallengeModeEntrybyMap; // @TODO: move this to private and make special getters

    BattlePetBreedStates _battlePetBreedStates;
    BattlePetSpeciesStates _battlePetSpeciesStates;
private:
    StorageMap _stores;
    HotfixData _hotfixData;

    HeirloomCurvesContainer _heirloomCurvePoints;
    ItemBonusListContainer _itemBonusLists;
    ItemBonusTreeContainer _itemBonusTrees;
    ItemDisplayIdContainer _itemDisplayIDs;
    ItemToBonusTreeContainer _itemToBonusTree;
    ItemSpecOverridesContainer _itemSpecOverrides;
    MountContainer _mountsBySpellId;
    //MountCapabilitiesByTypeContainer _mountCapabilitiesByType;
    std::list<uint32> sGameObjectsList;
    std::map<uint32 /*landID*/, LanguageWordsMap> sLanguageWordsMapStore;
    AreaGroupMemberContainer _areaGroupMembers;
    ItemUpgradeDataMap _itemUpgradeDataMap;
    QuestPackageItemContainer _questPackages;
    ToyItemIdsContainer _toys;
    HeirloomItemsContainer _heirlooms;
    BattlePetQualityMultiplierMap _battlePetQualityMultiplierId;
    BattlePetTurnByAbilityIdMap _battlePetTurnByAbilityId;
    BattlePetXAbilityEntryBySpecIdMap _battlePetXAbilityEntryBySpecId;
    BattlePetEffectEntryByTurnIdMap _battlePetEffectEntryByTurnId;
    BattlePetSpeciesBySpellIdMap _battlePetSpeciesBySpellId;
};

#define sDB2Manager DB2Manager::Instance()

#endif