REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(158188, 'spell_rog_nightstalker');

DELETE FROM spell_script_names WHERE spell_script_names = 'spell_rog_nightstalker' AND spell_id = 1784;

REPLACE INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastalent`, `hastalent2`, `chance`, `cooldown`, `type2`, `hitmask`, `learnspell`, `removeMask`, `comment`) VALUES
 ('1784', '98877', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Stelth - Sanctuary'),
 ('1784', '158188', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Stelth - shapeshift'),
 ('1784', '158185', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Stelth - stelth'),
 ('-1784', '-158188', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Stelth - remove shapeshift'),
 ('-1784', '-158185', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Stelth - remove stelth');