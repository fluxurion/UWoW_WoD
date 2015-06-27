DELETE FROM `conditions` WHERE SourceTypeOrReferenceId = 23 AND SourceGroup = 4;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(23, 4, 1, 0, 0, 1, 0, 176111, 0, 0, 1, 0, '', 'DARK_PORTAL_WOD no time back aura'),
(23, 4, 1, 0, 0, 27, 0, 89, 1, 0, 0, 0, '', 'DARK_PORTAL_WOD higher then 89'),
(23, 4, 2, 0, 0, 1, 0, 176111, 0, 0, 1, 0, '', 'DARK_PORTAL_WOD no time back aura'),
(23, 4, 3, 0, 0, 1, 0, 176111, 0, 0, 1, 0, '', 'DARK_PORTAL_WOD no time back aura');

-- Q36881 Q34398
INSERT IGNORE INTO `gameobject_questrelation` (`id`, `quest`) VALUES
(206109, 34398),
(206111, 34398),
(206116, 34398),
(206294, 34398),
(207320, 34398),
(207323, 34398);

--
INSERT IGNORE INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `RewardMailTemplateID`, `RewardMailDelay`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES 
('36881', '0', '0', '0', '0', '0', '34398', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'),
('34398', '0', '0', '0', '0', '0', '34398', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');

UPDATE `creature_template` SET `gossip_menu_id`=16863, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=35, `npcflag`=3, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=33024, `unit_flags2`=34816, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=78423;
UPDATE `creature_template` SET `gossip_menu_id`=16518, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=2636, `npcflag`=4739, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33024, `unit_flags2`=34816, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=80521;

DELETE FROM `gossip_menu` WHERE (`entry`=16376 AND `text_id`=23740) OR (`entry`=16863 AND `text_id`=24524) OR (`entry`=16641 AND `text_id`=24170) OR (`entry`=16428 AND `text_id`=23808) OR (`entry`=16858 AND `text_id`=24515) OR (`entry`=16433 AND `text_id`=23824) OR (`entry`=16518 AND `text_id`=23994);
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(16376, 23740), -- 78559
(16863, 24524), -- 78423
(16641, 24170), -- 78556
(16428, 23808), -- 79316
(16858, 24515), -- 78563
(16433, 23824), -- 79675
(16518, 23994); -- 78568

ALTER TABLE `gossip_menu_option` CHANGE `option_id` `option_id` TINYINT(3) UNSIGNED NOT NULL DEFAULT '1';
ALTER TABLE `gossip_menu_option` CHANGE `npc_option_npcflag` `npc_option_npcflag` INT(10) UNSIGNED NOT NULL DEFAULT '1';

DELETE FROM `gossip_menu_option` WHERE (`menu_id`=16863 AND `id`=0) OR (`menu_id`=16641 AND `id`=0) OR (`menu_id`=16518 AND `id`=1) OR (`menu_id`=16518 AND `id`=0);
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `box_coded`, `box_money`, `box_text`) VALUES
(16863, 0, 0, 'ЗА АЗЕРОТ!', 0, 0, 'Вы собираетесь отправиться на опасное задание и не сможете тотчас вернуться. Вы уверены?'), -- 78423
(16641, 0, 0, 'Кадгар попросил нас отвлечь внимание Ока Килрогга.', 0, 0, ''), -- 78556
(16518, 1, 0, 'Мне бы хотелось купить что-нибудь у вас.', 0, 0, ''), -- 78568
(16518, 0, 0, 'Да. Помоги-ка мне управиться с этим огромным танком.', 0, 0, ''); -- 78568

INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES 
('24524', '<Верховный маг пристально смотрит на портал.>$b$bСмотри, их число уменьшилось, пусть и ненадолго. Возможно, это наш шанс.$b$bТы $gготов:готова; сразиться с тем, что ждет нас по ту сторону портала?', NULL, '0', '0', '0', '0', '0', '0', '0', '0', NULL, NULL, '0', '0', '0', '0', '0', '0', '0', '0', NULL, NULL, '0', '0', '0', '0', '0', '0', '0', '0', NULL, NULL, '0', '0', '0', '0', '0', '0', '0', '0', NULL, NULL, '0', '0', '0', '0', '0', '0', '0', '0', NULL, NULL, '0', '0', '0', '0', '0', '0', '0', '0', NULL, NULL, '0', '0', '0', '0', '0', '0', '0', '0', NULL, NULL, '0', '0', '0', '0', '0', '0', '0', '0', '1');

DELETE FROM `conditions` WHERE SourceTypeOrReferenceId = 15 AND SourceGroup = 16863;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
('15', '16863', '0', '0', '0', '9', '0', '34398', '0', '0', '0', '0', '', NULL),
('15', '16863', '0', '0', '1', '9', '0', '36881', '0', '0', '0', '0', '', NULL);

--
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
('167771', '1265', '4066.5', '-2382.25', '94.8536', '1.570796');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=78423;
DELETE FROM smart_scripts WHERE entryorguid = 78423;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(78423, 0, 0, 0, 62, 0, 100, 0, 16863, 0, 0, 0, 11, 168956, 18, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'At gossip select cast movie'),
(78423, 0, 1, 0, 62, 0, 100, 0, 16863, 0, 0, 0, 11, 167771, 18, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'At gossip select cast teleportation');

--
-- 3880 while not complete Q35933. but nothing linked whith it.
-- 3764 brake portal door. id 234622
-- 3948 new destroid portal. id 234623
-- 4150 crystal L id 236913
-- 4151 crystal R id 236914
-- 4143 Teron'gor Credit
-- 4142 Cho'gall Credit
REPLACE INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `PreloadMapID`, `VisibleMapID`, `flags`, `comment`) VALUES 
('7025', '1', '0', '3248 3249 3250 3251 3263 3480 3563 3568 3605 3693 3712 3763 3764 3824 3833 3834 3880 3946 4142 4143 4200', '0', '0', '16', 'Draenor Dark Portal Intro'),
('7025', '2', '0', '', '0', '992', '0', 'Draenor Dark Portal Intro'),
('7025', '3', '0', '', '0', '683', '0', 'Draenor Dark Portal Intro'),
-- remove 3880 complete Q35933
('7025', '4', '0', '3248 3249 3250 3251 3263 3480 3563 3568 3605 3693 3712 3763 3764 3824 3833 3834 3946 4142 4143 4200', '0', '0', '16', 'DraenorIntro: Q34392 started'),

