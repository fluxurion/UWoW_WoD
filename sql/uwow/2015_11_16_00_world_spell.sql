DELETE FROM spell_bonus_data WHERE entry IN (124081,124098,125033,124101);
insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('124081','0','0','0','0.095','0','0','Monk - Zen Sphere (heal'),
('124098','0','0','0.095','0','0','0','Monk - Zen Sphere (damage)'),
('124101','0','0','1.25','0','0','0','Monk - Zen Sphere : Detonate (heal)'),
('125033','0','0','1.25','0','0','0','Monk - Zen Sphere : Detonate (damage)');

DELETE FROM spell_bonus_data WHERE entry IN (132463,132467);
insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('132463','0','0','0.5','0','0','0','Monk - Chi Wave(Heal)'),
('132467','0','0','0.5','0','0','0','Monk - Chi Wave(Damage)');

DELETE FROM spell_bonus_data WHERE entry IN (130654,148135);
insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('130654','0','0','2.75','0','0','0','Monk - Chi Burst (heal)'),
('148135','0','0','2.75','0','0','0','Выброс ци');

insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('362','0','1','0','2','123687','0','0','0','0','0','0','0','Несущийся бык');

insert into `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `damage_bonus`, `heal_bonus`, `comments`) values
('158221','0','0','2.5','0','0','0','Ураганный удар');
insert into `spell_script_names` (`spell_id`, `ScriptName`) values('152175','spell_monk_hurricane_strike');

insert into `spell_script_names` (`spell_id`, `ScriptName`) values('157535','spell_monk_breath_of_the_serpent');
insert into `spell_pet_auras` (`petEntry`, `spellId`, `option`, `target`, `targetaura`, `bp0`, `bp1`, `bp2`, `aura`, `casteraura`, `createdspell`, `fromspell`, `comment`) values
('78065','157627','3','2','0','0','0','0','0','0','0','0','Дыхание Змеи');
