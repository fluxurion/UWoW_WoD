insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('35395','85416','0','0','0','0','85043','0','0','30','0','0','0','0','0','-1','0','0','Великий воин Света'),
('53595','85416','0','0','0','0','85043','0','0','30','0','0','0','0','0','-1','0','0','Великий воин Света'),
('85416','31935','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','8','0','Великий воин Света');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('20271','105427','0','0','0','0','105424','0','0','0','0','0','0','0','0','-1','0','0','Правосудие мудрых');

DELETE FROM spell_proc_check WHERE entry IN (53563);
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('53563','0','0','0','0','0','0','1','-1','-1','0','0','0','0','0','-30','0','0','0','0','Спасенный Светом'),
('53563','0','0','19750','0','0','0','4','-1','-1','0','0','0','0','0','0','0','0','0','0','Частица Света'),
('53563','0','0','82326','0','0','0','4','-1','-1','0','0','0','0','0','0','0','0','0','0','Частица Света');
DELETE FROM spell_trigger WHERE spell_id IN (53563);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('53563','88852','0','2','7','1','0','0','40','0','0','4','0','0','0','0','0','0','0','-1','0','0','0','Частица Света'),
('53563','157128','0','20','1','1','1','1','0','0','0','1','157047','-157131','0','0','0','0','0','-1','0','0','0','Спасенный Светом');

DELETE FROM spell_proc_event WHERE entry IN (53651);

