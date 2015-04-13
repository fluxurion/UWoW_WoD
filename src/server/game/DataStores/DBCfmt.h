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

const char Achievementfmt[]="niiisxiixixxiii";
const std::string CustomAchievementfmt="pppaaaapapaappa";
const std::string CustomAchievementIndex = "ID";
//const char AchievementCriteriafmt[] = "niiiiiiiisiiiiixxiiiiii";
const char AchievementCriteriafmt[] = "niiiiiiiixxx";
const char Criteriafmt[]="niiiiiiiiiii";
const char CriteriaTreefmt[] = "niixiiis";
const char ModifierTreefmt[]="niiiixi";
//const std::string CustomAchievementCriteriafmt="pppppppppapppppaapppppp";
//const std::string CustomAchievementCriteriaIndex = "ID";
char const AreaTableEntryfmt[] = "iiiniixxxxsxixiiiiixixxxxxiix";
const char AreaGroupEntryfmt[] = "niiiiiii";
const char AreaPOIEntryfmt[] = "niiiiiiiiiiiffixixxixxx";
const char AreaTriggerEntryfmt[] = "nifffxxxfffffxxxx";
const char ArmorLocationfmt[] = "nfffff";
const char AuctionHouseEntryfmt[] = "niiix";
const char BankBagSlotPricesEntryfmt[] = "ni";
char const BannedAddOnsfmt[] = "nxxxxxxxxxx";
const char BarberShopStyleEntryfmt[] = "nixxxiii";
const char BattlemasterListEntryfmt[] = "niiiiiiiiiiiiiiiiixsiiiiiiiixxx";
char const CharStartOutfitEntryfmt[] = "dbbbXiiiiiiiiiiiiiiiiiiiiiiiiii";
char const CharTitlesEntryfmt[] = "nxssix";
const char ChatChannelsEntryfmt[] = "nixsx";

const char ChrClassesEntryfmt[] = "nixsxxxixiiiixxxxxx";
char const ChrRacesEntryfmt[] = "niixiixxxxxxiisxxxxxxxxxxxxxxxxxxxxxxxxx";
const char ChrClassesXPowerTypesfmt[] = "nii";
const char ChrSpecializationsfmt[] = "nxiiiiiiiiixxxii";

const char CinematicSequencesEntryfmt[] = "nxxxxxxxxx";
const char CreatureDisplayInfofmt[] = "nixifxxxxxxxxxxxxxxix";
char const CreatureDisplayInfoExtrafmt[] = "dixxxxxxxxxxxxxxxxxxxx";
const char CreatureModelDatafmt[] = "nxxxxxxxxxxxxxffxxxxxxxxxxxxxxxxxx";
const char CreatureFamilyfmt[] = "nfifiiiiixsx";
const char CreatureSpellDatafmt[] = "niiiixxxx";
const char CreatureTypefmt[] = "nxx";
const char CurrencyTypesfmt[] = "nixxxxiiiixx";

const char DestructibleModelDatafmt[] = "ixxixxxixxxixxxixxxxxxxx";
char const Difficultyfmt[] = "niiiixiixxxxx";
const char DungeonEncounterfmt[] = "niixisxxx";
const char DurabilityCostsfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiii";
const char DurabilityQualityfmt[] = "nf";
const char EmotesEntryfmt[] = "nxxiiixx";
const char EmotesTextEntryfmt[] = "nxixxxxxxxxxxxxxxxx";
const char FactionEntryfmt[] = "niiiiiiiiiiiiiiiiiiffixsxxix";
const char FactionTemplateEntryfmt[] = "niiiiiiiiiiiii";
const char GameObjectDisplayInfofmt[] = "nixxxxxxxxxxffffffxxx";
char const GameTablesFmt[] = "dsii";
const char GemPropertiesEntryfmt[] = "nixxii";
const char GlyphPropertiesfmt[] = "niiix";
const char GlyphSlotfmt[] = "nii";

