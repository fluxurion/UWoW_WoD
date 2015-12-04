CREATE TABLE `spell_check_cast`(  
  `spellId` INT(11) NOT NULL DEFAULT 0,
  `type` INT(3) NOT NULL DEFAULT 0,
  `param1` INT(11) NOT NULL DEFAULT 0,
  `param2` INT(11) NOT NULL DEFAULT 0,
  `param3` INT(11) NOT NULL DEFAULT 0,
  `errorId` INT(11) NOT NULL DEFAULT 13,
  `customErrorId` INT(11) NOT NULL DEFAULT 0,
  `caster` INT(3) NOT NULL DEFAULT 0,
  `target` INT(3) NOT NULL DEFAULT 0,
  `checkType` INT(3) NOT NULL DEFAULT 0,
  `dataType` INT(11) NOT NULL DEFAULT 0,
  `checkType2` INT(3) NOT NULL DEFAULT 0,
  `dataType2` INT(11) NOT NULL DEFAULT 0,
  `comment` VARCHAR(250) NOT NULL,
  PRIMARY KEY (`spellId`, `type`)
) ENGINE=MYISAM CHARSET=utf8 COLLATE=utf8_general_ci;

