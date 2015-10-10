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

#include "Channel.h"
#include "Chat.h"
#include "ObjectMgr.h"
#include "SocialMgr.h"
#include "World.h"
#include "DatabaseEnv.h"
#include "AccountMgr.h"
#include <cwctype>
#include "ChannelPackets.h"
#include "ChatPackets.h"

Channel::Channel(const std::string& name, uint32 channel_id, uint32 Team)
 : m_announce(true), m_ownership(true), m_name(name), m_password(""), m_flags(0), m_channelId(channel_id), m_ownerGUID(), m_Team(Team), _special(false)
{
    m_IsSaved = false;

    if (IsWorld())
        m_announce = false;

    // set special flags if built-in channel
    if (ChatChannelsEntry const* ch = sChatChannelsStore.LookupEntry(channel_id)) // check whether it's a built-in channel
    {
        m_announce = false;                                 // no join/leave announces
        m_ownership = false;                                // no ownership handout

        m_flags |= CHANNEL_FLAG_GENERAL;                    // for all built-in channels

        if (ch->Flags & CHANNEL_DBC_FLAG_TRADE)             // for trade channel
            m_flags |= CHANNEL_FLAG_TRADE;

        if (ch->Flags & CHANNEL_DBC_FLAG_CITY_ONLY2)        // for city only channels
            m_flags |= CHANNEL_FLAG_CITY;

        if (ch->Flags & CHANNEL_DBC_FLAG_LFG)               // for LFG channel
            m_flags |= CHANNEL_FLAG_LFG;
        else                                                // for all other channels
            m_flags |= CHANNEL_FLAG_NOT_LFG;
    }
    else if (!stricmp(m_name.c_str(), "world"))
    {
        m_announce = false;
        _special = true;
    }
    else                                                    // it's custom channel
    {
        m_flags |= CHANNEL_FLAG_CUSTOM;

        // If storing custom channels in the db is enabled either load or save the channel
        if (sWorld->getBoolConfig(CONFIG_PRESERVE_CUSTOM_CHANNELS))
        {
            PreparedStatement *stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHANNEL);
            stmt->setString(0, name);
            stmt->setUInt32(1, m_Team);
            PreparedQueryResult result = CharacterDatabase.Query(stmt);

            if (result) //load
            {
                Field* fields = result->Fetch();
                m_announce = fields[0].GetBool();
                m_ownership = fields[1].GetBool();
                m_password  = fields[2].GetString();
                const char* db_BannedList = fields[3].GetCString();

                if (db_BannedList)
                {
                    Tokenizer tokens(db_BannedList, ' ');
                    Tokenizer::const_iterator iter;
                    for (iter = tokens.begin(); iter != tokens.end(); ++iter)
                    {
                        std::string bannedGuidStr(*iter);
                        ObjectGuid banned_guid;
                        banned_guid.SetRawValue(uint64(strtoull(bannedGuidStr.substr(0, 16).c_str(), NULL, 16)), uint64(strtoull(bannedGuidStr.substr(16).c_str(), NULL, 16)));

                        if (!banned_guid.IsEmpty())
                        {
                            sLog->outDebug(LOG_FILTER_CHATSYS, "Channel(%s) loaded banned guid: %s", name.c_str(), banned_guid.ToString().c_str());
                            banned.insert(banned_guid);
                        }
                    }
                }
            }
            else // save
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHANNEL);
                stmt->setString(0, name);
                stmt->setUInt32(1, m_Team);
                CharacterDatabase.Execute(stmt);
                sLog->outDebug(LOG_FILTER_CHATSYS, "Channel(%s) saved in database", name.c_str());
            }

            m_IsSaved = true;
        }
    }
}

bool Channel::IsWorld() const
{
    std::string lowername;
    uint32 nameLength = m_name.length();
    for (uint32 i = 0; i < nameLength; ++i)
        lowername.push_back(std::towlower(m_name[i]));

    if (lowername == "world")
        return true;

    return false;
}

