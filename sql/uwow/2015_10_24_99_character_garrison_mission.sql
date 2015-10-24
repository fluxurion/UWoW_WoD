DELETE FROM character_garrison_missions;
SET @n:=0;
INSERT INTO character_garrison_missions (dbId, guid, missionRecID, offerTime, offerDuration, startTime, travelDuration, missionDuration, missionState) 
SELECT @n:=@n+1, characters.guid, 3, 1445460599, 9999999, 0, 0, 900, 0 FROM characters, character_garrison WHERE characters.guid = character_garrison.guid;