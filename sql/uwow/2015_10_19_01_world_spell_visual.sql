RENAME TABLE `spell_visual_send` TO `spell_visual`;

CREATE TABLE `spell_visual_play_orphan` (
  `spellId` INT(11) NOT NULL,
  `SpellVisualID` INT(11) NOT NULL,
  `TravelSpeed` FLOAT NOT NULL DEFAULT '0',
  `SpeedAsTime` INT(3) NOT NULL DEFAULT '0',
  `UnkFloat` FLOAT NOT NULL DEFAULT '0',
  `type` INT(3) NOT NULL DEFAULT '0',
  `temp` INT(11) NOT NULL DEFAULT '0',
  `comment` TEXT,
  PRIMARY KEY (`spellId`,`SpellVisualID`)
) ENGINE=MYISAM DEFAULT CHARSET=utf8;
