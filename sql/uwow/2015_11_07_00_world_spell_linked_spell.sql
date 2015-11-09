ALTER TABLE `spell_linked_spell`   
  ADD COLUMN `targetCountType` TINYINT(3) DEFAULT 0  NOT NULL AFTER `removeMask`,
  ADD COLUMN `targetCount` TINYINT(3) DEFAULT -1  NOT NULL AFTER `targetCountType`,
  CHANGE `actiontype` `actiontype` TINYINT(3) DEFAULT 0  NOT NULL;
ALTER TABLE `spell_linked_spell`   
  ADD COLUMN `group` TINYINT(3) DEFAULT 0  NOT NULL AFTER `actiontype`;
