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
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Guild.h"
#include "GossipDef.h"
#include "SocialMgr.h"

// Helper for getting guild object of session's player.
// If guild does not exist, sends error (if necessary).
inline Guild* _GetPlayerGuild(WorldSession* session, bool sendError = false)
{
    if (uint32 guildId = session->GetPlayer()->GetGuildId())    // If guild id = 0, player is not in guild
        if (Guild* guild = sGuildMgr->GetGuildById(guildId))   // Find guild by id
            return guild;
    if (sendError)
        Guild::SendCommandResult(session, GUILD_CREATE_S, ERR_GUILD_PLAYER_NOT_IN_GUILD);
    return NULL;
}

//! 6.0.3
void WorldSession::HandleGuildQueryOpcode(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUERY_GUILD_INFO");

    ObjectGuid guildGuid, playerGuid;
    recvPacket >> guildGuid >> playerGuid;

    // If guild doesn't exist or player is not part of the guild send error
    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid))
        //if (guild->IsMember(playerGuid))
        {
            guild->HandleQuery(this);
            return;
        }
    else
    {
        WorldPacket data(SMSG_QUERY_GUILD_INFO_RESPONSE, 8 + 1 + 1);
        data << guildGuid;
        data.WriteBit(0);   // no data
        SendPacket(&data);
    }

    Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_PLAYER_NOT_IN_GUILD);
}

//! 6.0.3
void WorldSession::HandleGuildInviteOpcode(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastGuildInviteCommand < 5)
        return;
    else
       timeLastGuildInviteCommand = now;

    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_INVITE_BY_NAME");
    std::string invitedName = recvPacket.ReadString(recvPacket.ReadBits(9));

    if (normalizePlayerName(invitedName))
        if (Guild* guild = _GetPlayerGuild(this, true))
            guild->HandleInviteMember(this, invitedName);
}

//! 6.0.3
void WorldSession::HandleGuildRemoveOpcode(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_OFFICER_REMOVE_MEMBER");

    ObjectGuid playerGuid;
    recvPacket >> playerGuid;

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleRemoveMember(this, playerGuid);
}

//! 6.0.3
void WorldSession::HandleGuildAcceptOpcode(WorldPacket& /*recvPacket*/)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received /*CMSG_ACCEPT_GUILD_INVITE*/");
    // Player cannot be in guild
    if (!GetPlayer()->GetGuildId())
        // Guild where player was invited must exist
        if (Guild* guild = sGuildMgr->GetGuildById(GetPlayer()->GetGuildIdInvited()))
            guild->HandleAcceptMember(this);
}

//! 6.0.3
void WorldSession::HandleGuildDeclineOpcode(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_DECLINE_INVITATION");

    if (Player* inviter = ObjectAccessor::FindPlayer(GetPlayer()->GetGuildInviterGuid()))
    {
        std::string name = GetPlayer()->GetName();
        WorldPacket data(SMSG_GUILD_INVITE_DECLINED, 1 + name.size() + 4);
        data.WriteBits(name.size(), 6);
        data.WriteBit(recvPacket.GetOpcode() == CMSG_GUILD_AUTO_DECLINE_INVITATION);
        data << uint32(GetVirtualRealmAddress());
        data.WriteString(name);
        inviter->SendDirectMessage(&data);
    }

    GetPlayer()->SetGuildIdInvited(UI64LIT(0));
    GetPlayer()->SetInGuild(UI64LIT(0));
}

//! 6.0.3
void WorldSession::HandleGuildRosterOpcode(WorldPacket& /*recvPacket*/)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_GET_ROSTER");

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleRoster(this);
}

//! 6.0.3
void WorldSession::HandleGuildPromoteOpcode(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_PROMOTE_MEMBER");

    ObjectGuid targetGuid;
    recvPacket >> targetGuid;

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleUpdateMemberRank(this, targetGuid, false);
}

//! 6.0.3
void WorldSession::HandleGuildDemoteOpcode(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_DEMOTE_MEMBER");

    ObjectGuid targetGuid;
    recvPacket >> targetGuid;

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleUpdateMemberRank(this, targetGuid, true);
}

//! 6.0.3
void WorldSession::HandleGuildAssignRankOpcode(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_ASSIGN_MEMBER_RANK");

    ObjectGuid targetGuid;
    uint32 rankId;
    recvPacket >> targetGuid >> rankId;

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetMemberRank(this, targetGuid, GetPlayer()->GetGUID(), rankId);
}

//! 6.0.3
void WorldSession::HandleGuildLeaveOpcode(WorldPacket& /*recvPacket*/)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_LEAVE");

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleLeaveMember(this);
}

