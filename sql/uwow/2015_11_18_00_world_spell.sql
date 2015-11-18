DELETE FROM spell_trigger WHERE spell_id IN (116095);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('116095','116095','0','5','1','1','0','0','0','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Вывести из строя');

insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('2763','0','1','0','9','166646','0','0','0','0','0','0','0','Стойка разъяренного тигра (Стойка)'),
('2763','1','42','1','9','166646','0','0','0','0','0','0','0','Стойка разъяренного тигра (Стойка)');

DELETE FROM areatrigger_actions WHERE entry IN (718);
insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('718','0','1','0','2','140023','0','0','0','0','0','0','0','Круг мира'),
('718','1','42','1','2','140023','0','0','0','0','0','0','0','Круг мира');
