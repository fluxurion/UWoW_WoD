UPDATE characters SET speccount = 1 WHERE speccount > 1;
UPDATE characters SET specialization2 = 0, activespec = 0;

DELETE FROM character_spell WHERE spell IN (63645, 63644);
UPDATE characters SET at_login = at_login | 0x4;