void Channel::UpdateChannelInDB() const
{
    if (m_IsSaved)
    {
        std::ostringstream banlist;
        BannedList::const_iterator iter;
        for (iter = banned.begin(); iter != banned.end(); ++iter)
            banlist << (*iter) << ' ';

        std::string banListStr = banlist.str();

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHANNEL);
        stmt->setBool(0, m_announce);
        stmt->setBool(1, m_ownership);
        stmt->setString(2, m_password);
        stmt->setString(3, banListStr);
        stmt->setString(4, m_name);
        stmt->setUInt32(5, m_Team);
        CharacterDatabase.Execute(stmt);

        sLog->outDebug(LOG_FILTER_CHATSYS, "Channel(%s) updated in database", m_name.c_str());
    }

}

void Channel::UpdateChannelUseageInDB() const
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHANNEL_USAGE);
    stmt->setString(0, m_name);
    stmt->setUInt32(1, m_Team);
    CharacterDatabase.Execute(stmt);
}

void Channel::CleanOldChannelsInDB()
{
    if (sWorld->getIntConfig(CONFIG_PRESERVE_CUSTOM_CHANNEL_DURATION) > 0)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_OLD_CHANNELS);
        stmt->setUInt32(0, sWorld->getIntConfig(CONFIG_PRESERVE_CUSTOM_CHANNEL_DURATION) * DAY);
        CharacterDatabase.Execute(stmt);

        sLog->outDebug(LOG_FILTER_CHATSYS, "Cleaned out unused custom chat channels.");
    }
}

void Channel::JoinChannel(Player* player, const char *pass)
{
    ObjectGuid const& guid = player->GetGUID();
    if (IsOn(guid))
    {
        if (!IsConstant())                                   // non send error message for built-in channels
        {
            WorldPackets::Channel::ChannelNotify notify;
            MakePlayerAlreadyMember(notify, guid);
            player->SendDirectMessage(notify.Write());
        }
        return;
    }

    if (IsBanned(guid))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeBanned(notify);
        player->SendDirectMessage(notify.Write());
        return;
    }

    if (m_password.length() > 0 && strcmp(pass, m_password.c_str()))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeWrongPassword(notify);
        player->SendDirectMessage(notify.Write());
        return;
    }

    if (HasFlag(CHANNEL_FLAG_LFG) &&
        sWorld->getBoolConfig(CONFIG_RESTRICTED_LFG_CHANNEL) && AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()) && player->GetGroup())
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotInLfg(notify);
        player->SendDirectMessage(notify.Write());
        return;
    }

    player->JoinedChannel(this);

    if (m_announce && (!player || !AccountMgr::IsModeratorAccount(player->GetSession()->GetSecurity()) || !sWorld->getBoolConfig(CONFIG_SILENTLY_GM_JOIN_TO_CHANNEL)))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeJoined(notify, guid);
        SendToAll(notify.Write());
    }

    PlayerInfo pinfo;
    pinfo.player = guid;
    pinfo.flags = MEMBER_FLAG_NONE;
    players[guid] = pinfo;

    /*
    WorldPackets::Channel::ChannelNotify notify;
    MakeYouJoined(notify);
    SendToOne(notify.Write(), p);
    */

    WorldPackets::Channel::ChannelNotifyJoined notify;
    //notify.ChannelWelcomeMsg = "";
    notify.ChatChannelID = m_channelId;
    //notify.InstanceID = 0;
    notify._ChannelFlags = m_flags;
    notify._Channel = m_name;
    player->SendDirectMessage(notify.Write());

    JoinNotify(guid);

    // Custom channel handling
    if (!IsConstant())
    {
        // Update last_used timestamp in db
        if (!players.empty())
            UpdateChannelUseageInDB();

        // If the channel has no owner yet and ownership is allowed, set the new owner.
        if (!m_ownerGUID && m_ownership)
        {
            SetOwner(guid, (players.size() > 1 ? true : false));
            players[guid].SetModerator(true);
        }
    }
}

