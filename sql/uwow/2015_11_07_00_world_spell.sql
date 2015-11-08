insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('157784','0','0','165462','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Безудержное высвобождение стихий'),
('157784','0','0','73685','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Безудержное высвобождение стихий');

DELETE FROM spell_script_names WHERE spell_id IN (45284, 45297);
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('51505', 'spell_sha_rolling_thunder');
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `comment`) values
('421','157766','0','0','0','0','157765','0','0','0','0','0','0','0','3','0','Усиленная цепная молния'),
('114074','157766','0','0','0','0','157765','0','0','0','0','0','0','0','3','0','Усиленная цепная молния');

insert into `spell_concatenate_aura` (`spellid`, `effectSpell`, `auraId`, `effectAura`, `type`, `comment`) values
('157774','0','88766','0','1','88766 - Сверкание');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('157804','0','0','8050','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Усиленный огненный шок');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('157804','60103','0','4','1','0','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Усиленный огненный шок');

replace into `spell_aura_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `slot`, `comment`) values
('170377','170379','0','4','0','0','0','0','0','7','0','0','0','Расплавленная земля');
insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('170379','0.24','0','0','0','0','0','Расплавленная земля');
