insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('-115191','-158188','0','0','0','0','0','0','0','0','0','0','0','0','Stelth - remove shapeshift'),
('-115191','-158185','0','0','0','0','0','0','0','0','0','0','0','0','Stelth - remove stelth'),
('115191','158185','0','0','0','0','0','0','0','0','0','0','0','0','Stelth - stelth'),
('115191','158188','0','0','0','0','0','0','0','0','0','0','0','0','Stelth - shapeshift');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('51723','139546','1','0','0','0','0','0','0','0','1','0','0','0','КП от Веер клинков');

DELETE FROM spell_linked_spell WHERE spell_trigger IN (86392);

DELETE FROM spell_proc_check WHERE entry IN (79134);
REPLACE INTO `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) VALUES('79134','0','0','1943','0','0','0','2','-1','-1','0','0','0','0','0','0','0','0','0','0','0');

insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('14161','13750','0','23','0','0','0','0','0','0','4','0','0','0','0','0','0','-1','0','0','0','Жестокость'),
('14161','2983','0','23','0','0','0','0','0','0','4','0','0','0','0','0','0','-1','0','0','0','Жестокость'),
('14161','51690','0','23','0','0','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Жестокость');
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values('14161','0','0','0','0','0','0','4','-1','-1','0','0','0','0','0','0','0','1','0','0','Жестокость');

DELETE FROM spell_linked_spell WHERE spell_trigger IN (51753);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('51753','51755','2','5','0','0','0','0','0','0','0','0','0','6','Camouflage'),
('51753','80326','2','5','0','0','0','0','0','0','0','0','0','6','Camouflage'),
('51753','160729','0','0','0','0','0','0','0','0','0','0','0','0','Camouflage'),
('-51755','-51753','0','0','0','0','0','0','0','0','0','0','0','0','Camouflage'),
('-51755','-160729','0','0','0','0','0','0','0','0','0','0','0','0','Camouflage');

DELETE FROM spell_trigger WHERE spell_id IN (58423);

DELETE FROM spell_trigger WHERE spell_id IN (14161);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('14161','2983','0','23','1','0','0','0','0','0','4','0','0','0','0','0','0','-1','0','0','0','Жестокость'),
('14161','13750','0','23','1','0','0','0','0','0','4','0','0','0','0','0','0','-1','0','0','0','Жестокость'),
('14161','51690','0','23','1','0','0','0','0','0','4','0','0','0','0','0','0','-1','0','0','0','Жестокость');
