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

#include "DatabaseEnv.h"
#include "Mail.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Language.h"
#include "WordFilterMgr.h"
#include "Chat.h"
#include "DBCStores.h"
#include "Item.h"
#include "AccountMgr.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "MailPackets.h"

//! 6.0.3
void WorldSession::HandleSendMail(WorldPackets::Mail::SendMail& packet)
{
    time_t now = time(NULL);
    if (now - timeLastHandleSendMail < 15)
    {
        _player->SendMailResult(0, MAIL_SEND, MAIL_ERR_INTERNAL_ERROR);
        SendNotification("You can't send mail more than once every few seconds");
        return;
    }
    else
       timeLastHandleSendMail = now;

    if (packet.Info.Attachments.size() > MAX_MAIL_ITEMS)        // client limit
    {
        GetPlayer()->SendMailResult(0, MAIL_SEND, MAIL_ERR_TOO_MANY_ATTACHMENTS);
        return;
    }

    sLog->outInfo(LOG_FILTER_NETWORKIO, "Player %u includes %u items, " UI64FMTD " copper and " UI64FMTD " COD copper with stationery = %u, package = %u",
    _player->GetGUID().GetCounter(), packet.Info.Attachments.size(), packet.Info.SendMoney, packet.Info.Cod, packet.Info.StationeryID, packet.Info.PackageID);

    // packet read complete, now do check

    if (!GetPlayer()->GetGameObjectIfCanInteractWith(packet.Info.Mailbox, GAMEOBJECT_TYPE_MAILBOX))
        return;

    if (packet.Info.Target.empty())
        return;

    Player* player = _player;

    if (player->getLevel() < sWorld->getIntConfig(CONFIG_MAIL_LEVEL_REQ))
    {
        SendNotification(GetTrinityString(LANG_MAIL_SENDER_REQ), sWorld->getIntConfig(CONFIG_MAIL_LEVEL_REQ));
        return;
    }

    // check msg to bad word
    if (sWorld->getBoolConfig(CONFIG_WORD_FILTER_ENABLE))
    {
        std::string badWord = sWordFilterMgr->FindBadWord(packet.Info.Subject, true);

        if (badWord.empty())
            badWord = sWordFilterMgr->FindBadWord(packet.Info.Body, true);

        if (!badWord.empty())
        {
            SendNotification(LANG_WORD_FILTER_FOUND_BAD_WORD_IN_MAIL, badWord.c_str());
            // send packet
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_MAIL_ATTACHMENT_INVALID);
            return;
        }
    }

    ObjectGuid receiverGuid;
    if (normalizePlayerName(packet.Info.Target))
        receiverGuid = ObjectMgr::GetPlayerGUIDByName(packet.Info.Target);

    if (!receiverGuid)
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "Player %u is sending mail to %s (GUID: not existed!) with subject %s and body %s includes %u items, " UI64FMTD " copper and " UI64FMTD " COD copper with stationery = %u, package = %u",
            player->GetGUID().GetCounter(), packet.Info.Target.c_str(), packet.Info.Subject.c_str(), packet.Info.Body.c_str(), packet.Info.Attachments.size(), packet.Info.SendMoney, packet.Info.Cod, packet.Info.StationeryID, packet.Info.PackageID);
        player->SendMailResult(0, MAIL_SEND, MAIL_ERR_RECIPIENT_NOT_FOUND);
        return;
    }

    sLog->outInfo(LOG_FILTER_NETWORKIO, "Player %u is sending mail to %s (GUID: %u) with subject %s and body %s includes %u items, " UI64FMTD " copper and " UI64FMTD " COD copper with stationery = %u, package = %u", player->GetGUID().GetCounter(), packet.Info.Target.c_str(), receiverGuid.GetCounter(), packet.Info.Subject.c_str(), packet.Info.Body.c_str(), packet.Info.Attachments.size(), packet.Info.SendMoney, packet.Info.Cod, packet.Info.StationeryID, packet.Info.PackageID);

    if (player->GetGUID() == receiverGuid)
    {
        player->SendMailResult(0, MAIL_SEND, MAIL_ERR_CANNOT_SEND_TO_SELF);
        return;
    }

    uint64 cost = !packet.Info.Attachments.empty() ? 30 * packet.Info.Attachments.size() : 30;  // price hardcoded in client

    uint64 reqmoney = cost + packet.Info.SendMoney;

    if (!player->HasEnoughMoney(reqmoney) && !player->isGameMaster())
    {
        player->SendMailResult(0, MAIL_SEND, MAIL_ERR_NOT_ENOUGH_MONEY);
        return;
    }

    Player* receive = ObjectAccessor::FindPlayer(receiverGuid);

    uint32 rc_team = 0;
    uint8 mails_count = 0;                                  //do not allow to send to one player more than 100 mails
    uint8 receiveLevel = 0;

    if (receive)
    {
        rc_team = receive->GetTeam();
        mails_count = receive->GetMailSize();
        receiveLevel = receive->getLevel();
    }
    else
    {
        rc_team = sObjectMgr->GetPlayerTeamByGUID(receiverGuid);

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_MAIL_COUNT);

        stmt->setUInt64(0, receiverGuid.GetCounter());

        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        if (result)
        {
            Field* fields = result->Fetch();
            mails_count = fields[0].GetUInt64();
        }

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_LEVEL);

        stmt->setUInt64(0, receiverGuid.GetCounter());

        result = CharacterDatabase.Query(stmt);

        if (result)
        {
            Field* fields = result->Fetch();
            receiveLevel = fields[0].GetUInt8();
        }
    }
    //do not allow to have more than 100 mails in mailbox.. mails count is in opcode uint8!!! - so max can be 255..
    if (mails_count > 100)
    {
        player->SendMailResult(0, MAIL_SEND, MAIL_ERR_RECIPIENT_CAP_REACHED);
        return;
    }
    // test the receiver's Faction... or all items are account bound
    bool accountBound = !packet.Info.Attachments.empty();
    for (auto const& att : packet.Info.Attachments)
    {
        Item* item = player->GetItemByGuid(att.ItemGUID);
        if (item)
        {
            ItemTemplate const* itemProto = item->GetTemplate();
            if (!itemProto || !(itemProto->Flags & ITEM_PROTO_FLAG_BIND_TO_ACCOUNT))
            {
                accountBound = false;
                break;
            }
        }
    }

    if (!accountBound && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_MAIL) && player->GetTeam() != rc_team && AccountMgr::IsPlayerAccount(GetSecurity()))
    {
        player->SendMailResult(0, MAIL_SEND, MAIL_ERR_NOT_YOUR_TEAM);
        return;
    }

    if (receiveLevel < sWorld->getIntConfig(CONFIG_MAIL_LEVEL_REQ))
    {
        SendNotification(GetTrinityString(LANG_MAIL_RECEIVER_REQ), sWorld->getIntConfig(CONFIG_MAIL_LEVEL_REQ));
        return;
    }

    uint32 rc_account = receive
        ? receive->GetSession()->GetAccountId()
        : ObjectMgr::GetPlayerAccountIdByGUID(receiverGuid);

    std::vector<Item*> items;

    for (auto const& att : packet.Info.Attachments)
    {
        if (att.ItemGUID.IsEmpty())
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_MAIL_ATTACHMENT_INVALID);
            return;
        }

        Item* item = player->GetItemByGuid(att.ItemGUID);

        // prevent sending bag with items (cheat: can be placed in bag after adding equipped empty bag to mail)
        if (!item)
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_MAIL_ATTACHMENT_INVALID);
            return;
        }

        if (!item->CanBeTraded(true))
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_MAIL_BOUND_ITEM);
            return;
        }

        if (item->IsBoundAccountWide() && item->IsSoulBound() && player->GetSession()->GetAccountId() != rc_account)
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_NOT_SAME_ACCOUNT);
            return;
        }

        if (item->GetTemplate()->Flags & ITEM_PROTO_FLAG_CONJURED || item->GetUInt32Value(ITEM_FIELD_EXPIRATION))
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_MAIL_BOUND_ITEM);
            return;
        }

        if (packet.Info.Cod && item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_CANT_SEND_WRAPPED_COD);
            return;
        }

        if (item->IsNotEmptyBag())
        {
            player->SendMailResult(0, MAIL_SEND, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_DESTROY_NONEMPTY_BAG);
            return;
        }

        items.push_back(item);
    }

    player->SendMailResult(0, MAIL_SEND, MAIL_OK);

    player->ModifyMoney(-int64(reqmoney));
    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL, cost);

    bool needItemDelay = false;

    MailDraft draft(packet.Info.Subject, packet.Info.Body);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    if (!packet.Info.Attachments.empty() || packet.Info.SendMoney > 0)
    {
        if (!packet.Info.Attachments.empty())
        {
            for (auto const& item : items)
            {
                if (!AccountMgr::IsPlayerAccount(GetSecurity()) && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
                {
                    sLog->outCommand(GetAccountId(), "GM %s (Account: %u) mail item: %s (Entry: %u Count: %u) to player: %s (Account: %u)",
                        GetPlayerName().c_str(), GetAccountId(), item->GetTemplate()->Name1.c_str(), item->GetEntry(), item->GetCount(), packet.Info.Target.c_str(), rc_account);
                }

                item->SetNotRefundable(GetPlayer()); // makes the item no longer refundable
                player->MoveItemFromInventory(item->GetBagSlot(), item->GetSlot(), true);

                item->DeleteFromInventoryDB(trans);     // deletes item from character's inventory
                item->SetOwnerGUID(receiverGuid);
                item->SaveToDB(trans);                  // recursive and not have transaction guard into self, item not in inventory and can be save standalone

                draft.AddItem(item);
            }

            // if item send to character at another account, then apply item delivery delay
            needItemDelay = player->GetSession()->GetAccountId() != rc_account;
        }

        if (packet.Info.SendMoney > 0 && !AccountMgr::IsPlayerAccount(GetSecurity()) && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
        {
            //TODO: charcter guid
            sLog->outCommand(GetAccountId(), "GM %s (Account: %u) mail money: " UI64FMTD " to player: %s (Account: %u)",
                GetPlayerName().c_str(), GetAccountId(), packet.Info.SendMoney, packet.Info.Target.c_str(), rc_account);
        }
    }

    // If theres is an item, there is a one hour delivery delay if sent to another account's character.
    uint32 deliver_delay = needItemDelay ? sWorld->getIntConfig(CONFIG_MAIL_DELIVERY_DELAY) : 0;

    // will delete item or place to receiver mail list
    draft
        .AddMoney(packet.Info.SendMoney)
        .AddCOD(packet.Info.Cod)
        .SendMailTo(trans, MailReceiver(receive, receiverGuid.GetCounter()), MailSender(player), packet.Info.Body.empty() ? MAIL_CHECK_MASK_COPIED : MAIL_CHECK_MASK_HAS_BODY, deliver_delay);

    player->SaveInventoryAndGoldToDB(trans);
    CharacterDatabase.CommitTransaction(trans);
}

//! 6.0.3
// Called when mail is read
void WorldSession::HandleMailMarkAsRead(WorldPackets::Mail::MailMarkAsRead& packet)
{
    if (!GetPlayer()->GetGameObjectIfCanInteractWith(packet.Mailbox, GAMEOBJECT_TYPE_MAILBOX))
        return;

    Player* player = _player;
    Mail* m = player->GetMail(packet.MailID);
    if (m)
    {
        if (player->unReadMails)
            --player->unReadMails;
        m->checked = m->checked | MAIL_CHECK_MASK_READ;
        player->m_mailsUpdated = true;
        m->state = MAIL_STATE_CHANGED;
    }
}

//! 6.0.3
// Called when client deletes mail
void WorldSession::HandleMailDelete(WorldPackets::Mail::MailDelete& packet)
{
    //if (!GetPlayer()->GetGameObjectIfCanInteractWith(mailbox, GAMEOBJECT_TYPE_MAILBOX))
    //    return;

    Mail* m = _player->GetMail(packet.MailID);
    Player* player = _player;
    player->m_mailsUpdated = true;
    if (m)
    {
        // delete shouldn't show up for COD mails
        if (m->COD)
        {
            player->SendMailResult(packet.MailID, MAIL_DELETED, MAIL_ERR_INTERNAL_ERROR);
            return;
        }

        m->state = MAIL_STATE_DELETED;
    }
    player->SendMailResult(packet.MailID, MAIL_DELETED, MAIL_OK);
}

//! 6.0.3
void WorldSession::HandleMailReturnToSender(WorldPackets::Mail::MailReturnToSender& packet)
{
    //if (!GetPlayer()->GetGameObjectIfCanInteractWith(mailbox, GAMEOBJECT_TYPE_MAILBOX))
    //    return;

    Player* player = _player;
    Mail* m = player->GetMail(packet.MailID);
    if (!m || m->state == MAIL_STATE_DELETED || m->deliver_time > time(NULL) || m->sender != packet.SenderGUID.GetCounter())
    {
        player->SendMailResult(packet.MailID, MAIL_RETURNED_TO_SENDER, MAIL_ERR_INTERNAL_ERROR);
        return;
    }
    //we can return mail now
    //so firstly delete the old one
    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_BY_ID);
    stmt->setUInt32(0, packet.MailID);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_ITEM_BY_ID);
    stmt->setUInt32(0, packet.MailID);
    trans->Append(stmt);

    player->RemoveMail(packet.MailID);

    // only return mail if the player exists (and delete if not existing)
    if (m->messageType == MAIL_NORMAL && m->sender)
    {
        MailDraft draft(m->subject, m->body);
        if (m->mailTemplateId)
            draft = MailDraft(m->mailTemplateId, false);     // items already included

        if (m->HasItems())
        {
            for (MailItemInfoVec::iterator itr2 = m->items.begin(); itr2 != m->items.end(); ++itr2)
            {
                if (Item* item = player->GetMItem(itr2->item_guid))
                    draft.AddItem(item);

                player->RemoveMItem(itr2->item_guid);
            }
        }
        draft.AddMoney(m->money).SendReturnToSender(GetAccountId(), m->receiver, m->sender, trans);
    }

    CharacterDatabase.CommitTransaction(trans);

    delete m;                                               //we can deallocate old mail
    player->SendMailResult(packet.MailID, MAIL_RETURNED_TO_SENDER, MAIL_OK);
}

