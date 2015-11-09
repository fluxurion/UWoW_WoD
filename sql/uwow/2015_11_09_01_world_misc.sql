delete from where spell_id = 6544 and spell_trigger = 178367;
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(6544, 178367, 5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Warrior - Heroic Leap');

delete from where spell_trigger = 6544 and spell_effect = 355;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `comment`) VALUES 
(6544, 355, 0, 0, 1, 0, 157449, 0, 0, 0, 0, 0, 0, 0, -1, 8, 'Warrior - Heroic Leap - Remove cd Taunt');