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
#include "Packet.h"

class BlackMarketEntry;
class CollectionMgr;
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
struct BlackMarketTemplate;
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
        class GetUndeleteCharacterCooldownStatus;
        class SetActionBarToggles;
        class RequestPlayedTime;
        class ShowingCloak;
        class ShowingHelm;
        class SetTitle;
        class AlterApperance;
        class SetFactionAtWar;
        class SetFactionNotAtWar;
        class SetFactionInactive;
        class SetWatchedFaction;
        class EmoteClient;


        enum class LoginFailureReason : uint8;
    }

    namespace ClientConfig
    {
        class RequestAccountData;
        class UserClientUpdateAccountData;
        class SetAdvancedCombatLogging;
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
        class SetSheathed;
    }

    namespace Duel
    {
        class DuelResponse;
        class CanDuel;
    }

    namespace EquipmentSet
    {
        class SaveEquipmentSet;
        class UseEquipmentSet;
        class DeleteEquipmentSet;
    }

    namespace GameObject
    {
        class GameObjectReportUse;
        class GameObjectUse;
    }

    namespace Guild
    {
        class QueryGuildInfo;
        class GuildInviteByName;
        class GuildOfficerRemoveMember;
        class AcceptGuildInvite;
        class GuildDeclineInvitation;
        class GuildEventLogQuery;
        class GuildGetRoster;
        class GuildPromoteMember;
        class GuildDemoteMember;
        class GuildAssignMemberRank;
        class GuildLeave;
        class GuildDelete;
        class GuildSetGuildMaster;
        class GuildUpdateMotdText;
        class GuildNewsUpdateSticky;
        class GuildSetMemberNote;
        class GuildGetRanks;
        class GuildQueryNews;
        class GuildShiftRank;
        class GuildSetRankPermissions;
        class GuildAddRank;
        class GuildDeleteRank;
        class GuildUpdateInfoText;
        class RequestGuildPartyState;
        class AutoDeclineGuildInvites;
        class RequestGuildRewardsList;

        class GuildPermissionsQuery;
        class GuildBankRemainingWithdrawMoneyQuery;
        class GuildBankActivate;
        class GuildBankQueryTab;
        class GuildBankLogQuery;
        class GuildBankDepositMoney;
        class GuildBankWithdrawMoney;
        class GuildBankSwapItems;
        class GuildBankUpdateTab;
        class GuildBankBuyTab;
        class GuildBankTextQuery;
        class GuildBankSetTabText;

        class LFGuildSetGuildPost;

        class GuildChallengeUpdateRequest;
    }

    namespace Item
    {
        struct ItemInstance;
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
        class UseCritterItem;
    }

    namespace Loot
    {
        class LootUnit;
        class AutoStoreLootItem;
        class LootRelease;
        class LootMoney;
        class SetLootSpecialization;
        class LootRoll;
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
        class ViolenceLevel;
        class RandomRollClient;
        class ObjectUpdateFailed;
        class ObjectUpdateRescued;
        class CompleteCinematic;
        class NextCinematicCamera;
        class FarSight;
        class OpeningCinematic;
        class TogglePvP;
        class SetPvP;
        class WorldTeleport;
        class ConfirmRespecWipe;
        class ShowTradeSkill;
    }

    namespace Movement
    {
        class ClientPlayerMovement;
        class WorldPortAck;
        class MoveTeleportAck;
        class MovementSpeedAck;
        class MovementAckMessage;
        class MoveSetCollisionHeightAck;
        class SummonResponse;
        class MoveTimeSkipped;
        class SetActiveMover;
        class MoveSplineDone;
    }

    namespace NPC
    {
        class Hello;
        class GossipSelectOption;
        class SpiritHealerActivate;
        class TrainerBuySpell;
    }

    namespace BattlePay
    {
        class DistributionAssignToTarget;
        class StartPurchase;
        class ConfirmPurchaseResponse;
        class GetProductList;
        class GetPurchaseListQuery;
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
        class ItemTextQuery;
        class QueryPetName;
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
        class QuestPushResult;
        class QuestConfirmAccept;
        class QuestLogRemoveQuest;
        class QuestGiverAcceptQuest;
    }

    namespace Social
    {
        class AddFriend;
        class AddIgnore;
        class DelFriend;
        class DelIgnore;
        class SendContactList;
        class SetContactNotes;
        class VoiceDelIgnore;
        class VoiceAddIgnore;
    }

    namespace Spells
    {
        class CancelAura;
        class CastSpell;
        class SetActionButton;
        class PetCastSpell;
        class ItemUse;
        class SpellClick;
        class GetMirrorImageData;
        class SelfRes;
        class UnlearnSkill;
        class OpenItem;
        class CancelCast;
        class RequestCategoryCooldowns;
        class CancelMountAura;
        class CancelGrowthAura;
    }

    namespace Talent
    {
        class SetSpecialization;
        class LearnTalent;
    }

    namespace Trade
    {
        class NullCmsg;
        class AcceptTrade;
        class ClearTradeItem;
        class InitiateTrade;
        class SetTradeGold;
        class SetTradeItem;
        class SetTradeCurrency;
        class TradeStatus;
    }

    namespace Who
    {
        class WhoIsRequest;
        class WhoRequestPkt;
    }

    namespace Battleground
    {
        class ListClient;
        class NullCmsg;
        class EntryOrQueueInviteResponse;
        class ExitRequest;
        class Join;
        class JoinArena;
        class AreaSpiritHealerQuery;
        class AreaSpiritHealerQueue;
        class Port;
        class QueueRequest;
        class JoinSkirmish;
    }

    namespace VoidStorage
    {
        class UnlockVoidStorage;
        class VoidStorageTransfer;
        class QueryVoidStorage;
        class SwapVoidItem;
    }

    namespace AuctionHouse
    {
        class AuctionHelloRequest;
        class AuctionListBidderItems;
        class AuctionListItems;
        class AuctionListOwnerItems;
        class AuctionListPendingSales;
        class AuctionPlaceBid;
        class AuctionRemoveItem;
        class AuctionReplicateItems;
        class AuctionSellItem;
    }

    namespace Inspect
    {
        class Inspect;
        class InspectPVPRequest;
        class QueryInspectAchievements;
        class RequestHonorStats;
    }

    namespace Vehicle
    {
        class MoveDismissVehicle;
        class RequestVehiclePrevSeat;
        class RequestVehicleNextSeat;
        class MoveChangeVehicleSeats;
        class RequestVehicleSwitchSeat;
        class RideVehicleInteract;
        class EjectPassenger;
        class RequestVehicleExit;
        class MoveSetVehicleRecIdAck;
    }

    namespace Instance
    {
        class ResetInstances;
        class InstanceLockResponse;
    }

    namespace Toy
    {
        class AccountToysUpdate;
        class AddToy;
        class ToySetFavorite;
        class UseToy;
    }

    namespace Achievement
    {
        class GuildSetFocusedAchievement;
    }

    namespace Scene
    {
        class QueryScenarioPOI;
        class SceneTriggerEvent;
        class SceneInstance;
    }

    namespace Calendar
    {
        class CalendarAddEvent;
        class CalendarCopyEvent;
        class CalendarEventInvite;
        class CalendarEventModeratorStatus;
        class CalendarEventRSVP;
        class CalendarEventSignUp;
        class CalendarEventStatus;
        class CalendarGetCalendar;
        class CalendarGetEvent;
        class CalendarGetNumPending;
        class CalendarGuildFilter;
        class CalendarRemoveEvent;
        class CalendarRemoveInvite;
        class CalendarUpdateEvent;
        class SetSavedInstanceExtend;
    }

    namespace Retupation
    {
        class RequestForcedReactions;
    }

    namespace ReferAFriend
    {
        class AcceptLevelGrant;
        class GrantLevel;
    }

    namespace Auth
    {
        enum class ConnectToSerial : uint32;
    }

    namespace Bank
    {
        class AutoBankItem;
        class AutoStoreBankItem;
        class BuyBankSlot;
    }

    namespace Reputation
    {
        class RequestForcedReactions;
    }

    namespace BlackMarket
    {
        class BlackMarketOpen;
        class BlackMarketRequestItems;
        class BlackMarketBidOnItem;
        class BlackMarketOutbid;
    }

    namespace ChallengeMode
    {
        class Misc;
        class RequestLeaders;
    }

    namespace Totem
    {
        class TotemDestroyed;
    }

    namespace Party
    {
        class PartyInviteClient;
        class PartyInviteResponse;
        class PartyUninvite;
        class RequestPartyMemberStats;
        class SetPartyLeader;
        class SetRole;
        class LeaveGroup;
        class SetLootMethod;
        class MinimapPingClient;
        class UpdateRaidTarget;
        class ConvertRaid;
        class RequestPartyJoinUpdates;
        class SetAssistantLeader;
        class DoReadyCheck;
        class ReadyCheckResponseClient;
        class RequestRaidInfo;
        class OptOutOfLoot;
        class InitiateRolePoll;
        class SetEveryoneIsAssistant;
        class ChangeSubGroup;
        class SwapSubGroups;
        class ClearRaidMarker;
    }

    namespace Petition
    {
        class PetitionRenameGuild;
        class OfferPetition;
        class TurnInPetition;
        class DeclinePetition;
        class SignPetition;
        class PetitionShowSignatures;
        class PetitionBuy;
        class PetitionShowList;
        class QueryPetition;
    }

    namespace PetPackets
    {
        class StopAttack;
        class DismissCritter;
        class LearnPetSpecializationGroup;
        class SetPetSpecialization;
        class Action;
        class NameQuery;
        class SetAction;
        class PetCancelAura;
    }

    namespace LFG
    {
        class LockInfoRequest;
        class DFJoin;
        class DFBootPlayerVote;
        class DFProposalResponse;
        class ListInviteResponse;
        class DFSetComment;
        class DFTeleport;
        class DFSetRoles;
        class DFLeave;
        class ListJoin;
        class ListLeave;
        class BonusFactionID;
        class NullCmsg;
        class DFSearchJoin;
        class DFSearchLeave;
    }

    namespace BattlePet
    {
        class NullCmsg;
        class Query;
        class BattlePetGuidRead;
        class ModifyName;
        class SetBattleSlot;
        class SetFlags;
        class RequestWild;
        class RequestPVP;
        class ReplaceFrontPet;
        class QueueProposeMatchResult;
        class LeaveQueue;
        class RequestUpdate;
        class PetBattleInput;
    }

    namespace Garrison
    {
        class GetGarrisonInfo;
        class GarrisonPurchaseBuilding;
        class GarrisonCancelConstruction;
        class GarrisonRequestBlueprintAndSpecializationData;
        class GarrisonGetBuildingLandmarks;
        class GarrisonMissionBonusRoll;
        class GarrisonRequestLandingPageShipmentInfo;
        class GarrisonCheckUpgradeable;
        class GarrisonStartMission;
        class GarrisonCompleteMission;
        class CreateShipment;
        class GarrisonOpenMissionNpc;
        class CompleteAllReadyShipments;
        class UpgradeGarrison;
        class TrophyData;
        class RevertTrophy;
        class GetTrophyList;
        class GarrisonSetFollowerInactive;
        class GarrisonRemoveFollowerFromBuilding;
        class GarrisonAssignFollowerToBuilding;
    }
    
    namespace Taxi
    {
        class ShowTaxiNodes;
        class TaxiNodeStatusQuery;
        class EnableTaxiNode;
        class TaxiQueryAvailableNodes;
        class ActivateTaxi;
        class TaxiRequestEarlyLanding;
    }

    class Null final : public ClientPacket
    {
    public:
        Null(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

        void Read() override { _worldPacket.rfinish(); }
    };
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

enum BarberShopResult
{
    BARBER_SHOP_RESULT_SUCCESS      = 0,
    BARBER_SHOP_RESULT_NO_MONEY     = 1,
    BARBER_SHOP_RESULT_NOT_ON_CHAIR = 2,
    BARBER_SHOP_RESULT_NO_MONEY_2   = 3
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
    BF_LEAVE_REASON_CLOSE               = 1,
    BF_LEAVE_REASON_EXITED              = 8,
    BF_LEAVE_REASON_LOW_LEVEL           = 10,
    BF_LEAVE_REASON_NOT_WHILE_IN_RAID   = 15,
    BF_LEAVE_REASON_DESERTER            = 16
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

        bool PlayerLoading() const { return !m_playerLoading.IsEmpty(); }
        bool PlayerLogout() const { return m_playerLogout; }
        bool PlayerLogoutWithSave() const { return m_playerLogout && m_playerSave; }
        bool PlayerRecentlyLoggedOut() const { return m_playerRecentlyLogout; }

        void ReadAddonsInfo(ByteBuffer& data);
        void SendAddonsInfo();
        bool IsAddonRegistered(const std::string& prefix) const;

        void SendPacket(WorldPacket const* packet, bool forced = false);
        void SendNotification(const char *format, ...) ATTR_PRINTF(2, 3);
        void SendNotification(uint32 string_id, ...);
        void SendPetNameInvalid(uint32 error, ObjectGuid const& guid, const std::string& name, DeclinedName *declinedName);
        void SendPartyResult(PartyOperation operation, const std::string& member, PartyResult res, uint32 val = 0);
        void SendSetPhaseShift(std::set<uint32> const& phaseIds, std::set<uint32> const& visibleMapIDs, std::set<uint32> const& uiWorldMapAreaIDSwaps, std::set<uint32> const& preloadMapIDs, uint32 flags = 0x1F);
        void SendQueryTimeResponse();
        void HandleLearnPetSpecializationGroup(WorldPackets::PetPackets::LearnPetSpecializationGroup& packet);

        void SendAuthResponse(uint8 code, bool hasAccountData = true, bool queued = false, uint32 queuePos = 0);
        void SendClientCacheVersion(uint32 version);
        void HandleGetPurchaseListQuery(WorldPackets::BattlePay::GetPurchaseListQuery& packet);
        void HandleGetProductList(WorldPackets::BattlePay::GetProductList& packet);
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

        void InitializeWarden(BigNumber* k, std::string os);

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

        void QueuePacket(WorldPacket* new_packet, bool& deletePacket);
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

        void SendTradeStatus(WorldPackets::Trade::TradeStatus& packet);
        void SendUpdateTrade(bool trader_data = true);
        void SendCancelTrade();

        void SendPetitionQueryOpcode(ObjectGuid petitionguid);

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
        void SendAuctionOutBidNotification(AuctionEntry const* auction, Item const* item);
        void SendAuctionClosedNotification(AuctionEntry const* auction, float mailDelay, bool sold, Item const* item);
        void SendAuctionWonNotification(AuctionEntry const* auction, Item const* item);
        void SendAuctionOwnerBidNotification(AuctionEntry const* auction, Item const* item);

        void HandleBlackMarketOpen(WorldPackets::BlackMarket::BlackMarketOpen& packet);
        void SendBlackMarketOpenResult(ObjectGuid guid);
        void SendBlackMarketBidOnItemResult(int32 result, int32 marketId, WorldPackets::Item::ItemInstance& item);
        void SendBlackMarketWonNotification(BlackMarketEntry const* entry, Item const* item);
        void SendBlackMarketOutbidNotification(BlackMarketTemplate const* templ);
        void HandleBlackMarketRequestItems(WorldPackets::BlackMarket::BlackMarketRequestItems& packet);
        void HandleBlackMarketBidOnItem(WorldPackets::BlackMarket::BlackMarketBidOnItem& packet);
        
        //Taxi
        void SendTaxiStatus(ObjectGuid guid);
        void SendTaxiMenu(Creature* unit);
        void SendDoFlight(uint32 mountDisplayId, uint32 path, uint32 pathNode = 0);
        bool SendLearnNewTaxiNode(Creature* unit);
        void SendDiscoverNewTaxiNode(uint32 nodeid);

        // Guild
        void SendPetitionShowList(ObjectGuid guid);

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

        void Handle_NULL(WorldPackets::Null& null);
        void Handle_NULL(WorldPacket& recvPacket);          // not used
        void Handle_EarlyProccess(WorldPacket& recvPacket); // just mark packets processed in WorldSocket::OnRead

        void HandleCharEnumOpcode(WorldPackets::Character::EnumCharacters& /*enumCharacters*/);
        void HandleCharDeleteOpcode(WorldPackets::Character::DeleteChar& charDelete);
        void HandleCharCreateOpcode(WorldPackets::Character::CreateChar& charCreate);
        void HandleCharCreateCallback(PreparedQueryResult result, WorldPackets::Character::CharacterCreateInfo* createInfo);
        void HandlePlayerLoginOpcode(WorldPackets::Character::PlayerLogin& playerLogin);
        void HandleLoadScreenOpcode(WorldPackets::Character::LoadingScreenNotify& loadingScreenNotify);
        void HandleCharEnum(PreparedQueryResult result, bool isDelete);
        void HandlePlayerLogin(LoginQueryHolder * holder);
        void HandleCharFactionOrRaceChange(WorldPacket& recvData);
        void HandleGenerateRandomCharacterName(WorldPackets::Character::GenerateRandomCharacterName& packet);
        void HandleReorderCharacters(WorldPacket& recvData);

        void SendCharCreate(ResponseCodes result);
        void SendCharDelete(ResponseCodes result);

        void HandleRequestPlayedTime(WorldPackets::Character::RequestPlayedTime& packet);

        void HandleLookingForGroup(WorldPacket& recvPacket);

        // cemetery/graveyard related
        void HandlePortGraveyard(WorldPackets::Misc::PortGraveyard& packet);
        void HandleRequestCemeteryList(WorldPackets::Misc::RequestCemeteryList& packet);

        void HandleInspect(WorldPackets::Inspect::Inspect& packet);

        void HandleRequestHonorStats(WorldPackets::Inspect::RequestHonorStats& packet);

        void HandleInspectPVP(WorldPackets::Inspect::InspectPVPRequest& packet);

        void HandleMoveWaterWalkAck(WorldPacket& recvPacket);

        void HandleMountSpecialAnimOpcode(WorldPacket& recvdata);

        // character view
        void HandleShowingHelm(WorldPackets::Character::ShowingHelm& /*packet*/);
        void HandleShowingCloak(WorldPackets::Character::ShowingCloak& packet);

        void HandleRepairItem(WorldPackets::Item::RepairItem& packet);

        void HandleMoveTeleportAck(WorldPackets::Movement::MoveTeleportAck& packet);
        void HandleForceSpeedChangeAck(WorldPackets::Movement::MovementSpeedAck& packet);
        void HandleMoveKnockBackAck(WorldPackets::Movement::MovementAckMessage& packet);
        void HandleMovementAckMessage(WorldPackets::Movement::MovementAckMessage& packet);
        void HandleSetCollisionHeightAck(WorldPackets::Movement::MoveSetCollisionHeightAck& packet);

        void HandlePingOpcode(WorldPacket& recvPacket);
        void HandleAuthSessionOpcode(WorldPacket& recvPacket);
        void HandleRepopRequest(WorldPackets::Misc::RepopRequest& packet);
        void HandleAutostoreLootItemOpcode(WorldPackets::Loot::AutoStoreLootItem& packet);
        void HandleLootMoney(WorldPackets::Loot::LootMoney& packet);
        void HandleSetLootSpecialization(WorldPackets::Loot::SetLootSpecialization& packet);
        void HandleLootUnit(WorldPackets::Loot::LootUnit& packet);
        void HandleLootRelease(WorldPackets::Loot::LootRelease& packet);
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

        void HandleTogglePvP(WorldPackets::Misc::TogglePvP& packet);
        void HandleSetPvP(WorldPackets::Misc::SetPvP& packet);

        void HandleSetSelectionOpcode(WorldPackets::Misc::SetSelection& packet);
        void HandleStandStateChangeOpcode(WorldPackets::Misc::StandStateChange& packet);
        void HandleEmote(WorldPackets::Character::EmoteClient& packet);
        void HandleContactListOpcode(WorldPackets::Social::SendContactList& packet);
        void HandleAddFriend(WorldPackets::Social::AddFriend& packet);
        void HandleAddFriendOpcodeCallBack(PreparedQueryResult result, std::string friendNote);
        void HandleDelFriendOpcode(WorldPackets::Social::DelFriend& packet);
        void HandleAddIgnore(WorldPackets::Social::AddIgnore& packet);
        void HandleAddIgnoreOpcodeCallBack(PreparedQueryResult result);
        void HandleDelIgnoreOpcode(WorldPackets::Social::DelIgnore& packet);
        void HandleSetContactNotesOpcode(WorldPackets::Social::SetContactNotes& packet);
        void HandleBugOpcode(WorldPacket& recvPacket);

        void HandleCanDuel(WorldPackets::Duel::CanDuel& packet);
        void HandleDuelResponse(WorldPackets::Duel::DuelResponse& duelResponse);
        void HandleDuelAccepted();
        void HandleDuelCancelled();

        void HandleAreaTrigger(WorldPackets::Misc::AreaTrigger& packet);

        void HandleSetFactionAtWar(WorldPackets::Character::SetFactionAtWar& packet);
        void HandleUnsetFactionAtWar(WorldPackets::Character::SetFactionNotAtWar& packet);
        void HandleBonusFactionID(WorldPackets::LFG::BonusFactionID& packet);
        void HandleSetFactionCheat(WorldPacket& recvData);
        void HandleSetWatchedFaction(WorldPackets::Character::SetWatchedFaction& packet);
        void HandleSetFactionInactive(WorldPackets::Character::SetFactionInactive& packet);

        void HandleUpdateAccountData(WorldPackets::ClientConfig::UserClientUpdateAccountData& packet);
        void HandleRequestAccountData(WorldPackets::ClientConfig::RequestAccountData& request);
        void HandleSetAdvancedCombatLogging(WorldPackets::ClientConfig::SetAdvancedCombatLogging& setAdvancedCombatLogging);
        void HandleSetActionButtonOpcode(WorldPackets::Spells::SetActionButton& packet);
        
        void SendConnectToInstance(WorldPackets::Auth::ConnectToSerial serial);
        void HandleContinuePlayerLogin();
        void AbortLogin(WorldPackets::Character::LoginFailureReason reason);

        void HandleGameObjectUseOpcode(WorldPacket& recPacket);
        void HandleMeetingStoneInfo(WorldPacket& recPacket);
        void HandleGameobjectReportUse(WorldPacket& recvPacket);

        void HandleQueryPlayerName(WorldPackets::Query::QueryPlayerName& packet);

        void HandleQueryTimeOpcode(WorldPacket& recvPacket);

        void HandleCreatureQuery(WorldPackets::Query::QueryCreature& packet);

        void HandleQueryGameObject(WorldPackets::Query::QueryGameObject& packet);

        void HandleWorldPortAck(WorldPackets::Movement::WorldPortAck& packet);
        void HandleWorldPortAck();                // for server-side calls

        void HandleMovementOpcodes(WorldPackets::Movement::ClientPlayerMovement& packet);
        void HandleSetActiveMover(WorldPackets::Movement::SetActiveMover& packet);
        void HandleMoveDismissVehicle(WorldPackets::Vehicle::MoveDismissVehicle& packet);
        void HandleRequestVehiclePrevSeat(WorldPackets::Vehicle::RequestVehiclePrevSeat& packet);
        void HandleRequestVehicleNextSeat(WorldPackets::Vehicle::RequestVehicleNextSeat& packet);
        void HandleMoveChangeVehicleSeats(WorldPackets::Vehicle::MoveChangeVehicleSeats& packet);
        void HandleRequestVehicleSwitchSeat(WorldPackets::Vehicle::RequestVehicleSwitchSeat& packet);

        void HandleRequestVehicleExit(WorldPackets::Vehicle::RequestVehicleExit& packet);
        void HandleMoveTimeSkipped(WorldPackets::Movement::MoveTimeSkipped& packet);

        void HandleRequestRaidInfo(WorldPackets::Party::RequestRaidInfo& packet);

        void HandleBattlefieldStatus(WorldPackets::Battleground::NullCmsg& packet);
        void HandleBattleMasterHelloOpcode(WorldPacket& recvData);

        void HandlePartyInvite(WorldPackets::Party::PartyInviteClient& packet);
        void HandlePartyInviteResponse(WorldPackets::Party::PartyInviteResponse& packet);
        void HandlePartyUninvite(WorldPackets::Party::PartyUninvite& packet);
        void HandleSetPartyLeader(WorldPackets::Party::SetPartyLeader& packet);
        void HandleSetRole(WorldPackets::Party::SetRole& packet);
        void HandleLeaveGroup(WorldPackets::Party::LeaveGroup& packet);
        void HandleOptOutOfLoot(WorldPackets::Party::OptOutOfLoot& packet);
        void HandleSetLootMethod(WorldPackets::Party::SetLootMethod& packet);
        void HandleLootRoll(WorldPackets::Loot::LootRoll& packet);
        void HandleDoMasterLootRoll(WorldPacket& recvData);
        void HandleRequestPartyJoinUpdates(WorldPackets::Party::RequestPartyJoinUpdates& packet);
        void HandleRequestPartyMemberStats(WorldPackets::Party::RequestPartyMemberStats& packet);
        void HandleReadyCheckResponse(WorldPackets::Party::ReadyCheckResponseClient& packet);
        void HandleDoReadyCheck(WorldPackets::Party::DoReadyCheck& packet);
        void HandleUpdateRaidTarget(WorldPackets::Party::UpdateRaidTarget& packet);
        void HandleConvertRaid(WorldPackets::Party::ConvertRaid& packet);
        void HandleChangeSubGroup(WorldPackets::Party::ChangeSubGroup& packet);
        void HandleSwapSubGroups(WorldPackets::Party::SwapSubGroups& packet);
        void HandleSetAssistantLeader(WorldPackets::Party::SetAssistantLeader& packet);
        void HandleSetEveryoneIsAssistant(WorldPackets::Party::SetEveryoneIsAssistant& packet);
        void HandlePartyAssignmentOpcode(WorldPacket& recvData);
        void HandleInitiateRolePoll(WorldPackets::Party::InitiateRolePoll& packet);

        void HandlePetitionBuy(WorldPackets::Petition::PetitionBuy& packet);
        void HandlePetitionShowSignatures(WorldPackets::Petition::PetitionShowSignatures& packet);
        void HandleQueryPetition(WorldPackets::Petition::QueryPetition& packet);
        void HandlePetitionRenameGuild(WorldPackets::Petition::PetitionRenameGuild& packet);
        void HandleSignPetition(WorldPackets::Petition::SignPetition& packet);
        void SendPetitionSignResult(ObjectGuid const& playerGuid, ObjectGuid const& petitionGuid, uint8 result);
        void HandleDeclinePetition(WorldPackets::Petition::DeclinePetition& packet);
        void HandleOfferPetition(WorldPackets::Petition::OfferPetition& packet);
        void HandleTurnInPetition(WorldPackets::Petition::TurnInPetition& packet);

        void HandleGuildQueryOpcode(WorldPackets::Guild::QueryGuildInfo& packet);
        void HandleGuildInviteByName(WorldPackets::Guild::GuildInviteByName& packet);
        void HandleGuildOfficerRemoveMember(WorldPackets::Guild::GuildOfficerRemoveMember& packet);
        void HandleGuildAcceptInvite(WorldPackets::Guild::AcceptGuildInvite& /*packet*/);
        void HandleGuildDeclineInvitation(WorldPackets::Guild::GuildDeclineInvitation& /*decline*/);
        void HandleGuildEventLogQuery(WorldPackets::Guild::GuildEventLogQuery& /*packet*/);
        void HandleGuildRosterOpcode(WorldPackets::Guild::GuildGetRoster& packet);
        void HandleGuildRewardsQueryOpcode(WorldPackets::Guild::RequestGuildRewardsList& packet);
        void HandleGuildPromoteMember(WorldPackets::Guild::GuildPromoteMember& packet);
        void HandleGuildDemoteMember(WorldPackets::Guild::GuildDemoteMember& packet);
        void HandleGuildAssignRank(WorldPackets::Guild::GuildAssignMemberRank& packet);
        void HandleGuildLeave(WorldPackets::Guild::GuildLeave& /*packet*/);
        void HandleGuildDisbandOpcode(WorldPackets::Guild::GuildDelete& packet);
        void HandleGuildSetGuildMaster(WorldPackets::Guild::GuildSetGuildMaster& packet);
        void HandleGuildUpdateMotdText(WorldPackets::Guild::GuildUpdateMotdText& packet);
        void HandleGuildNewsUpdateStickyOpcode(WorldPackets::Guild::GuildNewsUpdateSticky& packet);
        void HandleGuildSetMemberNote(WorldPackets::Guild::GuildSetMemberNote& packet);
        void HandleGuildGetRanks(WorldPackets::Guild::GuildGetRanks& packet);
        void HandleGuildQueryNews(WorldPackets::Guild::GuildQueryNews& packet);
        void HandleShiftRank(WorldPackets::Guild::GuildShiftRank& packet);
        void HandleGuildSetRankPermissions(WorldPackets::Guild::GuildSetRankPermissions& packet);
        void HandleGuildAddRank(WorldPackets::Guild::GuildAddRank& packet);
        void HandleGuildDeleteRank(WorldPackets::Guild::GuildDeleteRank& packet);
        void HandleGuildUpdateInfoText(WorldPackets::Guild::GuildUpdateInfoText& packet);
        void HandleSaveGuildEmblemOpcode(WorldPacket& recvPacket);
        void HandleGuildRequestPartyState(WorldPackets::Guild::RequestGuildPartyState& packet);
        void HandleGuildSetFocusedAchievement(WorldPackets::Achievement::GuildSetFocusedAchievement& packet);
        void HandleAutoDeclineGuildInvites(WorldPackets::Guild::AutoDeclineGuildInvites& packet);
        void HandleGuildQueryGuildRecipesOpcode(WorldPacket& recvPacket);
        void HandleGuildQueryGuildMembersForRecipe(WorldPacket& recvPacket);
        void HandleGuildQueryGuildMembersRecipes(WorldPacket& recvPacket);

        void HandleGuildRequestChallengeUpdate(WorldPackets::Guild::GuildChallengeUpdateRequest& /*packet*/);

        void HandleGuildFinderAddRecruit(WorldPacket& recvPacket);
        void HandleGuildFinderBrowse(WorldPacket& recvPacket);
        void HandleGuildFinderDeclineRecruit(WorldPacket& recvPacket);
        void HandleGuildFinderGetApplications(WorldPacket& recvPacket);
        void HandleGuildFinderGetRecruits(WorldPacket& recvPacket);
        void HandleGuildFinderPostRequest(WorldPacket& recvPacket);
        void HandleGuildFinderRemoveRecruit(WorldPacket& recvPacket);
        void HandleLFGuildSetGuildPost(WorldPackets::Guild::LFGuildSetGuildPost& packet);

        void HandleEnableTaxiNode(WorldPackets::Taxi::EnableTaxiNode& packet);
        void HandleTaxiNodeStatusQuery(WorldPackets::Taxi::TaxiNodeStatusQuery& packet);
        void HandleTaxiQueryAvailableNodes(WorldPackets::Taxi::TaxiQueryAvailableNodes& packet);
        void HandleActivateTaxi(WorldPackets::Taxi::ActivateTaxi& packet);
        void HandleMoveSplineDone(WorldPackets::Movement::MoveSplineDone& packet);
        void SendActivateTaxiReply(ActivateTaxiReply reply);
        void HandleTaxiRequestEarlyLanding(WorldPackets::Taxi::TaxiRequestEarlyLanding& packet);

        void HandleTabardVendorActivateOpcode(WorldPacket& recvPacket);
        void HandleBankerActivate(WorldPackets::NPC::Hello& packet);
        void HandleBuyBankSlot(WorldPackets::Bank::BuyBankSlot& packet);
        void HandleTrainerList(WorldPackets::NPC::Hello& packet);
        void HandleTrainerBuySpell(WorldPackets::NPC::TrainerBuySpell& packet);
        void HandlePetitionShowList(WorldPackets::Petition::PetitionShowList& packet);
        void HandleGossipHelloOpcode(WorldPackets::NPC::Hello& packet);
        void HandleGossipSelectOption(WorldPackets::NPC::GossipSelectOption& packet);
        void HandleSpiritHealerActivate(WorldPackets::NPC::SpiritHealerActivate& packet);
        void HandleQueryNPCText(WorldPackets::Query::QueryNPCText& packet);
        void HandleBinderActivate(WorldPackets::NPC::Hello& packet);
        void HandleListStabledPetsOpcode(WorldPacket& recvPacket);
        void HandleStableChangeSlot(WorldPacket& recvPacket);
        void HandleStableChangeSlotCallback(PreparedQueryResult result, uint8 new_slot);        //void HandleBuyStableSlot(WorldPacket& recvPacket);
        void HandleStableRevivePet(WorldPacket& recvPacket);
        void SendTrainerService(ObjectGuid guid, uint32 spellId, uint32 trainState);

        void HandleDuelCancelledOpcode(WorldPacket& recvPacket);

        void HandleAcceptTrade(WorldPackets::Trade::AcceptTrade& packet);
        void HandleBeginTrade(WorldPackets::Trade::NullCmsg& packet);
        void HandleBusyTrade(WorldPackets::Trade::NullCmsg& packet);
        void HandleCancelTrade(WorldPackets::Trade::NullCmsg& packet);
        void HandleClearTradeItem(WorldPackets::Trade::ClearTradeItem& packet);
        void HandleIgnoreTrade(WorldPackets::Trade::NullCmsg& packet);
        void HandleInitiateTrade(WorldPackets::Trade::InitiateTrade& packet);
        void HandleSetTradeGold(WorldPackets::Trade::SetTradeGold& packet);
        void HandleSetTradeCurrency(WorldPackets::Trade::SetTradeCurrency& packet);
        void HandleSetTradeItem(WorldPackets::Trade::SetTradeItem& packet);
        void HandleUnacceptTrade(WorldPackets::Trade::NullCmsg& packet);

        void HandleAuctionHelloRequest(WorldPackets::AuctionHouse::AuctionHelloRequest& packet);
        void HandleAuctionListItems(WorldPackets::AuctionHouse::AuctionListItems& packet);
        void HandleAuctionListBidderItems(WorldPackets::AuctionHouse::AuctionListBidderItems& packet);
        void HandleAuctionSellItem(WorldPackets::AuctionHouse::AuctionSellItem& packet);
        void HandleAuctionRemoveItem(WorldPackets::AuctionHouse::AuctionRemoveItem& packet);
        void HandleAuctionListOwnerItems(WorldPackets::AuctionHouse::AuctionListOwnerItems& packet);
        void HandleAuctionPlaceBid(WorldPackets::AuctionHouse::AuctionPlaceBid& packet);
        void HandleAuctionListPendingSales(WorldPackets::AuctionHouse::AuctionListPendingSales& packet);
        void HandleReplicateItems(WorldPackets::AuctionHouse::AuctionReplicateItems& packet);

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
        void HandleAutoEquipItem(WorldPackets::Item::AutoEquipItem& autoEquipItem);
        void HandleSellItemOpcode(WorldPackets::Item::SellItem& packet);
        void HandleBuyItemInSlotOpcode(WorldPacket& recvPacket);
        void HandleBuyItemOpcode(WorldPackets::Item::BuyItem& packet);
        void HandleListInventory(WorldPackets::NPC::Hello& packet);
        void HandleAutoStoreBagItem(WorldPackets::Item::AutoStoreBagItem& packet);
        void HandleReadItem(WorldPacket& recvPacket);
        void HandleAutoEquipItemSlotOpcode(WorldPacket& recvPacket);
        void HandleSwapItem(WorldPackets::Item::SwapItem& swapItem);
        void HandleBuybackItem(WorldPackets::Item::BuyBackItem& packet);
        void HandleAutoBankItem(WorldPackets::Bank::AutoBankItem& packet);
        void HandleAutoStoreBankItem(WorldPackets::Bank::AutoStoreBankItem& packet);
        void HandleWrapItemOpcode(WorldPacket& recvPacket);
        void HandleUseCritterItem(WorldPackets::Item::UseCritterItem& packet);

        void HandleAttackSwing(WorldPackets::Combat::AttackSwing& packet);
        void HandleAttackStop(WorldPackets::Combat::AttackStop& packet);
        void HandleSetSheathed(WorldPackets::Combat::SetSheathed& packet);

        void HandleUseItemOpcode(WorldPackets::Spells::ItemUse& recvPacket);
        void HandleOpenItem(WorldPackets::Spells::OpenItem& packet);
        void HandleCastSpellOpcode(WorldPackets::Spells::CastSpell& castRequest);
        void HandleCancelCast(WorldPackets::Spells::CancelCast& packet);
        void HandleCancelAura(WorldPackets::Spells::CancelAura& packet);
        void HandleCancelAutoRepeatSpellOpcode(WorldPacket& recvPacket);

        void HandleConfirmRespecWipe(WorldPackets::Misc::ConfirmRespecWipe& packet);
        void HandleUnlearnSkill(WorldPackets::Spells::UnlearnSkill& packet);
        void HandleSetSpecialization(WorldPackets::Talent::SetSpecialization& packet);
        void HandleLearnTalent(WorldPackets::Talent::LearnTalent& packet);

        void HandleQuestGiverStatusQuery(WorldPackets::Quest::QuestGiverStatusQuery& packet);
        void HandleQuestgiverStatusMultipleQuery(WorldPackets::Quest::QuestGiverStatusMultipleQuery& packet);
        void HandleQuestGiverHello(WorldPackets::Quest::QuestGiverHello& packet);
        void HandleQuestGiverAcceptQuest(WorldPackets::Quest::QuestGiverAcceptQuest& packet);
        void HandleQuestGiverQueryQuest(WorldPackets::Quest::QuestGiverQueryQuest& packet);
        void HandleQuestGiverChooseReward(WorldPackets::Quest::QuestGiverChooseReward& packet);
        void HandleQuestGiverRequestReward(WorldPackets::Quest::QuestGiverRequestReward& packet);
        void HandleQueryQuestInfo(WorldPackets::Quest::QueryQuestInfo& packet);
        void HandleQuestgiverCancel(WorldPacket& recvData);
        void HandleQuestLogSwapQuest(WorldPacket& recvData);
        void HandleQuestLogRemoveQuest(WorldPackets::Quest::QuestLogRemoveQuest& packet);
        void HandleQuestConfirmAccept(WorldPackets::Quest::QuestConfirmAccept& packet);
        void HandleQuestgiverCompleteQuest(WorldPackets::Quest::QuestGiverCompleteQuest& packet);
        void HandleQuestgiverQuestAutoLaunch(WorldPacket& recvPacket);
        void HandlePushQuestToParty(WorldPacket& recvPacket);
        void HandleQuestPushResult(WorldPackets::Quest::QuestPushResult& packet);

        void SendQuestgiverStatusMultipleQuery();

        bool processChatmessageFurtherAfterSecurityChecks(std::string&, uint32);
        void HandleChatMessageOpcode(WorldPackets::Chat::ChatMessage& packet);
        void HandleChatMessageAFK(WorldPackets::Chat::ChatMessageAFK& packet);
        void HandleChatMessageWhisperOpcode(WorldPackets::Chat::ChatMessageWhisper& packet);
        void HandleChatMessageChannelOpcode(WorldPackets::Chat::ChatMessageChannel& packet);
        void HandleChatMessageEmoteOpcode(WorldPackets::Chat::ChatMessageEmote& packet);
        void HandleChatMessage(ChatMsg type, uint32 lang, std::string msg, std::string target = "");
        void HandleChatAddonMessageOpcode(WorldPackets::Chat::ChatAddonMessage& packet);
        void HandleChatAddonMessageWhisper(WorldPackets::Chat::ChatAddonMessageWhisper& packet);
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
        void HandleSummonResponse(WorldPackets::Movement::SummonResponse& packet);

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

        void HandleCompleteCinematic(WorldPackets::Misc::CompleteCinematic& packet);
        void HandleNextCinematicCamera(WorldPackets::Misc::NextCinematicCamera& packet);
        void HandleCompleteMovie(WorldPacket& recvPacket);

        void HandleQueryPageText(WorldPackets::Query::QueryPageText& packet);

        void HandleTutorialFlag(WorldPackets::Misc::TutorialSetFlag& packet);

        void HandlePetAction(WorldPacket& recvData);
        void HandleStopAttack(WorldPackets::PetPackets::StopAttack& packet);
        void HandlePetActionHelper(Unit* pet, ObjectGuid guid1, uint32 spellid, uint16 flag, ObjectGuid guid2, float x, float y, float z);
        void HandleQueryPetName(WorldPackets::Query::QueryPetName& packet);
        void HandlePetSetAction(WorldPacket& recvData);
        void HandlePetAbandon(WorldPacket& recvData);
        void HandlePetRename(WorldPacket& recvData);
        void HandlePetCancelAura(WorldPackets::PetPackets::PetCancelAura& packet);
        void HandlePetSpellAutocastOpcode(WorldPacket& recvPacket);
        void HandlePetCastSpellOpcode(WorldPackets::Spells::PetCastSpell& recvPacket);

        void HandleSetActionBarToggles(WorldPackets::Character::SetActionBarToggles& packet);

        void HandleCharacterRenameRequest(WorldPackets::Character::CharacterRenameRequest& packet);
        void HandleChangePlayerNameOpcodeCallBack(PreparedQueryResult result, std::string newName);
        void HandleSetPlayerDeclinedNames(WorldPacket& recvData);

        void HandleTotemDestroyed(WorldPackets::Totem::TotemDestroyed& packet);
        void HandleDismissCritter(WorldPackets::PetPackets::DismissCritter& packet);

        void HandleBattlemasterHello(WorldPackets::NPC::Hello& packet);
        void HandleBattlemasterJoin(WorldPackets::Battleground::Join& packet);
        void HandlePVPLogData(WorldPackets::Battleground::NullCmsg& packet);
        void HandleBattleFieldPort(WorldPackets::Battleground::Port& packet);
        void HandleBattlefieldList(WorldPackets::Battleground::ListClient& packet);
        void HandleLeaveBattlefield(WorldPackets::Battleground::NullCmsg& packet);
        void HandleBattlemasterJoinArena(WorldPackets::Battleground::JoinArena& packet);
        void HandleJoinSkirmish(WorldPackets::Battleground::JoinSkirmish& packet);
        void HandleBattlemasterJoinRated(WorldPacket& recvData);
        void JoinBracket(uint8 slot);

        void HandleReportPvPAFK(WorldPacket& recvData);
        void HandleRequestRatedInfo(WorldPackets::Battleground::NullCmsg& packet);
        void HandleRequestPvpOptions(WorldPackets::Battleground::NullCmsg& packet);
        void HandleRequestPvpReward(WorldPacket& recvData);
        void HandleRequestRatedBgStats(WorldPacket& recvData);

        void HandleWardenDataOpcode(WorldPacket& recvData);
        void HandleWorldTeleport(WorldPackets::Misc::WorldTeleport& packet);
        void HandleMinimapPing(WorldPackets::Party::MinimapPingClient& packet);
        void HandleRandomRollClient(WorldPackets::Misc::RandomRollClient& packet);
        void HandleFarSight(WorldPackets::Misc::FarSight& packet);
        void HandleSetDungeonDifficulty(WorldPackets::Misc::SetDungeonDifficulty& packet);
        void HandleSetRaidDifficulty(WorldPackets::Misc::SetRaidDifficulty& packet);
        void HandleMoveSetCanFlyAckOpcode(WorldPacket& recvData);
        void HandleSetTitle(WorldPackets::Character::SetTitle& packet);
        void HandleRealmQueryNameOpcode(WorldPacket& recvData);
        void HandleTimeSyncResp(WorldPackets::Misc::TimeSyncResponse& packet);
        void HandleWhoisOpcode(WorldPackets::Who::WhoIsRequest& packet);
        void HandleResetInstances(WorldPackets::Instance::ResetInstances& packet);
        void HandleHearthAndResurrect(WorldPacket& recvData);
        void HandleInstanceLockResponse(WorldPackets::Instance::InstanceLockResponse& packet);
        void HandlePersonalRatedInfoRequest(WorldPackets::Battleground::NullCmsg& packet);

        // Battlefield
        void SendBfInvitePlayerToWar(uint64 const& guid,uint32 ZoneId,uint32 time);
        void SendBfInvitePlayerToQueue(uint64 queueId, int8 battleState);
        void SendBfQueueInviteResponse(uint64 queueId, uint32 zoneId, int8 battleStatus, bool canQueue = true, bool loggingIn = false);
        void SendBfEntered(uint64 queueId, bool relocated, bool onOffense);
        void SendBfLeaveMessage(uint64 queueId, int8 battleState, bool relocated, BFLeaveReason reason = BF_LEAVE_REASON_EXITED);
        void HandleBfQueueInviteResponse(WorldPackets::Battleground::EntryOrQueueInviteResponse& packet);
        void HandleBfEntryInviteResponse(WorldPackets::Battleground::EntryOrQueueInviteResponse& packet);
        void HandleBfExitQueueRequest(WorldPackets::Battleground::ExitRequest& packet);
        void HandleBfSetPreferedCemetry(WorldPacket & recvData);
        void HandleBfExitRequest(WorldPacket &recv_data);
        void HandleBfQueueRequest(WorldPackets::Battleground::QueueRequest& packet);

        void HandleDFSetComment(WorldPackets::LFG::DFSetComment& packet);
        void HandleLockInfoRequest(WorldPackets::LFG::LockInfoRequest& packet);
        void HandleDFJoin(WorldPackets::LFG::DFJoin& packet);
        void HandleDFLeave(WorldPackets::LFG::DFLeave& packet);
        void HandleDFSetRoles(WorldPackets::LFG::DFSetRoles& packet);
        void HandleDFProposalResponse(WorldPackets::LFG::DFProposalResponse& packet);
        void HandleDFBootPlayerVote(WorldPackets::LFG::DFBootPlayerVote& packet);
        void HandleDFTeleport(WorldPackets::LFG::DFTeleport& packet);
        void HandleLfrJoinOpcode(WorldPacket& recvData);
        void HandleLfrLeaveOpcode(WorldPacket& recvData);
        void HandleDfGetJoinStatus(WorldPackets::LFG::NullCmsg& packet);

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

        void HandleAreaSpiritHealerQuery(WorldPackets::Battleground::AreaSpiritHealerQuery& packet);
        void HandleAreaSpiritHealerQueue(WorldPackets::Battleground::AreaSpiritHealerQueue& packet);
        void HandleCancelMountAura(WorldPackets::Spells::CancelMountAura& packet);
        void HandleCancelGrowthAura(WorldPackets::Spells::CancelGrowthAura& packet);
        void HandleSelfRes(WorldPackets::Spells::SelfRes& packet);
        void HandleComplainOpcode(WorldPacket& recvData);
        void HandleRequestPetInfoOpcode(WorldPacket& recvData);

        // Socket gem
        void HandleSocketOpcode(WorldPacket& recvData);

        void HandleSortBagsOpcode(WorldPacket& recvData);

        void HandleCancelTempEnchantmentOpcode(WorldPacket& recvData);

        void HandleGetItemPurchaseData(WorldPackets::Item::ItemRefundInfo& packet);
        void HandleItemRefund(WorldPacket& recvData);
        void HandleItemTextQuery(WorldPackets::Query::ItemTextQuery& packet);
        void HandleChannelVoiceOnOpcode(WorldPacket& recvData);
        void HandleVoiceSessionEnableOpcode(WorldPacket& recvData);
        void HandleSetActiveVoiceChannel(WorldPacket& recvData);
        void HandleSetTaxiBenchmarkOpcode(WorldPacket& recvData);

        // Guild Bank
        void HandleGuildPermissions(WorldPackets::Guild::GuildPermissionsQuery& packet);
        void HandleGuildBankMoneyWithdrawn(WorldPackets::Guild::GuildBankRemainingWithdrawMoneyQuery& packet);
        void HandleGuildBankActivate(WorldPackets::Guild::GuildBankActivate& packet);
        void HandleGuildBankQueryTab(WorldPackets::Guild::GuildBankQueryTab& packet);
        void HandleGuildBankLogQuery(WorldPackets::Guild::GuildBankLogQuery& packet);
        void HandleGuildBankDepositMoney(WorldPackets::Guild::GuildBankDepositMoney& packet);
        void HandleGuildBankWithdrawMoney(WorldPackets::Guild::GuildBankWithdrawMoney& packet);
        void HandleGuildBankSwapItems(WorldPackets::Guild::GuildBankSwapItems& packet);

        void HandleGuildBankUpdateTab(WorldPackets::Guild::GuildBankUpdateTab& packet);
        void HandleGuildBankBuyTab(WorldPackets::Guild::GuildBankBuyTab& packet);
        void HandleGuildBankTextQuery(WorldPackets::Guild::GuildBankTextQuery& packet);
        void HandleGuildBankSetTabText(WorldPackets::Guild::GuildBankSetTabText& packet);

        void HandleGrantLevel(WorldPackets::ReferAFriend::GrantLevel& packet);
        void HandleAcceptGrantLevel(WorldPackets::ReferAFriend::AcceptLevelGrant& packet);

        void HandleCalendarGetCalendar(WorldPackets::Calendar::CalendarGetCalendar& packet);
        void HandleCalendarGetEvent(WorldPackets::Calendar::CalendarGetEvent& packet);
        void HandleCalendarGuildFilter(WorldPackets::Calendar::CalendarGuildFilter& packet);
        void HandleCalendarAddEvent(WorldPackets::Calendar::CalendarAddEvent& packet);
        void HandleCalendarUpdateEvent(WorldPackets::Calendar::CalendarUpdateEvent& packet);
        void HandleCalendarRemoveEvent(WorldPackets::Calendar::CalendarRemoveEvent& packet);
        void HandleCalendarCopyEvent(WorldPackets::Calendar::CalendarCopyEvent& packet);
        void HandleCalendarEventInvite(WorldPackets::Calendar::CalendarEventInvite& packet);
        void HandleCalendarEventRsvp(WorldPackets::Calendar::CalendarEventRSVP& packet);
        void HandleCalendarEventRemoveInvite(WorldPackets::Calendar::CalendarRemoveInvite& packet);
        void HandleCalendarEventStatus(WorldPackets::Calendar::CalendarEventStatus& packet);
        void HandleCalendarEventModeratorStatus(WorldPackets::Calendar::CalendarEventModeratorStatus& packet);
        void HandleCalendarGetNumPending(WorldPackets::Calendar::CalendarGetNumPending& packet);
        void HandleCalendarEventSignup(WorldPackets::Calendar::CalendarEventSignUp& packet);
        void HandleSetSavedInstanceExtend(WorldPackets::Calendar::SetSavedInstanceExtend& packet);

        void SendCalendarRaidLockout(InstanceSave const* save, bool add);
        void SendCalendarRaidLockoutUpdated(InstanceSave const* save);

        void HandleCalendarComplain(WorldPacket& recvData);

        // Void Storage
        void HandleVoidStorageUnlock(WorldPackets::VoidStorage::UnlockVoidStorage& packet);
        void HandleVoidStorageQuery(WorldPackets::VoidStorage::QueryVoidStorage& packet);
        void HandleVoidStorageTransfer(WorldPackets::VoidStorage::VoidStorageTransfer& packet);
        void HandleVoidSwapItem(WorldPackets::VoidStorage::SwapVoidItem& packet);
        void SendVoidStorageTransferResult(VoidTransferError result);
        void SendVoidStorageFailed(int8 reason = 0);

        // Transmogrification
        void HandleTransmogrifyItems(WorldPackets::Item::TransmogrigyItem& request);

        // Upgrade item
        void HandleUpgradeItem(WorldPacket& recvData);

        void HandleBattlePetSetFlags(WorldPackets::BattlePet::SetFlags& packet);
        void HandleModifyName(WorldPackets::BattlePet::ModifyName& packet);
        void HandleBattlePetNameQuery(WorldPackets::BattlePet::Query& packet);
        void HandleCageBattlePet(WorldPackets::BattlePet::BattlePetGuidRead& packet);
        void HandleBattlePetSetSlot(WorldPackets::BattlePet::SetBattleSlot& packet);
        void HandleBattlePetSummon(WorldPackets::BattlePet::BattlePetGuidRead& packet);
        void HandlePetBattleRequestWild(WorldPackets::BattlePet::RequestWild& packet);
        void HandlePetBattleRequestUpdate(WorldPackets::BattlePet::RequestUpdate& packet);
        void HandlePetBattleInput(WorldPackets::BattlePet::PetBattleInput& packet);
        void HandlePetBattleFinalNotify(WorldPackets::BattlePet::NullCmsg& packet);
        void HandlePetBattleQuitNotify(WorldPackets::BattlePet::NullCmsg& packet);
        void HandleBattlePetDelete(WorldPackets::BattlePet::BattlePetGuidRead& packet);
        void HandleBattlePetRequestJournal(WorldPackets::BattlePet::NullCmsg& packet);
        void HandleBattlePetJournalLock(WorldPackets::BattlePet::NullCmsg& packet);
        void HandleJoinPetBattleQueue(WorldPackets::BattlePet::NullCmsg& packet);
        void HandlePetBattleScriptErrorNotify(WorldPackets::BattlePet::NullCmsg& packet);
        void HandleBattlePetDeletePetCheat(WorldPackets::BattlePet::BattlePetGuidRead& packet);
        void HandlePetBattleRequestPVP(WorldPackets::BattlePet::RequestPVP& packet);
        void HandleReplaceFrontPet(WorldPackets::BattlePet::ReplaceFrontPet& packet);
        void HanldeQueueProposeMatchResult(WorldPackets::BattlePet::QueueProposeMatchResult& packet);
        void HandleLeaveQueue(WorldPackets::BattlePet::LeaveQueue& packet);
        

        void HandleSpellClick(WorldPackets::Spells::SpellClick& packet);
        void HandleGetMirrorImageData(WorldPackets::Spells::GetMirrorImageData& packet);
        void HandleAlterAppearance(WorldPackets::Character::AlterApperance& packet);
        void HandleRemoveGlyph(WorldPacket& recvData);
        void HandleCharCustomize(WorldPacket& recvData);
        void HandleCharUndelete(WorldPacket& recvData);
        void HandleQueryInspectAchievements(WorldPackets::Inspect::QueryInspectAchievements& inspect);
        void HandleGuildAchievementProgressQuery(WorldPacket& recvData);
        void HandleSaveEquipmentSet(WorldPackets::EquipmentSet::SaveEquipmentSet& packet);
        void HandleDeleteEquipmentSet(WorldPackets::EquipmentSet::DeleteEquipmentSet& packet);
        void HandleEquipmentSetUse(WorldPackets::EquipmentSet::UseEquipmentSet& packet);
        void HandleUITimeRequest(WorldPackets::Misc::UITimeRequest& /*request*/);
        void HandleQueryQuestCompletionNPCs(WorldPackets::Query::QueryQuestCompletionNPCs& packet);
        void HandleQuestPOIQuery(WorldPackets::Query::QuestPOIQuery& packet);
        void HandleEjectPassenger(WorldPackets::Vehicle::EjectPassenger& packet);
        void HandleRideVehicleInteract(WorldPackets::Vehicle::RideVehicleInteract& packet);
        void HandleSetVehicleRecId(WorldPackets::Vehicle::MoveSetVehicleRecIdAck& packet);
        void HandleUpdateProjectilePosition(WorldPacket& recvPacket);
        void HandleDBQueryBulk(WorldPackets::Query::DBQueryBulk& packet);
        void HandleUpdateMissileTrajectory(WorldPacket& recvPacket);
        void HandleViolenceLevel(WorldPackets::Misc::ViolenceLevel& packet);
        void HandleVoiceDelIgnore(WorldPackets::Social::VoiceDelIgnore& packet);
        void HandleVoiceAddIgnore(WorldPackets::Social::VoiceAddIgnore& packet);
        void HandleObjectUpdateFailed(WorldPackets::Misc::ObjectUpdateFailed& packet);
        void HandleObjectUpdateRescued(WorldPackets::Misc::ObjectUpdateRescued& packet);
        void HandleOpeningCinematic(WorldPackets::Misc::OpeningCinematic& packet);
        void HandleSetFactionOpcode(WorldPacket& recvPacket);
        void HandleRequestResearchHistory(WorldPacket& recvPacket);
        void HandleChangeCurrencyFlags(WorldPacket& recvPacket);
        void HandleRequestCategoryCooldowns(WorldPackets::Spells::RequestCategoryCooldowns& packet);
        void HandleClearRaidMarker(WorldPackets::Party::ClearRaidMarker& packet);
        void HandleShowTradeSkill(WorldPackets::Misc::ShowTradeSkill& packet);
        void HandleForcedReactions(WorldPackets::Reputation::RequestForcedReactions& packet);
        void HandleSaveCUFProfiles(WorldPacket& recvPacket);
        void SendLoadCUFProfiles();
        void HandleBattlePayDistributionAssign(WorldPackets::BattlePay::DistributionAssignToTarget& packet);
        void HandleBattlePayStartPurchase(WorldPackets::BattlePay::StartPurchase& packet);
        void HandleBattlePayConfirmPurchase(WorldPackets::BattlePay::ConfirmPurchaseResponse& packet);

        void HandleGetGarrisonInfo(WorldPackets::Garrison::GetGarrisonInfo& packet);
        void HandleGarrisonPurchaseBuilding(WorldPackets::Garrison::GarrisonPurchaseBuilding& packet);
        void HandleGarrisonCancelConstruction(WorldPackets::Garrison::GarrisonCancelConstruction& packet);
        void HandleGarrisonRequestBlueprintAndSpecializationData(WorldPackets::Garrison::GarrisonRequestBlueprintAndSpecializationData& packet);
        void HandleGarrisonGetBuildingLandmarks(WorldPackets::Garrison::GarrisonGetBuildingLandmarks& packet);
        void HandleGarrisonMissionBonusRoll(WorldPackets::Garrison::GarrisonMissionBonusRoll& packet);
        void HandleGarrisonRequestLandingPageShipmentInfo(WorldPackets::Garrison::GarrisonRequestLandingPageShipmentInfo& packet);
        void HandleGarrisonCheckUpgradeable(WorldPackets::Garrison::GarrisonCheckUpgradeable& packet);
        void HandleGarrisonStartMission(WorldPackets::Garrison::GarrisonStartMission& packet);
        void HandleGarrisonCompleteMission(WorldPackets::Garrison::GarrisonCompleteMission& packet);
        void HandleCreateShipment(WorldPackets::Garrison::CreateShipment& packet);
        void HandleGarrisonOpenMissionNpc(WorldPackets::Garrison::GarrisonOpenMissionNpc& packet);
        void HandleCompleteAllReadyShipments(WorldPackets::Garrison::CompleteAllReadyShipments& packet);
        void HandleUpgradeGarrison(WorldPackets::Garrison::UpgradeGarrison& packet);
        void HandleTrophyData(WorldPackets::Garrison::TrophyData& packet);
        void HandleRevertTrophy(WorldPackets::Garrison::RevertTrophy& packet);
        void HandleGetTrophyList(WorldPackets::Garrison::GetTrophyList& packet);
        void HandleGarrisonSetFollowerInactive(WorldPackets::Garrison::GarrisonSetFollowerInactive& packet);
        void HandleGarrisonRemoveFollowerFromBuilding(WorldPackets::Garrison::GarrisonRemoveFollowerFromBuilding& packet);
        void HandleGarrisonAssignFollowerToBuilding(WorldPackets::Garrison::GarrisonAssignFollowerToBuilding& packet);

        void HandleAddToy(WorldPackets::Toy::AddToy& packet);
        void HandleToySetFavorite(WorldPackets::Toy::ToySetFavorite& packet);
        void HandleUseToy(WorldPackets::Toy::UseToy& packet);

        void HandleQueryScenarioPOI(WorldPackets::Scene::QueryScenarioPOI& packet);

        void HandleGetChallengeModeRewards(WorldPackets::ChallengeMode::Misc& packet);
        void HandleChallengeModeRequestMapStats(WorldPackets::ChallengeMode::Misc& packet);
        void HandleRequestLeaders(WorldPackets::ChallengeMode::RequestLeaders& packet);

        // WarGames
        void HandleWarGameStart(WorldPacket& recvPacket);
        void HandleWarGameAccept(WorldPacket& recvPacket);

        void HandleScenePlaybackCanceled(WorldPackets::Scene::SceneInstance& packet);
        void HandleScenePlaybackComplete(WorldPackets::Scene::SceneInstance& packet);
        void HandleSceneTriggerEvent(WorldPackets::Scene::SceneTriggerEvent& packet);

        void SuspendTokenResponse(WorldPacket& recvPacket);
        void HandleGetUndeleteCharacterCooldownStatus(WorldPackets::Character::GetUndeleteCharacterCooldownStatus& packet);
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
        void SetWardenModuleFailed(bool s) { wardenModuleFailed = s; }
        bool IsWardenModuleFailed() { return wardenModuleFailed; }
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
        ObjectGuid m_playerLoading;                         // code processed in LoginPlayer
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
        StringVector _registeredAddonPrefixes;
        bool _filterAddonMessages;
        uint32 recruiterId;
        bool isRecruiter;
        LockedQueue<WorldPacket*> _recvQueue;
        time_t timeCharEnumOpcode;
        uint8 playerLoginCounter;

        bool wardenModuleFailed;

        uint32 antispamm[PACKETS_COUNT][2];//0 count, 1 savetime

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
