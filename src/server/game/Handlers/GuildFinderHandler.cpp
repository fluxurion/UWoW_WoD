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

#include "WorldSession.h"
#include "WorldPacket.h"
#include "Object.h"
#include "SharedDefines.h"
#include "GuildFinderMgr.h"
#include "GuildMgr.h"
#include "GuildPackets.h"

void WorldSession::HandleGuildFinderAddRecruit(WorldPacket& recvPacket)
{
    if (sGuildFinderMgr->GetAllMembershipRequestsForPlayer(GetPlayer()->GetGUID()).size() == 10)
        return;

    uint32 classRoles = 0;
    uint32 availability = 0;
    uint32 guildInterests = 0;

    recvPacket >> classRoles >> guildInterests >> availability;

    ObjectGuid guid;
    uint16 commentLength = recvPacket.ReadBits(11);

    uint8 bitOrder[8] = {7, 2, 6, 5, 0, 3, 4, 1};
    //recvPacket.ReadBitInOrder(guid, bitOrder);

    uint8 nameLength = recvPacket.ReadBits(7);
    
    //recvPacket.ReadByteSeq(guid[3]);
    //recvPacket.ReadByteSeq(guid[0]);
    //recvPacket.ReadByteSeq(guid[2]);
    std::string playerName = recvPacket.ReadString(nameLength);
    std::string comment = recvPacket.ReadString(commentLength);
    //recvPacket.ReadByteSeq(guid[4]);
    //recvPacket.ReadByteSeq(guid[7]);
    //recvPacket.ReadByteSeq(guid[1]);
    //recvPacket.ReadByteSeq(guid[5]);
    //recvPacket.ReadByteSeq(guid[6]);

    if (!guid.IsGuild())
        return;
    if (!(classRoles & GUILDFINDER_ALL_ROLES) || classRoles > GUILDFINDER_ALL_ROLES)
        return;
    if (!(availability & ALL_WEEK) || availability > ALL_WEEK)
        return;
    if (!(guildInterests & ALL_INTERESTS) || guildInterests > ALL_INTERESTS)
        return;

    MembershipRequest request = MembershipRequest(GetPlayer()->GetGUID(), guid, availability, classRoles, guildInterests, comment, time(NULL));
    sGuildFinderMgr->AddMembershipRequest(guid, request);
}

void WorldSession::HandleGuildFinderBrowse(WorldPacket& recvPacket)
{
    uint32 classRoles = 0;
    uint32 availability = 0;
    uint32 guildInterests = 0;
    uint32 playerLevel = 0; // Raw player level (1-90), do they use MAX_FINDER_LEVEL when on level 90 ?

    recvPacket >> guildInterests >> availability >> classRoles >> playerLevel;
    
    if (!(classRoles & GUILDFINDER_ALL_ROLES) || classRoles > GUILDFINDER_ALL_ROLES)
        return;
    if (!(availability & ALL_WEEK) || availability > ALL_WEEK)
        return;
    if (!(guildInterests & ALL_INTERESTS) || guildInterests > ALL_INTERESTS)
        return;
    if (playerLevel > sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL) || playerLevel < 1)
        return;

    Player* player = GetPlayer();

    LFGuildPlayer settings(player->GetGUID(), classRoles, availability, guildInterests, ANY_FINDER_LEVEL);
    LFGuildStore guildList = sGuildFinderMgr->GetGuildsMatchingSetting(settings, player->GetTeamId());

    if (!guildList.size())
    {
        player->SendDirectMessage(WorldPackets::Guild::LFGuildBrowse().Write());
        return;
    }

    WorldPackets::Guild::LFGuildBrowse browse;
    browse.Browses.reserve(guildList.size());
    for (auto const& x : guildList)
    {
        Guild* guild = sGuildMgr->GetGuildById(x.first.GetCounter());
        WorldPackets::Guild::LFGuildBrowseData data;
        data.GuildGUID = guild->GetGUID();
        data.GuildVirtualRealm = GetVirtualRealmAddress();
        data.GuildMembers = guild->GetMembersCount();
        data.GuildAchievementPoints = 0;
        data.PlayStyle = x.second.GetPlayStyle();
        data.Availability = x.second.GetAvailability();
        data.ClassRoles = x.second.GetClassRoles();
        data.LevelRange = guild->GetLevel();
        data.EmblemStyle = guild->GetEmblemInfo().GetStyle();
        data.EmblemColor = guild->GetEmblemInfo().GetColor();
        data.BorderStyle = guild->GetEmblemInfo().GetBorderStyle();
        data.BorderColor = guild->GetEmblemInfo().GetBorderColor();
        data.Background = guild->GetEmblemInfo().GetBackgroundColor();
        data.GuildName = guild->GetName();
        data.Comment = x.second.GetComment();
        data.Cached = 0;
        data.MembershipRequested = sGuildFinderMgr->HasRequest(player->GetGUID(), guild->GetGUID());
        browse.Browses.push_back(data);
    }

    player->SendDirectMessage(browse.Write());
}