//! 6.0.3
void WorldSession::HandleGuildDisbandOpcode(WorldPacket& /*recvPacket*/)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_DELETE");

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleDisband(this);
}

//! 6.0.3
void WorldSession::HandleGuildLeaderOpcode(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_SET_GUILD_MASTER");

    std::string name = recvPacket.ReadString(recvPacket.ReadBits(9));

    if (normalizePlayerName(name))
        if (Guild* guild = _GetPlayerGuild(this, true))
            guild->HandleSetLeader(this, name);
}

//! 6.0.3
void WorldSession::HandleGuildMOTDOpcode(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_UPDATE_MOTD_TEXT");

    std::string motd = recvPacket.ReadString(recvPacket.ReadBits(10));

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetMOTD(this, motd);
}

//! 6.0.3
void WorldSession::HandleSwapRanks(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_SHIFT_RANK");

    uint32 id;

    recvPacket >> id;
    bool up = recvPacket.ReadBit();
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSwapRanks(this, id, up);
}

//! 6.0.3
void WorldSession::HandleGuildSetNoteOpcode(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_SET_MEMBER_NOTE");

    ObjectGuid playerGuid;
    recvPacket >> playerGuid;
    uint32 noteLength = recvPacket.ReadBits(8);
    bool type = recvPacket.ReadBit();      // 0 == Officer, 1 == Public
    std::string note = recvPacket.ReadString(noteLength);

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetMemberNote(this, note, playerGuid, type);

    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: end CMSG_GUILD_SET_MEMBER_NOTE");
}

//! 6.0.3
void WorldSession::HandleGuildQueryRanksOpcode(WorldPacket& recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_GET_RANKS");

    ObjectGuid guildGuid;
    recvData >> guildGuid;

    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid))
        if (guild->IsMember(_player->GetGUID()))
            guild->HandleGuildRanks(this);
}

//! 6.0.3
void WorldSession::HandleGuildAddRankOpcode(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_ADD_RANK");

    uint32 rankId;
    uint32 len = recvPacket.ReadBits(7);
    recvPacket >> rankId;
    std::string rankName = recvPacket.ReadString(len);

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleAddNewRank(this, rankName); //, rankId);
}

//! 6.0.3
void WorldSession::HandleGuildDelRankOpcode(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_DELETE_RANK");

    uint32 rankId;
    recvPacket >> rankId;

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleRemoveRank(this, rankId);
}

//! 6.0.3
void WorldSession::HandleGuildChangeInfoTextOpcode(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_UPDATE_INFO_TEXT");

    uint32 length = recvPacket.ReadBits(11);
    std::string info = recvPacket.ReadString(length);

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetInfo(this, info);
}

//! 6.0.3 ToDo: check emblem read.
void WorldSession::HandleSaveGuildEmblemOpcode(WorldPacket& recvPacket)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SAVE_GUILD_EMBLEM");

    ObjectGuid vendorGuid;
    EmblemInfo emblemInfo;

    recvPacket >> vendorGuid;
    emblemInfo.ReadPacket(recvPacket);

    if (GetPlayer()->GetNPCIfCanInteractWith(vendorGuid, UNIT_NPC_FLAG_TABARDDESIGNER))
    {
        // Remove fake death
        if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
            GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

        if (Guild* guild = _GetPlayerGuild(this))
            guild->HandleSetEmblem(this, emblemInfo);
        else
            // "You are not part of a guild!";
            Guild::SendSaveEmblemResult(this, ERR_GUILDEMBLEM_NOGUILD);
    }
    else
    {
        // "That's not an emblem vendor!"
        Guild::SendSaveEmblemResult(this, ERR_GUILDEMBLEM_INVALIDVENDOR);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleSaveGuildEmblemOpcode - Unit (GUID: %u) not found or you can't interact with him.", vendorGuid.GetCounter());
    }
}

//! 6.0.3
void WorldSession::HandleGuildEventLogQueryOpcode(WorldPacket& /* recvPacket */)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_EVENT_LOG_QUERY)");

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendEventLog(this);
}

//! 6.0.3
void WorldSession::HandleGuildBankMoneyWithdrawn(WorldPacket & /* recvData */)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_REMAINING_WITHDRAW_MONEY_QUERY)");

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendMoneyInfo(this);
}

//! 6.0.3
void WorldSession::HandleGuildPermissions(WorldPacket& /* recvData */)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_PERMISSIONS_QUERY)");

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendPermissions(this);
}

