/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Garrison spells
-- 175090
-- 171474
-- 155075


-- Phase's
-- 4086 global world.
-- 3194 В самом начале группа после высадки. 70859 76411 78288
-- 3441 до взятия квеста 34402
-- 3195 - npc 78272, 80192
-- 3962 4177 походу при обновлении зоны 7004 и ее подзон или вообще к этой зоне относится. это фаза гарнизона. персональная получается. а точнее это AreaInstance.
-- 3331 - scena 594 выполнена. npc 78272
-- 3196 - scena 594 выполнена. + пропадает после сцены 604 npc 78288, 70859, 76411, 80227 и потом возвращается после реварда 34364
-- 3197  - инженер из портала. 
-- 3213 - 34364 reward. 
-- 3324 - taken 34375 for area 7004 до конца.
-- 3325 - taken 34375 removed on some.
-- 3326 - rew 34375 removed with 3326
-- 3887 - taken 34378
-- 3364 - compl. 34378
REPLACE INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `PreloadMapID`, `VisibleMapID`, `flags`, `comment`) VALUES 
('6720', '1', '0', '3194', '0', '0', '16', 'Draenor. FrostFireRidge. While not rew q33868'),    --                                      2406 2537 3007 3021 3023 3025 3026 3194 3195 3427 3441 3592 4086
('6720', '2', '0', '3441 3195', '0', '0', '16', 'Draenor. FrostFireRidge. While not teken q34402'),                  -- q33815 take     2406 2537 3007 3021 3023 3025 3026 3195 3427 3441 3592 4086
--                                                                                                                      q33815 rewarded 2406 2537 3007 3021 3023 3025 3026 3195 3427 3592 4086
--                                                                                                                      q34402 take     2406 2537 3007 3021 3023 3025 3026 3427 3592 4086
('6720', '3', '0', '3962 4177', '0', '0', '16', 'Draenor. FrostFireRidge. While not teken q34402'),                  -- зона 7004       2406 2537 3007 3021 3023 3025 3026 3427 3592 3962 4086 4177
('6720', '4', '0', '3331', '0', '0', '16', 'Draenor. FrostFireRidge. scene 594 complete'),                           -- scene778 comp.  2406 2537 3007 3021 3023 3025 3026 3196 3331 3427 3592 3962 4086 4177
('6720', '5', '0', '3196', '0', '0', '16', 'Draenor. FrostFireRidge. scene 594 complete'),                           -- complete 34364  2406 2537 3007 3021 3023 3025 3026 3331 3427 3592 3962 4086 4177
('6720', '6', '0', '3197', '0', '0', '16', 'Draenor. FrostFireRidge. scene 604 trigger'),                            -- scene trig.     2406 2537 3007 3021 3023 3025 3026 3197 3331 3427 3592 3962 4086 4177
('6720', '7', '0', '3213', '0', '0', '16', 'Draenor. FrostFireRidge. 34364 rew '),                                   -- 34364 rew       2406 2537 3007 3021 3023 3025 3026 3197 3213 3427 3592 3962 4086 4177
('6720', '8', '0', '3324', '0', '0', '16', 'Draenor. FrostFireRidge. 34375 taken '),                                 --                 2406 2537 3007 3021 3023 3025 3026 3196 3197 3213 3324 3325 3427 3592 3962 4086 4177
('6720', '9', '0', '3325', '0', '0', '16', 'Draenor. FrostFireRidge. 34375 taken '),                                 
('6720', '10', '0', '3326', '0', '0', '16', 'Draenor. FrostFireRidge. 34375 rew '),                                  -- compl. 34375    2406 2537 3007 3021 3023 3025 3026 3196 3197 3213 3324 3325 3326 3427 3592 3962 4086 4177
('6720', '11', '0', '3887', '0', '0', '16', 'Draenor. FrostFireRidge. 34378 taken '),                                -- take 34378      2406 2537 3007 3021 3023 3025 3026 3196 3197 3213 3324 3325 3326 3427 3592 3887 3962 4086 4177
('6720', '12', '0', '0', '1152', '0', '24', 'Draenor. FrostFireRidge. Garrison terrain lvl.1 '),                     -- comp. 34378     2406 2537 3007 3021 3023 3025 3026 3196 3213 3324 3364 3592 3962 4086 4177
('6720', '13', '0', '3364', '0', '0', '16', 'Draenor. FrostFireRidge. 34378 complete '),                     --
('6720', '100', '0', '2406 2537 3007 3021 3023 3025 3026 3427 3592 4086', '0', '0', '16', 'Draenor. FrostFireRidge.'); --

