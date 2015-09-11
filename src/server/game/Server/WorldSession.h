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

/// \addtogroup u2w
/// @{
/// \file

#ifndef __WORLDSESSION_H
#define __WORLDSESSION_H

#include "Common.h"
#include "SharedDefines.h"
#include "AddonMgr.h"
#include "DatabaseEnv.h"
#include "World.h"
#include "WorldPacket.h"
#include "Cryptography/BigNumber.h"
#include "Opcodes.h"
#include "EventProcessor.h"

class CalendarEvent;
class CalendarInvite;
class Creature;
class GameObject;
class InstanceSave;
class Item;
class Object;
class Player;
class Quest;
class SpellCastTargets;
class Unit;
class Warden;
class WorldObject;
class WorldPacket;
class WorldSocket;
struct AreaTableEntry;
struct AuctionEntry;
struct DeclinedName;
struct ItemTemplate;
struct MovementInfo;
class ObjectGuid;
class BattlePayMgr;

namespace lfg
{
struct LfgJoinResultData;
struct LfgPlayerBoot;
struct LfgProposal;
struct LfgQueueStatusData;
struct LfgPlayerRewardData;
struct LfgRoleCheck;
struct LfgUpdateData;
}

enum AuthFlags
{
    AT_AUTH_FLAG_NONE                       = 0x0,
    AT_AUTH_FLAG_90_LVL_UP                  = 0x1,
    AT_AUTH_FLAG_RESTORE_DELETED_CHARACTER  = 0x2,
};

enum BattlePayDistribStatus
{
    BATTLE_PAY_DIST_STATUS_NONE             = 0,
    BATTLE_PAY_DIST_STATUS_AVAILABLE        = 1,
    BATTLE_PAY_DIST_STATUS_ADD_TO_PROCESS   = 2,
    BATTLE_PAY_DIST_STATUS_PROCESS_COMPLETE = 3,    //send SMSG_CHARACTER_UPGRADE_CHARACTER_CHOSEN
    BATTLE_PAY_DIST_STATUS_FINISHED         = 4
};

namespace WorldPackets
{
    namespace Character
    {
        struct CharacterCreateInfo;
        struct CharacterRenameInfo;
        struct CharCustomizeInfo;
        struct CharRaceOrFactionChangeInfo;
        struct CharacterUndeleteInfo;

        class EnumCharacters;
        class CreateChar;
        class DeleteChar;
        class CharacterRenameRequest;
        class CharCustomize;
        class CharRaceOrFactionChange;
        class GenerateRandomCharacterName;
        class ReorderCharacters;
        class UndeleteCharacter;
        class PlayerLogin;
        class LogoutRequest;
        class LogoutCancel;
        class LoadingScreenNotify;
    }

    namespace ClientConfig
    {
        class RequestAccountData;
        class UserClientUpdateAccountData;
    }

    namespace Channel
    {
        class ChannelPlayerCommand;
        class JoinChannel;
        class LeaveChannel;
    }

    namespace Chat
    {
        class ChatMessage;
        class ChatMessageWhisper;
        class ChatMessageChannel;
        class ChatAddonMessage;
        class ChatAddonMessageWhisper;
        class ChatMessageAFK;
        class ChatMessageDND;
        class ChatMessageEmote;
        class CTextEmote;
    }

    namespace Combat
    {
        class AttackSwing;
        class AttackStop;
    }

    namespace Duel
    {
        class DuelResponse;
    }

    namespace EquipmentSet
    {
        class SaveEquipmentSet;
    }

    namespace GameObject
    {
        class GameObjectReportUse;
        class GameObjectUse;
    }

    namespace Guild
    {
        class QueryGuildInfo;
    }

    namespace Item
    {
        class BuyBackItem;
        class ItemRefundInfo;
        class RepairItem;
        class SellItem;
        class SplitItem;
        class SwapInvItem;
        class SwapItem;
        class AutoEquipItem;
        class DestroyItem;
        class BuyItem;
        class AutoStoreBagItem;
        class TransmogrigyItem;
        class VoidStorageContents;
        struct ItemInstance;
    }

    namespace Loot
    {
        class LootUnit;
        class AutoStoreLootItem;
        class LootRelease;
        class LootMoney;
    }

    namespace Mail
    {
        class MailCreateTextItem;
        class MailDelete;
        class MailGetList;
        class MailMarkAsRead;
        class MailQueryNextMailTime;
        class MailReturnToSender;
        class MailTakeItem;
        class MailTakeMoney;
        class SendMail;
    }

    namespace Misc
    {
        class AreaTrigger;
        class SetSelection;
        class ViolenceLevel;
        class TimeSyncResponse;
        class TutorialSetFlag;
        class SetDungeonDifficulty;
        class SetRaidDifficulty;
        class PortGraveyard;
        class ReclaimCorpse;
        class RepopRequest;
        class RequestCemeteryList;
        class ResurrectResponse;
        class StandStateChange;
        class UITimeRequest;
    }

    namespace Movement
    {
        class ClientPlayerMovement;
        class WorldPortAck;
        class MoveTeleportAck;
    }

    namespace NPC
    {
        class Hello;
    }

    namespace BattlePay
    {
        class DistributionAssignToTarget;
        class StartPurchase;
        class ConfirmPurchaseResponse;
    }

    namespace Query
    {
        class QueryCreature;
        class QueryPlayerName;
        class QueryPageText;
        class QueryNPCText;
        class DBQueryBulk;
        class QueryGameObject;
        class QueryCorpseLocationFromClient;
        class QueryCorpseTransport;
        class QuestPOIQuery;
        class QueryQuestCompletionNPCs;
    }

    namespace Quest
    {
        class QuestGiverStatusQuery;
        class QuestGiverStatusMultipleQuery;
        class QuestGiverHello;
        class QueryQuestInfo;
        class QuestGiverChooseReward;
        class QuestGiverCompleteQuest;
        class QuestGiverRequestReward;
        class QuestGiverQueryQuest;
    }

    namespace Social
    {
        class AddFriend;
        class AddIgnore;
        class DelFriend;
        class DelIgnore;
        class SendContactList;
        class SetContactNotes;
    }

    namespace Spells
    {
        class CancelAura;
        class CastSpell;
        class SetActionButton;
        class PetCastSpell;
        class ItemUse;
    }

    namespace Talent
    {
        class SetSpecialization;
        class LearnTalent;
    }

    namespace Trade
    {
        class CancelTrade;
    }

    namespace Who
    {
        class WhoIsRequest;
        class WhoRequestPkt;
    }
}

enum AccountDataType
{
    GLOBAL_CONFIG_CACHE             = 0,                    // 0x01 g
    PER_CHARACTER_CONFIG_CACHE      = 1,                    // 0x02 p
    GLOBAL_BINDINGS_CACHE           = 2,                    // 0x04 g
    PER_CHARACTER_BINDINGS_CACHE    = 3,                    // 0x08 p
    GLOBAL_MACROS_CACHE             = 4,                    // 0x10 g
    PER_CHARACTER_MACROS_CACHE      = 5,                    // 0x20 p
    PER_CHARACTER_LAYOUT_CACHE      = 6,                    // 0x40 p
    PER_CHARACTER_CHAT_CACHE        = 7,                    // 0x80 p
};

#define NUM_ACCOUNT_DATA_TYPES        8

#define GLOBAL_CACHE_MASK           0x15
#define PER_CHARACTER_CACHE_MASK    0xAA

#define REGISTERED_ADDON_PREFIX_SOFTCAP 64

enum TutorialAction
{
    TUTORIAL_ACTION_RESET  = 0,
    TUTORIAL_ACTION_UPDATE = 1,
    TUTORIAL_ACTION_CLEAR  = 2
};

struct AccountData
{
    AccountData() : Time(0), Data("") {}

    time_t Time;
    std::string Data;
};

enum PartyOperation
{
    PARTY_OP_INVITE = 0,
    PARTY_OP_UNINVITE = 1,
    PARTY_OP_LEAVE = 2,
    PARTY_OP_SWAP = 4
};

