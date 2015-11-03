DROP TABLE IF EXISTS `garr_ability_effect`;
DROP TABLE IF EXISTS `garrison_ability_effect`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_ability_effect` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrAbilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk1` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrMechanicTypeID` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk3` int(10) unsigned NOT NULL DEFAULT '0',
  `Amount` float NOT NULL DEFAULT '0',
  `Amount2` float NOT NULL DEFAULT '0',
  `Amount3` float NOT NULL DEFAULT '0',
  `Unk4` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk5` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `garr_follower_level_xp`;
CREATE TABLE `garr_follower_level_xp`
( `ID` INT(10) UNSIGNED NOT NULL DEFAULT 0,
`Level` INT(10) UNSIGNED NOT NULL DEFAULT 0,
`NextLevelXP` INT(10) UNSIGNED NOT NULL DEFAULT 0,
`Unk` INT(10) UNSIGNED NOT NULL DEFAULT 0,
PRIMARY KEY (`ID`) )
ENGINE=MYISAM CHARSET=utf8 COLLATE=utf8_general_ci;

DROP TABLE IF EXISTS `garr_follower_quality`;
CREATE TABLE `garr_follower_quality`
( `ID` INT(10) UNSIGNED NOT NULL DEFAULT 0,
`Quality` INT(10) UNSIGNED NOT NULL DEFAULT 0, 
`NextQualityXP` INT(10) UNSIGNED NOT NULL DEFAULT 0, 
`Unk` INT(10) UNSIGNED NOT NULL DEFAULT 0, 
PRIMARY KEY (`ID`) ) 
ENGINE=MYISAM CHARSET=utf8 COLLATE=utf8_general_ci;

DROP TABLE IF EXISTS `garrison_encounter`;
DROP TABLE IF EXISTS `garr_encounter`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_encounter` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `CreatureEntry` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Mod` float NOT NULL DEFAULT '0',
  `Mod1` float NOT NULL DEFAULT '0',
  `SomeFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `garrison_encounter_x_mechanic`;
DROP TABLE IF EXISTS `garr_encounter_x_mechanic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_encounter_x_mechanic` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrEncounterID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrMechanicID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `garrison_mechanic`;
DROP TABLE IF EXISTS `garr_mechanic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mechanic` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `Mod` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `garrison_mechanic_type`;
DROP TABLE IF EXISTS `garr_mechanic_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mechanic_type` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Category` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Description` text,
  `SomeFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `garrison_mission`;
DROP TABLE IF EXISTS `garr_mission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mission` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ReqLevel` int(10) unsigned NOT NULL DEFAULT '0',
  `ReqFollowersItemLevel` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrMechanicTypeID` int(10) unsigned NOT NULL DEFAULT '0',
  `ReqFollowersCount` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk1` int(10) unsigned NOT NULL DEFAULT '0',
  `MissionDuration` int(10) unsigned NOT NULL DEFAULT '0',
  `OfferDuration` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk2` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrMissionTypeID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Description` text,
  `Location` text,
  `CriteriaID` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk3` int(10) unsigned NOT NULL DEFAULT '0',
  `ReqResourcesCount` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk4` int(10) unsigned NOT NULL DEFAULT '0',
  `BaseXP` int(10) unsigned NOT NULL DEFAULT '0',
  `BaseChance` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `garrison_mission_reward`;
DROP TABLE IF EXISTS `garr_mission_reward`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mission_reward` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MissionID` int(10) unsigned NOT NULL DEFAULT '0',
  `RewardXP` int(10) unsigned NOT NULL DEFAULT '0',
  `RewardItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemAmount` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrencyID` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrencyValue` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk2` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk3` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk4` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `garrison_mission_x_encounter`;
DROP TABLE IF EXISTS `garr_mission_x_encounter`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mission_x_encounter` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrMissionID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrEncounterID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;