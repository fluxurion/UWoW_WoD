ALTER TABLE `creature_template`   
  ADD COLUMN `AiID` INT(10) DEFAULT 0  NOT NULL AFTER `WorldEffectID`,
  ADD COLUMN `MovementIDKit` INT(10) DEFAULT 0  NOT NULL AFTER `AiID`,
  ADD COLUMN `MeleeID` INT(10) DEFAULT 0  NOT NULL AFTER `MovementIDKit`;

UPDATE creature SET spawnMask = 1 WHERE map IN (0);
UPDATE gameobject SET spawnMask = 1 WHERE map IN (0);