enum PartyResult
{
    ERR_PARTY_RESULT_OK                 = 0,
    ERR_BAD_PLAYER_NAME_S               = 1,
    ERR_TARGET_NOT_IN_GROUP_S           = 2,
    ERR_TARGET_NOT_IN_INSTANCE_S        = 3,
    ERR_GROUP_FULL                      = 4,
    ERR_ALREADY_IN_GROUP_S              = 5,
    ERR_NOT_IN_GROUP                    = 6,
    ERR_NOT_LEADER                      = 7,
    ERR_PLAYER_WRONG_FACTION            = 8,
    ERR_IGNORING_YOU_S                  = 9,
    ERR_LFG_PENDING                     = 12,
    ERR_INVITE_RESTRICTED               = 13,
    ERR_GROUP_SWAP_FAILED               = 14,               // if (PartyOperation == PARTY_OP_SWAP) ERR_GROUP_SWAP_FAILED else ERR_INVITE_IN_COMBAT
    ERR_INVITE_UNKNOWN_REALM            = 15,
    ERR_INVITE_NO_PARTY_SERVER          = 16,
    ERR_INVITE_PARTY_BUSY               = 17,
    ERR_PARTY_TARGET_AMBIGUOUS          = 18,
    ERR_PARTY_LFG_INVITE_RAID_LOCKED    = 19,
    ERR_PARTY_LFG_BOOT_LIMIT            = 20,
    ERR_PARTY_LFG_BOOT_COOLDOWN_S       = 21,
    ERR_PARTY_LFG_BOOT_IN_PROGRESS      = 22,
    ERR_PARTY_LFG_BOOT_TOO_FEW_PLAYERS  = 23,
    ERR_PARTY_LFG_BOOT_NOT_ELIGIBLE_S   = 24,
    ERR_RAID_DISALLOWED_BY_LEVEL        = 25,
    ERR_PARTY_LFG_BOOT_IN_COMBAT        = 26,
    ERR_VOTE_KICK_REASON_NEEDED         = 27,
    ERR_PARTY_LFG_BOOT_DUNGEON_COMPLETE = 28,
    ERR_PARTY_LFG_BOOT_LOOT_ROLLS       = 29,
    ERR_PARTY_LFG_TELEPORT_IN_COMBAT    = 30,
    ERR_PARTY_ALREADY_IN_BATTLEGROUND_QUEUE = 31,
    ERR_PARTY_CONFIRMING_BATTLEGROUND_QUEUE = 32,
    ERR_CROSS_REALM_RAID_INVITE         = 33,
    ERR_RAID_DISALLOWED_BY_CROSS_REALM  = 34,
    ERR_PARTY_ROLE_NOT_AVAILABLE        = 35,
    ERR_PARTY_LFG_BOOT_VOTE_REGISTERED  = 36,
    ERR_PARTY_PRIVATE_GROUP_ONLY        = 37,

};


enum BFLeaveReason
{
    BF_LEAVE_REASON_CLOSE     = 0x00000001,
    //BF_LEAVE_REASON_UNK1      = 0x00000002, (not used)
    //BF_LEAVE_REASON_UNK2      = 0x00000004, (not used)
    BF_LEAVE_REASON_EXITED    = 0x00000008,
    BF_LEAVE_REASON_LOW_LEVEL = 0x00000010,
};

enum ChatRestrictionType
{
    ERR_CHAT_RESTRICTED = 0,
    ERR_CHAT_THROTTLED  = 1,
    ERR_USER_SQUELCHED  = 2,
    ERR_YELL_RESTRICTED = 3
};

enum CharterTypes
{
    GUILD_CHARTER_TYPE                            = 4,
};

class LoginQueryHolder : public SQLQueryHolder
{
private:
    uint32 m_accountId;
    ObjectGuid m_guid;
public:
    LoginQueryHolder(uint32 accountId, ObjectGuid guid)
        : m_accountId(accountId), m_guid(guid) { }
    ObjectGuid const& GetGuid() const { return m_guid; }
    uint32 GetAccountId() const { return m_accountId; }
    bool Initialize();
};

//class to deal with packet processing
//allows to determine if next packet is safe to be processed
class PacketFilter
{
public:
    explicit PacketFilter(WorldSession* pSession) : m_pSession(pSession) {}
    virtual ~PacketFilter() {}

    virtual bool Process(WorldPacket* /*packet*/) { return true; }
    virtual bool ProcessLogout() const { return true; }

protected:
    WorldSession* const m_pSession;
};
//process only thread-safe packets in Map::Update()
class MapSessionFilter : public PacketFilter
{
public:
    explicit MapSessionFilter(WorldSession* pSession) : PacketFilter(pSession) {}
    ~MapSessionFilter() {}

    virtual bool Process(WorldPacket* packet);
    //in Map::Update() we do not process player logout!
    virtual bool ProcessLogout() const { return false; }
};

//class used to filer only thread-unsafe packets from queue
//in order to update only be used in World::UpdateSessions()
class WorldSessionFilter : public PacketFilter
{
public:
    explicit WorldSessionFilter(WorldSession* pSession) : PacketFilter(pSession) {}
    ~WorldSessionFilter() {}

    virtual bool Process(WorldPacket* packet);
};

/// Player session in the World
class WorldSession
{
    public:
        WorldSession(uint32 id, uint32 battlenetAccountId, std::shared_ptr<WorldSocket> sock, AccountTypes sec, uint8 expansion, time_t mute_time, LocaleConstant locale, uint32 recruiter, bool isARecruiter, AuthFlags flag);
        ~WorldSession();

        bool PlayerLoading() const { return m_playerLoading; }
        bool PlayerLogout() const { return m_playerLogout; }
        bool PlayerLogoutWithSave() const { return m_playerLogout && m_playerSave; }
        bool PlayerRecentlyLoggedOut() const { return m_playerRecentlyLogout; }

        void ReadAddonsInfo(ByteBuffer& data);
        void SendAddonsInfo();
        bool IsAddonRegistered(const std::string& prefix) const;

        void ReadMovementInfo(WorldPacket& data, MovementInfo* mi);
        void CheckMovementInfo(MovementInfo* mi);
        static void WriteMovementInfo(WorldPacket& data, MovementInfo* mi, Unit* unit = NULL);

        void SendPacket(WorldPacket const* packet, bool forced = false);
        void SendNotification(const char *format, ...) ATTR_PRINTF(2, 3);
        void SendNotification(uint32 string_id, ...);
        void SendPetNameInvalid(uint32 error, ObjectGuid const& guid, const std::string& name, DeclinedName *declinedName);
        void SendPartyResult(PartyOperation operation, const std::string& member, PartyResult res, uint32 val = 0);
        void SendGroupInvite(Player* player, bool AlowEnter);
        void SendSetPhaseShift(std::set<uint32> const& phaseIds, std::set<uint32> const& terrainswaps, std::set<uint32> const& worldareaids, uint32 flags = 0x1F);
        void SendQueryTimeResponse();
        void HandleLearnPetSpecialization(WorldPacket& data);

        void SendAuthResponse(uint8 code, bool hasAccountData = true, bool queued = false, uint32 queuePos = 0);
        void SendClientCacheVersion(uint32 version);
        void HandleBattlePayPurchaseListGet(WorldPacket& packet);
        void HandleBattlePayProductList(WorldPacket& packet);
        void SendBattlePayDistribution(uint8 status, uint64 purchaseID = 1, ObjectGuid guid =ObjectGuid::Empty);
        void SendDisplayPromo(int32 promo);
        void SendFeatureSystemStatusGlueScreen();

        AccountTypes GetSecurity() const { return _security; }
        uint32 GetAccountId() const { return _accountId; }
        ObjectGuid GetAccountGUID() const;
        uint32 GetBattlenetAccountId() const { return _battlenetAccountId; }
        ObjectGuid GetBattlenetAccountGUID() const;
        Player* GetPlayer() const { return _player; }
        std::string GetPlayerName(bool simple = true) const;

        ObjectGuid::LowType GetGuidLow() const;
        void SetSecurity(AccountTypes security) { _security = security; }
        std::string const& GetRemoteAddress() { return m_Address; }
        void SetPlayer(Player* player);
        uint8 Expansion() const { return m_expansion; }

        void InitWarden(BigNumber* k, std::string os);

        /// Session in auth.queue currently
        void SetInQueue(bool state) { m_inQueue = state; }

        /// Is the user engaged in a log out process?
        bool isLogingOut() const { return _logoutTime || m_playerLogout; }

        /// Engage the logout process for the user
        void LogoutRequest(time_t requestTime)
        {
            _logoutTime = requestTime;
        }

        /// Is logout cooldown expired?
        bool ShouldLogOut(time_t currTime) const
        {
            return (_logoutTime > 0 && currTime >= _logoutTime + 20);
        }

        void LogoutPlayer(bool Save);
        void KickPlayer();

        void QueuePacket(WorldPacket* new_packet);
        bool Update(uint32 diff, PacketFilter& updater);

        /// Handle the authentication waiting queue (to be completed)
        void SendAuthWaitQue(uint32 position);

        //void SendTestCreatureQueryOpcode(uint32 entry, ObjectGuid guid, uint32 testvalue);
        void SendNameQueryOpcode(ObjectGuid guid);

