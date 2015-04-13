ALTER TABLE `creature_classlevelstats` DROP `basehp0`;
ALTER TABLE `creature_classlevelstats` DROP `basehp1`;
ALTER TABLE `creature_classlevelstats` DROP `basehp2`;
ALTER TABLE `creature_classlevelstats` DROP `basehp3`;
ALTER TABLE `creature_classlevelstats` DROP `basehp4`;
ALTER TABLE `creature_classlevelstats` DROP `basehp5`;

DELETE FROM `creature_classlevelstats` WHERE `level` BETWEEN 101 AND 105;
INSERT INTO `creature_classlevelstats` (`level`, `class`, `basemana`, `basearmor`) VALUES
(101, 1, 1, 1),
(101, 2, 1, 1),
(101, 4, 1, 1),
(101, 8, 1, 1),
(102, 1, 1, 1),
(102, 2, 1, 1),
(102, 4, 1, 1),
(102, 8, 1, 1),
(103, 1, 1, 1),
(103, 2, 1, 1),
(103, 4, 1, 1),
(103, 8, 1, 1),
(104, 1, 1, 1),
(104, 2, 1, 1),
(104, 4, 1, 1),
(104, 8, 1, 1),
(105, 1, 1, 1),
(105, 2, 1, 1),
(105, 4, 1, 1),
(105, 8, 1, 1);