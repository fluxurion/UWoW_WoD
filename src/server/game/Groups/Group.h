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

#ifndef GROUP_H
#define GROUP_H

#include "DBCEnums.h"
#include "GroupRefManager.h"
#include "LootMgr.h"
#include "QueryResult.h"
#include "SharedDefines.h"
#include "Object.h"

class Battlefield;
class Battleground;
class Creature;
class GroupReference;
class InstanceSave;
class Map;
class Player;
class Unit;
class WorldObject;
class WorldPacket;
class WorldSession;
namespace lfg {
struct LFGDungeonData;
}

struct MapEntry;

#define MAX_GROUP_SIZE 5
#define MAX_RAID_SIZE 40
#define MAX_RAID_SUBGROUPS MAX_RAID_SIZE / MAX_GROUP_SIZE
#define TARGET_ICONS_COUNT 8
#define RAID_MARKERS_COUNT 8
#define RAID_ROLL_TIMER 180000
#define NORMAL_ROLL_TIMER 60000
#define READY_CHECK_DURATION 35000

enum RollVote
{
    PASS              = 0,
    NEED              = 1,
    GREED             = 2,
    DISENCHANT        = 3,
    NOT_EMITED_YET    = 4,
    NOT_VALID         = 5
};

enum GroupMemberOnlineStatus
{
    MEMBER_STATUS_OFFLINE   = 0x0000,
    MEMBER_STATUS_ONLINE    = 0x0001,                       // Lua_UnitIsConnected
    MEMBER_STATUS_PVP       = 0x0002,                       // Lua_UnitIsPVP
    MEMBER_STATUS_DEAD      = 0x0004,                       // Lua_UnitIsDead
    MEMBER_STATUS_GHOST     = 0x0008,                       // Lua_UnitIsGhost
    MEMBER_STATUS_PVP_FFA   = 0x0010,                       // Lua_UnitIsPVPFreeForAll
    MEMBER_STATUS_UNK3      = 0x0020,                       // used in calls from Lua_GetPlayerMapPosition/Lua_GetBattlefieldFlagPosition
    MEMBER_STATUS_AFK       = 0x0040,                       // Lua_UnitIsAFK
    MEMBER_STATUS_DND       = 0x0080,                       // Lua_UnitIsDND
};

enum GroupMemberFlags
{
    MEMBER_FLAG_ASSISTANT   = 0x01,
    MEMBER_FLAG_MAINTANK    = 0x02,
    MEMBER_FLAG_MAINASSIST  = 0x04,
};

enum GroupMemberAssignment
{
    GROUP_ASSIGN_MAINTANK   = 0,
    GROUP_ASSIGN_MAINASSIST = 1,
};

enum GroupType
{
    GROUPTYPE_NORMAL                = 0x00,
    GROUPTYPE_BG                    = 0x01,
    GROUPTYPE_RAID                  = 0x02,
    GROUPTYPE_BGRAID                = GROUPTYPE_BG | GROUPTYPE_RAID, // mask
    GROUPTYPE_LFG_RESTRICTED        = 0x04, // Script_HasLFGRestrictions
    GROUPTYPE_LFG                   = 0x08,
    //GROUPTYPE_UNK                   = 0x20, Script_IsInScenarioGroup() WOD 6.0.1
    GROUPTYPE_EVERYONE_IS_ASSISTANT = 0x40, // Script_IsEveryoneAssistant
};

