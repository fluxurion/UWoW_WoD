ALTER TABLE `spell_concatenate_aura`   
  ADD COLUMN `type` TINYINT(3) DEFAULT 0  NOT NULL AFTER `effectAura`;
