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
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "SystemConfig.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"
#include "Chat.h"
#include "Group.h"
#include "Guild.h"
#include "GuildFinderMgr.h"
#include "Language.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include "PlayerDump.h"
#include "SharedDefines.h"
#include "SocialMgr.h"
#include "UpdateMask.h"
#include "Util.h"
#include "ScriptMgr.h"
#include "Battleground.h"
#include "AccountMgr.h"
#include "DBCStores.h"
#include "LFGMgr.h"
#include "BattlenetServerManager.h"
#include <boost/regex.hpp>

#include "ClientConfigPackets.h"
#include "CharacterPackets.h"
#include "SystemPackets.h"
#include "BattlegroundPackets.h"
#include "MiscPackets.h"
#include "BattlePayPackets.h"
#include "EquipmentSetPackets.h"
#include "QueryPackets.h"
#include "AuthenticationPackets.h"

bool LoginQueryHolder::Initialize()
{
    SetSize(MAX_PLAYER_LOGIN_QUERY);

    bool res = true;
    ObjectGuid::LowType lowGuid = m_guid.GetCounter();
    PreparedStatement* stmt = NULL;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADFROM, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GROUP_MEMBER);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADGROUP, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_INSTANCE);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADBOUNDINSTANCES, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_AURAS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADAURAS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_AURAS_EFFECTS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADAURAS_EFFECTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SPELL);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSPELLS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_MOUNTS);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTMOUNTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_QUESTSTATUS);
    stmt->setUInt32(0, m_accountId);
    stmt->setUInt64(1, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADQUESTSTATUS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_QUESTSTATUS_OBJECTIVES);
    stmt->setUInt32(0, m_accountId);
    stmt->setUInt64(1, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_QUEST_STATUS_OBJECTIVES, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_DAILYQUESTSTATUS);
    stmt->setUInt32(0, m_accountId);
    stmt->setUInt64(1, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADDAILYQUESTSTATUS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_WEEKLYQUESTSTATUS);
    stmt->setUInt32(0, m_accountId);
    stmt->setUInt64(1, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADWEEKLYQUESTSTATUS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SEASONALQUESTSTATUS);
    stmt->setUInt32(0, m_accountId);
    stmt->setUInt32(1, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSEASONALQUESTSTATUS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_REPUTATION);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADREPUTATION, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_INVENTORY);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADINVENTORY, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_VOID_STORAGE);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADVOIDSTORAGE, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_ACTIONS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACTIONS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_MAILCOUNT);
    stmt->setUInt64(0, lowGuid);
    stmt->setUInt64(1, uint64(time(NULL)));
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADMAILCOUNT, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_MAILDATE);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADMAILDATE, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SOCIALLIST);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSOCIALLIST, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_HOMEBIND);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADHOMEBIND, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SPELLCOOLDOWNS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSPELLCOOLDOWNS, stmt);

    if (sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_DECLINEDNAMES);
        stmt->setUInt64(0, lowGuid);
        res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADDECLINEDNAMES, stmt);
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_MEMBER);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADGUILD, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_ACHIEVEMENTS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACHIEVEMENTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_ACHIEVEMENTS);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTACHIEVEMENTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_CRITERIAPROGRESS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADCRITERIAPROGRESS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_CRITERIAPROGRESS);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTCRITERIAPROGRESS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_EQUIPMENTSETS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADEQUIPMENTSETS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_BGDATA);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADBGDATA, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GLYPHS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADGLYPHS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_TALENTS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADTALENTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_ACCOUNT_DATA);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTDATA, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SKILLS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSKILLS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_RANDOMBG);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADRANDOMBG, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_BANNED);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADBANNED, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_QUESTSTATUSREW);
    stmt->setUInt32(0, m_accountId);
    stmt->setUInt64(1, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADQUESTSTATUSREW, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_INSTANCELOCKTIMES);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADINSTANCELOCKTIMES, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_CURRENCY);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADCURRENCY, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CUF_PROFILES);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_CUF_PROFILES, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_BATTLE_PET_LIST);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_BATTLE_PETS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_BATTLE_PET_SLOTS);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_BATTLE_PET_SLOTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_ARCHAELOGY);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADARCHAELOGY, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_ARCHAEOLOGY_FINDS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ARCHAEOLOGY_FINDS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PERSONAL_RATE);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_PERSONAL_RATE, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_VISUAL);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_VISUAL, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_LOOTCOOLDOWN);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_LOOTCOOLDOWN, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_KILL);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_HONOR, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GARRISON);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_GARRISON, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GARRISON_BLUEPRINTS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_GARRISON_BLUEPRINTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GARRISON_BUILDINGS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_GARRISON_BUILDINGS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GARRISON_FOLLOWERS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_GARRISON_FOLLOWERS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GARRISON_FOLLOWER_ABILITIES);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_GARRISON_FOLLOWER_ABILITIES, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GARRISON_MISSIONS);
    stmt->setUInt64(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_GARRISON_MISSIONS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_TOYS);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_TOYS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_HEIRLOOMS);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_HEIRLOOMS, stmt);

    return res;
}

void WorldSession::HandleCharEnum(PreparedQueryResult result, bool isDeleted)
{
    WorldPackets::Character::EnumCharactersResult charEnum;
    charEnum.Success = true;
    charEnum.IsDeletedCharacters = isDeleted;

    _allowedCharsToLogin.clear();

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            WorldPackets::Character::EnumCharactersResult::CharacterInfo charInfo(fields);
            sLog->outInfo(LOG_FILTER_NETWORKIO, "Loading char guid %s from account %u.", charInfo.Guid.ToString().c_str(), GetAccountId());

            //Player::BuildEnumData(result, &dataBuffer, &bitBuffer);
            // Do not allow locked characters to login
            if (!(charInfo.Flags & (CHARACTER_LOCKED_FOR_TRANSFER | CHARACTER_FLAG_LOCKED_BY_BILLING)))
                _allowedCharsToLogin.insert(charInfo.Guid.GetCounter());

            charEnum.Characters.emplace_back(charInfo);

        } while (result->NextRow());
    }

    SendPacket(charEnum.Write());
    timeCharEnumOpcode = 0;
}

//6.1.2
void WorldSession::HandleCharEnumOpcode(WorldPackets::Character::EnumCharacters& enumCharacters)
{
    if (timeCharEnumOpcode)
        return;
    timeCharEnumOpcode = 1;

    SendCharacterEnum(enumCharacters.GetOpcode() == CMSG_ENUM_CHARACTERS_DELETED_BY_CLIENT);
}

void WorldSession::SendCharacterEnum(bool deleted /*= false*/)
{
    // remove expired bans
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_EXPIRED_BANS);
    CharacterDatabase.Execute(stmt);

    /// get all the data necessary for loading all characters (along with their pets) on the account

    //if (sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
    //    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ENUM_DECLINED_NAME);
    //else
    if (deleted)
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ENUM_DELETED);
    else
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ENUM);

    stmt->setUInt32(0, GetAccountId());

    _charEnumCallback.SetParam(deleted);
    _charEnumCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

//! 6.1.2
void WorldSession::HandleCharUndelete(WorldPacket & recvData)
{
    //CMSG_UNDELETE_CHARACTER

    uint32 token = recvData.read<uint32>();
    ObjectGuid guid;
    recvData >> guid;


    WorldPacket data(SMSG_UNDELETE_CHARACTER_RESPONSE, 20);
    data << uint32(token);

    if (!HasAuthFlag(AT_AUTH_FLAG_RESTORE_DELETED_CHARACTER))
    {
        data << uint32(CHARACTER_UNDELETE_RESULT_ERROR_DISABLED);
        data << guid;
        SendPacket(&data);
        return;
    }

    uint32 res = CHARACTER_UNDELETE_RESULT_ERROR_UNKNOWN;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHECK_DELETED_CHARACTER);
    stmt->setUInt64(0, guid.GetCounter());
    stmt->setUInt32(1, GetAccountId());
    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
    {
        res = CHARACTER_UNDELETE_RESULT_OK;

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_RESTORE_DELETED_CHARACTER);
        stmt->setUInt32(0, GetAccountId());
        stmt->setUInt32(1, rand()); //rand int name send with future rename option.
        stmt->setUInt64(2, guid.GetCounter());
        CharacterDatabase.Execute(stmt);

        RemoveAuthFlag(AT_AUTH_FLAG_RESTORE_DELETED_CHARACTER);
        SendFeatureSystemStatusGlueScreen();
    }

    data << res;
    data << guid;
    SendPacket(&data);
}

