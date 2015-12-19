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
#include "AchievementPackets.h"
#include "GuildPackets.h"

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

void WorldSession::HandleGuildQueryOpcode(WorldPackets::Guild::QueryGuildInfo& packet)
{
    // If guild doesn't exist or player is not part of the guild send error
    if (Guild* guild = sGuildMgr->GetGuildByGuid(packet.GuildGuid))
    {
        guild->SendQueryResponse(this);
        return;
    }
    else
    {
        WorldPackets::Guild::QueryGuildInfoResponse response;
        response.GuildGuid = packet.GuildGuid;
        SendPacket(response.Write());
    }

    Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_PLAYER_NOT_IN_GUILD);
}

void WorldSession::HandleGuildInviteByName(WorldPackets::Guild::GuildInviteByName& packet)
{
    if (normalizePlayerName(packet.Name))
        if (Guild* guild = _GetPlayerGuild(this, true))
            guild->HandleInviteMember(this, packet.Name);
}

void WorldSession::HandleGuildOfficerRemoveMember(WorldPackets::Guild::GuildOfficerRemoveMember& packet)
{
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleRemoveMember(this, packet.Removee);
}

void WorldSession::HandleGuildAcceptInvite(WorldPackets::Guild::AcceptGuildInvite& /*packet*/)
{
    if (_GetPlayerGuild(this, true))
        return;

    if (Guild* guild = sGuildMgr->GetGuildById(GetPlayer()->GetGuildIdInvited()))
        guild->HandleAcceptMember(this);
}

void WorldSession::HandleGuildDeclineInvitation(WorldPackets::Guild::GuildDeclineInvitation& /*decline*/)
{
    /*if (Player* inviter = ObjectAccessor::FindPlayer(GetPlayer()->GetGuildInviterGuid()))
    {
        std::string name = GetPlayer()->GetName();
        WorldPacket data(SMSG_GUILD_INVITE_DECLINED, 1 + name.size() + 4);
        data.WriteBits(name.size(), 6);
        data.WriteBit(recvPacket.GetOpcode() == CMSG_GUILD_AUTO_DECLINE_INVITATION);
        data << uint32(GetVirtualRealmAddress());
        data.WriteString(name);
        inviter->SendDirectMessage(&data);
    }*/

    GetPlayer()->SetGuildIdInvited(UI64LIT(0));
    GetPlayer()->SetInGuild(UI64LIT(0));
}

void WorldSession::HandleGuildRosterOpcode(WorldPackets::Guild::GuildGetRoster& /*packet*/)
{
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->SendRoster(this);
}

void WorldSession::HandleGuildPromoteMember(WorldPackets::Guild::GuildPromoteMember& packet)
{
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleUpdateMemberRank(this, packet.Promotee, false);
}

void WorldSession::HandleGuildDemoteMember(WorldPackets::Guild::GuildDemoteMember& packet)
{
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleUpdateMemberRank(this, packet.Demotee, true);
}

void WorldSession::HandleGuildAssignRank(WorldPackets::Guild::GuildAssignMemberRank& packet)
{
    if (packet.RankOrder < 0 || packet.RankOrder > GUILD_RANKS_MAX_COUNT)
        return;

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetMemberRank(this, packet.Member, GetPlayer()->GetGUID(), packet.RankOrder);
}

void WorldSession::HandleGuildLeave(WorldPackets::Guild::GuildLeave& /*packet*/)
{
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleLeaveMember(this);
}

void WorldSession::HandleGuildDisbandOpcode(WorldPackets::Guild::GuildDelete& packet)
{
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleDisband(this);
}

void WorldSession::HandleGuildSetGuildMaster(WorldPackets::Guild::GuildSetGuildMaster& packet)
{
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetNewGuildMaster(this, packet.NewMasterName);
}

void WorldSession::HandleGuildUpdateMotdText(WorldPackets::Guild::GuildUpdateMotdText& packet)
{
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetMOTD(this, packet.MotdText);
}

void WorldSession::HandleShiftRank(WorldPackets::Guild::GuildShiftRank& packet)
{
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleShiftRank(this, packet.RankOrder, packet.ShiftUp);
}

void WorldSession::HandleGuildSetMemberNote(WorldPackets::Guild::GuildSetMemberNote& packet)
{
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetMemberNote(this, packet.Note, packet.NoteeGUID, packet.IsPublic);
}

void WorldSession::HandleGuildGetRanks(WorldPackets::Guild::GuildGetRanks& packet)
{
    if (Guild* guild = sGuildMgr->GetGuildByGuid(packet.GuildGUID))
        if (guild->IsMember(_player->GetGUID()))
            guild->SendGuildRankInfo(this);
}

void WorldSession::HandleGuildAddRank(WorldPackets::Guild::GuildAddRank& packet)
{
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleAddNewRank(this, packet.Name); //, rankId);
}

void WorldSession::HandleGuildDeleteRank(WorldPackets::Guild::GuildDeleteRank& packet)
{
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleRemoveRank(this, packet.RankOrder);
}

