REPLACE INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `terrainswapmap`, `wmAreaId`, `flags`, `comment`) VALUES
(4, 1, 0, 3971, 1190, 992, 0, 'DARK_PORTAL_WOD'),   -- phase aura 176111
(4, 2, 0, 3970, 1066, 910, 0, 'Dark Portal. ALways ON.'),
(4, 3, 0, 0, 736, 770, 0, 'Dark Portal. ALways ON.');

DELETE FROM `conditions` WHERE SourceTypeOrReferenceId = 23 AND SourceGroup = 4;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(23, 4, 1, 0, 0, 1, 0, 176111, 0, 0, 1, 0, '', 'DARK_PORTAL_WOD no time back aura'),
(23, 4, 1, 0, 0, 27, 0, 89, 1, 0, 0, 0, '', 'DARK_PORTAL_WOD higher then 89'),
(23, 4, 2, 0, 0, 1, 0, 176111, 0, 0, 1, 0, '', 'DARK_PORTAL_WOD no time back aura'),
(23, 4, 3, 0, 0, 1, 0, 176111, 0, 0, 1, 0, '', 'DARK_PORTAL_WOD no time back aura');

REPLACE INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES ('4354', 'at_wod_dark_portal');

REPLACE INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
('173143', '1116', '5352.16', '-3944.96', '32.7331', '3.921144'),
('167220', '1116', '3735', '-4043', '45', '3.104'),
('167221', '1116', '2308.57', '447.469', '5.11977', '2.199202'),
('167220', '1116', '5535.01', '5019.88', '12.6375', '5.174203');