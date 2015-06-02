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

#include "Opcodes.h"
#include "SharedDefines.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "AuthenticationPackets.h"
#include "ClientConfigPackets.h"
#include "SystemPackets.h"
#include "BattlePayPackets.h"
#include "BattlePayMgr.h"

void WorldSession::HandleBattlePayPurchaseListGet(WorldPacket& /*recvPacket*/)
{
    //CMSG_BATTLE_PAY_GET_PURCHASE_LIST

    if (HasAuthFlag(AT_AUTH_FLAG_90_LVL_UP))
        SendBattlePayDistribution(BATTLE_PAY_DIST_STATUS_AVAILABLE);

    WorldPacket data(SMSG_BATTLE_PAY_GET_DISTRIBUTION_LIST_RESPONSE, 7);
    data << uint32(0);
    data << uint32(0);
    SendPacket(&data);

    WorldPacket data2(SMSG_BATTLE_PAY_GET_PURCHASE_LIST_RESPONSE, 7);
    data2 << uint32(0);
    data2 << uint32(0);
    SendPacket(&data2);
}

//! 6.1.2
//! Send status apgrade for 90 lvl
void WorldSession::SendBattlePayDistribution(uint8 status, ObjectGuid TargetPlayer /*=ObjectGuid::Empty*/)
{
    //SMSG_BATTLE_PAY_DISTRIBUTION_UPDATE
    WorldPackets::BattlePay::DistributionUpdate distributionBattlePay;
    distributionBattlePay.object.DistributionID = 1;
    distributionBattlePay.object.Status = status;
    distributionBattlePay.object.ProductID = 88;

    if (TargetPlayer)
    {
        distributionBattlePay.object.TargetPlayer = TargetPlayer;
        distributionBattlePay.object.TargetVirtualRealm = GetVirtualRealmAddress();
        distributionBattlePay.object.TargetNativeRealm = GetVirtualRealmAddress();
    }

    WorldPackets::BattlePay::Product product;
    product.ProductID = 88;
    product.NormalPriceFixedPoint = 999900;
    product.CurrentPriceFixedPoint = 949905;
    product.ChoiceType = 1;
    product.Type = 2;
    product.Flags = 975;

    WorldPackets::BattlePay::DisplayInfo displayInfo;
    displayInfo.FileDataID.Set(614740);
    displayInfo.Name1 = "Up to 90 level!"; //Повышение до 90-го уровня
    displayInfo.Name3 = "Process your character to 90 level!"; //Повысьте уровень вашего персонажа до 90!
    product.displayInfo.Set(displayInfo);

    distributionBattlePay.object.product.Set(product);

    SendPacket(distributionBattlePay.Write());
}

//! 6.1.2
void WorldSession::HandleBattlePayDistributionAssign(WorldPackets::BattlePay::DistributionAssignToTarget& packet)
{
    SendBattlePayDistribution(BATTLE_PAY_DIST_STATUS_ADD_TO_PROCESS, packet.TargetCharacter);
}

//! 6.1.2
void WorldSession::HandleBattlePayProductList(WorldPacket& /*recvPacket*/)
{
    WorldPackets::BattlePay::ProductListResponse response;
    //CMSG_BATTLE_PAY_GET_PRODUCT_LIST
    response.Result = 0;
    response.CurrencyID = 5;

    response.product = sObjectMgr->productList.product;
    response.productGroup = sObjectMgr->productList.productGroup;
    response.shop = sObjectMgr->productList.shop;

    SendPacket(response.Write());
}

void WorldSession::HandleBattlePayStartPurchase(WorldPackets::BattlePay::StartPurchase& packet)
{
    _battlePay->StartPurchase(packet);
}

void WorldSession::HandleBattlePayConfirmPurchase(WorldPackets::BattlePay::ConfirmPurchaseResponse& packet)
{
    _battlePay->ConfirmPurchaseResponse(packet.ServerToken, packet.ConfirmPurchase);
}