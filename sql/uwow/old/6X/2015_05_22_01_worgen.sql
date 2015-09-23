/*!40101 SET NAMES utf8 */;
REPLACE INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
 ('67503', '4762', '0', '0', '0', '0', '2', '1', '64', '11');
-- ------------
-- Q: 14293
-- ------------

-- Save Krennan Aranas Waypoints
DELETE FROM script_waypoint WHERE entry=35905;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`) VALUES
(35905,1,-1791.55,1381.92,19.8183,0),
(35905,2,-1772.75,1361.92,19.6939,0),
(35905,3,-1759.41,1357.74,19.9291,0),
(35905,4,-1713.19,1347.35,19.6855,0),
(35905,5,-1706.18,1347.33,19.90,0),
(35905,6,-1668.35,1348.68,15.1382,0),
(35905,7,-1664.11,1356.47,15.1351,0),
(35905,8,-1678.52,1360.17,15.1359,0),
(35905,9,-1702.23,1353.7,19.2279,0),
(35905,10,-1742.5,1366.78,19.9632,0),
(35905,11,-1762.66,1390.97,19.9748,0),
(35905,12,-1774.7,1431.09,19.7806,0);

DELETE FROM creature_text WHERE entry in (35905, 35753, 35907);
INSERT INTO creature_text VALUES
(35905,0,0,'Rescue Krennan Aranas by using your vehicle''s ability.$B|TInterface/\Icons/\inv_misc_groupneedmore.blp:64|t',42,0,100,0,0,0,'King Greymane''s Horse'),
(35753,0,0,'Помогите! Сюда!',14,0,100,0,0,0,''),
(35907,0,0,'Спасибо! Я обязан тебе жизнью.',12,0,100,0,0,20922,'');

-- 
DELETE FROM conditions WHERE sourcetypeorreferenceid=13 AND sourceentry=68228;

DELETE FROM creature WHERE id=35753;
INSERT INTO `creature` ( `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(35753, 654, 1, 7, 0, 0, -1672.8, 1345.26, 20.796, 0.415266, 300, 0, 0, 42, 0, 0, 0, 0, 0);
REPLACE INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
('35753', '0', '0', '0', '0', '473', NULL);

DELETE FROM `gameobject_template` WHERE entry = 300247 AND name LIKE 'TEMP Krennan Aranas Location';
DELETE FROM `gameobject` WHERE id = 300247;

REPLACE INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(68228, 654, -1674.09, 1348.42, 15.2845, 0.0996814),
(68219, 654, -1674.09, 1348.42, 15.2845, 0.0996814);

--
UPDATE `creature_template` SET `unit_flags` = `unit_flags` | 768 WHERE `entry` in ( 35905, 35753 );

--
REPLACE INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
 ('68228', '654', '-1674.09', '1348.42', '15.2845', '0.0996814');
 
--
UPDATE creature SET phaseMask = phaseMask | 4 WHERE id = 35914;

-- ------------
-- Q: 14218
-- ------------
UPDATE `quest_template` SET `QuestType` = '2' WHERE `quest_template`.`ID` = 14218;

-- Phase start. 70696 use -654 byt where is it?
DELETE FROM spell_area WHERE area = -654 AND spell in(68481, 67789, 68482, 68483, 70695, 74093);
REPLACE INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES 
('68481', '4714', '0', '14396', '0', '0', '2', '1', '0', '66');

-- ------------
-- Q: 14395
-- ------------
REPLACE INTO `quest_start_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES 
('14395', '0', '7', '14395', '0', '0', '0', '0', '0', '0');
UPDATE `quest_template` SET `StartScript` = '14395' WHERE `quest_template`.`ID` = 14395;

-- ------------
-- Q: 24904
-- ------------
REPLACE INTO `quest_start_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES 
('24904', '0', '7', '24904', '0', '0', '0', '0', '0', '0');
UPDATE `quest_template` SET `StartScript` = '24904' WHERE `quest_template`.`ID` = 24904;

-- ------------
-- Q: 24681
-- ------------
UPDATE `quest_template` SET `QuestType` = '2' WHERE `quest_template`.`ID` = 24681;

-- ------------
-- Q: 24468 Stranded at the Marsh
-- ------------
UPDATE `creature_template` SET `KillCredit1` = '37078' WHERE `creature_template`.`entry` = 36882;

-- ------------
-- Q: 24483
-- ------------
UPDATE `quest_template_addon` SET `PrevQuestID` = '24472' WHERE `quest_template_addon`.`ID` = 24483;

-- ------------
-- Q: 24628
-- ------------
REPLACE INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `shared`) VALUES
 ('201914', '50017', '-100', '1', '0', '1', '1', '0');