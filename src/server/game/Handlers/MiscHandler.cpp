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
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Log.h"
#include "Player.h"
#include "GossipDef.h"
#include "World.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "WorldSession.h"
#include "BigNumber.h"
#include "SHA1.h"
#include "UpdateData.h"
#include "LootMgr.h"
#include "Chat.h"
#include "zlib.h"
#include "ObjectAccessor.h"
#include "Object.h"
#include "Battleground.h"
#include "OutdoorPvP.h"
#include "Pet.h"
#include "SocialMgr.h"
#include "CellImpl.h"
#include "AccountMgr.h"
#include "Vehicle.h"
#include "CreatureAI.h"
#include "DBCEnums.h"
#include "ScriptMgr.h"
#include "MapManager.h"
#include "InstanceScript.h"
#include "GameObjectAI.h"
#include "Group.h"
#include "AccountMgr.h"
#include "Spell.h"
#include "BattlegroundMgr.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Bracket.h"
#include "LFGMgr.h"
#include "ClientConfigPackets.h"
#include "MiscPackets.h"
#include "CharacterPackets.h"
#include "SpellPackets.h"
#include "WhoPackets.h"
#include "SocialPackets.h"
#include "ItemPackets.h"
#include "InstancePackets.h"
#include "ReputationPackets.h"

void WorldSession::HandleRepopRequest(WorldPackets::Misc::RepopRequest& packet)
{
    if (GetPlayer()->isAlive() || GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;

    if (GetPlayer()->HasAuraType(SPELL_AURA_PREVENT_RESURRECTION))
        return; // silently return, client should display the error by itself

    // the world update order is sessions, players, creatures
    // the netcode runs in parallel with all of these
    // creatures can kill players
    // so if the server is lagging enough the player can
    // release spirit after he's killed but before he is updated
    if (GetPlayer()->getDeathState() == JUST_DIED)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleRepopRequestOpcode: got request after player %s(%d) was killed and before he was updated", GetPlayer()->GetName(), GetPlayer()->GetGUID().GetCounter());
        GetPlayer()->KillPlayer();
    }

    //this is spirit release confirm?
    GetPlayer()->RemovePet(NULL);
    GetPlayer()->BuildPlayerRepop();
    GetPlayer()->RepopAtGraveyard();
}

void WorldSession::HandleWhoOpcode(WorldPackets::Who::WhoRequestPkt& whoRequest)
{
    time_t now = time(NULL);
    if (now - timeLastWhoCommand < 15)
        return;
    else timeLastWhoCommand = now;

    WorldPackets::Who::WhoRequest& request = whoRequest.Request;

    // zones count, client limit = 10 (2.0.10)
    // can't be received from real client or broken packet
    if (whoRequest.Areas.size() > 10)
        return;

    // user entered strings count, client limit=4 (checked on 2.0.10)
    // can't be received from real client or broken packet
    if (request.Words.size() > 4)
        return;

    std::vector<std::wstring> wWords;
    wWords.resize(request.Words.size());
    for (size_t i = 0; i < request.Words.size(); ++i)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSession::HandleWhoOpcode: Word: %s", request.Words[i].Word.c_str());

        // user entered string, it used as universal search pattern(guild+player name)?
        if (!Utf8toWStr(request.Words[i].Word, wWords[i]))
            continue;

        wstrToLower(wWords[i]);
    }

    std::wstring wPlayerName;
    std::wstring wGuildName;

    if (!(Utf8toWStr(request.Name, wPlayerName) && Utf8toWStr(request.Guild, wGuildName)))
        return;

    wstrToLower(wPlayerName);
    wstrToLower(wGuildName);

    // client send in case not set max level value 100 but Trinity supports 255 max level,
    // update it to show GMs with characters after 100 level
    if (whoRequest.Request.MaxLevel >= MAX_LEVEL)
        whoRequest.Request.MaxLevel = STRONG_MAX_LEVEL;

    uint32 team = _player->GetTeam();
    uint32 security = GetSecurity();
    bool allowTwoSideWhoList = sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_WHO_LIST);
    uint32 gmLevelInWhoList  = sWorld->getIntConfig(CONFIG_GM_LEVEL_IN_WHO_LIST);
    uint8 displaycount = 0;
    Player *target = NULL;

    WorldPackets::Who::WhoResponsePkt response;

    boost::shared_lock<boost::shared_mutex> lock(*HashMapHolder<Player>::GetLock());

    HashMapHolder<Player>::MapType const& m = sObjectAccessor->GetPlayers();
    for (HashMapHolder<Player>::MapType::const_iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        target = itr->second;
        if (AccountMgr::IsPlayerAccount(security))
        {
            // player can see member of other team only if CONFIG_ALLOW_TWO_SIDE_WHO_LIST
            if (target->GetTeam() != team && !allowTwoSideWhoList)
                continue;

            // player can see MODERATOR, GAME MASTER, ADMINISTRATOR only if CONFIG_GM_IN_WHO_LIST
            if ((target->GetSession()->GetSecurity() > AccountTypes(gmLevelInWhoList)))
                continue;
        }

        // do not process players which are not in world
        if (!target || !target->IsInWorld())
            continue;

        // check if target is globally visible for player
        if (!target->IsVisibleGloballyFor(_player))
            continue;

        // check if target's level is in level range
        uint8 lvl = target->getLevel();
        if (lvl < request.MinLevel || lvl > request.MaxLevel)
            continue;

        // check if class matches classmask
        if (request.ClassFilter >= 0 && !(request.ClassFilter & (1 << target->getClass())))
            continue;

        // check if race matches racemask
        if (request.RaceFilter >= 0 && !(request.RaceFilter & (1 << target->getRace())))
            continue;

        if (!whoRequest.Areas.empty())
        {
            if (std::find(whoRequest.Areas.begin(), whoRequest.Areas.end(), target->GetZoneId()) == whoRequest.Areas.end())
                continue;
        }

        std::wstring wTargetName;

        if (!Utf8toWStr(target->GetName(), wTargetName))
            continue;

        wstrToLower(wTargetName);

        if (!wPlayerName.empty() && wTargetName.find(wPlayerName) == std::wstring::npos)
            continue;

        std::string gname = target->GetGuildName();
        std::wstring wTargetGuildName;
        if (!Utf8toWStr(gname, wTargetGuildName))
            continue;
        wstrToLower(wTargetGuildName);

        if (!wGuildName.empty() && wTargetGuildName.find(wGuildName) == std::wstring::npos)
            continue;

        if (!wWords.empty())
        {
            std::string aName;
            if (AreaTableEntry const* areaEntry = GetAreaEntryByAreaID(target->GetZoneId()))
                aName = areaEntry->ZoneName;

            bool show = false;
            for (size_t i = 0; i < wWords.size(); ++i)
            {
                if (!wWords[i].empty())
                {
                    if (wTargetName.find(wWords[i]) != std::wstring::npos ||
                        wTargetGuildName.find(wWords[i]) != std::wstring::npos ||
                        Utf8FitTo(aName, wWords[i]))
                    {
                        show = true;
                        break;
                    }
                }
            }

            if (!show)
                continue;
        }

        WorldPackets::Who::WhoEntry whoEntry;
        if (!whoEntry.PlayerData.Initialize(target->GetGUID(), target))
            continue;

        if (Guild* targetGuild = target->GetGuild())
        {
            whoEntry.GuildGUID = targetGuild->GetGUID();
            whoEntry.GuildVirtualRealmAddress = GetVirtualRealmAddress();
            whoEntry.GuildName = targetGuild->GetName();
        }

        whoEntry.AreaID = target->GetZoneId();
        whoEntry.IsGM = target->isGameMaster();

        response.Response.Entries.push_back(whoEntry);

        // 49 is maximum player count sent to client - can be overridden
        // through config, but is unstable
        if ((displaycount++) >= sWorld->getIntConfig(CONFIG_MAX_WHO))
        {
            if (sWorld->getBoolConfig(CONFIG_LIMIT_WHO_ONLINE))
                break;
            else
                continue;

            break;
        }
    }

    SendPacket(response.Write());
}

