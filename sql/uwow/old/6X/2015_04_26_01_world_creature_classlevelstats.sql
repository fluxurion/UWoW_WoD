-- 3018ff4e6cbed7e3270df7727d1a04e28a901954
-- b72373f1ee45507c51e275cad185086be2d95655
-- https://github.com/TrinityCore/TrinityCore/commit/a4d03a5ed1fb94e763559b45522196262fdd1b2c

TRUNCATE TABLE `creature_classlevelstats`;

ALTER TABLE creature_classlevelstats ADD attackpower SMALLINT(6) DEFAULT 0 NOT NULL;
ALTER TABLE creature_classlevelstats ADD rangedattackpower SMALLINT(6) DEFAULT 0 NOT NULL;
ALTER TABLE creature_classlevelstats ADD damage_base FLOAT DEFAULT 0 NOT NULL;
ALTER TABLE creature_classlevelstats ADD damage_exp1 FLOAT DEFAULT 0 NOT NULL;
ALTER TABLE creature_classlevelstats ADD damage_exp2 FLOAT DEFAULT 0 NOT NULL;
ALTER TABLE creature_classlevelstats ADD damage_exp3 FLOAT DEFAULT 0 NOT NULL;
ALTER TABLE creature_classlevelstats ADD COLUMN damage_exp4 FLOAT NOT NULL DEFAULT 0 AFTER damage_exp3;
ALTER TABLE creature_classlevelstats ADD COLUMN damage_exp5 FLOAT NOT NULL DEFAULT 0 AFTER damage_exp4;
ALTER TABLE creature_classlevelstats ADD `comment` BLOB;

