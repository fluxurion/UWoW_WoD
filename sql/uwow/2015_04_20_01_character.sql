ALTER TABLE `character_achievement` ADD `date` INT(11) UNSIGNED NOT NULL AFTER `achievement`;
RENAME TABLE `scenario_criteria_progress` TO `achievement_scenario_criteria_progress`;