enum GroupUpdateFlags
{
    GROUP_UPDATE_FLAG_NONE              = 0x00000000,       // nothing
    GROUP_UPDATE_FLAG_OTHER_PARTY       = 0x00000001,       // for (2) uint8 unk
    GROUP_UPDATE_FLAG_STATUS            = 0x00000002,       // uint16
    GROUP_UPDATE_FLAG_POWER_TYPE        = 0x00000004,       // uint8 (PowerType)
    GROUP_UPDATE_FLAG_UNK322            = 0x00000008,       // uint16 (unk)
    GROUP_UPDATE_FLAG_CUR_HP            = 0x00000010,       // uint32 (HP)
    GROUP_UPDATE_FLAG_MAX_HP            = 0x00000020,       // uint32 (max HP)
    GROUP_UPDATE_FLAG_CUR_POWER         = 0x00000040,       // int16 (power value)
    GROUP_UPDATE_FLAG_MAX_POWER         = 0x00000080,       // int16 (max power value)
    GROUP_UPDATE_FLAG_LEVEL             = 0x00000100,       // uint16 (level value)
    GROUP_UPDATE_FLAG_UNK200000         = 0x00000200,       // int16 (unk)
    GROUP_UPDATE_FLAG_ZONE              = 0x00000400,       // uint16 (zone id)
    GROUP_UPDATE_FLAG_SPECIALIZATION_ID = 0x00000800,       // int16 (specID)
    GROUP_UPDATE_FLAG_UNK4000000        = 0x00001000,       // int32 (unk)
    GROUP_UPDATE_FLAG_POSITION          = 0x00002000,       // uint16 (x), uint16 (y), uint16 (z)
    GROUP_UPDATE_FLAG_VEHICLE_SEAT      = 0x00104000,       // int32 (vehicle seat id)
    GROUP_UPDATE_FLAG_AURAS             = 0x00008000,       // uint8 (unk), uint64 (mask), uint32 (count), for each bit set: uint32 (spell id) + uint16 (AuraFlags)  (if has flags Scalable -> 3x int32 (bps))
    GROUP_UPDATE_FLAG_PET               = 0x00010000,       // complex (pet)
    GROUP_UPDATE_FLAG_PHASE             = 0x00020000,       // int32 (unk), uint32 (phase count), for (count) uint16(phaseId)

    GROUP_UPDATE_FULL = GROUP_UPDATE_FLAG_OTHER_PARTY   | GROUP_UPDATE_FLAG_STATUS      | GROUP_UPDATE_FLAG_POWER_TYPE |
                        GROUP_UPDATE_FLAG_UNK322        | GROUP_UPDATE_FLAG_CUR_HP      | GROUP_UPDATE_FLAG_MAX_HP |
                        GROUP_UPDATE_FLAG_CUR_POWER     | GROUP_UPDATE_FLAG_MAX_POWER   | GROUP_UPDATE_FLAG_LEVEL |
                        GROUP_UPDATE_FLAG_UNK200000     | GROUP_UPDATE_FLAG_ZONE        | GROUP_UPDATE_FLAG_SPECIALIZATION_ID |
                        GROUP_UPDATE_FLAG_UNK4000000    | GROUP_UPDATE_FLAG_POSITION    | GROUP_UPDATE_FLAG_VEHICLE_SEAT |
                        GROUP_UPDATE_FLAG_AURAS         | GROUP_UPDATE_FLAG_PET         | GROUP_UPDATE_FLAG_PHASE
};

enum GroupUpdatePetFlags
{
    GROUP_UPDATE_FLAG_PET_NONE      = 0x00000000,       // nothing
    GROUP_UPDATE_FLAG_PET_GUID      = 0x00000001,       // ObjectGuid (pet guid)
    GROUP_UPDATE_FLAG_PET_NAME      = 0x00000002,       // cstring (name, NULL terminated string)
    GROUP_UPDATE_FLAG_PET_MODEL_ID  = 0x00000004,       // uint16 (model id)
    GROUP_UPDATE_FLAG_PET_CUR_HP    = 0x00000008,       // uint32 (HP)
    GROUP_UPDATE_FLAG_PET_MAX_HP    = 0x00000010,       // uint32 (max HP)
    GROUP_UPDATE_FLAG_PET_AURAS     = 0x00000020,       // [see GROUP_UPDATE_FLAG_AURAS]

    GROUP_UPDATE_PET_FULL = GROUP_UPDATE_FLAG_PET_GUID      | GROUP_UPDATE_FLAG_PET_NAME    | GROUP_UPDATE_FLAG_PET_MODEL_ID |
                            GROUP_UPDATE_FLAG_PET_CUR_HP    | GROUP_UPDATE_FLAG_PET_MAX_HP  | GROUP_UPDATE_FLAG_PET_AURAS // all pet flags
};

class Roll : public LootValidatorRef
{
    public:
        Roll(ObjectGuid _guid, LootItem const& li);
        ~Roll();
        void setLoot(Loot* pLoot);
        Loot* getLoot();
        void targetObjectBuildLink();
        uint8 TotalEmited() const { return totalNeed + totalGreed + totalPass; }
        
        struct
        {
            ObjectGuid itemGUID;
            uint32 ItemID = 0;
            uint32 RandomPropertiesSeed = 0;
            uint32 RandomPropertiesID = 0;

            struct
            {
                uint8 Context = 0;
                std::vector<int32> BonusListIDs;
            } ItemBonus;

        } item;

