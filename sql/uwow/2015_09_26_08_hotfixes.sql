--
-- Table structure for table `item_x_bonus_tree`
--

DROP TABLE IF EXISTS `item_x_bonus_tree`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_x_bonus_tree` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusTreeID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `item_bonus_tree_node`
--

DROP TABLE IF EXISTS `item_bonus_tree_node`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_bonus_tree_node` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusTreeID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusTreeModID` int(10) unsigned NOT NULL DEFAULT '0',
  `SubTreeID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusListID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
