-- phpMyAdmin SQL Dump
-- version 4.1.14
-- http://www.phpmyadmin.net
--
-- Хост: 127.0.0.1
-- Время создания: Май 31 2015 г., 21:59
-- Версия сервера: 10.0.10-MariaDB
-- Версия PHP: 5.5.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- База данных: `wod_world`
--

-- --------------------------------------------------------

--
-- Структура таблицы `battlepay_group`
--

DROP TABLE IF EXISTS `battlepay_group`;
CREATE TABLE `battlepay_group` (
  `GroupID` int(11) unsigned NOT NULL DEFAULT '0',
  `IconFileDataID` int(11) unsigned NOT NULL DEFAULT '0',
  `DisplayType` smallint(3) unsigned NOT NULL DEFAULT '0',
  `Ordering` int(11) unsigned NOT NULL DEFAULT '0',
  `Name` varchar(255) CHARACTER SET utf8 NOT NULL,
  PRIMARY KEY (`GroupID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Дамп данных таблицы `battlepay_group`
--

INSERT INTO `battlepay_group` (`GroupID`, `IconFileDataID`, `DisplayType`, `Ordering`, `Name`) VALUES
(13, 939380, 0, 3, 'Питомцы'),
(15, 939379, 0, 4, 'Средства передвижения'),
(17, 939376, 1, 1, 'Рекомендуем'),
(22, 939382, 0, 20, 'Услуги'),
(30, 940869, 1, 19, 'Жетон WoW');

-- --------------------------------------------------------

--
-- Структура таблицы `battlepay_product`
--

DROP TABLE IF EXISTS `battlepay_product`;
CREATE TABLE `battlepay_product` (
  `ProductID` int(11) unsigned NOT NULL,
  `NormalPriceFixedPoint` bigint(20) unsigned NOT NULL DEFAULT '0',
  `CurrentPriceFixedPoint` bigint(20) unsigned NOT NULL DEFAULT '0',
  `ChoiceType` smallint(3) unsigned NOT NULL DEFAULT '0',
  `Type` smallint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` int(11) unsigned NOT NULL DEFAULT '0',
  `dInfo_CreatureDisplayInfoID` int(11) unsigned NOT NULL DEFAULT '0',
  `dInfo_FileDataID` int(11) unsigned NOT NULL DEFAULT '0',
  `dInfo_Name1` varchar(255) CHARACTER SET utf8 DEFAULT NULL,
  `dInfo_Name2` varchar(255) CHARACTER SET utf8 DEFAULT NULL,
  `dInfo_Name3` varchar(500) CHARACTER SET utf8 DEFAULT NULL,
  `dInfo_Flags` int(11) unsigned NOT NULL DEFAULT '0',
  `pItem_ID` int(11) unsigned NOT NULL DEFAULT '0',
  `pItem_ItemID` int(11) unsigned NOT NULL DEFAULT '0',
  `pItem_Quantity` int(11) unsigned NOT NULL DEFAULT '0',
  `pItem_HasPet` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `pItem_PetResult` smallint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ProductID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Дамп данных таблицы `battlepay_product`
--

INSERT INTO `battlepay_product` (`ProductID`, `NormalPriceFixedPoint`, `CurrentPriceFixedPoint`, `ChoiceType`, `Type`, `Flags`, `dInfo_CreatureDisplayInfoID`, `dInfo_FileDataID`, `dInfo_Name1`, `dInfo_Name2`, `dInfo_Name3`, `dInfo_Flags`, `pItem_ID`, `pItem_ItemID`, `pItem_Quantity`, `pItem_HasPet`, `pItem_PetResult`) VALUES
(50, 4500000, 4500000, 2, 0, 47, 30507, 134514, 'Коро''Тышка', NULL, 'Коро''Тышка замораживает противников и весело смеется.', 0, 18, 49693, 1, 0, 0),
(51, 4500000, 4500000, 2, 0, 47, 40019, 574806, 'Душа Аспектов', NULL, 'Неукрощенный, но дружелюбный, этот золотистый дракон будет рассекать небеса вместе с вами.', 0, 19, 78916, 1, 0, 0),
(52, 4500000, 4500000, 2, 0, 47, 37541, 254652, 'Рагнароша', NULL, 'Рагнароша очистит противников вашей команды в пламени!', 0, 20, 68385, 1, 0, 0),
(53, 4500000, 4500000, 2, 0, 47, 32031, 254104, 'Разрушитель XXS-002', NULL, 'Когда Разрушитель XXS-002 хочет поиграть, никто не может отказаться.', 0, 21, 54847, 1, 0, 0),
(54, 4500000, 4500000, 2, 0, 47, 46720, 666530, 'Магматический котенок', NULL, 'Очарование этого магматического котенка обжигает.', 0, 22, 92707, 1, 0, 0),
(55, 4500000, 4500000, 2, 0, 47, 16943, 656241, 'Детеныш кенарийского гиппогрифа', NULL, 'Детеныш кенарийского гиппогрифа поможет вам восстановить природный баланс.', 0, 23, 70099, 1, 0, 0),
(56, 4500000, 4500000, 2, 0, 47, 48933, 874857, 'Цветущее древо', NULL, 'Это цветущее дерево позволит вам взрастить собственную коллекцию!', 0, 24, 98550, 1, 0, 0),
(57, 4500000, 4500000, 2, 0, 47, 30414, 328269, 'Пандарен-монах', NULL, 'Подружиться с лучшими из лучших в Пандарии вам позволит знакомство с этим маленьким, но сильным пандареном-монахом.', 0, 25, 49665, 1, 0, 0),
(58, 10000000, 10000000, 3, 0, 47, 48020, 804461, 'Бронированный кровокрыл', NULL, 'Этот крылатый властитель ночных небес всегда готов посеять панику среди врагов.', 0, 26, 95341, 1, 0, 0),
(59, 10000000, 10000000, 3, 0, 47, 38260, 526356, 'Крылатый страж', NULL, 'Этот огненный лев с орлиными крыльями способен рассеять любую тьму.', 0, 27, 69846, 1, 0, 0),
(60, 10000000, 10000000, 3, 0, 47, 46729, 773250, 'Стремительный ветророг', NULL, 'Скачите по облакам на этом стремительном чуде природы!', 0, 28, 92724, 1, 0, 0),
(61, 10000000, 10000000, 3, 0, 47, 40029, 576138, 'Сердце Аспектов', NULL, 'Тьма рассеется с приходом этого дракона, сияющего изнутри.', 0, 29, 78924, 1, 0, 0),
(62, 10000000, 10000000, 3, 0, 47, 31958, 369225, 'Небесный скакун', NULL, 'Оседлайте этого сверхъестественного скакуна и вознеситесь на нем к небесам!', 0, 30, 54811, 1, 0, 0),
(73, 10000000, 10000000, 3, 0, 47, 53038, 943739, 'Железный небесный разбойник', NULL, 'Одно из самых устрашающих верховых животных Железной Орды.', 0, 32, 107951, 1, 0, 0),
(80, 10000000, 10000000, 3, 0, 47, 48714, 841541, 'Зачарованный волшебный дракончик', NULL, 'Отличитесь в любой ситуации, сидя верхом на этом меняющем цвета драконе.', 0, 37, 97989, 1, 0, 0),
(81, 4500000, 4500000, 2, 0, 47, 51988, 132620, 'Щенок альтеракского мастифа', NULL, 'Покорите высочайшую вершину вместе с вашим верным щенком альтеракского мастифа.', 0, 38, 106240, 1, 0, 0),
(83, 30000000, 30000000, 1, 2, 975, 0, 614740, 'Повышение до 90-го уровня', NULL, 'Повысьте уровень вашего персонажа до 90!', 0, 0, 0, 0, 0, 0),
(104, 12000000, 12000000, 3, 0, 47, 55896, 0, 'Боевой конь кошмаров', NULL, 'Вы и ваши друзья можете смело бросаться в бой верхом на этом достойном восхищения металлическом скакуне.', 0, 91, 112326, 1, 0, 0),
(105, 10000000, 10000000, 3, 0, 47, 55907, 0, 'Оскаленный разоритель', NULL, 'Станьте частью роя верхом на этом оскаленном ужасе.', 0, 92, 112327, 1, 0, 0),
(108, 4500000, 4500000, 2, 0, 47, 61128, 0, 'Арги', NULL, 'Веселая козочка из иного мира.', 0, 95, 118516, 1, 0, 0),
(128, 11000000, 11000000, 5, 0, 4748, 0, 1121394, 'Жетон WoW', 'Жетон WoW', 'Жетон WoW можно продать другим игрокам за золото в аукционном доме. Покупатель впоследствии сможет использовать жетон, чтобы получить 30 дней игрового времени. Этот жетон нельзя обменять или использовать для своей учетной записи – он может быть только выставлен на продажу в аукционном доме.', 0, 107, 122270, 1, 0, 0),
(136, 10000000, 10000000, 3, 0, 47, 62148, 1096090, 'Мистический руноклык', NULL, 'Наполненный тайной магией, этот зверь покрыт мистическими рунами, преобразующими его мощь в крылья из чистой энергии.', 0, 112, 122469, 1, 0, 0);

-- --------------------------------------------------------

--
-- Структура таблицы `battlepay_shopentry`
--

DROP TABLE IF EXISTS `battlepay_shopentry`;
CREATE TABLE `battlepay_shopentry` (
  `EntryID` int(11) unsigned NOT NULL,
  `GroupID` int(11) unsigned NOT NULL,
  `ProductID` int(11) unsigned NOT NULL,
  `Ordering` int(11) NOT NULL DEFAULT '0',
  `Flags` int(11) unsigned NOT NULL DEFAULT '0',
  `BannerType` int(11) unsigned NOT NULL DEFAULT '0',
  `dInfo_CreatureDisplayInfoID` int(11) unsigned NOT NULL DEFAULT '0',
  `dInfo_FileDataID` int(11) unsigned NOT NULL DEFAULT '0',
  `dInfo_Name1` varchar(255) CHARACTER SET utf8 DEFAULT NULL,
  `dInfo_Name2` varchar(255) CHARACTER SET utf8 DEFAULT NULL,
  `dInfo_Name3` varchar(400) CHARACTER SET utf8 DEFAULT NULL,
  `dInfo_Flags` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`EntryID`),
  KEY `with_battlepay_group` (`GroupID`),
  KEY `with_battlepay_product` (`ProductID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Дамп данных таблицы `battlepay_shopentry`
--

INSERT INTO `battlepay_shopentry` (`EntryID`, `GroupID`, `ProductID`, `Ordering`, `Flags`, `BannerType`, `dInfo_CreatureDisplayInfoID`, `dInfo_FileDataID`, `dInfo_Name1`, `dInfo_Name2`, `dInfo_Name3`, `dInfo_Flags`) VALUES
(6, 13, 50, 3, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(7, 13, 51, 4, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(8, 13, 52, 5, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(9, 13, 53, 6, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(10, 13, 54, 7, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(11, 13, 51, 2, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(12, 13, 56, 8, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(13, 13, 57, 9, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(14, 15, 58, 1, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(15, 15, 59, 3, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(16, 15, 60, 15, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(17, 15, 61, 5, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(18, 15, 62, 6, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(25, 15, 73, 0, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(28, 15, 80, 4, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(29, 13, 81, 1, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(40, 22, 83, 0, 0, 2, 0, 614740, 'Повышение до 90-го уровня', NULL, 'На пути каждого героя бывают такие моменты, когда ему нужна помощь в преодолении возникшего препятствия, чтобы он смог вновь вернуться к своим приключениям. Благодаря услуге мгновенного повышения уровня персонажа один из ваших персонажей может немедленно получить 90-й уровень.', 0),
(46, 15, 104, -20, 0, 0, 55896, 0, 'Боевой конь кошмаров', NULL, 'Вы и ваши друзья можете смело бросаться в бой верхом на этом достойном восхищения металлическом скакуне.', 0),
(48, 15, 105, -30, 0, 0, 55907, 0, 'Оскаленный разоритель', NULL, 'Станьте частью роя верхом на этом оскаленном ужасе.', 0),
(50, 13, 108, 0, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(52, 30, 128, 0, 0, 0, 0, 0, NULL, NULL, NULL, 0),
(55, 13, 136, -40, 0, 0, 62148, 1096090, 'Мистический руноклык', NULL, 'Наполненный тайной магией, этот зверь покрыт мистическими рунами, преобразующими его мощь в крылья из чистой энергии.', 0),
(56, 17, 136, 0, 0, 2, 62148, 1096090, 'Мистический руноклык', NULL, 'Чародейская энергия хаоса много тысячелетий скрывала мистического руноклыка от чужих глаз. Лишь недавно пойманный, этот зверь был покрыт магическими рунами, преобразующими его мощь в крылья из чистой энергии!', 0);

--
-- Ограничения внешнего ключа сохраненных таблиц
--

--
-- Ограничения внешнего ключа таблицы `battlepay_shopentry`
--
ALTER TABLE `battlepay_shopentry`
  ADD CONSTRAINT `battlepay_shopentry_ibfk_1` FOREIGN KEY (`GroupID`) REFERENCES `battlepay_group` (`GroupID`),
  ADD CONSTRAINT `battlepay_shopentry_ibfk_2` FOREIGN KEY (`ProductID`) REFERENCES `battlepay_product` (`ProductID`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
