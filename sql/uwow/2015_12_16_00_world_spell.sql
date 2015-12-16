DELETE FROM spell_linked_spell WHERE spell_trigger IN (93400) AND spell_effect IN (48505);

DELETE FROM spell_aura_dummy WHERE spellId IN (48438);
insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('48438','114108','1','9','0','0','0','0','0','2','7','0','0','0','0','0','1','Душа леса');
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('48438','-114108','8','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Душа леса');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('1850','768','6','0','0','0','-768','0','0','0','0','0','0','0','0','-1','0','0','Порыв - активация облика');

insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('113828','0','0','0','0','0','0','0','7','0','0','0','0','-1','Целительное прикосновение');

DELETE FROM spell_trigger WHERE spell_id IN (124974);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('124974','124988','0','3','0','0','0','0','0','0','0','4','0','0','0','0','327696','0','0','-1','0','0','0','Природная чуткость'),
('124974','124988','0','3','0','0','0','0','0','0','0','8','0','0','0','0','278528','0','0','-1','0','0','0','Природная чуткость'),
('124974','124991','0','3','0','0','0','0','0','0','0','8','85101','0','0','0','278528','0','0','-1','0','0','0','Природная чуткость');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('2912','164812','0','0','0','0','152220','0','0','0','0','6000','0','0','0','-1','10','0','Баланс сил'),
('5176','164815','0','0','0','0','152220','0','0','0','0','4000','0','0','0','-1','10','0','Баланс сил');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('768','155627','0','0','0','0','155580','0','0','0','0','0','0','0','0','-1','0','0','Лунное вдохновение'),
('-768','-155627','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Лунное вдохновение');

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('171745','spell_dru_cat_form');