// Called when clicking on Guild bank gameobject
//! 6.0.3
void WorldSession::HandleGuildBankerActivate(WorldPacket& recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_ACTIVATE)");

    ObjectGuid GoGuid;
    recvData >> GoGuid;
    bool fullUpdate = recvData.ReadBit();

    if (GetPlayer()->GetGameObjectIfCanInteractWith(GoGuid, GAMEOBJECT_TYPE_GUILD_BANK))
    {
        if (Guild* guild = _GetPlayerGuild(this))
            guild->SendBankList(this, 0, true, true, fullUpdate);
        else
            Guild::SendCommandResult(this, GUILD_UNK1, ERR_GUILD_PLAYER_NOT_IN_GUILD);
    }
}

// Called when opening guild bank tab only (first one)
//! 6.0.3
void WorldSession::HandleGuildBankQueryTab(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_QUERY_TAB)");

    ObjectGuid GoGuid;
    uint8 tabId;

    recvData >> GoGuid>> tabId;
    bool fullUpdate = recvData.ReadBit();  // fullUpdate

    if (GetPlayer()->GetGameObjectIfCanInteractWith(GoGuid, GAMEOBJECT_TYPE_GUILD_BANK))
        if (Guild* guild = _GetPlayerGuild(this))
            guild->SendBankList(this, tabId, true, false, fullUpdate);
}

//! 6.0.3
void WorldSession::HandleGuildBankDepositMoney(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_DEPOSIT_MONEY)");

    ObjectGuid goGuid;
    uint64 money;

    recvData >> goGuid >> money;

    if (GetPlayer()->GetGameObjectIfCanInteractWith(goGuid, GAMEOBJECT_TYPE_GUILD_BANK))
        if (money && GetPlayer()->HasEnoughMoney(money))
            if (Guild* guild = _GetPlayerGuild(this))
                guild->HandleMemberDepositMoney(this, money);
}

//! 6.0.3
void WorldSession::HandleGuildBankWithdrawMoney(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_WITHDRAW_MONEY)");

    ObjectGuid GoGuid;
    uint64 money;

    recvData >> GoGuid >> money;

    if (money)
        if (GetPlayer()->GetGameObjectIfCanInteractWith(GoGuid, GAMEOBJECT_TYPE_GUILD_BANK))
            if (Guild* guild = _GetPlayerGuild(this))
                guild->HandleMemberWithdrawMoney(this, money);
}

//! 6.0.3 TODo: check it.
void WorldSession::HandleGuildBankSwapItems(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_SWAP_ITEMS)");

    Guild* guild = _GetPlayerGuild(this);
    if (!guild)
    {
        recvData.rfinish();                   // Prevent additional spam at rejected packet
        return;
    }

    ObjectGuid GoGuid;
    bool bankToBank, autoStore;
    uint8 tabId = 0, slotId = 0, toChar, destTabId, destSlotId, playerBag = NULL_BAG, playerSlotId = NULL_SLOT;
    uint32 itemEntry = 0, destItemEntry, splitedAmount, BankItemCount;

    recvData >> GoGuid
        >> destTabId
        >> destSlotId
        >> itemEntry
        >> tabId
        >> slotId
        >> destItemEntry
        >> BankItemCount
        >> playerBag
        >> playerSlotId
        >> toChar
        >> splitedAmount;

    bankToBank = recvData.ReadBit();
    autoStore = recvData.ReadBit();

    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_SWAP_ITEMS): tabId %u slotId %u itemEntry %u destTabId %u destSlotId %u destItemEntry %u BankItemCount %u playerBag %u playerSlotId %u splitedAmount %u toChar %u bankToBank %u autoStore %u",
    //    tabId, slotId, itemEntry, destTabId, destSlotId, destItemEntry, BankItemCount, playerBag, playerSlotId, splitedAmount, toChar, bankToBank, autoStore );

    if (!GetPlayer()->GetGameObjectIfCanInteractWith(GoGuid, GAMEOBJECT_TYPE_GUILD_BANK))
        return;

    if (bankToBank)
        guild->SwapItems(GetPlayer(), tabId, slotId, destTabId, destSlotId, splitedAmount);
    else
    {
        tabId = destTabId;
        slotId = destSlotId;

        // Player <-> Bank
        // Allow to work with inventory only
        if (!Player::IsInventoryPos(playerBag, playerSlotId) && !(playerBag == NULL_BAG && playerSlotId == NULL_SLOT))
            GetPlayer()->SendEquipError(EQUIP_ERR_INTERNAL_BAG_ERROR, NULL);
        else
            guild->SwapItemsWithInventory(GetPlayer(), toChar != 0, tabId, slotId, playerBag, playerSlotId, splitedAmount);
    }
}

