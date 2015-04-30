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
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"

#include "CellImpl.h"
#include "WordFilterMgr.h"
#include "Chat.h"
#include "ChannelMgr.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "Guild.h"
#include "Language.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "Util.h"
#include "ScriptMgr.h"
#include "AccountMgr.h"

#include "ChatPackets.h"

bool WorldSession::processChatmessageFurtherAfterSecurityChecks(std::string& msg, uint32 lang)
{
    if (lang != LANG_ADDON)
    {
        // strip invisible characters for non-addon messages
        if (sWorld->getBoolConfig(CONFIG_CHAT_FAKE_MESSAGE_PREVENTING))
            stripLineInvisibleChars(msg);

        if (sWorld->getIntConfig(CONFIG_CHAT_STRICT_LINK_CHECKING_SEVERITY) && AccountMgr::IsPlayerAccount(GetSecurity())
                && !ChatHandler(this).isValidChatMessage(msg.c_str()))
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %s (GUID: %u) sent a chatmessage with an invalid link: %s", GetPlayer()->GetName(),
                    GetPlayer()->GetGUID().GetCounter(), msg.c_str());
            if (sWorld->getIntConfig(CONFIG_CHAT_STRICT_LINK_CHECKING_KICK))
                KickPlayer();
            return false;
        }
    }

    return true;
}

void WorldSession::HandleChatMessageOpcode(WorldPackets::Chat::ChatMessage& packet)
{
    ChatMsg type;

    switch (packet.GetOpcode())
    {
        case CMSG_CHAT_MESSAGE_SAY:
            type = CHAT_MSG_SAY;
            break;
        case CMSG_CHAT_MESSAGE_YELL:
            type = CHAT_MSG_YELL;
            break;
        case CMSG_CHAT_MESSAGE_CHANNEL: //WorldSession::HandleChatMessageChannelOpcode
            type = CHAT_MSG_CHANNEL;
            break;
        case CMSG_CHAT_MESSAGE_WHISPER: //WorldSession::HandleChatMessageWhisperOpcode
            type = CHAT_MSG_WHISPER;
            break;
        case CMSG_CHAT_MESSAGE_GUILD:
            type = CHAT_MSG_GUILD;
            break;
        case CMSG_CHAT_MESSAGE_OFFICER:
            type = CHAT_MSG_OFFICER;
            break;
        case CMSG_CHAT_MESSAGE_AFK:
            type = CHAT_MSG_AFK;
            break;
        case CMSG_CHAT_MESSAGE_DND:
            type = CHAT_MSG_DND;
            break;
        case CMSG_CHAT_MESSAGE_EMOTE: //WorldSession::HandleChatMessageEmoteOpcode
            type = CHAT_MSG_EMOTE;
            break;
        case CMSG_CHAT_MESSAGE_PARTY:
            type = CHAT_MSG_PARTY;
            break;
        case CMSG_CHAT_MESSAGE_RAID:
            type = CHAT_MSG_RAID;
            break;
        case CMSG_CHAT_MESSAGE_RAID_WARNING:
            type = CHAT_MSG_RAID_WARNING;
            break;
        case CMSG_MESSAGECHAT_INSTANCE:
            type = CHAT_MSG_INSTANCE;
            break;
        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "HandleMessagechatOpcode : Unknown chat opcode (%u)", packet.GetOpcode());
            return;
    }
    HandleChatMessage(type, packet.Language, packet.Text);
}

void WorldSession::HandleChatMessageWhisperOpcode(WorldPackets::Chat::ChatMessageWhisper& packet)
{
    HandleChatMessage(CHAT_MSG_WHISPER, packet.Language, packet.Text, packet.Target);
}

void WorldSession::HandleChatMessageChannelOpcode(WorldPackets::Chat::ChatMessageChannel& packet)
{
    HandleChatMessage(CHAT_MSG_CHANNEL, packet.Language, packet.Text, packet.Target);
}

void WorldSession::HandleChatMessageEmoteOpcode(WorldPackets::Chat::ChatMessageEmote& packet)
{
    HandleChatMessage(CHAT_MSG_EMOTE, LANG_UNIVERSAL, packet.Text);
}

