insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('77758','158723','1','0','0','0','0','0','0','0','0','0','0','0','Взбучка');

insert into `spell_pet_auras` (`petEntry`, `spellId`, `option`, `target`, `targetaura`, `bp0`, `bp1`, `bp2`, `aura`, `casteraura`, `createdspell`, `fromspell`, `comment`) values
('47649','164717','0','2','0','0','0','0','0','0','88747','0','Дикий гриб Баланс'),
('47649','81262','0','0','0','0','0','0','0','0','145205','0','Дикий гриб Исцеление');
insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('2612','0','1','0','2','81281','0','0','0','0','0','0','0','Дикий гриб');