//! 6.0.3
//called when player takes item attached in mail
void WorldSession::HandleMailTakeItem(WorldPackets::Mail::MailTakeItem& packet)
{
    uint32 AttachID = packet.AttachID;

    if (!GetPlayer()->GetGameObjectIfCanInteractWith(packet.Mailbox, GAMEOBJECT_TYPE_MAILBOX))
        return;

    Player* player = _player;

    Mail* m = player->GetMail(packet.MailID);
    if (!m || m->state == MAIL_STATE_DELETED || m->deliver_time > time(NULL))
    {
        player->SendMailResult(packet.MailID, MAIL_ITEM_TAKEN, MAIL_ERR_INTERNAL_ERROR);
        return;
    }

    // verify that the mail has the item to avoid cheaters taking COD items without paying
    if (std::find_if(m->items.begin(), m->items.end(), [AttachID](MailItemInfo info){ return info.item_guid == AttachID; }) == m->items.end())
    {
        player->SendMailResult(packet.MailID, MAIL_ITEM_TAKEN, MAIL_ERR_INTERNAL_ERROR);
        return;
    }

    // prevent cheating with skip client money check
    if (!player->HasEnoughMoney(uint64(m->COD)))
    {
        player->SendMailResult(packet.MailID, MAIL_ITEM_TAKEN, MAIL_ERR_NOT_ENOUGH_MONEY);
        return;
    }

    Item* item = player->GetMItem(packet.AttachID);
    if(!item)
        return;
    item->SetOwnerGUID(player->GetGUID());
    ItemPosCountVec dest;
    uint8 msg = _player->CanStoreItem(NULL_BAG, NULL_SLOT, dest, item, false);
    if (msg == EQUIP_ERR_OK)
    {
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        m->RemoveItem(packet.AttachID);
        m->removedItems.push_back(packet.AttachID);

        if (m->COD > 0)                                     //if there is COD, take COD money from player and send them to sender by mail
        {
            ObjectGuid sender_guid = ObjectGuid::Create<HighGuid::Player>(m->sender);
            Player* receive = ObjectAccessor::FindPlayer(sender_guid);

            uint32 sender_accId = 0;

            if (!AccountMgr::IsPlayerAccount(GetSecurity()) && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
            {
                std::string sender_name;
                if (receive)
                {
                    sender_accId = receive->GetSession()->GetAccountId();
                    sender_name = receive->GetName();
                }
                else
                {
                    // can be calculated early
                    sender_accId = ObjectMgr::GetPlayerAccountIdByGUID(sender_guid);

                    if (!ObjectMgr::GetPlayerNameByGUID(sender_guid, sender_name))
                        sender_name = sObjectMgr->GetTrinityStringForDBCLocale(LANG_UNKNOWN);
                }
                sLog->outCommand(GetAccountId(), "GM %s (Account: %u) receive mail item: %s (Entry: %u Count: %u) and send COD money: " UI64FMTD " to player: %s (Account: %u)",
                    GetPlayerName().c_str(), GetAccountId(), item->GetTemplate()->Name1.c_str(), item->GetEntry(), item->GetCount(), m->COD, sender_name.c_str(), sender_accId);
            }
            else if (!receive)
                sender_accId = ObjectMgr::GetPlayerAccountIdByGUID(sender_guid);

            // check player existence
            if (receive || sender_accId)
            {
                MailDraft(m->subject, "")
                    .AddMoney(m->COD)
                    .SendMailTo(trans, MailReceiver(receive, m->sender), MailSender(MAIL_NORMAL, m->receiver), MAIL_CHECK_MASK_COD_PAYMENT);
            }

            player->ModifyMoney(-int32(m->COD));
        }
        m->COD = 0;
        m->state = MAIL_STATE_CHANGED;
        player->m_mailsUpdated = true;
        player->RemoveMItem(item->GetGUID().GetCounter());

        uint32 count = item->GetCount();                      // save counts before store and possible merge with deleting
        item->SetState(ITEM_UNCHANGED);                       // need to set this state, otherwise item cannot be removed later, if neccessary
        player->MoveItemToInventory(dest, item, true);

        player->SaveInventoryAndGoldToDB(trans);
        player->_SaveMail(trans);
        CharacterDatabase.CommitTransaction(trans);

        player->SendMailResult(packet.MailID, MAIL_ITEM_TAKEN, MAIL_OK, 0, packet.AttachID, count);
    }
    else
        player->SendMailResult(packet.MailID, MAIL_ITEM_TAKEN, MAIL_ERR_EQUIP_ERROR, msg);
}

//! 6.0.3
void WorldSession::HandleMailTakeMoney(WorldPackets::Mail::MailTakeMoney& packet)
{
    if (!GetPlayer()->GetGameObjectIfCanInteractWith(packet.Mailbox, GAMEOBJECT_TYPE_MAILBOX))
        return;

    Player* player = _player;

    Mail* m = player->GetMail(packet.MailID);
    if ((!m || m->state == MAIL_STATE_DELETED || m->deliver_time > time(NULL)) ||
        (packet.Money > 0 && m->money != packet.Money))
    {
        player->SendMailResult(packet.MailID, MAIL_MONEY_TAKEN, MAIL_ERR_INTERNAL_ERROR);
        return;
    }

    if (!player->ModifyMoney(m->money, false))
    {
        player->SendMailResult(packet.MailID, MAIL_MONEY_TAKEN, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_TOO_MUCH_GOLD);
        return;
    }

    player->SendMailResult(packet.MailID, MAIL_MONEY_TAKEN, MAIL_OK);

    m->money = 0;
    m->state = MAIL_STATE_CHANGED;
    player->m_mailsUpdated = true;

    // save money and mail to prevent cheating
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    player->SaveGoldToDB(trans);
    player->_SaveMail(trans);
    CharacterDatabase.CommitTransaction(trans);
}

//called when player lists his received mails
void WorldSession::HandleGetMailList(WorldPackets::Mail::MailGetList& packet)
{
    if (!GetPlayer()->GetGameObjectIfCanInteractWith(packet.Mailbox, GAMEOBJECT_TYPE_MAILBOX))
        return;

    Player* player = _player;

    //load players mails, and mailed items
    if (!player->m_mailsLoaded)
        player->_LoadMail();

    // client can't work with packets > max int16 value
    const uint32 maxPacketSize = 32767;

    GameObject* _mailbox = NULL;
    Trinity::MailBoxMasterCheck check(player);
    Trinity::GameObjectSearcher<Trinity::MailBoxMasterCheck> searcher(player, _mailbox, check);
    player->VisitNearbyObject(5.0f, searcher);

    WorldPackets::Mail::MailListResult response;
    response.TotalNumRecords = player->GetMailSize();

    time_t cur_time = time(nullptr);

    // create bit stream
    for (PlayerMails::iterator itr = player->GetMailBegin(); itr != player->GetMailEnd(); ++itr)
    {
        // skip deleted or not delivered (deliver delay not expired) mails
        if ((*itr)->state == MAIL_STATE_DELETED || cur_time < (*itr)->deliver_time)
            continue;

        response.Mails.emplace_back(*itr, player);

        // max. 50 mails can be sent
        if (response.Mails.size() >= 50)
            break;
    }

    SendPacket(response.Write());

    // recalculate m_nextMailDelivereTime and unReadMails
    _player->UpdateNextMailTimeAndUnreads();
}

//used when player copies mail body to his inventory
void WorldSession::HandleMailCreateTextItem(WorldPacket& recvData)
{
    ObjectGuid mailbox;
    uint32 mailId;

    recvData >> mailId;
    //recvData.ReadGuidMask<4, 6, 2, 3, 7, 1, 5, 0>(mailbox);
    //recvData.ReadGuidBytes<0, 7, 1, 5, 3, 2, 4, 6>(mailbox);

    if (!GetPlayer()->GetGameObjectIfCanInteractWith(mailbox, GAMEOBJECT_TYPE_MAILBOX))
        return;

    Player* player = _player;

    Mail* m = player->GetMail(mailId);
    if (!m || (m->body.empty() && !m->mailTemplateId) || m->state == MAIL_STATE_DELETED || m->deliver_time > time(NULL))
    {
        player->SendMailResult(mailId, MAIL_MADE_PERMANENT, MAIL_ERR_INTERNAL_ERROR);
        return;
    }

    Item* bodyItem = new Item;                              // This is not bag and then can be used new Item.
    if (!bodyItem->Create(sObjectMgr->GetGenerator<HighGuid::Item>()->Generate(), MAIL_BODY_ITEM_TEMPLATE, player))
    {
        delete bodyItem;
        return;
    }

    // in mail template case we need create new item text
    if (m->mailTemplateId)
    {
        MailTemplateEntry const* mailTemplateEntry = sMailTemplateStore.LookupEntry(m->mailTemplateId);
        if (!mailTemplateEntry)
        {
            player->SendMailResult(mailId, MAIL_MADE_PERMANENT, MAIL_ERR_INTERNAL_ERROR);
            return;
        }

        bodyItem->SetText(mailTemplateEntry->content);
    }
    else
        bodyItem->SetText(m->body);

    bodyItem->SetUInt32Value(ITEM_FIELD_CREATOR, m->sender);
    bodyItem->SetFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_MAIL_TEXT_MASK);

    sLog->outInfo(LOG_FILTER_NETWORKIO, "HandleMailCreateTextItem mailid=%u", mailId);

    ItemPosCountVec dest;
    uint8 msg = _player->CanStoreItem(NULL_BAG, NULL_SLOT, dest, bodyItem, false);
    if (msg == EQUIP_ERR_OK)
    {
        m->checked = m->checked | MAIL_CHECK_MASK_COPIED;
        m->state = MAIL_STATE_CHANGED;
        player->m_mailsUpdated = true;

        player->StoreItem(dest, bodyItem, true);
        player->SendMailResult(mailId, MAIL_MADE_PERMANENT, MAIL_OK);
    }
    else
    {
        player->SendMailResult(mailId, MAIL_MADE_PERMANENT, MAIL_ERR_EQUIP_ERROR, msg);
        delete bodyItem;
    }
}