        void SendTrainerList(ObjectGuid const& guid);
        void SendTrainerList(ObjectGuid const& guid, const std::string& strTitle);
        void SendListInventory(ObjectGuid const& guid);
        void SendShowBank(ObjectGuid const& guid);
        void SendTabardVendorActivate(ObjectGuid const& guid);
        void SendSpiritResurrect();
        void SendBindPoint(Creature* npc);

        void SendBattleGroundList(ObjectGuid guid, BattlegroundTypeId bgTypeId);

        void SendTradeStatus(TradeStatus status);
        void SendUpdateTrade(bool trader_data = true);
        void SendCancelTrade();

        void SendPetitionQueryOpcode(ObjectGuid petitionguid);

        // Pet
        void SendPetNameQuery(ObjectGuid const& guid);
        void SendStablePet(ObjectGuid const& guid);
        void SendStablePetCallback(PreparedQueryResult result, ObjectGuid const& guid);
        void SendStableResult(uint8 res);
        bool CheckStableMaster(ObjectGuid const& guid);

        // Account Data
        AccountData* GetAccountData(AccountDataType type) { return &m_accountData[type]; }
        void SetAccountData(AccountDataType type, time_t tm, std::string data);
        void SendTimeZoneInformation();
        void LoadGlobalAccountData();
        void LoadAccountData(PreparedQueryResult result, uint32 mask);

        void LoadTutorialsData();
        void SendTutorialsData();
        void SaveTutorialsData(SQLTransaction& trans);
        uint32 GetTutorialInt(uint8 index) const { return m_Tutorials[index]; }
        void SetTutorialInt(uint8 index, uint32 value)
        {
            if (m_Tutorials[index] != value)
            {
                m_Tutorials[index] = value;
                m_TutorialsChanged = true;
            }
        }
        //used with item_page table
        bool SendItemInfo(uint32 itemid, WorldPacket data);
        //auction
        void SendAuctionHello(ObjectGuid guid, Creature* unit);
        void SendAuctionCommandResult(AuctionEntry* auction, uint32 Action, uint32 ErrorCode, uint32 bidError = 0);
        void SendAuctionBidderNotification(OpcodeServer opcode, uint32 auctionId, ObjectGuid const& bidder, uint64 bidSum, uint64 diff, WorldPackets::Item::ItemInstance const& item);
        void SendAuctionOwnerNotification(OpcodeServer opcode, AuctionEntry* auction, WorldPackets::Item::ItemInstance const& item, bool profit = false);

        //Item Enchantment
        void SendEnchantmentLog(ObjectGuid const& Target, ObjectGuid const& Caster, ObjectGuid const& item, uint32 ItemID, uint32 SpellID);
        void SendItemEnchantTimeUpdate(ObjectGuid const& Playerguid, ObjectGuid const& Itemguid, uint32 slot, uint32 Duration);

        //Taxi
        void SendTaxiStatus(ObjectGuid guid);
        void SendTaxiMenu(Creature* unit);
        void SendDoFlight(uint32 mountDisplayId, uint32 path, uint32 pathNode = 0);
        bool SendLearnNewTaxiNode(Creature* unit);
        void SendDiscoverNewTaxiNode(uint32 nodeid);

        // Guild
        void SendPetitionShowList(ObjectGuid guid);

        void BuildPartyMemberStatsChangedPacket(Player* player, WorldPacket* data, bool full = false);

        void DoLootRelease(ObjectGuid lguid);

        // Account mute time
        time_t m_muteTime;

        // Locales
        LocaleConstant GetSessionDbcLocale() const { return m_sessionDbcLocale; }
        LocaleConstant GetSessionDbLocaleIndex() const { return m_sessionDbLocaleIndex; }
        const char *GetTrinityString(int32 entry) const;

        uint32 GetLatency() const { return m_latency; }
        void SetLatency(uint32 latency) { m_latency = latency; }
        uint32 getDialogStatus(Player* player, Object* questgiver, uint32 defstatus);

        std::atomic<int32> m_timeOutTime;

        void UpdateTimeOutTime(uint32 diff)
        {
            m_timeOutTime -= int32(diff);
        }

        void ResetTimeOutTime()
        {
            m_timeOutTime = int32(sWorld->getIntConfig(CONFIG_SOCKET_TIMEOUTTIME));
        }

        bool IsConnectionIdle() const
        {
            return m_timeOutTime <= 0 && !m_inQueue;
        }

        // Recruit-A-Friend Handling
        uint32 GetRecruiterId() const { return recruiterId; }
        bool IsARecruiter() const { return isRecruiter; }

    public:                                                 // opcodes handlers

        void Handle_NULL(WorldPacket& recvPacket);          // not used
        void Handle_EarlyProccess(WorldPacket& recvPacket); // just mark packets processed in WorldSocket::OnRead
        void Handle_ServerSide(WorldPacket& recvPacket);    // sever side only, can't be accepted from client
        void Handle_Deprecated(WorldPacket& recvPacket);    // never used anymore by client

        void HandleCharEnumOpcode(WorldPackets::Character::EnumCharacters& /*enumCharacters*/);
        void HandleCharDeleteOpcode(WorldPackets::Character::DeleteChar& charDelete);
        void HandleCharCreateOpcode(WorldPackets::Character::CreateChar& charCreate);
        void HandleCharCreateCallback(PreparedQueryResult result, WorldPackets::Character::CharacterCreateInfo* createInfo);
        void HandlePlayerLoginOpcode(WorldPackets::Character::PlayerLogin& playerLogin);
        void HandleLoadScreenOpcode(WorldPackets::Character::LoadingScreenNotify& loadingScreenNotify);
        void HandleCharEnum(PreparedQueryResult result, bool isDelete);
        void HandlePlayerLogin(LoginQueryHolder * holder);
        void HandleCharFactionOrRaceChange(WorldPacket& recvData);
        void HandleRandomizeCharNameOpcode(WorldPacket& recvData);
        void HandleReorderCharacters(WorldPacket& recvData);

        void SendCharCreate(ResponseCodes result);
        void SendCharDelete(ResponseCodes result);

        // played time
        void HandlePlayedTime(WorldPacket& recvPacket);

        // new
        void HandleMoveUnRootAck(WorldPacket& recvPacket);
        void HandleMoveRootAck(WorldPacket& recvPacket);
        void HandleLookingForGroup(WorldPacket& recvPacket);

        // cemetery/graveyard related
        void HandlePortGraveyard(WorldPackets::Misc::PortGraveyard& packet);
        void HandleRequestCemeteryList(WorldPackets::Misc::RequestCemeteryList& packet);

        // new inspect
        void HandleInspectOpcode(WorldPacket& recvPacket);

        // new party stats
        void HandleInspectHonorStatsOpcode(WorldPacket& recvPacket);

        void HandleInspectRatedBGStats(WorldPacket& recvPacket);

        void HandleMoveWaterWalkAck(WorldPacket& recvPacket);
        void HandleFeatherFallAck(WorldPacket& recvData);

        void HandleMoveHoverAck(WorldPacket& recvData);

        void HandleMountSpecialAnimOpcode(WorldPacket& recvdata);

        // character view
        void HandleShowingHelmOpcode(WorldPacket& recvData);
        void HandleShowingCloakOpcode(WorldPacket& recvData);

        // repair
        void HandleRepairItemOpcode(WorldPacket& recvPacket);

        void HandleMoveTeleportAck(WorldPackets::Movement::MoveTeleportAck& packet);
        void HandleForceSpeedChangeAck(WorldPacket& recvData);

        void HandlePingOpcode(WorldPacket& recvPacket);
        void HandleAuthSessionOpcode(WorldPacket& recvPacket);
        void HandleRepopRequest(WorldPackets::Misc::RepopRequest& packet);
        void HandleAutostoreLootItemOpcode(WorldPackets::Loot::AutoStoreLootItem& packet);
        void HandleLootMoneyOpcode(WorldPacket& recvPacket);
        void HandleLootSpecIdOpcode(WorldPacket& recvPacket);
        void HandleLootOpcode(WorldPackets::Loot::LootUnit& packet);
        void HandleLootReleaseOpcode(WorldPacket& recvPacket);
        void HandleLootMasterGiveOpcode(WorldPacket& recvPacket);
        void HandleWhoOpcode(WorldPackets::Who::WhoRequestPkt& whoRequest);
        void HandleLogoutRequestOpcode(WorldPacket& recvPacket);
        void HandlePlayerLogoutOpcode(WorldPacket& recvPacket);
        void HandleLogoutCancelOpcode(WorldPacket& recvPacket);

