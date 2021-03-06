/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WorldStatePackets_h__
#define WorldStatePackets_h__

#include "Packet.h"

namespace WorldPackets
{
    namespace WorldState
    {
        class InitWorldStates final : public ServerPacket
        {
        public:
            struct WorldStateInfo
            {
                WorldStateInfo(WorldStates variableID, int32 value)
                    : VariableID(variableID), Value(value)
                { }

                WorldStates VariableID;
                int32 Value;
            };

            InitWorldStates();

            WorldPacket const* Write() override;

            uint32 AreaID = 0;
            uint32 SubareaID = 0;
            uint32 MapID = 0;

            std::list<WorldStateInfo> Worldstates;
        };

        class UpdateWorldState final : public ServerPacket
        {
        public:
            UpdateWorldState();

            WorldPacket const* Write() override;

            int32 Value = 0;
            bool Hidden = false;
            WorldStates VariableID = WorldStates::WS_NONE;
        };
    }
}

#endif // WorldStatePackets_h__
