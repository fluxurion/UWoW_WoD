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