void Channel::LeaveChannel(Player* player, bool send)
{
    ObjectGuid const& guid = player->GetGUID();
    if (!IsOn(guid))
    {
        if (send)
        {
            WorldPackets::Channel::ChannelNotify notify;
            MakeNotMember(notify);
            player->SendDirectMessage(notify.Write());
        }
    }
    else
    {
        if (send)
        {
            /*
            WorldPackets::Channel::ChannelNotify notify;
            MakeYouLeft(notify);
            player->SendDirectMessage(notify.Write());
            */

            WorldPackets::Channel::ChannelNotifyLeft notify;
            notify.Channel = m_name;
            notify.ChatChannelID = 0;
            //notify.Suspended = false;
            player->SendDirectMessage(notify.Write());

            if (player)
                player->LeftChannel(this);
        }

        bool changeowner = players[guid].IsOwner();

        players.erase(guid);
        if (m_announce && (!player || !AccountMgr::IsModeratorAccount(player->GetSession()->GetSecurity()) || !sWorld->getBoolConfig(CONFIG_SILENTLY_GM_JOIN_TO_CHANNEL)))
        {
            WorldPackets::Channel::ChannelNotify notify;
            MakeLeft(notify, guid);
            SendToAll(notify.Write());
        }

        LeaveNotify(guid);

        if (!IsConstant())
        {
            // Update last_used timestamp in db
            UpdateChannelUseageInDB();

            // If the channel owner left and there are still players inside, pick a new owner
            if (changeowner && m_ownership && !players.empty())
            {
                ObjectGuid newowner = players.begin()->second.player;
                players[newowner].SetModerator(true);
                SetOwner(newowner);
            }
        }
    }
}

void Channel::KickOrBan(Player const* player, const char *badname, bool ban)
{
    ObjectGuid const& good = player->GetGUID();

    AccountTypes sec = player->GetSession()->GetSecurity();

    if (!IsOn(good))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotMember(notify);
        player->SendDirectMessage(notify.Write());
    }
    else if (!players[good].IsModerator() && !AccountMgr::IsModeratorAccount(sec))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotModerator(notify);
        player->SendDirectMessage(notify.Write());
    }
    else
    {
        Player* bad = sObjectAccessor->FindPlayerByName(badname);
        ObjectGuid victim = bad ? bad->GetGUID() : ObjectGuid::Empty;
        if (bad == NULL || !IsOn(bad->GetGUID()))
        {
            WorldPackets::Channel::ChannelNotify notify;
            MakePlayerNotFound(notify, badname);
            player->SendDirectMessage(notify.Write());
        }
        else if (!AccountMgr::IsModeratorAccount(sec) && bad->GetGUID() == m_ownerGUID && good != m_ownerGUID)
        {
            WorldPackets::Channel::ChannelNotify notify;
            MakeNotOwner(notify);
            player->SendDirectMessage(notify.Write());
        }
        else
        {
            bool changeowner = (m_ownerGUID == bad->GetGUID());

            WorldPacket data;
            bool notify = !(AccountMgr::IsModeratorAccount(sec) && sWorld->getBoolConfig(CONFIG_SILENTLY_GM_JOIN_TO_CHANNEL));

            if (ban && !IsBanned(bad->GetGUID()))
            {
                banned.insert(bad->GetGUID());
                UpdateChannelInDB();

                WorldPackets::Channel::ChannelNotify notify;
                MakePlayerBanned(notify, victim, good);
                SendToAll(notify.Write());
            }
            else if (notify)
            {
                WorldPackets::Channel::ChannelNotify notify;
                MakePlayerKicked(notify, victim, good);
                SendToAll(notify.Write());
            }

            players.erase(bad->GetGUID());
            bad->LeftChannel(this);

            if (changeowner && m_ownership && !players.empty())
            {
                ObjectGuid newowner = good;
                players[newowner].SetModerator(true);
                SetOwner(newowner);
            }
        }
    }
}

void Channel::UnBan(Player const* player, const char *badname)
{
    ObjectGuid const& good = player->GetGUID();
    uint32 sec = player->GetSession()->GetSecurity();

    if (!IsOn(good))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotMember(notify);
        player->SendDirectMessage(notify.Write());
    }
    else if (!players[good].IsModerator() && !AccountMgr::IsModeratorAccount(sec))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotModerator(notify);
        player->SendDirectMessage(notify.Write());
    }
    else
    {
        Player* bad = sObjectAccessor->FindPlayerByName(badname);
        if (bad == NULL || !IsBanned(bad->GetGUID()))
        {
            WorldPackets::Channel::ChannelNotify notify;
            MakePlayerNotFound(notify, badname);
            player->SendDirectMessage(notify.Write());
        }
        else
        {
            banned.erase(bad->GetGUID());

            WorldPackets::Channel::ChannelNotify notify;
            MakePlayerUnbanned(notify, bad->GetGUID(), good);
            SendToAll(notify.Write());

            UpdateChannelInDB();
        }
    }
}