//TODO Fix me! ... this void has probably bad condition, but good data are sent
void WorldSession::HandleQueryNextMailTime(WorldPackets::Mail::MailQueryNextMailTime& /*packet*/)
{
    WorldPacket data(SMSG_QUERY_NEXT_MAIL_TIME, 8 + _player->unReadMails*24);

    if (!_player->m_mailsLoaded)
        _player->_LoadMail();

    if (_player->unReadMails > 0)
    {
        data << float(0);                                  // float
        data << uint32(0);                                 // count

        uint32 count = 0;
        time_t now = time(NULL);
        std::set<ObjectGuid::LowType> sentSenders;
        for (PlayerMails::iterator itr = _player->GetMailBegin(); itr != _player->GetMailEnd(); ++itr)
        {
            Mail* m = (*itr);
            // must be not checked yet
            if (m->checked & MAIL_CHECK_MASK_READ)
                continue;

            // and already delivered
            if (now < m->deliver_time)
                continue;

            // only send each mail sender once
            if (sentSenders.count(m->sender))
                continue;

            data << uint64(m->messageType == MAIL_NORMAL ? m->sender : 0);  // player guid
            data << uint32(m->messageType != MAIL_NORMAL ? m->sender : 0);  // non-player entries
            data << uint32(m->messageType);
            data << uint32(m->stationery);
            data << float(m->deliver_time - now);

            sentSenders.insert(m->sender);
            ++count;
            if (count == 2)                                  // do not display more than 2 mails
                break;
        }

        data.put<uint32>(4, count);
    }
    else
    {
        data << float(-DAY);
        data << uint32(0);
    }

    SendPacket(&data);
}
