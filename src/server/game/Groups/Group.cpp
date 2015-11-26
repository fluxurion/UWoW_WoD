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
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Player.h"
#include "World.h"
#include "ObjectMgr.h"
#include "GroupMgr.h"
#include "Group.h"
#include "Formulas.h"
#include "ObjectAccessor.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "MapManager.h"
#include "InstanceSaveMgr.h"
#include "MapInstanced.h"
#include "Util.h"
#include "LFGMgr.h"
#include "UpdateFieldFlags.h"
#include "GuildMgr.h"
#include "Bracket.h"
#include "LootPackets.h"
#include "PartyPackets.h"
#include "ItemPackets.h"
#include "LootMgr.h"

Roll::Roll(ObjectGuid _guid, LootItem const& li) : itemCount(li.count), totalPlayersRolling(0),
    totalNeed(0), totalGreed(0), totalPass(0), itemSlot(0), rollVoteMask(ROLL_ALL_TYPE_NO_DISENCHANT), aoeSlot(0)
{
    item.itemGUID = _guid;
    item.ItemID = li.item.ItemID;
    item.RandomPropertiesID = li.item.RandomPropertiesID;
    item.RandomPropertiesSeed = li.item.RandomPropertiesSeed;
    item.ItemBonus.Context = li.item.ItemBonus.Context;
    item.ItemBonus.BonusListIDs = li.item.ItemBonus.BonusListIDs;
}

Roll::~Roll()
{
}

void Roll::setLoot(Loot* pLoot)
{
    link(pLoot, this);
}

Loot* Roll::getLoot()
{
    return getTarget();
}

Group::Group() : m_leaderGuid(), m_leaderName(""), m_groupType(GROUPTYPE_NORMAL),
    m_dungeonDifficulty(DIFFICULTY_NORMAL), m_raidDifficulty(DIFFICULTY_10_N), m_legacyRaidDifficulty(DIFFICULTY_10_N),
    m_bgGroup(NULL), m_bfGroup(NULL), m_lootMethod(FREE_FOR_ALL), m_lootThreshold(ITEM_QUALITY_UNCOMMON), m_looterGuid(),
    m_subGroupsCounts(NULL), m_guid(), m_counter(0), m_maxEnchantingLevel(0), m_dbStoreId(0), m_readyCheckCount(0), m_readyCheck(false),
    m_aoe_slots(0), m_activeMarkers(0)
{
    for (uint8 i = 0; i < TARGET_ICONS_COUNT; ++i)
        m_targetIcons[i].Clear();

    for (uint8 i = 0; i < RAID_MARKERS_COUNT; ++i)
        m_markers[i] = nullptr;
}

Group::~Group()
{
    if (m_bgGroup)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Group::~Group: battleground group being deleted.");
        if (m_bgGroup->GetBgRaid(ALLIANCE) == this) m_bgGroup->SetBgRaid(ALLIANCE, NULL);
        else if (m_bgGroup->GetBgRaid(HORDE) == this) m_bgGroup->SetBgRaid(HORDE, NULL);
        else sLog->outError(LOG_FILTER_GENERAL, "Group::~Group: battleground group is not linked to the correct battleground.");
    }
    Rolls::iterator itr;
    while (!RollId.empty())
    {
        itr = RollId.begin();
        Roll *r = *itr;
        RollId.erase(itr);
        delete(r);
    }

    // it is undefined whether objectmgr (which stores the groups) or instancesavemgr
    // will be unloaded first so we must be prepared for both cases
    // this may unload some instance saves
    for (uint8 i = 0; i < MAX_BOUND; ++i)
        for (BoundInstancesMap::iterator itr2 = m_boundInstances[i].begin(); itr2 != m_boundInstances[i].end(); ++itr2)
            itr2->second.save->RemoveGroup(this);

    // Sub group counters clean up
    delete[] m_subGroupsCounts;
}

bool Group::Create(Player* leader)
{
    ObjectGuid leaderGuid = leader->GetGUID();

    m_guid = ObjectGuid::Create<HighGuid::Party>(sGroupMgr->GenerateGroupId());
    m_leaderGuid = leaderGuid;
    m_leaderName = leader->GetName();

    m_groupType  = (isBGGroup() || isBFGroup()) ? GROUPTYPE_BGRAID : GROUPTYPE_NORMAL;

    if (m_groupType & GROUPTYPE_RAID)
        _initRaidSubGroupsCounter();

    m_lootMethod = GROUP_LOOT;
    m_lootThreshold = ITEM_QUALITY_UNCOMMON;
    m_looterGuid = leaderGuid;

    m_dungeonDifficulty = DIFFICULTY_NORMAL;
    m_raidDifficulty = DIFFICULTY_NORMAL_RAID;
    m_legacyRaidDifficulty = DIFFICULTY_10_N;

    if (!isBGGroup() && !isBFGroup())
    {
        m_dungeonDifficulty = leader->GetDungeonDifficultyID();
        m_raidDifficulty = leader->GetRaidDifficultyID();
        m_legacyRaidDifficulty = leader->GetLegacyRaidDifficultyID();

        m_dbStoreId = sGroupMgr->GenerateNewGroupDbStoreId();

        sGroupMgr->RegisterGroupDbStoreId(m_dbStoreId, this);

        // Store group in database
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GROUP);

        uint8 index = 0;

        stmt->setUInt32(index++, m_dbStoreId);
        stmt->setUInt64(index++, m_leaderGuid.GetCounter());
        stmt->setUInt8(index++, uint8(m_lootMethod));
        stmt->setUInt64(index++, m_looterGuid.GetCounter());
        stmt->setUInt8(index++, uint8(m_lootThreshold));
        stmt->setBinary(index++, m_targetIcons[0].GetRawValue());
        stmt->setBinary(index++, m_targetIcons[1].GetRawValue());
        stmt->setBinary(index++, m_targetIcons[2].GetRawValue());
        stmt->setBinary(index++, m_targetIcons[3].GetRawValue());
        stmt->setBinary(index++, m_targetIcons[4].GetRawValue());
        stmt->setBinary(index++, m_targetIcons[5].GetRawValue());
        stmt->setBinary(index++, m_targetIcons[6].GetRawValue());
        stmt->setBinary(index++, m_targetIcons[7].GetRawValue());
        stmt->setUInt8(index++, uint8(m_groupType));
        stmt->setUInt32(index++, uint8(m_dungeonDifficulty));
        stmt->setUInt32(index++, uint8(m_legacyRaidDifficulty));
        stmt->setUInt32(index++, uint8(m_raidDifficulty));

        CharacterDatabase.Execute(stmt);


        ASSERT(AddMember(leader)); // If the leader can't be added to a new group because it appears full, something is clearly wrong.

        if (!isLFGGroup())
            Player::ConvertInstancesToGroup(leader, this, false);
    }
    else if (!AddMember(leader))
        return false;

    return true;
}

void Group::LoadGroupFromDB(Field* fields)
{
    m_dbStoreId = fields[15].GetUInt32();
    m_guid = ObjectGuid::Create<HighGuid::Party>(sGroupMgr->GenerateGroupId());
    m_leaderGuid = ObjectGuid::Create<HighGuid::Player>(fields[0].GetUInt64());

    // group leader not exist
    if (!ObjectMgr::GetPlayerNameByGUID(m_leaderGuid, m_leaderName))
        return;

    m_lootMethod = LootMethod(fields[1].GetUInt8());
    m_looterGuid = ObjectGuid::Create<HighGuid::Player>(fields[2].GetUInt64());
    m_lootThreshold = ItemQualities(fields[3].GetUInt8());

    for (uint8 i = 0; i < TARGET_ICONS_COUNT; ++i)
        m_targetIcons[i].SetRawValue(fields[4 + i].GetBinary());

    m_groupType  = GroupType(fields[12].GetUInt8());
    if (m_groupType & GROUPTYPE_RAID)
        _initRaidSubGroupsCounter();

    m_dungeonDifficulty = Player::CheckLoadedDungeonDifficultyID(Difficulty(fields[13].GetUInt8()));
    m_raidDifficulty = Player::CheckLoadedRaidDifficultyID(Difficulty(fields[14].GetUInt8()));
    m_legacyRaidDifficulty = Player::CheckLoadedRaidDifficultyID(Difficulty(fields[15].GetUInt8()));

    if (m_groupType & GROUPTYPE_LFG)
        sLFGMgr->_LoadFromDB(fields, GetGUID());
}

void Group::LoadMemberFromDB(ObjectGuid::LowType guidLow, uint8 memberFlags, uint8 subgroup, uint8 roles)
{
    MemberSlot member;
    member.guid = ObjectGuid::Create<HighGuid::Player>(guidLow);

    // skip non-existed member
    if (!ObjectMgr::GetPlayerNameByGUID(member.guid, member.name))
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_MEMBER);
        stmt->setUInt64(0, guidLow);
        CharacterDatabase.Execute(stmt);
        return;
    }

    member.group = subgroup;
    member.flags = memberFlags;
    member.roles = roles;

    m_memberSlots.push_back(member);

    SubGroupCounterIncrease(subgroup);

    sLFGMgr->SetupGroupMember(member.guid, GetGUID());
}

void Group::ChangeFlagEveryoneAssistant(bool apply)
{
    if (apply)
        m_groupType = GroupType(m_groupType | GROUPTYPE_EVERYONE_IS_ASSISTANT);
    else
        m_groupType = GroupType(m_groupType &~ GROUPTYPE_EVERYONE_IS_ASSISTANT);

    this->SendUpdate();
}

void Group::ConvertToLFG(lfg::LFGDungeonData const* dungeon)
{
    if (dungeon->dbc->subType == LFG_SUBTYPE_FLEX || dungeon->dbc->subType == LFG_SUBTYPE_RAID)
        ConvertToRaid(false);

    m_groupType = GroupType(m_groupType | GROUPTYPE_LFG | GROUPTYPE_LFG_RESTRICTED);
    m_lootMethod = NEED_BEFORE_GREED;
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_TYPE);

        stmt->setUInt8(0, uint8(m_groupType));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();
}

void Group::ConvertToRaid(bool update)
{
    m_groupType = GroupType(m_groupType | GROUPTYPE_RAID);

    _initRaidSubGroupsCounter();

    if (update && !isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_TYPE);

        stmt->setUInt8(0, uint8(m_groupType));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    if (update)
        SendUpdate();

    // update quest related GO states (quest activity dependent from raid membership)
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
        if (Player* player = ObjectAccessor::FindPlayer(citr->guid))
            player->UpdateForQuestWorldObjects();
}

void Group::ConvertToGroup()
{
    if (m_memberSlots.size() > 5)
        return; // What message error should we send?

    m_groupType = GroupType(GROUPTYPE_NORMAL);

    if (m_subGroupsCounts)
    {
        delete[] m_subGroupsCounts;
        m_subGroupsCounts = NULL;
    }

    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_TYPE);

        stmt->setUInt8(0, uint8(m_groupType));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();

    // update quest related GO states (quest activity dependent from raid membership)
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
        if (Player* player = ObjectAccessor::FindPlayer(citr->guid))
            player->UpdateForQuestWorldObjects();
}

bool Group::AddInvite(Player* player)
{
    if (!player || player->GetGroupInvite())
        return false;
    Group* group = player->GetGroup();
    if (group && (group->isBGGroup() || group->isBFGroup()))
        group = player->GetOriginalGroup();
    if (group)
        return false;

    RemoveInvite(player);

    m_invitees.insert(player);

    player->SetGroupInvite(this);

    sScriptMgr->OnGroupInviteMember(this, player->GetGUID());

    return true;
}

bool Group::AddLeaderInvite(Player* player)
{
    if (!AddInvite(player))
        return false;

    m_leaderGuid = player->GetGUID();
    m_leaderName = player->GetName();
    return true;
}

void Group::RemoveInvite(Player* player)
{
    if (player)
    {
        if(!m_invitees.empty())
            m_invitees.erase(player);
        player->SetGroupInvite(NULL);
    }
}

