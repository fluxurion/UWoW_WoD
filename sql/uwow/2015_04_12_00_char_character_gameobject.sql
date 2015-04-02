DROP TABLE IF EXISTS `character_gameobject`;
CREATE TABLE `character_gameobject` (
  `guid` BIGINT(20) unsigned NOT NULL,
  `entry` int(11) NOT NULL DEFAULT '0',
  `respawnTime` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`entry`),
  KEY `entry` (`entry`),
  KEY `respawnTime` (`respawnTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8