void WorldSession::HandleLogoutRequestOpcode(WorldPacket& /*recvData*/)
{
    if (ObjectGuid lguid = GetPlayer()->GetLootGUID())
        DoLootRelease(lguid);
    GetPlayer()->ClearAoeLootList();

    uint32 reason = 0;

    if (GetPlayer()->isInCombat())
        reason = 1;
    else if (GetPlayer()->m_movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING | MOVEMENTFLAG_FALLING_FAR))
        reason = 3;                                         // is jumping or falling
    else if (GetPlayer()->duel || GetPlayer()->HasAura(9454)) // is dueling or frozen by GM via freeze command
        reason = 2;                                         // FIXME - Need the correct value

    //instant logout in taverns/cities or on taxi or for admins, gm's, mod's if its enabled in worldserver.conf
    bool instantLogout = GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_RESTING) || GetPlayer()->isInFlight() ||
        GetSecurity() >= AccountTypes(sWorld->getIntConfig(CONFIG_INSTANT_LOGOUT));

    WorldPackets::Character::LogoutResponse logoutResponse;
    logoutResponse.LogoutResult = reason;
    logoutResponse.Instant = instantLogout;
    SendPacket(logoutResponse.Write());

    // instant logout in taverns/cities or on taxi or for admins, gm's, mod's if its enabled in worldserver.conf
    if (instantLogout)
    {
        LogoutPlayer(true);
        return;
    }

    // not set flags if player can't free move to prevent lost state at logout cancel
    if (GetPlayer()->CanFreeMove())
    {
        GetPlayer()->SetStandState(UNIT_STAND_STATE_SIT);
        GetPlayer()->SetRooted(true);
        GetPlayer()->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
    }

    LogoutRequest(time(NULL));
}

void WorldSession::HandlePlayerLogoutOpcode(WorldPacket& recvData)
{
    bool bit = !recvData.ReadBit();
    if (bit)
        recvData >> Unused<uint32>();
}

void WorldSession::HandleLogoutCancelOpcode(WorldPacket& /*recvData*/)
{
    // Player have already logged out serverside, too late to cancel
    if (!GetPlayer())
        return;

    LogoutRequest(0);

    SendPacket(WorldPackets::Character::LogoutCancelAck().Write());

    // not remove flags if can't free move - its not set in Logout request code.
    if (GetPlayer()->CanFreeMove())
    {
        //!we can move again
        GetPlayer()->SetRooted(false);

        //! Stand Up
        GetPlayer()->SetStandState(UNIT_STAND_STATE_STAND);

        //! DISABLE_ROTATE
        GetPlayer()->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
    }

    GetPlayer()->PetSpellInitialize();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_LOGOUT_CANCEL_ACK Message");
}

void WorldSession::HandleTogglePvP(WorldPackets::Misc::TogglePvP& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    bool inPvP = player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP);

    player->ApplyModFlag(PLAYER_FIELD_PLAYER_FLAGS, inPvP ? PLAYER_FLAGS_PVP_TIMER : PLAYER_FLAGS_IN_PVP, inPvP);

    if (player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
    {
        if (!player->IsPvP() || player->pvpInfo.endTimer)
            player->UpdatePvP(true, true);
    }
    else
    {
        if (!player->pvpInfo.inHostileArea && player->IsPvP())
            player->pvpInfo.endTimer = time(nullptr);
    }
}