void Group::RemoveAllInvites()
{
    for (InvitesList::iterator itr=m_invitees.begin(); itr != m_invitees.end(); ++itr)
        if (*itr)
            (*itr)->SetGroupInvite(NULL);

    m_invitees.clear();
}

Player* Group::GetInvited(ObjectGuid guid) const
{
    for (InvitesList::const_iterator itr = m_invitees.begin(); itr != m_invitees.end(); ++itr)
    {
        if ((*itr) && (*itr)->GetGUID() == guid)
            return (*itr);
    }
    return NULL;
}

Player* Group::GetInvited(std::string const& name) const
{
    for (InvitesList::const_iterator itr = m_invitees.begin(); itr != m_invitees.end(); ++itr)
    {
        if ((*itr) && (*itr)->GetName() == name)
            return (*itr);
    }
    return NULL;
}

bool Group::AddCreatureMember(Creature* creature)
{
    uint8 subGroup = 0;
    if (m_subGroupsCounts)
    {
        bool groupFound = false;
        for (; subGroup < MAX_RAID_SUBGROUPS; ++subGroup)
        {
            if (m_subGroupsCounts[subGroup] < MAX_GROUP_SIZE)
            {
                groupFound = true;
                break;
            }
        }
        if (!groupFound)
            return false;
    }

    MemberSlot member;
    member.guid      = creature->GetGUID();
    member.name      = creature->GetName();
    member.group     = subGroup;
    member.flags     = 0;
    member.roles     = 0;
    m_memberSlots.push_back(member);

    SubGroupCounterIncrease(subGroup);
    SendUpdate();
    return true;
}

bool Group::AddMember(Player* player)
{
    // Get first not-full group
    uint8 subGroup = 0;
    if (m_subGroupsCounts)
    {
        bool groupFound = false;
        for (; subGroup < MAX_RAID_SUBGROUPS; ++subGroup)
        {
            if (m_subGroupsCounts[subGroup] < MAX_GROUP_SIZE)
            {
                groupFound = true;
                break;
            }
        }
        // We are raid group and no one slot is free
        if (!groupFound)
            return false;
    }

    MemberSlot member;
    member.guid      = player->GetGUID();
    member.name      = player->GetName();
    member.group     = subGroup;
    member.flags     = 0;
    member.roles     = 0;
    m_memberSlots.push_back(member);

    SubGroupCounterIncrease(subGroup);

    if (player)
    {
        player->SetGroupInvite(NULL);
        if (player->GetGroup())
        {
            if (isBGGroup() || isBFGroup()) // if player is in group and he is being added to BG raid group, then call SetBattlegroundRaid()
                player->SetBattlegroundOrBattlefieldRaid(this, subGroup);
            else //if player is in bg raid and we are adding him to normal group, then call SetOriginalGroup()
                player->SetOriginalGroup(this, subGroup);
        }
        else //if player is not in group, then call set group
            player->SetGroup(this, subGroup);

        // if the same group invites the player back, cancel the homebind timer
        InstanceGroupBind* bind = GetBoundInstance(player);
        if (bind && bind->save->GetInstanceId() == player->GetInstanceId())
            player->m_InstanceValid = true;
    }

    if (!isRaidGroup())                                      // reset targetIcons for non-raid-groups
    {
        for (uint8 i = 0; i < TARGET_ICONS_COUNT; ++i)
            m_targetIcons[i].Clear();
    }

    // insert into the table if we're not a battleground group
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GROUP_MEMBER);

        stmt->setUInt32(0, m_dbStoreId);
        stmt->setUInt64(1, member.guid.GetCounter());
        stmt->setUInt8(2, member.flags);
        stmt->setUInt8(3, member.group);
        stmt->setUInt8(4, member.roles);

        CharacterDatabase.Execute(stmt);

    }

    SendUpdate();

    if (player)
    {
        sScriptMgr->OnGroupAddMember(this, player->GetGUID());

        if (!IsLeader(player->GetGUID()) && !isBGGroup() && !isBFGroup())
        {
            // reset the new member's instances, unless he is currently in one of them
            // including raid/heroic instances that they are not permanently bound to!
            player->ResetInstances(INSTANCE_RESET_GROUP_JOIN, false, false);
            player->ResetInstances(INSTANCE_RESET_GROUP_JOIN, true, false);
            player->ResetInstances(INSTANCE_RESET_GROUP_JOIN, true, true);

            if (player->getLevel() >= LEVELREQUIREMENT_HEROIC)
            {
                if (player->GetDungeonDifficultyID() != GetDungeonDifficultyID())
                {
                    player->SetDungeonDifficultyID(GetDungeonDifficultyID());
                    player->SendDungeonDifficulty();
                }
                if (player->GetRaidDifficultyID() != GetRaidDifficultyID())
                {
                    player->SetRaidDifficultyID(GetRaidDifficultyID());
                    player->SendRaidDifficulty(false);
                }
                if (player->GetLegacyRaidDifficultyID() != GetLegacyRaidDifficultyID())
                {
                     player->SetLegacyRaidDifficultyID(GetLegacyRaidDifficultyID());
                     player->SendRaidDifficulty(true);
                }
            }
        }

        player->SetGroupUpdateFlag(GROUP_UPDATE_FULL);
        if (Pet* pet = player->GetPet())
            pet->SetGroupUpdateFlag(GROUP_UPDATE_PET_FULL);

        UpdatePlayerOutOfRange(player);

        // quest related GO state dependent from raid membership
        if (isRaidGroup())
            player->UpdateForQuestWorldObjects();

        {
            // Broadcast new player group member fields to rest of the group
            player->SetFieldNotifyFlag(UF_FLAG_PARTY_MEMBER);

            UpdateData groupData(player->GetMapId());
            WorldPacket groupDataPacket;

            // Broadcast group members' fields to player
            for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
            {
                if (itr->getSource() == player)
                    continue;

                if (Player* member = itr->getSource())
                {
                    if (player->HaveAtClient(member))   // must be on the same map, or shit will break
                    {
                        member->SetFieldNotifyFlag(UF_FLAG_PARTY_MEMBER);
                        member->BuildValuesUpdateBlockForPlayer(&groupData, player);
                        member->RemoveFieldNotifyFlag(UF_FLAG_PARTY_MEMBER);
                    }

                    if (member->HaveAtClient(player))
                    {
                        UpdateData newData(player->GetMapId());
                        WorldPacket newDataPacket;
                        player->BuildValuesUpdateBlockForPlayer(&newData, member);
                        if (newData.HasData())
                        {
                            newData.BuildPacket(&newDataPacket);
                            member->SendDirectMessage(&newDataPacket);
                        }
                    }
                }
            }

            if (groupData.HasData())
            {
                groupData.BuildPacket(&groupDataPacket);
                player->SendDirectMessage(&groupDataPacket);
            }

            player->RemoveFieldNotifyFlag(UF_FLAG_PARTY_MEMBER);
        }

        if (m_maxEnchantingLevel < player->GetSkillValue(SKILL_ENCHANTING))
            m_maxEnchantingLevel = player->GetSkillValue(SKILL_ENCHANTING);
    }

    return true;
}

bool Group::RemoveCreatureMember(ObjectGuid const& guid)
{
    if (guid.IsEmpty())
        return false;

    BroadcastGroupUpdate();

    member_witerator slot = _getMemberWSlot(guid);
    if (slot != m_memberSlots.end())
    {
        SubGroupCounterDecrease(slot->group);
        m_memberSlots.erase(slot);
    }

    SendUpdate();

    return true;
}

bool Group::RemoveMember(ObjectGuid const& guid, RemoveMethod const& method /*= GROUP_REMOVEMETHOD_DEFAULT*/, ObjectGuid kicker /*= 0*/, char const* reason /*= NULL*/)
{
    BroadcastGroupUpdate();

    sScriptMgr->OnGroupRemoveMember(this, guid, method, kicker, reason);

    // LFG group vote kick handled in scripts
    if (isLFGGroup() && method == GROUP_REMOVEMETHOD_KICK)
        return !m_memberSlots.empty();

    // remove member and change leader (if need) only if strong more 2 members _before_ member remove (BG/BF allow 1 member group)
    if (GetMembersCount() > ((isBGGroup() || isLFGGroup() || isBFGroup()) ? 1u : 2u))
    {
        Player* player = ObjectAccessor::GetObjectInOrOutOfWorld(guid, (Player*)NULL);
        if (player)
        {
            // Battleground group handling
            if (isBGGroup() || isBFGroup())
                player->RemoveFromBattlegroundOrBattlefieldRaid();
            else
                // Regular group
            {
                if (player->GetOriginalGroup() == this)
                    player->SetOriginalGroup(NULL);
                else
                    player->SetGroup(NULL);

                // quest related GO state dependent from raid membership
                player->UpdateForQuestWorldObjects();
            }

            if (method == GROUP_REMOVEMETHOD_KICK || method == GROUP_REMOVEMETHOD_KICK_LFG)
            {
                WorldPacket data(SMSG_GROUP_UNINVITE, 0);
                player->GetSession()->SendPacket(&data);
            }

            _homebindIfInstance(player);
        }

        // Remove player from group in DB
        if (!isBGGroup() && !isBFGroup())
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_MEMBER);
            stmt->setUInt64(0, guid.GetCounter());
            CharacterDatabase.Execute(stmt);
            DelinkMember(guid);
        }

        // Reevaluate group enchanter if the leaving player had enchanting skill or the player is offline
        if ((player && player->GetSkillValue(SKILL_ENCHANTING)) || !player)
            ResetMaxEnchantingLevel();

        // Remove player from loot rolls
        for (Rolls::iterator it = RollId.begin(); it != RollId.end(); ++it)
        {
            Roll* roll = *it;
            Roll::PlayerVote::iterator itr2 = roll->playerVote.find(guid);
            if (itr2 == roll->playerVote.end())
                continue;

            if (itr2->second == GREED || itr2->second == DISENCHANT)
                --roll->totalGreed;
            else if (itr2->second == NEED)
                --roll->totalNeed;
            else if (itr2->second == PASS)
                --roll->totalPass;

            if (itr2->second != NOT_VALID)
                --roll->totalPlayersRolling;

            roll->playerVote.erase(itr2);

            CountRollVote(guid, roll->aoeSlot, MAX_ROLL_TYPE);
        }

        // Update subgroups
        member_witerator slot = _getMemberWSlot(guid);
        if (slot != m_memberSlots.end())
        {
            SubGroupCounterDecrease(slot->group);
            m_memberSlots.erase(slot);
        }

        // Pick new leader if necessary
        if (m_leaderGuid == guid)
        {
            for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
            {
                if (ObjectAccessor::FindPlayer(itr->guid))
                {
                    ChangeLeader(itr->guid);
                    break;
                }
            }
        }

        SendUpdate();

        if (isLFGGroup() && GetMembersCount() == 1)
        {
            Player* leader = ObjectAccessor::FindPlayer(GetLeaderGUID());
            uint32 mapId = sLFGMgr->GetDungeonMapId(GetGUID());
            if (!mapId || !leader || (leader->isAlive() && leader->GetMapId() != mapId))
            {
                Disband();
                return false;
            }
        }

        if (m_memberMgr.getSize() < ((isLFGGroup() || isBGGroup()) ? 1u : 2u))
            Disband();

        return true;
    }
    // If group size before player removal <= 2 then disband it
    else
    {
        Disband();
        return false;
    }
}

