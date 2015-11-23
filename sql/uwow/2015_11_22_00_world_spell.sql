insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('162536','-162537','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','ќсобенные боеприпасы'),
('162536','-162539','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','ќсобенные боеприпасы'),
('162537','-162539','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','ќсобенные боеприпасы'),
('162537','-162536','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','ќсобенные боеприпасы'),
('162539','-162537','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','ќсобенные боеприпасы'),
('162539','-162536','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','ќсобенные боеприпасы');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('123393','0','0','-123725','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Breath of Fire DoT shoudnt remove Breath of Fire disorientation');

insert into `spell_pet_auras` (`petEntry`, `spellId`, `option`, `target`, `targetaura`, `bp0`, `bp1`, `bp2`, `aura`, `casteraura`, `createdspell`, `fromspell`, `comment`) values
('-1','-164273','0','4','1','0','0','0','155228','0','0','0','ќдинокий волк');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('160198','spell_hun_lone_wolf');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('160199','spell_hun_lone_wolf');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('160200','spell_hun_lone_wolf');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('160203','spell_hun_lone_wolf');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('160205','spell_hun_lone_wolf');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('160206','spell_hun_lone_wolf');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('172967','spell_hun_lone_wolf');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('172968','spell_hun_lone_wolf');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('53260','0','0','3044','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Бросок кобры');

DELETE FROM spell_script_names WHERE spell_id IN (3044);

DELETE FROM spell_proc_event WHERE entry IN (53397);

DELETE FROM spell_bonus_data WHERE entry IN (83381);
insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('83381','-1','-1','1.632','-1','0','0','Hunter - Kill Command');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('53209','171454','1','0','0','0','0','0','0','0','0','0','0','0','0','-1','0','0','Выстрел химеры');

DELETE FROM spell_proc_event WHERE entry IN (35110);
insert into `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) values
('35110','0','0','0','0','0','0','0','2','0','0','0','7');
insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('35110','0','0','2643','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Ураганный обстрел');

DELETE FROM spell_proc_check WHERE entry IN (76659);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('76659','spell_hun_mastery_wild_quiver');
insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('-168809','168811','0','0','0','0','0','0','0','0','0','0','0','16','0','-1','0','0','Снайперская подготовка');
insert into `spell_talent_linked_spell` (`spellid`, `spelllink`, `type`, `target`, `caster`, `comment`) values
('-76659','-168809','0','0','0','скусность: снайперская подготовка'),
('-76659','-168811','0','0','0','скусность: снайперская подготовка');

DELETE FROM spell_bonus_data WHERE entry IN (53301);
insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('53301','0','0','0.47','0','0','0','Explosive Shot');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('2643','118253','1','0','0','2','87935','0','0','0','0','0','0','0','0','-1','0','0','Укус змеи'),
('3044','118253','1','0','0','2','87935','0','0','0','0','0','0','0','0','-1','0','0','Укус змеи');

DELETE FROM spell_script_names WHERE ScriptName IN ('spell_hun_serpent_spread');

insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('63900','0','0','0.25','0','0','0','Грохочущие шаги (Упорство)');

insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) values
('53490','53490','0','0','0','0','0','0','0','0','0','0','0','0','0','-1','9','0','Ослиное упрямство (Хитрость)');

DELETE FROM spell_trigger WHERE spell_id IN (118455);
insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('118455','118459','0','3','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Удар зверя');

insert into `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) values
('157724','0','0','19434','0','0','0','7','-1','-1','0','0','0','0','0','0','0','0','0','0','Усиленный прицельный выстрел');
insert into `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) values
('157724','0','0','0','0','0','0','0','2','0','0','0','7');

insert into `spell_aura_dummy` (`spellId`, `spellDummyId`, `type`, `option`, `target`, `caster`, `targetaura`, `aura`, `removeAura`, `effectDummy`, `effectmask`, `chance`, `attr`, `attrValue`, `custombp`, `specId`, `charge`, `comment`) values
('64803','157752','0','1','0','0','0','0','0','0','7','0','0','0','0','0','1','Усиленная западня');

insert into `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `targetaura2`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `aura2`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) values
('162537','170661','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','-1','0','0','0','Отравленные боеприпасы');
