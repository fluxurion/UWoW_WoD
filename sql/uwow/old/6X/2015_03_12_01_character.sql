ALTER TABLE `item_instance`
  DROP `reforgeId`,
  DROP `transmogrifyId`,
  DROP `upgradeId`,
  ADD `transmogrification` int(10) unsigned not null default '0' AFTER `text`,
  ADD `upgradeId` int(10) unsigned not null default '0' AFTER `transmogrification`,
  ADD `enchantIllusion` int(10) unsigned not null default '0' AFTER `upgradeId`,
  ADD `bonusListIDs` text AFTER `enchantIllusion`;