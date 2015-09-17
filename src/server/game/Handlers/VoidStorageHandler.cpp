/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include <list>
#include <vector>
#include <utility>
#include "ItemPackets.h"

//! 6.0.3
void WorldSession::SendVoidStorageTransferResult(VoidTransferError result)
{
    WorldPacket data(SMSG_VOID_TRANSFER_RESULT, 4);
    data << uint32(result);
    SendPacket(&data);
}

//! 6.0.3
void WorldSession::SendVoidStorageFailed(bool unk/*=false*/)
{
    WorldPacket data(SMSG_VOID_STORAGE_FAILED, 4);
    data << uint8(unk << 7);   //unk bit
    SendPacket(&data);
}

//! 6.0.3
void WorldSession::HandleVoidStorageUnlock(WorldPacket& recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_VOID_STORAGE_UNLOCK");
    Player* player = GetPlayer();

    ObjectGuid npcGuid;
    recvData >> npcGuid;

    Creature* unit = player->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_VAULTKEEPER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageUnlock - Unit (GUID: %u) not found or player can't interact with it.", npcGuid.GetCounter());
        return;
    }

    if (player->IsVoidStorageUnlocked())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageUnlock - Player (GUID: %u, name: %s) tried to unlock void storage a 2nd time.", player->GetGUID().GetCounter(), player->GetName());
        return;
    }

    player->ModifyMoney(-int64(VOID_STORAGE_UNLOCK));
    player->UnlockVoidStorage();
}

//! 6.0.3
void WorldSession::HandleVoidStorageQuery(WorldPacket& recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_VOID_STORAGE_QUERY");
    Player* player = GetPlayer();

    ObjectGuid npcGuid;
    recvData >> npcGuid;

    Creature* unit = player->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_VAULTKEEPER);
    if (!unit)
    {
        SendVoidStorageFailed(true);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageQuery - Unit (GUID: %u) not found or player can't interact with it.", npcGuid.GetCounter());
        return;
    }

    if (!player->IsVoidStorageUnlocked())
    {
        SendVoidStorageFailed(true);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageQuery - Player (GUID: %u, name: %s) queried void storage without unlocking it.", player->GetGUID().GetCounter(), player->GetName());
        return;
    }

    WorldPackets::Item::VoidStorageContents packet;
    packet.Data.resize(VOID_STORAGE_MAX_SLOT);

    uint32 count = 0;
    for (uint16 i = 0; i < VOID_STORAGE_MAX_SLOT; ++i)
    {
        VoidStorageItem* item = player->GetVoidStorageItem(i);
        if (!item)
            continue;

        packet.Data[count].Guid = item->ItemId;
        packet.Data[count].Creator = item->CreatorGuid;
        packet.Data[count].Slot = i;    //slot conter really need. don't remove
        packet.Data[count].Item.ItemID = item->ItemEntry;
        packet.Data[count].Item.RandomPropertiesSeed = item->ItemSuffixFactor;
        packet.Data[count].Item.RandomPropertiesID = item->ItemRandomPropertyId;
        ++count;
    }
    packet.Data.resize(count);  //cut over-size

    SendPacket(packet.Write());
}

