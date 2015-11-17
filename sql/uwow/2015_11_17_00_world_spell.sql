DELETE FROM areatrigger_actions WHERE entry IN (373);
insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('373','0','5','0','132','124507','1','0','0','0','0','0','0','Дар быка');

UPDATE creature_template SET unit_flags = 0, unit_flags2 = 0 WHERE entry IN (61146);

insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('116095','116095','0','5','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Вывести из строя');

DELETE FROM spell_proc_event WHERE entry IN (46915);
insert into `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) values
('46915','0','4','0','0','0','0','16','1027','0','20','0','1');

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('115460','spell_monk_detonate_chi');

DELETE FROM spell_script_names WHERE spell_id IN (107270, 148187);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('107270','117640','0','0','0','0','116645','0','0','0','0','0','0','0','0','-1','0','0','Монастырские знания'),
('148187','117640','0','0','0','0','116645','0','0','0','0','0','0','0','0','-1','0','0','Монастырские знания');

DELETE FROM spell_trigger WHERE spell_id IN (130324);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('130324','117895','0','3','0','0','1','0','0','0','0','7','115070','0','0','0','332112','0','0','-1','0','115313','2','Величие (статуя)'),
('130324','117895','0','3','0','0','1','0','0','0','0','7','154436','0','0','0','4','0','0','-1','0','115313','2','Величие (статуя) - Монастырские знания');

insert into `spell_visual` (`spellId`, `SpellVisualID`, `TravelSpeed`, `MissReason`, `ReflectStatus`, `SpeedAsTime`, `type`, `temp`, `comment`) values
('115175','24208','15','0','0','0','1','0',' Успокаивающий туман');