void WorldSession::HandleCharCreateOpcode(WorldPackets::Character::CreateChar& charCreate)
{
    if (AccountMgr::IsPlayerAccount(GetSecurity()))
    {
        if (uint32 mask = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_DISABLED))
        {
            bool disabled = false;

            uint32 team = Player::TeamForRace(charCreate.CreateInfo->Race);
            switch (team)
            {
                case ALLIANCE: disabled = (mask & (1 << 0)) != 0; break;
                case HORDE:    disabled = (mask & (1 << 1)) != 0; break;
            }

            if (disabled)
            {
                SendCharCreate(CHAR_CREATE_DISABLED);
                return;
            }
        }
    }

    ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(charCreate.CreateInfo->Class);
    if (!classEntry)
    {
        SendCharCreate(CHAR_CREATE_FAILED);
        sLog->outError(LOG_FILTER_NETWORKIO, "Class (%u) not found in DBC while creating new char for account (ID: %u): wrong DBC files or cheater?", charCreate.CreateInfo->Class, GetAccountId());
        return;
    }

    ChrRacesEntry const* raceEntry = sChrRacesStore.LookupEntry(charCreate.CreateInfo->Race);
    if (!raceEntry)
    {
        SendCharCreate(CHAR_CREATE_FAILED);
        sLog->outError(LOG_FILTER_NETWORKIO, "Race (%u) not found in DBC while creating new char for account (ID: %u): wrong DBC files or cheater?", charCreate.CreateInfo->Race, GetAccountId());
        return;
    }

    if (AccountMgr::IsPlayerAccount(GetSecurity()))
    {
        uint32 raceMaskDisabled = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_DISABLED_RACEMASK);
        if ((1 << (charCreate.CreateInfo->Race - 1)) & raceMaskDisabled)
        {
            SendCharCreate(CHAR_CREATE_DISABLED);
            return;
        }

        uint32 classMaskDisabled = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_DISABLED_CLASSMASK);
        if ((1 << (charCreate.CreateInfo->Class - 1)) & classMaskDisabled)
        {
            SendCharCreate(CHAR_CREATE_DISABLED);
            return;
        }
    }

    // prevent character creating with invalid name
    if (!normalizePlayerName(charCreate.CreateInfo->Name))
    {
        SendCharCreate(CHAR_NAME_NO_NAME);
        sLog->outError(LOG_FILTER_NETWORKIO, "Account:[%d] but tried to Create character with empty [name] ", GetAccountId());
        return;
    }

    // check name limitations
    ResponseCodes res = ObjectMgr::CheckPlayerName(charCreate.CreateInfo->Name, true);
    if (res != CHAR_NAME_SUCCESS)
    {
        SendCharCreate(res);
        return;
    }

    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(charCreate.CreateInfo->Name))
    {
        SendCharCreate(CHAR_NAME_RESERVED);
        return;
    }

    // speedup check for heroic class disabled case
    uint32 heroic_free_slots = sWorld->getIntConfig(CONFIG_HEROIC_CHARACTERS_PER_REALM);
    if (heroic_free_slots == 0 && AccountMgr::IsPlayerAccount(GetSecurity()) && charCreate.CreateInfo->Class == CLASS_DEATH_KNIGHT)
    {
        SendCharCreate(CHAR_CREATE_UNIQUE_CLASS_LIMIT);
        return;
    }

    // speedup check for heroic class disabled case
    uint32 req_level_for_heroic = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_MIN_LEVEL_FOR_HEROIC_CHARACTER);
    if (AccountMgr::IsPlayerAccount(GetSecurity()) && charCreate.CreateInfo->Class == CLASS_DEATH_KNIGHT && req_level_for_heroic > sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
    {
        SendCharCreate(CHAR_CREATE_LEVEL_REQUIREMENT);
        return;
    }

    _charCreateCallback.SetParam(charCreate.CreateInfo);
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHECK_NAME);
    stmt->setString(0, charCreate.CreateInfo->Name);
    _charCreateCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::HandleCharCreateCallback(PreparedQueryResult result, WorldPackets::Character::CharacterCreateInfo* createInfo)
{
    /** This is a series of callbacks executed consecutively as a result from the database becomes available.
        This is much more efficient than synchronous requests on packet handler, and much less DoS prone.
        It also prevents data syncrhonisation errors.
    */
    switch (_charCreateCallback.GetStage())
    {
        case 0:
        {
            if (result)
            {
                SendCharCreate(CHAR_CREATE_NAME_IN_USE);
                _charCreateCallback.Reset();
                return;
            }

            ASSERT(_charCreateCallback.GetParam().get() == createInfo);

            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_SUM_REALM_CHARACTERS);
            stmt->setUInt32(0, GetAccountId());

            _charCreateCallback.FreeResult();
            _charCreateCallback.SetFutureResult(LoginDatabase.AsyncQuery(stmt));
            _charCreateCallback.NextStage();
            break;
        }
        case 1:
        {
            uint16 acctCharCount = 0;
            if (result)
            {
                Field* fields = result->Fetch();
                // SELECT SUM(x) is MYSQL_TYPE_NEWDECIMAL - needs to be read as string
                const char* ch = fields[0].GetCString();
                if (ch)
                    acctCharCount = atoi(ch);
            }

            if (acctCharCount >= sWorld->getIntConfig(CONFIG_CHARACTERS_PER_ACCOUNT))
            {
                SendCharCreate(CHAR_CREATE_ACCOUNT_LIMIT);
                _charCreateCallback.Reset();
                return;
            }

            ASSERT(_charCreateCallback.GetParam().get() == createInfo);

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_SUM_CHARS);
            stmt->setUInt32(0, GetAccountId());

            _charCreateCallback.FreeResult();
            _charCreateCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
            _charCreateCallback.NextStage();
        }
        break;
        case 2:
        {
            if (result)
            {
                Field* fields = result->Fetch();
                createInfo->CharCount = uint8(fields[0].GetUInt64()); // SQL's COUNT() returns uint64 but it will always be less than uint8.Max

                if (createInfo->CharCount >= sWorld->getIntConfig(CONFIG_CHARACTERS_PER_REALM))
                {
                    SendCharCreate(CHAR_CREATE_SERVER_LIMIT);
                    _charCreateCallback.Reset();
                    return;
                }
            }

            bool allowTwoSideAccounts = !sWorld->IsPvPRealm() || sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_ACCOUNTS) || !AccountMgr::IsPlayerAccount(GetSecurity());
            uint32 skipCinematics = sWorld->getIntConfig(CONFIG_SKIP_CINEMATICS);

            _charCreateCallback.FreeResult();

            if (!allowTwoSideAccounts || skipCinematics == 1 || createInfo->Class == CLASS_DEATH_KNIGHT)
            {
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_CREATE_INFO);
                stmt->setUInt32(0, GetAccountId());
                stmt->setUInt32(1, (skipCinematics == 1 || createInfo->Class == CLASS_DEATH_KNIGHT) ? 10 : 1);
                _charCreateCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
                _charCreateCallback.NextStage();
                return;
            }

            _charCreateCallback.NextStage();
            HandleCharCreateCallback(PreparedQueryResult(NULL), createInfo);   // Will jump to case 3
        }
        break;
        case 3:
        {
            bool haveSameRace = false;
            uint32 heroicReqLevel = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_MIN_LEVEL_FOR_HEROIC_CHARACTER);
            bool hasHeroicReqLevel = (heroicReqLevel == 0);
            bool allowTwoSideAccounts = !sWorld->IsPvPRealm() || sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_ACCOUNTS) || !AccountMgr::IsPlayerAccount(GetSecurity());
            uint32 skipCinematics = sWorld->getIntConfig(CONFIG_SKIP_CINEMATICS);

            if (result)
            {
                uint32 team = Player::TeamForRace(createInfo->Race);
                uint32 freeHeroicSlots = sWorld->getIntConfig(CONFIG_HEROIC_CHARACTERS_PER_REALM);

                Field* field = result->Fetch();
                uint8 accRace  = field[1].GetUInt8();

                if (AccountMgr::IsPlayerAccount(GetSecurity()) && createInfo->Class == CLASS_DEATH_KNIGHT)
                {
                    uint8 accClass = field[2].GetUInt8();
                    if (accClass == CLASS_DEATH_KNIGHT)
                    {
                        if (freeHeroicSlots > 0)
                            --freeHeroicSlots;

                        if (freeHeroicSlots == 0)
                        {
                            SendCharCreate(CHAR_CREATE_UNIQUE_CLASS_LIMIT);
                            _charCreateCallback.Reset();
                            return;
                        }
                    }

                    if (!hasHeroicReqLevel)
                    {
                        uint8 accLevel = field[0].GetUInt8();
                        if (accLevel >= heroicReqLevel)
                            hasHeroicReqLevel = true;
                    }
                }

                // need to check team only for first character
                // TODO: what to if account already has characters of both races?
                if (!allowTwoSideAccounts)
                {
                    uint32 accTeam = 0;
                    if (accRace > 0)
                        accTeam = Player::TeamForRace(accRace);

                    if (accTeam != team)
                    {
                        SendCharCreate(CHAR_CREATE_PVP_TEAMS_VIOLATION);
                        _charCreateCallback.Reset();
                        return;
                    }
                }

                // search same race for cinematic or same class if need
                // TODO: check if cinematic already shown? (already logged in?; cinematic field)
                while ((skipCinematics == 1 && !haveSameRace) || createInfo->Class == CLASS_DEATH_KNIGHT)
                {
                    if (!result->NextRow())
                        break;

                    field = result->Fetch();
                    accRace = field[1].GetUInt8();

                    if (!haveSameRace)
                        haveSameRace = createInfo->Race == accRace;

                    if (AccountMgr::IsPlayerAccount(GetSecurity()) && createInfo->Class == CLASS_DEATH_KNIGHT)
                    {
                        uint8 acc_class = field[2].GetUInt8();
                        if (acc_class == CLASS_DEATH_KNIGHT)
                        {
                            if (freeHeroicSlots > 0)
                                --freeHeroicSlots;

                            if (freeHeroicSlots == 0)
                            {
                                SendCharCreate(CHAR_CREATE_UNIQUE_CLASS_LIMIT);
                                _charCreateCallback.Reset();
                                return;
                            }
                        }

                        if (!hasHeroicReqLevel)
                        {
                            uint8 acc_level = field[0].GetUInt8();
                            if (acc_level >= heroicReqLevel)
                                hasHeroicReqLevel = true;
                        }
                    }
                }
            }

            if (AccountMgr::IsPlayerAccount(GetSecurity()) && createInfo->Class == CLASS_DEATH_KNIGHT && !hasHeroicReqLevel)
            {
                SendCharCreate(CHAR_CREATE_LEVEL_REQUIREMENT);
                _charCreateCallback.Reset();
                return;
            }

            // Avoid exploit of create multiple characters with same name
            if (!sWorld->AddCharacterName(createInfo->Name))
            {
                SendCharCreate(CHAR_CREATE_NAME_IN_USE);
                _charCreateCallback.Reset();
                return;
            }

            Player newChar(this);
            newChar.GetMotionMaster()->Initialize();
            if (!newChar.Create(sObjectMgr->GetGenerator<HighGuid::Player>()->Generate(), createInfo))
            {
                // Player not create (race/class/etc problem?)
                newChar.CleanupsBeforeDelete();

                SendCharCreate(CHAR_CREATE_ERROR);
                _charCreateCallback.Reset();
                return;
            }

            createInfo->guid = newChar.GetGUID();

            if ((haveSameRace && skipCinematics == 1) || skipCinematics == 2)
                newChar.setCinematic(1);                          // not show intro

            newChar.SetAtLoginFlag(AT_LOGIN_FIRST);               // First login

            // Player created, save it now
            newChar.SaveToDB(true);
            createInfo->CharCount += 1;

            SQLTransaction trans = LoginDatabase.BeginTransaction();

            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_REALM_CHARACTERS_BY_REALM);
            stmt->setUInt32(0, GetAccountId());
            stmt->setUInt32(1, realm.Id.Realm);
            trans->Append(stmt);

            stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_REALM_CHARACTERS);
            stmt->setUInt32(0, createInfo->CharCount);
            stmt->setUInt32(1, GetAccountId());
            stmt->setUInt32(2, realm.Id.Realm);
            trans->Append(stmt);

            LoginDatabase.CommitTransaction(trans);

            /*SendCharCreate(CHAR_CREATE_SUCCESS);*/

            std::string IP_str = GetRemoteAddress();
            sLog->outInfo(LOG_FILTER_CHARACTER, "Account: %d (IP: %s) Create Character:[%s] (GUID: %u)", GetAccountId(), IP_str.c_str(), createInfo->Name.c_str(), newChar.GetGUIDLow());
            sScriptMgr->OnPlayerCreate(&newChar);
            sWorld->AddCharacterNameData(newChar.GetGUIDLow(), std::string(newChar.GetName()), newChar.getGender(), newChar.getRace(), newChar.getClass(), newChar.getLevel());

            newChar.CleanupsBeforeDelete();

            //Try check succesfull creation
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHECK_GUID);
            stmt->setUInt64(0, newChar.GetGUIDLow());

            _charCreateCallback.FreeResult();
            _charCreateCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
            _charCreateCallback.NextStage();
            _charCreateCallback.InitTimer();    //set timer for manual waithing
        }
        break;
        case 4:
        default:
        {
            ASSERT(_charCreateCallback.GetParam().get() == createInfo);

            //Char create succesfull
            if (result)
            {
                SendCharCreate(CHAR_CREATE_SUCCESS);
                _charCreateCallback.Reset();
                return;
            }

            //Try more
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHECK_GUID);
            stmt->setUInt64(0, createInfo->guid.GetCounter());

            _charCreateCallback.FreeResult();
            _charCreateCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
            _charCreateCallback.NextStage();
            _charCreateCallback.InitTimer();    //set timer for manual waithing
        }
        break;
        case 20:
            SendCharCreate(CHAR_CREATE_ERROR);
            _charCreateCallback.Reset();
            break;
    }
}