-- 3248 3249 3250 3251 3263 3480 3563 3568 3605 3693 3712 3764 3824 3833 3834 3947 4143 4150 4200 QuestID: 34392 ObjectID: 82606 remove 3605 Cho'gall Credit
-- 3248 3249 3250 3251 3263 3480 3563 3568 3605 3693 3712 3824 3833 3834 3948 4150 4151 4200 quest 34392 ObjectID: 82607 remove Teron'gor Credit

-- 3248 3249 3250 3251 3263 3480 3563 3568 3693 3712 3824 3833 3834 3948 4150 4151 4200 
('7025', '5', '0', '3248 3249 3250 3251 3263 3480 3563 3568 3693 3712 3824 3833 3834 3948 4150 4151 4200', '0', '0', '16', 'DraenorIntro: Q34393 started');

DELETE FROM `conditions` WHERE SourceTypeOrReferenceId = 23 AND SourceGroup = 7025;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(23, 7025, 1, 0, 0, 8, 0, 35933, 0, 0, 1, 0, '', 'DARK_PORTAL_INTRO PHASE not rewarded q35933'),
(23, 7025, 2, 0, 0, 27, 0, 89, 1, 0, 0, 0, '', 'DARK_PORTAL_INTRO PHASE1.1'),
(23, 7025, 3, 0, 0, 27, 0, 89, 1, 0, 0, 0, '', 'DARK_PORTAL_INTRO PHASE1.2'),
(23, 7025, 4, 0, 0, 8, 0, 35933, 0, 0, 0, 0, '', 'DARK_PORTAL_INTRO completed q35933'),
(23, 7025, 4, 0, 0, 8, 0, 34392, 0, 0, 1, 0, '', 'DARK_PORTAL_INTRO completed not Q34392'),
(23, 7025, 5, 0, 0, 8, 0, 34392, 0, 0, 0, 0, '', 'DARK_PORTAL_INTRO completed Q34392'),
(23, 7025, 5, 0, 0, 8, 0, 34393, 0, 0, 1, 0, '', 'DARK_PORTAL_INTRO completed not Q34393');
--
INSERT INTO `game_tele` (`id`, `position_x`, `position_y`, `position_z`, `orientation`, `map`, `name`) VALUES 
(NULL, '4066.5', '-2382.25', '94.8536', '1.570796', '1265', 'DarkPortalIntro');

