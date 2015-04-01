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

#ifndef TRINITY_DB2SFRM_H
#define TRINITY_DB2SFRM_H

char const CurvePointEntryfmt[] = "niiff";
char const BroadcastTextEntryfmt[] = "nissiiiiiiiii";
char const HolidaysEntryfmt[]="niiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiixxsiix";
const char Itemfmt[] = "niiiiiiii";
char const ItemAppearanceEntryfmt[]="nii";
char const ItemBonusEntryfmt[] = "niiiii";
const char ItemCurrencyCostfmt[] = "xn";
char const ItemModifiedAppearanceEntryfmt[] = "niiiii";
const char ItemSparsefmt[] = "niiiiffiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiffffffffffiiifisssssiiiiiiiiiiiiiiiiiiifiiifiii";
char const ItemExtendedCostEntryfmt[]="niiiiiiiiiiiiiiiiiiiiiiiiiiiii";
const std::string CustomItemExtendedCostEntryfmt="paappppppppppppappppppppppaapa";
const std::string CustomItemExtendedCostEntryIndex = "ID";
char const ItemEffectEntryfmt[]="niiiiiiii";
const char LanguageWordsEntryfmt[] = "dis";
char const KeyChainfmt[]="nbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
char const OverrideSpellDataEntryfmt[] = "niiiiiiiiiixx";
char const PhaseGroupEntryfmt[] = "nii";
char const SpellAuraRestrictionsEntryfmt[] = "diiiiiiii";
char const SpellCastingRequirementsEntryfmt[] = "dixxixi";
char const SpellClassOptionsEntryfmt[] = "niiiiii";
const char BattlePetAbilityEntryfmt[]="nixiiixx";
const char BattlePetAbilityEffectEntryfmt[]="nixxiiiiiiii";
const char BattlePetEffectPropertiesEntryfmt[]="nxssssssxxxxxx";
const char BattlePetAbilityTurnEntryfmt[]="nixixx";
const char BattlePetAbilityStateEntryfmt[]="nxxx";
const char BattlePetSpeciesEntryfmt[]="nixiiiixx";
const char BattlePetSpeciesStateEntryfmt[]="niii";
const char BattlePetSpeciesXAbilityEntryfmt[]="niiii";
const char BattlePetStateEntryfmt[]="nxsi";
const char BattlePetBreedQualityEntryfmt[]="nif";
const char BattlePetBreedStateEntryfmt[]="niii";
char const SpellMiscEntryfmt[] = "niiiiiiiiiiiiiiiiifiiiiix";
char const SpellPowerEntryfmt[] = "niiiiiixixffif";
const char SpellReagentsEntryfmt[] = "diiiiiiiiiiiiiiiiii";
char const SpellRuneCostEntryfmt[] = "niiixi";
char const SpellTotemsEntryfmt[] = "niiii";
char const TaxiNodesEntryfmt[] = "nifffsiiiiff";
char const TaxiPathEntryfmt[] = "niii";
char const TaxiPathNodeEntryfmt[] = "niiifffiiii";

const char QuestPackageItemfmt[] = "iiiix";
const char ItemUpgradeEntryfmt[] = "niiiii";
const char RuleSetItemUpgradeEntryfmt[] = "niii";
const char GameObjectsEntryfmt[] = "niiffffffffiiiiiiiiiiiis";
const char MapChallengeModeEntryfmt[] = "niiiiiiiii";
const char SpellVisualEntryfmt[] = "nxxxxxxxxxxxxxxxxxxxxxxxxixxxx";

#endif
