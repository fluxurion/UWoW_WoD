insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('5185','145152','0','0','0','0','155672','0','0','0','0','0','0','0','Кровавые когти');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('80313','158792','0','0','0','0','0','0','0','0','0','0','0','0','Раздавить'),
('80313','-158790','1','0','0','0','0','0','0','0','0','0','0','0','Растерзать'),
('80313','-33745','1','0','0','0','0','0','0','0','0','0','0','0','Растерзать');

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('33745','spell_dru_lacerate');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('774','155777','6','0','5','1','774','0','155675','0','0','0','0','0','Зарождение');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('18562','-774','1','0','0','0','155834','0','0','0','0','0','0','0','Обильный рост'),
('18562','-8936','1','0','0','0','155834','0','0','0','0','0','0','0','Обильный рост');

replace into `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) values
('145153','31','0','0','0','0','0','0','7','0','1','0','0','2','Сон Кенария'),
('172176','31','0','0','0','0','0','0','7','0','1','0','0','2','Сон Кенария');

-- Fix crash
DELETE FROM creature_formations WHERE leaderGUID IN (SELECT guid FROM creature WHERE id IN (59895));
DELETE FROM creature_formations WHERE memberGUID IN (SELECT guid FROM creature WHERE id IN (59808));

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('76669','0','0','-53652','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Частица Света - фризит сервер похоже'),
('53651','0','0','-86273','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Озаряющее исцеление - фризит сервер похоже');
