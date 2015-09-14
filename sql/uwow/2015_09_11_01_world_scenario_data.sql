CREATE TABLE `scenario_data`(  
  `ScenarioID` INT(11) UNSIGNED NOT NULL DEFAULT 0,
  `mapId` INT(11) UNSIGNED NOT NULL DEFAULT 0,
  `DifficultyID` INT(11) NOT NULL DEFAULT 0,
  `WaveCurrent` INT(11) NOT NULL DEFAULT 0,
  `WaveMax` INT(11) NOT NULL DEFAULT 0,
  `TimerDuration` INT(11) NOT NULL DEFAULT 0,
  `CriteriaProgressCount` INT(11) NOT NULL DEFAULT 0,
  `BonusObjectiveDataCount` INT(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`ScenarioID`, `mapId`)
);
