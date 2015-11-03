insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('77758','158723','1','0','0','0','0','0','0','0','0','0','0','0','Взбучка');

insert into `spell_pet_auras` (`petEntry`, `spellId`, `option`, `target`, `targetaura`, `bp0`, `bp1`, `bp2`, `aura`, `casteraura`, `createdspell`, `fromspell`, `comment`) values
('47649','164717','0','2','0','0','0','0','0','0','88747','0','Дикий гриб Баланс'),
('47649','81262','0','0','0','0','0','0','0','0','145205','0','Дикий гриб Исцеление');
replace into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('2612','0','5','0','2','81281','0','-81281','0','0','0','0','0','Дикий гриб'),
('2612','1','42','1','2','81281','0','0','0','0','0','0','0','Дикий гриб');

DELETE FROM spell_script_names WHERE spell_id IN (88423);

insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('145518','31','7','0','0','0','0','0','7','0','0','0','0','0','Сотворение'),
('145518','31','1','0','774','0','0','0','7','0','0','0','0','0','Сотворение');

DELETE FROM spell_linked_spell WHERE spell_trigger IN (18562);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('18562','114108','0','0','0','0','158478','0','0','0','0','0','0','0','Soul of the Forest');

DELETE FROM spell_linked_spell WHERE spell_trigger IN (102560);

insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('314','2','42','1','2','127797','0','0','0','0','0','0','0','Вихрь Урсола');

DELETE FROM spell_script_names WHERE spell_id IN (124974);

insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('124974','124988','0','9','0','0','0','0','0','0','4','0','0','0','327696','0','0','-1','0','0','0','Природная чуткость'),
('124974','124991','0','9','0','0','0','0','0','0','8','85101','0','0','278528','0','0','-1','0','0','0','Природная чуткость'),
('124974','124988','0','9','0','0','0','0','0','0','8','0','0','0','278528','0','0','-1','0','0','0','Природная чуткость');

insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('124988','31','0','0','0','0','0','0','7','0','1','0','0','2','Природная чуткость'),
('124991','16','2','0','0','0','0','0','7','0','1','0','0','0','Природная чуткость');
