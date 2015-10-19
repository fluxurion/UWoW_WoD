CREATE TABLE `spell_aura_trigger` (
  `spell_id` mediumint(8) NOT NULL,
  `spell_trigger` mediumint(8) NOT NULL,
  `option` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `target` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `caster` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `targetaura` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `bp0` float NOT NULL DEFAULT '0',
  `bp1` float NOT NULL DEFAULT '0',
  `bp2` float NOT NULL DEFAULT '0',
  `effectmask` tinyint(3) NOT NULL DEFAULT '7',
  `aura` mediumint(8) NOT NULL DEFAULT '0',
  `chance` mediumint(8) NOT NULL DEFAULT '0',
  `comment` text NOT NULL,
  PRIMARY KEY (`spell_id`,`spell_trigger`,`option`,`aura`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

insert into `spell_aura_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) values
('145715','0','0','0','0','0','0','0','0','7','0','0','Gusting Bomb'),
('146198','148008','1','0','0','0','0','0','0','7','0','0','Essence of Yu\'lon'),
('146199','-148956','3','3','0','0','0','0','0','2','0','50','Spirit of Chi-Ji'),
('146195','-148957','3','3','0','0','0','0','0','2','0','50','Flurry of Xuen'),
('146193','-148958','3','3','0','0','0','0','0','2','0','50','Endurance of Niuzao'),
('146197','-148954','3','3','0','0','0','0','0','2','0','50','Essence of Yu\'lon'),
('146285','146293','1','0','0','0','0','0','0','7','0','0','Cruelty'),
('146184','146202','1','0','0','0','0','0','0','7','0','0','Wrath'),
('113957','113950','0','3','5','0','0','0','0','7','0','0','Cooking'),
('125950','125953','0','0','0','0','0','0','0','7','0','0','Soothing Mist');
