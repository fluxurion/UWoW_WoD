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

#include "ObjectMgr.h"                                      // for normalizePlayerName
#include "ChannelMgr.h"
#include "WordFilterMgr.h"
#include "ChannelPackets.h"

void WorldSession::HandleJoinChannel(WorldPackets::Channel::JoinChannel& packet)
{
    if (packet.ChatChannelId)
    {
        ChatChannelsEntry const* channel = sChatChannelsStore.LookupEntry(packet.ChatChannelId);
        if (!channel)
            return;

        AreaTableEntry const* current_zone = GetAreaEntryByAreaID(_player->GetZoneId());
        if (!current_zone)
            return;

        if (!_player->CanJoinConstantChannelInZone(channel, current_zone))
            return;
    }

    if (packet.ChannelName.empty())
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
    {
        cMgr->team = _player->GetTeam();
        if (Channel* chn = cMgr->GetJoinChannel(packet.ChannelName, packet.ChatChannelId))
            chn->JoinChannel(_player, packet.Password.c_str());
    }
}

void WorldSession::HandleLeaveChannel(WorldPackets::Channel::LeaveChannel& packet)
{
    if (packet.ChannelName.empty())
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* chn = cMgr->GetChannel(packet.ChannelName, _player))
            chn->LeaveChannel(_player, true);
        cMgr->LeftChannel(packet.ChannelName);
    }
}

void WorldSession::HandleChannelList(WorldPacket& recvPacket)
{
    uint32 length = recvPacket.ReadBits(7);
    std::string channelname = recvPacket.ReadString(length);

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->List(_player);
}

void WorldSession::HandleChannelPassword(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastChannelPassCommand < 5)
    {
        recvPacket.rfinish();
        return;
    }
    else
        timeLastChannelPassCommand = now;

    uint32 nameLength = recvPacket.ReadBits(7);
    uint32 passLength = recvPacket.ReadBits(7);

    std::string channelname = recvPacket.ReadString(nameLength);
    std::string pass = recvPacket.ReadString(passLength);

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->Password(_player, pass.c_str());
}

void WorldSession::HandleChannelSetOwner(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastChannelSetownerCommand < 5)
    {
        recvPacket.rfinish();
        return;
    }
    else
       timeLastChannelSetownerCommand = now;

    uint32 channelLength = recvPacket.ReadBits(7);
    uint32 nameLength = recvPacket.ReadBits(9);

    std::string channelname = recvPacket.ReadString(channelLength);
    std::string newp = recvPacket.ReadString(nameLength);

    if (!normalizePlayerName(newp))
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->SetOwner(_player->GetGUID(), newp.c_str());
}

void WorldSession::HandleChannelOwner(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastChannelOwnerCommand < 5)
    {
        recvPacket.rfinish();
        return;
    }
    else
       timeLastChannelOwnerCommand = now;

    uint32 length = recvPacket.ReadBits(7);
    std::string channelname = recvPacket.ReadString(length);

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->SendWhoOwner(_player);
}

void WorldSession::HandleChannelModerator(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastChannelModerCommand < 5)
    {
        recvPacket.rfinish();
        return;
    }
    else
       timeLastChannelModerCommand = now;

    uint32 channelLength = recvPacket.ReadBits(7);
    uint32 nameLength = recvPacket.ReadBits(9);

    std::string channelname = recvPacket.ReadString(channelLength);
    std::string otp = recvPacket.ReadString(nameLength);

    if (!normalizePlayerName(otp))
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->SetModerator(_player, otp.c_str());
}

void WorldSession::HandleChannelUnmoderator(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastChannelUnmoderCommand < 5)
    {
        recvPacket.rfinish();
        return;
    }
    else
       timeLastChannelUnmoderCommand = now;

    uint32 channelLength = recvPacket.ReadBits(7);
    uint32 nameLength = recvPacket.ReadBits(9);

    std::string channelname = recvPacket.ReadString(channelLength);
    std::string otp = recvPacket.ReadString(nameLength);

    if (!normalizePlayerName(otp))
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->UnsetModerator(_player, otp.c_str());
}