void Group::ChangeLeader(ObjectGuid const& guid, int8 partyIndex /*= 0*/)
{
    member_witerator slot = _getMemberWSlot(guid);

    if (slot == m_memberSlots.end())
        return;

    Player* player = ObjectAccessor::FindPlayer(slot->guid);

    // Don't allow switching leader to offline players
    if (!player)
        return;

    sScriptMgr->OnGroupChangeLeader(this, m_leaderGuid, guid);

    if (!isBGGroup() && !isBFGroup())
    {
        // Remove the groups permanent instance bindings
        for (uint8 i = 0; i < MAX_BOUND; ++i)
        {
            for (BoundInstancesMap::iterator itr = m_boundInstances[i].begin(); itr != m_boundInstances[i].end();)
            {
                if (itr->second.perm)
                {
                    itr->second.save->RemoveGroup(this);
                    m_boundInstances[i].erase(itr++);
                }
                else
                    ++itr;
            }
        }

        // Same in the database

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_INSTANCE_PERM_BINDING);

        stmt->setUInt32(0, m_dbStoreId);
        stmt->setUInt64(1, player->GetGUIDLow());

        CharacterDatabase.Execute(stmt);

        // Copy the permanent binds from the new leader to the group
        if (!isLFGGroup())
            Player::ConvertInstancesToGroup(player, this, true);

        // Update the group leader
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_LEADER);

        stmt->setUInt64(0, player->GetGUIDLow());
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    m_leaderGuid = player->GetGUID();
    m_leaderName = player->GetName();
    ToggleGroupMemberFlag(slot, MEMBER_FLAG_ASSISTANT, false);

    WorldPackets::Party::GroupNewLeader groupNewLeader;
    groupNewLeader.Name = slot->name;
    groupNewLeader.PartyIndex = partyIndex;
    BroadcastPacket(groupNewLeader.Write(), true);
}

void Group::Disband(bool hideDestroy /* = false */)
{
    sScriptMgr->OnGroupDisband(this);

    Player* player;
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        player = ObjectAccessor::FindPlayer(citr->guid);
        if (!player)
            continue;

        //we cannot call _removeMember because it would invalidate member iterator
        //if we are removing player from battleground raid
        if (isBGGroup() || isBFGroup())
            player->RemoveFromBattlegroundOrBattlefieldRaid();
        else
        {
            //we can remove player who is in battleground from his original group
            if (player->GetOriginalGroup() == this)
                player->SetOriginalGroup(NULL);
            else
                player->SetGroup(NULL);
        }

        // quest related GO state dependent from raid membership
        if (isRaidGroup())
            player->UpdateForQuestWorldObjects();

        if (!player->GetSession())
            continue;

        WorldPacket data;
        if (!hideDestroy)
        {
            data.Initialize(SMSG_GROUP_DESTROYED, 0);
            player->GetSession()->SendPacket(&data);
        }

        //we already removed player from group and in player->GetGroup() is his original group, send update
        if (Group* group = player->GetGroup())
            group->SendUpdate();
        else
            player->GetSession()->SendPacket(WorldPackets::Party::PartyUpdate().Write());

        if (!isLFGGroup())
            _homebindIfInstance(player);
    }
    RollId.clear();
    m_memberSlots.clear();

    RemoveAllInvites();

    if (!isBGGroup() && !isBFGroup())
    {
        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP);
        stmt->setUInt32(0, m_dbStoreId);
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_MEMBER_ALL);
        stmt->setUInt32(0, m_dbStoreId);
        trans->Append(stmt);

        CharacterDatabase.CommitTransaction(trans);

        ResetInstances(INSTANCE_RESET_GROUP_DISBAND, false, false, NULL);
        ResetInstances(INSTANCE_RESET_GROUP_DISBAND, true, false, NULL);
        ResetInstances(INSTANCE_RESET_GROUP_DISBAND, true, true, NULL);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_LFG_DATA);
        stmt->setUInt32(0, m_dbStoreId);
        CharacterDatabase.Execute(stmt);

        sGroupMgr->FreeGroupDbStoreId(this);
    }

    sGroupMgr->RemoveGroup(this);
    delete this;
}

/*********************************************************/
/***                   LOOT SYSTEM                     ***/
/*********************************************************/

void Group::SendLootStartRoll(uint32 mapID, Roll const& roll)
{
    WorldPackets::Loot::StartLootRoll lootRoll;
    lootRoll.LootObj = roll.lootedGUID;
    lootRoll.MapID = mapID;
    lootRoll.LootItems.Type = LOOT_SLOT_TYPE_ITEM;
    lootRoll.LootItems.UIType = LOOT_ITEM_UI_MASTER; // or LOOT_ITEM_UI_ROLL. same from SMSG_LOOT_RESPONSE but no metter that correct
    lootRoll.LootItems.CanTradeToTapList = false;
    lootRoll.LootItems.Quantity = roll.itemCount;
    lootRoll.LootItems.LootItemType = 0; // always 0... wtf?
    lootRoll.LootItems.LootListID = roll.aoeSlot;
    //lootRoll.LootItems.Loot.Initialize(&roll);
    lootRoll.LootItems.Loot.ItemID = roll.item.ItemID;  
    lootRoll.LootItems.Loot.RandomPropertiesSeed = roll.item.RandomPropertiesSeed;
    lootRoll.LootItems.Loot.RandomPropertiesID = roll.item.RandomPropertiesID;
    lootRoll.RollTime = NORMAL_ROLL_TIMER;
    lootRoll.ValidRolls = roll.totalPlayersRolling;
    lootRoll.Method = roll.rollVoteMask;

    WorldPacket const* p = lootRoll.Write();
    for (Roll::PlayerVote::const_iterator itr = roll.playerVote.begin(); itr != roll.playerVote.end(); ++itr)
    {
        Player* player = ObjectAccessor::FindPlayer(itr->first);
        if (!player || !player->GetSession())
            continue;

        if (itr->second == NOT_EMITED_YET)
            player->GetSession()->SendPacket(p);
    }
}

void Group::SendLootStartRollToPlayer(uint32 mapID, Player* player, bool canNeed, Roll const& roll)
{
    if (!player || !player->GetSession())
        return;

    WorldPackets::Loot::StartLootRoll lootRoll;
    lootRoll.LootObj = roll.lootedGUID;
    lootRoll.MapID = mapID;
    lootRoll.LootItems.Type = LOOT_SLOT_TYPE_ITEM;  // not sure
    lootRoll.LootItems.UIType = 1;
    lootRoll.LootItems.CanTradeToTapList = true; // not sure
    lootRoll.LootItems.Quantity = roll.itemCount;
    lootRoll.LootItems.LootItemType = 0; // always 0... wtf?
    lootRoll.LootItems.LootListID = roll.aoeSlot; // not sure... get 1-2-3-4-5 values at sniffs
    lootRoll.LootItems.Loot.ItemID = roll.item.ItemID;
    lootRoll.LootItems.Loot.RandomPropertiesSeed = roll.item.RandomPropertiesSeed;
    lootRoll.LootItems.Loot.RandomPropertiesID = roll.item.RandomPropertiesID;
    lootRoll.RollTime = NORMAL_ROLL_TIMER;
    lootRoll.ValidRolls = roll.TotalEmited();
    lootRoll.Method = roll.rollVoteMask; // not sure
    player->GetSession()->SendPacket(lootRoll.Write());
}

void Group::SendLootRoll(ObjectGuid targetGuid, uint8 rollNumber, uint8 rollType, Roll const& roll)
{
    WorldPackets::Loot::LootRollResponse response;
    response.LootItems.Type = LOOT_SLOT_TYPE_ITEM;
    response.LootItems.UIType = 2;
    response.LootItems.CanTradeToTapList = false;
    response.LootItems.Quantity = roll.itemCount;
    response.LootItems.LootItemType = 0; // always 0... wtf?
    response.LootItems.LootListID = roll.aoeSlot;
    // roll.TotalEmited()
    response.LootItems.Loot.ItemID = roll.item.ItemID;
    response.LootItems.Loot.RandomPropertiesSeed = roll.item.RandomPropertiesSeed;
    response.LootItems.Loot.RandomPropertiesID = roll.item.RandomPropertiesID;
    response.LootObj = roll.lootedGUID;
    response.Player = targetGuid;
    response.Roll = rollNumber;
    response.RollType = rollType;
    response.Autopassed = false;

    WorldPacket const* pdata = response.Write();
    for (Roll::PlayerVote::const_iterator itr = roll.playerVote.begin(); itr != roll.playerVote.end(); ++itr)
    {
        Player* p = ObjectAccessor::FindPlayer(itr->first);
        if (!p || !p->GetSession())
            continue;

        if (itr->second != NOT_VALID)
            p->GetSession()->SendPacket(pdata);
    }
}

void Group::SendLootRollWon(ObjectGuid targetGuid, uint8 rollNumber, uint8 rollType, Roll const& roll)
{
    WorldPackets::Loot::LootRollWon won;
    won.LootObj = roll.lootedGUID;
    won.Player = targetGuid;
    won.Roll = rollNumber;
    won.RollType = rollType;
    won.LootItems.Type = LOOT_SLOT_TYPE_ITEM;
    won.LootItems.UIType = 1;
    won.LootItems.CanTradeToTapList = false;
    won.LootItems.Quantity = roll.itemCount;
    won.LootItems.LootItemType = 0; // always 0... wtf?
    won.LootItems.LootListID = roll.aoeSlot;
    //won.LootItems.Loot.Initialize(&roll);
    won.LootItems.Loot.ItemID = roll.item.ItemID;  
    won.LootItems.Loot.RandomPropertiesSeed = roll.item.RandomPropertiesSeed;
    won.LootItems.Loot.RandomPropertiesID = roll.item.RandomPropertiesID;

    // roll.TotalEmited()

    WorldPackets::Loot::LootRollsComplete rollsComplete;
    rollsComplete.LootListID = roll.itemSlot;
    rollsComplete.LootObj = roll.lootedGUID;

    WorldPacket const* pwon = won.Write();
    WorldPacket const* prollsComplete = rollsComplete.Write();

    for (Roll::PlayerVote::const_iterator itr = roll.playerVote.begin(); itr != roll.playerVote.end(); ++itr)
    {
        Player* p = ObjectAccessor::FindPlayer(itr->first);
        if (!p || !p->GetSession())
            continue;

        if (itr->second != NOT_VALID)
        {
            p->GetSession()->SendPacket(pwon);
            p->GetSession()->SendPacket(prollsComplete);
        }
    }
}

void Group::SendLootAllPassed(Roll const& roll)
{
    WorldPackets::Loot::LootAllPassed  passed;
    passed.LootObj = roll.lootedGUID;
    passed.LootItems.Type = LOOT_SLOT_TYPE_ITEM;
    passed.LootItems.UIType = 1;
    passed.LootItems.CanTradeToTapList = false;
    passed.LootItems.Quantity = roll.itemCount;
    passed.LootItems.LootItemType = 0; // always 0... wtf?
    passed.LootItems.LootListID = roll.aoeSlot;
    //passed.LootItems.Loot.Initialize(&roll);
    passed.LootItems.Loot.ItemID = roll.item.ItemID;  
    passed.LootItems.Loot.RandomPropertiesSeed = roll.item.RandomPropertiesSeed;
    passed.LootItems.Loot.RandomPropertiesID = roll.item.RandomPropertiesID;
    // roll.TotalEmited() ???

    WorldPacket const* ppassed = passed.Write();

    for (Roll::PlayerVote::const_iterator itr = roll.playerVote.begin(); itr != roll.playerVote.end(); ++itr)
    {
        Player* player = ObjectAccessor::FindPlayer(itr->first);
        if (!player || !player->GetSession())
            continue;

        if (itr->second != NOT_VALID)
            player->GetSession()->SendPacket(ppassed);
    }
}

void Group::SendLooter(Creature* creature, Player* groupLooter)
{
    ASSERT(creature);

    WorldPackets::Loot::LootList lootList;
    lootList.Owner = creature->GetGUID();
    lootList.Master = groupLooter ? groupLooter->GetGUID() : ObjectGuid::Empty;
    //lootList.RoundRobinWinner;
    BroadcastPacket(lootList.Write(), false);
}

