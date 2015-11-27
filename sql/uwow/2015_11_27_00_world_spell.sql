DELETE FROM spell_proc_event WHERE entry IN (12846);
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('12846','0','0','133','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Искусность: воспламенение'),
('12846','0','0','108853','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Искусность: воспламенение'),
('12846','0','0','2948','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Искусность: воспламенение'),
('12846','0','0','11366','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Искусность: воспламенение'),
('12846','0','0','2120','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Искусность: воспламенение'),
('12846','0','0','153564','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Искусность: воспламенение');

DELETE FROM spell_trigger WHERE spell_id IN (77485);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('77485','77489','0','3','0','0','0','0','0','2','0','1','0','0','0','0','0','0','0','-1','0','0','0','Искусность: отблеск света');

