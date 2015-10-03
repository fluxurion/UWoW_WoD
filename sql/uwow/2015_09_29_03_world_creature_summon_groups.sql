CREATE TABLE `creature_summon_groups` (
  `summonerId` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `summonerType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `groupId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `count` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `actionType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `distance` float NOT NULL DEFAULT '0',
  `summonType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `summonTime` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


ALTER TABLE `creature_summon_groups`   
  CHANGE `summonerId` `summonerId` INT(10) UNSIGNED DEFAULT 0  NOT NULL,
  ADD COLUMN `id` INT(10) DEFAULT 0  NOT NULL AFTER `summonerId`;

ALTER TABLE `creature_summon_groups`   
  DROP PRIMARY KEY,
  ADD PRIMARY KEY (`summonerId`, `id`, `summonerType`, `groupId`);

