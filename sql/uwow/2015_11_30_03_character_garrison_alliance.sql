-- Удаляем альянсу гарнизон и чистим базу от лишнего.
DELETE FROM `character_garrison` WHERE siteLevelId = 5;
DELETE FROM character_garrison_blueprints WHERE guid not in (select guid from `character_garrison`);
DELETE FROM character_garrison_buildings WHERE guid not in (select guid from `character_garrison`);
DELETE FROM character_garrison_followers WHERE guid not in (select guid from `character_garrison`);
DELETE FROM character_garrison_follower_abilities WHERE dbId not in (select dbId from `character_garrison_followers`);
DELETE FROM character_garrison_missions WHERE guid not in (select guid from `character_garrison`);

-- Чистка квестов.
DELETE FROM `character_queststatus` WHERE quest in (36100, 37288, 34778, 34787, 33359, 34820, 34779,33814, 36163, 36160, 34026, 36592, 34584, 34583, 34585, 34586, 35176, 34587, 35174, 35166, 34692, 34646, 34575, 34582, 34616, 36624 );
DELETE FROM `character_queststatus_objectives` WHERE quest in (36100, 37288, 34778, 34787, 33359, 34820, 34779,33814, 36163, 36160, 34026, 36592, 34584, 34583, 34585, 34586, 35176, 34587, 35174, 35166, 34692, 34646, 34575, 34582, 34616, 36624 );
DELETE FROM `character_queststatus_rewarded` WHERE quest in (36100, 37288, 34778, 34787, 33359, 34820, 34779,33814, 36163, 36160, 34026, 36592, 34584, 34583, 34585, 34586, 35176, 34587, 35174, 35166, 34692, 34646, 34575, 34582, 34616, 36624 );