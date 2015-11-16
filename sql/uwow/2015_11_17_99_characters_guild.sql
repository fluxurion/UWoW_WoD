DROP TABLE IF EXISTS `guild_news_log`;
DROP TABLE IF EXISTS `guild_newslog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `guild_newslog` (
  `guildid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `LogGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `EventType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `PlayerGuid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Value` int(10) unsigned NOT NULL DEFAULT '0',
  `TimeStamp` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`,`LogGuid`),
  KEY `guildid_key` (`guildid`),
  KEY `Idx_PlayerGuid` (`PlayerGuid`),
  KEY `Idx_LogGuid` (`LogGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `guild`
DROP COLUMN `experience`, 
DROP COLUMN `todayExperience`;