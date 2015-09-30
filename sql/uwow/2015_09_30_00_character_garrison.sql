-- ----------------------------
-- Table structure for `character_garrison_missions`
-- ----------------------------
DROP TABLE IF EXISTS `character_garrison_missions`;
CREATE TABLE `character_garrison_missions` (
  `dbId` bigint(20) unsigned NOT NULL,
  `guid` bigint(20) unsigned NOT NULL,
  `missionRecID` int(10) unsigned NOT NULL DEFAULT '0',
  `offerTime` int(10) unsigned NOT NULL DEFAULT '0',
  `offerDuration` int(10) unsigned NOT NULL DEFAULT '0',
  `startTime` int(10) unsigned NOT NULL DEFAULT '0',
  `travelDuration` int(10) unsigned NOT NULL DEFAULT '0',
  `missionDuration` int(10) unsigned NOT NULL DEFAULT '0',
  `missionState` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`dbId`),
  UNIQUE KEY `idx_guid_id` (`guid`,`missionRecID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;