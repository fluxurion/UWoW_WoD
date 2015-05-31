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

#ifndef BattlePayPackets_h__
#define BattlePayPackets_h__

#include "Packet.h"
#include "Packets/ItemPackets.h"
#include "WorldSession.h"

namespace WorldPackets
{
    namespace BattlePay
    {
        class GetProductList final : public ClientPacket
        {
        public:
            GetProductList(WorldPacket&& packet) : ClientPacket(CMSG_BATTLE_PAY_GET_PRODUCT_LIST, std::move(packet)) { }

            void Read() override { }
        };

        class GetPurchaseListQuery final : public ClientPacket
        {
        public:
            GetPurchaseListQuery(WorldPacket&& packet) : ClientPacket(CMSG_BATTLE_PAY_GET_PURCHASE_LIST, std::move(packet)) { }

            void Read() override { }
        };

        struct DisplayInfo
        {
            Optional<uint32> CreatureDisplayInfoID;
            Optional<uint32> FileDataID;
            std::string Name1;
            std::string Name2;
            std::string Name3;
            Optional<uint32> Flags;
        };

        struct ProductItem
        {
            uint32 ID = 0;
            uint32 ItemID = 0;
            uint32 Quantity = 0;
            Optional<DisplayInfo> displayInfo;
            bool HasPet = false;
            Optional<uint16> PetResult;
        };

        struct Product
        {
            uint32 ProductID = 0;
            uint64 NormalPriceFixedPoint = 0;
            uint64 CurrentPriceFixedPoint = 0;
            std::vector<ProductItem> battlePayProduct;
            uint8 ChoiceType = 0;
            Optional<DisplayInfo> displayInfo;
            uint8 Type = 0;
            uint32 Flags = 0;
        };

        struct DistributionObject
        {
            uint64 DistributionID = 0;
            uint32 Status = 0;
            uint32 ProductID = 0;
            ObjectGuid TargetPlayer;
            uint32 TargetVirtualRealm = 0;
            uint32 TargetNativeRealm = 0;
            uint64 PurchaseID = 0;
            Optional<Product> product;
            bool Revoked = false;
        };

        struct Purchase
        {
            uint64 PurchaseID = 0;
            uint32 Status = 0;
            uint32 ResultCode = 0;
            uint32 ProductID = 0;
            std::string WalletName;
        };

        class PurchaseListResponse final : public ServerPacket
        {
        public:
            PurchaseListResponse() : ServerPacket(SMSG_BATTLE_PAY_GET_PURCHASE_LIST_RESPONSE, 100) { }

            WorldPacket const* Write() override;

            uint32 Result = 0;
            std::vector<Purchase> purchase;
        };

        class DistributionListResponse final : public ServerPacket
        {
        public:
            DistributionListResponse() : ServerPacket(SMSG_BATTLE_PAY_GET_DISTRIBUTION_LIST_RESPONSE, 100) { }

            WorldPacket const* Write() override;

            uint32 Result = 0;
            std::vector<DistributionObject> object;
        };

        class DistributionUpdate final : public ServerPacket
        {
        public:
            DistributionUpdate() : ServerPacket(SMSG_BATTLE_PAY_DISTRIBUTION_UPDATE, 100) { }

            WorldPacket const* Write() override;

            DistributionObject object;
        };

        struct ProductGroup
        {
            uint32 GroupID = 0;
            uint32 IconFileDataID = 0;
            uint8 DisplayType = 0;
            uint32 Ordering = 0;
            std::string Name;
        };

        struct ShopEntry
        {
            uint32 EntryID = 0;
            uint32 GroupID = 0;
            uint32 ProductID = 0;
            int32 Ordering = 0;
            uint32 Flags = 0;
            uint8 BannerType = 0;
            Optional<DisplayInfo> displayInfo;
        };

        class ProductListResponse final : public ServerPacket
        {
        public:
            ProductListResponse() : ServerPacket(SMSG_BATTLE_PAY_GET_PRODUCT_LIST_RESPONSE, 600) { }

            WorldPacket const* Write() override;

            uint32 Result = 0;
            uint32 CurrencyID = 0;
            std::vector<Product> product;
            std::vector<ProductGroup> productGroup;
            std::vector<ShopEntry> shop;
        };

        class StartPurchase final : public ClientPacket
        {
        public:
            StartPurchase(WorldPacket&& packet) : ClientPacket(CMSG_BATTLE_PAY_START_PURCHASE, std::move(packet)) { }

            void Read() override;

            uint32 ClientToken = 0;
            uint32 ProductID = 0;
            ObjectGuid TargetCharacter;
        };

        class StartPurchaseResponse final : public ServerPacket
        {
        public:
            StartPurchaseResponse() : ServerPacket(SMSG_BATTLE_PAY_START_PURCHASE_RESPONSE, 16) { }

            WorldPacket const* Write() override;

            uint64 PurchaseID = 0;
            uint32 ClientToken = 0;
            uint32 PurchaseResult = 0;
        };

        class PurchaseUpdate final : public ServerPacket
        {
        public:
            PurchaseUpdate() : ServerPacket(SMSG_BATTLE_PAY_PURCHASE_UPDATE, 600) { }

            WorldPacket const* Write() override;

            std::vector<Purchase> purchase;
        };

        class ConfirmPurchase final : public ServerPacket
        {
        public:
            ConfirmPurchase() : ServerPacket(SMSG_BATTLE_PAY_CONFIRM_PURCHASE, 600) { }

            WorldPacket const* Write() override;

            uint64 PurchaseID = 0;
            uint64 CurrentPriceFixedPoint = 0;
            uint32 ServerToken = 0;
        };

        class ConfirmPurchaseResponse final : public ClientPacket
        {
        public:
            ConfirmPurchaseResponse(WorldPacket&& packet) : ClientPacket(CMSG_BATTLE_PAY_CONFIRM_PURCHASE_RESPONSE, std::move(packet)) { }

            void Read() override;

            bool ConfirmPurchase = 0;
            uint32 ServerToken = 0;
            uint64 ClientCurrentPriceFixedPoint = 0;
        };

        class DeliveryEnded final : public ServerPacket
        {
        public:
            DeliveryEnded() : ServerPacket(SMSG_BATTLE_PAY_DELIVERY_ENDED, 600) { }

            WorldPacket const* Write() override;

            uint64 DistributionID = 0;
            std::vector<WorldPackets::Item::ItemInstance> item;
        };

        class UpgradeStarted final : public ServerPacket
        {
        public:
            UpgradeStarted() : ServerPacket(SMSG_CHARACTER_UPGRADE_STARTED) { }

            WorldPacket const* Write() override;

            ObjectGuid CharacterGUID;
        };

        class DistributionAssignToTarget final : public ClientPacket
        {
        public:
            DistributionAssignToTarget(WorldPacket&& packet) : ClientPacket(CMSG_BATTLE_PAY_DISTRIBUTION_ASSIGN_TO_TARGET, std::move(packet)) { }

            void Read() override;

            uint32 unk = 0;
            uint64 DistributionID = 0;
            uint32 SpecializationID = 0;
            ObjectGuid TargetCharacter;
        };
    }
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePay::DisplayInfo const& displayInfo);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePay::Product const& product);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePay::DistributionObject const& object);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePay::Purchase const& purchase);

#endif // BattlePayPackets_h__
