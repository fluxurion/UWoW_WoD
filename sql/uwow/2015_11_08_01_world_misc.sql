delete from spell_script_names where spell_id in (12294,23881);

delete from spell_trigger where spell_id = 174737;
INSERT INTO `spell_trigger` (`spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `comment`) VALUES 
(174737, 174736, 0, 20, 0, 0, 2, 0, 0, 0, 1, 772, 0, 0, 0, 0, 0, -1, 0, 0, 0, 'Warrior: Enhanced Rend');