delete from areatrigger_teleport where id in (10022,10023);
INSERT INTO `areatrigger_teleport` (`id`, `name`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(10022, 'ShadowmoonBurialGrounds (exit)', 1116, 763.88, 128.89, 7.53, 5.5),
(10023, 'ShadowmoonBurialGrounds (enter)', 1176, 1716.09, 245.09, 325.97, 5.29);

delete from instance_template where map = 1176;
INSERT INTO `instance_template` (`map`, `parent`, `script`, `allowMount`, `bonusChance`) VALUES
(1176, 0, 'instance_shadowmoon_burial_grounds', 0, 0);

update creature_template set modelid2 = 0, ScriptName = 'npc_sadana_shadow_rune', flags_extra = 128 where entry = 75778;

delete from areatrigger_actions where entry in (1536,1537);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(1536, 0, 1, 0, 4096, 152696, 0, 0, 0, 0, 0, 0, 0, 'Shadow Rune - Apply'),
(1536, 1, 42, 1, 4096, 152696, 0, 0, 0, 0, 0, 0, 0, 'Shadow Rune - Remove'),
(1537, 0, 1, 0, 4096, 152690, 0, 0, 0, 0, 0, 0, 0, 'Shadow Rune 2 - Apply'),
(1537, 1, 42, 1, 4096, 152690, 0, 0, 0, 0, 0, 0, 0, 'Shadow Rune 2 - Remove');
/* --------------------------------------------
    Садана Кровавое Безумие / Sadana Bloodfury
   -------------------------------------------- */
delete from creature where id in (75966, 76052);
update creature set spawntimesecs = 86400 where id = 75509;
update creature_template set rank = 3, type_flags = type_flags | 4, mechanic_immune_mask = 617299967, ScriptName = 'boss_sadana_bloodfury', flags_extra = 1 where entry = 75509;
update creature_template set minlevel = 102, maxlevel = 102, flags_extra = 128, ScriptName = 'npc_sadana_daggerfall' where entry = 75981;
update creature_template set ScriptName = 'npc_sadana_defiled_spirit' where entry = 75966;
update creature_template set flags_extra = 130 where entry = 76052;
update creature_template_addon set bytes1 = 0 where entry = 76052;
update creature_template_addon set auras = 'NULL' where entry = 75509;

delete from areatrigger_actions where entry in (2404);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(2404, 0, 1, 0, 2, 162652, 0, 0, 0, 0, 0, 0, 0, 'Sadana - Lunar Purity Apply'),
(2404, 1, 42, 1, 2, 162652, 0, 0, 0, 0, 0, 0, 0, 'Sadana - Lunar Purity Remove');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (153153,153164,164685,164705);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 153153, 0, 0, 31, 0, 3, 75966, 0, 0, 0, '', NULL),
(13, 7, 153164, 0, 0, 31, 0, 3, 75509, 0, 0, 0, '', NULL),
(13, 1, 164685, 0, 0, 31, 0, 3, 76052, 0, 0, 0, '', NULL),
(13, 1, 164705, 0, 0, 31, 0, 3, 75778, 0, 0, 0, '', NULL);

