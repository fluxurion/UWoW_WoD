REPLACE INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
('176247','1116','3745.1','-4056.01','46.6','2.811'),
('176243','1116','5267.64','-4061.54','21.15','0.926'),
('121858','1','-3748.85','-4440.35','30.551','0.725'),
('121856','0','-10468.9','-3329.99','25.472','0.859'),
('121852','1','1776.21','-4332.21','-7.97','3.773'),
('121859','1','-964.02','284.161','111.241','3.673'),
('121849','1','9657.56','2513.23','1331.72','4.336'),
('121862','0','1770.89','63.296','-46.32','0.645'),
('121851','0','-4610.5','-912.9','501.13','3.869');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('2948','157633','0','0','0','0','157632','0','0','0','0','0','0','0','0','-1','0','0','Улучшенный ожог');

insert into `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) values
('157642','0','0','0','0','0','0','0','1','0','0','0','7');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('157727','0','0','10','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Улучшенная снежная буря');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('157727','84714','0','4','1','0','0','0','500','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Улучшенная снежная буря');

insert into `pet_stats` (`entry`, `hp`, `ap`, `ap_type`, `spd`, `school_mask`, `maxspdorap`, `state`, `energy`, `energy_type`, `armor`, `type`, `damage`, `haste`, `comment`) values
('78116','0.5','-0.5','0','1','16','0','1','100','0','3','1','0','1','Water Elemental');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('116','157648','1','0','0','0','157646','0','-157648','0','0','0','0','0','0','-1','0','0','Усиленная ледяная стрела');

insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('36032','-157595','0','2','0','0','0','0','0','0','16','0','0','0','0','0','1','Усиленная чародейская вспышка');

DELETE FROM spell_bonus_data WHERE entry IN (11426);
insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('11426','4.95','0','0','0','0','0','Mage - Ice Barrier');

DELETE FROM spell_script_names WHERE spell_id IN (11958);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('11958','120','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','8','0','Холодная хватка'),
('11958','122','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','8','0','Холодная хватка'),
('11958','45438','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','8','0','Холодная хватка'),
('11958','12043','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','8','0','Холодная хватка'),
('11958','31661','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','8','0','Холодная хватка'),
('11958','157913','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','8','0','Холодная хватка');

insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('114923','36032','1','9','0','0','0','0','0','0','7','0','0','0','0','0','1','Буря Пустоты'),
('114954','36032','1','9','0','0','0','0','0','0','7','0','0','0','0','0','1','Буря Пустоты');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('133','spell_mage_fireball');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('116','spell_mage_unstable_magic');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('44614','spell_mage_unstable_magic');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('157980','spell_mage_supernova');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('157981','spell_mage_supernova');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('157997','spell_mage_supernova');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('108853','118280','0','0','0','1','44457','0','0','0','0','0','1','0','0','-1','0','0','Живая бомба');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('30455','113092','1','0','0','1','112948','7','4','0','0','0','0','0','0','-1','0','0','Ледяная бомба'),
('113092','-112948','1','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Ледяная бомба');

DELETE FROM spell_script_names WHERE spell_id IN (1463, -1463);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('1463','spell_mage_incanters_flow');

insert into `pet_stats` (`entry`, `hp`, `ap`, `ap_type`, `spd`, `school_mask`, `maxspdorap`, `state`, `energy`, `energy_type`, `armor`, `type`, `damage`, `haste`, `comment`) values
('76933','65','1','0','1','127','0','1','100','0','1','1','0','0','Радужный кристалл');
insert into `spell_pet_auras` (`petEntry`, `spellId`, `option`, `target`, `targetaura`, `bp0`, `bp1`, `bp2`, `aura`, `casteraura`, `createdspell`, `fromspell`, `comment`) values
('76933','155153','0','0','0','0','0','0','0','0','0','0','Радужный кристалл');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('155153','155152','0','3','1','5','0','0','0','-2','0','2','0','0','0','0','0','0','0','-1','0','0','0','Радужный кристалл');
insert into `spell_visual` (`spellId`, `SpellVisualID`, `TravelSpeed`, `MissReason`, `ReflectStatus`, `SpeedAsTime`, `type`, `temp`, `comment`) values
('155152','36621','0','0','0','0','0','0','Радужный кристалл');
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('155152','16','9','0','0','0','0','0','7','0','0','0','0','0','Радужный кристалл');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('30455','12472','0','0','3','2','155149','0','0','0','0','2000','0','0','0','-1','10','0','Ледяная Бездна'),
('5143','12042','0','0','3','2','155147','0','0','0','0','2000','0','0','0','-1','10','0','Перегрузка');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('155148','0','0','133','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Разжигание'),
('155148','0','0','11366','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Разжигание'),
('155148','0','0','108853','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Разжигание');
insert into `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) values
('155148','0','0','0','0','0','0','0','2','0','0','0','7');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('155148','11129','0','4','1','0','0','0','-1000','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Разжигание');

insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('1612','0','1','0','2','153640','0','0','0','0','0','0','0','Чародейский шар');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('153595','spell_monk_comet_storm');

insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('3467','0','8','0','32','153564','0','0','0','0','0','0','0','Метеор');
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('153561','177345','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','11','0','Метеор');

insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('1712','0','1','0','2','155158','0','0','0','0','0','0','0','Метеор'),
('1712','1','42','1','2','155158','0','0','0','0','0','0','0','Метеор');

DELETE FROM spell_proc_event WHERE entry IN (44448);
insert into `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) values
('44448','0','0','0','0','0','0','0','2','0','0','0','7');
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('44448','0','0','0','0','0','0','7','-1','-1','0','0','0','0','0','0','-4','0','0','0','Ясность мысли Огненная глыба');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('152261','spell_pal_holy_shield');
DELETE FROM spell_proc_event WHERE entry IN (152261);
insert into `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) values
('152261','0','0','0','0','0','0','0','1088','0','0','0','7');

DELETE FROM spell_trigger WHERE spell_id IN (157727);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('157727','84714','0','4','1','0','0','0','-500','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Улучшенная снежная буря');

DELETE FROM spell_proc_check WHERE entry IN (157727);
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('157727','0','0','42208','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Улучшенная снежная буря');

DELETE FROM spell_bonus_data WHERE entry IN (44461);

UPDATE creature_template SET `exp` = 5, spell1 = 59638, ScriptName = '' WHERE entry IN (31216);
UPDATE creature_template SET `exp` = 5, spell1 = 88084, ScriptName = '' WHERE entry IN (47243);
UPDATE creature_template SET `exp` = 5, spell1 = 88082, ScriptName = '' WHERE entry IN (47244);
DELETE FROM spell_pet_auras WHERE spellId IN (45205,41054,45206);
UPDATE spell_pet_auras SET `target` = 1 WHERE spellId IN (41055);
DELETE FROM spell_linked_spell WHERE spell_trigger IN (55342);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('55342','88085','6','0','0','8','62','0','0','0','0','0','0','0','0','-1','0','0','Зеркальное изображение - Темная магия'),
('55342','88087','6','0','0','8','62','0','0','0','0','0','0','0','0','-1','0','0','Зеркальное изображение - Темная магия'),
('55342','88089','6','0','0','8','62','0','0','0','0','0','0','0','0','-1','0','0','Зеркальное изображение - Темная магия'),
('55342','58833','6','0','0','8','64','0','0','0','0','0','0','0','0','-1','0','0','Зеркальное изображение - Лед'),
('55342','58831','6','0','0','8','64','0','0','0','0','0','0','0','0','-1','0','0','Зеркальное изображение - Лед'),
('55342','58834','6','0','0','8','64','0','0','0','0','0','0','0','0','-1','0','0','Зеркальное изображение - Лед'),
('55342','88088','6','0','0','8','63','0','0','0','0','0','0','0','0','-1','0','0','Зеркальное изображение - Огонь'),
('55342','88086','6','0','0','8','63','0','0','0','0','0','0','0','0','-1','0','0','Зеркальное изображение - Огонь'),
('55342','88090','6','0','0','8','63','0','0','0','0','0','0','0','0','-1','0','0','Зеркальное изображение - Огонь');
replace into `spell_pet_auras` (`petEntry`, `spellId`, `option`, `target`, `targetaura`, `bp0`, `bp1`, `bp2`, `aura`, `casteraura`, `createdspell`, `fromspell`, `comment`) values
('47243','58838','0','0','0','0','0','0','0','0','0','0','Inherit Masters Threat List'),
('47243','45204','0','2','0','0','0','0','0','0','0','0','Клонирование себя'),
('47243','41055','0','1','0','0','0','0','0','0','0','0','Копирование оружия'),
('47244','58838','0','0','0','0','0','0','0','0','0','0','Inherit Masters Threat List'),
('47244','45204','0','2','0','0','0','0','0','0','0','0','Клонирование себя'),
('47244','41055','0','1','0','0','0','0','0','0','0','0','Копирование оружия');

replace into `pet_stats` (`entry`, `hp`, `ap`, `ap_type`, `spd`, `school_mask`, `maxspdorap`, `state`, `energy`, `energy_type`, `armor`, `type`, `damage`, `haste`, `comment`) values
('31216','0.1','-0.45','0','1','16','0','1','100','0','1','1','0','1','Mirror Image'),
('47243','0.1','-0.45','0','1','16','0','1','100','0','1','1','0','1','Mirror Image'),
('47244','0.1','-0.45','0','1','16','0','1','100','0','1','1','0','1','Mirror Image');

UPDATE creature_template SET `exp` = 5, minlevel = 101 WHERE entry IN (76933);

insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('153626','16','8','0','0','0','0','0','7','0','0','0','0','0','Чародейский шар');

DELETE FROM areatrigger_actions WHERE entry IN (1712,3467);
insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('1712','0','1','0','2','155158','0','0','0','0','0','0','0','Метеор'),
('1712','1','42','1','2','155158','0','0','0','0','0','0','0','Метеор'),
('3467','0','32','0','32','153564','1','0','0','0','0','0','0','Метеор');
