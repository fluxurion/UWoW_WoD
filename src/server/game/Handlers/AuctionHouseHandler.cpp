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

#include "ObjectMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include "AuctionHouseMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "UpdateMask.h"
#include "Util.h"
#include "AccountMgr.h"
#include "ItemPackets.h"
#include "MailPackets.h"
#include "AuctionHousePackets.h"

//please DO NOT use iterator++, because it is slower than ++iterator!!!
//post-incrementation is always slower than pre-incrementation !

void WorldSession::HandleAuctionHelloOpcode(WorldPackets::AuctionHouse::AuctionHelloRequest& packet)
{
    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(packet.Guid, UNIT_NPC_FLAG_AUCTIONEER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleAuctionHelloOpcode - Unit (GUID: %u) not found or you can't interact with him.", uint32(packet.Guid.GetCounter()));
        return;
    }

    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendAuctionHello(packet.Guid, unit);
}

void WorldSession::SendAuctionHello(ObjectGuid guid, Creature* unit)
{
    if (GetPlayer()->getLevel() < sWorld->getIntConfig(CONFIG_AUCTION_LEVEL_REQ))
    {
        SendNotification(GetTrinityString(LANG_AUCTION_REQ), sWorld->getIntConfig(CONFIG_AUCTION_LEVEL_REQ));
        return;
    }

    AuctionHouseEntry const* ahEntry = AuctionHouseMgr::GetAuctionHouseEntry(unit->getFaction());
    if (!ahEntry)
        return;

    WorldPackets::AuctionHouse::AuctionHelloResponse auctionHelloResponse;
    auctionHelloResponse.Guid = guid;
    auctionHelloResponse.OpenForBusiness = true;                         // 3.3.3: 1 - AH enabled, 0 - AH disabled
    SendPacket(auctionHelloResponse.Write());
}

void WorldSession::SendAuctionCommandResult(AuctionEntry* auction, uint32 action, uint32 errorCode, uint32 /*bidError = 0*/)
{
    WorldPackets::AuctionHouse::AuctionCommandResult auctionCommandResult;
    auctionCommandResult.InitializeAuction(auction);
    auctionCommandResult.Command = action;
    auctionCommandResult.ErrorCode = errorCode;
    SendPacket(auctionCommandResult.Write());
}

void WorldSession::SendAuctionOutBidNotification(AuctionEntry const* auction, Item const* item)
{
    WorldPackets::AuctionHouse::AuctionOutBidNotification packet;
    packet.BidAmount = auction->bid;
    packet.MinIncrement = auction->GetAuctionOutBid();
    packet.Info.Initialize(auction, item);
    SendPacket(packet.Write());
}

void WorldSession::SendAuctionClosedNotification(AuctionEntry const* auction, float mailDelay, bool sold, Item const* item)
{
    WorldPackets::AuctionHouse::AuctionClosedNotification packet;
    packet.Info.Initialize(auction, item);
    packet.ProceedsMailDelay = mailDelay;
    packet.Sold = sold;
    SendPacket(packet.Write());
}

void WorldSession::SendAuctionWonNotification(AuctionEntry const* auction, Item const* item)
{
    WorldPackets::AuctionHouse::AuctionWonNotification packet;
    packet.Info.Initialize(auction, item);
    SendPacket(packet.Write());
}

void WorldSession::SendAuctionOwnerBidNotification(AuctionEntry const* auction, Item const* item)
{
    WorldPackets::AuctionHouse::AuctionOwnerBidNotification packet;
    packet.Info.Initialize(auction, item);
    packet.Bidder = ObjectGuid::Create<HighGuid::Player>(auction->bidder);
    packet.MinIncrement = auction->GetAuctionOutBid();
    SendPacket(packet.Write());
}

