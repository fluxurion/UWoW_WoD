DELETE FROM `instance_template` where `map` = 1148;
INSERT INTO `instance_template` (`map`, `parent`, `script`, `allowMount`, `bonusChance`) VALUES 
(1148, 0, 'instance_proving_grounds', 0, 0);

DELETE FROM `lfg_entrances` where `dungeonId` = 640;
INSERT INTO `lfg_entrances` (`dungeonId`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(640, 3756.82, 521.777, 639.692, 2.509592); -- 640

UPDATE `creature_template` SET `gossip_menu_id`=16811 WHERE `entry`=79953;
UPDATE `creature_template` SET `gossip_menu_id`=13807, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=1770, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=61636;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=16, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33554432, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=61638;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=1770, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=1905, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33554432, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=18, `WorldEffectID`=0, `AiID`=1507, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=71820;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=35, `npcflag`=4224, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=0.888888, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=73327;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=60, `MaxLevel`=60, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=73328;

DELETE FROM `gossip_menu` WHERE (`entry`=13807 AND `text_id`=19911) OR (`entry`=16811 AND `text_id`=24440);
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(16811, 24440), -- 79953
(13807, 19911); -- 61636

DELETE FROM `gossip_menu_option` WHERE (`menu_id`=16811 AND `id`=0) OR (`menu_id`=13807 AND `id`=0) OR (`menu_id`=13807 AND `id`=1) OR (`menu_id`=13807 AND `id`=2) OR (`menu_id`=13807 AND `id`=3) OR (`menu_id`=13807 AND `id`=4) OR (`menu_id`=13807 AND `id`=5) OR (`menu_id`=13807 AND `id`=6) OR (`menu_id`=13807 AND `id`=7) OR (`menu_id`=13807 AND `id`=8) OR (`menu_id`=13807 AND `id`=9) OR (`menu_id`=13807 AND `id`=10) OR (`menu_id`=13807 AND `id`=11);
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `box_coded`, `box_money`, `box_text`) VALUES
(16811, 0, 0, 'Enter the Arena challenge', 21, 0, 640, ''), -- 79953 non blizzlike text :C
(13807, 0, 0, 'Start a basic challenge of a fighter (Bronze)', 0, 0, 0, ''), -- 61636
(13807, 1, 0, 'Start a basic challenge of a fighter (silver)', 0, 0, 0, ''), -- 61636
(13807, 2, 0, 'Start a basic challenge of a fighter (gold)', 0, 0, 0, ''), -- 61636
(13807, 3, 0, 'Start a basic challenge of a fighter (endless mode)', 0, 0, 0, ''), -- 61636
(13807, 4, 0, 'Start a basic challenge of a tank (бронза)', 0, 0, 0, ''), -- 61636
(13807, 5, 0, 'Start a basic challenge of a tank (silver)', 0, 0, 0, ''), -- 61636
(13807, 6, 0, 'Start a basic challenge of a tank (gold)', 0, 0, 0, ''), -- 61636
(13807, 7, 0, 'Start a basic challenge of a tank (endless mode)', 0, 0, 0, ''), -- 61636
(13807, 8, 0, 'Start a basic challenge of a heal (бронза)', 0, 0, 0, ''), -- 61636
(13807, 9, 0, 'Start a basic challenge of a heal (silver)', 0, 0, 0, ''), -- 61636
(13807, 10, 0, 'Start a basic challenge of a heal (gold)', 0, 0, 0, ''), -- 61636
(13807, 11, 0, 'Start a basic challenge of a heal (endless mode)', 0, 0, 0, ''); -- 61636

DELETE FROM `locales_gossip_menu_option` WHERE (`menu_id`=16811 AND `id`=0) OR (`menu_id`=13807 AND `id`=0) OR (`menu_id`=13807 AND `id`=1) OR (`menu_id`=13807 AND `id`=2) OR (`menu_id`=13807 AND `id`=3) OR (`menu_id`=13807 AND `id`=4) OR (`menu_id`=13807 AND `id`=5) OR (`menu_id`=13807 AND `id`=6) OR (`menu_id`=13807 AND `id`=7) OR (`menu_id`=13807 AND `id`=8) OR (`menu_id`=13807 AND `id`=9) OR (`menu_id`=13807 AND `id`=10) OR (`menu_id`=13807 AND `id`=11);
INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc8`) VALUES
(16811, 0, 'Войти на Арену испытаний'), -- 79953
(13807, 0, 'Начать базовое испытание бойца (бронза)'), -- 79953
(13807, 1, 'Начать базовое испытание бойца (серебро)'), -- 79953
(13807, 2, 'Начать базовое испытание бойца (золото)'), -- 79953
(13807, 3, 'Начать базовое испытание бойца (бесконечный режим)'), -- 79953
(13807, 4, 'Начать базовое испытание танка (бронза)'), -- 79953
(13807, 5, 'Начать базовое испытание танка (серебро)'), -- 79953
(13807, 6, 'Начать базовое испытание танка (золото)'), -- 79953
(13807, 7, 'Начать базовое испытание танка (бесконечный режим)'), -- 79953
(13807, 8, 'Начать базовое испытание лекаря (бронза)'), -- 79953
(13807, 9, 'Начать базовое испытание лекаря (серебро)'), -- 79953
(13807, 10, 'Начать базовое испытание лекаря (золото)'), -- 79953
(13807, 11, 'Начать базовое испытание лекаря (бесконечный режим)'); -- 79953

SELECT @CGUID := `guid`+1 FROM `creature` ORDER BY `creature`.`guid` DESC limit 1;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+4;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `phaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@CGUID+0, 71820, 1148, 3, 1, '', 3849.864, 526.9305, 658.7128, 2.984779, 7200, 0, 0), -- 71820 (Area: 6852) (Auras: 144041 - 144041)
(@CGUID+1, 73328, 1148, 3, 1, '', 3749.479, 515.8871, 639.774, 0.9554676, 7200, 0, 0), -- 73328 (Area: 6852)
(@CGUID+2, 61638, 1148, 3, 1, '', 3795.603, 533.7483, 649.0903, 0, 7200, 0, 0), -- 61638 (Area: 6852)
(@CGUID+3, 73327, 1148, 3, 1, '', 3747.675, 520.0799, 639.774, 0.5023382, 7200, 0, 0), -- 73327 (Area: 6852)
(@CGUID+4, 61636, 1148, 3, 1, '', 3751.513, 524.3802, 639.7741, 0, 7200, 0, 0); -- 61636 (Area: 6852) (Auras: )

SELECT @OGUID := `guid`+1 FROM `gameobject` ORDER BY `guid` DESC limit 1;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+2;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `phaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(@OGUID+0, 211668, 1148, 3, 1, '', 3700.17, 362.7052, 597.2863, 2.836158, 0, 0, 0.9883615, 0.1521232, 7200, 255, 1), -- 211668 (Area: 6852)
(@OGUID+1, 211668, 1148, 3, 1, '', 3666.686, 637.7451, 637.3908, 4.337144, 0, 0, -0.8265896, 0.5628051, 7200, 255, 1), -- 211668 (Area: 6852)
(@OGUID+2, 222794, 1148, 3, 1, '', 3748.704, 516.7153, 639.6907, 0, 0, 0, 0, 1, 7200, 255, 1); -- 222794 (Area: 6852)
