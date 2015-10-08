DROP TABLE IF EXISTS `account_heirlooms`;
CREATE TABLE `account_heirlooms` (
  `accountId` int(10) unsigned NOT NULL,
  `itemId` int(11) unsigned NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`accountId`,`itemId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `account_toys`;
CREATE TABLE `account_toys` (
  `accountId` int(10) unsigned NOT NULL,
  `itemId` int(11) NOT NULL DEFAULT '0',
  `isFavourite` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`accountId`,`itemId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
