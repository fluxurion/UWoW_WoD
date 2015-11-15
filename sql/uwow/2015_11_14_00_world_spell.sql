insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('20271','87173','0','0','0','0','87172','0','0','0','0','0','0','0','0','-1','0','0','Длинная рука закона');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('114163','156322','1','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Вечное пламя');

DELETE FROM spell_proc_check WHERE entry IN (155159);
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('155159','0','0','0','0','0','7','2','-1','-1','250','0','0','0','0','0','0','0','0','0','Мертвящая чума');

DELETE FROM spell_proc_event WHERE entry IN (53376);
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('31842','114232','0','0','0','0','53376','0','112859','0','0','0','0','0','0','-1','0','0','Священный гнев');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('119072','141462','0','0','0','0','171648','0','0','0','0','0','0','0','0','1','0','0','Священный гнев');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('76672','0','0','157048','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Искусность: длань Света');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('152262','172320','0','0','0','0','165375','0','0','0','0','0','0','0','0','-1','0','0','Серафим - Защита'),
('152262','172321','0','0','0','0','53503','0','0','0','0','0','0','0','0','-1','0','0','Серафим - Воздаяние');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('157128','157131','1','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Недавно спасенный Светом');
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('157128','spell_pal_saved_by_the_light');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('157007','0','0','0','0','0','0','2','-1','-1','0','0','0','0','0','90','0','0','0','0','Частица озарения');
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('157007','157026','0','20','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Частица озарения');
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('157026','31','0','0','0','0','0','0','7','1','1','0','0','-1','Частица озарения');
insert into `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) values
('157026','157044','6','0','10','0','0','0','0','2','0','0','Частица озарения');
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('157044','spell_pal_beacon_of_insight');

insert into `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) values
('152261','0','0','0','0','0','0','0','64','0','0','0','7');

insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('121129','157455','1','9','0','0','0','0','0','0','7','0','0','0','0','0','1','Улучшенный рассвет');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('76672','0','0','158392','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Искусность: длань Света');
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('158392','138248','1','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Усиленный молот гнева ');

DELETE FROM spell_target_filter WHERE spellId IN (85222);
insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('85222','31','0','0','0','0','54940','0','7','1','6','0','-2','0','Свет зари и символ Символ света зари');

DELETE FROM spell_proc_check WHERE entry IN (53563);
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('53563','0','0','19750','0','0','0','1','-1','-1','0','0','0','0','0','0','0','0','0','0','Частица Света'),
('53563','0','0','82326','0','0','0','1','-1','-1','0','0','0','0','0','0','0','0','0','0','Частица Света'),
('53563','0','0','0','0','0','0','4','-1','-1','0','0','0','0','0','-30','0','0','0','0','Спасенный Светом');
DELETE FROM spell_trigger WHERE spell_id IN (53563);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('53563','88852','0','2','7','1','0','0','40','0','0','1','0','0','0','0','0','0','0','-1','0','0','0','Частица Света'),
('53563','157128','0','20','1','1','1','1','0','0','0','4','157047','-157131','0','0','0','0','0','-1','0','0','0','Спасенный Светом');

DELETE FROM spell_script_names WHERE spell_id IN (156910,53563);
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('156910','spell_pal_beacon_of_light');
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('53563','spell_pal_beacon_of_light');
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('177168','spell_pal_beacon_of_light');

insert into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('121129','31','0','0','0','0','0','0','7','1','6','0','0','0','Рассвет');

DELETE FROM spell_linked_spell WHERE spell_trigger IN (53563);
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('53651','spell_pal_beacon_of_light_proc');


