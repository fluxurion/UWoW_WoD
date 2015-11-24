DELETE FROM spell_talent_linked_spell WHERE spellid IN (152244, -152244);
insert into `spell_talent_linked_spell` (`spellid`, `spelllink`, `type`, `target`, `caster`, `comment`) values
('-152244','152244','2','0','0','Адаптация'),
('152244','152244','3','0','0','Адаптация');


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

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('-164273','-160198','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Одинокий волк'),
('-164273','-160199','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Одинокий волк'),
('-164273','-160200','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Одинокий волк'),
('-164273','-160203','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Одинокий волк'),
('-164273','-160205','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Одинокий волк'),
('-164273','-160206','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Одинокий волк'),
('-164273','-172967','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Одинокий волк'),
('-164273','-172968','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Одинокий волк');