        // GM Ticket opcodes
        void HandleGMTicketCreateOpcode(WorldPacket& recvPacket);
        void HandleGMTicketUpdateOpcode(WorldPacket& recvPacket);
        void HandleGMTicketDeleteOpcode(WorldPacket& recvPacket);
        void HandleGMTicketGetTicketOpcode(WorldPacket& recvPacket);
        void HandleGMTicketSystemStatusOpcode(WorldPacket& recvPacket);
        void HandleGMSurveySubmit(WorldPacket& recvPacket);
        void HandleReportLag(WorldPacket& recvPacket);
        void HandleGMResponseResolve(WorldPacket& recvPacket);

        void HandleTogglePvP(WorldPacket& recvPacket);

        void HandleZoneUpdateOpcode(WorldPacket& recvPacket);
        void HandleSetSelectionOpcode(WorldPackets::Misc::SetSelection& packet);
        void HandleStandStateChangeOpcode(WorldPackets::Misc::StandStateChange& packet);
        void HandleEmoteOpcode(WorldPacket& recvPacket);
        void HandleContactListOpcode(WorldPackets::Social::SendContactList& packet);
        void HandleAddFriendOpcode(WorldPackets::Social::AddFriend& packet);
        void HandleAddFriendOpcodeCallBack(PreparedQueryResult result, std::string friendNote);
        void HandleDelFriendOpcode(WorldPackets::Social::DelFriend& packet);
        void HandleAddIgnoreOpcode(WorldPackets::Social::AddIgnore& packet);
        void HandleAddIgnoreOpcodeCallBack(PreparedQueryResult result);
        void HandleDelIgnoreOpcode(WorldPackets::Social::DelIgnore& packet);
        void HandleSetContactNotesOpcode(WorldPackets::Social::SetContactNotes& packet);
        void HandleBugOpcode(WorldPacket& recvPacket);

        void HandleSendDuelRequest(WorldPacket& recvData);
        void HandleDuelResponseOpcode(WorldPackets::Duel::DuelResponse& duelResponse);
        void HandleDuelAccepted();
        void HandleDuelCancelled();

        void HandleAreaTriggerOpcode(WorldPackets::Misc::AreaTrigger& packet);

        void HandleSetFactionAtWar(WorldPacket& recvData);
        void HandleUnsetFactionAtWar(WorldPacket& recvData);
        void HandleSetLfgBonusFaction(WorldPacket& recvData);
        void HandleSetFactionCheat(WorldPacket& recvData);
        void HandleSetWatchedFactionOpcode(WorldPacket& recvData);
        void HandleSetFactionInactiveOpcode(WorldPacket& recvData);

        void HandleUpdateAccountData(WorldPackets::ClientConfig::UserClientUpdateAccountData& packet);
        void HandleRequestAccountData(WorldPackets::ClientConfig::RequestAccountData& request);
        void HandleSetActionButtonOpcode(WorldPackets::Spells::SetActionButton& packet);

        void HandleGameObjectUseOpcode(WorldPacket& recPacket);
        void HandleMeetingStoneInfo(WorldPacket& recPacket);
        void HandleGameobjectReportUse(WorldPacket& recvPacket);

        void HandleNameQueryOpcode(WorldPackets::Query::QueryPlayerName& packet);

        void HandleQueryTimeOpcode(WorldPacket& recvPacket);

        void HandleCreatureQuery(WorldPackets::Query::QueryCreature& packet);

        void HandleGameObjectQueryOpcode(WorldPackets::Query::QueryGameObject& packet);

        void HandleMoveWorldportAckOpcode(WorldPacket& recvPacket);
        void HandleMoveWorldportAckOpcode();                // for server-side calls

        void HandleMovementOpcodes(WorldPackets::Movement::ClientPlayerMovement& packet);
        void HandleSetActiveMoverOpcode(WorldPacket& recvData);
        void HandleMoveNotActiveMover(WorldPacket& recvData);
        void HandleDismissControlledVehicle(WorldPacket& recvData);
        void HandleRequestVehicleExit(WorldPacket& recvData);
        void HandleChangeSeatsOnControlledVehicle(WorldPacket& recvData);
        void HandleMoveTimeSkippedOpcode(WorldPacket& recvData);

        void HandleRequestRaidInfoOpcode(WorldPacket& recvData);

        void HandleBattlefieldStatusOpcode(WorldPacket& recvData);
        void HandleBattleMasterHelloOpcode(WorldPacket& recvData);

        void HandleGroupInviteOpcode(WorldPacket& recvPacket);
        //void HandleGroupCancelOpcode(WorldPacket& recvPacket);
        void HandleGroupInviteResponseOpcode(WorldPacket& recvPacket);
        void HandleGroupUninviteOpcode(WorldPacket& recvPacket);
        void HandleGroupSetLeaderOpcode(WorldPacket& recvPacket);
        void HandleGroupSetRolesOpcode(WorldPacket& recvData);
        void HandleGroupDisbandOpcode(WorldPacket& recvPacket);
        void HandleOptOutOfLootOpcode(WorldPacket& recvData);
        void HandleLootMethodOpcode(WorldPacket& recvPacket);
        void HandleLootRoll(WorldPacket& recvData);
        void HandleLootMasterAskForRoll(WorldPacket& recvData);
        void HandleRequestPartyMemberStatsOpcode(WorldPacket& recvData);
        void HandleRaidConfirmReadyCheck(WorldPacket& recvData);
        void HandleRaidLeaderReadyCheck(WorldPacket& recvData);
        void HandleRaidTargetUpdateOpcode(WorldPacket& recvData);
        void HandleGroupRaidConvertOpcode(WorldPacket& recvData);
        void HandleGroupChangeSubGroupOpcode(WorldPacket& recvData);
        void HandleGroupSwapSubGroupOpcode(WorldPacket& recvData);
        void HandleGroupAssistantLeaderOpcode(WorldPacket& recvData);
        void HandleGroupEveryoneIsAssistantOpcode(WorldPacket& recvData);
        void HandlePartyAssignmentOpcode(WorldPacket& recvData);
        void HandleRolePollBegin(WorldPacket & recvData);

        void HandlePetitionBuyOpcode(WorldPacket& recvData);
        void HandlePetitionShowSignOpcode(WorldPacket& recvData);
        void HandlePetitionQueryOpcode(WorldPacket& recvData);
        void HandlePetitionRenameOpcode(WorldPacket& recvData);
        void HandlePetitionSignOpcode(WorldPacket& recvData);
        void SendPetitionSignResult(ObjectGuid const& playerGuid, ObjectGuid const& petitionGuid, uint8 result);
        void HandlePetitionDeclineOpcode(WorldPacket& recvData);
        void HandleOfferPetitionOpcode(WorldPacket& recvData);
        void HandleTurnInPetitionOpcode(WorldPacket& recvData);

        void HandleGuildQueryOpcode(WorldPacket& recvPacket);
        void HandleGuildInviteOpcode(WorldPacket& recvPacket);
        void HandleGuildRemoveOpcode(WorldPacket& recvPacket);
        void HandleGuildAcceptOpcode(WorldPacket& recvPacket);
        void HandleGuildDeclineOpcode(WorldPacket& recvPacket);
        void HandleGuildEventLogQueryOpcode(WorldPacket& recvPacket);
        void HandleGuildRosterOpcode(WorldPacket& recvPacket);
        void HandleGuildRewardsQueryOpcode(WorldPacket& recvPacket);
        void HandleGuildPromoteOpcode(WorldPacket& recvPacket);
        void HandleGuildDemoteOpcode(WorldPacket& recvPacket);
        void HandleGuildAssignRankOpcode(WorldPacket& recvPacket);
        void HandleGuildLeaveOpcode(WorldPacket& recvPacket);
        void HandleGuildDisbandOpcode(WorldPacket& recvPacket);
        void HandleGuildLeaderOpcode(WorldPacket& recvPacket);
        void HandleGuildMOTDOpcode(WorldPacket& recvPacket);
        void HandleGuildNewsUpdateStickyOpcode(WorldPacket& recvPacket);
        void HandleGuildSetNoteOpcode(WorldPacket& recvPacket);
        void HandleGuildQueryRanksOpcode(WorldPacket& recvPacket);
        void HandleGuildQueryNewsOpcode(WorldPacket& recvPacket);
        void HandleSwapRanks(WorldPacket& recvPacket);
        void HandleGuildSetRankPermissionsOpcode(WorldPacket& recvPacket);
        void HandleGuildAddRankOpcode(WorldPacket& recvPacket);
        void HandleGuildDelRankOpcode(WorldPacket& recvPacket);
        void HandleGuildChangeInfoTextOpcode(WorldPacket& recvPacket);
        void HandleSaveGuildEmblemOpcode(WorldPacket& recvPacket);
        void HandleGuildRequestPartyState(WorldPacket& recvPacket);
        void HandleAutoDeclineGuildInvites(WorldPacket& recvPacket);
        void HandleGuildQueryGuildRecipesOpcode(WorldPacket& recvPacket);
        void HandleGuildQueryGuildMembersForRecipe(WorldPacket& recvPacket);
        void HandleGuildQueryGuildMembersRecipes(WorldPacket& recvPacket);