void Group::GroupLoot(Loot* loot, WorldObject* pLootedObject)
{
    std::vector<LootItem>::iterator i;
    ItemTemplate const* item;
    uint8 itemSlot = 0;

    for (i = loot->items.begin(); i != loot->items.end(); ++i, ++itemSlot)
    {
        if (i->freeforall || i->currency)
            continue;

        item = sObjectMgr->GetItemTemplate(i->item.ItemID);
        if (!item)
        {
            //sLog->outDebug(LOG_FILTER_GENERAL, "Group::GroupLoot: missing item prototype for item with id: %d", i->itemid);
            continue;
        }

        //roll for over-threshold item if it's one-player loot
        if (item->Quality >= uint32(m_lootThreshold))
        {
            ObjectGuid newitemGUID = ObjectGuid::Create<HighGuid::Item>(sObjectMgr->GetGenerator<HighGuid::Item>()->Generate());
            Roll* r = new Roll(newitemGUID, *i);
            r->lootedGUID = loot->GetGUID();

            //a vector is filled with only near party members
            for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* member = itr->getSource();
                if (!member || !member->GetSession())
                    continue;
                if (i->AllowedForPlayer(member))
                {
                    if (member->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                    {
                        r->totalPlayersRolling++;

                        if (member->GetPassOnGroupLoot())
                        {
                            r->playerVote[member->GetGUID()] = PASS;
                            r->totalPass++;
                            // can't broadcast the pass now. need to wait until all rolling players are known.
                        }
                        else
                            r->playerVote[member->GetGUID()] = NOT_EMITED_YET;
                    }
                }
            }

            if (r->totalPlayersRolling > 0)
            {
                r->setLoot(loot);
                r->itemSlot = itemSlot;
                r->aoeSlot = ++m_aoe_slots;
                if (item->DisenchantID && m_maxEnchantingLevel >= item->RequiredDisenchantSkill)
                    r->rollVoteMask |= ROLL_FLAG_TYPE_DISENCHANT;

                loot->items[itemSlot].is_blocked = true;

                // If there is any "auto pass", broadcast the pass now.
                if (r->totalPass)
                {
                    for (Roll::PlayerVote::const_iterator itr=r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
                    {
                        Player* p = ObjectAccessor::FindPlayer(itr->first);
                        if (!p || !p->GetSession())
                            continue;

                        if (itr->second == PASS)
                            SendLootRoll(p->GetGUID(), 128, ROLL_PASS, *r);
                    }
                }

                SendLootStartRoll(pLootedObject->GetMapId(), *r);

                RollId.push_back(r);

                if (Creature* creature = pLootedObject->ToCreature())
                {
                    creature->m_groupLootTimer = NORMAL_ROLL_TIMER;
                    creature->lootingGroupLowGUID = GetGUID();
                }
                else if (GameObject* go = pLootedObject->ToGameObject())
                {
                    go->m_groupLootTimer = NORMAL_ROLL_TIMER;
                    go->lootingGroupLowGUID = GetGUID();
                }
            }
            else
                delete r;
        }
        else
            i->is_underthreshold = true;
    }

    for (i = loot->quest_items.begin(); i != loot->quest_items.end(); ++i, ++itemSlot)
    {
        if (!i->follow_loot_rules)
            continue;

        item = sObjectMgr->GetItemTemplate(i->item.ItemID);
        if (!item)
            continue;

        ObjectGuid newitemGUID = ObjectGuid::Create<HighGuid::Item>(sObjectMgr->GetGenerator<HighGuid::Item>()->Generate());
        Roll* r = new Roll(newitemGUID, *i);
        r->lootedGUID = loot->GetGUID();

        //a vector is filled with only near party members
        for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* member = itr->getSource();
            if (!member || !member->GetSession())
                continue;

            if (i->AllowedForPlayer(member))
            {
                if (member->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                {
                    r->totalPlayersRolling++;
                    r->playerVote[member->GetGUID()] = NOT_EMITED_YET;
                }
            }
        }

        if (r->totalPlayersRolling > 0)
        {
            r->setLoot(loot);
            r->itemSlot = itemSlot;
            r->aoeSlot = ++m_aoe_slots;

            loot->quest_items[itemSlot - loot->items.size()].is_blocked = true;

            SendLootStartRoll(pLootedObject->GetMapId(), *r);

            RollId.push_back(r);

            if (Creature* creature = pLootedObject->ToCreature())
            {
                creature->m_groupLootTimer = NORMAL_ROLL_TIMER;
                creature->lootingGroupLowGUID = GetGUID();
            }
            else if (GameObject* go = pLootedObject->ToGameObject())
            {
                go->m_groupLootTimer = NORMAL_ROLL_TIMER;
                go->lootingGroupLowGUID = GetGUID();
            }
        }
        else
            delete r;
    }
}

void Group::NeedBeforeGreed(Loot* loot, WorldObject* lootedObject)
{
    ItemTemplate const* item;
    uint8 itemSlot = 0;
    for (std::vector<LootItem>::iterator i = loot->items.begin(); i != loot->items.end(); ++i, ++itemSlot)
    {
        if (i->freeforall || i->currency)
            continue;

        item = sObjectMgr->GetItemTemplate(i->item.ItemID);

        //roll for over-threshold item if it's one-player loot
        if (item->Quality >= uint32(m_lootThreshold))
        {
            ObjectGuid newitemGUID = ObjectGuid::Create<HighGuid::Item>(sObjectMgr->GetGenerator<HighGuid::Item>()->Generate());
            Roll* r = new Roll(newitemGUID, *i);
            r->lootedGUID = loot->GetGUID();

            for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* playerToRoll = itr->getSource();
                if (!playerToRoll || !playerToRoll->GetSession())
                    continue;

                bool allowedForPlayer = i->AllowedForPlayer(playerToRoll);
                if (allowedForPlayer && playerToRoll->IsWithinDistInMap(lootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                {
                    r->totalPlayersRolling++;
                    if (playerToRoll->GetPassOnGroupLoot())
                    {
                        r->playerVote[playerToRoll->GetGUID()] = PASS;
                        r->totalPass++;
                        // can't broadcast the pass now. need to wait until all rolling players are known.
                    }
                    else
                        r->playerVote[playerToRoll->GetGUID()] = NOT_EMITED_YET;
                }
            }

            if (r->totalPlayersRolling > 0)
            {
                r->setLoot(loot);
                r->itemSlot = itemSlot;
                r->aoeSlot = ++m_aoe_slots;

                if (item->DisenchantID && m_maxEnchantingLevel >= item->RequiredDisenchantSkill)
                    r->rollVoteMask |= ROLL_FLAG_TYPE_DISENCHANT;

                if (item->Flags2 & ITEM_FLAGS_EXTRA_NEED_ROLL_DISABLED)
                    r->rollVoteMask &= ~ROLL_FLAG_TYPE_NEED;

                loot->items[itemSlot].is_blocked = true;

                //Broadcast Pass and Send Rollstart
                for (Roll::PlayerVote::const_iterator itr = r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
                {
                    Player* p = ObjectAccessor::FindPlayer(itr->first);
                    if (!p || !p->GetSession())
                        continue;

                    if (itr->second == PASS)
                        SendLootRoll(p->GetGUID(), 128, ROLL_PASS, *r);
                    else
                        SendLootStartRollToPlayer(lootedObject->GetMapId(), p, p->CanRollForItemInLFG(item, lootedObject) == EQUIP_ERR_OK, *r);
                }

                RollId.push_back(r);

                if (Creature* creature = lootedObject->ToCreature())
                {
                    creature->m_groupLootTimer = NORMAL_ROLL_TIMER;
                    creature->lootingGroupLowGUID = GetGUID();
                }
                else if (GameObject* go = lootedObject->ToGameObject())
                {
                    go->m_groupLootTimer = NORMAL_ROLL_TIMER;
                    go->lootingGroupLowGUID = GetGUID();
                }
            }
            else
                delete r;
        }
        else
            i->is_underthreshold = true;
    }

    for (std::vector<LootItem>::iterator i = loot->quest_items.begin(); i != loot->quest_items.end(); ++i, ++itemSlot)
    {
        if (!i->follow_loot_rules)
            continue;

        item = sObjectMgr->GetItemTemplate(i->item.ItemID);
        ObjectGuid newitemGUID = ObjectGuid::Create<HighGuid::Item>(sObjectMgr->GetGenerator<HighGuid::Item>()->Generate());
        Roll* r = new Roll(newitemGUID, *i);
        r->lootedGUID = loot->GetGUID();

        for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* playerToRoll = itr->getSource();
            if (!playerToRoll || !playerToRoll->GetSession())
                continue;

            bool allowedForPlayer = i->AllowedForPlayer(playerToRoll);
            if (allowedForPlayer && playerToRoll->IsWithinDistInMap(lootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
            {
                r->totalPlayersRolling++;
                r->playerVote[playerToRoll->GetGUID()] = NOT_EMITED_YET;
            }
        }

        if (r->totalPlayersRolling > 0)
        {
            r->setLoot(loot);
            r->itemSlot = itemSlot;
            r->aoeSlot = ++m_aoe_slots;

            loot->quest_items[itemSlot - loot->items.size()].is_blocked = true;

            //Broadcast Pass and Send Rollstart
            for (Roll::PlayerVote::const_iterator itr = r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
            {
                Player* p = ObjectAccessor::FindPlayer(itr->first);
                if (!p || !p->GetSession())
                    continue;

                if (itr->second == PASS)
                    SendLootRoll(p->GetGUID(), 128, ROLL_PASS, *r);
                else
                    SendLootStartRollToPlayer(lootedObject->GetMapId(), p, p->CanRollForItemInLFG(item, lootedObject) == EQUIP_ERR_OK, *r);
            }

            RollId.push_back(r);

            if (Creature* creature = lootedObject->ToCreature())
            {
                creature->m_groupLootTimer = NORMAL_ROLL_TIMER;
                creature->lootingGroupLowGUID = GetGUID();
            }
            else if (GameObject* go = lootedObject->ToGameObject())
            {
                go->m_groupLootTimer = NORMAL_ROLL_TIMER;
                go->lootingGroupLowGUID = GetGUID();
            }
        }
        else
            delete r;
    }
}

void Group::MasterLoot(Loot* loot, WorldObject* lootObj)
{
    WorldPackets::Loot::MasterLootCandidateList list;
    list.LootObj = loot->GetGUID();
    for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        Player* looter = itr->getSource();
        if (!looter->IsInWorld())
            continue;

        if (looter->IsWithinDistInMap(lootObj, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
            list.Players.push_back(ObjectGuid::Create<HighGuid::Player>(looter->GetGUIDLow())); //HardHack! Plr should have off-like hiGuid
    }

    if (Player* player = ObjectAccessor::FindPlayer(GetLooterGuid()))
        player->GetSession()->SendPacket(list.Write());
}

void Group::DoRollForAllMembers(ObjectGuid guid, uint8 slot, uint32 mapid, Loot* loot, LootItem& item, Player* player)
{
    // Already rolled?
    for (Rolls::iterator iter=RollId.begin(); iter != RollId.end(); ++iter)
        if ((*iter)->itemSlot == slot && loot == (*iter)->getLoot() && (*iter)->isValid())
            return;

    ObjectGuid newitemGUID = ObjectGuid::Create<HighGuid::Item>(sObjectMgr->GetGenerator<HighGuid::Item>()->Generate());
    Roll* r = new Roll(newitemGUID, item);
    r->lootedGUID = loot->GetGUID();
    WorldObject* pLootedObject = NULL;

    if (guid.IsLoot())
        guid = loot->objGuid;

    if (guid.IsCreatureOrVehicle())
        pLootedObject = player->GetMap()->GetCreature(guid);
    else if (guid.IsGameObject())
        pLootedObject = player->GetMap()->GetGameObject(guid);

    if (!pLootedObject)
        return;

    //a vector is filled with only near party members
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->getSource();
        if (!member || !member->GetSession())
            continue;

        if (item.AllowedForPlayer(member))
        {
            if (member->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
            {
                r->totalPlayersRolling++;
                r->playerVote[member->GetGUID()] = NOT_EMITED_YET;
            }
        }
    }

    if (r->totalPlayersRolling > 0)
    {
        r->setLoot(loot);
        r->itemSlot = slot;
        r->aoeSlot = ++m_aoe_slots;     //restart at next loot. it's normall

        RollId.push_back(r);
    }

    SendLootStartRoll(mapid, *r);
}

void Group::CountRollVote(ObjectGuid playerGUID, uint8 AoeSlot, uint8 Choice)
{
    Rolls::iterator rollI = GetRoll(AoeSlot);
    if (rollI == RollId.end())
        return;
    Roll* roll = *rollI;

    Roll::PlayerVote::iterator itr = roll->playerVote.find(playerGUID);
    // this condition means that player joins to the party after roll begins
    if (itr == roll->playerVote.end())
        return;

    if (roll->getLoot())
        if (roll->getLoot()->items.empty())
            return;

    switch (Choice)
    {
    case ROLL_PASS:                                     // Player choose pass
        ++roll->totalPass;
        SendLootRoll(playerGUID, 0, ROLL_PASS, *roll);
        itr->second = PASS;
        break;
    case ROLL_NEED:                                     // player choose Need
        ++roll->totalNeed;
        SendLootRoll(playerGUID, 0, ROLL_NEED, *roll);
        itr->second = NEED;
        break;
    case ROLL_GREED:                                    // player choose Greed
        ++roll->totalGreed;
        SendLootRoll(playerGUID, 0, ROLL_GREED, *roll);
        itr->second = GREED;
        break;
    case ROLL_DISENCHANT:                               // player choose Disenchant
        ++roll->totalGreed;
        SendLootRoll(playerGUID, 0, ROLL_DISENCHANT, *roll);
        itr->second = DISENCHANT;
        break;
    }

    if (roll->TotalEmited() >= roll->totalPlayersRolling)
        CountTheRoll(rollI);
}

//called when roll timer expires
void Group::EndRoll(Loot* pLoot)
{
    for (Rolls::iterator itr = RollId.begin(); itr != RollId.end();)
    {
        if ((*itr)->getLoot() == pLoot) {
            CountTheRoll(itr);           //i don't have to edit player votes, who didn't vote ... he will pass
            itr = RollId.begin();
        }
        else
            ++itr;
    }
}

void Group::CountTheRoll(Rolls::iterator rollI)
{
    Roll* roll = *rollI;
    if (!roll->isValid())                                   // is loot already deleted ?
    {
        RollId.erase(rollI);
        delete roll;
        return;
    }

    //end of the roll
    if (roll->totalNeed > 0)
    {
        if (!roll->playerVote.empty())
        {
            uint8 maxresul = 0;
            ObjectGuid maxguid  = (*roll->playerVote.begin()).first;
            Player* player;

            for (Roll::PlayerVote::const_iterator itr=roll->playerVote.begin(); itr != roll->playerVote.end(); ++itr)
            {
                if (itr->second != NEED)
                    continue;

                uint8 randomN = urand(1, 100);
                SendLootRoll(itr->first, randomN, ROLL_NEED, *roll);
                if (maxresul < randomN)
                {
                    maxguid  = itr->first;
                    maxresul = randomN;
                }
            }
            SendLootRollWon(maxguid, maxresul, ROLL_NEED, *roll);
            player = ObjectAccessor::FindPlayer(maxguid);

            if (player && player->GetSession())
            {
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT, roll->item.ItemID, maxresul);

                ItemPosCountVec dest;
                LootItem* item = &(roll->itemSlot >= roll->getLoot()->items.size() ? roll->getLoot()->quest_items[roll->itemSlot - roll->getLoot()->items.size()] : roll->getLoot()->items[roll->itemSlot]);
                InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, roll->item.ItemID, item->count);
                if (msg == EQUIP_ERR_OK)
                {
                    item->is_looted = true;
                    roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                    roll->getLoot()->unlootedCount--;
                    GuidSet looters = item->GetAllowedLooters();
                    player->StoreNewItem(dest, roll->item.ItemID, true, item->item.RandomPropertiesID, looters, item->item.ItemBonus.BonusListIDs);
                }
                else
                {
                    item->is_blocked = false;
                    player->SendEquipError(msg, NULL, NULL, roll->item.ItemID);
                }
            }
        }
    }
    else if (roll->totalGreed > 0)
    {
        if (!roll->playerVote.empty())
        {
            uint8 maxresul = 0;
            ObjectGuid maxguid = (*roll->playerVote.begin()).first;
            Player* player;
            RollVote rollvote = NOT_VALID;

            Roll::PlayerVote::iterator itr;
            for (itr = roll->playerVote.begin(); itr != roll->playerVote.end(); ++itr)
            {
                if (itr->second != GREED && itr->second != DISENCHANT)
                    continue;

                uint8 randomN = urand(1, 100);
                SendLootRoll(itr->first, randomN, itr->second, *roll);
                if (maxresul < randomN)
                {
                    maxguid  = itr->first;
                    maxresul = randomN;
                    rollvote = itr->second;
                }
            }
            SendLootRollWon(maxguid, maxresul, rollvote, *roll);
            player = ObjectAccessor::FindPlayer(maxguid);

            if (player && player->GetSession())
            {
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT, roll->item.ItemID, maxresul);

                LootItem* item = &(roll->itemSlot >= roll->getLoot()->items.size() ? roll->getLoot()->quest_items[roll->itemSlot - roll->getLoot()->items.size()] : roll->getLoot()->items[roll->itemSlot]);

                if (rollvote == GREED)
                {
                    ItemPosCountVec dest;
                    InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, roll->item.ItemID, item->count);
                    if (msg == EQUIP_ERR_OK)
                    {
                        item->is_looted = true;
                        roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                        roll->getLoot()->unlootedCount--;
                        GuidSet looters = item->GetAllowedLooters();
                        player->StoreNewItem(dest, roll->item.ItemID, true, item->item.RandomPropertiesID, looters, item->item.ItemBonus.BonusListIDs);
                    }
                    else
                    {
                        item->is_blocked = false;
                        player->SendEquipError(msg, NULL, NULL, roll->item.ItemID);
                    }
                }
                else if (rollvote == DISENCHANT)
                {
                    item->is_looted = true;
                    roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                    roll->getLoot()->unlootedCount--;
                    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(roll->item.ItemID);
                    player->AutoStoreLoot(pProto->ItemId, LootTemplates_Disenchant, 0, true);
                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL, 13262); // Disenchant
                }
            }
        }
    }
    else
    {
        SendLootAllPassed(*roll);

        // remove is_blocked so that the item is lootable by all players
        LootItem* item = &(roll->itemSlot >= roll->getLoot()->items.size() ? roll->getLoot()->quest_items[roll->itemSlot - roll->getLoot()->items.size()] : roll->getLoot()->items[roll->itemSlot]);
        if (item)
            item->is_blocked = false;
    }

    RollId.erase(rollI);
    delete roll;
}

void Group::SetTargetIcon(uint8 symbol, ObjectGuid target, ObjectGuid changedBy, uint8 partyIndex)
{
    if (symbol >= TARGET_ICONS_COUNT)
        return;

    if (!target.IsEmpty())
        for (int8 i = 0; i < TARGET_ICONS_COUNT; ++i)
            if (m_targetIcons[i] == target)
                SetTargetIcon(i, ObjectGuid::Empty, changedBy, partyIndex);

    m_targetIcons[symbol] = target;

    WorldPackets::Party::SendRaidTargetUpdateSingle updateSingle;
    updateSingle.PartyIndex = partyIndex;
    updateSingle.Target = target;
    updateSingle.ChangedBy = changedBy;
    updateSingle.Symbol = symbol;
    BroadcastPacket(updateSingle.Write(), true);
}

void Group::SendTargetIconList(WorldSession* session, int8 partyIndex)
{
    if (!session)
        return;

    WorldPackets::Party::SendRaidTargetUpdateAll updateAll;
    updateAll.PartyIndex = partyIndex;
    for (uint8 i = 0; i < TARGET_ICONS_COUNT; i++)
        updateAll.TargetIcons.insert(std::pair<uint8, ObjectGuid>(i, m_targetIcons[i]));

    session->SendPacket(updateAll.Write());
}

void Group::SendUpdate()
{
    for (member_witerator witr = m_memberSlots.begin(); witr != m_memberSlots.end(); ++witr)
        SendUpdateToPlayer(witr->guid, &(*witr));
}

void Group::SendUpdateToPlayer(ObjectGuid playerGUID, MemberSlot* slot)
{
    Player* player = ObjectAccessor::FindPlayer(playerGUID);

    if (!player || !player->GetSession() || player->GetGroup() != this)
        return;

    // if MemberSlot wasn't provided
    if (!slot)
    {
        member_witerator witr = _getMemberWSlot(playerGUID);

        if (witr == m_memberSlots.end()) // if there is no MemberSlot for such a player
            return;

        slot = &(*witr);
    }

    Player* member = NULL;

    uint8 const gStatusOfline = (isBGGroup() || isBFGroup()) ? MEMBER_STATUS_PVP : MEMBER_STATUS_OFFLINE;
    uint8 const gStatusOnline = gStatusOfline | MEMBER_STATUS_ONLINE;

    WorldPackets::Party::PartyUpdate partyUpdate;

    partyUpdate.PartyType = m_groupType;
    partyUpdate.PartyIndex = 0;
    partyUpdate.PartyFlags = IsHomeGroup() ? 0 : 1;

    partyUpdate.PartyGUID = m_guid;
    partyUpdate.LeaderGUID = m_leaderGuid;

    partyUpdate.SequenceNum = ++m_counter;

    partyUpdate.MyIndex = -1;
    uint8 index = 0;
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr, ++index)
    {
        if (slot->guid == citr->guid)
            partyUpdate.MyIndex = index;

        Player* member = ObjectAccessor::FindPlayer(citr->guid);

        WorldPackets::Party::GroupPlayerInfos playerInfos;

        playerInfos.GUID = citr->guid;
        playerInfos.Name = citr->name;
        //playerInfos.Class = citr->_class;

        playerInfos.Status = MEMBER_STATUS_OFFLINE;
        if (member && member->GetSession() && !member->GetSession()->PlayerLogout())
            playerInfos.Status = MEMBER_STATUS_ONLINE | (isBGGroup() || isBFGroup() ? MEMBER_STATUS_PVP : 0);

        playerInfos.Subgroup = citr->group;
        playerInfos.Flags = citr->flags;
        playerInfos.RolesAssigned = citr->roles;

        partyUpdate.PlayerList.push_back(playerInfos);
    }

    if (GetMembersCount() > 1)
    {
        // LootSettings
        partyUpdate.LootSettings = boost::in_place();
        partyUpdate.LootSettings->Method = m_lootMethod;
        partyUpdate.LootSettings->Threshold = m_lootThreshold;
        partyUpdate.LootSettings->LootMaster = m_lootMethod == MASTER_LOOT ? m_looterGuid : ObjectGuid::Empty;

        // Difficulty Settings
        partyUpdate.DifficultySettings = boost::in_place();
        partyUpdate.DifficultySettings->DungeonDifficultyID = m_dungeonDifficulty;
        partyUpdate.DifficultySettings->RaidDifficultyID = m_raidDifficulty;
        partyUpdate.DifficultySettings->LegacyRaidDifficultyID = m_legacyRaidDifficulty;
    }

    // LfgInfos
    if (isLFGGroup())
    {
        lfg::LFGDungeonData const* dungeon = sLFGMgr->GetLFGDungeon(sLFGMgr->GetDungeon(m_guid, true), player->GetTeam());
        lfg::LFGDungeonData const* rDungeon = NULL;
        if (dungeon)
        {
            lfg::LfgDungeonSet const& dungeons = sLFGMgr->GetSelectedDungeons(m_guid);
            if (!dungeons.empty())
            {
                 rDungeon = sLFGMgr->GetLFGDungeon(*dungeons.begin(), player->GetTeam());
                 if (rDungeon && rDungeon->type != LFG_TYPE_RANDOM)
                     rDungeon = NULL;
            }
        }

        lfg::LfgState lfgState = sLFGMgr->GetState(m_guid);
        uint8 flags = 0;
        if (lfgState == lfg::LFG_STATE_FINISHED_DUNGEON || dungeon && dungeon->dbc->flags & LFG_FLAG_NON_BACKFILLABLE)
            flags |= 2;

        partyUpdate.LfgInfos = boost::in_place();

        partyUpdate.LfgInfos->Slot = sLFGMgr->GetDungeon(m_guid);
        partyUpdate.LfgInfos->BootCount = 0;
        partyUpdate.LfgInfos->Aborted = false;

        partyUpdate.LfgInfos->MyFlags = flags;
        partyUpdate.LfgInfos->MyRandomSlot = 0;

        partyUpdate.LfgInfos->MyPartialClear = sLFGMgr->GetState(m_guid) == lfg::LFG_STATE_FINISHED_DUNGEON ? 2 : 0;
        partyUpdate.LfgInfos->MyGearDiff = dungeon ? 1.0f : 0.0f;
        partyUpdate.LfgInfos->MyFirstReward = lfgState != lfg::LFG_STATE_FINISHED_DUNGEON;

        partyUpdate.LfgInfos->MyStrangerCount = 0;
        partyUpdate.LfgInfos->MyKickVoteCount = 0;
    }

    player->GetSession()->SendPacket(partyUpdate.Write());
}