void WorldSession::HandleCharDeleteOpcode(WorldPackets::Character::DeleteChar& charDelete)
{
    // can't delete loaded character
    if (ObjectAccessor::FindPlayer(charDelete.Guid))
        return;

    uint32 accountId = 0;
    std::string name;

    // is guild leader
    if (sGuildMgr->GetGuildByLeader(charDelete.Guid))
    {
        SendCharDelete(CHAR_DELETE_FAILED_GUILD_LEADER);
        return;
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_NAME_BY_GUID);
    stmt->setUInt64(0, charDelete.Guid.GetCounter());

    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
    {
        Field* fields = result->Fetch();
        accountId     = fields[0].GetUInt32();
        name          = fields[1].GetString();
    }

    // prevent deleting other players' characters using cheating tools
    if (accountId != GetAccountId())
        return;

    std::string IP_str = GetRemoteAddress();
    sLog->outInfo(LOG_FILTER_CHARACTER, "Account: %d (IP: %s) Delete Character:[%s] (GUID: %u)", GetAccountId(), IP_str.c_str(), name.c_str(), charDelete.Guid.GetGUIDLow());
    sScriptMgr->OnPlayerDelete(charDelete.Guid);
    sWorld->DeleteCharacterNameData(charDelete.Guid.GetCounter());

    if (sLog->ShouldLog(LOG_FILTER_PLAYER_DUMP, LOG_LEVEL_INFO)) // optimize GetPlayerDump call
    {
        std::string dump;
        if (PlayerDumpWriter().GetDump(charDelete.Guid.GetCounter(), dump))

            sLog->outCharDump(dump.c_str(), GetAccountId(), charDelete.Guid.GetCounter(), name.c_str());
    }

    sGuildFinderMgr->RemoveAllMembershipRequestsFromPlayer(charDelete.Guid);
    Player::DeleteFromDB(charDelete.Guid, GetAccountId());
    sWorld->DeleteCharName(name);

    SendCharDelete(CHAR_DELETE_SUCCESS);

    // reset timer.
    timeCharEnumOpcode = 0;
}

void WorldSession::HandlePlayerLoginOpcode(WorldPackets::Character::PlayerLogin& playerLogin)
{
    // Prevent flood of CMSG_PLAYER_LOGIN
    if (++playerLoginCounter > 10)
    {
        sLog->outError(LOG_FILTER_OPCODES, "Player kicked due to flood of CMSG_PLAYER_LOGIN");
        KickPlayer();
        return;
    }
    
    if (PlayerLoading() || GetPlayer() != NULL)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Player tries to login again, AccountId = %d", GetAccountId());
        return;
    }

    m_playerLoading = playerLogin.Guid;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Character (Guid: %s) logging in", playerLogin.Guid.ToString().c_str());

    if (!CharCanLogin(playerLogin.Guid.GetCounter()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Account (%u) can't login with that character (%u).", GetAccountId(), playerLogin.Guid.GetGUIDLow());
        KickPlayer();
        return;
    }

    LoginQueryHolder *holder = new LoginQueryHolder(GetAccountId(), playerLogin.Guid);
    if (!holder->Initialize())
    {
        delete holder;                                      // delete all unprocessed queries
        return;
    }

    _charLoginCallback = CharacterDatabase.DelayQueryHolder((SQLQueryHolder*)holder);
    SendConnectToInstance(WorldPackets::Auth::ConnectToSerial::WorldAttempt1);
}

void WorldSession::HandleLoadScreenOpcode(WorldPackets::Character::LoadingScreenNotify& /*loadingScreenNotify*/) { }

void WorldSession::HandlePlayerLogin(LoginQueryHolder* holder)
{
    ObjectGuid playerGuid = holder->GetGuid();

    Player* pCurrChar = new Player(this);
     // for send server info and strings (config)
    ChatHandler chH = ChatHandler(pCurrChar);

    // "GetAccountId() == db stored account id" checked in LoadFromDB (prevent login not own character using cheating tools)
    if (!pCurrChar->LoadFromDB(playerGuid, holder))
    {
        SetPlayer(NULL);
        KickPlayer();                                       // disconnect client, player no set to session and it will not deleted or saved at kick
        delete pCurrChar;                                   // delete it manually
        delete holder;                                      // delete all unprocessed queries
        m_playerLoading.Clear();
        return;
    }

    pCurrChar->GetMotionMaster()->Initialize();
    pCurrChar->SendDungeonDifficulty();

    WorldPackets::Character::LoginVerifyWorld loginVerifyWorld;
    loginVerifyWorld.MapID = pCurrChar->GetMapId();
    loginVerifyWorld.Pos = pCurrChar->GetPosition();
    SendPacket(loginVerifyWorld.Write());

    // load player specific part before send times
    LoadAccountData(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOADACCOUNTDATA), PER_CHARACTER_CACHE_MASK);

    // rewrite client channel mask if not valid
    if (AccountData* aData = GetAccountData(PER_CHARACTER_CHAT_CACHE))
    {
        std::string data = aData->Data;

        if (!data.empty())
        {
            boost::regex regEx("ZONECHANNELS[ \f\n\r\t\v][1-9][0-9]+");
            boost::smatch res;
            bool replace = false;
            if (boost::regex_search(data, res, regEx))
            {
                std::string m = res[0];
                std::string channel = m.substr(12, m.size());
                int channelMask = std::stoi(channel);

                if (channelMask != 0x2200003)
                {
                    // set now time for invalidate cache on client and forced request
                    aData->Time = uint32(sWorld->GetGameTime());
                    replace = true;
                }
            }

            if (replace)
                aData->Data = boost::regex_replace(data, regEx, "ZONECHANNELS 35651587");
        }
    }

    WorldPackets::ClientConfig::AccountDataTimes accountDataTimes;
    accountDataTimes.PlayerGuid = playerGuid;
    accountDataTimes.ServerTime = uint32(sWorld->GetGameTime());
    for (uint32 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
        accountDataTimes.AccountTimes[i] = uint32(GetAccountData(AccountDataType(i))->Time);

    SendPacket(accountDataTimes.Write());

    /// Send FeatureSystemStatus
    {
        WorldPackets::System::FeatureSystemStatus features;

        /// START OF DUMMY VALUES
        features.ComplaintStatus = 2;
        features.ScrollOfResurrectionRequestsRemaining = 1;
        features.ScrollOfResurrectionMaxRequestsPerDay = 1;
        features.CfgRealmID = 2;
        features.CfgRealmRecID = 0;
        features.VoiceEnabled = false;
        /// END OF DUMMY VALUES

        features.CharUndeleteEnabled = false/*sWorld->getBoolConfig(CONFIG_FEATURE_SYSTEM_CHARACTER_UNDELETE_ENABLED)*/;
        features.BpayStoreEnabled    = true/*sWorld->getBoolConfig(CONFIG_PURCHASE_SHOP_ENABLED)*/;
        features.BpayStoreAvailable = true;
        features.CommerceSystemEnabled = true;
        SendPacket(features.Write());
    }

    // Send MOTD
    {
        WorldPackets::System::MOTD motd;
        motd.Text = &sWorld->GetMotd();
        SendPacket(motd.Write());

        // send server info
        if (sWorld->getIntConfig(CONFIG_ENABLE_SINFO_LOGIN) == 1)
            chH.PSendSysMessage(_FULLVERSION);

        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent server info");
    }

    SendTimeZoneInformation();

    //QueryResult* result = CharacterDatabase.PQuery("SELECT guildid, rank FROM guild_member WHERE guid = '%u'", pCurrChar->GetGUIDLow());
    if (PreparedQueryResult resultGuild = holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOADGUILD))
    {
        Field* fields = resultGuild->Fetch();
        pCurrChar->SetInGuild(fields[0].GetUInt64());
        pCurrChar->SetRank(fields[1].GetUInt8());
        if (Guild* guild = sGuildMgr->GetGuildById(pCurrChar->GetGuildId()))
            pCurrChar->SetGuildLevel(guild->GetLevel());
    }
    else if (pCurrChar->GetGuildId())                        // clear guild related fields in case wrong data about non existed membership
    {
        pCurrChar->SetInGuild(UI64LIT(0));
        pCurrChar->SetRank(0);
        pCurrChar->SetGuildLevel(0);
    }

    // Send PVPSeason
    {
        WorldPackets::Battleground::PVPSeason season;
        season.PreviousSeason = sWorld->getIntConfig(CONFIG_ARENA_SEASON_ID) - sWorld->getBoolConfig(CONFIG_ARENA_SEASON_IN_PROGRESS);

        if (sWorld->getBoolConfig(CONFIG_ARENA_SEASON_IN_PROGRESS))
            season.CurrentSeason = sWorld->getIntConfig(CONFIG_ARENA_SEASON_ID);

        SendPacket(season.Write());
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent PVPSeason");
    }

    WorldPackets::Query::HotfixNotifyBlob hotfixInfo;
    hotfixInfo.Hotfixes = sDB2Manager.GetHotfixData();
    SendPacket(hotfixInfo.Write());

    pCurrChar->SendInitialPacketsBeforeAddToMap();

    //Show cinematic at the first time that player login
    if (!pCurrChar->getCinematic())
    {
        pCurrChar->setCinematic(1);

        if (ChrClassesEntry const* cEntry = sChrClassesStore.LookupEntry(pCurrChar->getClass()))
        {
            if (cEntry->CinematicSequenceID)
                pCurrChar->SendCinematicStart(cEntry->CinematicSequenceID);
            else if (ChrRacesEntry const* rEntry = sChrRacesStore.LookupEntry(pCurrChar->getRace()))
                pCurrChar->SendCinematicStart(rEntry->CinematicSequenceID);

            // send new char string if not empty
            if (!sWorld->GetNewCharString().empty())
                chH.PSendSysMessage("%s", sWorld->GetNewCharString().c_str());
        }
    }

    if (!pCurrChar->GetMap()->AddPlayerToMap(pCurrChar) || !pCurrChar->GetMap()->IsGarrison() && !pCurrChar->CheckInstanceLoginValid())
    {
        AreaTriggerStruct const* at = sObjectMgr->GetGoBackTrigger(pCurrChar->GetMapId());
        if (at)
            pCurrChar->TeleportTo(at->target_mapId, at->target_X, at->target_Y, at->target_Z, pCurrChar->GetOrientation());
        else
            pCurrChar->TeleportTo(pCurrChar->m_homebindMapId, pCurrChar->m_homebindX, pCurrChar->m_homebindY, pCurrChar->m_homebindZ, pCurrChar->GetOrientation());
    }

    sObjectAccessor->AddObject(pCurrChar);
    //sLog->outDebug(LOG_FILTER_GENERAL, "Player %s added to Map.", pCurrChar->GetName());

    if (pCurrChar->GetGuildId() != 0)
    {
        if (Guild* guild = sGuildMgr->GetGuildById(pCurrChar->GetGuildId()))
            guild->SendLoginInfo(this);
        else
        {
            // remove wrong guild data
            sLog->outError(LOG_FILTER_GENERAL, "Player %s (GUID: %u) marked as member of not existing guild (id: %u), removing guild membership for player.", pCurrChar->GetName(), pCurrChar->GetGUIDLow(), pCurrChar->GetGuildId());
            pCurrChar->SetInGuild(0);
        }
    }

    pCurrChar->SendInitialPacketsAfterAddToMap();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_ONLINE);
    stmt->setUInt32(0, pCurrChar->GetGUIDLow());
    CharacterDatabase.Execute(stmt);

    stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_ONLINE);
    stmt->setUInt32(0, GetAccountId());
    LoginDatabase.Execute(stmt);

    pCurrChar->SetInGameTime(getMSTime());

    if (Group* group = pCurrChar->GetGroup())
    {
        group->SendUpdate();
        group->ResetMaxEnchantingLevel();
    }

    // friend status
    sSocialMgr->SendFriendStatus(pCurrChar, FRIEND_ONLINE, pCurrChar->GetGUID(), true);

    // Place character in world (and load zone) before some object loading
    pCurrChar->LoadCorpse();

    // setting Ghost+speed if dead
    if (pCurrChar->m_deathState != ALIVE)
    {
        // not blizz like, we must correctly save and load player instead...
        if (pCurrChar->getRace() == RACE_NIGHTELF)
            pCurrChar->CastSpell(pCurrChar, 20584, true, 0);// auras SPELL_AURA_INCREASE_SPEED(+speed in wisp form), SPELL_AURA_INCREASE_SWIM_SPEED(+swim speed in wisp form), SPELL_AURA_TRANSFORM (to wisp form)
        pCurrChar->CastSpell(pCurrChar, 8326, true, 0);     // auras SPELL_AURA_GHOST, SPELL_AURA_INCREASE_SPEED(why?), SPELL_AURA_INCREASE_SWIM_SPEED(why?)

        pCurrChar->SetWaterWalking(true);
    }

    pCurrChar->ContinueTaxiFlight();
    pCurrChar->LoadPet();

    // Set FFA PvP for non GM in non-rest mode
    if (sWorld->IsFFAPvPRealm() && !pCurrChar->isGameMaster() && !pCurrChar->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_RESTING))
        pCurrChar->SetByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);

    if (pCurrChar->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_CONTESTED_PVP))
        pCurrChar->SetContestedPvP();

    // Apply at_login requests
    if (pCurrChar->HasAtLoginFlag(AT_LOGIN_RESET_SPELLS))
    {
        pCurrChar->resetSpells();
        SendNotification(LANG_RESET_SPELLS);
    }

    if (pCurrChar->HasAtLoginFlag(AT_LOGIN_RESET_TALENTS))
    {
        pCurrChar->ResetTalentSpecialization();
        pCurrChar->ResetTalents(true);
        pCurrChar->SendTalentsInfoData(false);              // original talents send already in to SendInitialPacketsBeforeAddToMap, resend reset state
        SendNotification(LANG_RESET_TALENTS);
    }

    if (pCurrChar->HasAtLoginFlag(AT_LOGIN_FIRST))
        pCurrChar->RemoveAtLoginFlag(AT_LOGIN_FIRST);

    // show time before shutdown if shutdown planned.
    if (sWorld->IsShuttingDown())
        sWorld->ShutdownMsg(true, pCurrChar);

    if (sWorld->getBoolConfig(CONFIG_ALL_TAXI_PATHS))
        pCurrChar->SetTaxiCheater(true);

    if (pCurrChar->isGameMaster())
        SendNotification(LANG_GM_ON);

    // Send CUF profiles (new raid UI 4.2)
    SendLoadCUFProfiles();

    // Hackfix Remove Talent spell - Remove Glyph spell
    pCurrChar->learnSpell(111621, false); // Reset Glyph
    pCurrChar->learnSpell(113873, false); // Reset Talent

    std::string IP_str = GetRemoteAddress();
    sLog->outInfo(LOG_FILTER_CHARACTER, "Account: %d (IP: %s) Login Character:[%s] (GUID: %u) Level: %d",
        GetAccountId(), IP_str.c_str(), pCurrChar->GetName(), pCurrChar->GetGUIDLow(), pCurrChar->getLevel());

    if (!pCurrChar->IsStandState() && !pCurrChar->HasUnitState(UNIT_STATE_STUNNED))
        pCurrChar->SetStandState(UNIT_STAND_STATE_STAND);

    m_playerLoading.Clear();

    sScriptMgr->OnPlayerLogin(pCurrChar);

    sBattlenetServer.SendChangeToonOnlineState(GetBattlenetAccountId(), GetAccountId(), _player->GetGUID(), _player->GetName(), true);

    delete holder;
}

