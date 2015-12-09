DELETE FROM pet_stats WHERE entry IN (78158, 78217, 78216);
insert into `pet_stats` (`entry`, `hp`, `ap`, `ap_type`, `spd`, `school_mask`, `maxspdorap`, `state`, `energy`, `energy_type`, `armor`, `type`, `damage`, `haste`, `comment`) values
('78158','0.5','-0.06','0','1','36','0','2','100','3','3','1','0','1','Страж ужаса'),
('78217','0.5','-0.06','0','1','36','0','2','1','0','3','0','0','1','Инфернал'),
('78216','0.5','-0.06','0','1','127','0','2','1','0','3','0','0','1','Абиссал');
UPDATE creature_template SET unit_flags2 = unit_flags2 | 2048 WHERE entry IN (78158);
UPDATE creature_template SET unit_flags2 = unit_flags2 | 2048 WHERE entry IN (78216);

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('85692','spell_warl_doom_bolt');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('152107','spell_warl_demonic_servitude');

DELETE FROM spell_dummy_trigger WHERE spell_id IN (119905,119907,119913,119911,119909,119910,119914,119915);
insert into `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) values
('119905','119899','4','0','1','0','0','0','0','7','0','0','Прижигание ран хозяина (Особая способность)'),
('119907','0','4','0','1','0','0','0','0','7','0','0','Обезоруживание (Особая способность)'),
('119909','0','5','0','1','0','0','0','0','7','0','0','Удар кнутом (Особая способность)'),
('119910','0','4','0','1','0','0','0','0','7','0','0','Запрет чар (Особая способность)'),
('119911','0','4','0','1','0','0','0','0','7','0','0','Оптический удар (Особая способность)'),
('119913','0','5','0','1','0','0','0','0','7','0','0','Кнут Скверны (Особая способность)'),
('119914','89751','4','0','1','0','0','0','0','7','0','0','Буря Скверны (Особая способность)'),
('119915','115831','4','0','1','0','0','0','0','7','0','0','Буря гнева (Особая способность)');

DELETE FROM spell_linked_spell WHERE spell_effect IN (119905,119907,119913,119911,119909,119910,119914,119915);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('119899','119905','0','2','0','0','0','0','0','0','0','0','0','0','0','-1','3','0','Прижигание ран хозяина (Особая способность)'),
('115781','119911','0','2','0','0','0','0','0','0','0','0','0','0','0','-1','3','0','Оптический удар (Особая способность)'),
('19647','119910','0','2','0','0','0','0','0','0','0','0','0','0','0','-1','3','0','Запрет чар (Особая способность)'),
('89751','119914','0','2','0','0','0','0','0','0','0','0','0','0','0','-1','3','0','Буря Скверны (Особая способность)'),
('115831','119915','0','2','0','0','0','0','0','0','0','0','0','0','0','-1','3','0','Буря гнева (Особая способность)'),
('115770','119913','0','2','0','0','0','0','0','0','0','0','0','0','0','-1','3','0','Кнут Скверны (Особая способность)'),
('6360','119909','0','2','0','0','0','0','0','0','0','0','0','0','0','-1','3','0','Удар кнутом (Особая способность)');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('86121','-74434','8','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Горящая душа');

UPDATE creature_template SET unit_flags2 = unit_flags2 | 2048 WHERE entry IN (78215);
insert into `pet_stats` (`entry`, `hp`, `ap`, `ap_type`, `spd`, `school_mask`, `maxspdorap`, `state`, `energy`, `energy_type`, `armor`, `type`, `damage`, `haste`, `comment`) values
('78215','0.5','-0.06','0','1','127','0','2','100','3','3','1','0','1','Стражник жути');

DELETE FROM spell_linked_spell WHERE spell_trigger IN (17767);

DELETE FROM spell_script_names WHERE ScriptName IN ('spell_warl_shadow_shield_damage');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('115234','spell_warl_shadow_shield_damage');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('114635','177376','8','0','0','0','174848','0','0','0','0','0','0','0','0','-1','0','0','Жгучее пламя');

insert into `spell_talent_linked_spell` (`spellid`, `spelllink`, `type`, `target`, `caster`, `comment`) values
('-108415','-108446','2','1','1','Родство душ');

insert into `spell_check_cast` (`spellId`, `type`, `param1`, `param2`, `param3`, `errorId`, `customErrorId`, `caster`, `target`, `checkType`, `dataType`, `checkType2`, `dataType2`, `comment`) values
('113858','0','0','0','0','10','0','0','0','0','-113858','0','0','Тьма Архимонда'),
('113860','0','0','0','0','10','0','0','0','0','-113860','0','0','Тьма Архимонда'),
('113861','0','0','0','0','10','0','0','0','0','-113861','0','0','Тьма Архимонда');

insert into `spell_visual` (`spellId`, `SpellVisualID`, `TravelSpeed`, `MissReason`, `ReflectStatus`, `SpeedAsTime`, `type`, `temp`, `comment`) values
('157701','45285','19','0','0','0','0','0','Стрела Хаоса');

DELETE FROM spell_trigger WHERE spell_id IN (123050);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('123050','123051','0','1','5','0','0','0','75','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Высасывание маны Атаки подчиняющего разум');

insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('152118','77484','0','1','0','0','0','0','0','0','7','0','0','0','0','0','1','Ясность воли');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('74434','157697','0','0','0','0','152109','0','0','0','0','0','0','0','0','-1','0','0','Soulburn Dummy'),
('48181','157698','1','3','3','0','157697','0','0','0','0','0','0','0','0','-1','0','0','Блуждающие духи'),
('-74434','-157697','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Soulburn Dummy');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('152108','30108','1','0','0','2','980','0','0','0','0','0','0','0','0','-1','0','0','Катаклизм'),
('152108','980','1','0','0','2','980','0','0','0','0','0','0','0','0','-1','0','0','Катаклизм'),
('152108','146739','1','0','0','2','104315','0','0','0','0','0','0','0','0','-1','0','0','Катаклизм'),
('152108','157736','1','0','0','2','348','0','0','0','0','0','0','0','0','-1','0','0','Катаклизм');

insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('171975','171982','0','20','10','0','0','0','0','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Гримуар помощи');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('78674','164547','0','0','0','0','164724','0','0','0','0','0','0','0','0','-1','0','0','Звездный поток'),
('78674','164545','0','0','0','0','164725','0','0','0','0','0','0','0','0','-1','0','0','Звездный поток');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('8921','164812','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Лунный огонь'),
('93402','164815','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Солнечный огонь');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('93399','0','0','164812','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Падающие звезды'),
('93399','0','0','164815','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Падающие звезды');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('93399','93400','0','20','1','0','0','0','0','0','0','7','0','0','7','0','0','0','0','-1','0','0','0','Падающие звезды');
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('93400','78674','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','8','0','Падающие звезды'),
('93400','48505','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Падающие звезды');