void WorldSession::HandleSetPvP(WorldPackets::Misc::SetPvP& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    player->ApplyModFlag(PLAYER_FIELD_PLAYER_FLAGS, packet.EnablePVP ? PLAYER_FLAGS_IN_PVP: PLAYER_FLAGS_PVP_TIMER, packet.EnablePVP);

    if (player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
    {
        if (!player->IsPvP() || player->pvpInfo.endTimer)
            player->UpdatePvP(true, true);
    }
    else
    {
        if (!player->pvpInfo.inHostileArea && player->IsPvP())
            player->pvpInfo.endTimer = time(nullptr);
    }
}

void WorldSession::HandlePortGraveyard(WorldPackets::Misc::PortGraveyard& /*packet*/)
{
    if (GetPlayer()->isAlive() || !GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;

    GetPlayer()->RepopAtGraveyard();
}

void WorldSession::HandleSetSelectionOpcode(WorldPackets::Misc::SetSelection& packet)
{
    _player->SetSelection(packet.Selection);
}

void WorldSession::HandleStandStateChangeOpcode(WorldPackets::Misc::StandStateChange& packet)
{
    // sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_STAND_STATE_CHANGE"); -- too many spam in log at lags/debug stop

    _player->SetStandState(packet.StandState);
}

void WorldSession::HandleContactListOpcode(WorldPackets::Social::SendContactList& packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SEND_CONTACT_LIST");
    _player->GetSocial()->SendSocialList(_player, packet.Flags);
}

void WorldSession::HandleAddFriend(WorldPackets::Social::AddFriend& packet)
{
    time_t now = time(NULL);
    if (now - timeAddIgnoreOpcode < 3)
        return;
    else
       timeAddIgnoreOpcode = now;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ADD_FRIEND");


    if (!normalizePlayerName(packet.Name))
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: %s asked to add friend : '%s'", GetPlayer()->GetName(), packet.Name.c_str());

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_RACE_ACC_BY_NAME);
    stmt->setString(0, packet.Name.c_str());

    _addFriendCallback.SetParam(std::move(packet.Notes));
    _addFriendCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::HandleAddFriendOpcodeCallBack(PreparedQueryResult result, std::string friendNote)
{
    if (!GetPlayer())
        return;

    ObjectGuid friendGuid;
    uint32 friendAccountId;
    uint32 team;
    FriendsResult friendResult;

    friendResult = FRIEND_NOT_FOUND;

    if (result)
    {
        Field* fields = result->Fetch();

        friendGuid = ObjectGuid::Create<HighGuid::Player>(fields[0].GetUInt32());
        team = Player::TeamForRace(fields[1].GetUInt8());
        friendAccountId = fields[2].GetUInt32();

        if (!AccountMgr::IsPlayerAccount(GetSecurity()) || sWorld->getBoolConfig(CONFIG_ALLOW_GM_FRIEND) || AccountMgr::IsPlayerAccount(AccountMgr::GetSecurity(friendAccountId, realmHandle.Index)))
        {
            if (friendGuid)
            {
                if (friendGuid == GetPlayer()->GetGUID())
                    friendResult = FRIEND_SELF;
                else if (GetPlayer()->GetTeam() != team && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_ADD_FRIEND) && AccountMgr::IsPlayerAccount(GetSecurity()))
                    friendResult = FRIEND_ENEMY;
                else if (GetPlayer()->GetSocial()->HasFriend(friendGuid))
                    friendResult = FRIEND_ALREADY;
                else
                {
                    Player* pFriend = ObjectAccessor::FindPlayer(friendGuid);
                    if (pFriend && pFriend->IsInWorld() && pFriend->IsVisibleGloballyFor(GetPlayer()))
                        friendResult = FRIEND_ADDED_ONLINE;
                    else
                        friendResult = FRIEND_ADDED_OFFLINE;
                    if (!GetPlayer()->GetSocial()->AddToSocialList(friendGuid, SOCIAL_FLAG_FRIEND))
                    {
                        friendResult = FRIEND_LIST_FULL;
                        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: %s's friend list is full.", GetPlayer()->GetName());
                    }
                }
                GetPlayer()->GetSocial()->SetFriendNote(friendGuid, friendNote);
            }
        }
    }

    sSocialMgr->SendFriendStatus(GetPlayer(), friendResult, friendGuid, false);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent (SMSG_FRIEND_STATUS)");
}

void WorldSession::HandleDelFriendOpcode(WorldPackets::Social::DelFriend& packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_DEL_FRIEND");
    _player->GetSocial()->RemoveFromSocialList(packet.Player.Guid, SOCIAL_FLAG_FRIEND);
    sSocialMgr->SendFriendStatus(GetPlayer(), FRIEND_REMOVED, packet.Player.Guid, false);
}

void WorldSession::HandleAddIgnore(WorldPackets::Social::AddIgnore& packet)
{
    time_t now = time(NULL);
    if (now - timeAddIgnoreOpcode < 3)
        return;
    else
       timeAddIgnoreOpcode = now;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ADD_IGNORE");

    if (!normalizePlayerName(packet.Name))
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: %s asked to Ignore: '%s'",
        GetPlayer()->GetName(), packet.Name.c_str());

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);

    stmt->setString(0, packet.Name);

    _addIgnoreCallback = CharacterDatabase.AsyncQuery(stmt);
}

void WorldSession::HandleAddIgnoreOpcodeCallBack(PreparedQueryResult result)
{
    if (!GetPlayer())
        return;

    ObjectGuid IgnoreGuid;
    FriendsResult ignoreResult;

    ignoreResult = FRIEND_IGNORE_NOT_FOUND;

    if (result)
    {
        IgnoreGuid = ObjectGuid::Create<HighGuid::Player>((*result)[0].GetUInt32());

        if (IgnoreGuid)
        {
            if (IgnoreGuid == GetPlayer()->GetGUID())              //not add yourself
                ignoreResult = FRIEND_IGNORE_SELF;
            else if (GetPlayer()->GetSocial()->HasIgnore(IgnoreGuid))
                ignoreResult = FRIEND_IGNORE_ALREADY_S;
            else
            {
                ignoreResult = FRIEND_IGNORE_ADDED_S;

                // ignore list full
                if (!GetPlayer()->GetSocial()->AddToSocialList(IgnoreGuid, SOCIAL_FLAG_IGNORED))
                    ignoreResult = FRIEND_IGNORE_FULL;
            }
        }
    }

    sSocialMgr->SendFriendStatus(GetPlayer(), ignoreResult, IgnoreGuid, false);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent (SMSG_FRIEND_STATUS)");
}

void WorldSession::HandleDelIgnoreOpcode(WorldPackets::Social::DelIgnore& packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_DEL_IGNORE");
    _player->GetSocial()->RemoveFromSocialList(packet.Player.Guid, SOCIAL_FLAG_IGNORED);
    sSocialMgr->SendFriendStatus(GetPlayer(), FRIEND_IGNORE_REMOVED, packet.Player.Guid, false);
}

void WorldSession::HandleSetContactNotesOpcode(WorldPackets::Social::SetContactNotes& packet)
{
    _player->GetSocial()->SetFriendNote(packet.Player.Guid, packet.Notes);
}

//! 5.4.1
void WorldSession::HandleBugOpcode(WorldPacket& recvData)
{
    uint32 contentlen, typelen;
    uint8 suggestion;   //bit
    std::string content, type;

    recvData >> suggestion >> contentlen;
    content = recvData.ReadString(contentlen);
    recvData >> typelen;
    type = recvData.ReadString(typelen);

    if (suggestion == 0)
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SUPPORT_TICKET_SUBMIT_BUG [Bug Report]");
    else
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SUPPORT_TICKET_SUBMIT_BUG [Suggestion]");

    sLog->outDebug(LOG_FILTER_NETWORKIO, "%s", type.c_str());
    sLog->outDebug(LOG_FILTER_NETWORKIO, "%s", content.c_str());

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_BUG_REPORT);

    stmt->setString(0, type);
    stmt->setString(1, content);

    CharacterDatabase.Execute(stmt);
}

//! 6.0.3
void WorldSession::HandleReclaimCorpse(WorldPackets::Misc::ReclaimCorpse& packet)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_RECLAIM_CORPSE");

    if (GetPlayer()->isAlive())
        return;

    // do not allow corpse reclaim in arena
    if (GetPlayer()->InArena())
        return;

    // body not released yet
    if (!GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;

    Corpse* corpse = GetPlayer()->GetCorpse();

    if (!corpse)
        return;

    // prevent resurrect before 30-sec delay after body release not finished
    if (time_t(corpse->GetGhostTime() + GetPlayer()->GetCorpseReclaimDelay(corpse->GetType() == CORPSE_RESURRECTABLE_PVP)) > time_t(time(NULL)))
        return;

    if (!corpse->IsWithinDist(GetPlayer(), CORPSE_RECLAIM_RADIUS, true) || !corpse->IsInMap(GetPlayer()))
        return;

    // resurrect
    GetPlayer()->ResurrectPlayer(GetPlayer()->InBattleground() ? 1.0f : 0.5f);

    // spawn bones
    GetPlayer()->SpawnCorpseBones();
}