--
REPLACE INTO `spell_scene` (`ScenePackageId`, `MiscValue`, `hasO`, `PlaybackFlags`, `bit16`, `x`, `y`, `z`, `o`, `transport`, `comment`) VALUES
('937', '772', '1', '16', '0', '0', '0', '0', '0', '0', 'Темный портал: событие дворфов spell 164678'),	-- Закрывает дверь в гробницу.
('923', '754', '1', '16', '0', '0', '0', '0', '0', '0', 'Темный портал: грозная армия spell 163799 '),
('1018','733', '1', '16', '0', '0', '0', '0', '0', '0', 'Темный портал: обучение души spell 163341'), 
('961', '811', '1', '16', '0', '0', '0', '0', '0', '0', 'Q34392 Free Cho\'gall Credit spell 166407'),
('962', '812', '1', '16', '0', '0', '0', '0', '0', '0', 'Q34392 Free Teron\'gor Credit spell 166408');

-- Basic area auras
REPLACE INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES 
('164678', '7025', '0', '35933', '0', '0', '2', '1', '64', '65'), 
('164678', '7025', '34393', '0', '0', '0', '2', '1', '88', '11'),
('163799', '7025', '0', '0', '0', '0', '2', '1', '64', '11'),
('163341', '7025', '0', '0', '0', '0', '2', '1', '64', '11'), 
('167421', '7025', '0', '0', '0', '0', '2', '1', '64', '11');

--
UPDATE `quest_template_addon` SET `NextQuestID` = '35933' WHERE `quest_template_addon`.`ID` in (34398, 36881);
UPDATE `quest_template` SET `RewardNextQuest` = '35933' WHERE `ID` in (34398, 36881);
UPDATE `quest_template` SET `RewardNextQuest` = '34392' WHERE `ID` = 35933;


REPLACE INTO `quest_template_addon` (`ID`, `PrevQuestID`) VALUES 
('35933', '34398'),
('34392', '35933'),
('34393', '34392'),
('34420', '34393');

INSERT INTO `area_queststart` (`id`, `quest`) VALUES ('7037', '34392');
DELETE FROM `creature_questrelation` WHERE `creature_questrelation`.`id` = 78558 AND `creature_questrelation`.`quest` = 34392;


-- Q34392
UPDATE `creature_template` SET `InhabitType` = '4', `AIName`='SmartAI' WHERE `creature_template`.`entry` in (81695, 81696);
REPLACE INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
('81695', '0', '0', '0', '25', '0', '100', '0', '0', '0', '0', '0', '11', '166539', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', 'Тюрьма Скверны на себя.'),
('81696', '0', '0', '0', '25', '0', '100', '0', '0', '0', '0', '0', '11', '166539', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', 'Тюрьма Скверны на себя.');

REPLACE INTO `areatrigger_data` (`entry`, `sphereScale`, `sphereScaleMax`, `isMoving`, `moveType`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `customVisualId`, `customEntry`, `hitType`, `Height`, `RadiusTarget`, `Float5`, `Float4`, `Radius`, `HeightTarget`, `MoveCurveID`, `ElapsedTime`, `comment`) VALUES 
('2757', '5', '5', '0', '0', '0', '0', '0', '0', '41185', '7371', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Тюрьма Скверны Q34392');

REPLACE INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastalent`, `hastalent2`, `chance`, `cooldown`, `type2`, `hitmask`, `learnspell`, `removeMask`, `comment`) VALUES 
('166392', '166407', '1', '3', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Q34392: Free Cho\'gall Credit'),
('166393', '166408', '1', '3', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Q34392: Free Teron\'gor Credit');
