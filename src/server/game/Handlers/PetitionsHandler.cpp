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
#include "WordFilterMgr.h"
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
#include "PetitionPackets.h"

#define CHARTER_DISPLAY_ID 16161

// Charters ID in item_template
enum CharterItemIDs
{
    GUILD_CHARTER                                 = 5863,
};

enum CharterCosts
{
    GUILD_CHARTER_COST                            = 1000,
};

//! 6.0.3
void WorldSession::HandlePetitionBuyOpcode(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_PETITION_BUY");

    ObjectGuid guidNPC;
    std::string name;

    uint32 strLen = recvData.ReadBits(7);
    recvData >> guidNPC;
    name = recvData.ReadString(strLen);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Petitioner with GUID %u tried sell petition: name %s", guidNPC.GetCounter(), name.c_str());

    // prevent cheating
    Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(guidNPC, UNIT_NPC_FLAG_PETITIONER);
    if (!creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandlePetitionBuyOpcode - Unit (GUID: %u) not found or you can't interact with him.", guidNPC.GetCounter());
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    uint32 charterid = 0;
    uint32 cost = 0;
    uint32 type = 0;
    if (creature->isTabardDesigner())
    {
        // if tabard designer, then trying to buy a guild charter.
        // do not let if already in guild.
        if (_player->GetGuildId())
            return;

        charterid = GUILD_CHARTER;
        cost = GUILD_CHARTER_COST;
        type = GUILD_CHARTER_TYPE;
    }

    if (sGuildMgr->GetGuildByName(name))
    {
        Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NAME_EXISTS_S, name);
        return;
    }
    
    if (sObjectMgr->IsReservedName(name) || !ObjectMgr::IsValidCharterName(name)  ||
        (sWorld->getBoolConfig(CONFIG_WORD_FILTER_ENABLE) && !sWordFilterMgr->FindBadWord(name).empty()))
    {
        Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NAME_INVALID, name);
        return;
    }

    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(charterid);
    if (!pProto)
    {
        _player->SendBuyError(BUY_ERR_CANT_FIND_ITEM, NULL, charterid, 0);
        return;
    }

    if (!_player->HasEnoughMoney(uint64(cost)))
    {                                                       //player hasn't got enough money
        _player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, creature, charterid, 0);
        return;
    }

    ItemPosCountVec dest;
    InventoryResult msg = _player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, charterid, pProto->BuyCount);
    if (msg != EQUIP_ERR_OK)
    {
        _player->SendEquipError(msg, NULL, NULL, charterid);
        return;
    }

    _player->ModifyMoney(-(int32)cost);
    Item* charter = _player->StoreNewItem(dest, charterid, true);
    if (!charter)
        return;

    // ITEM_FIELD_ENCHANTMENT is guild id 
    // ITEM_FIELD_ENCHANTMENT+1 is current signatures count (showed on item)
    charter->SetUInt32Value(ITEM_FIELD_ENCHANTMENT, charter->GetGUID().GetCounter());
    charter->SetUInt32Value(ITEM_FIELD_ENCHANTMENT+1, 0);
    charter->SetState(ITEM_CHANGED, _player);
    _player->SendNewItem(charter, NULL, 1, true, false);

    // a petition is invalid, if both the owner and the type matches
    // we checked above, if this player is in an arenateam, so this must be
    // datacorruption
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_BY_OWNER);
    stmt->setUInt64(0, _player->GetGUID().GetCounter());
    stmt->setUInt8(1, type);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    std::ostringstream ssInvalidPetitionGUIDs;

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            ssInvalidPetitionGUIDs << '\'' << fields[0].GetUInt64() << "', ";
        } while (result->NextRow());
    }

    // delete petitions with the same guid as this one
    ssInvalidPetitionGUIDs << '\'' << charter->GetGUID().GetCounter() << '\'';

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Invalid petition GUIDs: %s", ssInvalidPetitionGUIDs.str().c_str());
    CharacterDatabase.EscapeString(name);
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    trans->PAppend("DELETE FROM petition WHERE petitionguid IN (%s)",  ssInvalidPetitionGUIDs.str().c_str());
    trans->PAppend("DELETE FROM petition_sign WHERE petitionguid IN (%s)", ssInvalidPetitionGUIDs.str().c_str());

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PETITION);
    stmt->setUInt64(0, _player->GetGUID().GetCounter());
    stmt->setUInt64(1, charter->GetGUID().GetCounter());
    stmt->setString(2, name);
    stmt->setUInt8(3, uint8(type));
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);
}