void WorldSession::HandleSetFactionAtWar(WorldPackets::Character::SetFactionAtWar& packet)
{
    GetPlayer()->GetReputationMgr().SetAtWar(packet.FactionIndex, true);
}

void WorldSession::HandleUnsetFactionAtWar(WorldPackets::Character::SetFactionNotAtWar& packet)
{
    GetPlayer()->GetReputationMgr().SetAtWar(packet.FactionIndex, false);
}

//I think this function is never used :/ I dunno, but i guess this opcode not exists
void WorldSession::HandleSetFactionCheat(WorldPacket & /*recvData*/)
{
    sLog->outError(LOG_FILTER_NETWORKIO, "WORLD SESSION: HandleSetFactionCheat, not expected call, please report.");
    GetPlayer()->GetReputationMgr().SendStates();
}

void WorldSession::HandleTutorialFlag(WorldPackets::Misc::TutorialSetFlag& packet)
{
    switch (packet.Action)
    {
        case TUTORIAL_ACTION_UPDATE:
        {
            uint8 index = uint8(packet.TutorialBit >> 5);
            if (index >= MAX_ACCOUNT_TUTORIAL_VALUES)
                return;

            uint32 flag = GetTutorialInt(index);
            flag |= (1 << (packet.TutorialBit & 0x1F));
            SetTutorialInt(index, flag);
            break;
        }
        case TUTORIAL_ACTION_CLEAR:
            for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
                SetTutorialInt(i, 0xFFFFFFFF);
            break;
        case TUTORIAL_ACTION_RESET:
            for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
                SetTutorialInt(i, 0x00000000);
            break;
        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "CMSG_TUTORIAL received unknown TutorialAction %u.", packet.Action);
            return;
    }
}

void WorldSession::HandleSetWatchedFaction(WorldPackets::Character::SetWatchedFaction& packet)
{
    GetPlayer()->SetUInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX, packet.FactionIndex);
}

void WorldSession::HandleSetFactionInactive(WorldPackets::Character::SetFactionInactive& packet)
{
    _player->GetReputationMgr().SetInactive(packet.Index, packet.State);
}

void WorldSession::HandleShowingHelm(WorldPackets::Character::ShowingHelm& /*packet*/)
{
    _player->ToggleFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM);
}

void WorldSession::HandleShowingCloak(WorldPackets::Character::ShowingCloak& /*packet*/)
{
    _player->ToggleFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_CLOAK);
}

void WorldSession::HandleCharacterRenameRequest(WorldPackets::Character::CharacterRenameRequest& packet)
{
    if (!normalizePlayerName(packet.RenameInfo->NewName))
    {
        WorldPackets::Character::CharacterRenameResult result;
        result.Result = CHAR_NAME_NO_NAME;
        result.Name = packet.RenameInfo->NewName;
        SendPacket(result.Write());
        return;
    }

    uint8 res = ObjectMgr::CheckPlayerName(packet.RenameInfo->NewName, true);
    if (res != CHAR_NAME_SUCCESS)
    {
        WorldPackets::Character::CharacterRenameResult result;
        result.Result = res;
        result.Name = packet.RenameInfo->NewName;
        SendPacket(result.Write());
        return;
    }

    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(packet.RenameInfo->NewName))
    {
        WorldPackets::Character::CharacterRenameResult result;
        result.Result = CHAR_NAME_RESERVED;
        result.Name = packet.RenameInfo->NewName;
        SendPacket(result.Write());
        return;
    }

    // Ensure that the character belongs to the current account, that rename at login is enabled
    // and that there is no character with the desired new name
    _charRenameCallback.SetParam(packet.RenameInfo->NewName);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_FREE_NAME);

    stmt->setUInt32(0, packet.RenameInfo->Guid.GetCounter());
    stmt->setUInt32(1, GetAccountId());
    stmt->setUInt16(2, AT_LOGIN_RENAME);
    stmt->setUInt16(3, AT_LOGIN_RENAME);
    stmt->setString(4, packet.RenameInfo->NewName);

    _charRenameCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::HandleChangePlayerNameOpcodeCallBack(PreparedQueryResult result, std::string newName)
{
    if (!result)
    {
        WorldPackets::Character::CharacterRenameResult packet;
        packet.Result = CHAR_CREATE_ERROR;
        packet.Name = newName;
        SendPacket(packet.Write());
        return;
    }

    Field* fields = result->Fetch();

    ObjectGuid::LowType guidLow = fields[0].GetUInt64();
    std::string oldName = fields[1].GetString();

    ObjectGuid guid = ObjectGuid::Create<HighGuid::Player>(guidLow);

    // Update name and at_login flag in the db
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_NAME);

    stmt->setString(0, newName);
    stmt->setUInt16(1, AT_LOGIN_RENAME);
    stmt->setUInt64(2, guidLow);

    CharacterDatabase.Execute(stmt);

    // Removed declined name from db
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_DECLINED_NAME);

    stmt->setUInt64(0, guidLow);

    CharacterDatabase.Execute(stmt);

    // Logging
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_NAME_LOG);

    stmt->setUInt64(0, guidLow);
    stmt->setString(1, oldName);
    stmt->setString(2, newName);

    CharacterDatabase.Execute(stmt);

    WorldPackets::Character::CharacterRenameResult packet;
    packet.Result = RESPONSE_SUCCESS;
    packet.Name = newName;
    packet.Guid = guid;
    SendPacket(packet.Write());
    sWorld->UpdateCharacterNameData(guidLow, newName);
}

