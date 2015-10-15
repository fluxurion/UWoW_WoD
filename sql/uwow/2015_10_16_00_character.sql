ALTER TABLE `character_loot_cooldown`
MODIFY COLUMN `entry`  int(11) NOT NULL DEFAULT 0 AFTER `guid`,
ADD COLUMN `difficultyMask`  tinyint(3) NOT NULL DEFAULT 0 AFTER `type`;