        uint8 itemCount;
        ObjectGuid lootedGUID;
        typedef std::map<ObjectGuid, RollVote> PlayerVote;
        PlayerVote playerVote;                              //vote position correspond with player position (in group)
        uint8 totalPlayersRolling;
        uint8 totalNeed;
        uint8 totalGreed;
        uint8 totalPass;
        uint8 itemSlot;
        uint8 aoeSlot;
        uint8 rollVoteMask;
};

struct InstanceGroupBind
{
    InstanceSave* save;
    bool perm;
    /* permanent InstanceGroupBinds exist if the leader has a permanent
       PlayerInstanceBind for the same instance. */
    InstanceGroupBind() : save(NULL), perm(false) {}
};

struct RaidMarker
{
    WorldLocation Location;
    ObjectGuid TransportGUID;

    RaidMarker(uint32 mapId, float positionX, float positionY, float positionZ, ObjectGuid transportGuid = ObjectGuid::Empty)
    {
        Location.WorldRelocate(mapId, positionX, positionY, positionZ);
        TransportGUID = transportGuid;
    }
};

class Group
{
    public:
        struct MemberSlot
        {
            ObjectGuid  guid;
            std::string name;
            uint8       group;
            uint8       flags;
            uint8       roles;
        };
        typedef std::list<MemberSlot> MemberSlotList;
        typedef MemberSlotList::const_iterator member_citerator;

        typedef std::unordered_map< uint32 /*mapId*/, InstanceGroupBind> BoundInstancesMap;
    protected:
        typedef MemberSlotList::iterator member_witerator;
        typedef std::set<Player*> InvitesList;

        typedef std::vector<Roll*> Rolls;

    public:
        Group();
        ~Group();

        // group manipulation methods
        bool   Create(Player* leader);
        void   LoadGroupFromDB(Field* field);
        void   LoadMemberFromDB(ObjectGuid::LowType guidLow, uint8 memberFlags, uint8 subgroup, uint8 roles);
        bool   AddInvite(Player* player);
        void   RemoveInvite(Player* player);
        void   RemoveAllInvites();
        bool   AddLeaderInvite(Player* player);
        bool   AddMember(Player* player);
        bool   RemoveMember(ObjectGuid const& guid, const RemoveMethod &method = GROUP_REMOVEMETHOD_DEFAULT, ObjectGuid kicker = ObjectGuid::Empty, const char* reason = NULL);
        bool   AddCreatureMember(Creature* creature);
        void   ChangeLeader(ObjectGuid const& guid, int8 partyIndex = 0);
        bool   RemoveCreatureMember(ObjectGuid const& guid);
        void   SetLootMethod(LootMethod method);
        void   SetLooterGuid(ObjectGuid const& guid);
        void   UpdateLooterGuid(WorldObject* pLootedObject, bool ifneed = false);
        void   SetLootThreshold(ItemQualities threshold);
        void   Disband(bool hideDestroy=false);
        void   SetLfgRoles(ObjectGuid guid, const uint8 roles);
        uint8  GetLfgRoles(ObjectGuid guid);

        // properties accessories
        bool IsFull() const;
        bool isLFGGroup()  const;
        bool isRaidGroup() const;
        bool isBGGroup()   const;
        bool isArenaGroup()const;
        bool isBFGroup()   const;
        bool IsCreated()   const;
        inline bool IsHomeGroup() const { return !isLFGGroup() && (!isBGGroup() || isArenaGroup()) && !isBFGroup(); }
        ObjectGuid GetLeaderGUID() const;
        ObjectGuid GetGUID() const;
        uint32 GetGUIDLow() const;
        const char * GetLeaderName() const;
        LootMethod GetLootMethod() const;
        ObjectGuid GetLooterGuid() const;
        ItemQualities GetLootThreshold() const;

        uint32 GetDbStoreId() const { return m_dbStoreId; };

        // member manipulation methods
        bool IsMember(ObjectGuid guid) const;
        bool IsLeader(ObjectGuid guid) const;
        ObjectGuid GetMemberGUID(std::string const& name);
        bool IsAssistant(ObjectGuid guid) const;
        bool IsGuildGroup(ObjectGuid const& guildId, bool AllInSameMap = false, bool AllInSameInstanceId = false);
        void UpdateGuildAchievementCriteria(AchievementCriteriaTypes type, uint32 miscValue1, uint32 miscValue2, uint32 miscValue3, Unit* unit, WorldObject* rewardSource);

