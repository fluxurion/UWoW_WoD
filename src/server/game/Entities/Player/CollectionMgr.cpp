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

#include "CollectionMgr.h"
#include "Player.h"

void CollectionMgr::SaveToDB(SQLTransaction& trans)
{
    PreparedStatement* stmt = nullptr;
    uint8 index = 0;

    for (auto const& i : _toys)
    {
        index = 0;
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_TOYS);
        stmt->setUInt32(index++, _owner->GetSession()->GetAccountId());
        stmt->setUInt32(index++, i.first);
        stmt->setBool(index++, i.second);
        trans->Append(stmt);
    }

    for (auto const& t : _heirlooms)
    {
        index = 0;
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_HEIRLOOMS);
        stmt->setUInt32(index++, _owner->GetSession()->GetAccountId());
        stmt->setUInt32(index++, t.first);
        stmt->setUInt32(index++, t.second.flags);
        trans->Append(stmt);
    }
}

bool CollectionMgr::LoadFromDB(PreparedQueryResult toys, PreparedQueryResult heirlooms)
{
    if (toys)
    {
        do
        {
            Field* fields = toys->Fetch();
            uint32 itemId = fields[0].GetUInt32();
            bool isFavourite = fields[1].GetBool();

            _toys[itemId] = isFavourite;
        } while (toys->NextRow());
    }

    if (heirlooms)
    {
        do
        {
            Field* fields = heirlooms->Fetch();
            uint32 itemId = fields[0].GetUInt32();
            uint32 flags = fields[1].GetUInt32();

            HeirloomEntry const* heirloom = sDB2Manager.GetHeirloomByItemId(itemId);
            if (!heirloom)
                continue;

            uint32 bonusId = 0;

            if (flags & HEIRLOOM_FLAG_BONUS_LEVEL_90)
                bonusId = heirloom->ItemBonusListID[0];
            if (flags & HEIRLOOM_FLAG_BONUS_LEVEL_100)
                bonusId = heirloom->ItemBonusListID[1];

            _heirlooms[itemId] = HeirloomData(flags, bonusId);
        } while (heirlooms->NextRow());
    }
    
    for (auto const& t : _toys)
        _owner->AddDynamicValue(PLAYER_DYNAMIC_FIELD_TOYS, t.first);

    for (auto const& item : _heirlooms)
    {
        _owner->AddDynamicValue(PLAYER_DYNAMIC_FIELD_HEIRLOOMS, item.first);
        _owner->AddDynamicValue(PLAYER_DYNAMIC_FIELD_HEIRLOOM_FLAGS, item.second.flags);
    }

    return true;
}

bool CollectionMgr::AddToy(uint32 itemId, bool isFavourite /*= false*/)
{
    if (UpdateAccountToys(itemId, isFavourite))
    {
        _owner->AddDynamicValue(PLAYER_DYNAMIC_FIELD_TOYS, itemId);
        return true;
    }

    return false;
}

bool CollectionMgr::UpdateAccountToys(uint32 itemId, bool isFavourite /*= false*/)
{
    return _toys.insert(ToyBoxContainer::value_type(itemId, isFavourite)).second;
}

void CollectionMgr::ToySetFavorite(uint32 itemId, bool favorite)
{
    ToyBoxContainer::iterator itr = _toys.find(itemId);
    if (itr == _toys.end())
        return;

    itr->second = favorite;
}

bool CollectionMgr::UpdateAccountHeirlooms(uint32 itemId, uint32 flags)
{
    return _heirlooms.insert(HeirloomContainer::value_type(itemId, HeirloomData(flags, 0))).second;
}

uint32 CollectionMgr::GetHeirloomBonus(uint32 itemId) const
{
    HeirloomContainer::const_iterator z = _heirlooms.find(itemId);
    if (z != _heirlooms.end())
        return z->second.bonusId;

    return 0;
}

void CollectionMgr::AddHeirloom(uint32 itemId, uint32 flags)
{
    if (UpdateAccountHeirlooms(itemId, flags))
    {
        _owner->AddDynamicValue(PLAYER_DYNAMIC_FIELD_HEIRLOOMS, itemId);
        _owner->AddDynamicValue(PLAYER_DYNAMIC_FIELD_HEIRLOOM_FLAGS, flags);
    }
}

