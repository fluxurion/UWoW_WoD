--
-- Table structure for table `battlenet_account_toys`
--

DROP TABLE IF EXISTS `battlenet_account_toys`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `battlenet_account_toys` (
  `accountId` int(10) unsigned NOT NULL,
  `itemId` int(11) NOT NULL DEFAULT '0',
  `isFavourite` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`accountId`,`itemId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battlenet_account_toys`
--

LOCK TABLES `battlenet_account_toys` WRITE;
/*!40000 ALTER TABLE `battlenet_account_toys` DISABLE KEYS */;
/*!40000 ALTER TABLE `battlenet_account_toys` ENABLE KEYS */;
UNLOCK TABLES;