        Player* GetInvited(ObjectGuid guid) const;
        Player* GetInvited(std::string const& name) const;

        bool SameSubGroup(ObjectGuid guid1, ObjectGuid guid2) const;
        bool SameSubGroup(ObjectGuid guid1, MemberSlot const* slot2) const;
        bool SameSubGroup(Player const* member1, Player const* member2) const;
        bool HasFreeSlotSubGroup(uint8 subgroup) const;

        MemberSlotList const& GetMemberSlots() const { return m_memberSlots; }
        GroupReference* GetFirstMember() { return m_memberMgr.getFirst(); }
        GroupReference const* GetFirstMember() const { return m_memberMgr.getFirst(); }
        uint32 GetMembersCount() const { return m_memberSlots.size(); }

        uint8 GetMemberGroup(ObjectGuid guid) const;

        void ChangeFlagEveryoneAssistant(bool apply);
        void ConvertToLFG(lfg::LFGDungeonData const* dungeon);
        void ConvertToRaid(bool update = true);
        void ConvertToGroup();

        void SetBattlegroundGroup(Battleground* bg);
        void SetBattlefieldGroup(Battlefield* bf);
        GroupJoinBattlegroundResult CanJoinBattlegroundQueue(Battleground const* bgOrTemplate, BattlegroundQueueTypeId bgQueueTypeId, uint32 MinPlayerCount, uint32 MaxPlayerCount, bool isRated, uint32 arenaSlot, ObjectGuid& errorGuid);

        void ChangeMembersGroup(ObjectGuid guid, uint8 group);
        void ChangeMembersGroup(Player* player, uint8 group);
        void SetTargetIcon(uint8 id, ObjectGuid whoGuid, ObjectGuid targetGuid, uint8 partyIndex);
        void SetGroupMemberFlag(ObjectGuid guid, bool apply, GroupMemberFlags flag);
        void setGroupMemberRole(ObjectGuid guid, uint32 role);
        void RemoveUniqueGroupMemberFlag(GroupMemberFlags flag);

        void SetDungeonDifficultyID(Difficulty difficulty);
        void SetRaidDifficultyID(Difficulty difficulty);
        void SetLegacyRaidDifficultyID(Difficulty difficulty);
        Difficulty GetDifficultyID(MapEntry const* mapEntry) const;
        Difficulty GetDungeonDifficultyID() const { return m_dungeonDifficulty; }
        Difficulty GetRaidDifficultyID() const { return m_raidDifficulty; }
        Difficulty GetLegacyRaidDifficultyID() const { return m_legacyRaidDifficulty; }
        void ResetInstances(uint8 method, bool isRaid, bool isLegacy, Player* SendMsgTo);

        void SendTargetIconList(WorldSession* session, int8 partyIndex);
        void SendUpdate();
        void SendUpdateToPlayer(ObjectGuid playerGUID, MemberSlot* slot = NULL);

        void UpdatePlayerOutOfRange(Player* player);

        void BroadcastPacket(WorldPacket const* packet, bool ignorePlayersInBGRaid, int group = -1, ObjectGuid ignore = ObjectGuid::Empty);
        void BroadcastAddonMessagePacket(WorldPacket* packet, std::string const& prefix, bool ignorePlayersInBGRaid, int group = -1, ObjectGuid ignore = ObjectGuid::Empty);
        void BroadcastReadyCheck(WorldPacket const* packet);
        void OfflineReadyCheck();
        bool leaderInstanceCheckFail();

        void AddRaidMarker(uint8 markerID, uint32 mapID, float positionX, float positionY, float positionZ, ObjectGuid transportGuid = ObjectGuid::Empty);
        void DeleteRaidMarker(uint8 markerID);
        void SendRaidMarkersChanged(WorldSession* session = nullptr, int8 partyIndex = 0);

        /*********************************************************/
        /***                   LOOT SYSTEM                     ***/
        /*********************************************************/

