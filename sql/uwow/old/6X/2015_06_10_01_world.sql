REPLACE INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `terrainswapmap`, `wmAreaId`, `flags`, `comment`) VALUES
(4, 1, 2, 3971, 1190, 992, 0, 'DARK_PORTAL_WOD'),   -- phase aura 176111
(4, 2, 0, 3970, 1066, 910, 0, 'Dark Portal. ALways ON.'),
(4, 3, 0, 0, 736, 770, 0, 'Dark Portal. ALways ON.');

--
UPDATE `creature_template` SET  dynamicflags = 0 WHERE dynamicflags > 0x80;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=14, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (76556, 78696, 76648, 77090);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=2580, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=33024, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (82014, 81998, 82011, 82002, 81999, 82005, 82009, 82008);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=2580, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=33024, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (82015, 82263, 82013, 82007, 82016, 81994, 82025, 81996, 81990, 82010, 81995, 82017, 82012, 82259);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=2580, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=4, `unit_flags`=33024, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (81997, 82189, 82075, 82191, 82082, 82001, 82260);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=2580, `npcflag`=3, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=256, `unit_flags2`=18432, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (79315, 78556);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=16, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (78674, 78348, 77101, 76673);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=2580, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33024, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (82000, 82187, 82188, 82006, 82264, 82004);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=16, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (78667, 78345, 78488, 82451, 77723, 77771, 82774, 78142, 77643, 77721, 76886);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=2636, `npcflag`=4739, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33024, `unit_flags2`=18432, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=78568;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=2636, `npcflag`=2, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33024, `unit_flags2`=18432, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=78569;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=93, `MaxLevel`=93, `faction`=2580, `npcflag`=2, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=256, `unit_flags2`=18432, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=78554;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=2580, `npcflag`=3, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=256, `unit_flags2`=18432, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=79316;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=92, `MaxLevel`=92, `faction`=35, `npcflag`=2, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=4, `unit_flags`=33024, `unit_flags2`=18432, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=78430;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=2580, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=4, `unit_flags`=33024, `unit_flags2`=4196352, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=82003;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=14, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=78670;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=3, `speed_run`=1.289683, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33024, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=79675;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=35, `npcflag`=2, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=18432, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=78553;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=35, `npcflag`=3, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=33024, `unit_flags2`=34816, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=78423;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=2580, `npcflag`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=33024, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=81993;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=188, `npcflag`=0, `speed_run`=0.8571429, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=15476;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=16, `npcflag`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (78193, 77790, 77845);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=31, `npcflag`=1073741824, `speed_run`=0.7142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=512, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=61328;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=55, `MaxLevel`=55, `faction`=73, `npcflag`=0, `speed_run`=1.428571, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=5983;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=16, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=78489;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=31, `npcflag`=0, `speed_run`=0.7142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=9699;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=7, `npcflag`=1073741824, `speed_run`=0.8571429, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33280, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=61325;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=16, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (77640, 77767, 77644);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=55, `MaxLevel`=56, `faction`=49, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1.111112, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=5990;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=55, `MaxLevel`=56, `faction`=38, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=5985;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=91, `MaxLevel`=91, `faction`=16, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=77724;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=57, `MaxLevel`=57, `faction`=14, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=5992;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=188, `npcflag`=1073741824, `speed_run`=0.8571429, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=512, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=61326;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1802, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (76085, 76098, 76448);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=7, `npcflag`=0, `speed_run`=0.8571429, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=3300;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1732, `npcflag`=2, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=82447;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1732, `npcflag`=642, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76111;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=40, `npcflag`=16777216, `speed_run`=1, `speed_walk`=0.444444, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=71321600, `unit_flags3`=0, `dynamicflags`=512, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=82273;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1801, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76524;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=16, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=1500, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76651;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=49, `MaxLevel`=50, `faction`=35, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=82850;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=190, `npcflag`=1073741824, `speed_run`=0.8571429, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33280, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=61327;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=1, `MinLevel`=65, `MaxLevel`=65, `faction`=11, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=537166080, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=64, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=41158;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=1, `MinLevel`=65, `MaxLevel`=65, `faction`=1074, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=1600, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=537166080, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=64, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (44311, 44313);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=35, `npcflag`=0, `speed_run`=1.285714, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=570721024, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=64, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=78141;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=190, `npcflag`=0, `speed_run`=0.8571429, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=14881;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=40, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (73446, 73459);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=60, `MaxLevel`=60, `faction`=2211, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=537166080, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=64, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=41125;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=14, `npcflag`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33554432, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=84199;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1732, `npcflag`=2688, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76117;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1575, `npcflag`=0, `speed_run`=1.385714, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=82448;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1732, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76103;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1732, `npcflag`=80, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76108;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=93, `MaxLevel`=93, `faction`=84, `npcflag`=3, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=82270;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1934, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76436;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1732, `npcflag`=4224, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76110;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=2502, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=3000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76189;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1733, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76429;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=2, `MaxLevel`=2, `faction`=35, `npcflag`=0, `speed_run`=1.289683, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=16225;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1935, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry` IN (76500, 76504);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1935, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76503;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1735, `npcflag`=4224, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76521;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1735, `npcflag`=3, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=82851;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1735, `npcflag`=2688, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76522;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=93, `MaxLevel`=93, `faction`=1735, `npcflag`=3, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=85247;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1735, `npcflag`=640, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76520;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1735, `npcflag`=80, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=76519;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=1734, `npcflag`=8193, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=85734;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=58, `MaxLevel`=58, `faction`=14, `npcflag`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1 WHERE `entry`=45087;

UPDATE `creature_template` SET equipment_id = entry WHERE entry in (82447, 82448, 82451, 78553, 78554, 78556, 76886, 79675, 85247, 78568, 78569, 81990, 81994, 81995, 81996, 81997, 81998, 76429, 81999, 82000, 82001, 82002, 82003, 82004, 82005, 76436, 82006, 82007, 82008, 78667, 82009, 82010, 82011, 82012, 78670, 82013, 82014, 82015, 82016, 78674, 82017, 76448, 82025, 78696, 78142, 76500, 76503, 76504, 82075, 82082, 76520, 76521, 76524, 77640, 77643, 77644, 79315, 79316, 77090, 77101, 76556, 77721, 77723, 77724, 44311, 82187, 82188, 44313, 82189, 82191, 76085, 82774, 76648, 76651, 77767, 76098, 77771, 76110, 76111, 76673, 78345, 77790, 78348, 82259, 82260, 82263, 82264, 82270, 82850, 82851, 76189, 78423, 78430, 41125, 73446, 73459, 41158, 78488, 83550, 80766, 81324, 80769, 85226, 80220, 75765, 80787, 84688, 80793, 86364, 74667, 85813, 80811, 80812, 80814, 83601, 79706, 80824, 82496, 80827, 74146, 74147, 74148, 74149, 74150, 82506, 79178, 80853, 85309, 75283, 80859, 77517, 77518, 77522, 88663, 82537, 88664, 74741, 89236, 74200, 78104, 72537, 80896, 76447, 73106, 78127, 83700, 78131, 74233, 78135, 84825, 79255, 73129, 82041, 78148, 82051, 85394, 79270, 73701, 81502, 78168, 82068, 82069, 84303, 72609, 83759, 86544, 72623, 79308, 79309, 81541, 81542, 84886, 84888, 72638, 83779, 80996, 86569, 83785, 81002, 72647, 78219, 86575, 74880, 86024, 78787, 78789, 78790, 84922, 84923, 84924, 74343, 74344, 72674, 84372, 84930, 81031, 72677, 84933, 88276, 84377, 77143, 88842, 84944, 84945, 81047, 81605, 82166, 82167, 74374, 73261, 79394, 84966, 77173, 73839, 81637, 82196, 81639, 81082, 79422, 74969, 73856, 85553, 85001, 82220, 78324, 73870, 84455, 83906, 73324, 78339, 85026, 73888, 77788, 81140, 75015, 83931, 72793, 83934, 83937, 78928, 85056, 83387, 81717, 77264, 81179, 81180, 75611, 72829, 88428, 85088, 72838, 78409, 78410, 73954, 75071, 85098, 78973, 76188, 88450, 76201, 76202, 80662, 78993, 73981, 78999, 79007, 81238, 81244, 82362, 84034, 85150, 84041, 78475, 85716, 77920, 73465, 89063, 73468, 81824, 81268, 85168, 80162, 84061, 81278, 79054, 80727, 89085, 84080, 81296, 81299, 81858, 72391, 81304, 81314, 80761);
DELETE FROM `creature_equip_template` WHERE `entry` IN (82447, 82448, 82451, 78553, 78554, 78556, 76886, 79675, 85247, 78568, 78569, 81990, 81994, 81995, 81996, 81997, 81998, 76429, 81999, 82000, 82001, 82002, 82003, 82004, 82005, 76436, 82006, 82007, 82008, 78667, 82009, 82010, 82011, 82012, 78670, 82013, 82014, 82015, 82016, 78674, 82017, 76448, 82025, 78696, 78142, 76500, 76503, 76504, 82075, 82082, 76520, 76521, 76524, 77640, 77643, 77644, 79315, 79316, 77090, 77101, 76556, 77721, 77723, 77724, 44311, 82187, 82188, 44313, 82189, 82191, 76085, 82774, 76648, 76651, 77767, 76098, 77771, 76110, 76111, 76673, 78345, 77790, 78348, 82259, 82260, 82263, 82264, 82270, 82850, 82851, 76189, 78423, 78430, 41125, 73446, 73459, 41158, 78488);
INSERT INTO `creature_equip_template` (`entry`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES
(82447, 58196, 1984, 0), -- 82447
(82448, 19980, 0, 0), -- 82448
(82451, 108971, 0, 0), -- 82451
(78553, 109674, 0, 0), -- 78553
(78554, 30802, 0, 0), -- 78554
(78556, 57012, 0, 0), -- 78556
(76886, 12629, 12629, 0), -- 76886
(79675, 49767, 0, 0), -- 79675
(85247, 29433, 0, 0), -- 85247
(78568, 1911, 0, 0), -- 78568
(78569, 3494, 11506, 0), -- 78569
(81990, 35015, 0, 0), -- 81990
(81994, 73477, 0, 0), -- 81994
(81995, 105408, 0, 0), -- 81995
(81996, 105925, 0, 0), -- 81996
(81997, 2632, 2632, 0), -- 81997
(81998, 65496, 0, 0), -- 81998
(76429, 1905, 2081, 0), -- 76429
(81999, 61403, 0, 0), -- 81999
(82000, 28773, 0, 0), -- 82000
(82001, 0, 0, 102597), -- 82001
(82002, 78477, 0, 0), -- 82002
(82003, 0, 0, 99805), -- 82003
(82004, 70165, 0, 0), -- 82004
(82005, 105301, 105356, 0), -- 82005
(76436, 1899, 143, 62285), -- 76436
(82006, 113093, 95686, 0), -- 82006
(82007, 9475, 0, 0), -- 82007
(82008, 90462, 0, 0), -- 82008
(78667, 109553, 106112, 0), -- 78667
(82009, 46033, 0, 0), -- 82009
(82010, 95809, 0, 0), -- 82010
(82011, 18082, 0, 0), -- 82011
(82012, 59778, 0, 0), -- 82012
(78670, 108969, 0, 0), -- 78670
(82013, 40280, 0, 0), -- 82013
(82014, 103875, 0, 0), -- 82014
(82015, 103875, 0, 0), -- 82015
(82016, 65974, 34011, 0), -- 82016
(78674, 109583, 106112, 0), -- 78674
(82017, 104503, 0, 0), -- 82017
(76448, 21573, 21572, 0), -- 76448
(82025, 106833, 0, 0), -- 82025
(78696, 106276, 106112, 0), -- 78696
(78142, 12629, 0, 110788), -- 78142
(76500, 10898, 1905, 0), -- 76500
(76503, 1905, 2081, 0), -- 76503
(76504, 43175, 46957, 0), -- 76504
(82075, 13036, 13036, 0), -- 82075
(82082, 105318, 103827, 0), -- 82082
(76520, 2196, 0, 0), -- 76520
(76521, 1903, 0, 0), -- 76521
(76524, 91813, 0, 91812), -- 76524
(77640, 109583, 39288, 0), -- 77640
(77643, 108654, 2081, 0), -- 77643
(77644, 109554, 60960, 0), -- 77644
(79315, 14084, 0, 0), -- 79315
(79316, 28678, 0, 0), -- 79316
(77090, 12629, 0, 0), -- 77090
(77101, 13750, 0, 0), -- 77101
(76556, 106275, 39288, 0), -- 76556
(77721, 106275, 49638, 101405), -- 77721
(77723, 34638, 0, 69207), -- 77723
(77724, 12304, 12304, 0), -- 77724
(44311, 1903, 0, 0), -- 44311
(82187, 33688, 0, 0), -- 82187
(82188, 49299, 0, 0), -- 82188
(44313, 2716, 0, 0), -- 44313
(82189, 95666, 94587, 0), -- 82189
(82191, 103827, 103775, 0), -- 82191
(76085, 107955, 108041, 107953), -- 76085
(82774, 107952, 0, 101405), -- 82774
(76648, 108970, 39288, 0), -- 76648
(76651, 107952, 0, 101405), -- 76651
(77767, 109582, 13630, 0), -- 77767
(76098, 1905, 0, 0), -- 76098
(77771, 108971, 0, 0), -- 77771
(76110, 1903, 0, 0), -- 76110
(76111, 2705, 0, 0), -- 76111
(76673, 32360, 0, 0), -- 76673
(78345, 109553, 109553, 0), -- 78345
(77790, 12629, 0, 110788), -- 77790
(78348, 110417, 12747, 0), -- 78348
(82259, 105001, 0, 0), -- 82259
(82260, 0, 0, 100816), -- 82260
(82263, 69609, 86778, 0), -- 82263
(82264, 113125, 113125, 0), -- 82264
(82270, 30802, 0, 0), -- 82270
(82850, 13822, 0, 0), -- 82850
(82851, 109674, 0, 0), -- 82851
(76189, 49657, 0, 110788), -- 76189
(78423, 28067, 0, 0), -- 78423
(78430, 32425, 0, 0), -- 78430
(41125, 12786, 0, 0), -- 41125
(73446, 37123, 0, 0), -- 73446
(73459, 0, 0, 49762), -- 73459
(41158, 2178, 1984, 2551), -- 41158
(78488, 110458, 0, 0); -- 78488

--
SET @CGUID = 477126;

DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+230;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(@OGUID+0, 233911, 0, 1, 2, -11608.19, -3190.632, 8.889158, 1.881576, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 72)
(@OGUID+1, 230609, 0, 1, 2, -11812.38, -3205.606, -29.58401, 0, 0, 0, 0, 1, 220, 255, 1), -- 230609 (Area: 72)
(@OGUID+2, 225742, 0, 1, 2, -11808.19, -3220.63, -30.8155, 3.141779, 0, 0, 0, 1, 220, 255, 1), -- 225742 (Area: 72)
(@OGUID+3, 142145, 0, 1, 2, -11713.8, -3222.83, 4.549989, 1.832595, 0, 0, 0, 1, 220, 255, 1), -- 142145 (Area: 72)
(@OGUID+4, 225787, 0, 1, 2, -11837.46, -3166.231, -29.65916, 0.7417632, 0, 0, 0, 1, 220, 255, 1), -- 225787 (Area: 72)
(@OGUID+5, 233911, 0, 1, 2, -11617.58, -3131.37, 8.954547, 4.861571, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 72)
(@OGUID+6, 233913, 0, 1, 2, -11610.68, -3131.698, 7.265414, 4.97041, 0, 0, 0, 1, 220, 255, 1), -- 233913 (Area: 72)
(@OGUID+7, 233911, 0, 1, 2, -11604.54, -3127.753, 6.739295, 5.162222, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 72)
(@OGUID+8, 142145, 0, 1, 2, -11791, -3068.5, 7.65385, 1.832595, 0, 0, 0, 1, 220, 255, 1), -- 142145 (Area: 72)
(@OGUID+9, 233911, 0, 1, 2, -11585.8, -3120.72, 6.383681, 5.275294, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 72)
(@OGUID+10, 232625, 0, 1, 2, -11452.57, -2816.769, -1.685798, 4.819524, 0, 0, 0, 1, 220, 255, 1), -- 232625 (Area: 72)
(@OGUID+11, 232625, 0, 1, 2, -11486.47, -2783.302, -0.9731424, 3.614258, 0, 0, 0, 1, 220, 255, 1), -- 232625 (Area: 72)
(@OGUID+12, 176586, 0, 1, 2, -11851.7, -2931.87, 15.857, 0, 0, 0, 0, 1, 220, 255, 1), -- 176586 (Area: 0)
(@OGUID+13, 233911, 0, 1, 2, -11625.17, -2850.035, 4.692759, 0.4695361, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 0)
(@OGUID+14, 142145, 0, 1, 2, -11684.9, -2964.76, 7.639727, 1.832595, 0, 0, 0, 1, 220, 255, 1), -- 142145 (Area: 0)
(@OGUID+15, 233913, 0, 1, 2, -11769.4, -2795.149, 35.11865, 2.388262, 0, 0, 0, 1, 220, 255, 1), -- 233913 (Area: 0)
(@OGUID+16, 233911, 0, 1, 2, -11785.48, -2752.695, 8.040092, 2.12082, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 0)
(@OGUID+17, 233911, 0, 1, 2, -11643.72, -2815.688, 4.330925, 6.202315, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 0)
(@OGUID+18, 233911, 0, 1, 2, -11643.48, -2786.563, 5.347189, 6.046505, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 0)
(@OGUID+19, 233911, 0, 1, 2, -11800.04, -2765.479, 8.163164, 2.33248, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 0)
(@OGUID+20, 233911, 0, 1, 2, -11642.81, -2772.554, 6.427967, 0.07470532, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 0)
(@OGUID+21, 233913, 0, 1, 2, -11644.81, -2779.285, 6.035004, 6.225528, 0, 0, 0, 1, 220, 255, 1), -- 233913 (Area: 0)
(@OGUID+22, 225951, 0, 1, 2, -11650.18, -2785.595, 5.852431, 0, 0, 0, 0, 1, 220, 255, 1), -- 225951 (Area: 0)
(@OGUID+23, 232625, 0, 1, 2, -11340.65, -2801.408, 9.355593, 0.3967488, 0, 0, 0, 1, 220, 255, 1), -- 232625 (Area: 0)
(@OGUID+24, 225951, 0, 1, 2, -11650.18, -2785.595, 5.852431, 0, 0, 0, 0, 1, 220, 255, 1), -- 225951 (Area: 0)
(@OGUID+25, 324, 0, 1, 2, -11605.1, -2723.5, 12.3934, 4.71239, 0, 0, 0, 1, 220, 255, 1), -- 324 (Area: 0)
(@OGUID+26, 176584, 0, 1, 2, -11614.8, -2787.17, 4.10833, 0, 0, 0, 0, 1, 220, 255, 1), -- 176584 (Area: 0)
(@OGUID+27, 176584, 0, 1, 2, -11489.8, -2807.82, -0.730605, 0, 0, 0, 0, 1, 220, 255, 1), -- 176584 (Area: 0)
(@OGUID+28, 176586, 0, 1, 2, -11504.3, -2649.13, 15.7898, 0, 0, 0, 0, 1, 220, 255, 1), -- 176586 (Area: 0)
(@OGUID+29, 204115, 0, 1, 2, -11459.66, -2635.728, 3.5822, 4.475208, 0, 0, -0.6857511, 0.7278361, 120, 255, 1), -- 204115 (Area: 1439)
(@OGUID+30, 204115, 0, 1, 2, -11459.66, -2635.728, 3.582194, 4.475208, 0, 0, -0.6857511, 0.7278361, 120, 255, 1), -- 204115 (Area: 1439)
(@OGUID+31, 225937, 0, 1, 2, -11396.35, -2828.951, -2.404514, 3.141593, 0, 0, 0, 1, 220, 255, 1), -- 225937 (Area: 1439)
(@OGUID+32, 324, 0, 1, 2, -11311.58, -2650.503, -14.31769, 0, 0, 0, 0, 1, 220, 255, 1), -- 324 (Area: 1439)
(@OGUID+33, 176586, 0, 1, 2, -11189.9, -2682.79, 12.3286, 0, 0, 0, 0, 1, 220, 255, 1), -- 176586 (Area: 0)
(@OGUID+34, 324, 0, 1, 2, -11153.7, -2677.12, 17.2944, 4.572764, 0, 0, 0, 1, 220, 255, 1), -- 324 (Area: 0)
(@OGUID+35, 324, 0, 1, 2, -11116, -2807.28, 37.4364, 1.361356, 0, 0, 0, 1, 220, 255, 1), -- 324 (Area: 0)
(@OGUID+36, 194802, 0, 1, 2, -10998.26, -2722.907, 6.301161, 5.373677, 0, 0, -0.4392414, 0.8983691, 120, 255, 1), -- 194802 (Area: 7026)
(@OGUID+37, 194802, 0, 1, 2, -10998.26, -2722.907, 6.301161, 5.373677, 0, 0, -0.4392414, 0.8983691, 120, 255, 1), -- 194802 (Area: 7026)
(@OGUID+38, 176584, 0, 1, 2, -10986.5, -2815.48, 6.09645, 0, 0, 0, 0, 1, 220, 255, 1), -- 176584 (Area: 7026)
(@OGUID+39, 227645, 0, 1, 2, -11278.75, -3157.309, 5.276927, 2.805681, 0, 0, 0, 1, 220, 255, 1), -- 227645 (Area: 2517)
(@OGUID+40, 225951, 0, 1, 2, -11272.43, -3162.12, 5.300002, 0, 0, 0, 0, 1, 220, 255, 1), -- 225951 (Area: 2517)
(@OGUID+41, 176584, 0, 1, 2, -11326.2, -3114.31, -1.65432, 0, 0, 0, 0, 1, 220, 255, 1), -- 176584 (Area: 2517)
(@OGUID+42, 225951, 0, 1, 2, -11272.43, -3162.12, 5.300002, 0, 0, 0, 0, 1, 220, 255, 1), -- 225951 (Area: 2517)
(@OGUID+43, 230771, 0, 1, 2, -11272.9, -3159.384, 5.121134, 0, 0, 0, 0, 1, 220, 255, 1), -- 230771 (Area: 2517)
(@OGUID+44, 233909, 0, 1, 2, -11354.6, -3232.498, 9.531609, 3.353297, 0, 0, 0, 1, 220, 255, 1), -- 233909 (Area: 2517)
(@OGUID+45, 233905, 0, 1, 2, -11355.68, -3250.142, 8.808319, 4.5158, 0, 0, 0, 1, 220, 255, 1), -- 233905 (Area: 2517)
(@OGUID+46, 227645, 0, 1, 2, -11276.56, -3160.632, 5.16964, 0, 0, 0, 0, 1, 220, 255, 1), -- 227645 (Area: 2517)
(@OGUID+47, 233904, 0, 1, 2, -11358.82, -3268.226, 8.979104, 2.985059, 0, 0, 0, 1, 220, 255, 1), -- 233904 (Area: 0)
(@OGUID+48, 233905, 0, 1, 2, -11402.92, -3240.538, 10.94097, 0.7010023, 0, 0, 0, 1, 220, 255, 1), -- 233905 (Area: 0)
(@OGUID+49, 233911, 0, 1, 2, -11508.71, -3183.934, 9.623264, 1.20406, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 0)
(@OGUID+50, 233911, 0, 1, 2, -11454.81, -3227.898, 14.88609, 5.811057, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 0)
(@OGUID+51, 233913, 0, 1, 2, -11527.7, -3182.038, 7.304455, 1.730899, 0, 0, 0, 1, 220, 255, 1), -- 233913 (Area: 0)
(@OGUID+52, 324, 0, 1, 2, -11397.7, -3196.66, 28.4255, 1.361356, 0, 0, 0, 1, 220, 255, 1), -- 324 (Area: 0)
(@OGUID+53, 233913, 0, 1, 2, -11470.59, -3258.559, 18.02386, 5.948521, 0, 0, 0, 1, 220, 255, 1), -- 233913 (Area: 0)
(@OGUID+54, 233911, 0, 1, 2, -11520.87, -3183.203, 8.66493, 1.801324, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 0)
(@OGUID+55, 233911, 0, 1, 2, -11534.19, -3185.365, 6.382185, 1.685077, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 0)
(@OGUID+56, 233905, 0, 1, 2, -11523.83, -3248.514, 7.044259, 4.619554, 0, 0, 0, 1, 220, 255, 1), -- 233905 (Area: 0)
(@OGUID+57, 233914, 0, 1, 2, -11509.36, -3283.3, 7.934028, 2.591761, 0, 0, 0, 1, 220, 255, 1), -- 233914 (Area: 0)
(@OGUID+58, 227645, 0, 1, 2, -11380.85, -3261.71, 7.267361, 1.881783, 0, 0, 0, 1, 220, 255, 1), -- 227645 (Area: 0)
(@OGUID+59, 227645, 0, 1, 2, -11378.52, -3263.533, 7.260417, 5.359289, 0, 0, 0, 1, 220, 255, 1), -- 227645 (Area: 0)
(@OGUID+60, 233911, 0, 1, 2, -11505.45, -3280.62, 8.046875, 6.087868, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 0)
(@OGUID+61, 227645, 0, 1, 2, -11406.92, -3247.299, 9.901724, 2.468386, 0, 0, 0, 1, 220, 255, 1), -- 227645 (Area: 0)
(@OGUID+62, 233909, 0, 1, 2, -11538.79, -3211.903, 6.547814, 4.971424, 0, 0, 0, 1, 220, 255, 1), -- 233909 (Area: 0)
(@OGUID+63, 225951, 0, 1, 2, -11360.16, -3259.483, 8.462201, 0, 0, 0, 0, 1, 220, 255, 1), -- 225951 (Area: 0)
(@OGUID+64, 225951, 0, 1, 2, -11360.16, -3259.483, 8.462201, 0, 0, 0, 0, 1, 220, 255, 1), -- 225951 (Area: 0)
(@OGUID+65, 233905, 0, 1, 2, -11551.5, -3213.752, 6.031723, 0.2316405, 0, 0, 0, 1, 220, 255, 1), -- 233905 (Area: 0)
(@OGUID+66, 233914, 0, 1, 2, -11519.54, -3306.568, 8.030267, 5.800016, 0, 0, 0, 1, 220, 255, 1), -- 233914 (Area: 0)
(@OGUID+67, 233911, 0, 1, 2, -11518.97, -3312.245, 7.904741, 5.765079, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 0)
(@OGUID+68, 233904, 0, 1, 2, -11546.33, -3275.524, 7.37938, 0.9814464, 0, 0, 0, 1, 220, 255, 1), -- 233904 (Area: 0)
(@OGUID+69, 147742, 0, 1, 2, -11300.02, -3430.823, 10.06506, 2.095059, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147742 (Area: 0)
(@OGUID+70, 176584, 0, 1, 2, -11352.1, -3430.03, 7.79347, 0, 0, 0, 0, 1, 220, 255, 1), -- 176584 (Area: 0)
(@OGUID+71, 147754, 0, 1, 2, -11280.16, -3442.261, 10.06506, 3.117795, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147754 (Area: 0)
(@OGUID+72, 147754, 0, 1, 2, -11280.16, -3442.261, 10.06506, 3.117795, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147754 (Area: 0)
(@OGUID+73, 147742, 0, 1, 2, -11300.02, -3430.823, 10.06506, 2.095059, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147742 (Area: 0)
(@OGUID+74, 324, 0, 1, 2, -11132.3, -3282.56, 15.667, 3.089183, 0, 0, 0, 1, 220, 255, 1), -- 324 (Area: 0)
(@OGUID+75, 230853, 0, 1, 2, -11071.97, -3413.299, 69.49965, 5.834037, 0, 0, 0, 1, 220, 255, 1), -- 230853 (Area: 0)
(@OGUID+76, 228509, 0, 1, 2, -11099.58, -3442.052, 64.93058, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 0)
(@OGUID+77, 147767, 0, 1, 2, -11215.33, -3472.688, 11.17652, 1.427909, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147767 (Area: 0)
(@OGUID+78, 228509, 0, 1, 2, -11110.45, -3447.019, 64.943, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 0)
(@OGUID+79, 228509, 0, 1, 2, -11089.12, -3441.222, 64.74865, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 0)
(@OGUID+80, 147745, 0, 1, 2, -11237.71, -3446.24, 7.112101, 6.215946, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147745 (Area: 0)
(@OGUID+81, 228509, 0, 1, 2, -11110.45, -3447.019, 64.943, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 0)
(@OGUID+82, 147767, 0, 1, 2, -11215.33, -3472.688, 11.17652, 1.427909, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147767 (Area: 0)
(@OGUID+83, 228509, 0, 1, 2, -11099.58, -3442.052, 64.93058, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 0)
(@OGUID+84, 147745, 0, 1, 2, -11237.71, -3446.24, 7.112101, 6.215946, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147745 (Area: 0)
(@OGUID+85, 228509, 0, 1, 2, -11089.12, -3441.222, 64.74865, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 0)
(@OGUID+86, 147768, 0, 1, 2, -11217.46, -3485.893, 8.386394, 0.04018151, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147768 (Area: 1438)
(@OGUID+87, 147747, 0, 1, 2, -11238.51, -3460.276, 7.748813, 5.891474, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147747 (Area: 1438)
(@OGUID+88, 147768, 0, 1, 2, -11217.46, -3485.893, 8.386394, 0.04018151, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147768 (Area: 1438)
(@OGUID+89, 147747, 0, 1, 2, -11238.51, -3460.276, 7.748813, 5.891474, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147747 (Area: 1438)
(@OGUID+90, 152328, 0, 1, 2, -10969.79, -3380.004, 68.4378, 6.056295, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152328 (Area: 1438)
(@OGUID+91, 152332, 0, 1, 2, -10945.46, -3410.689, 90.56601, 1.893682, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152332 (Area: 1438)
(@OGUID+92, 147283, 0, 1, 2, -10951.61, -3450.373, 66.76239, 4.804552, 0, 0, 0.9595121, 0.2816674, 120, 255, 1), -- 147283 (Area: 1438)
(@OGUID+93, 147284, 0, 1, 2, -10957.2, -3458.729, 66.76239, 0.004894262, 0, 0, 0.9595121, 0.2816674, 120, 255, 1), -- 147284 (Area: 1438)
(@OGUID+94, 147283, 0, 1, 2, -10951.61, -3450.373, 66.76239, 4.804552, 0, 0, 0.9595121, 0.2816674, 120, 255, 1), -- 147283 (Area: 1438)
(@OGUID+95, 147284, 0, 1, 2, -10957.2, -3458.729, 66.76239, 0.004894262, 0, 0, 0.9595121, 0.2816674, 120, 255, 1), -- 147284 (Area: 1438)
(@OGUID+96, 152325, 0, 1, 2, -10972.69, -3392.57, 68.4378, 6.056295, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152325 (Area: 1438)
(@OGUID+97, 152328, 0, 1, 2, -10969.79, -3380.004, 68.4378, 6.056295, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152328 (Area: 1438)
(@OGUID+98, 152332, 0, 1, 2, -10945.46, -3410.689, 90.56601, 1.893682, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152332 (Area: 1438)
(@OGUID+99, 152325, 0, 1, 2, -10972.69, -3392.57, 68.4378, 6.056295, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152325 (Area: 1438)
(@OGUID+100, 152327, 0, 1, 2, -10937.31, -3375.696, 90.56601, 2.417279, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152327 (Area: 1438)
(@OGUID+101, 152329, 0, 1, 2, -10915.6, -3380.505, 90.56601, 0.8464848, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152329 (Area: 1438)
(@OGUID+102, 152330, 0, 1, 2, -10923.48, -3415.787, 90.56601, 0.8464848, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152330 (Area: 1438)
(@OGUID+103, 152331, 0, 1, 2, -10922.31, -3403.123, 81.68633, 5.009097, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152331 (Area: 1438)
(@OGUID+104, 37026, 0, 1, 2, -10932.44, -3400.573, 80.45477, 2.792518, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37026 (Area: 1438)
(@OGUID+105, 37028, 0, 1, 2, -10930.28, -3390.906, 80.45477, 2.91469, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37028 (Area: 1438)
(@OGUID+106, 37029, 0, 1, 2, -10914.62, -3400.719, 66.50541, 2.879789, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37029 (Area: 1438)
(@OGUID+107, 37030, 0, 1, 2, -10917.67, -3400.04, 66.50541, 6.056295, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37030 (Area: 1438)
(@OGUID+108, 37033, 0, 1, 2, -10908.63, -3390.427, 81.82459, 4.468043, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37033 (Area: 1438)
(@OGUID+109, 37034, 0, 1, 2, -10909.24, -3400.81, 81.82459, 2.897245, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37034 (Area: 1438)
(@OGUID+110, 37035, 0, 1, 2, -10912.55, -3389.521, 81.82459, 4.468043, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37035 (Area: 1438)
(@OGUID+111, 37036, 0, 1, 2, -10910.15, -3404.735, 81.82459, 2.897245, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37036 (Area: 1438)
(@OGUID+112, 37037, 0, 1, 2, -10917.08, -3409.127, 81.82459, 1.370084, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37037 (Area: 1438)
(@OGUID+113, 37038, 0, 1, 2, -10913.12, -3409.861, 81.82459, 1.370084, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37038 (Area: 1438)
(@OGUID+114, 37026, 0, 1, 2, -10932.44, -3400.573, 80.45477, 2.792518, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37026 (Area: 1438)
(@OGUID+115, 147285, 0, 1, 2, -10950.88, -3456.816, 64.95444, 2.570528, 0, 0, 0.9595121, 0.2816674, 120, 255, 1), -- 147285 (Area: 1438)
(@OGUID+116, 37028, 0, 1, 2, -10930.28, -3390.906, 80.45477, 2.91469, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37028 (Area: 1438)
(@OGUID+117, 37029, 0, 1, 2, -10914.62, -3400.719, 66.50541, 2.879789, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37029 (Area: 1438)
(@OGUID+118, 37030, 0, 1, 2, -10917.67, -3400.04, 66.50541, 6.056295, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37030 (Area: 1438)
(@OGUID+119, 37033, 0, 1, 2, -10908.63, -3390.427, 81.82459, 4.468043, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37033 (Area: 1438)
(@OGUID+120, 37034, 0, 1, 2, -10909.24, -3400.81, 81.82459, 2.897245, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37034 (Area: 1438)
(@OGUID+121, 37035, 0, 1, 2, -10912.55, -3389.521, 81.82459, 4.468043, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37035 (Area: 1438)
(@OGUID+122, 37036, 0, 1, 2, -10910.15, -3404.735, 81.82459, 2.897245, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37036 (Area: 1438)
(@OGUID+123, 37037, 0, 1, 2, -10917.08, -3409.127, 81.82459, 1.370084, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37037 (Area: 1438)
(@OGUID+124, 37038, 0, 1, 2, -10913.12, -3409.861, 81.82459, 1.370084, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37038 (Area: 1438)
(@OGUID+125, 147285, 0, 1, 2, -10950.88, -3456.816, 64.95444, 2.570528, 0, 0, 0.9595121, 0.2816674, 120, 255, 1), -- 147285 (Area: 1438)
(@OGUID+126, 152326, 0, 1, 2, -10919.74, -3392.567, 81.68633, 4.031712, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152326 (Area: 1438)
(@OGUID+127, 152327, 0, 1, 2, -10937.31, -3375.696, 90.56601, 2.417279, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152327 (Area: 1438)
(@OGUID+128, 152329, 0, 1, 2, -10915.6, -3380.505, 90.56601, 0.8464848, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152329 (Area: 1438)
(@OGUID+129, 152330, 0, 1, 2, -10923.48, -3415.787, 90.56601, 0.8464848, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152330 (Area: 1438)
(@OGUID+130, 152331, 0, 1, 2, -10922.31, -3403.123, 81.68633, 5.009097, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152331 (Area: 1438)
(@OGUID+131, 152326, 0, 1, 2, -10919.74, -3392.567, 81.68633, 4.031712, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 152326 (Area: 1438)
(@OGUID+132, 146441, 0, 1, 2, -10950.17, -3463.397, 66.76239, 1.331345, 0, 0, 0.9595121, 0.2816674, 120, 255, 1), -- 146441 (Area: 1438)
(@OGUID+133, 233904, 0, 1, 2, -10945.96, -3212.922, 41.3475, 3.479053, 0, 0, 0, 1, 220, 255, 1), -- 233904 (Area: 1438)
(@OGUID+134, 146441, 0, 1, 2, -10950.17, -3463.397, 66.76239, 1.331345, 0, 0, 0.9595121, 0.2816674, 120, 255, 1), -- 146441 (Area: 1438)
(@OGUID+135, 37032, 0, 1, 2, -10908.47, -3397.46, 81.82459, 2.897245, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37032 (Area: 1438)
(@OGUID+136, 147282, 0, 1, 2, -10944.52, -3461.826, 66.76239, 3.364655, 0, 0, 0.9595121, 0.2816674, 120, 255, 1), -- 147282 (Area: 1438)
(@OGUID+137, 228509, 0, 1, 2, -10901.98, -3382.823, 66.49751, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 1438)
(@OGUID+138, 37032, 0, 1, 2, -10908.47, -3397.46, 81.82459, 2.897245, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37032 (Area: 1438)
(@OGUID+139, 147282, 0, 1, 2, -10944.52, -3461.826, 66.76239, 3.364655, 0, 0, 0.9595121, 0.2816674, 120, 255, 1), -- 147282 (Area: 1438)
(@OGUID+140, 228509, 0, 1, 2, -10901.98, -3382.823, 66.49751, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 1438)
(@OGUID+141, 228509, 0, 1, 2, -10885.65, -3413.092, 66.37199, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 1438)
(@OGUID+142, 228509, 0, 1, 2, -10887.59, -3396.536, 66.15986, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 1438)
(@OGUID+143, 3772, 0, 1, 2, -10897.59, -3377.261, 66.57138, 3.010666, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 3772 (Area: 1438)
(@OGUID+144, 228509, 0, 1, 2, -10887.59, -3396.536, 68.01679, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 1438)
(@OGUID+145, 37025, 0, 1, 2, -10900.06, -3376.071, 66.50545, 5.733408, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37025 (Area: 1438)
(@OGUID+146, 228509, 0, 1, 2, -10885.65, -3413.092, 66.37199, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 1438)
(@OGUID+147, 37027, 0, 1, 2, -10898.16, -3378.532, 66.50545, 2.330013, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37027 (Area: 1438)
(@OGUID+148, 37031, 0, 1, 2, -10898.11, -3375.746, 66.50545, 3.551751, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37031 (Area: 1438)
(@OGUID+149, 3772, 0, 1, 2, -10897.59, -3377.261, 66.57138, 3.010666, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 3772 (Area: 1438)
(@OGUID+150, 37025, 0, 1, 2, -10900.06, -3376.071, 66.50545, 5.733408, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37025 (Area: 1438)
(@OGUID+151, 228509, 0, 1, 2, -10877.51, -3426.257, 66.44753, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 1438)
(@OGUID+152, 37027, 0, 1, 2, -10898.16, -3378.532, 66.50545, 2.330013, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37027 (Area: 1438)
(@OGUID+153, 37031, 0, 1, 2, -10898.11, -3375.746, 66.50545, 3.551751, 0, 0, 0.9935719, 0.113203, 120, 255, 1), -- 37031 (Area: 1438)
(@OGUID+154, 228509, 0, 1, 2, -10877.51, -3426.257, 66.44753, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 1438)
(@OGUID+155, 228509, 0, 1, 2, -10914.53, -3512.939, 65.58915, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 1438)
(@OGUID+156, 228509, 0, 1, 2, -10914.53, -3512.939, 65.58915, 4.772956, 0, 0, 0, 1, 220, 255, 1), -- 228509 (Area: 1438)
(@OGUID+157, 147764, 0, 1, 2, -11216.8, -3516.592, 8.380037, 0.2462063, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147764 (Area: 1438)
(@OGUID+158, 147765, 0, 1, 2, -11211.41, -3497.146, 8.278817, 0.517717, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147765 (Area: 1438)
(@OGUID+159, 147769, 0, 1, 2, -11204.62, -3517.968, 6.585192, 5.1373, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147769 (Area: 1438)
(@OGUID+160, 147764, 0, 1, 2, -11216.8, -3516.592, 8.380037, 0.2462063, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147764 (Area: 1438)
(@OGUID+161, 147765, 0, 1, 2, -11211.41, -3497.146, 8.278817, 0.517717, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147765 (Area: 1438)
(@OGUID+162, 147769, 0, 1, 2, -11204.62, -3517.968, 6.585192, 5.1373, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147769 (Area: 1438)
(@OGUID+163, 147743, 0, 1, 2, -11239.04, -3527.447, 10.52678, 1.930074, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147743 (Area: 1438)
(@OGUID+164, 147763, 0, 1, 2, -11226.99, -3507.462, 12.32989, 3.55421, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147763 (Area: 1438)
(@OGUID+165, 147770, 0, 1, 2, -11232.89, -3560.618, 6.472799, 3.381158, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147770 (Area: 1438)
(@OGUID+166, 147771, 0, 1, 2, -11251.13, -3570.117, 7.732521, 4.539662, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147771 (Area: 1438)
(@OGUID+167, 147763, 0, 1, 2, -11226.99, -3507.462, 12.32989, 3.55421, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147763 (Area: 1438)
(@OGUID+168, 147770, 0, 1, 2, -11232.89, -3560.618, 6.472799, 3.381158, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147770 (Area: 1438)
(@OGUID+169, 147771, 0, 1, 2, -11251.13, -3570.117, 7.732521, 4.539662, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147771 (Area: 1438)
(@OGUID+170, 147755, 0, 1, 2, -11248.42, -3556.76, 8.125772, 4.554123, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147755 (Area: 1438)
(@OGUID+171, 147755, 0, 1, 2, -11248.42, -3556.76, 8.125772, 4.554123, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147755 (Area: 1438)
(@OGUID+172, 147743, 0, 1, 2, -11239.04, -3527.447, 10.52678, 1.930074, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147743 (Area: 1438)
-- (@OGUID+173, 188215, 0, 1, 2, -11259.98, -3624.955, 12.56676, 5.399792, 0, 0, 0, 1, 220, 255, 1), -- 188215 (Area: 0) - !!! might be temporary spawn !!!
(@OGUID+173, 147753, 0, 1, 2, -11264.91, -3553.192, 8.125773, 4.512619, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147753 (Area: 0)
(@OGUID+174, 147753, 0, 1, 2, -11264.91, -3553.192, 8.125773, 4.512619, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147753 (Area: 0)
(@OGUID+175, 147760, 0, 1, 2, -11253.57, -3529.399, 8.983669, 4.063222, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147760 (Area: 0)
(@OGUID+176, 147760, 0, 1, 2, -11253.57, -3529.399, 8.983669, 4.063222, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147760 (Area: 0)
(@OGUID+177, 1685, 0, 1, 2, -11291.79, -3628.212, 9.697325, 3.539459, 0, 0, 0, 1, 220, 255, 1), -- 1685 (Area: 0)
(@OGUID+178, 225908, 0, 1, 2, -11288.69, -3628.887, 10.01777, 0.7198139, 0, 0, 0, 1, 220, 255, 1), -- 225908 (Area: 0)
(@OGUID+179, 226752, 0, 1, 2, -11282.86, -3628.724, 10.28657, 6.248003, 0, 0, 0, 1, 220, 255, 1), -- 226752 (Area: 0)
(@OGUID+180, 226633, 0, 1, 2, -11283.07, -3637.455, 8.468307, 2.330012, 0, 0, 0, 1, 220, 255, 1), -- 226633 (Area: 0)
(@OGUID+181, 176584, 0, 1, 2, -11268.5, -3638.72, 8.40649, 0, 0, 0, 0, 1, 220, 255, 1), -- 176584 (Area: 0)
(@OGUID+182, 226707, 0, 1, 2, -11326.4, -3649.759, 6.841159, 1.708303, 0, 0, 0, 1, 220, 255, 1), -- 226707 (Area: 0)
(@OGUID+183, 233393, 0, 1, 2, -11328.94, -3645.514, 6.80382, 1.609814, 0, 0, 0, 1, 220, 255, 1), -- 233393 (Area: 0)
(@OGUID+184, 226707, 0, 1, 2, -11353.37, -3655.043, 4.066323, 1.007447, 0, 0, 0, 1, 220, 255, 1), -- 226707 (Area: 6953)
(@OGUID+185, 233393, 0, 1, 2, -11357.27, -3651.47, 4.135417, 1.882146, 0, 0, 0, 1, 220, 255, 1), -- 233393 (Area: 6953)
(@OGUID+186, 233393, 0, 1, 2, -11343.18, -3646.778, 4.894097, 1.798645, 0, 0, 0, 1, 220, 255, 1), -- 233393 (Area: 6953)
(@OGUID+187, 226707, 0, 1, 2, -11339.48, -3650.509, 5.208849, 1.853433, 0, 0, 0, 1, 220, 255, 1), -- 226707 (Area: 6953)
(@OGUID+188, 226707, 0, 1, 2, -11362.23, -3688.052, 3.980903, 3.050315, 0, 0, 0, 1, 220, 255, 1), -- 226707 (Area: 6953)
(@OGUID+189, 226707, 0, 1, 2, -11363.5, -3702.939, 1.548611, 2.273482, 0, 0, 0, 1, 220, 255, 1), -- 226707 (Area: 6953)
(@OGUID+190, 176583, 0, 1, 2, -11370.9, -3728.03, -0.12302, 0, 0, 0, 0, 1, 220, 255, 1), -- 176583 (Area: 6953)
(@OGUID+191, 233393, 0, 1, 2, -11368.08, -3705.575, 1.172095, 3.148359, 0, 0, 0, 1, 220, 255, 1), -- 233393 (Area: 6953)
(@OGUID+192, 233393, 0, 1, 2, -11367.05, -3690.132, 4.130208, 2.995558, 0, 0, 0, 1, 220, 255, 1), -- 233393 (Area: 6953)
(@OGUID+193, 176584, 0, 1, 2, -11411.1, -3670.02, 0.913928, 0, 0, 0, 0, 1, 220, 255, 1), -- 176584 (Area: 5077)
(@OGUID+194, 176584, 0, 1, 2, -11589.5, -3650.98, 7.92109, 0, 0, 0, 0, 1, 220, 255, 1), -- 176584 (Area: 5077)
(@OGUID+195, 226707, 0, 1, 2, -11568.11, -3690.868, 4.059028, 6.088908, 0, 0, 0, 1, 220, 255, 1), -- 226707 (Area: 5077)
(@OGUID+196, 226707, 0, 1, 2, -11565.96, -3673.063, 5.567362, 5.424788, 0, 0, 0, 1, 220, 255, 1), -- 226707 (Area: 5077)
(@OGUID+197, 233392, 0, 1, 2, -11563.03, -3689.611, 4.277794, 5.990414, 0, 0, 0, 1, 220, 255, 1), -- 233392 (Area: 5077)
(@OGUID+198, 233392, 0, 1, 2, -11561.31, -3670.417, 5.880208, 0.0163042, 0, 0, 0, 1, 220, 255, 1), -- 233392 (Area: 5077)
(@OGUID+199, 226707, 0, 1, 2, -11583.17, -3633.168, 8.220487, 0.644667, 0, 0, 0, 1, 220, 255, 1), -- 226707 (Area: 5077)
(@OGUID+200, 233392, 0, 1, 2, -11580.99, -3628.401, 9.16493, 0.5898768, 0, 0, 0, 1, 220, 255, 1), -- 233392 (Area: 5077)
(@OGUID+201, 233392, 0, 1, 2, -11571.31, -3646.516, 10.00932, 0.4010414, 0, 0, 0, 1, 220, 255, 1), -- 233392 (Area: 5077)
(@OGUID+202, 226707, 0, 1, 2, -11574.06, -3650.623, 9.775006, 0.4995363, 0, 0, 0, 1, 220, 255, 1), -- 226707 (Area: 5077)
(@OGUID+203, 226707, 0, 1, 2, -11594.87, -3613.8, 12.5, 6.081862, 0, 0, 0, 1, 220, 255, 1), -- 226707 (Area: 5077)
-- (@OGUID+204, 233819, 0, 1, 2, -11489.11, -3673.903, -1.947126, 3.906772, 0, 0, 0, 1, 220, 255, 1), -- 233819 (Area: 5077) - !!! might be temporary spawn !!!
-- (@OGUID+204, 233819, 0, 1, 2, -11511.46, -3618.236, 0.5470705, 4.03848, 0, 0, 0, 1, 220, 255, 1), -- 233819 (Area: 5077) - !!! might be temporary spawn !!!
(@OGUID+204, 233392, 0, 1, 2, -11592.9, -3608.901, 13.72743, 0.6733769, 0, 0, 0, 1, 220, 255, 1), -- 233392 (Area: 5077)
-- (@OGUID+205, 233819, 0, 1, 2, -11523.16, -3586.556, 4.681293, 4.362336, 0, 0, 0, 1, 220, 255, 1), -- 233819 (Area: 6963) - !!! might be temporary spawn !!!
-- (@OGUID+205, 233819, 0, 1, 2, -11555.96, -3566.899, 8.740532, 4.262506, 0, 0, 0, 1, 220, 255, 1), -- 233819 (Area: 6963) - !!! might be temporary spawn !!!
(@OGUID+205, 233914, 0, 1, 2, -11591.47, -3425.073, 25.80853, 5.093589, 0, 0, 0, 1, 220, 255, 1), -- 233914 (Area: 6963)
(@OGUID+206, 324, 0, 1, 2, -11642.4, -3534.97, 35.3055, 1.361356, 0, 0, 0, 1, 220, 255, 1), -- 324 (Area: 6963)
(@OGUID+207, 324, 0, 1, 2, -11495.8, -3482.05, 21.2096, 1.361356, 0, 0, 0, 1, 220, 255, 1), -- 324 (Area: 6963)
(@OGUID+208, 233913, 0, 1, 2, -11530.53, -3396.986, 21.66182, 5.625557, 0, 0, 0, 1, 220, 255, 1), -- 233913 (Area: 6963)
(@OGUID+209, 233911, 0, 1, 2, -11546.52, -3372.13, 8.782513, 6.149914, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 6963)
(@OGUID+210, 233911, 0, 1, 2, -11576.48, -3396.729, 7.535768, 5.309609, 0, 0, 0, 1, 220, 255, 1), -- 233911 (Area: 6963)
-- (@OGUID+211, 233819, 0, 1, 2, -11396.87, -3598.366, 0.206152, 5.499783, 0, 0, 0, 1, 220, 255, 1), -- 233819 (Area: 6963) - !!! might be temporary spawn !!!
-- (@OGUID+211, 233819, 0, 1, 2, -11459.15, -3671.717, -3.766034, 5.407629, 0, 0, 0, 1, 220, 255, 1), -- 233819 (Area: 6963) - !!! might be temporary spawn !!!
-- (@OGUID+211, 233819, 0, 1, 2, -11450.98, -3635.455, -0.9315901, 5.714775, 0, 0, 0, 1, 220, 255, 1), -- 233819 (Area: 6963) - !!! might be temporary spawn !!!
-- (@OGUID+211, 233819, 0, 1, 2, -11364.09, -3572.399, 5.921197, 5.081178, 0, 0, 0, 1, 220, 255, 1), -- 233819 (Area: 6963) - !!! might be temporary spawn !!!
-- (@OGUID+211, 233819, 0, 1, 2, -11418.91, -3588.806, 2.113873, 4.209538, 0, 0, 0, 1, 220, 255, 1), -- 233819 (Area: 6963) - !!! might be temporary spawn !!!
(@OGUID+211, 147761, 0, 1, 2, -11255.15, -3519.786, 8.914312, 3.561114, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147761 (Area: 6963)
(@OGUID+212, 147762, 0, 1, 2, -11269.9, -3524.736, 7.442627, 3.712646, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147762 (Area: 6963)
(@OGUID+213, 147761, 0, 1, 2, -11255.15, -3519.786, 8.914312, 3.561114, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147761 (Area: 6963)
(@OGUID+214, 147762, 0, 1, 2, -11269.9, -3524.736, 7.442627, 3.712646, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147762 (Area: 6963)
(@OGUID+215, 147749, 0, 1, 2, -11280.33, -3499.871, 9.854815, 3.312984, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147749 (Area: 6963)
(@OGUID+216, 147749, 0, 1, 2, -11280.33, -3499.871, 9.854815, 3.312984, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147749 (Area: 6963)
(@OGUID+217, 147759, 0, 1, 2, -11264.43, -3495.44, 9.975563, 5.875692, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147759 (Area: 6963)
(@OGUID+218, 147759, 0, 1, 2, -11264.43, -3495.44, 9.975563, 5.875692, 0, 0, 0.8808908, 0.4733195, 120, 255, 1), -- 147759 (Area: 6963)
(@OGUID+219, 233910, 0, 1, 2, -11596.07, -3262.686, 7.429289, 3.375972, 0, 0, 0, 1, 220, 255, 1), -- 233910 (Area: 0)
-- (@OGUID+220, 188215, 0, 1, 2, -11785.51, -3815.554, 56.97623, 0.4786584, 0, 0, 0, 1, 220, 255, 1), -- 188215 (Area: 0) - !!! might be temporary spawn !!!
(@OGUID+220, 324, 0, 1, 2, -11818.7, -3758.69, 65.1472, 1.361356, 0, 0, 0, 1, 220, 255, 1), -- 324 (Area: 0)
(@OGUID+221, 225730, 0, 1, 2, -11782.73, -3831.068, 56.61763, 4.049168, 0, 0, 0, 1, 220, 255, 1), -- 225730 (Area: 0)
(@OGUID+222, 235880, 0, 1, 2, -11791.94, -3854.641, 59.96929, 1.51902, 0, 0, 0, 1, 220, 255, 1), -- 235880 (Area: 6954)
(@OGUID+223, 324, 0, 1, 2, -11982.2, -3693.37, 53.6438, 1.361356, 0, 0, 0, 1, 220, 255, 1), -- 324 (Area: 0)
(@OGUID+224, 176586, 0, 1, 2, -11999.1, -3866.55, 58.5078, 0, 0, 0, 0, 1, 220, 255, 1), -- 176586 (Area: 5078)
(@OGUID+225, 176583, 0, 1, 2, -12116.7, -3823.96, 0.845268, 0, 0, 0, 0, 1, 220, 255, 1), -- 176583 (Area: 5078)
(@OGUID+226, 176583, 0, 1, 2, -12198.4, -3780.97, 5.03422, 0, 0, 0, 0, 1, 220, 255, 1), -- 176583 (Area: 5079)
(@OGUID+227, 176584, 0, 1, 2, -12175.9, -3733.05, 5.68798, 0, 0, 0, 0, 1, 220, 255, 1), -- 176584 (Area: 5079)
(@OGUID+228, 176583, 0, 1, 2, -12352.1, -3369.66, 19.3149, 0, 0, 0, 0, 1, 220, 255, 1), -- 176583 (Area: 5078)
(@OGUID+229, 176584, 0, 1, 2, -12389.3, -3357.88, 20.0128, 0, 0, 0, 0, 1, 220, 255, 1), -- 176584 (Area: 5078)
(@OGUID+230, 2047, 0, 1, 2, -12504.9, -2889.71, 20.7019, 0, 0, 0, 0, 1, 220, 255, 1); -- 2047 (Area: 0)

--
SET @CGUID = 995032;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+823;
DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+823;
DELETE FROM `waypoint_data` WHERE `id` BETWEEN @CGUID+0 AND @CGUID+823;

DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+823;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@CGUID+0, 76556, 0, 1, 2, -11836.73, -3188.783, -29.92892, 0.505581, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+1, 76556, 0, 1, 2, -11834.76, -3194.021, -30.33371, 0.3588338, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+2, 76556, 0, 1, 2, -11836.73, -3188.783, -29.92892, 0.505581, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+3, 76556, 0, 1, 2, -11836.73, -3188.783, -29.92892, 0.505581, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+4, 76556, 0, 1, 2, -11841.54, -3208.209, -29.82479, 4.33623, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+5, 76556, 0, 1, 2, -11835.59, -3199.745, -30.71066, 6.233938, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+6, 76556, 0, 1, 2, -11835.08, -3205.361, -30.54158, 6.19841, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+7, 76556, 0, 1, 2, -11834.58, -3213.292, -30.41072, 5.832369, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+8, 76556, 0, 1, 2, -11835.29, -3200.792, -30.6761, 4.990341, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+9, 76556, 0, 1, 2, -11843.75, -3191.666, -29.30392, 1.051593, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+10, 76556, 0, 1, 2, -11835.59, -3199.745, -30.71066, 3.458951, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+11, 76556, 0, 1, 2, -11841.15, -3207.203, -29.94539, 5.096207, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+12, 76556, 0, 1, 2, -11834.76, -3194.021, -30.33371, 0.4976151, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+13, 76556, 0, 1, 2, -11836.73, -3188.783, -29.92892, 0.505581, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+14, 76556, 0, 1, 2, -11835.59, -3199.745, -30.71066, 6.233938, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+15, 76556, 0, 1, 2, -11843.29, -3203.785, -29.99739, 5.730447, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+16, 76556, 0, 1, 2, -11841.37, -3203.347, -29.92474, 4.607256, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+17, 76556, 0, 1, 2, -11834.58, -3213.292, -30.41072, 5.832369, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+18, 76556, 0, 1, 2, -11843.75, -3191.666, -29.30392, 1.051593, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+19, 76556, 0, 1, 2, -11841.15, -3207.203, -29.94539, 5.67464, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+20, 76556, 0, 1, 2, -11834.58, -3213.292, -30.41072, 5.832369, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+21, 76556, 0, 1, 2, -11841.14, -3200.358, -29.85286, 1.614859, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+22, 76556, 0, 1, 2, -11843.41, -3196.955, -29.6572, 2.09324, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+23, 76556, 0, 1, 2, -11838.97, -3200.98, -30.33242, 6.263301, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+24, 76556, 0, 1, 2, -11841.15, -3207.203, -29.94539, 1.642768, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+25, 76556, 0, 1, 2, -11833.08, -3206.207, -30.5785, 4.934079, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+26, 76556, 0, 1, 2, -11835.08, -3205.361, -30.54158, 6.277085, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+27, 76556, 0, 1, 2, -11843.75, -3191.666, -29.30392, 1.051593, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+28, 76556, 0, 1, 2, -11834.58, -3213.292, -30.41072, 5.832369, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+29, 76556, 0, 1, 2, -11834.13, -3193.941, -30.32192, 0.127164, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+30, 76556, 0, 1, 2, -11834.54, -3193.37, -30.30392, 1.248288, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+31, 76556, 0, 1, 2, -11843.75, -3191.666, -29.30392, 1.051593, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+32, 82014, 0, 1, 2, -11828.99, -3206.884, -30.29663, 2.979622, 120, 0, 0), -- 82014 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+33, 78696, 0, 1, 2, -11893.42, -3194.116, -10.84945, 0.0868787, 120, 0, 0), -- 78696 (Area: 72) (Auras: 160145 - 160145)
(@CGUID+34, 82015, 0, 1, 2, -11840.82, -3201.462, -30.06453, 2.979743, 120, 0, 0), -- 82015 (Area: 72) (Auras: 147617 - 147617, 165754 - 165754)
(@CGUID+35, 81997, 0, 1, 2, -11837.52, -3189.804, -29.86374, 3.58933, 120, 0, 0), -- 81997 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+36, 79315, 0, 1, 2, -11841.31, -3205.009, -30.20882, 2.962795, 120, 0, 0), -- 79315 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+37, 82189, 0, 1, 2, -11839.41, -3193.59, -29.88693, 1.062071, 120, 0, 0), -- 82189 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+38, 82263, 0, 1, 2, -11838.65, -3191.033, -29.825, 3.307272, 120, 0, 0), -- 82263 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+39, 78674, 0, 1, 2, -11735.62, -3201.812, -9.941172, 2.550843, 120, 5, 1), -- 78674 (Area: 72) (possible waypoints or random movement)
(@CGUID+40, 82000, 0, 1, 2, -11841.62, -3185.512, -29.13052, 3.660774, 120, 0, 0), -- 82000 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+41, 82187, 0, 1, 2, -11836.24, -3210.622, -30.25174, 2.714649, 120, 0, 0), -- 82187 (Area: 72) (Auras: 147617 - 147617, 165750 - 165750)
(@CGUID+42, 81998, 0, 1, 2, -11843.28, -3188.865, -29.09936, 3.545804, 120, 0, 0), -- 81998 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+43, 78667, 0, 1, 2, -11771.17, -3276.118, -16.88611, 2.320298, 120, 5, 1), -- 78667 (Area: 72) (possible waypoints or random movement)
(@CGUID+44, 82075, 0, 1, 2, -11834.74, -3216.401, -30.22296, 2.642238, 120, 0, 0), -- 82075 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+45, 76648, 0, 1, 2, -11889.87, -3218.073, -10.84924, 0.04351285, 120, 0, 0), -- 76648 (Area: 72) (Auras: 160145 - 160145)
(@CGUID+46, 78568, 0, 1, 2, -11833.38, -3189.892, -30.30392, 3.6072, 120, 0, 0), -- 78568 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+47, 82191, 0, 1, 2, -11834.52, -3206.919, -30.41756, 2.670381, 120, 0, 0), -- 82191 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+48, 78569, 0, 1, 2, -11834.86, -3189.103, -30.22131, 3.689269, 120, 0, 0), -- 78569 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+49, 78554, 0, 1, 2, -11811.88, -3203.59, -29.46795, 3.358216, 120, 0, 0), -- 78554 (Area: 72) (Auras: 165746 - 165746)
(@CGUID+50, 78556, 0, 1, 2, -11834.17, -3210.813, -30.4511, 1.339758, 120, 0, 0), -- 78556 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+51, 82013, 0, 1, 2, -11839.79, -3210.555, -29.9095, 2.914625, 120, 0, 0), -- 82013 (Area: 72) (Auras: 147617 - 147617, 165754 - 165754)
(@CGUID+52, 78674, 0, 1, 2, -11876.79, -3192.274, -14.21498, 6.168885, 120, 0, 0), -- 78674 (Area: 72)
(@CGUID+53, 79316, 0, 1, 2, -11833.81, -3193.505, -30.34882, 3.466703, 120, 0, 0), -- 79316 (Area: 72) (Auras: 147617 - 147617, 32356 - 32356)
(@CGUID+54, 82188, 0, 1, 2, -11835.34, -3201.052, -30.81432, 3.242837, 120, 0, 0), -- 82188 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+55, 82006, 0, 1, 2, -11841.97, -3209.011, -29.69477, 2.812948, 120, 0, 0), -- 82006 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+56, 82007, 0, 1, 2, -11833.24, -3213.333, -30.40174, 2.874652, 120, 0, 0), -- 82007 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+57, 82016, 0, 1, 2, -11843.23, -3212.477, -29.35899, 1.194637, 120, 0, 0), -- 82016 (Area: 72) (Auras: 147617 - 147617, 165713 - 165713, 165712 - 165712)
(@CGUID+58, 82011, 0, 1, 2, -11829.79, -3203.938, -30.65783, 2.79148, 120, 0, 0), -- 82011 (Area: 72) (Auras: 147617 - 147617, 165767 - 165767)
(@CGUID+59, 78430, 0, 1, 2, -11811.49, -3201.297, -29.95002, 3.295083, 120, 0, 0), -- 78430 (Area: 72)
(@CGUID+60, 81994, 0, 1, 2, -11836.14, -3195.829, -30.11691, 3.455701, 120, 0, 0), -- 81994 (Area: 72) (Auras: 147617 - 147617, 165747 - 165747, 165746 - 165746)
(@CGUID+61, 78667, 0, 1, 2, -11868.42, -3194.891, -20.20026, 0.1542269, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+62, 82002, 0, 1, 2, -11836.89, -3185.558, -29.68918, 3.569637, 120, 0, 0), -- 82002 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+63, 82003, 0, 1, 2, -11829.86, -3196.797, -30.67625, 3.28783, 120, 0, 0), -- 82003 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+64, 78670, 0, 1, 2, -11860.11, -3214.328, -22.01482, 0.2130593, 120, 0, 0), -- 78670 (Area: 72)
(@CGUID+65, 81999, 0, 1, 2, -11830.5, -3193.477, -30.42655, 3.268757, 120, 0, 0), -- 81999 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+66, 82264, 0, 1, 2, -11839.31, -3197.861, -29.89265, 3.442991, 120, 0, 0), -- 82264 (Area: 72) (Auras: 147617 - 147617, 165723 - 165723)
(@CGUID+67, 82025, 0, 1, 2, -11831.26, -3189.05, -30.51012, 3.092938, 120, 0, 0), -- 82025 (Area: 72) (Auras: 147617 - 147617, 165754 - 165754)
(@CGUID+68, 79675, 0, 1, 2, -11811.17, -3211.63, -30.9785, 3.190637, 120, 0, 0), -- 79675 (Area: 72)
(@CGUID+69, 82005, 0, 1, 2, -11838.41, -3217.273, -29.60508, 2.769619, 120, 0, 0), -- 82005 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+70, 78553, 0, 1, 2, -11811.51, -3207.884, -29.53826, 3.295763, 120, 0, 0), -- 78553 (Area: 72)
(@CGUID+71, 78667, 0, 1, 2, -11736.22, -3198.872, -11.16282, 2.555497, 120, 5, 1), -- 78667 (Area: 72) (possible waypoints or random movement)
(@CGUID+72, 82082, 0, 1, 2, -11832.34, -3217.099, -30.42728, 2.70513, 120, 0, 0), -- 82082 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+73, 82001, 0, 1, 2, -11846.24, -3188.087, -28.82565, 3.629495, 120, 0, 0), -- 82001 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+74, 82260, 0, 1, 2, -11831.47, -3212.589, -30.4015, 2.740582, 120, 0, 0), -- 82260 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+75, 81996, 0, 1, 2, -11839.89, -3188.342, -29.42324, 3.639489, 120, 0, 0), -- 81996 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+76, 78667, 0, 1, 2, -11730.2, -3236.613, 7.16202, 2.856164, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+77, 82004, 0, 1, 2, -11841.31, -3196.703, -29.71914, 3.162749, 120, 0, 0), -- 82004 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+78, 78423, 0, 1, 2, -11812.26, -3205.597, -29.47917, 3.093543, 120, 0, 0), -- 78423 (Area: 72) (Auras: 128281 - 128281)
(@CGUID+79, 82009, 0, 1, 2, -11831.52, -3200.262, -30.8214, 3.014491, 120, 0, 0), -- 82009 (Area: 72) (Auras: 147617 - 147617, 165739 - 165739)
(@CGUID+80, 78667, 0, 1, 2, -11749.52, -3266.783, 6.433348, 2.677412, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+81, 82008, 0, 1, 2, -11832.27, -3209.781, -30.45945, 2.939795, 120, 0, 0), -- 82008 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+82, 81993, 0, 1, 2, -11842.25, -3193.478, -29.4509, 3.577764, 120, 0, 0), -- 81993 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+83, 81990, 0, 1, 2, -11835.77, -3193.201, -30.1987, 3.361897, 120, 0, 0), -- 81990 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+84, 82010, 0, 1, 2, -11834.58, -3203.27, -30.64263, 1.848784, 120, 0, 0), -- 82010 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+85, 81995, 0, 1, 2, -11834.72, -3198.245, -30.51241, 3.53997, 120, 0, 0), -- 81995 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+86, 82017, 0, 1, 2, -11840.64, -3215.719, -29.41927, 2.84771, 120, 0, 0), -- 82017 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+87, 82012, 0, 1, 2, -11837.36, -3207.628, -30.24282, 2.932442, 120, 0, 0), -- 82012 (Area: 72) (Auras: 147617 - 147617, 165754 - 165754)
(@CGUID+88, 78674, 0, 1, 2, -11867.86, -3222.91, -14.94012, 0.4335482, 120, 0, 0), -- 78674 (Area: 72)
(@CGUID+89, 82259, 0, 1, 2, -11838.13, -3213.793, -29.98421, 2.692714, 120, 0, 0), -- 82259 (Area: 72) (Auras: 147617 - 147617)
(@CGUID+90, 76556, 0, 1, 2, -11880.69, -3207.257, -15.89928, 0.3042077, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+91, 76556, 0, 1, 2, -11888.55, -3203.41, -13.52039, 0.021704, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+92, 76556, 0, 1, 2, -11880.64, -3204.018, -16.06746, 6.250019, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+93, 76556, 0, 1, 2, -11878.51, -3201.759, -16.99099, 0.1138861, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+94, 76556, 0, 1, 2, -11879.16, -3207.92, -16.48948, 0.1461763, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+95, 76556, 0, 1, 2, -11878.4, -3200.339, -16.93636, 0.1254863, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+96, 76556, 0, 1, 2, -11892.61, -3211.117, -11.77113, 0.1574257, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+97, 76556, 0, 1, 2, -11896.15, -3205.133, -11.61089, 0.156449, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+98, 76556, 0, 1, 2, -11895.4, -3208.172, -11.59763, 0.1485685, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+99, 76556, 0, 1, 2, -11872.1, -3203.799, -19.75873, 0.1417108, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+100, 76556, 0, 1, 2, -11889.53, -3210.628, -12.86461, 0.1573479, 120, 5, 1), -- 76556 (Area: 72) (Auras: ) (possible waypoints or random movement)
(@CGUID+101, 76556, 0, 1, 2, -11891.11, -3210.719, -12.38029, 0.2408447, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+102, 78667, 0, 1, 2, -11726.28, -3253.512, 5.708197, 2.765456, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+103, 76556, 0, 1, 2, -11874.97, -3199.906, -18.37715, 0.2726863, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+104, 76556, 0, 1, 2, -11896.7, -3203.878, -11.61636, 0.1287902, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+105, 76556, 0, 1, 2, -11875.17, -3199.931, -18.27311, 0.125487, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+106, 76556, 0, 1, 2, -11883.21, -3209.627, -14.9695, 0.157341, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+107, 76556, 0, 1, 2, -11866.28, -3202.968, -22.27836, 0.1417151, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+108, 76556, 0, 1, 2, -11866.04, -3205.221, -22.30756, 0.2315027, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+109, 78667, 0, 1, 2, -11741.54, -3130.899, -11.68879, 3.757757, 120, 5, 1), -- 78667 (Area: 72) (possible waypoints or random movement)
(@CGUID+110, 76556, 0, 1, 2, -11866.4, -3201.505, -22.27728, 0.149566, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+111, 76556, 0, 1, 2, -11875.53, -3205.637, -18.08854, 0.1278887, 120, 5, 1), -- 76556 (Area: 72) (Auras: ) (possible waypoints or random movement)
(@CGUID+112, 76556, 0, 1, 2, -11884.37, -3201.092, -14.85052, 0.1254755, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+113, 76556, 0, 1, 2, -11895.92, -3206.899, -11.60417, 0.1503888, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+114, 76556, 0, 1, 2, -11871.85, -3207.859, -19.60093, 0.1451495, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+115, 76556, 0, 1, 2, -11893.65, -3209.022, -11.48715, 4.8357, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+116, 76556, 0, 1, 2, -11866.36, -3201.5, -22.29337, 0.149566, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+117, 78667, 0, 1, 2, -11786.51, -3296.34, 9.233499, 2.095769, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+118, 76556, 0, 1, 2, -11881.37, -3202.085, -15.89276, 0.1138946, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+119, 76556, 0, 1, 2, -11879.11, -3207.913, -16.51097, 0.1461763, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+120, 78667, 0, 1, 2, -11792.54, -3298.79, 7.554691, 2.016864, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+121, 76556, 0, 1, 2, -11905.59, -3208.365, -11.63638, 0.1503888, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+122, 76556, 0, 1, 2, -11906.14, -3203.108, -11.5685, 0.05748955, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+123, 76556, 0, 1, 2, -11905.96, -3206.681, -11.64358, 0.156449, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+124, 76556, 0, 1, 2, -11904.43, -3212.773, -11.52012, 0.1943905, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+125, 76556, 0, 1, 2, -11904.82, -3211.245, -11.56851, 0.1736725, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+126, 76556, 0, 1, 2, -11905.03, -3209.613, -11.61907, 0.1485685, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+127, 76556, 0, 1, 2, -11906.18, -3205.106, -11.64383, 0.1287902, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+128, 76556, 0, 1, 2, -11905.59, -3208.365, -11.63638, 0.1503888, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+129, 76556, 0, 1, 2, -11905.03, -3209.613, -11.61907, 0.1485685, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+130, 78667, 0, 1, 2, -11803.26, -3082.297, 9.511176, 4.362135, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+131, 78667, 0, 1, 2, -11756.98, -3076.264, 9.852929, 4.170965, 120, 5, 1), -- 78667 (Area: 72) (possible waypoints or random movement)
(@CGUID+132, 78667, 0, 1, 2, -11748.7, -3110.516, 9.891431, 3.861171, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+133, 78667, 0, 1, 2, -11707.35, -3133.196, 10.29497, 3.988894, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+134, 78667, 0, 1, 2, -11710.34, -3133.408, 9.86548, 3.997818, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+135, 78667, 0, 1, 2, -11838.32, -3070.201, 7.520038, 4.517249, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+136, 78674, 0, 1, 2, -11811.1, -3043.34, 11.02239, 3.00625, 120, 0, 0), -- 78674 (Area: 72)
(@CGUID+137, 78667, 0, 1, 2, -11809.29, -3045.729, 10.49138, 3.004374, 120, 5, 1), -- 78667 (Area: 72) (possible waypoints or random movement)
(@CGUID+138, 78667, 0, 1, 2, -11815.34, -3062.037, 6.701705, 4.539529, 120, 5, 1), -- 78667 (Area: 72) (possible waypoints or random movement)
(@CGUID+139, 78667, 0, 1, 2, -11808.71, -3041.525, 11.90251, 3.007687, 120, 5, 1), -- 78667 (Area: 72) (possible waypoints or random movement)
(@CGUID+140, 78674, 0, 1, 2, -11752.08, -3027.469, 24.04373, 5.808526, 120, 5, 1), -- 78674 (Area: 72) (possible waypoints or random movement)
(@CGUID+141, 78667, 0, 1, 2, -11727.4, -3032.664, 21.78611, 2.814013, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+142, 78670, 0, 1, 2, -11666.03, -3114.137, 21.07727, 4.880967, 120, 0, 0), -- 78670 (Area: 72)
(@CGUID+143, 78670, 0, 1, 2, -11721.4, -3036.946, 21.06557, 2.732864, 120, 0, 0), -- 78670 (Area: 72)
(@CGUID+144, 78674, 0, 1, 2, -11665.51, -3117.082, 21.07727, 1.741103, 120, 0, 0), -- 78674 (Area: 72)
(@CGUID+145, 78667, 0, 1, 2, -11749.22, -3026.589, 24.26712, 5.79577, 120, 5, 1), -- 78667 (Area: 72) (possible waypoints or random movement)
(@CGUID+146, 78667, 0, 1, 2, -11680.42, -3073.616, 25.61748, 2.491333, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+147, 78667, 0, 1, 2, -11871.9, -3026.269, 18.48073, 4.005923, 120, 5, 1), -- 78667 (Area: 72) (possible waypoints or random movement)
(@CGUID+148, 15476, 0, 1, 2, -11742.1, -2958.291, 7.941827, 5.654056, 120, 0, 0), -- 15476 (Area: 72)
(@CGUID+149, 78667, 0, 1, 2, -11836.29, -3011.877, 17.03226, 3.317804, 120, 5, 1), -- 78667 (Area: 72) (possible waypoints or random movement)
(@CGUID+150, 78670, 0, 1, 2, -11870.16, -3036.455, 18.2267, 4.674741, 120, 5, 1), -- 78670 (Area: 72) (possible waypoints or random movement)
(@CGUID+151, 78670, 0, 1, 2, -11833.83, -3013.593, 17.46488, 3.316538, 120, 0, 0), -- 78670 (Area: 72)
(@CGUID+152, 78667, 0, 1, 2, -11867.66, -3026.429, 18.48073, 3.932634, 120, 5, 1), -- 78667 (Area: 72) (possible waypoints or random movement)
(@CGUID+153, 78667, 0, 1, 2, -11707.41, -2953.132, 7.239758, 4.892931, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+154, 78667, 0, 1, 2, -11707.61, -2946.104, 7.527085, 5.043636, 120, 0, 0), -- 78667 (Area: 72)
(@CGUID+155, 78667, 0, 1, 2, -11719.33, -2897.165, 4.442914, 1.838295, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+156, 78193, 0, 1, 2, -11679.96, -2986.889, 8.494791, 0.8791899, 120, 0, 0), -- 78193 (Area: 0)
(@CGUID+157, 78193, 0, 1, 2, -11675.21, -2983.217, 8.205517, 3.095433, 120, 0, 0), -- 78193 (Area: 0)
(@CGUID+158, 78667, 0, 1, 2, -11670.49, -2992.518, 8.155436, 2.055528, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+159, 78667, 0, 1, 2, -11674.12, -2994.711, 8.280436, 2.05477, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+160, 78670, 0, 1, 2, -11673.4, -2991.799, 8.280436, 2.114016, 120, 0, 0), -- 78670 (Area: 0)
(@CGUID+161, 78667, 0, 1, 2, -11715.22, -2896.117, 4.206098, 1.837656, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+162, 78674, 0, 1, 2, -11719.88, -2886.399, 4.043988, 1.820241, 120, 0, 0), -- 78674 (Area: 0)
(@CGUID+163, 78667, 0, 1, 2, -11723.35, -2814.191, 6.231262, 4.388182, 120, 5, 1), -- 78667 (Area: 0) (possible waypoints or random movement)
(@CGUID+164, 78667, 0, 1, 2, -11727.27, -2812.561, 6.455884, 4.38926, 120, 5, 1), -- 78667 (Area: 0) (possible waypoints or random movement)
(@CGUID+165, 78670, 0, 1, 2, -11643.7, -2875.112, 4.63923, 0.9467638, 120, 0, 0), -- 78670 (Area: 0)
(@CGUID+166, 78670, 0, 1, 2, -11726.22, -2815.69, 6.460514, 4.325727, 120, 0, 0), -- 78670 (Area: 0)
(@CGUID+167, 61328, 0, 1, 2, -11682.03, -2831.302, 4.758983, 1.368426, 120, 5, 1), -- 61328 (Area: 0) (possible waypoints or random movement)
(@CGUID+168, 78667, 0, 1, 2, -11753.21, -2768.816, 7.953737, 5.930724, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+169, 78667, 0, 1, 2, -11751.65, -2771.377, 8.016237, 6.045774, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+170, 78667, 0, 1, 2, -11790.28, -2819.759, 8.830951, 5.473446, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+171, 78670, 0, 1, 2, -11792.1, -2814.773, 8.517595, 5.354436, 120, 0, 0), -- 78670 (Area: 0)
(@CGUID+172, 78667, 0, 1, 2, -11675.25, -2811.873, 5.535436, 2.192551, 120, 5, 1), -- 78667 (Area: 0) (possible waypoints or random movement)
(@CGUID+173, 78674, 0, 1, 2, -11676.89, -2818.351, 5.690167, 1.676958, 120, 5, 1), -- 78674 (Area: 0) (possible waypoints or random movement)
(@CGUID+174, 5983, 0, 1, 2, -11834.5, -2852.552, 154.8423, 5.202392, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+175, 78667, 0, 1, 2, -11791.92, -2753.719, 8.436013, 2.421244, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+176, 78667, 0, 1, 2, -11797.72, -2759.186, 8.222258, 2.421244, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+177, 78667, 0, 1, 2, -11646.23, -2798.472, 4.598258, 0.1692332, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+178, 78667, 0, 1, 2, -11646.19, -2807.148, 4.131834, 0.1692332, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+179, 61328, 0, 1, 2, -11614.93, -2809.227, 3.066936, 1.584467, 120, 0, 0), -- 61328 (Area: 0)
(@CGUID+180, 78489, 0, 1, 2, -11581.58, -2752.215, 6.397654, 3.608795, 120, 0, 0), -- 78489 (Area: 0) (Auras: 164478 - 164478)
(@CGUID+181, 78345, 0, 1, 2, -11559.41, -2773.33, 6.489036, 3.462641, 120, 0, 0), -- 78345 (Area: 0)
(@CGUID+182, 78488, 0, 1, 2, -11584.17, -2756.687, 6.105145, 3.608726, 120, 0, 0), -- 78488 (Area: 0)
(@CGUID+183, 78489, 0, 1, 2, -11579.03, -2757.265, 6.027129, 3.608795, 120, 0, 0), -- 78489 (Area: 0) (Auras: 164478 - 164478)
(@CGUID+184, 78489, 0, 1, 2, -11581.91, -2809.013, 3.550141, 0.6677246, 120, 0, 0), -- 78489 (Area: 0) (Auras: 164478 - 164478)
(@CGUID+185, 9699, 0, 1, 2, -11522.91, -2801.296, 1.865693, 4.956894, 120, 0, 0), -- 9699 (Area: 0)
(@CGUID+186, 5983, 0, 1, 2, -11554.27, -2817.972, 59.54743, 2.898282, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+187, 78345, 0, 1, 2, -11582.4, -2831.991, 5.786788, 2.776134, 120, 0, 0), -- 78345 (Area: 0)
(@CGUID+188, 78489, 0, 1, 2, -11540.34, -2809.298, 5.160527, 3.124828, 120, 0, 0), -- 78489 (Area: 0) (Auras: 164478 - 164478)
(@CGUID+189, 78345, 0, 1, 2, -11514.97, -2778.066, 1.228737, 5.999722, 120, 0, 0), -- 78345 (Area: 0)
(@CGUID+190, 78348, 0, 1, 2, -11503.31, -2689.911, 6.467627, 0.4836268, 120, 0, 0), -- 78348 (Area: 0)
(@CGUID+191, 78345, 0, 1, 2, -11489.83, -2686.571, 6.744792, 2.264517, 120, 0, 0), -- 78345 (Area: 0)
(@CGUID+192, 78348, 0, 1, 2, -11493.23, -2790.325, -0.8620673, 1.533449, 120, 0, 0), -- 78348 (Area: 0) (Auras: 83811 - 83811)
(@CGUID+193, 78348, 0, 1, 2, -11495.59, -2679.856, 6.980903, 5.436236, 120, 0, 0), -- 78348 (Area: 0)
(@CGUID+194, 78348, 0, 1, 2, -11489.02, -2649.156, -4.361302, 4.255027, 120, 0, 0), -- 78348 (Area: 0) (Auras: 83811 - 83811)
(@CGUID+195, 78348, 0, 1, 2, -11441.53, -2665.736, -4.664515, 5.9565, 120, 0, 0), -- 78348 (Area: 0)
(@CGUID+196, 61328, 0, 1, 2, -11458, -2709.69, 6.817333, 1.653724, 120, 0, 0), -- 61328 (Area: 0)
(@CGUID+197, 78488, 0, 1, 2, -11456.5, -2694.642, 6.892065, 1.571792, 120, 0, 0), -- 78488 (Area: 0)
(@CGUID+198, 78489, 0, 1, 2, -11459.32, -2697.474, 6.892065, 1.57276, 120, 0, 0), -- 78489 (Area: 0) (Auras: 164478 - 164478)
(@CGUID+199, 82451, 0, 1, 2, -11481.11, -2783.519, -0.8647103, 3.199137, 120, 0, 0), -- 82451 (Area: 0)
(@CGUID+200, 78489, 0, 1, 2, -11453.67, -2697.468, 6.883276, 1.572761, 120, 0, 0), -- 78489 (Area: 0) (Auras: 164478 - 164478)
(@CGUID+201, 78345, 0, 1, 2, -11391.07, -2717.516, 10.41223, 4.528569, 120, 0, 0), -- 78345 (Area: 1439)
(@CGUID+202, 78348, 0, 1, 2, -11461.5, -2632.603, 3.300133, 5.259176, 120, 0, 0), -- 78348 (Area: 1439) (Auras: 83811 - 83811)
(@CGUID+203, 78489, 0, 1, 2, -11435.07, -2783.311, -0.7160791, 1.671623, 120, 0, 0), -- 78489 (Area: 1439) (Auras: 164478 - 164478)
(@CGUID+204, 78489, 0, 1, 2, -11437.9, -2794.563, -1.602465, 0.7213843, 120, 0, 0), -- 78489 (Area: 1439) (Auras: 164478 - 164478)
(@CGUID+205, 78348, 0, 1, 2, -11428.2, -2669.943, -4.061556, 1.13263, 120, 0, 0), -- 78348 (Area: 1439)
(@CGUID+206, 78345, 0, 1, 2, -11458.26, -2823.543, -1.599899, 1.342549, 120, 0, 0), -- 78345 (Area: 1439)
(@CGUID+207, 78348, 0, 1, 2, -11422.54, -2776.247, 1.852041, 3.49212, 120, 0, 0), -- 78348 (Area: 1439)
(@CGUID+208, 78488, 0, 1, 2, -11432.49, -2786.981, -1.108587, 4.849633, 120, 0, 0), -- 78488 (Area: 1439)
(@CGUID+209, 78348, 0, 1, 2, -11387.7, -2682.865, 6.202788, 1.575001, 120, 0, 0), -- 78348 (Area: 1439)
(@CGUID+210, 78348, 0, 1, 2, -11421.74, -2659.514, -5.749088, 3.548176, 120, 0, 0), -- 78348 (Area: 1439)
(@CGUID+211, 78489, 0, 1, 2, -11428.37, -2788.056, -0.5945683, 3.404588, 120, 0, 0), -- 78489 (Area: 1439) (Auras: 164478 - 164478)
(@CGUID+212, 78348, 0, 1, 2, -11390.31, -2673.154, 4.096121, 2.313677, 120, 0, 0), -- 78348 (Area: 1439)
(@CGUID+213, 78489, 0, 1, 2, -11354.84, -2803.392, 6.430762, 5.446527, 120, 0, 0), -- 78489 (Area: 1439) (Auras: 164478 - 164478)
(@CGUID+214, 78345, 0, 1, 2, -11343.88, -2802.733, 9.102802, 0.1684332, 120, 0, 0), -- 78345 (Area: 1439)
(@CGUID+215, 77090, 0, 1, 2, -11374.79, -2827.747, -1.137153, 2.351835, 120, 0, 0), -- 77090 (Area: 1439) (Auras: )
(@CGUID+216, 78489, 0, 1, 2, -11302.44, -2733.599, 9.454777, 3.109528, 120, 0, 0), -- 78489 (Area: 1439) (Auras: 164478 - 164478)
(@CGUID+217, 9699, 0, 1, 2, -11359.21, -2755.684, 7.980856, 5.932755, 120, 0, 0), -- 9699 (Area: 1439)
(@CGUID+218, 78345, 0, 1, 2, -11332.07, -2752.415, 10.00732, 0.5451714, 120, 0, 0), -- 78345 (Area: 1439)
(@CGUID+219, 78489, 0, 1, 2, -11302.61, -2739.253, 9.862492, 3.105817, 120, 0, 0), -- 78489 (Area: 1439) (Auras: 164478 - 164478)
(@CGUID+220, 61325, 0, 1, 2, -11377.94, -2828.289, -1.571134, 0.1153986, 120, 0, 0), -- 61325 (Area: 1439)
(@CGUID+221, 78348, 0, 1, 2, -11361.47, -2651.153, -12.47717, 3.063176, 120, 0, 0), -- 78348 (Area: 1439)
(@CGUID+222, 78488, 0, 1, 2, -11305.35, -2736.34, 9.558781, 3.111269, 120, 0, 0), -- 78488 (Area: 1439)
(@CGUID+223, 5983, 0, 1, 2, -11361.95, -2825.572, 53.20143, 3.616194, 120, 0, 0), -- 5983 (Area: 1439)
(@CGUID+224, 77640, 0, 1, 2, -11407.18, -2811.87, -2.2806, 5.535407, 120, 0, 0), -- 77640 (Area: 1439) (Auras: 159982 - 159982)
(@CGUID+225, 77640, 0, 1, 2, -11377.07, -2825.493, -1.786968, 5.513357, 120, 0, 0), -- 77640 (Area: 1439) (Auras: 159982 - 159982)
(@CGUID+226, 77090, 0, 1, 2, -11409.73, -2834.807, -2.818556, 3.694123, 120, 0, 0), -- 77090 (Area: 1439)
(@CGUID+227, 78488, 0, 1, 2, -11354.99, -2807.389, 6.518165, 5.459042, 120, 0, 0), -- 78488 (Area: 1439)
(@CGUID+228, 78489, 0, 1, 2, -11358.99, -2807.234, 5.55918, 5.459069, 120, 0, 0), -- 78489 (Area: 1439) (Auras: 164478 - 164478)
(@CGUID+229, 77090, 0, 1, 2, -11408.61, -2828.878, -2.818556, 1.900933, 120, 0, 0), -- 77090 (Area: 1439)
(@CGUID+230, 76556, 0, 1, 2, -11397.44, -2807.552, -2.278139, 1.231001, 120, 0, 0), -- 76556 (Area: 1439)
(@CGUID+231, 77090, 0, 1, 2, -11387.17, -2832.733, -2.286932, 1.670741, 120, 0, 0), -- 77090 (Area: 1439)
(@CGUID+232, 78345, 0, 1, 2, -11317.35, -2651.62, -15.02586, 2.557285, 120, 0, 0), -- 78345 (Area: 1439)
(@CGUID+233, 9699, 0, 1, 2, -11228.97, -2729.259, 14.50355, 0.8019713, 120, 0, 0), -- 9699 (Area: 1439)
(@CGUID+234, 77101, 0, 1, 2, -11195.72, -2743.229, 16.53359, 3.198869, 120, 0, 0), -- 77101 (Area: 0)
(@CGUID+235, 5990, 0, 1, 2, -11200.08, -2701.861, 11.10878, 5.548918, 120, 0, 0), -- 5990 (Area: 0)
(@CGUID+236, 5983, 0, 1, 2, -11176.24, -2714.243, 65.52914, 1.169377, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+237, 61328, 0, 1, 2, -11144.24, -2742.534, 14.78806, 3.30642, 120, 0, 0), -- 61328 (Area: 0)
(@CGUID+238, 5985, 0, 1, 2, -11127.12, -2710.139, 11.29504, 1.597303, 120, 0, 0), -- 5985 (Area: 0)
(@CGUID+239, 77724, 0, 1, 2, -11092.73, -2718.967, 26.74812, 4.360482, 120, 0, 0), -- 77724 (Area: 0) (Auras: 86603 - 86603, 164460 - 164460)
(@CGUID+240, 77724, 0, 1, 2, -11069.54, -2775.863, 8.125361, 2.403554, 120, 0, 0), -- 77724 (Area: 0) (Auras: 164460 - 164460)
(@CGUID+241, 77724, 0, 1, 2, -11073.42, -2725.774, 19.67188, 4.624724, 120, 0, 0), -- 77724 (Area: 0) (Auras: 86603 - 86603, 164460 - 164460)
(@CGUID+242, 5992, 0, 1, 2, -11066.18, -2721.6, 15.24998, 4.380139, 120, 0, 0), -- 5992 (Area: 0) (Auras: 12898 - 12898)
(@CGUID+243, 77090, 0, 1, 2, -11021.54, -2755.625, 3.374329, 2.362368, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+244, 77767, 0, 1, 2, -11066.4, -2782.278, 7.800387, 2.542775, 120, 0, 0), -- 77767 (Area: 0) (Auras: 164470 - 164470)
(@CGUID+245, 9699, 0, 1, 2, -11060.07, -2778.412, 7.172404, 2.220542, 120, 0, 0), -- 9699 (Area: 0)
(@CGUID+246, 77090, 0, 1, 2, -11005.68, -2777.929, 4.516964, 0.5313566, 120, 0, 0), -- 77090 (Area: 7026)
(@CGUID+247, 77090, 0, 1, 2, -10995.91, -2721.252, 6.866395, 3.755194, 120, 0, 0), -- 77090 (Area: 7026)
(@CGUID+248, 77723, 0, 1, 2, -10997.91, -2781.135, 4.566821, 3.922576, 120, 0, 0), -- 77723 (Area: 7026)
(@CGUID+249, 77723, 0, 1, 2, -11001.3, -2716.277, 6.415555, 0.05932118, 120, 0, 0), -- 77723 (Area: 7026)
(@CGUID+250, 77090, 0, 1, 2, -11000.54, -2724.832, 6.865854, 0.5313566, 120, 0, 0), -- 77090 (Area: 7026)
(@CGUID+251, 5983, 0, 1, 2, -11054.21, -2816.182, 53.55413, 3.287155, 120, 0, 0), -- 5983 (Area: 7026)
(@CGUID+252, 77090, 0, 1, 2, -10987.01, -2722.403, 6.498888, 5.789103, 120, 0, 0), -- 77090 (Area: 7026)
(@CGUID+253, 77767, 0, 1, 2, -10985.58, -2739.345, 6.128238, 2.413587, 120, 0, 0), -- 77767 (Area: 7026) (Auras: 164470 - 164470)
(@CGUID+254, 5992, 0, 1, 2, -11053.98, -2811.332, 8.74679, 5.566929, 120, 0, 0), -- 5992 (Area: 7026) (Auras: 12898 - 12898)
(@CGUID+255, 77090, 0, 1, 2, -10989.49, -2754.264, 5.378474, 2.633789, 120, 0, 0), -- 77090 (Area: 7026)
(@CGUID+256, 77724, 0, 1, 2, -11086.65, -2825.043, 17.102, 6.204057, 120, 0, 0), -- 77724 (Area: 7026) (Auras: 86603 - 86603, 164460 - 164460)
(@CGUID+257, 77090, 0, 1, 2, -10980.54, -2803.859, 5.383881, 1.364573, 120, 0, 0), -- 77090 (Area: 7026)
(@CGUID+258, 77724, 0, 1, 2, -11037.39, -2829.061, 14.82748, 5.893497, 120, 0, 0), -- 77724 (Area: 7026) (Auras: 86603 - 86603, 164460 - 164460)
(@CGUID+259, 77723, 0, 1, 2, -10986.06, -2793.035, 5.353382, 3.68814, 120, 0, 0), -- 77723 (Area: 7026)
(@CGUID+260, 15476, 0, 1, 2, -11074.44, -2871.189, 10.24067, 0.2433179, 120, 0, 0), -- 15476 (Area: 7026)
(@CGUID+261, 77723, 0, 1, 2, -11034.14, -2873.634, 8.727942, 2.262333, 120, 0, 0), -- 77723 (Area: 7026)
(@CGUID+262, 5990, 0, 1, 2, -11078.23, -2860.933, 10.61724, 0.2319597, 120, 0, 0), -- 5990 (Area: 7026)
(@CGUID+263, 77101, 0, 1, 2, -11066.22, -2848.953, 11.54542, 4.823668, 120, 0, 0), -- 77101 (Area: 7026)
(@CGUID+264, 77724, 0, 1, 2, -11106.18, -2862.108, 22.97103, 5.791014, 120, 0, 0), -- 77724 (Area: 7026) (Auras: 86603 - 86603, 164460 - 164460)
(@CGUID+265, 5992, 0, 1, 2, -11048.86, -2858.463, 10.41242, 4.261466, 120, 0, 0), -- 5992 (Area: 7026) (Auras: 12898 - 12898)
(@CGUID+266, 77090, 0, 1, 2, -11036.45, -2870.835, 9.03993, 5.408458, 120, 0, 0), -- 77090 (Area: 7026)
(@CGUID+267, 77724, 0, 1, 2, -11094.18, -2928.391, 10.71153, 1.596957, 120, 0, 0), -- 77724 (Area: 7026) (Auras: 164460 - 164460)
(@CGUID+268, 5985, 0, 1, 2, -11026.13, -2896.707, 9.417046, 6.098438, 120, 0, 0), -- 5985 (Area: 7026)
(@CGUID+269, 77723, 0, 1, 2, -11092.12, -2926.212, 10.97301, 1.553474, 120, 0, 0), -- 77723 (Area: 7026)
(@CGUID+270, 5990, 0, 1, 2, -11127.01, -2928.072, 9.042952, 0.8120745, 120, 0, 0), -- 5990 (Area: 7026)
(@CGUID+271, 77724, 0, 1, 2, -11061.66, -2916.306, 14.36794, 4.551999, 120, 0, 0), -- 77724 (Area: 7026) (Auras: 86603 - 86603, 164460 - 164460)
(@CGUID+272, 5992, 0, 1, 2, -11096.6, -2896.667, 10.19015, 2.748109, 120, 0, 0), -- 5992 (Area: 7026) (Auras: 12898 - 12898)
(@CGUID+273, 77724, 0, 1, 2, -11091.71, -2956.67, 11.26852, 2.656823, 120, 0, 0), -- 77724 (Area: 7026) (Auras: 86603 - 86603, 164460 - 164460)
(@CGUID+274, 77724, 0, 1, 2, -11157.59, -2943.052, 20.38564, 5.000575, 120, 0, 0), -- 77724 (Area: 7026) (Auras: 86603 - 86603, 164460 - 164460)
(@CGUID+275, 77771, 0, 1, 2, -11227.43, -2894.891, 7.183998, 3.416216, 120, 0, 0), -- 77771 (Area: 2517)
(@CGUID+276, 5985, 0, 1, 2, -11210.63, -2942.737, 10.11303, 1.478899, 120, 0, 0), -- 5985 (Area: 2517)
(@CGUID+277, 5992, 0, 1, 2, -11193.21, -2966.182, 8.411127, 4.352863, 120, 0, 0), -- 5992 (Area: 2517) (Auras: 12898 - 12898)
(@CGUID+278, 5992, 0, 1, 2, -11238.59, -2979.829, 8.798145, 0.6028905, 120, 0, 0), -- 5992 (Area: 2517) (Auras: 12898 - 12898)
(@CGUID+279, 5985, 0, 1, 2, -11209.71, -2983.528, 4.402054, 1.69612, 120, 0, 0), -- 5985 (Area: 2517)
(@CGUID+280, 61326, 0, 1, 2, -11209.37, -2968.903, 7.915998, 1.570796, 120, 0, 0), -- 61326 (Area: 2517)
(@CGUID+281, 5990, 0, 1, 2, -11208.73, -3053.857, 2.376367, 5.201663, 120, 0, 0), -- 5990 (Area: 2517)
(@CGUID+282, 5985, 0, 1, 2, -11312.18, -3000.331, 10.66247, 2.613237, 120, 0, 0), -- 5985 (Area: 2517)
(@CGUID+283, 5990, 0, 1, 2, -11280.57, -3049.466, -0.07302666, 4.754372, 120, 0, 0), -- 5990 (Area: 2517) (Auras: )
(@CGUID+284, 76085, 0, 1, 2, -11277.4, -3087.811, 5.113983, 1.641193, 120, 0, 0), -- 76085 (Area: 2517) (Auras: 18950 - 18950)
(@CGUID+285, 3300, 0, 1, 2, -11285.14, -3058.807, 0.3917183, 4.363451, 120, 0, 0), -- 3300 (Area: 2517)
(@CGUID+286, 5992, 0, 1, 2, -11249.26, -3061, 0.2048311, 4.198086, 120, 0, 0), -- 5992 (Area: 2517) (Auras: 12898 - 12898)
(@CGUID+287, 5992, 0, 1, 2, -11196.11, -3082.74, 8.97554, 5.317193, 120, 0, 0), -- 5992 (Area: 2517) (Auras: 12898 - 12898)
(@CGUID+288, 76085, 0, 1, 2, -11276.38, -3103.045, 5.252144, 5.640054, 120, 0, 0), -- 76085 (Area: 2517) (Auras: 18950 - 18950)
(@CGUID+289, 5990, 0, 1, 2, -11230.07, -3124.436, 4.2537, 6.228662, 120, 0, 0), -- 5990 (Area: 2517)
(@CGUID+290, 76085, 0, 1, 2, -11289.55, -3104.866, 4.153569, 3.677463, 120, 0, 0), -- 76085 (Area: 2517) (Auras: 18950 - 18950)
(@CGUID+291, 82447, 0, 1, 2, -11279.62, -3098.337, 3.605872, 2.565207, 120, 0, 0), -- 82447 (Area: 2517)
(@CGUID+292, 77090, 0, 1, 2, -11190.72, -3125.29, 8.406713, 1.363904, 120, 0, 0), -- 77090 (Area: 2517)
(@CGUID+293, 9699, 0, 1, 2, -11210.49, -3115.057, 6.526405, 2.387283, 120, 0, 0), -- 9699 (Area: 2517)
(@CGUID+294, 76111, 0, 1, 2, -11284.94, -3090.443, 4.541727, 5.178617, 120, 0, 0), -- 76111 (Area: 2517)
(@CGUID+295, 5983, 0, 1, 2, -11321.9, -3055.125, 52.21553, 4.722717, 120, 0, 0), -- 5983 (Area: 2517)
(@CGUID+296, 5990, 0, 1, 2, -11313.36, -3112.541, -1.441117, 0.6512047, 120, 0, 0), -- 5990 (Area: 2517) (Auras: )
(@CGUID+297, 76085, 0, 1, 2, -11292.6, -3090.089, 4.401597, 2.589651, 120, 0, 0), -- 76085 (Area: 2517) (Auras: 18950 - 18950)
(@CGUID+298, 5985, 0, 1, 2, -11364.54, -3064.946, -4.335959, 3.884905, 120, 0, 0), -- 5985 (Area: 2517)
(@CGUID+299, 5992, 0, 1, 2, -11292.56, -3149.823, 5.246623, 2.381816, 120, 0, 0), -- 5992 (Area: 2517) (Auras: 12898 - 12898)
(@CGUID+300, 9699, 0, 1, 2, -11263.08, -3151.868, 4.391815, 3.361955, 120, 0, 0), -- 9699 (Area: 2517)
(@CGUID+301, 82273, 0, 1, 2, -11413.63, -3429.009, 8.24132, 4.721706, 120, 0, 0), -- 82273 (Area: 2517)
(@CGUID+302, 77640, 0, 1, 2, -11271.35, -3160.163, 5.199565, 4.019624, 120, 0, 0), -- 77640 (Area: 2517) (Auras: 159982 - 159982)
(@CGUID+303, 77090, 0, 1, 2, -11278.24, -3154.976, 5.171519, 4.428687, 120, 0, 0), -- 77090 (Area: 2517)
(@CGUID+304, 5992, 0, 1, 2, -11395.32, -3067.776, 0.08261241, 4.730598, 120, 0, 0), -- 5992 (Area: 0) (Auras: 12898 - 12898)
(@CGUID+305, 5985, 0, 1, 2, -11363.04, -3022.748, -4.02796, 3.013855, 120, 0, 0), -- 5985 (Area: 0)
(@CGUID+306, 76524, 0, 1, 2, -11347.64, -2984.46, 1.701586, 5.613307, 120, 0, 0), -- 76524 (Area: 0) (Auras: 133067 - 133067, 18950 - 18950)
(@CGUID+307, 5992, 0, 1, 2, -11373.74, -3141.249, 7.795206, 1.535655, 120, 0, 0), -- 5992 (Area: 0) (Auras: 12898 - 12898)
(@CGUID+308, 5985, 0, 1, 2, -11393.87, -3125.648, 7.677325, 3.610048, 120, 0, 0), -- 5985 (Area: 0)
(@CGUID+309, 61325, 0, 1, 2, -11378.66, -3021.834, -4.340537, 2.36592, 120, 0, 0), -- 61325 (Area: 0)
(@CGUID+310, 5983, 0, 1, 2, -11394.59, -3042.994, 61.92463, 3.415616, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+311, 77090, 0, 1, 2, -11378.77, -3160.046, 11.77714, 1.963756, 120, 0, 0), -- 77090 (Area: 0) (Auras: )
(@CGUID+312, 5983, 0, 1, 2, -11418.82, -3137.605, 86.81584, 6.065024, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+313, 77640, 0, 1, 2, -11381.79, -3151.586, 11.24244, 1.914934, 120, 0, 0), -- 77640 (Area: 0) (Auras: 159982 - 159982)
(@CGUID+314, 5990, 0, 1, 2, -11325.43, -3174.08, 8.413517, 0.05828957, 120, 0, 0), -- 5990 (Area: 0)
(@CGUID+315, 5985, 0, 1, 2, -11424.49, -3041.452, 0.7298452, 3.276989, 120, 0, 0), -- 5985 (Area: 0)
(@CGUID+316, 77090, 0, 1, 2, -11295.31, -3179.707, 14.14063, 3.534976, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+317, 76651, 0, 1, 2, -11297.81, -3180.582, 14.45313, 6.269787, 120, 0, 0), -- 76651 (Area: 0)
(@CGUID+318, 15476, 0, 1, 2, -11380.11, -3214.136, 11.24528, 5.070398, 120, 0, 0), -- 15476 (Area: 0)
(@CGUID+319, 82273, 0, 1, 2, -11530.87, -3521.031, 14.74653, 4.46613, 120, 0, 0), -- 82273 (Area: 0)
(@CGUID+320, 82273, 0, 1, 2, -11309.54, -3528.241, 18.11066, 5.043106, 120, 0, 0), -- 82273 (Area: 0)
(@CGUID+321, 82273, 0, 1, 2, -11439.83, -3552.3, 14.26042, 4.379193, 120, 0, 0), -- 82273 (Area: 0)
(@CGUID+322, 82273, 0, 1, 2, -11385.78, -3526.292, 11.32118, 4.956842, 120, 0, 0), -- 82273 (Area: 0)
(@CGUID+323, 5992, 0, 1, 2, -11402.81, -3285.258, 2.876369, 1.514602, 120, 0, 0), -- 5992 (Area: 0) (Auras: 12898 - 12898)
(@CGUID+324, 82774, 0, 1, 2, -11380.96, -3284.46, 1.916555, 0.4625069, 120, 0, 0), -- 82774 (Area: 0)
(@CGUID+325, 61326, 0, 1, 2, -11377.38, -3281.83, 2.384779, 2.104846, 120, 0, 0), -- 61326 (Area: 0)
(@CGUID+326, 5983, 0, 1, 2, -11328.5, -3286.857, 96.05421, 5.329813, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+327, 82850, 0, 1, 2, -11376.09, -3282.821, 2.494792, 0.9656255, 120, 0, 0), -- 82850 (Area: 0)
(@CGUID+328, 5990, 0, 1, 2, -11440.21, -3251.509, 10.93508, 5.059715, 120, 0, 0), -- 5990 (Area: 0)
(@CGUID+329, 76556, 0, 1, 2, -11411.9, -3321.83, 7.438518, 4.471364, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+330, 77090, 0, 1, 2, -11355.38, -3312.856, 8.74785, 5.390686, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+331, 78667, 0, 1, 2, -11461.27, -3237.663, 16.88459, 5.418895, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+332, 82850, 0, 1, 2, -11425.81, -3344.186, 7.554033, 5.013417, 120, 0, 0), -- 82850 (Area: 0)
(@CGUID+333, 76556, 0, 1, 2, -11368.11, -3341.568, 7.670029, 4.38041, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+334, 82850, 0, 1, 2, -11275.16, -3367.776, 6.080897, 0.6370633, 120, 0, 0), -- 82850 (Area: 0)
(@CGUID+335, 82850, 0, 1, 2, -11427.75, -3346.253, 7.580122, 5.535197, 120, 0, 0), -- 82850 (Area: 0)
(@CGUID+336, 9699, 0, 1, 2, -11269.5, -3363.939, 6.066161, 4.746274, 120, 0, 0), -- 9699 (Area: 0)
(@CGUID+337, 61327, 0, 1, 2, -11271.67, -3364.426, 6.066161, 3.902012, 120, 5, 1), -- 61327 (Area: 0) (possible waypoints or random movement)
(@CGUID+338, 82774, 0, 1, 2, -11277.61, -3369.361, 6.079406, 0.3548385, 120, 0, 0), -- 82774 (Area: 0)
(@CGUID+339, 76556, 0, 1, 2, -11335.58, -3401.54, 7.606018, 0.9841573, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+340, 82774, 0, 1, 2, -11431.18, -3341.67, 7.582181, 5.678106, 120, 0, 0), -- 82774 (Area: 0)
(@CGUID+341, 77090, 0, 1, 2, -11409.21, -3360.019, 7.551929, 4.104319, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+342, 76651, 0, 1, 2, -11291.91, -3352.912, 9.254216, 3.603958, 120, 0, 0), -- 76651 (Area: 0)
(@CGUID+343, 82850, 0, 1, 2, -11311.25, -3337.253, 10.04728, 3.618517, 120, 0, 0), -- 82850 (Area: 0)
(@CGUID+344, 77090, 0, 1, 2, -11407.2, -3365.507, 7.552508, 0.09970875, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+345, 82774, 0, 1, 2, -11317.09, -3338.757, 10.40051, 0.04366625, 120, 0, 0), -- 82774 (Area: 0)
(@CGUID+346, 82850, 0, 1, 2, -11294.83, -3412.116, 8.260416, 3.535027, 120, 0, 0), -- 82850 (Area: 0)
(@CGUID+347, 77090, 0, 1, 2, -11294, -3397.604, 7.551076, 2.455627, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+348, 41158, 0, 1, 2, -11252.85, -3382.259, 7.423611, 5.439488, 120, 0, 0), -- 41158 (Area: 0) (Auras: 114371 - 114371)
(@CGUID+349, 77090, 0, 1, 2, -11311.17, -3424.443, 8.606874, 5.390686, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+350, 78142, 0, 1, 2, -11236.75, -3330.137, 9.727089, 0.6136097, 120, 0, 0), -- 78142 (Area: 0)
(@CGUID+351, 15476, 0, 1, 2, -11251.83, -3384.266, 8.134366, 3.691107, 120, 0, 0), -- 15476 (Area: 0)
(@CGUID+352, 77090, 0, 1, 2, -11318.34, -3423.576, 7.551075, 3.499559, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+353, 82774, 0, 1, 2, -11297.42, -3411.561, 8.057292, 5.831286, 120, 0, 0), -- 82774 (Area: 0)
(@CGUID+354, 41158, 0, 1, 2, -11250.8, -3380.853, 7.482639, 1.135276, 120, 0, 0), -- 41158 (Area: 0) (Auras: 114371 - 114371)
(@CGUID+355, 82850, 0, 1, 2, -11251.1, -3388.814, 10.24826, 1.348292, 120, 0, 0), -- 82850 (Area: 0)
(@CGUID+356, 82774, 0, 1, 2, -11251, -3391.953, 10.3289, 1.523825, 120, 0, 0), -- 82774 (Area: 0)
(@CGUID+357, 77090, 0, 1, 2, -11292.5, -3406.234, 7.551661, 5.05668, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+358, 9699, 0, 1, 2, -11254.44, -3378.842, 7.860929, 2.303612, 120, 0, 0), -- 9699 (Area: 0)
(@CGUID+359, 82774, 0, 1, 2, -11204.96, -3351.583, 5.086029, 2.853313, 120, 0, 0), -- 82774 (Area: 0)
(@CGUID+360, 82850, 0, 1, 2, -11209.28, -3353.675, 5.08603, 3.237361, 120, 0, 0), -- 82850 (Area: 0)
(@CGUID+361, 77643, 0, 1, 2, -11292.18, -3434.252, 10.21221, 2.254714, 120, 0, 0), -- 77643 (Area: 0)
(@CGUID+362, 77640, 0, 1, 2, -11184.13, -3262.326, 8.338541, 2.694079, 120, 0, 0), -- 77640 (Area: 0) (Auras: 159982 - 159982)
(@CGUID+363, 77640, 0, 1, 2, -11153.35, -3340.809, 8.888888, 1.204954, 120, 0, 0), -- 77640 (Area: 0) (Auras: 159982 - 159982)
(@CGUID+364, 77090, 0, 1, 2, -11175.74, -3292.455, 4.696191, 3.739881, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+365, 5990, 0, 1, 2, -11184.53, -3355.157, 8.516573, 0.2827241, 120, 0, 0), -- 5990 (Area: 0)
(@CGUID+366, 77090, 0, 1, 2, -11175.74, -3287.146, 6.658274, 4.134568, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+367, 77640, 0, 1, 2, -11176.11, -3306.804, 5.44502, 2.963142, 120, 0, 0), -- 77640 (Area: 0) (Auras: 159982 - 159982)
(@CGUID+368, 77090, 0, 1, 2, -11153.31, -3338.174, 8.557291, 4.753329, 120, 0, 0), -- 77090 (Area: 0) (Auras: )
(@CGUID+369, 5992, 0, 1, 2, -11190.24, -3322.283, 7.493225, 2.985276, 120, 0, 0), -- 5992 (Area: 0) (Auras: 12898 - 12898)
(@CGUID+370, 77090, 0, 1, 2, -11186.73, -3262.063, 8.552083, 6.242456, 120, 0, 0), -- 77090 (Area: 0) (Auras: )
(@CGUID+371, 61328, 0, 1, 2, -11192.77, -3309.203, 8.351027, 2.285447, 120, 0, 0), -- 61328 (Area: 0)
(@CGUID+372, 5992, 0, 1, 2, -11179.03, -3237.152, 7.731172, 2.285772, 120, 0, 0), -- 5992 (Area: 0) (Auras: 12898 - 12898)
(@CGUID+373, 5990, 0, 1, 2, -11167.17, -3254.063, 7.991637, 3.234349, 120, 0, 0), -- 5990 (Area: 0)
(@CGUID+374, 77090, 0, 1, 2, -11214.09, -3193.776, 8.050347, 1.004202, 120, 0, 0), -- 77090 (Area: 0) (Auras: )
(@CGUID+375, 77090, 0, 1, 2, -11145.27, -3199.309, 8.91493, 5.00246, 120, 0, 0), -- 77090 (Area: 0) (Auras: )
(@CGUID+376, 77090, 0, 1, 2, -11165.25, -3217.203, 6.641201, 2.812204, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+377, 5992, 0, 1, 2, -11151.07, -3227.792, 7.843037, 3.599647, 120, 0, 0), -- 5992 (Area: 0) (Auras: 12898 - 12898)
(@CGUID+378, 77090, 0, 1, 2, -11162.91, -3222.793, 6.778068, 4.377198, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+379, 77640, 0, 1, 2, -11144.66, -3201.868, 8.737847, 1.454084, 120, 0, 0), -- 77640 (Area: 0) (Auras: 159982 - 159982)
(@CGUID+380, 5990, 0, 1, 2, -11194.26, -3197.422, 7.452593, 4.157867, 120, 0, 0), -- 5990 (Area: 0)
(@CGUID+381, 77640, 0, 1, 2, -11212.55, -3191.651, 7.930875, 3.739011, 120, 0, 0), -- 77640 (Area: 0) (Auras: 159982 - 159982)
(@CGUID+382, 9699, 0, 1, 2, -11158.09, -3197.854, 7.670978, 4.869194, 120, 0, 0), -- 9699 (Area: 0)
(@CGUID+383, 5990, 0, 1, 2, -11235.99, -3181.83, 4.736037, 3.926991, 120, 0, 0), -- 5990 (Area: 0)
(@CGUID+384, 77644, 0, 1, 2, -11101.07, -3274.826, 56.11146, 2.573648, 120, 0, 0), -- 77644 (Area: 0) (Auras: 160098 - 160098)
(@CGUID+385, 77721, 0, 1, 2, -11090.29, -3313.427, 47.94098, 2.892005, 120, 0, 0), -- 77721 (Area: 0)
(@CGUID+386, 77790, 0, 1, 2, -11097.02, -3334.083, 53.33625, 1.03557, 120, 0, 0), -- 77790 (Area: 0) (Auras: 160055 - 160055)
(@CGUID+387, 76886, 0, 1, 2, -11170.19, -3425.781, 69.26355, 2.964241, 120, 0, 0), -- 76886 (Area: 0) (Auras: 86603 - 86603)
(@CGUID+388, 76556, 0, 1, 2, -11128.48, -3441.932, 65.03131, 3.325176, 120, 5, 1), -- 76556 (Area: 0) (possible waypoints or random movement)
(@CGUID+389, 44311, 0, 1, 2, -11071.22, -3414.748, 69.6408, 0, 120, 0, 0), -- 44311 (Area: 0) (Auras: 114371 - 114371)
(@CGUID+390, 76673, 0, 1, 2, -11099.98, -3452.064, 64.62254, 1.619944, 120, 0, 0), -- 76673 (Area: 0) (Auras: 83811 - 83811)
(@CGUID+391, 77090, 0, 1, 2, -11085.98, -3450.524, 65.03761, 5.582939, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+392, 76556, 0, 1, 2, -11070.31, -3369.356, 65.0376, 2.417153, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+393, 77640, 0, 1, 2, -11071.88, -3416.8, 69.72852, 0.2003512, 120, 0, 0), -- 77640 (Area: 0) (Auras: 159982 - 159982)
(@CGUID+394, 76673, 0, 1, 2, -11238.62, -3448.913, 7.019664, 1.051376, 120, 0, 0), -- 76673 (Area: 0) (Auras: 83811 - 83811)
(@CGUID+395, 77090, 0, 1, 2, -11148.19, -3475.462, 65.27943, 3.667491, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+396, 77090, 0, 1, 2, -11072.79, -3367.042, 65.03811, 5.809193, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+397, 76673, 0, 1, 2, -11219.65, -3475.698, 9.856577, 0.5374004, 120, 0, 0), -- 76673 (Area: 1438) (Auras: 83811 - 83811)
(@CGUID+398, 77090, 0, 1, 2, -11068.74, -3416.195, 69.76281, 3.322015, 120, 0, 0), -- 77090 (Area: 1438) (Auras: )
(@CGUID+399, 76556, 0, 1, 2, -11163.98, -3499.936, 66.22454, 0.1901855, 120, 5, 1), -- 76556 (Area: 1438) (possible waypoints or random movement)
(@CGUID+400, 76556, 0, 1, 2, -11083.83, -3477.906, 66.81665, 2.081219, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+401, 76556, 0, 1, 2, -11144.1, -3501.872, 65.23932, 4.004272, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+402, 77090, 0, 1, 2, -11120.7, -3510.809, 65.27942, 5.381903, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+403, 78141, 0, 1, 2, -11056.83, -3455.255, 65.79688, 4.754786, 120, 0, 0), -- 78141 (Area: 1438) (Auras: 29266 - 29266)
(@CGUID+404, 77790, 0, 1, 2, -11142.81, -3510.057, 64.58031, 0.8972542, 120, 5, 1), -- 77790 (Area: 1438) (Auras: 160055 - 160055) (possible waypoints or random movement)
(@CGUID+405, 77845, 0, 1, 2, -11055.59, -3456.974, 65.1778, 2.585149, 120, 0, 0), -- 77845 (Area: 1438)
(@CGUID+406, 77090, 0, 1, 2, -11005.07, -3409.726, 63.31168, 4.442096, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+407, 78141, 0, 1, 2, -11048.71, -3487.295, 65.03761, 0.7458358, 120, 0, 0), -- 78141 (Area: 1438) (Auras: 29266 - 29266)
(@CGUID+408, 78141, 0, 1, 2, -11027.72, -3477.747, 65.03882, 1.664071, 120, 0, 0), -- 78141 (Area: 1438) (Auras: 29266 - 29266)
(@CGUID+409, 76556, 0, 1, 2, -10940.77, -3416.425, 66.49753, 2.919717, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+410, 77640, 0, 1, 2, -11000.07, -3377.104, 62.51216, 4.766607, 120, 0, 0), -- 77640 (Area: 1438) (Auras: 159982 - 159982)
(@CGUID+411, 77640, 0, 1, 2, -11059.53, -3333.88, 67.22061, 2.036304, 120, 0, 0), -- 77640 (Area: 1438) (Auras: 159982 - 159982)
(@CGUID+412, 77090, 0, 1, 2, -10969.63, -3402.189, 65.21184, 2.308913, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+413, 77640, 0, 1, 2, -10956.6, -3406.097, 79.60923, 4.513689, 120, 0, 0), -- 77640 (Area: 1438) (Auras: 159982 - 159982)
(@CGUID+414, 76556, 0, 1, 2, -11024.65, -3336.748, 64.36581, 1.950697, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+415, 77090, 0, 1, 2, -10956.96, -3456.858, 66.848, 4.611111, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+416, 14881, 0, 1, 2, -10999.44, -3413.727, 61.99995, 4.269036, 120, 0, 0), -- 14881 (Area: 1438)
(@CGUID+417, 76556, 0, 1, 2, -10951.76, -3390.74, 65.34235, 3.023956, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+418, 61327, 0, 1, 2, -11059.99, -3315.677, 51.52364, 2.915087, 120, 0, 0), -- 61327 (Area: 1438)
(@CGUID+419, 76886, 0, 1, 2, -10945.99, -3392.163, 99.60607, 2.915281, 120, 0, 0), -- 76886 (Area: 1438) (Auras: 86603 - 86603)
(@CGUID+420, 77090, 0, 1, 2, -10953.85, -3450.455, 66.848, 6.239339, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+421, 77640, 0, 1, 2, -10991.73, -3461.043, 68.66841, 1.721734, 120, 0, 0), -- 77640 (Area: 1438) (Auras: 159982 - 159982)
(@CGUID+422, 44313, 0, 1, 2, -11021.4, -3373.113, 65.26941, 1.737342, 120, 0, 0), -- 44313 (Area: 1438) (Auras: 114371 - 114371)
(@CGUID+423, 76556, 0, 1, 2, -10998.9, -3331.597, 65.04451, 1.950697, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+424, 77090, 0, 1, 2, -11026.39, -3373.514, 65.17014, 3.322015, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+425, 77845, 0, 1, 2, -11029.37, -3476.514, 65.0382, 5.923302, 120, 0, 0), -- 77845 (Area: 1438)
(@CGUID+426, 77845, 0, 1, 2, -11048.72, -3485.243, 65.03761, 5.004124, 120, 0, 0), -- 77845 (Area: 1438)
(@CGUID+427, 76556, 0, 1, 2, -10985.91, -3433.648, 62.03051, 2.587693, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+428, 77090, 0, 1, 2, -11044.02, -3384.264, 65.40591, 2.942699, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+429, 77090, 0, 1, 2, -10968.18, -3422.936, 65.04391, 2.532864, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+430, 77790, 0, 1, 2, -11036.52, -3448.582, 65.13444, 2.977428, 120, 5, 1), -- 77790 (Area: 1438) (Auras: 160055 - 160055) (possible waypoints or random movement)
(@CGUID+431, 77090, 0, 1, 2, -10973.09, -3365.688, 65.37977, 0.3869211, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+432, 77090, 0, 1, 2, -10957.33, -3413.719, 66.58087, 1.783108, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+433, 77090, 0, 1, 2, -11025.23, -3408.769, 65.3288, 3.158288, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+434, 76556, 0, 1, 2, -10949.13, -3374.844, 79.60923, 1.411065, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+435, 76556, 0, 1, 2, -10962.79, -3359.316, 68.40625, 3.825459, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+436, 77090, 0, 1, 2, -10969.97, -3368.457, 65.65028, 1.19425, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+437, 77090, 0, 1, 2, -10958.06, -3409.799, 66.58087, 4.869559, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+438, 77090, 0, 1, 2, -10942.48, -3367.068, 66.49752, 0.2741138, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+439, 77640, 0, 1, 2, -10909.22, -3396.651, 66.5809, 2.163704, 120, 0, 0), -- 77640 (Area: 1438) (Auras: 159982 - 159982)
(@CGUID+440, 77640, 0, 1, 2, -10914.46, -3428.13, 79.60925, 1.401616, 120, 0, 0), -- 77640 (Area: 1438) (Auras: 159982 - 159982)
(@CGUID+441, 77090, 0, 1, 2, -10910.58, -3394.582, 66.5809, 5.047323, 120, 0, 0), -- 77090 (Area: 1438) (Auras: )
(@CGUID+442, 76556, 0, 1, 2, -10927.57, -3434.502, 66.07336, 2.209729, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+443, 73446, 0, 1, 2, -10919.05, -3398.377, 81.79813, 4.32933, 120, 0, 0), -- 73446 (Area: 1438)
(@CGUID+444, 77090, 0, 1, 2, -10931.43, -3429.63, 68.38985, 2.852906, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+445, 76556, 0, 1, 2, -10931.22, -3414.419, 75.3193, 6.050877, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+446, 77640, 0, 1, 2, -10936.59, -3371.76, 90.57882, 2.913306, 120, 0, 0), -- 77640 (Area: 1438) (Auras: 159982 - 159982)
(@CGUID+447, 77640, 0, 1, 2, -10908.58, -3423.594, 66.58084, 6.026, 120, 0, 0), -- 77640 (Area: 1438) (Auras: 159982 - 159982)
(@CGUID+448, 77090, 0, 1, 2, -10948.1, -3462.358, 66.848, 3.504573, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+449, 76556, 0, 1, 2, -10901.88, -3373.281, 79.60925, 4.491237, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+450, 77090, 0, 1, 2, -10919.55, -3401.639, 81.79813, 1.792914, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+451, 76556, 0, 1, 2, -10923.92, -3419.663, 90.57825, 6.060699, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+452, 76556, 0, 1, 2, -10948.74, -3337.146, 65.03761, 1.527752, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+453, 76673, 0, 1, 2, -10905.37, -3378.726, 66.58084, 5.440738, 120, 0, 0), -- 76673 (Area: 1438) (Auras: 83811 - 83811)
(@CGUID+454, 77640, 0, 1, 2, -10939.16, -3380.575, 80.42097, 4.392025, 120, 0, 0), -- 77640 (Area: 1438) (Auras: 159982 - 159982)
(@CGUID+455, 77640, 0, 1, 2, -10916.85, -3375.686, 66.49751, 6.042999, 120, 0, 0), -- 77640 (Area: 1438) (Auras: 159982 - 159982)
(@CGUID+456, 41125, 0, 1, 2, -10945.11, -3459.474, 66.84799, 0, 120, 0, 0), -- 41125 (Area: 1438) (Auras: 114371 - 114371)
(@CGUID+457, 77790, 0, 1, 2, -10913.67, -3451.701, 65.16062, 2.391117, 120, 0, 0), -- 77790 (Area: 1438) (Auras: 160055 - 160055)
(@CGUID+458, 77640, 0, 1, 2, -10936.06, -3492.23, 65.07929, 0.3769552, 120, 0, 0), -- 77640 (Area: 1438) (Auras: 159982 - 159982)
(@CGUID+459, 61328, 0, 1, 2, -10923.33, -3481.138, 65.0793, 0.6054912, 120, 0, 0), -- 61328 (Area: 1438)
(@CGUID+460, 76886, 0, 1, 2, -10944.91, -3508.747, 74.28247, 0.8810914, 120, 0, 0), -- 76886 (Area: 1438) (Auras: 86603 - 86603)
(@CGUID+461, 76673, 0, 1, 2, -10890.27, -3437.557, 72.25922, 1.0969, 120, 0, 0), -- 76673 (Area: 1438) (Auras: 83811 - 83811)
(@CGUID+462, 73459, 0, 1, 2, -10903.41, -3495.839, 65.03766, 5.544303, 120, 0, 0), -- 73459 (Area: 1438) (Auras: 158339 - 158339, 153776 - 153776)
(@CGUID+463, 76673, 0, 1, 2, -10920.93, -3509.359, 66.27167, 5.694692, 120, 0, 0), -- 76673 (Area: 1438) (Auras: 83811 - 83811)
(@CGUID+464, 76556, 0, 1, 2, -10994.14, -3520.226, 65.07929, 2.973757, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+465, 76886, 0, 1, 2, -11013.37, -3515.637, 73.88216, 5.964006, 120, 0, 0), -- 76886 (Area: 1438) (Auras: 86603 - 86603)
(@CGUID+466, 76556, 0, 1, 2, -11088.43, -3508.936, 64.99441, 4.225386, 120, 0, 0), -- 76556 (Area: 1438)
(@CGUID+467, 76886, 0, 1, 2, -11136.55, -3540.653, 66.2046, 1.596457, 120, 0, 0), -- 76886 (Area: 1438) (Auras: 86603 - 86603)
(@CGUID+468, 76886, 0, 1, 2, -11179.31, -3523.887, 67.39403, 4.859293, 120, 0, 0), -- 76886 (Area: 1438) (Auras: 86603 - 86603)
(@CGUID+469, 76886, 0, 1, 2, -11170.08, -3555.326, 58.57485, 2.554565, 120, 0, 0), -- 76886 (Area: 1438) (Auras: 86603 - 86603)
(@CGUID+470, 77090, 0, 1, 2, -11213.81, -3523.059, 7.135566, 4.555671, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+471, 76886, 0, 1, 2, -11183.7, -3578.415, 66.70314, 2.412577, 120, 0, 0), -- 76886 (Area: 1438) (Auras: 86603 - 86603)
(@CGUID+472, 77643, 0, 1, 2, -11214.48, -3513.271, 7.836781, 2.272008, 120, 0, 0), -- 77643 (Area: 1438)
(@CGUID+473, 77090, 0, 1, 2, -11207.81, -3522.505, 6.033212, 5.556872, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+474, 84199, 0, 1, 2, -11324.84, -3588.026, 15.32937, 4.7411, 120, 0, 0), -- 84199 (Area: 1438) (Auras: 168461 - 168461)
(@CGUID+475, 77643, 0, 1, 2, -11233.5, -3511.764, 13.06438, 1.136596, 120, 0, 0), -- 77643 (Area: 1438)
(@CGUID+476, 77090, 0, 1, 2, -11232.14, -3558.438, 6.657758, 5.987003, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+477, 9699, 0, 1, 2, -11253.43, -3544.68, 22.17657, 2.069364, 120, 0, 0), -- 9699 (Area: 1438)
(@CGUID+478, 76673, 0, 1, 2, -11238.65, -3558.839, 7.596284, 0.2315697, 120, 0, 0), -- 76673 (Area: 1438)
(@CGUID+479, 77090, 0, 1, 2, -11235.24, -3555.403, 7.017841, 0.5865399, 120, 0, 0), -- 77090 (Area: 1438)
(@CGUID+480, 76886, 0, 1, 2, -11260.61, -3601.719, 18.8058, 5.065533, 120, 0, 0), -- 76886 (Area: 0) (Auras: 86603 - 86603)
(@CGUID+481, 76098, 0, 1, 2, -11256.39, -3625.556, 11.48477, 2.964198, 120, 0, 0), -- 76098 (Area: 0)
(@CGUID+482, 76085, 0, 1, 2, -11249.01, -3623.531, 12.43765, 0.899685, 120, 0, 0), -- 76085 (Area: 0) (Auras: 18950 - 18950)
(@CGUID+483, 77643, 0, 1, 2, -11261.21, -3544.99, 7.780299, 1.544272, 120, 0, 0), -- 77643 (Area: 0)
(@CGUID+484, 76117, 0, 1, 2, -11259.98, -3624.955, 12.56676, 5.399792, 120, 0, 0), -- 76117 (Area: 0)
(@CGUID+485, 77090, 0, 1, 2, -11258.98, -3564.53, 7.470068, 5.55611, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+486, 82448, 0, 1, 2, -11257.95, -3628.233, 11.46889, 2.305428, 120, 0, 0), -- 82448 (Area: 0)
(@CGUID+487, 76098, 0, 1, 2, -11259.8, -3628.314, 11.41369, 1.636852, 120, 0, 0), -- 76098 (Area: 0)
(@CGUID+488, 76098, 0, 1, 2, -11256.44, -3627.457, 11.39414, 2.522635, 120, 0, 0), -- 76098 (Area: 0)
(@CGUID+489, 76085, 0, 1, 2, -11257.94, -3651.274, 7.062828, 4.371469, 120, 5, 1), -- 76085 (Area: 0) (Auras: 18950 - 18950) (possible waypoints or random movement)
(@CGUID+490, 76103, 0, 1, 2, -11275.69, -3659.939, 4.923237, 2.344842, 120, 0, 0), -- 76103 (Area: 0)
(@CGUID+491, 76085, 0, 1, 2, -11269.38, -3629.712, 9.655286, 5.104297, 120, 0, 0), -- 76085 (Area: 0) (Auras: 18950 - 18950)
(@CGUID+492, 76103, 0, 1, 2, -11284.68, -3642.786, 7.769097, 2.907002, 120, 0, 0), -- 76103 (Area: 0)
(@CGUID+493, 76108, 0, 1, 2, -11278.14, -3669.728, 4.174259, 0.9050022, 120, 5, 1), -- 76108 (Area: 0) (possible waypoints or random movement)
(@CGUID+494, 82270, 0, 1, 2, -11273.22, -3632.451, 9.469362, 1.459806, 120, 0, 0), -- 82270 (Area: 0)
(@CGUID+495, 76098, 0, 1, 2, -11280.18, -3637.531, 8.49299, 2.437049, 120, 0, 0), -- 76098 (Area: 0)
(@CGUID+496, 76436, 0, 1, 2, -11265.27, -3648.307, 7.317708, 1.75587, 120, 0, 0), -- 76436 (Area: 0)
(@CGUID+497, 76098, 0, 1, 2, -11287.01, -3640.984, 8.111111, 5.226589, 120, 0, 0), -- 76098 (Area: 0)
(@CGUID+498, 84199, 0, 1, 2, -11396.87, -3598.366, 9.395565, 5.499781, 120, 0, 0), -- 84199 (Area: 0) (Auras: 168461 - 168461)
(@CGUID+499, 76085, 0, 1, 2, -11264.97, -3655.682, 4.924133, 5.640054, 120, 0, 0), -- 76085 (Area: 0) (Auras: 18950 - 18950)
(@CGUID+500, 76085, 0, 1, 2, -11277.54, -3630.951, 9.4691, 4.341249, 120, 0, 0), -- 76085 (Area: 0) (Auras: 18950 - 18950)
(@CGUID+501, 76098, 0, 1, 2, -11286.83, -3643.375, 7.823236, 0.9900541, 120, 0, 0), -- 76098 (Area: 0)
(@CGUID+502, 61325, 0, 1, 2, -11251.12, -3654.324, 7.59868, 5.228929, 120, 0, 0), -- 61325 (Area: 0)
(@CGUID+503, 76103, 0, 1, 2, -11281.77, -3639.342, 8.224299, 2.7792, 120, 0, 0), -- 76103 (Area: 0)
(@CGUID+504, 76103, 0, 1, 2, -11284.66, -3639.056, 8.362952, 1.046479, 120, 0, 0), -- 76103 (Area: 0)
(@CGUID+505, 76085, 0, 1, 2, -11271.03, -3670.522, 4.654909, 0.7626788, 120, 5, 1), -- 76085 (Area: 0) (Auras: 18950 - 18950) (possible waypoints or random movement)
(@CGUID+506, 77090, 0, 1, 2, -11256.83, -3534.705, 10.29714, 0.8331683, 120, 0, 0), -- 77090 (Area: 0)
(@CGUID+507, 76110, 0, 1, 2, -11289.97, -3627.795, 10.17281, 5.358161, 120, 0, 0), -- 76110 (Area: 0)
(@CGUID+508, 76189, 0, 1, 2, -11290.02, -3574.368, 15.13075, 3.467368, 120, 0, 0), -- 76189 (Area: 0)
(@CGUID+509, 76103, 0, 1, 2, -11274.29, -3658.797, 4.923237, 5.446145, 120, 0, 0), -- 76103 (Area: 0)
(@CGUID+510, 76085, 0, 1, 2, -11299.73, -3630.967, 9.692812, 2.502675, 120, 0, 0), -- 76085 (Area: 0) (Auras: 18950 - 18950)
(@CGUID+511, 76436, 0, 1, 2, -11321.06, -3646.837, 8.021018, 1.528855, 120, 0, 0), -- 76436 (Area: 0)
(@CGUID+512, 76429, 0, 1, 2, -11331.99, -3647.953, 6.486111, 1.937201, 120, 0, 0), -- 76429 (Area: 0)
(@CGUID+513, 76436, 0, 1, 2, -11323.43, -3645.309, 7.793438, 1.658957, 120, 0, 0), -- 76436 (Area: 0)
(@CGUID+514, 76103, 0, 1, 2, -11286.35, -3669.932, 3.698418, 5.755675, 120, 0, 0), -- 76103 (Area: 0)
(@CGUID+515, 76085, 0, 1, 2, -11294.22, -3675.981, 3.47961, 4.420939, 120, 0, 0), -- 76085 (Area: 0) (Auras: 18950 - 18950)
(@CGUID+516, 76189, 0, 1, 2, -11321.51, -3616.714, 11.72616, 4.940909, 120, 0, 0), -- 76189 (Area: 0) (Auras: )
(@CGUID+517, 76429, 0, 1, 2, -11326.06, -3647.882, 7.123264, 1.881095, 120, 0, 0), -- 76429 (Area: 0)
(@CGUID+518, 76436, 0, 1, 2, -11334.34, -3646.038, 6.227431, 1.484999, 120, 0, 0), -- 76436 (Area: 0)
(@CGUID+519, 76436, 0, 1, 2, -11321.75, -3621.65, 10.97567, 1.156933, 120, 5, 1), -- 76436 (Area: 0) (possible waypoints or random movement)
(@CGUID+520, 76429, 0, 1, 2, -11328.71, -3650.606, 6.649306, 1.937201, 120, 0, 0), -- 76429 (Area: 0)
(@CGUID+521, 76103, 0, 1, 2, -11289.33, -3672.5, 3.692026, 5.119727, 120, 0, 0), -- 76103 (Area: 0)
(@CGUID+522, 76103, 0, 1, 2, -11286.69, -3671.858, 3.698418, 2.773972, 120, 0, 0), -- 76103 (Area: 0)
(@CGUID+523, 76436, 0, 1, 2, -11321.05, -3621.583, 11.02905, 1.457031, 120, 5, 1), -- 76436 (Area: 0) (possible waypoints or random movement)
(@CGUID+524, 76103, 0, 1, 2, -11287.44, -3673.743, 3.698418, 5.940218, 120, 0, 0), -- 76103 (Area: 0)
(@CGUID+525, 76085, 0, 1, 2, -11300.96, -3646.5, 7.622487, 1.480833, 120, 5, 1), -- 76085 (Area: 0) (Auras: 18950 - 18950) (possible waypoints or random movement)
(@CGUID+526, 76103, 0, 1, 2, -11288.64, -3669.262, 3.698418, 1.087883, 120, 0, 0), -- 76103 (Area: 0)
(@CGUID+527, 76448, 0, 1, 2, -11337.18, -3644.634, 5.881657, 1.749277, 120, 0, 0), -- 76448 (Area: 6953)
(@CGUID+528, 76429, 0, 1, 2, -11341.95, -3651.252, 4.920811, 1.937201, 120, 0, 0), -- 76429 (Area: 6953)
(@CGUID+529, 76436, 0, 1, 2, -11362.71, -3681.248, 4.779514, 2.725773, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+530, 84199, 0, 1, 2, -11450.98, -3635.455, 8.349174, 5.714775, 120, 0, 0), -- 84199 (Area: 0) (Auras: )
(@CGUID+531, 76436, 0, 1, 2, -11362.54, -3652.776, 3.849388, 1.726651, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+532, 76429, 0, 1, 2, -11359.37, -3654.299, 4.024007, 1.937201, 120, 0, 0), -- 76429 (Area: 6953)
(@CGUID+533, 76429, 0, 1, 2, -11364.14, -3687.486, 4.298611, 3.078013, 120, 0, 0), -- 76429 (Area: 6953)
(@CGUID+534, 76436, 0, 1, 2, -11351.83, -3649.625, 4.401042, 1.918205, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+535, 76189, 0, 1, 2, -11343.25, -3578.193, 11.79424, 4.81783, 120, 0, 0), -- 76189 (Area: 6953)
(@CGUID+536, 76436, 0, 1, 2, -11364.79, -3654.231, 3.694568, 2.136867, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+537, 76098, 0, 1, 2, -11304.87, -3740.95, 2.966141, 0.7032689, 120, 0, 0), -- 76098 (Area: 6953)
(@CGUID+538, 76429, 0, 1, 2, -11355.81, -3655.712, 4.090278, 1.937201, 120, 0, 0), -- 76429 (Area: 6953)
(@CGUID+539, 76098, 0, 1, 2, -11310.13, -3730.81, 2.432712, 4.406515, 120, 0, 0), -- 76098 (Area: 6953) (Auras: 132693 - 132693)
(@CGUID+540, 76436, 0, 1, 2, -11302.45, -3732.639, 2.086273, 1.28374, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+541, 76429, 0, 1, 2, -11345.7, -3649.538, 4.50398, 1.937201, 120, 0, 0), -- 76429 (Area: 6953)
(@CGUID+542, 61328, 0, 1, 2, -11358.16, -3643.139, 4.130248, 1.538808, 120, 5, 1), -- 61328 (Area: 6953) (possible waypoints or random movement)
(@CGUID+543, 76098, 0, 1, 2, -11297.9, -3738.229, 2.406814, 4.448558, 120, 0, 0), -- 76098 (Area: 6953) (Auras: 132693 - 132693)
(@CGUID+544, 76189, 0, 1, 2, -11347.01, -3611.698, 8.225778, 5.171571, 120, 5, 1), -- 76189 (Area: 6953) (Auras: ) (possible waypoints or random movement)
(@CGUID+545, 76429, 0, 1, 2, -11339.65, -3648.528, 5.4375, 1.881095, 120, 0, 0), -- 76429 (Area: 6953)
(@CGUID+546, 76098, 0, 1, 2, -11299.37, -3742.392, 2.539387, 4.429603, 120, 0, 0), -- 76098 (Area: 6953)
(@CGUID+547, 76436, 0, 1, 2, -11348.94, -3647.934, 4.503472, 1.782692, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+548, 76436, 0, 1, 2, -11345.27, -3616.341, 8.090734, 1.781976, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+549, 76436, 0, 1, 2, -11313.41, -3731.611, 2.398085, 1.674463, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+550, 76436, 0, 1, 2, -11364.93, -3682.387, 4.880733, 2.855875, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+551, 84199, 0, 1, 2, -11459.15, -3671.717, 5.781315, 5.407628, 120, 0, 0), -- 84199 (Area: 6953) (Auras: 168461 - 168461)
(@CGUID+552, 76429, 0, 1, 2, -11353.64, -3652.851, 4.274843, 1.881095, 120, 0, 0), -- 76429 (Area: 6953)
(@CGUID+553, 76429, 0, 1, 2, -11362.44, -3690.625, 3.708333, 3.134119, 120, 0, 0), -- 76429 (Area: 6953)
(@CGUID+554, 76436, 0, 1, 2, -11286.94, -3738.667, 2.896113, 1.120794, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+555, 76436, 0, 1, 2, -11396.02, -3669.497, 2.392168, 2.704017, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+556, 76429, 0, 1, 2, -11365.4, -3693.486, 3.59375, 3.134119, 120, 0, 0), -- 76429 (Area: 6953)
(@CGUID+557, 76436, 0, 1, 2, -11326.3, -3733.116, 2.631348, 1.906099, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+558, 84199, 0, 1, 2, -11489.11, -3673.903, 8.330221, 3.90677, 120, 0, 0), -- 84199 (Area: 6953) (Auras: 168461 - 168461)
(@CGUID+559, 76436, 0, 1, 2, -11368.37, -3710.648, 0.8611111, 3.184242, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+560, 76429, 0, 1, 2, -11365.67, -3702.533, 1.658461, 3.147132, 120, 0, 0), -- 76429 (Area: 6953)
(@CGUID+561, 76436, 0, 1, 2, -11374.62, -3628.219, 5.229928, 2.052386, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+562, 16225, 0, 1, 2, -11300.06, -3743.995, 2.961298, 1.978657, 120, 0, 0), -- 16225 (Area: 6953)
(@CGUID+563, 76098, 0, 1, 2, -11326.1, -3748.21, 2.80369, 4.76037, 120, 0, 0), -- 76098 (Area: 6953) (Auras: 132693 - 132693)
(@CGUID+564, 76436, 0, 1, 2, -11367.01, -3699.34, 2.321449, 3.07325, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+565, 76189, 0, 1, 2, -11376.52, -3625.08, 5.140226, 5.255299, 120, 0, 0), -- 76189 (Area: 6953) (Auras: )
(@CGUID+566, 76436, 0, 1, 2, -11366.77, -3696.078, 3.1875, 2.979611, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+567, 76189, 0, 1, 2, -11372.65, -3626.894, 5.116103, 3.728104, 120, 0, 0), -- 76189 (Area: 6953)
(@CGUID+568, 84199, 0, 1, 2, -11473.87, -3611.738, 11.2175, 4.23629, 120, 0, 0), -- 84199 (Area: 6953) (Auras: 168461 - 168461)
(@CGUID+569, 76436, 0, 1, 2, -11374.2, -3627.924, 5.14399, 2.310064, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+570, 76429, 0, 1, 2, -11363.59, -3705.464, 1.442708, 3.203237, 120, 0, 0), -- 76429 (Area: 6953)
(@CGUID+571, 76436, 0, 1, 2, -11366.17, -3712.04, 0.7895581, 3.402903, 120, 0, 0), -- 76436 (Area: 6953)
(@CGUID+572, 76429, 0, 1, 2, -11366.01, -3708.436, 1.081796, 3.203237, 120, 0, 0), -- 76429 (Area: 6953)
(@CGUID+573, 76189, 0, 1, 2, -11398.3, -3666.651, 1.979537, 5.386628, 120, 0, 0), -- 76189 (Area: 5077)
(@CGUID+574, 76436, 0, 1, 2, -11398.07, -3669.047, 2.179738, 2.464588, 120, 0, 0), -- 76436 (Area: 5077)
(@CGUID+575, 76189, 0, 1, 2, -11403.59, -3717.695, 1.181891, 5.639754, 120, 0, 0), -- 76189 (Area: 5077)
(@CGUID+576, 76189, 0, 1, 2, -11410.65, -3691.849, 2.429158, 5.289045, 120, 0, 0), -- 76189 (Area: 5077)
(@CGUID+577, 76436, 0, 1, 2, -11399.42, -3718.57, 1.314391, 3.623626, 120, 0, 0), -- 76436 (Area: 5077)
(@CGUID+578, 76436, 0, 1, 2, -11407.03, -3694.534, 2.731354, 3.059428, 120, 0, 0), -- 76436 (Area: 5077)
(@CGUID+579, 76436, 0, 1, 2, -11399.63, -3720.744, 0.9637495, 3.383559, 120, 0, 0), -- 76436 (Area: 5077)
(@CGUID+580, 84199, 0, 1, 2, -11511.46, -3618.236, 9.115969, 4.038479, 120, 0, 0), -- 84199 (Area: 5077) (Auras: 168461 - 168461)
(@CGUID+581, 76886, 0, 1, 2, -11607.56, -3726.09, 1.840265, 1.670121, 120, 0, 0), -- 76886 (Area: 5096) (Auras: 86603 - 86603)
(@CGUID+582, 76886, 0, 1, 2, -11579.71, -3722.695, 1.431458, 0.950545, 120, 0, 0), -- 76886 (Area: 5096) (Auras: 86603 - 86603)
(@CGUID+583, 76500, 0, 1, 2, -11567.78, -3698.736, 4.119792, 5.909453, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+584, 76503, 0, 1, 2, -11565.8, -3670.58, 6.105903, 0.071361, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+585, 76500, 0, 1, 2, -11563.95, -3678.33, 5.116859, 0.0523652, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+586, 76503, 0, 1, 2, -11563.41, -3667.594, 6.637153, 0.071361, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+587, 61328, 0, 1, 2, -11575.51, -3686.327, 5.250535, 1.582515, 120, 5, 1), -- 61328 (Area: 5077) (possible waypoints or random movement)
(@CGUID+588, 76500, 0, 1, 2, -11561.46, -3681.958, 4.496528, 6.058391, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+589, 76500, 0, 1, 2, -11564.51, -3662.146, 7.03125, 0.4191443, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+590, 76503, 0, 1, 2, -11564.35, -3685.938, 4.671875, 0.0346179, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+591, 76503, 0, 1, 2, -11563.71, -3673.486, 5.262252, 0.01525459, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+592, 84199, 0, 1, 2, -11523.16, -3586.556, 13.37949, 4.362333, 120, 0, 0), -- 84199 (Area: 5077) (Auras: 168461 - 168461)
(@CGUID+593, 76503, 0, 1, 2, -11567.91, -3688.177, 4.292352, 0.0346179, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+594, 76500, 0, 1, 2, -11564.93, -3696.413, 4.272225, 6.039555, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+595, 84199, 0, 1, 2, -11555.96, -3566.899, 17.90265, 4.262505, 120, 0, 0), -- 84199 (Area: 5077) (Auras: 168461 - 168461)
(@CGUID+596, 76503, 0, 1, 2, -11566.21, -3691.563, 4.114568, 6.261697, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+597, 76500, 0, 1, 2, -11590.21, -3617.731, 11.96875, 0.7094363, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+598, 76503, 0, 1, 2, -11581.38, -3632.307, 8.738031, 0.6723267, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+599, 76500, 0, 1, 2, -11523.33, -3685.237, 3.629987, 0.4313302, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+600, 76189, 0, 1, 2, -11517.86, -3664.114, 5.570733, 4.569339, 120, 0, 0), -- 76189 (Area: 5077)
(@CGUID+601, 76500, 0, 1, 2, -11538.61, -3640.16, 7.192312, 0.5969684, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+602, 76500, 0, 1, 2, -11523.66, -3684.057, 3.771118, 6.247525, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+603, 76504, 0, 1, 2, -11576.52, -3636.186, 9.597623, 0.5396704, 120, 0, 0), -- 76504 (Area: 5077)
(@CGUID+604, 76500, 0, 1, 2, -11539.05, -3639.746, 7.283381, 0.1014966, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+605, 76503, 0, 1, 2, -11575.97, -3648.533, 10.01215, 0.7284321, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+606, 76189, 0, 1, 2, -11535.72, -3638.199, 7.400712, 3.577178, 120, 0, 0), -- 76189 (Area: 5077) (Auras: 159782 - 159782)
(@CGUID+607, 76503, 0, 1, 2, -11574.66, -3644.533, 10.30864, 0.7284321, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+608, 76500, 0, 1, 2, -11585.12, -3620.332, 10.64931, 0.5799395, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+609, 76500, 0, 1, 2, -11566.25, -3657.299, 7.206597, 0.7676562, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+610, 76189, 0, 1, 2, -11520.97, -3684.152, 3.843179, 3.573003, 120, 0, 0), -- 76189 (Area: 5077)
(@CGUID+611, 76503, 0, 1, 2, -11584.74, -3631.127, 8.690972, 0.7284321, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+612, 76500, 0, 1, 2, -11577.14, -3641.04, 9.439236, 0.2762273, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+613, 76503, 0, 1, 2, -11584.46, -3627.016, 9.579861, 0.7284321, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+614, 76503, 0, 1, 2, -11571.67, -3649.953, 9.652229, 0.6723267, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+615, 76500, 0, 1, 2, -11517.83, -3661.432, 5.705442, 0.8387322, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+616, 76500, 0, 1, 2, -11553.14, -3612.66, 11.26505, 0.49449, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+617, 76503, 0, 1, 2, -11592.07, -3612.589, 13.04861, 0.6723267, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+618, 76503, 0, 1, 2, -11596.29, -3607.939, 13.60243, 0.7284321, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+619, 76886, 0, 1, 2, -11608.14, -3584.432, 17.9519, 5.542543, 120, 5, 1), -- 76886 (Area: 5077) (Auras: 86603 - 86603) (possible waypoints or random movement)
(@CGUID+620, 76189, 0, 1, 2, -11529.62, -3608.061, 8.789502, 3.632222, 120, 0, 0), -- 76189 (Area: 5077)
(@CGUID+621, 76503, 0, 1, 2, -11596.35, -3611.769, 12.80729, 0.7284321, 120, 0, 0), -- 76503 (Area: 5077)
(@CGUID+622, 76500, 0, 1, 2, -11575.32, -3588.946, 14.4443, 0.9717265, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+623, 76189, 0, 1, 2, -11572, -3585.257, 14.75963, 3.890782, 120, 5, 1), -- 76189 (Area: 5077) (possible waypoints or random movement)
(@CGUID+624, 76500, 0, 1, 2, -11596.94, -3601.078, 14.9184, 0.7094363, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+625, 76500, 0, 1, 2, -11601.09, -3600.523, 14.54688, 0.9280978, 120, 0, 0), -- 76500 (Area: 5077)
(@CGUID+626, 76500, 0, 1, 2, -11575.86, -3588.399, 14.60297, 0.583894, 120, 5, 1), -- 76500 (Area: 5077) (possible waypoints or random movement)
(@CGUID+627, 76189, 0, 1, 2, -11551.07, -3611.544, 11.0318, 3.635679, 120, 0, 0), -- 76189 (Area: 5077)
(@CGUID+628, 76189, 0, 1, 2, -11534.11, -3556.575, 17.90602, 4.43682, 120, 0, 0), -- 76189 (Area: 6963)
(@CGUID+629, 15476, 0, 1, 2, -11615.65, -3569.63, 18.91919, 0.9732388, 120, 0, 0), -- 15476 (Area: 6963)
(@CGUID+630, 76556, 0, 1, 2, -11546.14, -3522.175, 15.14173, 3.723187, 120, 0, 0), -- 76556 (Area: 6963)
(@CGUID+631, 76556, 0, 1, 2, -11518.79, -3530.349, 14.95139, 4.761179, 120, 0, 0), -- 76556 (Area: 6963)
(@CGUID+632, 5983, 0, 1, 2, -11656.65, -3542.883, 138.5913, 0.6909053, 120, 0, 0), -- 5983 (Area: 6963)
(@CGUID+633, 84199, 0, 1, 2, -11364.09, -3572.399, 15.20018, 5.081177, 120, 0, 0), -- 84199 (Area: 6963) (Auras: 168461 - 168461)
(@CGUID+634, 76189, 0, 1, 2, -11489.53, -3503.401, 14.8244, 3.200061, 120, 5, 1), -- 76189 (Area: 6963) (possible waypoints or random movement)
(@CGUID+635, 76189, 0, 1, 2, -11488.22, -3560.82, 13.94147, 5.068047, 120, 0, 0), -- 76189 (Area: 6963)
(@CGUID+636, 76189, 0, 1, 2, -11458.84, -3532.601, 13.60123, 0.77175, 120, 0, 0), -- 76189 (Area: 6963)
(@CGUID+637, 9699, 0, 1, 2, -11530.36, -3454.085, 8.8922, 1.080954, 120, 0, 0), -- 9699 (Area: 6963)
(@CGUID+638, 76556, 0, 1, 2, -11454.77, -3555.368, 13.79958, 3.813959, 120, 0, 0), -- 76556 (Area: 6963)
(@CGUID+639, 76189, 0, 1, 2, -11486.2, -3635.889, 7.672804, 3.93677, 120, 0, 0), -- 76189 (Area: 6963)
(@CGUID+640, 5983, 0, 1, 2, -11438.8, -3484.528, 72.43459, 3.996959, 120, 0, 0), -- 5983 (Area: 6963)
(@CGUID+641, 76556, 0, 1, 2, -11432.87, -3562.259, 13.3189, 4.878446, 120, 0, 0), -- 76556 (Area: 6963)
(@CGUID+642, 61328, 0, 1, 2, -11419.04, -3543.713, 12.94909, 6.220645, 120, 5, 1), -- 61328 (Area: 6963) (possible waypoints or random movement)
(@CGUID+643, 14881, 0, 1, 2, -11476.7, -3655.495, 5.680369, 0.02538517, 120, 0, 0), -- 14881 (Area: 6963)
(@CGUID+644, 84199, 0, 1, 2, -11418.91, -3588.806, 10.89849, 4.209537, 120, 0, 0), -- 84199 (Area: 6963) (Auras: )
(@CGUID+645, 76556, 0, 1, 2, -11372.07, -3531.505, 13.27431, 5.347214, 120, 0, 0), -- 76556 (Area: 6963)
(@CGUID+646, 76556, 0, 1, 2, -11396.8, -3531.222, 11.35764, 4.161148, 120, 0, 0), -- 76556 (Area: 6963)
(@CGUID+647, 76189, 0, 1, 2, -11336.22, -3542.365, 13.89263, 4.231576, 120, 0, 0), -- 76189 (Area: 6963)
(@CGUID+648, 9699, 0, 1, 2, -11330.05, -3550.424, 15.4003, 0.1747854, 120, 0, 0), -- 9699 (Area: 6963)
(@CGUID+649, 76189, 0, 1, 2, -11375.04, -3505.858, 11.88784, 2.76581, 120, 5, 1), -- 76189 (Area: 6963) (possible waypoints or random movement)
(@CGUID+650, 76436, 0, 1, 2, -11351.97, -3652.288, 4.339372, 1.880073, 120, 0, 0), -- 76436 (Area: 6963)
(@CGUID+651, 76556, 0, 1, 2, -11316.09, -3542.288, 18.00509, 4.373817, 120, 0, 0), -- 76556 (Area: 6963)
(@CGUID+652, 76189, 0, 1, 2, -11405.37, -3485.583, 8.245221, 3.340173, 120, 5, 1), -- 76189 (Area: 6963) (possible waypoints or random movement)
(@CGUID+653, 76556, 0, 1, 2, -11295.88, -3526.208, 18.50368, 5.347214, 120, 0, 0), -- 76556 (Area: 6963)
(@CGUID+654, 76189, 0, 1, 2, -11352.49, -3475.733, 14.00648, 5.232422, 120, 0, 0), -- 76189 (Area: 6963)
(@CGUID+655, 77643, 0, 1, 2, -11268.11, -3518.639, 7.391953, 5.163144, 120, 0, 0), -- 77643 (Area: 6963)
(@CGUID+656, 76189, 0, 1, 2, -11303.63, -3496.688, 17.91319, 2.658171, 120, 0, 0), -- 76189 (Area: 6963)
(@CGUID+657, 76651, 0, 1, 2, -11372.21, -3462.432, 7.777778, 4.334863, 120, 0, 0), -- 76651 (Area: 6963)
(@CGUID+658, 82850, 0, 1, 2, -11383.56, -3448.649, 4.263195, 2.105862, 120, 0, 0), -- 82850 (Area: 6963)
(@CGUID+659, 82774, 0, 1, 2, -11379.83, -3451.656, 5.732639, 1.721814, 120, 0, 0), -- 82774 (Area: 6963)
(@CGUID+660, 9699, 0, 1, 2, -11377.28, -3437.897, 6.790259, 5.817349, 120, 0, 0), -- 9699 (Area: 6963)
(@CGUID+661, 76189, 0, 1, 2, -11348.93, -3436.278, 8.117517, 2.329868, 120, 0, 0), -- 76189 (Area: 6963)
(@CGUID+662, 76556, 0, 1, 2, -11402.78, -3431.628, 7.821587, 4.838961, 120, 0, 0), -- 76556 (Area: 6963)
(@CGUID+663, 76651, 0, 1, 2, -11412.33, -3463.514, 7.904209, 4.334863, 120, 0, 0), -- 76651 (Area: 6963)
(@CGUID+664, 76651, 0, 1, 2, -11440.73, -3460.866, 8.715755, 4.334863, 120, 0, 0), -- 76651 (Area: 6963)
(@CGUID+665, 82774, 0, 1, 2, -11439.98, -3419.195, 8.355712, 2.862579, 120, 0, 0), -- 82774 (Area: 6963)
(@CGUID+666, 61328, 0, 1, 2, -11453.25, -3417.887, 6.17462, 0.3841308, 120, 0, 0), -- 61328 (Area: 6963)
(@CGUID+667, 41158, 0, 1, 2, -11454.19, -3418.752, 6.519097, 5.439488, 120, 0, 0), -- 41158 (Area: 6963) (Auras: 114371 - 114371)
(@CGUID+668, 61326, 0, 1, 2, -11455.89, -3418.773, 6.890949, 2.788999, 120, 0, 0), -- 61326 (Area: 6963)
(@CGUID+669, 76556, 0, 1, 2, -11427.56, -3430.776, 8.621528, 4.838961, 120, 0, 0), -- 76556 (Area: 6963)
(@CGUID+670, 82850, 0, 1, 2, -11443.11, -3420.479, 8.526462, 3.35486, 120, 0, 0), -- 82850 (Area: 6963)
(@CGUID+671, 41158, 0, 1, 2, -11452.14, -3417.345, 5.682292, 1.135276, 120, 0, 0), -- 41158 (Area: 6963) (Auras: 114371 - 114371)
(@CGUID+672, 76556, 0, 1, 2, -11374.83, -3403.229, 7.770306, 0.6622416, 120, 0, 0), -- 76556 (Area: 6963)
(@CGUID+673, 82850, 0, 1, 2, -11442.87, -3418.389, 8.128158, 2.915344, 120, 0, 0), -- 82850 (Area: 6963)
(@CGUID+674, 5992, 0, 1, 2, -11455.18, -3320.95, 8.431279, 4.924108, 120, 0, 0), -- 5992 (Area: 0) (Auras: 12898 - 12898)
(@CGUID+675, 5983, 0, 1, 2, -11494.25, -3350.963, 64.97147, 5.015158, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+676, 78667, 0, 1, 2, -11540.97, -3379.222, 7.573485, 0.513124, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+677, 5992, 0, 1, 2, -11550.15, -3447.388, 7.592746, 5.486315, 120, 0, 0), -- 5992 (Area: 0) (Auras: 12898 - 12898)
(@CGUID+678, 78667, 0, 1, 2, -11537.22, -3363.604, 8.803385, 5.875069, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+679, 78667, 0, 1, 2, -11571.64, -3401.264, 7.563592, 5.418895, 120, 0, 0), -- 78667 (Area: 0)
(@CGUID+680, 78667, 0, 1, 2, -11610.97, -3398.347, 7.605964, 3.700444, 120, 0, 0), -- 78667 (Area: 6963)
(@CGUID+681, 76886, 0, 1, 2, -11722.15, -3684.486, 39.35447, 5.603461, 120, 0, 0), -- 76886 (Area: 6963) (Auras: 86603 - 86603)
(@CGUID+682, 76886, 0, 1, 2, -11727.65, -3710.366, 32.59679, 4.410685, 120, 0, 0), -- 76886 (Area: 6963) (Auras: 86603 - 86603)
(@CGUID+683, 76886, 0, 1, 2, -11772.82, -3753.84, 62.50644, 5.361414, 120, 0, 0), -- 76886 (Area: 0) (Auras: 86603 - 86603)
(@CGUID+684, 76886, 0, 1, 2, -11769.22, -3752.052, 61.52257, 5.538994, 120, 5, 1), -- 76886 (Area: 0) (Auras: 86603 - 86603) (possible waypoints or random movement)
(@CGUID+685, 76886, 0, 1, 2, -11725.86, -3796.719, 62.77456, 3.686048, 120, 0, 0), -- 76886 (Area: 0) (Auras: 86603 - 86603)
(@CGUID+686, 76524, 0, 1, 2, -11767.03, -3798.611, 55.74363, 1.257746, 120, 0, 0), -- 76524 (Area: 0) (Auras: 18950 - 18950, 133067 - 133067)
(@CGUID+687, 76521, 0, 1, 2, -11760.91, -3816.056, 55.52731, 4.181231, 120, 0, 0), -- 76521 (Area: 0)
(@CGUID+688, 76886, 0, 1, 2, -11731.18, -3822.873, 57.80638, 5.237472, 120, 0, 0), -- 76886 (Area: 0) (Auras: 86603 - 86603)
(@CGUID+689, 82851, 0, 1, 2, -11748.91, -3824.127, 56.87147, 4.852554, 120, 0, 0), -- 82851 (Area: 0)
(@CGUID+690, 76524, 0, 1, 2, -11804.34, -3799.082, 56.88137, 3.504659, 120, 0, 0), -- 76524 (Area: 0) (Auras: 18950 - 18950, 133067 - 133067)
(@CGUID+691, 76524, 0, 1, 2, -11803.64, -3805.039, 56.92388, 1.81857, 120, 0, 0), -- 76524 (Area: 0) (Auras: 18950 - 18950, 133067 - 133067)
(@CGUID+692, 76522, 0, 1, 2, -11785.51, -3815.554, 56.97623, 0.4786595, 120, 0, 0), -- 76522 (Area: 0)
(@CGUID+693, 85247, 0, 1, 2, -11778.98, -3826.168, 56.70112, 4.37943, 120, 0, 0), -- 85247 (Area: 0)
(@CGUID+694, 76524, 0, 1, 2, -11775.57, -3799.049, 56.30825, 1.477545, 120, 0, 0), -- 76524 (Area: 0) (Auras: 18950 - 18950, 133067 - 133067)
(@CGUID+695, 76500, 0, 1, 2, -11748.92, -3826.983, 57.02894, 1.674055, 120, 0, 0), -- 76500 (Area: 0)
(@CGUID+696, 76524, 0, 1, 2, -11744.93, -3848.182, 57.29652, 5.912825, 120, 0, 0), -- 76524 (Area: 0) (Auras: 18950 - 18950, 133067 - 133067)
(@CGUID+697, 76520, 0, 1, 2, -11779.69, -3833.005, 56.70112, 2.5219, 120, 0, 0), -- 76520 (Area: 0)
(@CGUID+698, 76886, 0, 1, 2, -11835.24, -3775.833, 70.54514, 5.168669, 120, 0, 0), -- 76886 (Area: 0) (Auras: 86603 - 86603)
(@CGUID+699, 76524, 0, 1, 2, -11756.25, -3854.274, 57.44843, 4.561443, 120, 0, 0), -- 76524 (Area: 0) (Auras: 18950 - 18950, 133067 - 133067)
(@CGUID+700, 76524, 0, 1, 2, -11766.92, -3856.97, 57.35301, 3.372486, 120, 0, 0), -- 76524 (Area: 0) (Auras: 18950 - 18950, 133067 - 133067)
(@CGUID+701, 5985, 0, 1, 2, -11805.96, -3813.846, 56.92304, 1.461433, 120, 0, 0), -- 5985 (Area: 0) (Auras: )
(@CGUID+702, 76886, 0, 1, 2, -11731.43, -3854.13, 56.44195, 1.910338, 120, 0, 0), -- 76886 (Area: 0) (Auras: 86603 - 86603)
(@CGUID+703, 76519, 0, 1, 2, -11746.56, -3838.557, 57.10209, 2.791485, 120, 0, 0), -- 76519 (Area: 0)
(@CGUID+704, 15476, 0, 1, 2, -11808.69, -3827.035, 57.29804, 3.089249, 120, 0, 0), -- 15476 (Area: 0)
(@CGUID+705, 76886, 0, 1, 2, -11797.2, -3815.274, 56.93437, 2.459296, 120, 0, 0), -- 76886 (Area: 0) (Auras: )
(@CGUID+706, 5992, 0, 1, 2, -11857.16, -3764.733, 73.76409, 2.117142, 120, 0, 0), -- 5992 (Area: 6954) (Auras: 12898 - 12898)
(@CGUID+707, 76886, 0, 1, 2, -11712.13, -3866.241, 58.5452, 2.754175, 120, 0, 0), -- 76886 (Area: 6954) (Auras: 86603 - 86603)
(@CGUID+708, 76524, 0, 1, 2, -11797.73, -3858.201, 59.64466, 4.131856, 120, 0, 0), -- 76524 (Area: 6954) (Auras: 18950 - 18950, 133067 - 133067)
(@CGUID+709, 76886, 0, 1, 2, -11835.25, -3880.168, 71.11157, 0.6676083, 120, 0, 0), -- 76886 (Area: 6954) (Auras: 86603 - 86603)
(@CGUID+710, 76886, 0, 1, 2, -11866.54, -3830.978, 58.63155, 1.478126, 120, 5, 1), -- 76886 (Area: 6954) (Auras: 86603 - 86603) (possible waypoints or random movement)
(@CGUID+711, 5992, 0, 1, 2, -11815, -3857.065, 59.60051, 0.6335386, 120, 0, 0), -- 5992 (Area: 6954) (Auras: 12898 - 12898)
(@CGUID+712, 85734, 0, 1, 2, -11770.85, -3865.695, 58.4778, 0.9846047, 120, 0, 0), -- 85734 (Area: 6954)
(@CGUID+713, 5983, 0, 1, 2, -11772.65, -3853.074, 105.4253, 0.2137246, 120, 0, 0), -- 5983 (Area: 6954)
(@CGUID+714, 5983, 0, 1, 2, -11837.39, -3796.887, 120.7483, 5.871789, 120, 0, 0), -- 5983 (Area: 6954)
(@CGUID+715, 76886, 0, 1, 2, -11763.88, -3904.972, 67.49112, 1.242481, 120, 0, 0), -- 76886 (Area: 6954) (Auras: 86603 - 86603)
(@CGUID+716, 76886, 0, 1, 2, -11805.62, -3838.6, 58.4375, 0.9491838, 120, 0, 0), -- 76886 (Area: 6954) (Auras: 86603 - 86603)
(@CGUID+717, 76886, 0, 1, 2, -11812.08, -3897.486, 63.76437, 2.579059, 120, 0, 0), -- 76886 (Area: 6954) (Auras: 86603 - 86603)
(@CGUID+718, 5990, 0, 1, 2, -11868.85, -3835.082, 58.42501, 2.682625, 120, 0, 0), -- 5990 (Area: 6954)
(@CGUID+719, 61325, 0, 1, 2, -11897.38, -3828.016, 57.13174, 0.07789174, 120, 0, 0), -- 61325 (Area: 0)
(@CGUID+720, 5985, 0, 1, 2, -11904.93, -3775.487, 72.06049, 5.676807, 120, 0, 0), -- 5985 (Area: 0)
(@CGUID+721, 5983, 0, 1, 2, -11912.62, -3842.495, 119.2453, 3.751591, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+722, 5992, 0, 1, 2, -11911.62, -3865.298, 56.92305, 1.910341, 120, 0, 0), -- 5992 (Area: 0) (Auras: 12898 - 12898)
(@CGUID+723, 5985, 0, 1, 2, -11943.5, -3775.949, 62.13625, 4.690908, 120, 0, 0), -- 5985 (Area: 0)
(@CGUID+724, 5992, 0, 1, 2, -11975.48, -3728.364, 50.99915, 3.176735, 120, 0, 0), -- 5992 (Area: 0) (Auras: 12898 - 12898)
(@CGUID+725, 5983, 0, 1, 2, -11885.75, -3639.783, 148.1003, 3.351574, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+726, 15476, 0, 1, 2, -12096.11, -3783.214, 5.90929, 3.32634, 120, 0, 0), -- 15476 (Area: 0)
(@CGUID+727, 5983, 0, 1, 2, -12060.71, -3702.472, 101.3853, 4.480632, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+728, 5983, 0, 1, 2, -12057.66, -3813.942, 49.58553, 1.367261, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+729, 45087, 0, 1, 2, -12082.48, -3810.759, 5.487246, 4.60281, 120, 0, 0), -- 45087 (Area: 0) (Auras: 84161 - 84161)
(@CGUID+730, 5990, 0, 1, 2, -12006.58, -3674.106, 49.11898, 1.133363, 120, 0, 0), -- 5990 (Area: 0)
(@CGUID+731, 45087, 0, 1, 2, -12126.2, -3727.449, 10.72356, 3.924229, 120, 0, 0), -- 45087 (Area: 5078) (Auras: 84161 - 84161)
(@CGUID+732, 45087, 0, 1, 2, -12194.65, -3742.22, 5.957208, 0.2311573, 120, 0, 0), -- 45087 (Area: 5079) (Auras: 84161 - 84161)
(@CGUID+733, 45087, 0, 1, 2, -12185.05, -3876.439, -6.783633, 4.909019, 120, 0, 0), -- 45087 (Area: 5079) (Auras: 84161 - 84161)
(@CGUID+734, 15476, 0, 1, 2, -12167.9, -3719.389, 8.072949, 0.1529463, 120, 0, 0), -- 15476 (Area: 5079)
(@CGUID+735, 45087, 0, 1, 2, -12149.1, -3885.804, -7.654966, 6.044371, 120, 0, 0), -- 45087 (Area: 5079) (Auras: 84161 - 84161)
(@CGUID+736, 5983, 0, 1, 2, -12174.94, -3874.8, 33.95163, 4.862153, 120, 0, 0), -- 5983 (Area: 5079)
(@CGUID+737, 45087, 0, 1, 2, -12174.38, -3796.842, 2.510906, 4.871121, 120, 0, 0), -- 45087 (Area: 5079) (Auras: 84161 - 84161)
(@CGUID+738, 5983, 0, 1, 2, -12203.97, -3751.857, 59.75594, 4.554013, 120, 0, 0), -- 5983 (Area: 5079)
(@CGUID+739, 45087, 0, 1, 2, -12251.12, -3840.731, -2.020985, 1.077277, 120, 0, 0), -- 45087 (Area: 5078) (Auras: 84161 - 84161)
(@CGUID+740, 5983, 0, 1, 2, -12240.84, -3863.467, 37.33013, 1.943104, 120, 0, 0), -- 5983 (Area: 5078)
(@CGUID+741, 5983, 0, 1, 2, -12280.18, -3739.395, 47.60784, 5.051842, 120, 0, 0), -- 5983 (Area: 5078)
(@CGUID+742, 45087, 0, 1, 2, -12279.95, -3739.408, 5.836743, 1.394087, 120, 0, 0), -- 45087 (Area: 5078) (Auras: 84161 - 84161)
(@CGUID+743, 3300, 0, 1, 2, -12277.29, -3712.283, 6.17299, 3.956767, 120, 0, 0), -- 3300 (Area: 5078)
(@CGUID+744, 5983, 0, 1, 2, -12129.3, -3597.681, 145.1303, 1.758901, 120, 0, 0), -- 5983 (Area: 5078)
(@CGUID+745, 5983, 0, 1, 2, -12135.03, -3467.1, 149.0013, 0.6562456, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+746, 15476, 0, 1, 2, -12190.7, -3440.325, 27.39136, 3.930715, 120, 0, 0), -- 15476 (Area: 0)
(@CGUID+747, 5983, 0, 1, 2, -12080.48, -3391.761, 127.0673, 0.4910346, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+748, 5983, 0, 1, 2, -12114.81, -3346.912, 164.3963, 5.789333, 120, 0, 0), -- 5983 (Area: 5082)
(@CGUID+749, 5983, 0, 1, 2, -12247.35, -3301.427, 111.7163, 5.790707, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+750, 5983, 0, 1, 2, -12322.82, -3320.272, 108.3063, 0.1854026, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+751, 5985, 0, 1, 2, -12286.38, -3270.306, 27.51712, 4.569964, 120, 0, 0), -- 5985 (Area: 0)
(@CGUID+752, 15476, 0, 1, 2, -12326.58, -3418.892, 20.71648, 6.187401, 120, 0, 0), -- 15476 (Area: 5078)
(@CGUID+753, 5983, 0, 1, 2, -12363.8, -3384.05, 61.72812, 0.5093963, 120, 0, 0), -- 5983 (Area: 5078)
(@CGUID+754, 5985, 0, 1, 2, -12323.7, -3237.685, 27.48677, 1.409492, 120, 0, 0), -- 5985 (Area: 5078)
(@CGUID+755, 5985, 0, 1, 2, -12384.53, -3292.621, 25.37008, 2.214331, 120, 0, 0), -- 5985 (Area: 5078)
(@CGUID+756, 5983, 0, 1, 2, -12414.66, -3261.215, 85.43783, 3.90182, 120, 0, 0), -- 5983 (Area: 5078)
(@CGUID+757, 5985, 0, 1, 2, -12416.79, -3285.398, 25.2138, 0.8370946, 120, 0, 0), -- 5985 (Area: 5078)
(@CGUID+758, 15476, 0, 1, 2, -12390.01, -3263.877, 27.50996, 1.498515, 120, 0, 0), -- 15476 (Area: 5078)
(@CGUID+759, 45087, 0, 1, 2, -12473.96, -3208.973, 15.12636, 0.01171821, 120, 0, 0), -- 45087 (Area: 5078) (Auras: 84161 - 84161)
(@CGUID+760, 61326, 0, 1, 2, -12499.16, -3245.356, 9.501226, 0.5143035, 120, 0, 0), -- 61326 (Area: 5078)
(@CGUID+761, 5983, 0, 1, 2, -12509.56, -3186.605, 73.11123, 4.310784, 120, 0, 0), -- 5983 (Area: 5078)
(@CGUID+762, 45087, 0, 1, 2, -12500.54, -3261.434, 10.94881, 2.124629, 120, 0, 0), -- 45087 (Area: 5078) (Auras: 84161 - 84161)
(@CGUID+763, 5983, 0, 1, 2, -12514.15, -3118.96, 62.57214, 3.27441, 120, 0, 0), -- 5983 (Area: 5078)
(@CGUID+764, 45087, 0, 1, 2, -12562.38, -3179.236, 6.364377, 0.170083, 120, 0, 0), -- 45087 (Area: 5078) (Auras: 84161 - 84161)
(@CGUID+765, 45087, 0, 1, 2, -12506.56, -3130.368, 13.19839, 0.6020862, 120, 0, 0), -- 45087 (Area: 5078) (Auras: 84161 - 84161)
(@CGUID+766, 45087, 0, 1, 2, -12555.4, -3251.161, 5.065945, 3.872308, 120, 0, 0), -- 45087 (Area: 5078) (Auras: 84161 - 84161)
(@CGUID+767, 5983, 0, 1, 2, -12610.54, -3150.985, 48.92424, 0.3359368, 120, 0, 0), -- 5983 (Area: 5078)
(@CGUID+768, 45087, 0, 1, 2, -12581.79, -3112.587, 2.301975, 0.7510804, 120, 0, 0), -- 45087 (Area: 5078) (Auras: 84161 - 84161)
(@CGUID+769, 3300, 0, 1, 2, -12571.72, -3126.572, 2.51369, 2.28981, 120, 0, 0), -- 3300 (Area: 5078)
(@CGUID+770, 45087, 0, 1, 2, -12537.74, -3062.8, 10.30654, 1.188546, 120, 0, 0), -- 45087 (Area: 5078) (Auras: 84161 - 84161)
(@CGUID+771, 15476, 0, 1, 2, -12497.68, -2991.971, 1.681841, 3.292608, 120, 0, 0), -- 15476 (Area: 5078)
(@CGUID+772, 5983, 0, 1, 2, -12360.41, -2944.465, 173.2413, 0.719178, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+773, 5983, 0, 1, 2, -12020.71, -2910.75, 170.2153, 3.630473, 120, 0, 0), -- 5983 (Area: 5083)
(@CGUID+774, 5983, 0, 1, 2, -12072.55, -2983.005, 225.6323, 4.831112, 120, 0, 0), -- 5983 (Area: 5083)
(@CGUID+775, 5983, 0, 1, 2, -11987.11, -2869.37, 179.0773, 3.263797, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+776, 5983, 0, 1, 2, -11951.58, -3028.712, 152.3633, 3.242295, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+777, 5983, 0, 1, 2, -11926.38, -3115.139, 160.5223, 1.266381, 120, 0, 0), -- 5983 (Area: 0)
(@CGUID+778, 76556, 0, 1, 2, -11836.73, -3188.783, -29.92892, 0.505581, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+779, 76556, 0, 1, 2, -11843.75, -3191.666, -29.30392, 1.051593, 120, 0, 0), -- 76556 (Area: 0) (Auras: 159944 - 159944)
(@CGUID+780, 76556, 0, 1, 2, -11837.78, -3187.481, -29.58249, 0.4524615, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+781, 76556, 0, 1, 2, -11841.12, -3200.934, -29.9283, 0.1677357, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+782, 76556, 0, 1, 2, -11835.78, -3193.927, -30.19821, 3.049214, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+783, 76556, 0, 1, 2, -11840.57, -3204.908, -29.96108, 4.849198, 120, 0, 0), -- 76556 (Area: 0) (Auras: )
(@CGUID+784, 76556, 0, 1, 2, -11841.54, -3208.209, -29.82479, 4.33623, 120, 0, 0), -- 76556 (Area: 0) (Auras: )
(@CGUID+785, 76556, 0, 1, 2, -11834.13, -3193.941, -30.32192, 0.127164, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+786, 76556, 0, 1, 2, -11835.59, -3199.745, -30.71066, 3.458951, 120, 0, 0), -- 76556 (Area: 0) (Auras: 159944 - 159944)
(@CGUID+787, 76556, 0, 1, 2, -11844.13, -3206.851, -29.85286, 3.641849, 120, 0, 0), -- 76556 (Area: 0) (Auras: )
(@CGUID+788, 76556, 0, 1, 2, -11902.92, -3206.201, -11.63345, 0.156449, 120, 0, 0), -- 76556 (Area: 0) (Auras: 159944 - 159944)
(@CGUID+789, 76556, 0, 1, 2, -11835.59, -3199.745, -30.71066, 6.233938, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+790, 76556, 0, 1, 2, -11902.55, -3207.904, -11.62625, 0.1503888, 120, 0, 0), -- 76556 (Area: 0) (Auras: 159944 - 159944)
(@CGUID+791, 76556, 0, 1, 2, -11841.15, -3207.203, -29.94539, 5.096207, 120, 0, 0), -- 76556 (Area: 0) (Auras: )
(@CGUID+792, 76556, 0, 1, 2, -11841.15, -3207.203, -29.94539, 5.67464, 120, 0, 0), -- 76556 (Area: 0) (Auras: )
(@CGUID+793, 76556, 0, 1, 2, -11903.22, -3204.723, -11.63526, 0.1287902, 120, 0, 0), -- 76556 (Area: 0) (Auras: )
(@CGUID+794, 76556, 0, 1, 2, -11835.08, -3205.361, -30.54158, 6.093244, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+795, 76556, 0, 1, 2, -11834.76, -3194.021, -30.33371, 0.4976151, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+796, 76556, 0, 1, 2, -11835.59, -3199.745, -30.71066, 6.233938, 120, 0, 0), -- 76556 (Area: 0) (Auras: 159944 - 159944)
(@CGUID+797, 76556, 0, 1, 2, -11833.08, -3206.207, -30.5785, 4.934079, 120, 0, 0), -- 76556 (Area: 0) (Auras: )
(@CGUID+798, 76556, 0, 1, 2, -11906.14, -3203.108, -11.5685, 0.05748955, 120, 0, 0), -- 76556 (Area: 0) (Auras: )
(@CGUID+799, 76556, 0, 1, 2, -11835.12, -3204.703, -30.55471, 1.631712, 120, 0, 0), -- 76556 (Area: 0) (Auras: )
(@CGUID+800, 76556, 0, 1, 2, -11835.59, -3199.745, -30.71066, 1.046129, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+801, 76556, 0, 1, 2, -11834.76, -3194.021, -30.33371, 2.461349, 120, 0, 0), -- 76556 (Area: 0)
(@CGUID+802, 76556, 0, 1, 2, -11905.03, -3209.613, -11.61907, 0.1485685, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+803, 76556, 0, 1, 2, -11904.82, -3211.245, -11.56851, 0.1736725, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
 (@CGUID+804, 76556, 0, 1, 2, -11904.43, -3212.773, -11.52012, 0.1943905, 120, 0, 0), -- 76556 (Area: 72)
 (@CGUID+805, 76556, 0, 1, 2, -11906.14, -3203.108, -11.5685, 0.05748955, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
 (@CGUID+806, 76556, 0, 1, 2, -11905.03, -3209.613, -11.61907, 0.1485685, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+807, 76556, 0, 1, 2, -11904.82, -3211.245, -11.56851, 0.1736725, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
 (@CGUID+808, 76556, 0, 1, 2, -11905.96, -3206.681, -11.64358, 0.156449, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
 (@CGUID+809, 76556, 0, 1, 2, -11905.59, -3208.365, -11.63638, 0.1503888, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+810, 76556, 0, 1, 2, -11904.43, -3212.773, -11.52012, 0.1943905, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+811, 76556, 0, 1, 2, -11906.18, -3205.106, -11.64383, 0.1287902, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+812, 76556, 0, 1, 2, -11906.14, -3203.108, -11.5685, 0.05748955, 120, 0, 0), -- 76556 (Area: 72) (Auras: 159944 - 159944)
(@CGUID+813, 76556, 0, 1, 2, -11905.96, -3206.681, -11.64358, 0.156449, 120, 0, 0), -- 76556 (Area: 72) (Auras: )
(@CGUID+814, 76556, 0, 1, 2, -11904.82, -3211.245, -11.56851, 0.1736725, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+815, 76556, 0, 1, 2, -11905.03, -3209.613, -11.61907, 0.1485685, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+816, 76556, 0, 1, 2, -11905.59, -3208.365, -11.63638, 0.1503888, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+817, 76556, 0, 1, 2, -11904.43, -3212.773, -11.52012, 0.1943905, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+818, 76556, 0, 1, 2, -11906.14, -3203.108, -11.5685, 0.05748955, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+819, 76556, 0, 1, 2, -11906.18, -3205.106, -11.64383, 0.1287902, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+820, 76556, 0, 1, 2, -11905.96, -3206.681, -11.64358, 0.156449, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+821, 76556, 0, 1, 2, -11905.59, -3208.365, -11.63638, 0.1503888, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+822, 76556, 0, 1, 2, -11904.82, -3211.245, -11.56851, 0.1736725, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+823, 76556, 0, 1, 2, -11905.03, -3209.613, -11.61907, 0.1485685, 120, 0, 0); -- 76556 (Area: 72)

DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+823;
INSERT INTO `creature_addon` (`guid`, `mount`, `bytes1`, `bytes2`, `auras`) VALUES
(@CGUID+0, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+1, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+2, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+3, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+4, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+5, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+6, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+7, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+8, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+9, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+10, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+11, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+12, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+13, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+14, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+15, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+16, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+17, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+18, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+19, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+20, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+21, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+22, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+23, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+24, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+25, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+26, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+27, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+28, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+29, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+30, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+31, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+32, 0, 0x0, 0x1, '147617'), -- 82014 - 147617 - 147617
(@CGUID+33, 0, 0x0, 0x1, '160145'), -- 78696 - 160145 - 160145
(@CGUID+34, 0, 0x0, 0x1, '147617 165754'), -- 82015 - 147617 - 147617, 165754 - 165754
(@CGUID+35, 0, 0x0, 0x1, '147617'), -- 81997 - 147617 - 147617
(@CGUID+36, 0, 0x0, 0x1, '147617'), -- 79315 - 147617 - 147617
(@CGUID+37, 0, 0x0, 0x1, '147617'), -- 82189 - 147617 - 147617
(@CGUID+38, 0, 0x0, 0x1, '147617'), -- 82263 - 147617 - 147617
(@CGUID+39, 0, 0x0, 0x1, ''), -- 78674
(@CGUID+40, 0, 0x0, 0x1, '147617'), -- 82000 - 147617 - 147617
(@CGUID+41, 0, 0x0, 0x1, '147617 165750'), -- 82187 - 147617 - 147617, 165750 - 165750
(@CGUID+42, 0, 0x0, 0x1, '147617'), -- 81998 - 147617 - 147617
(@CGUID+43, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+44, 0, 0x0, 0x1, '147617'), -- 82075 - 147617 - 147617
(@CGUID+45, 0, 0x0, 0x1, '160145'), -- 76648 - 160145 - 160145
(@CGUID+46, 0, 0x0, 0x1, '147617'), -- 78568 - 147617 - 147617
(@CGUID+47, 0, 0x0, 0x1, '147617'), -- 82191 - 147617 - 147617
(@CGUID+48, 0, 0x0, 0x1, '147617'), -- 78569 - 147617 - 147617
(@CGUID+49, 0, 0x0, 0x1, '165746'), -- 78554 - 165746 - 165746
(@CGUID+50, 0, 0x0, 0x1, '147617'), -- 78556 - 147617 - 147617
(@CGUID+51, 0, 0x0, 0x1, '147617 165754'), -- 82013 - 147617 - 147617, 165754 - 165754
(@CGUID+52, 0, 0x0, 0x1, ''), -- 78674
(@CGUID+53, 0, 0x0, 0xF000001, '147617 32356'), -- 79316 - 147617 - 147617, 32356 - 32356
(@CGUID+54, 0, 0x0, 0x1, '147617'), -- 82188 - 147617 - 147617
(@CGUID+55, 0, 0x0, 0x1, '147617'), -- 82006 - 147617 - 147617
(@CGUID+56, 0, 0x0, 0x1, '147617'), -- 82007 - 147617 - 147617
(@CGUID+57, 0, 0x0, 0x1, '147617 165713 165712'), -- 82016 - 147617 - 147617, 165713 - 165713, 165712 - 165712
(@CGUID+58, 0, 0x0, 0x1, '147617 165767'), -- 82011 - 147617 - 147617, 165767 - 165767
(@CGUID+59, 0, 0x0, 0x1, ''), -- 78430
(@CGUID+60, 0, 0x0, 0x1, '147617 165747 165746'), -- 81994 - 147617 - 147617, 165747 - 165747, 165746 - 165746
(@CGUID+61, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+62, 0, 0x0, 0x1, '147617'), -- 82002 - 147617 - 147617
(@CGUID+63, 0, 0x0, 0x2, '147617'), -- 82003 - 147617 - 147617
(@CGUID+64, 0, 0x0, 0x1, ''), -- 78670
(@CGUID+65, 0, 0x0, 0x1, '147617'), -- 81999 - 147617 - 147617
(@CGUID+66, 0, 0x0, 0x1, '147617 165723'), -- 82264 - 147617 - 147617, 165723 - 165723
(@CGUID+67, 0, 0x0, 0x1, '147617 165754'), -- 82025 - 147617 - 147617, 165754 - 165754
(@CGUID+68, 19085, 0x0, 0x1, ''), -- 79675
(@CGUID+69, 0, 0x0, 0x1, '147617'), -- 82005 - 147617 - 147617
(@CGUID+70, 0, 0x0, 0x1, ''), -- 78553
(@CGUID+71, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+72, 0, 0x0, 0x1, '147617'), -- 82082 - 147617 - 147617
(@CGUID+73, 0, 0x0, 0x2, '147617'), -- 82001 - 147617 - 147617
(@CGUID+74, 0, 0x0, 0x2, '147617'), -- 82260 - 147617 - 147617
(@CGUID+75, 0, 0x0, 0x1, '147617'), -- 81996 - 147617 - 147617
(@CGUID+76, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+77, 0, 0x0, 0x1, '147617'), -- 82004 - 147617 - 147617
(@CGUID+78, 0, 0x0, 0x1, '128281'), -- 78423 - 128281 - 128281
(@CGUID+79, 0, 0x0, 0x1, '147617 165739'), -- 82009 - 147617 - 147617, 165739 - 165739
(@CGUID+80, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+81, 0, 0x0, 0x1, '147617'), -- 82008 - 147617 - 147617
(@CGUID+82, 0, 0x0, 0x1, '147617'), -- 81993 - 147617 - 147617
(@CGUID+83, 0, 0x0, 0x1, '147617'), -- 81990 - 147617 - 147617
(@CGUID+84, 0, 0x0, 0x1, '147617'), -- 82010 - 147617 - 147617
(@CGUID+85, 0, 0x0, 0x1, '147617'), -- 81995 - 147617 - 147617
(@CGUID+86, 0, 0x0, 0x1, '147617'), -- 82017 - 147617 - 147617
(@CGUID+87, 0, 0x0, 0x1, '147617 165754'), -- 82012 - 147617 - 147617, 165754 - 165754
(@CGUID+88, 0, 0x0, 0x1, ''), -- 78674
(@CGUID+89, 0, 0x0, 0x1, '147617'), -- 82259 - 147617 - 147617
(@CGUID+90, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+91, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+92, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+93, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+94, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+95, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+96, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+97, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+98, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+99, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+100, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+101, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+102, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+103, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+104, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+105, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+106, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+107, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+108, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+109, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+110, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+111, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+112, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+113, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+114, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+115, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+116, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+117, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+118, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+119, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+120, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+121, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+122, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+123, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+124, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+125, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+126, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+127, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+128, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+129, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+130, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+131, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+132, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+133, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+134, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+135, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+136, 0, 0x0, 0x1, ''), -- 78674
(@CGUID+137, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+138, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+139, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+140, 0, 0x0, 0x1, ''), -- 78674
(@CGUID+141, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+142, 0, 0x0, 0x0, ''), -- 78670
(@CGUID+143, 0, 0x0, 0x1, ''), -- 78670
(@CGUID+144, 0, 0x0, 0x0, ''), -- 78674
(@CGUID+145, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+146, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+147, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+148, 0, 0x0, 0x1, ''), -- 15476
(@CGUID+149, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+150, 0, 0x0, 0x1, ''), -- 78670
(@CGUID+151, 0, 0x0, 0x1, ''), -- 78670
(@CGUID+152, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+153, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+154, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+155, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+156, 0, 0x0, 0x0, ''), -- 78193
(@CGUID+157, 0, 0x0, 0x0, ''), -- 78193
(@CGUID+158, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+159, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+160, 0, 0x0, 0x1, ''), -- 78670
(@CGUID+161, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+162, 0, 0x0, 0x1, ''), -- 78674
(@CGUID+163, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+164, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+165, 0, 0x0, 0x1, ''), -- 78670
(@CGUID+166, 0, 0x0, 0x1, ''), -- 78670
(@CGUID+167, 0, 0x0, 0x1, ''), -- 61328
(@CGUID+168, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+169, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+170, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+171, 0, 0x0, 0x1, ''), -- 78670
(@CGUID+172, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+173, 0, 0x0, 0x1, ''), -- 78674
(@CGUID+174, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+175, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+176, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+177, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+178, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+179, 0, 0x0, 0x1, ''), -- 61328
(@CGUID+180, 0, 0x0, 0x1, '164478'), -- 78489 - 164478 - 164478
(@CGUID+181, 0, 0x0, 0x1, ''), -- 78345
(@CGUID+182, 0, 0x0, 0x1, ''), -- 78488
(@CGUID+183, 0, 0x0, 0x1, '164478'), -- 78489 - 164478 - 164478
(@CGUID+184, 0, 0x0, 0x1, '164478'), -- 78489 - 164478 - 164478
(@CGUID+185, 0, 0x0, 0x1, ''), -- 9699
(@CGUID+186, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+187, 0, 0x0, 0x1, ''), -- 78345
(@CGUID+188, 0, 0x0, 0x1, '164478'), -- 78489 - 164478 - 164478
(@CGUID+189, 0, 0x0, 0x1, ''), -- 78345
(@CGUID+190, 0, 0x0, 0x1, ''), -- 78348
(@CGUID+191, 0, 0x0, 0x0, ''), -- 78345
(@CGUID+192, 0, 0x0, 0x1, '83811'), -- 78348 - 83811 - 83811
(@CGUID+193, 0, 0x0, 0x0, ''), -- 78348
(@CGUID+194, 0, 0x0, 0x1, '83811'), -- 78348 - 83811 - 83811
(@CGUID+195, 0, 0x0, 0x1, ''), -- 78348
(@CGUID+196, 0, 0x0, 0x1, ''), -- 61328
(@CGUID+197, 0, 0x0, 0x1, ''), -- 78488
(@CGUID+198, 0, 0x0, 0x1, '164478'), -- 78489 - 164478 - 164478
(@CGUID+199, 0, 0x0, 0x1, ''), -- 82451
(@CGUID+200, 0, 0x0, 0x1, '164478'), -- 78489 - 164478 - 164478
(@CGUID+201, 0, 0x0, 0x1, ''), -- 78345
(@CGUID+202, 0, 0x0, 0x1, '83811'), -- 78348 - 83811 - 83811
(@CGUID+203, 0, 0x0, 0x1, '164478'), -- 78489 - 164478 - 164478
(@CGUID+204, 0, 0x0, 0x1, '164478'), -- 78489 - 164478 - 164478
(@CGUID+205, 0, 0x0, 0x1, ''), -- 78348
(@CGUID+206, 0, 0x1, 0x1, ''), -- 78345
(@CGUID+207, 0, 0x0, 0x1, ''), -- 78348
(@CGUID+208, 0, 0x0, 0x1, ''), -- 78488
(@CGUID+209, 0, 0x0, 0x1, ''), -- 78348
(@CGUID+210, 0, 0x0, 0x1, ''), -- 78348
(@CGUID+211, 0, 0x0, 0x1, '164478'), -- 78489 - 164478 - 164478
(@CGUID+212, 0, 0x0, 0x1, ''), -- 78348
(@CGUID+213, 0, 0x0, 0x1, '164478'), -- 78489 - 164478 - 164478
(@CGUID+214, 0, 0x0, 0x1, ''), -- 78345
(@CGUID+215, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+216, 0, 0x0, 0x1, '164478'), -- 78489 - 164478 - 164478
(@CGUID+217, 0, 0x0, 0x1, ''), -- 9699
(@CGUID+218, 0, 0x0, 0x1, ''), -- 78345
(@CGUID+219, 0, 0x0, 0x1, '164478'), -- 78489 - 164478 - 164478
(@CGUID+220, 0, 0x0, 0x1, ''), -- 61325
(@CGUID+221, 0, 0x0, 0x1, ''), -- 78348
(@CGUID+222, 0, 0x0, 0x1, ''), -- 78488
(@CGUID+223, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+224, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+225, 0, 0x0, 0x0, '159982'), -- 77640 - 159982 - 159982
(@CGUID+226, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+227, 0, 0x0, 0x1, ''), -- 78488
(@CGUID+228, 0, 0x0, 0x1, '164478'), -- 78489 - 164478 - 164478
(@CGUID+229, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+230, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+231, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+232, 0, 0x0, 0x1, ''), -- 78345
(@CGUID+233, 0, 0x0, 0x1, ''), -- 9699
(@CGUID+234, 0, 0x0, 0x1, ''), -- 77101
(@CGUID+235, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+236, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+237, 0, 0x0, 0x1, ''), -- 61328
(@CGUID+238, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+239, 0, 0x8, 0x1, '86603 164460'), -- 77724 - 86603 - 86603, 164460 - 164460
(@CGUID+240, 0, 0x0, 0x1, '164460'), -- 77724 - 164460 - 164460
(@CGUID+241, 0, 0x8, 0x1, '86603 164460'), -- 77724 - 86603 - 86603, 164460 - 164460
(@CGUID+242, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+243, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+244, 0, 0x0, 0x1, '164470'), -- 77767 - 164470 - 164470
(@CGUID+245, 0, 0x0, 0x1, ''), -- 9699
(@CGUID+246, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+247, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+248, 0, 0x0, 0x1, ''), -- 77723
(@CGUID+249, 0, 0x0, 0x1, ''), -- 77723
(@CGUID+250, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+251, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+252, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+253, 0, 0x0, 0x1, '164470'), -- 77767 - 164470 - 164470
(@CGUID+254, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+255, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+256, 0, 0x8, 0x1, '86603 164460'), -- 77724 - 86603 - 86603, 164460 - 164460
(@CGUID+257, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+258, 0, 0x8, 0x1, '86603 164460'), -- 77724 - 86603 - 86603, 164460 - 164460
(@CGUID+259, 0, 0x0, 0x1, ''), -- 77723
(@CGUID+260, 0, 0x0, 0x1, ''), -- 15476
(@CGUID+261, 0, 0x0, 0x0, ''), -- 77723
(@CGUID+262, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+263, 0, 0x0, 0x1, ''), -- 77101
(@CGUID+264, 0, 0x8, 0x1, '86603 164460'), -- 77724 - 86603 - 86603, 164460 - 164460
(@CGUID+265, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+266, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+267, 0, 0x0, 0x1, '164460'), -- 77724 - 164460 - 164460
(@CGUID+268, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+269, 0, 0x0, 0x1, ''), -- 77723
(@CGUID+270, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+271, 0, 0x8, 0x1, '86603 164460'), -- 77724 - 86603 - 86603, 164460 - 164460
(@CGUID+272, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+273, 0, 0x8, 0x1, '86603 164460'), -- 77724 - 86603 - 86603, 164460 - 164460
(@CGUID+274, 0, 0x8, 0x1, '86603 164460'), -- 77724 - 86603 - 86603, 164460 - 164460
(@CGUID+275, 0, 0x0, 0x1, ''), -- 77771
(@CGUID+276, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+277, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+278, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+279, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+280, 0, 0x0, 0x1, ''), -- 61326
(@CGUID+281, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+282, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+283, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+284, 0, 0x0, 0x102, '18950'), -- 76085 - 18950 - 18950
(@CGUID+285, 0, 0x0, 0x1, ''), -- 3300
(@CGUID+286, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+287, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+288, 0, 0x0, 0x102, '18950'), -- 76085 - 18950 - 18950
(@CGUID+289, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+290, 0, 0x0, 0x102, '18950'), -- 76085 - 18950 - 18950
(@CGUID+291, 0, 0x0, 0x0, ''), -- 82447
(@CGUID+292, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+293, 0, 0x0, 0x1, ''), -- 9699
(@CGUID+294, 0, 0x1, 0x1, ''), -- 76111
(@CGUID+295, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+296, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+297, 0, 0x0, 0x102, '18950'), -- 76085 - 18950 - 18950
(@CGUID+298, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+299, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+300, 0, 0x0, 0x1, ''), -- 9699
(@CGUID+301, 0, 0x0, 0x1, ''), -- 82273
(@CGUID+302, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+303, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+304, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+305, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+306, 0, 0x0, 0x101, '133067 18950'), -- 76524 - 133067 - 133067, 18950 - 18950
(@CGUID+307, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+308, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+309, 0, 0x0, 0x1, ''), -- 61325
(@CGUID+310, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+311, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+312, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+313, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+314, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+315, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+316, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+317, 0, 0x0, 0x2, ''), -- 76651
(@CGUID+318, 0, 0x0, 0x1, ''), -- 15476
(@CGUID+319, 0, 0x0, 0x1, ''), -- 82273
(@CGUID+320, 0, 0x0, 0x1, ''), -- 82273
(@CGUID+321, 0, 0x0, 0x1, ''), -- 82273
(@CGUID+322, 0, 0x0, 0x1, ''), -- 82273
(@CGUID+323, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+324, 0, 0x0, 0x2, ''), -- 82774
(@CGUID+325, 0, 0x0, 0x1, ''), -- 61326
(@CGUID+326, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+327, 0, 0x8, 0x1, ''), -- 82850
(@CGUID+328, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+329, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+330, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+331, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+332, 0, 0x8, 0x1, ''), -- 82850
(@CGUID+333, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+334, 0, 0x8, 0x1, ''), -- 82850
(@CGUID+335, 0, 0x8, 0x1, ''), -- 82850
(@CGUID+336, 0, 0x0, 0x1, ''), -- 9699
(@CGUID+337, 0, 0x0, 0x1, ''), -- 61327
(@CGUID+338, 0, 0x0, 0x2, ''), -- 82774
(@CGUID+339, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+340, 0, 0x0, 0x2, ''), -- 82774
(@CGUID+341, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+342, 0, 0x0, 0x2, ''), -- 76651
(@CGUID+343, 0, 0x8, 0x1, ''), -- 82850
(@CGUID+344, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+345, 0, 0x0, 0x2, ''), -- 82774
(@CGUID+346, 0, 0x8, 0x1, ''), -- 82850
(@CGUID+347, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+348, 0, 0x0, 0x1, '114371'), -- 41158 - 114371 - 114371
(@CGUID+349, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+350, 0, 0x0, 0x1, ''), -- 78142
(@CGUID+351, 0, 0x0, 0x1, ''), -- 15476
(@CGUID+352, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+353, 0, 0x0, 0x2, ''), -- 82774
(@CGUID+354, 0, 0x0, 0x1, '114371'), -- 41158 - 114371 - 114371
(@CGUID+355, 0, 0x8, 0x1, ''), -- 82850
(@CGUID+356, 0, 0x0, 0x2, ''), -- 82774
(@CGUID+357, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+358, 0, 0x0, 0x1, ''), -- 9699
(@CGUID+359, 0, 0x0, 0x2, ''), -- 82774
(@CGUID+360, 0, 0x8, 0x1, ''), -- 82850
(@CGUID+361, 0, 0x0, 0x1, ''), -- 77643
(@CGUID+362, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+363, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+364, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+365, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+366, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+367, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+368, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+369, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+370, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+371, 0, 0x0, 0x1, ''), -- 61328
(@CGUID+372, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+373, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+374, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+375, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+376, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+377, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+378, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+379, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+380, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+381, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+382, 0, 0x0, 0x1, ''), -- 9699
(@CGUID+383, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+384, 0, 0x0, 0x1, '160098'), -- 77644 - 160098 - 160098
(@CGUID+385, 0, 0x0, 0x1, ''), -- 77721
(@CGUID+386, 54975, 0x0, 0x1, '160055'), -- 77790 - 160055 - 160055
(@CGUID+387, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+388, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+389, 0, 0x0, 0x1, '114371'), -- 44311 - 114371 - 114371
(@CGUID+390, 0, 0x0, 0x1, '83811'), -- 76673 - 83811 - 83811
(@CGUID+391, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+392, 0, 0x0, 0x0, ''), -- 76556
(@CGUID+393, 0, 0x0, 0x0, '159982'), -- 77640 - 159982 - 159982
(@CGUID+394, 0, 0x0, 0x1, '83811'), -- 76673 - 83811 - 83811
(@CGUID+395, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+396, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+397, 0, 0x0, 0x1, '83811'), -- 76673 - 83811 - 83811
(@CGUID+398, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+399, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+400, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+401, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+402, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+403, 0, 0x0, 0x1, '29266'), -- 78141 - 29266 - 29266
(@CGUID+404, 54975, 0x0, 0x1, '160055'), -- 77790 - 160055 - 160055
(@CGUID+405, 0, 0x0, 0x1, ''), -- 77845
(@CGUID+406, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+407, 0, 0x0, 0x1, '29266'), -- 78141 - 29266 - 29266
(@CGUID+408, 0, 0x0, 0x1, '29266'), -- 78141 - 29266 - 29266
(@CGUID+409, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+410, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+411, 0, 0x8, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+412, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+413, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+414, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+415, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+416, 0, 0x0, 0x1, ''), -- 14881
(@CGUID+417, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+418, 0, 0x0, 0x1, ''), -- 61327
(@CGUID+419, 0, 0x0, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+420, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+421, 0, 0x0, 0x0, '159982'), -- 77640 - 159982 - 159982
(@CGUID+422, 0, 0x0, 0x1, '114371'), -- 44313 - 114371 - 114371
(@CGUID+423, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+424, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+425, 0, 0x0, 0x1, ''), -- 77845
(@CGUID+426, 0, 0x0, 0x1, ''), -- 77845
(@CGUID+427, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+428, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+429, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+430, 54975, 0x0, 0x1, '160055'), -- 77790 - 160055 - 160055
(@CGUID+431, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+432, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+433, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+434, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+435, 0, 0x0, 0x0, ''), -- 76556
(@CGUID+436, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+437, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+438, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+439, 0, 0x0, 0x0, '159982'), -- 77640 - 159982 - 159982
(@CGUID+440, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+441, 0, 0x0, 0x0, ''), -- 77090
(@CGUID+442, 0, 0x0, 0x0, ''), -- 76556
(@CGUID+443, 0, 0x0, 0x0, ''), -- 73446
(@CGUID+444, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+445, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+446, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+447, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+448, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+449, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+450, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+451, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+452, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+453, 0, 0x0, 0x1, '83811'), -- 76673 - 83811 - 83811
(@CGUID+454, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+455, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+456, 0, 0x0, 0x0, '114371'), -- 41125 - 114371 - 114371
(@CGUID+457, 54975, 0x0, 0x1, '160055'), -- 77790 - 160055 - 160055
(@CGUID+458, 0, 0x0, 0x1, '159982'), -- 77640 - 159982 - 159982
(@CGUID+459, 0, 0x0, 0x1, ''), -- 61328
(@CGUID+460, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+461, 0, 0x0, 0x1, '83811'), -- 76673 - 83811 - 83811
(@CGUID+462, 0, 0x0, 0x2, '158339 153776'), -- 73459 - 158339 - 158339, 153776 - 153776
(@CGUID+463, 0, 0x0, 0x1, '83811'), -- 76673 - 83811 - 83811
(@CGUID+464, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+465, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+466, 0, 0x8, 0x1, ''), -- 76556
(@CGUID+467, 0, 0x0, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+468, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+469, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+470, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+471, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+472, 0, 0x0, 0x1, ''), -- 77643
(@CGUID+473, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+474, 0, 0x0, 0x1, '168461'), -- 84199 - 168461 - 168461
(@CGUID+475, 0, 0x0, 0x1, ''), -- 77643
(@CGUID+476, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+477, 0, 0x0, 0x1, ''), -- 9699
(@CGUID+478, 0, 0x0, 0x1, ''), -- 76673
(@CGUID+479, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+480, 0, 0x0, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+481, 0, 0x0, 0x0, ''), -- 76098
(@CGUID+482, 0, 0x0, 0x102, '18950'), -- 76085 - 18950 - 18950
(@CGUID+483, 0, 0x0, 0x1, ''), -- 77643
(@CGUID+484, 0, 0x0, 0x0, ''), -- 76117
(@CGUID+485, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+486, 9473, 0x0, 0x100, ''), -- 82448
(@CGUID+487, 0, 0x0, 0x0, ''), -- 76098
(@CGUID+488, 0, 0x0, 0x0, ''), -- 76098
(@CGUID+489, 0, 0x0, 0x101, '18950'), -- 76085 - 18950 - 18950
(@CGUID+490, 0, 0x7, 0x1, ''), -- 76103
(@CGUID+491, 0, 0x0, 0x102, '18950'), -- 76085 - 18950 - 18950
(@CGUID+492, 0, 0x0, 0x0, ''), -- 76103
(@CGUID+493, 0, 0x0, 0x1, ''), -- 76108
(@CGUID+494, 0, 0x0, 0x1, ''), -- 82270
(@CGUID+495, 0, 0x0, 0x1, ''), -- 76098
(@CGUID+496, 0, 0x0, 0x100, ''), -- 76436
(@CGUID+497, 0, 0x0, 0x0, ''), -- 76098
(@CGUID+498, 0, 0x0, 0x1, '168461'), -- 84199 - 168461 - 168461
(@CGUID+499, 0, 0x0, 0x102, '18950'), -- 76085 - 18950 - 18950
(@CGUID+500, 0, 0x0, 0x102, '18950'), -- 76085 - 18950 - 18950
(@CGUID+501, 0, 0x0, 0x0, ''), -- 76098
(@CGUID+502, 0, 0x0, 0x1, ''), -- 61325
(@CGUID+503, 0, 0x0, 0x1, ''), -- 76103
(@CGUID+504, 0, 0x0, 0x1, ''), -- 76103
(@CGUID+505, 0, 0x0, 0x101, '18950'), -- 76085 - 18950 - 18950
(@CGUID+506, 0, 0x0, 0x1, ''), -- 77090
(@CGUID+507, 0, 0x0, 0x1, ''), -- 76110
(@CGUID+508, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+509, 0, 0x3, 0x1, ''), -- 76103
(@CGUID+510, 0, 0x0, 0x102, '18950'), -- 76085 - 18950 - 18950
(@CGUID+511, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+512, 0, 0x0, 0x1, ''), -- 76429
(@CGUID+513, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+514, 0, 0x3, 0x1, ''), -- 76103
(@CGUID+515, 0, 0x0, 0x102, '18950'), -- 76085 - 18950 - 18950
(@CGUID+516, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+517, 0, 0x0, 0x1, ''), -- 76429
(@CGUID+518, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+519, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+520, 0, 0x8, 0x1, ''), -- 76429
(@CGUID+521, 0, 0x1, 0x1, ''), -- 76103
(@CGUID+522, 0, 0x7, 0x1, ''), -- 76103
(@CGUID+523, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+524, 0, 0x3, 0x1, ''), -- 76103
(@CGUID+525, 0, 0x0, 0x101, '18950'), -- 76085 - 18950 - 18950
(@CGUID+526, 0, 0x7, 0x1, ''), -- 76103
(@CGUID+527, 0, 0x0, 0x101, ''), -- 76448
(@CGUID+528, 0, 0x8, 0x1, ''), -- 76429
(@CGUID+529, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+530, 0, 0x0, 0x1, ''), -- 84199
(@CGUID+531, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+532, 0, 0x0, 0x1, ''), -- 76429
(@CGUID+533, 0, 0x0, 0x1, ''), -- 76429
(@CGUID+534, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+535, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+536, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+537, 0, 0x0, 0x1, ''), -- 76098
(@CGUID+538, 0, 0x8, 0x1, ''), -- 76429
(@CGUID+539, 0, 0x0, 0x0, '132693'), -- 76098 - 132693 - 132693
(@CGUID+540, 0, 0x0, 0x102, ''), -- 76436
(@CGUID+541, 0, 0x0, 0x1, ''), -- 76429
(@CGUID+542, 0, 0x0, 0x1, ''), -- 61328
(@CGUID+543, 0, 0x0, 0x0, '132693'), -- 76098 - 132693 - 132693
(@CGUID+544, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+545, 0, 0x0, 0x1, ''), -- 76429
(@CGUID+546, 0, 0x0, 0x1, ''), -- 76098
(@CGUID+547, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+548, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+549, 0, 0x0, 0x102, ''), -- 76436
(@CGUID+550, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+551, 0, 0x0, 0x1, '168461'), -- 84199 - 168461 - 168461
(@CGUID+552, 0, 0x0, 0x1, ''), -- 76429
(@CGUID+553, 0, 0x8, 0x1, ''), -- 76429
(@CGUID+554, 0, 0x0, 0x102, ''), -- 76436
(@CGUID+555, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+556, 0, 0x0, 0x1, ''), -- 76429
(@CGUID+557, 0, 0x0, 0x102, ''), -- 76436
(@CGUID+558, 0, 0x0, 0x1, '168461'), -- 84199 - 168461 - 168461
(@CGUID+559, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+560, 0, 0x0, 0x1, ''), -- 76429
(@CGUID+561, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+562, 0, 0x0, 0x1, ''), -- 16225
(@CGUID+563, 0, 0x0, 0x0, '132693'), -- 76098 - 132693 - 132693
(@CGUID+564, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+565, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+566, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+567, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+568, 0, 0x0, 0x1, '168461'), -- 84199 - 168461 - 168461
(@CGUID+569, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+570, 0, 0x8, 0x1, ''), -- 76429
(@CGUID+571, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+572, 0, 0x0, 0x1, ''), -- 76429
(@CGUID+573, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+574, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+575, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+576, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+577, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+578, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+579, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+580, 0, 0x0, 0x1, '168461'), -- 84199 - 168461 - 168461
(@CGUID+581, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+582, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+583, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+584, 0, 0x8, 0x1, ''), -- 76503
(@CGUID+585, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+586, 0, 0x0, 0x1, ''), -- 76503
(@CGUID+587, 0, 0x0, 0x1, ''), -- 61328
(@CGUID+588, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+589, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+590, 0, 0x0, 0x1, ''), -- 76503
(@CGUID+591, 0, 0x0, 0x1, ''), -- 76503
(@CGUID+592, 0, 0x0, 0x1, '168461'), -- 84199 - 168461 - 168461
(@CGUID+593, 0, 0x8, 0x1, ''), -- 76503
(@CGUID+594, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+595, 0, 0x0, 0x1, '168461'), -- 84199 - 168461 - 168461
(@CGUID+596, 0, 0x0, 0x1, ''), -- 76503
(@CGUID+597, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+598, 0, 0x0, 0x1, ''), -- 76503
(@CGUID+599, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+600, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+601, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+602, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+603, 0, 0x0, 0x101, ''), -- 76504
(@CGUID+604, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+605, 0, 0x8, 0x1, ''), -- 76503
(@CGUID+606, 0, 0x0, 0x1, '159782'), -- 76189 - 159782 - 159782
(@CGUID+607, 0, 0x0, 0x1, ''), -- 76503
(@CGUID+608, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+609, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+610, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+611, 0, 0x8, 0x1, ''), -- 76503
(@CGUID+612, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+613, 0, 0x0, 0x1, ''), -- 76503
(@CGUID+614, 0, 0x0, 0x1, ''), -- 76503
(@CGUID+615, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+616, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+617, 0, 0x0, 0x1, ''), -- 76503
(@CGUID+618, 0, 0x0, 0x1, ''), -- 76503
(@CGUID+619, 0, 0x0, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+620, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+621, 0, 0x8, 0x1, ''), -- 76503
(@CGUID+622, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+623, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+624, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+625, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+626, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+627, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+628, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+629, 0, 0x0, 0x1, ''), -- 15476
(@CGUID+630, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+631, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+632, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+633, 0, 0x0, 0x1, '168461'), -- 84199 - 168461 - 168461
(@CGUID+634, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+635, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+636, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+637, 0, 0x0, 0x1, ''), -- 9699
(@CGUID+638, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+639, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+640, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+641, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+642, 0, 0x0, 0x1, ''), -- 61328
(@CGUID+643, 0, 0x0, 0x1, ''), -- 14881
(@CGUID+644, 0, 0x0, 0x1, ''), -- 84199
(@CGUID+645, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+646, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+647, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+648, 0, 0x0, 0x1, ''), -- 9699
(@CGUID+649, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+650, 0, 0x0, 0x101, ''), -- 76436
(@CGUID+651, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+652, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+653, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+654, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+655, 0, 0x0, 0x1, ''), -- 77643
(@CGUID+656, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+657, 0, 0x0, 0x1, ''), -- 76651
(@CGUID+658, 0, 0x8, 0x1, ''), -- 82850
(@CGUID+659, 0, 0x0, 0x2, ''), -- 82774
(@CGUID+660, 0, 0x0, 0x1, ''), -- 9699
(@CGUID+661, 0, 0x0, 0x1, ''), -- 76189
(@CGUID+662, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+663, 0, 0x0, 0x1, ''), -- 76651
(@CGUID+664, 0, 0x0, 0x2, ''), -- 76651
(@CGUID+665, 0, 0x0, 0x2, ''), -- 82774
(@CGUID+666, 0, 0x0, 0x1, ''), -- 61328
(@CGUID+667, 0, 0x0, 0x1, '114371'), -- 41158 - 114371 - 114371
(@CGUID+668, 0, 0x0, 0x1, ''), -- 61326
(@CGUID+669, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+670, 0, 0x8, 0x1, ''), -- 82850
(@CGUID+671, 0, 0x0, 0x1, '114371'), -- 41158 - 114371 - 114371
(@CGUID+672, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+673, 0, 0x8, 0x1, ''), -- 82850
(@CGUID+674, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+675, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+676, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+677, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+678, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+679, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+680, 0, 0x0, 0x1, ''), -- 78667
(@CGUID+681, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+682, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+683, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+684, 0, 0x0, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+685, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+686, 0, 0x0, 0x101, '18950 133067'), -- 76524 - 18950 - 18950, 133067 - 133067
(@CGUID+687, 0, 0x0, 0x1, ''), -- 76521
(@CGUID+688, 0, 0x0, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+689, 0, 0x0, 0x1, ''), -- 82851
(@CGUID+690, 0, 0x0, 0x101, '18950 133067'), -- 76524 - 18950 - 18950, 133067 - 133067
(@CGUID+691, 0, 0x0, 0x101, '18950 133067'), -- 76524 - 18950 - 18950, 133067 - 133067
(@CGUID+692, 0, 0x0, 0x1, ''), -- 76522
(@CGUID+693, 0, 0x1, 0x101, ''), -- 85247
(@CGUID+694, 0, 0x0, 0x101, '18950 133067'), -- 76524 - 18950 - 18950, 133067 - 133067
(@CGUID+695, 0, 0x0, 0x101, ''), -- 76500
(@CGUID+696, 0, 0x0, 0x101, '18950 133067'), -- 76524 - 18950 - 18950, 133067 - 133067
(@CGUID+697, 0, 0x0, 0x1, ''), -- 76520
(@CGUID+698, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+699, 0, 0x0, 0x101, '18950 133067'), -- 76524 - 18950 - 18950, 133067 - 133067
(@CGUID+700, 0, 0x0, 0x101, '18950 133067'), -- 76524 - 18950 - 18950, 133067 - 133067
(@CGUID+701, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+702, 0, 0x0, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+703, 0, 0x0, 0x1, ''), -- 76519
(@CGUID+704, 0, 0x0, 0x1, ''), -- 15476
(@CGUID+705, 0, 0x0, 0x1, ''), -- 76886
(@CGUID+706, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+707, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+708, 0, 0x0, 0x101, '18950 133067'), -- 76524 - 18950 - 18950, 133067 - 133067
(@CGUID+709, 0, 0x0, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+710, 0, 0x0, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+711, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+712, 0, 0x0, 0x101, ''), -- 85734
(@CGUID+713, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+714, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+715, 0, 0x8, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+716, 0, 0x0, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+717, 0, 0x0, 0x1, '86603'), -- 76886 - 86603 - 86603
(@CGUID+718, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+719, 0, 0x0, 0x1, ''), -- 61325
(@CGUID+720, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+721, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+722, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+723, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+724, 0, 0x0, 0x1, '12898'), -- 5992 - 12898 - 12898
(@CGUID+725, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+726, 0, 0x0, 0x1, ''), -- 15476
(@CGUID+727, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+728, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+729, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+730, 0, 0x0, 0x1, ''), -- 5990
(@CGUID+731, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+732, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+733, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+734, 0, 0x0, 0x1, ''), -- 15476
(@CGUID+735, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+736, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+737, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+738, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+739, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+740, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+741, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+742, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+743, 0, 0x0, 0x1, ''), -- 3300
(@CGUID+744, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+745, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+746, 0, 0x0, 0x1, ''), -- 15476
(@CGUID+747, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+748, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+749, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+750, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+751, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+752, 0, 0x0, 0x1, ''), -- 15476
(@CGUID+753, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+754, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+755, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+756, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+757, 0, 0x0, 0x1, ''), -- 5985
(@CGUID+758, 0, 0x0, 0x1, ''), -- 15476
(@CGUID+759, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+760, 0, 0x0, 0x1, ''), -- 61326
(@CGUID+761, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+762, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+763, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+764, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+765, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+766, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+767, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+768, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+769, 0, 0x0, 0x1, ''), -- 3300
(@CGUID+770, 0, 0x0, 0x1, '84161'), -- 45087 - 84161 - 84161
(@CGUID+771, 0, 0x0, 0x1, ''), -- 15476
(@CGUID+772, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+773, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+774, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+775, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+776, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+777, 0, 0x3000000, 0x1, ''), -- 5983
(@CGUID+778, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+779, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+780, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+781, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+782, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+783, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+784, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+785, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+786, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+787, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+788, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+789, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+790, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+791, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+792, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+793, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+794, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+795, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+796, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+797, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+798, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+799, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+800, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+801, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+802, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+803, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+804, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+805, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+806, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+807, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+808, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+809, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+810, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+811, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+812, 0, 0x0, 0x1, '159944'), -- 76556 - 159944 - 159944
(@CGUID+813, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+814, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+815, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+816, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+817, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+818, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+819, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+820, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+821, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+822, 0, 0x0, 0x1, ''), -- 76556
(@CGUID+823, 0, 0x0, 0x1, ''); -- 76556


INSERT INTO `waypoint_data` (`id`, `point`, `entry`, `position_x`, `position_y`, `position_z`) VALUES
(@CGUID+180, 1, 78489, -11580.29, -2751.563, 6.416425),
(@CGUID+180, 0, 78489, -11581.18, -2752.013, 6.416425),
(@CGUID+180, 3, 78489, -11585.72, -2754.302, 6.337568),
(@CGUID+180, 2, 78489, -11585.72, -2754.302, 6.337568),
(@CGUID+145, 1, 78667, -11776.84, -3019.505, 24.79493),
(@CGUID+145, 0, 78667, -11777.81, -3019.275, 24.79493),
(@CGUID+145, 3, 78667, -11761.7, -3020.127, 25.23831),
(@CGUID+145, 2, 78667, -11761.7, -3020.127, 25.23831),
(@CGUID+119, 1, 76556, -11904.82, -3211.245, -11.56851),
(@CGUID+119, 0, 76556, -11905.82, -3211.312, -11.56851),
(@CGUID+119, 3, 76556, -11893.73, -3209.299, -11.49278),
(@CGUID+119, 2, 76556, -11894.94, -3209.512, -11.59181),
(@CGUID+119, 5, 76556, -11892.31, -3211.014, -12.19763),
(@CGUID+119, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+119, 7, 76556, -11886.64, -3209.621, -13.88664),
(@CGUID+119, 6, 76556, -11891.26, -3210.755, -12.33153),
(@CGUID+119, 9, 76556, -11880.54, -3208.124, -15.91559),
(@CGUID+119, 8, 76556, -11883.76, -3208.915, -14.9311),
(@CGUID+119, 11, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+119, 10, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+37, 0, 82189, -11838.44, -3193.373, -29.92892),
(@CGUID+37, 1, 82189, -11839.41, -3193.59, -29.92892),
(@CGUID+37, 2, 82189, -11838.52, -3191.986, -29.92754),
(@CGUID+37, 3, 82189, -11838.52, -3191.986, -29.92754),
(@CGUID+813, 2, 76556, -11895.37, -3205.011, -11.60832),
(@CGUID+813, 3, 76556, -11894.95, -3204.945, -11.57077),
(@CGUID+813, 0, 76556, -11906.95, -3206.747, -11.64358),
(@CGUID+813, 1, 76556, -11905.96, -3206.681, -11.64358),
(@CGUID+813, 6, 76556, -11887.22, -3203.381, -13.94922),
(@CGUID+813, 7, 76556, -11881.18, -3203.098, -15.89048),
(@CGUID+813, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+813, 5, 76556, -11892.58, -3203.497, -12.22616),
(@CGUID+813, 8, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+813, 9, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+640, 9, 5983, -11526.4, -3451.58, 72.43459),
(@CGUID+640, 8, 5983, -11522.59, -3470.714, 72.43459),
(@CGUID+640, 11, 5983, -11511.75, -3416.225, 72.43459),
(@CGUID+640, 10, 5983, -11522.59, -3432.446, 72.43459),
(@CGUID+640, 13, 5983, -11476.4, -3401.58, 72.43459),
(@CGUID+640, 12, 5983, -11495.53, -3405.386, 72.43459),
(@CGUID+640, 15, 5983, -11441.04, -3416.225, 72.43459),
(@CGUID+640, 14, 5983, -11457.27, -3405.386, 72.43459),
(@CGUID+640, 1, 5983, -11426.4, -3451.58, 72.43459),
(@CGUID+640, 0, 5983, -11430.21, -3432.446, 72.43459),
(@CGUID+640, 3, 5983, -11441.04, -3486.935, 72.43459),
(@CGUID+640, 2, 5983, -11430.21, -3470.714, 72.43459),
(@CGUID+640, 5, 5983, -11476.4, -3501.58, 72.43459),
(@CGUID+640, 4, 5983, -11457.27, -3497.774, 72.43459),
(@CGUID+640, 7, 5983, -11511.75, -3486.935, 72.43459),
(@CGUID+640, 6, 5983, -11495.53, -3497.774, 72.43459),
(@CGUID+640, 17, 5983, -11426.4, -3451.58, 72.43459),
(@CGUID+640, 16, 5983, -11430.21, -3432.446, 72.43459),
(@CGUID+640, 18, 5983, -11430.21, -3470.714, 72.43459),
(@CGUID+105, 8, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+105, 9, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+105, 0, 76556, -11907.14, -3203.174, -11.5685),
(@CGUID+105, 1, 76556, -11906.14, -3203.108, -11.5685),
(@CGUID+105, 2, 76556, -11895.49, -3202.495, -11.60509),
(@CGUID+105, 3, 76556, -11894.45, -3202.363, -11.53603),
(@CGUID+105, 4, 76556, -11892.72, -3202.146, -12.16765),
(@CGUID+105, 5, 76556, -11887.48, -3201.484, -13.79149),
(@CGUID+105, 6, 76556, -11879.43, -3200.469, -16.53307),
(@CGUID+105, 7, 76556, -11875.71, -3200, -17.98649),
(@CGUID+820, 0, 76556, -11906.95, -3206.747, -11.64358),
(@CGUID+820, 1, 76556, -11905.96, -3206.681, -11.64358),
(@CGUID+820, 2, 76556, -11895.37, -3205.011, -11.60832),
(@CGUID+820, 3, 76556, -11894.95, -3204.945, -11.57077),
(@CGUID+820, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+820, 5, 76556, -11892.58, -3203.497, -12.22616),
(@CGUID+820, 6, 76556, -11887.22, -3203.381, -13.94922),
(@CGUID+820, 7, 76556, -11881.18, -3203.098, -15.89048),
(@CGUID+820, 8, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+820, 9, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+388, 0, 76556, -11127.58, -3442.358, 65.03131),
(@CGUID+388, 1, 76556, -11128.48, -3441.932, 65.03131),
(@CGUID+388, 2, 76556, -11137.84, -3443.67, 65.11285),
(@CGUID+388, 3, 76556, -11137.84, -3443.67, 65.11285),
(@CGUID+342, 4, 76651, -11291.92, -3352.918, 9.254149),
(@CGUID+342, 1, 76651, -11279.94, -3347.831, 9.447594),
(@CGUID+342, 0, 76651, -11278.96, -3347.619, 9.447594),
(@CGUID+342, 3, 76651, -11291.92, -3352.918, 9.254149),
(@CGUID+342, 2, 76651, -11283.08, -3348.512, 9.298095),
(@CGUID+94, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+94, 5, 76556, -11892.31, -3211.014, -12.19763),
(@CGUID+94, 6, 76556, -11891.26, -3210.755, -12.33153),
(@CGUID+94, 7, 76556, -11886.64, -3209.621, -13.88664),
(@CGUID+94, 0, 76556, -11905.82, -3211.312, -11.56851),
(@CGUID+94, 1, 76556, -11904.82, -3211.245, -11.56851),
(@CGUID+94, 2, 76556, -11894.94, -3209.512, -11.59181),
(@CGUID+94, 3, 76556, -11893.73, -3209.299, -11.49278),
(@CGUID+94, 8, 76556, -11883.76, -3208.915, -14.9311),
(@CGUID+94, 9, 76556, -11880.54, -3208.124, -15.91559),
(@CGUID+94, 10, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+94, 11, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+745, 18, 5983, -12058.21, -3525.724, 149.0013),
(@CGUID+745, 17, 5983, -12054.4, -3506.59, 149.0013),
(@CGUID+745, 16, 5983, -12058.21, -3487.456, 149.0013),
(@CGUID+96, 6, 76556, -11887.06, -3210.237, -13.66055),
(@CGUID+96, 7, 76556, -11880.78, -3209.241, -15.79766),
(@CGUID+96, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+745, 3, 5983, -12069.04, -3541.945, 149.0013),
(@CGUID+96, 5, 76556, -11891.23, -3210.898, -12.31716),
(@CGUID+745, 2, 5983, -12058.21, -3525.724, 149.0013),
(@CGUID+96, 2, 76556, -11894.81, -3210.88, -11.58679),
(@CGUID+745, 1, 5983, -12054.4, -3506.59, 149.0013),
(@CGUID+96, 3, 76556, -11894.46, -3210.811, -11.54245),
(@CGUID+745, 0, 5983, -12058.21, -3487.456, 149.0013),
(@CGUID+96, 0, 76556, -11905.43, -3212.839, -11.52012),
(@CGUID+745, 7, 5983, -12139.76, -3541.945, 149.0013),
(@CGUID+96, 1, 76556, -11904.43, -3212.773, -11.52012),
(@CGUID+745, 6, 5983, -12123.53, -3552.784, 149.0013),
(@CGUID+745, 5, 5983, -12104.4, -3556.59, 149.0013),
(@CGUID+745, 4, 5983, -12085.27, -3552.784, 149.0013),
(@CGUID+745, 11, 5983, -12139.76, -3471.235, 149.0013),
(@CGUID+745, 10, 5983, -12150.59, -3487.456, 149.0013),
(@CGUID+745, 9, 5983, -12154.4, -3506.59, 149.0013),
(@CGUID+745, 8, 5983, -12150.59, -3525.724, 149.0013),
(@CGUID+96, 8, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+745, 15, 5983, -12069.04, -3471.235, 149.0013),
(@CGUID+96, 9, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+745, 14, 5983, -12085.27, -3460.396, 149.0013),
(@CGUID+745, 13, 5983, -12104.4, -3456.59, 149.0013),
(@CGUID+745, 12, 5983, -12123.53, -3460.396, 149.0013),
(@CGUID+688, 4, 76886, -11729.99, -3821.248, 58.92601),
(@CGUID+688, 5, 76886, -11729.99, -3821.248, 58.92601),
(@CGUID+688, 0, 76886, -11732.14, -3821.141, 57.94505),
(@CGUID+688, 1, 76886, -11731.67, -3822.026, 57.94505),
(@CGUID+688, 2, 76886, -11730.53, -3824.003, 57.62108),
(@CGUID+688, 3, 76886, -11730.14, -3822.043, 58.60789),
(@CGUID+818, 8, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+818, 9, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+818, 4, 76556, -11892.72, -3202.146, -12.16765),
(@CGUID+818, 5, 76556, -11887.48, -3201.484, -13.79149),
(@CGUID+818, 6, 76556, -11879.43, -3200.469, -16.53307),
(@CGUID+818, 7, 76556, -11875.71, -3200, -17.98649),
(@CGUID+818, 0, 76556, -11907.14, -3203.174, -11.5685),
(@CGUID+818, 1, 76556, -11906.14, -3203.108, -11.5685),
(@CGUID+818, 2, 76556, -11895.49, -3202.495, -11.60509),
(@CGUID+818, 3, 76556, -11894.45, -3202.363, -11.53603),
(@CGUID+790, 8, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+790, 9, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+790, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+790, 5, 76556, -11892.56, -3203.622, -12.23155),
(@CGUID+790, 6, 76556, -11889.12, -3203.736, -13.35396),
(@CGUID+790, 7, 76556, -11881.07, -3204.004, -15.89535),
(@CGUID+790, 0, 76556, -11906.58, -3208.431, -11.63638),
(@CGUID+790, 1, 76556, -11905.59, -3208.365, -11.63638),
(@CGUID+790, 2, 76556, -11895.2, -3206.791, -11.60179),
(@CGUID+790, 3, 76556, -11894.67, -3206.71, -11.55334),
(@CGUID+802, 5, 76556, -11891.29, -3210.586, -12.34856),
(@CGUID+802, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+802, 7, 76556, -11880.66, -3207.246, -15.91098),
(@CGUID+802, 6, 76556, -11887.25, -3209.317, -13.74831),
(@CGUID+802, 1, 76556, -11905.03, -3209.613, -11.61907),
(@CGUID+802, 0, 76556, -11906.03, -3209.679, -11.61907),
(@CGUID+802, 3, 76556, -11893.78, -3207.929, -11.49541),
(@CGUID+802, 2, 76556, -11895.08, -3208.123, -11.5969),
(@CGUID+802, 9, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+802, 8, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+809, 8, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+809, 9, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+809, 0, 76556, -11906.58, -3208.431, -11.63638),
(@CGUID+809, 1, 76556, -11905.59, -3208.365, -11.63638),
(@CGUID+809, 2, 76556, -11895.2, -3206.791, -11.60179),
(@CGUID+809, 3, 76556, -11894.67, -3206.71, -11.55334),
(@CGUID+809, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+809, 5, 76556, -11892.56, -3203.622, -12.23155),
(@CGUID+809, 6, 76556, -11889.12, -3203.736, -13.35396),
(@CGUID+809, 7, 76556, -11881.07, -3204.004, -15.89535),
(@CGUID+748, 18, 5983, -12045.11, -3283.666, 164.3963),
(@CGUID+748, 17, 5983, -12041.3, -3302.8, 164.3963),
(@CGUID+748, 16, 5983, -12045.11, -3321.934, 164.3963),
(@CGUID+748, 7, 5983, -12126.66, -3267.445, 164.3963),
(@CGUID+748, 6, 5983, -12110.43, -3256.606, 164.3963),
(@CGUID+748, 5, 5983, -12091.3, -3252.8, 164.3963),
(@CGUID+748, 4, 5983, -12072.17, -3256.606, 164.3963),
(@CGUID+748, 3, 5983, -12055.94, -3267.445, 164.3963),
(@CGUID+748, 2, 5983, -12045.11, -3283.666, 164.3963),
(@CGUID+748, 1, 5983, -12041.3, -3302.8, 164.3963),
(@CGUID+748, 0, 5983, -12045.11, -3321.934, 164.3963),
(@CGUID+748, 15, 5983, -12055.94, -3338.155, 164.3963),
(@CGUID+748, 14, 5983, -12072.17, -3348.994, 164.3963),
(@CGUID+748, 13, 5983, -12091.3, -3352.8, 164.3963),
(@CGUID+767, 15, 5983, -12591.84, -3139.205, 48.92423),
(@CGUID+121, 2, 76556, -11895.2, -3206.791, -11.60179),
(@CGUID+748, 12, 5983, -12110.43, -3348.994, 164.3963),
(@CGUID+767, 14, 5983, -12608.07, -3150.044, 48.92423),
(@CGUID+121, 3, 76556, -11894.67, -3206.71, -11.55334),
(@CGUID+748, 11, 5983, -12126.66, -3338.155, 164.3963),
(@CGUID+767, 13, 5983, -12627.2, -3153.85, 48.92423),
(@CGUID+121, 0, 76556, -11906.58, -3208.431, -11.63638),
(@CGUID+748, 10, 5983, -12137.49, -3321.934, 164.3963),
(@CGUID+767, 12, 5983, -12646.33, -3150.044, 48.92423),
(@CGUID+121, 1, 76556, -11905.59, -3208.365, -11.63638),
(@CGUID+748, 9, 5983, -12141.3, -3302.8, 164.3963),
(@CGUID+767, 11, 5983, -12662.56, -3139.205, 48.92423),
(@CGUID+121, 6, 76556, -11889.12, -3203.736, -13.35396),
(@CGUID+748, 8, 5983, -12137.49, -3283.666, 164.3963),
(@CGUID+767, 10, 5983, -12673.39, -3122.984, 48.92423),
(@CGUID+121, 7, 76556, -11881.07, -3204.004, -15.89535),
(@CGUID+767, 9, 5983, -12677.2, -3103.85, 48.92423),
(@CGUID+121, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+767, 8, 5983, -12673.39, -3084.716, 48.92423),
(@CGUID+121, 5, 76556, -11892.56, -3203.622, -12.23155),
(@CGUID+767, 7, 5983, -12662.56, -3068.495, 48.92423),
(@CGUID+767, 6, 5983, -12646.33, -3057.656, 48.92423),
(@CGUID+767, 5, 5983, -12627.2, -3053.85, 48.92423),
(@CGUID+121, 8, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+767, 4, 5983, -12608.07, -3057.656, 48.92423),
(@CGUID+121, 9, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+767, 3, 5983, -12591.84, -3068.495, 48.92423),
(@CGUID+767, 2, 5983, -12581.01, -3084.716, 48.92423),
(@CGUID+767, 1, 5983, -12577.2, -3103.85, 48.92423),
(@CGUID+767, 0, 5983, -12581.01, -3122.984, 48.92423),
(@CGUID+701, 1, 5985, -11806.23, -3816.307, 56.92304),
(@CGUID+701, 0, 5985, -11806.12, -3815.313, 56.92304),
(@CGUID+701, 3, 5985, -11805.79, -3812.269, 56.92304),
(@CGUID+701, 2, 5985, -11805.79, -3812.269, 56.92304),
(@CGUID+767, 18, 5983, -12581.01, -3084.716, 48.92423),
(@CGUID+767, 17, 5983, -12577.2, -3103.85, 48.92423),
(@CGUID+767, 16, 5983, -12581.01, -3122.984, 48.92423),
(@CGUID+634, 4, 76189, -11530.62, -3493.609, 13.66477),
(@CGUID+634, 3, 76189, -11493.48, -3503.632, 15.1266),
(@CGUID+634, 2, 76189, -11493.48, -3503.632, 15.1266),
(@CGUID+634, 1, 76189, -11486.5, -3503.224, 14.59218),
(@CGUID+634, 0, 76189, -11485.67, -3503.78, 14.59218),
(@CGUID+710, 4, 76886, -11853.17, -3829.472, 58.42891),
(@CGUID+710, 3, 76886, -11853.17, -3829.472, 58.42891),
(@CGUID+710, 2, 76886, -11858.68, -3836.545, 58.71766),
(@CGUID+710, 1, 76886, -11861.26, -3838.576, 58.43885),
(@CGUID+710, 0, 76886, -11862.14, -3838.107, 58.43885),
(@CGUID+128, 0, 76556, -11906.58, -3208.431, -11.63638),
(@CGUID+128, 1, 76556, -11905.59, -3208.365, -11.63638),
(@CGUID+128, 2, 76556, -11895.2, -3206.791, -11.60179),
(@CGUID+128, 3, 76556, -11894.67, -3206.71, -11.55334),
(@CGUID+128, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+128, 5, 76556, -11892.56, -3203.622, -12.23155),
(@CGUID+128, 6, 76556, -11889.12, -3203.736, -13.35396),
(@CGUID+128, 7, 76556, -11881.07, -3204.004, -15.89535),
(@CGUID+128, 8, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+128, 9, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+110, 8, 76556, -11851.53, -3199.264, -28.401),
(@CGUID+110, 4, 76556, -11863.28, -3201.036, -23.66373),
(@CGUID+110, 5, 76556, -11860.78, -3200.658, -24.56051),
(@CGUID+110, 6, 76556, -11856.41, -3200, -26.30383),
(@CGUID+110, 7, 76556, -11851.53, -3199.264, -28.401),
(@CGUID+110, 0, 76556, -11876.07, -3202.859, -18.40056),
(@CGUID+110, 1, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+110, 2, 76556, -11871.8, -3202.319, -19.98167),
(@CGUID+110, 3, 76556, -11869.57, -3201.983, -20.86848),
(@CGUID+166, 0, 78670, -11724.4, -2811.025, 6.572398),
(@CGUID+166, 1, 78670, -11724.71, -2811.976, 6.572398),
(@CGUID+166, 2, 78670, -11729.81, -2824.498, 6.1952),
(@CGUID+166, 3, 78670, -11729.81, -2824.498, 6.1952),
(@CGUID+805, 9, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+805, 8, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+805, 7, 76556, -11875.71, -3200, -17.98649),
(@CGUID+805, 6, 76556, -11879.43, -3200.469, -16.53307),
(@CGUID+805, 5, 76556, -11887.48, -3201.484, -13.79149),
(@CGUID+805, 4, 76556, -11892.72, -3202.146, -12.16765),
(@CGUID+805, 3, 76556, -11894.45, -3202.363, -11.53603),
(@CGUID+805, 2, 76556, -11895.49, -3202.495, -11.60509),
(@CGUID+805, 1, 76556, -11906.14, -3203.108, -11.5685),
(@CGUID+805, 0, 76556, -11907.14, -3203.174, -11.5685),
(@CGUID+736, 15, 5983, -12090.14, -3902.825, 33.95163),
(@CGUID+736, 14, 5983, -12106.37, -3913.664, 33.95163),
(@CGUID+736, 13, 5983, -12125.5, -3917.47, 33.95163),
(@CGUID+736, 12, 5983, -12144.63, -3913.664, 33.95163),
(@CGUID+736, 11, 5983, -12160.86, -3902.825, 33.95163),
(@CGUID+736, 10, 5983, -12171.69, -3886.604, 33.95163),
(@CGUID+736, 9, 5983, -12175.5, -3867.47, 33.95163),
(@CGUID+736, 8, 5983, -12171.69, -3848.336, 33.95163),
(@CGUID+736, 7, 5983, -12160.86, -3832.115, 33.95163),
(@CGUID+736, 6, 5983, -12144.63, -3821.276, 33.95163),
(@CGUID+736, 5, 5983, -12125.5, -3817.47, 33.95163),
(@CGUID+736, 4, 5983, -12106.37, -3821.276, 33.95163),
(@CGUID+736, 3, 5983, -12090.14, -3832.115, 33.95163),
(@CGUID+736, 2, 5983, -12079.31, -3848.336, 33.95163),
(@CGUID+736, 1, 5983, -12075.5, -3867.47, 33.95163),
(@CGUID+736, 0, 5983, -12079.31, -3886.604, 33.95163),
(@CGUID+736, 18, 5983, -12079.31, -3848.336, 33.95163),
(@CGUID+736, 17, 5983, -12075.5, -3867.47, 33.95163),
(@CGUID+736, 16, 5983, -12079.31, -3886.604, 33.95163),
(@CGUID+140, 4, 78674, -11774.93, -3021.707, 24.62452),
(@CGUID+140, 1, 78674, -11785.96, -3019.523, 23.82325),
(@CGUID+140, 0, 78674, -11786.95, -3019.369, 23.82325),
(@CGUID+140, 3, 78674, -11774.93, -3021.707, 24.62452),
(@CGUID+140, 2, 78674, -11780.08, -3020.684, 24.44825),
(@CGUID+99, 5, 76556, -11861.3, -3202.258, -24.32598),
(@CGUID+99, 4, 76556, -11863.1, -3202.515, -23.68135),
(@CGUID+99, 7, 76556, -11851.33, -3200.836, -28.29107),
(@CGUID+99, 6, 76556, -11860.08, -3202.083, -24.72857),
(@CGUID+99, 1, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+99, 0, 76556, -11875.96, -3204.173, -18.38214),
(@CGUID+99, 3, 76556, -11870.05, -3203.507, -20.61034),
(@CGUID+99, 2, 76556, -11871.66, -3203.735, -19.97247),
(@CGUID+99, 8, 76556, -11851.33, -3200.836, -28.29107),
(@CGUID+267, 0, 77724, -11094.21, -2927.391, 10.71153),
(@CGUID+267, 1, 77724, -11094.18, -2928.391, 10.71153),
(@CGUID+267, 2, 77724, -11094.47, -2917.642, 10.6107),
(@CGUID+267, 3, 77724, -11094.47, -2917.642, 10.6107),
(@CGUID+149, 6, 78667, -11820.64, -3009.059, 19.42994),
(@CGUID+149, 5, 78667, -11820.64, -3009.059, 19.42994),
(@CGUID+149, 4, 78667, -11823.49, -3009.566, 18.88355),
(@CGUID+149, 3, 78667, -11825.46, -3009.922, 18.25855),
(@CGUID+149, 2, 78667, -11830.38, -3010.811, 17.63355),
(@CGUID+149, 1, 78667, -11836.29, -3011.877, 17.03226),
(@CGUID+149, 0, 78667, -11835.3, -3011.701, 17.03226),
(@CGUID+164, 0, 78667, -11722.67, -2796.696, 7.723067),
(@CGUID+164, 1, 78667, -11722.25, -2797.602, 7.723067),
(@CGUID+164, 2, 78667, -11723.52, -2801.391, 7.197398),
(@CGUID+164, 3, 78667, -11725.12, -2806.127, 6.572398),
(@CGUID+164, 4, 78667, -11727.43, -2813.03, 6.447398),
(@CGUID+164, 5, 78667, -11727.43, -2813.03, 6.447398),
(@CGUID+691, 0, 76524, -11797.58, -3830.579, 56.93703),
(@CGUID+172, 1, 78667, -11674.69, -2812.661, 5.30086),
(@CGUID+691, 1, 76524, -11797.43, -3829.59, 56.93703),
(@CGUID+172, 0, 78667, -11674.68, -2813.661, 5.30086),
(@CGUID+691, 2, 76524, -11804.03, -3803.483, 56.92304),
(@CGUID+172, 3, 78667, -11678.18, -2807.787, 6.650958),
(@CGUID+691, 3, 76524, -11804.03, -3803.483, 56.92304),
(@CGUID+172, 2, 78667, -11676.43, -2810.225, 6.025958),
(@CGUID+172, 5, 78667, -11678.43, -2807.434, 6.579424),
(@CGUID+172, 4, 78667, -11678.43, -2807.434, 6.579424),
(@CGUID+90, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+90, 5, 76556, -11891.29, -3210.586, -12.34856),
(@CGUID+90, 6, 76556, -11887.25, -3209.317, -13.74831),
(@CGUID+90, 7, 76556, -11880.66, -3207.246, -15.91098),
(@CGUID+90, 0, 76556, -11906.03, -3209.679, -11.61907),
(@CGUID+90, 1, 76556, -11905.03, -3209.613, -11.61907),
(@CGUID+90, 2, 76556, -11895.08, -3208.123, -11.5969),
(@CGUID+90, 3, 76556, -11893.78, -3207.929, -11.49541),
(@CGUID+90, 8, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+90, 9, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+753, 18, 5983, -12293.41, -3446.914, 61.72813),
(@CGUID+753, 17, 5983, -12289.6, -3427.78, 61.72813),
(@CGUID+753, 16, 5983, -12293.41, -3408.646, 61.72813),
(@CGUID+753, 3, 5983, -12304.24, -3463.135, 61.72813),
(@CGUID+753, 2, 5983, -12293.41, -3446.914, 61.72813),
(@CGUID+753, 1, 5983, -12289.6, -3427.78, 61.72813),
(@CGUID+753, 0, 5983, -12293.41, -3408.646, 61.72813),
(@CGUID+753, 7, 5983, -12374.96, -3463.135, 61.72813),
(@CGUID+753, 6, 5983, -12358.73, -3473.974, 61.72813),
(@CGUID+753, 5, 5983, -12339.6, -3477.78, 61.72813),
(@CGUID+753, 4, 5983, -12320.47, -3473.974, 61.72813),
(@CGUID+753, 11, 5983, -12374.96, -3392.425, 61.72813),
(@CGUID+753, 10, 5983, -12385.79, -3408.646, 61.72813),
(@CGUID+753, 9, 5983, -12389.6, -3427.78, 61.72813),
(@CGUID+753, 8, 5983, -12385.79, -3446.914, 61.72813),
(@CGUID+126, 9, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+753, 15, 5983, -12304.24, -3392.425, 61.72813),
(@CGUID+126, 8, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+753, 14, 5983, -12320.47, -3381.586, 61.72813),
(@CGUID+126, 7, 76556, -11880.66, -3207.246, -15.91098),
(@CGUID+753, 13, 5983, -12339.6, -3377.78, 61.72813),
(@CGUID+126, 6, 76556, -11887.25, -3209.317, -13.74831),
(@CGUID+753, 12, 5983, -12358.73, -3381.586, 61.72813),
(@CGUID+126, 5, 76556, -11891.29, -3210.586, -12.34856),
(@CGUID+126, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+126, 3, 76556, -11893.78, -3207.929, -11.49541),
(@CGUID+126, 2, 76556, -11895.08, -3208.123, -11.5969),
(@CGUID+126, 1, 76556, -11905.03, -3209.613, -11.61907),
(@CGUID+126, 0, 76556, -11906.03, -3209.679, -11.61907),
(@CGUID+812, 9, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+812, 8, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+713, 18, 5983, -11715.81, -3921.034, 105.4253),
(@CGUID+812, 1, 76556, -11906.14, -3203.108, -11.5685),
(@CGUID+713, 17, 5983, -11712, -3901.9, 105.4253),
(@CGUID+812, 0, 76556, -11907.14, -3203.174, -11.5685),
(@CGUID+713, 16, 5983, -11715.81, -3882.766, 105.4253),
(@CGUID+812, 3, 76556, -11894.45, -3202.363, -11.53603),
(@CGUID+812, 2, 76556, -11895.49, -3202.495, -11.60509),
(@CGUID+812, 5, 76556, -11887.48, -3201.484, -13.79149),
(@CGUID+812, 4, 76556, -11892.72, -3202.146, -12.16765),
(@CGUID+812, 7, 76556, -11875.71, -3200, -17.98649),
(@CGUID+812, 6, 76556, -11879.43, -3200.469, -16.53307),
(@CGUID+713, 7, 5983, -11797.36, -3937.255, 105.4253),
(@CGUID+713, 6, 5983, -11781.13, -3948.094, 105.4253),
(@CGUID+713, 5, 5983, -11762, -3951.9, 105.4253),
(@CGUID+713, 4, 5983, -11742.87, -3948.094, 105.4253),
(@CGUID+713, 3, 5983, -11726.64, -3937.255, 105.4253),
(@CGUID+713, 2, 5983, -11715.81, -3921.034, 105.4253),
(@CGUID+713, 1, 5983, -11712, -3901.9, 105.4253),
(@CGUID+713, 0, 5983, -11715.81, -3882.766, 105.4253),
(@CGUID+713, 15, 5983, -11726.64, -3866.545, 105.4253),
(@CGUID+713, 14, 5983, -11742.87, -3855.706, 105.4253),
(@CGUID+713, 13, 5983, -11762, -3851.9, 105.4253),
(@CGUID+713, 12, 5983, -11781.13, -3855.706, 105.4253),
(@CGUID+713, 11, 5983, -11797.36, -3866.545, 105.4253),
(@CGUID+713, 10, 5983, -11808.19, -3882.766, 105.4253),
(@CGUID+713, 9, 5983, -11812, -3901.9, 105.4253),
(@CGUID+713, 8, 5983, -11808.19, -3921.034, 105.4253),
(@CGUID+350, 0, 78142, -11238.98, -3332.116, 9.751066),
(@CGUID+350, 1, 78142, -11238.4, -3331.299, 9.751066),
(@CGUID+350, 2, 78142, -11233.69, -3327.976, 9.682463),
(@CGUID+350, 3, 78142, -11233.69, -3327.976, 9.682463),
(@CGUID+313, 0, 77640, -11378, -3162.008, 11.93827),
(@CGUID+313, 1, 77640, -11378.4, -3161.09, 11.93827),
(@CGUID+313, 2, 77640, -11381.08, -3153.559, 11.21122),
(@CGUID+313, 3, 77640, -11383.17, -3147.728, 11.30351),
(@CGUID+313, 4, 77640, -11383.17, -3147.728, 11.30351),
(@CGUID+793, 8, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+793, 4, 76556, -11892.59, -3203.369, -12.22058),
(@CGUID+793, 5, 76556, -11887.3, -3202.764, -13.82364),
(@CGUID+793, 6, 76556, -11880.79, -3202.019, -16.09395),
(@CGUID+793, 7, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+793, 0, 76556, -11907.18, -3205.173, -11.64383),
(@CGUID+793, 1, 76556, -11906.18, -3205.106, -11.64383),
(@CGUID+793, 2, 76556, -11895.56, -3203.731, -11.61308),
(@CGUID+793, 3, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+727, 16, 5983, -11965.91, -3733.184, 101.3853),
(@CGUID+727, 17, 5983, -11962.1, -3714.05, 101.3853),
(@CGUID+727, 18, 5983, -11965.91, -3694.916, 101.3853),
(@CGUID+700, 0, 76524, -11760.03, -3855.349, 57.37382),
(@CGUID+700, 1, 76524, -11759.06, -3855.12, 57.37382),
(@CGUID+700, 2, 76524, -11767.06, -3857.002, 57.35265),
(@CGUID+700, 3, 76524, -11767.06, -3857.002, 57.35265),
(@CGUID+727, 0, 5983, -11965.91, -3733.184, 101.3853),
(@CGUID+727, 1, 5983, -11962.1, -3714.05, 101.3853),
(@CGUID+727, 2, 5983, -11965.91, -3694.916, 101.3853),
(@CGUID+727, 3, 5983, -11976.74, -3678.695, 101.3853),
(@CGUID+727, 4, 5983, -11992.97, -3667.856, 101.3853),
(@CGUID+727, 5, 5983, -12012.1, -3664.05, 101.3853),
(@CGUID+727, 6, 5983, -12031.23, -3667.856, 101.3853),
(@CGUID+727, 7, 5983, -12047.46, -3678.695, 101.3853),
(@CGUID+727, 8, 5983, -12058.29, -3694.916, 101.3853),
(@CGUID+727, 9, 5983, -12062.1, -3714.05, 101.3853),
(@CGUID+727, 10, 5983, -12058.29, -3733.184, 101.3853),
(@CGUID+727, 11, 5983, -12047.46, -3749.405, 101.3853),
(@CGUID+727, 12, 5983, -12031.23, -3760.244, 101.3853),
(@CGUID+727, 13, 5983, -12012.1, -3764.05, 101.3853),
(@CGUID+727, 14, 5983, -11992.97, -3760.244, 101.3853),
(@CGUID+727, 15, 5983, -11976.74, -3749.405, 101.3853),
(@CGUID+231, 0, 77090, -11387.6, -2833.636, -2.286932),
(@CGUID+231, 1, 77090, -11387.17, -2832.733, -2.286932),
(@CGUID+231, 2, 77090, -11387.94, -2825.03, -2.286932),
(@CGUID+231, 3, 77090, -11387.94, -2825.03, -2.286932),
(@CGUID+138, 0, 78667, -11813.97, -3054.216, 8.068286),
(@CGUID+79, 3, 82009, -11831.95, -3200.206, -30.81585),
(@CGUID+138, 1, 78667, -11813.8, -3053.23, 8.068286),
(@CGUID+79, 2, 82009, -11831.95, -3200.206, -30.81585),
(@CGUID+138, 2, 78667, -11814.66, -3058.152, 7.318286),
(@CGUID+79, 1, 82009, -11831.52, -3200.262, -30.8285),
(@CGUID+138, 3, 78667, -11815.35, -3062.09, 6.693286),
(@CGUID+79, 0, 82009, -11830.61, -3200.687, -30.8285),
(@CGUID+138, 4, 78667, -11816.72, -3069.965, 7.392492),
(@CGUID+138, 5, 78667, -11817.58, -3074.887, 8.017492),
(@CGUID+138, 6, 78667, -11818.16, -3078.248, 8.594885),
(@CGUID+138, 7, 78667, -11818.16, -3078.248, 8.594885),
(@CGUID+138, 8, 78667, -11813.8, -3053.23, 8.167895),
(@CGUID+153, 0, 78667, -11708.14, -2950.222, 7.18),
(@CGUID+153, 1, 78667, -11707.77, -2951.153, 7.18),
(@CGUID+153, 2, 78667, -11706.29, -2959.269, 7.425117),
(@CGUID+223, 16, 5983, -11338.41, -2761.876, 53.20143),
(@CGUID+153, 3, 78667, -11706.29, -2959.269, 7.425117),
(@CGUID+223, 17, 5983, -11334.6, -2781.01, 53.20143),
(@CGUID+223, 18, 5983, -11338.41, -2800.144, 53.20143),
(@CGUID+223, 0, 5983, -11338.41, -2761.876, 53.20143),
(@CGUID+223, 1, 5983, -11334.6, -2781.01, 53.20143),
(@CGUID+223, 2, 5983, -11338.41, -2800.144, 53.20143),
(@CGUID+223, 3, 5983, -11349.24, -2816.365, 53.20143),
(@CGUID+223, 4, 5983, -11365.47, -2827.204, 53.20143),
(@CGUID+223, 5, 5983, -11384.6, -2831.01, 53.20143),
(@CGUID+223, 6, 5983, -11403.73, -2827.204, 53.20143),
(@CGUID+223, 7, 5983, -11419.96, -2816.365, 53.20143),
(@CGUID+223, 8, 5983, -11430.79, -2800.144, 53.20143),
(@CGUID+223, 9, 5983, -11434.6, -2781.01, 53.20143),
(@CGUID+223, 10, 5983, -11430.79, -2761.876, 53.20143),
(@CGUID+223, 11, 5983, -11419.96, -2745.655, 53.20143),
(@CGUID+223, 12, 5983, -11403.73, -2734.816, 53.20143),
(@CGUID+223, 13, 5983, -11384.6, -2731.01, 53.20143),
(@CGUID+223, 14, 5983, -11365.47, -2734.816, 53.20143),
(@CGUID+223, 15, 5983, -11349.24, -2745.655, 53.20143),
(@CGUID+788, 8, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+788, 9, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+788, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+788, 5, 76556, -11892.58, -3203.497, -12.22616),
(@CGUID+788, 6, 76556, -11887.22, -3203.381, -13.94922),
(@CGUID+788, 7, 76556, -11881.18, -3203.098, -15.89048),
(@CGUID+788, 0, 76556, -11906.95, -3206.747, -11.64358),
(@CGUID+788, 1, 76556, -11905.96, -3206.681, -11.64358),
(@CGUID+788, 2, 76556, -11895.37, -3205.011, -11.60832),
(@CGUID+788, 3, 76556, -11894.95, -3204.945, -11.57077),
(@CGUID+777, 18, 5983, -11927.81, -3080.816, 160.5223),
(@CGUID+777, 17, 5983, -11924, -3099.95, 160.5223),
(@CGUID+777, 16, 5983, -11927.81, -3119.084, 160.5223),
(@CGUID+777, 7, 5983, -12009.36, -3064.595, 160.5223),
(@CGUID+777, 6, 5983, -11993.13, -3053.756, 160.5223),
(@CGUID+777, 5, 5983, -11974, -3049.95, 160.5223),
(@CGUID+777, 4, 5983, -11954.87, -3053.756, 160.5223),
(@CGUID+777, 3, 5983, -11938.64, -3064.595, 160.5223),
(@CGUID+777, 2, 5983, -11927.81, -3080.816, 160.5223),
(@CGUID+777, 1, 5983, -11924, -3099.95, 160.5223),
(@CGUID+777, 0, 5983, -11927.81, -3119.084, 160.5223),
(@CGUID+777, 15, 5983, -11938.64, -3135.305, 160.5223),
(@CGUID+777, 14, 5983, -11954.87, -3146.144, 160.5223),
(@CGUID+777, 13, 5983, -11974, -3149.95, 160.5223),
(@CGUID+777, 12, 5983, -11993.13, -3146.144, 160.5223),
(@CGUID+777, 11, 5983, -12009.36, -3135.305, 160.5223),
(@CGUID+777, 10, 5983, -12020.19, -3119.084, 160.5223),
(@CGUID+777, 9, 5983, -12024, -3099.95, 160.5223),
(@CGUID+777, 8, 5983, -12020.19, -3080.816, 160.5223),
(@CGUID+100, 0, 76556, -11905.43, -3212.839, -11.52012),
(@CGUID+100, 1, 76556, -11904.43, -3212.773, -11.52012),
(@CGUID+100, 2, 76556, -11894.81, -3210.88, -11.58679),
(@CGUID+100, 3, 76556, -11894.46, -3210.811, -11.54245),
(@CGUID+100, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+100, 5, 76556, -11891.23, -3210.898, -12.31716),
(@CGUID+100, 6, 76556, -11887.06, -3210.237, -13.66055),
(@CGUID+100, 7, 76556, -11880.78, -3209.241, -15.79766),
(@CGUID+100, 8, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+100, 9, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+109, 0, 78667, -11728.8, -3121.729, -8.42988),
(@CGUID+109, 1, 78667, -11729.54, -3122.399, -8.42988),
(@CGUID+109, 2, 78667, -11731.99, -3124.135, -9.340036),
(@CGUID+109, 3, 78667, -11733.62, -3125.291, -10.11982),
(@CGUID+109, 4, 78667, -11736.07, -3127.025, -10.74482),
(@CGUID+109, 5, 78667, -11740.15, -3129.916, -11.36982),
(@CGUID+109, 6, 78667, -11743.42, -3132.229, -12.11982),
(@CGUID+109, 7, 78667, -11745.05, -3133.385, -12.64664),
(@CGUID+109, 8, 78667, -11747.09, -3134.809, -13.07291),
(@CGUID+109, 9, 78667, -11747.09, -3134.809, -13.07291),
(@CGUID+525, 5, 76085, -11300.12, -3637.28, 8.453472),
(@CGUID+525, 4, 76085, -11300.12, -3637.28, 8.453472),
(@CGUID+525, 1, 76085, -11301.24, -3649.615, 7.288188),
(@CGUID+525, 0, 76085, -11301.25, -3650.615, 7.288188),
(@CGUID+525, 3, 76085, -11300.16, -3637.662, 8.429546),
(@CGUID+525, 2, 76085, -11300.7, -3643.639, 7.929546),
(@CGUID+236, 5, 5983, -11130.2, -2783.74, 65.52914),
(@CGUID+236, 4, 5983, -11111.07, -2779.934, 65.52914),
(@CGUID+236, 7, 5983, -11165.56, -2769.095, 65.52914),
(@CGUID+236, 6, 5983, -11149.33, -2779.934, 65.52914),
(@CGUID+236, 1, 5983, -11080.2, -2733.74, 65.52914),
(@CGUID+236, 0, 5983, -11084.01, -2714.606, 65.52914),
(@CGUID+236, 3, 5983, -11094.84, -2769.095, 65.52914),
(@CGUID+236, 2, 5983, -11084.01, -2752.874, 65.52914),
(@CGUID+236, 13, 5983, -11130.2, -2683.74, 65.52914),
(@CGUID+236, 12, 5983, -11149.33, -2687.546, 65.52914),
(@CGUID+236, 15, 5983, -11094.84, -2698.385, 65.52914),
(@CGUID+236, 14, 5983, -11111.07, -2687.546, 65.52914),
(@CGUID+236, 9, 5983, -11180.2, -2733.74, 65.52914),
(@CGUID+236, 8, 5983, -11176.39, -2752.874, 65.52914),
(@CGUID+236, 11, 5983, -11165.56, -2698.385, 65.52914),
(@CGUID+236, 10, 5983, -11176.39, -2714.606, 65.52914),
(@CGUID+236, 17, 5983, -11080.2, -2733.74, 65.52914),
(@CGUID+236, 16, 5983, -11084.01, -2714.606, 65.52914),
(@CGUID+236, 18, 5983, -11084.01, -2752.874, 65.52914),
(@CGUID+776, 18, 5983, -11910.21, -2998.094, 152.3633),
(@CGUID+776, 17, 5983, -11906.4, -2978.96, 152.3633),
(@CGUID+776, 16, 5983, -11910.21, -2959.826, 152.3633),
(@CGUID+776, 7, 5983, -11991.76, -3014.315, 152.3633),
(@CGUID+776, 6, 5983, -11975.53, -3025.154, 152.3633),
(@CGUID+776, 5, 5983, -11956.4, -3028.96, 152.3633),
(@CGUID+776, 4, 5983, -11937.27, -3025.154, 152.3633),
(@CGUID+776, 3, 5983, -11921.04, -3014.315, 152.3633),
(@CGUID+776, 2, 5983, -11910.21, -2998.094, 152.3633),
(@CGUID+776, 1, 5983, -11906.4, -2978.96, 152.3633),
(@CGUID+776, 0, 5983, -11910.21, -2959.826, 152.3633),
(@CGUID+776, 15, 5983, -11921.04, -2943.605, 152.3633),
(@CGUID+776, 14, 5983, -11937.27, -2932.766, 152.3633),
(@CGUID+776, 13, 5983, -11956.4, -2928.96, 152.3633),
(@CGUID+776, 12, 5983, -11975.53, -2932.766, 152.3633),
(@CGUID+776, 11, 5983, -11991.76, -2943.605, 152.3633),
(@CGUID+776, 10, 5983, -12002.59, -2959.826, 152.3633),
(@CGUID+445, 2, 76556, -10919.6, -3417.168, 75.31927),
(@CGUID+776, 9, 5983, -12006.4, -2978.96, 152.3633),
(@CGUID+445, 3, 76556, -10919.6, -3417.168, 75.31927),
(@CGUID+776, 8, 5983, -12002.59, -2998.094, 152.3633),
(@CGUID+445, 0, 76556, -10933.18, -3413.957, 75.3193),
(@CGUID+445, 1, 76556, -10932.2, -3414.186, 75.3193),
(@CGUID+131, 8, 78667, -11769.5, -3097.088, 8.809041),
(@CGUID+131, 9, 78667, -11770.32, -3098.398, 8.929402),
(@CGUID+131, 10, 78667, -11770.32, -3098.398, 8.929402),
(@CGUID+131, 0, 78667, -11753.55, -3070.484, 12.37025),
(@CGUID+131, 1, 78667, -11753, -3069.65, 12.37025),
(@CGUID+131, 2, 78667, -11754.03, -3071.365, 11.74525),
(@CGUID+131, 3, 78667, -11755.06, -3073.08, 10.99525),
(@CGUID+131, 4, 78667, -11756.61, -3075.652, 10.12025),
(@CGUID+131, 5, 78667, -11757.64, -3077.367, 9.370247),
(@CGUID+131, 6, 78667, -11759.19, -3079.939, 8.745247),
(@CGUID+131, 7, 78667, -11761.77, -3084.227, 8.120247),
(@CGUID+702, 1, 76886, -11730.08, -3857.948, 56.2937),
(@CGUID+702, 0, 76886, -11729.78, -3858.901, 56.2937),
(@CGUID+702, 3, 76886, -11732.73, -3850.472, 56.58398),
(@CGUID+702, 2, 76886, -11732.73, -3850.472, 56.58398),
(@CGUID+821, 9, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+821, 8, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+821, 1, 76556, -11905.59, -3208.365, -11.63638),
(@CGUID+821, 0, 76556, -11906.58, -3208.431, -11.63638),
(@CGUID+821, 3, 76556, -11894.67, -3206.71, -11.55334),
(@CGUID+821, 2, 76556, -11895.2, -3206.791, -11.60179),
(@CGUID+821, 5, 76556, -11892.56, -3203.622, -12.23155),
(@CGUID+821, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+821, 7, 76556, -11881.07, -3204.004, -15.89535),
(@CGUID+821, 6, 76556, -11889.12, -3203.736, -13.35396),
(@CGUID+106, 8, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+106, 9, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+106, 2, 76556, -11894.81, -3210.88, -11.58679),
(@CGUID+106, 3, 76556, -11894.46, -3210.811, -11.54245),
(@CGUID+106, 0, 76556, -11905.43, -3212.839, -11.52012),
(@CGUID+106, 1, 76556, -11904.43, -3212.773, -11.52012),
(@CGUID+106, 6, 76556, -11887.06, -3210.237, -13.66055),
(@CGUID+106, 7, 76556, -11880.78, -3209.241, -15.79766),
(@CGUID+106, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+649, 5, 76189, -11374.41, -3506.107, 12.05331),
(@CGUID+106, 5, 76556, -11891.23, -3210.898, -12.31716),
(@CGUID+649, 4, 76189, -11372.55, -3506.846, 12.80331),
(@CGUID+649, 7, 76189, -11376.85, -3505.146, 11.41562),
(@CGUID+649, 6, 76189, -11376.85, -3505.146, 11.41562),
(@CGUID+649, 1, 76189, -11366.04, -3509.429, 14.74952),
(@CGUID+649, 0, 76189, -11365.16, -3509.896, 14.74952),
(@CGUID+649, 3, 76189, -11370.69, -3507.584, 13.42831),
(@CGUID+649, 2, 76189, -11367.9, -3508.691, 14.05331),
(@CGUID+747, 15, 5983, -12021.74, -3400.585, 127.0673),
(@CGUID+747, 14, 5983, -12037.97, -3389.746, 127.0673),
(@CGUID+747, 13, 5983, -12057.1, -3385.94, 127.0673),
(@CGUID+747, 12, 5983, -12076.23, -3389.746, 127.0673),
(@CGUID+747, 11, 5983, -12092.46, -3400.585, 127.0673),
(@CGUID+747, 10, 5983, -12103.29, -3416.806, 127.0673),
(@CGUID+747, 9, 5983, -12107.1, -3435.94, 127.0673),
(@CGUID+747, 8, 5983, -12103.29, -3455.074, 127.0673),
(@CGUID+747, 7, 5983, -12092.46, -3471.295, 127.0673),
(@CGUID+747, 6, 5983, -12076.23, -3482.134, 127.0673),
(@CGUID+747, 5, 5983, -12057.1, -3485.94, 127.0673),
(@CGUID+747, 4, 5983, -12037.97, -3482.134, 127.0673),
(@CGUID+747, 3, 5983, -12021.74, -3471.295, 127.0673),
(@CGUID+747, 2, 5983, -12010.91, -3455.074, 127.0673),
(@CGUID+747, 1, 5983, -12007.1, -3435.94, 127.0673),
(@CGUID+747, 0, 5983, -12010.91, -3416.806, 127.0673),
(@CGUID+114, 8, 76556, -11850.8, -3204.784, -28.28211),
(@CGUID+747, 18, 5983, -12010.91, -3455.074, 127.0673),
(@CGUID+747, 17, 5983, -12007.1, -3435.94, 127.0673),
(@CGUID+747, 16, 5983, -12010.91, -3416.806, 127.0673),
(@CGUID+114, 0, 76556, -11875.69, -3208.433, -18.27157),
(@CGUID+114, 1, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+114, 2, 76556, -11871.22, -3207.767, -19.89294),
(@CGUID+114, 3, 76556, -11868.52, -3207.373, -20.95753),
(@CGUID+114, 4, 76556, -11862.63, -3206.512, -23.67953),
(@CGUID+114, 5, 76556, -11859.45, -3206.047, -24.85323),
(@CGUID+114, 6, 76556, -11856.04, -3205.548, -26.17393),
(@CGUID+114, 7, 76556, -11850.8, -3204.784, -28.28211),
(@CGUID+744, 16, 5983, -12034.01, -3569.226, 145.1303),
(@CGUID+744, 17, 5983, -12030.2, -3588.36, 145.1303),
(@CGUID+744, 18, 5983, -12034.01, -3607.494, 145.1303),
(@CGUID+744, 0, 5983, -12034.01, -3569.226, 145.1303),
(@CGUID+744, 1, 5983, -12030.2, -3588.36, 145.1303),
(@CGUID+744, 2, 5983, -12034.01, -3607.494, 145.1303),
(@CGUID+744, 3, 5983, -12044.84, -3623.715, 145.1303),
(@CGUID+744, 4, 5983, -12061.07, -3634.554, 145.1303),
(@CGUID+744, 5, 5983, -12080.2, -3638.36, 145.1303),
(@CGUID+744, 6, 5983, -12099.33, -3634.554, 145.1303),
(@CGUID+744, 7, 5983, -12115.56, -3623.715, 145.1303),
(@CGUID+744, 8, 5983, -12126.39, -3607.494, 145.1303),
(@CGUID+744, 9, 5983, -12130.2, -3588.36, 145.1303),
(@CGUID+744, 10, 5983, -12126.39, -3569.226, 145.1303),
(@CGUID+744, 11, 5983, -12115.56, -3553.005, 145.1303),
(@CGUID+744, 12, 5983, -12099.33, -3542.166, 145.1303),
(@CGUID+744, 13, 5983, -12080.2, -3538.36, 145.1303),
(@CGUID+744, 14, 5983, -12061.07, -3542.166, 145.1303),
(@CGUID+744, 15, 5983, -12044.84, -3553.005, 145.1303),
(@CGUID+775, 9, 5983, -12031.2, -2919, 179.0773),
(@CGUID+775, 8, 5983, -12027.39, -2899.866, 179.0773),
(@CGUID+775, 11, 5983, -12016.56, -2954.355, 179.0773),
(@CGUID+775, 10, 5983, -12027.39, -2938.134, 179.0773),
(@CGUID+775, 13, 5983, -11981.2, -2969, 179.0773),
(@CGUID+775, 12, 5983, -12000.33, -2965.194, 179.0773),
(@CGUID+775, 15, 5983, -11945.84, -2954.355, 179.0773),
(@CGUID+775, 14, 5983, -11962.07, -2965.194, 179.0773),
(@CGUID+775, 1, 5983, -11931.2, -2919, 179.0773),
(@CGUID+775, 0, 5983, -11935.01, -2938.134, 179.0773),
(@CGUID+775, 3, 5983, -11945.84, -2883.645, 179.0773),
(@CGUID+775, 2, 5983, -11935.01, -2899.866, 179.0773),
(@CGUID+775, 5, 5983, -11981.2, -2869, 179.0773),
(@CGUID+775, 4, 5983, -11962.07, -2872.806, 179.0773),
(@CGUID+775, 7, 5983, -12016.56, -2883.645, 179.0773),
(@CGUID+775, 6, 5983, -12000.33, -2872.806, 179.0773),
(@CGUID+775, 17, 5983, -11931.2, -2919, 179.0773),
(@CGUID+775, 16, 5983, -11935.01, -2938.134, 179.0773),
(@CGUID+775, 18, 5983, -11935.01, -2899.866, 179.0773),
(@CGUID+367, 0, 77640, -11175.17, -3306.453, 5.44502),
(@CGUID+367, 1, 77640, -11176.11, -3306.804, 5.44502),
(@CGUID+367, 2, 77640, -11180.04, -3306.094, 6.074171),
(@CGUID+367, 3, 77640, -11182.01, -3305.738, 7.074171),
(@CGUID+104, 6, 76556, -11880.79, -3202.019, -16.09395),
(@CGUID+367, 4, 77640, -11183, -3305.561, 7.699171),
(@CGUID+104, 7, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+367, 5, 77640, -11183.98, -3305.383, 8.199171),
(@CGUID+104, 4, 76556, -11892.59, -3203.369, -12.22058),
(@CGUID+367, 6, 77640, -11185.62, -3305.087, 8.941114),
(@CGUID+104, 5, 76556, -11887.3, -3202.764, -13.82364),
(@CGUID+367, 7, 77640, -11185.62, -3305.087, 8.941114),
(@CGUID+104, 2, 76556, -11895.56, -3203.731, -11.61308),
(@CGUID+104, 3, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+104, 0, 76556, -11907.18, -3205.173, -11.64383),
(@CGUID+104, 1, 76556, -11906.18, -3205.106, -11.64383),
(@CGUID+104, 8, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+539, 3, 76098, -11311.11, -3733.913, 2.493001),
(@CGUID+539, 2, 76098, -11306.67, -3719.867, 2.220152),
(@CGUID+539, 1, 76098, -11300.36, -3699.851, 2.765698),
(@CGUID+539, 0, 76098, -11300.03, -3698.907, 2.765698),
(@CGUID+539, 4, 76098, -11311.11, -3733.913, 2.493001),
(@CGUID+112, 4, 76556, -11892.72, -3202.146, -12.16765),
(@CGUID+112, 5, 76556, -11887.48, -3201.484, -13.79149),
(@CGUID+112, 6, 76556, -11879.43, -3200.469, -16.53307),
(@CGUID+112, 7, 76556, -11875.71, -3200, -17.98649),
(@CGUID+112, 0, 76556, -11907.14, -3203.174, -11.5685),
(@CGUID+112, 1, 76556, -11906.14, -3203.108, -11.5685),
(@CGUID+112, 2, 76556, -11895.49, -3202.495, -11.60509),
(@CGUID+112, 3, 76556, -11894.45, -3202.363, -11.53603),
(@CGUID+112, 8, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+112, 9, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+804, 7, 76556, -11880.78, -3209.241, -15.79766),
(@CGUID+804, 6, 76556, -11887.06, -3210.237, -13.66055),
(@CGUID+804, 5, 76556, -11891.23, -3210.898, -12.31716),
(@CGUID+804, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+804, 3, 76556, -11894.46, -3210.811, -11.54245),
(@CGUID+804, 2, 76556, -11894.81, -3210.88, -11.58679),
(@CGUID+804, 1, 76556, -11904.43, -3212.773, -11.52012),
(@CGUID+804, 0, 76556, -11905.43, -3212.839, -11.52012),
(@CGUID+804, 9, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+804, 8, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+251, 15, 5983, -11011.74, -2901.005, 53.55413),
(@CGUID+251, 14, 5983, -11027.97, -2911.844, 53.55413),
(@CGUID+251, 13, 5983, -11047.1, -2915.65, 53.55413),
(@CGUID+251, 12, 5983, -11066.23, -2911.844, 53.55413),
(@CGUID+251, 11, 5983, -11082.46, -2901.005, 53.55413),
(@CGUID+251, 10, 5983, -11093.29, -2884.784, 53.55413),
(@CGUID+251, 9, 5983, -11097.1, -2865.65, 53.55413),
(@CGUID+251, 8, 5983, -11093.29, -2846.516, 53.55413),
(@CGUID+251, 7, 5983, -11082.46, -2830.295, 53.55413),
(@CGUID+251, 6, 5983, -11066.23, -2819.456, 53.55413),
(@CGUID+251, 5, 5983, -11047.1, -2815.65, 53.55413),
(@CGUID+251, 4, 5983, -11027.97, -2819.456, 53.55413),
(@CGUID+251, 3, 5983, -11011.74, -2830.295, 53.55413),
(@CGUID+251, 2, 5983, -11000.91, -2846.516, 53.55413),
(@CGUID+251, 1, 5983, -10997.1, -2865.65, 53.55413),
(@CGUID+251, 0, 5983, -11000.91, -2884.784, 53.55413),
(@CGUID+251, 18, 5983, -11000.91, -2846.516, 53.55413),
(@CGUID+251, 17, 5983, -10997.1, -2865.65, 53.55413),
(@CGUID+251, 16, 5983, -11000.91, -2884.784, 53.55413),
(@CGUID+93, 4, 76556, -11892.59, -3203.369, -12.22058),
(@CGUID+93, 5, 76556, -11887.3, -3202.764, -13.82364),
(@CGUID+93, 6, 76556, -11880.79, -3202.019, -16.09395),
(@CGUID+93, 7, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+93, 0, 76556, -11907.18, -3205.173, -11.64383),
(@CGUID+93, 1, 76556, -11906.18, -3205.106, -11.64383),
(@CGUID+93, 2, 76556, -11895.56, -3203.731, -11.61308),
(@CGUID+93, 3, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+811, 8, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+93, 8, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+811, 4, 76556, -11892.59, -3203.369, -12.22058),
(@CGUID+811, 5, 76556, -11887.3, -3202.764, -13.82364),
(@CGUID+811, 6, 76556, -11880.79, -3202.019, -16.09395),
(@CGUID+811, 7, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+811, 0, 76556, -11907.18, -3205.173, -11.64383),
(@CGUID+811, 1, 76556, -11906.18, -3205.106, -11.64383),
(@CGUID+811, 2, 76556, -11895.56, -3203.731, -11.61308),
(@CGUID+811, 3, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+91, 9, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+91, 8, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+91, 5, 76556, -11892.58, -3203.497, -12.22616),
(@CGUID+91, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+91, 7, 76556, -11881.18, -3203.098, -15.89048),
(@CGUID+91, 6, 76556, -11887.22, -3203.381, -13.94922),
(@CGUID+91, 1, 76556, -11905.96, -3206.681, -11.64358),
(@CGUID+91, 0, 76556, -11906.95, -3206.747, -11.64358),
(@CGUID+91, 3, 76556, -11894.95, -3204.945, -11.57077),
(@CGUID+91, 2, 76556, -11895.37, -3205.011, -11.60832),
(@CGUID+472, 0, 77643, -11212.57, -3515.544, 7.337011),
(@CGUID+472, 1, 77643, -11211.92, -3516.307, 7.337011),
(@CGUID+472, 2, 77643, -11215.04, -3512.618, 7.944282),
(@CGUID+472, 3, 77643, -11216.31, -3511.11, 8.030255),
(@CGUID+472, 4, 77643, -11216.73, -3510.609, 8.04099),
(@CGUID+472, 5, 77643, -11216.73, -3510.609, 8.04099),
(@CGUID+632, 5, 5983, -11685.11, -3454.542, 138.5913),
(@CGUID+632, 4, 5983, -11666.29, -3459.673, 138.5913),
(@CGUID+632, 7, 5983, -11721.4, -3466.685, 138.5913),
(@CGUID+632, 6, 5983, -11704.46, -3457.004, 138.5913),
(@CGUID+632, 1, 5983, -11638.72, -3507.908, 138.5913),
(@CGUID+632, 0, 5983, -11643.85, -3526.73, 138.5913),
(@CGUID+632, 3, 5983, -11650.86, -3471.617, 138.5913),
(@CGUID+632, 2, 5983, -11641.18, -3488.555, 138.5913),
(@CGUID+632, 13, 5983, -11692.09, -3554.298, 138.5913),
(@CGUID+632, 12, 5983, -11710.91, -3549.167, 138.5913),
(@CGUID+632, 15, 5983, -11655.8, -3542.155, 138.5913),
(@CGUID+632, 14, 5983, -11672.73, -3551.836, 138.5913),
(@CGUID+632, 9, 5983, -11738.48, -3500.932, 138.5913),
(@CGUID+632, 8, 5983, -11733.35, -3482.11, 138.5913),
(@CGUID+632, 11, 5983, -11726.33, -3537.223, 138.5913),
(@CGUID+632, 10, 5983, -11736.02, -3520.285, 138.5913),
(@CGUID+632, 17, 5983, -11638.72, -3507.908, 138.5913),
(@CGUID+632, 16, 5983, -11643.85, -3526.73, 138.5913),
(@CGUID+632, 18, 5983, -11641.18, -3488.555, 138.5913),
(@CGUID+409, 0, 76556, -10927.96, -3419.313, 66.49751),
(@CGUID+409, 1, 76556, -10926.99, -3419.533, 66.49751),
(@CGUID+409, 2, 76556, -10942.01, -3416.144, 66.49753),
(@CGUID+409, 3, 76556, -10942.01, -3416.144, 66.49753),
(@CGUID+67, 3, 82025, -11833.09, -3188.961, -30.30369),
(@CGUID+67, 2, 82025, -11833.09, -3188.961, -30.30369),
(@CGUID+67, 1, 82025, -11831.26, -3189.051, -30.49418),
(@CGUID+67, 0, 82025, -11830.36, -3188.611, -30.49418),
(@CGUID+808, 8, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+808, 9, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+808, 0, 76556, -11906.95, -3206.747, -11.64358),
(@CGUID+808, 1, 76556, -11905.96, -3206.681, -11.64358),
(@CGUID+808, 2, 76556, -11895.37, -3205.011, -11.60832),
(@CGUID+808, 3, 76556, -11894.95, -3204.945, -11.57077),
(@CGUID+808, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+808, 5, 76556, -11892.58, -3203.497, -12.22616),
(@CGUID+808, 6, 76556, -11887.22, -3203.381, -13.94922),
(@CGUID+808, 7, 76556, -11881.18, -3203.098, -15.89048),
(@CGUID+675, 9, 5983, -11496.6, -3335.851, 64.97147),
(@CGUID+675, 8, 5983, -11492.79, -3316.717, 64.97147),
(@CGUID+675, 11, 5983, -11481.96, -3371.206, 64.97147),
(@CGUID+675, 10, 5983, -11492.79, -3354.985, 64.97147),
(@CGUID+675, 13, 5983, -11446.6, -3385.851, 64.97147),
(@CGUID+675, 12, 5983, -11465.73, -3382.045, 64.97147),
(@CGUID+675, 15, 5983, -11411.25, -3371.206, 64.97147),
(@CGUID+675, 14, 5983, -11427.47, -3382.045, 64.97147),
(@CGUID+675, 1, 5983, -11396.6, -3335.851, 64.97147),
(@CGUID+675, 0, 5983, -11400.41, -3354.985, 64.97147),
(@CGUID+675, 3, 5983, -11411.25, -3300.496, 64.97147),
(@CGUID+675, 2, 5983, -11400.41, -3316.717, 64.97147),
(@CGUID+675, 5, 5983, -11446.6, -3285.851, 64.97147),
(@CGUID+675, 4, 5983, -11427.47, -3289.657, 64.97147),
(@CGUID+675, 7, 5983, -11481.96, -3300.496, 64.97147),
(@CGUID+675, 6, 5983, -11465.73, -3289.657, 64.97147),
(@CGUID+675, 17, 5983, -11396.6, -3335.851, 64.97147),
(@CGUID+675, 16, 5983, -11400.41, -3354.985, 64.97147),
(@CGUID+675, 18, 5983, -11400.41, -3316.717, 64.97147),
(@CGUID+717, 6, 76886, -11814.41, -3896.026, 62.25269),
(@CGUID+816, 6, 76556, -11889.12, -3203.736, -13.35396),
(@CGUID+717, 4, 76886, -11812.83, -3897.016, 63.0481),
(@CGUID+816, 7, 76556, -11881.07, -3204.004, -15.89535),
(@CGUID+717, 5, 76886, -11814.41, -3896.026, 62.25269),
(@CGUID+816, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+717, 2, 76886, -11809.45, -3899.148, 65.4231),
(@CGUID+816, 5, 76556, -11892.56, -3203.622, -12.23155),
(@CGUID+717, 3, 76886, -11811.14, -3898.082, 64.6731),
(@CGUID+816, 2, 76556, -11895.2, -3206.791, -11.60179),
(@CGUID+717, 0, 76886, -11807.69, -3900.086, 66.46899),
(@CGUID+816, 3, 76556, -11894.67, -3206.71, -11.55334),
(@CGUID+717, 1, 76886, -11808.6, -3899.681, 66.46899),
(@CGUID+816, 0, 76556, -11906.58, -3208.431, -11.63638),
(@CGUID+816, 1, 76556, -11905.59, -3208.365, -11.63638),
(@CGUID+816, 8, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+816, 9, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+39, 0, 78674, -11713.77, -3211, 2.308977),
(@CGUID+39, 1, 78674, -11714.72, -3210.697, 2.308977),
(@CGUID+39, 2, 78674, -11715.68, -3210.395, 1.558977),
(@CGUID+39, 3, 78674, -11718.54, -3209.486, 0.5589771),
(@CGUID+39, 4, 78674, -11719.49, -3209.184, -0.06602287),
(@CGUID+814, 11, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+39, 5, 78674, -11721.4, -3208.578, -1.191023),
(@CGUID+814, 10, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+39, 6, 78674, -11722.35, -3208.275, -2.066023),
(@CGUID+814, 9, 76556, -11880.54, -3208.124, -15.91559),
(@CGUID+39, 7, 78674, -11723.3, -3207.973, -2.691023),
(@CGUID+814, 8, 76556, -11883.76, -3208.915, -14.9311),
(@CGUID+39, 8, 78674, -11724.26, -3207.67, -3.316023),
(@CGUID+814, 7, 76556, -11886.64, -3209.621, -13.88664),
(@CGUID+39, 9, 78674, -11725.21, -3207.367, -4.066023),
(@CGUID+814, 6, 76556, -11891.26, -3210.755, -12.33153),
(@CGUID+39, 10, 78674, -11726.16, -3207.064, -4.691023),
(@CGUID+814, 5, 76556, -11892.31, -3211.014, -12.19763),
(@CGUID+39, 11, 78674, -11726.83, -3206.852, -4.84239),
(@CGUID+814, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+39, 12, 78674, -11726.83, -3206.852, -4.84239),
(@CGUID+814, 3, 76556, -11893.73, -3209.299, -11.49278),
(@CGUID+814, 2, 76556, -11894.94, -3209.512, -11.59181),
(@CGUID+814, 1, 76556, -11904.82, -3211.245, -11.56851),
(@CGUID+814, 0, 76556, -11905.82, -3211.312, -11.56851),
(@CGUID+183, 1, 78489, -11577.74, -2756.613, 6.041425),
(@CGUID+183, 0, 78489, -11578.63, -2757.064, 6.041425),
(@CGUID+183, 3, 78489, -11583.17, -2759.353, 5.981367),
(@CGUID+183, 2, 78489, -11583.17, -2759.353, 5.981367),
(@CGUID+108, 8, 76556, -11849.48, -3202.604, -28.87239),
(@CGUID+108, 4, 76556, -11859.86, -3203.763, -24.73165),
(@CGUID+108, 5, 76556, -11851.21, -3201.725, -28.28949),
(@CGUID+108, 6, 76556, -11850.52, -3201.563, -28.62239),
(@CGUID+108, 7, 76556, -11849.48, -3202.604, -28.87239),
(@CGUID+108, 0, 76556, -11875.71, -3207.411, -18.3378),
(@CGUID+108, 1, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+108, 2, 76556, -11871.39, -3206.481, -19.95624),
(@CGUID+108, 3, 76556, -11862.87, -3204.472, -23.70538),
(@CGUID+111, 5, 76556, -11861.83, -3203.876, -24.08862),
(@CGUID+111, 4, 76556, -11862.92, -3204.016, -23.69897),
(@CGUID+111, 7, 76556, -11851.11, -3202.497, -28.28738),
(@CGUID+111, 6, 76556, -11859.88, -3203.624, -24.73113),
(@CGUID+111, 1, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+111, 0, 76556, -11876.49, -3205.937, -18.08854),
(@CGUID+111, 3, 76556, -11870.53, -3204.994, -20.35902),
(@CGUID+111, 2, 76556, -11871.52, -3205.121, -19.9649),
(@CGUID+111, 8, 76556, -11851.11, -3202.497, -28.28738),
(@CGUID+430, 4, 77790, -11013.23, -3452.432, 64.81697),
(@CGUID+430, 1, 77790, -11024.57, -3450.56, 64.84375),
(@CGUID+430, 0, 77790, -11025.56, -3450.397, 64.84375),
(@CGUID+430, 3, 77790, -11013.23, -3452.432, 64.81697),
(@CGUID+430, 2, 77790, -11023.08, -3450.807, 64.80745),
(@CGUID+115, 3, 76556, -11893.78, -3207.929, -11.49541),
(@CGUID+115, 2, 76556, -11895.08, -3208.123, -11.5969),
(@CGUID+115, 1, 76556, -11905.03, -3209.613, -11.61907),
(@CGUID+115, 0, 76556, -11906.03, -3209.679, -11.61907),
(@CGUID+115, 7, 76556, -11880.66, -3207.246, -15.91098),
(@CGUID+115, 6, 76556, -11887.25, -3209.317, -13.74831),
(@CGUID+115, 5, 76556, -11891.29, -3210.586, -12.34856),
(@CGUID+115, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+115, 9, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+115, 8, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+312, 5, 5983, -11429.7, -3236.38, 86.81583),
(@CGUID+312, 4, 5983, -11410.57, -3232.574, 86.81583),
(@CGUID+312, 7, 5983, -11465.06, -3221.735, 86.81583),
(@CGUID+312, 6, 5983, -11448.83, -3232.574, 86.81583),
(@CGUID+312, 1, 5983, -11379.7, -3186.38, 86.81583),
(@CGUID+312, 0, 5983, -11383.51, -3167.246, 86.81583),
(@CGUID+312, 3, 5983, -11394.34, -3221.735, 86.81583),
(@CGUID+312, 2, 5983, -11383.51, -3205.514, 86.81583),
(@CGUID+312, 13, 5983, -11429.7, -3136.38, 86.81583),
(@CGUID+312, 12, 5983, -11448.83, -3140.186, 86.81583),
(@CGUID+312, 15, 5983, -11394.34, -3151.025, 86.81583),
(@CGUID+312, 14, 5983, -11410.57, -3140.186, 86.81583),
(@CGUID+312, 9, 5983, -11479.7, -3186.38, 86.81583),
(@CGUID+312, 8, 5983, -11475.89, -3205.514, 86.81583),
(@CGUID+312, 11, 5983, -11465.06, -3151.025, 86.81583),
(@CGUID+312, 10, 5983, -11475.89, -3167.246, 86.81583),
(@CGUID+312, 17, 5983, -11379.7, -3186.38, 86.81583),
(@CGUID+312, 16, 5983, -11383.51, -3167.246, 86.81583),
(@CGUID+312, 18, 5983, -11383.51, -3205.514, 86.81583),
(@CGUID+326, 15, 5983, -11252.44, -3293.215, 96.0542),
(@CGUID+326, 14, 5983, -11268.67, -3304.053, 96.0542),
(@CGUID+326, 13, 5983, -11287.8, -3307.859, 96.0542),
(@CGUID+326, 12, 5983, -11306.93, -3304.053, 96.0542),
(@CGUID+326, 11, 5983, -11323.15, -3293.215, 96.0542),
(@CGUID+326, 10, 5983, -11333.99, -3276.994, 96.0542),
(@CGUID+326, 9, 5983, -11337.8, -3257.859, 96.0542),
(@CGUID+326, 8, 5983, -11333.99, -3238.725, 96.0542),
(@CGUID+326, 7, 5983, -11323.15, -3222.504, 96.0542),
(@CGUID+326, 6, 5983, -11306.93, -3211.665, 96.0542),
(@CGUID+127, 3, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+326, 5, 5983, -11287.8, -3207.859, 96.0542),
(@CGUID+127, 2, 76556, -11895.56, -3203.731, -11.61308),
(@CGUID+326, 4, 5983, -11268.67, -3211.665, 96.0542),
(@CGUID+127, 1, 76556, -11906.18, -3205.106, -11.64383),
(@CGUID+326, 3, 5983, -11252.44, -3222.504, 96.0542),
(@CGUID+127, 0, 76556, -11907.18, -3205.173, -11.64383),
(@CGUID+326, 2, 5983, -11241.6, -3238.725, 96.0542),
(@CGUID+127, 7, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+326, 1, 5983, -11237.8, -3257.859, 96.0542),
(@CGUID+127, 6, 76556, -11880.79, -3202.019, -16.09395),
(@CGUID+326, 0, 5983, -11241.6, -3276.993, 96.0542),
(@CGUID+127, 5, 76556, -11887.3, -3202.764, -13.82364),
(@CGUID+127, 4, 76556, -11892.59, -3203.369, -12.22058),
(@CGUID+127, 8, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+772, 0, 5983, -12347.31, -2926.124, 173.2413),
(@CGUID+772, 1, 5983, -12343.5, -2906.99, 173.2413),
(@CGUID+772, 2, 5983, -12347.31, -2887.856, 173.2413),
(@CGUID+772, 3, 5983, -12358.14, -2871.635, 173.2413),
(@CGUID+326, 18, 5983, -11241.6, -3238.725, 96.0542),
(@CGUID+772, 4, 5983, -12374.37, -2860.796, 173.2413),
(@CGUID+326, 17, 5983, -11237.8, -3257.859, 96.0542),
(@CGUID+772, 5, 5983, -12393.5, -2856.99, 173.2413),
(@CGUID+326, 16, 5983, -11241.6, -3276.993, 96.0542),
(@CGUID+772, 6, 5983, -12412.63, -2860.796, 173.2413),
(@CGUID+772, 7, 5983, -12428.86, -2871.635, 173.2413),
(@CGUID+772, 8, 5983, -12439.69, -2887.856, 173.2413),
(@CGUID+772, 9, 5983, -12443.5, -2906.99, 173.2413),
(@CGUID+772, 10, 5983, -12439.69, -2926.124, 173.2413),
(@CGUID+772, 11, 5983, -12428.86, -2942.345, 173.2413),
(@CGUID+772, 12, 5983, -12412.63, -2953.184, 173.2413),
(@CGUID+772, 13, 5983, -12393.5, -2956.99, 173.2413),
(@CGUID+772, 14, 5983, -12374.37, -2953.184, 173.2413),
(@CGUID+772, 15, 5983, -12358.14, -2942.345, 173.2413),
(@CGUID+772, 16, 5983, -12347.31, -2926.124, 173.2413),
(@CGUID+772, 17, 5983, -12343.5, -2906.99, 173.2413),
(@CGUID+772, 18, 5983, -12347.31, -2887.856, 173.2413),
(@CGUID+721, 9, 5983, -11991.3, -3801.57, 119.2453),
(@CGUID+721, 8, 5983, -11987.49, -3820.704, 119.2453),
(@CGUID+721, 11, 5983, -11976.66, -3766.215, 119.2453),
(@CGUID+721, 10, 5983, -11987.49, -3782.436, 119.2453),
(@CGUID+721, 13, 5983, -11941.3, -3751.57, 119.2453),
(@CGUID+721, 12, 5983, -11960.43, -3755.376, 119.2453),
(@CGUID+721, 15, 5983, -11905.94, -3766.215, 119.2453),
(@CGUID+721, 14, 5983, -11922.17, -3755.376, 119.2453),
(@CGUID+721, 1, 5983, -11891.3, -3801.57, 119.2453),
(@CGUID+721, 0, 5983, -11895.11, -3782.436, 119.2453),
(@CGUID+721, 3, 5983, -11905.94, -3836.925, 119.2453),
(@CGUID+721, 2, 5983, -11895.11, -3820.704, 119.2453),
(@CGUID+721, 5, 5983, -11941.3, -3851.57, 119.2453),
(@CGUID+721, 4, 5983, -11922.17, -3847.764, 119.2453),
(@CGUID+721, 7, 5983, -11976.66, -3836.925, 119.2453),
(@CGUID+721, 6, 5983, -11960.43, -3847.764, 119.2453),
(@CGUID+721, 17, 5983, -11891.3, -3801.57, 119.2453),
(@CGUID+721, 16, 5983, -11895.11, -3782.436, 119.2453),
(@CGUID+721, 18, 5983, -11895.11, -3820.704, 119.2453),
(@CGUID+50, 0, 78556, -11833.22, -3211.141, -30.49739),
(@CGUID+50, 1, 78556, -11834.17, -3210.813, -30.49739),
(@CGUID+50, 2, 78556, -11833.92, -3209.775, -30.48934),
(@CGUID+50, 3, 78556, -11833.92, -3209.775, -30.48934),
(@CGUID+652, 1, 76189, -11401.44, -3484.792, 8.095489),
(@CGUID+652, 0, 76189, -11401.38, -3485.79, 8.095489),
(@CGUID+652, 3, 76189, -11408.33, -3486.179, 8.35794),
(@CGUID+652, 2, 76189, -11408.33, -3486.179, 8.35794),
(@CGUID+652, 4, 76189, -11422.77, -3490.752, 8.015655),
(@CGUID+822, 9, 76556, -11880.54, -3208.124, -15.91559),
(@CGUID+822, 8, 76556, -11883.76, -3208.915, -14.9311),
(@CGUID+822, 11, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+822, 10, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+822, 1, 76556, -11904.82, -3211.245, -11.56851),
(@CGUID+822, 0, 76556, -11905.82, -3211.312, -11.56851),
(@CGUID+822, 3, 76556, -11893.73, -3209.299, -11.49278),
(@CGUID+822, 2, 76556, -11894.94, -3209.512, -11.59181),
(@CGUID+822, 5, 76556, -11892.31, -3211.014, -12.19763),
(@CGUID+822, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+822, 7, 76556, -11886.64, -3209.621, -13.88664),
(@CGUID+822, 6, 76556, -11891.26, -3210.755, -12.33153),
(@CGUID+150, 3, 78670, -11870.31, -3040.37, 18.18499),
(@CGUID+150, 2, 78670, -11870.31, -3040.37, 18.18499),
(@CGUID+212, 4, 78348, -11392.88, -2670.354, 3.623497),
(@CGUID+150, 1, 78670, -11869.71, -3024.347, 18.35573),
(@CGUID+150, 0, 78670, -11868.96, -3023.683, 18.35573),
(@CGUID+212, 0, 78348, -11388.97, -2674.609, 4.22739),
(@CGUID+212, 1, 78348, -11388.3, -2675.345, 4.22739),
(@CGUID+212, 2, 78348, -11390.29, -2673.172, 4.099231),
(@CGUID+212, 3, 78348, -11392.88, -2670.354, 3.623497),
(@CGUID+43, 0, 78667, -11764.93, -3282.745, -15.02141),
(@CGUID+43, 1, 78667, -11765.65, -3282.049, -15.02141),
(@CGUID+43, 2, 78667, -11768.38, -3279.119, -15.73681),
(@CGUID+43, 3, 78667, -11770.42, -3276.922, -16.61181),
(@CGUID+43, 4, 78667, -11772.47, -3274.725, -17.36181),
(@CGUID+43, 5, 78667, -11775.19, -3271.795, -17.98681),
(@CGUID+43, 6, 78667, -11775.19, -3271.795, -17.98681),
(@CGUID+141, 3, 78667, -11728.81, -3032.182, 21.99413),
(@CGUID+141, 2, 78667, -11725.46, -3033.32, 21.50291),
(@CGUID+141, 1, 78667, -11718.83, -3035.576, 20.81557),
(@CGUID+141, 0, 78667, -11717.93, -3035.993, 20.81557),
(@CGUID+141, 4, 78667, -11728.81, -3032.182, 21.99413),
(@CGUID+116, 8, 76556, -11851.53, -3199.264, -28.401),
(@CGUID+116, 7, 76556, -11851.53, -3199.264, -28.401),
(@CGUID+116, 6, 76556, -11856.41, -3200, -26.30383),
(@CGUID+116, 5, 76556, -11860.78, -3200.658, -24.56051),
(@CGUID+116, 4, 76556, -11863.28, -3201.036, -23.66373),
(@CGUID+116, 3, 76556, -11869.57, -3201.983, -20.86848),
(@CGUID+116, 2, 76556, -11871.8, -3202.319, -19.98167),
(@CGUID+116, 1, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+116, 0, 76556, -11876.07, -3202.859, -18.40056),
(@CGUID+163, 0, 78667, -11718.47, -2797.951, 7.223067),
(@CGUID+163, 1, 78667, -11718.22, -2798.918, 7.223067),
(@CGUID+163, 2, 78667, -11720.76, -2806.496, 6.697398),
(@CGUID+163, 3, 78667, -11723.5, -2814.63, 6.204722),
(@CGUID+163, 4, 78667, -11723.5, -2814.63, 6.204722),
(@CGUID+102, 1, 78667, -11725.35, -3253.878, 5.790121),
(@CGUID+102, 0, 78667, -11724.38, -3254.1, 5.790121),
(@CGUID+102, 3, 78667, -11736.51, -3249.473, 5.903252),
(@CGUID+102, 2, 78667, -11730.93, -3251.676, 5.297689),
(@CGUID+102, 5, 78667, -11741.17, -3247.648, 7.069267),
(@CGUID+102, 4, 78667, -11740.22, -3248.004, 6.528252),
(@CGUID+102, 6, 78667, -11741.17, -3247.648, 7.069267),
(@CGUID+95, 4, 76556, -11892.72, -3202.146, -12.16765),
(@CGUID+95, 5, 76556, -11887.48, -3201.484, -13.79149),
(@CGUID+95, 6, 76556, -11879.43, -3200.469, -16.53307),
(@CGUID+95, 7, 76556, -11875.71, -3200, -17.98649),
(@CGUID+95, 0, 76556, -11907.14, -3203.174, -11.5685),
(@CGUID+95, 1, 76556, -11906.14, -3203.108, -11.5685),
(@CGUID+95, 2, 76556, -11895.49, -3202.495, -11.60509),
(@CGUID+95, 3, 76556, -11894.45, -3202.363, -11.53603),
(@CGUID+95, 8, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+95, 9, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+124, 6, 76556, -11887.06, -3210.237, -13.66055),
(@CGUID+773, 18, 5983, -11997.81, -2885.654, 170.2153),
(@CGUID+124, 7, 76556, -11880.78, -3209.241, -15.79766),
(@CGUID+773, 17, 5983, -11994, -2866.52, 170.2153),
(@CGUID+124, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+773, 16, 5983, -11997.81, -2847.386, 170.2153),
(@CGUID+124, 5, 76556, -11891.23, -3210.898, -12.31716),
(@CGUID+124, 2, 76556, -11894.81, -3210.88, -11.58679),
(@CGUID+124, 3, 76556, -11894.46, -3210.811, -11.54245),
(@CGUID+124, 0, 76556, -11905.43, -3212.839, -11.52012),
(@CGUID+124, 1, 76556, -11904.43, -3212.773, -11.52012),
(@CGUID+124, 8, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+124, 9, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+773, 3, 5983, -12008.64, -2901.875, 170.2153),
(@CGUID+773, 2, 5983, -11997.81, -2885.654, 170.2153),
(@CGUID+773, 1, 5983, -11994, -2866.52, 170.2153),
(@CGUID+773, 0, 5983, -11997.81, -2847.386, 170.2153),
(@CGUID+773, 7, 5983, -12079.36, -2901.875, 170.2153),
(@CGUID+773, 6, 5983, -12063.13, -2912.714, 170.2153),
(@CGUID+773, 5, 5983, -12044, -2916.52, 170.2153),
(@CGUID+773, 4, 5983, -12024.87, -2912.714, 170.2153),
(@CGUID+773, 11, 5983, -12079.36, -2831.165, 170.2153),
(@CGUID+773, 10, 5983, -12090.19, -2847.386, 170.2153),
(@CGUID+125, 11, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+773, 9, 5983, -12094, -2866.52, 170.2153),
(@CGUID+125, 10, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+773, 8, 5983, -12090.19, -2885.654, 170.2153),
(@CGUID+125, 9, 76556, -11880.54, -3208.124, -15.91559),
(@CGUID+773, 15, 5983, -12008.64, -2831.165, 170.2153),
(@CGUID+125, 8, 76556, -11883.76, -3208.915, -14.9311),
(@CGUID+773, 14, 5983, -12024.87, -2820.326, 170.2153),
(@CGUID+125, 7, 76556, -11886.64, -3209.621, -13.88664),
(@CGUID+773, 13, 5983, -12044, -2816.52, 170.2153),
(@CGUID+125, 6, 76556, -11891.26, -3210.755, -12.33153),
(@CGUID+773, 12, 5983, -12063.13, -2820.326, 170.2153),
(@CGUID+125, 5, 76556, -11892.31, -3211.014, -12.19763),
(@CGUID+125, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+125, 3, 76556, -11893.73, -3209.299, -11.49278),
(@CGUID+125, 2, 76556, -11894.94, -3209.512, -11.59181),
(@CGUID+125, 1, 76556, -11904.82, -3211.245, -11.56851),
(@CGUID+125, 0, 76556, -11905.82, -3211.312, -11.56851),
(@CGUID+186, 9, 5983, -11592.1, -2769.5, 59.54743),
(@CGUID+186, 8, 5983, -11588.29, -2788.634, 59.54743),
(@CGUID+186, 11, 5983, -11577.46, -2734.145, 59.54743),
(@CGUID+186, 10, 5983, -11588.29, -2750.366, 59.54743),
(@CGUID+186, 13, 5983, -11542.1, -2719.5, 59.54743),
(@CGUID+186, 12, 5983, -11561.23, -2723.306, 59.54743),
(@CGUID+186, 15, 5983, -11506.74, -2734.145, 59.54743),
(@CGUID+186, 14, 5983, -11522.97, -2723.306, 59.54743),
(@CGUID+186, 1, 5983, -11492.1, -2769.5, 59.54743),
(@CGUID+186, 0, 5983, -11495.91, -2750.366, 59.54743),
(@CGUID+186, 3, 5983, -11506.74, -2804.855, 59.54743),
(@CGUID+186, 2, 5983, -11495.91, -2788.634, 59.54743),
(@CGUID+186, 5, 5983, -11542.1, -2819.5, 59.54743),
(@CGUID+186, 4, 5983, -11522.97, -2815.694, 59.54743),
(@CGUID+186, 7, 5983, -11577.46, -2804.855, 59.54743),
(@CGUID+186, 6, 5983, -11561.23, -2815.694, 59.54743),
(@CGUID+186, 17, 5983, -11492.1, -2769.5, 59.54743),
(@CGUID+186, 16, 5983, -11495.91, -2750.366, 59.54743),
(@CGUID+186, 18, 5983, -11495.91, -2788.634, 59.54743),
(@CGUID+170, 0, 78667, -11794.41, -2815.424, 8.342058),
(@CGUID+170, 1, 78667, -11795.1, -2814.7, 8.342058),
(@CGUID+170, 2, 78667, -11789.33, -2820.755, 8.927263),
(@CGUID+170, 3, 78667, -11789.33, -2820.755, 8.927263),
(@CGUID+798, 8, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+798, 9, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+798, 4, 76556, -11892.72, -3202.146, -12.16765),
(@CGUID+798, 5, 76556, -11887.48, -3201.484, -13.79149),
(@CGUID+798, 6, 76556, -11879.43, -3200.469, -16.53307),
(@CGUID+798, 7, 76556, -11875.71, -3200, -17.98649),
(@CGUID+798, 0, 76556, -11907.14, -3203.174, -11.5685),
(@CGUID+798, 1, 76556, -11906.14, -3203.108, -11.5685),
(@CGUID+798, 2, 76556, -11895.49, -3202.495, -11.60509),
(@CGUID+798, 3, 76556, -11894.45, -3202.363, -11.53603),
(@CGUID+803, 5, 76556, -11892.31, -3211.014, -12.19763),
(@CGUID+803, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+803, 7, 76556, -11886.64, -3209.621, -13.88664),
(@CGUID+803, 6, 76556, -11891.26, -3210.755, -12.33153),
(@CGUID+803, 1, 76556, -11904.82, -3211.245, -11.56851),
(@CGUID+803, 0, 76556, -11905.82, -3211.312, -11.56851),
(@CGUID+803, 3, 76556, -11893.73, -3209.299, -11.49278),
(@CGUID+803, 2, 76556, -11894.94, -3209.512, -11.59181),
(@CGUID+803, 9, 76556, -11880.54, -3208.124, -15.91559),
(@CGUID+803, 8, 76556, -11883.76, -3208.915, -14.9311),
(@CGUID+803, 11, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+803, 10, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+750, 0, 5983, -12285.81, -3290.284, 108.3063),
(@CGUID+750, 1, 5983, -12282, -3271.15, 108.3063),
(@CGUID+750, 2, 5983, -12285.81, -3252.016, 108.3063),
(@CGUID+750, 3, 5983, -12296.64, -3235.795, 108.3063),
(@CGUID+750, 4, 5983, -12312.87, -3224.956, 108.3063),
(@CGUID+750, 5, 5983, -12332, -3221.15, 108.3063),
(@CGUID+750, 6, 5983, -12351.13, -3224.956, 108.3063),
(@CGUID+750, 7, 5983, -12367.36, -3235.795, 108.3063),
(@CGUID+750, 8, 5983, -12378.19, -3252.016, 108.3063),
(@CGUID+750, 9, 5983, -12382, -3271.15, 108.3063),
(@CGUID+750, 10, 5983, -12378.19, -3290.284, 108.3063),
(@CGUID+750, 11, 5983, -12367.36, -3306.505, 108.3063),
(@CGUID+750, 12, 5983, -12351.13, -3317.344, 108.3063),
(@CGUID+750, 13, 5983, -12332, -3321.15, 108.3063),
(@CGUID+750, 14, 5983, -12312.87, -3317.344, 108.3063),
(@CGUID+750, 15, 5983, -12296.64, -3306.505, 108.3063),
(@CGUID+750, 16, 5983, -12285.81, -3290.284, 108.3063),
(@CGUID+750, 17, 5983, -12282, -3271.15, 108.3063),
(@CGUID+750, 18, 5983, -12285.81, -3252.016, 108.3063),
(@CGUID+92, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+92, 5, 76556, -11892.56, -3203.622, -12.23155),
(@CGUID+92, 6, 76556, -11889.12, -3203.736, -13.35396),
(@CGUID+92, 7, 76556, -11881.07, -3204.004, -15.89535),
(@CGUID+92, 0, 76556, -11906.58, -3208.431, -11.63638),
(@CGUID+92, 1, 76556, -11905.59, -3208.365, -11.63638),
(@CGUID+92, 2, 76556, -11895.2, -3206.791, -11.60179),
(@CGUID+92, 3, 76556, -11894.67, -3206.71, -11.55334),
(@CGUID+810, 8, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+810, 9, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+92, 8, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+92, 9, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+810, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+810, 5, 76556, -11891.23, -3210.898, -12.31716),
(@CGUID+810, 6, 76556, -11887.06, -3210.237, -13.66055),
(@CGUID+810, 7, 76556, -11880.78, -3209.241, -15.79766),
(@CGUID+810, 0, 76556, -11905.43, -3212.839, -11.52012),
(@CGUID+810, 1, 76556, -11904.43, -3212.773, -11.52012),
(@CGUID+810, 2, 76556, -11894.81, -3210.88, -11.58679),
(@CGUID+810, 3, 76556, -11894.46, -3210.811, -11.54245),
(@CGUID+97, 9, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+97, 8, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+129, 9, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+129, 8, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+97, 1, 76556, -11905.96, -3206.681, -11.64358),
(@CGUID+97, 0, 76556, -11906.95, -3206.747, -11.64358),
(@CGUID+97, 3, 76556, -11894.95, -3204.945, -11.57077),
(@CGUID+97, 2, 76556, -11895.37, -3205.011, -11.60832),
(@CGUID+97, 5, 76556, -11892.58, -3203.497, -12.22616),
(@CGUID+97, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+129, 1, 76556, -11905.03, -3209.613, -11.61907),
(@CGUID+97, 7, 76556, -11881.18, -3203.098, -15.89048),
(@CGUID+129, 0, 76556, -11906.03, -3209.679, -11.61907),
(@CGUID+97, 6, 76556, -11887.22, -3203.381, -13.94922),
(@CGUID+129, 3, 76556, -11893.78, -3207.929, -11.49541),
(@CGUID+129, 2, 76556, -11895.08, -3208.123, -11.5969),
(@CGUID+129, 5, 76556, -11891.29, -3210.586, -12.34856),
(@CGUID+129, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+129, 7, 76556, -11880.66, -3207.246, -15.91098),
(@CGUID+129, 6, 76556, -11887.25, -3209.317, -13.74831),
(@CGUID+71, 0, 78667, -11701.11, -3213.364, 8.492327),
(@CGUID+71, 1, 78667, -11701.75, -3212.594, 8.492327),
(@CGUID+71, 2, 78667, -11703.65, -3211.988, 7.683977),
(@CGUID+71, 3, 78667, -11705.56, -3211.383, 6.683977),
(@CGUID+71, 4, 78667, -11707.47, -3210.777, 5.808977),
(@CGUID+71, 5, 78667, -11709.37, -3210.172, 5.058977),
(@CGUID+71, 6, 78667, -11711.28, -3209.566, 4.183977),
(@CGUID+71, 7, 78667, -11712.23, -3209.264, 3.558977),
(@CGUID+71, 8, 78667, -11714.14, -3208.658, 2.558977),
(@CGUID+71, 9, 78667, -11716.04, -3208.053, 1.433977),
(@CGUID+71, 10, 78667, -11717, -3207.75, 0.8089771),
(@CGUID+71, 11, 78667, -11718.9, -3207.145, -0.4410229),
(@CGUID+71, 12, 78667, -11721.76, -3206.236, -1.566023),
(@CGUID+71, 13, 78667, -11722.72, -3205.934, -2.316023),
(@CGUID+71, 14, 78667, -11723.67, -3205.631, -2.941023),
(@CGUID+71, 15, 78667, -11724.62, -3205.328, -3.691023),
(@CGUID+71, 16, 78667, -11725.58, -3205.025, -4.441023),
(@CGUID+71, 17, 78667, -11727.48, -3204.42, -5.566023),
(@CGUID+71, 18, 78667, -11728.31, -3204.156, -5.970564),
(@CGUID+71, 19, 78667, -11728.31, -3204.156, -5.970564),
(@CGUID+740, 16, 5983, -12148.11, -3826.056, 37.33013),
(@CGUID+740, 17, 5983, -12144.3, -3845.19, 37.33013),
(@CGUID+740, 18, 5983, -12148.11, -3864.324, 37.33013),
(@CGUID+740, 0, 5983, -12148.11, -3826.056, 37.33013),
(@CGUID+740, 1, 5983, -12144.3, -3845.19, 37.33013),
(@CGUID+740, 2, 5983, -12148.11, -3864.324, 37.33013),
(@CGUID+740, 3, 5983, -12158.94, -3880.545, 37.33013),
(@CGUID+740, 4, 5983, -12175.17, -3891.384, 37.33013),
(@CGUID+740, 5, 5983, -12194.3, -3895.19, 37.33013),
(@CGUID+740, 6, 5983, -12213.43, -3891.384, 37.33013),
(@CGUID+740, 7, 5983, -12229.66, -3880.545, 37.33013),
(@CGUID+740, 8, 5983, -12240.49, -3864.324, 37.33013),
(@CGUID+740, 9, 5983, -12244.3, -3845.19, 37.33013),
(@CGUID+740, 10, 5983, -12240.49, -3826.056, 37.33013),
(@CGUID+740, 11, 5983, -12229.66, -3809.835, 37.33013),
(@CGUID+740, 12, 5983, -12213.43, -3798.996, 37.33013),
(@CGUID+740, 13, 5983, -12194.3, -3795.19, 37.33013),
(@CGUID+740, 14, 5983, -12175.17, -3798.996, 37.33013),
(@CGUID+740, 15, 5983, -12158.94, -3809.835, 37.33013),
(@CGUID+493, 5, 76108, -11272.54, -3662.603, 4.839905),
(@CGUID+493, 4, 76108, -11282.84, -3675.714, 3.615087),
(@CGUID+493, 6, 76108, -11272.54, -3662.603, 4.839905),
(@CGUID+493, 1, 76108, -11289.25, -3674.276, 3.619689),
(@CGUID+493, 0, 76108, -11288.96, -3675.234, 3.619689),
(@CGUID+493, 3, 76108, -11285.98, -3677.306, 3.615085),
(@CGUID+493, 2, 76108, -11288.76, -3675.929, 3.614799),
(@CGUID+756, 9, 5983, -12499.2, -3225.06, 85.43784),
(@CGUID+756, 8, 5983, -12495.39, -3244.194, 85.43784),
(@CGUID+756, 11, 5983, -12484.56, -3189.705, 85.43784),
(@CGUID+756, 10, 5983, -12495.39, -3205.926, 85.43784),
(@CGUID+756, 13, 5983, -12449.2, -3175.06, 85.43784),
(@CGUID+756, 12, 5983, -12468.33, -3178.866, 85.43784),
(@CGUID+756, 15, 5983, -12413.84, -3189.705, 85.43784),
(@CGUID+756, 14, 5983, -12430.07, -3178.866, 85.43784),
(@CGUID+756, 1, 5983, -12399.2, -3225.06, 85.43784),
(@CGUID+756, 0, 5983, -12403.01, -3205.926, 85.43784),
(@CGUID+756, 3, 5983, -12413.84, -3260.415, 85.43784),
(@CGUID+756, 2, 5983, -12403.01, -3244.194, 85.43784),
(@CGUID+756, 5, 5983, -12449.2, -3275.06, 85.43784),
(@CGUID+756, 4, 5983, -12430.07, -3271.254, 85.43784),
(@CGUID+756, 7, 5983, -12484.56, -3260.415, 85.43784),
(@CGUID+756, 6, 5983, -12468.33, -3271.254, 85.43784),
(@CGUID+756, 17, 5983, -12399.2, -3225.06, 85.43784),
(@CGUID+756, 16, 5983, -12403.01, -3205.926, 85.43784),
(@CGUID+756, 18, 5983, -12403.01, -3244.194, 85.43784),
(@CGUID+806, 9, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+806, 8, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+806, 7, 76556, -11880.66, -3207.246, -15.91098),
(@CGUID+806, 6, 76556, -11887.25, -3209.317, -13.74831),
(@CGUID+806, 5, 76556, -11891.29, -3210.586, -12.34856),
(@CGUID+806, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+806, 3, 76556, -11893.78, -3207.929, -11.49541),
(@CGUID+806, 2, 76556, -11895.08, -3208.123, -11.5969),
(@CGUID+806, 1, 76556, -11905.03, -3209.613, -11.61907),
(@CGUID+806, 0, 76556, -11906.03, -3209.679, -11.61907),
(@CGUID+741, 0, 5983, -12186.91, -3741.704, 47.60783),
(@CGUID+741, 1, 5983, -12183.1, -3722.57, 47.60783),
(@CGUID+741, 2, 5983, -12186.91, -3703.436, 47.60783),
(@CGUID+741, 3, 5983, -12197.74, -3687.215, 47.60783),
(@CGUID+741, 4, 5983, -12213.97, -3676.376, 47.60783),
(@CGUID+741, 5, 5983, -12233.1, -3672.57, 47.60783),
(@CGUID+741, 6, 5983, -12252.23, -3676.376, 47.60783),
(@CGUID+741, 7, 5983, -12268.46, -3687.215, 47.60783),
(@CGUID+741, 8, 5983, -12279.29, -3703.436, 47.60783),
(@CGUID+741, 9, 5983, -12283.1, -3722.57, 47.60783),
(@CGUID+741, 10, 5983, -12279.29, -3741.704, 47.60783),
(@CGUID+741, 11, 5983, -12268.46, -3757.925, 47.60783),
(@CGUID+741, 12, 5983, -12252.23, -3768.764, 47.60783),
(@CGUID+741, 13, 5983, -12233.1, -3772.57, 47.60783),
(@CGUID+741, 14, 5983, -12213.97, -3768.764, 47.60783),
(@CGUID+741, 15, 5983, -12197.74, -3757.925, 47.60783),
(@CGUID+741, 16, 5983, -12186.91, -3741.704, 47.60783),
(@CGUID+741, 17, 5983, -12183.1, -3722.57, 47.60783),
(@CGUID+741, 18, 5983, -12186.91, -3703.436, 47.60783),
(@CGUID+705, 3, 76886, -11802.54, -3810.936, 56.92698),
(@CGUID+705, 2, 76886, -11802.54, -3810.936, 56.92698),
(@CGUID+705, 1, 76886, -11795.28, -3816.836, 56.93703),
(@CGUID+705, 0, 76886, -11796.16, -3816.358, 56.93703),
(@CGUID+103, 12, 76556, -11850.2, -3196.306, -28.36584),
(@CGUID+103, 10, 76556, -11852.96, -3196.127, -26.42541),
(@CGUID+103, 11, 76556, -11850.2, -3196.306, -28.36584),
(@CGUID+103, 8, 76556, -11858.03, -3195.799, -24.42565),
(@CGUID+103, 9, 76556, -11856.16, -3195.92, -26.06707),
(@CGUID+103, 6, 76556, -11864.4, -3196.133, -22.59183),
(@CGUID+103, 7, 76556, -11861.07, -3196.173, -23.81111),
(@CGUID+103, 4, 76556, -11866.17, -3198.074, -22.28523),
(@CGUID+103, 5, 76556, -11865.41, -3197.383, -21.96618),
(@CGUID+103, 2, 76556, -11873.54, -3199.506, -18.81035),
(@CGUID+103, 3, 76556, -11869.71, -3198.436, -20.49106),
(@CGUID+103, 0, 76556, -11875.96, -3200.031, -18.37715),
(@CGUID+103, 1, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+815, 6, 76556, -11887.25, -3209.317, -13.74831),
(@CGUID+815, 7, 76556, -11880.66, -3207.246, -15.91098),
(@CGUID+815, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+815, 5, 76556, -11891.29, -3210.586, -12.34856),
(@CGUID+815, 2, 76556, -11895.08, -3208.123, -11.5969),
(@CGUID+815, 3, 76556, -11893.78, -3207.929, -11.49541),
(@CGUID+815, 0, 76556, -11906.03, -3209.679, -11.61907),
(@CGUID+815, 1, 76556, -11905.03, -3209.613, -11.61907),
(@CGUID+815, 8, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+815, 9, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+107, 8, 76556, -11851.33, -3200.836, -28.29107),
(@CGUID+107, 4, 76556, -11863.1, -3202.515, -23.68135),
(@CGUID+107, 5, 76556, -11861.3, -3202.258, -24.32598),
(@CGUID+107, 6, 76556, -11860.08, -3202.083, -24.72857),
(@CGUID+107, 7, 76556, -11851.33, -3200.836, -28.29107),
(@CGUID+107, 0, 76556, -11875.96, -3204.173, -18.38214),
(@CGUID+107, 1, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+107, 2, 76556, -11871.66, -3203.735, -19.97247),
(@CGUID+240, 0, 77724, -11056.62, -2786.86, 7.564841),
(@CGUID+107, 3, 76556, -11870.05, -3203.507, -20.61034),
(@CGUID+240, 1, 77724, -11057.61, -2786.717, 7.564841),
(@CGUID+240, 2, 77724, -11065.01, -2779.979, 8.120994),
(@CGUID+240, 3, 77724, -11070.07, -2775.373, 8.125881),
(@CGUID+240, 4, 77724, -11070.07, -2775.373, 8.125881),
(@CGUID+823, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+823, 5, 76556, -11891.29, -3210.586, -12.34856),
(@CGUID+823, 6, 76556, -11887.25, -3209.317, -13.74831),
(@CGUID+823, 7, 76556, -11880.66, -3207.246, -15.91098),
(@CGUID+823, 0, 76556, -11906.03, -3209.679, -11.61907),
(@CGUID+823, 1, 76556, -11905.03, -3209.613, -11.61907),
(@CGUID+823, 2, 76556, -11895.08, -3208.123, -11.5969),
(@CGUID+823, 3, 76556, -11893.78, -3207.929, -11.49541),
(@CGUID+823, 8, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+823, 9, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+489, 0, 76085, -11256.37, -3646.606, 8.064513),
(@CGUID+489, 1, 76085, -11256.62, -3647.575, 8.064513),
(@CGUID+489, 2, 76085, -11257.29, -3649.457, 7.464904),
(@CGUID+489, 3, 76085, -11258.29, -3652.281, 6.839904),
(@CGUID+489, 4, 76085, -11259.29, -3655.105, 6.089904),
(@CGUID+489, 5, 76085, -11259.29, -3655.105, 6.089904),
(@CGUID+749, 16, 5983, -12224.61, -3326.436, 111.7163),
(@CGUID+749, 17, 5983, -12220.8, -3345.57, 111.7163),
(@CGUID+749, 18, 5983, -12224.61, -3364.704, 111.7163),
(@CGUID+118, 1, 76556, -11906.18, -3205.106, -11.64383),
(@CGUID+118, 0, 76556, -11907.18, -3205.173, -11.64383),
(@CGUID+118, 3, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+749, 0, 5983, -12224.61, -3326.436, 111.7163),
(@CGUID+118, 2, 76556, -11895.56, -3203.731, -11.61308),
(@CGUID+749, 1, 5983, -12220.8, -3345.57, 111.7163),
(@CGUID+118, 5, 76556, -11887.3, -3202.764, -13.82364),
(@CGUID+749, 2, 5983, -12224.61, -3364.704, 111.7163),
(@CGUID+118, 4, 76556, -11892.59, -3203.369, -12.22058),
(@CGUID+749, 3, 5983, -12235.44, -3380.925, 111.7163),
(@CGUID+118, 7, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+749, 4, 5983, -12251.67, -3391.764, 111.7163),
(@CGUID+118, 6, 76556, -11880.79, -3202.019, -16.09395),
(@CGUID+749, 5, 5983, -12270.8, -3395.57, 111.7163),
(@CGUID+749, 6, 5983, -12289.93, -3391.764, 111.7163),
(@CGUID+118, 8, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+749, 7, 5983, -12306.16, -3380.925, 111.7163),
(@CGUID+749, 8, 5983, -12316.99, -3364.704, 111.7163),
(@CGUID+749, 9, 5983, -12320.8, -3345.57, 111.7163),
(@CGUID+749, 10, 5983, -12316.99, -3326.436, 111.7163),
(@CGUID+749, 11, 5983, -12306.16, -3310.215, 111.7163),
(@CGUID+749, 12, 5983, -12289.93, -3299.376, 111.7163),
(@CGUID+749, 13, 5983, -12270.8, -3295.57, 111.7163),
(@CGUID+749, 14, 5983, -12251.67, -3299.376, 111.7163),
(@CGUID+749, 15, 5983, -12235.44, -3310.215, 111.7163),
(@CGUID+714, 9, 5983, -11907.3, -3842.75, 120.7483),
(@CGUID+714, 8, 5983, -11903.49, -3861.884, 120.7483),
(@CGUID+714, 11, 5983, -11892.66, -3807.395, 120.7483),
(@CGUID+714, 10, 5983, -11903.49, -3823.616, 120.7483),
(@CGUID+714, 13, 5983, -11857.3, -3792.75, 120.7483),
(@CGUID+714, 12, 5983, -11876.43, -3796.556, 120.7483),
(@CGUID+714, 15, 5983, -11821.94, -3807.395, 120.7483),
(@CGUID+714, 14, 5983, -11838.17, -3796.556, 120.7483),
(@CGUID+714, 1, 5983, -11807.3, -3842.75, 120.7483),
(@CGUID+714, 0, 5983, -11811.11, -3823.616, 120.7483),
(@CGUID+714, 3, 5983, -11821.94, -3878.105, 120.7483),
(@CGUID+714, 2, 5983, -11811.11, -3861.884, 120.7483),
(@CGUID+714, 5, 5983, -11857.3, -3892.75, 120.7483),
(@CGUID+714, 4, 5983, -11838.17, -3888.944, 120.7483),
(@CGUID+714, 7, 5983, -11892.66, -3878.105, 120.7483),
(@CGUID+714, 6, 5983, -11876.43, -3888.944, 120.7483),
(@CGUID+714, 17, 5983, -11807.3, -3842.75, 120.7483),
(@CGUID+714, 16, 5983, -11811.11, -3823.616, 120.7483),
(@CGUID+714, 18, 5983, -11811.11, -3861.884, 120.7483),
(@CGUID+761, 18, 5983, -12509.41, -3186.234, 73.11124),
(@CGUID+761, 17, 5983, -12505.6, -3167.1, 73.11124),
(@CGUID+761, 16, 5983, -12509.41, -3147.966, 73.11124),
(@CGUID+761, 7, 5983, -12590.96, -3202.455, 73.11124),
(@CGUID+761, 6, 5983, -12574.73, -3213.294, 73.11124),
(@CGUID+761, 5, 5983, -12555.6, -3217.1, 73.11124),
(@CGUID+761, 4, 5983, -12536.47, -3213.294, 73.11124),
(@CGUID+761, 3, 5983, -12520.24, -3202.455, 73.11124),
(@CGUID+761, 2, 5983, -12509.41, -3186.234, 73.11124),
(@CGUID+761, 1, 5983, -12505.6, -3167.1, 73.11124),
(@CGUID+761, 0, 5983, -12509.41, -3147.966, 73.11124),
(@CGUID+761, 15, 5983, -12520.24, -3131.745, 73.11124),
(@CGUID+761, 14, 5983, -12536.47, -3120.906, 73.11124),
(@CGUID+761, 13, 5983, -12555.6, -3117.1, 73.11124),
(@CGUID+761, 12, 5983, -12574.73, -3120.906, 73.11124),
(@CGUID+761, 11, 5983, -12590.96, -3131.745, 73.11124),
(@CGUID+761, 10, 5983, -12601.79, -3147.966, 73.11124),
(@CGUID+761, 9, 5983, -12605.6, -3167.1, 73.11124),
(@CGUID+761, 8, 5983, -12601.79, -3186.234, 73.11124),
(@CGUID+113, 6, 76556, -11889.12, -3203.736, -13.35396),
(@CGUID+113, 7, 76556, -11881.07, -3204.004, -15.89535),
(@CGUID+113, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+113, 5, 76556, -11892.56, -3203.622, -12.23155),
(@CGUID+113, 2, 76556, -11895.2, -3206.791, -11.60179),
(@CGUID+113, 3, 76556, -11894.67, -3206.71, -11.55334),
(@CGUID+113, 0, 76556, -11906.58, -3208.431, -11.63638),
(@CGUID+113, 1, 76556, -11905.59, -3208.365, -11.63638),
(@CGUID+113, 8, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+113, 9, 76556, -11874.96, -3204.207, -18.38214),
(@CGUID+728, 16, 5983, -12060.41, -3822.954, 49.58553),
(@CGUID+728, 17, 5983, -12056.6, -3803.82, 49.58553),
(@CGUID+728, 18, 5983, -12060.41, -3784.686, 49.58553),
(@CGUID+672, 1, 76556, -11378.66, -3406.212, 9.059752),
(@CGUID+672, 0, 76556, -11379.64, -3406.376, 9.059752),
(@CGUID+672, 3, 76556, -11374.71, -3403.137, 7.739196),
(@CGUID+672, 2, 76556, -11377.08, -3404.982, 8.364196),
(@CGUID+672, 5, 76556, -11372.65, -3401.533, 7.522643),
(@CGUID+672, 4, 76556, -11372.65, -3401.533, 7.522643),
(@CGUID+728, 0, 5983, -12060.41, -3822.954, 49.58553),
(@CGUID+728, 1, 5983, -12056.6, -3803.82, 49.58553),
(@CGUID+728, 2, 5983, -12060.41, -3784.686, 49.58553),
(@CGUID+728, 3, 5983, -12071.24, -3768.465, 49.58553),
(@CGUID+728, 4, 5983, -12087.47, -3757.626, 49.58553),
(@CGUID+728, 5, 5983, -12106.6, -3753.82, 49.58553),
(@CGUID+728, 6, 5983, -12125.73, -3757.626, 49.58553),
(@CGUID+728, 7, 5983, -12141.96, -3768.465, 49.58553),
(@CGUID+728, 8, 5983, -12152.79, -3784.686, 49.58553),
(@CGUID+728, 9, 5983, -12156.6, -3803.82, 49.58553),
(@CGUID+728, 10, 5983, -12152.79, -3822.954, 49.58553),
(@CGUID+728, 11, 5983, -12141.96, -3839.175, 49.58553),
(@CGUID+728, 12, 5983, -12125.73, -3850.014, 49.58553),
(@CGUID+728, 13, 5983, -12106.6, -3853.82, 49.58553),
(@CGUID+728, 14, 5983, -12087.47, -3850.014, 49.58553),
(@CGUID+728, 15, 5983, -12071.24, -3839.175, 49.58553),
(@CGUID+123, 6, 76556, -11887.22, -3203.381, -13.94922),
(@CGUID+123, 7, 76556, -11881.18, -3203.098, -15.89048),
(@CGUID+123, 4, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+123, 5, 76556, -11892.58, -3203.497, -12.22616),
(@CGUID+123, 2, 76556, -11895.37, -3205.011, -11.60832),
(@CGUID+123, 3, 76556, -11894.95, -3204.945, -11.57077),
(@CGUID+123, 0, 76556, -11906.95, -3206.747, -11.64358),
(@CGUID+123, 1, 76556, -11905.96, -3206.681, -11.64358),
(@CGUID+123, 8, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+123, 9, 76556, -11875.07, -3202.812, -18.40056),
(@CGUID+619, 1, 76886, -11622.65, -3648.299, 11.18083),
(@CGUID+619, 0, 76886, -11622.87, -3647.324, 11.18083),
(@CGUID+619, 3, 76886, -11620.94, -3655.887, 10.58049),
(@CGUID+619, 2, 76886, -11620.94, -3655.887, 10.58049),
(@CGUID+458, 2, 77640, -10929.68, -3489.707, 65.0793),
(@CGUID+458, 3, 77640, -10929.68, -3489.707, 65.0793),
(@CGUID+458, 0, 77640, -10939.87, -3493.338, 65.07928),
(@CGUID+458, 1, 77640, -10938.87, -3493.344, 65.07928),
(@CGUID+763, 5, 5983, -12520.6, -3119.4, 62.57213),
(@CGUID+763, 4, 5983, -12501.47, -3115.594, 62.57213),
(@CGUID+763, 7, 5983, -12555.96, -3104.755, 62.57213),
(@CGUID+763, 6, 5983, -12539.73, -3115.594, 62.57213),
(@CGUID+763, 1, 5983, -12470.6, -3069.4, 62.57213),
(@CGUID+763, 0, 5983, -12474.41, -3050.266, 62.57213),
(@CGUID+763, 3, 5983, -12485.24, -3104.755, 62.57213),
(@CGUID+763, 2, 5983, -12474.41, -3088.534, 62.57213),
(@CGUID+763, 13, 5983, -12520.6, -3019.4, 62.57213),
(@CGUID+763, 12, 5983, -12539.73, -3023.206, 62.57213),
(@CGUID+763, 15, 5983, -12485.24, -3034.045, 62.57213),
(@CGUID+763, 14, 5983, -12501.47, -3023.206, 62.57213),
(@CGUID+763, 9, 5983, -12570.6, -3069.4, 62.57213),
(@CGUID+763, 8, 5983, -12566.79, -3088.534, 62.57213),
(@CGUID+763, 11, 5983, -12555.96, -3034.045, 62.57213),
(@CGUID+763, 10, 5983, -12566.79, -3050.266, 62.57213),
(@CGUID+763, 17, 5983, -12470.6, -3069.4, 62.57213),
(@CGUID+182, 1, 78488, -11577.86, -2753.505, 6.291425),
(@CGUID+763, 16, 5983, -12474.41, -3050.266, 62.57213),
(@CGUID+182, 0, 78488, -11578.75, -2753.956, 6.291425),
(@CGUID+182, 3, 78488, -11590.56, -2759.91, 5.916425),
(@CGUID+763, 18, 5983, -12474.41, -3088.534, 62.57213),
(@CGUID+182, 2, 78488, -11590.56, -2759.91, 5.916425),
(@CGUID+77, 3, 82004, -11841.28, -3197.277, -29.71277),
(@CGUID+77, 2, 82004, -11841.28, -3197.277, -29.71277),
(@CGUID+77, 1, 82004, -11841.31, -3196.703, -29.67892),
(@CGUID+77, 0, 82004, -11840.31, -3196.682, -29.67892),
(@CGUID+774, 18, 5983, -12076.01, -3007.874, 225.6323),
(@CGUID+774, 17, 5983, -12072.2, -2988.74, 225.6323),
(@CGUID+774, 16, 5983, -12076.01, -2969.606, 225.6323),
(@CGUID+774, 7, 5983, -12157.56, -3024.095, 225.6323),
(@CGUID+774, 6, 5983, -12141.33, -3034.934, 225.6323),
(@CGUID+774, 5, 5983, -12122.2, -3038.74, 225.6323),
(@CGUID+774, 4, 5983, -12103.07, -3034.934, 225.6323),
(@CGUID+774, 3, 5983, -12086.84, -3024.095, 225.6323),
(@CGUID+774, 2, 5983, -12076.01, -3007.874, 225.6323),
(@CGUID+774, 1, 5983, -12072.2, -2988.74, 225.6323),
(@CGUID+774, 0, 5983, -12076.01, -2969.606, 225.6323),
(@CGUID+774, 15, 5983, -12086.84, -2953.385, 225.6323),
(@CGUID+774, 14, 5983, -12103.07, -2942.546, 225.6323),
(@CGUID+774, 13, 5983, -12122.2, -2938.74, 225.6323),
(@CGUID+774, 12, 5983, -12141.33, -2942.546, 225.6323),
(@CGUID+774, 11, 5983, -12157.56, -2953.385, 225.6323),
(@CGUID+774, 10, 5983, -12168.39, -2969.606, 225.6323),
(@CGUID+774, 9, 5983, -12172.2, -2988.74, 225.6323),
(@CGUID+774, 8, 5983, -12168.39, -3007.874, 225.6323),
(@CGUID+819, 5, 76556, -11887.3, -3202.764, -13.82364),
(@CGUID+819, 4, 76556, -11892.59, -3203.369, -12.22058),
(@CGUID+819, 7, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+819, 6, 76556, -11880.79, -3202.019, -16.09395),
(@CGUID+819, 1, 76556, -11906.18, -3205.106, -11.64383),
(@CGUID+819, 0, 76556, -11907.18, -3205.173, -11.64383),
(@CGUID+819, 3, 76556, -11894.3, -3203.564, -11.52697),
(@CGUID+819, 2, 76556, -11895.56, -3203.731, -11.61308),
(@CGUID+819, 8, 76556, -11875.16, -3201.375, -18.31035),
(@CGUID+154, 0, 78667, -11708.96, -2942.327, 7.550117),
(@CGUID+154, 1, 78667, -11708.59, -2943.254, 7.550117),
(@CGUID+154, 2, 78667, -11705.33, -2952.741, 7.473457),
(@CGUID+154, 3, 78667, -11705.33, -2952.741, 7.473457),
(@CGUID+505, 0, 76085, -11274.05, -3673.204, 4.51593),
(@CGUID+505, 1, 76085, -11273.23, -3672.625, 4.51593),
(@CGUID+505, 2, 76085, -11266.13, -3665.832, 4.964904),
(@CGUID+505, 3, 76085, -11266.13, -3665.832, 4.964904),
(@CGUID+122, 9, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+122, 8, 76556, -11874.97, -3199.906, -18.37715),
(@CGUID+122, 3, 76556, -11894.45, -3202.363, -11.53603),
(@CGUID+122, 2, 76556, -11895.49, -3202.495, -11.60509),
(@CGUID+122, 1, 76556, -11906.14, -3203.108, -11.5685),
(@CGUID+122, 0, 76556, -11907.14, -3203.174, -11.5685),
(@CGUID+122, 7, 76556, -11875.71, -3200, -17.98649),
(@CGUID+32, 3, 82014, -11829.47, -3206.806, -30.35743),
(@CGUID+122, 6, 76556, -11879.43, -3200.469, -16.53307),
(@CGUID+32, 2, 82014, -11829.47, -3206.806, -30.35743),
(@CGUID+122, 5, 76556, -11887.48, -3201.484, -13.79149),
(@CGUID+32, 1, 82014, -11828.99, -3206.885, -30.3285),
(@CGUID+122, 4, 76556, -11892.72, -3202.146, -12.16765),
(@CGUID+32, 0, 82014, -11828.1, -3207.331, -30.3285),
(@CGUID+174, 9, 5983, -11928.7, -2875.89, 154.8423),
(@CGUID+174, 8, 5983, -11924.89, -2895.024, 154.8423),
(@CGUID+174, 11, 5983, -11914.06, -2840.535, 154.8423),
(@CGUID+174, 10, 5983, -11924.89, -2856.756, 154.8423),
(@CGUID+174, 13, 5983, -11878.7, -2825.89, 154.8423),
(@CGUID+174, 12, 5983, -11897.83, -2829.696, 154.8423),
(@CGUID+174, 15, 5983, -11843.34, -2840.535, 154.8423),
(@CGUID+174, 14, 5983, -11859.57, -2829.696, 154.8423),
(@CGUID+174, 1, 5983, -11828.7, -2875.89, 154.8423),
(@CGUID+174, 0, 5983, -11832.51, -2856.756, 154.8423),
(@CGUID+174, 3, 5983, -11843.34, -2911.245, 154.8423),
(@CGUID+174, 2, 5983, -11832.51, -2895.024, 154.8423),
(@CGUID+174, 5, 5983, -11878.7, -2925.89, 154.8423),
(@CGUID+174, 4, 5983, -11859.57, -2922.084, 154.8423),
(@CGUID+174, 7, 5983, -11914.06, -2911.245, 154.8423),
(@CGUID+174, 6, 5983, -11897.83, -2922.084, 154.8423),
(@CGUID+174, 17, 5983, -11828.7, -2875.89, 154.8423),
(@CGUID+174, 16, 5983, -11832.51, -2856.756, 154.8423),
(@CGUID+174, 18, 5983, -11832.51, -2895.024, 154.8423),
(@CGUID+173, 1, 78674, -11676.73, -2819.826, 5.650958),
(@CGUID+173, 0, 78674, -11676.94, -2820.805, 5.650958),
(@CGUID+173, 3, 78674, -11677.53, -2812.33, 5.850176),
(@CGUID+101, 0, 76556, -11905.82, -3211.312, -11.56851),
(@CGUID+173, 2, 78674, -11677.53, -2812.33, 5.850176),
(@CGUID+101, 1, 76556, -11904.82, -3211.245, -11.56851),
(@CGUID+101, 2, 76556, -11894.94, -3209.512, -11.59181),
(@CGUID+101, 3, 76556, -11893.73, -3209.299, -11.49278),
(@CGUID+101, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+101, 5, 76556, -11892.31, -3211.014, -12.19763),
(@CGUID+101, 6, 76556, -11891.26, -3210.755, -12.33153),
(@CGUID+101, 7, 76556, -11886.64, -3209.621, -13.88664),
(@CGUID+101, 8, 76556, -11883.76, -3208.915, -14.9311),
(@CGUID+101, 9, 76556, -11880.54, -3208.124, -15.91559),
(@CGUID+101, 10, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+101, 11, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+725, 5, 5983, -11875.3, -3638.65, 148.1003),
(@CGUID+725, 4, 5983, -11856.17, -3642.456, 148.1003),
(@CGUID+725, 7, 5983, -11910.66, -3653.295, 148.1003),
(@CGUID+725, 6, 5983, -11894.43, -3642.456, 148.1003),
(@CGUID+725, 1, 5983, -11825.3, -3688.65, 148.1003),
(@CGUID+725, 0, 5983, -11829.11, -3707.784, 148.1003),
(@CGUID+725, 3, 5983, -11839.94, -3653.295, 148.1003),
(@CGUID+98, 9, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+725, 2, 5983, -11829.11, -3669.516, 148.1003),
(@CGUID+98, 8, 76556, -11875.53, -3205.637, -18.08854),
(@CGUID+725, 13, 5983, -11875.3, -3738.65, 148.1003),
(@CGUID+725, 12, 5983, -11894.43, -3734.844, 148.1003),
(@CGUID+725, 15, 5983, -11839.94, -3724.005, 148.1003),
(@CGUID+725, 14, 5983, -11856.17, -3734.844, 148.1003),
(@CGUID+725, 9, 5983, -11925.3, -3688.65, 148.1003),
(@CGUID+725, 8, 5983, -11921.49, -3669.516, 148.1003),
(@CGUID+725, 11, 5983, -11910.66, -3724.005, 148.1003),
(@CGUID+98, 1, 76556, -11905.03, -3209.613, -11.61907),
(@CGUID+725, 10, 5983, -11921.49, -3707.784, 148.1003),
(@CGUID+98, 0, 76556, -11906.03, -3209.679, -11.61907),
(@CGUID+98, 3, 76556, -11893.78, -3207.929, -11.49541),
(@CGUID+98, 2, 76556, -11895.08, -3208.123, -11.5969),
(@CGUID+98, 5, 76556, -11891.29, -3210.586, -12.34856),
(@CGUID+98, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+725, 17, 5983, -11825.3, -3688.65, 148.1003),
(@CGUID+98, 7, 76556, -11880.66, -3207.246, -15.91098),
(@CGUID+725, 16, 5983, -11829.11, -3707.784, 148.1003),
(@CGUID+98, 6, 76556, -11887.25, -3209.317, -13.74831),
(@CGUID+725, 18, 5983, -11829.11, -3669.516, 148.1003),
(@CGUID+817, 1, 76556, -11904.43, -3212.773, -11.52012),
(@CGUID+817, 0, 76556, -11905.43, -3212.839, -11.52012),
(@CGUID+817, 3, 76556, -11894.46, -3210.811, -11.54245),
(@CGUID+817, 2, 76556, -11894.81, -3210.88, -11.58679),
(@CGUID+817, 5, 76556, -11891.23, -3210.898, -12.31716),
(@CGUID+817, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+817, 7, 76556, -11880.78, -3209.241, -15.79766),
(@CGUID+817, 6, 76556, -11887.06, -3210.237, -13.66055),
(@CGUID+817, 9, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+817, 8, 76556, -11874.71, -3208.277, -18.27157),
(@CGUID+244, 0, 77767, -11063.17, -2784.57, 7.745994),
(@CGUID+244, 1, 77767, -11063.95, -2783.947, 7.745994),
(@CGUID+244, 2, 77767, -11072.68, -2777.99, 7.940091),
(@CGUID+244, 3, 77767, -11072.68, -2777.99, 7.940091),
(@CGUID+716, 4, 76886, -11803.89, -3836.191, 57.96293),
(@CGUID+716, 5, 76886, -11803.89, -3836.191, 57.96293),
(@CGUID+716, 0, 76886, -11810.6, -3843.872, 59.49809),
(@CGUID+716, 1, 76886, -11809.68, -3844.276, 59.49809),
(@CGUID+716, 2, 76886, -11806.77, -3840.213, 58.7476),
(@CGUID+716, 3, 76886, -11804.44, -3836.963, 58.1226),
(@CGUID+310, 5, 5983, -11408.3, -3044.93, 61.92463),
(@CGUID+310, 4, 5983, -11389.17, -3041.124, 61.92463),
(@CGUID+310, 7, 5983, -11443.66, -3030.285, 61.92463),
(@CGUID+310, 6, 5983, -11427.43, -3041.124, 61.92463),
(@CGUID+310, 1, 5983, -11358.3, -2994.93, 61.92463),
(@CGUID+310, 0, 5983, -11362.11, -2975.796, 61.92463),
(@CGUID+310, 3, 5983, -11372.94, -3030.285, 61.92463),
(@CGUID+310, 2, 5983, -11362.11, -3014.064, 61.92463),
(@CGUID+310, 13, 5983, -11408.3, -2944.93, 61.92463),
(@CGUID+310, 12, 5983, -11427.43, -2948.736, 61.92463),
(@CGUID+310, 15, 5983, -11372.94, -2959.575, 61.92463),
(@CGUID+310, 14, 5983, -11389.17, -2948.736, 61.92463),
(@CGUID+807, 8, 76556, -11883.76, -3208.915, -14.9311),
(@CGUID+310, 9, 5983, -11458.3, -2994.93, 61.92463),
(@CGUID+807, 9, 76556, -11880.54, -3208.124, -15.91559),
(@CGUID+310, 8, 5983, -11454.49, -3014.064, 61.92463),
(@CGUID+807, 10, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+310, 11, 5983, -11443.66, -2959.575, 61.92463),
(@CGUID+807, 11, 76556, -11874.72, -3207.266, -18.3378),
(@CGUID+310, 10, 5983, -11454.49, -2975.796, 61.92463),
(@CGUID+807, 0, 76556, -11905.82, -3211.312, -11.56851),
(@CGUID+310, 17, 5983, -11358.3, -2994.93, 61.92463),
(@CGUID+807, 1, 76556, -11904.82, -3211.245, -11.56851),
(@CGUID+310, 16, 5983, -11362.11, -2975.796, 61.92463),
(@CGUID+807, 2, 76556, -11894.94, -3209.512, -11.59181),
(@CGUID+807, 3, 76556, -11893.73, -3209.299, -11.49278),
(@CGUID+310, 18, 5983, -11362.11, -3014.064, 61.92463),
(@CGUID+807, 4, 76556, -11893.37, -3211.238, -11.47039),
(@CGUID+807, 5, 76556, -11892.31, -3211.014, -12.19763),
(@CGUID+807, 6, 76556, -11891.26, -3210.755, -12.33153),
(@CGUID+807, 7, 76556, -11886.64, -3209.621, -13.88664),
(@CGUID+738, 15, 5983, -12119.24, -3794.985, 59.75593),
(@CGUID+738, 14, 5983, -12135.47, -3805.824, 59.75593),
(@CGUID+738, 13, 5983, -12154.6, -3809.63, 59.75593),
(@CGUID+738, 12, 5983, -12173.73, -3805.824, 59.75593),
(@CGUID+738, 11, 5983, -12189.96, -3794.985, 59.75593),
(@CGUID+738, 10, 5983, -12200.79, -3778.764, 59.75593),
(@CGUID+738, 9, 5983, -12204.6, -3759.63, 59.75593),
(@CGUID+738, 8, 5983, -12200.79, -3740.496, 59.75593),
(@CGUID+738, 7, 5983, -12189.96, -3724.275, 59.75593),
(@CGUID+738, 6, 5983, -12173.73, -3713.436, 59.75593),
(@CGUID+738, 5, 5983, -12154.6, -3709.63, 59.75593),
(@CGUID+738, 4, 5983, -12135.47, -3713.436, 59.75593),
(@CGUID+738, 3, 5983, -12119.24, -3724.275, 59.75593),
(@CGUID+738, 2, 5983, -12108.41, -3740.496, 59.75593),
(@CGUID+738, 1, 5983, -12104.6, -3759.63, 59.75593),
(@CGUID+738, 0, 5983, -12108.41, -3778.764, 59.75593),
(@CGUID+419, 0, 76886, -10906.66, -3401.218, 99.61337),
(@CGUID+419, 1, 76886, -10905.68, -3401.443, 99.61337),
(@CGUID+419, 2, 76886, -10914.63, -3399.382, 99.61644),
(@CGUID+419, 3, 76886, -10950.71, -3391.076, 99.60451),
(@CGUID+419, 4, 76886, -10950.71, -3391.076, 99.60451),
(@CGUID+738, 18, 5983, -12108.41, -3740.496, 59.75593),
(@CGUID+738, 17, 5983, -12104.6, -3759.63, 59.75593),
(@CGUID+738, 16, 5983, -12108.41, -3778.764, 59.75593),
(@CGUID+295, 9, 5983, -11321.9, -3054.66, 52.21553),
(@CGUID+295, 8, 5983, -11318.09, -3035.526, 52.21553),
(@CGUID+295, 11, 5983, -11307.26, -3090.015, 52.21553),
(@CGUID+295, 10, 5983, -11318.09, -3073.794, 52.21553),
(@CGUID+295, 13, 5983, -11271.9, -3104.66, 52.21553),
(@CGUID+295, 12, 5983, -11291.03, -3100.854, 52.21553),
(@CGUID+295, 15, 5983, -11236.54, -3090.015, 52.21553),
(@CGUID+295, 14, 5983, -11252.77, -3100.854, 52.21553),
(@CGUID+295, 1, 5983, -11221.9, -3054.66, 52.21553),
(@CGUID+295, 0, 5983, -11225.71, -3073.794, 52.21553),
(@CGUID+295, 3, 5983, -11236.54, -3019.305, 52.21553),
(@CGUID+295, 2, 5983, -11225.71, -3035.526, 52.21553),
(@CGUID+295, 5, 5983, -11271.9, -3004.66, 52.21553),
(@CGUID+295, 4, 5983, -11252.77, -3008.466, 52.21553),
(@CGUID+295, 7, 5983, -11307.26, -3019.305, 52.21553),
(@CGUID+295, 6, 5983, -11291.03, -3008.466, 52.21553),
(@CGUID+295, 17, 5983, -11221.9, -3054.66, 52.21553),
(@CGUID+295, 16, 5983, -11225.71, -3073.794, 52.21553),
(@CGUID+295, 18, 5983, -11225.71, -3035.526, 52.21553),
(@CGUID+249, 2, 77723, -10997.5, -2716.051, 6.415553),
(@CGUID+249, 3, 77723, -10995.18, -2715.913, 6.41555),
(@CGUID+249, 0, 77723, -11003.63, -2717.329, 6.415556),
(@CGUID+249, 1, 77723, -11003.27, -2716.394, 6.415556),
(@CGUID+249, 4, 77723, -10995.18, -2715.913, 6.41555);

--
DELETE FROM `creature_addon` WHERE `guid`  in (select guid from creature where id = 76556);
DELETE FROM `waypoint_data` WHERE `id` in (select guid from creature where id = 76556);
DELETE FROM `creature` WHERE id = 76556;

SET @CGUID = 995032;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@CGUID+817, 76556, 0, 1, 2, -11904.43, -3212.773, -11.52012, 0.1943905, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+818, 76556, 0, 1, 2, -11906.14, -3203.108, -11.5685, 0.05748955, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+819, 76556, 0, 1, 2, -11906.18, -3205.106, -11.64383, 0.1287902, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+820, 76556, 0, 1, 2, -11905.96, -3206.681, -11.64358, 0.156449, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+821, 76556, 0, 1, 2, -11905.59, -3208.365, -11.63638, 0.1503888, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+822, 76556, 0, 1, 2, -11904.82, -3211.245, -11.56851, 0.1736725, 120, 0, 0), -- 76556 (Area: 72)
(@CGUID+823, 76556, 0, 1, 2, -11905.03, -3209.613, -11.61907, 0.1485685, 120, 0, 0); -- 76556 (Area: 72)