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

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('73510','-87160','8','0','0','0','0','0','0','0','0','0','0','0','0','-1','7','0','Пронзание разума');

insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('129197','139139','1','9','0','0','0','0','0','0','7','0','0','0','0','0','1','Безумие');

insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('2147','0','1','0','2','122128','0','0','0','0','0','0','0','Божественная звезда'),
('2148','0','1','0','1','110745','0','0','0','0','0','0','0','Божественная звезда');
replace into `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) values
('2147','122121','7262','25152','4','4','0','0','0','0','1','5','0','30','0','0','0','0','0','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','Божественная звезда'),
('2148','110744','6700','22903','4','4','0','0','0','0','1','5','0','30','0','0','0','0','0','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','Божественная звезда');