//! 6.0.3
void WorldSession::HandleResurrectResponse(WorldPackets::Misc::ResurrectResponse& packet)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_RESURRECT_RESPONSE");

    if (GetPlayer()->isAlive())
        return;

    if (packet.Response != 0)
    {
        GetPlayer()->ClearResurrectRequestData();           // reject
        return;
    }

    if (!GetPlayer()->IsRessurectRequestedBy(packet.Resurrecter))
        return;

    GetPlayer()->ResurectUsingRequestData();
}

//! 6.0.3
void WorldSession::HandleAreaTrigger(WorldPackets::Misc::AreaTrigger& packet)
{
    Player* player = GetPlayer();
    if (player->isInFlight())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTrigger: Player '%s' (GUID: %u) in flight, ignore Area Trigger ID:%u",
            player->GetName(), player->GetGUID().GetCounter(), packet.AreaTriggerID);
        return;
    }

    AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(packet.AreaTriggerID);
    if (!atEntry)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTrigger: Player '%s' (GUID: %u) send unknown (by DBC) Area Trigger ID:%u",
            player->GetName(), player->GetGUID().GetCounter(), packet.AreaTriggerID);
        return;
    }

    if (player->GetMapId() != atEntry->mapid)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTrigger: Player '%s' (GUID: %u) too far (trigger map: %u player map: %u), ignore Area Trigger ID: %u",
            player->GetName(), atEntry->mapid, player->GetMapId(), player->GetGUID().GetCounter(), packet.AreaTriggerID);
        return;
    }

    // delta is safe radius
    const float delta = 5.0f;

    if (atEntry->radius > 0)
    {
        // if we have radius check it
        float dist = player->GetDistance(atEntry->x, atEntry->y, atEntry->z);
        if (dist > atEntry->radius + delta)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTrigger: Player '%s' (GUID: %u) too far (radius: %f distance: %f), ignore Area Trigger ID: %u",
                player->GetName(), player->GetGUID().GetCounter(), atEntry->radius, dist, packet.AreaTriggerID);
            return;
        }
    }
    else
    {
        // we have only extent

        // rotate the players position instead of rotating the whole cube, that way we can make a simplified
        // is-in-cube check and we have to calculate only one point instead of 4

        // 2PI = 360°, keep in mind that ingame orientation is counter-clockwise
        double rotation = 2 * M_PI - atEntry->box_orientation;
        double sinVal = std::sin(rotation);
        double cosVal = std::cos(rotation);

        float playerBoxDistX = player->GetPositionX() - atEntry->x;
        float playerBoxDistY = player->GetPositionY() - atEntry->y;

        float rotPlayerX = float(atEntry->x + playerBoxDistX * cosVal - playerBoxDistY*sinVal);
        float rotPlayerY = float(atEntry->y + playerBoxDistY * cosVal + playerBoxDistX*sinVal);

        // box edges are parallel to coordiante axis, so we can treat every dimension independently :D
        float dz = player->GetPositionZ() - atEntry->z;
        float dx = rotPlayerX - atEntry->x;
        float dy = rotPlayerY - atEntry->y;
        if ((fabs(dx) > atEntry->box_x / 2 + delta) ||
            (fabs(dy) > atEntry->box_y / 2 + delta) ||
            (fabs(dz) > atEntry->box_z / 2 + delta))
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTrigger: Player '%s' (GUID: %u) too far (1/2 box X: %f 1/2 box Y: %f 1/2 box Z: %f rotatedPlayerX: %f rotatedPlayerY: %f dZ:%f), ignore Area Trigger ID: %u",
                player->GetName(), player->GetGUID().GetCounter(), atEntry->box_x/2, atEntry->box_y/2, atEntry->box_z/2, rotPlayerX, rotPlayerY, dz, packet.AreaTriggerID);
            return;
        }
    }

    if (player->isDebugAreaTriggers)
        ChatHandler(player).PSendSysMessage(LANG_DEBUG_AREATRIGGER_REACHED, packet.AreaTriggerID);

    // set for future scrip using.
    player->SetLastAreaTrigger(atEntry);

    if (sScriptMgr->OnAreaTrigger(player, atEntry, packet.Entered))
        return;

    if (player->isAlive())
        if (uint32 questId = sObjectMgr->GetQuestForAreaTrigger(packet.AreaTriggerID))
            if (player->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                player->AreaExploredOrEventHappens(questId);

    if (sObjectMgr->IsTavernAreaTrigger(packet.AreaTriggerID))
    {
        // set resting flag we are in the inn
        player->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_RESTING);
        player->InnEnter(time(NULL), atEntry->mapid, atEntry->x, atEntry->y, atEntry->z);
        player->SetRestType(REST_TYPE_IN_TAVERN);

        if (sWorld->IsFFAPvPRealm())
            player->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);

        return;
    }

    if (Battleground* bg = player->GetBattleground())
        if (bg->GetStatus() == STATUS_IN_PROGRESS)
        {
            bg->HandleAreaTrigger(player, packet.AreaTriggerID);
            return;
        }

    if (OutdoorPvP* pvp = player->GetOutdoorPvP())
        if (pvp->HandleAreaTrigger(_player, packet.AreaTriggerID))
            return;

    AreaTriggerStruct const* at = sObjectMgr->GetAreaTrigger(packet.AreaTriggerID);
    if (!at)
        return;

    bool teleported = false;
    if (player->GetMapId() != at->target_mapId)
    {
        if (!sMapMgr->CanPlayerEnter(at->target_mapId, player, false))
            return;

        if (Group* group = player->GetGroup())
            if (group->isLFGGroup() && player->GetMap()->IsDungeon())
                teleported = player->TeleportToBGEntryPoint();

        MapEntry const* targetMap = sMapStore.LookupEntry(at->target_mapId);
        if (targetMap->IsDungeon() && player->GetGroup())
        {
            if (uint32 dungeonId = sLFGMgr->GetDungeon(player->GetGroup()->GetGUID(), true))
                if (lfg::LFGDungeonData const* dungeon = sLFGMgr->GetLFGDungeon(dungeonId))
                    if (dungeon->map == targetMap->MapID)
                        teleported = player->TeleportTo(dungeon->map, dungeon->x, dungeon->y, dungeon->z, dungeon->o, TELE_TO_NOT_LEAVE_TRANSPORT);
        }
    }

    if (!teleported)
        player->TeleportTo(at->target_mapId, at->target_X, at->target_Y, at->target_Z, at->target_Orientation, TELE_TO_NOT_LEAVE_TRANSPORT);
}

