-- ALTER TABLE `account_battle_pet`
-- DROP COLUMN `slot_2`,
-- CHANGE COLUMN `slot_0` `battlePetGuid`  bigint(20) UNSIGNED NOT NULL DEFAULT 0 AFTER `ownerAccID`,
-- CHANGE COLUMN `slot_1` `locked`  tinyint(3) UNSIGNED NOT NULL DEFAULT 0 AFTER `battlePetGuid`,
-- ADD COLUMN `id`  tinyint(3) UNSIGNED NOT NULL DEFAULT 0 FIRST ,
-- DROP PRIMARY KEY,
-- ADD PRIMARY KEY (`tinyint`, `ownerAccID`);

-- ----------------------------
-- Table structure for `account_battle_pet_slots`
-- ----------------------------

DROP TABLE IF EXISTS `account_battle_pet_slots`;
CREATE TABLE `account_battle_pet_slots` (
  `id` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ownerAccID` int(11) unsigned NOT NULL DEFAULT '0',
  `battlePetGuid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `locked` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`ownerAccID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