void Channel::Password(Player const* player, const char *pass)
{
    uint32 sec = player->GetSession()->GetSecurity();
    ObjectGuid const& guid = player->GetGUID();

    if (!IsOn(guid))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotMember(notify);
        player->SendDirectMessage(notify.Write());
    }
    else if (!players[guid].IsModerator() && !AccountMgr::IsModeratorAccount(sec))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotModerator(notify);
        player->SendDirectMessage(notify.Write());
    }
    else if (IsWorld())
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotModerator(notify);
        player->SendDirectMessage(notify.Write());
    }
    else
    {
        m_password = pass;

        WorldPackets::Channel::ChannelNotify notify;
        MakePasswordChanged(notify, guid);
        SendToAll(notify.Write());

        UpdateChannelInDB();
    }
}

void Channel::SetMode(Player const* player, const char *p2n, bool mod, bool set)
{
    ObjectGuid const& guid = player->GetGUID();
    uint32 sec = player->GetSession()->GetSecurity();

    if (!IsOn(guid))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotMember(notify);
        player->SendDirectMessage(notify.Write());
    }
    else if (!players[guid].IsModerator() && !AccountMgr::IsModeratorAccount(sec))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotModerator(notify);
        player->SendDirectMessage(notify.Write());
    }
    else
    {
        Player* newp = sObjectAccessor->FindPlayerByName(p2n);
        if (!newp)
        {
            WorldPackets::Channel::ChannelNotify notify;
            MakePlayerNotFound(notify, p2n);
            player->SendDirectMessage(notify.Write());
            return;
        }

        if (guid == m_ownerGUID && newp->GetGUID() == m_ownerGUID && mod)
            return;

        if (!IsOn(newp->GetGUID()))
        {
            WorldPackets::Channel::ChannelNotify notify;
            MakePlayerNotFound(notify, p2n);
            player->SendDirectMessage(notify.Write());
            return;
        }

        // allow make moderator from another team only if both is GMs
        // at this moment this only way to show channel post for GM from another team
        if ((!AccountMgr::IsModeratorAccount(player->GetSession()->GetSecurity()) || !AccountMgr::IsModeratorAccount(newp->GetSession()->GetSecurity())) &&
            player->GetTeam() != newp->GetTeam() && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHANNEL))
        {
            WorldPackets::Channel::ChannelNotify notify;
            MakePlayerNotFound(notify, p2n);
            player->SendDirectMessage(notify.Write());
            return;
        }

        if (m_ownerGUID == newp->GetGUID() && m_ownerGUID != guid)
        {
            WorldPackets::Channel::ChannelNotify notify;
            MakeNotOwner(notify);
            player->SendDirectMessage(notify.Write());
            return;
        }

        if (mod)
            SetModerator(newp->GetGUID(), set);
        else
            SetMute(newp->GetGUID(), set);
    }
}

void Channel::SetOwner(Player const* player, const char *newname)
{
    ObjectGuid const& guid = player->GetGUID();
    uint32 sec = player->GetSession()->GetSecurity();

    if (!IsOn(guid))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotMember(notify);
        player->SendDirectMessage(notify.Write());
        return;
    }

    if (!AccountMgr::IsModeratorAccount(sec) && guid != m_ownerGUID)
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotOwner(notify);
        player->SendDirectMessage(notify.Write());
        return;
    }

    Player* newp = sObjectAccessor->FindPlayerByName(newname);
    if (newp == NULL || !IsOn(newp->GetGUID()))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakePlayerNotFound(notify, newname);
        player->SendDirectMessage(notify.Write());
        return;
    }

    if (newp->GetTeam() != player->GetTeam() && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHANNEL))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakePlayerNotFound(notify, newname);
        player->SendDirectMessage(notify.Write());
        return;
    }

    players[newp->GetGUID()].SetModerator(true);
    SetOwner(newp->GetGUID());
}