        void HandleGuildRequestChallengeUpdate(WorldPacket& recvPacket);

        void HandleGuildFinderAddRecruit(WorldPacket& recvPacket);
        void HandleGuildFinderBrowse(WorldPacket& recvPacket);
        void HandleGuildFinderDeclineRecruit(WorldPacket& recvPacket);
        void HandleGuildFinderGetApplications(WorldPacket& recvPacket);
        void HandleGuildFinderGetRecruits(WorldPacket& recvPacket);
        void HandleGuildFinderPostRequest(WorldPacket& recvPacket);
        void HandleGuildFinderRemoveRecruit(WorldPacket& recvPacket);
        void HandleGuildFinderSetGuildPost(WorldPacket& recvPacket);

        void HandleTaxiNodeStatusQueryOpcode(WorldPacket& recvPacket);
        void HandleTaxiQueryAvailableNodes(WorldPacket& recvPacket);
        void HandleTaxiRequestEarlyLandingOpcode(WorldPacket& recvPacket);
        void HandleActivateTaxiOpcode(WorldPacket& recvPacket);
        void HandleActivateTaxiExpressOpcode(WorldPacket& recvPacket);
        void HandleMoveSplineDoneOpcode(WorldPacket& recvPacket);
        void SendActivateTaxiReply(ActivateTaxiReply reply);

        void HandleTabardVendorActivateOpcode(WorldPacket& recvPacket);
        void HandleBankerActivateOpcode(WorldPacket& recvPacket);
        void HandleBuyBankSlotOpcode(WorldPacket& recvPacket);
        void HandleTrainerListOpcode(WorldPacket& recvPacket);
        void HandleTrainerBuySpellOpcode(WorldPacket& recvPacket);
        void HandlePetitionShowListOpcode(WorldPacket& recvPacket);
        void HandleGossipHelloOpcode(WorldPackets::NPC::Hello& packet);
        void HandleGossipSelectOptionOpcode(WorldPacket& recvPacket);
        void HandleSpiritHealerActivateOpcode(WorldPacket& recvPacket);
        void HandleNpcTextQueryOpcode(WorldPackets::Query::QueryNPCText& packet);
        void HandleBinderActivateOpcode(WorldPacket& recvPacket);
        void HandleListStabledPetsOpcode(WorldPacket& recvPacket);
        void HandleStableChangeSlot(WorldPacket& recvPacket);
        void HandleStableChangeSlotCallback(PreparedQueryResult result, uint8 new_slot);        //void HandleBuyStableSlot(WorldPacket& recvPacket);
        void HandleStableRevivePet(WorldPacket& recvPacket);
        void SendTrainerService(ObjectGuid guid, uint32 spellId, uint32 trainState);

        void HandleDuelCancelledOpcode(WorldPacket& recvPacket);

        void HandleAcceptTradeOpcode(WorldPacket& recvPacket);
        void HandleBeginTradeOpcode(WorldPacket& recvPacket);
        void HandleBusyTradeOpcode(WorldPacket& recvPacket);
        void HandleCancelTradeOpcode(WorldPacket& recvPacket);
        void HandleClearTradeItemOpcode(WorldPacket& recvPacket);
        void HandleIgnoreTradeOpcode(WorldPacket& recvPacket);
        void HandleInitiateTradeOpcode(WorldPacket& recvPacket);
        void HandleSetTradeGoldOpcode(WorldPacket& recvPacket);
        void HandleSetTradeItemOpcode(WorldPacket& recvPacket);
        void HandleUnacceptTradeOpcode(WorldPacket& recvPacket);

        void HandleAuctionHelloOpcode(WorldPacket& recvPacket);
        void HandleAuctionListItems(WorldPacket& recvData);
        void HandleAuctionListBidderItems(WorldPacket& recvData);
        void HandleAuctionSellItem(WorldPacket& recvData);
        void HandleAuctionRemoveItem(WorldPacket& recvData);
        void HandleAuctionListOwnerItems(WorldPacket& recvData);
        void HandleAuctionPlaceBid(WorldPacket& recvData);
        void HandleAuctionListPendingSales(WorldPacket& recvData);

        void HandleGetMailList(WorldPackets::Mail::MailGetList& packet);
        void HandleSendMail(WorldPackets::Mail::SendMail& packet);
        void HandleMailTakeMoney(WorldPackets::Mail::MailTakeMoney& packet);
        void HandleMailTakeItem(WorldPackets::Mail::MailTakeItem& packet);
        void HandleMailMarkAsRead(WorldPackets::Mail::MailMarkAsRead& packet);
        void HandleMailReturnToSender(WorldPackets::Mail::MailReturnToSender& packet);
        void HandleMailDelete(WorldPackets::Mail::MailDelete& packet);
        void HandleMailCreateTextItem(WorldPackets::Mail::MailCreateTextItem& packet);
        void HandleQueryNextMailTime(WorldPackets::Mail::MailQueryNextMailTime& packet);
        void HandleCancelChanneling(WorldPacket& recvData);

        void SendItemPageInfo(ItemTemplate* itemProto);
        void HandleSplitItemOpcode(WorldPackets::Item::SplitItem& splitItem);
        void HandleSwapInvItemOpcode(WorldPackets::Item::SwapInvItem& swapInvItem);
        void HandleDestroyItemOpcode(WorldPackets::Item::DestroyItem& destroyItem);
        void HandleAutoEquipItemOpcode(WorldPackets::Item::AutoEquipItem& autoEquipItem);
        void HandleSellItemOpcode(WorldPackets::Item::SellItem& packet);
        void HandleBuyItemInSlotOpcode(WorldPacket& recvPacket);
        void HandleBuyItemOpcode(WorldPackets::Item::BuyItem& packet);
        void HandleListInventoryOpcode(WorldPacket& recvPacket);
        void HandleAutoStoreBagItemOpcode(WorldPackets::Item::AutoStoreBagItem& packet);
        void HandleReadItem(WorldPacket& recvPacket);
        void HandleAutoEquipItemSlotOpcode(WorldPacket& recvPacket);
        void HandleSwapItem(WorldPackets::Item::SwapItem& swapItem);
        void HandleBuybackItem(WorldPackets::Item::BuyBackItem& packet);
        void HandleAutoBankItemOpcode(WorldPacket& recvPacket);
        void HandleAutoStoreBankItemOpcode(WorldPacket& recvPacket);
        void HandleWrapItemOpcode(WorldPacket& recvPacket);

        void HandleAttackSwingOpcode(WorldPackets::Combat::AttackSwing& packet);
        void HandleAttackStopOpcode(WorldPackets::Combat::AttackStop& packet);
        void HandleSetSheathedOpcode(WorldPacket& recvPacket);

        void HandleUseItemOpcode(WorldPackets::Spells::ItemUse& recvPacket);
        void HandleOpenItemOpcode(WorldPacket& recvPacket);
        void HandleCastSpellOpcode(WorldPackets::Spells::CastSpell& castRequest);
        void HandleCancelCastOpcode(WorldPacket& recvPacket);
        void HandleCancelAuraOpcode(WorldPacket& recvPacket);
        void HandleCancelGrowthAuraOpcode(WorldPacket& recvPacket);
        void HandleCancelAutoRepeatSpellOpcode(WorldPacket& recvPacket);

        void HandleTalentWipeConfirmOpcode(WorldPacket& recvPacket);
        void HandleUnlearnSkillOpcode(WorldPacket& recvPacket);
        void HandleSetSpecialization(WorldPacket& recvPacket);
        void HandleLearnTalents(WorldPacket& recvPacket);

        void HandleQuestgiverStatusQueryOpcode(WorldPackets::Quest::QuestGiverStatusQuery& packet);
        void HandleQuestgiverStatusMultipleQuery(WorldPackets::Quest::QuestGiverStatusMultipleQuery& packet);
        void HandleQuestgiverHelloOpcode(WorldPackets::Quest::QuestGiverHello& packet);
        void HandleQuestgiverAcceptQuestOpcode(WorldPacket& recvPacket);
        void HandleQuestgiverQueryQuestOpcode(WorldPackets::Quest::QuestGiverQueryQuest& packet);
        void HandleQuestgiverChooseRewardOpcode(WorldPackets::Quest::QuestGiverChooseReward& packet);
        void HandleQuestgiverRequestRewardOpcode(WorldPackets::Quest::QuestGiverRequestReward& packet);
        void HandleQuestQueryOpcode(WorldPackets::Quest::QueryQuestInfo& packet);
        void HandleQuestgiverCancel(WorldPacket& recvData);
        void HandleQuestLogSwapQuest(WorldPacket& recvData);
        void HandleQuestLogRemoveQuest(WorldPacket& recvData);
        void HandleQuestConfirmAccept(WorldPacket& recvData);
        void HandleQuestgiverCompleteQuest(WorldPackets::Quest::QuestGiverCompleteQuest& packet);
        void HandleQuestgiverQuestAutoLaunch(WorldPacket& recvPacket);
        void HandlePushQuestToParty(WorldPacket& recvPacket);
        void HandleQuestPushResult(WorldPacket& recvPacket);