void WorldSession::HandleChannelMute(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastChannelMuteCommand < 5)
    {
        recvPacket.rfinish();
        return;
    }
    else
        timeLastChannelMuteCommand = now;

    uint32 channelLength = recvPacket.ReadBits(7);
    uint32 nameLength = recvPacket.ReadBits(9);

    std::string channelname = recvPacket.ReadString(channelLength);
    std::string otp = recvPacket.ReadString(nameLength);

    if (!normalizePlayerName(otp))
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->SetMute(_player, otp.c_str());
}

void WorldSession::HandleChannelUnmute(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastChannelUnmuteCommand < 5)
    {
        recvPacket.rfinish();
        return;
    }
    else
       timeLastChannelUnmuteCommand = now;

    uint32 channelLength = recvPacket.ReadBits(7);
    uint32 nameLength = recvPacket.ReadBits(9);

    std::string channelname = recvPacket.ReadString(channelLength);
    std::string otp = recvPacket.ReadString(nameLength);

    if (!normalizePlayerName(otp))
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->UnsetMute(_player, otp.c_str());
}

void WorldSession::HandleChannelInvite(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastChannelInviteCommand < 5)
    {
        recvPacket.rfinish();
        return;
    }
    else
       timeLastChannelInviteCommand = now;

    uint32 channelLength = recvPacket.ReadBits(7);
    uint32 nameLength = recvPacket.ReadBits(9);

    std::string channelname = recvPacket.ReadString(channelLength);
    std::string otp = recvPacket.ReadString(nameLength);

    if (!normalizePlayerName(otp))
        return;

    // check msg to bad word
    if (sWorld->getBoolConfig(CONFIG_WORD_FILTER_ENABLE))
    {
        std::string badWord = sWordFilterMgr->FindBadWord(channelname, true);

        if (!badWord.empty())
            return;
    }

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->Invite(_player, otp.c_str());
}

void WorldSession::HandleChannelKick(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastChannelKickCommand < 5)
    {
        recvPacket.rfinish();
        return;
    }
    else
       timeLastChannelKickCommand = now;

    uint32 channelLength = recvPacket.ReadBits(7);
    uint32 nameLength = recvPacket.ReadBits(9);

    std::string channelname = recvPacket.ReadString(channelLength);
    std::string otp = recvPacket.ReadString(nameLength);

    if (!normalizePlayerName(otp))
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->Kick(_player, otp.c_str());
}

void WorldSession::HandleChannelBan(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastChannelBanCommand < 5)
    {
        recvPacket.rfinish();
        return;
    }
    else
       timeLastChannelBanCommand = now;

    uint32 channelLength, nameLength;
    std::string channelname, otp;

    channelLength = recvPacket.ReadBits(7);
    nameLength = recvPacket.ReadBits(9);
    
    channelname = recvPacket.ReadString(channelLength);
    otp = recvPacket.ReadString(nameLength);

    if (!normalizePlayerName(otp))
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->Ban(_player, otp.c_str());
}

void WorldSession::HandleChannelUnban(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastChannelUnbanCommand < 5)
    {
        recvPacket.rfinish();
        return;
    }
    else
       timeLastChannelUnbanCommand = now;

    uint32 channelLength = recvPacket.ReadBits(7);
    uint32 nameLength = recvPacket.ReadBits(9);

    std::string channelname = recvPacket.ReadString(channelLength);
    std::string otp = recvPacket.ReadString(nameLength);

    if (!normalizePlayerName(otp))
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->UnBan(_player, otp.c_str());
}

void WorldSession::HandleChannelAnnouncements(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastChannelAnnounceCommand < 5)
    {
        recvPacket.rfinish();
        return;
    }
    else
       timeLastChannelAnnounceCommand = now;

    uint32 length = recvPacket.ReadBits(7);
    std::string channelname = recvPacket.ReadString(length);

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->Announce(_player);
}

void WorldSession::HandleChannelDisplayListQuery(WorldPacket &recvPacket)
{
    // this should be OK because the 2 function _were_ the same
    HandleChannelList(recvPacket);
}

void WorldSession::HandleSetChannelWatch(WorldPacket& recvPacket)
{
    std::string channelname;
    recvPacket >> channelname;
    /*if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelName, _player))
            chn->JoinNotify(_player->GetGUID());*/
}