//! 6.0.3
void WorldSession::HandleGuildBankBuyTab(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_BUY_TAB)");

    ObjectGuid GoGuid;
    uint8 tabId;
    recvData >> GoGuid >> tabId;

    if (!GoGuid || GetPlayer()->GetGameObjectIfCanInteractWith(GoGuid, GAMEOBJECT_TYPE_GUILD_BANK))
        if (Guild* guild = _GetPlayerGuild(this))
            guild->HandleBuyBankTab(this, tabId);
}

//! 6.0.3
void WorldSession::HandleGuildBankUpdateTab(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_UPDATE_TAB)");

    ObjectGuid GoGuid;
    uint8 tabId;
    recvData >> GoGuid >> tabId;
    uint32 nameLen = recvData.ReadBits(7);
    uint32 iconLen = recvData.ReadBits(9);

    std::string name = recvData.ReadString(nameLen);
    std::string icon = recvData.ReadString(iconLen);

    if (!name.empty() && !icon.empty())
        if (GetPlayer()->GetGameObjectIfCanInteractWith(GoGuid, GAMEOBJECT_TYPE_GUILD_BANK))
            if (Guild* guild = _GetPlayerGuild(this))
                guild->HandleSetBankTabInfo(this, tabId, name, icon);
}

//! 6.0.3
void WorldSession::HandleGuildBankLogQuery(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_LOG_QUERY)");

    uint32 tabId;
    recvData >> tabId;

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendBankLog(this, tabId);
}

//! 6.0.3
void WorldSession::HandleQueryGuildBankTabText(WorldPacket &recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_BANK_TEXT_QUERY");

    uint32 tabId;
    recvData >> tabId;

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendBankTabText(this, tabId);
}

//! 6.0.3
void WorldSession::HandleSetGuildBankTabText(WorldPacket& recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_BANK_SET_TAB_TEXT");

    uint32 tabId;
    recvData >> tabId;

    std::string text = recvData.ReadString(recvData.ReadBits(14));

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SetBankTabText(tabId, text);
}

//! remove it
void WorldSession::HandleGuildQueryXPOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUERY_GUILD_XP");

    ObjectGuid guildGuid;
    //recvPacket.ReadGuidMask<7, 0, 6, 3, 2, 1, 4, 5>(guildGuid);
    //recvPacket.ReadGuidBytes<6, 5, 2, 0, 7, 1, 3, 4>(guildGuid);

    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid))
        if (guild->IsMember(_player->GetGUID()))
            guild->SendGuildXP(this);
}

//! 6.0.3
void WorldSession::HandleGuildSetRankPermissionsOpcode(WorldPacket& recvPacket)
{
    Guild* guild = _GetPlayerGuild(this, true);
    if (!guild)
    {
        recvPacket.rfinish();
        return;
    }

    uint32 oldRankId;
    uint32 rankId;
    uint32 oldRights;
    uint32 newRights;
    uint32 moneyPerDay;

    recvPacket >> rankId;
    recvPacket >> oldRankId;
    recvPacket >> newRights;
    recvPacket >> oldRights;
    recvPacket >> moneyPerDay;

    GuildBankRightsAndSlotsVec rightsAndSlots(GUILD_BANK_MAX_TABS);
    for (uint8 tabId = 0; tabId < GUILD_BANK_MAX_TABS; ++tabId)
    {
        uint32 bankRights;
        uint32 slots;

        recvPacket >> bankRights;
        recvPacket >> slots;

        rightsAndSlots[tabId] = GuildBankRightsAndSlots(tabId, uint8(bankRights), slots);
    }

    std::string rankName = recvPacket.ReadString(recvPacket.ReadBits(7));

    guild->HandleSetRankInfo(this, rankId, rankName, newRights, moneyPerDay * 10000, rightsAndSlots);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_SET_RANK_PERMISSIONS moneyPerDay %u, rankId %u, newRights %u"
    , moneyPerDay, rankId, newRights);
}

//! 6.0.3
void WorldSession::HandleGuildRequestPartyState(WorldPacket& recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_REQUEST_GUILD_PARTY_STATE");

    ObjectGuid guildGuid;
    recvData >> guildGuid;

    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid))
        guild->HandleGuildPartyRequest(this);
}

//! 6.0.3
void WorldSession::HandleAutoDeclineGuildInvites(WorldPacket& recvPacket)
{
    bool enable = recvPacket.ReadBit();

    GetPlayer()->ApplyModFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_AUTO_DECLINE_GUILD, enable);
}

