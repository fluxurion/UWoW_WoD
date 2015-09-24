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

#ifndef TRINITY_DBCSFRM_H
#define TRINITY_DBCSFRM_H

// x - skip<uint32>, X - skip<uint8>, s - char*, f - float, i - uint32, b - uint8, d - index (not included)
// n - index (included), l - bool, p - field present in sql dbc, a - field absent in sql dbc

//char const AchievementCriteriafmt[] = "niiiiiiiisiiiiixxiiiiii";
//char const GtOCTRegenMPfmt[] = "f";
//char const ItemDisplayTemplateEntryfmt[] = "nxxxxxxxxxxixxxxxxxxxxx";
//const std::string CustomAchievementCriteriafmt= "pppppppppapppppaapppppp";
//const std::string CustomAchievementCriteriaIndex = "ID";

char const CharStartOutfitEntryfmt[] = "dbbbXiiiiiiiiiiiiiiiiiiiiiiiiii";
char const CharTitlesEntryfmt[] = "nxssix";
char const AchievementCriteriafmt[] = "niiiiiiiixxx";
char const Achievementfmt[] = "niiisxiixixxiii";
char const AreaPOIEntryfmt[] = "niiiiiiiiiiiffixixxixxx";
char const AreaTableEntryfmt[] = "iiiniixxxxsxixiiiiixixxxxxiix";
char const AreaTriggerEntryfmt[] = "nifffxxxfffffxxxx";
char const ArmorLocationfmt[] = "nfffff";
char const AuctionHouseEntryfmt[] = "niiix";
char const BankBagSlotPricesEntryfmt[] = "ni";
char const BannedAddOnsfmt[] = "nxxxxxxxxxx";
char const BarberShopStyleEntryfmt[] = "nixxxiii";
char const BattlemasterListEntryfmt[] = "niiiiiiiiiiiiiiiiixsiiiiiiiixxx";
char const CharacterLoadoutItemfmt[]= "nii";
char const ChatChannelsEntryfmt[] = "nixsx";
char const ChrClassesEntryfmt[] = "nixsxxxixiiiiixxxxx";
char const ChrClassesXPowerTypesfmt[] = "nii";
char const ChrRacesEntryfmt[] = "niixiixxxxxxiisxxxxxxxxxxxxxxxxxxxxxxxxx";
char const ChrSpecializationsfmt[] = "nxiiiiiiiiixxxii";
char const CinematicSequencesEntryfmt[] = "nxxxxxxxxx";
char const CreatureDisplayInfoExtrafmt[] = "dixxxxxxxxxxxxxxxxxxxx";
char const CreatureDisplayInfofmt[] = "nixifxxxxxxxxxxxxxxix";
char const CreatureFamilyfmt[] = "nfifiiiiixsx";
char const CreatureModelDatafmt[] = "nxxxxxxxxxxxxxffxxxxxxxxxxxxxxxxxx";
char const CreatureSpellDatafmt[] = "niiiixxxx";
char const CreatureTypefmt[] = "nxx";
char const Criteriafmt[]= "niiiiiiiiiii";
char const CriteriaTreefmt[] = "niixiiis";
char const DestructibleModelDatafmt[] = "ixxixxxixxxixxxixxxxxxxx";
char const Difficultyfmt[] = "niiiixiixxxxx";
char const DungeonEncounterfmt[] = "niixisxxx";
char const DurabilityCostsfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiii";
char const DurabilityQualityfmt[] = "nf";
char const EmotesEntryfmt[] = "nxxiiixx";
char const EmotesTextEntryfmt[] = "nxixxxxxxxxxxxxxxxx";
char const FactionEntryfmt[] = "niiiiiiiiiiiiiiiiiiffixsxxix";
char const FactionTemplateEntryfmt[] = "niiiiiiiiiiiii";
char const GameObjectDisplayInfofmt[] = "nixxxxxxxxxxffffffxxx";
char const GameTablesFmt[] = "dsii";
char const GemPropertiesEntryfmt[] = "nixxii";
char const GlyphPropertiesfmt[] = "niiix";
char const GlyphSlotfmt[] = "nii";
char const GtBarberShopCostBasefmt[] = "xf";
char const GtBattlePetTypeDamageModfmt[]= "nf";
char const GtChanceToMeleeCritBasefmt[] = "xf";
char const GtChanceToMeleeCritfmt[] = "xf";
char const GtChanceToSpellCritBasefmt[] = "xf";
char const GtChanceToSpellCritfmt[] = "xf";
char const GtCombatRatingsfmt[] = "xf";
char const GtItemSocketCostPerLevelfmt[] = "xf";
char const GtNPCManaCostScalerfmt[] = "xf";
char const GtNpcTotalHpfmt[] = "xf";
char const GtOCTBaseHPByClassfmt[] = "df";
char const GtOCTBaseMPByClassfmt[] = "df";
char const GtOCTHpPerStaminafmt[] = "df";
char const GtOCTLevelExperiencefmt[] = "xf";
char const GtOCTRegenHPfmt[] = "f";
char const GtRegenMPPerSptfmt[] = "xf";
char const GtSpellScalingfmt[] = "df";
char const GuildPerkSpellsfmt[] = "dii";
char const ImportPriceArmorfmt[] = "nffff";
char const ImportPriceQualityfmt[] = "nf";
char const ImportPriceShieldfmt[] = "nf";
char const ImportPriceWeaponfmt[] = "nf";
char const ItemArmorQualityfmt[] = "nfffffffi";
char const ItemArmorShieldfmt[] = "nifffffff";
char const ItemArmorTotalfmt[] = "niffff";
char const ItemBagFamilyfmt[] = "nx";
char const ItemClassfmt[] = "difx";
char const ItemDamagefmt[] = "nfffffffi";
char const ItemDisenchantLootfmt[] = "niiiiii";
char const ItemLimitCategoryEntryfmt[] = "nxii";
char const ItemPriceBasefmt[] = "diff";
char const ItemRandomPropertiesfmt[] = "nxiiixxs";
char const ItemRandomSuffixfmt[] = "nsxiiiiiiiiii";
char const ItemReforgefmt[] = "nifif";
char const ItemSetEntryfmt[] = "nsiiiiiiiiiiiiiiiiiii";
char const ItemSetSpellEntryfmt[] = "niiii";
char const ItemSpecEntryfmt[]= "niiiiii";
char const ItemSpecOverrideEntryfmt[]= "nii";
char const LFGDungeonEntryfmt[]= "nsiiiiiiiiiixixixiiiiiiiiiixx";
char const LiquidTypefmt[] = "nxxixixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char LockEntryfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
char const MailTemplateEntryfmt[] = "nxs";
char const MapDifficultyEntryfmt[] = "diisiiix";
char const MapEntryfmt[] = "nxiixxsixxixiffxiiiixx";
char const ModifierTreefmt[]= "niiiixi";
char const MountCapabilityfmt[] = "niiiiiii";
char const MountTypefmt[] = "niiiiiiiiiiiiiiiiiiiiiiii";
char const MovieEntryfmt[] = "nxxxx";
char const NameGenfmt[] = "dsii";
char const PhaseEntryfmt[] = "ni";
char const PowerDisplayEntryfmt[]= "nixxxx";
char const PvPDifficultyfmt[] = "diiii";
char const QuestFactionRewardfmt[] = "niiiiiiiiii";
char const QuestPOIBlobfmt[] = "niix";
char const QuestPOIPointfmt[] = "diii";
char const QuestSortEntryfmt[] = "nx";
char const QuestV2fmt[] = "ni";
char const QuestXPfmt[] = "niiiiiiiiii";
char const RandomPropertiesPointsfmt[] = "niiiiiiiiiiiiiii";
char const ResearchBranchfmt[] = "nsxixi";
char const ResearchProjectfmt[] = "nssiiiixi";
char const ResearchSitefmt[] = "niisx";
char const ScalingStatDistributionfmt[] = "niii";
char const Scenariofmt[]= "nsi";
char const ScenarioStepfmt[]= "niiixxiiii";
char const SkillLineAbilityfmt[] = "niiiiiiiiiixx";
char const SkillLinefmt[] = "nisxixiix";
char const SpecializationSpellsfmt[] = "niiix";
char const SpellAuraOptionsEntryfmt[] = "nxxiiiiii";
char const SpellCastTimefmt[] = "nixx";
char const SpellCategoriesEntryfmt[] = "dxxiiiiiii";
char const SpellCategoryEntryfmt[] = "nixxii";
char const SpellCooldownsEntryfmt[] = "dxxiii";
char const SpellDifficultyfmt[] = "niiii";
char const SpellDurationfmt[] = "niii";
char const SpellEffectEntryfmt[] = "niifiiiffiiiiiifiifiiiiifiiiiif";
char const SpellEffectScalingEntryfmt[] = "xfffn";
char const SpellEntryfmt[] = "nssxxiiiiiiiiiiiiiiiiiii";
char const SpellEquippedItemsEntryfmt[] = "dxxiii";
char const SpellFocusObjectfmt[] = "nx";
char const SpellInterruptsEntryfmt[] = "dxxixixi";
char const SpellItemEnchantmentConditionfmt[] = "nbbbbbxxxxxbbbbbbbbbbiiiiiXXXXX";
char const SpellItemEnchantmentfmt[] = "niiiiiiiiiixiiiiiiiiiiifff";
char const SpellLevelsEntryfmt[] = "dixiii";
char const SpellProcsPerMinuteEntryfmt[] = "nfx";
char const SpellProcsPerMinuteModEntryfmt[] = "nxifi";
char const SpellRadiusfmt[] = "nfxxf";
char const SpellRangefmt[] = "nffffixx";
char const SpellRuneCostfmt[] = "niiiii";
char const SpellScalingEntryfmt[] = "diiiifiii";
char const SpellShapeshiftEntryfmt[] = "nixixx";
char const SpellShapeshiftFormfmt[] = "nxxiixiiixxiiiiiiiixx";
char const SpellTargetRestrictionsEntryfmt[] = "niifxiiii";
char const StableSlotPricesfmt[] = "ni";
char const SummonPropertiesfmt[] = "niiiii";
char const TalentEntryfmt[] = "nxiiixxxiis";
char const TeamContributionPointsfmt[] = "df";
char const TotemCategoryEntryfmt[] = "nxii";
char const TransportAnimationfmt[] = "diifffx";
char const TransportRotationfmt[] = "diiffff";
char const UnitPowerBarEntryfmt[] = "niiiiffixxxxxxxxxxxxixxxxxx";
char const UnitPowerBarfmt[] = "niiiiffixxxxxxxxxxxxixxxxxx";
char const VehicleEntryfmt[] = "niiffffiiiiiiiifffffffffffffffxxxxfifiiii";
char const VehicleSeatEntryfmt[] = "niiffffffffffiiiiiifffffffiiifffiiiiiiiffiiiiffffffffffffiiiiiiiii";
char const WMOAreaTableEntryfmt[] = "niiixxxxxiixxxx";
char const WorldMapAreaEntryfmt[] = "xinxffffixxxii";
char const WorldMapOverlayEntryfmt[] = "nxiiiixxxxxxxxxx";
char const WorldSafeLocsEntryfmt[] = "niffffx";

const std::string CustomAchievementfmt= "pppaaaapapaappp";
const std::string CustomAchievementIndex = "ID";
const std::string CustomCriteriafmt = "pppppppppppp";
const std::string CustomCriteriaIndex = "ID";
const std::string CustomCriteriaTreefmt = "pppapppa";
const std::string CustomCriteriaTreeIndex = "ID";
const std::string CustomItemSpecOverrideEntryfmt= "ppp";
const std::string CustomItemSpecOverrideEntryIndex = "Id";
const std::string CustomMapDifficultyEntryfmt= "pppappa";
const std::string CustomMapDifficultyEntryIndex = "m_id";
const std::string CustomSpellAuraOptionsEntryfmt= "paapppppp";
const std::string CustomSpellAuraOptionsEntryIndex = "Id";
const std::string CustomSpellDifficultyfmt= "ppppp";
const std::string CustomSpellDifficultyIndex= "id";
const std::string CustomSpellEntryfmt= "paaaapaapppppppppppppppp";
const std::string CustomSpellEntryIndex = "Id";
const std::string CustomtemDisenchantLootEntryfmt= "ppppppp";
const std::string CustomtemDisenchantLootEntryIndex = "Id";

#endif
