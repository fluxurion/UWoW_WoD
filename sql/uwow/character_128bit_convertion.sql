TRUNCATE `character_aura`;
TRUNCATE `pet_aura`;
TRUNCATE `group_instance`;
TRUNCATE `group_member`;
TRUNCATE `groups`;
--
ALTER TABLE `item_instance` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `item_instance` CHANGE `owner_guid` `owner_guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `item_instance` CHANGE `creatorGuid` `creatorGuid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `item_instance` CHANGE `giftCreatorGuid` `giftCreatorGuid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `item_loot_items` CHANGE `container_id` `container_id` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `item_loot_money` CHANGE `container_id` `container_id` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `item_refund_instance` CHANGE `item_guid` `item_guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `item_refund_instance` CHANGE `player_guid` `player_guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `item_soulbound_trade_data` CHANGE `itemGuid` `itemGuid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `lag_reports` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `mail` CHANGE `sender` `sender` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `mail` CHANGE `receiver` `receiver` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `mail_items` CHANGE `item_guid` `item_guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `mail_items` CHANGE `receiver` `receiver` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `petition` CHANGE `ownerguid` `ownerguid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `petition` CHANGE `petitionguid` `petitionguid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `petition_sign` CHANGE `ownerguid` `ownerguid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `petition_sign` CHANGE `petitionguid` `petitionguid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `petition_sign` CHANGE `playerguid` `playerguid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `pvpstats_players` CHANGE `character_guid` `character_guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_tracker` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `calendar_events` CHANGE `creator` `creator` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `calendar_invites` CHANGE `invitee` `invitee` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `calendar_invites` CHANGE `sender` `sender` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_pet` CHANGE `owner` `owner` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_respawn` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_respawn` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';

--
ALTER TABLE `character_aura` CHANGE `caster_guid` `caster_guid` binary(16) NOT NULL COMMENT 'Full Global Unique Identifier';
ALTER TABLE `character_aura` CHANGE `item_guid` `item_guid` binary(16) NOT NULL;
ALTER TABLE `character_aura` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier';
ALTER TABLE `pet_aura` CHANGE `caster_guid` `caster_guid` binary(16) NOT NULL COMMENT 'Full Global Unique Identifier';
ALTER TABLE `groups` CHANGE `icon1` `icon1` binary(16) NOT NULL;
ALTER TABLE `groups` CHANGE `icon2` `icon2` binary(16) NOT NULL;
ALTER TABLE `groups` CHANGE `icon3` `icon3` binary(16) NOT NULL;
ALTER TABLE `groups` CHANGE `icon4` `icon4` binary(16) NOT NULL;
ALTER TABLE `groups` CHANGE `icon5` `icon5` binary(16) NOT NULL;
ALTER TABLE `groups` CHANGE `icon6` `icon6` binary(16) NOT NULL;
ALTER TABLE `groups` CHANGE `icon7` `icon7` binary(16) NOT NULL;
ALTER TABLE `groups` CHANGE `icon8` `icon8` binary(16) NOT NULL;

--
ALTER TABLE `character_brackets_info` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `auctionhouse` CHANGE `auctioneerguid` `auctioneerguid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `auctionhouse` CHANGE `itemguid` `itemguid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `auctionhouse` CHANGE `itemowner` `itemowner` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `auctionhouse` CHANGE `buyguid` `buyguid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';

--
ALTER TABLE `challenge` CHANGE `guildId` `guildId` BIGINT(20) NOT NULL DEFAULT '0';
ALTER TABLE `challenge_member` CHANGE `member` `member` BIGINT(20) NOT NULL DEFAULT '0';
ALTER TABLE `character_account_data` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_achievement` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_achievement_progress` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_action` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_aura` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_aura_effect` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_banned` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_battleground_data` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_battleground_random` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_cuf_profiles` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_currency` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_declinedname` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `guild_achievement` CHANGE `guildId` `guildId` BIGINT(20) UNSIGNED NOT NULL;
ALTER TABLE `guild_achievement_progress` CHANGE `completedGuid` `completedGuid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `guild_achievement_progress` CHANGE `guildId` `guildId` BIGINT(20) UNSIGNED NOT NULL;

--
ALTER TABLE `character_social` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_social` CHANGE `friend` `friend` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_spell` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_spell_cooldown` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_stats` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_talent` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_void_storage` CHANGE `playerGuid` `playerGuid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_void_storage` CHANGE `creatorGuid` `creatorGuid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `corpse` CHANGE `corpseGuid` `corpseGuid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `corpse` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gm_surveys` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gm_tickets` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gm_tickets` CHANGE `closedBy` `closedBy` BIGINT(20) NOT NULL DEFAULT '0';
ALTER TABLE `gm_tickets` CHANGE `assignedTo` `assignedTo` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `characters` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `characters` CHANGE `transguid` `transguid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
--
ALTER TABLE `character_battle_pet_journal` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL;

--
ALTER TABLE `character_equipmentsets` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item0` `item0` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item1` `item1` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item2` `item2` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item3` `item3` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item4` `item4` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item5` `item5` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item6` `item6` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item7` `item7` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item8` `item8` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item9` `item9` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item10` `item10` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item11` `item11` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item12` `item12` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item13` `item13` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item14` `item14` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item15` `item15` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item16` `item16` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item17` `item17` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_equipmentsets` CHANGE `item18` `item18` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_gifts` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_gifts` CHANGE `item_guid` `item_guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_glyphs` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_homebind` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_instance` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_inventory` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_inventory` CHANGE `bag` `bag` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_inventory` CHANGE `item` `item` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_queststatus` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_queststatus_daily` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_queststatus_monthly` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_queststatus_rewarded` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_queststatus_seasonal` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_queststatus_weekly` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_reputation` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `character_skills` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';

--
ALTER TABLE `store_item` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL;
ALTER TABLE `character_kill` CHANGE `guid` `guid` BIGINT(20) NOT NULL DEFAULT '0';
ALTER TABLE `character_kill` CHANGE `victim_guid` `victim_guid` BIGINT(20) NOT NULL DEFAULT '0';