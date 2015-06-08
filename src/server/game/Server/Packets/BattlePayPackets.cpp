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

    data.WriteBit(displayInfo.CreatureDisplayInfoID.HasValue);
    data.WriteBit(displayInfo.FileDataID.HasValue);

    data.WriteBits(displayInfo.Name1.length(), 10);
    data.WriteBits(displayInfo.Name2.length(), 10);
    data.WriteBits(displayInfo.Name3.length(), 13);

    data.WriteBit(displayInfo.Flags.HasValue);

    if (displayInfo.CreatureDisplayInfoID.HasValue)
        data << displayInfo.CreatureDisplayInfoID.Value;

    if (displayInfo.FileDataID.HasValue)
        data << displayInfo.FileDataID.Value;

    data.WriteString(displayInfo.Name1);
    data.WriteString(displayInfo.Name2);
    data.WriteString(displayInfo.Name3);

    if (displayInfo.Flags.HasValue)
        data << displayInfo.Flags.Value;

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
    data.WriteBit(product.displayInfo.HasValue);

    for (const auto productItem : product.battlePayProduct)
    {
        data << productItem.ID;
        data << productItem.ItemID;
        data << productItem.Quantity;

        data.FlushBits();

        data.WriteBit(productItem.displayInfo.HasValue);
        data.WriteBit(productItem.HasPet);
        data.WriteBit(productItem.PetResult.HasValue);

        if (productItem.PetResult.HasValue)
            data.WriteBits(productItem.PetResult.Value, 4);

        if (productItem.displayInfo.HasValue)
            data << productItem.displayInfo.Value;;
    }

    data.FlushBits();

    if (product.displayInfo.HasValue)
        data << product.displayInfo.Value;

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

    data.WriteBit(object.product.HasValue);

    data.WriteBit(object.Revoked);

    data.FlushBits();

    if (object.product.HasValue)
        data << object.product.Value;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::BattlePay::Purchase const& purchase)
{
    data << purchase.PurchaseID;
    data << purchase.Status;
    data << purchase.ResultCode;
    data << purchase.ProductID;

    data.FlushBits();

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

        _worldPacket.FlushBits();

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

        _worldPacket.WriteBit(shopData.displayInfo.HasValue);
        _worldPacket.FlushBits();

        if (shopData.displayInfo.HasValue)
            _worldPacket << shopData.displayInfo.Value;
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