void WorldSession::HandleChatMessage(ChatMsg type, uint32 lang, std::string msg, std::string target)
{
    if (type >= MAX_CHAT_MSG_TYPE)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "CHAT: Wrong message type received: %u", type);
        return;
    }

    Player* sender = GetPlayer();

    //sLog->outDebug(LOG_FILTER_GENERAL, "CHAT: packet received. type %u, lang %u", type, lang);

    if (!sender->CanSpeak() && type != CHAT_MSG_AFK && type != CHAT_MSG_DND)
    {
        std::string timeStr = secsToTimeString(m_muteTime - time(NULL));
        SendNotification(GetTrinityString(LANG_WAIT_BEFORE_SPEAKING), timeStr.c_str());
        return;
    }

    // no language sent with emote packet.
    if (type != CHAT_MSG_EMOTE && type != CHAT_MSG_AFK && type != CHAT_MSG_DND)
    {
        // prevent talking at unknown language (cheating)
        LanguageDesc const* langDesc = GetLanguageDescByID(lang);
        if (!langDesc)
        {
            SendNotification(LANG_UNKNOWN_LANGUAGE);
            return;
        }
        if (!sender->CanSpeakLanguage(lang))
        {
            SendNotification(LANG_NOT_LEARNED_LANGUAGE);
            return;
        }

        if (lang == LANG_ADDON)
        {
            if (sWorld->getBoolConfig(CONFIG_CHATLOG_ADDON))
            {
                if (msg.empty())
                    return;

                sScriptMgr->OnPlayerChat(sender, uint32(CHAT_MSG_ADDON), lang, msg);
            }

            // Disabled addon channel?
            if (!sWorld->getBoolConfig(CONFIG_ADDON_CHANNEL))
                return;
        }
        // LANG_ADDON should not be changed nor be affected by flood control
        else
        {
            // send in universal language if player in .gm on mode (ignore spell effects)
            if (sender->isGameMaster())
                lang = LANG_UNIVERSAL;
            else if(sWorld->getBoolConfig(CONFIG_FUN_OPTION_ENABLED) && sender->GetAreaId() == 6823)
                lang = LANG_UNIVERSAL;
            else
            {
                // send in universal language in two side iteration allowed mode
                if (sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHAT))
                    lang = LANG_UNIVERSAL;
                else
                {
                    switch (type)
                    {
                        case CHAT_MSG_PARTY:
                        case CHAT_MSG_PARTY_LEADER:
                        case CHAT_MSG_RAID:
                        case CHAT_MSG_RAID_LEADER:
                        case CHAT_MSG_RAID_WARNING:
                            // allow two side chat at group channel if two side group allowed
                            if (sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP))
                                lang = LANG_UNIVERSAL;
                            break;
                        case CHAT_MSG_GUILD:
                        case CHAT_MSG_OFFICER:
                            // allow two side chat at guild channel if two side guild allowed
                            if (sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD))
                                lang = LANG_UNIVERSAL;
                            break;
                    }
                }

                // but overwrite it by SPELL_AURA_MOD_LANGUAGE auras (only single case used)
                Unit::AuraEffectList const& ModLangAuras = sender->GetAuraEffectsByType(SPELL_AURA_MOD_LANGUAGE);
                if (!ModLangAuras.empty())
                    lang = ModLangAuras.front()->GetMiscValue();
            }
        }
    }
    else
        lang = LANG_UNIVERSAL;

    if (sender->HasAura(1852) && type != CHAT_MSG_WHISPER)
    {
        SendNotification(GetTrinityString(LANG_GM_SILENCE), sender->GetName());
        return;
    }

    if (type != CHAT_MSG_AFK && CHAT_MSG_AFK != CHAT_MSG_DND)
    {
        if (msg.empty())
            return;

        if (ChatHandler(this).ParseCommands(msg.c_str()) > 0)
            return;

        if (!processChatmessageFurtherAfterSecurityChecks(msg, lang))
            return;

        if (msg.empty())
            return;
    }

    /// filtering of bad words
    if(sWorld->getBoolConfig(CONFIG_WORD_FILTER_ENABLE))
    {
        sender->m_sentMsgCache.append(msg);

        // horizontally
        std::string badWord = sWordFilterMgr->FindBadWord(msg);

        // vertically
        if(badWord.empty())
            badWord = sWordFilterMgr->FindBadWord(sender->m_sentMsgCache);

        if (!badWord.empty())
        {
            sender->m_sentMsgCache.erase();

            if(!sender->CanSpeak())
                return;

            time_t muteTime = time(NULL) + (int64)(sWorld->getIntConfig(CONFIG_WORD_FILTER_MUTE_DURATION) / 1000);
            sender->GetSession()->m_muteTime = muteTime;
            ChatHandler(sender->GetSession()).PSendSysMessage(LANG_WORD_FILTER_FOUND_BAD_WORD_IN_CHAT, badWord.c_str());
            return;
        }

        const uint8 maxSentMsgCacheSize = 64;

        if(sender->m_sentMsgCache.size() >= maxSentMsgCacheSize)
            sender->m_sentMsgCache.erase();
    }

    switch (type)
    {
        case CHAT_MSG_SAY:
        case CHAT_MSG_EMOTE:
        case CHAT_MSG_YELL:
        {
            if (sender->getLevel() < sWorld->getIntConfig(CONFIG_CHAT_SAY_LEVEL_REQ))
            {
                SendNotification(GetTrinityString(LANG_SAY_REQ), sWorld->getIntConfig(CONFIG_CHAT_SAY_LEVEL_REQ));
                return;
            }

            if (type == CHAT_MSG_SAY)
                sender->Say(msg, lang);
            else if (type == CHAT_MSG_EMOTE)
                sender->TextEmote(msg);
            else if (type == CHAT_MSG_YELL)
                sender->Yell(msg, lang);
            break;
        }
        case CHAT_MSG_WHISPER:
        {
            if (sender->getLevel() < sWorld->getIntConfig(CONFIG_CHAT_WHISPER_LEVEL_REQ))
            {
                SendNotification(GetTrinityString(LANG_WHISPER_REQ), sWorld->getIntConfig(CONFIG_CHAT_WHISPER_LEVEL_REQ));
                return;
            }

            if (!normalizePlayerName(target))
            {
                SendPlayerNotFoundNotice(target);
                break;
            }

            Player* receiver = sObjectAccessor->FindPlayerByName(target.c_str());
            bool senderIsPlayer = AccountMgr::IsPlayerAccount(GetSecurity());
            bool receiverIsPlayer = AccountMgr::IsPlayerAccount(receiver ? receiver->GetSession()->GetSecurity() : SEC_PLAYER);
            if (!receiver || (senderIsPlayer && !receiverIsPlayer && !receiver->isAcceptWhispers() && !receiver->IsInWhisperWhiteList(sender->GetGUID())))
            {
                SendPlayerNotFoundNotice(target);
                return;
            }

            if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHAT) && senderIsPlayer && receiverIsPlayer)
                if (GetPlayer()->GetTeam() != receiver->GetTeam())
                {
                    SendPlayerNotFoundNotice(target);
                    return;
                }

            if (GetPlayer()->HasAura(1852) && !receiver->isGameMaster())
            {
                SendNotification(GetTrinityString(LANG_GM_SILENCE), GetPlayer()->GetName());
                return;
            }

            // If player is a Gamemaster and doesn't accept whisper, we auto-whitelist every player that the Gamemaster is talking to
            if (!senderIsPlayer && !sender->isAcceptWhispers() && !sender->IsInWhisperWhiteList(receiver->GetGUID()))
                sender->AddWhisperWhiteList(receiver->GetGUID());

            GetPlayer()->Whisper(msg, lang, receiver->GetGUID());
            break;
        }
        case CHAT_MSG_PARTY:
        {
            // if player is in battleground, he cannot say to battleground members by /p
            Group* group = GetPlayer()->GetOriginalGroup();
            if (!group)
            {
                group = _player->GetGroup();
                if (!group || group->isBGGroup())
                    return;
            }

            if (group->IsLeader(GetPlayer()->GetGUID()))
                type = CHAT_MSG_PARTY_LEADER;

            sScriptMgr->OnPlayerChat(GetPlayer(), type, lang, msg, group);

            WorldPacket data;
            ChatHandler::FillMessageData(&data, this, uint8(type), lang, NULL, ObjectGuid::Empty, msg.c_str(), NULL);
            group->BroadcastPacket(&data, false, group->GetMemberGroup(GetPlayer()->GetGUID()));
            break;
        }
        case CHAT_MSG_GUILD:
        {
            if (GetPlayer()->GetGuildId())
            {
                if (Guild* guild = sGuildMgr->GetGuildById(GetPlayer()->GetGuildId()))
                {
                    sScriptMgr->OnPlayerChat(GetPlayer(), type, lang, msg, guild);

                    guild->BroadcastToGuild(this, false, msg, lang == LANG_ADDON ? LANG_ADDON : LANG_UNIVERSAL);
                }
            }
            break;
        }
        case CHAT_MSG_OFFICER:
        {
            if (GetPlayer()->GetGuildId())
            {
                if (Guild* guild = sGuildMgr->GetGuildById(GetPlayer()->GetGuildId()))
                {
                    sScriptMgr->OnPlayerChat(GetPlayer(), type, lang, msg, guild);

                    guild->BroadcastToGuild(this, true, msg, lang == LANG_ADDON ? LANG_ADDON : LANG_UNIVERSAL);
                }
            }
            break;
        }
        case CHAT_MSG_RAID:
        {
            // if player is in battleground, he cannot say to battleground members by /ra
            Group* group = GetPlayer()->GetOriginalGroup();
            if (!group)
            {
                group = GetPlayer()->GetGroup();
                if (!group || group->isBGGroup() || !group->isRaidGroup())
                    return;
            }

            if (group->IsLeader(GetPlayer()->GetGUID()))
                type = CHAT_MSG_RAID_LEADER;

            sScriptMgr->OnPlayerChat(GetPlayer(), type, lang, msg, group);

            WorldPacket data;
            ChatHandler::FillMessageData(&data, this, uint8(type), lang, "", ObjectGuid::Empty, msg.c_str(), NULL);
            group->BroadcastPacket(&data, false);
            break;
        }
        case CHAT_MSG_RAID_WARNING:
        {
            Group* group = GetPlayer()->GetGroup();
            if (!group || !group->isRaidGroup() || !(group->IsLeader(GetPlayer()->GetGUID()) || group->IsAssistant(GetPlayer()->GetGUID()) || group->GetGroupType() & GROUPTYPE_EVERYONE_IS_ASSISTANT) || group->isBGGroup())
                return;

            sScriptMgr->OnPlayerChat(GetPlayer(), type, lang, msg, group);

            WorldPacket data;
            //in battleground, raid warning is sent only to players in battleground - code is ok
            ChatHandler::FillMessageData(&data, this, CHAT_MSG_RAID_WARNING, lang, "", ObjectGuid::Empty, msg.c_str(), NULL);
            group->BroadcastPacket(&data, false);
            break;
        }
        case CHAT_MSG_INSTANCE:
        {
            // battleground raid is always in Player->GetGroup(), never in GetOriginalGroup()
            Group* group = GetPlayer()->GetGroup();
            // checks are not known yet
            if (!group/* || !group->isBGGroup()*/)
                return;

            if (group->IsLeader(GetPlayer()->GetGUID()))
                type = CHAT_MSG_INSTANCE_LEADER;

            sScriptMgr->OnPlayerChat(GetPlayer(), type, lang, msg, group);

            WorldPacket data;
            ChatHandler::FillMessageData(&data, this, uint8(type), lang, "", ObjectGuid::Empty, msg.c_str(), NULL);
            group->BroadcastPacket(&data, false);
            break;
        }
        case CHAT_MSG_CHANNEL:
        {
            if (AccountMgr::IsPlayerAccount(GetSecurity()))
            {
                if (_player->getLevel() < sWorld->getIntConfig(CONFIG_CHAT_CHANNEL_LEVEL_REQ))
                {
                    SendNotification(GetTrinityString(LANG_CHANNEL_REQ), sWorld->getIntConfig(CONFIG_CHAT_CHANNEL_LEVEL_REQ));
                    return;
                }
            }

            if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
            {

                if (Channel* chn = cMgr->GetChannel(target, _player))
                {
                    sScriptMgr->OnPlayerChat(_player, type, lang, msg, chn);

                    chn->Say(_player->GetGUID(), msg.c_str(), lang);
                }
            }
            break;
        }
        case CHAT_MSG_AFK:
        {
            if ((msg.empty() || !_player->isAFK()) && !_player->isInCombat())
            {
                if (!_player->isAFK())
                {
                    if (msg.empty())
                        msg  = GetTrinityString(LANG_PLAYER_AFK_DEFAULT);
                    _player->afkMsg = msg;
                }

                sScriptMgr->OnPlayerChat(_player, type, lang, msg);

                _player->ToggleAFK();
                if (_player->isAFK() && _player->isDND())
                    _player->ToggleDND();
            }
            break;
        }
        case CHAT_MSG_DND:
        {
            if (msg.empty() || !_player->isDND())
            {
                if (!_player->isDND())
                {
                    if (msg.empty())
                        msg = GetTrinityString(LANG_PLAYER_DND_DEFAULT);
                    _player->dndMsg = msg;
                }

                sScriptMgr->OnPlayerChat(_player, type, lang, msg);

                _player->ToggleDND();
                if (_player->isDND() && _player->isAFK())
                    _player->ToggleAFK();
            }
            break;
        }
        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "CHAT: unknown message type %u, lang: %u", type, lang);
            break;
    }
}

