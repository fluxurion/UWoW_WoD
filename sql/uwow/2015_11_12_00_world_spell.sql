DELETE FROM spell_linked_spell WHERE hastalent IN (152281,-152281);
DELETE FROM spell_linked_spell WHERE hastalent2 IN (152281,-152281);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) VALUES
('47632','55078','1','0','0','0','161497','0','-152281','0','0','4000','0','0','0','-1','10','0','Разносчик чумы'),
('47632','55095','1','0','0','0','161497','0','-152281','0','0','4000','0','0','0','-1','10','0','Разносчик чумы'),
('47632','155163','1','0','0','0','161497','0','152281','0','0','0','0','0','0','-1','0','0','Мертвящая чума + Разносчик чумы'),
('49143','55078','1','0','0','0','161497','0','-152281','0','0','4000','0','0','0','-1','10','0','Разносчик чумы'),
('49143','55095','1','0','0','0','161497','0','-152281','0','0','4000','0','0','0','-1','10','0','Разносчик чумы'),
('49143','155163','1','0','0','0','161497','0','152281','0','0','0','0','0','0','-1','0','0','Мертвящая чума + Разносчик чумы');
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) VALUES
('115994','55078','1','0','0','0','-152281','0','0','0','0','0','0','0','0','-1','10','0','Нечестивая порча'),
('115994','55095','1','0','0','0','-152281','0','0','0','0','0','0','0','0','-1','10','0','Нечестивая порча'),
('115994','155163','1','0','0','0','152281','0','0','0','0','0','0','0','0','-1','0','0','Нечестивая порча');
DELETE FROM spell_linked_spell WHERE spell_trigger IN (155163);

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('155159','spell_dk_necrotic_plague');
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('155163','spell_dk_necrotic_plague_cast');

insert into `spell_pet_auras` (`petEntry`, `spellId`, `option`, `target`, `targetaura`, `bp0`, `bp1`, `bp2`, `aura`, `casteraura`, `createdspell`, `fromspell`, `comment`) values
('82521','166182','0','0','0','0','0','0','0','0','0','0','Осквернение'),
('82521','31366','0','0','0','0','0','0','0','0','0','0','Осквернение');
update creature_template set flags_extra = 130 where entry = 82521;

insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('1713','0','512','0','2','156000','0','0','0','0','0','0','0','Осквернение'),
('1713','1','512','4','2','156000','0','0','0','0','0.025','10','0','Осквернение');

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('152280','spell_dk_defile');

insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('155166','155168','0','3','7','1','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Дыхание Синдрагосы');
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('155166','0','0','0','0','0','0','7','-1','-1','250','0','0','0','0','0','0','0','0','0','Метка Синдрагосы');
insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('155166','155522','0','4','0','0','0','0','0','0','1','0','0','0','0','0','1','Дыхание Синдрагосы');
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('155166','spell_dk_breath_of_sindragosa');