void Group::UpdatePlayerOutOfRange(Player* player)
{
    if (!player || !player->IsInWorld())
        return;

    WorldPackets::Party::PartyMemberStats packet;
    packet.Initialize(player);

    Player* member;
    WorldPacket const* p = packet.Write();
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        member = itr->getSource();
        if (member && member != player && (!member->IsInMap(player) || !member->IsWithinDist(player, member->GetSightRange(), false)))
            member->GetSession()->SendPacket(p);
    }
}

void Group::BroadcastAddonMessagePacket(WorldPacket* packet, std::string const& prefix, bool ignorePlayersInBGRaid, int group, ObjectGuid ignore)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (!player || (ignore && player->GetGUID() == ignore) || (ignorePlayersInBGRaid && player->GetGroup() != this))
            continue;

        if (WorldSession* session = player->GetSession())
            if (session && (group == -1 || itr->getSubGroup() == group))
                if (session->IsAddonRegistered(prefix))
                    session->SendPacket(packet);
    }
}

void Group::BroadcastPacket(const WorldPacket* packet, bool ignorePlayersInBGRaid, int group, ObjectGuid ignore)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (!player || (ignore && player->GetGUID() == ignore) || (ignorePlayersInBGRaid && player->GetGroup() != this))
            continue;

        if (player->GetSession() && (group == -1 || itr->getSubGroup() == group))
            player->GetSession()->SendPacket(packet);
    }
}

