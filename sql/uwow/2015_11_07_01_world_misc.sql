delete from spell_linked_spell where spell_trigger = -772;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) VALUES 
(-772, 94009, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Warrior - Rend end dot');

delete from spell_script_names = 174926;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(174926, 'spell_warr_shield_barrier');

delete from spell_linked_spell where spell_trigger in (355,1719,12328,86346,167105) and spell_effect in (71,2457);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `comment`) VALUES 
(355, 71, 7, 0, 0, 0, 2457, 0, 0, 0, 0, 0, 0, 0, -1, 0, 'Warrior: Taunt - switch stance'),
(1719, 2457, 7, 0, 0, 0, 71, 0, 0, 0, 0, 0, 0, 0, -1, 0, 'Warrior: recklessness - switch stance'),
(12328, 2457, 7, 0, 0, 0, 71, 0, 0, 0, 0, 0, 0, 0, -1, 0, 'Warrior: Sweeping Strikes - switch stance'),
(86346, 2457, 7, 0, 0, 0, 71, 0, 0, 0, 0, 0, 0, 0, -1, 0, 'Warrior: Colossus Smash - switch stance'),
(167105, 2457, 7, 0, 0, 0, 71, 0, 0, 0, 0, 0, 0, 0, -1, 0, 'Warrior: Colossus Smash - switch stance');

delete from spell_script_names where spell_id = 163201;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(163201, 'spell_warr_execute');