void WorldSession::HandleChatAddonMessageOpcode(WorldPackets::Chat::ChatAddonMessage& packet)
{
    ChatMsg type;

    switch (packet.GetOpcode())
    {
        case CMSG_CHAT_ADDON_MESSAGE_GUILD:
            type = CHAT_MSG_GUILD;
            break;
        case CMSG_MESSAGECHAT_ADDON_INSTANCE:
            type = CHAT_MSG_INSTANCE;
            break;
        case CMSG_CHAT_ADDON_MESSAGE_OFFICER:
            type = CHAT_MSG_OFFICER;
            break;
        case CMSG_CHAT_ADDON_MESSAGE_PARTY:
            type = CHAT_MSG_PARTY;
            break;
        case CMSG_CHAT_ADDON_MESSAGE_RAID:
            type = CHAT_MSG_RAID;
            break;
        case CMSG_CHAT_ADDON_MESSAGE_WHISPER:    //WorldSession::HandleChatAddonMessageWhisperOpcode
            type = CHAT_MSG_WHISPER;
            break;
        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "HandleAddonMessagechatOpcode: Unknown addon chat opcode (%u)", packet.GetOpcode());
            return;
    }

    HandleChatAddonMessage(type, packet.Prefix, packet.Text);
}

void WorldSession::HandleChatAddonMessageWhisperOpcode(WorldPackets::Chat::ChatAddonMessageWhisper& packet)
{
    HandleChatAddonMessage(CHAT_MSG_WHISPER, packet.Prefix, packet.Text, packet.Target);
}

