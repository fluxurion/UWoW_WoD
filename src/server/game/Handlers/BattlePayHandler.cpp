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
void WorldSession::SendBattlePayDistribution(uint8 status, uint64 DistributionID, ObjectGuid TargetPlayer /*=ObjectGuid::Empty*/)
{
    auto productitr  = sObjectMgr->BattlePayProductMap.find(PRODUCT_LEVEL_UP_90);
    if (productitr == sObjectMgr->BattlePayProductMap.end())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: SendBattlePayDistribution use not existen product = %u add %u", PRODUCT_LEVEL_UP_90, GetAccountId());
        return;
    }
    WorldPackets::BattlePay::Product product = productitr->second;

    WorldPackets::BattlePay::DistributionUpdate distributionBattlePay;
    distributionBattlePay.object.DistributionID = DistributionID;
    distributionBattlePay.object.Status = status;
    distributionBattlePay.object.ProductID = PRODUCT_LEVEL_UP_90;

    if (TargetPlayer)
    {
        distributionBattlePay.object.TargetPlayer = TargetPlayer;
        distributionBattlePay.object.TargetVirtualRealm = GetVirtualRealmAddress();
        distributionBattlePay.object.TargetNativeRealm = GetVirtualRealmAddress();
    }

    distributionBattlePay.object.product = product;

    SendPacket(distributionBattlePay.Write());
}

void WorldSession::HandleBattlePayDistributionAssign(WorldPackets::BattlePay::DistributionAssignToTarget& packet)
{
    _battlePay->LevelUp(packet);
}

void WorldSession::HandleBattlePayProductList(WorldPacket& /*recvPacket*/)
{
    WorldPackets::BattlePay::ProductListResponse response;
    response.Result = 0;
    response.CurrencyID = CURRENCY_RUB;
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