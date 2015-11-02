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