        bool isRollLootActive() const { return !RollId.empty(); }
        void SendLootStartRoll(uint32 mapid, const Roll &r);
        void SendLootStartRollToPlayer(uint32 mapId, Player* p, bool canNeed, Roll const& r);
        void SendLootRoll(ObjectGuid TargetGuid, uint8 RollNumber, uint8 RollType, const Roll &r);
        void SendLootRollWon(ObjectGuid TargetGuid, uint8 RollNumber, uint8 RollType, const Roll &r);
        void SendLootAllPassed(Roll const& roll);
        void SendLooter(Creature* creature, Player* pLooter);
        void GroupLoot(Loot* loot, WorldObject* pLootedObject);
        void NeedBeforeGreed(Loot* loot, WorldObject* pLootedObject);
        void MasterLoot(Loot* loot, WorldObject* pLootedObject);
        Rolls::iterator GetRoll(uint8 slot);
        void ErraseRollbyRealSlot(uint8 slot, Loot* loot);
        void CountTheRoll(Rolls::iterator roll);
        void CountRollVote(ObjectGuid playerGUID, uint8 slot, uint8 Choise);
        void DoRollForAllMembers(ObjectGuid guid, uint8 slot, uint32 mapid, Loot*, LootItem&, Player*);
        void EndRoll(Loot* loot);
        void ClearAoeSlots() { m_aoe_slots = 0; };
        bool isRolledSlot(uint8 _slot)
        {
            for (Rolls::iterator iter = RollId.begin(); iter != RollId.end(); ++iter)
                if ((*iter)->aoeSlot == _slot && (*iter)->isValid())
                    return true;
            return false;
        }
        bool RollIsActive()
        {
            if(RollId.empty())
                return false;
            return true;
        }

        // related to disenchant rolls
        void ResetMaxEnchantingLevel();

        void LinkMember(GroupReference* pRef);
        void DelinkMember(ObjectGuid guid);

        InstanceGroupBind* BindToInstance(InstanceSave* save, bool permanent, bool load = false);
        void UnbindInstance(uint32 mapid, uint8 difficulty, bool unload = false);
        InstanceGroupBind* GetBoundInstance(Player* player);
        InstanceGroupBind* GetBoundInstance(Map* aMap);
        InstanceGroupBind* GetBoundInstance(MapEntry const* mapEntry);
        InstanceGroupBind* GetBoundInstance(Difficulty difficulty, uint32 mapId);
        BoundInstancesMap& GetBoundInstances(Difficulty difficulty);

        // FG: evil hacks
        void BroadcastGroupUpdate(void);

        void SetReadyCheckCount(uint8 count) { m_readyCheckCount = count; }
        uint8 GetReadyCheckCount() { return m_readyCheckCount; }
        uint8 GetGroupType() const { return m_groupType; }

        uint32 GetAverageMMR(BracketType bracket) const;
        ItemQualities GetThreshold() const { return m_lootThreshold; }

    protected:
        bool _setMembersGroup(ObjectGuid guid, uint8 group);
        void _homebindIfInstance(Player* player);

        void _initRaidSubGroupsCounter();
        member_citerator _getMemberCSlot(ObjectGuid Guid) const;
        member_witerator _getMemberWSlot(ObjectGuid Guid);
        void SubGroupCounterIncrease(uint8 subgroup);
        void SubGroupCounterDecrease(uint8 subgroup);
        void ToggleGroupMemberFlag(member_witerator slot, uint8 flag, bool apply);

        MemberSlotList      m_memberSlots;
        GroupRefManager     m_memberMgr;
        InvitesList         m_invitees;
        ObjectGuid          m_leaderGuid;
        std::string         m_leaderName;
        GroupType           m_groupType;
        Difficulty          m_dungeonDifficulty;
        Difficulty          m_raidDifficulty;
        Difficulty          m_legacyRaidDifficulty;
        Battleground*       m_bgGroup;
        Battlefield*        m_bfGroup;
        ObjectGuid          m_targetIcons[TARGET_ICONS_COUNT];
        LootMethod          m_lootMethod;
        ItemQualities       m_lootThreshold;
        ObjectGuid          m_looterGuid;
        Rolls               RollId;
        BoundInstancesMap   m_boundInstances[MAX_BOUND];
        uint8*              m_subGroupsCounts;
        ObjectGuid          m_guid;
        uint32              m_counter;                      // used only in SMSG_PARTY_UPDATE
        uint32              m_maxEnchantingLevel;
        uint32              m_dbStoreId;                    // Represents the ID used in database (Can be reused by other groups if group was disbanded)
        uint8               m_readyCheckCount;
        uint8               m_aoe_slots;                    // centrilize aoe loot method
        bool                m_readyCheck;

        std::array<std::unique_ptr<RaidMarker>, RAID_MARKERS_COUNT> m_markers;
        uint32              m_activeMarkers;
};
#endif