//! 6.0.3
void WorldSession::HandleUpdateAccountData(WorldPackets::ClientConfig::UserClientUpdateAccountData& packet)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_UPDATE_ACCOUNT_DATA");

    //sLog->outDebug(LOG_FILTER_NETWORKIO, "UAD: type %u, time %u, decompressedSize %u", type, timestamp, decompressedSize);

    if (packet.DataType > NUM_ACCOUNT_DATA_TYPES)
        return;

    if (packet.Size == 0)                               // erase
    {
        SetAccountData(AccountDataType(packet.DataType), 0, "");
        return;
    }

    if (packet.Size > 0xFFFF)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "UAD: Account data packet too big, size %u", packet.Size);
        return;
    }

    ByteBuffer dest;
    dest.resize(packet.Size);

    uLongf realSize = packet.Size;
    if (uncompress(dest.contents(), &realSize, packet.CompressedData.contents(), packet.CompressedData.size()) != Z_OK)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "UAD: Failed to decompress account data");
        return;
    }

    std::string adata;
    dest >> adata;

    SetAccountData(AccountDataType(packet.DataType), packet.Time, adata);
}

//! 6.0.3
void WorldSession::HandleRequestAccountData(WorldPackets::ClientConfig::RequestAccountData& request)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_REQUEST_ACCOUNT_DATA");

    if (request.DataType >= NUM_ACCOUNT_DATA_TYPES)
        return;

    AccountData const* adata = GetAccountData(AccountDataType(request.DataType));

    WorldPackets::ClientConfig::UpdateAccountData data;
    data.Player = _player ? _player->GetGUID() : ObjectGuid::Empty;
    data.Time = adata->Time;
    data.Size = adata->Data.size();
    data.DataType = request.DataType;

    uLongf destSize = compressBound(data.Size);

    data.CompressedData.resize(destSize);

    if (data.Size && compress(data.CompressedData.contents(), &destSize, (uint8 const*)adata->Data.c_str(), data.Size) != Z_OK)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "RAD: Failed to compress account data");
        return;
    }

    data.CompressedData.resize(destSize);

    SendPacket(data.Write());

}

void WorldSession::HandleSetActionButtonOpcode(WorldPackets::Spells::SetActionButton& packet)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SET_ACTION_BUTTON");

    uint32 action = uint32(packet.Action & 0xFFFFFFFF);
    uint8  type = uint8(packet.Action >> 56);

    sLog->outInfo(LOG_FILTER_NETWORKIO, "BUTTON: %u ACTION: %u TYPE: %u", packet.Index, action, type);
    if (!packet.Action)
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Remove action from button %u", packet.Index);
        GetPlayer()->removeActionButton(packet.Index);
    }
    else
    {
        switch (type)
        {
        case ACTION_BUTTON_MACRO:
        case ACTION_BUTTON_CMACRO:
            sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added Macro %u into button %u", action, packet.Index);
            break;
        case ACTION_BUTTON_EQSET:
            sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added EquipmentSetInfo %u into button %u", action, packet.Index);
            break;
        case ACTION_BUTTON_SPELL:
            sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added Spell %u into button %u", action, packet.Index);
            break;
        case ACTION_BUTTON_SUB_BUTTON:
            sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added sub buttons %u into button %u", action, packet.Index);
            break;
        case ACTION_BUTTON_ITEM:
            sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added Item %u into button %u", action, packet.Index);
            break;
        case ACTION_BUTTON_PET:
            sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added Pet Spell %u into button %u", action, packet.Index);
            break;
        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "MISC: Unknown action button type %u for action %u into button %u for player %s (GUID: %u)", type, action, packet.Index, _player->GetName(), _player->GetGUID().GetCounter());
            return;
        }
        GetPlayer()->addActionButton(packet.Index, action, type);
    }
}

void WorldSession::HandleCompleteCinematic(WorldPackets::Misc::CompleteCinematic& /*packet*/) { }

void WorldSession::HandleNextCinematicCamera(WorldPackets::Misc::NextCinematicCamera& /*packet*/) { }

void WorldSession::HandleCompleteMovie(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_COMPLETE_MOVIE");

    if (!_player)
        return;

    _player->setWatchinMovie(false);
    _player->SetCanDelayTeleport(false);
}

//! 6.0.3
void WorldSession::HandleSetActionBarToggles(WorldPacket& recvData)
{
    uint8 actionBar;

    recvData >> actionBar;

    if (!GetPlayer())                                        // ignore until not logged (check needed because STATUS_AUTHED)
    {
        if (actionBar != 0)
            sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetActionBarToggles in not logged state with value: %u, ignored", uint32(actionBar));
        return;
    }

    GetPlayer()->SetByteValue(PLAYER_FIELD_BYTES, PLAYER_FIELD_BYTES_OFFSET_ACTION_BAR_TOGGLES, actionBar);
}

//! 6.0.3
void WorldSession::HandlePlayedTime(WorldPacket& recvData)
{
    uint8 TriggerEvent;
    recvData >> TriggerEvent;                                      // 0 or 127 expected. not it's bit

    WorldPacket data(SMSG_PLAYED_TIME, 4 + 4 + 1);
    data << uint32(_player->GetTotalPlayedTime());
    data << uint32(_player->GetLevelPlayedTime());
    data << uint8(TriggerEvent);                                    // 0 - will not show in chat frame
    SendPacket(&data);
}

void WorldSession::HandleWhoisOpcode(WorldPackets::Who::WhoIsRequest& packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_WHO_IS");

    if (!AccountMgr::IsAdminAccount(GetSecurity()))
    {
        SendNotification(LANG_YOU_NOT_HAVE_PERMISSION);
        return;
    }

    if (packet.CharName.empty() || !normalizePlayerName (packet.CharName))
    {
        SendNotification(LANG_NEED_CHARACTER_NAME);
        return;
    }

    Player* player = sObjectAccessor->FindPlayerByName(packet.CharName.c_str());

    if (!player)
    {
        SendNotification(LANG_PLAYER_NOT_EXIST_OR_OFFLINE, packet.CharName.c_str());
        return;
    }

    uint32 accid = player->GetSession()->GetAccountId();

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_WHOIS);

    stmt->setUInt32(0, accid);

    PreparedQueryResult result = LoginDatabase.Query(stmt);

    if (!result)
    {
        SendNotification(LANG_ACCOUNT_FOR_PLAYER_NOT_FOUND, packet.CharName.c_str());
        return;
    }

    Field* fields = result->Fetch();
    std::string acc = fields[0].GetString();
    if (acc.empty())
        acc = "Unknown";
    std::string email = fields[1].GetString();
    if (email.empty())
        email = "Unknown";
    std::string lastip = fields[2].GetString();
    if (lastip.empty())
        lastip = "Unknown";

    WorldPackets::Who::WhoIsResponse response;
    response.AccountName = packet.CharName + "'s " + "account is " + acc + ", e-mail: " + email + ", last ip: " + lastip;
    SendPacket(response.Write());

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received whois command from player %s for character %s", GetPlayer()->GetName(), packet.CharName.c_str());
}