void WorldSession::HandleGuildFinderDeclineRecruit(WorldPacket& recvPacket)
{
    ObjectGuid playerGuid;

    uint8 bitOrder[8] = {5, 1, 3, 7, 6, 2, 0, 4};
    //recvPacket.ReadBitInOrder(playerGuid, bitOrder);

    uint8 byteOrder[8] = {4, 2, 3, 7, 6, 0, 1, 5};
    //recvPacket.ReadBytesSeq(playerGuid, byteOrder);

    if (!playerGuid.IsPlayer())
        return;

    sGuildFinderMgr->RemoveMembershipRequest(playerGuid, ObjectGuid::Create<HighGuid::Guild>(GetPlayer()->GetGuildId()));
}

void WorldSession::HandleGuildFinderGetApplications(WorldPacket& /*recvPacket*/)
{
    std::list<MembershipRequest> applicatedGuilds = sGuildFinderMgr->GetAllMembershipRequestsForPlayer(GetPlayer()->GetGUID());
    WorldPackets::Guild::LFGuildApplication application;
    application.NumRemaining = 10 - sGuildFinderMgr->CountRequestsFromPlayer(GetPlayer()->GetGUID());

    if (!applicatedGuilds.empty())
    {
        application.Applications.reserve(applicatedGuilds.size());
        for (auto const& v : applicatedGuilds)
        {
            Guild* guild = sGuildMgr->GetGuildById(v.GetGuildGuid().GetCounter());
            if (!guild)
                continue;

            LFGuildSettings guildSettings = sGuildFinderMgr->GetGuildSettings(v.GetGuildGuid());
            WorldPackets::Guild::LFGuildApplicationData data;
            data.GuildGUID = guild->GetGUID();
            data.GuildVirtualRealm = GetVirtualRealmAddress();
            data.ClassRoles = guildSettings.GetClassRoles();
            data.PlayStyle = guildSettings.GetPlayStyle();
            data.Availability = guildSettings.GetAvailability();
            data.SecondsSinceCreated = time(nullptr) - v.GetSubmitTime();
            data.GuildName = guild->GetName();
            data.Comment = v.GetComment();
            application.Applications.push_back(data);
        }
    }

    GetPlayer()->SendDirectMessage(application.Write());
}

