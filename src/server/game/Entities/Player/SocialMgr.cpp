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

#include "SocialMgr.h"

#include "DatabaseEnv.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "World.h"
#include "Util.h"
#include "AccountMgr.h"
#include "SocialPackets.h"

PlayerSocial::PlayerSocial()
{
    m_playerGUID.Clear();
}

PlayerSocial::~PlayerSocial()
{
    m_playerSocialMap.clear();
}

uint32 PlayerSocial::GetNumberOfSocialsWithFlag(SocialFlag flag)
{
    uint32 counter = 0;
    for (PlayerSocialMap::const_iterator itr = m_playerSocialMap.begin(); itr != m_playerSocialMap.end(); ++itr)
        if (itr->second.Flags & flag)
            ++counter;

    return counter;
}

bool PlayerSocial::AddToSocialList(ObjectGuid const& friendGuid, SocialFlag flag)
{
    // check client limits
    if (GetNumberOfSocialsWithFlag(flag) >= (((flag & SOCIAL_FLAG_FRIEND) != 0) ? SOCIALMGR_FRIEND_LIMIT : SOCIALMGR_IGNORE_LIMIT))
        return false;

    PlayerSocialMap::iterator itr = m_playerSocialMap.find(friendGuid);
    if (itr != m_playerSocialMap.end())
    {
        itr->second.Flags |= flag;

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_CHARACTER_SOCIAL_FLAGS);

        stmt->setUInt8(0, itr->second.Flags);
        stmt->setUInt64(1, GetPlayerGUID().GetCounter());
        stmt->setUInt64(2, friendGuid.GetCounter());

        CharacterDatabase.Execute(stmt);
    }
    else
    {
        m_playerSocialMap[friendGuid].Flags |= flag;

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_SOCIAL);

        stmt->setUInt64(0, GetPlayerGUID().GetCounter());
        stmt->setUInt64(1, friendGuid.GetCounter());
        stmt->setUInt8(2, flag);

        CharacterDatabase.Execute(stmt);
    }
    return true;
}

void PlayerSocial::RemoveFromSocialList(ObjectGuid const& friendGuid, SocialFlag flag)
{
    PlayerSocialMap::iterator itr = m_playerSocialMap.find(friendGuid);
    if (itr == m_playerSocialMap.end())                     // not exist
        return;

    itr->second.Flags &= ~flag;
    if (!itr->second.Flags)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHARACTER_SOCIAL);

        stmt->setUInt64(0, GetPlayerGUID().GetCounter());
        stmt->setUInt64(1, friendGuid.GetCounter());

        CharacterDatabase.Execute(stmt);

        m_playerSocialMap.erase(itr);
    }
    else
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_REM_CHARACTER_SOCIAL_FLAGS);

        stmt->setUInt8(0, itr->second.Flags);
        stmt->setUInt64(1, GetPlayerGUID().GetCounter());
        stmt->setUInt64(2, friendGuid.GetCounter());

        CharacterDatabase.Execute(stmt);
    }
}

void PlayerSocial::SetFriendNote(ObjectGuid const& friendGuid, std::string note)
{
    PlayerSocialMap::const_iterator itr = m_playerSocialMap.find(friendGuid);
    if (itr == m_playerSocialMap.end())                     // not exist
        return;

    utf8truncate(note, 48);                                  // DB and client size limitation

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_SOCIAL_NOTE);

    stmt->setString(0, note);
    stmt->setUInt64(1, GetPlayerGUID().GetCounter());
    stmt->setUInt64(2, friendGuid.GetCounter());

    CharacterDatabase.Execute(stmt);

    m_playerSocialMap[friendGuid].Note = note;
}

void PlayerSocial::SendSocialList(Player* player, uint32 flags)
{
    if (!player)
        return;

    ASSERT(player);

    WorldPackets::Social::ContactList contactList;
    contactList.Flags = flags;

    for (PlayerSocialMap::value_type& v : m_playerSocialMap)
    {
        if (!(v.second.Flags & flags))
            continue;

        sSocialMgr->GetFriendInfo(player, v.first, v.second);

        contactList.Contacts.emplace_back(v.first, v.second);

        // client's friends list and ignore list limit
        if (contactList.Contacts.size() >= (((flags & SOCIAL_FLAG_FRIEND) != 0) ? SOCIALMGR_FRIEND_LIMIT : SOCIALMGR_IGNORE_LIMIT))
            break;
    }

    player->SendDirectMessage(contactList.Write());
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_CONTACT_LIST");
}

bool PlayerSocial::_HasContact(ObjectGuid const& guid, SocialFlag flags)
{
    PlayerSocialMap::const_iterator itr = m_playerSocialMap.find(guid);
    if (itr != m_playerSocialMap.end())
        return (itr->second.Flags & flags) != 0;

    return false;
}

bool PlayerSocial::HasFriend(ObjectGuid const& friendGuid)
{
    return _HasContact(friendGuid, SOCIAL_FLAG_FRIEND);
}

