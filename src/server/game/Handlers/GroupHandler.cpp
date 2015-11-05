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
#include "DatabaseEnv.h"
#include "Opcodes.h"
#include "Log.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectMgr.h"
#include "GroupMgr.h"
#include "Player.h"
#include "Group.h"
#include "SocialMgr.h"
#include "Util.h"
#include "SpellAuras.h"
#include "Vehicle.h"
#include "DB2Structure.h"
#include "DB2Stores.h"
#include "SpellAuraEffects.h"
#include "PartyPackets.h"

class Aura;

/* differeces from off:
-you can uninvite yourself - is is useful
-you can accept invitation even if leader went offline
*/
/* todo:
-group_destroyed msg is sent but not shown
-reduce xp gaining when in raid group
-quest sharing has to be corrected
-Master Loor corrections in sending SMSG_MASTER_LOOT_CANDIDATE_LIST
-raid markers
*/

void WorldSession::SendPartyResult(PartyOperation operation, std::string const& member, PartyResult res, uint32 val /* = 0 */)
{
    WorldPackets::Party::PartyCommandResult packet;
    packet.Name = member;
    packet.Command = operation;
    packet.Result = res;
    packet.ResultData = val;
    packet.ResultGUID;
    SendPacket(packet.Write());
}

void WorldSession::HandlePartyInvite(WorldPackets::Party::PartyInviteClient& packet)
{
    Player* c_player = GetPlayer();
    if (!c_player)
        return;

    // cheating
    if (!normalizePlayerName(packet.TargetName))
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    Player* player = sObjectAccessor->FindPlayerByName(packet.TargetName.c_str());

    // no player
    if (!player)
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    // restrict invite to GMs
    if (!sWorld->getBoolConfig(CONFIG_ALLOW_GM_GROUP) && !c_player->isGameMaster() && player->isGameMaster())
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    // can't group with
    if (!c_player->isGameMaster() && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP) && c_player->GetTeam() != player->GetTeam())
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_PLAYER_WRONG_FACTION);
        return;
    }
    if (c_player->GetInstanceId() != 0 && player->GetInstanceId() != 0 && c_player->GetInstanceId() != player->GetInstanceId() && c_player->GetMapId() == player->GetMapId())
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_TARGET_NOT_IN_INSTANCE_S);
        return;
    }
    // just ignore us
    if (player->GetInstanceId() != 0 && player->GetDungeonDifficultyID() != c_player->GetDungeonDifficultyID())
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_IGNORING_YOU_S);
        return;
    }

    if (player->GetSocial()->HasIgnore(c_player->GetGUID()))
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_IGNORING_YOU_S);
        return;
    }

    Group* group = c_player->GetGroup();
    if (group && group->isBGGroup())
        group = c_player->GetOriginalGroup();

    Group* group2 = player->GetGroup();
    if (group2 && group2->isBGGroup())
        group2 = player->GetOriginalGroup();
    // player already in another group or invited
    if (group2 || player->GetGroupInvite())
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_ALREADY_IN_GROUP_S);

        //! tell the player that they were invited but it failed as they were already in a group
        if (group2) 
        {
            WorldPackets::Party::PartyInvite partyInvite;
            partyInvite.Initialize(c_player, packet.ProposedRoles, false);
            player->GetSession()->SendPacket(partyInvite.Write());
        }
        return;
    }

    if (group)
    {
        // not have permissions for invite
        if (!group->IsLeader(c_player->GetGUID()) && !group->IsAssistant(c_player->GetGUID()) && !(group->GetGroupType() & GROUPTYPE_EVERYONE_IS_ASSISTANT))
        {
            SendPartyResult(PARTY_OP_INVITE, "", ERR_NOT_LEADER);
            return;
        }
        // not have place
        if (group->IsFull())
        {
            SendPartyResult(PARTY_OP_INVITE, "", ERR_GROUP_FULL);
            return;
        }
    }

    // ok, but group not exist, start a new group
    // but don't create and save the group to the DB until
    // at least one person joins
    if (!group)
    {
        group = new Group;
        // new group: if can't add then delete
        if (!group->AddLeaderInvite(c_player))
        {
            delete group;
            return;
        }
        if (!group->AddInvite(player))
        {
            delete group;
            return;
        }
    }
    else
    {
        // already existed group: if can't add then just leave
        if (!group->AddInvite(player))
        {
            return;
        }
    }

    WorldPackets::Party::PartyInvite partyInvite;
    partyInvite.Initialize(c_player, packet.ProposedRoles, true);
    player->GetSession()->SendPacket(partyInvite.Write());

    SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_PARTY_RESULT_OK);
}