void WorldSession::HandleChatAddonMessage(ChatMsg type, std::string prefix, std::string message, std::string targetName /*= ""*/)
{
    Player* sender = GetPlayer();

    if (prefix.empty() || prefix.length() > 16)
        return;

    // Logging enabled?
    if (sWorld->getBoolConfig(CONFIG_CHATLOG_ADDON))
    {
        if (message.empty())
            return;

        // Weird way to log stuff...
        sScriptMgr->OnPlayerChat(sender, CHAT_MSG_ADDON, LANG_ADDON, message);
    }

    // Disabled addon channel?
    if (!sWorld->getBoolConfig(CONFIG_ADDON_CHANNEL))
        return;

    switch (type)
    {
        case CHAT_MSG_INSTANCE:
        {
            Group* group = sender->GetGroup();
            if (!group)
                return;

            WorldPacket data;
            ChatHandler::FillMessageData(&data, this, type, LANG_ADDON, "", ObjectGuid::Empty, message.c_str(), NULL, prefix.c_str());
            group->BroadcastAddonMessagePacket(&data, prefix, false);
            break;
        }
        case CHAT_MSG_GUILD:
        case CHAT_MSG_OFFICER:
        {
            if (sender->GetGuildId())
                if (Guild* guild = sGuildMgr->GetGuildById(sender->GetGuildId()))
                    guild->BroadcastAddonToGuild(this, type == CHAT_MSG_OFFICER, message, prefix);
            break;
        }
        case CHAT_MSG_WHISPER:
        {
            if (!normalizePlayerName(targetName))
                break;
            Player* receiver = sObjectAccessor->FindPlayerByName(targetName.c_str());
            if (!receiver)
                break;

            sender->WhisperAddon(message, prefix, receiver);
            break;
        }
        // Messages sent to "RAID" while in a party will get delivered to "PARTY"
        case CHAT_MSG_PARTY:
        case CHAT_MSG_RAID:
        {

            Group* group = sender->GetGroup();
            if (!group || group->isBGGroup())
                break;

            WorldPacket data;
            ChatHandler::FillMessageData(&data, this, type, LANG_ADDON, "", ObjectGuid::Empty, message.c_str(), NULL, prefix.c_str());
            group->BroadcastAddonMessagePacket(&data, prefix, true, -1, sender->GetGUID());
            break;
        }
        default:
        {
            sLog->outError(LOG_FILTER_GENERAL, "HandleAddonMessagechatOpcode: unknown addon message type %u", type);
            break;
        }
    }
}

