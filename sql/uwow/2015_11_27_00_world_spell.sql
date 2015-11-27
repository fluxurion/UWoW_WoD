DELETE FROM spell_proc_event WHERE entry IN (12846);
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('12846','0','0','133','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','€скусность: воспламенение'),
('12846','0','0','108853','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','€скусность: воспламенение'),
('12846','0','0','2948','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','€скусность: воспламенение'),
('12846','0','0','11366','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','€скусность: воспламенение'),
('12846','0','0','2120','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','€скусность: воспламенение'),
('12846','0','0','153564','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','€скусность: воспламенение');

DELETE FROM spell_trigger WHERE spell_id IN (77485);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('77485','77489','0','3','0','0','0','0','0','2','0','1','0','0','0','0','0','0','0','-1','0','0','0','€скусность: отблеск света');

DELETE FROM spell_script_names WHERE spell_id IN (121135,127632);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('121135','spell_pri_cascade');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('127632','spell_pri_cascade_shadow');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('120840','spell_pri_cascade_marker');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('127631','spell_pri_cascade_marker');
DELETE FROM spell_script_names WHERE ScriptName IN ('spell_pri_cascade_trigger');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('127628','spell_pri_cascade_trigger');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('121146','spell_pri_cascade_trigger');
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('127630','8','15','0','0','0','0','0','7','1','2','0','0','0','Каскад - дамаг'),
('127630','8','13','-127628','0','0','0','0','7','0','0','0','0','0','Каскад - дамаг'),
('120786','8','5','0','0','0','0','0','7','1','2','0','0','0','Каскад - хил'),
('120786','8','13','-121146','0','0','0','0','7','0','0','0','0','0','Каскад - хил');