        void SendQuestgiverStatusMultipleQuery();

        bool processChatmessageFurtherAfterSecurityChecks(std::string&, uint32);
        void HandleChatMessageOpcode(WorldPackets::Chat::ChatMessage& packet);
        void HandleChatMessageWhisperOpcode(WorldPackets::Chat::ChatMessageWhisper& packet);
        void HandleChatMessageChannelOpcode(WorldPackets::Chat::ChatMessageChannel& packet);
        void HandleChatMessageEmoteOpcode(WorldPackets::Chat::ChatMessageEmote& packet);
        void HandleChatMessage(ChatMsg type, uint32 lang, std::string msg, std::string target = "");
        void HandleChatAddonMessageOpcode(WorldPackets::Chat::ChatAddonMessage& packet);
        void HandleChatAddonMessageWhisperOpcode(WorldPackets::Chat::ChatAddonMessageWhisper& packet);
        void HandleChatAddonMessage(ChatMsg type, std::string prefix, std::string text, std::string target = "");

        void SendPlayerNotFoundNotice(std::string name);
        void SendPlayerAmbiguousNotice(std::string name);
        void SendChatRestrictedNotice(ChatRestrictionType restriction);
        void HandleTextEmoteOpcode(WorldPackets::Chat::CTextEmote& packet);
        void HandleChatIgnoredOpcode(WorldPacket& recvPacket);

        void HandleUnregisterAddonPrefixesOpcode(WorldPacket& recvPacket);
        void HandleAddonRegisteredPrefixesOpcode(WorldPacket& recvPacket);

        void HandleReclaimCorpse(WorldPackets::Misc::ReclaimCorpse& packet);
        void HandleQueryCorpseLocation(WorldPackets::Query::QueryCorpseLocationFromClient& packet);
        void HandleQueryCorpseTransport(WorldPackets::Query::QueryCorpseTransport& packet);
        void HandleResurrectResponse(WorldPackets::Misc::ResurrectResponse& packet);
        void HandleSummonResponseOpcode(WorldPacket& recvData);

        void HandleJoinChannel(WorldPackets::Channel::JoinChannel& packet);
        void HandleLeaveChannel(WorldPackets::Channel::LeaveChannel& packet);
        void HandleChannelList(WorldPacket& recvPacket);
        void HandleChannelPassword(WorldPacket& recvPacket);
        void HandleChannelSetOwner(WorldPacket& recvPacket);
        void HandleChannelOwner(WorldPacket& recvPacket);
        void HandleChannelModerator(WorldPacket& recvPacket);
        void HandleChannelUnmoderator(WorldPacket& recvPacket);
        void HandleChannelMute(WorldPacket& recvPacket);
        void HandleChannelUnmute(WorldPacket& recvPacket);
        void HandleChannelInvite(WorldPacket& recvPacket);
        void HandleChannelKick(WorldPacket& recvPacket);
        void HandleChannelBan(WorldPacket& recvPacket);
        void HandleChannelUnban(WorldPacket& recvPacket);
        void HandleChannelAnnouncements(WorldPacket& recvPacket);
        void HandleChannelModerate(WorldPacket& recvPacket);
        void HandleChannelDeclineInvite(WorldPacket& recvPacket);
        void HandleChannelDisplayListQuery(WorldPacket& recvPacket);
        void HandleSetChannelWatch(WorldPacket& recvPacket);

        void HandleCompleteCinematic(WorldPacket& recvPacket);
        void HandleNextCinematicCamera(WorldPacket& recvPacket);
        void HandleCompleteMovie(WorldPacket& recvPacket);

        void HandlePageTextQueryOpcode(WorldPackets::Query::QueryPageText& packet);

        void HandleTutorialFlag(WorldPackets::Misc::TutorialSetFlag& packet);

        //Pet
        void HandlePetAction(WorldPacket& recvData);
        void HandlePetStopAttack(WorldPacket& recvData);
        void HandlePetActionHelper(Unit* pet, ObjectGuid guid1, uint32 spellid, uint16 flag, ObjectGuid guid2, float x, float y, float z);
        void HandlePetNameQuery(WorldPacket& recvData);
        void HandlePetSetAction(WorldPacket& recvData);
        void HandlePetAbandon(WorldPacket& recvData);
        void HandlePetRename(WorldPacket& recvData);
        void HandlePetCancelAuraOpcode(WorldPacket& recvPacket);
        void HandlePetSpellAutocastOpcode(WorldPacket& recvPacket);
        void HandlePetCastSpellOpcode(WorldPackets::Spells::PetCastSpell& recvPacket);

        void HandleSetActionBarToggles(WorldPacket& recvData);

        void HandleCharRenameOpcode(WorldPacket& recvData);
        void HandleChangePlayerNameOpcodeCallBack(PreparedQueryResult result, std::string newName);
        void HandleSetPlayerDeclinedNames(WorldPacket& recvData);

        void HandleTotemDestroyed(WorldPacket& recvData);
        void HandleDismissCritter(WorldPacket& recvData);

        //Battleground
        void HandleBattlemasterHelloOpcode(WorldPacket& recvData);
        void HandleBattlemasterJoinOpcode(WorldPacket& recvData);
        void HandlePVPLogDataOpcode(WorldPacket& recvData);
        void HandleBattleFieldPortOpcode(WorldPacket& recvData);
        void HandleBattlefieldListOpcode(WorldPacket& recvData);
        void HandleLeaveBattlefieldOpcode(WorldPacket& recvData);
        void HandleBattlemasterJoinArena(WorldPacket& recvData);
        void HandleBattlemasterJoinRated(WorldPacket& recvData);
        void JoinBracket(uint8 slot);

        void HandleReportPvPAFK(WorldPacket& recvData);
        void HandleRequestRatedInfo(WorldPacket & recvData);
        void HandleRequestPvpOptions(WorldPacket& recvData);
        void HandleRequestPvpReward(WorldPacket& recvData);
        void HandleRequestRatedBgStats(WorldPacket& recvData);

        void HandleWardenDataOpcode(WorldPacket& recvData);
        void HandleWorldTeleportOpcode(WorldPacket& recvData);
        void HandleMinimapPingOpcode(WorldPacket& recvData);
        void HandleRandomRollOpcode(WorldPacket& recvData);
        void HandleFarSightOpcode(WorldPacket& recvData);
        void HandleSetDungeonDifficultyOpcode(WorldPacket& recvData);
        void HandleSetRaidDifficultyOpcode(WorldPacket& recvData);
        void HandleMoveSetCanFlyAckOpcode(WorldPacket& recvData);
        void HandleSetTitleOpcode(WorldPacket& recvData);
        void HandleRealmSplitOpcode(WorldPacket& recvData);
        void HandleRealmQueryNameOpcode(WorldPacket& recvData);
        void HandleTimeSyncResp(WorldPackets::Misc::TimeSyncResponse& packet);
        void HandleWhoisOpcode(WorldPackets::Who::WhoIsRequest& packet);
        void HandleResetInstancesOpcode(WorldPacket& recvData);
        void HandleHearthAndResurrect(WorldPacket& recvData);
        void HandleInstanceLockResponse(WorldPacket& recvPacket);
        void HandlePersonalRatedInfoRequest(WorldPacket& recvPacket);

        // Battlefield
        void SendBfInvitePlayerToWar(uint64 const& guid,uint32 ZoneId,uint32 time);
        void SendBfInvitePlayerToQueue(uint64 const& guid);
        void SendBfQueueInviteResponse(uint64 const& guid,uint32 ZoneId, bool CanQueue = true, bool Full = false);
        void SendBfEntered(uint64 const& guid);
        void SendBfLeaveMessage(uint64 const& guid, BFLeaveReason reason = BF_LEAVE_REASON_EXITED);
        void HandleBfQueueInviteResponse(WorldPacket &recv_data);
        void HandleBfEntryInviteResponse(WorldPacket &recv_data);
        void HandleBfExitQueueRequest(WorldPacket &recv_data);
        void HandleBfSetPreferedCemetry(WorldPacket & recvData);
        void HandleBfExitRequest(WorldPacket &recv_data);
        void HandleBfQueueRequest(WorldPacket &recv_data);

