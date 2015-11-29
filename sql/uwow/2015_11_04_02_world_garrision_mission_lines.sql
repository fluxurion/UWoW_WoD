-- phpMyAdmin SQL Dump
-- version 4.4.14
-- http://www.phpmyadmin.net
--
-- Хост: localhost
-- Время создания: Ноя 04 2015 г., 17:39
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
-- Структура таблицы `garrison_mission_line`
--

DROP TABLE IF EXISTS `garrison_mission_line`;
CREATE TABLE `garrison_mission_line` (
  `ID` int(11) unsigned NOT NULL,
  `NextMission` int(11) unsigned NOT NULL DEFAULT '0',
  `ReqGarrFollowerID` int(11) unsigned NOT NULL DEFAULT '0',
  `IsRandom` tinyint(1) NOT NULL DEFAULT '0',
  `Comment` varchar(40) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=470 DEFAULT CHARSET=latin1;

--
-- Дамп данных таблицы `garrison_mission_line`
--

INSERT INTO `garrison_mission_line` (`ID`, `NextMission`, `ReqGarrFollowerID`, `IsRandom`, `Comment`) VALUES
(2, 3, 0, 0, 'Horde.'),
(3, 67, 0, 0, 'Horde'),
(44, 6, 0, 0, 'Horde.'),
(67, 7, 0, 0, 'Horde.'),
(87, 44, 153, 0, 'Horde.'),
(469, 0, 0, 1, '');

REPLACE INTO `garrison_mission_line` (`ID`, `NextMission`, `ReqGarrFollowerID`, `IsRandom`, `Comment`) VALUES
('66', '65', '34', '0', 'Alliance'),
('86', '88', '153', '0', 'Alliance'),
('88', '90', '0', '0', 'Alliance'),
('89', '0', '0', '1', 'Alliance'),
('91', '0', '0', '1', 'Alliance'),
('333', '0', '463', '0', 'Alliance'),
('137', '0', '0', '1', 'taken from snif on Alliance'),
('207', '0', '0', '1', 'taken from snif on Alliance'),
('369', '0', '0', '1', 'taken from snif on Alliance'),
('151', '0', '0', '1', 'taken from snif on Alliance'),
('176', '0', '0', '1', 'taken from snif on Alliance'),
('188', '0', '0', '1', 'taken from snif on Alliance'),
('171', '0', '0', '1', 'taken from snif on Alliance'),
('155', '0', '0', '1', 'taken from snif on Alliance');

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `garrison_mission_line`
--
ALTER TABLE `garrison_mission_line`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT для сохранённых таблиц
--

--
-- AUTO_INCREMENT для таблицы `garrison_mission_line`
--
ALTER TABLE `garrison_mission_line`
  MODIFY `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=470;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
