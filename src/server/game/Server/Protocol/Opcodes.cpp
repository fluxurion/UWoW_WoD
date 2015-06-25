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

#include "Opcodes.h"
#include "WorldSession.h"
#include "Packets/AchievementPackets.h"
#include "Packets/CharacterPackets.h"
#include "Packets/ChannelPackets.h"
#include "Packets/ChatPackets.h"
#include "Packets/ClientConfigPackets.h"
#include "Packets/CombatPackets.h"
#include "Packets/DuelPackets.h"
#include "Packets/EquipmentSetPackets.h"
#include "Packets/GameObjectPackets.h"
#include "Packets/GuildPackets.h"
#include "Packets/ItemPackets.h"
#include "Packets/LootPackets.h"
#include "Packets/MailPackets.h"
#include "Packets/MiscPackets.h"
#include "Packets/MovementPackets.h"
#include "Packets/NPCPackets.h"
#include "Packets/QueryPackets.h"
#include "Packets/QuestPackets.h"
#include "Packets/TalentPackets.h"
#include "Packets/TradePackets.h"
#include "Packets/WhoPackets.h"
#include "Packets/SocialPackets.h"
#include "Packets/BattlePayPackets.h"

template<class PacketClass, void(WorldSession::*HandlerFunction)(PacketClass&)>
class PacketHandler : public ClientOpcodeHandler
{
public:
    PacketHandler(char const* name, SessionStatus status, PacketProcessing processing) : ClientOpcodeHandler(name, status, processing) { }

    void Call(WorldSession* session, WorldPacket& packet) const override
    {
        PacketClass nicePacket(std::move(packet));
        nicePacket.Read();
        (session->*HandlerFunction)(nicePacket);
    }
};

template<void(WorldSession::*HandlerFunction)(WorldPacket&)>
class PacketHandler<WorldPacket, HandlerFunction> : public ClientOpcodeHandler
{
public:
    PacketHandler(char const* name, SessionStatus status, PacketProcessing processing) : ClientOpcodeHandler(name, status, processing) { }

    void Call(WorldSession* session, WorldPacket& packet) const override
    {
        (session->*HandlerFunction)(packet);
    }
};

OpcodeTable opcodeTable;

template<class PacketClass, void(WorldSession::*HandlerFunction)(PacketClass&)>
void OpcodeTable::ValidateAndSetClientOpcode(OpcodeClient opcode, char const* name, SessionStatus status, PacketProcessing processing)
{
    if (uint32(opcode) == NULL_OPCODE)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Opcode %s does not have a value", name);
        return;
    }

    if (uint32(opcode) >= NUM_OPCODE_HANDLERS)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Tried to set handler for an invalid opcode %d", opcode);
        return;
    }

    if (_internalTableClient[opcode] != NULL)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Tried to override client handler of %s with %s (opcode %u)", GetOpcodeNameForLogging(opcode).c_str(), name, opcode);
        return;
    }

    _internalTableClient[opcode] = new PacketHandler<PacketClass, HandlerFunction>(name, status, processing);
}

void OpcodeTable::ValidateAndSetServerOpcode(OpcodeServer opcode, char const* name, SessionStatus status)
{
    if (uint32(opcode) == NULL_OPCODE)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Opcode %s does not have a value", name);
        return;
    }

    if (uint32(opcode) >= NUM_OPCODE_HANDLERS)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Tried to set handler for an invalid opcode %d", opcode);
        return;
    }

    if (_internalTableServer[opcode] != NULL)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Tried to override server handler of %s with %s (opcode %u)", GetOpcodeNameForLogging(opcode).c_str(), name, opcode);
        return;
    }

    _internalTableServer[opcode] = new ServerOpcodeHandler(name, status);
}

