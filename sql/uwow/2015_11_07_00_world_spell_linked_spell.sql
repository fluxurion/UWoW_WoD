ALTER TABLE `spell_linked_spell`   
  ADD COLUMN `targetCountType` TINYINT(3) DEFAULT 0  NOT NULL AFTER `removeMask`,
  ADD COLUMN `targetCount` TINYINT(3) DEFAULT -1  NOT NULL AFTER `targetCountType`,
  CHANGE `actiontype` `actiontype` TINYINT(3) DEFAULT 0  NOT NULL;
ALTER TABLE `spell_linked_spell`   
  ADD COLUMN `group` TINYINT(3) DEFAULT 0  NOT NULL AFTER `actiontype`;
ALTER TABLE `spell_linked_spell`
  ADD COLUMN `duration` INT(11) DEFAULT 0  NOT NULL AFTER `cooldown`;
ALTER TABLE `spell_linked_spell`
  DROP INDEX `trigger_effect_type`,
  ADD  UNIQUE INDEX `trigger_effect_type` (`spell_trigger`, `spell_effect`, `type`, `hastalent`, `actiontype`);