//! 6.0.3
void WorldSession::HandleVoidStorageTransfer(WorldPacket& recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_VOID_STORAGE_TRANSFER");
    Player* player = GetPlayer();

    // Read everything

    ObjectGuid npcGuid;
    uint32 countWithdraw, countDeposit = 0;

    recvData >> npcGuid >> countDeposit >> countWithdraw;

    std::vector<ObjectGuid> itemGuids(countDeposit);
    std::vector<ObjectGuid> itemIds(countWithdraw);

    if (countDeposit > 9)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) wants to deposit more than 9 items (%u).", player->GetGUID().GetCounter(), player->GetName(), countDeposit);
        return;
    }

    if (countWithdraw > 9)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) wants to withdraw more than 9 items (%u).", player->GetGUID().GetCounter(), player->GetName(), countWithdraw);
        return;
    }

    for (uint32 i = 0; i < countDeposit; ++i)
        recvData >> itemGuids[i];

    for (uint32 i = 0; i < countWithdraw; ++i)
        recvData >> itemIds[i];

    
    Creature* unit = player->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_VAULTKEEPER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Unit (GUID: %u) not found or player can't interact with it.", npcGuid.GetCounter());
        return;
    }

    if (!player->IsVoidStorageUnlocked())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) queried void storage without unlocking it.", player->GetGUID().GetCounter(), player->GetName());
        return;
    }

    if (itemGuids.size() > player->GetNumOfVoidStorageFreeSlots())
    {
        SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_FULL);
        return;
    }

    uint32 freeBagSlots = 0;
    if (itemIds.size() != 0)
    {
        // make this a Player function
        for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
            if (Bag* bag = player->GetBagByPos(i))
                freeBagSlots += bag->GetFreeSlots();
        for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
            if (!player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                ++freeBagSlots;
    }

    if (itemIds.size() > freeBagSlots)
    {
        SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_INVENTORY_FULL);
        return;
    }

    float auraCostMod = player->GetTotalAuraMultiplier(SPELL_AURA_MOD_VOID_STORAGE_AND_TRANSMOGRIFY_COST);
    int64 cost = uint64(itemGuids.size() * VOID_STORAGE_STORE_ITEM * auraCostMod);
    if (!player->HasEnoughMoney(cost))
    {
        SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_NOT_ENOUGH_MONEY);
        return;
    }

    Item* item = NULL;

    std::pair<VoidStorageItem, uint8> depositItems[VOID_STORAGE_MAX_DEPOSIT];
    uint8 depositCount = 0;
    for (std::vector<ObjectGuid>::iterator itr = itemGuids.begin(); itr != itemGuids.end(); ++itr)
    {
        item = player->GetItemByGuid(*itr);
        if (!item)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) wants to deposit an invalid item (item guid: " UI64FMTD ").", player->GetGUID().GetCounter(), player->GetName(), (*itr).GetCounter());
            continue;
        }

        VoidStorageItem itemVS(sObjectMgr->GenerateVoidStorageItemId(), item->GetEntry(),item->GetGuidValue(ITEM_FIELD_CREATOR), item->GetItemRandomPropertyId(), item->GetItemSuffixFactor(), false);

        uint8 slot = player->AddVoidStorageItem(itemVS);

        depositItems[depositCount++] = std::make_pair(itemVS, slot);

        player->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);
    }

    cost = int64(depositCount * VOID_STORAGE_STORE_ITEM * auraCostMod);

    player->ModifyMoney(-cost);

    VoidStorageItem withdrawItems[VOID_STORAGE_MAX_WITHDRAW];

    VoidStorageItem* itemVS = NULL;
    uint8 withdrawCount = 0;
    uint32 slot = 0;
    ItemPosCountVec dest;
    InventoryResult msg;

    if (sObjectMgr->IsPlayerInLogList(player))
    {
        sObjectMgr->DumpDupeConstant(player);
        sLog->outDebug(LOG_FILTER_DUPE, "---HandleVoidStorageTransfer; depositCount: %u;", depositCount);
    }

    for (std::vector<ObjectGuid>::iterator itr = itemIds.begin(); itr != itemIds.end(); ++itr)
    {
        itemVS = player->GetVoidStorageItem(*itr, slot);
        if (!itemVS)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) tried to withdraw an invalid item (id: " UI64FMTD ")", player->GetGUID().GetCounter(), player->GetName(), itr->GetCounter());
            continue;
        }
     
        dest.clear();

        msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemVS->ItemEntry, 1);
        if (msg != EQUIP_ERR_OK)
        {
            SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_INVENTORY_FULL);
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) couldn't withdraw item id " UI64FMTD " because inventory was full.", player->GetGUID().GetCounter(), player->GetName(), itr->GetCounter());
            return;
        }

        item = player->StoreNewItem(dest, itemVS->ItemEntry, true, itemVS->ItemRandomPropertyId);
        item->SetGuidValue(ITEM_FIELD_CREATOR, itemVS->CreatorGuid);
        item->SetBinding(true);
        player->SendNewItem(item, 1, false, false, false);

        withdrawItems[withdrawCount++] = *itemVS;

        player->DeleteVoidStorageItem(slot);
    }

    player->SaveToDB();

    WorldPackets::Item::VoidStorageTransferChanges packet;
    packet.Data.resize(depositCount);
    packet.RemovedItemsGuid.resize(withdrawCount);

    for (uint8 i = 0; i < depositCount; ++i)
    {
        packet.Data[i].Guid = depositItems[i].first.ItemId;
        packet.Data[i].Creator = depositItems[i].first.CreatorGuid;
        packet.Data[i].Slot = depositItems[i].second;
        packet.Data[i].Item.ItemID = depositItems[i].first.ItemEntry;
        packet.Data[i].Item.RandomPropertiesSeed = depositItems[i].first.ItemSuffixFactor;
        packet.Data[i].Item.RandomPropertiesID = depositItems[i].first.ItemRandomPropertyId;
    }

    for (uint8 i = 0; i < withdrawCount; ++i)
        packet.RemovedItemsGuid[i] = withdrawItems[i].ItemId;

    SendPacket(packet.Write());

    SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_NO_ERROR);
}

//! 6.0.3
void WorldSession::HandleVoidSwapItem(WorldPacket& recvData)
{
    Player* player = GetPlayer();
    uint32 newSlot;
    ObjectGuid npcGuid;
    ObjectGuid itemId;

    recvData >> npcGuid >> itemId >> newSlot;

    Creature* unit = player->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_VAULTKEEPER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidSwapItem - Unit (GUID: %u) not found or player can't interact with it.", npcGuid.GetCounter());
        return;
    }

    if (!player->IsVoidStorageUnlocked())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidSwapItem - Player (GUID: %u, name: %s) queried void storage without unlocking it.", player->GetGUID().GetCounter(), player->GetName());
        return;
    }

    uint32 oldSlot;
    if (!player->GetVoidStorageItem(itemId, oldSlot))
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidSwapItem - Player (GUID: %u, name: %s) requested swapping an invalid item (slot: %u, itemid: " UI64FMTD ").", player->GetGUID().GetCounter(), player->GetName(), newSlot, itemId.GetCounter());
        return;
    }

    if (sObjectMgr->IsPlayerInLogList(player))
    {
        sObjectMgr->DumpDupeConstant(player);
        sLog->outDebug(LOG_FILTER_DUPE, "---HandleVoidSwapItem; oldSlot: %u; newSlot %u", oldSlot, newSlot);
    }

    bool usedSrcSlot = player->GetVoidStorageItem(oldSlot) != NULL; // should be always true
    bool usedDestSlot = player->GetVoidStorageItem(newSlot) != NULL;
    ObjectGuid itemIdDest;
    if (usedDestSlot)
        itemIdDest = player->GetVoidStorageItem(newSlot)->ItemId;

    if (!player->SwapVoidStorageItem(oldSlot, newSlot))
    {
        SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_INTERNAL_ERROR_1);
        return;
    }

    //! 6.0.3
    WorldPacket data(SMSG_VOID_ITEM_SWAP_RESPONSE, 1 + (usedSrcSlot + usedDestSlot) * (1 + 7 + 4));   
    data << itemId << newSlot << itemIdDest << oldSlot;
    SendPacket(&data);
}
