ALTER TABLE `creature_template` CHANGE `faction_A` `faction` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '35';
ALTER TABLE `creature_template` DROP `faction_H`;
ALTER TABLE `creature_template` CHANGE `Health_mod` `HealthModifier` FLOAT NOT NULL DEFAULT '1';
ALTER TABLE `creature_template` CHANGE `Mana_mod` `ManaModifier` FLOAT NOT NULL DEFAULT '1';
ALTER TABLE `creature_template` CHANGE `WDBVerified` `VerifiedBuild` SMALLINT(6) NULL DEFAULT '1';
ALTER TABLE `creature_difficulty_stat` CHANGE `Health_mod` `HealthModifier` INT UNSIGNED NOT NULL DEFAULT '1';
ALTER TABLE `creature_template` ADD `unit_flags3` INT(10) NOT NULL DEFAULT '0' AFTER `unit_flags2`;
ALTER TABLE `gameobject_template` CHANGE  `WDBVerified`  `VerifiedBuild` SMALLINT( 6 ) NULL DEFAULT  '1'
ALTER TABLE `creature_template` ADD `femaleName` VARCHAR(100) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `name`;