void Group::BroadcastReadyCheck(WorldPacket const* packet)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (player && player->GetSession())
            if (IsLeader(player->GetGUID()) || IsAssistant(player->GetGUID()) || m_groupType & GROUPTYPE_EVERYONE_IS_ASSISTANT)
                player->GetSession()->SendPacket(packet);
    }
}

//! 6.0.3
void Group::OfflineReadyCheck()
{
    bool ready = false;
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        Player* player = ObjectAccessor::FindPlayer(citr->guid);
        if (!player || !player->GetSession())
        {
            WorldPackets::Party::ReadyCheckResponse response;
            response.PartyGUID = GetGUID();
            response.Player = citr->guid;
            response.IsReady = ready;
            BroadcastReadyCheck(response.Write());

            m_readyCheckCount++;
        }
    }
}

bool Group::_setMembersGroup(ObjectGuid guid, uint8 group)
{
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return false;

    slot->group = group;

    SubGroupCounterIncrease(group);

    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_SUBGROUP);

        stmt->setUInt8(0, group);
        stmt->setUInt64(1, guid.GetCounter());

        CharacterDatabase.Execute(stmt);
    }

    return true;
}

bool Group::SameSubGroup(Player const* member1, Player const* member2) const
{
    if (!member1 || !member2)
        return false;

    if (member1->GetGroup() != this || member2->GetGroup() != this)
        return false;
    else
        return member1->GetSubGroup() == member2->GetSubGroup();
}

// Allows setting sub groups both for online or offline members
void Group::ChangeMembersGroup(ObjectGuid guid, uint8 group)
{
    // Only raid groups have sub groups
    if (!isRaidGroup())
        return;

    // Check if player is really in the raid
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    // Abort if the player is already in the target sub group
    uint8 prevSubGroup = GetMemberGroup(guid);
    if (prevSubGroup == group)
        return;

    // Update the player slot with the new sub group setting
    slot->group = group;

    // Increase the counter of the new sub group..
    SubGroupCounterIncrease(group);

    // ..and decrease the counter of the previous one
    SubGroupCounterDecrease(prevSubGroup);

    // Preserve new sub group in database for non-raid groups
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_SUBGROUP);

        stmt->setUInt8(0, group);
        stmt->setUInt64(1, guid.GetCounter());

        CharacterDatabase.Execute(stmt);
    }

    // In case the moved player is online, update the player object with the new sub group references
    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        if (player->GetGroup() == this)
            player->GetGroupRef().setSubGroup(group);
        else
        {
            // If player is in BG raid, it is possible that he is also in normal raid - and that normal raid is stored in m_originalGroup reference
            prevSubGroup = player->GetOriginalSubGroup();
            player->GetOriginalGroupRef().setSubGroup(group);
        }
    }

    // Broadcast the changes to the group
    SendUpdate();
}

// Retrieve the next Round-Roubin player for the group
//
// No update done if loot method is Master or FFA.
//
// If the RR player is not yet set for the group, the first group member becomes the round-robin player.
// If the RR player is set, the next player in group becomes the round-robin player.
//
// If ifneed is true,
//      the current RR player is checked to be near the looted object.
//      if yes, no update done.
//      if not, he loses his turn.
void Group::UpdateLooterGuid(WorldObject* pLootedObject, bool ifneed)
{
    switch (GetLootMethod())
    {
    case MASTER_LOOT:
    case FREE_FOR_ALL:
        return;
    default:
        // round robin style looting applies for all low
        // quality items in each loot method except free for all and master loot
        break;
    }

    ObjectGuid oldLooterGUID = GetLooterGuid();
    member_citerator guid_itr = _getMemberCSlot(oldLooterGUID);
    if (guid_itr != m_memberSlots.end())
    {
        if (ifneed)
        {
            // not update if only update if need and ok
            Player* looter = ObjectAccessor::FindPlayer(guid_itr->guid);
            if (looter && looter->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                return;
        }
        ++guid_itr;
    }

    // search next after current
    Player* pNewLooter = NULL;
    for (member_citerator itr = guid_itr; itr != m_memberSlots.end(); ++itr)
    {
        if (Player* player = ObjectAccessor::FindPlayer(itr->guid))
            if (player->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
            {
                pNewLooter = player;
                break;
            }
    }

    if (!pNewLooter)
    {
        // search from start
        for (member_citerator itr = m_memberSlots.begin(); itr != guid_itr; ++itr)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->guid))
                if (player->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                {
                    pNewLooter = player;
                    break;
                }
        }
    }

    if (pNewLooter)
    {
        if (oldLooterGUID != pNewLooter->GetGUID())
        {
            SetLooterGuid(pNewLooter->GetGUID());
            SendUpdate();
        }
    }
    else
    {
        SetLooterGuid(ObjectGuid::Empty);
        SendUpdate();
    }
}

//! ToDo: Arena check battlegroup
GroupJoinBattlegroundResult Group::CanJoinBattlegroundQueue(Battleground const* bgOrTemplate, BattlegroundQueueTypeId bgQueueTypeId, uint32 MinPlayerCount, uint32 /*MaxPlayerCount*/, bool isRated, uint32 arenaSlot, ObjectGuid& errorGuid)
{
    // check if this group is LFG group
    if (isLFGGroup())
        return ERR_LFG_CANT_USE_BATTLEGROUND;

    BattlemasterListEntry const* bgEntry = sBattlemasterListStore.LookupEntry(bgOrTemplate->GetTypeID());
    if (!bgEntry)
        return ERR_BATTLEGROUND_JOIN_FAILED;            // shouldn't happen

    // check for min / max count
    uint32 memberscount = GetMembersCount();

    if (memberscount > bgEntry->MaxPlayers)                // no MinPlayerCount for battlegrounds
        return ERR_BATTLEGROUND_NONE;                        // ERR_GROUP_JOIN_BATTLEGROUND_TOO_MANY handled on client side

    // get a player as reference, to compare other players' stats to (arena team id, queue id based on level, etc.)
    Player* reference = GetFirstMember()->getSource();
    // no reference found, can't join this way
    if (!reference)
        return ERR_BATTLEGROUND_JOIN_FAILED;

    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bgOrTemplate->GetMapId(), reference->getLevel());
    if (!bracketEntry)
        return ERR_BATTLEGROUND_JOIN_FAILED;

    Bracket* bracket = reference->getBracket(BracketType(arenaSlot));
    ASSERT(bracket);

    uint32 mmr = bracket->getMMV();
    uint32 team = reference->GetTeam();

    BattlegroundQueueTypeId bgQueueTypeIdRandom = BattlegroundMgr::BGQueueTypeId(BATTLEGROUND_RB, 0);

    // check every member of the group to be able to join
    memberscount = 0;
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next(), ++memberscount)
    {
        Player* member = itr->getSource();
        // offline member? don't let join
        if (!member)
            return ERR_BATTLEGROUND_JOIN_FAILED;

        if (member->HasAura(125761) && isRated)
            return ERR_BATTLEGROUND_JOIN_FAILED;
        // don't allow cross-faction join as group
        if (member->GetTeam() != team)
        {
            errorGuid = member->GetGUID();
            return ERR_BATTLEGROUND_JOIN_TIMED_OUT;
        }
        // not in the same battleground level braket, don't let join
        PvPDifficultyEntry const* memberBracketEntry = GetBattlegroundBracketByLevel(bracketEntry->mapId, member->getLevel());
        if (memberBracketEntry != bracketEntry)
            return ERR_BATTLEGROUND_JOIN_RANGE_INDEX;
        // don't let join rated matches if the arena team id doesn't match
        //if (isRated && member->GetArenaTeamId(arenaSlot) != arenaTeamId)
        //    return ERR_BATTLEGROUND_JOIN_FAILED;
        // don't let join if someone from the group is already in that bg queue
        if (member->InBattlegroundQueueForBattlegroundQueueType(bgQueueTypeId))
            return ERR_BATTLEGROUND_JOIN_FAILED;            // not blizz-like
        // don't let join if someone from the group is in bg queue random
        if (member->InBattlegroundQueueForBattlegroundQueueType(bgQueueTypeIdRandom))
            return ERR_IN_RANDOM_BG;
        // don't let join to bg queue random if someone from the group is already in bg queue
        if (bgOrTemplate->GetTypeID() == BATTLEGROUND_RB && member->InBattlegroundQueue())
            return ERR_IN_NON_RANDOM_BG;
        // check for deserter debuff in case not arena queue
        if (bgOrTemplate->GetTypeID() != BATTLEGROUND_AA && !member->CanJoinToBattleground())
            return ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS;
        // check if member can join any more battleground queues
        if (!member->HasFreeBattlegroundQueueId())
            return ERR_BATTLEGROUND_TOO_MANY_QUEUES;        // not blizz-like
        // check if someone in party is using dungeon system
        if (member->isUsingLfg())
            return ERR_LFG_CANT_USE_BATTLEGROUND;
    }

    // only check for MinPlayerCount since MinPlayerCount == MaxPlayerCount for arenas...
    if ((bgOrTemplate->isArena() || isRated) && memberscount != MinPlayerCount)
        return ERR_ARENA_TEAM_PARTY_SIZE;

    return ERR_BATTLEGROUND_NONE;
}

//===================================================
//============== Roll ===============================
//===================================================