void WorldSession::HandlePartyInviteResponse(WorldPackets::Party::PartyInviteResponse& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Group* group = player->GetGroupInvite();
    if (!group)
        return;

    if (packet.Accept)
    {
        // Remove player from invitees in any case
        group->RemoveInvite(player);

        if (group->GetLeaderGUID() == player->GetGUID())
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "HandleGroupAcceptOpcode: player %s(%d) tried to accept an invite to his own group", player->GetName(), player->GetGUIDLow());
            return;
        }

        // Group is full
        if (group->IsFull())
        {
            SendPartyResult(PARTY_OP_INVITE, "", ERR_GROUP_FULL);
            return;
        }

        Player* leader = ObjectAccessor::FindPlayer(group->GetLeaderGUID());

        // Forming a new group, create it
        if (!group->IsCreated())
        {
            // This can happen if the leader is zoning. To be removed once delayed actions for zoning are implemented
            if (!leader)
            {
                group->RemoveAllInvites();
                return;
            }

            // If we're about to create a group there really should be a leader present
            ASSERT(leader);
            group->RemoveInvite(leader);
            group->Create(leader);
            sGroupMgr->AddGroup(group);
        }

        // Everything is fine, do it, PLAYER'S GROUP IS SET IN ADDMEMBER!!!
        if (!group->AddMember(player))
            return;

        group->BroadcastGroupUpdate();
    }
    else
    {
        // Remember leader if online (group pointer will be invalid if group gets disbanded)
        Player* leader = ObjectAccessor::FindPlayer(group->GetLeaderGUID());

        // uninvite, group can be deleted
        player->UninviteFromGroup();

        if (!leader || !leader->GetSession())
            return;

        leader->GetSession()->SendPacket(WorldPackets::Party::GroupDecline(player->GetName()).Write());
    }
}

void WorldSession::HandlePartyUninvite(WorldPackets::Party::PartyUninvite& packet)
{
    if (packet.TargetGUID == GetPlayer()->GetGUID())
        return;

    PartyResult res = GetPlayer()->CanUninviteFromGroup();
    if (res != ERR_PARTY_RESULT_OK)
    {
        SendPartyResult(PARTY_OP_UNINVITE, "", res);
        return;
    }

    Group* grp = GetPlayer()->GetGroup();
    if (!grp)
        return;

    if (grp->IsLeader(packet.TargetGUID))
    {
        SendPartyResult(PARTY_OP_UNINVITE, "", ERR_NOT_LEADER);
        return;
    }

    if (grp->IsMember(packet.TargetGUID))
    {
        Player::RemoveFromGroup(grp, packet.TargetGUID, GROUP_REMOVEMETHOD_KICK, GetPlayer()->GetGUID(), packet.Reason.c_str());
        return;
    }

    if (Player* player = grp->GetInvited(packet.TargetGUID))
    {
        player->UninviteFromGroup();
        return;
    }

    SendPartyResult(PARTY_OP_UNINVITE, "", ERR_TARGET_NOT_IN_GROUP_S);
}

void WorldSession::HandleSetPartyLeader(WorldPackets::Party::SetPartyLeader& packet)
{
    Player* player = ObjectAccessor::FindPlayer(packet.TargetGUID);
    Group* group = GetPlayer()->GetGroup();

    if (!group || !player)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()) || player->GetGroup() != group)
        return;

    // Prevent exploits with instance saves
    for (GroupReference *itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
        if (Player* plr = itr->getSource())
            if (plr->GetMap() && plr->GetMap()->Instanceable())
                return;

    group->ChangeLeader(packet.TargetGUID, packet.PartyIndex);
    group->SendUpdate();
}

