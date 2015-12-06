DELETE FROM spell_script_names WHERE spell_id IN (755, -755);
DELETE FROM spell_bonus_data WHERE entry IN (689);

insert into `spell_check_cast` (`spellId`, `type`, `param1`, `errorId`, `customErrorId`, `caster`, `target`, `checkType`, `dataType`, `checkType2`, `dataType2`, `comment`) values
('755','1','-100','2','0','0','1','0','0','0','0','Канал здоровья - проверка фулл ХП'),
('1454','1','15','0','28','0','3','0','0','0','0','Жизнеотвод - проверка 15% ХП'),
('108482','1','20','0','28','0','3','0','0','0','0','Свободная воля - проверка 20% ХП'),
('120451','0','-1','8','0','0','1','0','0','0','0','Пламя Зорота - проверка отсутствия пета');

DELETE FROM pet_stats WHERE entry IN (416, 1860, 417, 1863, 17252, 11859);
insert into `pet_stats` (`entry`, `hp`, `ap`, `ap_type`, `spd`, `school_mask`, `maxspdorap`, `state`, `energy`, `energy_type`, `armor`, `type`, `damage`, `haste`, `comment`) values
('416','0.3','-1.0','0','1','36','0','2','200','3','3','1','0','1','Imp'),
('1860','0.5','-1.0','0','1','36','0','2','200','3','4','0','0','1','Voidwalker'),
('417','0.4','-1.0','0','1','36','0','2','200','3','3','0','0','1','Felhunter'),
('1863','0.4','-1.0','0','1','36','0','2','200','3','3','0','0','1','Succubus'),
('17252','0.5','-1.0','0','1','36','0','2','200','3','3','0','0','1','Felguard'),
('11859','0.4','-1','0','1','36','0','2','100','3','1','1','0','1','Doomguard');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('115232','spell_warl_shadow_shield');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('115240','spell_warl_shadow_shield_damage');

DELETE FROM spell_trigger WHERE spell_id IN (123050);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('123050','123051','0','1','5','0','0','0','75','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Высасывание маны Атаки подчиняющего разум');

DELETE FROM spell_aura_dummy WHERE spellId IN (152118);

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('121148','spell_pri_cascade_heal');

DELETE FROM spell_trigger WHERE spell_id IN (124495);
DELETE FROM spell_proc_check WHERE entry IN (124495);

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('2944','155361','1','0','0','0','0','0','0','0','0','0','0','0','0','-1','10','0','Энтропия Бездны');

DELETE FROM spell_linked_spell WHERE spell_trigger IN (148859);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('148859','155521','1','10','0','0','155271','0','0','0','0','0','0','0','0','-1','0','0','Благосклонные духи');

DELETE FROM spell_aura_dummy WHERE spellId IN (17962);
insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('17962','157736','0','4','0','0','3','0','0','0','2','0','0','0','0','0','1','Поджигание');

DELETE FROM spell_aura_dummy WHERE spellId IN (108683);
insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('108683','157696','0','0','0','0','0','0','0','0','8','0','0','0','0','0','1','Огонь и сера'),
('108683','-77220','0','2','0','0','0','0','0','0','32','0','0','0','225','0','1','Огонь и сера');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('157736','spell_warl_immolate_dot');
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('108686','157736','1','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Жертвенный огонь');

DELETE FROM creature_equip_template WHERE CreatureID IN (1860);

DELETE FROM pet_stats WHERE entry IN (78158,78217,58959,58960,58964,58963,58965);
insert into `pet_stats` (`entry`, `hp`, `ap`, `ap_type`, `spd`, `school_mask`, `maxspdorap`, `state`, `energy`, `energy_type`, `armor`, `type`, `damage`, `haste`, `comment`) values
('78158','0.5','-1','0','1','36','0','2','200','3','3','0','0','1','Страж ужаса'),
('78217','0.5','-1','0','1','36','0','2','200','3','3','0','0','1','Инфернал'),
('58959','0.4','-1','0','1','36','0','1','200','3','3','1','0','1','Fel Imp'),
('58960','0.6','-1','0','1','36','0','1','0','0','3','0','0','1','Voidlord'),
('58963','0.5','-1','0','1','36','0','1','200','3','3','0','0','1','Shivarra'),
('58964','0.5','-1','0','1','36','0','1','200','3','3','0','0','1','Observer'),
('58965','0.6','-1','0','1','36','0','1','0','0','1','0','0','1','Wrathguard');

