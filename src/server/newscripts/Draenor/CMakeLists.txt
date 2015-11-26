# Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  Draenor/dark_portal.cpp
  Draenor/frostfire_ridge.cpp
  Draenor/shadowmoon_valley.cpp
  Draenor/zone_ashran_battlefield.cpp

  Draenor/garrison/garrison_general.cpp
  Draenor/garrison/instance_garr.cpp

  Draenor/IronDocks/instance_iron_docks.cpp
  Draenor/IronDocks/boss_fleshrender_nokgar.cpp
  Draenor/IronDocks/boss_grimrail_enforcers.cpp
  Draenor/IronDocks/boss_oshir.cpp
  Draenor/IronDocks/boss_skulloc.cpp

  Draenor/BloodmaulSlagMines/bloodmaul_slag_mines.cpp
  Draenor/BloodmaulSlagMines/bloodmaul_slag_mines.h
  Draenor/BloodmaulSlagMines/bloodmaul_slag_mines_instance.cpp
  Draenor/BloodmaulSlagMines/boss_forgemaster_gogduh.cpp
  Draenor/BloodmaulSlagMines/boss_gugrokk.cpp
  Draenor/BloodmaulSlagMines/boss_roltall.cpp
  Draenor/BloodmaulSlagMines/boss_slave_watcher_crushto.cpp

  Draenor/UpperBlackrockSpire/instance_upper_blackrock_spire.cpp
  Draenor/UpperBlackrockSpire/boss_orebender_gorashan.cpp
  Draenor/UpperBlackrockSpire/boss_kyrak.cpp
  Draenor/UpperBlackrockSpire/boss_commander_tharbek.cpp
  Draenor/UpperBlackrockSpire/boss_ragewing_untamed.cpp
  Draenor/UpperBlackrockSpire/boss_warlord_zaela.cpp

  Draenor/ShadowmoonBurialGrounds/instance_shadowmoon_burial_grounds.cpp
  Draenor/ShadowmoonBurialGrounds/boss_sadana_bloodfury.cpp
  Draenor/ShadowmoonBurialGrounds/boss_nhallish.cpp
  Draenor/ShadowmoonBurialGrounds/boss_bonemaw.cpp
  Draenor/ShadowmoonBurialGrounds/boss_nerzhul.cpp

  Draenor/Skyreach/instance_skyreach.cpp
  Draenor/Skyreach/boss_ranjit.cpp
  Draenor/Skyreach/boss_araknath.cpp
  Draenor/Skyreach/boss_rukhran.cpp
  Draenor/Skyreach/boss_high_sage_viryx.cpp

  Draenor/TheEverbloom/instance_the_everbloom.cpp
  Draenor/TheEverbloom/boss_witherbark.cpp
  Draenor/TheEverbloom/boss_ancient_protectors.cpp
  Draenor/TheEverbloom/boss_xeritac.cpp
  Draenor/TheEverbloom/boss_archmage_sol.cpp
  Draenor/TheEverbloom/boss_yalnu.cpp
)

message("  -> Prepared: Draenor")