//! 6.0.3
void WorldSession::HandlePetitionShowSignOpcode(WorldPacket& recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_PETITION_SHOW_SIGNATURES");

    uint8 signs = 0;
    ObjectGuid petitionguid;
    recvData >> petitionguid;

    // solve (possible) some strange compile problems with explicit use GUID_LOPART(petitionguid) at some GCC versions (wrong code optimization in compiler?)
    ObjectGuid::LowType petitionGuidLow = petitionguid.GetCounter();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_TYPE);

    stmt->setUInt64(0, petitionGuidLow);

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Petition %u is not found for player %u %s", petitionGuidLow, GetPlayer()->GetGUID().GetCounter(), GetPlayer()->GetName());
        return;
    }
    Field* fields = result->Fetch();
    uint32 type = fields[0].GetUInt8();

    // if has guild => error, return;
    if (_player->GetGuildId())
        return;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIGNATURE);
    stmt->setUInt64(0, petitionGuidLow);

    result = CharacterDatabase.Query(stmt);

    // result == NULL also correct in case no sign yet
    if (result)
        signs = uint8(result->GetRowCount());

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_PETITION_SHOW_SIGNATURES petition entry: '%u'", petitionGuidLow);

    ObjectGuid playerGUID = _player->GetGUID();

    //! 6.0.3
    WorldPacket data(SMSG_PETITION_SHOW_SIGNATURES, (8+8+4+1+signs*12));
    data << petitionguid;
    data << playerGUID;                             // should be owner
    data << GetAccountGUID();

    data << uint32(petitionGuidLow);                // CGPetitionInfo__m_petitionID

    data << uint32(signs);                          // sign's count         
    for (uint8 i = 0; i < signs; i++)
    {
        Field* fields2 = result->Fetch();
        data << ObjectGuid::Create<HighGuid::Player>(fields2[0].GetUInt64());
        data << uint32(0);
    }

    SendPacket(&data);
}

//! 6.0.3
void WorldSession::HandlePetitionQueryOpcode(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_QUERY_PETITION");   // ok

    uint32 guildguid;
    ObjectGuid petitionguid;
    recvData >> guildguid >> petitionguid;                                 // in Trinity always same as GUID_LOPART(petitionguid)

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_QUERY_PETITION Petition GUID %u Guild GUID %u", petitionguid.GetCounter(), guildguid);

    SendPetitionQueryOpcode(petitionguid);
}

//! 6.0.3
void WorldSession::SendPetitionQueryOpcode(ObjectGuid petitionguid)
{
    ObjectGuid ownerguid;
    uint32 type;
    std::string name = "NO_NAME_FOR_GUID";

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION);

    stmt->setUInt64(0, petitionguid.GetCounter());

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        Field* fields = result->Fetch();
        ownerguid = ObjectGuid::Create<HighGuid::Player>(fields[0].GetUInt64());
        name      = fields[1].GetString();
        type      = fields[2].GetUInt8();
    }

    WorldPackets::Petition::QueryPetitionResponse packet;
    packet.PetitionID = petitionguid.GetCounter();
    packet.Allow = bool(result);
    packet.Info.PetitionID = petitionguid.GetCounter();
    packet.Info.Petitioner = ownerguid;
    packet.Info.Title = name;
    packet.Info.MinSignatures = sWorld->getIntConfig(CONFIG_MIN_PETITION_SIGNS);
    packet.Info.MaxSignatures = sWorld->getIntConfig(CONFIG_MIN_PETITION_SIGNS)+1;
    packet.Info.StaticType = type;
    SendPacket(packet.Write());
}

