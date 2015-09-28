delete from areatrigger_teleport where id in (10205, 1468);
INSERT INTO `areatrigger_teleport` (`id`, `name`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(10205, 'UpperBlackrockSpire (exit)', 0, -7489.42, -1324.35, 301.5, 3.2),
(1468, 'UpperBlackrockSpire (enter)', 1358, 131.51, -319.13, 71, 6.2);

delete from instance_template where map = 1358;
INSERT INTO `instance_template` (`map`, `parent`, `script`, `allowMount`, `bonusChance`) VALUES
(1358, 0, 'instance_upper_blackrock_spire', 0, 0);

update creature set spawntimesecs = 86400 where id = 76396;
update creature_template set scriptName = 'npc_rune_glow', flags_extra = 130 where entry = 76396;
update creature_template set faction = 190, unit_flags = 131332 where entry = 76314;
update creature_template_addon set bytes2 = 2 where entry = 76157;

delete from creature where guid in (502579,502581,502583);
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`) VALUES 
(502579, 76181, 1358, 7307, 7307, 524550, 1, '', 0, 0, 186.545, -314.945, 76.9257, 3.2469, 7200, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0),
(502581, 76181, 1358, 7307, 7307, 524550, 1, '', 0, 0, 187.04, -320.101, 76.9148, 3.23512, 7200, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0),
(502583, 76157, 1358, 7307, 7307, 524550, 1, '', 0, 0, 186.616, -317.46, 76.9201, 3.23119, 7200, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0);

delete from creature_addon where guid = 502583;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(502583, 50258300, 0, 0, 1, 0, NULL);

delete from creature_formations where leaderGUID = 502583;
INSERT INTO `creature_formations` (`leaderGUID`, `memberGUID`, `dist`, `angle`, `groupAI`) VALUES 
(502583, 502583, 0, 0, 2),
(502583, 502581, 2, 270, 2),
(502583, 502579, 2, 90, 2);

delete from waypoint_data where id = 50258300;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `entry`, `wpguid`) VALUES 
(50258300, 1, 199.068, -306.26, 76.9453, 0, 0, 0, 0, 100, 0, 0),
(50258300, 2, 204.562, -284.759, 76.9461, 0, 5000, 0, 0, 100, 0, 0),
(50258300, 3, 199.362, -305.539, 76.9453, 0, 0, 0, 0, 100, 0, 0),
(50258300, 4, 203.547, -322.317, 76.9266, 0, 5000, 0, 0, 100, 0, 0),
(50258300, 5, 186.909, -316.372, 76.923, 0, 5000, 0, 0, 100, 0, 0);

delete from world_visible_distance where type = 2 and id = 7307;
INSERT INTO `world_visible_distance` (`type`, `id`, `distance`, `comment`) VALUES 
(2, 7307, 250, 'Upper Blackrock Spire');

/* --------------------------------------------
   Orebender Gor'ashan / Владыка руды Гор'ашан
   -------------------------------------------- */
delete from creature where id in (76417,76464);
update creature set spawntimesecs = 86400 where id = 76413;
update creature_template set rank = 3, mechanic_immune_mask = 617297919, flags_extra = 1, ScriptName = 'boss_orebender_gorashan' where entry = 76413;
update creature_template set ScriptName = 'npc_gorashan_power_rune', flags_extra = 130 where entry = 76417;
update creature_template set movementType = 2, inhabitType = 7, flags_extra = 130 where entry = 76464;

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (154294,154302,154900,154901,154296);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 154294, 0, 0, 31, 0, 3, 76413, 0, 0, 0, '', NULL),
(13, 1, 154302, 0, 0, 31, 0, 3, 76417, 0, 0, 0, '', NULL),
(13, 1, 154900, 0, 0, 31, 0, 3, 76417, 0, 0, 0, '', NULL),
(13, 1, 154901, 0, 0, 31, 0, 3, 76417, 0, 0, 0, '', NULL),
(13, 4, 154296, 0, 0, 31, 0, 3, 76417, 0, 0, 0, '', NULL);

delete from spell_script_names where spell_id in (154294,154435,154345);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(154294, 'spell_gorashan_power_conduit'),
(154435, 'spell_lodestone_spike'),
(154345, 'spell_electric_pulse');

delete from areatrigger_data where entry = 1668;
INSERT INTO `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) VALUES 
(1668, 154428, 6164, 36384, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Gorashan - Lodestone Spike');

delete from areatrigger_actions where entry = 1668;
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(1668, 0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Gorashan - Lodestone Spike Apply'),
(1668, 1, 42, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Gorashan - Lodestone Spike Remove');

delete from waypoint_data where id in (7646400,7646401);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `entry`, `wpguid`) VALUES 
(7646400, 1, 126.402, -276.79, 95, 0, 0, 1, 0, 100, 0, 0),
(7646400, 2, 126.354, -240.77, 95, 0, 0, 1, 0, 100, 0, 0),
(7646400, 3, 162.362, -240.705, 95, 0, 0, 1, 0, 100, 0, 0),
(7646400, 4, 162.401, -276.824, 95, 0, 0, 1, 0, 100, 0, 0),
(7646401, 1, 162.362, -240.705, 95, 0, 0, 1, 0, 100, 0, 0),
(7646401, 2, 162.401, -276.824, 95, 0, 0, 1, 0, 100, 0, 0),
(7646401, 3, 126.402, -276.79, 95, 0, 0, 1, 0, 100, 0, 0),
(7646401, 4, 126.354, -240.77, 95, 0, 0, 1, 0, 100, 0, 0);

delete from creature_text where entry = 76413;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(76413, 0, 0, 'Who dares disturb my research!', 14, 0, 100, 15, 0, 43920, 'Владыка руды Горашан'),
(76413, 1, 0, 'Перекройте рунопровод, чтобы ослабить защиту Гор''ашана.', 41, 0, 100, 0, 0, 0, 'Владыка руды Горашан'),
(76413, 2, 0, 'So much power! I am invincible!', 14, 0, 100, 0, 0, 43923, 'Владыка руды Горашан'),
(76413, 3, 0, 'You can\'t handle this!', 14, 0, 100, 0, 0, 43924, 'Владыка руды Горашан'),
(76413, 4, 0, 'I can feel this in my soul.', 14, 0, 100, 0, 0, 43925, 'Владыка руды Горашан'),
(76413, 5, 0, 'You disappoint me, I thought you\'d be tougher.', 14, 0, 100, 0, 0, 43921, 'Владыка руды Горашан'),
(76413, 6, 0, 'I will destroy you.', 14, 0, 100, 0, 0, 43926, 'Владыка руды Горашан'),
(76413, 7, 0, 'But the power... it protects me...', 14, 0, 100, 0, 0, 43919, 'Владыка руды Горашан');

delete from locales_creature_text where entry = 76413;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES 
(76413, 0, 0, 'Кто помешал моей работе?!'),
(76413, 2, 0, 'Какая мощь! Я непобедим!'),
(76413, 3, 0, 'С этим вам не справиться!'),
(76413, 4, 0, 'Я чувствую силу в своей душе.'),
(76413, 5, 0, 'Как жаль. Я думал, ты крепче.'),
(76413, 6, 0, 'Я уничтожу вас.'),
(76413, 7, 0, 'Но ведь сила... Защищает меня...');

/* --------------------------------
            Kyrak / Кирак
   -------------------------------- */
delete from creature where id in (82556);
update creature set spawntimesecs = 86400 where id = 76021;
update creature_template set rank = 3, mechanic_immune_mask = 617299967, flags_extra = 1, ScriptName = 'boss_kyrak' where entry = 76021;
update creature_template set mechanic_immune_mask = 617299967, scriptName = 'npc_drakonid_monstrosity' where entry in (76018, 82556);

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (161235);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 161235, 0, 0, 31, 0, 3, 76021, 0, 0, 0, '', NULL),
(13, 1, 161235, 0, 1, 31, 0, 3, 82556, 0, 0, 0, '', NULL);

delete from spell_trigger_dummy where spell_id in (177660,161207,161235,162589);
INSERT INTO `spell_trigger_dummy` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `check_spell_id`, `comment`) VALUES 
(177660, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Kyrak - Debilitating Fixation'),
(161207, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Kyrak - Vileblood Serum target'),
(161235, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Kyrak - Vileblood Serum friendly'),
(162589, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Kyrak - Salve of Toxic Fumes');

delete from areatrigger_data where entry = 2276;
INSERT INTO `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `comment`) VALUES 
(2276, 161210, 6823, 38705, 2, 2, 'Kyrak - Vileblood Serum');

delete from areatrigger_actions where entry = 2276;
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(2276, 0, 1, 0, 2, 161288, 0, 0, 0, 0, 0, 0, 0, 'Kyrak - Vileblood Serum Apply'),
(2276, 1, 42, 1, 2, 161288, 0, 0, 0, 0, 0, 0, 0, 'Kyrak - Vileblood Serum Remove');

delete from creature_text where entry = 76021;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(76021, 0, 0, 'Excellent, more raw materials!', 14, 0, 100, 0, 0, 46185, 'Кирак'),
(76021, 1, 0, 'I can feel the vile taint flowing through me.', 14, 0, 100, 0, 0, 46193, 'Кирак'),
(76021, 2, 0, 'I will catch you and I will fix you.', 14, 0, 100, 0, 0, 46191, 'Кирак'),
(76021, 3, 0, 'Please, repurpose me!', 14, 0, 100, 0, 0, 46184, 'Кирак');

delete from locales_creature_text where entry = 76021;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES 
(76021, 0, 0, 'Отлично, новое сырье!'),
(76021, 1, 0, 'Скверна разливается по моим жилам.'),
(76021, 2, 0, 'Я поймаю вас и починю.'),
(76021, 3, 0, 'Господин, переделай меня…');

/* -------------------------------------
    Commander Tharbek / Командир Тарбек 
   ------------------------------------- */
delete from gameobject where id = 237469;
delete from creature WHERE position_x>'196.6' AND position_x<'222' AND position_y>'-436.8' AND position_y<'-406.9' AND position_z>105 AND position_z<115 AND map=1358;
update creature set spawntimesecs = 86400 where id = 79912;
update creature_template set rank = 3, mechanic_immune_mask = 617299967, flags_extra = 1, ScriptName = 'boss_commander_tharbek' where entry = 79912;
update creature_template set inhabittype = 7, mechanic_immune_mask = 617299967, ScriptName = 'npc_ironbarb_skyreaver' where entry = 80098;
update creature_template set modelid2 = 0, faction = 14, minlevel = 100, maxlevel = 100, flags_extra = 128, scriptName = 'npc_imbued_iron_axe_stalker' where entry = 80307;

delete from spell_trigger_dummy where spell_id in (161811,161987,162085);
INSERT INTO `spell_trigger_dummy` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `check_spell_id`, `comment`) VALUES 
(161811, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Tarbek - Noxious Spit'),
(161987, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Tarbek - Iron Reaver'),
(162085, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Tarbek - Imbued Iron Axe');

delete from areatrigger_data where entry in (2336,2355);
INSERT INTO `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) VALUES 
(2336, 161827, 6880, 39146, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Tarbek - Noxious Spit'),
(2355, 161989, 6902, 39201, 2.5, 2.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Tarbek - Iron Reaver');

delete from areatrigger_actions where entry in (2336,2355);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(2336, 0, 1, 0, 2, 161833, 0, 0, 0, 0, 0, 0, 0, 'Tarbek - Noxious Spit Apply'),
(2336, 1, 42, 1, 2, 161833, 0, 0, 0, 0, 0, 0, 0, 'Tarbek - Noxious Spit Remove'),
(2355, 0, 1, 0, 2, 162000, 0, 0, 0, 0, 0, 0, 0, 'Tarbek - Iron Reaver Apply'),
(2355, 1, 42, 1, 2, 162000, 0, 0, 0, 0, 0, 0, 0, 'Tarbek - Iron Reaver Remove');

delete from waypoint_data where id = 8009800;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `entry`, `wpguid`) VALUES 
(8009800, 1, 184.135, -420.372, 110.867, 0, 0, 1, 0, 100, 0, 0),
(8009800, 2, 155.087, -435.981, 121.828, 0, 2000, 1, 0, 100, 0, 0),
(8009800, 3, 135.413, -424.362, 125.574, 0, 0, 1, 0, 100, 0, 0),
(8009800, 4, 143.882, -403.948, 125.327, 0, 0, 1, 0, 100, 0, 0),
(8009800, 5, 169.565, -406.845, 125.202, 0, 0, 1, 0, 100, 0, 0),
(8009800, 6, 184.15, -417.56, 115.218, 0, 0, 1, 0, 100, 0, 0),
(8009800, 7, 179.082, -420.37, 110.472, 0, 0, 1, 0, 100, 0, 0);

update creature_template set mechanic_immune_mask = 1, AIName = 'SmartAI' where entry in (77096,77036,77035,76181,76176,76157,76101);
delete from smart_scripts where entryorguid in (77096,77036,77035,76181,76176,76157,76101);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(77096, 0, 0, 0, 0, 0, 100, 0, 2000, 10000, 10000, 20000, 11, 161406, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Poison Spit'),
(77036, 0, 1, 0, 0, 0, 100, 0, 20000, 26000, 20000, 20000, 11, 155589, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Frost Nova'),
(77036, 0, 0, 0, 0, 0, 100, 0, 2000, 4000, 4000, 4000, 11, 155590, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Fireball'),
(77035, 0, 2, 0, 0, 0, 100, 0, 30000, 30000, 20000, 20000, 11, 155586, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Veil of Shadow'),
(77035, 0, 1, 0, 0, 0, 100, 0, 14000, 14000, 18000, 18000, 11, 155588, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Shadow Bolt Volley'),
(77035, 0, 0, 0, 0, 0, 100, 0, 2000, 2000, 6000, 6000, 11, 155587, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Shadow Bolt'),
(76181, 0, 1, 0, 2, 0, 100, 1, 1, 20, 0, 0, 11, 154017, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Blackrock Rabies'),
(76181, 0, 0, 0, 0, 0, 100, 0, 8000, 8000, 18000, 18000, 11, 154039, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Frantic Mauling'),
(76176, 0, 1, 0, 0, 0, 100, 0, 12000, 12000, 8000, 8000, 11, 153897, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Rending Cleave'),
(76176, 0, 0, 0, 0, 0, 100, 0, 10000, 10000, 20000, 20000, 11, 153909, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Frenzy'),
(76157, 0, 1, 0, 0, 0, 100, 0, 10000, 10000, 16000, 16000, 11, 153981, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Incendiary Shell'),
(76157, 0, 0, 0, 0, 0, 100, 0, 4000, 4000, 1000, 2000, 11, 153974, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Rifle Shot'),
(76101, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 155505, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Debilitating Ray'),
(76101, 0, 0, 1, 0, 0, 100, 0, 2000, 2000, 8000, 8000, 13, 1000, 0, 0, 0, 0, 0, 125, 0, 0, 0, 0, 0, 0, 0, 'Add Threat');

delete from creature_text where entry = 79912;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(79912, 0, 0, 'My Warlord, I have failed you.', 14, 0, 100, 1, 0, 45855, 'Командир Тарбек');

delete from locales_creature_text where entry = 79912;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES
(79912, 0, 0, 'Полководец, я подвел тебя.');

/* ----------------------------------------
    Ragewing Untamed / Ярокрыл Неукротимый
   ---------------------------------------- */
delete from creature where id = 76801;
update creature set spawntimesecs = 86400 where id = 76585;
update creature_template set rank = 3, mechanic_immune_mask = 617299967, flags_extra = 1, ScriptName = 'boss_ragewing_untamed' where entry = 76585;
update creature_template set inhabittype = 7 where entry = 76801;
update creature_template set speed_run = 1.7, flags_extra = 128 where entry in (76813,76837);
update creature_template set flags_extra = 128 where entry = 76885;

delete from spell_trigger_dummy where spell_id in (155606,155050,154996);
INSERT INTO `spell_trigger_dummy` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `check_spell_id`, `comment`) VALUES 
(155606, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Ragewing - Fire Breath'),
(155050, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Ragewing - Magma Spit'),
(154996, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Ragewing - Engulfing Fire');

delete from spell_script_names where spell_id = 155070;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(155070, 'spell_fire_storm');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (154996,155070);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 154996, 0, 0, 31, 0, 3, 76813, 0, 0, 0, '', NULL),
(13, 1, 154996, 0, 1, 31, 0, 3, 76837, 0, 0, 0, '', NULL),
(13, 1, 155070, 0, 0, 31, 0, 3, 76885, 0, 0, 0, '', NULL);

delete from areatrigger_data where entry in (1705,1704);
INSERT INTO `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) VALUES 
(1705, 155051, 6204, 36586, 3.5, 3.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Ragewing - Magma Pool'),
(1704, 155029, 1704, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 'Ragewing - Engulfing Fire Apply');

delete from areatrigger_actions where entry in (1705,1704);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(1705, 0, 1, 0, 2, 155057, 0, 0, 0, 0, 0, 0, 0, 'Ragewing - Magma Pool apply'),
(1705, 1, 42, 1, 2, 155057, 0, 0, 0, 0, 0, 0, 0, 'Ragewing - Magma Pool remove'),
(1704, 0, 1, 0, 2, 155031, 0, 0, 0, 0, 0, 0, 0, 'Ragewing - Engulfing Fire Apply'),
(1704, 1, 1, 0, 2, 155056, 0, 0, 0, 0, 0, 0, 0, 'Ragewing - Engulfing Fire Apply');

delete from areatrigger_polygon where entry = 1704;
INSERT INTO `areatrigger_polygon` (`entry`, `spellId`, `id`, `x`, `y`) VALUES 
(1704, 155029, 0, 0, -3.5),
(1704, 155029, 1, 20, -5),
(1704, 155029, 2, 20, 5),
(1704, 155029, 3, 0, 3.5);

delete from waypoint_data where id in (7681300,7683700,7658500);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`) VALUES 
(7681300, 1, 26.7769, -430.114, 110.77, 0, 0, 1),
(7681300, 2, 31.6178, -423.644, 110.962, 0, 0, 1),
(7681300, 3, 36.6789, -398.784, 110.727, 0, 0, 1),
(7681300, 4, 26.7769, -430.114, 110.77, 0, 0, 1),
(7683700, 1, 30.2494, -380.687, 110.727, 0, 0, 1),
(7683700, 2, 35.4452, -414.441, 110.765, 0, 0, 1),
(7683700, 3, 30.2494, -380.687, 110.727, 0, 0, 1),
(7658500, 1, 71.1633, -419.872, 139.789, 3, 5000, 1),
(7658500, 2, -3.46972, -405.176, 129.546, 0.03, 5000, 1),
(7658500, 3, 22.08, -404.66, 115.5, 0, 0, 1);

delete from creature_text where entry = 76585;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(76585, 0, 0, '%s He begins to exhale |cFFFF0404|Hspell:155031|h["Engulfing Fire"]|h|r!', 41, 0, 100, 0, 0, 0, 'Ярокрыл Неукротимый');

delete from locales_creature_text where entry = 76585;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES
(76585, 0, 0, '%s начинает выдыхать |cFFFF0404|Hspell:155031|h["Охватывающий огонь"]|h|r!');

/* ----------------------------------
    Warlord Zaela / Полководец Зела 
   ---------------------------------- */
delete from creature where id in (82429,82480,83480);
update creature set spawntimesecs = 86400 where id = 77120;
update creature set spawndist = 15, movementtype = 1 where id = 82428;
update creature_template set inhabittype = 7 where entry in (82428,82480,83479);
update creature_template set rank = 3, mechanic_immune_mask = 617299967, flags_extra = 1, ScriptName = 'boss_warlord_zaela' where entry = 77120;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'npc_emberscale_ironflight' where entry in (82428,83479);
update creature_template set ScriptName = 'npc_emberscale_matron' where entry = 82480;
update creature_template set flags_extra = 2 where entry = 82429;

delete from spell_trigger_dummy where spell_id in (155720,155705);
INSERT INTO `spell_trigger_dummy` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `check_spell_id`, `comment`) VALUES 
(155720, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Zaela - Black Iron Cyclone'),
(155705, 0, 14, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'Zaela - Rebounding Blade');

delete from creature_template_addon where entry = 80098;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(80098, 0, 0, 50331648, 1, 0, '');

delete from areatrigger_actions where entry = 2775;
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(2775, 0, 1, 0, 2, 166730, 0, 0, 0, 0, 0, 0, 0, 'Zaela - Burning Bridge Apply'),
(2775, 1, 42, 1, 2, 166730, 0, 0, 0, 0, 0, 0, 0, 'Zaela - Burning Bridge Remove');

delete from npc_spellclick_spells where npc_entry in (82428,83479);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES 
(82428, 46598, 1, 0),
(83479, 46598, 1, 0);

delete from vehicle_template_accessory where EntryOrAura in (82428,83479);
INSERT INTO `vehicle_template_accessory` (`EntryOrAura`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES 
(82428, 82429, 0, 1, 'Emberscale Ironflight Zaela', 6, 30000),
(83479, 83480, 0, 0, 'Emberscale Ironflight 2 Zaela', 6, 30000);

delete from waypoint_data where id = 8248000;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `entry`, `wpguid`) VALUES 
(8248000, 1, -12.4091, -123.59, 95.657, 0, 1000, 1, 0, 100, 0, 0),
(8248000, 2, 2.09523, -56.5817, 109.16, 0, 0, 1, 0, 100, 0, 0),
(8248000, 3, 14.7827, -12.476, 118.377, 0, 0, 1, 0, 100, 0, 0),
(8248000, 4, 35.8678, 12.8592, 121.689, 0, 0, 1, 0, 100, 0, 0),
(8248000, 5, 81.3443, 6.12083, 127.105, 0, 15000, 1, 0, 100, 0, 0),
(8248000, 6, 84.7788, -23.5574, 128.325, 0, 0, 1, 0, 100, 0, 0),
(8248000, 7, 90.5996, -81.4702, 120.84, 0, 0, 1, 0, 100, 0, 0),
(8248000, 8, 53.3684, -119.943, 130.064, 0, 0, 1, 0, 100, 0, 0),
(8248000, 9, 31.7618, -124.968, 123.817, 0, 1000, 1, 0, 100, 0, 0),
(8248000, 10, 51.3412, -1.54165, 122.773, 0, 0, 1, 0, 100, 0, 0);

delete from waypoint_data where id >= 8242800 and id <= 8242805;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`) VALUES 
(8242800, 1, 45.85, -131.02, 98.6, 3.1, 0, 1),
(8242801, 1, 45.12, -120.77, 98.6, 3.1, 0, 1),
(8242802, 1, 46.9, -109.61, 98.6, 3.1, 0, 1),
(8242803, 1, 35.24, -97.68, 98.6, 4.7, 0, 1),
(8242804, 1, 23.92, -98.41, 98.6, 4.7, 0, 1),
(8242805, 1, 7.67, -97.51, 98.6, 4.7, 0, 1);

update creature_template set AIName = 'SmartAI' where entry = 83480;
delete from smart_scripts where entryorguid = 83480;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(83480, 0, 0, 0, 7, 0, 100, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evade - Despawn'),
(83480, 0, 1, 0, 0, 0, 100, 0, 8000, 12000, 16000, 18000, 11, 167242, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Dragon Roar'),
(83480, 0, 2, 0, 0, 0, 100, 0, 6000, 8000, 8000, 12000, 11, 167241, 0, 0, 0, 0, 0, 126, 0, 0, 0, 0, 0, 0, 0, 'Cast Chain Pull');

delete from creature_text where entry = 77120;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(77120, 0, 0, 'Have you come to get a front row seat for Stormwind\'s destruction?', 14, 0, 100, 25, 0, 43556, 'Полководец Зела'),
(77120, 1, 0, 'Good, I have been waiting for a fight!', 14, 0, 100, 15, 0, 43554, 'Полководец Зела'),
(77120, 2, 0, 'Flames will consume you!', 14, 0, 100, 22, 0, 43561, 'Полководец Зела'),
(77120, 3, 0, 'You will burn!', 14, 0, 100, 22, 0, 43560, 'Полководец Зела'),
(77120, 4, 0, 'And now, you die!', 14, 0, 100, 22, 0, 43563, 'Полководец Зела'),
(77120, 5, 0, 'My blade will end you!', 14, 0, 100, 22, 0, 43562, 'Полководец Зела'),
(77120, 6, 0, 'I am sorry Garrosh, I have failed you.', 14, 0, 100, 0, 0, 43555, 'Полководец Зела');

delete from locales_creature_text where entry = 77120;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES
(77120, 0, 0, 'Хотите полюбоваться на гибель Штормграда из первых рядов?'),
(77120, 1, 0, 'Отлично! Я соскучилась по драке!'),
(77120, 2, 0, 'Пламя поглотит вас!'),
(77120, 3, 0, 'Вы сгорите!'),
(77120, 4, 0, 'А теперь вы умрете!'),
(77120, 5, 0, 'Мой клинок разрубит вас!'),
(77120, 6, 0, 'Прости, Гаррош. Я подвела тебя.');


