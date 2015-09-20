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
            session->SendBattlePayDistribution(BATTLE_PAY_DIST_STATUS_AVAILABLE, 1);
            session->AddAuthFlag(AT_AUTH_FLAG_90_LVL_UP);
            return true;
        }
    }
    else if (product.Type == PRODUCT_TYPE_RESTORE_DELETE_CHARACTER)
    {
        if (!session->HasAuthFlag(AT_AUTH_FLAG_RESTORE_DELETED_CHARACTER))
        {
            session->AddAuthFlag(AT_AUTH_FLAG_RESTORE_DELETED_CHARACTER);
            session->SendFeatureSystemStatusGlueScreen();
            return true;
        }
    }
    else if (product.Type == PRODUCT_TYPE_FACTION_CHANGE)
    {
        if (Player* player = session->GetPlayer())
        {
            player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
            return true;
        }
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
        stmt->setInt32(0, AT_LOGIN_CHANGE_FACTION);
        stmt->setUInt64(1, targetGuid.GetCounter());
        CharacterDatabase.DirectExecute(stmt);

        session->SendCharacterEnum();
        
        return true;
    }
    else if (product.Type == PRODUCT_TYPE_RACE_CHAGE)
    {
        if (Player* player = session->GetPlayer())
        {
            player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
            return true;
        }

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
        stmt->setInt32(0, AT_LOGIN_CHANGE_RACE);
        stmt->setUInt64(1, targetGuid.GetCounter());
        CharacterDatabase.DirectExecute(stmt);

        session->SendCharacterEnum();
        return true;
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
            case 999:
                //! HandlePlayerLoginOpcode Level Up Loading.
                if (_charLoginCallback.valid() && _charLoginCallback.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
                {
                    SQLQueryHolder* param = _charLoginCallback.get();
                    HandlePlayerLevelUp((LoginQueryHolder*)param);
                }
                break;
            case 1000:  //level90UP
            {

                //check every second
                if (purchase.TimeOnProcess > 1000)
                {
                    purchase.TimeOnProcess = 0;

                    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_LEVEL);
                    stmt->setUInt64(0, purchase.TargetCharacter.GetCounter());
                    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
                    {
                        if ((*result)[0].GetUInt8() == 90)
                        {
                            //session->SendCharacterEnum(); //no need. query send after BATTLE_PAY_DIST_STATUS_PROCESS_COMPLETE
                            session->SendBattlePayDistribution(BATTLE_PAY_DIST_STATUS_PROCESS_COMPLETE, purchase.PurchaseID, purchase.TargetCharacter);

                            WorldPacket packet(SMSG_CHARACTER_UPGRADE_CHARACTER_CHOSEN);
                            packet.append(tmpPacket);
                            session->SendPacket(&packet);

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
    if (!session->HasAuthFlag(AT_AUTH_FLAG_90_LVL_UP))
        return;

    ChrSpecializationsEntry const* specialization = sChrSpecializationsStore.LookupEntry(packet.SpecializationID);
    if (!specialization)
        return;

    LoginQueryHolder *holder = new LoginQueryHolder(session->GetAccountId(), packet.TargetCharacter);
    if (!holder->Initialize())
    {
        delete holder;                                      // delete all unprocessed queries
        return;
    }
    _charLoginCallback = CharacterDatabase.DelayQueryHolder((SQLQueryHolder*)holder);

    //Add checker.
    ++PurchaseID;
    _store[PurchaseID].PurchaseID = packet.DistributionID;
    _store[PurchaseID].ProductID = PRODUCT_LEVEL_UP_90;
    _store[PurchaseID].TargetCharacter = packet.TargetCharacter;
    _store[PurchaseID].Status = 999;
    _store[PurchaseID].specID = packet.SpecializationID;
    _store[PurchaseID].factionChose = packet.unk2;

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

void BattlePayMgr::HandlePlayerLevelUp(LoginQueryHolder * holder)
{
    ObjectGuid playerGuid = holder->GetGuid();

    Player* pCurrChar = new Player(session);
    session->skip_send_packer = true;
    // "GetAccountId() == db stored account id" checked in LoadFromDB (prevent login not own character using cheating tools)
    if (!pCurrChar->LoadFromDB(playerGuid, holder))
    {
        session->skip_send_packer = false;                  // Enable send packets.
        delete pCurrChar;                                   // delete it manually
        delete holder;                                      // delete all unprocessed queries
        return;
    }

    auto data = _store.find(PurchaseID);
    if (data == _store.end())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "BattlePayMgr::HandlePlayerLevelUp no PurchaseID %u", PurchaseID);
        session->skip_send_packer = false;                  // Enable send packets.
        delete pCurrChar;                                   // delete it manually
        delete holder;                                      // delete all unprocessed queries
        return;
    }

    Purchase const &purchase = data->second;

    ChrSpecializationsEntry const* specialization = sChrSpecializationsStore.LookupEntry(purchase.specID);
    if (!specialization || specialization->ClassID != pCurrChar->getClass())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "BattlePayMgr::HandlePlayerLevelUp unsuported specID %u for class %u", purchase.specID, pCurrChar->getClass());
        session->skip_send_packer = false;                  // Enable send packets.
        delete pCurrChar;                                   // delete it manually
        delete holder;                                      // delete all unprocessed queries
        return;
    }

    //
    pCurrChar->setCinematic(1);

    //Professions.
    if (pCurrChar->getLevel() >= 60)
    {       
        if (pCurrChar->GetSkillValue(SKILL_FIRST_AID) < 600)
        {
            pCurrChar->learnSpell(110408, true);
            pCurrChar->SetSkill(SKILL_FIRST_AID, 600, 600, 600);
        }
        uint32 primeCount = pCurrChar->GetFreePrimaryProfessionPoints();
        uint32 skill_state = 0;

        if (pCurrChar->GetSkillValue(SKILL_PLATE_MAIL))
            skill_state = 2;
        else if (pCurrChar->GetSkillValue(SKILL_LEATHER) || pCurrChar->GetSkillValue(SKILL_MAIL))
            skill_state = 1;

        static const uint32 prof[3][2] =
        {
            { 110401, 110427 }, //SKILL_CLOTH
            { 110424, 102220 }, //SKILL_LEATHER
            { 110398, 102161 }, //SKILL_PLATE_MAIL
        };

        while (primeCount)
        {
            --primeCount;
            pCurrChar->learnSpell(prof[skill_state][primeCount], true);
        }

        for (auto data : pCurrChar->GetSpellMap())
        {
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(data.first))
                if (spellInfo->IsPrimaryProfessionFirstRank())
                {
                    --primeCount;
                    pCurrChar->learnSpellHighRank(data.first);
                    if (SpellLearnSkillNode const* spellLearnSkill = sSpellMgr->GetSpellLearnSkill(data.first))
                        pCurrChar->SetSkill(spellLearnSkill->skill, 600, 600, 600);
                }
        }
    }

    // Faction change for neutral pandaren
    if (pCurrChar->getRace() == RACE_PANDAREN_NEUTRAL)
    {
        //FROM WorldSession::HandleSetFactionOpcode
        if (!purchase.factionChose) //0 == HORDE
        {
            pCurrChar->SetByteValue(UNIT_FIELD_BYTES_0, 0, RACE_PANDAREN_HORDE);
            pCurrChar->setFactionForRace(RACE_PANDAREN_HORDE);
            WorldLocation location(1, 1349.72f, -4374.50f, 26.15f, M_PI);
            pCurrChar->SetHomebind(location, 363);
            pCurrChar->learnSpell(669, false); // Language Orcish
            pCurrChar->learnSpell(108127, false); // Language Pandaren
        }
        else
        {
            pCurrChar->SetByteValue(UNIT_FIELD_BYTES_0, 0, RACE_PANDAREN_ALLIANCE);
            pCurrChar->setFactionForRace(RACE_PANDAREN_ALLIANCE);
            WorldLocation location(0, -9076.77f, 424.74f, 92.42f, M_PI);
            pCurrChar->SetHomebind(location, 9);
            pCurrChar->learnSpell(668, false); // Language Common
            pCurrChar->learnSpell(108127, false); // Language Pandaren
        }
    }

    // Riding
    pCurrChar->learnSpell(34093, true);
    pCurrChar->learnSpell(54197, false);
    pCurrChar->learnSpell(90267, false);
    pCurrChar->learnSpell(115913, false);
    pCurrChar->learnSpell(pCurrChar->GetTeam() == HORDE ? 32243 : 32240, false);

    // Money
    pCurrChar->ModifyMoney(1500000, false);

    // Level
    pCurrChar->GiveLevel(90);
    pCurrChar->InitTalentForLevel();

    // Relocato to darkportal
    pCurrChar->SetLocationMapId(0);
    pCurrChar->Relocate(-11840.64f, -3215.719f, -29.41927f, 2.84771f);

    //
    if (pCurrChar->GetSpecializationId(2) == _store[PurchaseID].specID)
    {
        pCurrChar->SetActiveSpec(2);
    }
    else
    {
        if (pCurrChar->GetActiveSpec() == 2)
            pCurrChar->SetActiveSpec(1);

        pCurrChar->ResetSpec(false);
        pCurrChar->SetSpecializationId(pCurrChar->GetActiveSpec(), _store[PurchaseID].specID);
    }

    //New equipment
    uint32 bagEntry = 0;
    std::vector<uint32> items = GetItemLoadOutItems(LoadOutIdByClass(pCurrChar->getClass()));
    for (uint32 itemID : items)
    {
        if (ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(itemID))
        {
            if (pProto->GetInventoryType() == INVTYPE_BAG)
            {
                bagEntry = itemID;
                break;
            }
        }
    }
    uint8 bcount = 0;
    std::vector<Item*> mailer;
    for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_BAG_END; i++)
    {
        if (Item* pItem = pCurrChar->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
        {
            if (i < INVENTORY_SLOT_BAG_START)
            {
                ItemPosCountVec sDest;
                InventoryResult msg = pCurrChar->CanStoreItem(NULL_BAG, NULL_SLOT, sDest, pItem, false);
                pCurrChar->RemoveItem(INVENTORY_SLOT_BAG_0, i, true);

                if (msg == EQUIP_ERR_OK)
                    pItem = pCurrChar->StoreItem(sDest, pItem, true);
                else
                    mailer.push_back(pItem);
            }
            //else if (bagEntry)
            //{
            //    if (Item* bagItem = Item::CreateItem(bagEntry, 1, pCurrChar))
            //    {
            //        ItemPosCountVec sDest;
            //        InventoryResult msg = pCurrChar->CanStoreItem(NULL_BAG, NULL_SLOT, sDest, bagItem, false);
            //        if (msg == EQUIP_ERR_OK)
            //        {
            //            bagItem = pCurrChar->StoreItem(sDest, bagItem, true);
            //            pCurrChar->SwapItem(bagItem->GetPos(), pItem->GetPos());
            //            ++bcount;
            //        }
            //    }
            //}
        }
    }


    if (!mailer.empty())
    {
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        MailSender sender(MAIL_NORMAL, playerGuid.GetCounter(), MAIL_STATIONERY_GM);
        MailDraft draft = MailDraft("LevelUP", "LevelUP");
                
        for (Item* item : mailer)
            draft.AddItem(item);

        draft.SendMailTo(trans, MailReceiver(NULL, playerGuid.GetCounter()), sender);
        CharacterDatabase.CommitTransaction(trans);
    }
    pCurrChar->SetFullHealth();

    //SMSG_CHARACTER_UPGRADE_CHARACTER_CHOSEN
    uint32 count = 0;

    tmpPacket.clear();
    tmpPacket << playerGuid;
    size_t wPos = tmpPacket.wpos();
    tmpPacket << uint32(count);
    for (uint32 itemID : items)
    {
        if (ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(itemID))
        {
            if (pProto->GetInventoryType() > INVTYPE_NON_EQUIP &&
                pProto->GetInventoryType() != INVTYPE_BAG)
            {
                tmpPacket << uint32(itemID);
                ++count;

            }
            if (pProto->GetInventoryType() != INVTYPE_BAG || bcount < 4)
            {
                if (pProto->GetInventoryType() == INVTYPE_BAG)
                    ++bcount;
                pCurrChar->StoreNewItemInBestSlots(itemID, 1);
            }
        }
    }
    tmpPacket.put<uint32>(wPos, count);

    // Draenor quest line
    if (Quest const* quest = sObjectMgr->GetQuestTemplate(34398))
        pCurrChar->AddQuest(quest, pCurrChar);

    //pCurrChar->CleanupsBeforeDelete(); //crashed on Motion Clear
    // Some custom clear
    if (pCurrChar->IsInWorld())
        pCurrChar->RemoveFromWorld();
    pCurrChar->RemoveAllAuras();
    pCurrChar->RemoveAllGameObjects();

    pCurrChar->SaveToDB();
    session->SetPlayer(NULL);

    session->skip_send_packer = false;                  // Enable send packets.
    _store[PurchaseID].Status = 1000;                   // Final Check
    delete pCurrChar;
    delete holder;
}