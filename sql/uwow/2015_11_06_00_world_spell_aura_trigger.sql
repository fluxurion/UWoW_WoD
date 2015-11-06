ALTER TABLE `spell_aura_trigger`   
  ADD COLUMN `slot` INT(3) DEFAULT 0  NOT NULL AFTER `chance`;