//! 6.0.3 ToDo: more research
void WorldSession::HandleGuildRewardsQueryOpcode(WorldPacket& recvPacket)
{
    recvPacket.read_skip<uint32>();         // CurrentVersion

    if (Guild* guild = sGuildMgr->GetGuildById(_player->GetGuildId()))
    {
        std::vector<GuildReward> const& rewards = sGuildMgr->GetGuildRewards();

        WorldPacket data(SMSG_GUILD_REWARD_LIST, (3 + rewards.size() * (4 + 4 + 4 + 8 + 4 + 4)));
        data << uint32(time(NULL));         // counter?
        data << uint32(rewards.size());

        for (uint32 i = 0; i < rewards.size(); ++i)
        {
            data << uint32(rewards[i].Entry);
            data << uint32(0);                          //UNK V6_1_0_19678
            data << uint32(0);                          //AchievementsRequiredCount
            data << int32(rewards[i].Racemask);
            data << uint32(rewards[i].Standing);        //MinGuildLevel should be on trinity
            data << uint32(rewards[i].AchievementId);   //MinGuildRep should be on trinity
            data << uint64(rewards[i].Price);
        }

        SendPacket(&data);
    }
}

//! 6.0.3
void WorldSession::HandleGuildQueryNewsOpcode(WorldPacket& recvPacket)
{
    recvPacket.rfinish();   // guild guid

    if (Guild* guild = sGuildMgr->GetGuildById(_player->GetGuildId()))
    {
        WorldPacket data;
        guild->GetNewsLog().BuildNewsData(data);
        SendPacket(&data);
    }
}

//! 6.0.3
void WorldSession::HandleGuildNewsUpdateStickyOpcode(WorldPacket& recvPacket)
{
    uint32 newsId;
    ObjectGuid guid;

    recvPacket >> guid>> newsId;
    bool sticky = recvPacket.ReadBit();

    if (Guild* guild = sGuildMgr->GetGuildById(_player->GetGuildId()))
    {
        if (GuildNewsEntry* newsEntry = guild->GetNewsLog().GetNewsById(newsId))
        {
            if (sticky)
                newsEntry->Flags |= 1;
            else
                newsEntry->Flags &= ~1;
            WorldPacket data;
            guild->GetNewsLog().BuildNewsData(newsId, *newsEntry, data);
            SendPacket(&data);
        }
    }
}

//! 6.0.3
void WorldSession::HandleGuildQueryGuildRecipesOpcode(WorldPacket& recvPacket)
{
    ObjectGuid guildGuid;
    recvPacket >> guildGuid;

    Guild* guild = _player->GetGuild();
    if (!guild)
        return;

    Guild::KnownRecipesMap const& recipesMap = guild->GetGuildRecipes();

    //! 6.0.3
    WorldPacket* data = new WorldPacket(SMSG_GUILD_KNOWN_RECIPES, 2 + recipesMap.size() * (300 + 4));
    uint32 pos = data->wpos();
    uint32 count = 0;
    *data << uint32(count);

    for (Guild::KnownRecipesMap::const_iterator itr = recipesMap.begin(); itr != recipesMap.end(); ++itr)
    {
        if (itr->second.IsEmpty())
            continue;

        *data << uint32(itr->first);
        data->append(itr->second.recipesMask, KNOW_RECIPES_MASK_SIZE);
        ++count;
    }

    data->put<uint32>(pos, count);

    _player->ScheduleMessageSend(data, 500);
}

//! 6.0.3 ToDo: check order unk, skill, spell
void WorldSession::HandleGuildQueryGuildMembersForRecipe(WorldPacket& recvPacket)
{
    //CMSG_GUILD_QUERY_MEMBERS_FOR_RECIPE
    uint32 skillId, spellId, unk;
    ObjectGuid guildGuid;
    recvPacket >> guildGuid;
    recvPacket >> unk;
    recvPacket >> skillId;
    recvPacket >> spellId;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_QUERY_MEMBERS_FOR_RECIPE unk %u, skillId %u, spellId %u", unk, skillId, spellId);

    if (Guild* guild = _player->GetGuild())
        guild->SendGuildMembersForRecipeResponse(this, skillId, spellId);
}

//! 6.0.3
void WorldSession::HandleGuildQueryGuildMembersRecipes(WorldPacket& recvPacket)
{
    ObjectGuid playerGuid, guildGuid;
    uint32 skillId;

    recvPacket >> playerGuid >> guildGuid >> skillId;

    Guild* guild = _player->GetGuild();
    if (!guild || !guild->IsMember(playerGuid))
        return;

    guild->SendGuildMemberRecipesResponse(this, playerGuid, skillId);
}

void WorldSession::HandleGuildRequestChallengeUpdate(WorldPacket& recvPacket)
{
    if (Guild* guild = _player->GetGuild())
        guild->SendGuildChallengesInfo(this);
}