void WorldSession::HandleAuctionSellItem(WorldPackets::AuctionHouse::AuctionSellItem& packet)
{
    if (packet.Items.size() > MAX_AUCTION_ITEMS)
    {
        SendAuctionCommandResult(NULL, AUCTION_SELL_ITEM, ERR_AUCTION_DATABASE_ERROR);
        return;
    }

    for (auto const& item : packet.Items)
        if (!item.Guid || !item.UseCount || item.UseCount > 1000)
            return;

    if (!packet.MinBid || !packet.RunTime)
        return;

    Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(packet.Auctioneer, UNIT_NPC_FLAG_AUCTIONEER);
    if (!creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleAuctionSellItem - Unit (GUID: %u) not found or you can't interact with him.", packet.Auctioneer.GetCounter());
        return;
    }

    AuctionHouseEntry const* auctionHouseEntry = AuctionHouseMgr::GetAuctionHouseEntry(creature->getFaction());
    if (!auctionHouseEntry)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleAuctionSellItem - Unit (GUID: %u) has wrong faction.", packet.Auctioneer.GetCounter());
        return;
    }

    packet.RunTime *= MINUTE;

    switch (packet.RunTime)
    {
        case 1 * MIN_AUCTION_TIME:
        case 2 * MIN_AUCTION_TIME:
        case 4 * MIN_AUCTION_TIME:
            break;
        default:
            return;
    }

    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    uint32 finalCount = 0;

    for (auto const& packetItem : packet.Items)
    {
        Item* item = _player->GetItemByGuid(packetItem.Guid);

        if (!item)
        {
            SendAuctionCommandResult(NULL, AUCTION_SELL_ITEM, ERR_AUCTION_ITEM_NOT_FOUND);
            return;
        }

        if (sAuctionMgr->GetAItem(item->GetGUID().GetCounter()) || !item->CanBeTraded() || item->IsNotEmptyBag() ||
            item->GetTemplate()->Flags & ITEM_PROTO_FLAG_CONJURED || item->GetUInt32Value(ITEM_FIELD_EXPIRATION) ||
            item->GetCount() < packetItem.UseCount || _player->GetItemCount(item->GetEntry(), false) < packetItem.UseCount)
        {
            SendAuctionCommandResult(NULL, AUCTION_SELL_ITEM, ERR_AUCTION_DATABASE_ERROR);
            return;
        }

        if (packet.Items.size() != 0 && (packetItem.Guid.GetEntry() != item->GetEntry() || packetItem.Guid.GetCounter() == item->GetGUID().GetCounter()))
        {
            sWorld->BanAccount(BAN_CHARACTER, _player->GetName(), "45d", "Dupe Auction mop", "System");
            return;
        }

        finalCount += packetItem.UseCount;
    }

    if (!finalCount)
    {
        SendAuctionCommandResult(NULL, AUCTION_SELL_ITEM, ERR_AUCTION_DATABASE_ERROR);
        return;
    }

    for (uint32 i = 0; i < packet.Items.size() - 1; ++i)
    {
        for (uint32 j = i + 1; j < packet.Items.size(); ++j)
        {
            if (packet.Items[i].Guid == packet.Items[j].Guid)
            {
                SendAuctionCommandResult(NULL, AUCTION_SELL_ITEM, ERR_AUCTION_DATABASE_ERROR);
                return;
            }
        }
    }

    for (auto const& packetItem : packet.Items)
    {
        Item* item = _player->GetItemByGuid(packetItem.Guid);

        if (item->GetMaxStackCount() < finalCount)
        {
            SendAuctionCommandResult(NULL, AUCTION_SELL_ITEM, ERR_AUCTION_DATABASE_ERROR);
            return;
        }
    }

    for (auto const& packetItem : packet.Items)
    {
        Item* item = _player->GetItemByGuid(packetItem.Guid);

        uint32 auctionTime = uint32(packet.RunTime * sWorld->getRate(RATE_AUCTION_TIME));
        AuctionHouseObject* auctionHouse = sAuctionMgr->GetAuctionsMap(creature->getFaction());

        uint32 deposit = sAuctionMgr->GetAuctionDeposit(auctionHouseEntry, packet.RunTime, item, finalCount);
        if (!_player->HasEnoughMoney((uint64)deposit))
        {
            SendAuctionCommandResult(NULL, AUCTION_SELL_ITEM, ERR_AUCTION_NOT_ENOUGHT_MONEY);
            return;
        }

        _player->ModifyMoney(-int32(deposit));

        AuctionEntry* AH = new AuctionEntry;
        AH->Id = sObjectMgr->GenerateAuctionID();

        if (sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_AUCTION))
            AH->auctioneer = 174444;
        else
            AH->auctioneer = packet.Auctioneer.GetCounter();

        ASSERT(sObjectMgr->GetCreatureData(AH->auctioneer)); // Tentative de vendre un item a un pnj qui n'existe pas, mieux vaut crash ici sinon l'item en question risque de disparaitre tout simplement

        // Required stack size of auction matches to current item stack size, just move item to auctionhouse
        if (packet.Items.size() == 1 && item->GetCount() == packet.Items[0].UseCount)
        {
            if (GetSecurity() > SEC_PLAYER && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
            {
                sLog->outCommand(GetAccountId(), "GM %s (Account: %u) create auction: %s (Entry: %u Count: %u)",
                    GetPlayerName().c_str(), GetAccountId(), item->GetTemplate()->Name1.c_str(), item->GetEntry(), item->GetCount());
            }

            AH->itemGUIDLow = item->GetGUID().GetCounter();
            AH->itemEntry = item->GetEntry();
            AH->itemCount = item->GetCount();
            AH->owner = _player->GetGUID().GetCounter();
            AH->startbid = packet.MinBid;
            AH->bidder = 0;
            AH->bid = 0;
            AH->buyout = packet.BuyoutPrice;
            AH->expire_time = time(NULL) + auctionTime;
            AH->deposit = deposit;
            AH->auctionHouseEntry = auctionHouseEntry;

            sLog->outInfo(LOG_FILTER_NETWORKIO, "CMSG_AUCTION_SELL_ITEM: Player %s (guid %d) is selling item %s entry %u (guid %d) to auctioneer %u with count %u with initial bid %u with buyout %u and with time %u (in sec) in auctionhouse %u",
                _player->GetName(), _player->GetGUID().GetCounter(), item->GetTemplate()->Name1.c_str(), item->GetEntry(), item->GetGUID().GetCounter(), AH->auctioneer, item->GetCount(), packet.MinBid, packet.BuyoutPrice, auctionTime, AH->GetHouseId());

            sAuctionMgr->AddAItem(item);
            auctionHouse->AddAuction(AH);

            _player->MoveItemFromInventory(item, true);

            SQLTransaction trans = CharacterDatabase.BeginTransaction();
            item->DeleteFromInventoryDB(trans);
            item->SaveToDB(trans);
            AH->SaveToDB(trans);
            _player->SaveInventoryAndGoldToDB(trans);
            CharacterDatabase.CommitTransaction(trans);

            SendAuctionCommandResult(AH, AUCTION_SELL_ITEM, ERR_AUCTION_OK);

            GetPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION, 1);
            return;
        }
        else // Required stack size of auction does not match to current item stack size, clone item and set correct stack size
        {
            Item* newItem = item->CloneItem(finalCount, _player);
            if (!newItem)
            {
                sLog->outError(LOG_FILTER_NETWORKIO, "CMSG_AUCTION_SELL_ITEM: Could not create clone of item %u", item->GetEntry());
                SendAuctionCommandResult(NULL, AUCTION_SELL_ITEM, ERR_AUCTION_DATABASE_ERROR);
                return;
            }
            if (newItem->GetEntry() == 38186)
                sLog->outDebug(LOG_FILTER_EFIR, "HandleAuctionSellItem - CloneItem of item %u; finalCount = %u playerGUID %u, itemGUID %u", newItem->GetEntry(), finalCount, _player->GetGUID(), newItem->GetGUID());

            if (GetSecurity() > SEC_PLAYER && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
            {
                sLog->outCommand(GetAccountId(), "GM %s (Account: %u) create auction: %s (Entry: %u Count: %u)",
                    GetPlayerName().c_str(), GetAccountId(), newItem->GetTemplate()->Name1.c_str(), newItem->GetEntry(), newItem->GetCount());
            }

            AH->itemGUIDLow = newItem->GetGUID().GetCounter();
            AH->itemEntry = newItem->GetEntry();
            AH->itemCount = newItem->GetCount();
            AH->owner = _player->GetGUID().GetCounter();
            AH->startbid = packet.MinBid;
            AH->bidder = 0;
            AH->bid = 0;
            AH->buyout = packet.BuyoutPrice;
            AH->expire_time = time(NULL) + auctionTime;
            AH->deposit = deposit;
            AH->auctionHouseEntry = auctionHouseEntry;

            sLog->outInfo(LOG_FILTER_NETWORKIO, "CMSG_AUCTION_SELL_ITEM: Player %s (guid %d) is selling item %s entry %u (guid %d) to auctioneer %u with count %u with initial bid %u with buyout %u and with time %u (in sec) in auctionhouse %u",
                _player->GetName(), _player->GetGUID().GetCounter(), newItem->GetTemplate()->Name1.c_str(), newItem->GetEntry(), newItem->GetGUID().GetCounter(), AH->auctioneer, newItem->GetCount(), packet.MinBid, packet.BuyoutPrice, auctionTime, AH->GetHouseId());

            sAuctionMgr->AddAItem(newItem);
            auctionHouse->AddAuction(AH);

            SQLTransaction trans = CharacterDatabase.BeginTransaction();

            for (auto const& packetItem : packet.Items)
            {
                Item* item2 = _player->GetItemByGuid(packetItem.Guid);

                // Item stack count equals required count, ready to delete item - cloned item will be used for auction
                if (item2->GetCount() == packetItem.UseCount)
                {
                    _player->MoveItemFromInventory(item2, true);
                    item2->DeleteFromInventoryDB(trans);
                    item2->DeleteFromDB(trans);
                    delete item2;
                }
                else // Item stack count is bigger than required count, update item stack count and save to database - cloned item will be used for auction
                {
                    item2->SetCount(item2->GetCount() - packetItem.UseCount);
                    item2->SetState(ITEM_CHANGED, _player);
                    _player->ItemRemovedQuestCheck(item2->GetEntry(), packetItem.UseCount);
                    item2->SendUpdateToPlayer(_player);
                    item2->SaveToDB(trans);
                }
            }

            newItem->SaveToDB(trans);
            AH->SaveToDB(trans);
            _player->SaveInventoryAndGoldToDB(trans);
            CharacterDatabase.CommitTransaction(trans);

            SendAuctionCommandResult(AH, AUCTION_SELL_ITEM, ERR_AUCTION_OK);

            GetPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION, 1);
            return;
        }
    }
}

