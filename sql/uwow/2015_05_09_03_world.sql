-- SMSG_QUESTGIVER_QUEST_DETAILS
DROP TABLE IF EXISTS `quest_details`;
CREATE TABLE `quest_details` (
    `ID` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
    `Emote1` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `Emote2` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `Emote3` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `Emote4` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `EmoteDelay1` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `EmoteDelay2` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `EmoteDelay3` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `EmoteDelay4` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `VerifiedBuild` SMALLINT(5) NOT NULL DEFAULT '0',
    PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

INSERT INTO `quest_details` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`)
SELECT `ID`, `DetailsEmote1`, `DetailsEmote2`, `DetailsEmote3`, `DetailsEmote4`, `DetailsEmoteDelay1`, `DetailsEmoteDelay2`, `DetailsEmoteDelay3`, `DetailsEmoteDelay4` FROM `quest_template`
WHERE `DetailsEmote1`!=0 OR `DetailsEmote2`!=0 OR `DetailsEmote3`!=0 OR `DetailsEmote4`!=0 OR `DetailsEmoteDelay1`!=0 OR `DetailsEmoteDelay2`!=0 OR `DetailsEmoteDelay3`!=0 OR `DetailsEmoteDelay4`!=0;

-- `DetailsEmote1`, `DetailsEmote2`, `DetailsEmote3`, `DetailsEmote4`, `DetailsEmoteDelay1`, `DetailsEmoteDelay2`, `DetailsEmoteDelay3`, `DetailsEmoteDelay4`

-- SMSG_QUESTGIVER_REQUEST_ITEMS
DROP TABLE IF EXISTS `quest_request_items`;
CREATE TABLE `quest_request_items` (
    `ID` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
    `EmoteOnComplete` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `EmoteOnIncomplete` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `EmoteOnCompleteDelay` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `EmoteOnIncompleteDelay` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `CompletionText` TEXT,
    `VerifiedBuild` SMALLINT(5) NOT NULL DEFAULT '0',
    PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, EmoteOnCompleteDelay, EmoteOnIncompleteDelay, `CompletionText`)
SELECT `ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, EmoteOnCompleteDelay, EmoteOnIncompleteDelay, `RequestItemsText` FROM `quest_template`
WHERE `EmoteOnComplete`!=0 OR `EmoteOnIncomplete`!=0 OR `RequestItemsText`!='' OR EmoteOnCompleteDelay != 0 OR EmoteOnIncompleteDelay != 0;

-- `EmoteOnComplete`, `EmoteOnIncomplete`, `RequestItemsText`, EmoteOnCompleteDelay, EmoteOnIncompleteDelay

-- SMSG_QUESTGIVER_OFFER_REWARD
DROP TABLE IF EXISTS `quest_offer_reward`;
CREATE TABLE `quest_offer_reward` (
    `ID` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
    `Emote1` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `Emote2` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `Emote3` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `Emote4` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `EmoteDelay1` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `EmoteDelay2` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `EmoteDelay3` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `EmoteDelay4` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `RewardText` TEXT,
    `VerifiedBuild` SMALLINT(5) NOT NULL DEFAULT '0',
    PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`)
SELECT `ID`, `OfferRewardEmote1`, `OfferRewardEmote2`, `OfferRewardEmote3`, `OfferRewardEmote4`, `OfferRewardEmoteDelay1`, `OfferRewardEmoteDelay2`, `OfferRewardEmoteDelay3`, `OfferRewardEmoteDelay4`, `OfferRewardText` FROM `quest_template`
WHERE `OfferRewardEmote1`!=0 OR `OfferRewardEmote2`!=0 OR `OfferRewardEmote3`!=0 OR `OfferRewardEmote4`!=0 OR `OfferRewardEmoteDelay1`!=0 OR `OfferRewardEmoteDelay2`!=0 OR `OfferRewardEmoteDelay3`!=0 OR `OfferRewardEmoteDelay4`!=0 OR `OfferRewardText`!='';


DROP TABLE IF EXISTS `quest_template_addon`;
CREATE TABLE `quest_template_addon` (
    `ID` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
    `MaxLevel` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `AllowableClasses` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `SourceSpellID` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
    `PrevQuestID` MEDIUMINT(8) NOT NULL DEFAULT '0',
    `NextQuestID` MEDIUMINT(8) NOT NULL DEFAULT '0',
    `ExclusiveGroup` MEDIUMINT(8) NOT NULL DEFAULT '0',
    `RewardMailTemplateID` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
    `RewardMailDelay` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `ProvidedItemCount` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `SpecialFlags` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

INSERT INTO `quest_template_addon` (`ID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `RewardMailTemplateID`, `RewardMailDelay`, `SourceSpellID`, `SpecialFlags`, `MaxLevel`, `AllowableClasses`, `ProvidedItemCount`) 
SELECT `ID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `RewardMailTemplateId`, `RewardMailDelay`, `SourceSpellID`, `SpecialFlags`, `MaxLevel`, `AllowableClasses`, `SourceItemCount` FROM `quest_template` 
WHERE `PrevQuestID`!=0 OR `NextQuestID`!=0 OR `ExclusiveGroup`!=0 OR `RewardMailTemplateId`!=0 OR `RewardMailDelay`!=0 OR `SourceSpellID`!=0 OR `SpecialFlags`!=0 OR `MaxLevel`!=0 OR `AllowableClasses`!=0 OR `SourceItemCount`!=0;

ALTER TABLE `quest_template_addon`
  ADD `RequiredSkillID` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewardMailDelay`,
  ADD `RequiredSkillPoints` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0' AFTER `RequiredSkillID`,
  ADD `RequiredMinRepFaction` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0' AFTER `RequiredSkillPoints`,
  ADD `RequiredMaxRepFaction` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0' AFTER `RequiredMinRepFaction`,
  ADD `RequiredMinRepValue` MEDIUMINT(8) NOT NULL DEFAULT '0' AFTER `RequiredMaxRepFaction`,
  ADD `RequiredMaxRepValue` MEDIUMINT(8) NOT NULL DEFAULT '0' AFTER `RequiredMinRepValue`;

INSERT INTO `quest_template_addon` (`ID`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`)
SELECT `ID`, `RequiredSkillId`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue` FROM `quest_template`
WHERE `ID` NOT IN (SELECT `ID` FROM `quest_template_addon`) AND (`RequiredSkillId`!=0 OR `RequiredSkillPoints`!=0 OR `RequiredMinRepFaction`!=0 OR `RequiredMaxRepFaction`!=0 OR `RequiredMinRepValue`!=0 OR `RequiredMaxRepValue`!=0);

UPDATE `quest_template_addon` A INNER JOIN `quest_template` B ON A.`ID`=B.`ID`
SET A.`RequiredSkillID`=B.`RequiredSkillId`, A.`RequiredSkillPoints`=B.`RequiredSkillPoints`, A.`RequiredMinRepFaction`=B.`RequiredMinRepFaction`, A.`RequiredMaxRepFaction`=B.`RequiredMaxRepFaction`, A.`RequiredMinRepValue`=B.`RequiredMinRepValue`, A.`RequiredMaxRepValue`=B.`RequiredMaxRepValue`;


DROP TABLE IF EXISTS `quest_objectives`;
CREATE TABLE `quest_objectives` (
	`ID` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
	`QuestID` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
	`Type` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
	`StorageIndex` TINYINT(3) NOT NULL DEFAULT '0',
	`ObjectID` INT(10) NOT NULL DEFAULT '0',
	`Amount` INT(10) NOT NULL DEFAULT '0',
	`Flags` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`UnkFloat` FLOAT NOT NULL DEFAULT '0',
	`Description` TEXT,
	`VerifiedBuild` SMALLINT(5) NOT NULL DEFAULT '0',
	PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `quest_visual_effect`;
CREATE TABLE `quest_visual_effect` (
    `ID` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
    `Index` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `VisualEffect` MEDIUMINT(8) NOT NULL DEFAULT '0',
    `VerifiedBuild` SMALLINT(5) NOT NULL DEFAULT '0',
    PRIMARY KEY (`ID`, `Index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

ALTER TABLE `quest_template`
  DROP `DetailsEmote1`,
  DROP `DetailsEmote2`,
  DROP `DetailsEmote3`,
  DROP `DetailsEmote4`,
  DROP `DetailsEmoteDelay1`,
  DROP `DetailsEmoteDelay2`,
  DROP `DetailsEmoteDelay3`,
  DROP `DetailsEmoteDelay4`,
  DROP `EmoteOnIncomplete`,
  DROP `EmoteOnComplete`,
  DROP `RequestItemsText`,
  DROP EmoteOnCompleteDelay,
  DROP EmoteOnIncompleteDelay,
  DROP `OfferRewardEmote1`,
  DROP `OfferRewardEmote2`,
  DROP `OfferRewardEmote3`,
  DROP `OfferRewardEmote4`,
  DROP `OfferRewardEmoteDelay1`,
  DROP `OfferRewardEmoteDelay2`,
  DROP `OfferRewardEmoteDelay3`,
  DROP `OfferRewardEmoteDelay4`,
  DROP `OfferRewardText`,
  DROP `PrevQuestID`,
  DROP `NextQuestID`,
  DROP `ExclusiveGroup`,
  DROP `RewardMailTemplateId`,
  DROP `RewardMailDelay`,
  DROP `SourceSpellID`,
  DROP `SpecialFlags`,
  DROP `MaxLevel`,
  DROP `AllowableClasses`,
  DROP RequiredSkillId,
  DROP RequiredSkillPoints,
  DROP RequiredMinRepFaction,
  DROP RequiredMaxRepFaction,
  DROP RequiredMinRepValue,
  DROP RequiredMaxRepValue,
  DROP RequiredTeam,
  DROP `SourceItemCount`,
  DROP `RequiredFactionId1`,
  DROP `RequiredFactionId2`,
  DROP `RequiredFactionValue1`,
  DROP `RequiredFactionValue2`,
  DROP `RequirementType1`,
  DROP `RequirementType2`,
  DROP `RequirementType3`,
  DROP `RequirementType4`,
  DROP `RequirementType5`,
  DROP `RequirementType6`,
  DROP `RequirementType7`,
  DROP `RequirementType8`,
  DROP `RequirementType9`,
  DROP `RequirementType10`,
  DROP `RequiredId1`,
  DROP `RequiredId2`,
  DROP `RequiredId3`,
  DROP `RequiredId4`,
  DROP `RequiredId5`,
  DROP `RequiredId6`,
  DROP `RequiredId7`,
  DROP `RequiredId8`,
  DROP `RequiredId9`,
  DROP `RequiredId10`,
  DROP `RequiredIdCount1`,
  DROP `RequiredIdCount2`,
  DROP `RequiredIdCount3`,
  DROP `RequiredIdCount4`,
  DROP `RequiredIdCount5`,
  DROP `RequiredIdCount6`,
  DROP `RequiredIdCount7`,
  DROP `RequiredIdCount8`,
  DROP `RequiredIdCount9`,
  DROP `RequiredIdCount10`,
  DROP `RequiredPOI1`,
  DROP `RequiredPOI2`,
  DROP `RequiredPOI3`,
  DROP `RequiredPOI4`,
  DROP `RequiredPOI5`,
  DROP `RequiredPOI6`,
  DROP `RequiredPOI7`,
  DROP `RequiredPOI8`,
  DROP `RequiredPOI9`,
  DROP `RequiredPOI10`, 
  DROP `RequiredUnkFlag1`,
  DROP `RequiredUnkFlag2`,
  DROP `RequiredUnkFlag3`,
  DROP `RequiredUnkFlag4`,
  DROP `RequiredUnkFlag5`,
  DROP `RequiredUnkFlag6`,
  DROP `RequiredUnkFlag7`,
  DROP `RequiredUnkFlag8`,
  DROP `RequiredUnkFlag9`,
  DROP `RequiredUnkFlag10`,
  DROP `ObjectiveText1`,
  DROP `ObjectiveText2`,
  DROP `ObjectiveText3`,
  DROP `ObjectiveText4`,
  DROP `ObjectiveText5`,
  DROP `ObjectiveText6`,
  DROP `ObjectiveText7`,
  DROP `ObjectiveText8`,
  DROP `ObjectiveText9`,
  DROP `ObjectiveText10`,
  DROP RequiredSpell,
  DROP RequiredSpellCast1,
  DROP RequiredSpellCast2,
  DROP RequiredSpellCast3,
  DROP RequiredSpellCast4;
  