void WorldSession::HandleSetRole(WorldPackets::Party::SetRole& packet)
{
    WorldPackets::Party::RoleChangedInform roleChangedInform;

    Group* group = GetPlayer()->GetGroup();
    uint8 oldRole = group ? group->GetLfgRoles(packet.TargetGUID) : 0;
    if (oldRole == packet.Role)
        return;

    roleChangedInform.PartyIndex = packet.PartyIndex;
    roleChangedInform.From = GetPlayer()->GetGUID();
    roleChangedInform.ChangedUnit = packet.TargetGUID;
    roleChangedInform.OldRole = oldRole;
    roleChangedInform.NewRole = packet.Role;

    if (group)
    {
        group->BroadcastPacket(roleChangedInform.Write(), false);
        group->SetLfgRoles(packet.TargetGUID, packet.Role);
    }
    else
        SendPacket(roleChangedInform.Write());
}

void WorldSession::HandleLeaveGroup(WorldPackets::Party::LeaveGroup& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Group* grp = player->GetGroup();
    if (!grp)
        return;

    if (player->InBattleground())
    {
        SendPartyResult(PARTY_OP_INVITE, "", ERR_INVITE_RESTRICTED);
        return;
    }

    SendPartyResult(PARTY_OP_LEAVE, player->GetName(), ERR_PARTY_RESULT_OK);
    player->RemoveFromGroup(GROUP_REMOVEMETHOD_LEAVE);
}

//! 5.4.1
void WorldSession::HandleDoMasterLootRoll(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint8 slot = 0;

    recvData >> slot;

    if (!_player->GetGroup() || _player->GetGroup()->GetLooterGuid() != _player->GetGUID())
    {
        _player->SendLootRelease(guid);
        return;
    }

    Loot* loot = NULL;

    if (guid.IsCreatureOrVehicle())
    {
        Creature* creature = GetPlayer()->GetMap()->GetCreature(guid);
        if (!creature)
            return;

        loot = &creature->loot;
    }
    else if (guid.IsGameObject())
    {
        GameObject* pGO = GetPlayer()->GetMap()->GetGameObject(guid);
        if (!pGO)
            return;

        loot = &pGO->loot;
    }

    if (!loot)
        return;

    if (slot >= loot->items.size() + loot->quest_items.size())
    {
        sLog->outDebug(LOG_FILTER_LOOT, "MasterLootItem: Player %s might be using a hack! (slot %d, size %lu)", GetPlayer()->GetName(), slot, (unsigned long)loot->items.size());
        return;
    }

    LootItem& item = slot >= loot->items.size() ? loot->quest_items[slot - loot->items.size()] : loot->items[slot];

    _player->GetGroup()->DoRollForAllMembers(guid, slot, _player->GetMapId(), loot, item, _player);
}