void WorldSession::HandleGuildUpdateInfoText(WorldPackets::Guild::GuildUpdateInfoText& packet)
{
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetInfo(this, packet.InfoText);
}

// 6.1.2 TODO: rewrite
void WorldSession::HandleSaveGuildEmblemOpcode(WorldPacket& recvPacket)
{
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
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleSaveGuildEmblemOpcode - Unit (GUID: %u) not found or you can't interact with him.", vendorGuid.GetGUIDLow());
    }
}

void WorldSession::HandleGuildEventLogQuery(WorldPackets::Guild::GuildEventLogQuery& /*packet*/)
{
    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendEventLog(this);
}

void WorldSession::HandleGuildBankMoneyWithdrawn(WorldPackets::Guild::GuildBankRemainingWithdrawMoneyQuery& /* packet */)
{
    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendMoneyInfo(this);
}

void WorldSession::HandleGuildPermissions(WorldPackets::Guild::GuildPermissionsQuery& /* packet */)
{
    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendPermissions(this);
}

// Called when clicking on Guild bank gameobject
void WorldSession::HandleGuildBankActivate(WorldPackets::Guild::GuildBankActivate& packet)
{
    if (GetPlayer()->GetGameObjectIfCanInteractWith(packet.Banker, GAMEOBJECT_TYPE_GUILD_BANK))
    {
        if (Guild* guild = _GetPlayerGuild(this))
            guild->SendBankList(this, 0, packet.FullUpdate);
        else
            Guild::SendCommandResult(this, GUILD_UNK1, ERR_GUILD_PLAYER_NOT_IN_GUILD);
    }
}

// Called when opening guild bank tab only (first one)
void WorldSession::HandleGuildBankQueryTab(WorldPackets::Guild::GuildBankQueryTab& packet)
{
    if (GetPlayer()->GetGameObjectIfCanInteractWith(packet.Banker, GAMEOBJECT_TYPE_GUILD_BANK))
        if (Guild* guild = _GetPlayerGuild(this))
            guild->SendBankList(this, packet.TabId, packet.FullUpdate);
}

void WorldSession::HandleGuildBankDepositMoney(WorldPackets::Guild::GuildBankDepositMoney& packet)
{
    if (Player* player = GetPlayer())
    {
        if (!player->GetGameObjectIfCanInteractWith(packet.Banker, GAMEOBJECT_TYPE_GUILD_BANK))
            return;

        if (player->HasEnoughMoney(packet.Money))
            if (Guild* guild = _GetPlayerGuild(this))
                guild->HandleMemberDepositMoney(this, packet.Money);
    }
}

void WorldSession::HandleGuildBankWithdrawMoney(WorldPackets::Guild::GuildBankWithdrawMoney& packet)
{
    if (Player* player = GetPlayer())
    {
        if (!player->GetGameObjectIfCanInteractWith(packet.Banker, GAMEOBJECT_TYPE_GUILD_BANK))
            return;

        if (Guild* guild = _GetPlayerGuild(this))
            guild->HandleMemberWithdrawMoney(this, packet.Money);
    }
}

void WorldSession::HandleGuildBankSwapItems(WorldPackets::Guild::GuildBankSwapItems& packet)
{
    if (!GetPlayer()->GetGameObjectIfCanInteractWith(packet.Banker, GAMEOBJECT_TYPE_GUILD_BANK))
        return;

    Guild* guild = _GetPlayerGuild(this);

    if (!guild)
        return;

    if (packet.BankOnly)
        guild->SwapItems(GetPlayer(), packet.BankTab1, packet.BankSlot1, packet.BankTab, packet.BankSlot, packet.StackCount);
    else
    {
        // Player <-> Bank
        // Allow to work with inventory only
        if (!Player::IsInventoryPos(packet.ContainerSlot, packet.ContainerItemSlot) && !(packet.ContainerSlot == NULL_BAG && packet.ContainerItemSlot == NULL_SLOT) && !packet.AutoStore)
            GetPlayer()->SendEquipError(EQUIP_ERR_INTERNAL_BAG_ERROR, NULL);
        else
            guild->SwapItemsWithInventory(GetPlayer(), packet.ToSlot != 0, packet.BankTab, packet.BankSlot, packet.ContainerSlot, packet.ContainerItemSlot, packet.StackCount);
    }
}

void WorldSession::HandleGuildBankBuyTab(WorldPackets::Guild::GuildBankBuyTab& packet)
{
    if (!packet.Banker || GetPlayer()->GetGameObjectIfCanInteractWith(packet.Banker, GAMEOBJECT_TYPE_GUILD_BANK))
        if (Guild* guild = _GetPlayerGuild(this))
            guild->HandleBuyBankTab(this, packet.BankTab);
}

