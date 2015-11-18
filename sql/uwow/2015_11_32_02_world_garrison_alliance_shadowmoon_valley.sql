REPLACE INTO `quest_template_addon` (`ID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`) VALUES 
('33815', '33868', '0', '0');


-- 4352 мир
-- 3329 - фаза интро перед квестом.
-- 3639 - 34582 - после взятие, прокнул на смене арреи.
-- 3353 - while not complete 34582. Не увидел ничего подозрительного.
-- 3695 - at SMSG_QUEST_UPDATE_ADD_CREDIT 34583 id 84778 remove 79242 для ивента порой прячет кадгара 79242. у нас пусть будет всегда.

REPLACE INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `PreloadMapID`, `VisibleMapID`, `flags`, `comment`) VALUES
-- 2877 2884 2988 3054 3055 3122 3184 3238 3244 3253 3329 3353 3420 3434 3695 3926 3934 4086 4318 4352
(6719, 1, 0, '4352', 0, 0, 16, 'Draenor. ShadowMoon.Valley. фаза зоны'),
-- 2877 2884 2988 3054 3055 3122 3184 3238 3244 3253 3353 3420 3434 3695 3926 3934 4086 4318 4352               while not take 34582
(6719, 2, 0, '3329', 0, 0, 16, 'Draenor. ShadowMoon.Valley. Пока не взят квест  34582'), 
-- 2877 2884 2988 3054 3055 3122 3184 3238 3244 3253 3353 3420 3434 3639 3695 3926 3934 4086 4318 4352          while take 34582
(6719, 3, 0, '3639', 0, 0, 16, 'Draenor. ShadowMoon.Valley.  появляется когда выполняем 34582 квест.'),
-- 2877 2884 2988 3054 3055 3122 3184 3238 3244 3253 3420 3434 3639 3695 3926 3934 4086 4318 4352               while not Complete 34582
(6719, 4, 0, '3353', 0, 0, 16, 'Draenor. ShadowMoon.Valley. while not complete or reward.'),
-- 2877 2884 2988 3054 3055 3122 3184 3238 3244 3253 3420 3434 3639 3926 3934 4086 4318 4352                    at SMSG_QUEST_UPDATE_ADD_CREDIT 34583 id 84778
(6719, 5, 0, '3695', 0, 0, 16, 'Draenor. ShadowMoon.Valley. кадгар'),


(6719, 100, 0, '2877 2884 2988 3054 3055 3122 3184 3238 3244 3253 3420 3434 3926 3934 4086 4318', 0, 0, 16, 'Draenor. ShadowMoon.Valley. general.');


DELETE FROM `conditions` WHERE SourceTypeOrReferenceId = 23 AND SourceGroup = 6719;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(23, 6719, 2, 0, 0, 14, 0, 34582, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 34582'),
(23, 6719, 3, 0, 0, 14, 0, 34582, 0, 0, 1, 0, '', 'Draenor. ShadowMoon.Valley. 34582'),
(23, 6719, 4, 0, 0, 28, 0, 34582, 0, 0, 1, 0, '', 'Draenor. ShadowMoon.Valley. 34582'),
(23, 6719, 4, 0, 0, 8, 0, 34582, 0, 0, 1, 0, '', 'Draenor. ShadowMoon.Valley. 34582');


-- Q34583 spell 160405 npc 82125
DELETE FROM `creature_text` WHERE entry = 82125;
REPLACE INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(82125, 0, 0, 'Я могу открыть портал в Штормград, но лишь ненадолго.', 12, 0, 100, 0, 0, 44990, 'Верховный маг Кадгар to Player');
