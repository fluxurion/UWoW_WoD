insert into `spell_aura_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) values
('77758','158723','0','0','0','0','0','0','0','7','0','0','Взбучка');

insert into `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) values
('159286','0','0','0','0','0','0','0','2','0','0','0','7');

insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('1613','0','1','0','2','13812','1','0','0','0','0','0','0','Взрывная ловушка'),
('1610','0','1','0','2','3355','1','0','0','0','0','0','0','Замораживающая ловушка'),
('1614','0','1','0','2','13810','1','0','0','0','0','0','0','Ледяная ловушка'),
('1615','0','1','0','2','45145','1','0','0','0','0','0','0','Змеиная ловушка');

DELETE FROM spell_bonus_data WHERE entry IN (22568);
DELETE FROM spell_bonus_data WHERE entry IN (33745);

DELETE FROM spell_linked_spell WHERE spell_trigger IN (135597);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('135597','-135286','0','0','0','0','0','0','0','0','0','0','0','7','Зубы и когти');