        // Looking for Dungeon/Raid
        void HandleLfgSetCommentOpcode(WorldPacket& recvData);
        void HandleLfgPlayerLockInfoRequestOpcode(WorldPacket& recvData);
        void HandleLfgPartyLockInfoRequestOpcode(WorldPacket& recvData);
        void HandleLfgJoinOpcode(WorldPacket& recvData);
        void HandleLfgLeaveOpcode(WorldPacket& recvData);
        void HandleLfgSetRolesOpcode(WorldPacket& recvData);
        void HandleLfgProposalResultOpcode(WorldPacket& recvData);
        void HandleLfgSetBootVoteOpcode(WorldPacket& recvData);
        void HandleLfgTeleportOpcode(WorldPacket& recvData);
        void HandleLfrJoinOpcode(WorldPacket& recvData);
        void HandleLfrLeaveOpcode(WorldPacket& recvData);
        void HandleLfgGetStatus(WorldPacket& recvData);

        void SendLfgUpdatePlayer(lfg::LfgUpdateData const& updateData);
        void SendLfgUpdateParty(lfg::LfgUpdateData const& updateData);
        void SendLfgRoleChosen(ObjectGuid guid, uint8 roles);
        void SendLfgRoleCheckUpdate(lfg::LfgRoleCheck const& pRoleCheck);
        void SendLfgLfrList(bool update);
        void SendLfgJoinResult(lfg::LfgJoinResultData const& joinData);
        void SendLfgQueueStatus(lfg::LfgQueueStatusData const& queueData);
        void SendLfgPlayerReward(lfg::LfgPlayerRewardData const& lfgPlayerRewardData);
        void SendLfgBootProposalUpdate(lfg::LfgPlayerBoot const& boot);
        void SendLfgUpdateProposal(lfg::LfgProposal const& proposal);
        void SendLfgDisabled();
        void SendLfgOfferContinue(uint32 dungeonEntry);
        void SendLfgTeleportError(uint8 err);

        // Arena Team
        void HandleAreaSpiritHealerQueryOpcode(WorldPacket& recvData);
        void HandleAreaSpiritHealerQueueOpcode(WorldPacket& recvData);
        void HandleCancelMountAuraOpcode(WorldPacket& recvData);
        void HandleSelfResOpcode(WorldPacket& recvData);
        void HandleComplainOpcode(WorldPacket& recvData);
        void HandleRequestPetInfoOpcode(WorldPacket& recvData);

        // Socket gem
        void HandleSocketOpcode(WorldPacket& recvData);

        void HandleCancelTempEnchantmentOpcode(WorldPacket& recvData);

        void HandleGetItemPurchaseData(WorldPackets::Item::ItemRefundInfo& packet);
        void HandleItemRefund(WorldPacket& recvData);

        void HandleChannelVoiceOnOpcode(WorldPacket& recvData);
        void HandleVoiceSessionEnableOpcode(WorldPacket& recvData);
        void HandleSetActiveVoiceChannel(WorldPacket& recvData);
        void HandleSetTaxiBenchmarkOpcode(WorldPacket& recvData);

        // Guild Bank
        void HandleGuildPermissions(WorldPacket& recvData);
        void HandleGuildBankMoneyWithdrawn(WorldPacket& recvData);
        void HandleGuildBankerActivate(WorldPacket& recvData);
        void HandleGuildBankQueryTab(WorldPacket& recvData);
        void HandleGuildBankLogQuery(WorldPacket& recvData);
        void HandleGuildBankDepositMoney(WorldPacket& recvData);
        void HandleGuildBankWithdrawMoney(WorldPacket& recvData);
        void HandleGuildBankSwapItems(WorldPacket& recvData);

        void HandleGuildBankUpdateTab(WorldPacket& recvData);
        void HandleGuildBankBuyTab(WorldPacket& recvData);
        void HandleQueryGuildBankTabText(WorldPacket& recvData);
        void HandleSetGuildBankTabText(WorldPacket& recvData);
        void HandleGuildQueryXPOpcode(WorldPacket& recvData);

        // Refer-a-Friend
        void HandleGrantLevel(WorldPacket& recvData);
        void HandleAcceptGrantLevel(WorldPacket& recvData);

        // Calendar
        void HandleCalendarGetCalendar(WorldPacket& recvData);
        void HandleCalendarGetEvent(WorldPacket& recvData);
        void HandleCalendarGuildFilter(WorldPacket& recvData);
        void HandleCalendarArenaTeam(WorldPacket& recvData);
        void HandleCalendarAddEvent(WorldPacket& recvData);
        void HandleCalendarUpdateEvent(WorldPacket& recvData);
        void HandleCalendarRemoveEvent(WorldPacket& recvData);
        void HandleCalendarCopyEvent(WorldPacket& recvData);
        void HandleCalendarEventInvite(WorldPacket& recvData);
        void HandleCalendarEventRsvp(WorldPacket& recvData);
        void HandleCalendarEventRemoveInvite(WorldPacket& recvData);
        void HandleCalendarEventStatus(WorldPacket& recvData);
        void HandleCalendarEventModeratorStatus(WorldPacket& recvData);
        void HandleCalendarComplain(WorldPacket& recvData);
        void HandleCalendarGetNumPending(WorldPacket& recvData);
        void HandleCalendarEventSignup(WorldPacket& recvData);

        void SendCalendarEvent(CalendarEvent const& calendarEvent, CalendarSendEventType sendEventType);
        void SendCalendarEventInvite(CalendarInvite const& invite, bool pending);
        void SendCalendarEventInviteAlert(CalendarEvent const& calendarEvent, CalendarInvite const& calendarInvite);
        void SendCalendarEventInviteRemove(CalendarInvite const& invite, uint32 flags);
        void SendCalendarEventInviteRemoveAlert(CalendarEvent const& calendarEvent, CalendarInviteStatus status);
        void SendCalendarEventRemovedAlert(CalendarEvent const& calendarEvent);
        void SendCalendarEventUpdateAlert(CalendarEvent const& calendarEvent, CalendarSendEventType sendEventType);
        void SendCalendarEventStatus(CalendarEvent const& calendarEvent, CalendarInvite const& invite);
        void SendCalendarEventModeratorStatusAlert(CalendarInvite const& invite);
        void SendCalendarClearPendingAction();
        void SendCalendarRaidLockoutAdded(InstanceSave* save);
        void SendCalendarRaidLockoutRemoved(InstanceSave* save);
        void SendCalendarRaidLockoutUpdated(InstanceSave* save);
        void SendCalendarCommandResult(CalendarError err, char const* param = NULL);

        // Void Storage
        void HandleVoidStorageUnlock(WorldPacket& recvData);
        void HandleVoidStorageQuery(WorldPacket& recvData);
        void HandleVoidStorageTransfer(WorldPacket& recvData);
        void HandleVoidSwapItem(WorldPacket& recvData);
        void SendVoidStorageTransferResult(VoidTransferError result);
        void SendVoidStorageFailed(bool unk = false);

        // Transmogrification
        void HandleTransmogrifyItems(WorldPackets::Item::TransmogrigyItem& request);

        // Upgrade item
        void HandleUpgradeItem(WorldPacket& recvData);

        // Battle Pets
        void HandleBattlePetSetFlags(WorldPacket& recvData);
        void HandleBattlePetModifyName(WorldPacket& recvData);
        void HandleBattlePetNameQuery(WorldPacket& recvData);
        void HandleCageBattlePet(WorldPacket& recvData);
        void HandleBattlePetSetSlot(WorldPacket& recvData);
        void HandleBattlePetSummon(WorldPacket& recvData);
        void HandlePetBattleRequestWild(WorldPacket& recvData);
        void HandlePetBattleRequestUpdate(WorldPacket& recvData);
        void HandlePetBattleInputFirstPet(WorldPacket& recvData);
        void HandlePetBattleInput(WorldPacket& recvData);
        void HandlePetBattleFinalNotify(WorldPacket& recvData);
        void HandlePetBattleQuitNotify(WorldPacket& recvData);
        void HandleBattlePetDelete(WorldPacket& recvData);

        // Blizzard Store
        void HandlePurchaseGetProductList(WorldPacket& recvData);
        void HandlePurchaseGetPurchaseList(WorldPacket& recvData);

