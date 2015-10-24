
--
-- Table structure for table `garr_ability_effect`
--

DROP TABLE IF EXISTS `garr_ability_effect`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_ability_effect` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `garrAbilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_ability_effect`
--

LOCK TABLES `garr_ability_effect` WRITE;
/*!40000 ALTER TABLE `garr_ability_effect` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_ability_effect` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_mechanic`
--

DROP TABLE IF EXISTS `garr_mechanic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mechanic` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `mechanicType` int(10) unsigned NOT NULL DEFAULT '0',
  `mod` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_mechanic`
--

LOCK TABLES `garr_mechanic` WRITE;
/*!40000 ALTER TABLE `garr_mechanic` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_mechanic` ENABLE KEYS */;
UNLOCK TABLES;


--
-- Table structure for table `garr_mission`
--

DROP TABLE IF EXISTS `garr_mission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mission` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `requiredLevel` int(10) unsigned NOT NULL DEFAULT '0',
  `requiredItemLevel` int(10) unsigned NOT NULL DEFAULT '0',
  `mechanicID` int(10) unsigned NOT NULL DEFAULT '0',
  `reqFollowersCount` int(10) unsigned NOT NULL DEFAULT '0',
  `unk1` int(10) unsigned NOT NULL DEFAULT '0',
  `missionTime` int(10) unsigned NOT NULL DEFAULT '0',
  `vaildTime` int(10) unsigned NOT NULL DEFAULT '0',
  `unk2` int(10) unsigned NOT NULL DEFAULT '0',
  `category` int(10) unsigned NOT NULL DEFAULT '0',
  `name` text,
  `description` text,
  `location` text,
  `criteriaID` int(10) unsigned NOT NULL DEFAULT '0',
  `missionType` int(10) unsigned NOT NULL DEFAULT '0',
  `reqResourcesCount` int(10) unsigned NOT NULL DEFAULT '0',
  `unk4` int(10) unsigned NOT NULL DEFAULT '0',
  `bonusXP` int(10) unsigned NOT NULL DEFAULT '0',
  `baseChance` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_mission`
--

LOCK TABLES `garr_mission` WRITE;
/*!40000 ALTER TABLE `garr_mission` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_mission` ENABLE KEYS */;
UNLOCK TABLES;


--
-- Table structure for table `garr_mission_locale`
--

DROP TABLE IF EXISTS `garr_mission_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mission_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `name_lang` text,
  `description_lang` text,
  `location_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_mission_locale`
--

LOCK TABLES `garr_mission_locale` WRITE;
/*!40000 ALTER TABLE `garr_mission_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_mission_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_mission_reward`
--

DROP TABLE IF EXISTS `garr_mission_reward`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mission_reward` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `missionID` int(10) unsigned NOT NULL DEFAULT '0',
  `rewardXP` int(10) unsigned NOT NULL DEFAULT '0',
  `rewardItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `itemAmount` int(10) unsigned NOT NULL DEFAULT '0',
  `currencyID` int(10) unsigned NOT NULL DEFAULT '0',
  `currencyValue` int(10) unsigned NOT NULL DEFAULT '0',
  `unk2` int(10) unsigned NOT NULL DEFAULT '0',
  `unk3` int(10) unsigned NOT NULL DEFAULT '0',
  `unk4` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_mission_reward`
--

LOCK TABLES `garr_mission_reward` WRITE;
/*!40000 ALTER TABLE `garr_mission_reward` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_mission_reward` ENABLE KEYS */;
UNLOCK TABLES;

-- Table structure for table `garr_mission_x_encounter`
--

DROP TABLE IF EXISTS `garr_mission_x_encounter`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mission_x_encounter` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `missionID` int(10) unsigned NOT NULL DEFAULT '0',
  `encounterID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_mission_x_encounter`
--

LOCK TABLES `garr_mission_x_encounter` WRITE;
/*!40000 ALTER TABLE `garr_mission_x_encounter` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_mission_x_encounter` ENABLE KEYS */;
UNLOCK TABLES;
