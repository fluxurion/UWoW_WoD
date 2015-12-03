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

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('74434','93313','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Warlock - Seed of Corruption(Soulburn)'),
('74434','104249','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Warlock - Demonic Circle: Teleport(Soulburn)'),
('-74434','-104249','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Soulburn Dummy'),
('-74434','-93313','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Soulburn Dummy');

DELETE FROM spell_proc_event WHERE entry IN (108558);
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('108558','0','0','146739','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Сумерки');

replace into `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) values
('657','120644','3919','24817','5','30','0','0','0','0','0','0','0','0','1000','0','0','0','0','0','0','0','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','Сияние'),
('658','120517','3921','24801','5','30','0','0','0','0','0','0','0','0','1000','0','0','0','0','0','0','0','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','Сияние');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('348','157736','1','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Жертвенный огонь'),
('108686','157736','1','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Жертвенный огонь');

insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('17962','157736','0','4','0','0','3','0','0','0','7','0','0','0','0','0','1','Поджигание');

