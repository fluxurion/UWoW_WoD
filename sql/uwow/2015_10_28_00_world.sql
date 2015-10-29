insert into `spell_talent_linked_spell` (`spellid`, `spelllink`, `type`, `target`, `caster`, `comment`) values
('-155783','-155784','0','0','0','Первобытное упорство'),
('155783','155784','0','0','0','Первобытное упорство');

DELETE FROM spell_script_names WHERE spell_id IN (77758,33745);

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('33745','33917','0','0','0','0','0','0','0','25','0','0','0','8','Растерзать');

insert into `spell_concatenate_aura` (`spellid`, `effectSpell`, `auraId`, `effectAura`, `comment`) values
('155783','0','155784','1','Первобытное упорство'),
('155783','1','155784','0','Первобытное упорство');
