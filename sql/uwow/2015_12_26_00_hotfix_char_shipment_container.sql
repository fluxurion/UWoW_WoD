-- phpMyAdmin SQL Dump
-- version 4.4.14
-- http://www.phpmyadmin.net
--
-- Хост: localhost
-- Время создания: Дек 26 2015 г., 13:02
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
-- Структура таблицы `char_shipment_container`
--

DROP TABLE IF EXISTS `char_shipment_container`;
CREATE TABLE `char_shipment_container` (
  `ID` int(10) unsigned NOT NULL,
  `Flags` int(10) unsigned NOT NULL,
  `BuildingType` int(10) unsigned NOT NULL,
  `unt3` int(10) unsigned NOT NULL,
  `unk4` int(10) unsigned NOT NULL,
  `unk5` int(10) unsigned NOT NULL,
  `unk6` int(10) unsigned NOT NULL,
  `unk7` int(10) unsigned NOT NULL,
  `MaxShipments` int(10) unsigned NOT NULL,
  `enotherID` int(10) unsigned NOT NULL,
  `fractionType` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `char_shipment_container`
--
ALTER TABLE `char_shipment_container`
  ADD PRIMARY KEY (`ID`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