DELETE FROM `conditions` WHERE SourceTypeOrReferenceId = 23 AND SourceGroup = 6720; 
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(23, 6720, 1, 0, 0, 14, 0, 33815, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. While non 33815'),
(23, 6720, 2, 0, 0, 14, 0, 34402, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. While non 34402'),
(23, 6720, 3, 0, 0, 23, 0, 7004, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. requare areaID=7004'),

(23, 6720, 4, 0, 0, 8, 0, 34364, 0, 0, 1, 0, '', 'Draenor. FrostFireRidge. AND not rew 34364'),
(23, 6720, 4, 1, 0, 40, 0, 594, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. scene comp. 594'),
(23, 6720, 4, 1, 1, 8, 0, 34402, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. or rew 34402'),

-- 5. пропадает пока идет 2 ролика, но после реварда 34364 появляется. Сцены используют действующие гуиды по этому возможно лучше оставить и не трогать.
(23, 6720, 5, 0, 0, 40, 0, 594, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. AND scene comp. 594'),
(23, 6720, 5, 0, 1, 8, 0, 34402, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. AND or rew 34402'),
(23, 6720, 6, 0, 0, 42, 0, 604, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge.  triger scene 604'),
(23, 6720, 6, 0, 1, 8, 0, 34364, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge.  or rew 34364'),
(23, 6720, 7, 0, 0, 8, 0, 34364, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge.  or rew 34364'),
(23, 6720, 8, 0, 0, 23, 0, 7004, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. requare areaID=7004'),
(23, 6720, 8, 0, 0, 14, 0, 34375, 0, 0, 1, 0, '', 'Draenor. FrostFireRidge. While has status 34375'),
(23, 6720, 9, 0, 0, 14, 0, 34375, 0, 0, 1, 0, '', 'Draenor. FrostFireRidge. While has status 34375'),
(23, 6720, 10, 0, 0, 8, 0, 34375, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. rew 34375'),
(23, 6720, 11, 0, 0, 14, 0, 34378, 0, 0, 1, 0, '', 'Draenor. FrostFireRidge. While has status 34378'),
(23, 6720, 12, 0, 0, 28, 0, 34378, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. compl 34378'),
(23, 6720, 12, 0, 1, 8, 0, 34378, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. rew 34378'),
(23, 6720, 13, 0, 0, 28, 0, 34378, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. compl 34378'),
(23, 6720, 13, 0, 1, 8, 0, 34378, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. rew 34378');
-- Сцены
REPLACE INTO `spell_scene` (`SceneScriptPackageID`, `MiscValue`, `PlaybackFlags`, `trigerSpell`, MonsterCredit, ScriptName, `comment`) VALUES
('771', '578', '16', '0', '0', 'sceneTrigger_q33815', 'Draenor. FrostFireRidge. q33815'),
('778', '594', '16', '0', '0', '', 'Draenor. FrostFireRidge. q34402 spell 158228'),
('789', '604', '16', '82238', '0', '', 'Draenor. FrostFireRidge. q34364 spell 169422'),
('798', '608', '16', '0', '0', '', 'Draenor. FrostFireRidge. q: 34364 on quest rew.');


-- Spell Area
DELETE FROM `spell_area` WHERE area in (6720);
REPLACE INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES 
('158228', '6720', '34402', '34402', '0', '0', '2', '1', '10', '64'),
('49416', '6720', '34375', '34375', '0', '0', '2', '1', '10', '64'),
('175536', '6720', '34378', '34378', '0', '0', '2', '0', '2', '64');

-- Quest chaine fixes.
REPLACE INTO `quest_template_addon` (`ID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`) VALUES 
('33815', '33868', '0', '0'),
('34402', '33815', '0', '0'),
('34364', '34402', '0', '0'),
('34375', '34364', '34765', '-34364'),
('34592', '34364', '34765', '-34364'),
('34765', '34592', '0', '0'),
('34378', '34765', '0', '0'),
('34822', '34378', '0', '0'),

('34379', '33816', '0', '0');

-- Q: 33868 Родина Северных Волков
REPLACE INTO `area_queststart` (`id`, `quest`) VALUES ('7257', '33868');

-- Q: 33815 Песня льда и огня
UPDATE `quest_template` SET `StartScript` = '33815', `CompleteScript` = '33815' WHERE `quest_template`.`ID` = 33815;
DELETE FROM `quest_start_scripts` WHERE id = 33815;
INSERT INTO `quest_start_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES 
('33815', '0', '35', '578', '0', '0', '0', '0', '0', '0');
DELETE FROM `quest_end_scripts` WHERE id = 33815;
INSERT INTO `quest_end_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES 
('33815', '0', '36', '578', '0', '0', '0', '0', '0', '0');

DELETE FROM `creature_text` WHERE entry = 78272;
REPLACE INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(78272, 0, 0, 'Дрек''Тар, мой старый друг, ты жив!', 14, 0, 100, 5, 0, 45256, 'Дуротан to Player');

-- Q: 34364
UPDATE `quest_template` SET `CompleteScript` = '34364' WHERE `quest_template`.`ID` = 34364;
DELETE FROM `quest_end_scripts` WHERE id = 34364;
INSERT INTO `quest_end_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES 
('34364', '0', '35', '608', '0', '0', '0', '0', '0', '0');

-- Q: 34375 За работу GO: 230527 NPC: 79525 spell: 160657 area spell: 49416
UPDATE `creature_template` SET `ScriptName` = 'mob_wod_q34375' WHERE `creature_template`.`entry` = 79526;
UPDATE `gameobject_template` SET `ScriptName` = 'go_wod_q34375' WHERE `gameobject_template`.`entry` = 230527;
REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`) VALUES ('79526', '1', '109579', '0', '0');

-- Q: 34378
REPLACE INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, actiontype, `comment`)
VALUES ('161033', '160767', '0', '0', 'Q34378 link create garnisone');