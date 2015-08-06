-- class 8
update `creature_classlevelstats` set `damage_base` = `level` / 1.986  where `damage_base` = 0 and class = 8;
update `creature_classlevelstats` set `attackpower` = (`level` - 83)*17  where `attackpower` = 0 and class = 8;
update `creature_classlevelstats` set `rangedattackpower` = (`level` - 83)*5  where `rangedattackpower` = 0 and class = 8;

-- class 4
update `creature_classlevelstats` set `damage_base` = `level` / 1.76  where `damage_base` = 0 and class = 4;
update `creature_classlevelstats` set `attackpower` = (`level` - 83)*23  where `attackpower` = 0 and class = 4;
update `creature_classlevelstats` set `rangedattackpower` = (`level` - 83)*14  where `rangedattackpower` = 0 and class = 4;

-- class 2
update `creature_classlevelstats` set `damage_base` = `level` / 1.84  where `damage_base` = 0 and class = 2;
update `creature_classlevelstats` set `attackpower` = (`level` - 83)*57  where `attackpower` = 0 and class = 2;
update `creature_classlevelstats` set `rangedattackpower` = (`level` - 83)*7  where `rangedattackpower` = 0 and class = 2;

-- class 1
update `creature_classlevelstats` set `damage_base` = `level` / 1.76  where `damage_base` = 0 and class = 1;
update `creature_classlevelstats` set `attackpower` = (`level` - 83)*23  where `attackpower` = 0 and class = 1;
update `creature_classlevelstats` set `rangedattackpower` = (`level` - 83)*14  where `rangedattackpower` = 0 and class = 1;

-- all
update `creature_classlevelstats` set `damage_exp3` = damage_base*4  where `damage_exp3` = 0;
update `creature_classlevelstats` set `damage_exp4` = damage_base*5  where `damage_exp4` = 0;
update `creature_classlevelstats` set `damage_exp5` = damage_base*6  where `damage_exp5` = 0;