const char GtBarberShopCostBasefmt[] = "xf";
const char GtCombatRatingsfmt[] = "xf";
const char GtOCTHpPerStaminafmt[] = "df";
char const GtOCTLevelExperiencefmt[] = "xf";
const char GtChanceToMeleeCritBasefmt[] = "xf";
const char GtChanceToMeleeCritfmt[] = "xf";
const char GtChanceToSpellCritBasefmt[] = "xf";
const char GtChanceToSpellCritfmt[] = "xf";
const char GtItemSocketCostPerLevelfmt[] = "xf";
char const GtNPCManaCostScalerfmt[] = "xf";
const char GtOCTClassCombatRatingScalarfmt[] = "df";
const char GtOCTRegenHPfmt[] = "f";
//const char GtOCTRegenMPfmt[] = "f";
const char GtRegenMPPerSptfmt[] = "xf";
const char GtSpellScalingfmt[] = "df";
const char GtOCTBaseHPByClassfmt[] = "df";
const char GtOCTBaseMPByClassfmt[] = "df";
const char GtBattlePetTypeDamageModfmt[]="nf";
char const GtNpcTotalHpfmt[] = "xf";
const char GuildPerkSpellsfmt[] = "dii";
const char ImportPriceArmorfmt[] = "nffff";
const char ImportPriceQualityfmt[] = "nf";
const char ImportPriceShieldfmt[] = "nf";
const char ImportPriceWeaponfmt[] = "nf";
const char ItemPriceBasefmt[] = "diff";
const char ItemReforgefmt[] = "nifif";
const char ItemBagFamilyfmt[] = "nx";
const char ItemArmorQualityfmt[] = "nfffffffi";
const char ItemArmorShieldfmt[] = "nifffffff";
const char ItemArmorTotalfmt[] = "niffff";
const char ItemClassfmt[] = "difx";
const char ItemDamagefmt[] = "nfffffffi";
const char ItemDisenchantLootfmt[] = "niiiiii";
const std::string CustomtemDisenchantLootEntryfmt="ppppppp";
const std::string CustomtemDisenchantLootEntryIndex = "Id";
//const char ItemDisplayTemplateEntryfmt[] = "nxxxxxxxxxxixxxxxxxxxxx";
const char ItemLimitCategoryEntryfmt[] = "nxii";
const char ItemRandomPropertiesfmt[] = "nxiiixxs";
const char ItemRandomSuffixfmt[] = "nsxiiiiiiiiii";
const char ItemSetEntryfmt[] = "nsiiiiiiiiiiiiiiiiiii";
char const ItemSetSpellEntryfmt[] = "niiii";
const char ItemSpecEntryfmt[] = "nii";
const std::string CustomItemSpecEntryfmt="ppp";
const std::string CustomItemSpecEntryIndex = "Id";
const char LFGDungeonEntryfmt[]="nsiiiiiiiiiixixixiiiiiiiiiixx";
char const LiquidTypefmt[] = "nxxixixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char LockEntryfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiixxxxxxxx";
char const PhaseEntryfmt[] = "ni";
const char MailTemplateEntryfmt[] = "nxs";
const char MapEntryfmt[] = "nxiixxsixxixiffxiiiixx";
char const MapDifficultyEntryfmt[] = "diisiiix";
const std::string CustomMapDifficultyEntryfmt="pppappa";
const std::string CustomMapDifficultyEntryIndex = "m_id";
const char MovieEntryfmt[] = "nxxxx";
const char MountCapabilityfmt[] = "niiiiiii";
const char MountTypefmt[] = "niiiiiiiiiiiiiiiiiiiiiiii";
const char NameGenfmt[] = "dsii";
const char QuestSortEntryfmt[] = "nx";
const char QuestV2fmt[] = "ni";
const char QuestXPfmt[] = "niiiiiiiiii";
const char QuestFactionRewardfmt[] = "niiiiiiiiii";
const char PvPDifficultyfmt[] = "diiii";
const char RandomPropertiesPointsfmt[] = "niiiiiiiiiiiiiii";
const char ResearchBranchfmt[] = "nsxixi";
const char ResearchProjectfmt[] = "nssiiiixi";
const char ResearchSitefmt[] = "niisx";
const char QuestPOIBlobfmt[] = "niix";
const char QuestPOIPointfmt[] = "diii";
char const ScalingStatDistributionfmt[] = "niii";
const char Scenariofmt[]="nsi";
const char ScenarioStepfmt[]="niiixxiiii";
const char SkillLinefmt[] = "nisxixiix";
const char SkillLineAbilityfmt[] = "niiiiiiiiiixx";
const char SoundEntriesfmt[] = "nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char SpecializationSpellsfmt[] = "niiix";
const char SpellCastTimefmt[] = "nixx";
const char SpellCategoriesEntryfmt[] = "dxxiiiiiii";
const char SpellCategoryEntryfmt[] = "nixxii";
const char SpellDifficultyfmt[] = "niiii";
const std::string CustomSpellDifficultyfmt="ppppp";
const std::string CustomSpellDifficultyIndex="id";
const char SpellDurationfmt[] = "niii";
const char SpellEffectEntryfmt[] = "niifiiiffiiiiiifiifiiiiifiiiiif";
const char SpellEffectScalingEntryfmt[] = "xfffn";
char const SpellEntryfmt[] = "nssxxiiiiiiiiiiiiiiiiiii";
const std::string CustomSpellEntryfmt="paaaapaapppppppppppppppp";
const std::string CustomSpellEntryIndex = "Id";
const char SpellFocusObjectfmt[] = "nx";
const char SpellItemEnchantmentfmt[] = "niiiiiiiiiixiiiiiiiiiiifff";
const char SpellItemEnchantmentConditionfmt[] = "nbbbbbxxxxxbbbbbbbbbbiiiiiXXXXX";
const char SpellRadiusfmt[] = "nfxxf";
const char SpellRangefmt[] = "nffffixx";
const char SpellScalingEntryfmt[] = "diiiifiii";
const char SpellTargetRestrictionsEntryfmt[] = "niifxiiii";
const char PowerDisplayEntryfmt[]="nixxxx";
const char SpellInterruptsEntryfmt[] = "dxxixixi";
const char SpellEquippedItemsEntryfmt[] = "dxxiii";
const char SpellAuraOptionsEntryfmt[] = "nxxiiiiii";
const std::string CustomSpellAuraOptionsEntryfmt="paapppppp";
const std::string CustomSpellAuraOptionsEntryIndex = "Id";
const char SpellProcsPerMinuteEntryfmt[] = "nfx";
const char SpellProcsPerMinuteModEntryfmt[] = "nxifi";
const char SpellCooldownsEntryfmt[] = "dxxiii";
const char SpellLevelsEntryfmt[] = "dixiii";
const char SpellRuneCostfmt[] = "niiiii";
const char SpellShapeshiftEntryfmt[] = "nixixx";
const char SpellShapeshiftFormfmt[] = "nxxiixiiixxiiiiiiiixx";
const char StableSlotPricesfmt[] = "ni";
const char SummonPropertiesfmt[] = "niiiii";
const char TalentEntryfmt[] = "nxiiixxxiis";
const char TeamContributionPointsfmt[] = "df";
const char TotemCategoryEntryfmt[] = "nxii";
char const UnitPowerBarfmt[] = "niixxxxxxxxxxxxxxxxxxxxxxxx";
char const TransportAnimationfmt[] = "diifffx";
char const TransportRotationfmt[] = "diiffff";
char const VehicleEntryfmt[] = "niiffffiiiiiiiifffffffffffffffxxxxfifiiii";
char const VehicleSeatEntryfmt[] = "niiffffffffffiiiiiifffffffiiifffiiiiiiiffiiiiffffffffffffiiiiiiiii";
const char WMOAreaTableEntryfmt[] = "niiixxxxxiixxxx";
char const WorldMapAreaEntryfmt[] = "xinxffffixxxii";
const char WorldMapOverlayEntryfmt[] = "nxiiiixxxxxxxxxx";
char const WorldSafeLocsEntryfmt[] = "niffffx";

#endif
