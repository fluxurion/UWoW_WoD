ALTER TABLE `spell_aura_trigger`   
  ADD COLUMN `slot` INT(3) DEFAULT 0  NOT NULL AFTER `chance`;
ALTER TABLE `spell_aura_trigger`   
  CHANGE `slot` `slot` INT(11) DEFAULT 0  NOT NULL;