//! 6.0.3
void WorldSession::HandlePetitionRenameOpcode(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_PETITION_RENAME_GUILD");

    ObjectGuid petitionGuid;
    uint32 type;

    recvData >> petitionGuid;

    std::string newName = recvData.ReadString(recvData.ReadBits(7));
    
    Item* item = _player->GetItemByGuid(petitionGuid);
    if (!item)
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_TYPE);

    stmt->setUInt64(0, petitionGuid.GetCounter());

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        Field* fields = result->Fetch();
        type = fields[0].GetUInt8();
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_QUERY_PETITION failed for petition (GUID: %u)", petitionGuid.GetCounter());
        return;
    }

    if (sGuildMgr->GetGuildByName(newName))
    {
        Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NAME_EXISTS_S, newName);
        return;
    }
    if (sObjectMgr->IsReservedName(newName) || !ObjectMgr::IsValidCharterName(newName) ||
        (sWorld->getBoolConfig(CONFIG_WORD_FILTER_ENABLE) && !sWordFilterMgr->FindBadWord(newName).empty()))
    {
        Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NAME_INVALID, newName);
        return;
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_PETITION_NAME);

    stmt->setString(0, newName);
    stmt->setUInt64(1, petitionGuid.GetCounter());

    CharacterDatabase.Execute(stmt);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Petition (GUID: %u) renamed to '%s'", petitionGuid.GetCounter(), newName.c_str());

    //! 6.0.3
    WorldPacket data(SMSG_PETITION_RENAME_GUILD_RESPONSE, (8+newName.size()+1));
    data << petitionGuid;
    data.WriteBits(newName.size(), 7);
    data.WriteString(newName);
    SendPacket(&data);
}

//! 6.0.3
void WorldSession::HandlePetitionSignOpcode(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_SIGN_PETITION");    // ok

    Field* fields;
    ObjectGuid petitionGuid;
    uint8 Choice;
    recvData >> petitionGuid >> Choice;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIGNATURES);

    stmt->setUInt64(0, petitionGuid.GetCounter());
    stmt->setUInt64(1, petitionGuid.GetCounter());

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Petition %u is not found for player %u %s", petitionGuid.GetCounter(), GetPlayer()->GetGUID().GetCounter(), GetPlayer()->GetName());
        return;
    }

    fields = result->Fetch();
    ObjectGuid ownerGuid = ObjectGuid::Create<HighGuid::Player>(fields[0].GetUInt64());
    uint64 signs = fields[1].GetUInt64();
    uint8 type = fields[2].GetUInt8();

    if (type != GUILD_CHARTER_TYPE)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Petition %u of player %u %s has not supported type %u", petitionGuid.GetCounter(), GetPlayer()->GetGUID().GetCounter(), GetPlayer()->GetName(), type);
        return;
    }

    ObjectGuid playerGuid = _player->GetGUID();
    if (playerGuid == ownerGuid)
        return;

    // not let enemies sign guild charter
    if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD) && GetPlayer()->GetTeam() != sObjectMgr->GetPlayerTeamByGUID(ownerGuid))
    {
        Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NOT_ALLIED);
        return;
    }

    if (_player->GetGuildId())
    {
        Guild::SendCommandResult(this, GUILD_INVITE_S, ERR_ALREADY_IN_GUILD_S, _player->GetName());
        return;
    }

    if (_player->GetGuildIdInvited())
    {
        Guild::SendCommandResult(this, GUILD_INVITE_S, ERR_ALREADY_INVITED_TO_GUILD_S, _player->GetName());
        return;
    }

    if (++signs > type)                                        // client signs maximum
        return;

    // Client doesn't allow to sign petition two times by one character, but not check sign by another character from same account
    // not allow sign another player from already sign player account
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIG_BY_ACCOUNT);

    stmt->setUInt32(0, GetAccountId());
    stmt->setUInt64(1, petitionGuid.GetCounter());

    result = CharacterDatabase.Query(stmt);

    if (result)
    {
        // close at signer side
        SendPetitionSignResult(_player->GetGUID(), petitionGuid, PETITION_SIGN_ALREADY_SIGNED);
        return;
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PETITION_SIGNATURE);

    stmt->setUInt64(0, ownerGuid.GetCounter());
    stmt->setUInt64(1, petitionGuid.GetCounter());
    stmt->setUInt64(2, playerGuid.GetCounter());
    stmt->setUInt32(3, GetAccountId());

    CharacterDatabase.Execute(stmt);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "PETITION SIGN: GUID %u by player: %s (GUID: %u Account: %u) Choice %u", petitionGuid.GetCounter(), _player->GetName(), playerGuid, GetAccountId(), Choice);

    // close at signer side
    SendPetitionSignResult(_player->GetGUID(), petitionGuid, PETITION_SIGN_OK);

    if (Player* owner = ObjectAccessor::FindPlayer(ownerGuid))
    {
        // update signs count on charter
        if (Item* item = owner->GetItemByGuid(petitionGuid))
            item->SetUInt32Value(ITEM_FIELD_ENCHANTMENT+1, signs);

        // update sign result for owner
        owner->GetSession()->SendPetitionSignResult(_player->GetGUID(), petitionGuid, PETITION_SIGN_OK);
    }
}

