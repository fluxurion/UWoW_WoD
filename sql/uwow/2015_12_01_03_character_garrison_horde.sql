-- Удаляем орде гарнизон и чистим базу от лишнего.
DELETE FROM `character_garrison` WHERE siteLevelId in (444, 445, 6, 259);
DELETE FROM character_garrison_blueprints WHERE guid not in (select guid from `character_garrison`);
DELETE FROM character_garrison_buildings WHERE guid not in (select guid from `character_garrison`);
DELETE FROM character_garrison_followers WHERE guid not in (select guid from `character_garrison`);
DELETE FROM character_garrison_follower_abilities WHERE dbId not in (select dbId from `character_garrison_followers`);
DELETE FROM character_garrison_missions WHERE guid not in (select guid from `character_garrison`);

-- Чистка квестов.
DELETE FROM `character_queststatus` WHERE quest in (33815, 33868, 34402, 34364, 34375, 34765, 34592, 34765, 34378, 34822, 34461, 34824, 34823, 34861, 34462, 34736, 33828, 36567, 32796, 36706, 37669, 34379, 34775);
DELETE FROM `character_queststatus_objectives` WHERE quest in (33815, 33868, 34402, 34364, 34375, 34765, 34592, 34765, 34378, 34822, 34461, 34824, 34823, 34861, 34462, 34736, 33828, 36567, 32796, 36706, 37669, 34379, 34775);
DELETE FROM `character_queststatus_rewarded` WHERE quest in (33815, 33868, 34402, 34364, 34375, 34765, 34592, 34765, 34378, 34822, 34461, 34824, 34823, 34861, 34462, 34736, 33828, 36567, 32796, 36706, 37669, 34379, 34775);