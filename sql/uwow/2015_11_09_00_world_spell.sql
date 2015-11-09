replace into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('8050','165462','1','9','0','0','0','0','0','1','1','0','0','0','0','0','1','Высвободить чары огня'),
('8050','165462','0','1','0','0','0','0','165462','1','2','0','0','0','0','0','1','Высвободить чары огня'),
('8050','73683','1','9','0','0','0','0','0','1','1','0','0','0','0','0','1','Высвободить чары огня'),
('8050','73683','0','1','0','0','0','0','73683','1','2','0','0','0','0','0','1','Высвободить чары огня'),
('8050','157174','1','9','0','0','0','0','0','0','1','0','0','0','0','0','1','Высвободить чары огня'),
('8050','157174','0','1','0','0','0','0','157174','0','2','0','0','0','0','0','1','Высвободить чары огня');

replace into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('51530','0','0','-73680','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Оружие Водоворота'),
('51530','0','0','-73681','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Оружие Водоворота'),
('51530','0','0','-73683','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Оружие Водоворота');

DELETE FROM spell_dummy_trigger WHERE spell_id IN (105792);
insert into `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) values
('105792','8050','8','0','4','4','0','0','0','7','8050','0','Вскипание лавы');

DELETE FROM spell_target_filter WHERE spellId IN (8349);
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('8349','16','1','-8050','0','0','0','0','7','1','6','0','0','0','Кольцо огня');

DELETE FROM spell_aura_dummy WHERE spellId IN (61295);
insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('61295','73685','1','9','0','0','0','0','0','3','7','0','0','0','0','0','1','Высвободить чары жизни'),
('61295','73685','0','1','0','0','0','0','73685','3','7','0','0','0','0','0','1','Высвободить чары жизни');

DELETE FROM spell_proc_event WHERE entry IN (8178);
insert into `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) values
('8178','0','0','0','0','0','0','139936','65536','0','0','0','7');

DELETE FROM spell_trigger WHERE spell_id IN (108281);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('108281','114911','0','3','0','0','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Ancestral Guidance');

insert into `spell_concatenate_aura` (`spellid`, `effectSpell`, `auraId`, `effectAura`, `type`, `caster`, `target`, `comment`) values
('77223','0','34967','2','0','2','1','Тотем стихий');

DELETE FROM spell_script_names WHERE ScriptName IN ("spell_sha_elemental_blast");
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('117014','118522','0','0','0','0','0','0','0','25','0','0','0','0','-1','0','1','Удар духов стихии'),
('117014','173183','0','0','0','0','0','0','0','25','0','0','0','0','-1','0','1','Удар духов стихии'),
('117014','173184','0','0','0','0','0','0','0','25','0','0','0','0','-1','0','1','Удар духов стихии'),
('117014','173185','0','0','0','0','0','0','0','25','0','0','0','0','-1','0','1','Удар духов стихии'),
('117014','173187','0','0','0','2','1064','0','0','0','0','0','0','0','-1','0','0','Удар духов стихии - исцеление'),
('117014','173186','0','0','0','2','51533','0','0','25','0','0','0','0','-1','0','0','Удар духов стихии - совершенствование');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('51505','157174','0','0','0','0','152257','0','0','0','0','0','0','0','-1','0','0','Взаимодействие стихий'),
('60103','157174','0','0','0','0','152257','0','0','0','0','0','0','0','-1','0','0','Взаимодействие стихий');

insert into `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) values
('157501','177601','6','0','0','0','0','0','0','7','0','0','Жидкая магма');

insert into `spell_pet_auras` (`petEntry`, `spellId`, `option`, `target`, `targetaura`, `bp0`, `bp1`, `bp2`, `aura`, `casteraura`, `createdspell`, `fromspell`, `comment`) values
('78001','157504','2','0','0','0','0','0','0','0','0','0','Тотем разразившегося ливня'),
('78001','157504','2','1','0','0','0','0','0','0','0','0','Тотем разразившегося ливня');

insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('157504','157504','0','39','0','0','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Тотем разразившегося ливня');

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('157504','spell_sha_cloudburst_totem');


