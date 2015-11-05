
insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('51505','8050','1','9','0','1','0','0','0','1','7','0','0','0','50','0','1','Выброс лавы');

DELETE FROM spell_script_names WHERE spell_id IN (55090);

insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('77478','0.1276','0','0','0','0','0','Землетрясение');

insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('3691','0','1','0','2','182387','0','0','0','0','0','0','0','Землетрясение'),
('3691','1','42','1','2','182387','0','0','0','0','0','0','0','Землетрясение');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('77756','0','0','8050','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Волна лавы');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('77756','77762','0','20','0','0','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Волна лавы');
insert ignore into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('77762','174928','0','0','0','0','0','0','0','0','0','0','0','0','Волна лавы'),
('77762','51505','0','0','0','0','0','0','0','0','0','0','0','8','Волна лавы');

DELETE FROM spell_script_names WHERE spell_id IN (8050);

insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('170374','170377','0','20','0','0','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Расплавленная земля');

insert into `spell_aura_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) values
('170377','170379','0','6','0','0','0','0','0','7','0','0','Расплавленная земля');
