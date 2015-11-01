insert into `spell_talent_linked_spell` (`spellid`, `spelllink`, `type`, `target`, `caster`, `comment`) values
('-155783','-155784','0','0','0','Первобытное упорство'),
('155783','155784','0','0','0','Первобытное упорство');

DELETE FROM spell_script_names WHERE spell_id IN (77758,33745);

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('33745','33917','0','0','0','0','0','0','0','25','0','0','0','8','Растерзать');

insert into `spell_concatenate_aura` (`spellid`, `effectSpell`, `auraId`, `effectAura`, `comment`) values
('155783','0','155784','1','Первобытное упорство'),
('155783','1','155784','0','Первобытное упорство');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('5217','768','5','0','0','0','-768','0','0','0','0','0','0','0','Тигриное неистовство');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('48484','0','0','33917','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Увечье');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('1822','163505','6','0','0','0','5215','0','0','0','0','0','0','0','Глубокая рана'),
('106839','93985','5','0','0','0','0','0','0','0','0','0','0','0','Лобовая атака');

DELETE FROM spell_script_names WHERE spell_id IN (779,62078);
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('106785','spell_dru_swipe');

