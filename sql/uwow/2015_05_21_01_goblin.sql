/*!40101 SET NAMES utf8 */;
--
delete from creature where id in (34696, 34689, 34695, 48304) and phaseMask != 15;
-- Mage Trainer
SELECT `guid` FROM creature where id = 34696 LIMIT 1 INTO @warlock;
UPDATE  `creature_template` SET  `AIName` =  'SmartAI' WHERE  `entry` =34689;
UPDATE  `creature_equip_template` SET  `ItemEntry1` =  '11588' WHERE `entry` =34689;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 34689;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(34689, 0, 0, 0, 1, 0, 100, 0, 5000, 5000, 15000, 15000, 11, 69608, 0, 0, 0, 0, 0, 10, @warlock, 34696, 0, 0, 0, 0, 0, 'Cast Fireball OOC');

-- Warlock Trainer
SELECT `guid` FROM creature where id = 34689 LIMIT 1 INTO @mage;
UPDATE  `creature_template` SET  `AIName` =  'SmartAI' WHERE  `entry` =34696;
UPDATE  `creature_equip_template` SET  `ItemEntry1` =  '27923' WHERE `entry` =34696;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 34696;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(34696, 0, 0, 0, 1, 0, 100, 0, 10000, 10000, 20000, 20000, 11, 69607, 0, 0, 0, 0, 0, 10, @mage, 34689, 0, 0, 0, 0, 0, 'Cast Shadow bolt OOC');