void WorldSession::HandleSetPlayerDeclinedNames(WorldPacket& recvData)
{
    if (!sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
    {
        recvData.rfinish();
        //! 5.4.1
        WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4+8);
        data << uint32(0);                                      // OK
        data.WriteBits(0, 9);   //epmpty guid + unk bit
        data.FlushBits();
        SendPacket(&data);
        return;
    }

    ObjectGuid guid;
    DeclinedName declinedname;

    //recvData.ReadGuidMask<5, 1, 3, 0, 6, 4>(guid);

    uint32 declinedNamesLength[MAX_DECLINED_NAME_CASES];
    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
        declinedNamesLength[i] = recvData.ReadBits(7);

    //recvData.ReadGuidMask<2, 7>(guid);
    //recvData.ReadGuidBytes<6, 1, 2>(guid);

    bool decl_checl = true;
    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
    {
        declinedname.name[i] = recvData.ReadString(declinedNamesLength[i]);
        if (!normalizePlayerName(declinedname.name[i]))
            decl_checl = false;
    }

    //recvData.ReadGuidBytes<3, 4, 0, 5, 7>(guid);

    if (!decl_checl)
    {
        //! 5.4.1
        WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4+8);
        data << uint32(1);
        data.WriteBit(bool(guid));
        //data.WriteGuidMask<0, 1, 3, 7, 5, 4, 2, 6>(guid);
        data.FlushBits();
        //data.WriteGuidBytes<6, 4, 5, 0, 2, 7, 1, 3>(guid);
        SendPacket(&data);
        return;
    }

    // not accept declined names for unsupported languages
    std::string name;
    if (!ObjectMgr::GetPlayerNameByGUID(guid, name))
    {
        //! 5.4.1
        WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4+8);
        data << uint32(1);
        data.WriteBit(bool(guid));
        //data.WriteGuidMask<0, 1, 3, 7, 5, 4, 2, 6>(guid);
        data.FlushBits();
        //data.WriteGuidBytes<6, 4, 5, 0, 2, 7, 1, 3>(guid);
        SendPacket(&data);
        return;
    }

    std::wstring wname;
    if (!Utf8toWStr(name, wname) || !isCyrillicCharacter(wname[0]))
    {
        //! 5.4.1
        WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4+8);
        data << uint32(1);
        data.WriteBit(bool(guid));
        //data.WriteGuidMask<0, 1, 3, 7, 5, 4, 2, 6>(guid);
        data.FlushBits();
        //data.WriteGuidBytes<6, 4, 5, 0, 2, 7, 1, 3>(guid);
        SendPacket(&data);
        return;
    }

    if (!ObjectMgr::CheckDeclinedNames(wname, declinedname))
    {
        //5.4.1
        WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4+8);
        data << uint32(1);
        data.WriteBit(bool(guid));
        //data.WriteGuidMask<0, 1, 3, 7, 5, 4, 2, 6>(guid);
        data.FlushBits();
        //data.WriteGuidBytes<6, 4, 5, 0, 2, 7, 1, 3>(guid);
        SendPacket(&data);
        return;
    }

    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
        CharacterDatabase.EscapeString(declinedname.name[i]);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_DECLINED_NAME);
    stmt->setUInt64(0, guid.GetCounter());
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_DECLINED_NAME);
    stmt->setUInt64(0, guid.GetCounter());

    for (uint8 i = 0; i < 5; ++i)
        stmt->setString(i+1, declinedname.name[i]);

    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);

    //! 5.4.1
    WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4+8);
    data << uint32(0);                                      // OK
    data.WriteBits(0, 9);   //epmpty guid + unk bit
    data.FlushBits();
    SendPacket(&data);
}

void WorldSession::HandleAlterAppearance(WorldPackets::Character::AlterApperance& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    BarberShopStyleEntry const* bs_hair = sBarberShopStyleStore.LookupEntry(packet.NewHairStyle);
    if (!bs_hair || bs_hair->type != 0 || bs_hair->race != player->getRace() || bs_hair->gender != player->getGender())
        return;

    BarberShopStyleEntry const* bs_facialHair = sBarberShopStyleStore.LookupEntry(packet.NewFacialHair);
    if (!bs_facialHair || bs_facialHair->type != 2 || bs_facialHair->race != player->getRace() || bs_facialHair->gender != player->getGender())
        return;

    BarberShopStyleEntry const* bs_skinColor = sBarberShopStyleStore.LookupEntry(packet.NewSkinColor);
    if (bs_skinColor && (bs_skinColor->type != 3 || bs_skinColor->race != player->getRace() || bs_skinColor->gender != player->getGender()))
        return;

    BarberShopStyleEntry const* bs_face = sBarberShopStyleStore.LookupEntry(packet.NewFace);
    if (bs_face && (bs_face->type != 4 || bs_face->race != player->getRace() || bs_face->gender != player->getGender()))
        return;
    
    GameObject* go = player->FindNearestGameObjectOfType(GAMEOBJECT_TYPE_BARBER_CHAIR, 5.0f);
    if (!go)
    {
        SendPacket(WorldPackets::Character::BarberShopResultServer(BARBER_SHOP_RESULT_NOT_ON_CHAIR).Write());
        return;
    }

    if (player->getStandState() != UNIT_STAND_STATE_SIT_LOW_CHAIR + go->GetGOInfo()->barberChair.chairheight)
    {
        SendPacket(WorldPackets::Character::BarberShopResultServer(BARBER_SHOP_RESULT_NOT_ON_CHAIR).Write());
        return;
    }

    uint32 cost = player->GetBarberShopCost(bs_hair->hair_id, packet.NewSkinColor, bs_facialHair->hair_id, bs_skinColor);

    if (!player->HasEnoughMoney((uint64)cost))
    {
        SendPacket(WorldPackets::Character::BarberShopResultServer(BARBER_SHOP_RESULT_NO_MONEY).Write());
        return;
    }
    else
        SendPacket(WorldPackets::Character::BarberShopResultServer(BARBER_SHOP_RESULT_SUCCESS).Write());

    player->ModifyMoney(-int64(cost));
    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER, cost);

    player->SetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_STYLE_ID, uint8(bs_hair->hair_id));
    player->SetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID, packet.NewSkinColor);
    player->SetByteValue(PLAYER_BYTES_2, PLAYER_BYTES_2_OFFSET_FACIAL_STYLE, uint8(bs_facialHair->hair_id));
    if (bs_skinColor)
        player->SetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_SKIN_ID, uint8(bs_skinColor->hair_id));
    if (bs_face)
        player->SetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_FACE_ID, uint8(bs_face->Id));

    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP, 1);

    player->SetStandState(UNIT_STAND_STATE_STAND);
}

void WorldSession::HandleRemoveGlyph(WorldPacket & recvData)
{
    uint32 slot;
    recvData >> slot;

    if (slot >= MAX_GLYPH_SLOT_INDEX)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Client sent wrong glyph slot number in opcode CMSG_REMOVE_GLYPH %u", slot);
        return;
    }

    if (uint32 glyph = _player->GetGlyph(_player->GetActiveSpec(), slot))
    {
        if (GlyphPropertiesEntry const* gp = sGlyphPropertiesStore.LookupEntry(glyph))
        {
            _player->removeSpell(gp->SpellId);
            //_player->RemoveAurasDueToSpell(gp->SpellId);
            _player->SetGlyph(slot, 0);
            _player->SendTalentsInfoData(false);
        }
    }
}

void WorldSession::HandleSaveEquipmentSet(WorldPackets::EquipmentSet::SaveEquipmentSet& packet)
{
    if (packet.Set.SetID >= MAX_EQUIPMENT_SET_INDEX)
        return;

    for (uint8 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        if (!(packet.Set.IgnoreMask & (1 << i)))
        {
            Item* item = _player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);

            if (!item && !packet.Set.Pieces[i].IsEmpty())
                return;

            if (item && item->GetGUID() != packet.Set.Pieces[i])
                return;
        }
        else
            packet.Set.Pieces[i].Clear();
    }

    packet.Set.IgnoreMask &= 0x7FFFF;

    _player->SetEquipmentSet(std::move(packet.Set));
}

void WorldSession::HandleDeleteEquipmentSet(WorldPackets::EquipmentSet::DeleteEquipmentSet& packet)
{
    _player->DeleteEquipmentSet(packet.ID);
}

void WorldSession::HandleEquipmentSetUse(WorldPackets::EquipmentSet::UseEquipmentSet& packet)
{
    ObjectGuid ignoredItemGuid;
    ignoredItemGuid.SetRawValue(0, 1);

    for (uint8 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        if (packet.Items[i].Item == ignoredItemGuid)
            continue;

        if (_player->isInCombat() && i != EQUIPMENT_SLOT_MAINHAND && i != EQUIPMENT_SLOT_OFFHAND)
            continue;

        Item* item = _player->GetItemByGuid(packet.Items[i].Item);

        uint16 dstPos = i | (INVENTORY_SLOT_BAG_0 << 8);

        if (!item)
        {
            Item* uItem = _player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (!uItem)
                continue;

            ItemPosCountVec itemPosCountVec;
            InventoryResult inventoryResult = _player->CanStoreItem(NULL_BAG, NULL_SLOT, itemPosCountVec, uItem, false);
            if (inventoryResult == EQUIP_ERR_OK)
            {
                _player->RemoveItem(INVENTORY_SLOT_BAG_0, i, true);
                _player->StoreItem(itemPosCountVec, uItem, true);
            }
            else
                _player->SendEquipError(inventoryResult, uItem,  NULL);
            
            continue;
        }

        if (item->GetPos() == dstPos)
            continue;

        _player->SwapItem(item->GetPos(), dstPos);
    }

    WorldPackets::EquipmentSet::UseEquipmentSetResult result;
    result.Reason = 0; // 4 - inventory is full, 0 - ok, else failed                                     
    SendPacket(result.Write());
}

