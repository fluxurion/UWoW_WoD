DELETE FROM spell_linked_spell WHERE spell_trigger IN (18562) AND spell_effect IN (81262);

insert into `spell_script_names` (`spell_id`, `ScriptName`) values
('88423','spell_dru_natures_cure');

insert into `spell_aura_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) values
('81262','81269','5','0','0','0','0','0','0','2','0','0','Дикий гриб');

insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('48438','31','0','0','0','0','62970','0','7','0','5','0','1','0','Wild Growth');

insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `comment`) values
('5215','-157274','0','4','0','0','0','0','0','0','7','0','0','0','0','0','Улучшенное подкрадывание');

DELETE FROM spell_linked_spell WHERE spell_trigger IN (137452);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('102280','768','6','0','0','0','0','0','0','0','0','0','0','0','Druid: talent Displacer Beast');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('50334','33917','0','0','0','0','0','0','0','0','0','0','0','8','Берсерк');

insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('774','114108','1','9','0','0','0','0','114108','1','7','0','0','0','0','0','1','Душа леса'),
('8936','114108','1','9','0','0','0','0','114108','1','7','0','0','0','0','0','1','Душа леса'),
('48438','114108','1','9','0','0','0','0','114108','2','7','0','0','0','0','0','1','Душа леса');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('102560','24858','6','0','0','0','0','0','0','0','0','0','0','0','Перевоплощение: Избранный Элуны');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('1822','163505','6','0','0','0','102547','0','0','0','0','0','0','0','Крадущийся зверь');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('8921','172176','6','0','0','0','171743','0','108373','0','0','0','0','0','Сон Кенария'),
('93402','172176','6','0','0','0','171744','0','108373','0','0','0','0','0','Сон Кенария');
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('172176','31','0','0','0','0','0','0','7','0','1','0','0','0','Сон Кенария');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('158504','0','0','5176','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Сон Кенария');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('158504','145153','0','3','0','0','0','0','0','0','7','0','0','0','0','0','0','-1','0','0','0','Сон Кенария');
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('145153','31','0','0','0','0','0','0','7','0','1','0','0','0','Сон Кенария');

