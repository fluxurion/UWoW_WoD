--
-- Table structure for table `mount`
--

DROP TABLE IF EXISTS `mount`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mount` (
  `Id` int(10) unsigned NOT NULL DEFAULT '0',
  `MountTypeId` int(10) unsigned NOT NULL DEFAULT '0',
  `DisplayId` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Description` text,
  `SourceDescription` text,
  `Source` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `PlayerConditionId` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `pvp_item`
--

DROP TABLE IF EXISTS `pvp_item`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pvp_item` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusIlvl` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
