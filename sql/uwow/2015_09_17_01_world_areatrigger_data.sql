ALTER TABLE `areatrigger_data`   
  ADD COLUMN `hasAreaTriggerBox` INT(11) DEFAULT 0  NOT NULL AFTER `HasFaceMovementDir`,
  ADD COLUMN `RollPitchYaw1X` FLOAT DEFAULT 0  NOT NULL AFTER `hasAreaTriggerBox`,
  ADD COLUMN `RollPitchYaw1Y` FLOAT DEFAULT 0  NOT NULL AFTER `RollPitchYaw1X`,
  ADD COLUMN `RollPitchYaw1Z` FLOAT DEFAULT 0  NOT NULL AFTER `RollPitchYaw1Y`,
  ADD COLUMN `TargetRollPitchYawX` FLOAT DEFAULT 0  NOT NULL AFTER `RollPitchYaw1Z`,
  ADD COLUMN `TargetRollPitchYawY` FLOAT DEFAULT 0  NOT NULL AFTER `TargetRollPitchYawX`,
  ADD COLUMN `TargetRollPitchYawZ` FLOAT DEFAULT 0  NOT NULL AFTER `TargetRollPitchYawY`;

ALTER TABLE `areatrigger_data`   
  DROP PRIMARY KEY,
  ADD PRIMARY KEY (`entry`, `spellId`);