void Channel::SendWhoOwner(Player const* player)
{
    WorldPackets::Channel::ChannelNotify notify;
    if (IsOn(player->GetGUID()))
        MakeChannelOwner(notify);
    else
        MakeNotMember(notify);
    player->SendDirectMessage(notify.Write());
}

void Channel::List(Player const* player)
{
    if (!IsOn(player->GetGUID()))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotMember(notify);
        player->SendDirectMessage(notify.Write());
        return;
    }

    WorldPackets::Channel::ChannelListResponse list;
    list._Display = true; /// always true?
    list._Channel = GetName();
    list._ChannelFlags = GetFlags();

    uint32 gmLevelInWhoList = sWorld->getIntConfig(CONFIG_GM_LEVEL_IN_WHO_LIST);

    list._Members.reserve(players.size());
    for (PlayerList::value_type const& i : players)
    {
        Player* member = ObjectAccessor::FindPlayer(i.first);

        // PLAYER can't see MODERATOR, GAME MASTER, ADMINISTRATOR characters
        // MODERATOR, GAME MASTER, ADMINISTRATOR can see all
        if (member &&
            (!AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()) || member->GetSession()->GetSecurity() <= AccountTypes(gmLevelInWhoList)) &&
            member->IsVisibleGloballyFor(player))
        {
            list._Members.emplace_back(i.second.player, GetVirtualRealmAddress(), i.second.flags);
        }
    }

    player->SendDirectMessage(list.Write());
}

void Channel::Announce(Player const* player)
{
    ObjectGuid const& guid = player->GetGUID();
    uint32 sec = player->GetSession()->GetSecurity();

    if (!IsOn(guid))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotMember(notify);
        player->SendDirectMessage(notify.Write());
    }
    else if (!players[guid].IsModerator() && !AccountMgr::IsModeratorAccount(sec))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotModerator(notify);
        player->SendDirectMessage(notify.Write());
    }
    else
    {
        m_announce = !m_announce;

        WorldPackets::Channel::ChannelNotify notify;
        if (m_announce)
            MakeAnnouncementsOn(notify, guid);
        else
            MakeAnnouncementsOff(notify, guid);
        SendToAll(notify.Write());

        UpdateChannelInDB();
    }
}

void Channel::Say(ObjectGuid const& guid, const char *what, uint32 lang)
{
    if (!what)
        return;
    Player* player = ObjectAccessor::FindPlayer(guid);
    if (player)
        lang = player->GetTeam() == HORDE ? LANG_ORCISH : LANG_COMMON;

    if (sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHANNEL))
        lang = LANG_UNIVERSAL;


    if (!IsOn(guid))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotMember(notify);
        SendToOne(notify.Write(), guid);
        return;
    }
    else if (players[guid].IsMuted())
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeMuted(notify);
        SendToOne(notify.Write(), guid);
        return;
    }

    WorldPackets::Chat::Chat packet;
    if (player)
        packet.Initalize(CHAT_MSG_CHANNEL, Language(lang), player, player, what, 0, m_name);
    else
    {
        packet.Initalize(CHAT_MSG_CHANNEL, Language(lang), nullptr, nullptr, what, 0, m_name);
        packet.SenderGUID = guid;
        packet.TargetGUID = guid;
    }

    SendToAll(packet.Write(), !players[guid].IsModerator() ? guid : ObjectGuid::Empty);

    /*else
    {
        Trinity::ChatData c;
        c.message = what;
        c.channelName = m_name;
        c.sourceGuid = p;
        c.targetGuid = p;
        c.chatTag = player ? player->GetChatTag() : 0;
        c.language = lang;
        c.chatType = CHAT_MSG_CHANNEL;
        c.realmId = realm.Id.Realm;

        WorldPacket data;
        Trinity::BuildChatPacket(data, c);

        SendToAll(&data, !players[p].IsModerator() ? p : ObjectGuid::Empty);
    }*/
}