//! 6.0.3
void WorldSession::HandleCharCustomize(WorldPacket& recvData)
{
    ObjectGuid guid;
    std::string newName;

    uint8 gender, skin, face, hairStyle, hairColor, facialHair;
    recvData >> guid >> gender >> skin >> hairColor >> hairStyle >> facialHair >> face;

    uint32 newNameLen = recvData.ReadBits(6);
    newName = recvData.ReadString(newNameLen);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_AT_LOGIN);
    stmt->setUInt64(0, guid.GetCounter());
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE_FAILED, 17);
        data << uint8(CHAR_CREATE_ERROR);
        data << guid;
        SendPacket(&data);
        return;
    }

    Field* fields = result->Fetch();
    uint32 at_loginFlags = fields[0].GetUInt16();

    if (!(at_loginFlags & AT_LOGIN_CUSTOMIZE))
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE_FAILED, 17);
        data << uint8(CHAR_CREATE_ERROR);
        data << guid;
        SendPacket(&data);
        return;
    }

    // prevent character rename to invalid name
    if (!normalizePlayerName(newName))
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE_FAILED, 17);
        data << uint8(CHAR_NAME_NO_NAME);
        data << guid;
        SendPacket(&data);
        return;
    }

    uint8 res = ObjectMgr::CheckPlayerName(newName, true);
    if (res != CHAR_NAME_SUCCESS)
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE_FAILED, 1);
        data << uint8(res);
        data << guid;
        SendPacket(&data);
        return;
    }

    // check name limitations
    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(newName))
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE_FAILED, 17);
        data << uint8(CHAR_NAME_RESERVED);
        data << guid;
        SendPacket(&data);
        return;
    }

    // character with this name already exist
    if (ObjectGuid newguid = ObjectMgr::GetPlayerGUIDByName(newName))
    {
        if (newguid != guid)
        {
            WorldPacket data(SMSG_CHAR_CUSTOMIZE_FAILED, 17);
            data << uint8(CHAR_CREATE_NAME_IN_USE);
            data << guid;
            SendPacket(&data);
            return;
        }
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_NAME);
    stmt->setUInt64(0, guid.GetCounter());
    result = CharacterDatabase.Query(stmt);

    if (result)
    {
        std::string oldname = result->Fetch()[0].GetString();
        sLog->outInfo(LOG_FILTER_CHARACTER, "Account: %d (IP: %s), Character[%s] (guid:%u) Customized to: %s", GetAccountId(), GetRemoteAddress().c_str(), oldname.c_str(), guid.GetGUIDLow(), newName.c_str());
    }

    Player::Customize(guid, gender, skin, face, hairStyle, hairColor, facialHair);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_NAME_AT_LOGIN);

    stmt->setString(0, newName);
    stmt->setUInt16(1, uint16(AT_LOGIN_CUSTOMIZE));
    stmt->setUInt64(2, guid.GetCounter());

    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_DECLINED_NAME);

    stmt->setUInt64(0, guid.GetCounter());

    CharacterDatabase.Execute(stmt);

    sWorld->UpdateCharacterNameData(guid.GetCounter(), newName, gender);

    /*WorldPacket data(SMSG_CHAR_CUSTOMIZE_FAILED, 17);
    data << uint8(RESPONSE_SUCCESS);
    data << guid;
    SendPacket(&data);*/

    WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1+8+(newName.size()+1)+6);
    data << guid;
    data.WriteBits(newName.length(), 6);

    data << uint8(gender);
    data << uint8(skin);
    data << uint8(hairColor);
    data << uint8(hairStyle);
    data << uint8(facialHair);
    data << uint8(face);

    data.WriteString(newName);
    SendPacket(&data);
}