-- Shaman Trainer
-- 69626. 69630
UPDATE  `creature_template` SET  `AIName` =  'SmartAI' WHERE  `entry` =34695;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 34695;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(34695, 0, 0, 0, 1, 0, 100, 0, 500, 1000, 600000, 600000, 11, 78273, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Flametongue Weapon on Spawn'),
(34695, 0, 1, 0, 1, 0, 100, 0, 7500, 7500, 20000, 20000, 11, 69626, 0, 0, 0, 0, 0, 10, @mage, 34689, 0, 0, 0, 0, 0, 'Cast Lightning Bolt OOC');

-- Hunter Trainer
SELECT `guid` FROM creature where id = 48304 LIMIT 1 INTO @train;
UPDATE  `creature_template` SET  `AIName` =  'SmartAI' WHERE  `entry` =34673;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 34673;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(34673, 0, 0, 0, 1, 0, 100, 0, 10000, 10000, 20000, 20000, 11, 69509, 0, 0, 0, 0, 0, 10, @train, 48304, 0, 0, 0, 0, 0, 'Cast Shoot Gun OOC');

--
DELETE FROM `waypoint_data` WHERE `id` = 371140;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `delay`) VALUES 
('371140', '1', '-8289.612', '1479.857', '43.79533', '0'), 
('371140', '2', '-8288.619', '1479.970', '43.79533', '5000'), 
('371140', '3', '-8278.678', '1481.103', '43.17033', '5000'), 
('371140', '4', '-8268.736', '1482.235', '42.54533', '5000'), 
('371140', '5', '-8260.878', '1483.131', '42.17003', '5000');
DELETE FROM `waypoint_data` WHERE `id` = 371141;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `delay`) VALUES 
('371141', '1', '-8274.75', '1484.469', '42.92033', '0'), 
('371141', '2', '-8273.75', '1484.46', '42.92033', '5000'), 
('371141', '3', '-8264.75', '1484.381', '42.3981', '5000'), 
('371141', '4', '-8260.942', '1484.347', '42.1601', '10000');
DELETE FROM `waypoint_data` WHERE `id` = 371142;
-- lost from 2014_06_12_00_world_goblin.sql
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `delay`) VALUES 
('371142', '1', '-8289.076', '1487.813', '43.92033', '0'), 
('371142', '2', '-8288.08', '1487.72', '43.92033', '5000'), 
('371142', '3', '-8281.107', '1487.07', '43.29533', '5000'), 
('371142', '4', '-8269.154', '1485.957', '42.67033', '5000'), 
('371142', '5', '-8261.186', '1485.215', '42.1481', '5000');
DELETE FROM `waypoint_data` WHERE `id` = 371143;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `delay`) VALUES 
('371143', '1', '-8282.017', '1477.271', '43.29533', '0'), 
('371143', '2', '-8281.041', '1477.49', '43.29533', '5000'), 
('371143', '3', '-8270.32', '1479.907', '42.67033', '5000'), 
('371143', '4', '-8261.549', '1481.885', '42.1481', '5000'), 
('371143', '5', '-8260.705', '1482.075', '42.1481', '5000');
DELETE FROM `waypoint_data` WHERE `id` = 371144;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `delay`) VALUES 
('371144', '1', '-8282.311', '1490.609', '43.54533', '0'), 
('371144', '2', '-8281.33', '1490.41', '43.54533', '5000'), 
('371144', '3', '-8273.486', '1488.824', '42.92033', '5000'), 
('371144', '4', '-8264.662', '1487.04', '42.3981', '5000'), 
('371144', '5', '-8260.743', '1486.248', '42.14829', '5000');
DELETE FROM `waypoint_data` WHERE `id` = 371145;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `delay`) VALUES 
('371145', '1', '-8296.1', '1484.925', '44.29533', '0'), 
('371145', '2', '-8295.1', '1484.91', '44.29533', '5000'), 
('371145', '3', '-8284.1', '1484.749', '43.54533', '5000'), 
('371145', '4', '-8274.1', '1484.603', '42.92033', '5000'), 
('371145', '5', '-8265.1', '1484.471', '42.3981', '5000'), 
('371145', '6', '-8260.942', '1484.41', '42.1481', '5000');
DELETE FROM `waypoint_data` WHERE `id` = 371146;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `delay`) VALUES 
('371146', '1', '-8295.637', '1474.465', '44.17033', '0'), 
('371146', '2', '-8294.66', '1474.68', '44.17033', '5000'), 
('371146', '3', '-8284.895', '1476.818', '43.54533', '5000'), 
('371146', '4', '-8274.152', '1479.171', '42.92033', '5000'), 
('371146', '5', '-8266.34', '1480.882', '42.3981', '5000'), 
('371146', '6', '-8260.713', '1482.114', '42.1481', '5000');
DELETE FROM `waypoint_data` WHERE `id` = 371147;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `delay`) VALUES 
('371147', '1', '-8295.589', '1493.881', '44.29533', '0'), 
('371147', '2', '-8294.611', '1493.67', '44.29533', '5000'), 
('371147', '3', '-8284.846', '1491.561', '43.67033', '5000'), 
('371147', '4', '-8275.08', '1489.451', '43.04533', '5000'), 
('371147', '5', '-8266.291', '1487.553', '42.5231', '5000'), 
('371147', '6', '-8260.721', '1486.349', '42.16659', '5000');

-- Q: The Great Bank Heist 14122
UPDATE `creature_template` SET `unit_flags2` = '0' WHERE `creature_template`.`entry` = 35486;

-- Q: 14125
INSERT INTO `quest_start_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES ('14125', '0', '8', '37598', '0', '0', '0', '0', '0', '0');
UPDATE `quest_template` SET `StartScript` = '14125' WHERE `quest_template`.`ID` = 14125;

-- Q: 24671
UPDATE `quest_template` SET `QuestType` = '2' WHERE `quest_template`.`ID` = 24671;

--
DELETE FROM `gossip_menu_option` WHERE `menu_id`=11146;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(11146, 0, 0, 'Подними меня в небо, Хамми!', 1, 2, 0, 0, 0, 0, '');
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `box_coded`, `box_money`, `box_text`, npc_option_npcflag) VALUES
(11146, 1, 0, 'Хамми, отправляемся в Оргриммар, пока этот остров окончательно не взорвался!', 1, 0, 0, '', 2);

--
INSERT INTO `quest_start_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES ('25184', '0', '8', '39335', '0', '0', '0', '0', '0', '0');
UPDATE `quest_template` SET `StartScript` = '25184' WHERE `quest_template`.`ID` = 25184;