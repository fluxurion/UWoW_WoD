ALTER TABLE `creature` ADD `PhaseId` VARCHAR(255) NOT NULL DEFAULT '' AFTER `phaseMask`;
ALTER TABLE `gameobject` ADD `PhaseId` VARCHAR(255) NOT NULL DEFAULT '' AFTER `phaseMask`;

ALTER TABLE `phase_definitions` CHANGE `wmAreaId` `VisibleMapID` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `phase_definitions` CHANGE `terrainswapmap` `PreloadMapID` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `phase_definitions` CHANGE `phaseId` `phaseId` VARCHAR(128) NOT NULL DEFAULT '';
UPDATE `phase_definitions` set phaseid = '';