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

#ifndef ItemPackets_h__
#define ItemPackets_h__

#include "Packet.h"
#include "Item.h"

namespace WorldPackets
{
    namespace Item
    {
        class BuyBackItem final : public ClientPacket
        {
        public:
            BuyBackItem(WorldPacket&& packet) : ClientPacket(CMSG_BUY_BACK_ITEM, std::move(packet)) { }

            void Read() override;

            ObjectGuid VendorGUID;
            uint32 Slot = 0;
        };

        class ItemRefundInfo final : public ClientPacket
        {
        public:
            ItemRefundInfo(WorldPacket&& packet) : ClientPacket(CMSG_GET_ITEM_PURCHASE_DATA, std::move(packet)) { }

            void Read() override;

            ObjectGuid ItemGUID;
        };

        class RepairItem final : public ClientPacket
        {
        public:
            RepairItem(WorldPacket&& packet) : ClientPacket(CMSG_REPAIR_ITEM, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            ObjectGuid ItemGUID;
            bool UseGuildBank = false;
        };

        class SellItem final : public ClientPacket
        {
        public:
            SellItem(WorldPacket&& packet) : ClientPacket(CMSG_SELL_ITEM, std::move(packet)) { }

            void Read() override;

            ObjectGuid VendorGUID;
            ObjectGuid ItemGUID;
            uint32 Amount = 0;
        };

        class ItemTimeUpdate final : public ServerPacket
        {
        public:
            ItemTimeUpdate() : ServerPacket(SMSG_ITEM_TIME_UPDATE, 8 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid ItemGuid;
            uint32 DurationLeft = 0;
        };

        class SetProficiency final : public ServerPacket
        {
        public:
            SetProficiency() : ServerPacket(SMSG_SET_PROFICIENCY, 5) { }

            WorldPacket const* Write() override;

            uint32 ProficiencyMask = 0;
            uint8 ProficiencyClass = 0;
        };

        struct ItemBonusInstanceData
        {
            uint8 Context                   = 0;
            std::vector<int32> BonusListIDs;
        };

        struct ItemInstance
        {
            uint32 ItemID                   = 0;
            uint32 RandomPropertiesSeed     = 0;
            uint32 RandomPropertiesID       = 0;
            Optional<ItemBonusInstanceData> ItemBonus;
            std::vector<int32> Modifications;
        };

        struct InvUpdate
        {
            struct InvItem
            {
                uint8 ContainerSlot = 0;
                uint8 Slot = 0;
            };

            std::vector<InvItem> Items;
        };

        class InventoryChangeFailure final : public ServerPacket
        {
        public:
            InventoryChangeFailure() : ServerPacket(SMSG_INVENTORY_CHANGE_FAILURE, 22) { }

            WorldPacket const* Write() override;

            int8 BagResult = EQUIP_ERR_OK; /// @see enum InventoryResult
            uint8 ContainerBSlot = 0;
            ObjectGuid SrcContainer;
            ObjectGuid DstContainer;
            int32 SrcSlot = 0;
            int32 LimitCategory = 0;
            int32 Level = 0;
            ObjectGuid Item[2];
        };

        class SplitItem final : public ClientPacket
        {
        public:
            SplitItem(WorldPacket&& packet) : ClientPacket(CMSG_SPLIT_ITEM, std::move(packet)) { }

            void Read() override;

            uint8 ToSlot       = 0;
            uint8 ToPackSlot   = 0;
            uint8 FromPackSlot = 0;
            int32 Quantity     = 0;
            InvUpdate Inv;
            uint8 FromSlot     = 0;
        };

        class SwapInvItem final : public ClientPacket
        {
        public:
            SwapInvItem(WorldPacket&& packet) : ClientPacket(CMSG_SWAP_INV_ITEM, std::move(packet)) { }

            void Read() override;

            InvUpdate Inv;
            uint8 Slot1 = 0; /// Source Slot
            uint8 Slot2 = 0; /// Destination Slot
        };

        class SwapItem final : public ClientPacket
        {
        public:
            SwapItem(WorldPacket&& packet) : ClientPacket(CMSG_SWAP_ITEM, std::move(packet)) { }

            void Read() override;

