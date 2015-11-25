DELETE FROM creature_template_addon WHERE entry IN (45322);
insert into `spell_pet_auras` (`petEntry`, `spellId`, `option`, `target`, `targetaura`, `bp0`, `bp1`, `bp2`, `aura`, `casteraura`, `createdspell`, `fromspell`, `comment`) values
('45322','84717','0','0','0','0','0','0','0','0','0','0','Ледяной шар'),
('45322','123605','0','0','0','0','0','0','0','0','0','0','Ледяной шар'),
('45322','82736','0','0','0','0','0','0','0','0','0','0','Ледяной шар'),
('45322','44544','0','4','0','0','0','0','0','0','0','0','Ледяные пальцы'),
('45322','126084','0','4','1','0','0','0','44544','0','0','0','Ледяные пальцы');
UPDATE creature_template SET speed_walk = 4, speed_run = 1.4285 WHERE entry IN (45322,56658);

DELETE FROM spell_linked_spell WHERE spell_trigger IN (33395);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('33395','44544','1','2','2','0','0','0','0','0','0','0','0','0','0','-1','0','0','Mage: use Freeze on cast Fingers of Frost');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('116','57761','0','0','0','0','44549','0','0','10','0','0','0','0','0','-1','0','0','Заморозка мозгов');

DELETE FROM spell_bonus_data WHERE entry IN (31661);

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('11129','108853','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','8','0','Возгорание');

insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('44614','44549','1','9','0','0','0','57761','0','2','7','0','0','0','0','0','1','Заморозка мозгов');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('5143','36032','0','0','0','0','79684','0','0','0','0','0','0','0','0','-1','0','0','Чародейские стрелы');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('1953','157610','1','0','0','0','157606','0','0','0','0','0','0','0','0','-1','0','0','Улучшенный скачок');

