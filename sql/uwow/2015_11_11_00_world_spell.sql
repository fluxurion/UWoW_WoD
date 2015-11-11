insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('156321','169667','6','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Атака щитом'),
('156321','126661','6','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Атака щитом');
insert into `spell_talent_linked_spell` (`spellid`, `spelllink`, `type`, `target`, `caster`, `comment`) values
('-152276','-156291','0','0','0','Стойка гладиатора');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('29838','0','0','0','-16491','0','0','7','-1','-1','0','0','0','0','0','-35','0','0','0','0','Второе дыхание');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('29838','16491','0','20','0','0','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Второе дыхание');

DELETE FROM spell_script_names WHERE spell_id IN (45524);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('45524','96294','6','0','0','0','50041','0','0','0','0','0','0','0','-1','0','0','Вечная мерзлота');

DELETE FROM spell_script_names WHERE ScriptName IN ("spell_dk_blood_charges");
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('47632','55078','1','0','0','0','161497','0','-152281','0','0','4000','0','0','0','-1','10','0','Разносчик чумы'),
('47632','55095','1','0','0','0','161497','0','-152281','0','0','4000','0','0','0','-1','10','0','Разносчик чумы'),
('47632','155159','1','0','0','0','161497','0','152281','0','0','0','0','0','0','-1','7','0','Мертвящая чума + Разносчик чумы'),
('49143','55078','1','0','0','0','161497','0','-152281','0','0','4000','0','0','0','-1','10','0','Разносчик чумы'),
('49143','55095','1','0','0','0','161497','0','-152281','0','0','4000','0','0','0','-1','10','0','Разносчик чумы'),
('49143','155159','1','0','0','0','161497','0','152281','0','0','0','0','0','0','-1','7','0','Мертвящая чума + Разносчик чумы');


DELETE FROM spell_proc_check WHERE entry IN (51462);
DELETE FROM spell_trigger WHERE spell_id IN (51462);
DELETE FROM spell_proc_check WHERE entry IN (81229);
DELETE FROM spell_trigger WHERE spell_id IN (81229);

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('108201','108201','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','9','0','Оскверненная земля');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('155159','0','0','0','0','0','0','2','-1','-1','250','0','0','0','0','0','0','0','0','0','Мертвящая чума');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('155159','155165','0','20','7','1','0','0','0','0','2','0','0','0','0','0','0','-1','0','0','0','Мертвящая чума');

DELETE FROM spell_script_names WHERE spell_id IN (115994);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('115994','55078','1','0','0','0','-152281','0','0','0','0','0','0','0','0','-1','10','0','Нечестивая порча'),
('115994','55095','1','0','0','0','-152281','0','0','0','0','0','0','0','0','-1','10','0','Нечестивая порча'),
('115994','155159','1','0','0','0','152281','0','0','0','0','0','0','0','0','-1','7','0','Нечестивая порча');

DELETE FROM spell_script_names WHERE spell_id IN (113517);
