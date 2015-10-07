ALTER TABLE `spell_aura_dummy`   
  ADD COLUMN `type` TINYINT(3) DEFAULT 0  NOT NULL AFTER `spellDummyId`;

UPDATE `spell_aura_dummy` SET `type` = 1 WHERE `option` IN (9,10);
UPDATE `spell_aura_dummy` SET `type` = 2 WHERE `option` IN (5,6,7);
UPDATE `spell_aura_dummy` SET `type` = 3 WHERE `option` IN (13,14);
UPDATE `spell_aura_dummy` SET `type` = 5 WHERE `option` IN (11,12);

insert ignore into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `comment`) values
('1776','108216','4','15','0','0','0','0','0','0','7','0','0','0','524288','0','Грязные приемы (Пассивный талант)'),
('2094','108216','4','15','0','0','0','0','0','0','7','0','0','0','524288','0','Грязные приемы (Пассивный талант)'),
('107079','108216','4','15','0','0','0','0','0','0','7','0','0','0','524288','0','Грязные приемы (Пассивный талант)');