//! 5.4.1
void WorldSession::HandleLootMasterGiveOpcode(WorldPacket& recvData)
{
    ObjectGuid target_playerguid;

    uint32 count = recvData.ReadBits(23);
    GuidVector guids(count);
    std::vector<uint8> types(count);

    //recvData.ReadGuidMask<6, 2>(target_playerguid);
    
    if (count > 100)
    {
        recvData.rfinish();
        return;
    }

    for (uint32 i = 0; i < count; ++i)
        //recvData.ReadGuidMask<1, 0, 5, 2, 3, 6, 7, 4>(guids[i]);

    ///

    //recvData.ReadGuidMask<5, 7, 0, 3, 1, 4>(target_playerguid);

    for (uint32 i = 0; i < count; ++i)
    {
        //recvData.ReadGuidBytes<5, 2, 3, 0, 6, 7>(guids[i]);
        recvData >> types[i];
        //recvData.ReadGuidBytes<1, 4>(guids[i]);
    }

    //recvData.ReadGuidBytes<5, 3, 6, 7, 1, 2, 0, 4>(target_playerguid);

    Group* group = _player->GetGroup();
    if (!group || group->isLFGGroup() || group->GetLooterGuid() != _player->GetGUID())
    {
        _player->SendLootRelease(GetPlayer()->GetLootGUID());
        return;
    }

    //target_playerguid = GUID_LOPART(target_playerguid); //WARNING! TMP! plr should have off-like hi-guid, as server not suport it  - cut.

    Player* target = ObjectAccessor::FindPlayer(target_playerguid);
    if (!target)
        return;

    for (uint32 i = 0; i < count; ++i)
    {
        ObjectGuid lootguid = guids[i];
        uint8 slotid = types[i];
        Loot* loot = NULL;

        if (lootguid.IsCreatureOrVehicle())
        {
            Creature* creature = GetPlayer()->GetMap()->GetCreature(lootguid);
            if (!creature)
                return;

            loot = &creature->loot;
        }
        else if (lootguid.IsGameObject())
        {
            GameObject* pGO = GetPlayer()->GetMap()->GetGameObject(lootguid);
            if (!pGO)
                return;

            loot = &pGO->loot;
        }

        if (!loot)
            return;

        if (slotid >= loot->items.size() + loot->quest_items.size())
        {
            sLog->outDebug(LOG_FILTER_LOOT, "MasterLootItem: Player %s might be using a hack! (slot %d, size %lu)", GetPlayer()->GetName(), slotid, (unsigned long)loot->items.size());
            return;
        }

        LootItem& item = slotid >= loot->items.size() ? loot->quest_items[slotid - loot->items.size()] : loot->items[slotid];
        if (item.currency)
        {
            sLog->outDebug(LOG_FILTER_LOOT, "WorldSession::HandleLootMasterGiveOpcode: player %s tried to give currency via master loot! Hack alert! Slot %u, currency id %u", GetPlayer()->GetName(), slotid, item.item.ItemID);
            return;
        }

        ItemPosCountVec dest;
        InventoryResult msg = target->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item.item.ItemID, item.count);
        if (item.follow_loot_rules && !item.AllowedForPlayer(target))
            msg = EQUIP_ERR_CANT_EQUIP_EVER;
        if (msg != EQUIP_ERR_OK)
        {
            target->SendEquipError(msg, NULL, NULL, item.item.ItemID);
            // send duplicate of error massage to master looter
            _player->SendEquipError(msg, NULL, NULL, item.item.ItemID);
            return;
        }

        // delete roll's in progress for this aoeSlot
        group->ErraseRollbyRealSlot(slotid, loot);

        // ToDo: check for already rolled items. This could posible on packet spaming (special tools should be writen, no so important now)

        // list of players allowed to receive this item in trade
        GuidSet looters = item.GetAllowedLooters();

        // not move item from loot to target inventory
        Item* newitem = target->StoreNewItem(dest, item.item.ItemID, true, item.item.RandomPropertiesID, looters, item.item.ItemBonus.BonusListIDs);
        target->SendNewItem(newitem, uint32(item.count), false, false, true);
        target->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM, item.item.ItemID, item.count);
        target->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE, loot->loot_type, item.count);
        target->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM, item.item.ItemID, item.count);

        // mark as looted
        item.count=0;
        item.is_looted=true;

        loot->NotifyItemRemoved(slotid);
        --loot->unlootedCount;
    }
}

void WorldSession::HandleSetLootMethod(WorldPackets::Party::SetLootMethod& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group || group->RollIsActive())
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()) || group->isLFGGroup())
        return;

    if (packet.LootThreshold < ITEM_QUALITY_UNCOMMON || packet.LootThreshold > ITEM_QUALITY_ARTIFACT)
        return;

    if (packet.LootMethod == MASTER_LOOT && !group->IsMember(packet.LootMasterGUID))
        return;

    group->SetLootMethod(static_cast<LootMethod>(packet.LootMethod));
    group->SetLooterGuid(packet.LootMasterGUID);
    group->SetLootThreshold(static_cast<ItemQualities>(packet.LootThreshold));
    group->SendUpdate();
}

