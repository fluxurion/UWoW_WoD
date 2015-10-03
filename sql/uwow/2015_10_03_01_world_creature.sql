ALTER TABLE `creature`
  DROP COLUMN `equipment_id`,
  DROP COLUMN `AiID`,
  DROP COLUMN `MovementID`,
  DROP COLUMN `MeleeID`;

ALTER TABLE `creature_template`
  DROP COLUMN `equipment_id`;

ALTER TABLE `creature_equip_template`   
  CHANGE `entry` `CreatureID` INT(11) UNSIGNED DEFAULT 0  NOT NULL,
  ADD COLUMN `ID` INT(11) DEFAULT 1  NOT NULL AFTER `CreatureID`,
  CHANGE `itemEntry1` `ItemID1` INT(11) UNSIGNED DEFAULT 0  NOT NULL,
  CHANGE `itemEntry2` `ItemID2` INT(11) UNSIGNED DEFAULT 0  NOT NULL,
  CHANGE `itemEntry3` `ItemID3` INT(11) UNSIGNED DEFAULT 0  NOT NULL;

ALTER TABLE `creature`
  ADD COLUMN `equipment_id` TINYINT(3) DEFAULT 0  NOT NULL AFTER `modelid`;

ALTER TABLE `creature_equip_template`
  DROP PRIMARY KEY,
  ADD PRIMARY KEY (`CreatureID`, `ID`);
