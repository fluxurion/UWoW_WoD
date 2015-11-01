INSERT INTO trinity_string VALUES (21002, 'Your client version is not valid for playing on our server. You will be kicked from the server in a while for security reason. Please make sure that you run the game using epicwow_launcher.exe or follow the forum this topic', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ваша версия клиента не подходит для игры на нашем сервере. По соображениям безопасности вы будете исключены из игры через некоторое время. Пожалуйста, убедитесь, что вы запускаете игру через epicwow_launcher.exe или проследуйте на форум в тему - http://forum.epicwow.com/topic/871-nesovmestimaya-versiya-klienta/', NULL, NULL);
INSERT INTO trinity_string VALUES (21003, 'Your client version is not valid for playing on our server. You will be kicked from the server in a while for security reason', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ваша версия клиента не подходит для игры на нашем сервере. По соображениям безопасности вы будете исключены из игры через некоторое время', NULL, NULL);

DROP TABLE IF EXISTS `warden_checks`;

CREATE TABLE `warden_checks` (
  `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `type` tinyint(3) unsigned DEFAULT NULL,
  `data` varchar(48) DEFAULT NULL,
  `str` varchar(20) DEFAULT NULL,
  `address` int(10) unsigned DEFAULT NULL,
  `length` tinyint(3) unsigned DEFAULT NULL,
  `result` varchar(24) DEFAULT NULL,
  `comment` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

INSERT INTO warden_checks VALUES (NULL, 27, 'A444519CC419521B6D39990C1D95329C8D94B59226CBAA98', NULL, 16507, 32, 'E9', 'WPE PRO detected');

DROP TABLE IF EXISTS `warden_action`;

CREATE TABLE `warden_action` (
  `wardenId` smallint(5) unsigned NOT NULL,
  `action` tinyint(3) unsigned DEFAULT NULL,
  PRIMARY KEY (`wardenId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO warden_action VALUES (1, 2);