void WorldSession::HandleAuctionPlaceBid(WorldPackets::AuctionHouse::AuctionPlaceBid& packet)
{
    if (!packet.AuctionItemID || !packet.BidAmount)
        return;    
    // check for cheaters
    Player* player = GetPlayer();

    Creature* creature = player->GetNPCIfCanInteractWith(packet.Auctioneer, UNIT_NPC_FLAG_AUCTIONEER);
    if (!creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleAuctionPlaceBid - Unit (GUID: %u) not found or you can't interact with him.", uint32(packet.Auctioneer.GetCounter()));
        return;
    }

    if (player->HasUnitState(UNIT_STATE_DIED))
        player->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    AuctionHouseObject* auctionHouse = sAuctionMgr->GetAuctionsMap(creature->getFaction());
    AuctionEntry* auction = auctionHouse->GetAuction(packet.AuctionItemID);

    if (!auction || auction->owner == player->GetGUID().GetCounter())
    {
        //you cannot bid your own auction:
        SendAuctionCommandResult(NULL, AUCTION_PLACE_BID, ERR_AUCTION_BID_OWN);
        return;
    }

    // impossible have online own another character (use this for speedup check in case online owner)
    /*Player* auction_owner = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(auction->owner, 0, HighGuid::Player));
    if (!auction_owner && ObjectMgr::GetPlayerAccountIdByGUID(MAKE_NEW_GUID(auction->owner, 0, HighGuid::Player)) == player->GetSession()->GetAccountId())
    {
        //you cannot bid your another character auction:
        SendAuctionCommandResult(NULL, AUCTION_PLACE_BID, ERR_AUCTION_BID_OWN);
        return;
    }*/

    // cheating
    if (packet.BidAmount <= auction->bid || packet.BidAmount < auction->startbid)
        return;

    // BidAmount too low for next bid if not buyout
    if ((packet.BidAmount < auction->buyout || auction->buyout == 0) &&
        packet.BidAmount < auction->bid + auction->GetAuctionOutBid())
    {
        // client already test it but just in case ...
        SendAuctionCommandResult(auction, AUCTION_PLACE_BID, ERR_AUCTION_HIGHER_BID);
        return;
    }

    if (!player->HasEnoughMoney(packet.BidAmount))
    {
        // client already test it but just in case ...
        SendAuctionCommandResult(auction, AUCTION_PLACE_BID, ERR_AUCTION_NOT_ENOUGHT_MONEY);
        return;
    }

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    if (packet.BidAmount < auction->buyout || auction->buyout == 0)
    {
        if (auction->bidder > 0)
        {
            if (auction->bidder == player->GetGUID().GetCounter())
                player->ModifyMoney(-int64(packet.BidAmount - auction->bid));
            else
            {
                // mail to last bidder and return money
                sAuctionMgr->SendAuctionOutbiddedMail(auction, packet.BidAmount, player, trans);
                player->ModifyMoney(-int64(packet.BidAmount));
            }
        }
        else
            player->ModifyMoney(-int64(packet.BidAmount));

        auction->bidder = player->GetGUID().GetCounter();
        auction->bid = packet.BidAmount;
        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID, packet.BidAmount);

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_AUCTION_BID);
        stmt->setUInt32(0, auction->bidder);
        stmt->setUInt32(1, auction->bid);
        stmt->setUInt32(2, auction->Id);
        trans->Append(stmt);

        SendAuctionCommandResult(auction, AUCTION_PLACE_BID, ERR_AUCTION_OK);
    }
    else
    {
        //buyout:
        if (player->GetGUID().GetCounter() == auction->bidder)
            player->ModifyMoney(-int64(auction->buyout - auction->bid));
        else
        {
            player->ModifyMoney(-int64(auction->buyout));
            if (auction->bidder)                          //buyout for bidded auction ..
                sAuctionMgr->SendAuctionOutbiddedMail(auction, auction->buyout, player, trans);
        }
        auction->bidder = player->GetGUID().GetCounter();
        auction->bid = auction->buyout;
        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID, auction->buyout);

        SendAuctionCommandResult(auction, AUCTION_PLACE_BID, ERR_AUCTION_OK);

        //- Mails must be under transaction control too to prevent data loss
        sAuctionMgr->SendAuctionSalePendingMail(auction, trans);
        sAuctionMgr->SendAuctionSuccessfulMail(auction, trans);
        sAuctionMgr->SendAuctionWonMail(auction, trans);

        auction->DeleteFromDB(trans);

        uint32 itemEntry = auction->itemEntry;
        sAuctionMgr->RemoveAItem(auction->itemGUIDLow);
        auctionHouse->RemoveAuction(auction, itemEntry);
    }
    player->SaveInventoryAndGoldToDB(trans);
    CharacterDatabase.CommitTransaction(trans);
}

