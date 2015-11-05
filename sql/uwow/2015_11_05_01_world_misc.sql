delete from spell_trigger where spell_id = 29725;

delete from spell_proc where spellId = 52437;
INSERT INTO `spell_proc` (`spellId`, `schoolMask`, `spellFamilyName`, `spellFamilyMask0`, `spellFamilyMask1`, `spellFamilyMask2`, `spellFamilyMask3`, `typeMask`, `spellTypeMask`, `spellPhaseMask`, `hitMask`, `attributesMask`, `ratePerMinute`, `chance`, `cooldown`, `charges`, `modcharges`) VALUES 
(52437, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);

delete from spell_proc_event where entry in (29725, 52437);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) VALUES 
(29725, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 7);

delete from spell_proc_check where entry = 52437;
INSERT INTO `spell_proc_check` (`entry`, `entry2`, `entry3`, `checkspell`, `hastalent`, `chance`, `target`, `effectmask`, `powertype`, `dmgclass`, `specId`, `spellAttr0`, `targetTypeMask`, `mechanicMask`, `fromlevel`, `perchp`, `spelltypeMask`, `combopoints`, `deathstateMask`, `hasDuration`, `comment`) VALUES 
(52437, 0, 0, 5308, 0, 0, 0, 7, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Воин: Внезапная смерть - Казнь'),
(52437, 0, 0, 163201, 0, 0, 0, 7, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Воин: Внезапная смерть - Казнь');