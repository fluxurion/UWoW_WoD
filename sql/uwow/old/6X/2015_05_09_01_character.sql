ALTER TABLE `characters` ADD `power6` INT(11) UNSIGNED NOT NULL DEFAULT '0' AFTER `power5`;
ALTER TABLE `character_stats` ADD `maxpower6` INT(11) UNSIGNED NOT NULL DEFAULT '0' AFTER `maxpower5`;