void WorldSession::HandleCharFactionOrRaceChange(WorldPacket& recvData)
{
    // TODO: Move queries to prepared statements
    ObjectGuid guid;
    std::string newname;
    uint8 gender, skin, face, hairStyle, hairColor, facialHair, race = 0;

    bool isFactionChange = recvData.ReadBit();
    uint32 len = recvData.ReadBits(6);

    bool bit93 = recvData.ReadBit();
    bool bit96 = recvData.ReadBit();
    bool bit89 = recvData.ReadBit();
    bool bit17 = recvData.ReadBit();
    bool bit19 = recvData.ReadBit();

    recvData >> guid >> gender >> race;
    newname = recvData.ReadString(len);

    skin = bit93 ? recvData.read<uint8>() : 0;
    hairColor = bit96 ? recvData.read<uint8>() : 0;
    hairStyle = bit89 ? recvData.read<uint8>() : 0;
    facialHair = bit17 ? recvData.read<uint8>() : 0;
    face = bit19 ? recvData.read<uint8>() : 0;

    ObjectGuid::LowType lowGuid = guid.GetCounter();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_CLASS_LVL_AT_LOGIN);

    stmt->setUInt64(0, lowGuid);

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        WorldPacket data(SMSG_CHAR_FACTION_CHANGE_RESULT, 5);
        data << uint8(CHAR_CREATE_ERROR);
        data << guid;
        data.WriteBit(0);
        SendPacket(&data);
        return;
    }

    Field* fields = result->Fetch();
    uint8  oldRace          = fields[0].GetUInt8();
    uint32 playerClass      = uint32(fields[1].GetUInt8());
    uint32 level            = uint32(fields[2].GetUInt8());
    uint32 at_loginFlags    = fields[3].GetUInt16();
    uint32 used_loginFlag   = isFactionChange ? AT_LOGIN_CHANGE_FACTION : AT_LOGIN_CHANGE_RACE;
    char const* knownTitlesStr = fields[4].GetCString();

    TeamId oldTeam = TEAM_ALLIANCE;

    // Search each faction is targeted
    switch (oldRace)
    {
        case RACE_ORC:
        case RACE_GOBLIN:
        case RACE_TAUREN:
        case RACE_UNDEAD_PLAYER:
        case RACE_TROLL:
        case RACE_BLOODELF:
        case RACE_PANDAREN_HORDE:
        	oldTeam = TEAM_HORDE;
            break;
        default:
            break;
    }

    // We need to correct race when it's pandaren
    // Because client always send neutral ID
    if (race == RACE_PANDAREN_NEUTRAL)
    {
    	if (used_loginFlag == AT_LOGIN_CHANGE_RACE)
    		race = oldTeam == TEAM_ALLIANCE ? RACE_PANDAREN_ALLIANCE : RACE_PANDAREN_HORDE;
    	else
    		race = oldTeam == TEAM_ALLIANCE ? RACE_PANDAREN_HORDE : RACE_PANDAREN_ALLIANCE;
    }

    if (!sObjectMgr->GetPlayerInfo(race, playerClass))
    {
        WorldPacket data(SMSG_CHAR_FACTION_CHANGE_RESULT, 5);
        data << uint8(CHAR_CREATE_ERROR);
        data << guid;
        data.WriteBit(0);
        SendPacket(&data);
        return;
    }

    if (!(at_loginFlags & used_loginFlag))
    {
        WorldPacket data(SMSG_CHAR_FACTION_CHANGE_RESULT, 5);
        data << uint8(CHAR_CREATE_ERROR);
        data << guid;
        data.WriteBit(0);
        SendPacket(&data);
        return;
    }

    if (AccountMgr::IsPlayerAccount(GetSecurity()))
    {
        uint32 raceMaskDisabled = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_DISABLED_RACEMASK);
        if ((1 << (race - 1)) & raceMaskDisabled)
        {
            WorldPacket data(SMSG_CHAR_FACTION_CHANGE_RESULT, 5);
            data << uint8(CHAR_CREATE_ERROR);
            data << guid;
            data.WriteBit(0);
            SendPacket(&data);
            return;
        }
    }

    // prevent character rename to invalid name
    if (!normalizePlayerName(newname))
    {
        WorldPacket data(SMSG_CHAR_FACTION_CHANGE_RESULT, 5);
        data << uint8(CHAR_NAME_NO_NAME);
        data << guid;
        data.WriteBit(0);
        SendPacket(&data);
        return;
    }

    uint8 res = ObjectMgr::CheckPlayerName(newname, true);
    if (res != CHAR_NAME_SUCCESS)
    {
        WorldPacket data(SMSG_CHAR_FACTION_CHANGE_RESULT, 5);
        data << uint8(res);
        data << guid;
        data.WriteBit(0);
        SendPacket(&data);
        return;
    }

    // check name limitations
    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(newname))
    {
        WorldPacket data(SMSG_CHAR_FACTION_CHANGE_RESULT, 5);
        data << uint8(CHAR_NAME_RESERVED);
        data << guid;
        data.WriteBit(0);
        SendPacket(&data);
        return;
    }

    // character with this name already exist
    ObjectGuid newguid = ObjectMgr::GetPlayerGUIDByName(newname);
    if (!newguid.IsEmpty())
    {
        if (newguid != guid)
        {
            WorldPacket data(SMSG_CHAR_FACTION_CHANGE_RESULT, 5);
            data << uint8(CHAR_CREATE_NAME_IN_USE);
            data << guid;
            data.WriteBit(0);
            SendPacket(&data);
            return;
        }
    }

    CharacterDatabase.EscapeString(newname);
    Player::Customize(guid, gender, skin, face, hairStyle, hairColor, facialHair);
    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_FACTION_OR_RACE);
    stmt->setString(0, newname);
    stmt->setUInt8 (1, race);
    stmt->setUInt16(2, used_loginFlag);
    stmt->setUInt64(3, lowGuid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_FACTION_OR_RACE_LOG);
    stmt->setUInt64(0, lowGuid);
    stmt->setUInt32(1, GetAccountId());
    stmt->setUInt8 (2, oldRace);
    stmt->setUInt8 (3, race);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_DECLINED_NAME);
    stmt->setUInt64(0, lowGuid);
    trans->Append(stmt);

    sWorld->UpdateCharacterNameData(guid.GetCounter(), newname, gender, race);

    TeamId team = TEAM_ALLIANCE;

    // Search each faction is targeted
    switch (race)
    {
        case RACE_ORC:
        case RACE_GOBLIN:
        case RACE_TAUREN:
        case RACE_UNDEAD_PLAYER:
        case RACE_TROLL:
        case RACE_BLOODELF:
        case RACE_PANDAREN_HORDE:
            team = TEAM_HORDE;
            break;
        default:
            break;
    }

    // Switch Languages
    // delete all languages first
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SKILL_LANGUAGES);
    stmt->setUInt64(0, lowGuid);
    trans->Append(stmt);

    // Now add them back
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_SKILL_LANGUAGE);
    stmt->setUInt64(0, lowGuid);

    // Faction specific languages
    if (team == TEAM_HORDE)
        stmt->setUInt16(1, 109);
    else
        stmt->setUInt16(1, 98);

    trans->Append(stmt);

    // Race specific languages

    if (race != RACE_ORC && race != RACE_HUMAN)
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_SKILL_LANGUAGE);
        stmt->setUInt64(0, lowGuid);

        switch (race)
        {
            case RACE_DWARF:
                stmt->setUInt16(1, 111);
                break;
            case RACE_DRAENEI:
                stmt->setUInt16(1, 759);
                break;
            case RACE_GNOME:
                stmt->setUInt16(1, 313);
                break;
            case RACE_NIGHTELF:
                stmt->setUInt16(1, 113);
                break;
            case RACE_WORGEN:
                stmt->setUInt16(1, 791);
                break;
            case RACE_UNDEAD_PLAYER:
                stmt->setUInt16(1, 673);
                break;
            case RACE_TAUREN:
                stmt->setUInt16(1, 115);
                break;
            case RACE_TROLL:
                stmt->setUInt16(1, 315);
                break;
            case RACE_BLOODELF:
                stmt->setUInt16(1, 137);
                break;
            case RACE_GOBLIN:
                stmt->setUInt16(1, 792);
                break;
            case RACE_PANDAREN_ALLIANCE:
                stmt->setUInt16(1, 906);
                break;
            case RACE_PANDAREN_HORDE:
                stmt->setUInt16(1, 907);
                break;
            case RACE_PANDAREN_NEUTRAL:
                stmt->setUInt16(1, 905);
                break;
        }

        trans->Append(stmt);
    }

    if (used_loginFlag == AT_LOGIN_CHANGE_FACTION)
    {
        // Delete all Flypaths
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TAXI_PATH);
        stmt->setUInt64(0, lowGuid);
        trans->Append(stmt);

        if (level > 7)
        {
            // Update Taxi path
            // this doesn't seem to be 100% blizzlike... but it can't really be helped.
                std::ostringstream taximaskstream;
                TaxiMask const& factionMask = team == TEAM_HORDE ? sHordeTaxiNodesMask : sAllianceTaxiNodesMask;
                for (uint8 i = 0; i < TaxiMaskSize; ++i)
                {
                    // i = (315 - 1) / 8 = 39
                    // m = 1 << ((315 - 1) % 8) = 4
                    uint8 deathKnightExtraNode = playerClass != CLASS_DEATH_KNIGHT || i != 39 ? 0 : 4;
                    taximaskstream << uint32(factionMask[i] | deathKnightExtraNode) << ' ';
                }

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TAXIMASK);
            stmt->setString(0, taximaskstream.str());
            stmt->setUInt64(1, lowGuid);
            trans->Append(stmt);
        }

        // Delete all current quests
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_QUESTSTATUS);
        stmt->setUInt64(0, guid.GetCounter());
        trans->Append(stmt);

        // Delete record of the faction old completed quests
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_QUESTSTATUSREW_NON_ACC);
            stmt->setUInt32(0, GetAccountId());
            stmt->setUInt64(1, lowGuid);
            PreparedQueryResult result = CharacterDatabase.Query(stmt);

            if (result)
            {
                std::ostringstream quests;
                do
                {
                    Field* fields = result->Fetch();
                    if (Quest const* qinfo = sObjectMgr->GetQuestTemplate(fields[0].GetUInt32()))
                    {
                        uint32 requiredRaces = qinfo->GetAllowableRaces();
                        if (!requiredRaces)
                            continue;

                        if (team == TEAM_ALLIANCE)
                        {
                            if (requiredRaces & RACEMASK_ALLIANCE && (requiredRaces & RACEMASK_HORDE) == 0)
                            {
                                quests << uint32(qinfo->GetQuestId());
                                quests << ',';
                            }
                        }
                        else // if (team == TEAM_HORDE)
                        {
                            if (requiredRaces & RACEMASK_HORDE && (requiredRaces & RACEMASK_ALLIANCE) == 0)
                            {
                                quests << uint32(qinfo->GetQuestId());
                                quests << ',';
                            }
                        }
                    }

                } while (result->NextRow());

                std::string questsStr = quests.str();
                questsStr = questsStr.substr(0, questsStr.length() - 1);

                if (!questsStr.empty())
                    trans->PAppend("DELETE FROM `character_queststatus_rewarded` WHERE guid='%u' AND quest IN (%s)", lowGuid, questsStr.c_str());
            }
        }

        if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD))
        {
            // Reset guild
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_MEMBER);

            stmt->setUInt64(0, lowGuid);

            PreparedQueryResult result = CharacterDatabase.Query(stmt);
            if (result)
                if (Guild* guild = sGuildMgr->GetGuildById((result->Fetch()[0]).GetUInt64()))
                    guild->DeleteMember(ObjectGuid::Create<HighGuid::Player>(lowGuid));
        }

        if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_ADD_FRIEND))
        {
            // Delete Friend List
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SOCIAL_BY_GUID);
            stmt->setUInt64(0, lowGuid);
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SOCIAL_BY_FRIEND);
            stmt->setUInt64(0, lowGuid);
            trans->Append(stmt);

        }

        // Reset homebind and position
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_HOMEBIND);
        stmt->setUInt64(0, lowGuid);
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_HOMEBIND);
        stmt->setUInt64(0, lowGuid);
        if (team == TEAM_ALLIANCE)
        {
            stmt->setUInt16(1, 0);
            stmt->setUInt16(2, 1519);
            stmt->setFloat (3, -8867.68f);
            stmt->setFloat (4, 673.373f);
            stmt->setFloat (5, 97.9034f);
            Player::SavePositionInDB(0, -8867.68f, 673.373f, 97.9034f, 0.0f, 1519, guid);
        }
        else
        {
            stmt->setUInt16(1, 1);
            stmt->setUInt16(2, 1637);
            stmt->setFloat (3, 1633.33f);
            stmt->setFloat (4, -4439.11f);
            stmt->setFloat (5, 15.7588f);
            Player::SavePositionInDB(1, 1633.33f, -4439.11f, 15.7588f, 0.0f, 1637, guid);
        }
        trans->Append(stmt);

        // Achievement conversion
        for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Achievements.begin(); it != sObjectMgr->FactionChange_Achievements.end(); ++it)
        {
            uint32 achiev_alliance = it->first;
            uint32 achiev_horde = it->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_ACHIEVEMENT_BY_ACHIEVEMENT);
            stmt->setUInt16(0, uint16(team == TEAM_ALLIANCE ? achiev_alliance : achiev_horde));
            stmt->setUInt64(1, lowGuid);
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_ACHIEVEMENT);
            stmt->setUInt16(0, uint16(team == TEAM_ALLIANCE ? achiev_alliance : achiev_horde));
            stmt->setUInt16(1, uint16(team == TEAM_ALLIANCE ? achiev_horde : achiev_alliance));
            stmt->setUInt64(2, lowGuid);
            trans->Append(stmt);
        }

        // Item conversion
        for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Items.begin(); it != sObjectMgr->FactionChange_Items.end(); ++it)
        {
            uint32 item_alliance = it->first;
            uint32 item_horde = it->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_INVENTORY_FACTION_CHANGE);
            stmt->setUInt32(0, (team == TEAM_ALLIANCE ? item_alliance : item_horde));
            stmt->setUInt32(1, (team == TEAM_ALLIANCE ? item_horde : item_alliance));
            stmt->setUInt64(2, lowGuid);
            trans->Append(stmt);
        }

        // Spell conversion
        for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Spells.begin(); it != sObjectMgr->FactionChange_Spells.end(); ++it)
        {
            uint32 spell_alliance = it->first;
            uint32 spell_horde = it->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SPELL_BY_SPELL);
            stmt->setUInt32(0, (team == TEAM_ALLIANCE ? spell_alliance : spell_horde));
            stmt->setUInt64(1, lowGuid);
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_SPELL_FACTION_CHANGE);
            stmt->setUInt32(0, (team == TEAM_ALLIANCE ? spell_alliance : spell_horde));
            stmt->setUInt32(1, (team == TEAM_ALLIANCE ? spell_horde : spell_alliance));
            stmt->setUInt64(2, lowGuid);
            trans->Append(stmt);
        }

        // Reputation conversion
        for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Reputation.begin(); it != sObjectMgr->FactionChange_Reputation.end(); ++it)
        {
            uint32 reputation_alliance = it->first;
            uint32 reputation_horde = it->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_REP_BY_FACTION);
            stmt->setUInt32(0, uint16(team == TEAM_ALLIANCE ? reputation_alliance : reputation_horde));
            stmt->setUInt64(1, lowGuid);
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_REP_FACTION_CHANGE);
            stmt->setUInt16(0, uint16(team == TEAM_ALLIANCE ? reputation_alliance : reputation_horde));
            stmt->setUInt16(1, uint16(team == TEAM_ALLIANCE ? reputation_horde : reputation_alliance));
            stmt->setUInt64(2, lowGuid);
            trans->Append(stmt);
        }
        // Title conversion
        if (knownTitlesStr)
        {
            const uint32 ktcount = KNOWN_TITLES_SIZE * 2;
            uint32 knownTitles[ktcount];
            Tokenizer tokens(knownTitlesStr, ' ', ktcount);

            if (tokens.size() != ktcount)
                return;

            for (uint32 index = 0; index < ktcount; ++index)
                knownTitles[index] = atol(tokens[index]);

            for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Titles.begin(); it != sObjectMgr->FactionChange_Titles.end(); ++it)
            {
                uint32 title_alliance = it->first;
                uint32 title_horde = it->second;

                CharTitlesEntry const* atitleInfo = sCharTitlesStore.LookupEntry(title_alliance);
                CharTitlesEntry const* htitleInfo = sCharTitlesStore.LookupEntry(title_horde);
                // new team
                if (team == TEAM_ALLIANCE)
                {
                    uint32 bitIndex = htitleInfo->MaskID;
                    uint32 index = bitIndex / 32;
                    uint32 old_flag = 1 << (bitIndex % 32);
                    uint32 new_flag = 1 << (atitleInfo->MaskID % 32);
                    if (knownTitles[index] & old_flag)
                    {
                        knownTitles[index] &= ~old_flag;
                        // use index of the new title
                        knownTitles[atitleInfo->MaskID / 32] |= new_flag;
                    }
                }
                else
                {
                    uint32 bitIndex = atitleInfo->MaskID;
                    uint32 index = bitIndex / 32;
                    uint32 old_flag = 1 << (bitIndex % 32);
                    uint32 new_flag = 1 << (htitleInfo->MaskID % 32);
                    if (knownTitles[index] & old_flag)
                    {
                        knownTitles[index] &= ~old_flag;
                        // use index of the new title
                        knownTitles[htitleInfo->MaskID / 32] |= new_flag;
                    }
                }

                std::ostringstream ss;
                for (uint32 index = 0; index < ktcount; ++index)
                    ss << knownTitles[index] << ' ';

                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TITLES_FACTION_CHANGE);
                stmt->setString(0, ss.str().c_str());
                stmt->setUInt64(1, lowGuid);
                trans->Append(stmt);

                // unset any currently chosen title
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_RES_CHAR_TITLES_FACTION_CHANGE);
                stmt->setUInt64(0, lowGuid);
                trans->Append(stmt);
            }
        }

    }

    CharacterDatabase.CommitTransaction(trans);

    std::string IP_str = GetRemoteAddress();
    sLog->outDebug(LOG_FILTER_UNITS, "Account: %d (IP: %s), Character guid: %u Change Race/Faction to: %s", GetAccountId(), IP_str.c_str(), lowGuid, newname.c_str());

    WorldPacket data(SMSG_CHAR_FACTION_CHANGE_RESULT, 1 + 8 + (newname.size() + 1) + 1 + 1 + 1 + 1 + 1 + 1 + 1);
    data << uint8(RESPONSE_SUCCESS);
    data << guid;
    data.WriteBit(0);
    data.WriteBits(newname.length(), 6);
    data << uint8(gender);
    data << uint8(skin);
    data << uint8(hairColor);
    data << uint8(hairStyle);
    data << uint8(facialHair);
    data << uint8(face);
    data << uint8(race);
    data.WriteString(newname);

    SendPacket(&data);
}

