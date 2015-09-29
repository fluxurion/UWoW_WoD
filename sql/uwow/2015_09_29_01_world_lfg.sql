
SQLyog Community v12.14 (64 bit)
MySQL - 5.6.24-log 
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

ALTER TABLE `lfg_entrances`   
  CHANGE `name` `name` VARCHAR(255) CHARSET latin1 COLLATE latin1_swedish_ci DEFAULT ''  NOT NULL,
  CHARSET=utf8, COLLATE=utf8_general_ci;

replace into `lfg_entrances` (`dungeonId`, `name`, `position_x`, `position_y`, `position_z`, `orientation`) values
('640','Proving Grounds: White Tiger Temple','3756.82','521.777','639.692','2.50959'),
('779','Skyreach Dungeon','983.505','1857.36','241.933','4.00785'),
('780','Skyreach heroic','983.505','1857.36','241.933','4.00785'),
('783','Shadowmoon Burial Grounds Dungeon','1719.15','239.788','324.535','5.54098'),
('784','Shadowmoon Burial Grounds heroic','1719.15','239.788','324.535','5.54098'),
('787','Bloodmaul Slag Mines Dungeon','1829.37','-245.757','255.727','0.804761'),
('820','Auchindoun Dungeon','1487.36','2953.42','35.3079','6.27588'),
('821','Iron Docks Dungeon','6746.76','-546.786','4.92545','5.38226'),
('822','Grimrail Depot Dungeon','1738.38','1681.08','7.67828','3.1428'),
('823','Blackhand\'s Crucible LFR','131.172','3429.48','319.829','0.00134863'),
('824','The Everbloom Dungeon','429.429','1327.47','125.017','0.645025'),
('828','Upper Blackrock Spire Dungeon','97.9944','-258.636','91.4537','5.9983'),
('845','Auchindoun heroic','1487.36','2953.42','35.3079','6.27588'),
('846','The Black Forge LFR','306.731','3631.79','104.688','0.00638866'),
('847','Slagworks LFR','131.172','3429.48','319.829','0.00134863'),
('849','Walled City LFR','3486.48','7603.32','10.4853','4.02518'),
('850','Arcane Sanctum LFR','3500.13','7619.39','55.3054','0.899281'),
('851','Imperator Rise LFR','3950.33','8607.45','565.34','4.39762'),
('852','Molten Core LFR event inst','891.846','-544.53','-203.466','2.46616'),
('857','Iron Docks heroic','6746.76','-546.786','4.92545','5.38226'),
('858','Grimrail Depot heroic','1738.38','1681.08','7.67828','3.1428'),
('859','Bloodmaul Slag Mines heroic','1829.37','-245.757','255.727','0.804761'),
('860','Upper Blackrock Spire heroic','97.9944','-258.636','91.4537','5.9983'),
('866','The Everbloom heroic','429.429','1327.47','125.017','0.645025');

insert into `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) values
('2','230','15','','','Корен Худовар ивентовый данж'),
('2','409','15','','','Огненные Недра ивентовый инст на НГ');

insert ignore into `access_requirement` (`mapId`, `difficulty`, `level_min`, `level_max`, `item_level`, `item`, `item2`, `quest_done_A`, `quest_done_H`, `completed_achievement`, `completed_achievement_A`, `quest_failed_text`, `comment`) values
('1205','14','100','0','0','0','0','0','0','0','0',NULL,'Верховный Молот'),
('1205','15','100','0','0','0','0','0','0','0','0',NULL,'Верховный Молот'),
('1205','16','100','0','0','0','0','0','0','0','0',NULL,'Верховный Молот'),
('1205','17','100','0','635','0','0','0','0','0','0',NULL,'Верховный Молот'),
('1228','14','100','0','0','0','0','0','0','0','0',NULL,'Верховный Молот'),
('1228','15','100','0','0','0','0','0','0','0','0',NULL,'Верховный Молот'),
('1228','16','100','0','0','0','0','0','0','0','0',NULL,'Верховный Молот'),
('1228','17','100','0','615','0','0','0','0','0','0',NULL,'Верховный Молот');

replace into `access_requirement` (`mapId`, `difficulty`, `level_min`, `level_max`, `item_level`, `item`, `item2`, `quest_done_A`, `quest_done_H`, `completed_achievement`, `completed_achievement_A`, `quest_failed_text`, `comment`) values
('1136','14','90','0','0','0','0','0','0','0','0',NULL,'Siege of Orgrimmar - NORMAL'),
('1136','15','90','0','0','0','0','0','0','0','0',NULL,'Siege of Orgrimmar - HEROIC'),
('1136','16','90','0','0','0','0','0','0','8680','8679',NULL,'Siege of Orgrimmar - Mythic'),
('1136','17','90','0','496','0','0','0','0','8680','8679',NULL,'Siege of Orgrimmar - LFR');

