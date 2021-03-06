update quest_template SET RewardSpell = 0 WHERE RewardSpell < 0;
update quest_template SET RewardSpellCast = 0 WHERE RewardSpellCast < 0;

ALTER TABLE `quest_template` CHANGE `Id` `ID` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `Method` `QuestType` TINYINT(3) UNSIGNED NOT NULL DEFAULT '2';
ALTER TABLE `quest_template` CHANGE `Level` `QuestLevel` INT(11) NOT NULL DEFAULT '-1';
ALTER TABLE `quest_template` ADD COLUMN `QuestPackageID` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `QuestLevel`;
ALTER TABLE `quest_template` CHANGE `MinLevel` `MinLevel`  INT(11) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `MaxLevel` `MaxLevel`  INT(11) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `ZoneOrSort` `QuestSortID` SMALLINT(6) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `Type` `QuestInfoID` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `SuggestedPlayers` `SuggestedGroupNum` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `LimitTime` `TimeAllowed` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `SoundTurnIn`;
ALTER TABLE `quest_template` CHANGE `RequiredClasses` `AllowableClasses` INT(11) NOT NULL DEFAULT '-1' AFTER `TimeAllowed`;
ALTER TABLE `quest_template` CHANGE `RequiredRaces` `AllowableRaces` INT(11) NOT NULL DEFAULT '-1' AFTER `TimeAllowed`;
ALTER TABLE `quest_template` CHANGE `NextQuestIdChain` `RewardNextQuest` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardXPId` `RewardXPDifficulty` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` ADD COLUMN `Float10` float NOT NULL DEFAULT '1' AFTER `RewardXPDifficulty`;
ALTER TABLE `quest_template` CHANGE `RewardOrRequiredMoney` `RewardMoney` INT(11) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` ADD COLUMN `RewardMoneyDifficulty` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardMoney`;
ALTER TABLE `quest_template` ADD COLUMN `Float13` float NOT NULL DEFAULT '1' AFTER `RewardMoneyDifficulty`;
ALTER TABLE `quest_template` CHANGE `RewardMoneyMaxLevel` `RewardBonusMoney` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardSpell` `RewardDisplaySpell` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardSpellCast` `RewardSpell` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardHonor` `RewardHonor` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardHonorMultiplier` `RewardKillHonor` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `SourceItemId` `StartItem` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` ADD COLUMN `FlagsEx` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `Flags`;
ALTER TABLE `quest_template` ADD COLUMN `AreaGroupID` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `SoundTurnIn`;
ALTER TABLE `quest_template` CHANGE `RewardTitleId` `RewardTitle` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `PointOption`;
ALTER TABLE `quest_template` DROP COLUMN `RequiredPlayerKills`;
ALTER TABLE `quest_template` CHANGE `RewardTalents` `RewardTalents` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardTitle`;
ALTER TABLE `quest_template` CHANGE `RewardArenaPoints` `RewardArenaPoints` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardTalents`;
ALTER TABLE `quest_template` CHANGE `RewardSkillId` `RewardSkillLineID` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardArenaPoints`;
ALTER TABLE `quest_template` CHANGE `RewardSkillPoints` `RewardNumSkillUps` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardSkillLineID`;
ALTER TABLE `quest_template` CHANGE `QuestGiverPortrait` `PortraitGiver` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardNumSkillUps`;
ALTER TABLE `quest_template` CHANGE `QuestTurnInPortrait` `PortraitTurnIn` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `PortraitGiver`;
ALTER TABLE `quest_template` CHANGE `RewardItemId1` `RewardItem1` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardItemId2` `RewardItem2` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardItemCount1`;
ALTER TABLE `quest_template` CHANGE `RewardItemId3` `RewardItem3` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardItemCount2`;
ALTER TABLE `quest_template` CHANGE `RewardItemId4` `RewardItem4` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardItemCount3`;
ALTER TABLE `quest_template` CHANGE `RewardItemCount1` `RewardAmount1` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardItemCount2` `RewardAmount2` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardItemCount3` `RewardAmount3` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardItemCount4` `RewardAmount4` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardChoiceItemId1` `RewardChoiceItemID1` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardChoiceItemId2` `RewardChoiceItemID2` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardChoiceItemCount1`;
ALTER TABLE `quest_template` CHANGE `RewardChoiceItemId3` `RewardChoiceItemID3` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardChoiceItemCount2`;
ALTER TABLE `quest_template` CHANGE `RewardChoiceItemId4` `RewardChoiceItemID4` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardChoiceItemCount3`;
ALTER TABLE `quest_template` CHANGE `RewardChoiceItemId5` `RewardChoiceItemID5` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardChoiceItemCount4`;
ALTER TABLE `quest_template` CHANGE `RewardChoiceItemId6` `RewardChoiceItemID6` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardChoiceItemCount5`;
ALTER TABLE `quest_template` CHANGE `RewardChoiceItemCount1` `RewardChoiceItemQuantity1` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` ADD COLUMN `RewardChoiceItemDisplayID1` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardChoiceItemQuantity1`;
ALTER TABLE `quest_template` CHANGE `RewardChoiceItemCount2` `RewardChoiceItemQuantity2` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` ADD COLUMN `RewardChoiceItemDisplayID2` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardChoiceItemQuantity2`;
ALTER TABLE `quest_template` CHANGE `RewardChoiceItemCount3` `RewardChoiceItemQuantity3` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` ADD COLUMN `RewardChoiceItemDisplayID3` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardChoiceItemQuantity3`;
ALTER TABLE `quest_template` CHANGE `RewardChoiceItemCount4` `RewardChoiceItemQuantity4` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` ADD COLUMN `RewardChoiceItemDisplayID4` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardChoiceItemQuantity4`;
ALTER TABLE `quest_template` CHANGE `RewardChoiceItemCount5` `RewardChoiceItemQuantity5` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` ADD COLUMN `RewardChoiceItemDisplayID5` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardChoiceItemQuantity5`;
ALTER TABLE `quest_template` CHANGE `RewardChoiceItemCount6` `RewardChoiceItemQuantity6` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` ADD COLUMN `RewardChoiceItemDisplayID6` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardChoiceItemQuantity6`;
ALTER TABLE `quest_template` CHANGE `RewardFactionId1` `RewardFactionID1` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `PortraitTurnIn`;
ALTER TABLE `quest_template` CHANGE `RewardFactionValueId1` `RewardFactionValue1` INT(11) NOT NULL DEFAULT '0' AFTER `RewardFactionID1`;
ALTER TABLE `quest_template` CHANGE `RewardFactionValueIdOverride1` `RewardFactionOverride1` INT(11) NOT NULL DEFAULT '0' AFTER `RewardFactionValue1`;
ALTER TABLE `quest_template` CHANGE `RewardFactionId2` `RewardFactionID2` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardFactionOverride1`;
ALTER TABLE `quest_template` CHANGE `RewardFactionValueId2` `RewardFactionValue2` INT(11) NOT NULL DEFAULT '0' AFTER `RewardFactionID2`;
ALTER TABLE `quest_template` CHANGE `RewardFactionValueIdOverride2` `RewardFactionOverride2` INT(11) NOT NULL DEFAULT '0' AFTER `RewardFactionValue2`;
ALTER TABLE `quest_template` CHANGE `RewardFactionId3` `RewardFactionID3` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardFactionOverride2`;
ALTER TABLE `quest_template` CHANGE `RewardFactionValueId3` `RewardFactionValue3` INT(11) NOT NULL DEFAULT '0' AFTER `RewardFactionID3`;
ALTER TABLE `quest_template` CHANGE `RewardFactionValueIdOverride3` `RewardFactionOverride3` INT(11) NOT NULL DEFAULT '0' AFTER `RewardFactionValue3`;
ALTER TABLE `quest_template` CHANGE `RewardFactionId4` `RewardFactionID4` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardFactionOverride3`;
ALTER TABLE `quest_template` CHANGE `RewardFactionValueId4` `RewardFactionValue4` INT(11) NOT NULL DEFAULT '0' AFTER `RewardFactionID4`;
ALTER TABLE `quest_template` CHANGE `RewardFactionValueIdOverride4` `RewardFactionOverride4` INT(11) NOT NULL DEFAULT '0' AFTER `RewardFactionValue4`;
ALTER TABLE `quest_template` CHANGE `RewardFactionId5` `RewardFactionID5` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardFactionOverride4`;
ALTER TABLE `quest_template` CHANGE `RewardFactionValueId5` `RewardFactionValue5` INT(11) NOT NULL DEFAULT '0' AFTER `RewardFactionID5`;
ALTER TABLE `quest_template` CHANGE `RewardFactionValueIdOverride5` `RewardFactionOverride5` INT(11) NOT NULL DEFAULT '0' AFTER `RewardFactionValue5`;
ALTER TABLE `quest_template` CHANGE `RewardReputationMask` `RewardFactionFlags` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardFactionOverride5`;
ALTER TABLE `quest_template` CHANGE `PointMapId` `POIContinent` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `PointX` `POIx` float NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `PointY` `POIy` float NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `PointOption` `POIPriority` INT(11) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `Title` `LogTitle` text AFTER `AllowableClasses`;
ALTER TABLE `quest_template` CHANGE `Objectives` `LogDescription` text AFTER `LogTitle`;
ALTER TABLE `quest_template` CHANGE `Details` `QuestDescription` text AFTER `LogDescription`;
ALTER TABLE `quest_template` CHANGE `EndText` `AreaDescription` text AFTER `QuestDescription`;
ALTER TABLE `quest_template` CHANGE `OfferRewardText` `OfferRewardText` text AFTER `AreaDescription`;
ALTER TABLE `quest_template` CHANGE `RequestItemsText` `RequestItemsText` text AFTER `OfferRewardText`;
ALTER TABLE `quest_template` CHANGE `CompletedText` `QuestCompletionLog` text AFTER `AreaDescription`;
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemId1` `ItemDrop1` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardAmount4`;
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemCount1` `ItemDropQuantity1` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `ItemDrop1`;
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemId2` `ItemDrop2` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `ItemDropQuantity1`;
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemCount2` `ItemDropQuantity2` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `ItemDrop2`;
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemId3` `ItemDrop3` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `ItemDropQuantity2`;
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemCount3` `ItemDropQuantity3` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `ItemDrop3`;
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemId4` `ItemDrop4` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `ItemDropQuantity3`;
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemCount4` `ItemDropQuantity4` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `ItemDrop4`;
ALTER TABLE `quest_template` CHANGE `RewardCurrencyId1` `RewardCurrencyID1` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardCurrencyCount1` `RewardCurrencyQty1` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardCurrencyID1`;
ALTER TABLE `quest_template` CHANGE `RewardCurrencyId2` `RewardCurrencyID2` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardCurrencyCount2` `RewardCurrencyQty2` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardCurrencyID2`;
ALTER TABLE `quest_template` CHANGE `RewardCurrencyId3` `RewardCurrencyID3` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardCurrencyCount3` `RewardCurrencyQty3` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardCurrencyID3`;
ALTER TABLE `quest_template` CHANGE `RewardCurrencyId4` `RewardCurrencyID4` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardCurrencyCount4` `RewardCurrencyQty4` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardCurrencyID4`;
ALTER TABLE `quest_template` CHANGE `QuestGiverTextWindow` `PortraitGiverText` text AFTER `OfferRewardText`;
ALTER TABLE `quest_template` CHANGE `QuestGiverTargetName` `PortraitGiverName` text AFTER `PortraitGiverText`;
ALTER TABLE `quest_template` CHANGE `QuestTurnTextWindow` `PortraitTurnInText` text AFTER `PortraitGiverName`;
ALTER TABLE `quest_template` CHANGE `QuestTurnTargetName` `PortraitTurnInName` text AFTER `PortraitTurnInText`;
ALTER TABLE `quest_template` CHANGE `SoundAccept` `AcceptedSoundKitID` INT(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `SoundTurnIn` `CompleteSoundKitID` INT(10) UNSIGNED NOT NULL DEFAULT '0';

UPDATE `quest_template` SET `AllowableRaces` = -1 WHERE `AllowableRaces` = 0;

UPDATE `quest_template` SET FlagsEx = Flags2;
ALTER TABLE `quest_template` DROP COLUMN `Flags2`;
UPDATE `quest_template` SET QuestPackageID = PackageItem;
ALTER TABLE `quest_template` DROP COLUMN `PackageItem`;

ALTER TABLE `quest_template` ADD COLUMN `EmoteOnCompleteDelay` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `EmoteOnComplete`;
ALTER TABLE `quest_template` ADD COLUMN `EmoteOnIncompleteDelay` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `EmoteOnCompleteDelay`;

    
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredSkillId`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredSkillPoints`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredFactionId1`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredFactionId2`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredFactionValue1`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredFactionValue2`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredMinRepFaction`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredMaxRepFaction`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredMinRepValue`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredMaxRepValue`;
-- ALTER TABLE `quest_template` DROP COLUMN `PrevQuestId`;
-- ALTER TABLE `quest_template` DROP COLUMN `NextQuestId`;
-- ALTER TABLE `quest_template` DROP COLUMN `ExclusiveGroup`;
-- ALTER TABLE `quest_template` DROP COLUMN `RewardMailTemplateId`;
-- ALTER TABLE `quest_template` DROP COLUMN `RewardMailDelay`;
-- ALTER TABLE `quest_template` DROP COLUMN `SourceItemCount`;
-- ALTER TABLE `quest_template` DROP COLUMN `SourceSpellId`;
-- ALTER TABLE `quest_template` DROP COLUMN `SpecialFlags`;

-- ALTER TABLE `quest_template` DROP COLUMN `RequiredNpcOrGo1`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredNpcOrGo2`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredNpcOrGo3`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredNpcOrGo4`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredNpcOrGoCount1`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredNpcOrGoCount2`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredNpcOrGoCount3`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredNpcOrGoCount4`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredItemId1`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredItemId2`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredItemId3`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredItemId4`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredItemId5`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredItemId6`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredItemCount1`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredItemCount2`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredItemCount3`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredItemCount4`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredItemCount5`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredItemCount6`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredSpell`;
-- ALTER TABLE `quest_template` DROP COLUMN `ObjectiveText1`;
-- ALTER TABLE `quest_template` DROP COLUMN `ObjectiveText2`;
-- ALTER TABLE `quest_template` DROP COLUMN `ObjectiveText3`;
-- ALTER TABLE `quest_template` DROP COLUMN `ObjectiveText4`;

-- ALTER TABLE `quest_template` DROP COLUMN `RequiredCurrencyId1`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredCurrencyId2`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredCurrencyId3`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredCurrencyId4`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredCurrencyCount1`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredCurrencyCount2`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredCurrencyCount3`;
-- ALTER TABLE `quest_template` DROP COLUMN `RequiredCurrencyCount4`;

-- ALTER TABLE `quest_template` DROP COLUMN `DetailsEmote1`;
-- ALTER TABLE `quest_template` DROP COLUMN `DetailsEmote2`;
-- ALTER TABLE `quest_template` DROP COLUMN `DetailsEmote3`;
-- ALTER TABLE `quest_template` DROP COLUMN `DetailsEmote4`;
-- ALTER TABLE `quest_template` DROP COLUMN `DetailsEmoteDelay1`;
-- ALTER TABLE `quest_template` DROP COLUMN `DetailsEmoteDelay2`;
-- ALTER TABLE `quest_template` DROP COLUMN `DetailsEmoteDelay3`;
-- ALTER TABLE `quest_template` DROP COLUMN `DetailsEmoteDelay4`;
-- ALTER TABLE `quest_template` DROP COLUMN `EmoteOnIncomplete`;
-- ALTER TABLE `quest_template` DROP COLUMN `EmoteOnComplete`;
-- ALTER TABLE `quest_template` DROP COLUMN `OfferRewardEmote1`;
-- ALTER TABLE `quest_template` DROP COLUMN `OfferRewardEmote2`;
-- ALTER TABLE `quest_template` DROP COLUMN `OfferRewardEmote3`;
-- ALTER TABLE `quest_template` DROP COLUMN `OfferRewardEmote4`;
-- ALTER TABLE `quest_template` DROP COLUMN `OfferRewardEmoteDelay1`;
-- ALTER TABLE `quest_template` DROP COLUMN `OfferRewardEmoteDelay2`;
-- ALTER TABLE `quest_template` DROP COLUMN `OfferRewardEmoteDelay3`;
-- ALTER TABLE `quest_template` DROP COLUMN `OfferRewardEmoteDelay4`;