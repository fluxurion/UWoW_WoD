--
-- Table structure for table `item_to_battle_pet_species`
--

DROP TABLE IF EXISTS `item_to_battle_pet_species`;
CREATE TABLE `item_to_battle_pet_species` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BattlePetSpeciesID` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
