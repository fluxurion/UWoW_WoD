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
#include "Player.h"

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
bool BattlePayMgr::ActivateProduct(WorldPackets::BattlePay::Product product, uint64 productID, ObjectGuid const& targetGuid)
{
    if (product.Type == PRODUCT_TYPE_ITEM)
    {
        //ASSERT(!targetGuid.IsEmpty());

        Player* player = session->GetPlayer();
        WorldPackets::BattlePay::ProductItem pItem = *product.battlePayProduct.begin();

        //! If player online add to inventory
        if (player)
        {
            ItemPosCountVec dest;
            if (player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, pItem.ItemID, pItem.Quantity) == EQUIP_ERR_OK)
            {
                Item* item = player->StoreNewItem(dest, pItem.ItemID, true, Item::GenerateItemRandomPropertyId(pItem.ItemID));
                player->SendNewItem(item, pItem.Quantity, true, false, true);
                player->SaveToDB();
                return true;
            }
        }

        //! If no online or couldn't add item send mail.
        Item *attachment = Item::CreateItem ( pItem.ItemID, pItem.Quantity, player );
        if (!attachment)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: BattlePayMgr::ActivateProduct item = %u couldn't create add %u", pItem.ItemID,session->GetAccountId());
            return false;
        }

        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        attachment->SaveToDB(trans);

        MailSender sender(MAIL_NORMAL, targetGuid.GetCounter(), MAIL_STATIONERY_GM);
        MailDraft draft = MailDraft("BattlePay", "BattlePay");
        draft.AddItem(attachment);
        draft.SendMailTo(trans, MailReceiver(NULL, targetGuid.GetCounter()), sender);

        CharacterDatabase.CommitTransaction(trans);
        return true;

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
            case 1000:  //level90UP
            {
                //check every second
                if (purchase.TimeOnProcess % 1000 == 0)
                {
                    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_LEVEL);
                    stmt->setUInt64(0, purchase.TargetCharacter.GetCounter());
                    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
                    {
                        if ((*result)[0].GetUInt8() == 90)
                        {
                            //session->SendCharacterEnum(); //no need. query send after BATTLE_PAY_DIST_STATUS_PROCESS_COMPLETE
                            session->SendBattlePayDistribution(BATTLE_PAY_DIST_STATUS_PROCESS_COMPLETE, purchase.PurchaseID, purchase.TargetCharacter);
                            //SMSG_CHARACTER_UPGRADE_CHARACTER_CHOSEN
                            session->SendBattlePayDistribution(BATTLE_PAY_DIST_STATUS_FINISHED, purchase.PurchaseID, purchase.TargetCharacter);
                            session->RemoveAuthFlag(AT_AUTH_FLAG_90_LVL_UP);
                            _store.erase(data);
                            return;
                        }
                    }
                }
                break;
            }
        }
    }
}

void BattlePayMgr::LevelUp(WorldPackets::BattlePay::DistributionAssignToTarget const& packet)
{
    //          0           1           2               3               4
    //SELECT speccount, activespec, specialization1, specialization2, class FROM characters WHERE guid = ?
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_SPEC_DATA);
    stmt->setUInt64(0, packet.TargetCharacter.GetCounter());
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
        return;

    Field* fields = result->Fetch();
    uint8 speccount = fields[0].GetUInt8();
    uint8 activespec = fields[1].GetUInt8();
    uint32 specialization1 = fields[2].GetUInt8();
    uint32 specialization2 = fields[3].GetUInt8();
    uint8 plr_class = fields[4].GetUInt8();

    ChrSpecializationsEntry const* specialization = sChrSpecializationsStore.LookupEntry(packet.SpecializationID);
    if (!specialization || specialization->ClassID != plr_class)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: HandleBattlePayDistributionAssign non existen specializationID %u char %u", packet.SpecializationID, packet.TargetCharacter.GetCounter());
        return;
    }

    if (specialization2 == packet.SpecializationID)
    {
        activespec = 2;
    }else
    {
        specialization1 = packet.SpecializationID;
        if (activespec == 2)
            activespec = 1;
    }

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
    stmt->setUInt16(0, uint16(AT_LOGIN_LVL90_UP));
    stmt->setUInt64(1, packet.TargetCharacter.GetCounter());
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_LEVEL);
    stmt->setUInt16(0, uint16(90));
    stmt->setUInt64(1, packet.TargetCharacter.GetCounter());
    trans->Append(stmt);

    // Relocato to darkportal
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_POSITION);

    stmt->setFloat(0, -11840.64f);
    stmt->setFloat(1, -3215.719f);
    stmt->setFloat(2, -29.41927f);
    stmt->setFloat(3, 2.84771f);
    stmt->setUInt16(4, uint16(0));
    stmt->setUInt16(5, uint16(4));
    stmt->setUInt64(6, packet.TargetCharacter.GetCounter());
    trans->Append(stmt);

    // Set specialization
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_SPEC_DATA);
    stmt->setUInt8(0, activespec);
    stmt->setUInt32(1, specialization1);
    stmt->setUInt32(2, specialization2);
    stmt->setUInt64(3, packet.TargetCharacter.GetCounter());
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);

    //Add checker.
    _store[PurchaseID].PurchaseID = packet.DistributionID;
    _store[PurchaseID].ProductID = PRODUCT_LEVEL_UP_90;
    _store[PurchaseID].TargetCharacter = packet.TargetCharacter;
    _store[PurchaseID].Status = 1000;

    session->SendBattlePayDistribution(BATTLE_PAY_DIST_STATUS_ADD_TO_PROCESS, packet.DistributionID, packet.TargetCharacter);

    WorldPacket data(SMSG_CHARACTER_UPGRADE_STARTED);
    data << packet.TargetCharacter;
    session->SendPacket(&data);
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

        if (ActivateProduct(purchase.Product, purchase.PurchaseID, purchase.TargetCharacter))
        {
            SendResult(purchase.PurchaseID, purchase.ProductID, 6);
            RemoveCoinsFromDB(purchase.Product.CurrentPriceFixedPoint / 10000);

            if (Player* player = session->GetPlayer())
            {
                char buff[2048];
                sprintf(buff, session->GetTrinityString(LANG_BATTLE_PAY_SUCCES_COINS_MESSAGE), GetCoinsFromDB());
                sWorld->SendServerMessage(SERVER_MSG_STRING, buff);
            }

        }else
        {
            SendResult(purchase.PurchaseID, purchase.ProductID, 0, PURCHASE_RESULT_COULNOT_BUY);
            _store.erase(data);
        }
        break;
    }
}