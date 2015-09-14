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

  Draenor/IronDocks/instance_iron_docks.cpp
  Draenor/IronDocks/boss_fleshrender_nokgar.cpp
  Draenor/IronDocks/boss_grimrail_enforcers.cpp
  Draenor/IronDocks/boss_oshir.cpp
  Draenor/IronDocks/boss_skulloc.cpp
)

message("  -> Prepared: Draenor")
