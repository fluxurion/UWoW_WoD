-- phpMyAdmin SQL Dump
-- version 4.4.14
-- http://www.phpmyadmin.net
--
-- Хост: localhost
-- Время создания: Дек 19 2015 г., 10:15
-- Версия сервера: 10.0.20-MariaDB
-- Версия PHP: 5.3.8

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- База данных: `wod_hotfixes`
--

-- --------------------------------------------------------

--
-- Структура таблицы `char_shipment`
--

DROP TABLE IF EXISTS `char_shipment`;
CREATE TABLE `char_shipment` (
  `ID` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ShipmentConteinerID` mediumint(8) NOT NULL DEFAULT '0',
  `CriteriaID` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `Flags` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `TimeForShipment` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `SpellCreation` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ShipmentResultItemID` mediumint(8) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `char_shipment`
--
ALTER TABLE `char_shipment`
  ADD PRIMARY KEY (`ID`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
