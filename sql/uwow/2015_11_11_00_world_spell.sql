insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('156321','169667','6','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Атака щитом'),
('156321','126661','6','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Атака щитом');
insert into `spell_talent_linked_spell` (`spellid`, `spelllink`, `type`, `target`, `caster`, `comment`) values
('-152276','-156291','0','0','0','Стойка гладиатора');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('29838','0','0','0','-16491','0','0','7','-1','-1','0','0','0','0','0','-35','0','0','0','0','Второе дыхание');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('29838','16491','0','20','0','0','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Второе дыхание');