void WorldSession::HandleComplainOpcode(WorldPacket& recvData)
{
    uint8 spam_type;                                        // 0 - mail, 1 - chat
    ObjectGuid spammer_guid;
    uint32 unk1 = 0;
    uint32 unk2 = 0;
    uint32 unk3 = 0;
    uint32 unk4 = 0;
    std::string description = "";
    recvData >> spam_type;                                 // unk 0x01 const, may be spam type (mail/chat)
    recvData >> spammer_guid;                              // player guid
    switch (spam_type)
    {
    case 0:
        recvData >> unk1;                              // const 0
        recvData >> unk2;                              // probably mail id
        recvData >> unk3;                              // const 0
        break;
    case 1:
        recvData >> unk1;                              // probably language
        recvData >> unk2;                              // message type?
        recvData >> unk3;                              // probably channel id
        recvData >> unk4;                              // time
        recvData >> description;                       // spam description string (messagetype, channel name, player name, message)
        break;
    }

    // NOTE: all chat messages from this spammer automatically ignored by spam reporter until logout in case chat spam.
    // if it's mail spam - ALL mails from this spammer automatically removed by client

    // Complaint Received message
    WorldPacket data(SMSG_COMPLAINT_RESULT, 2);
    data << uint8(0); // value 1 resets CGChat::m_complaintsSystemStatus in client. (unused?)
    data << uint8(0); // value 0xC generates a "CalendarError" in client.
    SendPacket(&data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "REPORT SPAM: type %u, guid %u, unk1 %u, unk2 %u, unk3 %u, unk4 %u, message %s", spam_type, spammer_guid.GetCounter(), unk1, unk2, unk3, unk4, description.c_str());
}

//! 6.0.3
void WorldSession::HandleRealmQueryNameOpcode(WorldPacket& recvData)
{
    uint32 realmId = recvData.read<uint32>();

    WorldPacket data(SMSG_REALM_QUERY_RESPONSE, 10 + 10 + 1 + 1 + 1 + 4);
    if (realmId != realmHandle.Index && realmId != GetVirtualRealmAddress())  // Cheater ?
    {
        data << uint32(realmId);
        data << uint8(1);
        SendPacket(&data);
        return;
    }

    std::string realmName = sWorld->GetRealmName();
    std::string trimmedName = sWorld->GetTrimmedRealmName();

    data << uint32(realmId);
    data << uint8(0);   // ok, realmId exist server-side

    data.WriteBit(1);   // IsLocal
    data.WriteBit(0);

    data.WriteBits(realmName.size(), 8);
    data.WriteBits(trimmedName.size(), 8);
    data.WriteString(realmName);
    data.WriteString(trimmedName);
    SendPacket(&data);
}

void WorldSession::HandleFarSight(WorldPackets::Misc::FarSight& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (!packet.Enable)
        player->SetSeer(player);
    else
        if (WorldObject* target = player->GetViewpoint())
            player->SetSeer(target);

    player->UpdateVisibilityForPlayer();
}

//! 6.0.3
void WorldSession::HandleSetTitleOpcode(WorldPacket& recvData)
{
    int32 title;
    recvData >> title;

    // -1 at none
    if (title > 0 && title < MAX_TITLE_INDEX)
    {
        if (!GetPlayer()->HasTitle(title))
            return;
    }
    else
        title = 0;

    GetPlayer()->SetUInt32Value(PLAYER_FIELD_PLAYER_TITLE, title);
}

//! 6.0.3
void WorldSession::HandleTimeSyncResp(WorldPackets::Misc::TimeSyncResponse& packet)
{
    // Prevent crashing server if queue is empty
    if (_player->m_timeSyncQueue.empty())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Received CMSG_TIME_SYNC_RESPONSE from player %s without requesting it (hacker?)", _player->GetName());
        return;
    }

    if (packet.SequenceIndex != _player->m_timeSyncQueue.front())
        sLog->outError(LOG_FILTER_NETWORKIO, "Wrong time sync counter from player %s (cheater?)", _player->GetName());

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Time sync received: counter %u, client ticks %u, time since last sync %u", packet.SequenceIndex, packet.ClientTime, packet.ClientTime - _player->m_timeSyncClient);

    uint32 ourTicks = packet.ClientTime + (getMSTime() - _player->m_timeSyncServer);

    // diff should be small
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Our ticks: %u, diff %u, latency %u", ourTicks, ourTicks - packet.ClientTime, GetLatency());

    _player->m_timeSyncClient = packet.ClientTime;
    _player->m_timeSyncQueue.pop();
}

void WorldSession::HandleResetInstances(WorldPackets::Instance::ResetInstances& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (Group* group = player->GetGroup())
    {
        if (group->IsLeader(player->GetGUID()))
            group->ResetInstances(INSTANCE_RESET_ALL, false, false, player);
    }
    else
        player->ResetInstances(INSTANCE_RESET_ALL, false, false);
}

//! 6.0.3
void WorldSession::HandleSetDungeonDifficultyOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "MSG_SET_DUNGEON_DIFFICULTY");

    uint32 DifficultyID;
    recvData >> DifficultyID;

    DifficultyEntry const* difficultyEntry = sDifficultyStore.LookupEntry(DifficultyID);
    if (!difficultyEntry)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: %s sent an invalid instance mode %d!",
            _player->GetGUID().ToString().c_str(), DifficultyID);
        return;
    }

    if (difficultyEntry->InstanceType != MAP_INSTANCE)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: %s sent an non-dungeon instance mode %d!",
            _player->GetGUID().ToString().c_str(), difficultyEntry->m_ID);
        return;
    }

    if (!(difficultyEntry->Flags & DIFFICULTY_FLAG_CAN_SELECT))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: %s sent unselectable instance mode %d!",
            _player->GetGUID().ToString().c_str(), difficultyEntry->m_ID);
        return;
    }

    if (Difficulty(DifficultyID) == _player->GetDungeonDifficultyID())
        return;

    // cannot reset while in an instance
    Map* map = _player->FindMap();
    if (map && map->IsDungeon())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: player (Name: %s, GUID: %u) tried to reset the instance while player is inside!", _player->GetName(), _player->GetGUID().GetCounter());
        return;
    }

    Group* group = _player->GetGroup();
    if (group)
    {
        if (group->IsLeader(_player->GetGUID()))
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* groupGuy = itr->getSource();
                if (!groupGuy)
                    continue;

                if (!groupGuy->IsInMap(groupGuy))
                    return;

                if (groupGuy->GetMap()->IsNonRaidDungeon())
                {
                    sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: player %d tried to reset the instance while group member (Name: %s, GUID: %u) is inside!", _player->GetGUID().GetCounter(), groupGuy->GetName(), groupGuy->GetGUID().GetCounter());
                    return;
                }
            }

            // the difficulty is set even if the instances can't be reset
            //_player->SendDungeonDifficulty();
            group->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, false, false, _player);
            group->SetDungeonDifficultyID(Difficulty(DifficultyID));
            _player->SendDungeonDifficulty();
            group->SendUpdate();
        }
    }
    else
    {
        _player->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, false, false);
        _player->SetDungeonDifficultyID(Difficulty(DifficultyID));
        _player->SendDungeonDifficulty();
    }
}