delete from spell_trigger_dummy where spell_id in (153240); 
INSERT INTO `spell_trigger_dummy` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `check_spell_id`, `comment`) VALUES 
(153240, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Sadana - Daggerfall');

delete from spell_script_names where spell_id in (153200,153240,153153);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(153200, 'spell_sadana_daggerfall'),
(153240, 'spell_sadana_daggerfall'),
(153153, 'spell_sadana_dark_communion');

delete from spell_target_position where id in (155689,164704);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(155689, 1176, 1795.9, -26.73, 261.4, 1.57),
(164704, 1176, 1797.3, -26.51, 261.4, 4.76);

delete from creature_summon_groups where summonerId = 75509;
INSERT INTO `creature_summon_groups` (`summonerId`, `id`, `summonerType`, `groupId`, `entry`, `position_x`, `position_y`, `position_z`, `orientation`, `count`, `actionType`, `distance`, `summonType`, `summonTime`) VALUES 
(75509, 0, 0, 0, 75778, 1774.14, -29.3906, 261.392, 3.22, 0, 0, 0, 8, 0),
(75509, 1, 0, 0, 75778, 1774.67, -24.2257, 261.392, 3.06, 0, 0, 0, 8, 0),
(75509, 2, 0, 0, 75778, 1775.98, -18.7118, 261.392, 2.74, 0, 0, 0, 8, 0),
(75509, 3, 0, 0, 75778, 1776.1 , -34.8524, 261.392, 3.53, 0, 0, 0, 8, 0),
(75509, 4, 0, 0, 75778, 1778.46, -13.4722, 261.392, 2.43, 0, 0, 0, 8, 0),
(75509, 5, 0, 0, 75778, 1779.17, -39.6632, 261.392, 3.80, 0, 0, 0, 8, 0),
(75509, 6, 0, 0, 75778, 1782.28, -43.8195, 261.392, 3.96, 0, 0, 0, 8, 0),
(75509, 7, 0, 0, 75778, 1782.5 , -10.2066, 261.392, 2.27, 0, 0, 0, 8, 0),
(75509, 8, 0, 0, 75778, 1787.39, -6.89931, 261.392, 1.94, 0, 0, 0, 8, 0),
(75509, 9, 0, 0, 75778, 1787.42, -46.507 , 261.392, 4.27, 0, 0, 0, 8, 0),
(75509, 10, 0, 0, 75778, 1792.88, -5.03299, 261.392, 1.63, 0, 0, 0, 8, 0),
(75509, 11, 0, 0, 75778, 1793.36, -47.9705, 261.392, 4.65, 0, 0, 0, 8, 0),
(75509, 12, 0, 0, 75778, 1798.03, -5.65278, 261.392, 1.47, 0, 0, 0, 8, 0),
(75509, 13, 0, 0, 75778, 1798.54, -48.3958, 261.392, 4.81, 0, 0, 0, 8, 0),
(75509, 14, 0, 0, 75778, 1803.82, -6.95486, 261.392, 1.16, 0, 0, 0, 8, 0),
(75509, 15, 0, 0, 75778, 1803.95, -46.3351, 261.392, 5.12, 0, 0, 0, 8, 0),
(75509, 16, 0, 0, 75778, 1809.04, -9.46875, 261.392, 0.85, 0, 0, 0, 8, 0),
(75509, 17, 0, 0, 75778, 1809.07, -43.3333, 261.392, 5.43, 0, 0, 0, 8, 0),
(75509, 18, 0, 0, 75778, 1812.29, -13.526 , 261.392, 0.69, 0, 0, 0, 8, 0),
(75509, 19, 0, 0, 75778, 1813.04, -39.9844, 261.392, 5.59, 0, 0, 0, 8, 0),
(75509, 20, 0, 0, 75778, 1815.42, -34.6858, 261.392, 5.90, 0, 0, 0, 8, 0),
(75509, 21, 0, 0, 75778, 1815.57, -18.4566, 261.392, 0.37, 0, 0, 0, 8, 0),
(75509, 22, 0, 0, 75778, 1816.87, -29.1024, 261.392, 6.19, 0, 0, 0, 8, 0),
(75509, 23, 0, 0, 75778, 1817.47, -23.9444, 261.392, 0.06, 0, 0, 0, 8, 0);

delete from creature_text where entry = 75509;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(75509, 0, 0, 'Rivers of the damned, RISE! RISE in the name of darkness, RISE in the name of Ner\'zhul! The Shadowmoon beckons...', 14, 0, 100, 0, 0, 43541, 'Садана Кровавое Безумие'),
(75509, 1, 0, 'Ancestors of the Shadowmoon, shine in the darkness! Lend me your strength!', 14, 0, 100, 0, 0, 43539, 'Садана Кровавое Безумие'),
(75509, 2, 0, 'The dead... Shall rise again...', 14, 0, 100, 0, 0, 43540, 'Садана Кровавое Безумие'),
(75509, 3, 0, 'Another spirit for the army!', 14, 0, 100, 0, 0, 43542, 'Садана Кровавое Безумие'),
(75509, 3, 1, 'Embrace the eternal darkness...', 14, 0, 100, 0, 0, 43543, 'Садана Кровавое Безумие'),
(75509, 4, 0, 'Mmm, yes. The PERFECT sacrifice.', 14, 0, 100, 0, 0, 43545, 'Садана Кровавое Безумие'),
(75509, 4, 1, 'Restless spirits, HEED my command!', 14, 0, 100, 0, 0, 43547, 'Садана Кровавое Безумие'),
(75509, 5, 0, 'Your souls will belong to ME.', 14, 0, 100, 0, 0, 43546, 'Садана Кровавое Безумие'),
(75509, 5, 1, 'For Ner\'zhul...', 14, 0, 100, 0, 0, 43544, 'Садана Кровавое Безумие');

delete from locales_creature_text where entry = 75509;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES 
(75509, 0, 0, 'Поднимайтесь, воды проклятых рек! Затопите все во имя Тьмы и Нер''зула! Призрачная Луна зовет...'),
(75509, 1, 0, 'Предки клана Призрачной Луны, придите сквозь тьму! Дайте мне силу!'),
(75509, 2, 0, 'Мертвые... восстанут вновь...'),
(75509, 3, 0, 'Этот призрак пополнит мою армию!'),
(75509, 3, 1, 'Растворись в вечном мраке...'),
(75509, 4, 0, 'О да... Идеальная жертва.'),
(75509, 4, 1, 'Беспокойные духи, внемлите!'),
(75509, 5, 0, 'Я заберу ваши души.'),
(75509, 5, 1, 'За Нер\'зула...');

/* ----------------------------------
            Nhallish / Нхаллиш
   ---------------------------------- */
delete from creature where id in (75977);
update creature set PhaseId = '173 174 2439 2440 2441 4315' where map = 1176;
update gameobject set PhaseId = '173 174 2439 2440 2441 4315' where map = 1176;
update creature set spawntimesecs = 86400 where id = 75829;
update creature_template set rank = 3, type_flags = type_flags | 4, mechanic_immune_mask = 617297919, ScriptName = 'boss_nhallish', flags_extra = 1 where entry = 75829;
update creature_template set faction = 14, npcflag = 16777216, ScriptName = 'npc_nhallish_possessed_soul' where entry = 75899;
update creature_template set modelid1 = 59486, modelid2 = 0, modelid3 = 0, ScriptName = 'npc_nhallish_defiled_burial_site' where entry = 75977;
update creature_template set faction = 14, AIName = 'SmartAI' where entry = 75979;

update areatrigger_data set windspeed = 7, windtype = 1 where entry = 1562;
delete from areatrigger_actions where entry in (1599,1570);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(1599, 0, 1, 0, 4096, 153501, 0, 0, 0, 0, 0, 0, 0, 'Nhallish - Void Blast apply'),
(1599, 1, 42, 1, 4096, 153501, 0, 0, 0, 0, 0, 0, 0, 'Nhallish - Void Blast remove'),
(1570, 0, 1, 0, 4096, 153070, 0, 0, 0, 0, 0, 0, 0, 'Nhallish - Void Devastation apply'),
(1570, 1, 42, 1, 4096, 153070, 0, 0, 0, 0, 0, 0, 0, 'Nhallish - Void Devastation remove');

delete from areatrigger_polygon where entry = 6084;
INSERT INTO `areatrigger_polygon` (`entry`, `spellId`, `id`, `x`, `y`) VALUES 
(6084, 152792, 0, 0, 1),
(6084, 152792, 1, 0, -1),
(6084, 152792, 2, 15, -3),
(6084, 152792, 3, 15, 3);

delete from phase_definitions where zoneId = 6932;
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `PreloadMapID`, `VisibleMapID`, `flags`, `comment`) VALUES 
(6932, 1, 0, '4315', 0, 0, 16, NULL);

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (158372,158379,154925);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 158372, 0, 0, 31, 0, 3, 75977, 0, 0, 0, '', NULL),
(13, 1, 158372, 0, 1, 31, 0, 3, 75979, 0, 0, 0, '', NULL),
(13, 1, 158379, 0, 0, 31, 0, 3, 75977, 0, 0, 0, '', NULL),
(13, 1, 158379, 0, 1, 31, 0, 3, 75979, 0, 0, 0, '', NULL),
(13, 1, 154925, 0, 0, 31, 0, 3, 75899, 0, 0, 0, '', NULL);

