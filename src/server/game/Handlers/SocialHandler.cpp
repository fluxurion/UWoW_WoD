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
#include "SocialMgr.h"
#include "Opcodes.h"
#include "SocialPackets.h"
#include "Player.h"

void WorldSession::HandleContactListOpcode(WorldPackets::Social::SendContactList& packet)
{
    Player* player = GetPlayer();
    if (player)
        player->GetSocial()->SendSocialList(player, packet.Flags);
}

void WorldSession::HandleAddFriend(WorldPackets::Social::AddFriend& packet)
{
    time_t now = time(nullptr);
    if (now - timeAddIgnoreOpcode < 3)
        return;
    else
       timeAddIgnoreOpcode = now;

    if (!normalizePlayerName(packet.Name))
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_RACE_ACC_BY_NAME);
    stmt->setString(0, packet.Name.c_str());

    _addFriendCallback.SetParam(std::move(packet.Notes));
    _addFriendCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::HandleDelFriendOpcode(WorldPackets::Social::DelFriend& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    player->GetSocial()->RemoveFromSocialList(packet.Player.Guid, SOCIAL_FLAG_FRIEND);
    sSocialMgr->SendFriendStatus(player, FRIEND_REMOVED, packet.Player.Guid, false);
}

void WorldSession::HandleAddIgnore(WorldPackets::Social::AddIgnore& packet)
{
    time_t now = time(nullptr);
    if (now - timeAddIgnoreOpcode < 3)
        return;
    else
       timeAddIgnoreOpcode = now;

    if (!normalizePlayerName(packet.Name))
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);
    stmt->setString(0, packet.Name);
    _addIgnoreCallback = CharacterDatabase.AsyncQuery(stmt);
}

void WorldSession::HandleAddIgnoreOpcodeCallBack(PreparedQueryResult result)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    ObjectGuid IgnoreGuid;
    FriendsResult ignoreResult;

    ignoreResult = FRIEND_IGNORE_NOT_FOUND;

    if (result)
    {
        IgnoreGuid = ObjectGuid::Create<HighGuid::Player>((*result)[0].GetUInt32());

        if (IgnoreGuid)
        {
            if (IgnoreGuid == player->GetGUID())
                ignoreResult = FRIEND_IGNORE_SELF;
            else if (player->GetSocial()->HasIgnore(IgnoreGuid))
                ignoreResult = FRIEND_IGNORE_ALREADY_S;
            else
            {
                ignoreResult = FRIEND_IGNORE_ADDED_S;

                if (!player->GetSocial()->AddToSocialList(IgnoreGuid, SOCIAL_FLAG_IGNORED))
                    ignoreResult = FRIEND_IGNORE_FULL;
            }
        }
    }

    sSocialMgr->SendFriendStatus(player, ignoreResult, IgnoreGuid, false);
}

void WorldSession::HandleDelIgnoreOpcode(WorldPackets::Social::DelIgnore& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    player->GetSocial()->RemoveFromSocialList(packet.Player.Guid, SOCIAL_FLAG_IGNORED);
    sSocialMgr->SendFriendStatus(player, FRIEND_IGNORE_REMOVED, packet.Player.Guid, false);
}

void WorldSession::HandleSetContactNotesOpcode(WorldPackets::Social::SetContactNotes& packet)
{
    _player->GetSocial()->SetFriendNote(packet.Player.Guid, packet.Notes);
}

void WorldSession::HandleVoiceDelIgnore(WorldPackets::Social::VoiceDelIgnore& /*packet*/) { }

void WorldSession::HandleVoiceAddIgnore(WorldPackets::Social::VoiceAddIgnore& /*packet*/) { }
