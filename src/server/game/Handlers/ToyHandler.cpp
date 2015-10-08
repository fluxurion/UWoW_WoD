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

#include "Item.h"
#include "ToyPackets.h"
#include "WorldSession.h"

void WorldSession::HandleAddToy(WorldPackets::Toy::AddToy& packet)
{
    if (!packet.Guid)
        return;
    Player* player = GetPlayer();
    if (!player)
        return;

    Item* item = player->GetItemByGuid(packet.Guid);
    if (!item)
    {
        player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, nullptr, nullptr);
        return;
    }

    //if (!GetToyItemIdMatch(item->GetEntry()))
    //    return;

    InventoryResult msg = player->CanUseItem(item);
    if (msg != EQUIP_ERR_OK)
    {
        player->SendEquipError(msg, item, nullptr);
        return;
    }

    if (player->GetCollectionMgr()->AddToy(item->GetEntry(), false))
        player->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);
}

void WorldSession::HandleUseToy(WorldPackets::Toy::UseToy& packet)
{
    ItemTemplate const* item = sObjectMgr->GetItemTemplate(packet.ItemID);
    if (!item)
        return;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(packet.Cast.SpellID);
    if (!spellInfo)
        return;

    Player* player = GetPlayer();
    if (!player)
        return;

    if (player->isPossessing())
        return;

    SpellCastTargets targets(player, packet.Cast);

    Spell* spell = new Spell(player, spellInfo, TRIGGERED_NONE, ObjectGuid::Empty, false);
    spell->m_castItemEntry = packet.ItemID;
    spell->m_cast_count = packet.Cast.CastID;
    spell->m_misc.Data = packet.Cast.Misc;
    spell->m_castFlagsEx |= CAST_FLAG_EX_USE_TOY_SPELL;
    spell->prepare(&targets);
}

void WorldSession::HandleToySetFavorite(WorldPackets::Toy::ToySetFavorite& packet)
{
    Player* player = GetPlayer();
    if (player)
        player->GetCollectionMgr()->ToySetFavorite(packet.ItemID, packet.Favorite);
}
