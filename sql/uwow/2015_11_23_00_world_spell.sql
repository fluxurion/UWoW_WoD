insert into `spell_talent_linked_spell` (`spellid`, `spelllink`, `type`, `target`, `caster`, `comment`) values
('152244','0','3','0','0','Адаптация'),
('-152244','0','3','0','0','Адаптация');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('155228','spell_hun_lone_wolf_tal');
insert into `spell_talent_linked_spell` (`spellid`, `spelllink`, `type`, `target`, `caster`, `comment`) values
('-155228','-164273 ','0','0','0','Одинокий волк');

DELETE FROM spell_proc_event WHERE entry IN (19387);
insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('1614','1','1','0','2','64803','1','0','19387','0','0','0','0','Западня'),
('1615','1','1','0','2','64803','1','0','19387','0','0','0','0','Западня');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('17253','spell_hun_enhanced_basic_attacks');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('16827','spell_hun_enhanced_basic_attacks');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('49966','spell_hun_enhanced_basic_attacks');