void Channel::Invite(Player const* player, const char *newname)
{
    ObjectGuid const& guid = player->GetGUID();

    if (!IsOn(guid))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeNotMember(notify);
        player->SendDirectMessage(notify.Write());
        return;
    }

    Player* newp = sObjectAccessor->FindPlayerByName(newname);
    if (!newp || !newp->isGMVisible())
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakePlayerNotFound(notify, newname);
        player->SendDirectMessage(notify.Write());
        return;
    }

    if (IsBanned(newp->GetGUID()))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakePlayerInviteBanned(notify, newname);
        player->SendDirectMessage(notify.Write());
        return;
    }

    if (newp->GetTeam() != player->GetTeam() && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHANNEL))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeInviteWrongFaction(notify);
        player->SendDirectMessage(notify.Write());
        return;
    }

    if (IsOn(newp->GetGUID()))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakePlayerAlreadyMember(notify, newp->GetGUID());
        player->SendDirectMessage(notify.Write());
        return;
    }

    if (!newp->GetSocial()->HasIgnore(guid))
    {
        WorldPackets::Channel::ChannelNotify notify;
        MakeInvite(notify, guid);
        newp->SendDirectMessage(notify.Write());
    }

    WorldPackets::Channel::ChannelNotify notify;
    MakePlayerInvited(notify, newp->GetName());
    player->SendDirectMessage(notify.Write());
}

void Channel::SetOwner(ObjectGuid const& guid, bool exclaim)
{
    if (!m_ownerGUID.IsEmpty())
    {
        // [] will re-add player after it possible removed
        PlayerList::iterator p_itr = players.find(m_ownerGUID);
        if (p_itr != players.end())
            p_itr->second.SetOwner(false);
    }

    m_ownerGUID = guid;
    if (!m_ownerGUID.IsEmpty())
    {
        uint8 oldFlag = GetPlayerFlags(m_ownerGUID);
        players[m_ownerGUID].SetModerator(true);
        players[m_ownerGUID].SetOwner(true);


        WorldPackets::Channel::ChannelNotify notify;
        MakeModeChange(notify, m_ownerGUID, oldFlag);
        SendToAll(notify.Write());

        if (exclaim)
        {
            MakeOwnerChanged(notify, m_ownerGUID);
            SendToAll(notify.Write());
        }

        UpdateChannelInDB();
    }
}

void Channel::SendToAll(WorldPacket const* data, ObjectGuid const& p)
{
    for (PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
    {
        Player* player = ObjectAccessor::FindPlayer(i->first);
        if (player)
        {
            if (p.IsEmpty() || !player->GetSocial()->HasIgnore(p))
                player->GetSession()->SendPacket(data);
        }
    }
}

void Channel::SendToAllButOne(WorldPacket const* data, ObjectGuid const& who)
{
    for (PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
    {
        if (i->first != who)
        {
            Player* player = ObjectAccessor::FindPlayer(i->first);
            if (player)
                player->GetSession()->SendPacket(data);
        }
    }
}

void Channel::SendToOne(WorldPacket const* data, ObjectGuid const& who)
{
    Player* player = ObjectAccessor::FindPlayer(who);
    if (player)
        player->GetSession()->SendPacket(data);
}

void Channel::Voice(ObjectGuid const& /*guid1*/, ObjectGuid const& /*guid2*/)
{

}

void Channel::DeVoice(ObjectGuid const& /*guid1*/, ObjectGuid const& /*guid2*/)
{

}

// done
void Channel::MakeNotifyPacket(WorldPackets::Channel::ChannelNotify& data, uint8 notifyType)
{
    data.Type = notifyType;
    data._Channel = m_name;
}

// done 0x00
void Channel::MakeJoined(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& guid)
{
    MakeNotifyPacket(data, CHAT_JOINED_NOTICE);
    data.SenderGuid = guid;
}

// done 0x01
void Channel::MakeLeft(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& guid)
{
    MakeNotifyPacket(data, CHAT_LEFT_NOTICE);
    data.SenderGuid = guid;
}

// done 0x02
void Channel::MakeYouJoined(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_YOU_JOINED_NOTICE);
    data.ChatChannelID = GetChannelId();
    /**data << uint8(GetFlags());
    *data << uint32(GetChannelId());
    *data << uint32(0);*/
}

// done 0x03
void Channel::MakeYouLeft(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_YOU_LEFT_NOTICE);
    data.ChatChannelID = GetChannelId();
    //*data << uint8(IsConstant());
}

