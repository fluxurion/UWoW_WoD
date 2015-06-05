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

#ifndef __TRINITY_BATTLEPAYMGR_H
#define __TRINITY_BATTLEPAYMGR_H

#include <map>
#include <string>

namespace WorldPackets
{

    namespace BattlePay
    {
        class DistributionAssignToTarget;
        class StartPurchase;
        class ConfirmPurchaseResponse;
    }
}

class WorldSession;

struct Purchase
{
    uint64 PurchaseID           = 0;
    uint32 Status               = 0;

    WorldPackets::BattlePay::Product Product;

    uint32 ClientToken          = 0;
    uint32 ProductID            = 0;
    ObjectGuid TargetCharacter;

    uint32 TimeOnProcess        = 0;
};

//6.1.2
enum PurchaseResult
{
    PURCHASE_RESULT_OK                      = 0,    //Real 5
    PURCHASE_RESULT_ERROR_WHILE_PURCHASE    = 25,   //Real 0
    PURCHASE_RESULT_NO_PAY_METHOD           = 2,    //Real 1
    PURCHASE_RESULT_ERROR_PLZ_LATER         = 13,   //Real 3
    PURCHASE_RESULT_NOT_ENOUTH_MONEY        = 28,   //Real 4
    PURCHASE_RESULT_DISABLE_PARENT_CONTROL  = 34,   //Real 7
    PURCHASE_RESULT_COULNOT_BUY             = 1,    //Real 8
    PURCHASE_RESULT_EMPTY                   = 46,   //Real 9
};

enum ProductType
{
    PRODUCT_TYPE_ITEM                       = 0,
    PRODUCT_TYPE_SERVICE                    = 2,
};

enum ProductConstant
{
    PRODUCT_LEVEL_UP_90                     = 83,
};

typedef std::map<uint32/*PurchaseID*/, Purchase> BattlePayPurshaseStore;

class BattlePayMgr
{
public:
    explicit BattlePayMgr(WorldSession* s);
    ~BattlePayMgr()
    {

    }

    int32 GetCoinsFromDB();
    void RemoveCoinsFromDB(int32 count);
    bool ActivateProduct(WorldPackets::BattlePay::Product product, uint64 distrID, ObjectGuid const& guid);
    void SendResult(uint64 pID, uint32 Product, uint32 Status, PurchaseResult erorResult=PURCHASE_RESULT_OK);

    void LevelUp(WorldPackets::BattlePay::DistributionAssignToTarget const& packet);
    void StartPurchase(WorldPackets::BattlePay::StartPurchase const&p);
    void ConfirmPurchaseResponse(uint32 ClientToken, bool ConfirmPurchase);
    bool existClientToken(uint32 token) const;

    BattlePayPurshaseStore const& GetStore() const { return _store; };
    void Update(uint32 const diff);
private:
    WorldSession *session;
    uint32 PurchaseID = 0;
    BattlePayPurshaseStore _store;

};

#endif