/// Correspondence between opcodes and their names
void OpcodeTable::Initialize()
{

#define DEFINE_OPCODE_HANDLER_OLD(opcode, status, processing, handler) \
    ValidateAndSetClientOpcode<WorldPacket, handler>(opcode, #opcode, status, processing);

#define DEFINE_HANDLER(opcode, status, processing, packetclass, handler) \
    ValidateAndSetClientOpcode<packetclass, handler>(opcode, #opcode, status, processing);

    //------------                      C M S G                  ------------//

    DEFINE_OPCODE_HANDLER_OLD(CMSG_ACCEPT_LEVEL_GRANT,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAcceptGrantLevel          );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_ACCEPT_TRADE,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAcceptTradeOpcode         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_ACTIVATE_TAXI,                           STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleActivateTaxiOpcode        );//612
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_ACTIVATE_TAXI_EXPRESS,                   STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleActivateTaxiExpressOpcode );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_REGISTER_ADDON_PREFIXES,            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAddonRegisteredPrefixesOpcode);//603
    DEFINE_HANDLER(CMSG_ADD_FRIEND,                                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Social::AddFriend, &WorldSession::HandleAddFriendOpcode);//603
    DEFINE_HANDLER(CMSG_ADD_IGNORE,                                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Social::AddIgnore, &WorldSession::HandleAddIgnoreOpcode);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_ADD_VOICE_IGNORE,                        STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_ALTER_APPEARANCE,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAlterAppearance           );//603
    DEFINE_HANDLER(CMSG_AREA_TRIGGER,                                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Misc::AreaTrigger, &WorldSession::HandleAreaTriggerOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AREA_SPIRIT_HEALER_QUERY,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAreaSpiritHealerQueryOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AREA_SPIRIT_HEALER_QUEUE,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAreaSpiritHealerQueueOpcode);//603
    DEFINE_HANDLER(CMSG_ATTACK_STOP,                                        STATUS_LOGGEDIN,  PROCESS_INPLACE,      WorldPackets::Combat::AttackStop, &WorldSession::HandleAttackStopOpcode);//603
    DEFINE_HANDLER(CMSG_ATTACK_SWING,                                       STATUS_LOGGEDIN,  PROCESS_INPLACE,      WorldPackets::Combat::AttackSwing, &WorldSession::HandleAttackSwingOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AUCTION_LIST_BIDDER_ITEMS,               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionListBidderItems    );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AUCTION_LIST_ITEMS,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionListItems          );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AUCTION_LIST_OWNER_ITEMS,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionListOwnerItems     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AUCTION_LIST_PENDING_SALES,              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionListPendingSales   );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AUCTION_PLACE_BID,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionPlaceBid           );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AUCTION_REMOVE_ITEM,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionRemoveItem         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AUCTION_SELL_ITEM,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionSellItem           );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AUTH_SESSION,                            STATUS_NEVER,     PROCESS_THREADUNSAFE, &WorldSession::Handle_EarlyProccess            );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AUTOBANK_ITEM,                           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAutoBankItemOpcode        );//603
    DEFINE_HANDLER(CMSG_AUTO_EQUIP_ITEM,                                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Item::AutoEquipItem, &WorldSession::HandleAutoEquipItemOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AUTO_EQUIP_ITEM_SLOT,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAutoEquipItemSlotOpcode   );//603
    DEFINE_HANDLER(CMSG_AUTO_STORE_BAG_ITEM,                                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Item::AutoStoreBagItem, &WorldSession::HandleAutoStoreBagItemOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AUTOSTORE_BANK_ITEM,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAutoStoreBankItemOpcode   );//603
    DEFINE_HANDLER(CMSG_LOOT_ITEM,                                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Loot::AutoStoreLootItem, &WorldSession::HandleAutostoreLootItemOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_DECLINE_GUILD_INVITES,                   STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleAutoDeclineGuildInvites   );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BANKER_ACTIVATE,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBankerActivateOpcode      );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLEFIELD_LEAVE,                       STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleBfExitRequest             );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLEFIELD_LIST,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlefieldListOpcode     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BF_MGR_ENTRY_INVITE_RESPONSE,            STATUS_LOGGEDIN, PROCESS_INPLACE, &WorldSession::HandleBfEntryInviteResponse);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BF_MGR_QUEUE_EXIT_REQUEST,               STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleBfExitQueueRequest        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLEFIELD_LEAVE,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLeaveBattlefieldOpcode    );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PET_SET_SPECIALIZATION,                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLearnPetSpecialization    );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_LFD_PLAYER_LOCK_INFO_REQUEST,            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLfgPlayerLockInfoRequestOpcode);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BF_MGR_QUEUE_INVITE_RESPONSE,            STATUS_LOGGEDIN, PROCESS_INPLACE, &WorldSession::HandleBfQueueInviteResponse);//603
    //DEFINE_OPCODE_HANDLER_OLD(SMSG_BF_MGR_QUEUE_REQUEST_RESPONSE,           STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleBfQueueRequest            );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLEFIELD_PORT,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattleFieldPortOpcode     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_BATTLEFIELD_STATUS,              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlefieldStatusOpcode   );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLEMASTER_JOIN,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlemasterJoinOpcode    );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLEMASTER_JOIN_ARENA,                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlemasterJoinArena     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_JOIN_RATED_BATTLEGROUND,                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlemasterJoinRated     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BEGIN_TRADE,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBeginTradeOpcode          );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BINDER_ACTIVATE,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBinderActivateOpcode      );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SUPPORT_TICKET_SUBMIT_BUG,               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBugOpcode                 );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BUSY_TRADE,                              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBusyTradeOpcode           );//603
    DEFINE_HANDLER(CMSG_BUY_BACK_ITEM,                                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Item::BuyBackItem, &WorldSession::HandleBuybackItem);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BUY_BANK_SLOT,                           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBuyBankSlotOpcode         );//603
    DEFINE_HANDLER(CMSG_BUY_ITEM,                                           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Item::BuyItem, &WorldSession::HandleBuyItemOpcode);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_ADD_EVENT,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarAddEvent          );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_ARENA_TEAM,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarArenaTeam         );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_COMPLAIN,                       STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarComplain          );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_COPY_EVENT,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarCopyEvent         );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_EVENT_INVITE,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarEventInvite       );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_EVENT_MODERATOR_STATUS,         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarEventModeratorStatus);
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_EVENT_REMOVE_INVITE,            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarEventRemoveInvite );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_EVENT_RSVP,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarEventRsvp         );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_EVENT_SIGNUP,                   STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_EVENT_STATUS,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarEventStatus       );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_GET,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarGetCalendar       );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_GET_EVENT,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarGetEvent          );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_GET_NUM_PENDING,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarGetNumPending     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_GUILD_FILTER,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarGuildFilter       );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_REMOVE_EVENT,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarRemoveEvent       );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_UPDATE_EVENT,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCalendarUpdateEvent       );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CANCEL_AURA,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCancelAuraOpcode          );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CANCEL_AUTO_REPEAT_SPELL,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCancelAutoRepeatSpellOpcode);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CANCEL_CAST,                             STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleCancelCastOpcode          );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CANCEL_CHANNELLING,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCancelChanneling          );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CANCEL_MOUNT_AURA,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCancelMountAuraOpcode     );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CANCEL_QUEUED_SPELL,                     STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CANCEL_TEMP_ENCHANTMENT,                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCancelTempEnchantmentOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CANCEL_TRADE,                            STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT, PROCESS_THREADUNSAFE, &WorldSession::HandleCancelTradeOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CAN_DUEL,                                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSendDuelRequest);//603
    DEFINE_HANDLER(CMSG_CAST_SPELL,                                         STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   WorldPackets::Spells::CastSpell, &WorldSession::HandleCastSpellOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHALLENGE_MODE_REQUEST_REWARD_INFO,      STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleChallengeModeRequestRewardInfoOpcode);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHALLENGE_MODE_REQUEST_COMPLETION_INFO,  STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleChallengeModeRequestCompletionInfoOpcode);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHALLENGE_MODE_REQUEST_LEADERS,          STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleChallengeModeRequestOpcode);
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHANGEPLAYER_DIFFICULTY,                 STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_CHANGE_VEHICLE_SEATS,      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChangeSeatsOnControlledVehicle);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_ANNOUNCEMENTS,              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelAnnouncements      );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_BAN,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelBan                );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_DISPLAY_LIST,               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelDisplayListQuery   );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_INVITE,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelInvite             );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_KICK,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelKick               );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_LIST,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelList               );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_MODERATOR,                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelModerator          );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_MUTE,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelMute               );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_OWNER,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelOwner              );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_PASSWORD,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelPassword           );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_SET_OWNER,                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelSetOwner           );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_SILENCE_ALL,                     STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_SILENCE_VOICE,                   STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_UNBAN,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelUnban              );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_UNMODERATOR,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelUnmoderator        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_UNMUTE,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChannelUnmute             );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_UNSILENCE_ALL,                   STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_UNSILENCE_VOICE,                 STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_VOICE_OFF,                       STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_CHANNEL_VOICE_ON,                        STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleChannelVoiceOnOpcode      );
    DEFINE_HANDLER(CMSG_CREATE_CHARACTER,                                   STATUS_AUTHED,    PROCESS_THREADUNSAFE, WorldPackets::Character::CreateChar, &WorldSession::HandleCharCreateOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAR_CUSTOMIZE,                          STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleCharCustomize             );//603
    DEFINE_HANDLER(CMSG_CHAR_DELETE,                                        STATUS_AUTHED,    PROCESS_THREADUNSAFE, WorldPackets::Character::DeleteChar, &WorldSession::HandleCharDeleteOpcode);//603
    DEFINE_HANDLER(CMSG_ENUM_CHARACTERS,                                    STATUS_AUTHED,    PROCESS_THREADUNSAFE, WorldPackets::Character::EnumCharacters, &WorldSession::HandleCharEnumOpcode);//612
    DEFINE_HANDLER(CMSG_ENUM_CHARACTERS_DELETED_BY_CLIENT,                  STATUS_AUTHED,    PROCESS_THREADUNSAFE, WorldPackets::Character::EnumCharacters, &WorldSession::HandleCharEnumOpcode);//612
    DEFINE_OPCODE_HANDLER_OLD(CMSG_UNDELETE_CHARACTER,                      STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleCharUndelete);//612
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAR_RACE_OR_FACTION_CHANGE,             STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleCharFactionOrRaceChange   );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHARACTER_RENAME_REQUEST,                STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleCharRenameOpcode          );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_FILTERED,                           STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_REPORT_IGNORED,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChatIgnoredOpcode         );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CLEAR_CHANNEL_WATCH,                     STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CLEAR_RAID_MARKER,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleClearRaidMarkerOpcode     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CLEAR_TRADE_ITEM,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleClearTradeItemOpcode      );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CLOSE_INTERACTION,                       STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_COMPLAIN,                                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleComplainOpcode            );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_COMPLETE_CINEMATIC,                      STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleCompleteCinematic         );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_COMPLETE_MOVIE,                          STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleCompleteMovie             );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CONFIRM_RESPEC_WIPE,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTalentWipeConfirmOpcode   );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_CONNECT_TO_FAILED,                       STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_HANDLER(CMSG_SEND_CONTACT_LIST,                                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Social::SendContactList, &WorldSession::HandleContactListOpcode);//603
    DEFINE_HANDLER(CMSG_QUERY_CREATURE,                                     STATUS_LOGGEDIN,  PROCESS_INPLACE,      WorldPackets::Query::QueryCreature, &WorldSession::HandleCreatureQuery);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_DANCE_QUERY,                             STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_DECLINE_CHANNEL_INVITE,                  STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleChannelDeclineInvite      );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_DELETE_EQUIPMENT_SET,                    STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleEquipmentSetDelete        );//603
    DEFINE_HANDLER(CMSG_DEL_FRIEND,                                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Social::DelFriend, &WorldSession::HandleDelFriendOpcode);//603
    DEFINE_HANDLER(CMSG_DEL_IGNORE,                                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Social::DelIgnore, &WorldSession::HandleDelIgnoreOpcode);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_DEL_VOICE_IGNORE,                        STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_HANDLER(CMSG_DESTROY_ITEM,                                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Item::DestroyItem, &WorldSession::HandleDestroyItemOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_DISMISS_VEHICLE,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleDismissControlledVehicle  );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_DISMISS_CRITTER,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleDismissCritter            );//603
    DEFINE_HANDLER(CMSG_DUEL_RESPONSE,                                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Duel::DuelResponse, &WorldSession::HandleDuelResponseOpcode);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_EJECT_PASSENGER,                         STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_EMOTE,                                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleEmoteOpcode               );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_TAXI_QUERY_AVAILABLE_NODES,              STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleTaxiQueryAvailableNodes   );//612
    DEFINE_OPCODE_HANDLER_OLD(CMSG_TAXI_REQUEST_EARLY_LANDING,              STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleTaxiRequestEarlyLandingOpcode);//612
    DEFINE_OPCODE_HANDLER_OLD(CMSG_ENABLE_NAGLE,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_EarlyProccess            );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SAVE_EQUIPMENT_SET,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleEquipmentSetSave          );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_USE_EQUIPMENT_SET,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleEquipmentSetUse           );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_FAR_SIGHT,                               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleFarSightOpcode            );//603
    DEFINE_HANDLER(CMSG_QUERY_GAME_OBJECT,                                  STATUS_LOGGEDIN,  PROCESS_INPLACE,      WorldPackets::Query::QueryGameObject, &WorldSession::HandleGameObjectQueryOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GAME_OBJ_REPORT_USE,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGameobjectReportUse       );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GAME_OBJ_USE,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGameObjectUseOpcode       );//603
    DEFINE_HANDLER(CMSG_MAIL_GET_LIST,                                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Mail::MailGetList, &WorldSession::HandleGetMailList);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GET_MIRROR_IMAGE_DATA,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMirrorImageDataRequest    );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_TICKET_RESPONSE_RESOLVE,              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGMResponseResolve         );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_SURVEY_SUBMIT,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGMSurveySubmit            );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_TICKET_CREATE,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGMTicketCreateOpcode      );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_TICKET_DELETE_TICKET,                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGMTicketDeleteOpcode      );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_TICKET_GET_TICKET,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGMTicketGetTicketOpcode   );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_TICKET_GET_SYSTEM_STATUS,             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGMTicketSystemStatusOpcode);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_TICKET_UPDATE_TEXT,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGMTicketUpdateOpcode      );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_REPORT_LAG,                           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleReportLag                 );
    DEFINE_HANDLER(CMSG_TALK_TO_GOSSIP,                                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::NPC::Hello, &WorldSession::HandleGossipHelloOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GOSSIP_SELECT_OPTION,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGossipSelectOptionOpcode  );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GRANT_LEVEL,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGrantLevel                );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_ASSISTANT_LEADER,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupAssistantLeaderOpcode);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHANGE_SUB_GROUP,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupChangeSubGroupOpcode );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_LEAVE_GROUP,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupDisbandOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PARTY_INVITE,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupInviteOpcode         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PARTY_INVITE_RESPONSE,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupInviteResponseOpcode );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CONVERT_RAID,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupRaidConvertOpcode    );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_GROUP_REQUEST_JOIN_UPDATES,              STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_PARTY_LEADER,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupSetLeaderOpcode      );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_ROLE,                                STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGroupSetRolesOpcode       );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SWAP_SUB_GROUPS,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupSwapSubGroupOpcode   );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PARTY_UNINVITE,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGroupUninviteOpcode       );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_ACCEPT_GUILD_INVITE,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildAcceptOpcode         );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_ACHIEVEMENT_MEMBERS,               STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_ACHIEVEMENT_PROGRESS_QUERY,        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildAchievementProgressQuery);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_ADD_RANK,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildAddRankOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_ASSIGN_MEMBER_RANK,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildAssignRankOpcode     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_AUTO_DECLINE_INVITATION,           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildDeclineOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_BANK_ACTIVATE,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankerActivate       );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_BANK_BUY_TAB,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankBuyTab           );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_BANK_DEPOSIT_MONEY,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankDepositMoney     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_BANK_LOG_QUERY,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankLogQuery         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_BANK_REMAINING_WITHDRAW_MONEY_QUERY,STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankMoneyWithdrawn  );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_BANK_QUERY_TAB,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankQueryTab         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_BANK_TEXT_QUERY,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQueryGuildBankTabText     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_BANK_SWAP_ITEMS,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankSwapItems        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_BANK_UPDATE_TAB,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankUpdateTab        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_BANK_WITHDRAW_MONEY,               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildBankWithdrawMoney    );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_BANK_SET_TAB_TEXT,                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetGuildBankTabText       );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_CHANGE_NAME_REQUEST,               STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_DECLINE_INVITATION,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildDeclineOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_DELETE_RANK,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildDelRankOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_DEMOTE_MEMBER,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildDemoteOpcode         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_DELETE,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildDisbandOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_EVENT_LOG_QUERY,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildEventLogQueryOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_UPDATE_INFO_TEXT,                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildChangeInfoTextOpcode );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_INVITE_BY_NAME,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildInviteOpcode         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_LEAVE,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildLeaveOpcode          );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_MEMBER_SEND_SOR_REQUEST,           STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_UPDATE_MOTD_TEXT,                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildMOTDOpcode           );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_NEWS_UPDATE_STICKY,                STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGuildNewsUpdateStickyOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_PERMISSIONS_QUERY,                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildPermissions          );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_PROMOTE_MEMBER,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildPromoteOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_GUILD_INFO,                        STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleGuildQueryOpcode          );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_QUERY_NEWS,                        STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGuildQueryNewsOpcode      );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_GET_RANKS,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildQueryRanksOpcode     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_OFFICER_REMOVE_MEMBER,             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildRemoveOpcode         );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_REPLACE_GUILD_MASTER,              STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_REQUEST_CHALLENGE_UPDATE,          STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_REQUEST_MAX_DAILY_XP,              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildRequestMaxDailyXP    );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_GUILD_PARTY_STATE,               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildRequestPartyState    );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_GET_ROSTER,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildRosterOpcode         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_SET_ACHIEVEMENT_TRACKING,          STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_SET_GUILD_MASTER,                  STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGuildLeaderOpcode         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_SET_MEMBER_NOTE,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildSetNoteOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_SET_RANK_PERMISSIONS,              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildSetRankPermissionsOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_SHIFT_RANK,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSwapRanks                 );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_HEARTH_AND_RESURRECT,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleHearthAndResurrect        );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_IGNORE_TRADE,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleIgnoreTradeOpcode         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_INITIATE_TRADE,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleInitiateTradeOpcode       );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_INSPECT,                                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleInspectOpcode             );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_INSTANCE_LOCK_RESPONSE,                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleInstanceLockResponse      );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_ITEM_PURCHASE_REFUND,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleItemRefund                );//603
    DEFINE_HANDLER(CMSG_GET_ITEM_PURCHASE_DATA,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Item::ItemRefundInfo, &WorldSession::HandleGetItemPurchaseData);//603
    DEFINE_HANDLER(CMSG_CHAT_JOIN_CHANNEL,                                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Channel::JoinChannel, &WorldSession::HandleJoinChannel);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_KEEP_ALIVE,                              STATUS_NEVER,     PROCESS_THREADUNSAFE, &WorldSession::Handle_EarlyProccess            );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_LEARN_TALENTS,                           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLearnTalents              );//603
    DEFINE_HANDLER(CMSG_CHAT_LEAVE_CHANNEL,                                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Channel::LeaveChannel, &WorldSession::HandleLeaveChannel);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_DF_GET_JOIN_STATUS,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLfgGetStatus              );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_DF_JOIN,                                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLfgJoinOpcode             );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_DF_LEAVE,                                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLfgLeaveOpcode            );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_DF_PROPOSAL_RESPONSE,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLfgProposalResultOpcode   );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_DF_BOOT_PLAYER_VOTE,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLfgSetBootVoteOpcode      );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_DF_SET_ROLES,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLfgSetRolesOpcode         );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_DF_TELEPORT,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLfgTeleportOpcode         );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_LF_GUILD_ADD_RECRUIT,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderAddRecruit     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_LF_GUILD_BROWSE,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderBrowse         );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_LF_GUILD_DECLINE_RECRUIT,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderDeclineRecruit );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_LF_GUILD_GET_APPLICATIONS,               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderGetApplications);
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_LF_GUILD_GET_RECRUITS,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderGetRecruits    );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_LF_GUILD_POST_REQUEST,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderPostRequest    );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_LF_GUILD_REMOVE_RECRUIT,                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderRemoveRecruit  );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_LF_GUILD_SET_GUILD_POST,                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildFinderSetGuildPost   );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_LIST_INVENTORY,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleListInventoryOpcode       );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_DF_SET_COMMENT,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLfgSetCommentOpcode       );
    DEFINE_HANDLER(CMSG_LOADING_SCREEN_NOTIFY,                              STATUS_AUTHED,    PROCESS_THREADUNSAFE, WorldPackets::Character::LoadingScreenNotify, &WorldSession::HandleLoadScreenOpcode);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_LOGOUT_CANCEL,                           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLogoutCancelOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_LOGOUT_REQUEST,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLogoutRequestOpcode       );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_LOG_DISCONNECT,                          STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::Handle_EarlyProccess            );
    DEFINE_HANDLER(CMSG_LOOT_UNIT,                                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Loot::LootUnit, &WorldSession::HandleLootOpcode);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_LOOT_CURRENCY,                           STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_MASTER_LOOT_ITEM,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLootMasterGiveOpcode      );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_LOOT_METHOD,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLootMethodOpcode          );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_LOOT_SPECIALIZATION,                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLootSpecIdOpcode          );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_LOOT_MONEY,                              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLootMoneyOpcode           );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_LOOT_RELEASE,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLootReleaseOpcode         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_LOOT_ROLL,                               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLootRoll                  );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_LOOT_MASTER_ASK_FOR_ROLL,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLootMasterAskForRoll      );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_MINIMAP_PING,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMinimapPingOpcode         );//603
    DEFINE_HANDLER(CMSG_MAIL_CREATE_TEXT_ITEM,                              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Mail::MailCreateTextItem, &WorldSession::HandleMailCreateTextItem);//603
    DEFINE_HANDLER(CMSG_MAIL_DELETE,                                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Mail::MailDelete, &WorldSession::HandleMailDelete);//603
    DEFINE_HANDLER(CMSG_MAIL_MARK_AS_READ,                                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Mail::MailMarkAsRead, &WorldSession::HandleMailMarkAsRead);//603
    DEFINE_HANDLER(CMSG_MAIL_RETURN_TO_SENDER,                              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Mail::MailReturnToSender, &WorldSession::HandleMailReturnToSender);//603
    DEFINE_HANDLER(CMSG_MAIL_TAKE_ITEM,                                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Mail::MailTakeItem, &WorldSession::HandleMailTakeItem);//603
    DEFINE_HANDLER(CMSG_MAIL_TAKE_MONEY,                                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Mail::MailTakeMoney, &WorldSession::HandleMailTakeMoney);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_MESSAGECHAT_ADDON_BATTLEGROUND,          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatAddonMessage, &WorldSession::HandleChatAddonMessageOpcode    );
    DEFINE_HANDLER(CMSG_CHAT_ADDON_MESSAGE_GUILD,                           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatAddonMessage, &WorldSession::HandleChatAddonMessageOpcode    );//603
    DEFINE_HANDLER(CMSG_MESSAGECHAT_ADDON_INSTANCE,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatAddonMessage, &WorldSession::HandleChatAddonMessageOpcode    );//603
    DEFINE_HANDLER(CMSG_CHAT_ADDON_MESSAGE_OFFICER,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatAddonMessage, &WorldSession::HandleChatAddonMessageOpcode    );//603
    DEFINE_HANDLER(CMSG_CHAT_ADDON_MESSAGE_PARTY,                           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatAddonMessage, &WorldSession::HandleChatAddonMessageOpcode    );//603
    DEFINE_HANDLER(CMSG_CHAT_ADDON_MESSAGE_RAID,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatAddonMessage, &WorldSession::HandleChatAddonMessageOpcode    );//603
    DEFINE_HANDLER(CMSG_CHAT_ADDON_MESSAGE_WHISPER,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatAddonMessage, &WorldSession::HandleChatAddonMessageOpcode    );//603
    DEFINE_HANDLER(CMSG_CHAT_MESSAGE_AFK,                                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessage, &WorldSession::HandleChatMessageOpcode         );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_MESSAGECHAT_BATTLEGROUND,               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessage, &WorldSession::HandleChatMessageOpcode         );
    DEFINE_HANDLER(CMSG_CHAT_MESSAGE_CHANNEL,                               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessageChannel, &WorldSession::HandleChatMessageChannelOpcode);//603
    DEFINE_HANDLER(CMSG_CHAT_MESSAGE_DND,                                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessage, &WorldSession::HandleChatMessageOpcode         );//603
    DEFINE_HANDLER(CMSG_CHAT_MESSAGE_EMOTE,                                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessageEmote, &WorldSession::HandleChatMessageEmoteOpcode);//603
    DEFINE_HANDLER(CMSG_CHAT_MESSAGE_GUILD,                                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessage, &WorldSession::HandleChatMessageOpcode         );//603
    //DEFINE_HANDLER(CMSG_MESSAGECHAT_INSTANCE,                               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessage, &WorldSession::HandleChatMessageOpcode         );
    DEFINE_HANDLER(CMSG_CHAT_MESSAGE_OFFICER,                               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessage, &WorldSession::HandleChatMessageOpcode         );//603
    DEFINE_HANDLER(CMSG_CHAT_MESSAGE_PARTY,                                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessage, &WorldSession::HandleChatMessageOpcode         );//603
    DEFINE_HANDLER(CMSG_CHAT_MESSAGE_RAID,                                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessage, &WorldSession::HandleChatMessageOpcode         );//603
    DEFINE_HANDLER(CMSG_CHAT_MESSAGE_RAID_WARNING,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessage, &WorldSession::HandleChatMessageOpcode         );//603
    DEFINE_HANDLER(CMSG_CHAT_MESSAGE_SAY,                                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessage, &WorldSession::HandleChatMessageOpcode         );//603
    DEFINE_HANDLER(CMSG_CHAT_MESSAGE_WHISPER,                               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessageWhisper, &WorldSession::HandleChatMessageWhisperOpcode);//603
    DEFINE_HANDLER(CMSG_CHAT_MESSAGE_YELL,                                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::ChatMessage, &WorldSession::HandleChatMessageOpcode         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_MOUNT_SPECIAL_ANIM,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleMountSpecialAnimOpcode    );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_NOT_ACTIVE_MOVER,                   STATUS_LOGGEDIN, PROCESS_THREADSAFE,   &WorldSession::HandleMoveNotActiveMover        );
    DEFINE_HANDLER(CMSG_MOVE_KNOCK_BACK_ACK,                                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_SPLINE_DONE,                        STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleMoveSplineDoneOpcode      );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_TIME_SKIPPED,                       STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleMoveTimeSkippedOpcode     );
    DEFINE_HANDLER(CMSG_QUERY_PLAYER_NAME,                                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Query::QueryPlayerName, &WorldSession::HandleNameQueryOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_NEXT_CINEMATIC_CAMERA,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleNextCinematicCamera       );
    DEFINE_HANDLER(CMSG_QUERY_NPC_TEXT,                                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Query::QueryNPCText, &WorldSession::HandleNpcTextQueryOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_OBJECT_UPDATE_FAILED,                    STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleObjectUpdateFailedOpcode  );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_OBJECT_UPDATE_RESCUED,                   STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_OFFER_PETITION,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleOfferPetitionOpcode       );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_OPENING_CINEMATIC,                       STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_OPEN_ITEM,                               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleOpenItemOpcode            );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_OPT_OUT_OF_LOOT,                         STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleOptOutOfLootOpcode        );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PARTY_ASSIGNMENT,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePartyAssignmentOpcode     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_LOGOUT_INSTANT,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePlayerLogoutOpcode        );
    DEFINE_HANDLER(CMSG_QUERY_PAGE_TEXT,                                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Query::QueryPageText, &WorldSession::HandlePageTextQueryOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PETITION_BUY,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionBuyOpcode         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_PETITION,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionQueryOpcode       );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PETITION_SHOW_LIST,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionShowListOpcode    );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PETITION_SHOW_SIGNATURES,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionShowSignOpcode    );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SIGN_PETITION,                           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionSignOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PETITION_RENAME_GUILD,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionRenameOpcode      );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PET_ABANDON,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetAbandon                );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PET_ACTION,                              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetAction                 );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PET_CANCEL_AURA,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetCancelAuraOpcode       );
    DEFINE_HANDLER(CMSG_PET_CAST_SPELL,                                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Spells::PetCastSpell, &WorldSession::HandlePetCastSpellOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_PET_NAME,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetNameQuery              );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PET_RENAME,                              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetRename                 );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PET_SET_ACTION,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetSetAction              );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PET_SPELL_AUTOCAST,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetSpellAutocastOpcode    );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PET_STOP_ATTACK,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetStopAttack             );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_CONQUEST_FORMULA_CONSTANTS,      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePersonalRatedInfoRequest  );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PING,                                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_EarlyProccess            );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_PLAYED_TIME,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePlayedTime                );//603
    DEFINE_HANDLER(CMSG_PLAYER_LOGIN,                                       STATUS_AUTHED,    PROCESS_THREADUNSAFE, WorldPackets::Character::PlayerLogin, &WorldSession::HandlePlayerLoginOpcode            );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_RIDE_VEHICLE_INTERACT,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleEnterPlayerVehicle        );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_PLAY_DANCE,                              STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_HANDLER(CMSG_CLIENT_PORT_GRAVEYARD,                              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Misc::PortGraveyard, &WorldSession::HandlePortGraveyard);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PURCHASE_GET_PRODUCT_LIST,               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePurchaseGetProductList    );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PURCHASE_GET_PURCHASE_LIST,              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePurchaseGetPurchaseList   );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PUSH_QUEST_TO_PARTY,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePushQuestToParty          );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_PVP_LOG_DATA,                            STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandlePVPLogDataOpcode          );//603
    DEFINE_HANDLER(CMSG_QUERY_CORPSE_LOCATION_FROM_CLIENT,                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Query::QueryCorpseLocationFromClient, &WorldSession::HandleQueryCorpseLocation);//603
    DEFINE_HANDLER(CMSG_QUERY_CORPSE_TRANSPORT,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Query::QueryCorpseTransport, &WorldSession::HandleQueryCorpseTransport);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_BATTLEFIELD_STATE,                 STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_QUERY_MEMBERS_FOR_RECIPE,          STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGuildQueryGuildMembersForRecipe);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_QUERY_MEMBER_RECIPES,              STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGuildQueryGuildMembersRecipes);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_QUERY_RECIPES,                     STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGuildQueryGuildRecipesOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_GUILD_REWARDS_LIST,              STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleGuildRewardsQueryOpcode   );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_GUILD_XP,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildQueryXPOpcode        ); // STATUS_AUTHED
    DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_INSPECT_ACHIEVEMENTS,              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQueryInspectAchievements  );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_PLAYER_RECIPES,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQueryPlayerRecipes        );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_TIME,                              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQueryTimeOpcode           );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_QUEST_GIVER_ACCEPT_QUEST,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverAcceptQuestOpcode);//603
    DEFINE_HANDLER(CMSG_QUEST_GIVER_CHOOSE_REWARD,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Quest::QuestGiverChooseReward, &WorldSession::HandleQuestgiverChooseRewardOpcode);//603
    DEFINE_HANDLER(CMSG_QUEST_GIVER_COMPLETE_QUEST,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Quest::QuestGiverCompleteQuest, &WorldSession::HandleQuestgiverCompleteQuest);//603
    DEFINE_HANDLER(CMSG_QUEST_GIVER_HELLO,                                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Quest::QuestGiverHello, &WorldSession::HandleQuestgiverHelloOpcode);//603
    DEFINE_HANDLER(CMSG_QUEST_GIVER_QUERY_QUEST,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Quest::QuestGiverQueryQuest, &WorldSession::HandleQuestgiverQueryQuestOpcode);//603
    DEFINE_HANDLER(CMSG_QUEST_GIVER_REQUEST_REWARD,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Quest::QuestGiverRequestReward, &WorldSession::HandleQuestgiverRequestRewardOpcode);//603
    DEFINE_HANDLER(CMSG_QUEST_GIVER_STATUS_MULTIPLE_QUERY,                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Quest::QuestGiverStatusMultipleQuery, &WorldSession::HandleQuestgiverStatusMultipleQuery);//603
    DEFINE_HANDLER(CMSG_QUEST_GIVER_STATUS_QUERY,                           STATUS_LOGGEDIN,  PROCESS_INPLACE,      WorldPackets::Quest::QuestGiverStatusQuery, &WorldSession::HandleQuestgiverStatusQueryOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_QUEST_LOG_REMOVE_QUEST,                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestLogRemoveQuest       );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_QUEST_CONFIRM_ACCEPT,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestConfirmAccept        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_QUEST_COMPLETION_NPCS,             STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleQuestNpcQuery             );//603
    DEFINE_HANDLER(CMSG_QUEST_POI_QUERY,                                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Query::QuestPOIQuery, &WorldSession::HandleQuestPOIQuery);//612
    DEFINE_HANDLER(CMSG_QUERY_QUEST_INFO,                                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Quest::QueryQuestInfo, &WorldSession::HandleQuestQueryOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_DO_READY_CHECK,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRaidLeaderReadyCheck      );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_READY_CHECK_RESPONSE,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRaidConfirmReadyCheck     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GENERATE_RANDOM_CHARACTER_NAME,          STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleRandomizeCharNameOpcode   );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_READ_ITEM,                               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleReadItem                  );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_REALM_NAME,                        STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleRealmQueryNameOpcode      );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_REALM_SPLIT,                             STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleRealmSplitOpcode          );
    DEFINE_HANDLER(CMSG_RECLAIM_CORPSE,                                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Misc::ReclaimCorpse, &WorldSession::HandleReclaimCorpse);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_REDIRECTION_AUTH_PROOF,                  STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_REFORGE_ITEM,                            STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleReforgeItemOpcode         );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REORDER_CHARACTERS,                      STATUS_AUTHED,    PROCESS_INPLACE,      &WorldSession::HandleReorderCharacters         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REPAIR_ITEM,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRepairItemOpcode          );//603
    DEFINE_HANDLER(CMSG_REPOP_REQUEST,                                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Misc::RepopRequest, &WorldSession::HandleRepopRequest);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REPORT_PVP_PLAYER_AFK,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleReportPvPAFK              );//603
    DEFINE_HANDLER(CMSG_REQUEST_ACCOUNT_DATA,                               STATUS_AUTHED,    PROCESS_THREADUNSAFE, WorldPackets::ClientConfig::RequestAccountData, &WorldSession::HandleRequestAccountData);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_CATEGORY_COOLDOWNS,              STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandlerCategoryCooldownOpocde    );
    DEFINE_HANDLER(CMSG_REQUEST_CEMETERY_LIST,                              STATUS_LOGGEDIN,  PROCESS_INPLACE,      WorldPackets::Misc::RequestCemeteryList, &WorldSession::HandleRequestCemeteryList);//603
    DEFINE_HANDLER(CMSG_DB_QUERY_BULK,                                      STATUS_AUTHED,    PROCESS_INPLACE,      WorldPackets::Query::DBQueryBulk, &WorldSession::HandleDBQueryBulk);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_INSPECT_PVP,                             STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleInspectRatedBGStats       );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_FORCED_REACTIONS,                STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleForcedReactions           );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_PARTY_MEMBER_STATS,              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRequestPartyMemberStatsOpcode);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_PET_INFO,                        STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleRequestPetInfoOpcode      );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GET_PVP_OPTIONS_ENABLED,                 STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleRequestPvpOptions         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_PVP_REWARDS,                     STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleRequestPvpReward          );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_RAID_INFO,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRequestRaidInfoOpcode     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_RATED_BATTLEFIELD_INFO,          STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleRequestRatedInfo          );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_RESEARCH_HISTORY,                STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleRequestResearchHistory    );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_VEHICLE_EXIT,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRequestVehicleExit        );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_VEHICLE_NEXT_SEAT,               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChangeSeatsOnControlledVehicle);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_VEHICLE_PREV_SEAT,               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChangeSeatsOnControlledVehicle);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_VEHICLE_SWITCH_SEAT,             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChangeSeatsOnControlledVehicle);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_CHANGE_VEHICLE_SEATS,               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleChangeSeatsOnControlledVehicle);
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_RESET_FACTION_CHEAT,                     STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_RESET_INSTANCES,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleResetInstancesOpcode      );//603
    DEFINE_HANDLER(CMSG_RESURRECT_RESPONSE,                                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Misc::ResurrectResponse, &WorldSession::HandleResurrectResponse);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_INITIATE_ROLE_POLL,                      STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleRolePollBegin             );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SAVE_GUILD_EMBLEM,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSaveGuildEmblemOpcode     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SAVE_CUF_PROFILES,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE,   &WorldSession::HandleSaveCUFProfiles         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SELF_RES,                                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSelfResOpcode             );
    DEFINE_HANDLER(CMSG_SELL_ITEM,                                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Item::SellItem, &WorldSession::HandleSellItemOpcode);//603
    DEFINE_HANDLER(CMSG_SEND_MAIL,                                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Mail::SendMail, &WorldSession::HandleSendMail);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SEND_SOR_REQUEST_VIA_ADDRESS,            STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SEND_SOR_REQUEST_VIA_BNET_ACCOUNT_ID,    STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_NEUTRAL_PLAYER_SELECT_FACTION,           STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleSetFactionOpcode          );//612
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_SHEATHED,                            STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleSetSheathedOpcode         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_ACTION_BAR_TOGGLES,                  STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleSetActionBarToggles       );//603
    DEFINE_HANDLER(CMSG_SET_ACTION_BUTTON,                                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Spells::SetActionButton, &WorldSession::HandleSetActionButtonOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_ACTIVE_MOVER,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetActiveMoverOpcode      );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_ACTIVE_VOICE_CHANNEL,                STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleSetActiveVoiceChannel     ); // STATUS_AUTHED //603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_ALLOW_LOW_LEVEL_RAID1,               STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_ALLOW_LOW_LEVEL_RAID2,               STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_CHANNEL_WATCH,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetChannelWatch           );
    DEFINE_HANDLER(CMSG_SET_CONTACT_NOTES,                                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Social::SetContactNotes, &WorldSession::HandleSetContactNotesOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_CURRENCY_FLAGS,                      STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleChangeCurrencyFlags       );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_EVERYONE_IS_ASSISTANT,               STATUS_LOGGEDIN,  PROCESS_INPLACE,       &WorldSession::HandleGroupEveryoneIsAssistantOpcode);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_FACTION_AT_WAR,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetFactionAtWar           );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_SET_VEHICLE_REC_ID_ACK,             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetVehicleRecId           );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_FACTION_NOT_AT_WAR,                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleUnsetFactionAtWar         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_LFG_BONUS_FACTION,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetLfgBonusFaction        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_FACTION_INACTIVE,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetFactionInactiveOpcode  );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_PET_SLOT,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleStableChangeSlot          );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_PLAYER_DECLINED_NAMES,               STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleSetPlayerDeclinedNames    );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_PREFERRED_CEMETERY,                  STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::HandleBfSetPreferedCemetry        );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_RELATIVE_POSITION,                   STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_SAVED_INSTANCE_EXTEND,               STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_HANDLER(CMSG_SET_SELECTION,                                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Misc::SetSelection, &WorldSession::HandleSetSelectionOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_SPECIALIZATION,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetSpecialization         );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_TAXI_BENCHMARK_MODE,                 STATUS_AUTHED,    PROCESS_THREADUNSAFE, &WorldSession::HandleSetTaxiBenchmarkOpcode    );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_TITLE,                               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetTitleOpcode            );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_TRADE_CURRENCY,                      STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_TRADE_GOLD,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetTradeGoldOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_TRADE_ITEM,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetTradeItemOpcode        );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_VEHICLE_REC_ID_AC,K                  STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_WATCHED_FACTION,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetWatchedFactionOpcode   );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SHOWING_CLOAK,                           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleShowingCloakOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SHOWING_HELM,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleShowingHelmOpcode         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SOCKET_GEMS,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSocketOpcode              );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SPELL_CLICK,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSpellClick                );//612
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SPIRIT_HEALER_ACTIVATE,                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSpiritHealerActivateOpcode);//603
    DEFINE_HANDLER(CMSG_SPLIT_ITEM,                                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Item::SplitItem, &WorldSession::HandleSplitItemOpcode);//603
    DEFINE_HANDLER(CMSG_STAND_STATE_CHANGE,                                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Misc::StandStateChange, &WorldSession::HandleStandStateChangeOpcode);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_STOP_DANCE,                              STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SUBMIT_BUG,                              STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SUBMIT_COMPLAIN,                         STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SUGGESTION_SUBMIT,                       STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SUMMON_BATTLEPET_COMPANION,              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSummonBattlePet           );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SUMMON_RESPONSE,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSummonResponseOpcode      );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SUSPEND_TOKEN_RESPONSE,                  STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::SuspendTokenResponse            );//603
    DEFINE_HANDLER(CMSG_SWAP_INV_ITEM,                                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Item::SwapInvItem, &WorldSession::HandleSwapInvItemOpcode);//603
    DEFINE_HANDLER(CMSG_SWAP_ITEM,                                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Item::SwapItem, &WorldSession::HandleSwapItem);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_SYNC_DANCE,                              STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_TAXI_NODE_STATUS_QUERY,                  STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleTaxiNodeStatusQueryOpcode );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_ENABLE_TAXI_NODE,                        STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleTaxiQueryAvailableNodes   );//612
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_TELEPORT_TO_UNIT,                        STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_HANDLER(CMSG_SEND_TEXT_EMOTE,                                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Chat::CTextEmote, &WorldSession::HandleTextEmoteOpcode);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_TIME_ADJUSTMENT_RESPONSE,                STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_HANDLER(CMSG_TIME_SYNC_RESPONSE,                                 STATUS_LOGGEDIN,  PROCESS_INPLACE,      WorldPackets::Misc::TimeSyncResponse, &WorldSession::HandleTimeSyncResp);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_TIME_SYNC_RESPONSE_DROPPED,              STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_TIME_SYNC_RESPONSE_FAILED,               STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_PVP,                                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTogglePvP                 );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_TOTEM_DESTROYED,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTotemDestroyed            );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_TRAINER_BUY_SPELL,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTrainerBuySpellOpcode     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_TRAINER_LIST,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTrainerListOpcode         );//603
    DEFINE_HANDLER(CMSG_TRANSMOGRIFY_ITEMS,                                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Item::TransmogrigyItem, &WorldSession::HandleTransmogrifyItems);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_UPGRADE_ITEM,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleUpgradeItem               );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_TURN_IN_PETITION,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTurnInPetitionOpcode      );//603
    DEFINE_HANDLER(CMSG_TUTORIAL,                                           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Misc::TutorialSetFlag, &WorldSession::HandleTutorialFlag);//612
    DEFINE_OPCODE_HANDLER_OLD(CMSG_UNACCEPT_TRADE,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleUnacceptTradeOpcode       );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_UNLEARN_SKILL,                           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleUnlearnSkillOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_UNLEARN_SPECIALIZATION,                  STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_CHAT_UNREGISTER_ALL_ADDON_PREFIXES,      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleUnregisterAddonPrefixesOpcode);//603
    DEFINE_HANDLER(CMSG_UPDATE_ACCOUNT_DATA,                                STATUS_AUTHED,    PROCESS_THREADUNSAFE, WorldPackets::ClientConfig::UserClientUpdateAccountData, &WorldSession::HandleUpdateAccountData);//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_UPDATE_MISSILE_TRAJECTORY,               STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleUpdateMissileTrajectory   );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_UPDATE_PROJECTILE_POSITION,              STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleUpdateProjectilePosition  );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_USED_FOLLOW,                             STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_HANDLER(CMSG_USE_ITEM,                                           STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Spells::ItemUse, &WorldSession::HandleUseItemOpcode             );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_VIOLENCE_LEVEL,                          STATUS_AUTHED,    PROCESS_INPLACE,      &WorldSession::HandleViolenceLevel             );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_VOICE_ADD_IGNORE,                        STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_VOICE_DEL_IGNORE,                        STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_VOICE_SESSION_ENABLE,                    STATUS_UNHANDLED, PROCESS_THREADUNSAFE, &WorldSession::HandleVoiceSessionEnableOpcode  );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_VOID_STORAGE_QUERY,                      STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleVoidStorageQuery          );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_VOID_STORAGE_TRANSFER,                   STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleVoidStorageTransfer       );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_VOID_STORAGE_UNLOCK,                     STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleVoidStorageUnlock         );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_VOID_SWAP_ITEM,                          STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::HandleVoidSwapItem              );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_WARDEN_DATA,                             STATUS_AUTHED, PROCESS_THREADUNSAFE, &WorldSession::HandleWardenDataOpcode          ); // STATUS_AUTHED
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_WARGAME_ACCEPT,                          STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_WARGAME_START,                           STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_HANDLER(CMSG_WHO,                                                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Who::WhoRequestPkt, &WorldSession::HandleWhoOpcode);//603
    DEFINE_HANDLER(CMSG_WHO_IS,                                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Who::WhoIsRequest, &WorldSession::HandleWhoisOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_WORLD_PORT_RESPONSE,                     STATUS_TRANSFER,  PROCESS_THREADUNSAFE, &WorldSession::HandleMoveWorldportAckOpcode    );//603
    DEFINE_HANDLER(CMSG_UI_TIME_REQUEST,                                    STATUS_LOGGEDIN,  PROCESS_INPLACE,      WorldPackets::Misc::UITimeRequest, &WorldSession::HandleUITimeRequest);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_WORLD_TELEPORT,                          STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleWorldTeleportOpcode       );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_WRAP_ITEM,                               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleWrapItemOpcode            );//603
    //DEFINE_OPCODE_HANDLER_OLD(CMSG_ZONEUPDATE,                              STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   &WorldSession::HandleZoneUpdateOpcode          );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_AUCTION_HELLO_REQUEST,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleAuctionHelloOpcode        );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_HONOR_STATS,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleInspectHonorStatsOpcode   );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_STABLED_PETS,                    STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleListStabledPetsOpcode     );//603
    DEFINE_HANDLER(CMSG_MOVE_FALL_RESET,                                    STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_FALL_LAND,                                     STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_HEARTBEAT,                                     STATUS_LOGGEDIN, PROCESS_THREADSAFE,    WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_JUMP,                                          STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_SET_FACING,                                    STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_SET_PITCH,                                     STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_SET_RUN_MODE,                                  STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603 Trinity has WorldSession::Handle_NULL
    DEFINE_HANDLER(CMSG_MOVE_SET_WALK_MODE,                                 STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603 Trinity has WorldSession::Handle_NULL
    DEFINE_HANDLER(CMSG_MOVE_START_ASCEND,                                  STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_START_BACKWARD,                                STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_START_DESCEND,                                 STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_START_FORWARD,                                 STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_START_PITCH_DOWN,                              STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_START_PITCH_UP,                                STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_START_STRAFE_LEFT,                             STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_START_STRAFE_RIGHT,                            STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_START_SWIM,                                    STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_START_TURN_LEFT,                               STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_START_TURN_RIGHT,                              STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_STOP,                                          STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_STOP_ASCEND,                                   STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_STOP_PITCH,                                    STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_STOP_STRAFE,                                   STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_STOP_SWIM,                                     STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_STOP_TURN,                                     STATUS_LOGGEDIN, PROCESS_THREADSAFE, WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes);//603
    DEFINE_HANDLER(CMSG_MOVE_TELEPORT_ACK,                                  STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   WorldPackets::Movement::MoveTeleportAck, &WorldSession::HandleMoveTeleportAck);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_QUEST_PUSH_RESULT,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestPushResult           );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_UPDATE_RAID_TARGET,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRaidTargetUpdateOpcode    );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_RANDOM_ROLL,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRandomRollOpcode          );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_DUNGEON_DIFFICULTY,                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetDungeonDifficultyOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_TABARDVENDOR_ACTIVATE,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleTabardVendorActivateOpcode);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GET_WEB_TICKET,                          STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLE_PET_NAME_QUERY,                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE,   &WorldSession::HandleBattlePetNameQuery );
    DEFINE_OPCODE_HANDLER_OLD(CMSG_DECLINE_PETITION,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandlePetitionDeclineOpcode     );//603
    DEFINE_HANDLER(CMSG_QUERY_NEXT_MAIL_TIME,                               STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, WorldPackets::Mail::MailQueryNextMailTime, &WorldSession::HandleQueryNextMailTime);//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_RAID_DIFFICULTY,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetRaidDifficultyOpcode   );//603
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLE_PET_MODIFY_NAME,                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlePetModifyName);
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLE_PET_SET_BATTLE_SLOT,              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlePetSetSlot);
    DEFINE_HANDLER(CMSG_BATTLE_PAY_DISTRIBUTION_ASSIGN_TO_TARGET,           STATUS_AUTHED,    PROCESS_INPLACE,      WorldPackets::BattlePay::DistributionAssignToTarget, &WorldSession::HandleBattlePayDistributionAssign);//612
    DEFINE_HANDLER(CMSG_BATTLE_PAY_START_PURCHASE,                          STATUS_AUTHED,    PROCESS_INPLACE,      WorldPackets::BattlePay::StartPurchase, &WorldSession::HandleBattlePayStartPurchase);//612
    DEFINE_HANDLER(CMSG_BATTLE_PAY_CONFIRM_PURCHASE_RESPONSE,               STATUS_AUTHED,    PROCESS_INPLACE,      WorldPackets::BattlePay::ConfirmPurchaseResponse, &WorldSession::HandleBattlePayConfirmPurchase);//612
    DEFINE_OPCODE_HANDLER_OLD(CMSG_GET_UNDELETE_CHARACTER_COOLDOWN_STATUS,  STATUS_AUTHED,    PROCESS_INPLACE,      &WorldSession::HandleUndeleteCharacterCooldownnStatus);//612
    DEFINE_OPCODE_HANDLER_OLD(CMSG_REQUEST_WOW_TOKEN_MARKET_PRICE,          STATUS_AUTHED,    PROCESS_INPLACE,      &WorldSession::HandleWowTokenMarketPrice);//612
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLE_PAY_GET_PURCHASE_LIST,            STATUS_AUTHED,    PROCESS_INPLACE,      &WorldSession::HandleBattlePayPurchaseListGet);//612
    DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLE_PAY_GET_PRODUCT_LIST,             STATUS_AUTHED,    PROCESS_INPLACE,      &WorldSession::HandleBattlePayProductList);//612
    