// done 0x04
void Channel::MakeWrongPassword(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_WRONG_PASSWORD_NOTICE);
}

void Channel::MakeNotMember(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_NOT_MEMBER_NOTICE);
}

// done 0x06
void Channel::MakeNotModerator(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_NOT_MODERATOR_NOTICE);
}

// done 0x07
void Channel::MakePasswordChanged(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& guid)
{
    MakeNotifyPacket(data, CHAT_PASSWORD_CHANGED_NOTICE);
    data.SenderGuid = guid;
}

// done 0x08
void Channel::MakeOwnerChanged(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& guid)
{
    MakeNotifyPacket(data, CHAT_OWNER_CHANGED_NOTICE);
    data.SenderGuid = guid;
}

// done 0x09
void Channel::MakePlayerNotFound(WorldPackets::Channel::ChannelNotify& data, std::string const& name)
{
    MakeNotifyPacket(data, CHAT_PLAYER_NOT_FOUND_NOTICE);
    data.Sender = name;
}

// done 0x0A
void Channel::MakeNotOwner(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_NOT_OWNER_NOTICE);
}

// done 0x0B
void Channel::MakeChannelOwner(WorldPackets::Channel::ChannelNotify& data)
{
    std::string name = "";

    if (!ObjectMgr::GetPlayerNameByGUID(m_ownerGUID, name) || name.empty())
        name = "PLAYER_NOT_FOUND";

    MakeNotifyPacket(data, CHAT_CHANNEL_OWNER_NOTICE);
    data.Sender = ((IsConstant() || !m_ownerGUID) ? "Nobody" : name);
}

// done 0x0C
void Channel::MakeModeChange(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& guid, uint8 oldflags)
{
    MakeNotifyPacket(data, CHAT_MODE_CHANGE_NOTICE);
    data.SenderGuid = guid;
    data.OldFlags = oldflags;
    data.NewFlags = GetPlayerFlags(guid);
}

// done 0x0D
void Channel::MakeAnnouncementsOn(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& guid)
{
    MakeNotifyPacket(data, CHAT_ANNOUNCEMENTS_ON_NOTICE);
    data.SenderGuid = guid;
}

// done 0x0E
void Channel::MakeAnnouncementsOff(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& guid)
{
    MakeNotifyPacket(data, CHAT_ANNOUNCEMENTS_OFF_NOTICE);
    data.SenderGuid = guid;
}

// done 0x11
void Channel::MakeMuted(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_MUTED_NOTICE);
}

// done 0x12
void Channel::MakePlayerKicked(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& bad, ObjectGuid const& good)
{
    MakeNotifyPacket(data, CHAT_PLAYER_KICKED_NOTICE);
    data.SenderGuid = good;
    data.TargetGuid = bad;
}

// done 0x13
void Channel::MakeBanned(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_BANNED_NOTICE);
}

// done 0x14
void Channel::MakePlayerBanned(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& bad, ObjectGuid const& good)
{
    MakeNotifyPacket(data, CHAT_PLAYER_BANNED_NOTICE);
    data.SenderGuid = good;
    data.TargetGuid = bad;
}

// done 0x15
void Channel::MakePlayerUnbanned(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& bad, ObjectGuid const& good)
{
    MakeNotifyPacket(data, CHAT_PLAYER_UNBANNED_NOTICE);
    data.SenderGuid = good;
    data.TargetGuid = bad;
}

// done 0x16
void Channel::MakePlayerNotBanned(WorldPackets::Channel::ChannelNotify& data, std::string const& name)
{
    MakeNotifyPacket(data, CHAT_PLAYER_NOT_BANNED_NOTICE);
    data.Sender = name;
}

// done 0x17
void Channel::MakePlayerAlreadyMember(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& guid)
{
    MakeNotifyPacket(data, CHAT_PLAYER_ALREADY_MEMBER_NOTICE);
    data.SenderGuid = guid;
}

// done 0x18
void Channel::MakeInvite(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& guid)
{
    MakeNotifyPacket(data, CHAT_INVITE_NOTICE);
    data.SenderGuid = guid;
}