void WorldSession::HandleGuildFinderGetRecruits(WorldPacket& /*recvPacket*/)
{
    Player* player = GetPlayer();
    if (!player->GetGuildId())
        return;

    std::vector<MembershipRequest> recruitsList = sGuildFinderMgr->GetAllMembershipRequestsForGuild(ObjectGuid::Create<HighGuid::Guild>(player->GetGuildId()));
    
    WorldPackets::Guild::LFGuildRecruits recruits;
    recruits.Recruits.reserve(recruitsList.size());
    for (auto const& x : recruitsList)
    {
        WorldPackets::Guild::LFGuildRecruitData data;
        data.RecruitGUID = x.GetPlayerGUID();
        data.RecruitVirtualRealm = GetVirtualRealmAddress();
        data.CharacterClass = x.GetClass();
        data.CharacterGender = 0;
        data.CharacterLevel = x.GetLevel();
        data.ClassRoles = x.GetClassRoles();
        data.PlayStyle = x.GetPlayStyle();
        data.Availability = x.GetAvailability();
        data.SecondsSinceCreated = time(nullptr) - x.GetSubmitTime();
        data.SecondsUntilExpiration = x.GetExpiryTime() - time(nullptr);
        data.Name = x.GetName();
        data.Comment = x.GetComment();
        recruits.Recruits.push_back(data);
    }

    player->SendDirectMessage(recruits.Write());
}

void WorldSession::HandleGuildFinderPostRequest(WorldPacket& recvPacket)
{
    Player* player = GetPlayer();

    if (!player->GetGuildId()) // Player must be in guild
        return;

    bool isGuildMaster = true;
    if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId()))
        if (guild->GetLeaderGUID() != player->GetGUID())
            isGuildMaster = false;

    WorldPackets::Guild::LFGuildPost post;
    if (isGuildMaster)
    {
        LFGuildSettings settings = sGuildFinderMgr->GetGuildSettings(ObjectGuid::Create<HighGuid::Guild>(player->GetGuildId()));

        post.Post = boost::in_place();
        post.Post->PlayStyle = settings.GetPlayStyle();
        post.Post->Availability = settings.GetAvailability();
        post.Post->ClassRoles = settings.GetClassRoles();
        post.Post->LevelRange = settings.GetLevel();
        post.Post->SecondsRemaining = 0;
        post.Post->Comment = settings.GetComment();
        post.Post->Active = settings.IsListed();
    }

    player->SendDirectMessage(post.Write());
}

void WorldSession::HandleGuildFinderRemoveRecruit(WorldPacket& recvPacket)
{
    ObjectGuid guildGuid;

    uint8 bitOrder[8] = {3, 7, 4, 2, 6, 1, 0, 5};
    //recvPacket.ReadBitInOrder(guildGuid, bitOrder);

    uint8 byteOrder[8] = {6, 2, 7, 1, 5, 4, 0, 3};
    //recvPacket.ReadBytesSeq(guildGuid, byteOrder);

    if (!guildGuid.IsGuild())
        return;

    sGuildFinderMgr->RemoveMembershipRequest(GetPlayer()->GetGUID(), guildGuid);
}

void WorldSession::HandleLFGuildSetGuildPost(WorldPackets::Guild::LFGuildSetGuildPost& packet)
{
    // Level sent is zero if untouched, force to any (from interface). Idk why
    if (!packet.LevelRange)
        packet.LevelRange = ANY_FINDER_LEVEL;

    if (!(packet.ClassRoles & GUILDFINDER_ALL_ROLES) || packet.ClassRoles > GUILDFINDER_ALL_ROLES)
        return;

    if (!(packet.Availability & ALL_WEEK) || packet.Availability > ALL_WEEK)
        return;

    if (!(packet.PlayStyle & ALL_INTERESTS) || packet.PlayStyle > ALL_INTERESTS)
        return;

    if (!(packet.LevelRange & ALL_GUILDFINDER_LEVELS) || packet.LevelRange > ALL_GUILDFINDER_LEVELS)
        return;

    Player* player = GetPlayer();
    if (!player->GetGuildId()) // Player must be in guild
        return;

    if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId())) // Player must be guild master
        if (guild->GetLeaderGUID() != player->GetGUID())
            return;

    ObjectGuid guildGuid = ObjectGuid::Create<HighGuid::Guild>(player->GetGuildId());
    LFGuildSettings settings(packet.Active, player->GetTeamId(), guildGuid, packet.ClassRoles, packet.Availability, packet.PlayStyle, packet.LevelRange, packet.Comment);
    sGuildFinderMgr->SetGuildSettings(guildGuid, settings);
}
