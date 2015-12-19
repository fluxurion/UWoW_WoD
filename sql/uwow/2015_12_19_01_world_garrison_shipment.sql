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
  `NpcEntry` int(10) unsigned NOT NULL,
  `ConteinerGoEntry` int(10) unsigned NOT NULL,
  `ShipmentID` int(10) unsigned NOT NULL,
  `MaxShipments` int(10) unsigned NOT NULL,
  `BuildingTypeID` int(10) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `garrison_shipment`
--
ALTER TABLE `garrison_shipment`
  ADD PRIMARY KEY (`NpcEntry`,`BuildingTypeID`) USING BTREE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
