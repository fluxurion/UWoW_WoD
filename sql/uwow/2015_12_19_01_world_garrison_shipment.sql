-- phpMyAdmin SQL Dump
-- version 4.4.14
-- http://www.phpmyadmin.net
--
-- Хост: localhost
-- Время создания: Дек 19 2015 г., 12:04
-- Версия сервера: 10.0.20-MariaDB
-- Версия PHP: 5.3.8

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- База данных: `wod_world`
--

-- --------------------------------------------------------


--
-- Структура таблицы `garrison_shipment`
--

DROP TABLE IF EXISTS `garrison_shipment`;
CREATE TABLE `garrison_shipment` (
  `SiteID` int(11) NOT NULL DEFAULT '2' COMMENT '2 - Alliance, 71 - Horde, 0 - all',
  `ContainerID` int(11) NOT NULL,
  `NpcEntry` int(10) unsigned NOT NULL,
  `ConteinerGoEntry` int(10) unsigned NOT NULL,
  `ShipmentID` int(10) unsigned NOT NULL,
  `MaxShipments` int(10) unsigned NOT NULL,
  `BuildingTypeID` int(10) unsigned NOT NULL,
  `comment` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `garrison_shipment`
--

INSERT INTO `garrison_shipment` (`SiteID`, `ContainerID`, `NpcEntry`, `ConteinerGoEntry`, `ShipmentID`, `MaxShipments`, `BuildingTypeID`, `comment`) VALUES
(2, 66, 0, 237027, 0, 0, 6, 'Alliance. Garrison Resources'),
(2, 31, 0, 235704, 0, 0, 16, 'Alliance. Alchemical Catalyst + 236639'),
(2, 53, 0, 236646, 0, 0, 19, 'Alliance. Gearspring Parts + 236647'),
(71, 62, 0, 235773, 0, 0, 21, 'Horde.Taladite Crystal + 237067 + 237068'),
(2, 64, 0, 237181, 0, 0, 23, 'Horde. Hexweave Cloth + 237186 + 237190'),
(71, 74, 0, 0, 0, 0, 26, 'Horde. Overcharged Demolisher'),
(2, 56, 77730, 235886, 110, 12, 1, 'Alliance. Mine Shipment'),
(2, 51, 77775, 236651, 105, 19, 21, 'Alliance. Taladite Crystal + 236652 for lvl2'),
(2, 54, 77777, 236648, 108, 7, 20, 'Alliance. War Paints + 236649'),
(2, 37, 77778, 236420, 72, 7, 23, 'Alliance. Hexweave Cloth + 237665 + 237666'),
(2, 52, 77781, 236642, 106, 7, 18, 'Alliance. Temporal Crystal + 236643 + 236644'),
(2, 49, 77792, 236640, 103, 7, 17, 'Alliance.Truesteel Ingot'),
(2, 50, 78207, 236948, 104, 19, 22, 'Alliance. Burnished Leather'),
(71, 57, 79814, 237913, 121, 7, 16, 'Horde.Alchemical Catalyst'),
(71, 58, 79817, 237123, 123, 12, 17, 'Horde.Truesteel Ingot'),
(71, 59, 79820, 237133, 125, 7, 18, 'Horde.Temporal Crystal'),
(71, 61, 79831, 237063, 129, 7, 20, 'Horde.War Paints'),
(71, 63, 79833, 237102, 133, 7, 22, 'Horde.Burnished Leather'),
(71, 79, 81688, 239237, 177, 14, 1, 'Horde.Mine Shipment'),
(71, 36, 84247, 233832, 71, 19, 4, 'Horde.Lumber Mill Shipment'),
(2, 36, 84248, 233832, 71, 19, 4, 'Alliance. lamber mill'),
(2, 55, 85514, 235885, 109, 12, 2, 'Alliance.Garden'),
(71, 80, 85783, 239238, 178, 14, 2, 'Horde.Garden Shipment'),
(0, 65, 86639, 236721, 137, 7, 25, 'Horde.Conqueror''s Tribute'),
(71, 60, 86696, 237139, 127, 14, 19, 'Horde.Gearspring Parts'),
(71, 70, 87119, 237355, 162, 7, 6, 'Horde.Garrison Resources.Trading Post'),
(71, 32, 88882, 238761, 82, 36, 3, 'Horde.Barn Shipment.Barn Crafter - Fur L1'),
(71, 32, 88883, 238761, 83, 36, 3, 'Horde.Barn Shipment.Barn Crafter - Fur L2'),
(71, 32, 88884, 238761, 85, 36, 3, 'Horde.Barn Shipment.Barn Crafter - Leather L1'),
(71, 32, 88885, 238761, 86, 36, 3, 'Horde.Barn Shipment.Barn Crafter - Leather L2'),
(71, 32, 88886, 238761, 88, 36, 3, 'Horde.Barn Shipment.Barn Crafter - Meat L1'),
(71, 32, 88887, 238761, 89, 36, 3, 'Horde.Barn Shipment.Barn Crafter - Meat L2'),
(71, 32, 88894, 238761, 82, 36, 3, 'Horde.Barn Shipment.Barn Crafter - Fur L1'),
(71, 32, 88895, 238761, 83, 36, 3, 'Horde.Barn Shipment.Barn Crafter - Fur L2'),
(71, 32, 88896, 238761, 85, 36, 3, 'Horde.Barn Shipment.Barn Crafter - Leather L1'),
(71, 32, 88897, 238761, 86, 36, 3, 'Horde.Barn Shipment.Barn Crafter - Leather L2'),
(71, 32, 88898, 238761, 88, 36, 3, 'Horde.Barn Shipment.Barn Crafter - Meat L1'),
(71, 32, 88899, 238761, 89, 36, 3, 'Horde.Barn Shipment.Barn Crafter - Meat L2'),
(71, 77, 89066, 239067, 175, 21, 10, 'Horde.War Mill'),
(71, 100, 91577, 240601, 203, 7, 13, 'Horde. noname.Spirit Lodge'),
(2, 109, 95002, 0, 222, 1, 0, 'Alliance. Ships.');

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `garrison_shipment`
--
ALTER TABLE `garrison_shipment`
  ADD PRIMARY KEY (`NpcEntry`,`BuildingTypeID`) USING BTREE,
  ADD KEY `ContainerID` (`ContainerID`);
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
