REPLACE INTO `spell_learn_spell` (`entry`, `SpellID`, `ReqSpell`, `Active`) VALUES
(53428, 53343, 0, 1);

-- DK. Start quest INTRO
UPDATE `quest_template` SET `StartScript` = '12593' WHERE `quest_template`.`ID` = 12593;
DELETE FROM `quest_start_scripts` WHERE id = 12593;
REPLACE INTO `quest_start_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
 ('12593', '0', '16', '14970', '1', '0', '0', '0', '0', '0');