bool PlayerSocial::HasIgnore(ObjectGuid const& ignoreGuid)
{
    return _HasContact(ignoreGuid, SOCIAL_FLAG_IGNORED);
}

SocialMgr::SocialMgr()
{
}

SocialMgr::~SocialMgr()
{
}

void SocialMgr::GetFriendInfo(Player* player, ObjectGuid const& friendGUID, FriendInfo &friendInfo)
{
    if (!player)
        return;

    friendInfo.Status = FRIEND_STATUS_OFFLINE;
    friendInfo.Area = 0;
    friendInfo.Level = 0;
    friendInfo.Class = 0;

    Player* pFriend = ObjectAccessor::FindPlayer(friendGUID);
    if (!pFriend)
        return;

    uint32 team = player->GetTeam();
    AccountTypes security = player->GetSession()->GetSecurity();
    bool allowTwoSideWhoList = sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_WHO_LIST);
    AccountTypes gmLevelInWhoList = AccountTypes(sWorld->getIntConfig(CONFIG_GM_LEVEL_IN_WHO_LIST));

    PlayerSocialMap::iterator itr = player->GetSocial()->m_playerSocialMap.find(friendGUID);
    if (itr != player->GetSocial()->m_playerSocialMap.end())
        friendInfo.Note = itr->second.Note;

    // PLAYER see his team only and PLAYER can't see MODERATOR, GAME MASTER, ADMINISTRATOR characters
    // MODERATOR, GAME MASTER, ADMINISTRATOR can see all
    if (pFriend && pFriend->GetName() &&
        (!AccountMgr::IsPlayerAccount(security) ||
        ((pFriend->GetTeam() == team || allowTwoSideWhoList) && (pFriend->GetSession()->GetSecurity() <= gmLevelInWhoList))) &&
        pFriend->IsVisibleGloballyFor(player))
    {
        friendInfo.Status = FRIEND_STATUS_ONLINE;
        if (pFriend->isAFK())
            friendInfo.Status = FRIEND_STATUS_AFK;
        if (pFriend->isDND())
            friendInfo.Status = FRIEND_STATUS_DND;
        friendInfo.Area = pFriend->GetZoneId();
        friendInfo.Level = pFriend->getLevel();
        friendInfo.Class = pFriend->getClass();
    }
}

void SocialMgr::SendFriendStatus(Player* player, FriendsResult result, ObjectGuid const& friendGuid, bool broadcast)
{
    FriendInfo fi;
    GetFriendInfo(player, friendGuid, fi);

    WorldPackets::Social::FriendStatus friendStatus;
    friendStatus.Initialize(friendGuid, result, fi);

    if (broadcast)
        BroadcastToFriendListers(player, friendStatus.Write());
    else
        player->GetSession()->SendPacket(friendStatus.Write());
}

void SocialMgr::BroadcastToFriendListers(Player* player, WorldPacket const* packet)
{
    if (!player)
        return;

    uint32 team = player->GetTeam();
    AccountTypes security = player->GetSession()->GetSecurity();
    ObjectGuid guid = player->GetGUID();
    AccountTypes gmLevelInWhoList = AccountTypes(sWorld->getIntConfig(CONFIG_GM_LEVEL_IN_WHO_LIST));
    bool allowTwoSideWhoList = sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_WHO_LIST);

    for (SocialMap::const_iterator itr = m_socialMap.begin(); itr != m_socialMap.end(); ++itr)
    {
        PlayerSocialMap::const_iterator itr2 = itr->second.m_playerSocialMap.find(guid);
        if (itr2 != itr->second.m_playerSocialMap.end() && (itr2->second.Flags & SOCIAL_FLAG_FRIEND))
        {
            Player* pFriend = ObjectAccessor::FindPlayer(itr->first);

            // PLAYER see his team only and PLAYER can't see MODERATOR, GAME MASTER, ADMINISTRATOR characters
            // MODERATOR, GAME MASTER, ADMINISTRATOR can see all
            if (pFriend && pFriend->IsInWorld() &&
                (!AccountMgr::IsPlayerAccount(pFriend->GetSession()->GetSecurity()) ||
                ((pFriend->GetTeam() == team || allowTwoSideWhoList) && security <= gmLevelInWhoList)) &&
                player->IsVisibleGloballyFor(pFriend))
            {
                pFriend->GetSession()->SendPacket(packet);
            }
        }
    }
}

PlayerSocial* SocialMgr::LoadFromDB(PreparedQueryResult result, ObjectGuid const& guid)
{
    PlayerSocial *social = &m_socialMap[guid];
    social->SetPlayerGUID(guid);

    if (!result)
        return social;

    do
    {
        Field* fields = result->Fetch();

        ObjectGuid friendGuid = ObjectGuid::Create<HighGuid::Player>(fields[0].GetUInt64());
        uint8 flags = fields[1].GetUInt8();

        social->m_playerSocialMap[friendGuid] = FriendInfo(flags, fields[2].GetString());
    }
    while (result->NextRow());

    return social;
}