//! 6.0.3
void WorldSession::HandleSetRaidDifficultyOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "MSG_SET_RAID_DIFFICULTY");

    uint32 DifficultyID;
    uint8 Legacy;
    recvData >> DifficultyID >> Legacy;

    DifficultyEntry const* difficultyEntry = sDifficultyStore.LookupEntry(DifficultyID);
    if (!difficultyEntry)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: %s sent an invalid instance mode %u!",
            _player->GetGUID().ToString().c_str(), DifficultyID);
        return;
    }

    if (difficultyEntry->InstanceType != MAP_RAID)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: %s sent an non-dungeon instance mode %u!",
            _player->GetGUID().ToString().c_str(), difficultyEntry->m_ID);
        return;
    }

    if (!(difficultyEntry->Flags & DIFFICULTY_FLAG_CAN_SELECT))
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: %s sent unselectable instance mode %u!",
            _player->GetGUID().ToString().c_str(), difficultyEntry->m_ID);
        return;
    }

    if (((difficultyEntry->Flags & DIFFICULTY_FLAG_LEGACY) >> 5) != Legacy)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: %s sent not matching legacy difficulty %u!",
            _player->GetGUID().ToString().c_str(), difficultyEntry->m_ID);
        return;
    }

    Difficulty difficultyID = Difficulty(difficultyEntry->m_ID);
    if (difficultyID == (Legacy ?  _player->GetLegacyRaidDifficultyID() : _player->GetRaidDifficultyID()))
        return;

    // cannot reset while in an instance
    Map* map = _player->FindMap();
    if (map && map->IsDungeon())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetRaidDifficultyOpcode: player %d tried to reset the instance while inside!", _player->GetGUID().GetCounter());
        return;
    }

    Group* group = _player->GetGroup();
    if (group)
    {
        if (group->IsLeader(_player->GetGUID()))
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* groupGuy = itr->getSource();
                if (!groupGuy)
                    continue;

                if (!groupGuy->IsInMap(groupGuy))
                    return;

                if (groupGuy->GetMap()->IsRaid())
                {
                    sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetRaidDifficultyOpcode: player %d tried to reset the instance while inside!", _player->GetGUID().GetCounter());
                    return;
                }
            }
            // the difficulty is set even if the instances can't be reset
            //_player->SendDungeonDifficulty();
            group->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, true, Legacy != 0, _player);
            if (Legacy)
                group->SetLegacyRaidDifficultyID(difficultyID);
            else
                group->SetRaidDifficultyID(difficultyID);
            _player->SendRaidDifficulty(Legacy != 0);
            group->SendUpdate();
        }
    }
    else
    {
        _player->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, true, Legacy != 0);
        if (Legacy)
            _player->SetLegacyRaidDifficultyID(difficultyID);
        else
            _player->SetRaidDifficultyID(difficultyID);

        _player->SendRaidDifficulty(Legacy != 0);
    }
}

void WorldSession::HandleRequestPetInfoOpcode(WorldPacket& /*recvData */) { }

void WorldSession::HandleSetTaxiBenchmarkOpcode(WorldPacket& recvData)
{
    uint8 mode;
    recvData >> mode;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Client used \"/timetest %d\" command", mode);
}

void WorldSession::HandleGuildAchievementProgressQuery(WorldPacket& recvData)
{
    uint32 achievementId;
    recvData >> achievementId;

    if (Guild* guild = sGuildMgr->GetGuildById(_player->GetGuildId()))
        guild->GetAchievementMgr().SendAchievementInfo(_player, achievementId);
}

void WorldSession::HandleUITimeRequest(WorldPackets::Misc::UITimeRequest& /*request*/)
{
    WorldPackets::Misc::UITime response;
    response.Time = time(NULL);
    SendPacket(response.Write());
}

void WorldSession::SendSetPhaseShift(std::set<uint32> const& phaseIds, std::set<uint32> const& terrainswaps, std::set<uint32> const& worldMapAreaSwaps, uint32 flag /*=0x1F*/)
{
    WorldPackets::Misc::PhaseShift phaseShift;
    phaseShift.ClientGUID = _player->GetGUID();
    phaseShift.PersonalGUID = _player->GetGUID();
    phaseShift.PhaseShifts = phaseIds;
    phaseShift.VisibleMapIDs = terrainswaps;
    phaseShift.UiWorldMapAreaIDSwaps = worldMapAreaSwaps;
    SendPacket(phaseShift.Write());
}

void WorldSession::HandleAreaSpiritHealerQuery(WorldPackets::Battleground::AreaSpiritHealerQuery& packet)
{
    Creature* unit = GetPlayer()->GetMap()->GetCreature(packet.HealerGuid);
    if (!unit)
        return;

    if (!unit->isSpiritService())                            // it's not spirit service
        return;

    if (Battleground* bg = _player->GetBattleground())
        sBattlegroundMgr->SendAreaSpiritHealerQueryOpcode(_player, bg, packet.HealerGuid);

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(_player->GetZoneId()))
        bf->SendAreaSpiritHealerQueryOpcode(_player, packet.HealerGuid);
}

void WorldSession::HandleAreaSpiritHealerQueue(WorldPackets::Battleground::AreaSpiritHealerQueue& packet)
{
    Creature* unit = GetPlayer()->GetMap()->GetCreature(packet.HealerGuid);
    if (!unit)
        return;

    if (!unit->isSpiritService())                            // it's not spirit service
        return;

    if (Battleground* bg = _player->GetBattleground())
        bg->AddPlayerToResurrectQueue(packet.HealerGuid, _player->GetGUID());

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(_player->GetZoneId()))
    {
        bf->RemovePlayerFromResurrectQueue(_player->GetGUID());
        bf->AddPlayerToResurrectQueue(packet.HealerGuid, _player->GetGUID());
    }
}

void WorldSession::HandleHearthAndResurrect(WorldPacket& /*recvData*/)
{
    if (_player->isInFlight())
        return;

    AreaTableEntry const* atEntry = GetAreaEntryByAreaID(_player->GetAreaId());
    if (!atEntry || !(atEntry->Flags[0] & AREA_FLAG_WINTERGRASP_2))
        return;

    Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(_player->GetZoneId());
    if (!bf || !bf->IsWarTime())
        return;

    _player->BuildPlayerRepop();
    _player->ResurrectPlayer(100);
    _player->TeleportTo(_player->m_homebindMapId, _player->m_homebindX, _player->m_homebindY, _player->m_homebindZ, _player->GetOrientation());
}

