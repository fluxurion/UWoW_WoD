INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('49821','spell_pri_clarity_of_power');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('32379','spell_pri_clarity_of_power');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('73510','spell_pri_clarity_of_power');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('124495','0','0','2944','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Энтропия Бездны');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('124495','155361','0','5','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Энтропия Бездны');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('148859','155521','1','0','3','0','155271','0','0','0','0','0','0','0','0','-1','0','0','Благосклонные духи');

insert into `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) values
('152117','0','0','0','0','0','0','16384','65536','0','100','0','7'),
('155363','0','0','0','0','0','0','16384','65536','0','100','0','7');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('152117','155362','0','20','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Слова восстановления');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('155362','spell_pri_words_of_mending');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('155363','156678','0','20','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Молитва восстановления');

