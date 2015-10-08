/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Phase's
-- 4086 global world.
-- 3441 В самом начале группа после высадки.
-- 3195 до взятия квеста 34402
REPLACE INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `PreloadMapID`, `VisibleMapID`, `flags`, `comment`) VALUES 
('6720', '1', '0', '3441', '0', '0', '16', 'Draenor. FrostFireRidge. While not rew q33868'),
('6720', '2', '0', '3195', '0', '0', '16', 'Draenor. FrostFireRidge. While not teken q34402'),
('6720', '3', '0', '2406 2537 3007 3021 3023 3025 3026 3427 3592 4086', '0', '0', '16', 'Draenor. FrostFireRidge.');

DELETE FROM `conditions` WHERE SourceTypeOrReferenceId = 23 AND SourceGroup = 6720;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(23, 6720, 1, 0, 0, 14, 0, 33815, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. While non 33815'),
(23, 6720, 2, 0, 0, 14, 0, 34402, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. While non 34402');

-- Сцены
REPLACE INTO `spell_scene` (`SceneScriptPackageID`, `MiscValue`, `PlaybackFlags`, `trigerSpell`, MonsterCredit, `comment`) VALUES
('771', '578', '16', '0', '0', 'Draenor. FrostFireRidge. q33815'),
('778', '594', '16', '0', '0', 'Draenor. FrostFireRidge. q34402 spell 158228');
UPDATE spell_scene SET `ScriptName` = 'sceneTrigger_q33815' WHERE MiscValue = 578;

-- Spell Area
DELETE FROM `spell_area` WHERE area in (6720);
REPLACE INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES 
('158228', '6720', '34402', '34402', '0', '0', '2', '1', '10', '64');

-- Quest chaine fixes.
REPLACE INTO `quest_template_addon` (`ID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`) VALUES 
('33815', '33868', '0', '0'),
('34402', '33815', '0', '0'),
('34364', '34402', '0', '0'),

('34379', '33816', '0', '0');

-- Q: Родина Северных Волков
REPLACE INTO `area_queststart` (`id`, `quest`) VALUES ('7257', '33868');

-- Q: Песня льда и огня
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