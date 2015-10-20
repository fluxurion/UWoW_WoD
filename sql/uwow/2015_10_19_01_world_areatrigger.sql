ALTER TABLE `areatrigger_polygon`   
  ADD COLUMN `type` TINYINT(3) DEFAULT 1  NOT NULL AFTER `spellId`, 
  DROP PRIMARY KEY,
  ADD PRIMARY KEY (`entry`, `spellId`, `type`, `id`);

ALTER TABLE `areatrigger_data`   
  CHANGE `speed` `speed` FLOAT DEFAULT 0  NOT NULL;