void WorldSession::HandleGenerateRandomCharacterName(WorldPackets::Character::GenerateRandomCharacterName& packet)
{
    if (!Player::IsValidRace(packet.Race))
        return;

    if (!Player::IsValidGender(packet.Sex))
        return;

    std::string name = GetRandomCharacterName(packet.Race, packet.Sex);

    WorldPackets::Character::GenerateRandomCharacterNameResult result;
    result.Success = true;
    result.Name = name;
    SendPacket(result.Write());
}

//! 6.1.2
void WorldSession::HandleReorderCharacters(WorldPacket& recvData)
{
    uint32 charactersCount = recvData.ReadBits(9);

    GuidVector guids(charactersCount);
    uint8 position;

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    for (uint8 i = 0; i < charactersCount; ++i)
    {
        recvData >> guids[i];
        recvData >> position;

        //! WARNING!!!
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_LIST_SLOT);
        stmt->setUInt8(0, position);
        stmt->setUInt64(1, guids[i].GetCounter());
        trans->Append(stmt);
    }

    CharacterDatabase.CommitTransaction(trans);
}

void WorldSession::HandleSaveCUFProfiles(WorldPacket& recvData)
{
    uint32 profilesCount;
    recvData >> profilesCount;

    if (profilesCount > MAX_CUF_PROFILES)
    {
        recvData.rfinish();
        return;
    }

    CUFProfile* profiles[MAX_CUF_PROFILES];

    for (uint8 i = 0; i < profilesCount; ++i)
    {
        profiles[i] = new CUFProfile;
        profiles[i]->setOptionBit(CUF_KEEP_GROUPS_TOGETHER, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_DISPLAY_PETS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_DISPLAY_MAIN_TANK_AND_ASSIST, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_DISPLAY_HEAL_PREDICTION, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_DISPLAY_AGGRO_HIGHLIGHT, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_DISPLAY_ONLY_DISPELLABLE_DEBUFFS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_DISPLAY_POWER_BAR, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_DISPLAY_BORDER, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_USE_CLASS_COLORS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_DISPLAY_HORIZONTAL_GROUPS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_DISPLAY_NON_BOSS_DEBUFFS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_DYNAMIC_POSITION, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_LOCKED, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_SHOWN, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_AUTO_ACTIVATE_2_PLAYERS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_AUTO_ACTIVATE_3_PLAYERS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_AUTO_ACTIVATE_5_PLAYERS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_AUTO_ACTIVATE_10_PLAYERS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_AUTO_ACTIVATE_15_PLAYERS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_AUTO_ACTIVATE_25_PLAYERS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_AUTO_ACTIVATE_40_PLAYERS, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_AUTO_ACTIVATE_SPEC_1, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_AUTO_ACTIVATE_SPEC_2, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_AUTO_ACTIVATE_PVP, recvData.ReadBit());
        profiles[i]->setOptionBit(CUF_AUTO_ACTIVATE_PVE, recvData.ReadBit());

        recvData >> profiles[i]->frameHeight;
        recvData >> profiles[i]->frameWidth;
        recvData >> profiles[i]->sortBy;
        recvData >> profiles[i]->showHealthText;
        recvData >> profiles[i]->TopPoint;
        recvData >> profiles[i]->BottomPoint;
        recvData >> profiles[i]->LeftPoint;
        recvData >> profiles[i]->TopOffset;
        recvData >> profiles[i]->BottomOffset;
        recvData >> profiles[i]->LeftOffset;

        recvData >> profiles[i]->profileName;

        // save current profile
        _player->SaveCUFProfile(i, profiles[i]);
    }

    // clear other profiles
    for (uint8 i = profilesCount; i < MAX_CUF_PROFILES; ++i)
        _player->SaveCUFProfile(i, NULL);
}

void WorldSession::SendLoadCUFProfiles()
{
    uint8 profilesCount = _player->GetCUFProfilesCount();

    WorldPacket data(SMSG_LOAD_CUF_PROFILES);

    data << uint32(profilesCount);
    for (uint8 i = 0; i < profilesCount; ++i)
    {
        CUFProfile * profile = _player->GetCUFProfile(i);

        if (!profile)
            continue;

        data.WriteBit(profile->getOptionBit(CUF_KEEP_GROUPS_TOGETHER));
        data.WriteBit(profile->getOptionBit(CUF_DISPLAY_PETS));
        data.WriteBit(profile->getOptionBit(CUF_DISPLAY_MAIN_TANK_AND_ASSIST));
        data.WriteBit(profile->getOptionBit(CUF_DISPLAY_HEAL_PREDICTION));
        data.WriteBit(profile->getOptionBit(CUF_DISPLAY_AGGRO_HIGHLIGHT));
        data.WriteBit(profile->getOptionBit(CUF_DISPLAY_ONLY_DISPELLABLE_DEBUFFS));
        data.WriteBit(profile->getOptionBit(CUF_DISPLAY_POWER_BAR));
        data.WriteBit(profile->getOptionBit(CUF_DISPLAY_BORDER));
        data.WriteBit(profile->getOptionBit(CUF_USE_CLASS_COLORS));
        data.WriteBit(profile->getOptionBit(CUF_DISPLAY_HORIZONTAL_GROUPS));
        data.WriteBit(profile->getOptionBit(CUF_DISPLAY_NON_BOSS_DEBUFFS));
        data.WriteBit(profile->getOptionBit(CUF_DYNAMIC_POSITION));
        data.WriteBit(profile->getOptionBit(CUF_LOCKED));
        data.WriteBit(profile->getOptionBit(CUF_SHOWN));
        data.WriteBit(profile->getOptionBit(CUF_AUTO_ACTIVATE_2_PLAYERS));
        data.WriteBit(profile->getOptionBit(CUF_AUTO_ACTIVATE_3_PLAYERS));
        data.WriteBit(profile->getOptionBit(CUF_AUTO_ACTIVATE_5_PLAYERS));
        data.WriteBit(profile->getOptionBit(CUF_AUTO_ACTIVATE_10_PLAYERS));
        data.WriteBit(profile->getOptionBit(CUF_AUTO_ACTIVATE_15_PLAYERS));
        data.WriteBit(profile->getOptionBit(CUF_AUTO_ACTIVATE_25_PLAYERS));
        data.WriteBit(profile->getOptionBit(CUF_AUTO_ACTIVATE_40_PLAYERS));
        data.WriteBit(profile->getOptionBit(CUF_AUTO_ACTIVATE_SPEC_1));
        data.WriteBit(profile->getOptionBit(CUF_AUTO_ACTIVATE_SPEC_2));
        data.WriteBit(profile->getOptionBit(CUF_AUTO_ACTIVATE_PVP));
        data.WriteBit(profile->getOptionBit(CUF_AUTO_ACTIVATE_PVE));

        data.FlushBits();

        data << uint16(profile->frameHeight);
        data << uint16(profile->frameWidth);

        data << uint8(profile->sortBy);
        data << uint8(profile->showHealthText);
        data << uint8(profile->TopPoint);
        data << uint8(profile->BottomPoint);
        data << uint8(profile->LeftPoint);

        data << uint16(profile->TopOffset);
        data << uint16(profile->BottomOffset);
        data << uint16(profile->LeftOffset);

        data.WriteString(profile->profileName);
    }

    SendPacket(&data);
}

void WorldSession::SendCharCreate(ResponseCodes result)
{
    WorldPackets::Character::CharacterCreateResponse response;
    response.Code = result;

    SendPacket(response.Write());
}

void WorldSession::SendCharDelete(ResponseCodes result)
{
    WorldPackets::Character::CharacterDeleteResponse response;
    response.Code = result;

    SendPacket(response.Write());
}

void WorldSession::HandleSetAdvancedCombatLogging(WorldPackets::ClientConfig::SetAdvancedCombatLogging& packet)
{
    _player->SetAdvancedCombatLogging(packet.Enable);
}

void WorldSession::SendConnectToInstance(WorldPackets::Auth::ConnectToSerial serial)
{
    //boost::system::error_code ignored_error;
    //boost::asio::ip::tcp::endpoint instanceAddress = realm.GetAddressForClient(boost::asio::ip::address::from_string(GetRemoteAddress(), ignored_error));
    //instanceAddress.port(8086);

    //WorldPackets::Auth::ConnectTo connectTo;
    //connectTo.Key = MAKE_PAIR64(GetAccountId(), CONNECTION_TYPE_INSTANCE);
    //connectTo.Serial = serial;
    //connectTo.Payload.Where = instanceAddress;
    //connectTo.Con = CONNECTION_TYPE_INSTANCE;

    //SendPacket(connectTo.Write());
}

void WorldSession::HandleContinuePlayerLogin()
{
    if (!PlayerLoading() || GetPlayer())
    {
        KickPlayer();
        return;
    }

    LoginQueryHolder* holder = new LoginQueryHolder(GetAccountId(), m_playerLoading);
    if (!holder->Initialize())
    {
        delete holder;                                      // delete all unprocessed queries
        m_playerLoading.Clear();
        return;
    }

    SendPacket(WorldPackets::Auth::ResumeComms(CONNECTION_TYPE_INSTANCE).Write());

    _charLoginCallback = CharacterDatabase.DelayQueryHolder(holder);
}

void WorldSession::AbortLogin(WorldPackets::Character::LoginFailureReason reason)
{
    if (!PlayerLoading() || GetPlayer())
    {
        KickPlayer();
        return;
    }

    m_playerLoading.Clear();
    SendPacket(WorldPackets::Character::CharacterLoginFailed(reason).Write());
}

void WorldSession::HandleSetActionBarToggles(WorldPackets::Character::SetActionBarToggles& packet)
{
    if (!GetPlayer())                                        // ignore until not logged (check needed because STATUS_AUTHED)
    {
        if (packet.Mask != 0)
            sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetActionBarToggles in not logged state with value: %u, ignored", packet.Mask);
        return;
    }

    GetPlayer()->SetByteValue(PLAYER_FIELD_BYTES, PLAYER_FIELD_BYTES_OFFSET_ACTION_BAR_TOGGLES, packet.Mask);
}

void WorldSession::HandleRequestPlayedTime(WorldPackets::Character::RequestPlayedTime& packet)
{
    WorldPackets::Character::PlayedTime playedTime;
    playedTime.TotalTime = _player->GetTotalPlayedTime();
    playedTime.LevelTime = _player->GetLevelPlayedTime();
    playedTime.TriggerEvent = packet.TriggerScriptEvent;
    SendPacket(playedTime.Write());
}

void WorldSession::HandleSetTitle(WorldPackets::Character::SetTitle& packet)
{
    if (packet.TitleID > 0 && packet.TitleID < MAX_TITLE_INDEX)
    {
        if (!GetPlayer()->HasTitle(packet.TitleID))
            return;
    }
    else
        packet.TitleID = 0;

    GetPlayer()->SetUInt32Value(PLAYER_FIELD_PLAYER_TITLE, packet.TitleID);
}
