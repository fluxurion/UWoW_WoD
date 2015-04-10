-- phpMyAdmin SQL Dump
-- version 4.1.14
-- http://www.phpmyadmin.net
--
-- Хост: 127.0.0.1
-- Время создания: Апр 13 2015 г., 13:34
-- Версия сервера: 10.0.10-MariaDB
-- Версия PHP: 5.5.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- База данных: `wod_char`
--

-- --------------------------------------------------------

--
-- Структура таблицы `character_gameobject`
--

DROP TABLE IF EXISTS `character_loot_cooldown`;
CREATE TABLE `character_loot_cooldown` (
  `guid` bigint(20) unsigned NOT NULL,
  `type` tinyint(3) NOT NULL DEFAULT '0',
  `entry` int(11) NOT NULL DEFAULT '0',
  `respawnTime` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`type`,`entry`),
  KEY `entry` (`entry`),
  KEY `respawnTime` (`respawnTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
