insert into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `hitmask`, `removeMask`, `actiontype`, `comment`) values
('11327','158188','0','0','0','0','0','0','0','0','0','0','0','0','Stelth - shapeshift'),
('-11327','-158188','0','0','0','0','0','0','0','0','0','0','0','0','Stelth - remove shapeshift');

insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('1461','0','1','0','7','88611','0','0','0','0','0','0','0','Дымовая шашка'),
('1461','1','42','1','7','88611','0','0','0','0','0','0','0','Дымовая шашка');