void WorldSession::HandleMinimapPing(WorldPackets::Party::MinimapPingClient& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (!player->GetGroup())
        return;

    WorldPackets::Party::MinimapPing minimapPing;
    minimapPing.Sender = player->GetGUID();
    minimapPing.PositionX = packet.PositionX;
    minimapPing.PositionY = packet.PositionY;
    player->GetGroup()->BroadcastPacket(minimapPing.Write(), true, -1, player->GetGUID());
}

void WorldSession::HandleUpdateRaidTarget(WorldPackets::Party::UpdateRaidTarget& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Group* group = player->GetGroup();
    if (!group)
        return;

    if (packet.Symbol == 0xFF)
        group->SendTargetIconList(this, packet.PartyIndex);
    else
    {
        if (!group->IsLeader(player->GetGUID()) && !group->IsAssistant(player->GetGUID()) && !(group->GetGroupType() & GROUPTYPE_EVERYONE_IS_ASSISTANT))
            return;

        group->SetTargetIcon(packet.Symbol, player->GetGUID(), packet.Target, packet.PartyIndex);
    }
}

void WorldSession::HandleConvertRaid(WorldPackets::Party::ConvertRaid& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (_player->InBattleground())
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()) || group->GetMembersCount() < 2)
        return;

    SendPartyResult(PARTY_OP_INVITE, "", ERR_PARTY_RESULT_OK);

    if (packet.Raid && !group->isRaidGroup())
        group->ConvertToRaid();
    else
        group->ConvertToGroup();
}

void WorldSession::HandleChangeSubGroup(WorldPackets::Party::ChangeSubGroup& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (packet.NewSubGroup >= MAX_RAID_SUBGROUPS)
        return;

    ObjectGuid senderGuid = GetPlayer()->GetGUID();
    if (!group->IsLeader(senderGuid) && !group->IsAssistant(senderGuid) && !(group->GetGroupType() & GROUPTYPE_EVERYONE_IS_ASSISTANT))
        return;

    if (!group->HasFreeSlotSubGroup(packet.NewSubGroup))
        return;

    group->ChangeMembersGroup(packet.TargetGUID, packet.NewSubGroup);
}

//! ToDo: Write swipe command.
void WorldSession::HandleSwapSubGroups(WorldPackets::Party::SwapSubGroups& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    group->SendUpdate();
}

void WorldSession::HandleSetEveryoneIsAssistant(WorldPackets::Party::SetEveryoneIsAssistant& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;

    group->ChangeFlagEveryoneAssistant(packet.EveryoneIsAssistant);
}

void WorldSession::HandleSetAssistantLeader(WorldPackets::Party::SetAssistantLeader& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;

    group->SetGroupMemberFlag(packet.Target, packet.Apply, MEMBER_FLAG_ASSISTANT);
    group->SendUpdate();
}

//! 5.4.1
void WorldSession::HandlePartyAssignmentOpcode(WorldPacket& recvData)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    ObjectGuid senderGuid = GetPlayer()->GetGUID();
    if (!group->IsLeader(senderGuid) && !group->IsAssistant(senderGuid) && !(group->GetGroupType() & GROUPTYPE_EVERYONE_IS_ASSISTANT))
        return;

    uint8 assignment;
    uint8 unk;
    bool apply;
    ObjectGuid guid;

    recvData >> unk >> assignment;

    //recvData.ReadGuidMask<3, 5>(guid);
    apply = recvData.ReadBit();
    //recvData.ReadGuidMask<0, 7, 2, 6, 4, 1>(guid);
    //recvData.ReadGuidBytes<4, 1, 0, 6, 5, 3, 7, 2>(guid);

    switch (assignment)
    {
    case GROUP_ASSIGN_MAINASSIST:
        group->RemoveUniqueGroupMemberFlag(MEMBER_FLAG_MAINASSIST);
        group->SetGroupMemberFlag(guid, apply, MEMBER_FLAG_MAINASSIST);
        break;
    case GROUP_ASSIGN_MAINTANK:
        group->RemoveUniqueGroupMemberFlag(MEMBER_FLAG_MAINTANK);           // Remove main assist flag from current if any.
        group->SetGroupMemberFlag(guid, apply, MEMBER_FLAG_MAINTANK);
    default:
        break;
    }

    group->SendUpdate();
}

