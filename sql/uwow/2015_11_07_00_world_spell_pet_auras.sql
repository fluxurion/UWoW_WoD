ALTER TABLE `spell_pet_auras`   
  DROP PRIMARY KEY,
  ADD PRIMARY KEY (`petEntry`, `spellId`, `option`, `aura`, `bp0`, `createdspell`, `target`);