delete from conditions where SourceTypeOrReferenceId = 23 and SourceGroup = 6932;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(23, 6932, 1, 0, 0, 1, 0, 155005, 0, 0, 1, 0, '', NULL),
(23, 6932, 1, 0, 0, 1, 0, 155006, 0, 0, 1, 0, '', NULL),
(23, 6932, 1, 0, 0, 1, 0, 155007, 0, 0, 1, 0, '', NULL),
(23, 6932, 1, 0, 0, 1, 0, 155009, 0, 0, 1, 0, '', NULL),
(23, 6932, 1, 0, 0, 1, 0, 155010, 0, 0, 1, 0, '', NULL);

delete from spell_trigger_dummy where spell_id in (153623);
INSERT INTO `spell_trigger_dummy` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `check_spell_id`, `comment`) VALUES 
(153623, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Nhallish - Planar Shift');

delete from spell_script_names where spell_id in (153493,152962);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(153493, 'spell_nhallish_mirror_image'),
(152962, 'spell_nhallish_soul_summon');

delete from npc_spellclick_spells where npc_entry = 75899;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES 
(75899, 154925, 1, 0);

delete from spell_area where spell in (155005,155006,155007,155009,155010);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES 
(155005, 6932, 0, 0, 0, 0, 2, 0, 64, 11),
(155006, 6932, 0, 0, 0, 0, 2, 0, 64, 11),
(155007, 6932, 0, 0, 0, 0, 2, 0, 64, 11),
(155009, 6932, 0, 0, 0, 0, 2, 0, 64, 11),
(155010, 6932, 0, 0, 0, 0, 2, 0, 64, 11);

delete from smart_scripts where entryorguid = 75979;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(75979, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 18, 50, 0, 0, 0, 0, 0, 0, 'Just Summon - Attack Start');

delete from creature_summon_groups where summonerId = 75829;
INSERT INTO `creature_summon_groups` (`summonerId`, `id`, `summonerType`, `groupId`, `entry`, `position_x`, `position_y`, `position_z`, `orientation`, `count`, `actionType`, `distance`, `summonType`, `summonTime`) VALUES 
(75829, 0, 0, 0, 75977, 1800.28, -173.98, 260.68, 3.22, 0, 0, 0, 8, 0),
(75829, 1, 0, 0, 75977, 1781.14, -170.81, 259.83, 3.06, 0, 0, 0, 8, 0),
(75829, 2, 0, 0, 75977, 1736.02, -188.08, 253.63, 2.74, 0, 0, 0, 8, 0),
(75829, 3, 0, 0, 75977, 1769.47, -196.93, 253.86, 3.53, 0, 0, 0, 8, 0),
(75829, 4, 0, 0, 75977, 1768.43, -189.25, 254.03, 2.43, 0, 0, 0, 8, 0),
(75829, 5, 0, 0, 75977, 1721.32, -177.72, 253.41, 3.80, 0, 0, 0, 8, 0),
(75829, 6, 0, 0, 75977, 1708.15, -173.75, 253.41, 3.96, 0, 0, 0, 8, 0),
(75829, 7, 0, 0, 75977, 1730.45, -204.87, 252.60, 2.27, 0, 0, 0, 8, 0),
(75829, 8, 0, 0, 75977, 1735.72, -197.14, 252.98, 1.94, 0, 0, 0, 8, 0),
(75829, 9, 0, 0, 75977, 1721.65, -215.5 , 252.04, 4.27, 0, 0, 0, 8, 0),
(75829, 10, 0, 0, 75977, 1693.51, -181.69, 253.41, 1.63, 0, 0, 0, 8, 0),
(75829, 11, 0, 0, 75977, 1693.19, -236.86, 250.69, 4.65, 0, 0, 0, 8, 0),
(75829, 12, 0, 0, 75977, 1674.6 , -210.30, 251.95, 1.47, 0, 0, 0, 8, 0),
(75829, 13, 0, 0, 75977, 1676.5 , -195.59, 252.67, 4.81, 0, 0, 0, 8, 0),
(75829, 14, 0, 0, 75977, 1674.18, -229.43, 251.58, 1.16, 0, 0, 0, 8, 0);

delete from creature_text where entry = 75829;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(75829, 0, 0, 'Yes, come closer... your souls are strong and proud! I will feast on your terror!', 14, 0, 100, 0, 0, 43197, 'Нхаллиш'), -- aggro
(75829, 1, 0, 'You are just maggots swarming to the corpse of a dying god.', 14, 0, 100, 0, 0, 43198, 'Нхаллиш'), -- death
(75829, 2, 0, 'Cower before me!', 14, 0, 100, 0, 0, 43199, 'Нхаллиш'), -- intro_1
(75829, 3, 0, 'Your undying souls will fuel my ascension from the darkness!', 14, 0, 100, 0, 0, 43200, 'Нхаллиш'), -- intro_2
(75829, 4, 0, 'Your blissful afterlife is over.', 14, 0, 100, 0, 0, 43201, 'Нхаллиш'), -- intro_3
(75829, 5, 0, 'Your mortal bodies are so fragile.', 14, 0, 100, 0, 0, 43202, 'Нхаллиш'), -- kill_1
(75829, 5, 1, 'Another soul to devour.', 14, 0, 100, 0, 0, 43203, 'Нхаллиш'), -- kill_2
(75829, 6, 0, 'None escape the Void!', 14, 0, 100, 0, 0, 43204, 'Нхаллиш'), -- spell_1
(75829, 7, 0, 'You cannot flee.', 14, 0, 100, 0, 0, 43205, 'Нхаллиш'), -- spell_2
(75829, 8, 0, 'Flee! Flee from your former masters!', 14, 0, 100, 0, 0, 43208, 'Нхаллиш'); -- spell_5

delete from locales_creature_text where entry = 75829;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES 
(75829, 0, 0, 'Да... Ближе... Сильные, гордые души, ваш ужас надолго насытит меня!'),
(75829, 1, 0, 'Вы черви, кишащие в теле умирающего бога.'),
(75829, 2, 0, 'Падите ниц предо мной!'),
(75829, 3, 0, 'Я вернусь из мрака, поглотив ваши бессмертные души.'),
(75829, 4, 0, 'Конец вашему блаженству после смерти...'),
(75829, 5, 0, 'Тела смертных такие хрупкие.'),
(75829, 5, 1, 'Я поглощу эту душу.'),
(75829, 6, 0, 'Бездна ждет!'),
(75829, 7, 0, 'Тебе не уйти...'),
(75829, 8, 0, 'Бегите от прежних господ!');

/* ----------------------------------
            Bonemaw / Костебрюх
   ---------------------------------- */
delete from creature where id in (81881,76250,76057);
update creature set spawntimesecs = 86400 where id = 75452;
update creature_template set inhabittype = 7 where entry in (75459,75452,77676,76057);
update creature_template set rank = 3, type_flags = type_flags | 4, mechanic_immune_mask = 617297919, ScriptName = 'boss_bonemaw', flags_extra = 1 where entry = 75452;
update creature_template set flags_extra = 130 where entry in (66508,76250,81881);
update creature_template set faction = 35 where entry = 66508;
update creature_template set ScriptName = 'npc_bonemaw_carrion_worm' where entry = 76057;

delete from areatrigger_actions where entry in (1616);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(1616, 0, 1, 0, 4096, 153692, 0, 0, 0, 0, 0, 0, 0, 'Bonemaw - Necrotic Pitch apply'),
(1616, 1, 42, 1, 4096, 153692, 0, 0, 0, 0, 0, 0, 0, 'Bonemaw - Necrotic Pitch remove');

update areatrigger_data set radius = 30, radiustarget = 30, windspeed = 7, windtype = 1 where entry = 1650;

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (153713, 164848, 156663);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 2, 153713, 0, 0, 31, 0, 3, 77006, 0, 0, 0, '', NULL),
(13, 1, 164848, 0, 0, 31, 0, 3, 81881, 0, 0, 0, '', NULL),
(13, 1, 156663, 0, 0, 31, 0, 3, 66508, 0, 0, 0, '', NULL);

