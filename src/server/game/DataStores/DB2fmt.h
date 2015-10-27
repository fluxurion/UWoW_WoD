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

// x - skip<uint32>, X - skip<uint8>, s - char*, f - float, i - uint32, b - uint8, d - index (not included)
// n - index (included), l - uint64, p - field present in sql dbc, a - field absent in sql dbc

#ifndef TRINITY_DB2SFRM_H
#define TRINITY_DB2SFRM_H

char const AreaGroupFormat[] = "n";
char const AreaGroupMemberFormat[] = "nii";
char const BattlePetAbilityEffectFormat[] = "niiiiiiiiiii";
char const BattlePetAbilityFormat[] = "niiiiiss";
char const BattlePetAbilityStateFormat[] = "niii";
char const BattlePetAbilityTurnFormat[] = "niiiii";
char const BattlePetBreedQualityFormat[] = "nif";
char const BattlePetBreedStateFormat[] = "niii";
char const BattlePetEffectPropertiesFormat[] = "nissssssiiiiii";
char const BattlePetSpeciesFormat[] = "niiiiiiss";
char const BattlePetSpeciesStateFormat[] = "niii";
char const BattlePetSpeciesXAbilityFormat[] = "niiii";
char const BattlePetStateFormat[] = "nisi";
char const BroadcastTextFormat[] = "nissiiiiiiiii";
char const CreatureDifficultyFormat[] = "niiiiiiiiii";
char const CurrencyTypesFormat[] = "nisssiiiiiis";
char const CurvePointFormat[] = "niiff";
char const GameObjectsFormat[] = "niiffffffffiiiiiiiiiiiis";
char const GarrAbilityFormat[] = "nissiii";
char const GarrBuildingFormat[] = "niiiiissssiiiiiiiiiiiiii";
char const GarrBuildingPlotInstFormat[] = "niiiff";
char const GarrClassSpecFormat[] = "nsssii";
char const GarrFollowerFormat[] = "niiiiiiiiiiiiiissiiii";
char const GarrFollowerLevelXPFormat[] = "niii";
char const GarrFollowerQualityFormat[] = "niii";
char const GarrFollowerXAbilityFormat[] = "niii";
char const GarrMissionFormat[] = "niiiiiiiiisssiiiiii";
char const GarrMissionRewardFormat[] = "niiiiiiiii";
char const GarrPlotBuildingFormat[] = "nii";
char const GarrPlotFormat[] = "niiisiiii";
char const GarrPlotInstanceFormat[] = "nis";
char const GarrSiteLevelFormat[] = "niiiiffiiii";
char const GarrSiteLevelPlotInstFormat[] = "niiffi";
char const HeirloomFormat[] = "niisiiiiiiii";
char const HolidaysFormat[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiisiii";
char const ItemAppearanceFormat[] = "nii";
char const ItemBonusFormat[] = "niiiii";
char const ItemBonusTreeNodeFormat[] = "niiii";
char const ItemCurrencyCostFormat[] = "in";
char const ItemEffectFormat[] = "niiiiiiii";
char const ItemExtendedCostFormat[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiii";
char const ItemFormat[] = "niiiiiiii";
char const ItemModifiedAppearanceFormat[] = "niiiii";
char const ItemSparseFormat[] = "niiiiffiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiffffffffffiiifisssssiiiiiiiiiiiiiiiiiiifiiifiii";
char const ItemToBattlePetSpeciesFormat[] = "ni";
char const ItemUpgradeFormat[] = "niiiii";
char const ItemXBonusTreeFormat[] = "nii";
char const KeyChainFormat[] = "nbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
char const LanguageWordsFormat[] = "dis";
char const MapChallengeModeFormat[] = "niiiiiiiii";
char const MountCapabilityFormat[] = "niiiiiii";
char const MountFormat[] = "niiiisssii";
char const MountTypeXCapabilityFormat[] = "niii";
char const OverrideSpellDataFormat[] = "niiiiiiiiiiii";
char const PhaseGroupFormat[] = "nii";
char const PvpItemFormat[] = "ini";
char const QuestPackageItemFormat[] = "niiii";
char const RuleSetItemUpgradeFormat[] = "niii";
char const SoundEntriesFormat[] = "nisiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiififfiifffffii";
char const SpellAuraRestrictionsFormat[] = "niiiiiiii";
char const SpellCastingRequirementsFormat[] = "niiiiii";
char const SpellClassOptionsFormat[] = "niiiiii";
char const SpellLearnSpellFormat[] = "niii";
char const SpellMiscFormat[] = "niiiiiiiiiiiiiiiiifiiiiif";
char const SpellPowerFormat[] = "niiiiiiiiiffif";
char const SpellReagentsFormat[] = "niiiiiiiiiiiiiiiiii";
char const SpellRuneCostFormat[] = "niiiii";
char const SpellTotemsFormat[] = "niiii";
char const SpellVisualFormat[] = "niiiiiiiiiiiiiiiiiiffffffiiiii";
char const TaxiNodesFormat[] = "nifffsiiiiiff";
char const TaxiPathFormat[] = "niii";
char const TaxiPathNodeFormat[] = "niiifffiiii";
char const ToyFormat[] = "niisi";

#endif
