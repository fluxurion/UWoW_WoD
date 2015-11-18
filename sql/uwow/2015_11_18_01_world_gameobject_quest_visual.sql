-- phpMyAdmin SQL Dump
-- version 4.4.14
-- http://www.phpmyadmin.net
--
-- Хост: localhost
-- Время создания: Ноя 18 2015 г., 20:13
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
-- Структура таблицы `gameobject_quest_visual`
--

DROP TABLE IF EXISTS `gameobject_quest_visual`;
CREATE TABLE IF NOT EXISTS `gameobject_quest_visual` (
  `goID` int(11) unsigned NOT NULL,
  `questID` int(11) unsigned NOT NULL DEFAULT '0',
  `incomplete_state_spell_visual` int(11) NOT NULL DEFAULT '0',
  `incomplete_state_world_effect` int(11) NOT NULL DEFAULT '0',
  `complete_state_spell_visual` int(11) NOT NULL DEFAULT '0',
  `complete_state_world_effect` int(11) NOT NULL DEFAULT '0',
  `Comment` varchar(128) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=230281 DEFAULT CHARSET=latin1;

--
-- Дамп данных таблицы `gameobject_quest_visual`
--

INSERT INTO `gameobject_quest_visual` (`goID`, `questID`, `incomplete_state_spell_visual`, `incomplete_state_world_effect`, `complete_state_spell_visual`, `complete_state_world_effect`, `Comment`) VALUES
(229057, 34364, 37794, 2099, 40887, 0, ''),
(230280, 34583, 37794, 2099, 40887, 0, '');

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `gameobject_quest_visual`
--
ALTER TABLE `gameobject_quest_visual`
  ADD PRIMARY KEY (`goID`);

--
-- AUTO_INCREMENT для сохранённых таблиц
--

--
-- AUTO_INCREMENT для таблицы `gameobject_quest_visual`
--
ALTER TABLE `gameobject_quest_visual`
  MODIFY `goID` int(11) unsigned NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=230281;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
