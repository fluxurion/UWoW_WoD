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

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('107428','130320','1','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Удар восходящего солнца');

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('117418','spell_monk_fists_of_fury');

insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('115129','16','0','0','0','0','0','0','7','1','1','0','0','0','Устранение вреда');

DELETE FROM spell_bonus_data WHERE entry IN (115072);

DELETE FROM spell_script_names WHERE spell_id IN (117405,109248);
replace into `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) values
('1524','109248','5972','24326','5','5','2','2','0.3','0.3','0','0','0','0','0','1000','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','Связующий выстрел');
insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('1524','0','1','0','2','117405','0','0','0','0','0','0','0','Связующий выстрел'),
('1524','1','42','1','2','117405','0','0','0','0','0','0','0','Связующий выстрел'),
('1524','2','2','0','2','117526','0','0','0','0','0','0','0','Связующий выстрел'),
('1524','3','4','0','98','117614','0','0','0','0','0','0','0','Binding Shot Visual');

replace into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('117418','0','0','0','0','7.755','0','Неистовые кулаки'),
('100780','0','0','0','0','1.38','0','Дзуки'),
('108557','0','0','0','0','1.38','0','Дзуки'),
('115698','0','0','0','0','1.38','0','Дзуки'),
('115687','0','0','0','0','1.38','0','Дзуки'),
('115693','0','0','0','0','1.38','0','Дзуки'),
('115695','0','0','0','0','1.38','0','Дзуки'),
('100787','0','0','0','0','3','0','Лапа тигра'),
('107270','0','0','0','0','0.9','0','Танцующий журавль'),
('148187','0','0','0','0','0.72','0','Порыв нефритового ветра'),
('107428','0','0','0','0','10.56','0','Удар восходящего солнца'),
('100784','0','0','0','0','6.4','0','Нокаутирующий удар'),
('121253','0','0','0','0','14.5','0','Удар бочонком'),
('115072','0','0','0','0','0','7.5','Устранение вреда'),
('147489','0','0','0','0','0','11.4','Устранение вреда');

