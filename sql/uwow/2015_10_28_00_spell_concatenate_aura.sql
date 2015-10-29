CREATE TABLE `spell_concatenate_aura`(  
  `spellid` INT(11) NOT NULL DEFAULT 0,
  `effectSpell` TINYINT(11) NOT NULL DEFAULT 0,
  `auraId` INT(11) NOT NULL DEFAULT 0,
  `effectAura` TINYINT(11) NOT NULL DEFAULT 0,
  `comment` VARCHAR(100) NOT NULL DEFAULT '',
  PRIMARY KEY (`spellid`, `effectSpell`, `auraId`, `effectAura`)
);
