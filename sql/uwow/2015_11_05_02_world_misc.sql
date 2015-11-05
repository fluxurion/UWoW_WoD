update creature_template set modelid1 = 55644, unit_flags = 33587208 where entry = 76168;

delete from spell_pet_auras where petEntry = 76168;
INSERT INTO `spell_pet_auras` (`petEntry`, `spellId`, `option`, `target`, `targetaura`, `bp0`, `bp1`, `bp2`, `aura`, `casteraura`, `createdspell`, `fromspell`, `comment`) VALUES 
(76168, 31366, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Ravager - Stun'),
(76168, 63416, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Ravager - Copy Weapon');

delete from spell_script_names where spell_id = 152277;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(152277, 'spell_warr_ravager');