void WorldSession::HandleDoReadyCheck(WorldPackets::Party::DoReadyCheck& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()) && !(group->GetGroupType() & GROUPTYPE_EVERYONE_IS_ASSISTANT))
        return;

    group->SetReadyCheckCount(1);

    WorldPackets::Party::ReadyCheckStarted readyCheckStarted;
    readyCheckStarted.PartyGUID = group->GetGUID();
    readyCheckStarted.PartyIndex = packet.PartyIndex;
    readyCheckStarted.InitiatorGUID = GetPlayer()->GetGUID();
    readyCheckStarted.Duration = READY_CHECK_DURATION;
    group->BroadcastPacket(readyCheckStarted.Write(), false, -1);

    group->OfflineReadyCheck();
}

void WorldSession::HandleReadyCheckResponse(WorldPackets::Party::ReadyCheckResponseClient& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    group->SetReadyCheckCount(group->GetReadyCheckCount() + 1);

    WorldPackets::Party::ReadyCheckResponse response;
    response.PartyGUID = group->GetGUID();
    response.Player = GetPlayer()->GetGUID();
    response.IsReady = packet.IsReady;
    group->BroadcastPacket(response.Write(), true);

    if (group->GetReadyCheckCount() >= group->GetMembersCount())
    {
        WorldPackets::Party::ReadyCheckCompleted readyCheckCompleted;
        readyCheckCompleted.PartyIndex = packet.PartyIndex;
        readyCheckCompleted.PartyGUID = group->GetGUID();
        group->BroadcastPacket(readyCheckCompleted.Write(), true);
    }
}

void WorldSession::HandleRequestPartyJoinUpdates(WorldPackets::Party::RequestPartyJoinUpdates& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    group->SendTargetIconList(this, packet.PartyIndex);
    group->SendRaidMarkersChanged(this, packet.PartyIndex);
}

void WorldSession::HandleRequestPartyMemberStats(WorldPackets::Party::RequestPartyMemberStats& packet)
{
    WorldPackets::Party::PartyMemberStats partyMemberStats;

    Player* player = ObjectAccessor::FindPlayer(packet.TargetGUID);
    if (!player)
    {
        partyMemberStats.MemberStats.GUID = packet.TargetGUID;
        partyMemberStats.MemberStats.Status = MEMBER_STATUS_OFFLINE;
    }
    else
        partyMemberStats.Initialize(player);

    SendPacket(partyMemberStats.Write());
}

void WorldSession::HandleRequestRaidInfo(WorldPackets::Party::RequestRaidInfo& /*packet*/)
{
    _player->SendRaidInfo();
}

void WorldSession::HandleOptOutOfLoot(WorldPackets::Party::OptOutOfLoot& packet)
{
    if (!GetPlayer())
    {
        if (packet.PassOnLoot)
            sLog->outError(LOG_FILTER_NETWORKIO, "CMSG_OPT_OUT_OF_LOOT value<>0 for not-loaded character!");
        return;
    }

    GetPlayer()->SetPassOnGroupLoot(packet.PassOnLoot);
}

void WorldSession::HandleInitiateRolePoll(WorldPackets::Party::InitiateRolePoll& packet)
{
    if (!GetPlayer()->GetGroup())
        return;

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()) && !(group->GetGroupType() & GROUPTYPE_EVERYONE_IS_ASSISTANT))
    {
        SendPartyResult(PARTY_OP_INVITE, "", ERR_NOT_LEADER);
        return;
    }

    WorldPackets::Party::RolePollInform rolePollInform;
    rolePollInform.From = GetPlayer()->GetGUID();
    rolePollInform.PartyIndex = packet.PartyIndex;
    group->BroadcastPacket(rolePollInform.Write(), true);
}

void WorldSession::HandleClearRaidMarker(WorldPackets::Party::ClearRaidMarker& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Group* group = player->GetGroup();
    if (!group)
        return;

    if (group->isRaidGroup() && !group->IsLeader(player->GetGUID()) && !group->IsAssistant(player->GetGUID()))
        return;

    group->DeleteRaidMarker(packet.MarkerId);
}