void Roll::targetObjectBuildLink()
{
    // called from link()
    getTarget()->addLootValidatorRef(this);
}

void Group::SetDungeonDifficultyID(Difficulty difficulty)
{
    m_dungeonDifficulty = difficulty;
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_DIFFICULTY);

        stmt->setUInt8(0, uint8(m_dungeonDifficulty));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (!player->GetSession())
            continue;

        player->SetDungeonDifficultyID(difficulty);
        player->SendDungeonDifficulty();
    }
}

void Group::SetRaidDifficultyID(Difficulty difficulty)
{
    m_raidDifficulty = difficulty;
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_RAID_DIFFICULTY);

        stmt->setUInt8(0, uint8(m_raidDifficulty));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (!player->GetSession())
            continue;

        player->SetRaidDifficultyID(difficulty);
        player->SendRaidDifficulty(false);
    }
}

void Group::SetLegacyRaidDifficultyID(Difficulty difficulty)
{
    m_legacyRaidDifficulty = difficulty;
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_LEGACY_RAID_DIFFICULTY);

        stmt->setUInt8(0, uint8(m_legacyRaidDifficulty));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (!player->GetSession())
            continue;

        player->SetLegacyRaidDifficultyID(difficulty);
        player->SendRaidDifficulty(true);
    }
}

Difficulty Group::GetDifficultyID(MapEntry const* mapEntry) const
{
    if (!mapEntry->IsRaid())
        return m_dungeonDifficulty;

    MapDifficultyEntry const* defaultDifficulty = GetDefaultMapDifficulty(mapEntry->MapID);
    if (!defaultDifficulty)
        return m_legacyRaidDifficulty;

    DifficultyEntry const* difficulty = sDifficultyStore.LookupEntry(defaultDifficulty->DifficultyID);
    if (!difficulty || difficulty->Flags & DIFFICULTY_FLAG_LEGACY)
        return m_legacyRaidDifficulty;

    return m_raidDifficulty;
}

void Group::ResetInstances(uint8 method, bool isRaid, bool isLegacy, Player* SendMsgTo)
{
    if (isBGGroup() || isBFGroup())
        return;

    // method can be INSTANCE_RESET_ALL, INSTANCE_RESET_CHANGE_DIFFICULTY, INSTANCE_RESET_GROUP_DISBAND

    // we assume that when the difficulty changes, all instances that can be reset will be
    Difficulty diff = GetDungeonDifficultyID();
    uint8 boundType = sObjectMgr->GetboundTypeFromDifficulty(diff);
    if (isRaid)
    {
        if (!isLegacy)
            diff = GetRaidDifficultyID();
        else
            diff = GetLegacyRaidDifficultyID();
    }

    for (BoundInstancesMap::iterator itr = m_boundInstances[boundType].begin(); itr != m_boundInstances[boundType].end();)
    {
        InstanceSave* instanceSave = itr->second.save;
        const MapEntry* entry = sMapStore.LookupEntry(itr->first);
        if (!entry || entry->IsRaid() != isRaid || (!instanceSave->CanReset() && method != INSTANCE_RESET_GROUP_DISBAND))
        {
            ++itr;
            continue;
        }

        if (method == INSTANCE_RESET_ALL)
        {
            // the "reset all instances" method can only reset normal maps
            if (entry->InstanceType == MAP_RAID || diff == DIFFICULTY_HEROIC)
            {
                ++itr;
                continue;
            }
        }

        bool isEmpty = true;
        // if the map is loaded, reset it
        Map* map = sMapMgr->FindMap(instanceSave->GetMapId(), instanceSave->GetInstanceId());
        if (map && map->IsDungeon() && !(method == INSTANCE_RESET_GROUP_DISBAND && !instanceSave->CanReset()))
        {
            if (instanceSave->CanReset())
                isEmpty = ((InstanceMap*)map)->Reset(method);
            else
                isEmpty = !map->HavePlayers();
        }

        if (SendMsgTo)
        {
            if (isEmpty)
                SendMsgTo->SendResetInstanceSuccess(instanceSave->GetMapId());
            else
                SendMsgTo->SendResetInstanceFailed(0, instanceSave->GetMapId());
        }

        if (isEmpty || method == INSTANCE_RESET_GROUP_DISBAND || method == INSTANCE_RESET_CHANGE_DIFFICULTY)
        {
            // do not reset the instance, just unbind if others are permanently bound to it
            if (instanceSave->CanReset())
                instanceSave->DeleteFromDB();
            else
            {
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_INSTANCE_BY_INSTANCE);

                stmt->setUInt32(0, instanceSave->GetInstanceId());

                CharacterDatabase.Execute(stmt);
            }


            // i don't know for sure if hash_map iterators
            m_boundInstances[boundType].erase(itr);
            itr = m_boundInstances[boundType].begin();
            // this unloads the instance save unless online players are bound to it
            // (eg. permanent binds or GM solo binds)
            instanceSave->RemoveGroup(this);
        }
        else
            ++itr;
    }
}

InstanceGroupBind* Group::GetBoundInstance(Player* player)
{
    uint32 mapid = player->GetMapId();
    MapEntry const* mapEntry = sMapStore.LookupEntry(mapid);
    return GetBoundInstance(mapEntry);
}

InstanceGroupBind* Group::GetBoundInstance(Map* aMap)
{
    return GetBoundInstance(aMap->GetEntry());
}

InstanceGroupBind* Group::GetBoundInstance(MapEntry const* mapEntry)
{
    if (!mapEntry || !mapEntry->IsDungeon())
        return NULL;

    Difficulty difficulty = GetDifficultyID(mapEntry);
    return GetBoundInstance(difficulty, mapEntry->MapID);
}

InstanceGroupBind* Group::GetBoundInstance(Difficulty difficulty, uint32 mapId)
{
    // some instances only have one difficulty
    GetDownscaledMapDifficultyData(mapId, difficulty);

    uint8 boundType = sObjectMgr->GetboundTypeFromDifficulty(difficulty);

    BoundInstancesMap::iterator itr = m_boundInstances[boundType].find(mapId);
    if (itr != m_boundInstances[boundType].end())
        return &itr->second;
    else
        return NULL;
}

InstanceGroupBind* Group::BindToInstance(InstanceSave* save, bool permanent, bool load)
{
    if (!save || isBGGroup() || isBFGroup())
        return NULL;

    if(!save->CanBeSave())
        permanent = false;

    uint8 boundType = sObjectMgr->GetboundTypeFromDifficulty(save->GetDifficultyID());
    InstanceGroupBind& bind = m_boundInstances[boundType][save->GetMapId()];
    if(save->CanBeSave())
    {
        if (!load && (!bind.save || permanent != bind.perm || save != bind.save))
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_GROUP_INSTANCE);

            stmt->setUInt32(0, m_dbStoreId);
            stmt->setUInt32(1, save->GetInstanceId());
            stmt->setBool(2, permanent);

            CharacterDatabase.Execute(stmt);
        }
    }

    if (bind.save != save)
    {
        if (bind.save)
            bind.save->RemoveGroup(this);
        save->AddGroup(this);
    }

    bind.save = save;
    bind.perm = permanent;
    if (!load)
        sLog->outDebug(LOG_FILTER_MAPS, "Group::BindToInstance: Group (guid: %u, storage id: %u) is now bound to map %d, instance %d, difficulty %d",
        GetGUIDLow(), m_dbStoreId, save->GetMapId(), save->GetInstanceId(), save->GetDifficultyID());

    return &bind;
}

void Group::UnbindInstance(uint32 mapid, uint8 difficulty, bool unload)
{
    uint8 boundType = sObjectMgr->GetboundTypeFromDifficulty(difficulty);
    BoundInstancesMap::iterator itr = m_boundInstances[boundType].find(mapid);
    if (itr != m_boundInstances[boundType].end())
    {
        if (!unload)
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_INSTANCE_BY_GUID);

            stmt->setUInt32(0, m_dbStoreId);
            stmt->setUInt32(1, itr->second.save->GetInstanceId());

            CharacterDatabase.Execute(stmt);
        }

        itr->second.save->RemoveGroup(this);                // save can become invalid
        m_boundInstances[boundType].erase(itr);
    }
}

void Group::_homebindIfInstance(Player* player)
{
    if (player && !player->isGameMaster() && sMapStore.LookupEntry(player->GetMapId())->IsDungeon())
        player->m_InstanceValid = false;
}

void Group::BroadcastGroupUpdate(void)
{
    // FG: HACK: force flags update on group leave - for values update hack
    // -- not very efficient but safe
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        Player* pp = ObjectAccessor::FindPlayer(citr->guid);
        if (pp && pp->IsInWorld())
        {
            pp->ForceValuesUpdateAtIndex(UNIT_FIELD_BYTES_2);
            pp->ForceValuesUpdateAtIndex(UNIT_FIELD_FACTION_TEMPLATE);
            sLog->outDebug(LOG_FILTER_GENERAL, "-- Forced group value update for '%s'", pp->GetName());
        }
    }
}

void Group::ResetMaxEnchantingLevel()
{
    m_maxEnchantingLevel = 0;
    Player* pMember = NULL;
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        pMember = ObjectAccessor::FindPlayer(citr->guid);
        if (pMember && m_maxEnchantingLevel < pMember->GetSkillValue(SKILL_ENCHANTING))
            m_maxEnchantingLevel = pMember->GetSkillValue(SKILL_ENCHANTING);
    }
}

void Group::SetLootMethod(LootMethod method)
{
    m_lootMethod = method;
}

void Group::SetLooterGuid(ObjectGuid const& guid)
{
    m_looterGuid = guid;
}

void Group::SetLootThreshold(ItemQualities threshold)
{
    m_lootThreshold = threshold;
}

void Group::SetLfgRoles(ObjectGuid guid, const uint8 roles)
{
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    slot->roles = roles;
}

uint8 Group::GetLfgRoles(ObjectGuid guid)
{
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return 0;

    return slot->roles;
}

bool Group::IsFull() const
{
    return isRaidGroup() ? (m_memberSlots.size() >= MAX_RAID_SIZE) : (m_memberSlots.size() >= MAX_GROUP_SIZE);
}

bool Group::isLFGGroup() const
{
    return (m_groupType & GROUPTYPE_LFG) != 0;
}

bool Group::isRaidGroup() const
{
    return (m_groupType & GROUPTYPE_RAID) != 0;
}

bool Group::isBGGroup() const
{
    return m_bgGroup != NULL;
}

bool Group::isArenaGroup() const
{
    return m_bgGroup && m_bgGroup->isArena();
}

bool Group::isBFGroup() const
{
    return m_bfGroup != NULL;
}

bool Group::IsCreated() const
{
    return GetMembersCount() > 0;
}

ObjectGuid Group::GetLeaderGUID() const
{
    return m_leaderGuid;
}

ObjectGuid Group::GetGUID() const
{
    return m_guid;
}

uint32 Group::GetGUIDLow() const
{
    return m_guid.GetGUIDLow();
}

const char * Group::GetLeaderName() const
{
    return m_leaderName.c_str();
}

LootMethod Group::GetLootMethod() const
{
    return m_lootMethod;
}

ObjectGuid Group::GetLooterGuid() const
{
    return m_looterGuid;
}

ItemQualities Group::GetLootThreshold() const
{
    return m_lootThreshold;
}

bool Group::IsMember(ObjectGuid guid) const
{
    return _getMemberCSlot(guid) != m_memberSlots.end();
}

bool Group::IsLeader(ObjectGuid guid) const
{
    return (GetLeaderGUID() == guid);
}

ObjectGuid Group::GetMemberGUID(std::string const& name)
{
    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->name == name)
            return itr->guid;
        return ObjectGuid::Empty;
}

bool Group::IsAssistant(ObjectGuid guid) const
{
    member_citerator mslot = _getMemberCSlot(guid);
    if (mslot == m_memberSlots.end())
        return false;
    return mslot->flags & MEMBER_FLAG_ASSISTANT;
}

