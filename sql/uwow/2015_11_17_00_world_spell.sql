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

insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('124682','115175','3','13','0','0','0','115175','0','0','7','0','0','0','-100','0','1','Окутывающий туман'),
('157675','115175','3','13','0','0','0','115175','0','0','7','0','0','0','-100','0','1','Взрыв ци'),
('116694','115175','3','13','0','0','0','115175','0','0','7','0','0','0','-100','0','1','Благотворный туман');

DELETE FROM spell_script_names WHERE spell_id IN (124336, 115073, 123408);

insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('117418','0','0','7.755','0','0','0','Неистовые кулаки'),
('100780','0','0','1.38','0','0','0','Дзуки'),
('108557','0','0','1.38','0','0','0','Дзуки'),
('115698','0','0','1.38','0','0','0','Дзуки'),
('115687','0','0','1.38','0','0','0','Дзуки'),
('115693','0','0','1.38','0','0','0','Дзуки'),
('115695','0','0','1.38','0','0','0','Дзуки'),
('100787','0','0','3','0','0','0','Лапа тигра'),
('107270','0','0','0.9','0','0','0','Танцующий журавль'),
('148187','0','0','0.72','0','0','0','Порыв нефритового ветра'),
('107428','0','0','10.56','0','0','0','Удар восходящего солнца'),
('100784','0','0','6.4','0','0','0','Нокаутирующий удар'),
('121253','0','0','14.5','0','0','0','Удар бочонком'),
('115072','0','0','7.5','0','0','0','Устранение вреда'),
('147489','0','0','11.4','0','0','0','Устранение вреда');

