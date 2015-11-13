delete from areatrigger_actions where entry = 1713 and id = 1;
insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('1713','1','512','0','2','156004','0','0','0','0','0','0','0','Осквернение'),
('1713','2','42','1','2','156004','0','0','0','0','0','0','0','Осквернение');

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('156000','spell_dk_defile_damage');

delete from spell_proc_check where entry = 155166;
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('155166','0','0','0','0','0','7','7','-1','-1','250','0','0','0','0','0','0','0','0','0','Метка Синдрагосы');

insert into `spell_concatenate_aura` (`spellid`, `effectSpell`, `auraId`, `effectAura`, `type`, `caster`, `target`, `comment`) values
('50385','0','48266','2','1','0','0','Великая власть льда'),
('50385','0','48266','2','0','0','0','Великая власть льда');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('50842','50421','0','0','0','0','49509','0','0','0','0','0','0','0','0','-1','0','0','Запах крови'),
('114866','50421','0','0','0','0','49509','0','0','0','0','0','0','0','0','-1','0','0','Запах крови');

DELETE FROM spell_proc_check WHERE entry IN (81141);

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('152280','169018','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','11','0','Осквернение');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('163952','0','0','163952','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Отключаем прок Кровавый обряд, до реализации многократной атаки');

DELETE FROM spell_bonus_data WHERE entry IN (50842);

DELETE FROM spell_script_names WHERE spell_id IN (48721);
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('50842','spell_dk_blood_boil');
DELETE FROM spell_linked_spell WHERE spell_trigger IN (50842,-50842) AND `type` = 1;

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('171049','spell_dk_rune_tap');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('45477','55095','1','0','0','0','-152281','0','0','0','0','0','0','0','0','-1','0','0','Ледяное прикосновение'),
('45477','155163','1','0','0','0','152281','0','0','0','0','0','0','0','0','-1','0','0','Ледяное прикосновение');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('45462','55078','1','0','0','0','-152281','0','0','0','0','0','0','0','0','-1','0','0','Удар чумы'),
('45462','155163','1','0','0','0','152281','0','0','0','0','0','0','0','0','-1','0','0','Удар чумы');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('49998','45470','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Удар смерти');

insert into `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) values
('77606','0','0','0','0','0','0','0','65536','0','0','0','7');
DELETE FROM spell_proc_event WHERE entry IN (77616);
insert into `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) values
('77616','0','0','0','0','0','0','87040','65536','0','0','0','7');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('47541','164047','0','0','0','0','157343','0','0','0','0','0','0','0','0','-1','0','0','Усиленный лик смерти');
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('164047','spell_dk_enhanced_death_coil');

DELETE FROM spell_linked_spell WHERE spell_trigger IN (49576);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('49576','45524','1','0','0','2','157367','0','0','0','0','0','0','0','0','-1','0','0','Улучшенная хватка смерти');

DELETE FROM conditions WHERE SourceEntry IN (50515);
insert into `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) values
('13','1','50515','0','0','31','0','3','27829','0','0','0','','Освобождение горгульи');

DELETE FROM spell_script_names WHERE spell_id IN (45470);

DELETE FROM spell_linked_spell WHERE spell_trigger IN (171049);
DELETE FROM spell_proc_check WHERE entry IN (81164);
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('81164','0','0','0','-157335','0','0','7','-1','-1','0','0','0','0','0','-31','0','0','0','0','Захват рун');

