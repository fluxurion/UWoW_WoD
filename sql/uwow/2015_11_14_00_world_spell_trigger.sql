ALTER TABLE `spell_trigger`   
  ADD COLUMN `targetaura2` TINYINT(3) DEFAULT 0  NOT NULL AFTER `targetaura`,
  CHANGE `aura` `aura` INT(11) DEFAULT 0  NOT NULL,
  ADD COLUMN `aura2` INT(11) DEFAULT 0  NOT NULL AFTER `aura`;
