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

#ifndef ToyPackets_h__
#define ToyPackets_h__

#include "Packet.h"
#include "ObjectGuid.h"
#include "SpellPackets.h"
#include "CollectionMgr.h"

namespace WorldPackets
{
    namespace Toy
    {
        class AddToy final : public ClientPacket
        {
        public:
            AddToy(WorldPacket&& packet) : ClientPacket(CMSG_ADD_TOY, std::move(packet)) { }

            void Read() override;

            ObjectGuid Guid;
        };

        class UseToy final : public ClientPacket
        {
        public:
            UseToy(WorldPacket&& packet) : ClientPacket(CMSG_USE_TOY, std::move(packet)) { }

            void Read() override;

            WorldPackets::Spells::SpellCastRequest Cast;
            uint32 ItemID = 0;
        };

        class AccountToysUpdate final : public ServerPacket
        {
        public:
            AccountToysUpdate() : ServerPacket(SMSG_ACCOUNT_TOYS_UPDATE, 1 + 4 + 1) { }

            WorldPacket const* Write() override;

            bool IsFullUpdate = false;
            ToyBoxContainer const* Toys = nullptr;
        };

        class ToySetFavorite final : public ClientPacket
        {
        public:
            ToySetFavorite(WorldPacket&& packet) : ClientPacket(CMSG_TOY_SET_FAVORITE, std::move(packet)) { }

            void Read() override;

            uint32 ItemID = 0;
            bool Favorite = false;
        };
    }
}

#endif // ToyPackets_h__
