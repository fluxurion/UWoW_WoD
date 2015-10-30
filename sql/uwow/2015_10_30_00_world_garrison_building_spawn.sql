
--
-- Структура таблицы `garrison_building_creature`
--

DROP TABLE IF EXISTS `garrison_building_creature`;
CREATE TABLE `garrison_building_creature` (
  `plotID` mediumint(8) unsigned NOT NULL,
  `BuildID` mediumint(8) unsigned NOT NULL,
  `id` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Creature Identifier',
  `map` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `idx` int(11) NOT NULL
) ENGINE=MyISAM AUTO_INCREMENT=13 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Garrison building spawn System';

--
-- Дамп данных таблицы `garrison_building_creature`
--

INSERT INTO `garrison_building_creature` (`plotID`, `BuildID`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `idx`) VALUES
-- Barrack lvl1. 26 instance.
(23, 26, 79812, 1152, 5576.35, 4460.13, 130.826, 0.95993, 1),
(23, 26, 79781, 1152, 5583.54, 4465.59, 130.779, 3.49343, 2),
(23, 26, 80571, 1152, 5596.58, 4463.71, 130.501, 2.09985, 3),
(23, 26, 80569, 1152, 5586.86, 4453.04, 130.779, 2.1951, 4),
(23, 26, 80571, 1152, 5589.37, 4467.52, 130.779, 5.63951, 5),
(23, 26, 80569, 1152, 5591.84, 4467.31, 130.779, 0.0105276, 6),
(23, 26, 80299, 1152, 5579.26, 4468.18, 130.784, 5.65445, 7),
(23, 26, 80299, 1152, 5581.39, 4466.27, 130.779, 2.17006, 8),
(23, 26, 80569, 1152, 5594.32, 4466.94, 130.779, 3.09207, 9),
(23, 26, 80569, 1152, 5594.29, 4455.23, 130.779, 1.74426, 10),
(23, 26, 80571, 1152, 5575.39, 4479.17, 130.789, 0.189679, 11),
(23, 26, 80572, 1152, 5582.73, 4479.78, 130.779, 0.262985, 12);

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `garrison_building_creature`
--
ALTER TABLE `garrison_building_creature`
  ADD PRIMARY KEY (`idx`);

--
-- AUTO_INCREMENT для сохранённых таблиц
--

--
-- AUTO_INCREMENT для таблицы `garrison_building_creature`
--
ALTER TABLE `garrison_building_creature`
  MODIFY `idx` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=13;

--
-- Структура таблицы `garrison_building_gameobject`
--

DROP TABLE IF EXISTS `garrison_building_gameobject`;
CREATE TABLE IF NOT EXISTS `garrison_building_gameobject` (
  `plotID` mediumint(8) unsigned NOT NULL,
  `BuildID` mediumint(8) unsigned NOT NULL,
  `id` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Gameobject Identifier',
  `map` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `rotation0` float NOT NULL DEFAULT '0',
  `rotation1` float NOT NULL DEFAULT '0',
  `rotation2` float NOT NULL DEFAULT '0',
  `rotation3` float NOT NULL DEFAULT '0',
  `idx` int(11) NOT NULL
) ENGINE=MyISAM AUTO_INCREMENT=11 DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Garrison building spawn System';

--
-- Дамп данных таблицы `garrison_building_gameobject`
--

INSERT INTO `garrison_building_gameobject` (`plotID`, `BuildID`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `idx`) VALUES
-- Horde. Big. empty building
(23, 0, 231012, 1152, 5579.41, 4464.98, 132.137, 0.958846, 0, 0, 0, 1, 1),
-- Barrack lvl1. 26 instance.
(23, 26, 236297, 1152, 5576.58, 4470.37, 130.71, 5.79449, 0, 0, 0.461748, 0.887011, 2),
(23, 26, 236298, 1152, 5577.53, 4464.96, 130.695, 5.16617, 0, 0, 0.461748, 0.887011, 3),
(23, 26, 236299, 1152, 5576.66, 4463.07, 130.757, 0.584684, 0, 0, 0.461748, 0.887011, 4),
(23, 26, 236300, 1152, 5578.8, 4461.31, 130.742, 1.76278, 0, 0, 0.461748, 0.887011, 5),
(23, 26, 238790, 1152, 5566.97, 4461.79, 130.368, 3.2376, 0, 0, 0.461748, 0.887011, 6),
(23, 26, 238791, 1152, 5567.65, 4468.12, 130.368, 2.67908, 0, 0, 0.461748, 0.887011, 7),
(23, 26, 238796, 1152, 5580.14, 4453.97, 132.247, 4.95674, 0, 0, 0.461748, 0.887011, 8),
(23, 26, 238797, 1152, 5568.37, 4450.02, 130.368, 0.261796, 0, 0, 0.461748, 0.887011, 9),
(23, 26, 238798, 1152, 5571.33, 4448.41, 130.368, 2.19038, 0, 0, 0.461748, 0.887011, 10);

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `garrison_building_gameobject`
--
ALTER TABLE `garrison_building_gameobject`
  ADD PRIMARY KEY (`idx`);

--
-- AUTO_INCREMENT для сохранённых таблиц
--

--
-- AUTO_INCREMENT для таблицы `garrison_building_gameobject`
--
ALTER TABLE `garrison_building_gameobject`
  MODIFY `idx` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=11;