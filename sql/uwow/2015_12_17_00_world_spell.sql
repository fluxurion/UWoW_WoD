insert into `spell_script_names` (`spell_id`, `ScriptName`) values('783','spell_dru_travel_form');

insert ignore into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('5487','-783','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('5487','-114282','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('5487','-24858','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('768','-783','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('768','-114282','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('768','-24858','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('783','-5487','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('783','-768','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('783','-114282','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('783','-24858','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('114282','-5487','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('114282','-768','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('114282','-783','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('114282','-24858','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('24858','-5487','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('24858','-768','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('24858','-114282','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы'),
('24858','-783','6','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Снятие формы');

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('1822','spell_dru_rake');

DELETE FROM spell_target_filter WHERE spellId IN (113828);
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('113828','31','0','0','0','0','0','0','7','0','2','0','0','-1','Целительное прикосновение');

insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('1822','145152','1','9','0','0','0','0','0','0','7','0','0','0','0','0','1','Кровавые когти'),
('106830','145152','1','9','0','0','0','0','0','0','7','0','0','0','0','0','1','Кровавые когти'),
('77758','145152','1','9','0','0','0','0','0','0','7','0','0','0','0','0','1','Кровавые когти'),
('155722','145152','1','9','0','0','0','0','0','0','7','0','0','0','0','0','1','Кровавые когти');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('106830','-145152','8','0','3','0','0','0','0','0','0','0','0','0','0','-1','12','0','Кровавые когти'),
('1822','-145152','8','0','3','0','0','0','0','0','0','0','0','0','0','-1','12','0','Кровавые когти'),
('77758','-145152','8','0','3','0','0','0','0','0','0','0','0','0','0','-1','12','0','Кровавые когти');

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('155578','spell_dru_guardian_of_elune');

DELETE FROM spell_linked_spell WHERE spell_trigger IN (80313);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('80313','-158790','1','5','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Растерзать'),
('80313','-33745','1','5','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Растерзать'),
('80313','158792','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Раздавить');

