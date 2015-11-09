DELETE FROM spell_trigger WHERE spell_id IN (108281);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('108281','114911','0','9','0','0','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Ancestral Guidance');

DELETE FROM spell_target_filter WHERE spellId IN (114911);
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('114911','31','0','0','0','0','0','0','7','1','3','3','0','0','Ancestral Guidance');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `comment`) values
('165462','118470','0','0','0','0','165477','0','0','0','0','0','0','0','-1','0','Освобожденная ярость');
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `comment`) values
('73680','118472','0','0','0','0','117012','0','0','0','0','0','0','0','-1','0','Освобожденная ярость');
DELETE FROM spell_linked_spell WHERE spell_trigger IN (73685);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `comment`) VALUES
('73685','118473','0','0','0','0','165479','0','0','0','0','0','0','0','-1','0','Освобожденная ярость');

UPDATE creature_template SET spell1 = 157331, spell2 = 157348 WHERE entry IN (77936);
UPDATE creature_template SET spell1 = 157331, spell2 = 157348, spell3 = 157375, spell4 = 157382 WHERE entry IN (77942);

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('157784','0','0','73683','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Безудержное высвобождение стихий');

DELETE FROM spell_aura_dummy WHERE spellId IN (61882,77478);
insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('61882','157766','0','0','0','0','0','0','0','0','2','0','0','0','0','0','1','Усиленная цепная молния'),
('61882','157766','0','2','0','0','0','0','157766','0','2','0','0','0','200','0','1','Усиленная цепная молния'),
('77478','61882','1','9','0','0','0','0','0','1','7','0','0','0','0','0','1','Усиленная цепная молния');

DELETE FROM spell_linked_spell WHERE spell_trigger IN (152256);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `comment`) values
('152256','157299','6','0','0','0','-117013','0','0','0','0','0','0','0','-1','0','Тотем элементаля бури'),
('152256','157319','6','0','0','0','117013','0','0','0','0','0','0','0','-1','0','Тотем элементаля бури');

replace into `pet_stats` (`entry`, `hp`, `ap`, `ap_type`, `spd`, `school_mask`, `maxspdorap`, `state`, `energy`, `energy_type`, `armor`, `type`, `damage`, `haste`, `comment`) values
('61029','0.9','1','0','-1','8','0','0','17','0','1','0','0.9','1','Primal Fire Elemental'),
('61056','1.5','1.5','0','-1.5','8','0','0','0','1','1','0','1.1','1','Earth Elemental Totem'),
('77936','0.75','1','0','1','127','0','0','0','0','1','1','0','1','Тотем элементаля бури'),
('77942','0.9','1','0','1','127','0','0','0','0','1','1','0','1','Тотем элементаля бури');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('51530','0','0','0','0','0','0','7','-1','-1','263','0','0','0','0','0','0','0','0','0','Оружие Водоворота');

insert into `spell_pet_auras` (`petEntry`, `spellId`, `option`, `target`, `targetaura`, `bp0`, `bp1`, `bp2`, `aura`, `casteraura`, `createdspell`, `fromspell`, `comment`) values
('77936','34967','0','0','0','0','0','0','0','0','0','0','Тотем стихий'),
('77936','157328','0','0','0','0','0','0','0','0','0','0','Элементаль бури');

insert into `spell_talent_linked_spell` (`spellid`, `spelllink`, `type`, `target`, `caster`, `comment`) values
('-88766','-324','0','0','0','Сверкание');

delete from spell_script_names where spell_id IN (107574,122294,116841,108843,113073,97993,1850);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `comment`) values
('1850','-106898','1','0','0','0','0','0','0','0','0','0','0','0','-1','0','Порыв'),
('1850','1850','0','0','0','0','0','0','0','0','0','0','0','0','-1','9','Порыв'),
('20589','20589','0','0','0','0','0','0','0','0','0','0','0','0','-1','9','Мастер побега (Расовая)'),
('97993','97993','1','0','0','0','0','0','0','0','0','0','0','0','-1','9','Тревожный рев'),
('107574','107574','0','0','0','0','0','0','0','0','0','0','0','0','-1','9','Аватара'),
('108843','108843','0','0','0','0','0','0','0','0','0','0','0','0','-1','9','Молниеносность'),
('114029','114029','6','0','0','0','0','0','0','0','0','0','0','0','-1','9','Охрана'),
('116841','116841','1','0','0','0','0','0','0','0','0','0','0','0','-1','9','Тигриное рвение');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `comment`) values
('58875','58875','0','0','0','0','0','0','0','0','0','0','0','0','-1','9','Поступь духа');

