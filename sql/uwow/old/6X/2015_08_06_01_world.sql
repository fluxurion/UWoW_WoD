-- class 8
update `creature_classlevelstats` set `damage_base` = `level` / 1.986  where `level` > 83 and class = 8;
update `creature_classlevelstats` set `attackpower` = (`level` - 83)*17  where `level` > 83 and class = 8;
update `creature_classlevelstats` set `rangedattackpower` = (`level` - 83)*5  where `level` > 83 and class = 8;

-- class 4
update `creature_classlevelstats` set `damage_base` = `level` / 1.76  where `level` > 83 and class = 4;
update `creature_classlevelstats` set `attackpower` = (`level` - 83)*23  where `level` > 83 and class = 4;
update `creature_classlevelstats` set `rangedattackpower` = (`level` - 83)*14  where `level` > 83 and class = 4;

-- class 2
update `creature_classlevelstats` set `damage_base` = `level` / 1.84  where `level` > 83 and class = 2;
update `creature_classlevelstats` set `attackpower` = (`level` - 83)*57  where `level` > 83 and class = 2;
update `creature_classlevelstats` set `rangedattackpower` = (`level` - 83)*7  where `level` > 83 and class = 2;

-- class 1
update `creature_classlevelstats` set `damage_base` = `level` / 1.76  where `level` > 83 and class = 1;
update `creature_classlevelstats` set `attackpower` = (`level` - 83)*23  where `level` > 83 and class = 1;
update `creature_classlevelstats` set `rangedattackpower` = (`level` - 83)*14  where `level` > 83 and class = 1;

-- all
update `creature_classlevelstats` set `damage_exp3` = damage_base*2  where `level` > 83;
update `creature_classlevelstats` set `damage_exp4` = damage_base*3  where `level` > 83;
update `creature_classlevelstats` set `damage_exp5` = damage_base*4  where `level` > 83;