void WorldSession::HandleAuctionRemoveItem(WorldPackets::AuctionHouse::AuctionRemoveItem& packet)
{
    Player* player = GetPlayer();

    Creature* creature = player->GetNPCIfCanInteractWith(packet.Auctioneer, UNIT_NPC_FLAG_AUCTIONEER);
    if (!creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleAuctionRemoveItem - Unit (GUID: %u) not found or you can't interact with him.", uint32(packet.Auctioneer.GetCounter()));
        return;
    }

    if (player->HasUnitState(UNIT_STATE_DIED))
        player->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    AuctionHouseObject* auctionHouse = sAuctionMgr->GetAuctionsMap(creature->getFaction());
    AuctionEntry* auction = auctionHouse->GetAuction(packet.AuctionItemID);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    if (auction && auction->owner == player->GetGUID().GetCounter())
    {
        Item* pItem = sAuctionMgr->GetAItem(auction->itemGUIDLow);
        if (pItem)
        {
            if (auction->bidder > 0)                        // If we have a bidder, we have to send him the money he paid
            {
                uint32 auctionCut = auction->GetAuctionCut();
                if (!player->HasEnoughMoney((uint64)auctionCut))          //player doesn't have enough money, maybe message needed
                    return;
                sAuctionMgr->SendAuctionCancelledToBidderMail(auction, trans, pItem);
                player->ModifyMoney(-int64(auctionCut));
            }

            // item will deleted or added to received mail list
            MailDraft(auction->BuildAuctionMailSubject(AUCTION_CANCELED), AuctionEntry::BuildAuctionMailBody(0, 0, auction->buyout, auction->deposit, 0))
                .AddItem(pItem)
                .SendMailTo(trans, player, auction, MAIL_CHECK_MASK_COPIED);
        }
        else
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Auction id: %u got non existing item (item guid : %u)!", auction->Id, auction->itemGUIDLow);
            SendAuctionCommandResult(NULL, AUCTION_CANCEL, ERR_AUCTION_DATABASE_ERROR);
            return;
        }
    }
    else
    {
        SendAuctionCommandResult(NULL, AUCTION_CANCEL, ERR_AUCTION_DATABASE_ERROR);
        //this code isn't possible ... maybe there should be assert
        sLog->outError(LOG_FILTER_NETWORKIO, "CHEATER: %u tried to cancel auction (id: %u) of another player or auction is NULL", player->GetGUID().GetCounter(), packet.AuctionItemID);
        return;
    }

    //inform player, that auction is removed
    SendAuctionCommandResult(auction, AUCTION_CANCEL, ERR_AUCTION_OK);

    // Now remove the auction

    player->SaveInventoryAndGoldToDB(trans);
    auction->DeleteFromDB(trans);
    CharacterDatabase.CommitTransaction(trans);

    uint32 itemEntry = auction->itemEntry;
    sAuctionMgr->RemoveAItem(auction->itemGUIDLow);
    auctionHouse->RemoveAuction(auction, itemEntry);
}

