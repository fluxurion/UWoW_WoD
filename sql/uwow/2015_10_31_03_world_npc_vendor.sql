ALTER TABLE `npc_vendor`   
  ADD COLUMN `gold` INT(11) DEFAULT 0  NOT NULL AFTER `type`;
