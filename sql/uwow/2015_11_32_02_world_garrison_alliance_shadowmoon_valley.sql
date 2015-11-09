INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `PreloadMapID`, `VisibleMapID`, `flags`, `comment`) VALUES
(6719, 1, 0, '4352', 0, 0, 16, 'Draenor. ShadowMoon.Valley. фаза зоны'),
(6719, 2, 0, '3329', 0, 0, 16, 'Draenor. ShadowMoon.Valley. Пока не взят квест  34582'),
(6719, 3, 0, '3639', 0, 0, 16, 'Draenor. ShadowMoon.Valley.  появляется когда выполняем 34582 квест.'),
(6719, 4, 0, '3639 2877 2884 2988 3054 3055 3122 3184 3238 3244 3253 3353 3420 3434 3695 3926 3934 4086 4318', 0, 0, 16, 'Draenor. ShadowMoon.Valley. еще не знаем.'),
(6719, 5, 0, '4352 4086 3934 3926 3695 3562 3532 3525 3524 3523 3511 3510 3435 3290 3253 3244 3238 3184 3122 3055 3054 2988 2884 2877 2559', 0, 0, 16, 'Draenor. ShadowMoon.Valley.'),
(6719, 6, 0, '4352 4086 3934 3926 3695 3435 3290 3253 3244 3238 3184 3122 3055 3054 2988 2884 2877 2559 2554', 0, 0, 16, 'Draenor. ShadowMoon.Valley.'),
(6719, 7, 0, '4352 4086 3934 3926 3695 3681 3434 3420 3415 3407 3290 3253 3244 3238 3184 3122 3055 3054 2988 2884 2877 2559', 0, 0, 16, 'Draenor. ShadowMoon.Valley.');

DELETE FROM `conditions` WHERE SourceTypeOrReferenceId = 23 AND SourceGroup = 6719;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(23, 6719, 2, 0, 0, 9, 0, 34582, 0, 0, 1, 0, '', 'Draenor. ShadowMoon.Valley. 34582'),
(23, 6719, 3, 0, 0, 9, 0, 34582, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 34582'),
(23, 6719, 4, 0, 0, 28, 0, 34582, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 34582'),
(23, 6719, 4, 0, 1, 8, 0, 34582, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 34582'),
(23, 6719, 5, 0, 0, 8, 0, 33794, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 33794'),
(23, 6719, 5, 0, 1, 9, 0, 33794, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 33794'),
(23, 6719, 5, 0, 2, 8, 0, 33083, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 33083'),
(23, 6719, 5, 0, 3, 9, 0, 33083, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 33083'),
(23, 6719, 5, 0, 4, 8, 0, 33793, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 33793'),
(23, 6719, 5, 0, 5, 9, 0, 33793, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 33793'),
(23, 6719, 5, 0, 6, 8, 0, 35032, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 35032'),
(23, 6719, 5, 0, 7, 9, 0, 35032, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 35032'),
(23, 6719, 6, 0, 0, 8, 0, 34054, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 34054'),
(23, 6719, 6, 0, 1, 9, 0, 34054, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 34054'),
(23, 6719, 6, 0, 2, 28, 0, 34054, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 34054'),
(23, 6719, 7, 0, 0, 8, 0, 33075, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 33075'),
(23, 6719, 7, 0, 1, 9, 0, 33075, 0, 0, 0, 0, '', 'Draenor. ShadowMoon.Valley. 33075');