void WorldSession::HandleAuctionListBidderItems(WorldPackets::AuctionHouse::AuctionListBidderItems& packet)
{
    Player* player = GetPlayer();

    Creature* creature = player->GetNPCIfCanInteractWith(packet.Auctioneer, UNIT_NPC_FLAG_AUCTIONEER);
    if (!creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleAuctionListBidderItems - Unit (GUID: %u) not found or you can't interact with him.", uint32(packet.Auctioneer.GetCounter()));
        return;
    }

    if (player->HasUnitState(UNIT_STATE_DIED))
        player->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    AuctionHouseObject* auctionHouse = sAuctionMgr->GetAuctionsMap(creature->getFaction());

    WorldPackets::AuctionHouse::AuctionListBidderItemsResult result;
    auctionHouse->BuildListBidderItems(result, player, result.TotalCount);
    result.DesiredDelay = 300;
    SendPacket(result.Write());
}

void WorldSession::HandleAuctionListOwnerItems(WorldPackets::AuctionHouse::AuctionListOwnerItems& packet)
{
    Player* player = GetPlayer();

    Creature* creature = player->GetNPCIfCanInteractWith(packet.Auctioneer, UNIT_NPC_FLAG_AUCTIONEER);
    if (!creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleAuctionListOwnerItems - Unit (GUID: %u) not found or you can't interact with him.", uint32(packet.Auctioneer.GetCounter()));
        return;
    }

    if (player->HasUnitState(UNIT_STATE_DIED))
        player->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    AuctionHouseObject* auctionHouse = sAuctionMgr->GetAuctionsMap(creature->getFaction());

    WorldPackets::AuctionHouse::AuctionListOwnerItemsResult result;
    auctionHouse->BuildListOwnerItems(result, player, result.TotalCount);
    result.DesiredDelay = 300;
    SendPacket(result.Write());
}

