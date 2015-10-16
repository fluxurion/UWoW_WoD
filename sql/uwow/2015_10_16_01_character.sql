
-- ----------------------------
-- Table structure for `account_battle_pet_journal`
-- ----------------------------
DROP TABLE IF EXISTS `account_battle_pet_journal`;
CREATE TABLE `account_battle_pet_journal` (
  `guid` bigint(20) NOT NULL,
  `ownerAccID` int(10) NOT NULL,
  `species` int(10) NOT NULL,
  `breed` smallint(5) NOT NULL,
  `level` smallint(5) NOT NULL DEFAULT '1',
  `exp` smallint(5) NOT NULL DEFAULT '0',
  `health` int(10) NOT NULL DEFAULT '1',
  `maxHealth` int(10) NOT NULL DEFAULT '1',
  `speed` int(10) NOT NULL DEFAULT '1',
  `power` int(10) NOT NULL DEFAULT '1',
  `collar` int(10) NOT NULL	DEFAULT '0',
  `quality` tinyint(3) NOT NULL DEFAULT '0',
  `flags` smallint(5) NOT NULL DEFAULT '0',
  `name` varchar(12) NOT NULL,
  `creatureID` int(10) NOT NULL DEFAULT '0',
  `spellID` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

