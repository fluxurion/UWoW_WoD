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

#ifndef __BATTLEGROUNDMGR_H
#define __BATTLEGROUNDMGR_H

#include "Battleground.h"
#include "BattlegroundQueue.h"
#include "Common.h"
#include "DBCEnums.h"
#include "Object.h"

namespace WorldPackets
{
    namespace Battleground
    {
        struct BattlefieldStatusHeader;

        class BattlefieldStatusNone;
        class BattlefieldStatusNeedConfirmation;
        class BattlefieldStatusActive;
        class BattlefieldStatusQueued;
        class BattlefieldStatusFailed;
    }
}

uint32 const bgQuests[][2] = // for 100 lvl
{
    {38118, 38119}, //First bg week win
    {38129, 38130}, //First daily bg win
    {38172, 38174}, //Not first daily win
    {38140, 38141}, //BG loose

    {38171, 38173}, //Skirmish first win
    {38169, 38170}, //Skirmish win
    {38158, 38159}, //Skirmish loose
};

typedef std::map<uint32, Battleground*> BattlegroundSet;

typedef std::unordered_map<uint32, BattlegroundTypeId> BattleMastersMap;
typedef std::map<BattlegroundTypeId, uint8> BattlegroundSelectionWeightMap; // TypeId and its selectionWeight

#define WS_CURRENCY_RESET_TIME 20001                    // Custom worldstate

struct CreateBattlegroundData
{
    BattlegroundTypeId bgTypeId;
    Position TeamStartLoc[MAX_TEAMS];
    uint32 MinPlayersPerTeam;
    uint32 MaxPlayersPerTeam;
    uint32 LevelMin;
    uint32 LevelMax;
    uint32 MapID;
    uint32 scriptId;
    char* BattlegroundName;
    bool IsArena;
    bool IsRbg;
};

struct QueueSchedulerItem
{
    QueueSchedulerItem(uint32 MMRating, uint8 joinType, BattlegroundQueueTypeId bgQueueTypeId, BattlegroundTypeId bgTypeId, BattlegroundBracketId bracketid)
        : _MMRating (MMRating), _joinType (joinType), _bgQueueTypeId (bgQueueTypeId), _bgTypeId (bgTypeId), _bracket_id (bracketid) { }

    uint32 const _MMRating;
    uint8 const _joinType;
    BattlegroundQueueTypeId const _bgQueueTypeId;
    BattlegroundTypeId const _bgTypeId;
    BattlegroundBracketId const _bracket_id;
};

class BattlegroundMgr
{
    private:
        BattlegroundMgr();
        ~BattlegroundMgr();

    public:
        static BattlegroundMgr* instance()
        {
            static BattlegroundMgr instance;
            return &instance;
        }

        typedef std::vector<QueueSchedulerItem*> QueueSchedulerItems;
        typedef std::set<uint32> ClientBattlegroundIDs;

        void Update(uint32 diff);

        /* Packet Building */
        void SendBattlegroundList(Player* player, ObjectGuid const& guid, BattlegroundTypeId bgTypeId);
        void BuildBattlegroundStatusHeader(WorldPackets::Battleground::BattlefieldStatusHeader* battlefieldStatus, Battleground* bg, Player* player, uint32 ticketId, uint32 joinTime, uint32 arenaType);
        void BuildBattlegroundStatusNone(WorldPackets::Battleground::BattlefieldStatusNone* battlefieldStatus, Player* player, uint32 ticketId, uint32 joinTime, Battleground* bg, uint32 arenaType);
        void BuildBattlegroundStatusNeedConfirmation(WorldPackets::Battleground::BattlefieldStatusNeedConfirmation* battlefieldStatus, Battleground* bg, Player* player, uint32 ticketId, uint32 joinTime, uint32 timeout, uint32 arenaType);
        void BuildBattlegroundStatusActive(WorldPackets::Battleground::BattlefieldStatusActive* battlefieldStatus, Battleground* bg, Player* player, uint32 ticketId, uint32 joinTime, uint32 arenaType);
        void BuildBattlegroundStatusQueued(WorldPackets::Battleground::BattlefieldStatusQueued* battlefieldStatus, Battleground* bg, Player* player, uint32 ticketId, uint32 joinTime, uint32 avgWaitTime, uint32 arenaType, bool asGroup);
        void BuildBattlegroundStatusFailed(WorldPackets::Battleground::BattlefieldStatusFailed* battlefieldStatus, Battleground* bg, Player* player, uint32 ticketId, GroupJoinBattlegroundResult result, ObjectGuid const* errorGuid = nullptr);
        void SendAreaSpiritHealerQuery(Player* player, Battleground* bg, ObjectGuid const& guid);

