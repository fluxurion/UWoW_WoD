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

#include "TokenPackets.h"

WorldPacket const* WorldPackets::Token::UpdateWowTokenAuctionableListResponse::Write()
{
    _worldPacket << UnkInt;
    _worldPacket << Result;
    _worldPacket << static_cast<uint32>(List.size());

    for (auto const& v : List)
    {
        _worldPacket << v.DistributionID;
        _worldPacket << uint32(v.DateCreated);
        _worldPacket << v.Owner;
        _worldPacket << v.BuyoutPrice;
        _worldPacket << v.EndTime;
    }

    return &_worldPacket;
}

