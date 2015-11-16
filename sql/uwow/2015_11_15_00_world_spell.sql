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

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('157478','0','0','82326','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Усиленный шок небес'),
('157478','0','0','19750','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Усиленный шок небес');

DELETE FROM spell_aura_dummy WHERE spellId IN (121129,88819);
insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('88819','157455','0','1','0','0','0','0','0','0','7','0','0','0','0','0','1','Улучшенный рассвет');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('114163','144595','0','0','0','0','174718','0','0','25','0','0','0','0','0','-1','0','0','cast Eternal Flame add aura Divine Purpose'),
('85673','144595','0','0','0','0','174718','0','0','25','0','0','0','0','0','-1','0','0','cast Word of Glory add aura Divine Purpose'),
('85256','144595','0','0','0','0','174718','0','0','25','0','0','0','0','0','-1','0','0','cast Templars Verdict add aura Divine Purpose'),
('85222','144595','0','0','0','0','174718','0','0','25','0','0','0','0','0','-1','0','0','cast Light of Dawn add aura Divine Purpose'),
('53600','144595','0','0','0','0','174718','0','0','25','0','0','0','0','0','-1','0','0','cast Shield of the Righteous add aura Divine Purpose'),
('53385','144595','0','0','0','0','174718','0','0','25','0','0','0','0','0','-1','0','0','cast Divine Storm add aura Divine Purpose'),
('136494','144595','0','0','0','0','174718','0','0','25','0','0','0','0','0','-1','0','0','cast Word of Glory add aura Divine Purpose'),
('130552','144595','0','0','0','0','174718','0','0','25','0','0','0','0','0','-1','0','0','cast Harsh Word add aura Divine Purpose');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('20473','-160002','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Усиленный шок небес');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('20271','111528','0','0','0','2','85256','0','0','0','0','0','0','0','0','-1','0','0','Правосудие');

DELETE FROM spell_trigger WHERE spell_id IN (31801);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('31801','31803','0','20','0','0','0','0','0','0','0','4','0','0','0','0','0','0','0','-1','0','0','0','Paladin - Seal of Truth - Dot Aura'),
('31801','42463','0','1','0','0','0','0','12','0','0','1','0','0','0','0','0','0','0','-1','0','0','0','Paladin - Seal of Truth - Weapon Dmg');
DELETE FROM spell_proc_check WHERE entry IN (31801);
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('31801','0','0','-24275','0','0','0','1','-1','-1','0','0','0','0','0','0','0','0','0','0','Печать правды'),
('31801','0','0','0','0','0','0','4','-1','-1','0','0','0','0','0','0','-4','0','0','0','Печать правды');

DELETE FROM spell_linked_spell WHERE spell_effect IN (-114250) AND spell_trigger IN (82326,82327);

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('128863','0','0','19750','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Самоотверженный целитель');

DELETE FROM spell_proc_check WHERE entry IN (20154,20164);
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('20154','0','0','0','0','0','0','7','-1','-1','0','0','0','0','0','0','-4','0','0','0','Печать праведности'),
('20164','20165','0','-24275','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Печати'),
('20164','20165','0','-20271','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Печати');