        void BuildUpdateWorldStatePacket(WorldPacket* data, uint32 field, uint32 value);
        void BuildUpdateWorldStatePacket(WorldPacket* data, WorldStates variableID, uint32 value);

        /* Battlegrounds */
        Battleground* GetBattlegroundThroughClientInstance(uint32 instanceId, BattlegroundTypeId bgTypeId);
        Battleground* GetBattleground(uint32 InstanceID, BattlegroundTypeId bgTypeId);

        Battleground* GetBattlegroundTemplate(BattlegroundTypeId bgTypeId);
        Battleground* CreateNewBattleground(BattlegroundTypeId bgTypeId, PvPDifficultyEntry const* bracketEntry, uint8 arenaType, bool isRated, BattlegroundTypeId generatedType = BATTLEGROUND_TYPE_NONE);

        uint32 CreateBattleground(CreateBattlegroundData& data);

        void AddBattleground(uint32 InstanceID, BattlegroundTypeId bgTypeId, Battleground* BG) { m_Battlegrounds[bgTypeId][InstanceID] = BG; };
        void RemoveBattleground(uint32 instanceID, BattlegroundTypeId bgTypeId) { m_Battlegrounds[bgTypeId].erase(instanceID); }
        uint32 CreateClientVisibleInstanceId(BattlegroundTypeId bgTypeId, BattlegroundBracketId bracket_id);

        void CreateInitialBattlegrounds();
        void InitAutomaticArenaPointDistribution();
        void DeleteAllBattlegrounds();

        void SendToBattleground(Player* player, uint32 InstanceID, BattlegroundTypeId bgTypeId);

        /* Battleground queues */
        //these queues are instantiated when creating BattlegroundMrg
        BattlegroundQueue& GetBattlegroundQueue(BattlegroundQueueTypeId bgQueueTypeId) { return m_BattlegroundQueues[bgQueueTypeId]; }
        BattlegroundQueue m_BattlegroundQueues[MAX_BATTLEGROUND_QUEUE_TYPES]; // public, because we need to access them in BG handler code

        BGFreeSlotQueueType BGFreeSlotQueue[MAX_BATTLEGROUND_TYPE_ID];

        void ScheduleQueueUpdate(uint32 arenaMatchmakerRating, uint8 arenaType, BattlegroundQueueTypeId bgQueueTypeId, BattlegroundTypeId bgTypeId, BattlegroundBracketId bracket_id);
        uint32 GetMaxRatingDifference() const;
        uint32 GetRatingDiscardTimer()  const;
        uint32 GetPrematureFinishTime() const;

        void ToggleTesting();

        void LoadBattleMastersEntry();
        BattlegroundTypeId GetBattleMasterBG(uint32 entry) const
        {
            BattleMastersMap::const_iterator itr = mBattleMastersMap.find(entry);
            if (itr != mBattleMastersMap.end())
                return itr->second;

            return BATTLEGROUND_WS;
        }

        bool isTesting() const { return m_Testing; }

        static bool IsArenaType(BattlegroundTypeId bgTypeId);
        static bool IsBattlegroundType(BattlegroundTypeId bgTypeId) { return !IsArenaType(bgTypeId); }
        static BracketType BracketByJoinType(uint8 joinType);
        static uint8 GetJoinTypeByBracketSlot(uint8 slot);
        static BattlegroundQueueTypeId BGQueueTypeId(BattlegroundTypeId bgTypeId, uint8 arenaType);
        static BattlegroundTypeId BGTemplateId(BattlegroundQueueTypeId bgQueueTypeId);
        static uint8 BGJoinType(BattlegroundQueueTypeId bgQueueTypeId);

        BattlegroundSelectionWeightMap* GetArenaSelectionWeight() { return &m_ArenaSelectionWeights; }
        BattlegroundSelectionWeightMap* GetRBGSelectionWeight() { return &m_BGSelectionWeights; }
    private:
        BattleMastersMap    mBattleMastersMap;

        /* Battlegrounds */
        BattlegroundSet m_Battlegrounds[MAX_BATTLEGROUND_TYPE_ID];
        BattlegroundSelectionWeightMap m_ArenaSelectionWeights;
        BattlegroundSelectionWeightMap m_BGSelectionWeights;
        QueueSchedulerItems m_QueueUpdateScheduler;
        ClientBattlegroundIDs m_ClientBattlegroundIds[MAX_BATTLEGROUND_TYPE_ID][MAX_BATTLEGROUND_BRACKETS]; //the instanceids just visible for the client
        uint32 m_NextRatedArenaUpdate;
        bool   m_Testing;
};

#define sBattlegroundMgr BattlegroundMgr::instance()

#endif