#undef DEFINE_OPCODE_HANDLER_OLD
#undef DEFINE_HANDLER


#define DEFINE_SERVER_OPCODE_HANDLER(opcode, status) \
    ValidateAndSetServerOpcode(opcode, #opcode, status)

    //------------                      S M S G                  ------------//

    DEFINE_SERVER_OPCODE_HANDLER(SMSG_COMPRESSED_PACKET,                       STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TABARDVENDOR_ACTIVATE,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_RANDOM_ROLL,                             STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ACCOUNT_CRITERIA_UPDATE,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ACCOUNT_DATA_TIMES,                      STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ACCOUNT_MOUNT_UPDATE,                    STATUS_NEVER);//603    
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_ACCOUNT_INFO_RESPONSE,                   STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_ACCOUNT_RESTRICTED_WARNING,              STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ACHIEVEMENT_DELETED,                     STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ACHIEVEMENT_EARNED,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_UPDATE_ACTION_BUTTONS,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ACTIVATE_TAXI_REPLY,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ADDON_INFO,                              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ADD_LOSS_OF_CONTROL,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_REMOVE_LOSS_OF_CONTROL,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ADD_RUNE_POWER,                          STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AE_LOOT_TARGETS,                         STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AE_LOOT_TARGET_ACK,                      STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AI_REACTION,                             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ALL_ACHIEVEMENT_DATA,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ALL_ACCOUNT_CRITERIA,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AREA_SPIRIT_HEALER_TIME,                 STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_AREA_TRIGGER_MESSAGE,                    STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_AREA_TRIGGER_MOVEMENT_UPDATE,            STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AREA_TRIGGER_NO_CORPSE,                  STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_AREA_TRIGGER_RESET,                      STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ARENA_ERROR,                             STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ARENA_OPPONENT_UPDATE,                   STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ARENA_PREP_OPPONENT_SPECIALIZATIONS,     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ATTACKER_STATE_UPDATE,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ATTACK_START,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ATTACK_STOP,                             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ATTACKSWING_ERROR,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUCTION_LIST_BIDDER_ITEMS_RESULT,        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUCTION_OUTBID_NOTIFICATION,             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUCTION_WON_NOTIFICATION,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUCTION_COMMAND_RESULT,                  STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUCTION_LIST_PENDING_SALES_RESULT,       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUCTION_LIST_ITEMS_RESULT,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUCTION_LIST_OWNER_ITEMS_RESULT,         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUCTION_CLOSED_NOTIFICATION,             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUCTION_OWNER_BID_NOTIFICATION,          STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUCTION_REMOVED_NOTIFICATION,            STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_AURA_POINTS_DEPLETED,                    STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AURA_UPDATE,                             STATUS_NEVER); //603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_AURA_UPDATE_ALL,                         STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUTH_CHALLENGE,                          STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUTH_RESPONSE,                           STATUS_NEVER); //603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_AVAILABLE_VOICE_CHANNEL,                 STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_AVERAGE_ITEM_LEVEL_INFORM,               STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BARBER_SHOP_RESULT,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PAY_GET_DISTRIBUTION_LIST_RESPONSE, STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PAY_GET_PURCHASE_LIST_RESPONSE,   STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_BATTLE_FINISHED,              STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_CHAT_RESTRICTED,              STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_DELETED,                      STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_ERROR,                        STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_FINALIZE_LOCATION,            STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_FINAL_ROUND,                  STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_FIRST_ROUND,                  STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_FULL_UPDATE,                  STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_HEALED,                       STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_JOURNAL,                      STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_JOURNAL_LOCK_ACQUIRED,        STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_JOURNAL_LOCK_DENIED,          STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_LICENSE_CHANGED,              STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_MAX_GAME_LENGTH_WARNING,      STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_PVP_CHALLENGE,                STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_QUEUE_PROPOSE_MATCH,          STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_QUEUE_STATUS,                 STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_REPLACEMENTS_MADE,            STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_REQUEST_FAILED,               STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_ROUND_RESULT,                 STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_SLOT_UPDATE,                  STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_TRAP_LEVEL,                   STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PET_UPDATES,                      STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUERY_BATTLE_PET_NAME_RESPONSE,          STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEFIELD_LIST,                        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BF_MGR_EJECTED,                          STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BF_MGR_EJECT_PENDING,                   STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BF_MGR_ENTERING,                         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BF_MGR_ENTRY_INVITE,                     STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEFIELD_MGR_EXIT_REQUEST,            STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BF_MGR_QUEUE_INVITE,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BF_MGR_QUEUE_REQUEST_RESPONSE,           STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BF_MGR_STATE_CHANGED,                     STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEFIELD_PLAYER_POSITIONS,            STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEFIELD_PORT_DENIED,                 STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEFIELD_RATED_INFO,                  STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEGROUND_PLAYER_POSITIONS,           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEFIELD_STATUS_NONE,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEFIELD_STATUS_WAIT_FOR_GROUPS,      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEFIELD_STATUS_QUEUED,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEFIELD_STATUS_NEED_CONFIRMATION,    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEFIELD_STATUS_ACTIVE,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEFIELD_STATUS_FAILED,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEGROUND_INFO_THROTTLED,             STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEGROUND_PLAYER_JOINED,              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEGROUND_PLAYER_LEFT,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BINDER_CONFIRM,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BIND_POINT_UPDATE,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BREAK_TARGET,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BUY_FAILED,                              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BUY_SUCCEEDED,                           STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_ARENA_TEAM,                     STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_CLEAR_PENDING_ACTION,           STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_COMMAND_RESULT,                 STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE,                   STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_ALERT,             STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_NOTES,             STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_NOTES_ALERT,       STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_REMOVED,           STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_REMOVED_ALERT,     STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_STATUS_ALERT,      STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_MODERATOR_STATUS,   STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_REMOVED_ALERT,            STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_INVITE_STATUS,                   STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_EVENT_UPDATED_ALERT,            STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_FILTER_GUILD,                   STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_RAID_LOCKOUT_ADDED,             STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_RAID_LOCKOUT_REMOVED,           STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_RAID_LOCKOUT_UPDATED,           STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_SEND_CALENDAR,                  STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_SEND_EVENT,                     STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CALENDAR_SEND_NUM_PENDING,               STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CAMERA_SHAKE,                            STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CANCEL_AUTO_REPEAT,                      STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CANCEL_COMBAT,                           STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CAST_FAILED,                             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHANNEL_LIST,                            STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHANNEL_MEMBER_COUNT,                    STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHANNEL_NOTIFY,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHANNEL_NOTIFY_JOINED,                   STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHARACTER_LOGIN_FAILED,                  STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CREATE_CHAR,                             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAR_CUSTOMIZE,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAR_CUSTOMIZE_FAILED,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DELETE_CHAR,                             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ENUM_CHARACTERS_RESULT,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAR_FACTION_CHANGE_RESULT,              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHARACTER_RENAME_RESULT,                 STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAT_IGNORED_ACCOUNT_MUTED,              STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAT_NOT_IN_PARTY,                       STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAT_PLAYER_AMBIGUOUS,                   STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAT_PLAYER_NOTFOUND,                    STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAT_RESTRICTED,                         STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAT_SERVER_DISCONNECTED,                STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAT_SERVER_RECONNECTED,                 STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAT_WRONG_FACTION,                      STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CLEAR_BOSS_EMOTES,                       STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CLEAR_COOLDOWN,                          STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CLEAR_COOLDOWNS,                         STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CLEAR_FAR_SIGHT_IMMEDIATE,               STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CLEAR_TARGET,                            STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CACHE_VERSION,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CONTROL_UPDATE,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_VIGNETTE_UPDATE,                         STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_COMBAT_EVENT_FAILED,                     STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_COMMENTATOR_MAP_INFO,                    STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_COMMENTATOR_PARTY_INFO,                  STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_COMMENTATOR_PLAYER_INFO,                 STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_COMMENTATOR_SKIRMISH_QUEUE_RESULT1,      STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_COMMENTATOR_SKIRMISH_QUEUE_RESULT2,      STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_COMMENTATOR_STATE_CHANGED,               STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_COMPLAINT_RESULT,                         STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_COMPRESSED_MOVES,                        STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_COMSAT_CONNECT_FAIL,                     STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_COMSAT_DISCONNECT,                       STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_COMSAT_RECONNECT_TRY,                    STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CONTACT_LIST,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CONVERT_RUNE,                            STATUS_NEVER);//612
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_COOLDOWN_CHEAT,                          STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_COOLDOWN_EVENT,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CORPSE_LOCATION,                         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CORPSE_RECLAIM_DELAY,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CORPSE_TRANSPORT_QUERY,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUERY_CREATURE_RESPONSE,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CRITERIA_DELETED,                        STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CRITERIA_UPDATE,                         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CROSSED_INEBRIATION_THRESHOLD,           STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CURRENCY_LOOT_REMOVED,                   STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CURRENCY_LOOT_RESTORED,                  STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CUSTOM_LOAD_SCREEN,                      STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_DAMAGE_CALC_LOG,                         STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_DANCE_QUERY_RESPONSE,                    STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DB_REPLY,                                STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DEATH_RELEASE_LOC,                       STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_DEBUG_RUNE_REGEN,                        STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DEFENSE_MESSAGE,                         STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DESTRUCTIBLE_BUILDING_DAMAGE,            STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_DIFFERENT_INSTANCE_FROM_PARTY,           STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_DISENCHANT_CREDIT,                       STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DISMOUNT,                                STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_DISMOUNTRESULT,                          STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DISPEL_FAILED,                           STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_DISPLAY_GAME_ERROR,                      STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DISPLAY_PROMOTION,                       STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_DONT_AUTO_PUSH_SPELLS_TO_ACTION_BAR,     STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_DROP_NEW_CONNECTION,                     STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DUEL_COMPLETE,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DUEL_COUNTDOWN,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DUEL_IN_BOUNDS,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DUEL_OUT_OF_BOUNDS,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DUEL_REQUESTED,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DUEL_WINNER,                             STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_DUMP_RIDE_TICKETS_RESPONSE,              STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DURABILITY_DAMAGE_DEATH,                 STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_ECHO_PARTY_SQUELCH,                      STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_EMOTE,                                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ENABLE_BARBER_SHOP,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ENCHANTMENT_LOG,                         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ENVIRONMENTAL_DAMAGE_LOG,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOAD_EQUIPMENT_SET,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_EQUIPMENT_SET_ID,                        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_USE_EQUIPMENT_SET_RESULT,                STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_EXPECTED_SPAM_RECORDS,                   STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_EXPLORATION_EXPERIENCE,                  STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_FAILED_PLAYER_CONDITION,                 STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_FEATURE_SYSTEM_STATUS,                   STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_FEATURE_SYSTEM_STATUS_GLUE_SCREEN,       STATUS_NEVER);//612
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_FEIGN_DEATH_RESISTED,                    STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_FISH_ESCAPED,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_FISH_NOT_HOOKED,                         STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_FLIGHT_SPLINE_SYNC,                      STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_FLOOD_DETECTED,                          STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_FORCED_DEATH_UPDATE,                     STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_FORCE_SEND_QUEUED_PACKETS,               STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_VEHICLE_REC_ID,                 STATUS_NEVER);//612
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_FORGE_MASTER_SET,                        STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_FRIEND_STATUS,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GAME_OBJECT_CUSTOM_ANIM,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GAME_OBJECT_DESPAWN,                     STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PAGE_TEXT,                               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUERY_GAME_OBJECT_RESPONSE,              STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GAMEOBJECT_RESET_STATE,                  STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GAMESPEED_SET,                           STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GAMETIME_SET,                            STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GAMETIME_UPDATE,                         STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GAME_EVENT_DEBUG_LOG,                    STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GAME_OBJECT_ACTIVATE_ANIM_KIT,           STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GM_TICKET_RESPONSE_ERROR,                STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GM_TICKET_RESPONSE,                      STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GM_TICKET_STATUS_UPDATE,                 STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GMTICKET_CREATE,                         STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GMTICKET_DELETETICKET,                   STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GM_TICKET_GET_TICKET_RESPONSE,           STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GM_TICKET_SYSTEM_STATUS,                 STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GM_TICKET_UPDATE,                        STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GM_MESSAGECHAT,                          STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GM_PLAYER_INFO,                          STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GM_TICKET_STATUS_UPDATE,                 STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GODMODE,                                 STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GOSSIP_COMPLETE,                         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GOSSIP_MESSAGE,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GOSSIP_POI,                              STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GROUPACTION_THROTTLED,                   STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GROUP_CANCEL,                            STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GROUP_DECLINE,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GROUP_DESTROYED,                         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PARTY_INVITE,                            STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GROUP_JOINED_BATTLEGROUND,               STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PARTY_UPDATE,                            STATUS_LOGGEDIN);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GROUP_NEW_LEADER,                        STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GROUP_SET_ROLE,                          STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GROUP_UNINVITE,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ALL_GUILD_ACHIEVEMENTS,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_ACHIEVEMENT_DELETED,               STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_ACHIEVEMENT_EARNED,                STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_CRITERIA_UPDATE,                   STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_ACHIEVEMENT_MEMBERS,               STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_BANK_QUERY_RESULTS,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_BANK_LOG_QUERY_RESULTS,            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_BANK_REMAINING_WITHDRAW_MONEY,     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_BANK_MONEY_CHANGED,          STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_BANK_TEXT_QUERY_RESULT,            STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_CHALLENGE_COMPLETED,               STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_CHALLENGE_UPDATE,                 STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_CHANGE_NAME_RESULT,                STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_COMMAND_RESULT,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_CRITERIA_DELETED,                  STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_INVITE_DECLINED,                   STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT,                             STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_DISBANDED,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_TAB_MODIFIED,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_TAB_TEXT_CHANGED,            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_BANK_CONTENTS_CHANGED,       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_PLAYER_JOINED,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_NEW_LEADER,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_MOTD,                        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_PRESENCE_CHANGE,             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_RANKS_UPDATED,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_RANK_CHANGED,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_PLAYER_LEFT,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_TAB_ADDED,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_TAB_REMOVED,                 STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_EVENT_LOG_QUERY_RESULTS,           STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_FLAGGED_FOR_RENAME,                STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_INVITE,                            STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_INVITE_CANCEL,                     STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_MAX_DAILY_XP,                      STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_MEMBERS_WITH_RECIPE,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_MEMBER_DAILY_RESET,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_MEMBER_RECIPES,                    STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_MEMBER_UPDATE_NOTE,                STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_MOVE_COMPLETE,                     STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_MOVE_STARTING,                     STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_NEWS_DELETED,                      STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_NEWS,                              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_PARTY_STATE,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_PERMISSIONS_QUERY_RESULTS,         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUERY_GUILD_INFO_RESPONSE,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_RANKS,                             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_SEND_RANK_CHANGE,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_KNOWN_RECIPES,                     STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_RENAMED,                           STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_REPUTATION_REACTION_CHANGED,       STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_RESET,                             STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_REWARD_LIST,                       STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_ROSTER,                            STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_UPDATE_ROSTER,                     STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_XP,                                STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_XP_GAIN,                           STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_HEALTH_UPDATE,                           STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_HIGHEST_THREAT_UPDATE,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_HOTFIX_NOTIFY_BLOB,                      STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_HOTFIX_NOTIFY,                           STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_INITIALIZE_FACTIONS,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SETUP_CURRENCY,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_INIT_WORLD_STATES,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_INITIAL_COOLDOWNS,                       STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_INSPECT_HONOR_STATS,                     STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_INSPECT_RESULTS_UPDATE,                  STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_INSPECT_RESULT,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PENDING_RAID_LOCK,                       STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_INSTANCE_RESET,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_INSTANCE_RESET_FAILED,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_INSTANCE_SAVE_CREATED,                   STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_INVALIDATE_DANCE,                        STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_INVALIDATE_PLAYER,                       STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_INVALID_PROMOTION_CODE,                  STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_INVENTORY_CHANGE_FAILURE,                STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_ITEM_ADD_PASSIVE,                        STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ITEM_COOLDOWN,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ITEM_ENCHANT_TIME_UPDATE,                STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_ITEM_EXPIRE_PURCHASE_REFUND,             STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ITEM_PUSH_RESULT,                        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_ITEM_PURCHASE_DATA,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ITEM_PURCHASE_REFUND_RESULT,             STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_ITEM_REMOVE_PASSIVE,                     STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_ITEM_SEND_PASSIVE,                       STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUERY_ITEM_TEXT_RESPONSE,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ITEM_TIME_UPDATE,                        STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_LEARNED_DANCE_MOVES,                     STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LEARNED_SPELLS,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LEVEL_UP_INFO,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_STABLE_LIST,                         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_BOOT_PLAYER,                         STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_DISABLED,                            STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_JOIN_RESULT,                         STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_OFFER_CONTINUE,                      STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_PARTY_INFO,                          STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_PLAYER_INFO,                         STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_PLAYER_REWARD,                       STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_PROPOSAL_UPDATE,                     STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_QUEUE_STATUS,                        STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_ROLE_CHECK_UPDATE,                   STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ROLE_CHOSEN,                             STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_SLOT_INVALID,                        STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_TELEPORT_DENIED,                     STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_UPDATE_SEARCH,                       STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_UPDATE_STATUS,                       STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_LFG_UPDATE_STATUS_NONE,                  STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_LF_GUILD_APPLICANT_LIST_UPDATED,         STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_LF_GUILD_APPLICATIONS_LIST_CHANGED,      STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_LF_GUILD_BROWSE,                 STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_LF_GUILD_COMMAND_RESULT,                 STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_LF_GUILD_APPLICATIONS,        STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_LF_GUILD_POST,                   STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_LF_GUILD_RECRUITS,           STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_VENDOR_INVENTORY,                        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOAD_CUF_PROFILES,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOGIN_SET_TIME_SPEED,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOGIN_VERIFY_WORLD,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOGOUT_CANCEL_ACK,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOGOUT_COMPLETE,                         STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOGOUT_RESPONSE,                         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOG_XP_GAIN,                             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOOT_ALL_PASSED,                         STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOOT_CONTENTS,                           STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MASTER_LOOT_CANDIDATE_LIST,              STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOOT_LIST,                               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOOT_MONEY_NOTIFY,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOOT_RELEASE,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOOT_RELEASE,                            STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOOT_REMOVED,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOOT_RESPONSE,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOOT_ROLL,                               STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOOT_ROLLS_COMPLETE,                     STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_LOOT_ROLL_WON,                           STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_START_LOOT_ROLL,                         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_COIN_REMOVED,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MAIL_LIST_RESULT,                        STATUS_NEVER);//612
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_MAP_OBJ_EVENTS,                          STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_MEETINGSTONE_COMPLETE,                   STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_MEETINGSTONE_IN_PROGRESS,                STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAT,                                    STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SAVE_GUILD_EMBLEM,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_INITIAL_SETUP,                           STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SEND_KNOWN_SPELLS,                       STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_MESSAGE_BOX,                             STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MINIMAP_PING,                            STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_MINIGAME_SETUP,                          STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_MINIGAME_STATE,                          STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MIRROR_IMAGE_COMPONENTED_DATA,           STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MISSILE_CANCEL,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MODIFY_COOLDOWN,                         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ON_MONSTER_MOVE,                         STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_MONSTER_MOVE_TRANSPORT,                  STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOTD,                                    STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOUNTRESULT,                             STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPECIAL_MOUNT_ANIM,                      STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_COLLISION_DISABLE,                  STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_COLLISION_ENABLE,                   STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_FEATHER_FALL,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_DISABLE_GRAVITY,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_ENABLE_GRAVITY,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_KNOCK_BACK,                         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_LAND_WALK,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_NORMAL_FALL,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_ROOT,                               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_ACTIVE_MOVER,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_CAN_FLY,                        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_ENABLE_TRANSITION_BETWEEN_SWIM_AND_FLY, STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_COLLISION_HEIGHT,               STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_COMPOUND_STATE,                 STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_FLIGHT_BACK_SPEED,              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_FLIGHT_SPEED,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_HOVERING,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_PITCH_RATE,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_RUN_BACK_SPEED,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_RUN_SPEED,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_SWIM_BACK_SPEED,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_SWIM_SPEED,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_TURN_RATE,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_WALK_SPEED,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UNROOT,                             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UNSET_CAN_FLY,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_DISABLE_TRANSITION_BETWEEN_SWIM_AND_FLY, STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UNSET_HOVERING,                     STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UPDATE_COLLISION_HEIGHT,            STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UPDATE_FLIGHT_BACK_SPEED,           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UPDATE_FLIGHT_SPEED,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UPDATE_KNOCK_BACK,                  STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UPDATE_PITCH_RATE,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UPDATE_RUN_BACK_SPEED,              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UPDATE_SWIM_BACK_SPEED,             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UPDATE_SWIM_SPEED,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UPDATE_TURN_RATE,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UPDATE_WALK_SPEED,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UPDATE_RUN_SPEED,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SET_WATER_WALK,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UPDATE_TELEPORT,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MULTIPLE_PACKETS,                        STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUERY_PLAYER_NAME_RESPONSE,              STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_NEW_TAXI_PATH,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_NEW_WORLD,                               STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_NEW_WORLD_ABORT,                         STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PRINT_NOTIFICATION,                      STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_NOTIFY_DANCE,                            STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_NOTIFY_DEST_LOC_SPELL_CAST,              STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUERY_NPC_TEXT_RESPONSE,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_OFFER_PETITION_ERROR,                    STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ON_CANCEL_EXPECTED_RIDE_VEHICLE_AURA,    STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_OPEN_CONTAINER,                          STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_OPEN_LFG_DUNGEON_FINDER,                 STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_OVERRIDE_LIGHT,                          STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUERY_PAGE_TEXT_RESPONSE,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PARTY_KILL_LOG,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PARTY_COMMAND_RESULT,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PARTY_MEMBER_STATE,                      STATUS_NEVER);//612
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PAUSE_MIRROR_TIMER,                      STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_PERIODIC_AURA_LOG,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CONQUEST_FORMULA_CONSTANTS,              STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PETGODMODE,                              STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PETITION_ALREADY_SIGNED,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUERY_PETITION_RESPONSE,                 STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PETITION_SHOW_LIST,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PETITION_SHOW_SIGNATURES,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PETITION_SIGN_RESULTS,                   STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PETITION_DECLINED,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PETITION_RENAME_GUILD_RESPONSE,          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_ACTION_FEEDBACK,                     STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_ACTION_SOUND,                        STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_ADDED,                               STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_BROKEN,                              STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_CAST_FAILED,                         STATUS_NEVER);//612
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_DISMISS_SOUND,                       STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_GUIDS,                               STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_MODE,                                STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_NAME_INVALID,                        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUERY_PET_NAME_RESPONSE,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_UNLEARNED_SPELLS,                    STATUS_NEVER);//612
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_RENAMEABLE,                          STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_SLOT_UPDATED,                        STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_LEARNED_SPELLS,                      STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_TAME_FAILURE,                        STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_UPDATE_COMBO_POINTS,                 STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAYED_TIME,                             STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAYERBINDERROR,                         STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAYER_BOUND,                            STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_UPDATE,                             STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAYER_SKINNED,                          STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_VEHICLE_REC_ID,                      STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAY_DANCE,                              STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAY_MUSIC,                              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAY_OBJECT_SOUND,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAY_ONE_SHOT_ANIM_KIT,                  STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAY_SCENE,                              STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CANCEL_SCENE,                            STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAY_SOUND,                              STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAY_SPELL_VISUAL,                       STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAY_SPELL_VISUAL_KIT,                   STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAY_TIME_WARNING,                       STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PLAYER_RECIPES,                          STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PONG,                                    STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_POWER_UPDATE,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PRE_RESSURECT,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PROC_RESIST,                             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PROPOSE_LEVEL_GRANT,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PURCHASE_STORE_PRODUCTS_UPDATED,         STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PURCHASE_STORE_PURCHASE_LIST_UPDATED,    STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PVP_CREDIT,                              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PVP_SEASON,                              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_INSPECT_PVP,                             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PVP_LOG_DATA,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PVP_OPTIONS_ENABLED,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUERY_TIME_RESPONSE,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_GIVER_OFFER_REWARD_MESSAGE,        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_GIVER_QUEST_COMPLETE,              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_GIVER_QUEST_DETAILS,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_GIVER_QUEST_FAILED,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_GIVER_QUEST_LIST_MESSAGE,          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_GIVER_REQUEST_ITEMS,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_GIVER_STATUS,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_GIVER_STATUS_MULTIPLE,             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_LOG_FULL,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_UPDATE_ADD_CREDIT,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_UPDATE_ADD_PVP_CREDIT,             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_UPDATE_COMPLETE,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_UPDATE_FAILED,                     STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_UPDATE_FAILED_TIMER,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_CONFIRM_ACCEPT,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_FORCE_REMOVED,                     STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_GIVER_INVALID_QUEST,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_COMPLETION_NPC_RESPONSE,           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_POI_QUERY_RESPONSE,                STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUEST_PUSH_RESULT,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_QUERY_QUEST_INFO_RESPONSE,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_RAID_GROUP_ONLY,                         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_INSTANCE_INFO,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_RAID_INSTANCE_MESSAGE,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_RAID_MARKERS_CHANGED,                    STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_READY_CHECK_RESPONSE,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_READY_CHECK_STARTED,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_READY_CHECK_COMPLETED,                   STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_RAID_READY_CHECK_THROTTLED_ERROR,        STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_RAID_SUMMON_FAILED,                      STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_GENERATE_RANDOM_CHARACTER_NAME_RESULT,   STATUS_AUTHED);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLEFIELD_RATED_INFO,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_READ_ITEM_RESULT_FAILED,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_READ_ITEM_RESULT_OK,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_REALM_QUERY_RESPONSE,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_REALM_SPLIT,                             STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_REAL_GROUP_UPDATE,                       STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_NOTIFY_RECEIVED_MAIL,                    STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_CONNECT_TO,                              STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_REFER_A_FRIEND_EXPIRED,                  STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_REFER_A_FRIEND_FAILURE,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_UNLEARNED_SPELLS,                        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_REPORT_PVP_PLAYER_AFK_RESULT,            STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_REQUEST_CEMETERY_LIST_RESPONSE,          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_REQUEST_PVP_REWARDS_RESPONSE,            STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_RESEARCH_COMPLETE,                       STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SETUP_RESEARCH_HISTORY,                  STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_RESET_COMPRESSION_CONTEXT,               STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_RESET_FAILED_NOTIFY,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_RESPEC_WIPE_CONFIRM,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_RESPOND_INSPECT_ACHIEVEMENTS,            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_RESURRECT_REQUEST,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_RESYNC_RUNES,                            STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ROLE_POLL_INFORM,                        STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_RWHOIS,                                  STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SELL_RESPONSE,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MAIL_COMMAND_RESULT,                        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SEND_SPELL_CHARGES,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SEND_SPELL_HISTORY,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SEND_UNLEARN_SPELLS,                     STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SERVERTIME,                              STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SERVER_FIRST_ACHIEVEMENT,                STATUS_NEVER);//612
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SERVER_INFO_RESPONSE,                    STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAT_SERVER_MESSAGE,                     STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SERVER_SCENE_PLAYBACK,                   STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SERVER_PERF,                             STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_DF_FAST_LAUNCH_RESULT,               STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_DUNGEON_DIFFICULTY,                  STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_FACTION_ATWAR,                       STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_FACTION_STANDING,                    STATUS_NEVER);//6.1.2
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_FACTION_VISIBLE,                     STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_FLAT_SPELL_MODIFIER,                 STATUS_NEVER);//6.0.3
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_FORCED_REACTIONS,                    STATUS_NEVER);//6.0.3
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_PCT_SPELL_MODIFIER,                  STATUS_NEVER);//6.0.3
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_PET_SPECIALIZATION,                  STATUS_NEVER);//6.0.3
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_SPELLS_MESSAGE,                      STATUS_NEVER);//6.1.2
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PHASE_SHIFT_CHANGE,                      STATUS_NEVER);//6.0.3
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT,        STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_PLAY_HOVER_ANIM,                     STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_PROFICIENCY,                         STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_NOTIFY_MISSILE_TRAJECTORY_COLLISION,                 STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_TIME_ZONE_INFORMATION,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SHOW_TAXI_NODES,                         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SHOW_BANK,                               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SHOW_NEUTRAL_PLAYER_FACTION_SELECT_UI,   STATUS_NEVER);//612
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SHOW_RATINGS,                            STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SOR_START_EXPERIENCE_INCOMPLETE,         STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELLBREAKLOG,                           STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_DAMAGE_SHIELD,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_DISPELL_LOG,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_ENERGIZE_LOG,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_HEAL_LOG,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_INSTAKILL_LOG,                     STATUS_NEVER);//612
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELLINTERRUPTLOG,                       STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_EXECUTE_LOG,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_MISS_LOG,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_NON_MELEE_DAMAGE_LOG,              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_OR_DAMAGE_IMMUNE,                  STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELLSTEALLOG,                           STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CATEGORY_COOLDOWN,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SEND_SPELL_CHARGES,                      STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_COOLDOWN,                          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_CREATE_VISUAL,                     STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_DELAYED,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_FAILED_OTHER,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_FAILURE,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_GO,                                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_START,                             STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_UPDATE_CHAIN_TARGETS,              STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPIRIT_HEALER_CONFIRM,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_DISABLE_GRAVITY,             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_ENABLE_GRAVITY,              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_ROOT,                        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_FEATHER_FALL,            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_FLIGHT_BACK_SPEED,       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_FLIGHT_SPEED,            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_FLYING,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_HOVER,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_LAND_WALK,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_NORMAL_FALL,             STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_PITCH_RATE,              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_RUN_BACK_SPEED,          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_RUN_MODE,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_RUN_SPEED,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_SWIM_BACK_SPEED,         STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_SWIM_SPEED,              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_TURN_RATE,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_WALK_MODE,               STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_WALK_SPEED,              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_SET_WATER_WALK,              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_START_SWIM,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_STOP_SWIM,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_UNROOT,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_UNSET_FLYING,                STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_SPLINE_UNSET_HOVER,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_PET_STABLE_RESULT,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_STAND_STATE_UPDATE,                      STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_START_MIRROR_TIMER,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_START_TIMER,                             STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_STOP_DANCE,                              STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_STOP_MIRROR_TIMER,                       STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_STREAMING_MOVIE,                         STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SUMMON_CANCEL,                           STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SUMMON_REQUEST,                          STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SUPERCEDED_SPELLS,                        STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SUPPRESS_NPC_GREETINGS,                  STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SURVEY_CAST,                             STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_SUSPEND_COMMS,                           STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_RESUME_TOKEN,                            STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_TALENTS_INVOLUNTARILY_RESET,             STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TAXI_NODE_STATUS,                        STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_TEST_DROP_RATE_RESULT,                   STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TEXT_EMOTE,                              STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_THREAT_CLEAR,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_THREAT_REMOVE,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_THREAT_UPDATE,                           STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_TIME_ADJUSTMENT,                         STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TIME_SYNC_REQUEST,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TITLE_EARNED,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TITLE_LOST,                              STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_TOGGLE_XP_GAIN,                          STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TOTEM_CREATED,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TRADE_STATUS,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TRADE_UPDATED,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TRAINER_LIST,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TRAINER_BUY_FAILED,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TRANSFER_ABORTED,                        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TRANSFER_PENDING,                        STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TRIGGER_CINEMATIC,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TRIGGER_MOVIE,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TURN_IN_PETITION_RESULT,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_TUTORIAL_FLAGS,                          STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_UNIT_HEALTH_FREQUENT,                    STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_UNIT_SPELLCAST_START,                    STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_UPDATE_ACCOUNT_DATA,                     STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_UPDATE_ACCOUNT_DATA_COMPLETE,            STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_CURRENCY,                            STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_MAX_WEEKLY_QUANTITY,                 STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_UPDATE_DUNGEON_ENCOUNTER_FOR_LOOT,       STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_INSTANCE_ENCOUNTER_ENGAGE_UNIT,          STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_UPDATE_INSTANCE_OWNERSHIP,               STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_UPDATE_ITEM_ENCHANTMENTS,                STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_UPDATE_LAST_INSTANCE,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_UPDATE_OBJECT,                           STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_UPDATE_SERVER_PLAYER_POSITION,           STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_UPDATE_TALENT_DATA,                      STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_UPDATE_WORLD_STATE,                      STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_USERLIST_ADD,                            STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_USERLIST_REMOVE,                         STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_USERLIST_UPDATE,                         STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_VOICESESSION_FULL,                       STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_VOICE_CHAT_STATUS,                       STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_VOICE_PARENTAL_CONTROLS,                 STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_VOICE_SESSION_LEAVE,                     STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_VOICE_SESSION_ROSTER_UPDATE,             STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_VOICE_SET_TALKER_MUTED,                  STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_VOID_ITEM_SWAP_RESPONSE,                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_VOID_STORAGE_CONTENTS,                   STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_VOID_STORAGE_FAILED,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_VOID_STORAGE_TRANSFER_CHANGES,           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_VOID_TRANSFER_RESULT,                    STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_WAIT_QUEUE_FINISH,                       STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_WAIT_QUEUE_UPDATE,                       STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_WARDEN_DATA,                             STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_WARGAME_CHECK_ENTRY,                     STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_WARGAME_REQUEST_SENT,                    STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_WEATHER,                                 STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_WEEKLY_LAST_RESET,                       STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_RESET_WEEKLY_CURRENCY,                   STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_WEEKLY_SPELL_USAGE,                      STATUS_UNHANDLED);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_WEEKLY_SPELL_USAGE_UPDATE,               STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_WHO,                                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_WHO_IS,                                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_WORLD_SERVER_INFO,                       STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_START_ELAPSED_TIMERS,                    STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_WORLD_STATE_TIMER_START,                 STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_WORLD_STATE_TIMER_STOP,                  STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_UI_TIME,                                 STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DISPLAY_TOAST,                           STATUS_NEVER);
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_XP_GAIN_ABORTED,                         STATUS_UNHANDLED);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ZONE_UNDER_ATTACK,                       STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_MOVEFLAG2_0x1000,                    STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_UNSET_MOVEFLAG2_0x1000,                  STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_0x00E9,                            STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SCRIPT_CAST,                             STATUS_NEVER);//603

    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUCTION_HELLO_RESPONSE,                  STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHALLENGE_UNK,                           STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHALLENGE_MODE_COMPLETION_INFO,          STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHALLENGE_MODE_REWARD_INFO,              STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHALLENGE_MODE_REQUEST_LEADERS_RESULT,   STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_CHANNEL_START,                     STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SPELL_CHANNEL_UPDATE,                    STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MOVE_TELEPORT,                           STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SEND_RAID_TARGET_UPDATE_SINGLE,          STATUS_NEVER);//603
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_RAID_TARGET_UPDATE_ALL,                  STATUS_NEVER);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_0x1965,                                  STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PAY_DISTRIBUTION_UPDATE,          STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_DANCE_STUDIO_CREATE_RESULT,              STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_UNDELETE_COOLDOWN_STATUS_RESPONSE,       STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PAY_GET_PRODUCT_LIST_RESPONSE,    STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_REQUEST_WOW_TOKEN_MARKET_PRICE_RESPONSE, STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PAY_START_PURCHASE_RESPONSE,      STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PAY_PURCHASE_UPDATE,              STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_BATTLE_PAY_CONFIRM_PURCHASE,             STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHARACTER_UPGRADE_STARTED,               STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHARACTER_UPGRADE_CHARACTER_CHOSEN,      STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_UNDELETE_CHARACTER_RESPONSE,             STATUS_NEVER);//612
    
    //------------M S G                   ------------//
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_MAIL_QUERY_NEXT_TIME_RESULT,             STATUS_NEVER);//612
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_SET_RAID_DIFFICULTY,                     STATUS_NEVER);//603
    //DEFINE_SERVER_OPCODE_HANDLER(SMSG_PETITION_DECLINE,                        STATUS_NEVER);

    //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_CHARM_TELEPORT_CHEAT,                STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_RUN_MODE,                        STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_WALK_MODE,                       STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_TELEPORT_CHEAT,                      STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(MSG_INSPECT_ARENA_TEAMS,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleInspectArenaTeamsOpcode   );
    //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_TIME_SKIPPED,                        STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_TOGGLE_COLLISION_CHEAT,              STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_UPDATE_FLIGHT_SPEED,                 STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_UPDATE_RUN_SPEED,                    STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(MSG_NOTIFY_PARTY_SQUELCH,                     STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(MSG_BATTLEGROUND_PLAYER_POSITIONS,            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlegroundPlayerPositionsOpcode);
    //DEFINE_OPCODE_HANDLER_OLD(MSG_DELAY_GHOST_TELEPORT,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(MSG_DEV_SHOWLABEL,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(MSG_GM_ACCOUNT_ONLINE,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
    //DEFINE_OPCODE_HANDLER_OLD(MSG_GM_BIND_OTHER,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_GM_CHANGE_ARENA_RATING,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_GM_DESTROY_CORPSE,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_GM_GEARRATING,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_GM_RESETINSTANCELIMIT,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_GM_SHOWLABEL,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_GM_SUMMON,                                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_FEATHER_FALL,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_GRAVITY_CHNG,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_HOVER,                               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_ROOT,                                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_ALL_SPEED_CHEAT,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_COLLISION_HEIGHT,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_FLIGHT_BACK_SPEED_CHEAT,         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_FLIGHT_SPEED_CHEAT,              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_PITCH_RATE_CHEAT,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_RAW_POSITION_ACK,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_RUN_SPEED,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_RUN_SPEED_CHEAT,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_SWIM_BACK_SPEED_CHEAT,           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_SWIM_SPEED_CHEAT,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_TURN_RATE_CHEAT,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_SET_WALK_SPEED_CHEAT,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_START_SWIM_CHEAT,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_STOP_SWIM_CHEAT,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_TOGGLE_FALL_LOGGING,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_TOGGLE_LOGGING,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_UPDATE_CAN_FLY,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_UPDATE_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY, STATUS_NEVER, PROCESS_INPLACE,  &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_UPDATE_MOUSE,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_MOVE_WATER_WALK,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_NULL_ACTION,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(MSG_VIEW_PHASE_SHIFT,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );

        //------------         NOT FINED                             ------------//

      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ACTIVE_PVP_CHEAT,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ADD_PVP_MEDAL_CHEAT,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ADVANCE_SPAWN_TIME,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_AFK_MONITOR_INFO_CLEAR,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_AFK_MONITOR_INFO_REQUEST,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ARENA_TEAM_ACCEPT,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleArenaTeamAcceptOpcode     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ARENA_TEAM_CREATE,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleArenaTeamCreateOpcode     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ARENA_TEAM_DECLINE,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleArenaTeamDeclineOpcode    );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ARENA_TEAM_DISBAND,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleArenaTeamDisbandOpcode    );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ARENA_TEAM_INVITE,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleArenaTeamInviteOpcode     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ARENA_TEAM_LEADER,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleArenaTeamLeaderOpcode     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ARENA_TEAM_LEAVE,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleArenaTeamLeaveOpcode      );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ARENA_TEAM_QUERY,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleArenaTeamQueryOpcode      );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ARENA_TEAM_REMOVE,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleArenaTeamRemoveOpcode     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ARENA_TEAM_ROSTER,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleArenaTeamRosterOpcode     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_AUTH_SRP6_BEGIN,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_AUTH_SRP6_PROOF,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_AUTH_SRP6_RECODE,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_AUTOEQUIP_GROUND_ITEM,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_AUTOSTORE_GROUND_ITEM,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLEFIELD_JOIN,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLEGROUND_PLAYER_POSITIONS,           STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLEFIELD_MANAGER_ADVANCE_STATE,       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLEFIELD_MANAGER_SET_NEXT_TRANSITION_TIME, STATUS_NEVER,PROCESS_INPLACE,     &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_BATTLEMASTER_HELLO,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBattlemasterHelloOpcode   );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_BEASTMASTER,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_BOOTME,                                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_BOT_DETECTED,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_BOT_DETECTED2,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_BUY_ITEM_IN_SLOT,                        STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBuyItemInSlotOpcode       );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_BUY_LOTTERY_TICKET_OBSOLETE,             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_BUY_STABLE_SLOT,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleBuyStableSlot             );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_CONTEXT_EVENT_SIGNUP,           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CALENDAR_EVENT_INVITE_NOTES,             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CANCEL_GROWTH_AURA,                      STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleCancelGrowthAuraOpcode    );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHANGE_GDF_ARENA_RATING,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHANGE_PERSONAL_ARENA_RATING,            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHARACTER_POINT_CHEAT,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHEAT_DUMP_ITEMS_DEBUG_ONLY,             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHEAT_PLAYER_LOGIN,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHEAT_PLAYER_LOOKUP,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHEAT_SETMONEY,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHEAT_SET_ARENA_CURRENCY,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHEAT_SET_HONOR_CURRENCY,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CHECK_LOGIN_CRITERIA,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CLEAR_EXPLORATION,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CLEAR_HOLIDAY_BG_WIN_TIME,               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CLEAR_QUEST,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CLEAR_RANDOM_BG_WIN_TIME,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CLEAR_SERVER_BUCK_DATA,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_COMPLETE_ACHIEVEMENT_CHEAT,              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CONTROLLER_EJECT_PASSENGER,              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleEjectPassenger            );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_COOLDOWN_CHEAT,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CREATEGAMEOBJECT,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CREATEITEM,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_CREATEMONSTER,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DBLOOKUP,                                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DEBUG_ACTIONS_START,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DEBUG_ACTIONS_STOP,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DEBUG_AISTATE,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DEBUG_CHANGECELLZONE,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DEBUG_LIST_TARGETS,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DEBUG_PASSIVE_AURA,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DEBUG_SERVER_GEO,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DECHARGE,                                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DELETE_DANCE,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DEL_PVP_MEDAL_CHEAT,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DESTROYMONSTER,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DESTROY_ITEMS,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DISABLE_PVP_CHEAT,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DROP_NEW_CONNECTION,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_DUMP_OBJECTS,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ENABLE_DAMAGE_LOG,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_END_BATTLEFIELD_CHEAT,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_EXPIRE_RAID_INSTANCE,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_FLAG_QUEST,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_FLAG_QUEST_FINISH,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_FLOOD_GRACE_CHEAT,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_FORCEACTION,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_FORCEACTIONONOTHER,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_FORCEACTIONSHOW,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_FORCE_ANIM,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_FORCE_SAY_CHEAT,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GAMESPEED_SET,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GAMETIME_SET,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GETDEATHBINDZONE,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GET_CHANNEL_MEMBER_COUNT,                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGetChannelMemberCount     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GHOST,                                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GMRESPONSE_CREATE_TICKET,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_CHARACTER_RESTORE,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_CHARACTER_SAVE,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_CREATE_ITEM_TARGET,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_DESTROY_ONLINE_CORPSE,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_FREEZE,                               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_GRANT_ACHIEVEMENT,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_INVIS,                                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_MOVECORPSE,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_NUKE,                                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_NUKE_ACCOUNT,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_NUKE_CHARACTER,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_REMOVE_ACHIEVEMENT,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_REQUEST_PLAYER_INFO,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_RESURRECT,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_REVEALTO,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_SET_CRITERIA_FOR_PLAYER,              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_SET_SECURITY_GROUP,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_SHOW_COMPLAINTS,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_SILENCE,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_SUMMONMOB,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_TEACH,                                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_UBERINVIS,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_UNSQUELCH,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_UNTEACH,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_UPDATE_TICKET_STATUS,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_VISION,                               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GM_WHISPER,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GODMODE,                                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GROUP_CANCEL,                            STATUS_LOGGEDIN,  PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_BANK_NOTE,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_CREATE,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildCreateOpcode         );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_GUILD_INFO,                              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleGuildInfoOpcode           );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_IGNORE_DIMINISHING_RETURNS_CHEAT,        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_IGNORE_KNOCKBACK_CHEAT,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_IGNORE_REQUIREMENTS_CHEAT,               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_LEARN_DANCE_MOVE,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_LEARN_SPELL,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_LEVEL_CHEAT,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_LFD_PARTY_LOCK_INFO_REQUEST,             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleLfgPartyLockInfoRequestOpcode);
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_LFG_GET_PLAYER_INFO,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_LFG_PARTY_LOCK_INFO_REQUEST,             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_LFG_SET_NEEDS,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_LF_GUILD_JOIN,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_LOAD_DANCES,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_LOTTERY_QUERY_OBSOLETE,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_LUA_USAGE,                               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MAELSTROM_GM_SENT_MAIL,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MAELSTROM_INVALIDATE_CACHE,              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MAELSTROM_RENAME_GUILD,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MAKEMONSTERATTACKGUID,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MEETINGSTONE_INFO,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_CHARACTER_CHEAT,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_CHARM_PORT_CHEAT,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_CHARM_TELEPORT_CHEAT,               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_ENABLE_SWIM_TO_FLY_TRANS_ACK,       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_SET_FLY,                            STATUS_LOGGEDIN,  PROCESS_THREADSAFE,   WorldPackets::Movement::ClientPlayerMovement, &WorldSession::HandleMovementOpcodes           );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_SET_RELATIVE_POSITION,              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_SET_RUN_SPEED,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_SET_VEHICLE_REC_ID_ACK,             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_START_SWIM_CHEAT,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_STOP_SWIM_CHEAT,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_MOVE_TOGGLE_COLLISION_ACK,               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_NEW_SPELL_SLOT,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_NO_SPELL_VARIANCE,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_PERFORM_ACTION_SET,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_PETGODMODE,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_PET_LEVEL_CHEAT,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_PET_NAME_CACHE,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_PET_UNLEARN,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_PET_UNLEARN_TALENTS,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_PLAYER_AI_CHEAT,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_PLAYER_DIFFICULTY_CHANGE,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_PROFILEDATA_REQUEST,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_PVP_QUEUE_STATS_REQUEST,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_GUILD_MAX_XP,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_OBJECT_POSITION,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_OBJECT_ROTATION,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_SERVER_BUCK_DATA,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_QUERY_VEHICLE_STATUS,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_QUEST_GIVER_CANCEL,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverCancel          );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_QUEST_GIVER_QUEST_AUTOLAUNCH,             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestgiverQuestAutoLaunch );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_QUESTLOG_SWAP_QUEST,                     STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleQuestLogSwapQuest         );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_RECHARGE,                                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_REDIRECTION_FAILED,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_REFER_A_FRIEND,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_REMOVE_GLYPH,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleRemoveGlyph               );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_REPLACE_ACCOUNT_DATA,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_RUN_SCRIPT,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SAVE_DANCE,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SAVE_PLAYER,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SEND_COMBAT_TRIGGER,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SEND_EVENT,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SEND_GENERAL_TRIGGER,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SEND_LOCAL_EVENT,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SERVERINFO,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SERVERTIME,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SERVER_BROADCAST,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SERVER_COMMAND,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SERVER_INFO_QUERY,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SETDEATHBINDPOINT,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_ACTIVE_TALENT_GROUP_OBSOLETE,        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_ARENA_MEMBER_SEASON_GAMES,           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_ARENA_MEMBER_WEEKLY_GAMES,           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_ARENA_TEAM_RATING_BY_INDEX,          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_ARENA_TEAM_SEASON_GAMES,             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_ARENA_TEAM_WEEKLY_GAMES,             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_BREATH,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_CHARACTER_MODEL,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_CRITERIA_CHEAT,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_DURABILITY_CHEAT,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_EXPLORATION,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_EXPLORATION_ALL,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_FACTION_CHEAT,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleSetFactionCheat           );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_GLYPH,                               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_GLYPH_SLOT,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_GRANTABLE_LEVELS,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_PAID_SERVICE_CHEAT,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_PVP_RANK_CHEAT,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_PVP_TITLE,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_RUNE_COOLDOWN,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_RUNE_COUNT,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_SKILL_CHEAT,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_STAT_CHEAT,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_TITLE_SUFFIX,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SET_WORLDSTATE,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SKILL_BUY_RANK,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SKILL_BUY_STEP,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_STABLE_PET,                              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleStablePet                 );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_STABLE_REVIVE_PET,                       STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleStableRevivePet           );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_STABLE_SWAP_PET,                         STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleStableSwapPet             );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_START_BATTLEFIELD_CHEAT,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_START_QUEST,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_STORE_LOOT_IN_SLOT,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_SUSPEND_COMMS_ACK,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_TARGET_CAST,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_TARGET_SCRIPT_CAST,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_TAXICLEARALLNODES,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_TAXICLEARNODE,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_TAXIENABLEALLNODES,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_TAXIENABLENODE,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_TAXISHOWNODES,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_TEST_DROP_RATE,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_TOGGLE_XP_GAIN,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_TRIGGER_CINEMATIC_CHEAT,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_UNCLAIM_LICENSE,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_UNDRESSPLAYER,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_UNITANIMTIER_CHEAT,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_UNLEARN_DANCE_MOVE,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_UNLEARN_SPELL,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_UNLEARN_TALENTS,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_UNSTABLE_PET,                            STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::HandleUnstablePet               );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_UNUSED5,                                 STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_UNUSED6,                                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_USE_SKILL_CHEAT,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_VOICE_SET_TALKER_MUTED_REQUEST,          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_WEATHER_SPEED_CHEAT,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_XP_CHEAT,                                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(CMSG_ZONE_MAP,                                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_NULL                     );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_AFK_MONITOR_INFO_RESPONSE,               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_AURACASTLOG,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_ARENA_TEAM_CHANGE_FAILED_QUEUED,         STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_ARENA_TEAM_COMMAND_RESULT,               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_ARENA_TEAM_EVENT,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_ARENA_TEAM_INVITE,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_ARENA_TEAM_QUERY_RESPONSE,               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_ARENA_TEAM_ROSTER,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_ARENA_TEAM_STATS,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_BATTLEFIELD_STATUS1,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_BINDZONEREPLY,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_BUY_BANK_SLOT_RESULT,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_CALENDAR_ACTION_PENDING,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_CALENDAR_UPDATE_INVITE_LIST,             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_CHANGEPLAYER_DIFFICULTY_RESULT,          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_CHARACTER_PROFILE,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_CHARACTER_PROFILE_REALM_CONNECTED,       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_CHEAT_DUMP_ITEMS_DEBUG_ONLY_RESPONSE,    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_CHEAT_DUMP_ITEMS_DEBUG_ONLY_RESPONSE_WRITE_FILE, STATUS_NEVER, PROCESS_INPLACE, &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_CHEAT_PLAYER_LOOKUP,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_CHECK_FOR_BOTS,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_CLEAR_EXTRA_AURA_INFO_OBSOLETE,          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_COMBAT_LOG_MULTIPLE,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_COMMENTATOR_GET_PLAYER_INFO,             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_COMPLETION_NPC_RESPONCE,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_COMPRESSED_ACHIEVEMENT_DATA,             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_COMPRESSED_CHAR_ENUM,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_COMPRESSED_GUILD_ROSTER,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_COMPRESSED_UNKNOWN_1310,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_COMPRESSED_UPDATE_OBJECT,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_CORPSE_IS_NOT_IN_INSTANCE,               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_DAMAGE_DONE_OBSOLETE,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_DBLOOKUP,                                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_DEBUGAURAPROC,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_DEBUG_AISTATE,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_DEBUG_LIST_TARGETS,                      STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_DEBUG_SERVER_GEO,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_DUMP_OBJECTS_DATA,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_DYNAMIC_DROP_ROLL_RESULT,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_FORCEACTIONSHOW,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_FORCE_ANIM,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_FORCE_DISPLAY_UPDATE,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_GAMETIMEBIAS_SET,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_GHOSTEE_GONE,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_GMRESPONSE_CREATE_TICKET,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_GOGOGO_OBSOLETE,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_GUILD_CANCEL,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_GUILD_INFO,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_GUILD_SET_NOTE,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_GUILD_TRADESKILL_UPDATE,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_GUILD_XP_UPDATE,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_IGNORE_DIMINISHING_RETURNS_CHEAT,        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_IGNORE_REQUIREMENTS_CHEAT,               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_INSPECT_RATED_BG_STATS,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_INIT_EXTRA_AURA_INFO_OBSOLETE,           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_INSPECT,                                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_INSTANCE_DIFFICULTY,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_JOINED_BATTLEGROUND_QUEUE,               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_KICK_REASON,                             STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_LFG_OPEN_FROM_GOSSIP,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_LFG_UPDATE_LIST,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_LFG_UPDATE_PARTY,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_LFG_UPDATE_PLAYER,                       STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_LF_GUILD_SEARCH_RESULT,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_LOTTERY_QUERY_RESULT_OBSOLETE,           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_LOTTERY_RESULT_OBSOLETE,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_MEETINGSTONE_MEMBER_ADDED,               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_MEETINGSTONE_SETQUEUE,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_MINIGAME_MOVE_FAILED,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_MOVE_CHARACTER_CHEAT,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_MOVE_DISABLE_COLLISION,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_MOVE_DISABLE_GRAVITY,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_MOVE_ENABLE_COLLISION,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_MOVE_ENABLE_GRAVITY,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_MOVE_SET_WALK_IN_AIR,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_MOVE_SKIP_TIME,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_MOVE_UNSET_WALK_IN_AIR,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_NPC_WONT_TALK,                           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_PET_UNLEARN_CONFIRM,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_PLAYER_UNK_DEAD_ALIVE,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_PLAYER_DIFFICULTY_CHANGE,                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_PROFILEDATA_RESPONSE,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_PUREMOUNT_CANCELLED_OBSOLETE,            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_PVP_QUEUE_STATS,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_QUERY_OBJECT_POSITION,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_QUERY_OBJECT_ROTATION,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_RAID_READY_CHECK_ERROR,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_RATED_BG_RATING,                         STATUS_UNHANDLED, PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_REMOVED_FROM_PVP_QUEUE,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_RESET_RANGED_COMBAT_TIMER,               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_RESISTLOG,                               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_RESUME_CAST_BAR,                         STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_RESURRECT_FAILED,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SCRIPT_MESSAGE,                          STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SEND_ALL_COMBAT_LOG,                     STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SERVERINFO,                              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SERVER_BUCK_DATA,                        STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SERVER_BUCK_DATA_START,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SETUP_RESEARCH_HISTORY,                  STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SET_EXTRA_AURA_INFO_NEED_UPDATE_OBSOLETE, STATUS_NEVER,   PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SET_EXTRA_AURA_INFO_OBSOLETE,            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SHOW_MAILBOX,                            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SPELL_CHANCE_PROC_LOG,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SPELL_CHANCE_RESIST_PUSHBACK,            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SPLINE_MOVE_LAND_WALK,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SPLINE_MOVE_NORMAL_FALL,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SPLINE_SET_FLIGHT_BACK_SPEED,            STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SPLINE_SET_FLIGHT_SPEED,                 STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SPLINE_SET_PITCH_RATE,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SPLINE_SET_RUN_BACK_SPEED,               STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SPLINE_SET_RUN_SPEED,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SPLINE_SET_SWIM_BACK_SPEED,              STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SPLINE_SET_SWIM_SPEED,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SPLINE_SET_TURN_RATE,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_SPLINE_SET_WALK_SPEED,                   STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_VOICE_SESSION_ADJUST_PRIORITY,           STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_VOICE_SESSION_ENABLE,                    STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_OPCODE_HANDLER_OLD(SMSG_ZONE_MAP,                                STATUS_NEVER,     PROCESS_INPLACE,      &WorldSession::Handle_ServerSide               );
      //DEFINE_SERVER_OPCODE_HANDLER(SMSG_GUILD_REPUTATION_WEEKLY_CAP,             STATUS_NEVER);

#undef DEFINE_SERVER_OPCODE_HANDLER
};
