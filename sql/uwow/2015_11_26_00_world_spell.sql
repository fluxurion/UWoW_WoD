DELETE FROM spell_bonus_data WHERE entry IN (17);
insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values('17','4.59','0','0','0','0','0','Priest - Power Word: Shield');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('48045','spell_pri_mind_sear');

DELETE FROM spell_bonus_data WHERE entry IN (33110);

insert into `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) values
('41637','41635','0','0','0','0','0','0','0','7','0','0','Молитва восстановления'),
('155793','41635','0','0','0','0','0','0','0','7','0','0','Молитва восстановления');
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('155793','30','0','0','0','0','0','0','7','2','1','0','0','0','Молитва восстановления');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('41635','spell_pri_prayer_of_mending');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('41635','-155793','0','41','1','0','0','0','0','-1','0','7','0','0','0','0','0','0','0','-1','0','0','0','Молитва восстановления'),
('41635','33110','0','20','1','1','0','0','0','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Молитва восстановления');

insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('8092','162532','0','4','0','0','0','0','0','0','8','0','0','0','0','0','1','Символ жатвы разума');

DELETE FROM spell_bonus_data WHERE entry IN (2944);

DELETE FROM spell_proc_check WHERE entry IN (81749);
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('81749','0','0','585','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Искупление вины'),
('81749','0','0','14914','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Искупление вины'),
('81749','0','0','47666','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Искупление вины'),
('81749','0','0','129250','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Искупление вины');

insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('81206','34861','0','23','1','0','0','0','0','0','0','2','0','0','0','0','0','0','0','-1','0','0','0','Чакра: святилище');

DELETE FROM spell_trigger WHERE spell_id IN (77485);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('77485','77489','0','3','0','0','0','0','0','2','0','1','0','0','0','0','0','0','0','-1','0','0','0','Искусность: отблеск света');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('81700','172359','0','0','0','0','157197','0','0','0','0','0','0','0','0','-1','0','0','Всесильный архангел');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('73325','157146','1','0','0','0','157145','0','0','0','0','0','0','0','0','-1','0','0','Усиленное духовное рвение');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('121536','158624','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','11','0','Божественное перышко');
DELETE FROM areatrigger_data WHERE entry IN (337);
insert into `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) values
('337','121536','3153','27404','3','3','0','0','0','0','0','0','0','0','0','0','3','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','Божественное перышко'),
('337','158624','3153','25022','3','3','0','0','0','0','0','0','0','0','0','0','3','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','Божественное перышко');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('162448','0','0','158831','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Наступление Тьмы'),
('162448','0','0','34914','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Наступление Тьмы');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('162448','87160','0','20','0','0','0','0','0','0','0','7','0','0','12','0','0','0','0','-1','0','0','0','Наступление Тьмы');

DELETE FROM spell_trigger WHERE spell_id IN (139139);
DELETE FROM spell_proc_check WHERE entry IN (139139);
replace into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('139139','0','0','139139','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Безумие');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('132573','spell_pri_insanity');

insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('140815','31','0','0','0','0','0','0','7','1','1','0','0','0','Слово силы: Утешение');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('45243','0','0','-47758','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Средоточие воли');

DELETE FROM spell_trigger WHERE spell_id IN (81749);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('81749','81751','0','3','0','0','0','0','0','0','0','1','0','0','0','0','0','0','0','-1','0','0','0','Искупление вины');
