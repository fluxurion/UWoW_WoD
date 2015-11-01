update creature_template set dmg_multiplier = 1;

update creature_template set dmg_multiplier = 2 where rank = 1;
update creature_template set dmg_multiplier = 1.5 where rank = 2;
update creature_template set dmg_multiplier = 3 where rank = 3;
update creature_template set dmg_multiplier = 1.5 where rank = 4;


update creature_classlevelstats set damage_base = damage_base * 4 where level > 0;
update creature_classlevelstats set damage_base = damage_base * 1.5 where level > 30;

update creature_classlevelstats set damage_exp1 = damage_base * 1.3;
update creature_classlevelstats set damage_exp2 = damage_exp1 * 1.3;
update creature_classlevelstats set damage_exp3 = damage_exp2 * 1.3;
update creature_classlevelstats set damage_exp4 = damage_exp3 * 1.2;
update creature_classlevelstats set damage_exp5 = damage_exp4 * 3.8;