//! 6.0.3
void WorldSession::SendPetitionSignResult(ObjectGuid const& playerGuid, ObjectGuid const& petitionGuid, uint8 result)
{
    WorldPacket data(SMSG_PETITION_SIGN_RESULTS, 8 + 8 + 1 + 1 + 1);
    data << petitionGuid << playerGuid;
    data.WriteBits(result, 4);
    SendPacket(&data);
}

//! 6.0.3
void WorldSession::HandlePetitionDeclineOpcode(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_DECLINE_PETITION");  // ok

    ObjectGuid petitionguid;
    ObjectGuid ownerguid;
    recvData >> petitionguid;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Petition %u declined by %u", petitionguid.GetCounter(), _player->GetGUID().GetCounter());

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_OWNER_BY_GUID);

    stmt->setUInt64(0, petitionguid.GetCounter());

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
        return;

    Field* fields = result->Fetch();
    ownerguid = ObjectGuid::Create<HighGuid::Player>(fields[0].GetUInt64());

    if (Player* owner = ObjectAccessor::FindPlayer(ownerguid))                                               // petition owner online
    {
        WorldPacket data(SMSG_PETITION_DECLINED, 8);
        data << _player->GetGUID();
        owner->GetSession()->SendPacket(&data);
    }
}

//! 6.0.3
void WorldSession::HandleOfferPetitionOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_OFFER_PETITION");

    uint8 signs = 0;
    ObjectGuid petitionguid, plguid;
    uint32 type;
    Player* player;

    recvData >> petitionguid >> plguid;

    player = ObjectAccessor::FindPlayer(plguid);
    if (!player)
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_TYPE);

    stmt->setUInt64(0, petitionguid.GetCounter());

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
        return;

    Field* fields = result->Fetch();
    type = fields[0].GetUInt8();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "OFFER PETITION: type %u, GUID1 %u, to player id: %u", type, petitionguid.GetCounter(), plguid.GetCounter());

    if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD) && GetPlayer()->GetTeam() != player->GetTeam())
    {
        Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NOT_ALLIED);
        return;
    }

    if (player->GetGuildId())
    {
        Guild::SendCommandResult(this, GUILD_INVITE_S, ERR_ALREADY_IN_GUILD_S, _player->GetName());
        return;
    }

    if (player->GetGuildIdInvited())
    {
        Guild::SendCommandResult(this, GUILD_INVITE_S, ERR_ALREADY_INVITED_TO_GUILD_S, _player->GetName());
        return;
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIGNATURE);
    stmt->setUInt64(0, petitionguid.GetCounter());

    result = CharacterDatabase.Query(stmt);

    // result == NULL also correct charter without signs
    if (result)
    {
        signs = uint8(result->GetRowCount());

        // check already signed petition
        for (uint8 i = 0; i < signs; i++)
        {
            Field* fields1 = result->Fetch();
            ObjectGuid::LowType lowGuid = fields1[0].GetUInt64();
            ObjectGuid plSignGuid = ObjectGuid::Create<HighGuid::Player>(lowGuid);

            if (player->GetGUID() == plSignGuid)
            {
                WorldPacket data(SMSG_PETITION_ALREADY_SIGNED, 16);
                data << plSignGuid;
                SendPacket(&data);
                return;
            }
        }
    }

    //! 6.0.3
    WorldPacket data(SMSG_PETITION_SHOW_SIGNATURES, (8+8+4+1+signs*12));
    data << petitionguid;
    data << _player->GetGUID();                    // should be owner
    data << GetAccountGUID();

    data << uint32(petitionguid.GetCounter());     // CGPetitionInfo__m_petitionID

    data << uint32(signs);                          // sign's count         
    for (uint8 i = 0; i < signs; i++)
    {
        Field* fields2 = result->Fetch();
        data << ObjectGuid::Create<HighGuid::Player>(fields2[0].GetUInt64());
        data << uint32(0);
    }

    player->GetSession()->SendPacket(&data);
}

//! 6.0.3
void WorldSession::HandlePetitionShowListOpcode(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "Received CMSG_PETITION_SHOW_LIST");

    ObjectGuid guid;
    recvData >> guid;
    SendPetitionShowList(guid);
}

