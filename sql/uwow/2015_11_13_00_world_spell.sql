delete from areatrigger_actions where entry = 1713 and id = 1;
insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('1713','1','512','0','2','156004','0','0','0','0','0','0','0','Осквернение'),
('1713','2','42','1','2','156004','0','0','0','0','0','0','0','Осквернение');

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('156000','spell_dk_defile_damage');

delete from spell_proc_check where entry = 155166;
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('155166','0','0','0','0','0','7','7','-1','-1','250','0','0','0','0','0','0','0','0','0','Метка Синдрагосы');

insert into `spell_concatenate_aura` (`spellid`, `effectSpell`, `auraId`, `effectAura`, `type`, `caster`, `target`, `comment`) values
('50385','0','48266','2','1','0','0','Великая власть льда'),
('50385','0','48266','2','0','0','0','Великая власть льда');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('81164','0','0','0','-157335','0','0','7','-1','-1','0','0','0','0','0','-30','0','0','0','0','Захват рун');
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('171049','157335','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Захват рун');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('50842','50421','0','0','0','0','49509','0','0','0','0','0','0','0','0','-1','0','0','Запах крови'),
('114866','50421','0','0','0','0','49509','0','0','0','0','0','0','0','0','-1','0','0','Запах крови');

INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) VALUES
('50842','55078','1','0','0','1','55078','0','49509','0','0','0','0','0','0','-1','10','0','Нечестивая порча'),
('50842','55095','1','0','0','1','55095','0','49509','0','0','0','0','0','0','-1','10','0','Нечестивая порча'),
('50842','155159','1','0','0','1','155159','0','49509','0','0','0','0','0','0','-1','0','0','Нечестивая порча');

DELETE FROM spell_proc_check WHERE entry IN (81141);