// done 0x19
void Channel::MakeInviteWrongFaction(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_INVITE_WRONG_FACTION_NOTICE);
}

// done 0x1A
void Channel::MakeWrongFaction(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_WRONG_FACTION_NOTICE);
}

// done 0x1B
void Channel::MakeInvalidName(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_INVALID_NAME_NOTICE);
}

// done 0x1C
void Channel::MakeNotModerated(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_NOT_MODERATED_NOTICE);
}

// done 0x1D
void Channel::MakePlayerInvited(WorldPackets::Channel::ChannelNotify& data, std::string const& name)
{
    MakeNotifyPacket(data, CHAT_PLAYER_INVITED_NOTICE);
    data.Sender = name;
}

// done 0x1E
void Channel::MakePlayerInviteBanned(WorldPackets::Channel::ChannelNotify& data, std::string const& name)
{
    MakeNotifyPacket(data, CHAT_PLAYER_INVITE_BANNED_NOTICE);
    data.Sender = name;
}

// done 0x1F
void Channel::MakeThrottled(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_THROTTLED_NOTICE);
}

// done 0x20
void Channel::MakeNotInArea(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_NOT_IN_AREA_NOTICE);
}

// done 0x21
void Channel::MakeNotInLfg(WorldPackets::Channel::ChannelNotify& data)
{
    MakeNotifyPacket(data, CHAT_NOT_IN_LFG_NOTICE);
}

// done 0x22
void Channel::MakeVoiceOn(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& guid)
{
    MakeNotifyPacket(data, CHAT_VOICE_ON_NOTICE);
    data.SenderGuid = guid;
}

// done 0x23
void Channel::MakeVoiceOff(WorldPackets::Channel::ChannelNotify& data, ObjectGuid const& guid)
{
    MakeNotifyPacket(data, CHAT_VOICE_OFF_NOTICE);
    data.SenderGuid = guid;
}

void Channel::JoinNotify(ObjectGuid const& guid)
{
    if (IsConstant())
    {
        WorldPackets::Channel::UserlistAdd userlistAdd;
        userlistAdd.AddedUserGUID = guid;
        userlistAdd._ChannelFlags = GetFlags();
        userlistAdd.UserFlags = GetPlayerFlags(guid);
        userlistAdd.ChannelID = GetChannelId();
        userlistAdd.ChannelName = GetName();
        SendToAllButOne(userlistAdd.Write(), guid);
    }
    else
    {
        WorldPackets::Channel::UserlistUpdate userlistUpdate;
        userlistUpdate.UpdatedUserGUID = guid;
        userlistUpdate._ChannelFlags = GetFlags();
        userlistUpdate.UserFlags = GetPlayerFlags(guid);
        userlistUpdate.ChannelID = GetChannelId();
        userlistUpdate.ChannelName = GetName();
        SendToAll(userlistUpdate.Write());
    }
}

void Channel::LeaveNotify(ObjectGuid const& guid)
{
    WorldPackets::Channel::UserlistRemove userlistRemove;
    userlistRemove.RemovedUserGUID = guid;
    userlistRemove._ChannelFlags = GetFlags();
    userlistRemove.ChannelID = GetChannelId();
    userlistRemove.ChannelName = GetName();

    if (IsConstant())
        SendToAllButOne(userlistRemove.Write(), guid);
    else
        SendToAll(userlistRemove.Write());
}

void Channel::SetModerator(ObjectGuid const& guid, bool set)
{
    if (players[guid].IsModerator() != set)
    {
        uint8 oldFlag = GetPlayerFlags(guid);
        players[guid].SetModerator(set);

        WorldPackets::Channel::ChannelNotify data;
        MakeModeChange(data, guid, oldFlag);
        SendToAll(data.Write());
    }
}

void Channel::SetMute(ObjectGuid const& guid, bool set)
{
    if (players[guid].IsMuted() != set)
    {
        uint8 oldFlag = GetPlayerFlags(guid);
        players[guid].SetMuted(set);

        WorldPackets::Channel::ChannelNotify data;
        MakeModeChange(data, guid, oldFlag);
        SendToAll(data.Write());
    }
}
