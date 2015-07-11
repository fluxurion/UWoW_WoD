ALTER TABLE `spell_scene` DROP `transport`;
ALTER TABLE `spell_scene` ADD `trigerSpell` INT(11) UNSIGNED NOT NULL DEFAULT '0' AFTER `MiscValue`;
ALTER TABLE `spell_scene` ADD `MonsterCredit` INT(11) UNSIGNED NOT NULL DEFAULT '0' AFTER `trigerSpell`;