INSERT INTO `creature_classlevelstats` (`level`, `class`, `basemana`, `basearmor`, `attackpower`, `rangedattackpower`, `damage_base`, `damage_exp1`, `damage_exp2`, `damage_exp3`, `damage_exp4`, `damage_exp5`, `comment`) VALUES
(1, 1, 0, 15, 10, 1, 0.0607, 0, 0, 0, 0, 0, NULL),
(2, 1, 0, 16, 10, 1, 0.3603, 0, 0, 0, 0, 0, NULL),
(3, 1, 0, 41, 11, 1, 0.6976, 0, 0, 0, 0, 0, NULL),
(4, 1, 0, 76, 13, 1, 1.0863, 0, 0, 0, 0, 0, NULL),
(5, 1, 0, 120, 17, 1, 1.4422, 0, 0, 0, 0, 0, NULL),
(6, 1, 0, 174, 19, 1, 2.1375, 0, 0, 0, 0, 0, NULL),
(7, 1, 0, 239, 21, 1, 2.9811, 0, 0, 0, 0, 0, NULL),
(8, 1, 0, 316, 24, 1, 3.485, 0, 0, 0, 0, 0, NULL),
(9, 1, 0, 406, 28, 1, 3.0304, 0, 0, 0, 0, 0, NULL),
(10, 1, 0, 512, 32, 1, 3.2707, 0, 0, 0, 0, 0, NULL),
(11, 1, 0, 538, 36, 1, 4.8425, 0, 0, 0, 0, 0, NULL),
(12, 1, 0, 573, 40, 1, 5.603, 0, 0, 0, 0, 0, NULL),
(13, 1, 0, 608, 44, 1, 6.0787, 0, 0, 0, 0, 0, NULL),
(14, 1, 0, 642, 50, 1, 6.6101, 0, 0, 0, 0, 0, NULL),
(15, 1, 0, 677, 54, 2, 7.1981, 0, 0, 0, 0, 0, NULL),
(16, 1, 0, 713, 56, 2, 7.7861, 0, 0, 0, 0, 0, NULL),
(17, 1, 0, 748, 60, 2, 8.3741, 0, 0, 0, 0, 0, NULL),
(18, 1, 0, 782, 64, 3, 8.9621, 0, 0, 0, 0, 0, NULL),
(19, 1, 0, 817, 68, 3, 9.5501, 0, 0, 0, 0, 0, NULL),
(20, 1, 0, 852, 70, 4, 10.1381, 0, 0, 0, 0, 0, NULL),
(21, 1, 0, 888, 74, 4, 10.7261, 0, 0, 0, 0, 0, NULL),
(22, 1, 0, 922, 78, 4, 11.3141, 0, 0, 0, 0, 0, NULL),
(23, 1, 0, 957, 80, 5, 11.9021, 0, 0, 0, 0, 0, NULL),
(24, 1, 0, 992, 84, 6, 12.4901, 0, 0, 0, 0, 0, NULL),
(25, 1, 0, 1026, 86, 6, 13.0781, 0, 0, 0, 0, 0, NULL),
(26, 1, 0, 1061, 90, 7, 13.6661, 0, 0, 0, 0, 0, NULL),
(27, 1, 0, 1097, 92, 7, 14.2541, 0, 0, 0, 0, 0, NULL),
(28, 1, 0, 1130, 96, 8, 14.8421, 0, 0, 0, 0, 0, NULL),
(29, 1, 0, 1165, 100, 8, 15.4301, 0, 0, 0, 0, 0, NULL),
(30, 1, 0, 1200, 102, 9, 16.0181, 0, 0, 0, 0, 0, NULL),
(31, 1, 0, 1234, 106, 9, 16.6061, 0, 0, 0, 0, 0, NULL),
(32, 1, 0, 1270, 108, 10, 17.1941, 0, 0, 0, 0, 0, NULL),
(33, 1, 0, 1304, 112, 10, 17.7821, 0, 0, 0, 0, 0, NULL),
(34, 1, 0, 1340, 114, 11, 18.3701, 0, 0, 0, 0, 0, NULL),
(35, 1, 0, 1373, 118, 11, 18.9581, 0, 0, 0, 0, 0, NULL),
(36, 1, 0, 1480, 120, 12, 19.5461, 0, 0, 0, 0, 0, NULL),
(37, 1, 0, 1593, 124, 12, 20.1341, 0, 0, 0, 0, 0, NULL),
(38, 1, 0, 1709, 128, 13, 20.7221, 0, 0, 0, 0, 0, NULL),
(39, 1, 0, 1834, 132, 13, 21.3101, 0, 0, 0, 0, 0, NULL),
(40, 1, 0, 1964, 136, 14, 21.8981, 0, 0, 0, 0, 0, NULL),
(41, 1, 0, 2101, 142, 15, 22.4861, 0, 0, 0, 0, 0, NULL),
(42, 1, 0, 2246, 152, 15, 23.0741, 0, 0, 0, 0, 0, NULL),
(43, 1, 0, 2397, 162, 16, 23.6621, 0, 0, 0, 0, 0, NULL),
(44, 1, 0, 2557, 174, 16, 24.2501, 0, 0, 0, 0, 0, NULL),
(45, 1, 0, 2725, 184, 17, 24.8381, 0, 0, 0, 0, 0, NULL),
(46, 1, 0, 2780, 188, 17, 25.4261, 0, 0, 0, 0, 0, NULL),
(47, 1, 0, 2835, 192, 18, 26.0141, 0, 0, 0, 0, 0, NULL),
(48, 1, 0, 2888, 196, 19, 26.6021, 0, 0, 0, 0, 0, NULL),
(49, 1, 0, 2944, 200, 19, 27.1901, 0, 0, 0, 0, 0, NULL),
(50, 1, 0, 2999, 206, 20, 27.7781, 0, 0, 0, 0, 0, NULL),
(51, 1, 0, 3052, 210, 20, 28.3661, 0, 0, 0, 0, 0, NULL),
(52, 1, 0, 3108, 214, 21, 28.9541, 0, 0, 0, 0, 0, NULL),
(53, 1, 0, 3163, 218, 22, 29.5421, 0, 0, 0, 0, 0, NULL),
(54, 1, 0, 3216, 224, 22, 30.1301, 0, 0, 0, 0, 0, NULL),
(55, 1, 0, 3271, 228, 23, 30.7177, 0, 0, 0, 0, 0, NULL),
(56, 1, 0, 3327, 234, 23, 31.3057, 0, 0, 0, 0, 0, NULL),
(57, 1, 0, 3380, 238, 24, 31.8937, 42.1353, 0, 0, 0, 0, NULL),
(58, 1, 0, 3435, 242, 25, 32.4817, 46.9207, 0, 0, 0, 0, NULL),
(59, 1, 0, 3489, 248, 25, 33.0697, 51.7061, 0, 0, 0, 0, NULL),
(60, 1, 0, 3791, 252, 26, 33.6577, 56.4915, 0, 0, 0, 0, NULL),
(61, 1, 0, 4091, 258, 28, 34.2457, 61.2769, 0, 0, 0, 0, NULL),
(62, 1, 0, 4391, 262, 30, 34.8337, 66.0623, 0, 0, 0, 0, NULL),
(63, 1, 0, 4691, 268, 32, 35.4217, 70.8477, 0, 0, 0, 0, NULL),
(64, 1, 0, 4991, 272, 33, 36.0097, 75.6331, 0, 0, 0, 0, NULL),
(65, 1, 0, 5291, 278, 35, 36.5977, 80.4185, 0, 0, 0, 0, NULL),
(66, 1, 0, 5591, 282, 37, 37.1857, 85.2039, 0, 0, 0, 0, NULL),
(67, 1, 0, 5892, 288, 39, 37.7737, 89.9893, 111.174, 0, 0, 0, NULL),
(68, 1, 0, 6192, 292, 41, 38.3617, 94.7747, 115.224, 0, 0, 0, NULL),
(69, 1, 0, 6492, 298, 43, 38.9497, 99.5601, 119.274, 0, 0, 0, NULL),
(70, 1, 0, 6792, 304, 44, 39.5377, 104.346, 123.324, 0, 0, 0, NULL),
(71, 1, 0, 7089, 308, 48, 40.1257, 109.131, 127.374, 0, 0, 0, NULL),
(72, 1, 0, 7387, 314, 53, 40.7137, 113.916, 131.424, 0, 0, 0, NULL),
(73, 1, 0, 7684, 320, 58, 41.3017, 118.702, 135.474, 0, 0, 0, NULL),
(74, 1, 0, 7918, 354, 63, 41.8897, 0, 139.524, 0, 0, 0, NULL),
(75, 1, 0, 8219, 392, 68, 42.4777, 0, 143.574, 0, 0, 0, NULL),
(76, 1, 0, 8520, 432, 74, 43.0657, 0, 147.624, 0, 0, 0, NULL),
(77, 1, 0, 8822, 478, 81, 43.6537, 0, 151.674, 0, 0, 0, NULL),
(78, 1, 0, 9124, 528, 88, 44.2417, 0, 155.724, 0, 0, 0, NULL),
(79, 1, 0, 9426, 582, 95, 44.8297, 0, 160.258, 0, 0, 0, NULL),
(80, 1, 0, 9729, 642, 103, 45.4177, 0, 164.924, 0, 0, 0, NULL),
(81, 1, 0, 10033, 708, 112, 46.0057, 0, 168.974, 0, 0, 0, NULL),
(82, 1, 0, 10356, 782, 121, 46.5937, 0, 173.024, 0, 0, 0, NULL),
(83, 1, 0, 10673, 805, 135, 47.1817, 0, 177.074, 0, 0, 0, NULL),
(84, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(85, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(86, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(87, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(88, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(89, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(90, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(91, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(92, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(93, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(94, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(95, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(96, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(97, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(98, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(99, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(100, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(1, 2, 60, 13, 1, 1, 0.746, 0, 0, 0, 0, 0, 'armor estimated'),
(2, 2, 69, 14, 3, 1, 0.7365, 0, 0, 0, 0, 0, 'armor estimated'),
(3, 2, 79, 37, 7, 1, 0.6984, 0, 0, 0, 0, 0, NULL),
(4, 2, 104, 67, 18, 1, 1.2138, 0, 0, 0, 0, 0, NULL),
(5, 2, 115, 105, 19, 1, 1.8471, 0, 0, 0, 0, 0, NULL),
(6, 2, 126, 150, 24, 1, 2.6006, 0, 0, 0, 0, 0, NULL),
(7, 2, 138, 205, 28, 1, 3.5412, 0, 0, 0, 0, 0, NULL),
(8, 2, 165, 268, 32, 1, 4.1435, 0, 0, 0, 0, 0, NULL),
(9, 2, 178, 342, 36, 1, 4.7933, 0, 0, 0, 0, 0, NULL),
(10, 2, 191, 426, 38, 1, 5.066, 0, 0, 0, 0, 0, NULL),
(11, 2, 205, 450, 40, 1, 5.3387, 0, 0, 0, 0, 0, NULL),
(12, 2, 249, 479, 42, 1, 5.6114, 0, 0, 0, 0, 0, NULL),
(13, 2, 264, 507, 44, 1, 5.8841, 0, 0, 0, 0, 0, NULL),
(14, 2, 295, 537, 46, 1, 6.1568, 0, 0, 0, 0, 0, NULL),
(15, 2, 326, 566, 48, 2, 6.8211, 0, 0, 0, 0, 0, NULL),
(16, 2, 357, 596, 52, 2, 7.1374, 0, 0, 0, 0, 0, NULL),
(17, 2, 390, 625, 56, 2, 7.6355, 0, 0, 0, 0, 0, NULL),
(18, 2, 408, 652, 58, 3, 8.1353, 0, 0, 0, 0, 0, NULL),
(19, 2, 456, 683, 60, 3, 8.6351, 0, 0, 0, 0, 0, NULL),
(20, 2, 490, 712, 64, 4, 9.0674, 0, 0, 0, 0, 0, NULL),
(21, 2, 510, 740, 68, 4, 9.3169, 0, 0, 0, 0, 0, NULL),
(22, 2, 545, 770, 72, 5, 9.7231, 0, 0, 0, 0, 0, NULL),
(23, 2, 581, 800, 74, 5, 10.0761, 0, 0, 0, 0, 0, NULL),
(24, 2, 618, 829, 78, 6, 10.4619, 0, 0, 0, 0, 0, NULL),
(25, 2, 655, 858, 80, 6, 10.8234, 0, 0, 0, 0, 0, NULL),
(26, 2, 693, 887, 84, 6, 11.185, 0, 0, 0, 0, 0, NULL),
(27, 2, 732, 916, 86, 7, 11.7126, 0, 0, 0, 0, 0, NULL),
(28, 2, 756, 946, 90, 7, 11.9663, 0, 0, 0, 0, 0, NULL),
(29, 2, 811, 975, 94, 8, 12.352, 0, 0, 0, 0, 0, NULL),
(30, 2, 852, 1004, 94, 8, 13.0007, 0, 0, 0, 0, 0, NULL),
(31, 2, 878, 1033, 98, 9, 13.2348, 0, 0, 0, 0, 0, NULL),
(32, 2, 935, 1063, 100, 9, 13.6924, 0, 0, 0, 0, 0, NULL),
(33, 2, 963, 1091, 104, 10, 13.9511, 0, 0, 0, 0, 0, NULL),
(34, 2, 1007, 1121, 106, 10, 14.4142, 0, 0, 0, 0, 0, NULL),
(35, 2, 1067, 1149, 110, 11, 14.7365, 0, 0, 0, 0, 0, NULL),
(36, 2, 1097, 1234, 112, 11, 15.1346, 0, 0, 0, 0, 0, NULL),
(37, 2, 1142, 1325, 116, 11, 15.5816, 0, 0, 0, 0, 0, NULL),
(38, 2, 1189, 1418, 120, 12, 15.9286, 0, 0, 0, 0, 0, NULL),
(39, 2, 1236, 1517, 124, 12, 16.4168, 0, 0, 0, 0, 0, NULL),
(40, 2, 1283, 1619, 128, 13, 16.9294, 0, 0, 0, 0, 0, NULL),
(41, 2, 1332, 1727, 132, 13, 17.8592, 0, 0, 0, 0, 0, NULL),
(42, 2, 1381, 1841, 142, 15, 18.8825, 0, 0, 0, 0, 0, NULL),
(43, 2, 1432, 1958, 152, 14, 20.1126, 0, 0, 0, 0, 0, NULL),
(44, 2, 1483, 2082, 166, 15, 21.6981, 0, 0, 0, 0, 0, NULL),
(45, 2, 1534, 2211, 172, 15, 22.9018, 0, 0, 0, 0, 0, NULL),
(46, 2, 1587, 2255, 176, 16, 23.345, 0, 0, 0, 0, 0, NULL),
(47, 2, 1640, 2300, 180, 16, 23.9542, 0, 0, 0, 0, 0, NULL),
(48, 2, 1695, 2344, 184, 17, 24.401, 0, 0, 0, 0, 0, NULL),
(49, 2, 1750, 2389, 188, 17, 24.9975, 0, 0, 0, 0, 0, NULL),
(50, 2, 1807, 2432, 194, 18, 25.5204, 0, 0, 0, 0, 0, NULL),
(51, 2, 1864, 2477, 198, 19, 26.0047, 0, 0, 0, 0, 0, NULL),
(52, 2, 1923, 2522, 202, 19, 26.6877, 0, 0, 0, 0, 0, NULL),
(53, 2, 1982, 2566, 206, 20, 27.1912, 0, 0, 0, 0, 0, NULL),
(54, 2, 2041, 2610, 210, 20, 27.8506, 0, 0, 0, 0, 0, NULL),
(55, 2, 2117, 2654, 214, 21, 28.3834, 0, 0, 0, 0, 0, NULL),
(56, 2, 2163, 2699, 220, 21, 28.9161, 0, 0, 0, 0, 0, NULL),
(57, 2, 2241, 2744, 224, 22, 29.5702, 41.2575, 0, 0, 0, 0, NULL),
(58, 2, 2289, 2788, 228, 23, 30.1469, 45.5251, 0, 0, 0, 0, NULL),
(59, 2, 2369, 2832, 234, 23, 30.7656, 49.7927, 0, 0, 0, 0, NULL),
(60, 2, 2434, 3075, 238, 24, 31.3842, 54.0603, 0, 0, 0, 0, NULL),
(61, 2, 2486, 3316, 244, 25, 31.9807, 58.3279, 0, 0, 0, 0, NULL),
(62, 2, 2568, 3555, 246, 26, 32.5772, 62.4676, 0, 0, 0, 0, NULL),
(63, 2, 2620, 3795, 252, 26, 33.1737, 66.121, 0, 0, 0, 0, NULL),
(64, 2, 2705, 4036, 256, 27, 33.7702, 70.2568, 0, 0, 0, 0, NULL),
(65, 2, 2790, 4275, 262, 29, 34.3667, 74.3252, 0, 0, 0, 0, NULL),
(66, 2, 2846, 4515, 266, 29, 34.9632, 78.472, 0, 0, 0, 0, NULL),
(67, 2, 2933, 4755, 272, 31, 35.5597, 82.7944, 110.698, 0, 0, 0, NULL),
(68, 2, 2991, 4995, 276, 31, 36.1562, 87.2676, 114.869, 0, 0, 0, NULL),
(69, 2, 3080, 5235, 282, 32, 36.7527, 91.8915, 119.04, 0, 0, 0, NULL),
(70, 2, 3155, 5474, 286, 33, 37.3492, 96.7365, 123.212, 0, 0, 0, NULL),
(71, 2, 3231, 5714, 290, 37, 37.9457, 101.004, 127.383, 0, 0, 0, NULL),
(72, 2, 3309, 5954, 296, 40, 38.5422, 105.272, 131.092, 0, 0, 0, NULL),
(73, 2, 3387, 6193, 302, 44, 39.1387, 109.539, 134.908, 0, 0, 0, NULL),
(74, 2, 3466, 7903, 334, 48, 39.7352, 0, 138.836, 0, 0, 0, NULL),
(75, 2, 3561, 8204, 370, 53, 40.3317, 0, 142.878, 0, 0, 0, NULL),
(76, 2, 3643, 8503, 408, 57, 40.9282, 0, 147.038, 0, 0, 0, NULL),
(77, 2, 3725, 8803, 452, 62, 41.5247, 0, 151.319, 0, 0, 0, NULL),
(78, 2, 3809, 9104, 500, 68, 42.1212, 0, 155.724, 0, 0, 0, NULL),
(79, 2, 3893, 9405, 550, 74, 42.7177, 0, 160.258, 0, 0, 0, NULL),
(80, 2, 3994, 9706, 608, 80, 43.3142, 0, 164.924, 0, 0, 0, NULL),
(81, 2, 4081, 10007, 670, 86, 43.9107, 0, 169.095, 0, 0, 0, NULL),
(82, 2, 4169, 10253, 726, 91, 44.5072, 0, 173.266, 0, 0, 0, NULL),
(83, 2, 4258, 10573, 783, 98, 45.1037, 0, 177.438, 0, 0, 0, NULL),
(84, 2, 4363, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(85, 2, 4454, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(86, 2, 4502, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(87, 2, 4655, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(88, 2, 4735, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(89, 2, 5502, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(90, 2, 5502, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(91, 2, 5502, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(92, 2, 5502, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(93, 2, 5502, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(94, 2, 5502, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(95, 2, 5502, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(96, 2, 5502, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(97, 2, 5502, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(98, 2, 5502, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(99, 2, 5502, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(100, 2, 5502, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(1, 4, 0, 6, 24, 1, 0.25, 0, 0, 0, 0, 0, NULL),
(2, 4, 0, 18, 26, 1, 0.54, 0, 0, 0, 0, 0, NULL),
(3, 4, 0, 31, 28, 1, 0.74, 0, 0, 0, 0, 0, NULL),
(4, 4, 0, 63, 30, 1, 0.9435, 0, 0, 0, 0, 0, NULL),
(5, 4, 0, 102, 32, 1, 1.5, 0, 0, 0, 0, 0, NULL),
(6, 4, 0, 152, 34, 1, 2.2, 0, 0, 0, 0, 0, NULL),
(7, 4, 0, 212, 36, 1, 3, 0, 0, 0, 0, 0, NULL),
(8, 4, 0, 286, 38, 1, 4.3597, 0, 0, 0, 0, 0, NULL),
(9, 4, 0, 363, 40, 1, 4.6993, 0, 0, 0, 0, 0, NULL),
(10, 4, 0, 443, 42, 1, 4.8281, 0, 0, 0, 0, 0, NULL),
(11, 4, 0, 488, 44, 1, 5.1609, 0, 0, 0, 0, 0, NULL),
(12, 4, 0, 519, 46, 1, 5.6558, 0, 0, 0, 0, 0, NULL),
(13, 4, 0, 553, 48, 1, 6.0221, 0, 0, 0, 0, 0, NULL),
(14, 4, 0, 577, 50, 1, 6.6101, 0, 0, 0, 0, 0, NULL),
(15, 4, 0, 612, 54, 2, 7.1981, 0, 0, 0, 0, 0, NULL),
(16, 4, 0, 645, 56, 2, 7.7861, 0, 0, 0, 0, 0, NULL),
(17, 4, 0, 676, 60, 2, 8.3741, 0, 0, 0, 0, 0, NULL),
(18, 4, 0, 706, 64, 3, 8.9621, 0, 0, 0, 0, 0, NULL),
(19, 4, 0, 738, 68, 3, 9.5501, 0, 0, 0, 0, 0, NULL),
(20, 4, 0, 769, 70, 4, 10.1381, 0, 0, 0, 0, 0, NULL),
(21, 4, 0, 801, 74, 4, 10.7261, 0, 0, 0, 0, 0, NULL),
(22, 4, 0, 833, 78, 4, 11.3141, 0, 0, 0, 0, 0, NULL),
(23, 4, 0, 863, 80, 5, 11.9021, 0, 0, 0, 0, 0, NULL),
(24, 4, 0, 895, 84, 6, 12.4901, 0, 0, 0, 0, 0, NULL),
(25, 4, 0, 926, 86, 6, 13.0781, 0, 0, 0, 0, 0, NULL),
(26, 4, 0, 957, 90, 7, 13.6661, 0, 0, 0, 0, 0, NULL),
(27, 4, 0, 989, 92, 7, 14.2541, 0, 0, 0, 0, 0, NULL),
(28, 4, 0, 1020, 96, 8, 14.8421, 0, 0, 0, 0, 0, NULL),
(29, 4, 0, 1051, 100, 8, 15.4301, 0, 0, 0, 0, 0, NULL),
(30, 4, 0, 1082, 102, 9, 16.0181, 0, 0, 0, 0, 0, NULL),
(31, 4, 0, 1113, 106, 9, 16.6061, 0, 0, 0, 0, 0, NULL),
(32, 4, 0, 1146, 108, 10, 17.1941, 0, 0, 0, 0, 0, NULL),
(33, 4, 0, 1173, 112, 10, 17.7821, 0, 0, 0, 0, 0, NULL),
(34, 4, 0, 1208, 114, 11, 18.3701, 0, 0, 0, 0, 0, NULL),
(35, 4, 0, 1237, 118, 11, 18.9581, 0, 0, 0, 0, 0, NULL),
(36, 4, 0, 1349, 120, 12, 19.5461, 0, 0, 0, 0, 0, NULL),
(37, 4, 0, 1434, 124, 12, 20.1341, 0, 0, 0, 0, 0, NULL),
(38, 4, 0, 1538, 128, 13, 20.7221, 0, 0, 0, 0, 0, NULL),
(39, 4, 0, 1649, 132, 13, 21.3101, 0, 0, 0, 0, 0, NULL),
(40, 4, 0, 1764, 136, 14, 21.8981, 0, 0, 0, 0, 0, NULL),
(41, 4, 0, 1886, 142, 15, 22.4861, 0, 0, 0, 0, 0, NULL),
(42, 4, 0, 2015, 152, 15, 23.0741, 0, 0, 0, 0, 0, NULL),
(43, 4, 0, 2148, 162, 16, 23.6621, 0, 0, 0, 0, 0, NULL),
(44, 4, 0, 2303, 174, 16, 24.2501, 0, 0, 0, 0, 0, NULL),
(45, 4, 0, 2436, 184, 17, 24.8381, 0, 0, 0, 0, 0, NULL),
(46, 4, 0, 2485, 188, 17, 25.4261, 0, 0, 0, 0, 0, NULL),
(47, 4, 0, 2535, 192, 18, 26.0141, 0, 0, 0, 0, 0, NULL),
(48, 4, 0, 2582, 196, 19, 26.6021, 0, 0, 0, 0, 0, NULL),
(49, 4, 0, 2631, 200, 19, 27.1901, 0, 0, 0, 0, 0, NULL),
(50, 4, 0, 2680, 206, 20, 27.7781, 0, 0, 0, 0, 0, NULL),
(51, 4, 0, 2728, 210, 20, 28.3661, 0, 0, 0, 0, 0, NULL),
(52, 4, 0, 2778, 214, 21, 28.9541, 0, 0, 0, 0, 0, NULL),
(53, 4, 0, 2826, 218, 22, 29.5421, 0, 0, 0, 0, 0, NULL),
(54, 4, 0, 2874, 224, 22, 30.1301, 0, 0, 0, 0, 0, NULL),
(55, 4, 0, 2922, 228, 23, 30.7177, 0, 0, 0, 0, 0, NULL),
(56, 4, 0, 2972, 234, 23, 31.3057, 0, 0, 0, 0, 0, NULL),
(57, 4, 0, 3020, 238, 24, 31.8937, 42.1353, 0, 0, 0, 0, NULL),
(58, 4, 0, 3068, 242, 25, 32.4817, 46.9207, 0, 0, 0, 0, NULL),
(59, 4, 0, 3117, 248, 25, 33.0697, 51.7061, 0, 0, 0, 0, NULL),
(60, 4, 0, 3388, 252, 26, 33.6577, 56.4915, 0, 0, 0, 0, NULL),
(61, 4, 0, 3655, 258, 28, 34.2457, 61.2769, 0, 0, 0, 0, NULL),
(62, 4, 0, 3922, 262, 30, 34.8337, 66.0623, 0, 0, 0, 0, NULL),
(63, 4, 0, 4189, 268, 32, 35.4217, 70.8477, 0, 0, 0, 0, NULL),
(64, 4, 0, 4457, 272, 33, 36.0097, 75.6331, 0, 0, 0, 0, NULL),
(65, 4, 0, 4724, 278, 35, 36.5977, 80.4185, 0, 0, 0, 0, NULL),
(66, 4, 0, 5104, 282, 37, 37.1857, 85.2039, 0, 0, 0, 0, NULL),
(67, 4, 0, 5326, 288, 39, 37.7737, 89.9893, 111.174, 0, 0, 0, NULL),
(68, 4, 0, 5527, 292, 41, 38.3617, 94.7747, 115.224, 0, 0, 0, NULL),
(69, 4, 0, 5795, 298, 43, 38.9497, 99.5601, 119.274, 0, 0, 0, NULL),
(70, 4, 0, 6062, 304, 44, 39.5377, 104.346, 123.324, 0, 0, 0, NULL),
(71, 4, 0, 6332, 308, 48, 40.1257, 109.131, 127.374, 0, 0, 0, NULL),
(72, 4, 0, 6602, 314, 53, 40.7137, 113.916, 131.424, 0, 0, 0, NULL),
(73, 4, 0, 6872, 320, 58, 41.3017, 118.702, 135.474, 0, 0, 0, NULL),
(74, 4, 0, 7143, 354, 63, 41.8897, 0, 139.524, 0, 0, 0, NULL),
(75, 4, 0, 7415, 392, 68, 42.4777, 0, 143.574, 0, 0, 0, NULL),
(76, 4, 0, 7686, 432, 74, 43.0657, 0, 147.624, 0, 0, 0, NULL),
(77, 4, 0, 7958, 478, 81, 43.6537, 0, 151.674, 0, 0, 0, NULL),
(78, 4, 0, 8230, 528, 88, 44.2417, 0, 155.724, 0, 0, 0, NULL),
(79, 4, 0, 8503, 582, 95, 44.8297, 0, 160.258, 0, 0, 0, NULL),
(80, 4, 0, 8776, 642, 103, 45.4177, 0, 164.924, 0, 0, 0, NULL),
(81, 4, 0, 9068, 708, 112, 46.0057, 0, 168.974, 0, 0, 0, NULL),
(82, 4, 0, 9348, 782, 121, 46.5937, 0, 173.024, 0, 0, 0, NULL),
(83, 4, 0, 9589, 805, 135, 47.1817, 0, 177.074, 0, 0, 0, NULL),
(84, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(85, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(86, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(87, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(88, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(89, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(90, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(91, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(92, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(93, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(94, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(95, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(96, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(97, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(98, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(99, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(100, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(1, 8, 120, 5, 1, 10, 0.4564, 0, 0, 0, 0, 0, NULL),
(2, 8, 147, 16, 3, 10, 0.4564, 0, 0, 0, 0, 0, NULL),
(3, 8, 174, 28, 7, 10, 0.7334, 0, 0, 0, 0, 0, NULL),
(4, 8, 202, 57, 8, 11, 1.1235, 0, 0, 0, 0, 0, NULL),
(5, 8, 230, 93, 9, 11, 1.3456, 0, 0, 0, 0, 0, NULL),
(6, 8, 259, 139, 10, 11, 1.9557, 0, 0, 0, 0, 0, NULL),
(7, 8, 289, 194, 13, 11, 2.7876, 0, 0, 0, 0, 0, NULL),
(8, 8, 319, 265, 15, 11, 3.2334, 0, 0, 0, 0, 0, NULL),
(9, 8, 350, 339, 16, 11, 3.7654, 0, 0, 0, 0, 0, NULL),
(10, 8, 382, 423, 17, 12, 4.2456, 0, 0, 0, 0, 0, NULL),
(11, 8, 459, 447, 17, 12, 4.7654, 0, 0, 0, 0, 0, NULL),
(12, 8, 537, 475, 18, 12, 5.0345, 0, 0, 0, 0, 0, NULL),
(13, 8, 601, 509, 19, 12, 5.199, 0, 0, 0, 0, 0, NULL),
(14, 8, 710, 523, 21, 12, 5.7241, 0, 0, 0, 0, 0, NULL),
(15, 8, 790, 559, 22, 13, 6.3298, 0, 0, 0, 0, 0, NULL),
(16, 8, 856, 589, 24, 13, 6.6051, 0, 0, 0, 0, 0, NULL),
(17, 8, 938, 617, 26, 13, 7.0485, 0, 0, 0, 0, 0, NULL),
(18, 8, 1020, 643, 27, 13, 7.4083, 0, 0, 0, 0, 0, NULL),
(19, 8, 1118, 674, 28, 13, 7.9334, 0, 0, 0, 0, 0, NULL),
(20, 8, 1202, 701, 30, 14, 8.3382, 0, 0, 0, 0, 0, NULL),
(21, 8, 1272, 729, 32, 14, 8.5517, 0, 0, 0, 0, 0, NULL),
(22, 8, 1357, 759, 34, 14, 8.9102, 0, 0, 0, 0, 0, NULL),
(23, 8, 1443, 786, 35, 14, 9.228, 0, 0, 0, 0, 0, NULL),
(24, 8, 1545, 815, 36, 15, 9.6818, 0, 0, 0, 0, 0, NULL),
(25, 8, 1633, 843, 37, 15, 10.1014, 0, 0, 0, 0, 0, NULL),
(26, 8, 1707, 871, 39, 15, 10.3246, 0, 0, 0, 0, 0, NULL),
(27, 8, 1812, 900, 40, 15, 10.804, 0, 0, 0, 0, 0, NULL),
(28, 8, 1977, 928, 42, 15, 11.0214, 0, 0, 0, 0, 0, NULL),
(29, 8, 2068, 957, 44, 16, 11.3608, 0, 0, 0, 0, 0, NULL),
(30, 8, 2175, 984, 44, 16, 11.9607, 0, 0, 0, 0, 0, NULL),
(31, 8, 2253, 1012, 46, 16, 12.16, 0, 0, 0, 0, 0, NULL),
(32, 8, 2362, 1042, 47, 16, 12.5746, 0, 0, 0, 0, 0, NULL),
(33, 8, 2457, 1065, 49, 17, 12.7966, 0, 0, 0, 0, 0, NULL),
(34, 8, 2553, 1098, 50, 17, 13.2162, 0, 0, 0, 0, 0, NULL),
(35, 8, 2680, 1124, 52, 17, 13.497, 0, 0, 0, 0, 0, NULL),
(36, 8, 2763, 1241, 53, 18, 13.8566, 0, 0, 0, 0, 0, NULL),
(37, 8, 2861, 1300, 54, 18, 14.367, 0, 0, 0, 0, 0, NULL),
(38, 8, 2975, 1391, 56, 18, 14.6707, 0, 0, 0, 0, 0, NULL),
(39, 8, 3075, 1489, 58, 18, 15.1049, 0, 0, 0, 0, 0, NULL),
(40, 8, 3191, 1590, 60, 19, 15.5617, 0, 0, 0, 0, 0, NULL),
(41, 8, 3293, 1697, 62, 19, 16.4044, 0, 0, 0, 0, 0, NULL),
(42, 8, 3471, 1811, 67, 19, 17.3076, 0, 0, 0, 0, 0, NULL),
(43, 8, 3575, 1926, 71, 19, 18.5165, 0, 0, 0, 0, 0, NULL),
(44, 8, 3680, 2078, 78, 20, 19.9224, 0, 0, 0, 0, 0, NULL),
(45, 8, 3801, 2177, 81, 20, 21.0097, 0, 0, 0, 0, 0, NULL),
(46, 8, 3923, 2220, 83, 20, 21.4023, 0, 0, 0, 0, 0, NULL),
(47, 8, 4031, 2265, 85, 21, 21.9484, 0, 0, 0, 0, 0, NULL),
(48, 8, 4140, 2307, 87, 21, 22.3444, 0, 0, 0, 0, 0, NULL),
(49, 8, 4281, 2349, 89, 21, 22.8788, 0, 0, 0, 0, 0, NULL),
(50, 8, 4393, 2393, 91, 22, 23.4508, 0, 0, 0, 0, 0, NULL),
(51, 8, 4506, 2437, 93, 22, 23.8815, 0, 0, 0, 0, 0, NULL),
(52, 8, 4650, 2481, 95, 22, 24.4958, 0, 0, 0, 0, 0, NULL),
(53, 8, 4765, 2524, 97, 23, 24.9442, 0, 0, 0, 0, 0, NULL),
(54, 8, 4896, 2567, 99, 23, 25.5368, 0, 0, 0, 0, 0, NULL),
(55, 8, 5013, 2609, 101, 23, 26.079, 0, 0, 0, 0, 0, NULL),
(56, 8, 5206, 2654, 104, 24, 26.4791, 0, 0, 0, 0, 0, NULL),
(57, 8, 5340, 2698, 106, 24, 27.0667, 39.5236, 0, 0, 0, 0, NULL),
(58, 8, 5461, 2740, 107, 24, 27.6972, 43.1021, 0, 0, 0, 0, NULL),
(59, 8, 5598, 2784, 110, 25, 28.1686, 46.5379, 0, 0, 0, 0, NULL),
(60, 8, 5751, 3025, 112, 25, 28.7983, 50.045, 0, 0, 0, 0, NULL),
(61, 8, 5875, 3263, 115, 25, 29.2976, 53.4807, 0, 0, 0, 0, NULL),
(62, 8, 6015, 3500, 116, 25, 29.7969, 57.2713, 0, 0, 0, 0, NULL),
(63, 8, 6156, 3736, 119, 26, 30.2962, 60.577, 0, 0, 0, 0, NULL),
(64, 8, 6299, 3977, 121, 26, 30.7955, 64.3364, 0, 0, 0, 0, NULL),
(65, 8, 6443, 4214, 124, 26, 31.2948, 68.0244, 0, 0, 0, 0, NULL),
(66, 8, 6588, 4460, 125, 27, 31.7941, 72.0499, 0, 0, 0, 0, NULL),
(67, 8, 6749, 4710, 128, 27, 32.2934, 75.9676, 87.7106, 0, 0, 0, NULL),
(68, 8, 6882, 4928, 130, 27, 32.7927, 79.9198, 96.6868, 0, 0, 0, NULL),
(69, 8, 7031, 5167, 133, 28, 33.292, 83.8005, 105.591, 0, 0, 0, NULL),
(70, 8, 7196, 5404, 135, 28, 33.7913, 87.7526, 114.496, 0, 0, 0, NULL),
(71, 8, 7332, 5645, 137, 31, 34.2906, 91.7048, 117.829, 0, 0, 0, NULL),
(72, 8, 7500, 5886, 140, 34, 34.7899, 95.5855, 121.259, 0, 0, 0, NULL),
(73, 8, 7654, 6126, 143, 37, 35.2892, 99.4662, 124.79, 0, 0, 0, NULL),
(74, 8, 7809, 6368, 158, 41, 35.7885, 0, 128.423, 0, 0, 0, NULL),
(75, 8, 7981, 6610, 175, 45, 36.2878, 0, 132.162, 0, 0, 0, NULL),
(76, 8, 8139, 6851, 193, 49, 36.7871, 0, 136.01, 0, 0, 0, NULL),
(77, 8, 8313, 7094, 214, 54, 37.2864, 0, 139.97, 0, 0, 0, NULL),
(78, 8, 8459, 7335, 236, 58, 37.7857, 0, 144.045, 0, 0, 0, NULL),
(79, 8, 8636, 7579, 260, 64, 38.285, 0, 148.239, 0, 0, 0, NULL),
(80, 8, 8814, 7822, 289, 69, 38.7843, 0, 152.412, 0, 0, 0, NULL),
(81, 8, 8979, 8102, 304, 74, 39.2836, 0, 157.584, 0, 0, 0, NULL),
(82, 8, 9160, 8340, 326, 79, 39.7829, 0, 162.257, 0, 0, 0, NULL),
(83, 8, 9328, 8505, 343, 84, 40.2822, 0, 167.286, 0, 0, 0, NULL),
(84, 8, 9512, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(85, 8, 9697, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(86, 8, 9873, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(87, 8, 10052, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(88, 8, 10232, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(89, 8, 10052, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(90, 8, 10052, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(91, 8, 10052, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(92, 8, 10052, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(93, 8, 10052, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(94, 8, 10052, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(95, 8, 10052, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(96, 8, 10052, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(97, 8, 10052, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(98, 8, 10052, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(99, 8, 10052, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(100, 8, 10052, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(101, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(101, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(101, 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(101, 8, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(102, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(102, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(102, 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(102, 8, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(103, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(103, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(103, 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(103, 8, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(104, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(104, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(104, 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(104, 8, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(105, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(105, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(105, 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(105, 8, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, '');


--
DELETE FROM `creature_classlevelstats` WHERE `level` IN (101, 102, 103);
INSERT INTO `creature_classlevelstats` (`level`, `class`, `basemana`, `basearmor`, `attackpower`, `rangedattackpower`, `damage_base`, `damage_exp1`, `damage_exp2`, `damage_exp3`, `damage_exp4`, `damage_exp5`, `comment`) VALUES 
(101, 1, 0, 1670, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(101, 2, 12082, 1670, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(101, 4, 0, 1670, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(101, 8, 25282, 1336, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(102, 1, 0, 1804, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(102, 2, 12082, 1804, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(102, 4, 0, 1804, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(102, 8, 25282, 143, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(103, 1, 0, 1938, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(103, 2, 12082, 1938, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(103, 4, 0, 1938, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(103, 8, 25282, 1550, 0, 0, 0, 0, 0, 0, 0, 0, NULL);
 

-- basemana UnitClass Paladin
UPDATE `creature_classlevelstats` SET `basemana` = 9916 WHERE `level` = 90 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 10084 WHERE `level` = 91 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 10312 WHERE `level` = 92 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 10514 WHERE `level` = 93 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 10748 WHERE `level` = 94 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 10954 WHERE `level` = 95 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 11164 WHERE `level` = 96 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 11404 WHERE `level` = 97 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 11618 WHERE `level` = 98 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 11864 WHERE `level` = 99 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 12082 WHERE `level` = 100 AND `class` = 2;
-- basemana UnitClass Mage
UPDATE `creature_classlevelstats` SET `basemana` = 21262 WHERE `level` = 90 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 21620 WHERE `level` = 91 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 22012 WHERE `level` = 92 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 22406 WHERE `level` = 93 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 22804 WHERE `level` = 94 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 23204 WHERE `level` = 95 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 23578 WHERE `level` = 96 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 24014 WHERE `level` = 97 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 24394 WHERE `level` = 98 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 24836 WHERE `level` = 99 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 25282 WHERE `level` = 100 AND `class` = 8;
 
-- BaseArmor UnitClass Warrior/Paladin/Rogue
UPDATE `creature_classlevelstats` SET `basearmor`= 67 WHERE `level` = 1 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 71 WHERE `level` = 2 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 75 WHERE `level` = 3 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 80 WHERE `level` = 4 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 84 WHERE `level` = 5 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 88 WHERE `level` = 6 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 92 WHERE `level` = 7 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 97 WHERE `level` = 8 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 101 WHERE `level` = 9 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 105 WHERE `level` = 10 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 109 WHERE `level` = 11 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 114 WHERE `level` = 12 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 118 WHERE `level` = 13 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 122 WHERE `level` = 14 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 126 WHERE `level` = 15 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 131 WHERE `level` = 16 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 135 WHERE `level` = 17 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 139 WHERE `level` = 18 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 143 WHERE `level` = 19 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 148 WHERE `level` = 20 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 152 WHERE `level` = 21 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 156 WHERE `level` = 22 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 160 WHERE `level` = 23 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 165 WHERE `level` = 24 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 169 WHERE `level` = 25 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 173 WHERE `level` = 26 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 177 WHERE `level` = 27 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 182 WHERE `level` = 28 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 186 WHERE `level` = 29 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 190 WHERE `level` = 30 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 194 WHERE `level` = 31 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 199 WHERE `level` = 32 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 203 WHERE `level` = 33 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 207 WHERE `level` = 34 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 211 WHERE `level` = 35 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 216 WHERE `level` = 36 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 220 WHERE `level` = 37 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 224 WHERE `level` = 38 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 228 WHERE `level` = 39 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 233 WHERE `level` = 40 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 237 WHERE `level` = 41 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 241 WHERE `level` = 42 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 245 WHERE `level` = 43 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 250 WHERE `level` = 44 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 254 WHERE `level` = 45 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 258 WHERE `level` = 46 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 262 WHERE `level` = 47 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 267 WHERE `level` = 48 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 271 WHERE `level` = 49 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 275 WHERE `level` = 50 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 279 WHERE `level` = 51 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 284 WHERE `level` = 52 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 288 WHERE `level` = 53 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 292 WHERE `level` = 54 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 296 WHERE `level` = 55 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 301 WHERE `level` = 56 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 305 WHERE `level` = 57 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 309 WHERE `level` = 58 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 313 WHERE `level` = 59 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 318 WHERE `level` = 60 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 322 WHERE `level` = 61 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 326 WHERE `level` = 62 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 330 WHERE `level` = 63 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 335 WHERE `level` = 64 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 339 WHERE `level` = 65 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 343 WHERE `level` = 66 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 347 WHERE `level` = 67 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 352 WHERE `level` = 68 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 356 WHERE `level` = 69 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 360 WHERE `level` = 70 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 365 WHERE `level` = 71 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 369 WHERE `level` = 72 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 373 WHERE `level` = 73 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 377 WHERE `level` = 74 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 382 WHERE `level` = 75 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 386 WHERE `level` = 76 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 390 WHERE `level` = 77 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 394 WHERE `level` = 78 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 399 WHERE `level` = 79 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 403 WHERE `level` = 80 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 407 WHERE `level` = 81 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 411 WHERE `level` = 82 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 416 WHERE `level` = 83 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 420 WHERE `level` = 84 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 424 WHERE `level` = 85 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 428 WHERE `level` = 86 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 433 WHERE `level` = 87 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 437 WHERE `level` = 88 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 441 WHERE `level` = 89 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 445 WHERE `level` = 90 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 504 WHERE `level` = 91 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 571 WHERE `level` = 92 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 646 WHERE `level` = 93 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 731 WHERE `level` = 94 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 827 WHERE `level` = 95 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 936 WHERE `level` = 96 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 1059 WHERE `level` = 97 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 1199 WHERE `level` = 98 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 1357 WHERE `level` = 99 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 1536 WHERE `level` = 100 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 1670 WHERE `level` = 101 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 1804 WHERE `level` = 102 AND `class` IN (1, 2, 4);
UPDATE `creature_classlevelstats` SET `basearmor`= 1938 WHERE `level` = 103 AND `class` IN (1, 2, 4);
-- BaseArmor UnitClass Mage
UPDATE `creature_classlevelstats` SET `basearmor` = 53 WHERE `level` = 1 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 57 WHERE `level` = 2 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 60 WHERE `level` = 3 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 64 WHERE `level` = 4 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 67 WHERE `level` = 5 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 70 WHERE `level` = 6 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 74 WHERE `level` = 7 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 77 WHERE `level` = 8 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 81 WHERE `level` = 9 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 84 WHERE `level` = 10 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 87 WHERE `level` = 11 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 91 WHERE `level` = 12 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 94 WHERE `level` = 13 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 98 WHERE `level` = 14 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 101 WHERE `level` = 15 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 104 WHERE `level` = 16 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 108 WHERE `level` = 17 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 111 WHERE `level` = 18 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 115 WHERE `level` = 19 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 118 WHERE `level` = 20 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 121 WHERE `level` = 21 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 125 WHERE `level` = 22 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 128 WHERE `level` = 23 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 132 WHERE `level` = 24 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 135 WHERE `level` = 25 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 139 WHERE `level` = 26 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 142 WHERE `level` = 27 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 145 WHERE `level` = 28 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 149 WHERE `level` = 29 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 152 WHERE `level` = 30 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 156 WHERE `level` = 31 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 159 WHERE `level` = 32 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 162 WHERE `level` = 33 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 166 WHERE `level` = 34 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 169 WHERE `level` = 35 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 173 WHERE `level` = 36 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 176 WHERE `level` = 37 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 179 WHERE `level` = 38 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 183 WHERE `level` = 39 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 186 WHERE `level` = 40 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 190 WHERE `level` = 41 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 193 WHERE `level` = 42 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 196 WHERE `level` = 43 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 200 WHERE `level` = 44 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 203 WHERE `level` = 45 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 207 WHERE `level` = 46 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 210 WHERE `level` = 47 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 213 WHERE `level` = 48 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 217 WHERE `level` = 49 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 220 WHERE `level` = 50 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 224 WHERE `level` = 51 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 227 WHERE `level` = 52 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 230 WHERE `level` = 53 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 234 WHERE `level` = 54 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 237 WHERE `level` = 55 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 241 WHERE `level` = 56 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 244 WHERE `level` = 57 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 247 WHERE `level` = 58 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 251 WHERE `level` = 59 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 254 WHERE `level` = 60 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 258 WHERE `level` = 61 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 261 WHERE `level` = 62 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 264 WHERE `level` = 63 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 268 WHERE `level` = 64 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 271 WHERE `level` = 65 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 275 WHERE `level` = 66 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 278 WHERE `level` = 67 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 281 WHERE `level` = 68 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 285 WHERE `level` = 69 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 288 WHERE `level` = 70 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 292 WHERE `level` = 71 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 295 WHERE `level` = 72 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 298 WHERE `level` = 73 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 302 WHERE `level` = 74 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 305 WHERE `level` = 75 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 309 WHERE `level` = 76 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 312 WHERE `level` = 77 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 315 WHERE `level` = 78 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 319 WHERE `level` = 79 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 322 WHERE `level` = 80 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 326 WHERE `level` = 81 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 329 WHERE `level` = 82 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 332 WHERE `level` = 83 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 336 WHERE `level` = 84 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 339 WHERE `level` = 85 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 343 WHERE `level` = 86 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 346 WHERE `level` = 87 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 350 WHERE `level` = 88 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 353 WHERE `level` = 89 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 356 WHERE `level` = 90 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 403 WHERE `level` = 91 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 457 WHERE `level` = 92 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 517 WHERE `level` = 93 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 585 WHERE `level` = 94 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 662 WHERE `level` = 95 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 749 WHERE `level` = 96 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 847 WHERE `level` = 97 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 959 WHERE `level` = 98 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 1086 WHERE `level` = 99 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basearmor` = 1229 WHERE `level` = 100 AND `class` = 8;
-- MoP

-- basemana UnitClass Paladin
UPDATE `creature_classlevelstats` SET `basemana` = 8726 WHERE `level` = 84 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 8908 WHERE `level` = 85 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 9094 WHERE `level` = 86 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 9310 WHERE `level` = 87 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 9470 WHERE `level` = 88 AND `class` = 2;
UPDATE `creature_classlevelstats` SET `basemana` = 9692 WHERE `level` = 89 AND `class` = 2;
-- basemana UnitClass Mage
UPDATE `creature_classlevelstats` SET `basemana` = 18656 WHERE `level` = 83 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 19024 WHERE `level` = 84 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 19394 WHERE `level` = 85 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 19738 WHERE `level` = 86 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 20114 WHERE `level` = 87 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 20464 WHERE `level` = 88 AND `class` = 8;
UPDATE `creature_classlevelstats` SET `basemana` = 20846 WHERE `level` = 89 AND `class` = 8;