        // Miscellaneous
        void HandleSpellClick(WorldPacket& recvData);
        void HandleMirrorImageDataRequest(WorldPacket& recvData);
        void HandleAlterAppearance(WorldPacket& recvData);
        void HandleRemoveGlyph(WorldPacket& recvData);
        void HandleCharCustomize(WorldPacket& recvData);
        void HandleCharUndelete(WorldPacket& recvData);
        void HandleQueryInspectAchievements(WorldPacket& recvData);
        void HandleGuildAchievementProgressQuery(WorldPacket& recvData);
        void HandleEquipmentSetSave(WorldPacket& recvData);
        void HandleEquipmentSetDelete(WorldPacket& recvData);
        void HandleEquipmentSetUse(WorldPacket& recvData);
        void HandleUITimeRequest(WorldPackets::Misc::UITimeRequest& /*request*/);
        void HandleQuestNpcQuery(WorldPacket& recvData);
        void HandleQuestPOIQuery(WorldPackets::Query::QuestPOIQuery& packet);
        void HandleEjectPassenger(WorldPacket& data);
        void HandleEnterPlayerVehicle(WorldPacket& data);
        void HandleSetVehicleRecId(WorldPacket& data);
        void HandleUpdateProjectilePosition(WorldPacket& recvPacket);
        void HandleDBQueryBulk(WorldPackets::Query::DBQueryBulk& packet);
        void HandleUpdateMissileTrajectory(WorldPacket& recvPacket);
        void HandleViolenceLevel(WorldPacket& recvPacket);
        void HandleObjectUpdateFailedOpcode(WorldPacket& recvPacket);
        void HandleSetFactionOpcode(WorldPacket& recvPacket);
        int32 HandleEnableNagleAlgorithm();
        void HandleRequestResearchHistory(WorldPacket& recvPacket);
        void HandleChangeCurrencyFlags(WorldPacket& recvPacket);
        void HandlerCategoryCooldownOpocde(WorldPacket& recvPacket);
        void HandleClearRaidMarkerOpcode(WorldPacket& recvPacket);
        void HandleQueryPlayerRecipes(WorldPacket& recvPacket);
        void HandleForcedReactions(WorldPacket& recvPacket);
        void HandleSaveCUFProfiles(WorldPacket& recvPacket);
        void SendLoadCUFProfiles();
        void HandleBattlePayDistributionAssign(WorldPackets::BattlePay::DistributionAssignToTarget& packet);
        void HandleBattlePayStartPurchase(WorldPackets::BattlePay::StartPurchase& packet);
        void HandleBattlePayConfirmPurchase(WorldPackets::BattlePay::ConfirmPurchaseResponse& packet);

        // Scenarios
        void HandleScenarioPOIQuery(WorldPacket& recvPacket);

        // Challenge Mode
        void HandleChallengeModeRequestRewardInfoOpcode(WorldPacket& recvPacket);
        void HandleChallengeModeRequestCompletionInfoOpcode(WorldPacket& recvPacket);
        void HandleChallengeModeRequestOpcode(WorldPacket& recvPacket);

        // WarGames
        void HandleWarGameStart(WorldPacket& recvPacket);
        void HandleWarGameAccept(WorldPacket& recvPacket);

        // Scene
        void HandleSceneComplete(WorldPacket& recvPacket);
        void HandleTrigerSceneEvent(WorldPacket& recvPacket);

        //
        void SuspendTokenResponse(WorldPacket& recvPacket);
        void HandleUndeleteCharacterCooldownnStatus(WorldPacket& recvPacket);
        void HandleWowTokenMarketPrice(WorldPacket& recvPacket);

        void LootCorps(ObjectGuid corpsGUID, WorldObject* lootedBy = NULL);

        // Battle Pay
        AuthFlags GetAF() const { return atAuthFlag;  }
        bool HasAuthFlag(AuthFlags f) const { return atAuthFlag & f; }
        void AddAuthFlag(AuthFlags f);
        void RemoveAuthFlag(AuthFlags f);
        void SaveAuthFlag();

        void SendCharacterEnum(bool deleted = false);
        bool skip_send_packer = false;

        void SetBankerGuid(ObjectGuid const& g) { m_currentBankerGUID = g; }
    private:
        void InitializeQueryCallbackParameters();
        void ProcessQueryCallbacks();

        QueryCallback<PreparedQueryResult, bool> _charEnumCallback;
        PreparedQueryResultFuture _addIgnoreCallback;
        PreparedQueryResultFuture _stablePetCallback;
        QueryCallback<PreparedQueryResult, std::string> _charRenameCallback;
        QueryCallback<PreparedQueryResult, std::string> _addFriendCallback;
        QueryCallback<PreparedQueryResult, ObjectGuid> _sendStabledPetCallback;
        QueryCallback<PreparedQueryResult, uint32> _stableChangeSlotCallback;
        QueryCallback<PreparedQueryResult, std::shared_ptr<WorldPackets::Character::CharacterCreateInfo>, true> _charCreateCallback;
        QueryResultHolderFuture _charLoginCallback;

    private:
        // private trade methods
        void moveItems(Item* myItems[], Item* hisItems[]);

        bool CanUseBank(ObjectGuid bankerGUID = ObjectGuid::Empty) const;

        // logging helper
        void LogUnexpectedOpcode(WorldPacket* packet, const char* status, const char *reason);
        void LogUnprocessedTail(WorldPacket* packet);

        // EnumData helpers
        bool CharCanLogin(ObjectGuid::LowType lowGUID)
        {
            return _allowedCharsToLogin.find(lowGUID) != _allowedCharsToLogin.end();
        }

        // this stores the GUIDs of the characters who can login
        // characters who failed on Player::BuildEnumData shouldn't login
        std::set<ObjectGuid::LowType> _allowedCharsToLogin;

        ObjectGuid::LowType m_GUIDLow;                                   // set loggined or recently logout player (while m_playerRecentlyLogout set)
        Player* _player;
        std::shared_ptr<WorldSocket> m_Socket;
        std::string m_Address;

        AccountTypes _security;
        uint32 _accountId;
        uint32 _battlenetAccountId;
        uint8 m_expansion;

        typedef std::list<AddonInfo> AddonsList;

        // Warden
        Warden* _warden;                                    // Remains NULL if Warden system is not enabled by config

        // BattlePay
        BattlePayMgr* _battlePay;

        time_t _logoutTime;
        bool m_inQueue;                                     // session wait in auth.queue
        bool m_playerLoading;                               // code processed in LoginPlayer
        bool m_playerLogout;                                // code processed in LogoutPlayer
        bool m_playerRecentlyLogout;
        bool m_playerSave;
        LocaleConstant m_sessionDbcLocale;
        LocaleConstant m_sessionDbLocaleIndex;
        std::atomic<uint32> m_latency;
        std::atomic<uint32> m_clientTimeDelay;
        AccountData m_accountData[NUM_ACCOUNT_DATA_TYPES];
        uint32 m_Tutorials[MAX_ACCOUNT_TUTORIAL_VALUES];
        bool   m_TutorialsChanged;
        AddonsList m_addonsList;
        std::vector<std::string> _registeredAddonPrefixes;
        bool _filterAddonMessages;
        uint32 recruiterId;
        bool isRecruiter;
        LockedQueue<WorldPacket*> _recvQueue;
        time_t timeLastWhoCommand;
        time_t timeCharEnumOpcode;
        time_t timeLastChannelInviteCommand;
        time_t timeLastChannelPassCommand;
        time_t timeLastChannelMuteCommand;
        time_t timeLastChannelBanCommand;
        time_t timeLastChannelUnbanCommand;
        time_t timeLastChannelAnnounceCommand;
        time_t timeLastGroupInviteCommand;
        time_t timeLastGuildInviteCommand;
        time_t timeLastChannelModerCommand;
        time_t timeLastChannelOwnerCommand;
        time_t timeLastChannelSetownerCommand;
        time_t timeLastChannelUnmoderCommand;
        time_t timeLastChannelUnmuteCommand;
        time_t timeLastChannelKickCommand;
        time_t timeLastHandleSendMail;
        time_t timeLastHandleSellItem;
        time_t timeLastHandlePlayerLogin;
        time_t timeAddIgnoreOpcode;
        time_t timeMoveTeleportAck;
        time_t timeLastHandleSpellClick;
        uint8 playerLoginCounter;

        uint32 _pakagepersecond;
        uint32 _second;
        uint32 _counttokick;

        ObjectGuid m_currentBankerGUID;

        AuthFlags atAuthFlag = AT_AUTH_FLAG_NONE;
};

class PacketSendEvent : public BasicEvent
{
    Player* m_owner;
    WorldPacket* m_packet;
    uint32 m_delay;
public:
    explicit PacketSendEvent(Player* owner, WorldPacket* data, uint32 delay) : m_owner(owner), m_packet(data), m_delay(delay) { }
    explicit PacketSendEvent(Player* me, WorldPacket& data, uint32 delay);

    ~PacketSendEvent();

    virtual bool Execute(uint64 , uint32);

    void Schedule();
};

#endif
/// @}
