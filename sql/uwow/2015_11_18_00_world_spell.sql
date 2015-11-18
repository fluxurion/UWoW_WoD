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

DELETE FROM spell_trigger WHERE spell_id IN (53563);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('53563','88852','0','2','7','1','0','0','40','0','0','4','0','0','0','0','0','0','0','-1','0','0','0','Частица Света'),
('53563','157128','0','20','1','1','1','0','0','0','0','1','157047','-157131','0','0','0','0','0','-1','0','0','0','Спасенный Светом');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('154436','0','0','100784','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Стойка смелого журавля (Стойка ткача туманов)');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('154436','127722','0','20','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Техники стиля журавля'),
('154436','159621','0','3','0','0','0','0','50','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Техники стиля журавля');
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('159621','31','0','0','0','0','0','0','7','1','4','0','0','0','Техники стиля журавля - величие');

DELETE FROM spell_trigger WHERE spell_id IN (130324);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('130324','117895','0','3','0','0','1','0','50','0','0','7','154436','0','0','0','0','0','0','-1','0','0','0','Величие (статуя) - Монастырские знания');
