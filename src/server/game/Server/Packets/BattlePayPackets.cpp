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

#include "BattlePayPackets.h"

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePay::DisplayInfo const& displayInfo)
{
    data.FlushBits();

    data.WriteBit(displayInfo.CreatureDisplayInfoID.is_initialized());
    data.WriteBit(displayInfo.FileDataID.is_initialized());

    data.WriteBits(displayInfo.Name1.length(), 10);
    data.WriteBits(displayInfo.Name2.length(), 10);
    data.WriteBits(displayInfo.Name3.length(), 13);

    data.WriteBit(displayInfo.Flags.is_initialized());

    if (displayInfo.CreatureDisplayInfoID.is_initialized())
        data << *displayInfo.CreatureDisplayInfoID;

    if (displayInfo.FileDataID)
        data << *displayInfo.FileDataID;

    data.WriteString(displayInfo.Name1);
    data.WriteString(displayInfo.Name2);
    data.WriteString(displayInfo.Name3);

    if (displayInfo.Flags)
        data << *displayInfo.Flags;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePay::Product const& product)
{
    data << product.ProductID;

    data << product.NormalPriceFixedPoint;
    data << product.CurrentPriceFixedPoint;

    data << product.Type;
    data << product.Flags;

    data.WriteBits(product.battlePayProduct.size(), 7);
    data.WriteBits(product.ChoiceType, 7);
    data.WriteBit(product.displayInfo.is_initialized());

    for (const auto productItem : product.battlePayProduct)
    {
        data << productItem.ID;
        data << productItem.ItemID;
        data << productItem.Quantity;

        data.WriteBit(productItem.displayInfo.is_initialized());
        data.WriteBit(productItem.HasPet);
        data.WriteBit(productItem.PetResult.is_initialized());

        if (productItem.PetResult)
            data.WriteBits(*productItem.PetResult, 4);

        if (productItem.displayInfo)
            data << *productItem.displayInfo;;
    }

    data.FlushBits();

    if (product.displayInfo)
        data << *product.displayInfo;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePay::DistributionObject const& object)
{
    data << object.DistributionID;

    data << object.Status;
    data << object.ProductID;

    data << object.TargetPlayer;
    data << object.TargetVirtualRealm;
    data << object.TargetNativeRealm;

    data << object.PurchaseID;
    data.WriteBit(object.product.is_initialized());
    data.WriteBit(object.Revoked);
    data.FlushBits();

    if (object.product)
        data << *object.product;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePay::Purchase const& purchase)
{
    data << purchase.PurchaseID;
    data << purchase.Status;
    data << purchase.ResultCode;
    data << purchase.ProductID;

    data.WriteBits(purchase.WalletName.length(), 8);
    data.WriteString(purchase.WalletName);

    return data;
}

WorldPacket const* WorldPackets::BattlePay::PurchaseListResponse::Write()
{
    _worldPacket << Result;
    _worldPacket << uint32(purchase.size());
    for (Purchase const& purchaseData : purchase)
        _worldPacket << purchaseData;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::BattlePay::DistributionListResponse::Write()
{
    _worldPacket << Result;
    _worldPacket << uint32(object.size());
    for (DistributionObject const& objectData : object)
        _worldPacket << objectData;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::BattlePay::DistributionUpdate::Write()
{
    _worldPacket << object;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::BattlePay::ProductListResponse::Write()
{
    _worldPacket << Result;
    _worldPacket << CurrencyID;

    _worldPacket << uint32(product.size());
    _worldPacket << uint32(productGroup.size());
    _worldPacket << uint32(shop.size());

    for (Product const& productData : product)
        _worldPacket << productData;

    for (ProductGroup const& productGroupData : productGroup)
    {
        _worldPacket << productGroupData.GroupID;
        _worldPacket << productGroupData.IconFileDataID;
        _worldPacket << productGroupData.DisplayType;
        _worldPacket << productGroupData.Ordering;

        _worldPacket.WriteBits(productGroupData.Name.length(), 8);
        _worldPacket.WriteString(productGroupData.Name);
    }

    for (ShopEntry const& shopData : shop)
    {
        _worldPacket << shopData.EntryID;
        _worldPacket << shopData.GroupID;
        _worldPacket << shopData.ProductID;
        _worldPacket << shopData.Ordering;
        _worldPacket << shopData.Flags;
        _worldPacket << shopData.BannerType;

        _worldPacket.WriteBit(shopData.displayInfo.is_initialized());
        _worldPacket.FlushBits();

        if (shopData.displayInfo)
            _worldPacket << *shopData.displayInfo;
    }

    return &_worldPacket;
}

void WorldPackets::BattlePay::StartPurchase::Read()
{
    _worldPacket >> ClientToken;
    _worldPacket >> ProductID;
    _worldPacket >> TargetCharacter;
}

WorldPacket const* WorldPackets::BattlePay::StartPurchaseResponse::Write()
{
    _worldPacket << PurchaseID;
    _worldPacket << PurchaseResult;
    _worldPacket << ClientToken;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::BattlePay::PurchaseUpdate::Write()
{
    _worldPacket << uint32(purchase.size());
    for (Purchase const& purchaseData : purchase)
        _worldPacket << purchaseData;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::BattlePay::ConfirmPurchase::Write()
{
    _worldPacket << PurchaseID;
    _worldPacket << CurrentPriceFixedPoint;
    _worldPacket << ServerToken;

    return &_worldPacket;
}

void WorldPackets::BattlePay::ConfirmPurchaseResponse::Read()
{
    ConfirmPurchase = _worldPacket.ReadBit();
    _worldPacket >> ServerToken;
    _worldPacket >> ClientCurrentPriceFixedPoint;
}

WorldPacket const* WorldPackets::BattlePay::DeliveryEnded::Write()
{
    _worldPacket << DistributionID;

    _worldPacket << int32(item.size());
    for (WorldPackets::Item::ItemInstance const& itemData : item)
        _worldPacket << itemData;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::BattlePay::UpgradeStarted::Write()
{
    _worldPacket << CharacterGUID;

    return &_worldPacket;
}

void WorldPackets::BattlePay::DistributionAssignToTarget::Read()
{
    _worldPacket >> unk >> DistributionID >> TargetCharacter >> SpecializationID >> unk2;
}