delete from spell_trigger_dummy where spell_id in (153691);
INSERT INTO `spell_trigger_dummy` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `check_spell_id`, `comment`) VALUES 
(153691, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Bonemaw - Necrotic Pitch');

delete from spell_target_position where id in (153713,154013);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(153713, 1176, 1835.73, -522.978, 214.65, 4.4),
(154013, 1176, 1873.05, -503.27, 195.79, 3.6);

delete from npc_spellclick_spells where npc_entry = 75452;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES 
(75452, 46598, 1, 0);

delete from vehicle_template_accessory where EntryOrAura = 75452;
INSERT INTO `vehicle_template_accessory` (`EntryOrAura`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES 
(75452, 81881, 5, 1, '75452 - 81881', 8, 0);

delete from creature_text where entry = 75452;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(75452, 0, 0, '%s начинает |cFFFF0404|Hspell:153804|h[засасывать]|h|r своих врагов!', 41, 0, 100, 0, 0, 0, 'Костебрюх'),
(75452, 1, 0, '%s: сила вдоха увеличивается!', 16, 0, 100, 0, 0, 0, 'Костебрюх'),
(75452, 2, 0, '%s привлекает ближайших червей-трупоедов своим пронзительным визгом!', 41, 0, 100, 0, 0, 0, 'Костебрюх'),
(75452, 3, 0, '%s шипит и уползает обратно в темные глубины!', 41, 0, 100, 0, 0, 0, 'Костебрюх');

/* ----------------------------------
            Ner'zhul / Нер'зул
   ---------------------------------- */
delete from creature where id in (79497,85541,76518);
update creature_template set inhabittype = 7 where entry in (79497,64367,89003,85541);
update creature_template set rank = 3, type_flags = type_flags | 4, mechanic_immune_mask = 617297919, ScriptName = 'boss_nerzhul', flags_extra = 1 where entry = 76407;
update creature_template set ScriptName = 'npc_shadowmoon_nerzhul_intro' where entry = 79497;
update creature_template set AiId = 0, ScriptName = 'npc_nerzhul_ritual_bones' where entry = 76518;
update creature_template set flags_extra = 130 where entry in (64367,76514);
update creature_template set minlevel = 102, maxlevel = 102, faction = 16, flags_extra = 128 where entry = 76462;

update areatrigger_data set radius = 5, radiusTarget = 5 where entry = 1670;
delete from areatrigger_actions where entry = 1670;
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(1670, 0, 1, 0, 4096, 154468, 0, 0, 0, 0, 0, 0, 0, 'Nerzhul - Ritual of Bones Apply');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (156312);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 156312, 0, 0, 31, 0, 3, 76518, 0, 0, 0, '', NULL);

delete from creature_summon_groups where id = 76407;
INSERT INTO `creature_summon_groups` (`summonerId`, `id`, `summonerType`, `groupId`, `entry`, `position_x`, `position_y`, `position_z`, `orientation`, `count`, `actionType`, `distance`, `summonType`, `summonTime`) VALUES 
(76407, 0, 0, 0, 79497, 1711.66, -822.693, 229.825, 1.20656, 0, 0, 0, 8, 0);

delete from spell_script_names where spell_id = 154353;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(154353, 'spell_omen_of_death');

delete from spell_trigger_dummy where spell_id in (177691);
INSERT INTO `spell_trigger_dummy` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `check_spell_id`, `comment`) VALUES 
(177691, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Nerzhul - Omen of Death');

delete from spell_target_position where id in (178093,170790);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(178093, 1176, 1729.28, -796.20, 137.30, 4.1),
(170790, 1176, 1795.04, 143.40, 295, 4.7);

delete from npc_spellclick_spells where npc_entry = 85541;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES 
(85541, 170790, 1, 0);

delete from creature_text where entry = 76407;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(76407, 0, 0, 'The Shadowmoon CANNOT be defeated - we walk in the realm of death itself.', 14, 0, 100, 0, 0, 43682, 'Нерзул'),
(76407, 1, 0, 'A kindling of bone and shadow! Army of the dead, arise!', 14, 0, 100, 0, 0, 43689, 'Нерзул'),
(76407, 2, 0, 'My power... Will ripple... Across eternity.', 14, 0, 100, 0, 0, 43683, 'Нерзул');

delete from locales_creature_text where entry = 76407;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES 
(76407, 0, 0, 'Воинов Призрачной Луны не победить – мы живем в царстве смерти.'),
(76407, 1, 0, 'Создания из костей и тьмы! Восстаньте, мертвые воители!'),
(76407, 2, 0, 'Моя сила... отзовется... в вечности.');




