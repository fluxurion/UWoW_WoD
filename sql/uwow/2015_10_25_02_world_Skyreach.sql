delete from areatrigger_teleport where id in (10157);
INSERT INTO `areatrigger_teleport` (`id`, `name`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(10157, 'Skyreach (enter)', 1209, 1233.90, 1743.79, 177.16, 5.8);

delete from instance_template where map = 1209;
INSERT INTO `instance_template` (`map`, `parent`, `script`, `allowMount`, `bonusChance`) VALUES
(1209, 0, 'instance_skyreach', 0, 0);

update creature_template set gossip_menu_id = 16675, AIName = 'SmartAI' where entry in (82376,84782);
update creature_template set inhabitType = 7, flags_extra = 128 where entry in (77345,77346,77347,76286,77398,76285);

delete from smart_scripts where entryorguid in (82376,84782);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(82376, 0, 0, 1, 62, 0, 100, 0, 16675, 0, 0, 0, 62, 1116, 0, 0, 0, 0, 0, 7, 0, 0, 0, 33.52, 2528.28, 103.6, 3.62, 'Select Gossip - TeleportTo'),
(82376, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Link - Gossip Close'),
(84782, 0, 0, 1, 62, 0, 100, 0, 16675, 0, 0, 0, 62, 1116, 0, 0, 0, 0, 0, 7, 0, 0, 0, 33.52, 2528.28, 103.6, 3.62, 'Select Gossip - TeleportTo'),
(84782, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Link - Gossip Close');

delete from gossip_menu where entry = 16675;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES 
(16675, 24221);

delete from gossip_menu_option where menu_id = 16675;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES 
(16675, 0, 0, 'Перенеси меня на поверхность, дружище.', 1, 1, 0, 0, 0, 0, NULL);

delete from areatrigger_scripts where entry = 10142;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES 
(10142, 'SmartTrigger');

delete from smart_scripts where entryorguid = 10142 and source_type = 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(10142, 2, 0, 0, 46, 0, 100, 0, 10142, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Enter AT - KillUnit');

update creature_template set AIName = 'SmartAI' where entry in (77345,76285);
delete from smart_scripts where entryorguid in (77345,76285);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(76285, 0, 0, 0, 8, 0, 100, 0, 154031, 0, 1000, 1000, 11, 154034, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'spellhit - cast Lens Flare'),
(77345, 0, 0, 0, 8, 0, 100, 0, 156042, 0, 1000, 1000, 11, 156045, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'spellhit - cast Empower Warbird'),
(77345, 0, 1, 0, 8, 0, 100, 0, 156099, 0, 1000, 1000, 11, 154031, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'spellhit - cast Lens Flare');

delete from world_visible_distance where type = 1 and id = 6988;
INSERT INTO `world_visible_distance` (`type`, `id`, `distance`, `comment`) VALUES 
(1, 6988, 200, 'Skyreach');

/* --------------------------------------------
                Ranjit / Ранжит 
   -------------------------------------------- */
update creature set spawntimesecs = 86400 where id = 75964;
update creature_template set rank = 3, type_flags = type_flags | 4, mechanic_immune_mask = 617299967, ScriptName = 'boss_ranjit', flags_extra = 1 where entry = 75964;
update creature_template set modelid2 = 0, InhabitType = 7, flags_extra = 128 where entry in (76285); -- доделать
update creature_model_info set boundingradius = 0, combatreach = 0 where displayId = 21423;

delete from areatrigger_actions where entry in (1590,1591,1879,1881,1883);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(1590, 0, 1, 0, 4096, 153759, 0, 0, 0, 0, 0, 0, 0, 'Ranjit - Windwall_L apply'),
(1590, 1, 42, 1, 4096, 153759, 0, 0, 0, 0, 0, 0, 0, 'Ranjit - Windwall_L remove'),
(1591, 0, 1, 0, 4096, 153759, 0, 0, 0, 0, 0, 0, 0, 'Ranjit - Windwall_R apply'),
(1591, 1, 42, 1, 4096, 153759, 0, 0, 0, 0, 0, 0, 0, 'Ranjit - Windwall_R remove'),
(1879, 0, 1, 0, 4096, 153123, 0, 0, 0, 0, 0, 0, 0, 'Ranjit - Spinning Blade apply'),
(1879, 1, 42, 1, 4096, 153123, 0, 0, 0, 0, 0, 0, 0, 'Ranjit - Spinning Blade remove'),
(1881, 0, 1, 0, 4096, 153139, 0, 0, 0, 0, 0, 0, 0, 'Ranjit - Four Winds apply'),
(1881, 1, 42, 1, 4096, 153139, 0, 0, 0, 0, 0, 0, 0, 'Ranjit - Four Winds remove'),
(1883, 0, 1, 0, 4096, 153139, 0, 0, 0, 0, 0, 0, 0, 'Ranjit - Four Winds_2 apply'),
(1883, 1, 42, 1, 4096, 153139, 0, 0, 0, 0, 0, 0, 0, 'Ranjit - Four Winds_2 remove');

delete from areatrigger_data where entry in (1879,1590,1591,1881,1883);
insert into `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) values
('1879','153536','6038','35911','0','2','0','0','0','0','1','3','0','8.62','0','0','0','0','0','0','503','0','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','Вращающийся клинок'),
('1879','153588','6038','35911','0','2','0','0','0','0','1','3','0','8.62','0','0','0','0','0','0','503','0','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','Вращающийся клинок'),
('1590','153311','6062','35987','0','0','0','0','0','0','1','4','4000','-0.145','0','0','0','0','0','0','466','452','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','2','Стена ветра'),
('1591','153314','6062','38030','0','0','0','0','0','0','1','4','4000','0.145','0','0','0','0','0','0','467','453','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','2','Стена ветра'),
('1881','156634','6385','37284','0','0','0','0','0','0','1','4','1000','-0.14','0','0','0','0','0','0','505','506','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','2','Четыре ветра'),
('1883','156636','6385','35915','0','0','0','0','0','0','1','4','1000','0.14','0','0','0','0','0','0','507','508','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','2','Четыре ветра');

delete from spell_dummy_trigger where spell_id in (165733,165782);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `caster`, `effectmask`, `comment`) VALUES 
(165733, 0, 4, 0, 1, 'Ranjit - Piercing Rush'),
(165782, 154029, 4, 1, 2, 'Viryx - Lens Flare Summon');

delete from spell_target_position where id in (156793,156634,156636);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(156793, 1209, 1165.87, 1727.6, 189.45, 1.48),
(156634, 1209, 1165.87, 1727.6, 189.45, 3.14),
(156636, 1209, 1165.87, 1727.6, 189.45, 3.14);

delete from spell_script_names where spell_id in (153315,156793);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(153315, 'spell_ranjit_windwall'),
(156793, 'spell_ranjit_windwall');

delete from spell_visual_play_orphan where spellid in (156634,156636);
insert into `spell_visual_play_orphan` (`spellId`, `SpellVisualID`, `TravelSpeed`, `SpeedAsTime`, `UnkFloat`, `temp`, `comment`) values
('156634','41231','15','1','0','0','Четыре ветра'),
('156636','41213','15','1','0','0','Четыре ветра');

delete from creature_text where entry = 75964;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(75964, 0, 0, 'I will slice the flesh from your bones!', 14, 0, 100, 0, 0, 43887, 'Ранжит'),
(75964, 1, 0, 'I... perish....', 14, 0, 100, 0, 0, 43888, 'Ранжит'),
(75964, 2, 0, 'We will feed your corpse to the Kaliri!', 14, 0, 100, 0, 0, 43889, 'Ранжит'),
(75964, 2, 1, 'Move faster ground dweller!', 14, 0, 100, 0, 0, 43890, 'Ранжит'),
(75964, 3, 0, 'Your doom is coming.', 14, 0, 100, 0, 0, 43891, 'Ранжит'),
(75964, 3, 1, 'You cannot outrun the wind!', 14, 0, 100, 0, 0, 43892, 'Ранжит'),
(75964, 4, 0, 'As long as we rule the skies, none shall oppose us!', 14, 0, 100, 0, 0, 43893, 'Ранжит'),
(75964, 5, 0, '$n активирует Радужное Средоточие великого пика!', 41, 0, 100, 0, 0, 0, 'Ранжит');

delete from locales_creature_text where entry = 75964;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES 
(75964, 0, 0, 'Я срежу плоть с ваших костей!'),
(75964, 1, 0, 'Я... умираю...'),
(75964, 2, 0, 'Мы скормим ваши тела калири!'),
(75964, 2, 1, 'Слишком медленно, червяк!'),
(75964, 3, 0, 'Погибель близка.'),
(75964, 3, 1, 'Нельзя перегнать ветер!'),
(75964, 4, 0, 'Пока мы царим в небесах, никто не угроза для нас!');

/* --------------------------------------------
                Araknath / Аракнат 
   -------------------------------------------- */
delete from creature where id in (81080,81081,76376,76142,77543,76367);
update creature set spawntimesecs = 86400 where id = 76141;
update creature_template set rank = 3, unit_flags = 0, type_flags = type_flags | 4, mechanic_immune_mask = 617299967, ScriptName = 'boss_araknath', flags_extra = 1 where entry = 76141;
update creature_template set mechanic_immune_mask = 891, ScriptName = 'npc_skyreach_arcanologist' where entry = 76376;
update creature_template set modelid2 = 0, InhabitType = 7, ScriptName = 'npc_araknath_energizer', flags_extra = 128 where entry in (76367,77543);
update creature_template set ScriptName = 'npc_skyreach_sun_prototype' where entry = 76142;

delete from areatrigger_actions where entry in (2144,1978);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(2144, 0, 1, 0, 4096, 159226, 0, 0, 0, 0, 0, 0, 0, 'Solar Storm - apply'),
(2144, 1, 42, 1, 4096, 159226, 0, 0, 0, 0, 0, 0, 0, 'Solar Storm - remove'),
(1978, 0, 1, 0, 4096, 154132, 0, 0, 0, 0, 0, 0, 0, 'Smash - apply'),
(1978, 1, 42, 1, 4096, 154132, 0, 0, 0, 0, 0, 0, 0, 'Smash - remove');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (156382,154177,154179,156384,154159,154149,154140);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 156382, 0, 0, 31, 0, 3, 76367, 0, 0, 0, '', NULL),
(13, 1, 154177, 0, 0, 31, 0, 3, 76142, 0, 0, 0, '', NULL),
(13, 1, 154179, 0, 0, 31, 0, 3, 76141, 0, 0, 0, '', NULL),
(13, 3, 156384, 0, 0, 31, 0, 3, 76367, 0, 0, 0, '', NULL),
(13, 1, 154159, 0, 0, 31, 0, 3, 76142, 0, 0, 0, '', NULL),
(13, 1, 154149, 0, 0, 31, 0, 3, 76141, 0, 0, 0, '', NULL),
(13, 1, 154140, 0, 0, 31, 0, 3, 76141, 0, 0, 0, '', NULL);

delete from spell_linked_spell where spell_trigger = -160288;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `actiontype`, `comment`) VALUES 
(-160288, 160282, 0, 0, 'Solar Detonation');

delete from spell_script_names where spell_id = 154140;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(154140, 'spell_araknath_energize');

delete from spell_dummy_trigger where spell_id in (159215);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(159215, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Solar Storm');

delete from spell_target_filter where spellId = 154140;
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(154140, 15, 11, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 'Наполнение энергией');

delete from creature_summon_groups where summonerId = 76141;
INSERT INTO `creature_summon_groups` (`summonerId`, `id`, `summonerType`, `groupId`, `entry`, `position_x`, `position_y`, `position_z`, `orientation`, `count`, `actionType`, `distance`, `summonType`, `summonTime`) VALUES 
(76141, 0, 0, 0, 76142, 1014.06, 1797.3,  199.639, 0.59, 0, 0, 0, 8, 0),
(76141, 1, 0, 0, 76142, 1049.07, 1846.66, 199.639, 4.50, 0, 0, 0, 8, 0),
(76141, 2, 0, 0, 76142, 1049.8,  1781.45, 199.642, 1.70, 0, 0, 0, 8, 0),
(76141, 3, 0, 0, 76142, 1075.85, 1818.42, 199.642, 3.35, 0, 0, 0, 8, 0),
(76141, 4, 0, 0, 76142, 1022.04, 1839.5,  199.642, 5.24, 0, 0, 0, 8, 0),
(76141, 5, 0, 0, 76142, 1068.93, 1834.82, 199.642, 3.79, 0, 0, 0, 8, 0),
(76141, 6, 0, 0, 76142, 1067.91, 1792.21, 199.642, 2.33, 0, 0, 0, 8, 0),
(76141, 7, 0, 0, 76142, 1011.64, 1824.66, 199.642, 6.10, 0, 0, 0, 8, 0),
(76141, 8, 0, 0, 76142, 1032,    1782.56, 199.642, 1.25, 0, 0, 0, 8, 0),
(76141, 9, 0, 0, 76142, 1072.3,  1797.82, 199.642, 2.72, 0, 0, 0, 8, 0),
(76141, 10, 0, 0, 77543, 1044.21, 1812, 402.691, 5.48, 0, 0, 0, 8, 0),
(76141, 11, 0, 0, 76367, 1044.21, 1814, 250.954, 5.48, 0, 0, 0, 8, 0);

delete from creature_text where entry = 76376;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(76376, 0, 0, 'Arise my creation, slay these intruders!', 14, 0, 100, 0, 0, 46427, 'Арканолог Небесного Пути');

delete from locales_creature_text where entry = 76376;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES
(76376, 0, 0, 'Восстань, мой слуга! Убей чужаков!\n');

/* -----------------------------------
            Rukhran / Рухран 
   ----------------------------------- */
delete from creature where id in (76143,79505,76154,76253);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`) VALUES 
(76143, 1209, 6988, 6988, 262, 1, '', 0, 0, 896.64, 1896.37, 216.44, 6.06, 86400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

update gameobject set spawntimesecs = -86400 where id = 234165;
update creature_template set rank = 3, unit_flags = 0, type_flags = type_flags | 4, inhabitType = 7, mechanic_immune_mask = 617299967, ScriptName = 'boss_rukhran', flags_extra = 1 where entry = 76143;
update creature_template set AIName = 'SmartAI' where entry in (76253);
update creature_template set scriptName = 'npc_pile_of_ash' where entry = 79505;
update creature_template set speed_run = 0.6 where entry = 76227;

delete from creature_model_info where DisplayID = 59008;
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `DisplayID_Other_Gender`, `hostileId`) VALUES 
(59008, 0.31, 12, 0, 0);

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (154181,156042,156045,154182,153827,176544,169810,153828);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 154181, 0, 0, 31, 0, 3, 76286, 0, 0, 0, '', NULL),
(13, 1, 156042, 0, 0, 31, 0, 3, 77345, 0, 0, 0, '', NULL),
(13, 3, 156045, 0, 0, 31, 0, 3, 77347, 0, 0, 0, '', NULL),
(13, 1, 154182, 0, 0, 31, 0, 3, 76143, 0, 0, 0, '', NULL),
(13, 1, 153827, 0, 0, 31, 0, 3, 79505, 0, 0, 0, '', NULL),
(13, 1, 176544, 0, 0, 31, 0, 3, 79505, 0, 0, 0, '', NULL),
(13, 2, 169810, 0, 0, 31, 0, 3, 79505, 0, 0, 0, '', NULL),
(13, 2, 153828, 0, 0, 31, 0, 3, 79505, 0, 0, 0, '', NULL);

delete from spell_dummy_trigger where spell_id in (153898);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(153898, 160149, 1, 0, 0, 0, 10, 0, 0, 1, 0, 0, 'Rukhran - Screech');

delete from spell_target_filter where spellid = 159381;
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES
(159381, 15, 10, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 'Rukhran - Quills');

delete from spell_script_names where spell_id in (176544,159381);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(176544, 'spell_rukhran_ash_fixate'),
(159381, 'spell_rukhran_quills');

delete from creature_summon_groups where summonerId = 76143;
INSERT INTO `creature_summon_groups` (`summonerId`, `id`, `summonerType`, `groupId`, `entry`, `position_x`, `position_y`, `position_z`, `orientation`, `count`, `actionType`, `distance`, `summonType`, `summonTime`) VALUES 
(76143, 0 , 0, 0, 79505, 957.115, 1895.52, 213.951, 0, 0, 0, 0, 8, 0),
(76143, 1 , 0, 0, 79505, 949.354, 1906.89, 213.951, 0, 0, 0, 0, 8, 0),
(76143, 2 , 0, 0, 79505, 943.932, 1894.73, 213.947, 0, 0, 0, 0, 8, 0),
(76143, 3 , 0, 0, 79505, 930.16, 1910.75, 213.951, 0, 0, 0, 0, 8, 0),
(76143, 4 , 0, 0, 79505, 945.365, 1887.57, 213.947, 0, 0, 0, 0, 8, 0),
(76143, 5 , 0, 0, 79505, 937.847, 1898.8, 213.947, 0, 0, 0, 0, 8, 0),
(76143, 6 , 0, 0, 79505, 941.201, 1881.06, 213.947, 0, 0, 0, 0, 8, 0),
(76143, 7 , 0, 0, 79505, 930.137, 1897.23, 213.948, 0, 0, 0, 0, 8, 0),
(76143, 8 , 0, 0, 79505, 955.05, 1875.94, 213.951, 0, 0, 0, 0, 8, 0),
(76143, 9 , 0, 0, 79505, 917.177, 1901.93, 213.951, 0, 0, 0, 0, 8, 0),
(76143, 10, 0, 0, 79505, 933.812, 1879.6, 213.948, 0, 0, 0, 0, 8, 0),
(76143, 11, 0, 0, 79505, 914.21, 1883.06, 213.951, 0, 0, 0, 0, 8, 0),
(76143, 12, 0, 0, 79505, 927.696, 1883.42, 213.947, 0, 0, 0, 0, 8, 0),
(76143, 13, 0, 0, 79505, 925.917, 1890.5, 213.947, 0, 0, 0, 0, 8, 0),
(76143, 14, 0, 0, 79505, 941.688, 1866.34, 213.951, 0, 0, 0, 0, 8, 0),
(76143, 15, 0, 0, 79505, 922.844, 1870.87, 213.951, 0, 0, 0, 0, 8, 0);

delete from creature_text where entry = 76143;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(76143, 0, 0, '|TInterface\\Icons\\inv_feather_14.blp:20|tРухран обрушивает на вас множество |cFFFF0000|Hspell:159382|h[Перьев].|h|r В укрытие!', 41, 0, 100, 0, 0, 0, 'Рухран');

/* ------------------------------------------------
        High Sage Viryx /  Высший мудрец Вирикс
   ------------------------------------------------ */
delete from creature where id in (76306,76307,76309,76310,76146);
delete from creature where id = 81088 and position_z > 260 and position_z < 266;
update creature set spawntimesecs = 86400 where id = 76266;
update creature_template set rank = 3, type_flags = type_flags | 4, mechanic_immune_mask = 617299967, ScriptName = 'boss_high_sage_viryx', flags_extra = 1 where entry = 76266;
update creature_template set inhabitType = 7, vehicleId = 3293, ScriptName = 'npc_solar_zealot', flags_extra = 536870912 where entry = 76267;
update creature_template set inhabitType = 7 where entry = 81088;
update creature_template set modelid2 = 0, faction = 16, minlevel = 101, maxlevel = 101, speed_run = 0.8, unit_flags = 256, scriptName = 'npc_viryx_lens_flare', flags_extra = 128 where entry = 76083;
update creature_template set ScriptName = 'npc_skyreach_shield_construct', mechanic_immune_mask = 536936465 where entry = 76292;
update creature_template set AIName = 'SmartAI' where entry = 76145;

delete from creature_template_addon where entry = 76267;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(76267, 0, 0, 50331648, 1, 0, NULL);

delete from areatrigger_actions where entry = 1641;
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(1641, 0, 1, 0, 4096, 154043, 0, 0, 0, 0, 0, 0, 0, 'Viryx - Lens Flare apply'),
(1641, 1, 42, 1, 4096, 154043, 0, 0, 0, 0, 0, 0, 0, 'Viryx - Lens Flare remove');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (154032,156099,154031,154034,154055,165782);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 154032, 0, 0, 31, 0, 3, 76286, 0, 0, 0, '', NULL),
(13, 1, 156099, 0, 0, 31, 0, 3, 77345, 0, 0, 0, '', NULL),
(13, 1, 154031, 0, 0, 31, 0, 3, 76285, 0, 0, 0, '', NULL),
(13, 1, 154034, 0, 0, 31, 0, 3, 76083, 0, 0, 0, '', NULL),
(13, 1, 154055, 0, 0, 31, 0, 3, 76266, 0, 0, 0, '', NULL),
(13, 1, 165782, 0, 0, 31, 0, 3, 76286, 0, 0, 0, '', NULL);

delete from spell_dummy_trigger where spell_id in (153954,165845,154032);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(153954, 156789, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Viryx - Cast Down Find Player'),
(165845, 153955, 4, 0, 1, 0, 0, 0, 0, 1, 0, 0, 'Viryx - Cast Down Sum Veh'),
(154032, 154029, 4, 0, 1, 0, 0, 0, 0, 2, 0, 0, 'Viryx - Lens Flare Summon');

delete from spell_target_filter where spellid = 154032;
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(154032, 15, 3, 0, 16, 0, 0, 0, 2, 2, 1, 0, 0, 0, 'Viryx - Lens Flare');

delete from spell_target_position where id in (154049,156791);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(154049, 1209, 1080.41, 1787.73, 262.17, 5.65),
(156791, 1209, 1080.41, 1787.73, 262.17, 5.65);

delete from spell_script_names where spell_id in (153954,153955);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(153954, 'spell_viryx_cast_down'),
(153955, 'spell_viryx_cast_down_summ');

delete from npc_spellclick_spells where npc_entry = 76145;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES 
(76145, 46598, 1, 0);

delete from vehicle_template_accessory where EntryOrAura = 76145;
/* INSERT INTO `vehicle_template_accessory` (`EntryOrAura`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES 
(76145, 76146, 0, 1, '76145 - 76146', 8, 0),
(76145, 76306, 1, 1, '76145 - 76306', 8, 0),
(76145, 76307, 2, 1, '76145 - 76307', 8, 0),
(76145, 76309, 4, 1, '76145 - 76309', 8, 0),
(76145, 76310, 5, 1, '76145 - 76310', 8, 0); */

delete from creature_text where entry = 76266;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(76266, 0, 0, 'We will cast you down like the mongrels you are!', 14, 0, 100, 0, 0, 45176, 'Высший мудрец Вирикс'),
(76266, 1, 0, 'Cast them down, let the scavengers below feed on their pulp!', 14, 0, 100, 0, 0, 45180, 'Высший мудрец Вирикс'),
(76266, 2, 0, '$n активирует Радужное Средоточие великого пика!', 41, 0, 100, 0, 0, 0, 'Высший мудрец Вирикс'),
(76266, 3, 0, 'Behold the might of the Arakkoa!', 14, 0, 100, 0, 0, 45182, 'Высший мудрец Вирикс'),
(76266, 4, 0, 'Servants, protect your master!', 14, 0, 100, 0, 0, 45181, 'Высший мудрец Вирикс'),
(76266, 5, 0, 'You are unfit to rise to this height, return to your filth below!', 14, 0, 100, 0, 0, 45178, 'Высший мудрец Вирикс'),
(76266, 5, 1, 'What funny little birds, you have no wings!', 14, 0, 100, 0, 0, 45179, 'Высший мудрец Вирикс'),
(76266, 6, 0, 'We will always... soar....', 14, 0, 100, 0, 0, 45177, 'Высший мудрец Вирикс');

delete from locales_creature_text where entry = 76266;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES
(76266, 0, 0, 'Мы сбросим вас вниз, пресмыкающиеся!'),
(76266, 1, 0, 'Они рухнут на землю, и стервятники склюют их плоть!'),
(76266, 2, 0, '$n активирует Радужное Средоточие великого пика!'),
(76266, 3, 0, 'Узрите мощь араккоа!'),
(76266, 4, 0, 'Слуги, защищайте хозяина!'),
(76266, 5, 0, 'Тебе не дано подняться на пик, возвращайся обратно в грязь!'),
(76266, 5, 1, 'Какие странные птички – совсем без крыльев!'),
(76266, 6, 0, 'Мы всегда будем… парить…');