//! 6.0.3
void WorldSession::SendPetitionShowList(ObjectGuid guid)
{
    Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_PETITIONER);
    if (!creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandlePetitionShowListOpcode - Unit (GUID: %u) not found or you can't interact with him.", uint32(guid.GetCounter()));
        return;
    }

    if (!creature->isTabardDesigner())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandlePetitionShowListOpcode - Unit (GUID: %u) is not a tabard designer.", uint32(guid.GetCounter()));
        return;
    }

    WorldPacket data(SMSG_PETITION_SHOW_LIST, 4 + 8 + 1);
    data << guid;
    data << uint32(GUILD_CHARTER_COST);                 // charter cost
    SendPacket(&data);
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "Sent SMSG_PETITION_SHOW_LIST");
}

void WorldSession::HandleTurnInPetitionOpcode(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_TURN_IN_PETITION");

    // Get petition guid from packet
    WorldPacket data;
    ObjectGuid petitionGuid;
    recvData >> petitionGuid;

    // Check if player really has the required petition charter
    Item* item = _player->GetItemByGuid(petitionGuid);
    if (!item)
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Petition %u turned in by %u", petitionGuid.GetCounter(), _player->GetGUID().GetCounter());

    // Get petition data from db
    ObjectGuid::LowType ownerguidlo;
    uint32 type;
    std::string name;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION);
    stmt->setUInt64(0, petitionGuid.GetCounter());
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        Field* fields = result->Fetch();
        ownerguidlo = fields[0].GetUInt64();
        name = fields[1].GetString();
        type = fields[2].GetUInt8();
    }
    else
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Player %s (guid: %u) tried to turn in petition (guid: %u) that is not present in the database", _player->GetName(), _player->GetGUID().GetCounter(), petitionGuid.GetCounter());
        return;
    }

    // Only the petition owner can turn in the petition
    if (_player->GetGUID().GetCounter() != ownerguidlo)
        return;

    // Check if player is already in a guild
    if (_player->GetGuildId())
    {
        data.Initialize(SMSG_TURN_IN_PETITION_RESULTS, 1);
        data.WriteBits(PETITION_TURN_ALREADY_IN_GUILD, 4);
        _player->GetSession()->SendPacket(&data);
        return;
    }

    // Check if guild name is already taken
    if (sGuildMgr->GetGuildByName(name))
    {
        Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NAME_EXISTS_S, name);
        return;
    }

    // Get petition signatures from db
    uint8 signatures;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIGNATURE);
    stmt->setUInt64(0, petitionGuid.GetCounter());
    result = CharacterDatabase.Query(stmt);

    if (result)
        signatures = uint8(result->GetRowCount());
    else
        signatures = 0;

    uint32 requiredSignatures;
    requiredSignatures = sWorld->getIntConfig(CONFIG_MIN_PETITION_SIGNS);

    // Notify player if signatures are missing
    if (signatures < requiredSignatures)
    {
        data.Initialize(SMSG_TURN_IN_PETITION_RESULTS, 1);
        data.WriteBits(PETITION_TURN_NEED_MORE_SIGNATURES, 4);
        SendPacket(&data);
        return;
    }

    // Proceed with guild creation

    // Delete charter item
    _player->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);

    // Create guild
    Guild* guild = new Guild;

    if (!guild->Create(_player, name))
    {
        delete guild;
        return;
    }

    // Register guild and add guild master
    sGuildMgr->AddGuild(guild);

    // Add members from signatures
    for (uint8 i = 0; i < signatures; ++i)
    {
        Field* fields = result->Fetch();
        guild->AddMember(ObjectGuid::Create<HighGuid::Player>(fields[0].GetUInt64()));
        result->NextRow();
    }

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PETITION_BY_GUID);
    stmt->setUInt64(0, petitionGuid.GetCounter());
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PETITION_SIGNATURE_BY_GUID);
    stmt->setUInt64(0, petitionGuid.GetCounter());
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);

    // created
    sLog->outDebug(LOG_FILTER_NETWORKIO, "TURN IN PETITION GUID %u", petitionGuid.GetCounter());

    data.Initialize(SMSG_TURN_IN_PETITION_RESULTS, 1);
    data.WriteBits(PETITION_TURN_OK, 4);
    SendPacket(&data);
}