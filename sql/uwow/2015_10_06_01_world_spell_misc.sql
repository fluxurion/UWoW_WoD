insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastalent`, `hastalent2`, `chance`, `cooldown`, `type2`, `hitmask`, `learnspell`, `removeMask`, `comment`) values
('408','154953','0','0','0','154904','0','0','0','0','0','0','0','Внутреннее кровотечение');
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('154953','spell_rog_internal_bleeding');

replace into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('2094','1776','107079','-154953','108216','100','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Грязные приемы (Пассивный талант)'),
('2094','1776','107079','-89775','108216','100','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Грязные приемы (Пассивный талант)'),
('2094','1776','107079','-2818','108216','100','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Грязные приемы (Пассивный талант)'),
('2094','1776','107079','-1943','108216','100','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Грязные приемы (Пассивный талант)'),
('2094','1776','107079','-703','108216','100','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Грязные приемы (Пассивный талант)');

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('137619','spell_rog_marked_for_death');

delete from spell_script_names where ScriptName = 'spell_rog_master_poisoner';
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastalent`, `hastalent2`, `chance`, `cooldown`, `type2`, `hitmask`, `learnspell`, `removeMask`, `comment`) values
('-152152','-156719','0','0','0','0','0','0','0','0','0','0','0','Сила отравителя');
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('2818','spell_rog_deadly_poison_venom');
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('3409','spell_rog_crippling_poison_venom');
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('8680','spell_rog_wound_poison_venom');

replace into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('152150','163786','0','0','3','0','0','0','0','0','0','0','0','0','Смерть с небес'),
('152150','178236','5','0','3','0','0','0','0','0','0','0','0','0','Смерть с небес'),
('156327','2098','0','0','4','2','84617','0','0','0','0','0','0','0','Смерть с небес'),
('156327','32645','0','0','4','2','1329','0','0','0','0','0','0','0','Смерть с небес'),
('156327','178070','0','0','4','0','0','0','0','0','0','0','0','0','Смерть с небес');


insert into `spell_script_names` (`spell_id`, `ScriptName`) values('178236','spell_rog_death_from_above_jump');