void WorldSession::HandleGuildBankUpdateTab(WorldPackets::Guild::GuildBankUpdateTab& packet)
{
    if (!packet.Name.empty() && !packet.Icon.empty())
        if (GetPlayer()->GetGameObjectIfCanInteractWith(packet.Banker, GAMEOBJECT_TYPE_GUILD_BANK))
            if (Guild* guild = _GetPlayerGuild(this))
                guild->HandleSetBankTabInfo(this, packet.BankTab, packet.Name, packet.Icon);
}

void WorldSession::HandleGuildBankLogQuery(WorldPackets::Guild::GuildBankLogQuery& packet)
{
    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendBankLog(this, packet.TabId);
}

void WorldSession::HandleGuildBankTextQuery(WorldPackets::Guild::GuildBankTextQuery& packet)
{
    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendBankTabText(this, packet.TabId);
}

void WorldSession::HandleGuildBankSetTabText(WorldPackets::Guild::GuildBankSetTabText& packet)
{
    if (Guild* guild = _GetPlayerGuild(this))
        guild->SetBankTabText(packet.TabId, packet.TabText);
}

void WorldSession::HandleGuildSetRankPermissions(WorldPackets::Guild::GuildSetRankPermissions& packet)
{
    Guild* guild = _GetPlayerGuild(this, true);

    if (!guild)
        return;

    GuildBankRightsAndSlotsVec rightsAndSlots(GUILD_BANK_MAX_TABS);
    for (uint8 tabId = 0; tabId < GUILD_BANK_MAX_TABS; ++tabId)
        rightsAndSlots[tabId] = GuildBankRightsAndSlots(tabId, uint8(packet.TabFlags[tabId]), uint32(packet.TabWithdrawItemLimit[tabId]));

    guild->HandleSetRankInfo(this, packet.RankOrder, packet.RankName, packet.Flags, packet.WithdrawGoldLimit * GOLD, rightsAndSlots);
}

void WorldSession::HandleGuildRequestPartyState(WorldPackets::Guild::RequestGuildPartyState& packet)
{
    if (Guild* guild = _GetPlayerGuild(this))
        if (guild->GetGUID() == packet.GuildGUID)
            guild->HandleGuildPartyRequest(this);
}

void WorldSession::HandleAutoDeclineGuildInvites(WorldPackets::Guild::AutoDeclineGuildInvites& packet)
{
    GetPlayer()->ApplyModFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_AUTO_DECLINE_GUILD, packet.Allow);
}

void WorldSession::HandleGuildRewardsQueryOpcode(WorldPackets::Guild::RequestGuildRewardsList& packet)
{
    // from sniffs, strange but testing
    if (packet.CurrentVersion != 0)
    {
        WorldPackets::Guild::GuildRewardList resp;
        resp.Version = packet.CurrentVersion;
        resp.RewardItems.clear();

        SendPacket(resp.Write());
        return;
    }

    if (Guild* guild = sGuildMgr->GetGuildById(_player->GetGuildId()))
    {
        std::vector<GuildReward> const& rewards = sGuildMgr->GetGuildRewards();

        WorldPackets::Guild::GuildRewardList rewardList;
        rewardList.Version = time(nullptr);
        rewardList.RewardItems.reserve(rewards.size());

        for (size_t i = 0; i < rewards.size(); i++)
        {
            WorldPackets::Guild::GuildRewardItem rewardItem;
            rewardItem.ItemID = rewards[i].Entry;
            if (rewards[i].AchievementsRequired.size() >= 2)
                rewardItem.Unk4 = 2;
            rewardItem.RaceMask = rewards[i].Racemask;
            rewardItem.MinGuildLevel = 0;
            rewardItem.MinGuildRep = rewards[i].Standing;
            rewardItem.AchievementsRequired = rewards[i].AchievementsRequired;
            rewardItem.Cost = rewards[i].Price;
            rewardList.RewardItems.push_back(rewardItem);
        }

        SendPacket(rewardList.Write());
    }
}

void WorldSession::HandleGuildQueryNews(WorldPackets::Guild::GuildQueryNews& packet)
{
    if (Guild* guild = _GetPlayerGuild(this))
        if (guild->GetGUID() == packet.GuildGUID)
            guild->SendNewsUpdate(this);
}

void WorldSession::HandleGuildNewsUpdateStickyOpcode(WorldPackets::Guild::GuildNewsUpdateSticky& packet)
{
    if (Guild* guild = _GetPlayerGuild(this))
        guild->HandleNewsSetSticky(this, packet.NewsID, packet.Sticky);
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

void WorldSession::HandleGuildRequestChallengeUpdate(WorldPackets::Guild::GuildChallengeUpdateRequest& /*packet*/)
{
    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendGuildChallengesInfo(this);
}

void WorldSession::HandleGuildSetFocusedAchievement(WorldPackets::Achievement::GuildSetFocusedAchievement& packet)
{
    if (Guild* guild = sGuildMgr->GetGuildById(_player->GetGuildId()))
        guild->GetAchievementMgr().SendAchievementInfo(_player, packet.AchievementID);
}
