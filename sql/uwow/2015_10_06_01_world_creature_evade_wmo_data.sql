CREATE TABLE `creature_evade_wmo_data`(  
  `entry` INT(11) NOT NULL,
  `distance` INT(11) NOT NULL DEFAULT 0,
  `wmoId` INT(11) NOT NULL DEFAULT 0,
  `wmoSet` INT(11) NOT NULL DEFAULT -1,
  `wmoGroupId` INT(11) NOT NULL DEFAULT 0,
  `comment` TEXT NOT NULL,
  ADD PRIMARY KEY (`entry`)
) ENGINE=MYISAM CHARSET=utf8 COLLATE=utf8_general_ci;
