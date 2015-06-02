/*
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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
#include "ObjectMgr.h"
#include "BattlePayMgr.h"
#include "BattlePayPackets.h"
#include "WorldSession.h"

// g_purchaseID == __PAIR__(CGBattlePay::m_startedPurchaseID, CGBattlePay::ClientToken)
BattlePayMgr::BattlePayMgr(WorldSession* s) : session(s)
{

}

bool BattlePayMgr::existClientToken(uint32 token) const
{
    for (BattlePayPurshaseStore::const_iterator data = _store.begin(); data != _store.end(); ++data)
        if(data->second.ClientToken == token)
            return true;
    return false;
}

void BattlePayMgr::Update(uint32 const diff)
{
    for (BattlePayPurshaseStore::iterator data = _store.begin(); data != _store.end(); ++data)
    {
        Purchase &purchase = data->second;
        purchase.TimeOnProcess += diff;
        switch (purchase.Status)
        {
            case 2:
            {
                //Check if we have money ir true set 3 state, else send error.
                bool noMoney = true;
                if (noMoney)
                {
                    WorldPackets::BattlePay::PurchaseUpdate result;
                    WorldPackets::BattlePay::Purchase p;
                    p.PurchaseID = purchase.PurchaseID;
                    p.Status = purchase.Status;
                    p.ResultCode = PURCHASE_RESULT_NOT_ENOUTH_MONEY;
                    p.ProductID = purchase.ProductID;
                    result.purchase.push_back(p);
                    session->SendPacket(result.Write());

                    _store.erase(data);
                    return;
                }
                purchase.Status = 3;
                break;
            }
            case 3: //Whaiting for purchase.
            {
                WorldPackets::BattlePay::PurchaseUpdate result;
                WorldPackets::BattlePay::Purchase p;
                p.PurchaseID = purchase.PurchaseID;
                p.Status = purchase.Status;
                p.ResultCode = 0;
                p.ProductID = purchase.ProductID;
                result.purchase.push_back(p);
                session->SendPacket(result.Write());

                WorldPackets::BattlePay::ConfirmPurchase confirm;
                confirm.PurchaseID = purchase.PurchaseID;
                confirm.CurrentPriceFixedPoint = 139718536857744;
                confirm.ServerToken = purchase.PurchaseID;
                session->SendPacket(confirm.Write());

                purchase.Status = 100;  //whaiting for order
                break;
            }
            case 100: //after send 3 status whait if player comfirm it.
                break;
        }
    }
}

//! By normal way StartPurchase register client purshase.
void BattlePayMgr::StartPurchase(WorldPackets::BattlePay::StartPurchase const& packet)
{
    if (existClientToken(packet.ClientToken))
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: BattlePayMgr::StartPurchase already has client token on process = %u", packet.ClientToken);
        return;
    }

    ++PurchaseID;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: BattlePayMgr::StartPurchase start for token = %u purchaseID %u", packet.ClientToken, PurchaseID);

    _store[PurchaseID].PurchaseID = PurchaseID;
    _store[PurchaseID].ClientToken = packet.ClientToken;
    _store[PurchaseID].ProductID = packet.ProductID;
    _store[PurchaseID].TargetCharacter = packet.TargetCharacter;
    _store[PurchaseID].Status = 2;

    WorldPackets::BattlePay::StartPurchaseResponse response;
    response.PurchaseID = PurchaseID;
    response.ClientToken = _store[PurchaseID].ClientToken;
    response.PurchaseResult = 0;            //! Statuses for 6.1.2: 25=0, 2=1, 12=2, 13-14=3, 28-29=4, 34=7, 1=8, 46=9, 47=10
    session->SendPacket(response.Write());

    WorldPackets::BattlePay::PurchaseUpdate result;
    WorldPackets::BattlePay::Purchase purchase;
    purchase.PurchaseID = PurchaseID;
    purchase.Status = _store[PurchaseID].Status;
    purchase.ResultCode = 0;
    purchase.ProductID = packet.ProductID;
    result.purchase.push_back(purchase);
    session->SendPacket(result.Write());
}

void BattlePayMgr::ConfirmPurchaseResponse(uint32 ServerToken, bool ConfirmPurchase)
{
    for (BattlePayPurshaseStore::iterator data = _store.begin(); data != _store.end(); ++data)
    {
        Purchase &purchase = data->second;

        if (purchase.PurchaseID != ServerToken)
            continue;

        if (!ConfirmPurchase)
        {
            WorldPackets::BattlePay::PurchaseUpdate result;
            WorldPackets::BattlePay::Purchase p;
            p.PurchaseID = purchase.PurchaseID;
            p.Status = 6;
            p.ResultCode = 46;
            p.ProductID = purchase.ProductID;
            result.purchase.push_back(p);
            session->SendPacket(result.Write());

            _store.erase(data);
            return;
        }

        WorldPackets::BattlePay::PurchaseUpdate result;
        WorldPackets::BattlePay::Purchase p;
        p.PurchaseID = purchase.PurchaseID;
        p.Status = 6;    //5 - in process, 6 - finished.
        p.ResultCode = 0;
        p.ProductID = purchase.ProductID;
        result.purchase.push_back(p);
        session->SendPacket(result.Write());
        break;
    }
}