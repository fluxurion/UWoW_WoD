ALTER TABLE `battleground_template`
DROP COLUMN `MinPlayersPerTeam`,
DROP COLUMN `MaxPlayersPerTeam`,
DROP COLUMN `MinLvl`,
DROP COLUMN `MaxLvl`,
DROP COLUMN `AllianceStartO`,
DROP COLUMN `HordeStartO`,
DROP COLUMN `holiday`;

DELETE FROM `gameobject_template` WHERE `entry` IN (228050, 228052, 228053);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `IconName`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `unkInt32`, `VerifiedBuild`) VALUES
(228050, 42, 21202, '', '', 1, 60000, 0, 1, 2178, 45904, 45906, 45908, 45903, 45905, 45907, 8851, 39601, 39602, 39603, 39604, 39606, 39618, 42975, 42976, 42978, 42979, 42980, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20574), -- 228050
(228052, 42, 21202, '', '', 1, 60000, 0, 1, 2178, 45888, 45890, 45892, 45887, 45889, 45891, 8852, 39607, 39608, 39609, 39610, 39611, 39605, 42975, 42976, 42978, 42979, 42980, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20574), -- 228052
(228053, 42, 21202, '', '', 1, 60000, 0, 1, 2178, 45896, 45898, 45900, 45895, 45897, 45899, 8853, 39612, 39613, 39614, 39615, 39616, 39617, 42975, 42976, 42978, 42979, 42980, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20574); -- 228053
