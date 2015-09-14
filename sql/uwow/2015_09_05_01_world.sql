ALTER TABLE `creature_template`   
  ADD COLUMN `personalloot` INT(11) UNSIGNED DEFAULT 0  NOT NULL AFTER `mechanic_immune_mask`;
ALTER TABLE `creature_template`   
  ADD COLUMN `VignetteId` INT(11) UNSIGNED DEFAULT 0  NOT NULL AFTER `personalloot`;

ALTER TABLE `locales_creature`   
  CHANGE `name_loc1` `name_loc1` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc2` `name_loc2` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc3` `name_loc3` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc4` `name_loc4` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc5` `name_loc5` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc6` `name_loc6` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc7` `name_loc7` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc8` `name_loc8` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc9` `name_loc9` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL,
  CHANGE `name_loc10` `name_loc10` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL,
  CHANGE `subname_loc1` `subname_loc1` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL,
  CHANGE `subname_loc2` `subname_loc2` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL,
  CHANGE `subname_loc3` `subname_loc3` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL,
  CHANGE `subname_loc4` `subname_loc4` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL,
  CHANGE `subname_loc5` `subname_loc5` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL,
  CHANGE `subname_loc6` `subname_loc6` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL,
  CHANGE `subname_loc7` `subname_loc7` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL,
  CHANGE `subname_loc8` `subname_loc8` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL,
  CHANGE `subname_loc9` `subname_loc9` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL,
  CHANGE `subname_loc10` `subname_loc10` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL;

ALTER TABLE `locales_gameobject`   
  CHANGE `name_loc1` `name_loc1` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc2` `name_loc2` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc3` `name_loc3` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc4` `name_loc4` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc5` `name_loc5` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc6` `name_loc6` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc7` `name_loc7` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc8` `name_loc8` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc9` `name_loc9` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `name_loc10` `name_loc10` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `castbarcaption_loc1` `castbarcaption_loc1` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `castbarcaption_loc2` `castbarcaption_loc2` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `castbarcaption_loc3` `castbarcaption_loc3` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `castbarcaption_loc4` `castbarcaption_loc4` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `castbarcaption_loc5` `castbarcaption_loc5` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `castbarcaption_loc6` `castbarcaption_loc6` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `castbarcaption_loc7` `castbarcaption_loc7` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `castbarcaption_loc8` `castbarcaption_loc8` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `castbarcaption_loc9` `castbarcaption_loc9` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `castbarcaption_loc10` `castbarcaption_loc10` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL;

ALTER TABLE `creature_template`   
  CHANGE `name` `name` CHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `femaleName` `femaleName` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NOT NULL,
  CHANGE `subname` `subname` CHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL,
  CHANGE `IconName` `IconName` CHAR(100) CHARSET utf8 COLLATE utf8_general_ci DEFAULT ''  NULL;

DROP TABLE quest_conditions;

ALTER TABLE `quest_template`   
  CHANGE `Float10` `RevardXPMultiplier` FLOAT DEFAULT 1  NOT NULL,
  CHANGE `Float13` `RewardMoneyMultiplier` FLOAT DEFAULT 1  NOT NULL;

ALTER TABLE `scenario_poi`   
  CHANGE `id` `BlobID` INT(10) UNSIGNED DEFAULT 0  NOT NULL,
  CHANGE `mapid` `MapID` INT(10) UNSIGNED DEFAULT 0  NOT NULL,
  CHANGE `WorldMapAreaId` `WorldMapAreaID` INT(10) UNSIGNED DEFAULT 0  NOT NULL,
  CHANGE `unk12` `Floor` INT(10) UNSIGNED DEFAULT 0  NOT NULL,
  CHANGE `unk16` `Priority` INT(10) UNSIGNED DEFAULT 0  NOT NULL,
  CHANGE `unk20` `Flags` INT(10) UNSIGNED DEFAULT 0  NOT NULL,
  CHANGE `unk24` `WorldEffectID` INT(10) UNSIGNED DEFAULT 0  NOT NULL,
  CHANGE `unk28` `PlayerConditionID` INT(10) UNSIGNED DEFAULT 0  NOT NULL;

ALTER TABLE `spell_visual_send`   
  CHANGE `spell_id` `spellId` INT(11) NOT NULL,
  CHANGE `visual` `SpellVisualID` INT(11) NOT NULL,
  CHANGE `speed` `TravelSpeed` FLOAT DEFAULT 0  NOT NULL  AFTER `SpellVisualID`,
  CHANGE `unk1` `MissReason` INT(8) DEFAULT 0  NOT NULL,
  CHANGE `unk2` `ReflectStatus` INT(8) DEFAULT 0  NOT NULL,
  CHANGE `position` `SpeedAsTime` INT(3) DEFAULT 0  NOT NULL;

ALTER TABLE `spell_scene`   
  DROP COLUMN `hasO`, 
  DROP COLUMN `bit16`, 
  DROP COLUMN `x`, 
  DROP COLUMN `y`, 
  DROP COLUMN `z`, 
  DROP COLUMN `o`, 
  CHANGE `ScenePackageId` `SceneScriptPackageID` INT(11) DEFAULT 0  NOT NULL;