void WorldSession::HandleEmoteOpcode(WorldPacket & recvData)
{
    if (!GetPlayer()->isAlive() || GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        return;

    uint32 emote;
    recvData >> emote;
    sScriptMgr->OnPlayerEmote(GetPlayer(), emote);
    GetPlayer()->HandleEmoteCommand(emote);
}

void WorldSession::HandleTextEmoteOpcode(WorldPackets::Chat::CTextEmote& packet)
{
    if (!GetPlayer()->isAlive())
        return;

    if (!GetPlayer()->CanSpeak())
    {
        std::string timeStr = secsToTimeString(m_muteTime - time(NULL));
        SendNotification(GetTrinityString(LANG_WAIT_BEFORE_SPEAKING), timeStr.c_str());
        return;
    }

    sScriptMgr->OnPlayerTextEmote(GetPlayer(), packet.SoundIndex, packet.EmoteID, packet.Target);

    EmotesTextEntry const* em = sEmotesTextStore.LookupEntry(packet.EmoteID);
    if (!em)
        return;

    uint32 emote_anim = em->textid;

    switch (emote_anim)
    {
        case EMOTE_STATE_SLEEP:
        case EMOTE_STATE_SIT:
        case EMOTE_STATE_KNEEL:
        case EMOTE_ONESHOT_NONE:
            break;
        case EMOTE_STATE_DANCE:
        case EMOTE_STATE_READ:
            _player->SetUInt32Value(UNIT_NPC_EMOTESTATE, emote_anim);
            break;
        default:
            // Only allow text-emotes for "dead" entities (feign death included)
            if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
                break;
             GetPlayer()->HandleEmoteCommand(emote_anim);
             break;
    }

    WorldPackets::Chat::STextEmote textEmote;
    textEmote.SourceGUID = _player->GetGUID();
    textEmote.SourceAccountGUID = GetAccountGUID();
    textEmote.TargetGUID = packet.Target;
    textEmote.EmoteID = packet.EmoteID;
    textEmote.SoundIndex = packet.SoundIndex;
    _player->SendMessageToSetInRange(textEmote.Write(), sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_TEXTEMOTE), true);

    Unit* unit = ObjectAccessor::GetUnit(*_player, packet.Target);

    GetPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE, packet.SoundIndex, 0, unit);

    //Send scripted event call
    if (unit && unit->GetTypeId() == TYPEID_UNIT && ((Creature*)unit)->AI())
        ((Creature*)unit)->AI()->ReceiveEmote(GetPlayer(), packet.SoundIndex);
}

