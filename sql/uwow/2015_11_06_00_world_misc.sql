
insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('51505','8050','1','9','0','1','0','0','0','1','7','0','0','0','50','0','1','Выброс лавы');

DELETE FROM spell_script_names WHERE spell_id IN (55090);

insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('77478','0.1276','0','0','0','0','0','Землетрясение');

insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('3691','0','1','0','2','182387','0','0','0','0','0','0','0','Землетрясение'),
('3691','1','42','1','2','182387','0','0','0','0','0','0','0','Землетрясение');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('77756','0','0','8050','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Волна лавы');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('77756','77762','0','20','0','0','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Волна лавы');
insert ignore into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('77762','174928','0','0','0','0','0','0','0','0','0','0','0','0','Волна лавы'),
('77762','51505','0','0','0','0','0','0','0','0','0','0','0','8','Волна лавы');

DELETE FROM spell_script_names WHERE spell_id IN (8050);

insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('170374','170377','0','20','0','0','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Расплавленная земля');

insert into `spell_aura_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) values
('170377','170379','0','6','0','0','0','0','0','7','0','0','Расплавленная земля');

DELETE FROM spell_script_names WHERE spell_id IN (73680);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('73680','73681','0','0','0','0','0','0','0','0','0','0','0','0','Высвободить чары воздуха'),
('73680','73683','0','0','0','0','0','0','0','0','0','0','0','0','Высвободить чары огня');

update spell_dummy_trigger SET target = 3 where target = 1;
update spell_dummy_trigger SET caster = 5 where caster = 1;
update spell_dummy_trigger SET target = 1 where target = 2;

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('60103','105792','6','0','0','0','-55444','1','8050','0','0','0','0','0','Вскипание лавы');
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('105792','16','1','-8050','0','0','0','0','7','1','6','0','0','0','Вскипание лавы');
insert into `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) values
('105792','8050','8','0','4','0','0','0','0','7','8050','0','Вскипание лавы');

DELETE FROM spell_script_names WHERE spell_id IN (1535);
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('1535','15','1','8050','0','0','0','0','7','1','6','0','0','0','Кольцо огня'),
('8349','16','9','0','0','0','0','0','7','0','0','0','0','0','Кольцо огня');
insert into `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) values
('1535','8349','0','0','0','0','0','0','0','7','0','0','Кольцо огня');

insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('58877','58879','0','3','5','0','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Призрачная охота');
insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('58879','63271','1','9','0','3','0','0','0','0','7','0','0','0','0','0','1','Символ духа дикого зверя (Совершенствование)');

DELETE FROM spell_bonus_data WHERE entry IN (10444);
insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('10444','0','0','0.075','0','0','0','Flametongue Weapon');
DELETE FROM spell_proc_event WHERE entry IN (33757);


insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('73685','0','0','61295','0','0','0','4','-1','-1','0','0','0','0','0','0','0','0','0','0','Высвободить чары жизни');
insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('61295','73685','1','9','0','0','0','0','0','3','7','0','0','0','0','0','1','Высвободить чары жизни');

DELETE FROM spell_script_names WHERE spell_id IN (61882, 77478);
insert into `spell_aura_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) values
('61882','77478','7','0','0','0','0','0','0','7','0','0','Землетрясение');
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('77478','77505','1','0','0','0','0','0','0','0','0','0','0','0','Землетрясение');

insert into `spell_aura_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `slot`, `comment`) values
('108280','114942','6','0','0','0','0','0','0','7','0','0','2','Целительный прилив');

insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('974','379','0','0','1','1','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Щит земли');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('114051','17364 ','6','0','0','0','0','0','0','0','0','0','0','8','Перерождение');

DELETE FROM spell_pet_auras WHERE petEntry IN (15352,15438) AND spellId IN (65223);
UPDATE creature_template SET spell1 = 0 WHERE entry IN (15352);
insert into `spell_aura_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `slot`, `comment`) values
('34967','36213','0','7','0','0','0','0','0','7','0','0','0','Тотем стихий');
replace into `pet_stats` (`entry`, `hp`, `ap`, `ap_type`, `spd`, `school_mask`, `maxspdorap`, `state`, `energy`, `energy_type`, `armor`, `type`, `damage`, `haste`, `comment`) values
('15352','1','-1','0','1','8','0','0','0','1','1','0','0','1','Earth Elemental Totem'),
('61056','1.5','1.5','0','-1.5','8','0','0','0','1','1','0','1.1','1','Earth Elemental Totem');

UPDATE creature_template SET spell1 = 0 WHERE entry IN (3527);
insert into `spell_aura_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `slot`, `comment`) values
('5394','52042','6','0','0','0','0','0','0','7','0','0','3','Тотем исцеляющего потока');
DELETE FROM spell_target_filter WHERE spellId IN (52042);
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('52042','31','0','0','0','0','147074','0','7','1','1','0','1','0','Тотем исцеляющего потока');

DELETE FROM spell_target_filter WHERE spellId IN (114942);
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('114942','31','0','0','0','0','0','0','7','1','5','12','0','0','Целительный прилив');

UPDATE creature_template SET spell1 = 0 WHERE entry IN (5929);
insert into `spell_aura_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `slot`, `comment`) values
('8190','8187','6','0','0','0','0','0','0','7','0','0','3','Тотем магмы');