            InvUpdate Inv;
            uint8 SlotA          = 0;
            uint8 ContainerSlotB = 0;
            uint8 SlotB          = 0;
            uint8 ContainerSlotA = 0;
        };

        class AutoEquipItem final : public ClientPacket
        {
        public:
            AutoEquipItem(WorldPacket&& packet) : ClientPacket(CMSG_AUTO_EQUIP_ITEM, std::move(packet)) { }

            void Read() override;

            uint8 Slot = 0;
            InvUpdate Inv;
            uint8 PackSlot = 0;
        };

        class AutoStoreBagItem final : public ClientPacket
        {
        public:
            AutoStoreBagItem(WorldPacket&& packet) : ClientPacket(CMSG_AUTO_STORE_BAG_ITEM, std::move(packet)) { }

            void Read() override;

            uint8 ContainerSlotB = 0;
            InvUpdate Inv;
            uint8 ContainerSlotA = 0;
            uint8 SlotA = 0;
        };

        class DestroyItem final : public ClientPacket
        {
        public:
            DestroyItem(WorldPacket&& packet) : ClientPacket(CMSG_DESTROY_ITEM, std::move(packet)) { }

            void Read() override;

            uint32 Count = 0;
            uint8 SlotNum = 0;
            uint8 ContainerId = 0;
        };

        ByteBuffer& operator>>(ByteBuffer& data, InvUpdate& invUpdate);

        class BuyItem final : public ClientPacket
        {
        public:
            BuyItem(WorldPacket&& packet) : ClientPacket(CMSG_BUY_ITEM, std::move(packet)) { }

            void Read() override;

            ObjectGuid VendorGUID;
            ItemInstance Item;
            uint32 BagSlot = 0u;
            uint32 Slot = 0u;
            ItemVendorType ItemType = ITEM_VENDOR_TYPE_NONE;
            int32 Quantity = 0;
            ObjectGuid ContainerGUID;
        };

        class TransmogrigyItem final : public ClientPacket
        {
        public:
            TransmogrigyItem(WorldPacket&& packet) : ClientPacket(CMSG_TRANSMOGRIFY_ITEMS, std::move(packet)) { }

            void Read() override;

            uint32 Count;
            ObjectGuid NpcGUID;
            std::vector<ItemInstance> Items;
            std::vector<ObjectGuid> SrcItemGUID;
            std::vector<ObjectGuid> SrcVoidItemGUID;
            std::vector<uint32> Slots;
        };

        struct VoidStorageContentStruct
        {
            ItemInstance Item;
            ObjectGuid Guid;
            ObjectGuid Creator;
            uint32 Slot;
        };

        class VoidStorageContents final : public ServerPacket
        {
        public:
            VoidStorageContents() : ServerPacket(SMSG_VOID_STORAGE_CONTENTS) { }

            WorldPacket const* Write() override;

            std::vector<VoidStorageContentStruct> Data;
        };

        class VoidStorageTransferChanges final : public ServerPacket
        {
        public:
            VoidStorageTransferChanges() : ServerPacket(SMSG_VOID_STORAGE_TRANSFER_CHANGES) { }

            WorldPacket const* Write() override;

            std::vector<VoidStorageContentStruct> Data;
            std::vector<ObjectGuid> RemovedItemsGuid;
        };

        class ItemPushResult final : public ServerPacket
        {
        public:
            ItemPushResult() : ServerPacket(SMSG_ITEM_PUSH_RESULT) { }

            WorldPacket const* Write() override;

            ObjectGuid PlayerGUID;
            uint8 Slot;
            uint32 SlotInBag;
            ItemInstance Item;
            uint32 WodUnk;
            uint32 Quantity;
            uint32 QuantityInInventory;
            uint32 BattlePetBreedID;
            uint32 BattlePetBreedQuality;
            uint32 BattlePetSpeciesID;
            uint32 BattlePetLevel;
            ObjectGuid ItemGUID;
            bool Pushed;
            bool DisplayText;
            bool Created;
            bool IsBonusRoll;
        };
    }
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Item::ItemBonusInstanceData const& itemBonusInstanceData);
ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Item::ItemBonusInstanceData& itemBonusInstanceData);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Item::ItemInstance const& itemInstance);
ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Item::ItemInstance& itemInstance);

#endif // ItemPackets_h__