void WorldSession::HandleChatIgnoredOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint8 unk;
    //sLog->outDebug(LOG_FILTER_PACKETIO, "WORLD: Received CMSG_CHAT_IGNORED");

    recvData >> unk;                                       // probably related to spam reporting

    //recvData.ReadGuidMask<0, 6, 7, 3, 2, 1, 4, 5>(guid);
    //recvData.ReadGuidBytes<7, 5, 2, 6, 0, 1, 3, 4>(guid);

    Player* player = ObjectAccessor::FindPlayer(guid);
    if (!player || !player->GetSession())
        return;

    WorldPacket data;
    ChatHandler::FillMessageData(&data, this, CHAT_MSG_IGNORED, LANG_UNIVERSAL, NULL, GetPlayer()->GetGUID(), GetPlayer()->GetName(), NULL);
    player->GetSession()->SendPacket(&data);
}

void WorldSession::HandleChannelDeclineInvite(WorldPacket &recvPacket)
{
    recvPacket.rfinish();
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
}

//! 5.4.1
void WorldSession::SendPlayerNotFoundNotice(std::string name)
{
    WorldPacket data(SMSG_CHAT_PLAYER_NOTFOUND, name.size()+1);
    data.WriteBits(name.size(), 9);
    data.WriteString(name);
    SendPacket(&data);
}

//! 5.4.1
void WorldSession::SendPlayerAmbiguousNotice(std::string name)
{
    WorldPacket data(SMSG_CHAT_PLAYER_AMBIGUOUS, name.size()+1);
    data.WriteBits(name.size(), 9);
    data.WriteString(name);
    SendPacket(&data);
}

//! 5.4.1
void WorldSession::SendChatRestrictedNotice(ChatRestrictionType restriction)
{
    WorldPacket data(SMSG_CHAT_RESTRICTED, 1);
    data << uint8(restriction);
    SendPacket(&data);
}