void WorldSession::HandleInstanceLockResponse(WorldPackets::Instance::InstanceLockResponse& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (!player->HasPendingBind())
        return;

    if (packet.AcceptLock)
        player->BindToInstance();
    else
        player->RepopAtGraveyard();

    player->SetPendingBind(0, 0);
}

void WorldSession::HandleViolenceLevel(WorldPackets::Misc::ViolenceLevel& /*packet*/) { }

void WorldSession::HandleObjectUpdateFailed(WorldPackets::Misc::ObjectUpdateFailed& packet)
{
    if (_player->GetGUID() == packet.ObjectGUID)
    {
        LogoutPlayer(true);
        return;
    }

    _player->m_clientGUIDs.erase(packet.ObjectGUID);
}

void WorldSession::HandleObjectUpdateRescued(WorldPackets::Misc::ObjectUpdateRescued& packet)
{
    _player->m_clientGUIDs.insert(packet.ObjectGUID);
}

// DestrinyFrame.xml : lua function NeutralPlayerSelectFaction
#define JOIN_THE_ALLIANCE 1
#define JOIN_THE_HORDE    0

void WorldSession::HandleSetFactionOpcode(WorldPacket& recvPacket)
{
    uint32 choice = recvPacket.read<uint32>();

    if (_player->getRace() != RACE_PANDAREN_NEUTRAL)
        return;

    if (choice == JOIN_THE_HORDE)
    {
        _player->SetByteValue(UNIT_FIELD_BYTES_0, 0, RACE_PANDAREN_HORDE);
        _player->setFactionForRace(RACE_PANDAREN_HORDE);
        _player->SaveToDB();
        WorldLocation location(1, 1349.72f, -4374.50f, 26.15f, M_PI);
        _player->TeleportTo(location);
        _player->SetHomebind(location, 363);
        _player->learnSpell(669, false); // Language Orcish
        _player->learnSpell(108127, false); // Language Pandaren
    }
    else if (choice == JOIN_THE_ALLIANCE)
    {
        _player->SetByteValue(UNIT_FIELD_BYTES_0, 0, RACE_PANDAREN_ALLIANCE);
        _player->setFactionForRace(RACE_PANDAREN_ALLIANCE);
        _player->SaveToDB();
        WorldLocation location(0, -9076.77f, 424.74f, 92.42f, M_PI);
        _player->TeleportTo(location);
        _player->SetHomebind(location, 9);
        _player->learnSpell(668, false); // Language Common
        _player->learnSpell(108127, false); // Language Pandaren
    }

    //if (_player->GetQuestStatus(31450) == QUEST_STATUS_INCOMPLETE)
        //_player->KilledMonsterCredit(64594);

    _player->CompleteQuest(31450);
    _player->SendMovieStart(116);
}

//! 5.4.1
void WorldSession::HandleChangeCurrencyFlags(WorldPacket& recvPacket)
{
    uint32 currencyId, flags;
    recvPacket >> currencyId >> flags;

    if (GetPlayer())
        GetPlayer()->ModifyCurrencyFlag(currencyId, uint8(flags));
}

void WorldSession::HandleRequestCemeteryList(WorldPackets::Misc::RequestCemeteryList& /*packet*/)
{
    uint32 zoneId = _player->GetZoneId();
    uint32 team = _player->GetTeam();

    std::vector<uint32> graveyardIds;
    auto range = sObjectMgr->GraveYardStore.equal_range(zoneId);

    for (auto it = range.first; it != range.second && graveyardIds.size() < 16; ++it) // client max
    {
        if (it->second.team == 0 || it->second.team == team)
            graveyardIds.push_back(it->first);
    }

    if (graveyardIds.empty())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "No graveyards found for zone %u for %s (team %u) in CMSG_REQUEST_CEMETERY_LIST",
            zoneId, _player->GetGUID().ToString().c_str(), team);
        return;
    }

    WorldPackets::Misc::RequestCemeteryListResponse packet;
    packet.IsGossipTriggered = false;
    packet.CemeteryID.reserve(graveyardIds.size());

    for (uint32 id : graveyardIds)
        packet.CemeteryID.push_back(id);

    SendPacket(packet.Write());
}

//! ToDo: WTF?
void WorldSession::SuspendTokenResponse(WorldPacket& recvPacket)
{
    uint32 Sequence;
    recvPacket >> Sequence;

    WorldPacket data(SMSG_RESUME_TOKEN, 15);
    data << uint32(0);
    data.WriteBits(2, 2);
    SendPacket(&data);
}

void WorldSession::HandleForcedReactions(WorldPackets::Reputation::RequestForcedReactions& /*packet*/)
{
    _player->GetReputationMgr().SendForceReactions();
}

void WorldSession::HandleWarGameStart(WorldPacket& recvPacket) { }

void WorldSession::HandleWarGameAccept(WorldPacket& recvPacket) { }

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket& /*recvData*/)
{
    ObjectGuid guid = _player->GetGUID();

    WorldPacket data(SMSG_SPECIAL_MOUNT_ANIM, 8 + 1);

    GetPlayer()->SendMessageToSet(&data, false);
}

void WorldSession::HandleSummonResponse(WorldPackets::Movement::SummonResponse& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (!player->isAlive() || player->isInCombat())
        return;

    player->SummonIfPossible(packet.Accept);
}

void WorldSession::HandleRandomRollClient(WorldPackets::Misc::RandomRollClient& packet)
{
    Player* player = GetPlayer();
    if (packet.Min > packet.Max || packet.Max > 10000 || !player)
        return;

    WorldPackets::Misc::RandomRoll randomRoll;
    randomRoll.Min = packet.Min;
    randomRoll.Max = packet.Max;
    randomRoll.Result = urand(packet.Min, packet.Max);
    randomRoll.Roller = player->GetGUID();
    randomRoll.RollerWowAccount = GetBattlenetAccountGUID();
    if (player->GetGroup())
        player->GetGroup()->BroadcastPacket(randomRoll.Write(), false);
    else
        SendPacket(randomRoll.Write());
}

void WorldSession::HandleOpeningCinematic(WorldPackets::Misc::OpeningCinematic& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (player->GetUInt32Value(PLAYER_FIELD_XP))
        return;

    if (ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(player->getClass()))
    {
        if (classEntry->CinematicSequenceID)
            player->SendCinematicStart(classEntry->CinematicSequenceID);
        else if (ChrRacesEntry const* raceEntry = sChrRacesStore.LookupEntry(player->getRace()))
            player->SendCinematicStart(raceEntry->CinematicSequenceID);
    }
}

void WorldSession::HandleWorldTeleport(WorldPackets::Misc::WorldTeleport& packet)
{
    if (GetPlayer()->isInFlight())
        return;

    if (AccountMgr::IsAdminAccount(GetSecurity()))
        GetPlayer()->TeleportTo(packet.MapID, packet.Pos.x, packet.Pos.y, packet.Pos.z, packet.Facing);
    else
        SendNotification(LANG_YOU_NOT_HAVE_PERMISSION);
}