bool Group::SameSubGroup(ObjectGuid guid1, ObjectGuid guid2) const
{
    member_citerator mslot2 = _getMemberCSlot(guid2);
    if (mslot2 == m_memberSlots.end())
        return false;
    return SameSubGroup(guid1, &*mslot2);
}

bool Group::SameSubGroup(ObjectGuid guid1, MemberSlot const* slot2) const
{
    member_citerator mslot1 = _getMemberCSlot(guid1);
    if (mslot1 == m_memberSlots.end() || !slot2)
        return false;
    return (mslot1->group == slot2->group);
}

bool Group::HasFreeSlotSubGroup(uint8 subgroup) const
{
    return (m_subGroupsCounts && m_subGroupsCounts[subgroup] < MAX_GROUP_SIZE);
}


uint8 Group::GetMemberGroup(ObjectGuid guid) const
{
    member_citerator mslot = _getMemberCSlot(guid);
    if (mslot == m_memberSlots.end())
        return (MAX_RAID_SUBGROUPS+1);
    return mslot->group;
}

void Group::SetBattlegroundGroup(Battleground* bg)
{
    m_bgGroup = bg;
}

void Group::SetBattlefieldGroup(Battlefield *bg)
{
    m_bfGroup = bg;
}

void Group::setGroupMemberRole(ObjectGuid guid, uint32 role)
{
    for (MemberSlotList::iterator member = m_memberSlots.begin(); member != m_memberSlots.end(); ++member)
    {
        if (member->guid == guid)
        {
            member->roles = role;
            break;
        }
    }
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_ROLE);
    if (stmt != NULL)
    {
        stmt->setUInt8(0, role);
        stmt->setUInt64(1, guid.GetCounter());
        CharacterDatabase.Execute(stmt);
    }
}

void Group::SetGroupMemberFlag(ObjectGuid guid, bool apply, GroupMemberFlags flag)
{
    // Assistants, main assistants and main tanks are only available in raid groups
    if (!isRaidGroup())
        return;

    // Check if player is really in the raid
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    // Do flag specific actions, e.g ensure uniqueness
    switch (flag) {
    case MEMBER_FLAG_MAINASSIST:
        RemoveUniqueGroupMemberFlag(MEMBER_FLAG_MAINASSIST);         // Remove main assist flag from current if any.
        break;
    case MEMBER_FLAG_MAINTANK:
        RemoveUniqueGroupMemberFlag(MEMBER_FLAG_MAINTANK);           // Remove main tank flag from current if any.
        break;
    case MEMBER_FLAG_ASSISTANT:
        break;
    default:
        return;                                                      // This should never happen
    }

    // Switch the actual flag
    ToggleGroupMemberFlag(slot, flag, apply);

    // Preserve the new setting in the db
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_FLAG);

    stmt->setUInt8(0, slot->flags);
    stmt->setUInt64(1, guid.GetCounter());

    CharacterDatabase.Execute(stmt);

    // Broadcast the changes to the group
    SendUpdate();
}


void Group::ErraseRollbyRealSlot(uint8 slot, Loot* loot)
{
    for (Rolls::iterator iter=RollId.begin(); iter != RollId.end();++iter)
    {
        if ((*iter)->itemSlot == slot && loot == (*iter)->getLoot() && (*iter)->isValid() )
        {
            delete *iter;
            RollId.erase(iter);
            break;
        }
    }
}

Group::Rolls::iterator Group::GetRoll(uint8 _aoeSlot)
{
    Rolls::iterator iter;
    for (iter=RollId.begin(); iter != RollId.end(); ++iter)
        if ((*iter)->aoeSlot == _aoeSlot && (*iter)->isValid())
            return iter;
    return RollId.end();
}

void Group::LinkMember(GroupReference* pRef)
{
    m_memberMgr.insertFirst(pRef);
}

void Group::DelinkMember(ObjectGuid guid)
{
    GroupReference* ref = m_memberMgr.getFirst();
    while (ref)
    {
        GroupReference* nextRef = ref->next();
        if (ref->getSource()->GetGUID() == guid)
        {
            ref->unlink();
            break;
        }
        ref = nextRef;
    }
}

Group::BoundInstancesMap& Group::GetBoundInstances(Difficulty difficulty)
{
    uint8 boundType = sObjectMgr->GetboundTypeFromDifficulty(difficulty);
    return m_boundInstances[boundType];
}

void Group::_initRaidSubGroupsCounter()
{
    // Sub group counters initialization
    if (!m_subGroupsCounts)
        m_subGroupsCounts = new uint8[MAX_RAID_SUBGROUPS];

    memset((void*)m_subGroupsCounts, 0, (MAX_RAID_SUBGROUPS)*sizeof(uint8));

    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        ++m_subGroupsCounts[itr->group];
}

Group::member_citerator Group::_getMemberCSlot(ObjectGuid Guid) const
{
    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->guid == Guid)
            return itr;
    return m_memberSlots.end();
}

Group::member_witerator Group::_getMemberWSlot(ObjectGuid Guid)
{
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->guid == Guid)
            return itr;
    return m_memberSlots.end();
}

void Group::SubGroupCounterIncrease(uint8 subgroup)
{
    if (m_subGroupsCounts)
        ++m_subGroupsCounts[subgroup];
}

void Group::SubGroupCounterDecrease(uint8 subgroup)
{
    if (m_subGroupsCounts)
        --m_subGroupsCounts[subgroup];
}

void Group::RemoveUniqueGroupMemberFlag(GroupMemberFlags flag)
{
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->flags & flag)
            itr->flags &= ~flag;
}

void Group::ToggleGroupMemberFlag(member_witerator slot, uint8 flag, bool apply)
{
    if (apply)
        slot->flags |= flag;
    else
        slot->flags &= ~flag;
}

bool Group::IsGuildGroup(ObjectGuid const& guildId, bool AllInSameMap, bool AllInSameInstanceId)
{
    uint32 mapId = 0;
    uint32 InstanceId = 0;
    uint32 count = 0;
    std::vector<Player*> members;
    // First we populate the array
    for (GroupReference *itr = GetFirstMember(); itr != NULL; itr = itr->next()) // Loop trought all members
        if (Player *player = itr->getSource())
            if (player->GetGuildId() == guildId.GetCounter()) // Check if it has a guild
                members.push_back(player);

    bool ret = false;
    count = members.size();
    for (std::vector<Player*>::iterator itr = members.begin(); itr != members.end(); ++itr) // Iterate through players
    {
        if (Player* player = (*itr))
        {
            if (mapId == 0)
                mapId = player->GetMapId();

            if (InstanceId == 0)
                InstanceId = player->GetInstanceId();

            if (player->GetMap()->IsNonRaidDungeon() && !ret)
                if (count >= 3)
                    ret = true;

            if (player->GetMap()->IsRaid() && !ret)
            {
                switch (player->GetMap()->GetDifficultyID())
                {
                    case DIFFICULTY_10_N:
                    case DIFFICULTY_10_HC:
                        if (count >= 8)
                            ret = true;
                        break;

                    case DIFFICULTY_25_N:
                    case DIFFICULTY_25_HC:
                        if (count >= 20)
                            ret = true;
                        break;
                }
            }

            if (player->GetMap()->IsBattleArena() && !ret)
                if (count == GetMembersCount())
                    ret = true;

            if (player->GetMap()->IsBattleground() && !ret)
                if (Battleground* bg = player->GetBattleground())
                    if (count >= uint32(bg->GetMaxPlayers() * 0.8f))
                        ret = true;

            // ToDo: Check 40-player raids: 10/40

            if (AllInSameMap && (mapId != player->GetMapId()))
                return false;

            if (AllInSameInstanceId && (InstanceId != player->GetInstanceId()))
                return false;
        }
    }

    return ret;
}

void Group::UpdateGuildAchievementCriteria(AchievementCriteriaTypes type, uint32 miscValue1, uint32 miscValue2, uint32 miscValue3, Unit* pUnit, WorldObject* pRewardSource)
{
    // We will update criteria for each guild in grouplist but only once
    std::list<uint32> guildList;
    for (GroupReference *itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        if (Player *pPlayer = itr->getSource())
        {
            // Check for reward
            if (pRewardSource)
            {
                if (!pPlayer->IsAtGroupRewardDistance(pRewardSource))
                    continue;

                if (!pPlayer->isAlive())
                    continue;
            }

            uint32 guildId = pPlayer->GetGuildId();

            if (guildId == 0)
                continue;

            if (!guildList.empty())
            {
                // if we already have any guild in list
                // then check new guild
                bool bUnique = true;
                for (std::list<uint32>::const_iterator itr2 = guildList.begin(); itr2 != guildList.end(); ++itr2)
                {
                    if ((*itr2) == guildId)
                    {
                        bUnique = false;
                        break;
                    }
                }
                // If we have already rewarded current guild then continue
                // else update criteria 
                if (bUnique && guildId)
                {
                    guildList.push_back(guildId);
                    //if (Guild* pGuild = sGuildMgr->GetGuildById(guildId))
                        //pGuild->GetAchievementMgr().UpdateAchievementCriteria(type, miscValue1, miscValue2, miscValue3, pUnit, pPlayer);
                }
            }
            else
            {
                // If that's first guild in list
                // then add to the list and update criteria
                guildList.push_back(guildId);
                //if (Guild* pGuild = sGuildMgr->GetGuildById(guildId))
                    //pGuild->GetAchievementMgr().UpdateAchievementCriteria(type, miscValue1, miscValue2, miscValue3, pUnit, pPlayer);
            }
        }
    }
}

bool Group::leaderInstanceCheckFail()
{
    if (Player *leader = ObjectAccessor::FindPlayer(GetLeaderGUID()))
    {
        const uint32 leaderInstanceID = leader->GetInstanceId();
        if (leaderInstanceID == 0)
        {
            Player *pMember = NULL;
            for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
            {
                pMember = ObjectAccessor::FindPlayer(citr->guid);
                if ( pMember && (pMember->GetInstanceId() != leaderInstanceID) )
                    return true;
            }
        }       
    }

    return false;
}

uint32 Group::GetAverageMMR(BracketType bracket) const
{
    uint32 matchMakerRating = 0;
    uint32 playerDivider = 0;

    Player *pMember = NULL;
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        pMember = ObjectAccessor::FindPlayer(citr->guid);
        if (pMember)
        {
            matchMakerRating += pMember->getBracket(bracket)->getMMV();
            ++playerDivider;
        }
    }

    // x/0 = crash
    if (playerDivider == 0)
        playerDivider = 1;

    matchMakerRating /= playerDivider;

    return matchMakerRating;
}

void Group::AddRaidMarker(uint8 markerId, uint32 mapId, float positionX, float positionY, float positionZ, ObjectGuid transportGuid)
{
    if (markerId >= RAID_MARKERS_COUNT || m_markers[markerId])
        return;

    m_activeMarkers |= (1 << markerId);
    m_markers[markerId] = Trinity::make_unique<RaidMarker>(mapId, positionX, positionY, positionZ, transportGuid);
    SendRaidMarkersChanged();
}

void Group::DeleteRaidMarker(uint8 markerId)
{
    if (markerId > RAID_MARKERS_COUNT)
        return;

    for (uint8 i = 0; i < RAID_MARKERS_COUNT; i++)
        if (m_markers[i] && (markerId == i || markerId == RAID_MARKERS_COUNT))
        {
            m_markers[i] = nullptr;
            m_activeMarkers &= ~(1 << i);
        }

    SendRaidMarkersChanged();
}

void Group::SendRaidMarkersChanged(WorldSession* session, int8 partyIndex)
{
    WorldPackets::Party::RaidMarkersChanged packet;

    packet.PartyIndex = partyIndex;
    packet.ActiveMarkers = m_activeMarkers;

    for (uint8 i = 0; i < RAID_MARKERS_COUNT; i++)
        if (m_markers[i])
            packet.RaidMarkers.push_back(m_markers[i].get());

    if (session)
        session->SendPacket(packet.Write());
    else
        BroadcastPacket(packet.Write(), false);
}