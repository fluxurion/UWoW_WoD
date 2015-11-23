delete from instance_template where map = 1279;
INSERT INTO `instance_template` (`map`, `parent`, `script`, `allowMount`, `bonusChance`) VALUES 
(1279, 0, 'instance_the_everbloom', 0, 0);

delete from areatrigger_teleport where id in (10244,10285);
INSERT INTO `areatrigger_teleport` (`id`, `name`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(10244, 'TheEverbloom (enter)', 1279, 454.16, 1351.08, 117.5, 0.65),
(10285, 'TheEverbloom (exit)', 1116, 7117.57, 197.1, 145.211, 0.3);

/* --------------------------------------
        Witherbark  /  Сухокожий
   -------------------------------------- */
delete from creature where id in (73400, 81821) and map = 1279;
delete from gameobject where id in (231968, 231970);
update creature set spawntimesecs = 86400 where id = 81522;
update creature_template set rank = 3, unit_flags = unit_flags &~ 256, mechanic_immune_mask = 617299967, ScriptName = 'boss_witherbark', flags_extra = 1 where entry = 81522;
update creature_template set ScriptName = 'npc_witherbark_aqueous_globule', flags_extra = 128 where entry = 81821;
update creature_template set inhabitType = 7, flags_extra = 130 where entry = 88862;
update creature_template set faction = 16, speed_fly = 0.6, exp = 5, inhabitType = 7, ScriptName = 'npc_witherbark_aqueous_globule_follower', mechanic_immune_mask = 617299967 where entry = 81638;
update creature_template set minlevel = 102, maxlevel = 102, faction = 16, exp = 5, unit_flags = 33587200, ScriptName = 'npc_witherbark_unchecked_growth' where entry = 81564;
update creature_template set faction = 16, exp = 5, AIName = 'SmartAI' where entry = 81737;

update creature_template_addon set auras = '' where entry = 81522;
delete from creature_addon where guid in (select guid from creature where id in (81522, 88862));

delete from creature_template_addon where entry = 88862;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(88862, 0, 0, 50331648, 1, 0, '177722');

delete from smart_scripts where entryorguid = 81737;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(81737, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 38, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Just Summon - Aggro'),
(81737, 0, 1, 0, 7, 0, 100, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evade - Despawn');

delete from areatrigger_actions where entry = 2571;
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(2571, 0, 1, 0, 4096, 164294, 0, 0, 0, 0, 0, 0, 0, 'Unchecked Growth Enter'),
(2571, 1, 42, 1, 4096, 164294, 0, 0, 0, 0, 0, 0, 0, 'Unchecked Growth Remove');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (164438,177733);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 164438, 0, 0, 31, 0, 3, 81522, 0, 0, 0, '', NULL),
(13, 1, 177733, 0, 0, 31, 0, 3, 88862, 0, 0, 0, '', NULL);

delete from spell_dummy_trigger where spell_id in (177732);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(177732, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Witherbark - Agitated Water');

delete from creature_text where entry = 81522;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(81522, 0, 0, 'So. Thirsty.', 14, 0, 100, 0, 0, 45108, 'Сухокожий to 0'),
(81522, 1, 0, '|TInterface\\Icons\\inv_misc_embers.blp:20|t %s полностью обезвоживается, что вызывает эффект |cFFFF0404|Hspell:164275|h[Хрупкая кора]|h|r!', 41, 0, 100, 0, 0, 0, 'Сухокожий to 0'),
(81522, 2, 0, 'Refreshed.', 14, 0, 100, 0, 0, 45112, 'Сухокожий to 0'),
(81522, 2, 1, 'I. Cannot. Be. Stopped.', 14, 0, 100, 0, 0, 45109, 'Сухокожий to Player'),
(81522, 3, 0, '%s поглотил достаточно воды и снова ожил!', 41, 0, 100, 0, 0, 0, 'Сухокожий to 0'),
(81522, 4, 0, 'No!', 14, 0, 100, 0, 0, 45109, 'Сухокожий to Player'),
(81522, 5, 0, 'Returned to the earth.', 14, 0, 100, 0, 0, 45110, 'Сухокожий to Player'),
(81522, 5, 1, 'Feed my roots.', 14, 0, 100, 0, 0, 45111, 'Сухокожий to Player');

delete from locales_creature_text where entry = 81522;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES
(81522, 0, 0, 'Хочу. Пить.'),
(81522, 2, 0, 'Так лучше.'),
(81522, 2, 1, 'Меня. Не. Остановить.'),
(81522, 4, 0, 'Нет!'),
(81522, 5, 0, 'Назад, к земле…'),
(81522, 5, 1, 'Питай мои корни…');

/* ------------------------------------------
    Ancient Protectors  /  Древние защитники
   ------------------------------------------ */
update creature set spawntimesecs = 86400 where id in (83892,83893,83894);
update creature_template set rank = 3, mechanic_immune_mask = 617299967, ScriptName = 'boss_life_warden_gola', flags_extra = 1 where entry = 83892;
update creature_template set rank = 3, mechanic_immune_mask = 617299967, ScriptName = 'boss_earthshaper_telu', flags_extra = 1 where entry = 83893;
update creature_template set rank = 3, mechanic_immune_mask = 617299967, ScriptName = 'boss_dulhu', flags_extra = 1 where entry = 83894;
update creature_template set minlevel = 102, maxlevel = 120, faction = 14, unit_flags = 33555200, AIName = 'SmartAI', flags_extra = 128 where entry = 84008;
update creature_template_addon set auras = '' where entry in (83892,83893);
delete from creature_addon where guid in (select guid from creature where id in (83892,83893));
delete from creature_loot_template where entry in (83892,83893);

delete from areatrigger_actions where entry in (2893);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(2893, 0, 1, 0, 4096, 167977, 0, 0, 0, 0, 0, 0, 0, 'Bramble Patch - enter'),
(2893, 1, 42, 1, 4096, 167977, 0, 0, 0, 0, 0, 0, 0, 'Bramble Patch - exit');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry = 168105;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 168105, 0, 0, 31, 0, 3, 83892, 0, 0, 0, '', NULL),
(13, 1, 168105, 0, 1, 31, 0, 3, 83893, 0, 0, 0, '', NULL),
(13, 1, 168105, 0, 2, 31, 0, 3, 83894, 0, 0, 0, '', NULL);

delete from spell_dummy_trigger where spell_id in (177497,168375);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(177497, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Bramble Patch'),
(168375, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Grasping Vine Visual');

delete from smart_scripts where entryorguid = 84008;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(84008, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Just Summon - State Passive'),
(84008, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 167967, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Link - Cast Bramble Patch Visual');

delete from creature_text where entry in (83892,83893);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(83892, 0, 0, 'The forest closes in around you.', 14, 0, 100, 0, 0, 46135, 'Страж жизни Гола to Player'),
(83892, 1, 0, 'You are not welcome here!', 14, 0, 100, 0, 0, 46133, 'Страж жизни Гола to Player'),
(83892, 2, 0, 'Life renewed!', 14, 0, 100, 0, 0, 46140, 'Страж жизни Гола to Страж жизни Гола'),
(83892, 3, 0, 'Grow!', 14, 0, 100, 0, 0, 46139, 'Страж жизни Гола to 0'),
(83892, 4, 0, 'Trespassers beware!', 14, 0, 100, 0, 0, 46137, 'Страж жизни Гола to Тотем заземления'),
(83892, 5, 0, 'Water brings life!', 14, 0, 100, 0, 0, 46138, 'Страж жизни Гола to Страж жизни Гола'),
(83892, 6, 0, 'I return... to the soil...', 14, 0, 100, 0, 0, 46134, 'Страж жизни Гола to Player'),

(83893, 0, 0, 'You will never escape...', 14, 0, 100, 0, 0, 46209, 'Демиург Телу to Player'),
(83893, 1, 0, 'You will be purged!', 14, 0, 100, 0, 0, 46212, 'Демиург Телу to Player'),
(83893, 2, 0, 'The forest shields us.', 14, 0, 100, 0, 0, 46213, 'Демиург Телу to Player'),
(83893, 3, 0, 'The cycle continues...', 14, 0, 100, 0, 0, 46210, 'Демиург Телу to Тотем заземления'),
(83893, 4, 0, 'I will... be... renewed...', 14, 0, 100, 0, 0, 46213, 'Демиург Телу to Player');

delete from locales_creature_text where entry in (83892,83893);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES
(83892, 0, 0, 'Лес смыкается вокруг вас.'),
(83892, 1, 0, 'Вас сюда не звали!'),
(83892, 2, 0, 'Все возрождается!'),
(83892, 3, 0, 'Расти!'),
(83892, 4, 0, 'Смерть незваным гостям!'),
(83892, 5, 0, 'Вода - источник жизни!'),
(83892, 6, 0, 'Я возвращаюсь… в землю.'),

(83893, 0, 0, 'Вам некуда бежать…'),
(83893, 1, 0, 'Я уничтожу вас!'),
(83893, 2, 0, 'Лес укрывает нас.'),
(83893, 3, 0, 'Цикл продолжается…'),
(83893, 4, 0, 'Я… буду… возрожден...');


/* -------------------------------------
    Archmage Sol  /  Верховный маг Сол
   ------------------------------------- */
delete from creature where id = 84957;
update creature set spawntimesecs = 86400 where id = 82682;
update creature_template set rank = 3, unit_flags = 0, VehicleId = 0, mechanic_immune_mask = 617299967, ScriptName = 'boss_archmage_sol', flags_extra = 1 where entry = 82682;
update creature_template set minlevel = 102, maxlevel = 102, faction = 14, unit_flags = 559362, unit_flags2 = 268437504, flags_extra = 128 where entry = 82846;

update creature_template_addon set auras = '' where entry in (82682);
delete from creature_addon where guid in (select guid from creature where id in (82682));

update areatrigger_data set radius = 8 where entry = 2777;
delete from areatrigger_actions where entry = 2777;
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(2777, 0, 1, 0, 4096, 166726, 0, 0, 0, 0, 0, 0, 0, 'Frozen Rain Apply'),
(2777, 1, 42, 1, 4096, 166726, 0, 0, 0, 0, 0, 0, 0, 'Frozen Rain Remove');

delete from spell_dummy_trigger where spell_id = 166572;
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(166572, 166562, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Firebloom');

delete from spell_script_names where spell_id = 166723;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(166723, 'spell_sol_frozen_rain');

delete from creature_text where entry = 82682;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(82682, 0, 0, 'No... You can\'t... It\'s... I can\'t FIGHT IT!', 14, 0, 100, 0, 0, 45481, 'Верховный маг Сол to Одурманенный маг'),
(82682, 1, 0, 'Flee! The vines! They control us all!', 14, 0, 100, 0, 0, 45479, 'Верховный маг Сол to 0'),
(82682, 2, 0, 'No! They will take you too!', 14, 0, 100, 0, 0, 45482, 'Верховный маг Сол to Тотем заземления'),
(82682, 3, 0, 'Yes, fire! Fire will burn away the contagion.', 14, 0, 100, 0, 0, 45484, 'Верховный маг Сол to 0'),
(82682, 4, 0, '|TInterface\\Icons\\Ability_Mage_MoltenArmor.blp:20|tВерховный маг Сол применяет заклинания магии льда!', 41, 0, 100, 0, 0, 0, 'Верховный маг Сол to 0'),
(82682, 5, 0, 'Surely the cold will make them wither?', 14, 0, 100, 0, 0, 45485, 'Верховный маг Сол to 0'),
(82682, 6, 0, '|TInterface\\Icons\\Ability_Mage_MoltenArmor.blp:20|tВерховный маг Сол применяет заклинания тайной магии!', 41, 0, 100, 0, 0, 0, 'Верховный маг Сол to 0'),
(82682, 7, 0, 'Get away! GET AWAY!', 14, 0, 100, 0, 0, 45486, 'Верховный маг Сол to 0'),
(82682, 8, 0, 'Burn my body, it\'s the only way to stop them.', 14, 0, 100, 0, 0, 45480, 'Верховный маг Сол to Player');

delete from locales_creature_text where entry = 82682;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES
(82682, 0, 0, 'Нет… Я… Я не могу сопротивляться!'),
(82682, 1, 0, 'Бегите! Лианы! Они нами управляют!'),
(82682, 2, 0, 'Нет! Они и тебя заберут!'),
(82682, 3, 0, 'Огонь! Огонь выжжет всю эту заразу.'),
(82682, 5, 0, 'Они же... увянут от холода?'),
(82682, 7, 0, 'Прочь! Убирайтесь!'),
(82682, 8, 0, 'Сожгите мое тело, чтобы остановить их.');

/* ----------------------------
        Yalnu  /  Йалну
   ---------------------------- */
delete from creature where id in (83846,84399,84400,84401,84358,84329,84336);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`) VALUES 
(83846, 1279, 7109, 7330, 262, 1, '', 0, 0, 959.69, -1224.24, 181.24, 0.0735075, 86400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

update gameobject set spawntimesecs = 86400 where id in (235363,235355);

update creature_template set modelid1 = 11686, modelid2 = 0, flags_extra = 0, scriptName = 'npc_yalnu_teleporter' where entry = 68553;
update creature_template set rank = 3, unit_flags = 0, VehicleId = 0, mechanic_immune_mask = 617299967, ScriptName = 'boss_yalnu', flags_extra = 1 where entry = 83846;
update creature_template set flags_extra = 128 where entry = 84964;
update creature_template set ScriptName = 'npc_yalnu_kirin_tor_mage' where entry in (84329,84358);
update creature_template set modelid1 = 58399, modelid2 = 0, minlevel = 100, maxlevel = 100, exp = 5, faction = 14, unit_flags = 524288 where entry in (84499,85194);
update creature_template set ScriptName = 'npc_yalnu_eventer' where entry = 85194;
update creature_template set ScriptName = 'npc_yalnu_eventer', flags_extra = 128 where entry = 85107;
update creature_template set minlevel = 100, maxlevel = 100, exp = 5, faction = 2741, ScriptName = 'npc_yalnu_feral_lasher' where entry = 86684;

delete from areatrigger_data where entry = 3232;
INSERT INTO `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) VALUES 
(3232, 173537, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Genesis');

delete from areatrigger_actions where entry in (3232,2953,3013);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(3232, 0, 1, 0, 12288, 173539, 0, 0, 0, 0, 0, 0, 0, 'Apply Visual Genesis'),
(2953, 0, 1, 0, 4096, 169100, 0, 0, 0, 0, 0, 0, 0, 'Apply Flamestrike'),
(2953, 1, 42, 1, 4096, 169100, 0, 0, 0, 0, 0, 0, 0, 'Remove Flamestrike'),
(3013, 0, 1, 0, 4096, 170077, 0, 0, 0, 0, 0, 0, 0, 'Arcane Orb');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (169247,169126);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 169247, 0, 0, 31, 0, 3, 84329, 0, 0, 0, '', NULL),
(13, 1, 169126, 0, 0, 31, 0, 3, 86684, 0, 0, 0, '', NULL);

delete from creature_text where entry in (84358,83846);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(83846, 0, 0, '|TInterface\\Icons\\Ability_Druid_EmpoweredTouch:20|t Йалну начинает поддерживать |cFFFF0404|Hspell:169613|h["Сотворение"]|h|r.', 41, 0, 100, 0, 0, 0, 'Йалну to 0'),

(84358, 0, 0, 'The portal is lost! We must stop this beast before it can escape!', 14, 0, 100, 0, 0, 44237, 'Леди Байху to Player'),
(84358, 1, 0, 'We\'ve got its attention!', 14, 0, 100, 0, 0, 44227, 'Леди Байху to Йалну'),
(84358, 2, 0, 'Do you want to die here?! Move!', 14, 0, 100, 0, 0, 44236, 'Леди Байху to Йалну'),
(84358, 3, 0, 'We are lost! It\'s pushing us back!', 14, 0, 100, 0, 0, 44235, 'Леди Байху to Йалну'),
(84358, 4, 0, 'That doesn\'t look good...', 14, 0, 100, 0, 0, 44234, 'Леди Байху to Йалну'),
(84358, 5, 0, 'Quickly, draw it towards the flames!', 14, 0, 100, 0, 0, 44231, 'Леди Байху to Йалну'),
(84358, 6, 0, 'It\'s reviving the little ones!', 14, 0, 100, 0, 0, 44232, 'Леди Байху to Йалну');

delete from locales_creature_text where entry = 84358;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES
(84358, 0, 0, 'Портала больше нет! Надо остановить зверя, пока он не сбежал!'),
(84358, 1, 0, 'Мы привлекли его внимание!'),
(84358, 2, 0, 'Шевелитесь! Решили тут умереть?'),
(84358, 3, 0, 'Конец! Эта тварь теснит нас!'),
(84358, 4, 0, 'Это плохо…'),
(84358, 5, 0, 'Быстрее, ведите его к пламени!'),
(84358, 6, 0, 'Он оживляет мелких существ!');