void WorldSession::HandleAuctionListItems(WorldPackets::AuctionHouse::AuctionListItems& packet)
{
    Player* player = GetPlayer();

    Creature* creature = player->GetNPCIfCanInteractWith(packet.Auctioneer, UNIT_NPC_FLAG_AUCTIONEER);
    if (!creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleAuctionListItems - Unit (GUID: %u) not found or you can't interact with him.", uint32(packet.Auctioneer.GetCounter()));
        return;
    }

    if (player->HasUnitState(UNIT_STATE_DIED))
        player->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    AuctionHouseObject* auctionHouse = sAuctionMgr->GetAuctionsMap(creature->getFaction());

    WorldPackets::AuctionHouse::AuctionListItemsResult result;

    std::wstring wsearchedname;
    if (!Utf8toWStr(packet.Name, wsearchedname))
        return;

    wstrToLower(wsearchedname);

    auctionHouse->BuildListAuctionItems(result, _player,
        wsearchedname, packet.Offset, packet.MinLevel, packet.MaxLevel, packet.OnlyUsable,
        packet.InvType, packet.ItemClass, packet.ItemSubclass, packet.Quality, result.TotalCount);

    result.DesiredDelay = 300;
    result.OnlyUsable = packet.OnlyUsable;
    SendPacket(result.Write());
}

void WorldSession::HandleAuctionListPendingSales(WorldPackets::AuctionHouse::AuctionListPendingSales& /*packet*/)
{
    uint32 count = 0;
    time_t cur_time = time(nullptr);

    WorldPackets::AuctionHouse::AuctionListPendingSalesResult result;
    for (PlayerMails::iterator itr = _player->GetMailBegin(); itr != _player->GetMailEnd(); ++itr)
    {
        if ((*itr)->state == MAIL_STATE_DELETED || (*itr)->messageType == MAIL_AUCTION)
            continue;

        ++count;
        if (count < 50)
            result.Mails.push_back(WorldPackets::Mail::MailListEntry(*itr, _player));

        break;
    }

    result.TotalNumRecords = count;

    SendPacket(result.Write());
}

void WorldSession::HandleReplicateItems(WorldPackets::AuctionHouse::AuctionReplicateItems& packet)
{
    WorldPackets::AuctionHouse::AuctionReplicateResponse response;
    response.ChangeNumberCursor = packet.ChangeNumberCursor;
    response.ChangeNumberGlobal = packet.ChangeNumberGlobal;
    response.ChangeNumberTombstone = packet.ChangeNumberTombstone;
    response.DesiredDelay = 300;
    response.Result = 0;
    SendPacket(response.Write());
}