void CollectionMgr::UpgradeHeirloom(uint32 itemId, uint32 castItem)
{
    Player* player = _owner;
    if (!player)
        return;

    HeirloomEntry const* heirloom = sDB2Manager.GetHeirloomByItemId(itemId);
    if (!heirloom)
        return;

    HeirloomContainer::iterator s = _heirlooms.find(itemId);
    if (s == _heirlooms.end())
        return;

    uint32 flags = s->second.flags;
    uint32 bonusId = 0;

    if (heirloom->UpgradeItemID[0] == castItem)
    {
        flags |= HEIRLOOM_FLAG_BONUS_LEVEL_90;
        bonusId = heirloom->ItemBonusListID[0];
    }
    if (heirloom->UpgradeItemID[1] == castItem)
    {
        flags |= HEIRLOOM_FLAG_BONUS_LEVEL_100;
        bonusId = heirloom->ItemBonusListID[1];
    }

    for (Item* item : player->GetItemListByEntry(itemId, true))
        item->AddBonuses(bonusId);

    std::vector<uint32> const& fields = player->GetDynamicValues(PLAYER_DYNAMIC_FIELD_HEIRLOOMS);
    uint8 offset = std::find(fields.begin(), fields.end(), itemId) - fields.begin();

    player->SetDynamicUInt32Value(PLAYER_DYNAMIC_FIELD_HEIRLOOM_FLAGS, offset, flags);
    s->second.flags = flags;
    s->second.bonusId = bonusId;
}

void CollectionMgr::CheckHeirloomUpgrades(Item* item)
{
    Player* player = _owner;
    if (!player)
        return;

    if (HeirloomEntry const* heirloom = sDB2Manager.GetHeirloomByOldItem(item->GetEntry()))
    {
        player->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);

        ItemPosCountVec dest;
        InventoryResult msg = player->CanStoreNewItem(item->GetBagSlot(), item->GetSlot(), dest, heirloom->ItemID, 1);
        if (msg == EQUIP_ERR_OK)
        {
            Item* newItem = player->StoreNewItem(dest, heirloom->ItemID, true);

            std::vector<uint32> const& fields = newItem->GetDynamicValues(ITEM_DYNAMIC_FIELD_BONUSLIST_IDS);

            uint32 bonusId = GetHeirloomBonus(heirloom->ItemID);
            if (std::find(fields.begin(), fields.end(), bonusId) == fields.end())
                newItem->AddBonuses(bonusId);

            player->SendNewItem(newItem, 1, false, false);
        }

        return;
    }

    if (HeirloomEntry const* heirloom = sDB2Manager.GetHeirloomByItemId(item->GetEntry()))
    {
        HeirloomContainer::iterator v = _heirlooms.find(item->GetEntry());
        if (v == _heirlooms.end())
            return;

        uint32 heirloomItemId = heirloom->NextDifficultyItemID;
        uint32 newItemId = 0;
        while (HeirloomEntry const* heirloomDiff = sDB2Manager.GetHeirloomByItemId(heirloomItemId))
        {
            if (player->GetItemByEntry(heirloomDiff->ItemID))
                newItemId = heirloomDiff->ItemID;

            if (HeirloomEntry const* heirloomSub = sDB2Manager.GetHeirloomByItemId(heirloomDiff->NextDifficultyItemID))
            {
                heirloomItemId = heirloomSub->ItemID;
                continue;
            }

            break;
        }

        if (newItemId)
        {
            std::vector<uint32> const& fields = player->GetDynamicValues(PLAYER_DYNAMIC_FIELD_HEIRLOOMS);
            uint8 offset = std::find(fields.begin(), fields.end(), v->first) - fields.begin();

            player->SetDynamicUInt32Value(PLAYER_DYNAMIC_FIELD_HEIRLOOMS, offset, newItemId);
            player->SetDynamicUInt32Value(PLAYER_DYNAMIC_FIELD_HEIRLOOM_FLAGS, offset, 0);

            _heirlooms.erase(v);
            _heirlooms[newItemId] = 0;

            return;
        }

        std::vector<uint32> const& fields = item->GetDynamicValues(ITEM_DYNAMIC_FIELD_BONUSLIST_IDS);

        for (uint32 bonusId : fields)
            if (bonusId != v->second.bonusId)
                item->ClearDynamicValue(ITEM_DYNAMIC_FIELD_BONUSLIST_IDS);

        if (std::find(fields.begin(), fields.end(), v->second.bonusId) == fields.end())
            item->AddBonuses(v->second.bonusId);
    }
}

bool CollectionMgr::CanApplyHeirloomXpBonus(uint32 itemId, uint32 level)
{
    if (sDB2Manager.GetHeirloomByItemId(itemId))
    {
        HeirloomContainer::iterator b = _heirlooms.find(itemId);
        if (b == _heirlooms.end())
            return false;

        if (b->second.flags & HEIRLOOM_FLAG_BONUS_LEVEL_100)
            return level <= 100;
        else if (b->second.flags & HEIRLOOM_FLAG_BONUS_LEVEL_90)
            return level <= 90;
        else
            return level <= 60;
    }

    return true;
}
