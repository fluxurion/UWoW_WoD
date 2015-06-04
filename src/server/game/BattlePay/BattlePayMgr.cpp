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

int32 BattlePayMgr::GetCoinsFromDB()
{
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_BATTLEPAY_COINS);
    stmt->setUInt32(0, session->GetAccountId());
    if (PreparedQueryResult result = LoginDatabase.Query(stmt))
        return (*result)[0].GetInt32();

    return 0;
}

//! Negative value ADD coins.
void BattlePayMgr::RemoveCoinsFromDB(int32 count)
{
    //UPDATE account SET coins = coins - ? WHERE id = ?

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_BATTLEPAY_CHANGE_COINS_COUNT);
    stmt->setInt32(0, count);
    stmt->setUInt32(1, session->GetAccountId());
    LoginDatabase.Execute(stmt);
}

//! ToDo: log for activation
//! If add to inventory save player.
bool BattlePayMgr::ActivateProduct(WorldPackets::BattlePay::Product product, uint64 productID)
{
    if (product.Type == PRODUCT_TYPE_ITEM)
    {

    }else if (product.Type == PRODUCT_TYPE_SERVICE)
    {
        //LevelUP
        if (product.ProductID == PRODUCT_LEVEL_UP_90 && !session->HasAuthFlag(AT_AUTH_FLAG_90_LVL_UP))
        {
            session->SendBattlePayDistribution(BATTLE_PAY_DIST_STATUS_AVAILABLE, productID);
            session->AddAuthFlag(AT_AUTH_FLAG_90_LVL_UP);
            return true;
        }
    }
    return false;
}

void BattlePayMgr::SendResult(uint64 pID, uint32 Product, uint32 Status, PurchaseResult erorResult/*=PURCHASE_RESULT_OK*/)
{
    WorldPackets::BattlePay::PurchaseUpdate result;
    WorldPackets::BattlePay::Purchase p;
    p.PurchaseID = pID;
    p.Status = Status;
    p.ResultCode = erorResult;
    p.ProductID = Product;
    //p.WalletName = "MasterCard";
    result.purchase.push_back(p);
    session->SendPacket(result.Write());
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
                //Check if we have money -> if true set 3 state, else send error.
                if ((purchase.Product.CurrentPriceFixedPoint / 10000) > GetCoinsFromDB())
                {
                    SendResult(purchase.PurchaseID, purchase.ProductID, purchase.Status, PURCHASE_RESULT_NOT_ENOUTH_MONEY);
                    _store.erase(data);
                    return;
                }
                purchase.Status = 3;
                break;
            }
            case 3: //Whaiting for purchase.
            {
                SendResult(purchase.PurchaseID, purchase.ProductID, purchase.Status);

                WorldPackets::BattlePay::ConfirmPurchase confirm;
                confirm.PurchaseID = purchase.PurchaseID;
                confirm.CurrentPriceFixedPoint = 0;
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

    auto productitr  = sObjectMgr->BattlePayProductMap.find(packet.ProductID);
    if (productitr == sObjectMgr->BattlePayProductMap.end())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: BattlePayMgr::StartPurchase use not existen product = %u add %u", packet.ProductID,session->GetAccountId());
        return;
    }

    ++PurchaseID;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: BattlePayMgr::StartPurchase start for token = %u productID %u purchaseID %u", packet.ClientToken, packet.ProductID, PurchaseID);

    _store[PurchaseID].PurchaseID = PurchaseID;
    _store[PurchaseID].ClientToken = packet.ClientToken;
    _store[PurchaseID].ProductID = packet.ProductID;
    _store[PurchaseID].Product = productitr->second;
    _store[PurchaseID].TargetCharacter = packet.TargetCharacter;
    _store[PurchaseID].Status = 2;

    WorldPackets::BattlePay::StartPurchaseResponse response;
    response.PurchaseID = PurchaseID;
    response.ClientToken = _store[PurchaseID].ClientToken;
    response.PurchaseResult = 0;            //! Statuses for 6.1.2: 25=0, 2=1, 12=2, 13-14=3, 28-29=4, 34=7, 1=8, 46=9, 47=10
    session->SendPacket(response.Write());

    SendResult(PurchaseID, packet.ProductID, _store[PurchaseID].Status);
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
            SendResult(purchase.PurchaseID, purchase.ProductID, 6, PURCHASE_RESULT_EMPTY);
            _store.erase(data);
            return;
        }

        if ((purchase.Product.CurrentPriceFixedPoint / 10000) > GetCoinsFromDB())
        {
            SendResult(purchase.PurchaseID, purchase.ProductID, 6, PURCHASE_RESULT_NOT_ENOUTH_MONEY);
            _store.erase(data);
            return;
        }

        if (ActivateProduct(purchase.Product, purchase.PurchaseID))
        {

            SendResult(purchase.PurchaseID, purchase.ProductID, 6);
            RemoveCoinsFromDB(purchase.Product.CurrentPriceFixedPoint / 10000);
        }else
        {
            SendResult(purchase.PurchaseID, purchase.ProductID, 0, PURCHASE_RESULT_COULNOT_BUY);
            _store.erase(data);
        }
        break;
    }
}