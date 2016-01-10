-- phpMyAdmin SQL Dump
-- version 4.4.14
-- http://www.phpmyadmin.net
--
-- Хост: localhost
-- Время создания: Янв 10 2016 г., 13:34
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
-- Структура таблицы `garrison_plot_finalize_info`
--

DROP TABLE IF EXISTS `garrison_plot_finalize_info`;
CREATE TABLE `garrison_plot_finalize_info` (
  `garrPlotInstanceId` int(10) unsigned NOT NULL,
  `hordeGameObjectId` int(10) unsigned NOT NULL DEFAULT '0',
  `hordeX` float NOT NULL DEFAULT '0',
  `hordeY` float NOT NULL DEFAULT '0',
  `hordeZ` float NOT NULL DEFAULT '0',
  `hordeO` float NOT NULL DEFAULT '0',
  `allianceGameObjectId` int(10) unsigned NOT NULL DEFAULT '0',
  `allianceX` float NOT NULL DEFAULT '0',
  `allianceY` float NOT NULL DEFAULT '0',
  `allianceZ` float NOT NULL DEFAULT '0',
  `allianceO` float NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `garrison_plot_finalize_info`
--

INSERT INTO `garrison_plot_finalize_info` (`garrPlotInstanceId`, `hordeGameObjectId`, `hordeX`, `hordeY`, `hordeZ`, `hordeO`, `allianceGameObjectId`, `allianceX`, `allianceY`, `allianceZ`, `allianceO`) VALUES
(18, 233248, 5642.37, 4514.43, 119.27, 1.88088, 233251, 1822.81, 218.632, 72.0252, 5.06065),
(19, 231964, 5652.18, 4539.69, 119.27, 3.74629, 232652, 1826.32, 209.054, 71.9697, 2.00165),
(20, 236186, 5623.58, 4516.67, 119.27, 0.97567, 236261, 1799.64, 200.113, 70.076, 2.00165),
(22, 233249, 5547.58, 4522.44, 131.393, 0.0108777, 233250, 1866.62, 296.582, 81.6605, 4.79668),
(23, 231217, 5589.08, 4480.11, 130.349, 1.05262, 232651, 1892.78, 240.971, 76.6396, 2.7895),
(24, 236187, 5643.26, 4463.4, 130.525, 1.9379, 236262, 1892.78, 240.971, 76.6396, 2.7895),
(25, 236188, 5679.95, 4486.13, 131.107, 2.4223, 236263, 1874.78, 197.835, 78.0711, 2.54897),
(59, 236176, 5475.8, 4447.15, 144.519, 0.955649, 236191, 1906.68, 92.5087, 83.5267, 0.404931),
(63, 236177, 5424.65, 4567.79, 137.431, 0.112897, 236190, 1853.47, 144.314, 144.314, 1.58967),
(67, 236185, 5476.71, 4617.02, 134.45, 5.02338, 236185, 0, 0, 0, 0),
(81, 236175, 5621.77, 4652.88, 142.448, 4.39222, 236193, 0, 0, 0, 0);

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `garrison_plot_finalize_info`
--
ALTER TABLE `garrison_plot_finalize_info`
  ADD PRIMARY KEY (`garrPlotInstanceId`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
