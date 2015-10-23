DELETE FROM `spell_linked_spell` WHERE `spell_effect` = 1130;
DELETE FROM `spell_trigger` WHERE `spell_id` in (7381, 51527, 60503, 63225, 84839, 84840, 87100, 92297, 93098, 93099, 108288, 120267, 126119, 131523, 143268);
DELETE FROM `spell_trigger` WHERE `spell_trigger` in (123750, 146361, -50464, 82987, 84963, 124464, 124465, 124467, 124468, 124469, 132365, 89485, 145151, 145138, 118605, -131523, 52759, 142896, 29842, 144770, 144772);

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_pal_inquisition', 'spell_dru_demonic_circle_teleport', 'spell_warr_taste_for_blood_aura', 'spell_sha_mana_tide_totem',
'spell_sha_mana_tide', 'spell_rog_redirect', 'spell_dru_life_tap', 'spell_dru_symbiosis', 'spell_dru_symbiosis_aura', 'spell_dru_consecration', 'spell_dru_consecration_area',
'spell_mage_replenish_mana', 'spell_pri_hymn_of_hope', 'spell_pri_strength_of_soul', 'spell_dru_soul_swap', 'spell_dru_shattering_blow', 'spell_dru_anti_magic_shell');
