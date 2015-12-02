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

#include "Arena.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "BattlegroundPackets.h"
#include "Bracket.h"
#include "BracketMgr.h"
#include "Creature.h"
#include "Duration.h"
#include "Formulas.h"
#include "GameObjectPackets.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "GroupMgr.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "InstancePackets.h"
#include "Language.h"
#include "MapManager.h"
#include "MiscPackets.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "Util.h"
#include "World.h"
#include "WorldPacket.h"

namespace Trinity
{
    class BattlegroundChatBuilder
    {
        public:
            BattlegroundChatBuilder(ChatMsg msgtype, int32 textId, Player const* source, va_list* args = nullptr)
                : _msgtype(msgtype), _textId(textId), _source(source), _args(args) { }

            void operator()(WorldPacket& data, LocaleConstant loc_idx)
            {
                char const* text = sObjectMgr->GetTrinityString(_textId, loc_idx);
                if (_args)
                {
                    // we need copy va_list before use or original va_list will corrupted
                    va_list ap;
                    va_copy(ap, *_args);

                    char str[2048];
                    vsnprintf(str, 2048, text, ap);
                    va_end(ap);

                    do_helper(data, &str[0]);
                }
                else
                    do_helper(data, text);
            }

        private:
            void do_helper(WorldPacket& data, char const* text)
            {
                Trinity::ChatData c;
                c.targetGuid = _source ? _source->GetGUID() : ObjectGuid::Empty;
                c.sourceGuid = _source ? _source->GetGUID() : ObjectGuid::Empty;
                c.chatTag = _source ? _source->GetChatTag() : 0;
                c.message = text;
                c.chatType = _msgtype;

                Trinity::BuildChatPacket(data, c);
            }

            ChatMsg _msgtype;
            int32 _textId;
            Player const* _source;
            va_list* _args;
    };

    class Battleground2ChatBuilder
    {
        public:
            Battleground2ChatBuilder(ChatMsg msgtype, int32 textId, Player const* source, int32 arg1, int32 arg2)
                : _msgtype(msgtype), _textId(textId), _source(source), _arg1(arg1), _arg2(arg2) { }

            void operator()(WorldPacket& data, LocaleConstant loc_idx)
            {
                char const* text = sObjectMgr->GetTrinityString(_textId, loc_idx);
                char const* arg1str = _arg1 ? sObjectMgr->GetTrinityString(_arg1, loc_idx) : "";
                char const* arg2str = _arg2 ? sObjectMgr->GetTrinityString(_arg2, loc_idx) : "";

                char str[2048];
                snprintf(str, 2048, text, arg1str, arg2str);

                Trinity::ChatData c;
                c.targetGuid = _source ? _source->GetGUID() : ObjectGuid::Empty;
                c.sourceGuid = _source ? _source->GetGUID() : ObjectGuid::Empty;
                c.chatTag = _source ? _source->GetChatTag() : 0;
                c.message = str;
                c.chatType = _msgtype;

                Trinity::BuildChatPacket(data, c);
            }

        private:
            ChatMsg _msgtype;
            int32 _textId;
            Player const* _source;
            int32 _arg1;
            int32 _arg2;
    };
}                                                           // namespace Trinity

template<class Do>
void Battleground::BroadcastWorker(Do& _do)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            _do(player);
}

Battleground::Battleground()
{
    m_QueueID              = 0;
    m_TypeID            = BATTLEGROUND_TYPE_NONE;
    m_RandomTypeID      = BATTLEGROUND_TYPE_NONE;
    m_InstanceID        = 0;
    m_Status            = STATUS_NONE;
    m_ClientInstanceID  = 0;
    m_EndTime           = Milliseconds(0);
    m_LastResurrectTime = 0;
    m_BracketId         = BG_BRACKET_ID_FIRST;
    m_InvitedAlliance   = 0;
    m_InvitedHorde      = 0;
    m_JoinType = JOIN_TYPE_NONE;
    m_IsArena           = false;
    m_Winner            = 2;

    m_StartTime = Milliseconds(0);
    m_CountdownTimer = Milliseconds(0);
    m_ResetStatTimer    = 0;

    m_Events            = 0;
    m_IsRated           = false;
    m_IsTournament = false;
    m_BuffChange        = false;
    m_IsRandom          = false;
    m_Name              = "";
    m_LevelMin          = 0;
    m_LevelMax          = 0;
    m_InBGFreeSlotQueue = false;
    m_SetDeleteThis     = false;

    m_MaxPlayersPerTeam = 0;
    m_MaxPlayers        = 0;
    m_MinPlayersPerTeam = 0;
    m_MinPlayers        = 0;

    m_MapId             = 0;
    m_Map               = nullptr;

    m_PrematureCountDown = false;

    StartMessageIds[BG_STARTING_EVENT_FIRST] = LANG_BG_WS_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_WS_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD] = LANG_BG_WS_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_WS_HAS_BEGUN;

    for (uint8 i = BG_STARTING_EVENT_FIRST; i < BG_STARTING_EVENT_COUNT; ++i)
    {
        m_broadcastMessages[i] = BattlegroundBroadcastTexts[i];
        m_hasBroadcasts[i] = false;
    }

    m_IsRBG = false;

    m_sameBgTeamId = false;

    m_LastPlayerPositionBroadcast = PositionBroadcastUpdate;

    m_StartDelayTime = Milliseconds(0);

    for (int8 i = TEAM_ALLIANCE; i < MAX_TEAMS; ++i)
    {
        m_TeamStartPos[i] = { };
        m_GroupIds[i] = 0;
        m_BgRaids[i] = nullptr;
        m_PlayersCount[i] = 0;
        m_TeamScores[i] = 0;
    }

    m_lastFlagCaptureTeam = 0;
}

Battleground::~Battleground()
{
    // remove objects and creatures
    // (this is done automatically in mapmanager update, when the instance is reset after the reset time)
    uint32 size = uint32(BgCreatures.size());
    for (uint32 i = 0; i < size; ++i)
        DelCreature(i);

    size = uint32(BgObjects.size());
    for (uint32 i = 0; i < size; ++i)
        DelObject(i);

    sBattlegroundMgr->RemoveBattleground(GetInstanceID(), GetTypeID());
    // unload map
    if (m_Map)
    {
        m_Map->SetUnload();
        //unlink to prevent crash, always unlink all pointer reference before destruction
        m_Map->SetBG(nullptr);
        m_Map = nullptr;
    }
    // remove from bg free slot queue
    RemoveFromBGFreeSlotQueue();

    for (BattlegroundScoreMap::const_iterator itr = PlayerScores.begin(); itr != PlayerScores.end(); ++itr)
        delete itr->second;
}

void Battleground::Update(uint32 diff)
{
    if (!PreUpdateImpl(diff))
        return;

    if (!GetPlayersSize())
    {
        //BG is empty
        // if there are no players invited, delete BG
        // this will delete arena or bg object, where any player entered
        // [[   but if you use battleground object again (more battles possible to be played on 1 instance)
        //      then this condition should be removed and code:
        //      if (!GetInvitedCount(HORDE) && !GetInvitedCount(ALLIANCE))
        //          AddToFreeBGObjectsQueue(); // not yet implemented
        //      should be used instead of current
        // ]]
        // Battleground Template instance cannot be updated, because it would be deleted
        if (!GetInvitedCount(HORDE) && !GetInvitedCount(ALLIANCE))
            m_SetDeleteThis = true;
        return;
    }

    switch (GetStatus())
    {
        case STATUS_WAIT_JOIN:
        {
            if (GetPlayersSize())
                _ProcessJoin(diff);
            break;
        }
        case STATUS_IN_PROGRESS:
        {
            _ProcessOfflineQueue();
            if (!isArena())
            {
                _ProcessPlayerPositionBroadcast(Milliseconds(diff));
                _ProcessRessurect(diff);
                if (sBattlegroundMgr->GetPrematureFinishTime() && (GetPlayersCountByTeam(ALLIANCE) < GetMinPlayersPerTeam() || GetPlayersCountByTeam(HORDE) < GetMinPlayersPerTeam()))
                    _ProcessProgress(diff);
                else if (m_PrematureCountDown)
                    m_PrematureCountDown = false;
            }
            break;
        }
        case STATUS_WAIT_LEAVE:
            _ProcessLeave(diff);
            break;
        default:
            break;
    }

    // Update start time and reset stats timer
    SetElapsedTime(GetElapsedTime() + Milliseconds(diff));
    if (GetStatus() == STATUS_WAIT_JOIN)
    {
        m_ResetStatTimer += diff;
        m_CountdownTimer += Milliseconds(diff);
    }

    PostUpdateImpl(diff);
    PostUpdateImpl(Milliseconds(diff));
}

inline void Battleground::_ProcessOfflineQueue()
{
    // remove offline players from bg after 5 minutes
    if (!m_OfflineQueue.empty())
    {
        BattlegroundPlayerMap::iterator itr = m_Players.find(*(m_OfflineQueue.begin()));
        if (itr != m_Players.end())
        {
            if (itr->second.OfflineRemoveTime <= sWorld->GetGameTime())
            {
                RemovePlayerAtLeave(itr->first, true, true);// remove player from BG
                m_OfflineQueue.pop_front();                 // remove from offline queue
                //do not use itr for anything, because it is erased in RemovePlayerAtLeave()
            }
        }
    }
}

inline void Battleground::_ProcessRessurect(uint32 diff)
{
    // *********************************************************
    // ***        BATTLEGROUND RESSURECTION SYSTEM           ***
    // *********************************************************
    // this should be handled by spell system
    m_LastResurrectTime += diff;
    if (m_LastResurrectTime >= RESURRECTION_INTERVAL)
    {
        if (GetReviveQueueSize())
        {
            for (std::map<ObjectGuid, GuidVector >::iterator itr = m_ReviveQueue.begin(); itr != m_ReviveQueue.end(); ++itr)
            {
                Creature* sh = nullptr;
                for (GuidVector::const_iterator itr2 = (itr->second).begin(); itr2 != (itr->second).end(); ++itr2)
                {
                    Player* player = ObjectAccessor::FindPlayer(*itr2);
                    if (!player)
                        continue;

                    if (!sh && player->IsInWorld())
                    {
                        sh = player->GetMap()->GetCreature(itr->first);
                        if (sh)
                            sh->CastSpell(sh, SPELL_BG_SPIRIT_HEAL, true);
                    }

                    // Resurrection visual
                    player->CastSpell(player, SPELL_BG_RESURRECTION_VISUAL, true);
                    m_ResurrectQueue.push_back(*itr2);
                }
                (itr->second).clear();
            }

            m_ReviveQueue.clear();
            m_LastResurrectTime = 0;
        }
        else
            // queue is clear and time passed, just update last resurrection time
            m_LastResurrectTime = 0;
    }
    else if (m_LastResurrectTime > 500)    // Resurrect players only half a second later, to see spirit heal effect on NPC
    {
        for (GuidVector::const_iterator itr = m_ResurrectQueue.begin(); itr != m_ResurrectQueue.end(); ++itr)
        {
            Player* player = ObjectAccessor::FindPlayer(*itr);
            if (!player)
                continue;
            player->ResurrectPlayer(1.0f);
            player->CastSpell(player, 6962, true);
            player->CastSpell(player, SPELL_BG_SPIRIT_HEAL_MANA, true);
            sObjectAccessor->ConvertCorpseForPlayer(*itr);
        }
        m_ResurrectQueue.clear();
    }
}

inline void Battleground::_ProcessProgress(uint32 diff)
{
    // *********************************************************
    // ***           BATTLEGROUND BALLANCE SYSTEM            ***
    // *********************************************************
    // if less then minimum players are in on one side, then start premature finish timer
    if (!m_PrematureCountDown)
    {
        m_PrematureCountDown = true;
        m_PrematureCountDownTimer = sBattlegroundMgr->GetPrematureFinishTime();
    }
    else if (m_PrematureCountDownTimer < diff)
    {
        // time's up!
        uint32 winner = 0;
        if (GetPlayersCountByTeam(ALLIANCE) >= GetMinPlayersPerTeam())
            winner = ALLIANCE;
        else if (GetPlayersCountByTeam(HORDE) >= GetMinPlayersPerTeam())
            winner = HORDE;

        EndBattleground(winner);
        m_PrematureCountDown = false;
    }
    else if (!sBattlegroundMgr->isTesting())
    {
        uint32 newtime = m_PrematureCountDownTimer - diff;
        // announce every minute
        if (newtime > (MINUTE * IN_MILLISECONDS))
        {
            if (newtime / (MINUTE * IN_MILLISECONDS) != m_PrematureCountDownTimer / (MINUTE * IN_MILLISECONDS))
                PSendMessageToAll(LANG_BATTLEGROUND_PREMATURE_FINISH_WARNING, CHAT_MSG_SYSTEM, nullptr, (uint32)(m_PrematureCountDownTimer / (MINUTE * IN_MILLISECONDS)));
        }
        else
        {
            //announce every 15 seconds
            if (newtime / (15 * IN_MILLISECONDS) != m_PrematureCountDownTimer / (15 * IN_MILLISECONDS))
                PSendMessageToAll(LANG_BATTLEGROUND_PREMATURE_FINISH_WARNING_SECS, CHAT_MSG_SYSTEM, nullptr, (uint32)(m_PrematureCountDownTimer / IN_MILLISECONDS));
        }
        m_PrematureCountDownTimer = newtime;
    }
}

inline void Battleground::_ProcessJoin(uint32 diff)
{
    // *********************************************************
    // ***           BATTLEGROUND STARTING SYSTEM            ***
    // *********************************************************
    ModifyStartDelayTime(Milliseconds(diff));

    // I know it's a too big but it's the value sent in packet, I get it from retail sniff.
    // I think it's link to the countdown when bgs start
    SetRemainingTime(Minutes(5));

    if (m_ResetStatTimer > 5000)
    {
        m_ResetStatTimer = 0;
        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                player->ResetAllPowers(true);
    }

    // Send packet every 10 seconds until the 2nd field reach 0
    if (m_CountdownTimer >= Seconds(10) && GetElapsedTime() < Minutes(2))
    {
        m_CountdownTimer = Seconds(0);

        Minutes countdownMaxForBGType = isArena() ? Minutes(1) : Minutes(2);

        WorldPackets::Instance::StartTimer startTimer;
        startTimer.Type = WORLD_TIMER_TYPE_PVP;
        startTimer.TimeRemaining = countdownMaxForBGType - std::chrono::duration_cast<Seconds>(GetElapsedTime());
        startTimer.TotalTime = countdownMaxForBGType;
        SendPacketToAll(startTimer.Write());
    }

    if (!(m_Events & BG_STARTING_EVENT_1))
    {
        m_Events |= BG_STARTING_EVENT_1;

        if (!FindBgMap())
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::_ProcessJoin: map (map id: %u, instance id: %u) is not created!", m_MapId, m_InstanceID);
            EndNow();
            return;
        }

        // Setup here, only when at least one player has ported to the map
        if (!SetupBattleground())
        {
            EndNow();
            return;
        }

        StartingEventCloseDoors();
        SetStartDelayTime(isArena() ? m_messageTimer[BG_STARTING_EVENT_FIRST] / 2 : m_messageTimer[BG_STARTING_EVENT_FIRST]);

        if (m_hasBroadcasts[BG_STARTING_EVENT_FIRST])
            SendBroadcastTextToAll(m_broadcastMessages[BG_STARTING_EVENT_FIRST], CHAT_MSG_BG_SYSTEM_NEUTRAL);
        else
            SendWarningToAll(StartMessageIds[BG_STARTING_EVENT_FIRST]);
    }
    else if (GetStartDelayTime() <= (isArena() ? m_messageTimer[BG_STARTING_EVENT_SECOND] / 2 : m_messageTimer[BG_STARTING_EVENT_SECOND]) && !(m_Events & BG_STARTING_EVENT_2))
    {
        m_Events |= BG_STARTING_EVENT_2;

        if (m_hasBroadcasts[BG_STARTING_EVENT_SECOND])
            SendBroadcastTextToAll(m_broadcastMessages[BG_STARTING_EVENT_SECOND], CHAT_MSG_BG_SYSTEM_NEUTRAL);
        else
            SendWarningToAll(StartMessageIds[BG_STARTING_EVENT_SECOND]);
    }
    else if (GetStartDelayTime() <= (isArena() ? m_messageTimer[BG_STARTING_EVENT_THIRD] / 2 : m_messageTimer[BG_STARTING_EVENT_THIRD]) && !(m_Events & BG_STARTING_EVENT_3))
    {
        m_Events |= BG_STARTING_EVENT_3;

        if (m_hasBroadcasts[BG_STARTING_EVENT_THIRD])
            SendBroadcastTextToAll(m_broadcastMessages[BG_STARTING_EVENT_THIRD], CHAT_MSG_BG_SYSTEM_NEUTRAL);
        else
            SendWarningToAll(StartMessageIds[BG_STARTING_EVENT_THIRD]);
    }
    else if (GetStartDelayTime() <= m_messageTimer[BG_STARTING_EVENT_FOURTH] && !(m_Events & BG_STARTING_EVENT_4))
    {
        m_Events |= BG_STARTING_EVENT_4;

        StartingEventOpenDoors();

        if (m_hasBroadcasts[BG_STARTING_EVENT_FOURTH])
            SendBroadcastTextToAll(m_broadcastMessages[BG_STARTING_EVENT_FOURTH], CHAT_MSG_BG_SYSTEM_NEUTRAL);
        else
            SendWarningToAll(StartMessageIds[BG_STARTING_EVENT_FOURTH], CHAT_MSG_BG_SYSTEM_NEUTRAL);

        SetStatus(STATUS_IN_PROGRESS);
        SetStartDelayTime(m_messageTimer[BG_STARTING_EVENT_FOURTH]);

        if (!isArena())
        {
            PlaySoundToAll(BG_SOUND_START);

            for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                {
                    player->RemoveAurasDueToSpell(SPELL_BG_PREPARATION);
                    player->ResetAllPowers();
                }
            // Announce BG starting
            if (sWorld->getBoolConfig(CONFIG_BATTLEGROUND_QUEUE_ANNOUNCER_ENABLE))
                sWorld->SendWorldText(LANG_BG_STARTED_ANNOUNCE_WORLD, GetName(), GetMinLevel(), GetMaxLevel());
        }
    }

    if (GetRemainingTime() > Milliseconds(0) && (m_EndTime -= Milliseconds(diff)) > Milliseconds(0))
        SetRemainingTime(GetRemainingTime() - Milliseconds(diff));
}

inline void Battleground::_ProcessLeave(uint32 diff)
{
    // *********************************************************
    // ***           BATTLEGROUND ENDING SYSTEM              ***
    // *********************************************************
    // remove all players from battleground after 2 minutes
    SetRemainingTime(GetRemainingTime() - Milliseconds(diff));
    if (GetRemainingTime() <= Milliseconds(0))
    {
        SetRemainingTime(Milliseconds(0));
        BattlegroundPlayerMap::iterator itr, next;
        for (itr = m_Players.begin(); itr != m_Players.end(); itr = next)
        {
            next = itr;
            ++next;
            //itr is erased here!
            RemovePlayerAtLeave(itr->first, true, true);// remove player from BG
            // do not change any battleground's private variables
        }
    }
}

inline Player* Battleground::GetPlayer(ObjectGuid guid, bool offlineRemove, const char* context) const
{
    Player* player = nullptr;
    if (!offlineRemove)
    {
        player = ObjectAccessor::FindPlayer(guid);
        if (!player)
            sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::%s: player (GUID: %u) not found for BG (map: %u, instance id: %u)!",
            context, guid.GetCounter(), m_MapId, m_InstanceID);
    }
    return player;
}

inline Player* Battleground::GetPlayer(BattlegroundPlayerMap::iterator itr, const char* context)
{
    return GetPlayer(itr->first, itr->second.OfflineRemoveTime, context);
}

inline Player* Battleground::GetPlayer(BattlegroundPlayerMap::const_iterator itr, const char* context) const
{
    return GetPlayer(itr->first, itr->second.OfflineRemoveTime, context);
}

inline Player* Battleground::GetPlayerForTeam(uint32 teamId, BattlegroundPlayerMap::const_iterator itr, const char* context) const
{
    Player* player = GetPlayer(itr, context);
    if (player)
    {
        uint32 team = itr->second.Team;
        if (!team)
            team = player->GetBGTeam();
        if (team != teamId)
            player = nullptr;
    }
    return player;
}

void Battleground::SetTeamStartPosition(TeamId teamId, Position const& pos)
{
    ASSERT(teamId < TEAM_NEUTRAL);
    m_TeamStartPos[teamId] = pos;
}

Position const* Battleground::GetTeamStartPosition(TeamId teamId) const
{
    ASSERT(teamId < TEAM_NEUTRAL);
    return &m_TeamStartPos[teamId];
}

void Battleground::SendPacketToAll(WorldPacket const* packet)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = GetPlayer(itr, "SendPacketToAll"))
            player->GetSession()->SendPacket(packet);
}

void Battleground::SendPacketToTeam(uint32 TeamID, WorldPacket const* packet, Player* sender, bool self)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = GetPlayerForTeam(TeamID, itr, "SendPacketToTeam"))
            if (self || sender != player)
                player->GetSession()->SendPacket(packet);
}

void Battleground::PlaySoundToAll(uint32 soundKitID, ObjectGuid sourceGuid /*= ObjectGuid::Empty*/)
{
    SendPacketToAll(WorldPackets::Misc::PlaySound(sourceGuid, soundKitID).Write());
}

void Battleground::PlaySoundToTeam(uint32 soundKitID, uint32 TeamID)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = GetPlayerForTeam(TeamID, itr, "PlaySoundToTeam"))
            player->GetSession()->SendPacket(WorldPackets::Misc::PlaySound(ObjectGuid::Empty, soundKitID).Write());
}

void Battleground::CastSpellOnTeam(uint32 SpellID, uint32 TeamID)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = GetPlayerForTeam(TeamID, itr, "CastSpellOnTeam"))
            player->CastSpell(player, SpellID, true);
}

void Battleground::RemoveAuraOnTeam(uint32 SpellID, uint32 TeamID)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = GetPlayerForTeam(TeamID, itr, "RemoveAuraOnTeam"))
            player->RemoveAura(SpellID);
}

void Battleground::YellToAll(Creature* creature, const char* text, uint32 language)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = GetPlayer(itr, "YellToAll"))
        {
            WorldPacket data(SMSG_CHAT, 200);
            creature->BuildMonsterChat(&data, CHAT_MSG_MONSTER_YELL, text, language, creature->GetName(), itr->first);
            player->GetSession()->SendPacket(&data);
        }
}

void Battleground::RewardHonorToTeam(uint32 Honor, uint32 TeamID)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = GetPlayerForTeam(TeamID, itr, "RewardHonorToTeam"))
            UpdatePlayerScore(player, SCORE_BONUS_HONOR, Honor);
}

void Battleground::RewardReputationToTeam(uint32 factionIDAlliance, uint32 factionIDHorde, uint32 reputation, uint32 teamID)
{
    if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(teamID == ALLIANCE ? factionIDAlliance : factionIDHorde))
        for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
            if (Player* player = GetPlayerForTeam(teamID, itr, "RewardReputationToTeam"))
                player->GetReputationMgr().ModifyReputation(factionEntry, reputation);
}

void Battleground::UpdateWorldState(uint32 Field, uint32 Value)
{
    WorldPacket data;
    sBattlegroundMgr->BuildUpdateWorldStatePacket(&data, Field, Value);
    SendPacketToAll(&data);
}

void Battleground::UpdateWorldState(WorldStates variableID, uint32 value)
{
    WorldPacket data;
    sBattlegroundMgr->BuildUpdateWorldStatePacket(&data, variableID, value);
    SendPacketToAll(&data);
}

void Battleground::UpdateWorldStateForPlayer(WorldStates variableID, uint32 value, Player* Source)
{
    WorldPacket data;
    sBattlegroundMgr->BuildUpdateWorldStatePacket(&data, variableID, value);
    Source->GetSession()->SendPacket(&data);
}

void Battleground::SendBattleGroundPoints(bool isHorde, int32 teamScores, bool broadcast /*= true*/, Player* player /*= nullptr*/)
{
    WorldPackets::Battleground::Points point;
    point.BgPoints = teamScores;
    point.Team = isHorde;
    if (broadcast)
        SendPacketToAll(point.Write());
    else
        player->SendDirectMessage(point.Write());
}

void Battleground::EndBattleground(uint32 winner)
{
    RemoveFromBGFreeSlotQueue();

    int32 broadcastID = 0;

    BracketType bType = BattlegroundMgr::BracketByJoinType(GetJoinType());

    if (winner == ALLIANCE)
    {
        broadcastID = isBattleground() ? 7335 : 63033;
        PlaySoundToAll(BG_SOUND_ALLIANCE_WIN);                // alliance wins sound
        SetWinner(WINNER_ALLIANCE);
    }
    else if (winner == HORDE)
    {
        broadcastID = isBattleground() ? 7336 : 63037;
        PlaySoundToAll(BG_SOUND_HORDE_WIN);                   // horde wins sound
        SetWinner(WINNER_HORDE);
    }
    else
        SetWinner(3);

    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BATTLEGROUND: EndBattleground. JounType: %u, Bracket %u, Winer %u", GetJoinType(), bType, winner);

    SetStatus(STATUS_WAIT_LEAVE);
    //we must set it this way, because end time is sent in packet!
    SetRemainingTime(Minutes(2));

    bool guildAwarded = false;
    uint8 aliveWinners = GetAlivePlayersCountByTeam(winner);
    std::ostringstream info;

    for (BattlegroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        uint32 team = itr->second.Team;
        Player* player = GetPlayer(itr, "EndBattleground");
        if (!player)
            continue;

        Bracket* bracket = GetJoinType() ? player->getBracket(bType) : nullptr;

        // should remove spirit of redemption
        if (player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
            player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

        if (!player->isAlive())
        {
            player->ResurrectPlayer(1.0f);
            player->SpawnCorpseBones();
        }
        else
        {
            //needed cause else in av some creatures will kill the players at the end
            player->CombatStop();
            player->getHostileRefManager().deleteReferences();
        }

        // remove temporary currency bonus auras before rewarding player
        player->RemoveAura(SPELL_BG_HONORABLE_DEFENDER_25Y);
        player->RemoveAura(SPELL_BG_HONORABLE_DEFENDER_60Y);

        PlayerReward(player, team == winner, (!isArena() && !isRated()) ? 0 : 2, false /*firstWeekly*/, !player->GetRandomWinner());

        // Reward winner team
        if (team == winner)
        {
            if (isBattleground() && !IsRBG() && IsRandom())
            {
                if (!player->GetRandomWinner())
                    player->SetRandomWinner(true);
            }

            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_BG, 1);
            if (!guildAwarded)
            {
                guildAwarded = true;
                if (ObjectGuid::LowType guildId = GetBgMap()->GetOwnerGuildId(player->GetTeam()))
                    if (Guild* guild = sGuildMgr->GetGuildById(guildId))
                    {
                        guild->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_BG, 1, 0, 0, nullptr, player);

                    }
            }
        }

        player->ResetAllPowers();
        player->CombatStopWithPets(true);

        BlockMovement(player);

        if (IsRBG())
        {
            if (player->GetBGTeam() != team)
                player->setFactionForRace(player->getRace());

            PlayerReward(player, team == winner, 1, false, false);

            player->getBracket(BRACKET_TYPE_RATED_BG)->FinishGame(team == winner, GetMatchmakerRating(team == winner ? GetOtherTeam(winner) : winner));
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_BATTLEGROUND, GetMapId());
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_REACH_RBG_RATING, std::max<uint32>(bracket->getRating(), 1));
        }

        BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(GetTypeID(), GetJoinType());

        WorldPackets::Battleground::BattlefieldStatusActive battlefieldStatus;
        sBattlegroundMgr->BuildBattlegroundStatusActive(&battlefieldStatus, this, player, player->GetBattlegroundQueueIndex(bgQueueTypeId), player->GetBattlegroundQueueJoinTime(isArena() ? BATTLEGROUND_AA : GetTypeID()), GetJoinType());
        player->SendDirectMessage(battlefieldStatus.Write());

        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND, 1);
    }

    WorldPackets::Battleground::PVPLogData pvpLogData;
    BuildPvPLogDataPacket(pvpLogData);
    SendPacketToAll(pvpLogData.Write());

    if (broadcastID)
        SendBroadcastTextToAll(broadcastID, CHAT_MSG_BG_SYSTEM_NEUTRAL);
}

void Battleground::PlayerReward(Player* player, bool isWinner, uint8 type, bool firstWeekly, bool firstDaily)
{
    uint8 t = 0;
    switch (type)
    {
        case 0: // bg
            if (isWinner)
            {
                if (firstWeekly)
                    t = 0;
                else if (firstDaily)
                    t = 1;
                else
                    t = 2;
            }
            else
                t = 3;
            break;
        case 1: // rated bg
            //player->ModifyCurrency(CURRENCY_TYPE_CONQUEST_RATED_BG_META, sWorld->getIntConfig(CONFIG_CURRENCY_CONQUEST_POINTS_RBG_REWARD));
            break;
        case 2: // skirmish
            if (isWinner)
                t = firstDaily ? 4 : 5;
            else
                t = 6;
            break;
        default:
            break;
    }

    auto quest = sObjectMgr->GetQuestTemplate(bgQuests[t][player->GetBGTeam() == ALLIANCE ? 0 : 1]);
    if (quest)
        player->RewardQuest(quest, 0, nullptr, false);
}

uint32 Battleground::GetBonusHonorFromKill(uint32 kills) const
{
    //variable kills means how many honorable kills you scored (so we need kills * honor_for_one_kill)
    uint32 maxLevel = std::min(GetMaxLevel(), 100U);
    return Trinity::Honor::hk_honor_at_level(maxLevel, float(kills));
}

void Battleground::BlockMovement(Player* player)
{
    player->SetClientControl(player, false);                          // movement disabled NOTE: the effect will be automatically removed by client when the player is teleported from the battleground, so no need to send with uint8(1) in RemovePlayerAtLeave()
}

void Battleground::RemovePlayerAtLeave(ObjectGuid guid, bool Transport, bool SendPacket)
{
    uint32 team = GetPlayerTeam(guid);
    bool participant = false;
    // Remove from lists/maps
    BattlegroundPlayerMap::iterator itr = m_Players.find(guid);
    if (itr != m_Players.end())
    {
        UpdatePlayersCountByTeam(team, true);               // -1 player
        m_Players.erase(itr);
        // check if the player was a participant of the match, or only entered through gm command (goname)
        participant = true;
    }

    RemovePlayerFromResurrectQueue(guid);

    Player* player = ObjectAccessor::FindPlayer(guid);

    // should remove spirit of redemption
    if (player)
    {
        if (player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
            player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

        if (!player->isAlive())                              // resurrect on exit
        {
            player->ResurrectPlayer(1.0f);
            player->SpawnCorpseBones();
        }

        if (IsRBG())
            if (player->GetBGTeam() != team)
                player->setFactionForRace(player->getRace());

        player->RemoveAura(SPELL_BATTLE_FATIGUE);
    }

    RemovePlayer(player, guid, team);                           // BG subclass specific code

    BattlegroundTypeId bgTypeId = GetTypeID();
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(GetTypeID(), GetJoinType());

    if (participant) // if the player was a match participant, remove auras, calc rating, update queue
    {
        if (player)
        {
            player->ClearAfkReports();

            if (!team)
                team = player->GetBGTeam();

            // if arena, remove the specific arena auras
            if (isArena() || IsRBG())
            {
                // set the bg type to all arenas & rbg (it will be used for queue refreshing)
                bgTypeId = isArena() ? BATTLEGROUND_AA : BATTLEGROUND_RATED_10_VS_10;

                // unsummon current and summon old pet if there was one and there isn't a current pet
                player->RemovePet(nullptr);
                player->ResummonPetTemporaryUnSummonedIfAny();

                if (isRated() && GetStatus() == STATUS_IN_PROGRESS)
                {
                    //left a rated match while the encounter was in progress, consider as loser
                    BracketType bType = BattlegroundMgr::BracketByJoinType(GetJoinType());
                    Bracket* bracket = player->getBracket(bType);
                    ASSERT(bracket);    //shouldn't happend
                    bracket->FinishGame(false/*lost*/, GetMatchmakerRating(GetOtherTeam(team)));
                }
            }
            if (SendPacket)
            {
                WorldPackets::Battleground::BattlefieldStatusNone battlefieldStatus;
                sBattlegroundMgr->BuildBattlegroundStatusNone(&battlefieldStatus, player, player->GetBattlegroundQueueIndex(bgQueueTypeId), player->GetBattlegroundQueueJoinTime(bgQueueTypeId), nullptr, 0);
                player->SendDirectMessage(battlefieldStatus.Write());
            }

            // this call is important, because player, when joins to battleground, this method is not called, so it must be called when leaving bg
            player->RemoveBattlegroundQueueId(bgQueueTypeId);
        }
        else // removing offline participant
        {
            if (isRated() && GetStatus() == STATUS_IN_PROGRESS)
            {
                //left a rated match while the encounter was in progress, consider as loser
                BracketType bType = BattlegroundMgr::BracketByJoinType(GetJoinType());
                Bracket* bracket = sBracketMgr->TryGetOrCreateBracket(guid, bType);
                ASSERT(bracket);    //shouldn't happend
                bracket->FinishGame(false/*lost*/, GetMatchmakerRating(GetOtherTeam(team)));
            }
        }

        // remove from raid group if player is member
        if (Group* group = GetBgRaid(team))
            if (!group->RemoveMember(guid))                // group was disbanded
                SetBgRaid(team, nullptr);

        DecreaseInvitedCount(team);
        //we should update battleground queue, but only if bg isn't ending
        if (isBattleground() && GetStatus() < STATUS_WAIT_LEAVE)
        {
            // a player has left the battleground, so there are free slots -> add to queue
            AddToBGFreeSlotQueue();
            sBattlegroundMgr->ScheduleQueueUpdate(0, 0, bgQueueTypeId, bgTypeId, GetBracketId());
        }

        SendPacketToTeam(team, WorldPackets::Battleground::PlayerLeft(guid).Write(), player, false);
    }

    if (player)
    {
        // Do next only if found in battleground
        player->SetBattlegroundId(0, BATTLEGROUND_TYPE_NONE);  // We're not in BG.
        // reset destination bg team
        player->SetBGTeam(0);
        player->SetByteValue(PLAYER_BYTES_3, PLAYER_BYTES_3_OFFSET_ARENA_FACTION, 0);
        player->RemoveBattlegroundQueueJoinTime(bgTypeId);

        if (Transport)
            player->TeleportToBGEntryPoint();

        player->SetUInt32Value(UNIT_FIELD_EFFECTIVE_LEVEL, 0);

        sLog->outInfo(LOG_FILTER_BATTLEGROUND, "BATTLEGROUND: Removed player %s from Battleground.", player->GetName());
    }

    //battleground object will be deleted next Battleground::Update() call
}

// this method is called when no players remains in battleground
void Battleground::Reset()
{
    SetWinner(WINNER_NONE);
    SetStatus(STATUS_WAIT_QUEUE);
    SetElapsedTime(Milliseconds(0));
    SetRemainingTime(Milliseconds(0));
    SetLastResurrectTime(0);
    SetJoinType(0);
    SetRated(false);

    m_Events = 0;

    if (m_InvitedAlliance > 0 || m_InvitedHorde > 0)
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::Reset: one of the counters is not 0 (alliance: %u, horde: %u) for BG (map: %u, instance id: %u)!",
        m_InvitedAlliance, m_InvitedHorde, m_MapId, m_InstanceID);

    m_InvitedAlliance = 0;
    m_InvitedHorde = 0;
    m_InBGFreeSlotQueue = false;

    m_Players.clear();

    for (BattlegroundScoreMap::const_iterator itr = PlayerScores.begin(); itr != PlayerScores.end(); ++itr)
        delete itr->second;
    PlayerScores.clear();

    for (uint8 i = TEAM_ALLIANCE; i < MAX_TEAMS; ++i)
        _arenaTeamScores[i].Reset();

    ResetBGSubclass();
}

void Battleground::RelocateDeadPlayers(ObjectGuid guideGuid)
{
    GuidVector& ghostList = m_ReviveQueue[guideGuid];
    if (!ghostList.empty())
    {
        WorldSafeLocsEntry const* closestGrave = nullptr;
        for (auto const& v : ghostList)
        {
            Player* player = ObjectAccessor::FindPlayer(v);
            if (!player)
                continue;

            closestGrave = GetClosestGraveYard(player);
            if (closestGrave)
                player->TeleportTo(GetMapId(), closestGrave->Loc.X, closestGrave->Loc.Y, closestGrave->Loc.Z, player->GetOrientation());
        }
        ghostList.clear();
    }
}

void Battleground::StartBattleground()
{
    SetElapsedTime(Milliseconds(0));
    SetLastResurrectTime(0);
    // add BG to free slot queue
    AddToBGFreeSlotQueue();

    // add bg to update list
    // This must be done here, because we need to have already invited some players when first BG::Update() method is executed
    // and it doesn't matter if we call StartBattleground() more times, because m_Battlegrounds is a map and instance id never changes
    sBattlegroundMgr->AddBattleground(GetInstanceID(), GetTypeID(), this);
}

void Battleground::AddPlayer(Player* player)
{
    // remove afk from player
    if (player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_AFK))
        player->ToggleAFK();

    // score struct must be created in inherited class

    ObjectGuid guid = player->GetGUID();
    uint32 team = player->GetBGTeam();

    BattlegroundPlayer bp;
    bp.OfflineRemoveTime = 0;
    bp.Team = team;

    // Add to list/maps
    m_Players[guid] = bp;

    UpdatePlayersCountByTeam(team, false);                  // +1 player
    SendPacketToTeam(team, WorldPackets::Battleground::PlayerJoined(guid).Write(), player, false);
    // BG Status packet
    BattlegroundQueueTypeId bgQueueTypeId = sBattlegroundMgr->BGQueueTypeId(m_TypeID, GetJoinType());
    uint32 queueSlot = player->GetBattlegroundQueueIndex(bgQueueTypeId);
    // if is BATTLEGROUND_AA or BATTLEGROUND_RB or BATTLEGROUND_RATED_10_VS_10 m_TypeID == right data

    WorldPackets::Battleground::BattlefieldStatusActive battlefieldStatus;
    sBattlegroundMgr->BuildBattlegroundStatusActive(&battlefieldStatus, this, player, queueSlot, player->GetBattlegroundQueueJoinTime(m_TypeID), GetJoinType());
    player->SendDirectMessage(battlefieldStatus.Write());

    player->SetUInt32Value(UNIT_FIELD_EFFECTIVE_LEVEL, GetMaxLevel());
    player->Dismount();
    player->RemoveAurasByType(SPELL_AURA_MOUNTED);
    player->RemoveAurasByType(SPELL_AURA_FLY);

    if (IsRBG())
    {
        if (player->GetBGTeam() != team)
            player->setFaction(team == ALLIANCE ? 1 : 2);

        player->CastSpell(player, SPELL_BATTLE_FATIGUE, true);
    }

    if (!isArena() && GetStatus() == STATUS_WAIT_JOIN)
        player->CastSpell(player, SPELL_BG_PREPARATION, true);

    // setup BG group membership
    PlayerAddedToBGCheckIfBGIsRunning(player);
    AddOrSetPlayerToCorrectBgGroup(player, team);

    //Recal amaunt in auras
    player->ScheduleDelayedOperation(DELAYED_UPDATE_AFTER_TO_BG);

    // Log
    sLog->outInfo(LOG_FILTER_BATTLEGROUND, "BATTLEGROUND: Player %s joined the battle.", player->GetName());
}

// this method adds player to his team's bg group, or sets his correct group if player is already in bg group
void Battleground::AddOrSetPlayerToCorrectBgGroup(Player* player, uint32 team)
{
    ObjectGuid playerGuid = player->GetGUID();
    Group* group = GetBgRaid(team);
    if (!group)                                      // first player joined
    {
        group = new Group;
        SetBgRaid(team, group);
        group->Create(player);
        sGroupMgr->AddGroup(group);
    }
    else                                            // raid already exist
    {
        if (group->IsMember(playerGuid))
        {
            uint8 subgroup = group->GetMemberGroup(playerGuid);
            player->SetBattlegroundOrBattlefieldRaid(group, subgroup);
        }
        else
        {
            group->AddMember(player);
            if (Group* originalGroup = player->GetOriginalGroup())
                if (originalGroup->IsLeader(playerGuid))
                {
                    group->ChangeLeader(playerGuid);
                    group->SendUpdate();
                }
        }
    }
}

void Battleground::UpdateCapturePoint(uint8 type, TeamId teamID, GameObject* node, Player const* player /*= nullptr*/, bool initial /*= false*/)
{
    auto pointInfo = node->GetGOInfo()->capturePoint;
    uint32 const spellVisualArray[] = {pointInfo.SpellVisual1, pointInfo.SpellVisual2, pointInfo.SpellVisual3, pointInfo.SpellVisual4, pointInfo.SpellVisual5};
    auto nodeState = NODE_STATE_NONE;
    auto broadcastID = 0, kitID = 0, visualID = 0;

    switch (type)
    {
        case NODE_STATUS_NEUTRAL:
            nodeState = NODE_STATE_NEUTRAL;
            visualID = spellVisualArray[0];
            break;
        case NODE_STATUS_ASSAULT:
            nodeState = teamID == TEAM_ALLIANCE ? NODE_STATE_ALLIANCE_ASSAULT : NODE_STATE_HORDE_ASSAULT;
            broadcastID = teamID == TEAM_ALLIANCE ? pointInfo.AssaultBroadcastAlliance : pointInfo.AssaultBroadcastHorde;
            kitID = 1;
            visualID = spellVisualArray[teamID == TEAM_ALLIANCE ? 2 : 1];
            break;
        case NODE_STATUS_CAPTURE:
            nodeState = teamID == TEAM_ALLIANCE ? NODE_STATE_ALLIANCE_CAPTURE : NODE_STATE_HORDE_CAPTURE;
            broadcastID = teamID == TEAM_ALLIANCE ? pointInfo.CaptureBroadcastAlliance : pointInfo.CaptureBroadcastHorde;
            kitID = teamID == TEAM_ALLIANCE ? 4 : 3;
            visualID = spellVisualArray[TEAM_ALLIANCE ? 4 : 3];
            break;
        default:
            break;
    }

    if (!initial)
    {
        WorldPackets::GameObject::GoCustomAnim customAnim;
        customAnim.ObjectGUID = node->GetGUID();
        customAnim.CustomAnim = kitID;
        SendPacketToAll(customAnim.Write());

        WorldPackets::GameObject::GameObjectPlaySpellVisual objectSpellVisual;
        objectSpellVisual.ObjectGUID = node->GetGUID();
        objectSpellVisual.SpellVisualID = visualID;
        SendPacketToAll(objectSpellVisual.Write());

        auto lang = CHAT_MSG_BG_SYSTEM_NEUTRAL;
        switch (teamID)
        {
            case TEAM_ALLIANCE:
                lang = CHAT_MSG_BG_SYSTEM_ALLIANCE;
                break;
            case TEAM_HORDE:
                lang = CHAT_MSG_BG_SYSTEM_HORDE;
                break;
            default:
                break;
        }

        PlayeCapturePointSound(type, teamID);
        SendBroadcastTextToAll(broadcastID, lang, player);
    }

    WorldPackets::Battleground::BattlegroundCapturePointInfo info;
    info.Info.Guid = node->GetGUID();
    info.Info.Pos = node->GetPosition();
    info.Info.NodeState = nodeState;
    if (type == NODE_STATUS_ASSAULT)
    {
        info.Info.CaptureTime = pointInfo.CaptureTime - 1;
        info.Info.CaptureTotalDuration = pointInfo.CaptureTime;
    }
    SendPacketToAll(info.Write());

    UpdateWorldState(pointInfo.worldState1, nodeState);

    node->SetUInt32Value(GAMEOBJECT_FIELD_SPELL_VISUAL_ID, visualID);
}

void Battleground::PlayeCapturePointSound(uint8 type, TeamId teamID)
{
    switch (type)
    {
        case NODE_STATUS_NEUTRAL:
            PlaySoundToAll(teamID == TEAM_HORDE ? BG_SOUND_FLAG_PLACED_HORDE : BG_SOUND_FLAG_PLACED_ALLIANCE);
            break;
        case NODE_STATUS_ASSAULT:
            PlaySoundToAll(teamID == TEAM_HORDE ? BG_SOUND_CAPTURE_POINT_ASSAULT_HORDE : BG_SOUND_CAPTURE_POINT_ASSAULT_ALLIANCE);
            break;
        case NODE_STATUS_CAPTURE:
            PlaySoundToAll(teamID == TEAM_HORDE ? BG_SOUND_CAPTURE_POINT_CAPTURED_HORDE : BG_SOUND_CAPTURE_POINT_CAPTURED_ALLIANCE);
            break;
        default:
            break;
    }
}

// This method should be called when player logs into running battleground
void Battleground::EventPlayerLoggedIn(Player* player)
{
    ObjectGuid guid = player->GetGUID();
    // player is correct pointer
    for (GuidDeque::iterator itr = m_OfflineQueue.begin(); itr != m_OfflineQueue.end(); ++itr)
    {
        if (*itr == guid)
        {
            m_OfflineQueue.erase(itr);
            break;
        }
    }
    m_Players[guid].OfflineRemoveTime = 0;
    PlayerAddedToBGCheckIfBGIsRunning(player);
    // if battleground is starting, then add preparation aura
    // we don't have to do that, because preparation aura isn't removed when player logs out
}

// This method should be called when player logs out from running battleground
void Battleground::EventPlayerLoggedOut(Player* player)
{
    ObjectGuid guid = player->GetGUID();
    if (!IsPlayerInBattleground(guid))  // Check if this player really is in battleground (might be a GM who teleported inside)
        return;

    // player is correct pointer, it is checked in WorldSession::LogoutPlayer()
    m_OfflineQueue.push_back(player->GetGUID());
    m_Players[guid].OfflineRemoveTime = sWorld->GetGameTime() + MAX_OFFLINE_TIME;
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        // drop flag and handle other cleanups
        RemovePlayer(player, guid, GetPlayerTeam(guid));

        // 1 player is logging out, if it is the last, then end arena!
        if (isArena())
            if (GetAlivePlayersCountByTeam(player->GetBGTeam()) <= 1 && GetPlayersCountByTeam(GetOtherTeam(player->GetBGTeam())))
                EndBattleground(GetOtherTeam(player->GetBGTeam()));
    }
}

// This method should be called only once ... it adds pointer to queue
void Battleground::AddToBGFreeSlotQueue()
{
    // make sure to add only once
    if (!m_InBGFreeSlotQueue && isBattleground())
    {
        sBattlegroundMgr->BGFreeSlotQueue[m_TypeID].push_front(this);
        m_InBGFreeSlotQueue = true;
    }
}

// This method removes this battleground from free queue - it must be called when deleting battleground - not used now
void Battleground::RemoveFromBGFreeSlotQueue()
{
    // set to be able to re-add if needed
    m_InBGFreeSlotQueue = false;
    // uncomment this code when battlegrounds will work like instances
    for (BGFreeSlotQueueType::iterator itr = sBattlegroundMgr->BGFreeSlotQueue[m_TypeID].begin(); itr != sBattlegroundMgr->BGFreeSlotQueue[m_TypeID].end(); ++itr)
    {
        if ((*itr)->GetInstanceID() == m_InstanceID)
        {
            sBattlegroundMgr->BGFreeSlotQueue[m_TypeID].erase(itr);
            return;
        }
    }
}

// get the number of free slots for team
// returns the number how many players can join battleground to MaxPlayersPerTeam
uint32 Battleground::GetFreeSlotsForTeam(uint32 Team) const
{
    // if BG is starting ... invite anyone
    if (GetStatus() == STATUS_WAIT_JOIN)
        return (GetInvitedCount(Team) < GetMaxPlayersPerTeam()) ? GetMaxPlayersPerTeam() - GetInvitedCount(Team) : 0;

    uint32 otherTeam = GetInvitedCount(Team == ALLIANCE ? HORDE : ALLIANCE);
    uint32 otherIn = GetPlayersCountByTeam(Team == ALLIANCE ? HORDE : ALLIANCE);

    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        // difference based on ppl invited (not necessarily entered battle)
        // default: allow 0
        uint32 diff = 0;
        // allow join one person if the sides are equal (to fill up bg to minplayersperteam)
        if (otherTeam == GetInvitedCount(Team))
            diff = 1;
        // allow join more ppl if the other side has more players
        else if (otherTeam > GetInvitedCount(Team))
            diff = otherTeam - GetInvitedCount(Team);

        // difference based on max players per team (don't allow inviting more)
        uint32 diff2 = (GetInvitedCount(Team) < GetMaxPlayersPerTeam()) ? GetMaxPlayersPerTeam() - GetInvitedCount(Team) : 0;
        // difference based on players who already entered
        // default: allow 0
        uint32 diff3 = 0;
        // allow join one person if the sides are equal (to fill up bg minplayersperteam)
        if (otherIn == GetPlayersCountByTeam(Team))
            diff3 = 1;
        // allow join more ppl if the other side has more players
        else if (otherIn > GetPlayersCountByTeam(Team))
            diff3 = otherIn - GetPlayersCountByTeam(Team);
        // or other side has less than minPlayersPerTeam
        else if (GetInvitedCount(Team) <= GetMinPlayersPerTeam())
            diff3 = GetMinPlayersPerTeam() - GetInvitedCount(Team) + 1;

        // return the minimum of the 3 differences

        // min of diff and diff 2
        diff = std::min(diff, diff2);
        // min of diff, diff2 and diff3
        return std::min(diff, diff3);
    }
    return 0;
}

bool Battleground::HasFreeSlots() const
{
    return GetPlayersSize() < GetMaxPlayers();
}

void Battleground::BuildPvPLogDataPacket(WorldPackets::Battleground::PVPLogData& packet)
{
    BracketType bType = BattlegroundMgr::BracketByJoinType(GetJoinType());

    if (GetStatus() == STATUS_WAIT_LEAVE)
        packet.Winner = GetWinner();

    packet.Players.reserve(GetPlayerScoresSize());
    for (auto const& score : PlayerScores)
    {
        if (!IsPlayerInBattleground(score.first))
            continue;

        WorldPackets::Battleground::PVPLogData::PlayerData playerData;

        playerData.PlayerGUID = score.second->PlayerGuid;
        playerData.Kills = score.second->KillingBlows;
        playerData.Faction = score.second->TeamID;
        if (score.second->HonorableKills || score.second->Deaths || score.second->BonusHonor)
        {
            playerData.Honor = boost::in_place();
            playerData.Honor->HonorKills = score.second->HonorableKills;
            playerData.Honor->Deaths = score.second->Deaths;
            playerData.Honor->ContributionPoints = score.second->BonusHonor;
        }

        playerData.DamageDone = score.second->DamageDone;
        playerData.HealingDone = score.second->HealingDone;
        score.second->BuildObjectivesBlock(playerData.Stats);

        if (Player* player = ObjectAccessor::FindPlayer(playerData.PlayerGUID))
        {
            playerData.IsInWorld = true;
            playerData.PrimaryTalentTree = player->GetUInt32Value(PLAYER_FIELD_CURRENT_SPEC_ID);

            if (isRated())
            {
                Bracket* bracket = player->getBracket(bType);
                if (!bracket)
                    bracket = sBracketMgr->TryGetOrCreateBracket(score.first, bType);

                if (bracket)
                {
                    playerData.PreMatchRating = bracket->getRatingLastChange();
                    playerData.RatingChange = bracket->getRating();
                    playerData.PreMatchMMR = bracket->getLastMMRChange();
                    playerData.MmrChange = bracket->getMMV() - bracket->getLastMMRChange();
                }
            }
        }

        packet.Players.push_back(playerData);
    }

    if (isRated())
    {
        packet.Ratings = boost::in_place();

        for (uint8 i = TEAM_ALLIANCE; i < MAX_TEAMS; ++i)
        {
            packet.Ratings->Postmatch[i] = _arenaTeamScores[i].NewRating;
            packet.Ratings->Prematch[i] = _arenaTeamScores[i].OldRating;
            packet.Ratings->PrematchMMR[i] = _arenaTeamScores[i].MatchmakerRating;
        }
    }

    packet.PlayerCount[0] = int8(GetPlayersCountByTeam(HORDE));
    packet.PlayerCount[1] = int8(GetPlayersCountByTeam(ALLIANCE));
}

bool Battleground::UpdatePlayerScore(Player* player, uint32 type, uint32 value, bool doAddHonor /*= true*/)
{
    BattlegroundScoreMap::const_iterator itr = PlayerScores.find(player->GetGUID());
    if (itr == PlayerScores.end())
        return false;

    if (type == SCORE_BONUS_HONOR && doAddHonor && isBattleground())
        player->RewardHonor(nullptr, 1, value);
    else
        itr->second->UpdateScore(type, value);

    return true;
}

void Battleground::AddPlayerToResurrectQueue(ObjectGuid npc_guid, ObjectGuid player_guid)
{
    m_ReviveQueue[npc_guid].push_back(player_guid);

    Player* player = ObjectAccessor::FindPlayer(player_guid);
    if (!player)
        return;

    player->CastSpell(player, SPELL_BG_WAITING_FOR_RESURRECT, true);
}

void Battleground::RemovePlayerFromResurrectQueue(ObjectGuid player_guid)
{
    for (std::map<ObjectGuid, GuidVector >::iterator itr = m_ReviveQueue.begin(); itr != m_ReviveQueue.end(); ++itr)
    {
        for (GuidVector::iterator itr2 = (itr->second).begin(); itr2 != (itr->second).end(); ++itr2)
        {
            if (*itr2 == player_guid)
            {
                (itr->second).erase(itr2);
                if (Player* player = ObjectAccessor::FindPlayer(player_guid))
                    player->RemoveAurasDueToSpell(SPELL_BG_WAITING_FOR_RESURRECT);
                return;
            }
        }
    }
}

bool Battleground::AddObject(uint32 type, uint32 entry, Position pos, Position rotation /*= { }*/, uint32 respawnTime /*= 0*/)
{
    return AddObject(type, entry, pos.m_positionX, pos.m_positionY, pos.m_positionZ, pos.m_orientation, rotation.m_positionX, rotation.m_positionY, rotation.m_positionZ, rotation.m_orientation, respawnTime);
}

bool Battleground::AddObject(uint32 type, uint32 entry, float x, float y, float z, float o, float rotation0, float rotation1, float rotation2, float rotation3, uint32 /*respawnTime*/)
{
    // If the assert is called, means that BgObjects must be resized!
    ASSERT(type < BgObjects.size());

    Map* map = FindBgMap();
    if (!map)
        return false;
    // Must be created this way, adding to godatamap would add it to the base map of the instance
    // and when loading it (in go::LoadFromDB()), a new guid would be assigned to the object, and a new object would be created
    // So we must create it specific for this instance
    GameObject* go = new GameObject;
    if (!go->Create(sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate(), entry, GetBgMap(),
        PHASEMASK_NORMAL, x, y, z, o, rotation0, rotation1, rotation2, rotation3, 100, GO_STATE_READY))
    {
        sLog->outError(LOG_FILTER_SQL, "Battleground::AddObject: cannot create gameobject (entry: %u) for BG (map: %u, instance id: %u)!",
                       entry, m_MapId, m_InstanceID);
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::AddObject: cannot create gameobject (entry: %u) for BG (map: %u, instance id: %u)!",
                       entry, m_MapId, m_InstanceID);
        delete go;
        return false;
    }
/*
    ObjectGuid::LowType guid = go->GetGUID().GetCounter();

    // without this, UseButtonOrDoor caused the crash, since it tried to get go info from godata
    // iirc that was changed, so adding to go data map is no longer required if that was the only function using godata from GameObject without checking if it existed
    GameObjectData& data = sObjectMgr->NewGOData(guid);

    data.id             = entry;
    data.mapid          = GetMapId();
    data.posX           = x;
    data.posY           = y;
    data.posZ           = z;
    data.orientation    = o;
    data.rotation0      = rotation0;
    data.rotation1      = rotation1;
    data.rotation2      = rotation2;
    data.rotation3      = rotation3;
    data.spawntimesecs  = respawnTime;
    data.spawnMask      = 1;
    data.animprogress   = 100;
    data.go_state       = 1;
*/
    if (go->IsTransport())
    {
        go->SetGoState(GO_STATE_ACTIVE);
        go->SetManualAnim(true);
    }

    // Add to world, so it can be later looked up from HashMapHolder
    if (!map->AddToMap(go))
    {
        delete go;
        return false;
    }
    BgObjects[type] = go->GetGUID();
    return true;
}


void Battleground::DoorsOpen(uint32 type1, uint32 type2)
{
    DoorOpen(type1);
    DoorOpen(type2);
}

void Battleground::DoorsClose(uint32 type1, uint32 type2)
{
    DoorClose(type1);
    DoorClose(type2);
}

// Some doors aren't despawned so we cannot handle their closing in gameobject::update()
// It would be nice to correctly implement GO_ACTIVATED state and open/close doors in gameobject code
void Battleground::DoorClose(uint32 type)
{
    if (GameObject* obj = GetBgMap()->GetGameObject(BgObjects[type]))
    {
        // If doors are open, close it
        if (obj->getLootState() == GO_ACTIVATED && obj->GetGoState() != GO_STATE_READY)
        {
            obj->SetLootState(GO_READY);
            obj->SetGoState(GO_STATE_READY);
        }
    }
    else
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::DoorClose: door gameobject (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!",
        type, BgObjects[type].GetCounter(), m_MapId, m_InstanceID);
}

void Battleground::DoorOpen(uint32 type)
{
    if (GameObject* obj = GetBgMap()->GetGameObject(BgObjects[type]))
    {
        obj->SetLootState(GO_ACTIVATED);
        obj->SetGoState(GO_STATE_ACTIVE);
    }
    else
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::DoorOpen: door gameobject (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!",
        type, BgObjects[type].GetCounter(), m_MapId, m_InstanceID);
}

GameObject* Battleground::GetBGObject(uint32 type)
{
    GameObject* obj = GetBgMap()->GetGameObject(BgObjects[type]);
    if (!obj)
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::GetBGObject: gameobject (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!",
        type, BgObjects[type].GetCounter(), m_MapId, m_InstanceID);
    return obj;
}

Creature* Battleground::GetBGCreature(uint32 type)
{
    Creature* creature = GetBgMap()->GetCreature(BgCreatures[type]);
    if (!creature)
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::GetBGCreature: creature (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!",
        type, BgCreatures[type].GetCounter(), m_MapId, m_InstanceID);
    return creature;
}

void Battleground::SpawnBGObject(uint32 type, uint32 respawntime)
{
    if (Map* map = FindBgMap())
        if (GameObject* obj = map->GetGameObject(BgObjects[type]))
        {
            if (respawntime)
                obj->SetLootState(GO_JUST_DEACTIVATED);
            else
                if (obj->getLootState() == GO_JUST_DEACTIVATED)
                    // Change state from GO_JUST_DEACTIVATED to GO_READY in case battleground is starting again
                    obj->SetLootState(GO_READY);
            obj->SetRespawnTime(respawntime);
            map->AddToMap(obj);
        }
}

Creature* Battleground::AddCreature(uint32 entry, uint32 type, uint32 teamval, Position pos, uint32 respawntime /*= 0*/)
{
    return AddCreature(entry, type, teamval, pos.m_positionX, pos.m_positionY, pos.m_positionZ, pos.m_orientation, respawntime);
}

Creature* Battleground::AddCreature(uint32 entry, uint32 type, uint32 teamval, float x, float y, float z, float o, uint32 respawntime)
{
    // If the assert is called, means that BgCreatures must be resized!
    ASSERT(type < BgCreatures.size());

    Map* map = FindBgMap();
    if (!map)
        return nullptr;

    Creature* creature = new Creature;
    if (!creature->Create(sObjectMgr->GetGenerator<HighGuid::Creature>()->Generate(), map, PHASEMASK_NORMAL, entry, 0, teamval, x, y, z, o))
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::AddCreature: cannot create creature (entry: %u) for BG (map: %u, instance id: %u)!",
                       entry, m_MapId, m_InstanceID);
        delete creature;
        return nullptr;
    }

    creature->SetHomePosition(x, y, z, o);

    CreatureTemplate const* cinfo = sObjectMgr->GetCreatureTemplate(entry);
    if (!cinfo)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::AddCreature: creature template (entry: %u) does not exist for BG (map: %u, instance id: %u)!",
                       entry, m_MapId, m_InstanceID);
        delete creature;
        return nullptr;
    }
    // Force using DB speeds
    creature->SetSpeed(MOVE_WALK, cinfo->speed_walk);
    creature->SetSpeed(MOVE_RUN, cinfo->speed_run);
    creature->SetSpeed(MOVE_FLIGHT, cinfo->speed_fly);

    if (!map->AddToMap(creature))
    {
        delete creature;
        return nullptr;
    }

    BgCreatures[type] = creature->GetGUID();

    if (respawntime)
        creature->SetRespawnDelay(respawntime);

    return  creature;
}

bool Battleground::DelCreature(uint32 type)
{
    if (!BgCreatures[type])
        return true;

    if (Creature* creature = GetBgMap()->GetCreature(BgCreatures[type]))
    {
        creature->AddObjectToRemoveList();
        BgCreatures[type].Clear();
        return true;
    }

    sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::DelCreature: creature (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!",
                   type, BgCreatures[type].GetCounter(), m_MapId, m_InstanceID);
    BgCreatures[type].Clear();
    return false;
}

bool Battleground::DelObject(uint32 type)
{
    if (!BgObjects[type])
        return true;

    if (GameObject* obj = GetBgMap()->GetGameObject(BgObjects[type]))
    {
        obj->SetRespawnTime(0);                                 // not save respawn time
        obj->Delete();
        BgObjects[type].Clear();
        return true;
    }
    sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::DelObject: gameobject (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!",
                   type, BgObjects[type].GetCounter(), m_MapId, m_InstanceID);
    BgObjects[type].Clear();
    return false;
}

bool Battleground::AddSpiritGuide(uint32 type, DBCPosition4D loc, TeamId team)
{
    return AddSpiritGuide(type, loc.X, loc.Y, loc.Z, loc.O, GetTeamByTeamId(team));
}

bool Battleground::AddSpiritGuide(uint32 type, Position pos, TeamId team)
{
    return AddSpiritGuide(type, pos.m_positionX, pos.m_positionY, pos.m_positionZ, pos.m_orientation, GetTeamByTeamId(team));
}

bool Battleground::AddSpiritGuide(uint32 type, float x, float y, float z, float o, uint32 team)
{
    uint32 entry = (team == ALLIANCE) ? BG_CREATURE_ENTRY_A_SPIRITGUIDE : BG_CREATURE_ENTRY_H_SPIRITGUIDE;

    if (Creature* creature = AddCreature(entry, type, team, x, y, z, o))
    {
        creature->setDeathState(DEAD);
        creature->SetGuidValue(UNIT_FIELD_CHANNEL_OBJECT, creature->GetGUID());
        // aura
        // TODO: Fix display here
        // creature->SetVisibleAura(0, SPELL_BG_SPIRIT_HEAL_CHANNEL);
        // casting visual effect
        creature->SetUInt32Value(UNIT_FIELD_CHANNEL_SPELL, SPELL_BG_SPIRIT_HEAL_CHANNEL);
        // correct cast speed
        creature->SetFloatValue(UNIT_FIELD_MOD_CASTING_SPEED, 1.0f);
        creature->SetFloatValue(UNIT_FIELD_MOD_SPELL_HASTE, 1.0f);
        //creature->CastSpell(creature, SPELL_BG_SPIRIT_HEAL_CHANNEL, true);
        return true;
    }
    sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::AddSpiritGuide: cannot create spirit guide (type: %u, entry: %u) for BG (map: %u, instance id: %u)!",
                   type, entry, m_MapId, m_InstanceID);
    EndNow();
    return false;
}

void Battleground::SendMessageToAll(int32 entry, ChatMsg type, Player const* source)
{
    if (!entry)
        return;

    Trinity::BattlegroundChatBuilder bg_builder(type, entry, source);
    Trinity::LocalizedPacketDo<Trinity::BattlegroundChatBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);
}

void Battleground::PSendMessageToAll(int32 entry, ChatMsg type, Player const* source, ...)
{
    if (!entry)
        return;

    va_list ap;
    va_start(ap, source);

    Trinity::BattlegroundChatBuilder bg_builder(type, entry, source, &ap);
    Trinity::LocalizedPacketDo<Trinity::BattlegroundChatBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);

    va_end(ap);
}

void Battleground::SendWarningToAll(int32 entry, ...)
{
    if (!entry)
        return;

    const char *format = sObjectMgr->GetTrinityStringForDBCLocale(entry);

    char str[1024];
    va_list ap;
    va_start(ap, entry);
    vsnprintf(str, 1024, format, ap);
    va_end(ap);
    std::string msg(str);

    WorldPacket data;

    Trinity::ChatData c;
    c.chatType = CHAT_MSG_RAID_BOSS_EMOTE;
    c.message = msg;

    Trinity::BuildChatPacket(data, c);

    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            if (player->GetSession())
                player->GetSession()->SendPacket(&data);
}

void Battleground::SendMessage2ToAll(int32 entry, ChatMsg type, Player const* source, int32 arg1, int32 arg2)
{
    Trinity::Battleground2ChatBuilder bg_builder(type, entry, source, arg1, arg2);
    Trinity::LocalizedPacketDo<Trinity::Battleground2ChatBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);
}

void Battleground::SendBroadcastTextToAll(int32 broadcastTextID, ChatMsg type, Unit const* unit /*= nullptr*/)
{
    BroadcastTextEntry const* bct = sBroadcastTextStore.LookupEntry(broadcastTextID);
    if (!bct)
        return;

    WorldPacket data;
    Trinity::ChatData c;

    if (unit)
        c.targetGuid = unit->GetGUID();
    c.chatType = type;

    for (auto const& v : m_Players)
        if (Player* _player = ObjectAccessor::FindPlayer(v.first))
            if (WorldSession* session = _player->GetSession())
            {
                LocaleConstant locale = session->GetSessionDbLocaleIndex();
                if (locale >= LOCALE_enUS)
                {
                    c.message = DB2Manager::GetBroadcastTextValue(bct, locale);
                    Trinity::BuildChatPacket(data, c);
                    session->SendPacket(&data);
                }
            }
}

void Battleground::EndNow()
{
    RemoveFromBGFreeSlotQueue();
    SetStatus(STATUS_WAIT_LEAVE);
    SetRemainingTime(Milliseconds(0));
}

bool Battleground::HandlePlayerUnderMap(Player* player)
{
    player->TeleportTo(GetMapId(), GetTeamStartPosition(player->GetBGTeamId()), TELE_TO_NOT_LEAVE_COMBAT);
    return true;
}

// To be removed
const char* Battleground::GetTrinityString(int32 entry)
{
    // FIXME: now we have different DBC locales and need localized message for each target client
    return sObjectMgr->GetTrinityStringForDBCLocale(entry);
}

// IMPORTANT NOTICE:
// buffs aren't spawned/despawned when players captures anything
// buffs are in their positions when battleground starts
void Battleground::HandleTriggerBuff(ObjectGuid go_guid)
{
    if (!GetBgMap())
        return;
    GameObject* obj = GetBgMap()->GetGameObject(go_guid);
    if (!obj || obj->GetGoType() != GAMEOBJECT_TYPE_TRAP || !obj->isSpawned())
        return;

    // Change buff type, when buff is used:
    int32 index = BgObjects.size() - 1;
    while (index >= 0 && BgObjects[index] != go_guid)
        index--;
    if (index < 0)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::HandleTriggerBuff: cannot find buff gameobject (GUID: %u, entry: %u, type: %u) in internal data for BG (map: %u, instance id: %u)!",
                       go_guid.GetCounter(), obj->GetEntry(), obj->GetGoType(), m_MapId, m_InstanceID);
        return;
    }

    // Randomly select new buff
    uint8 buff = urand(0, 2);
    uint32 entry = obj->GetEntry();
    if (m_BuffChange && entry != Buff_Entries[buff])
    {
        // Despawn current buff
        SpawnBGObject(index, RESPAWN_ONE_DAY);
        // Set index for new one
        for (uint8 currBuffTypeIndex = 0; currBuffTypeIndex < 3; ++currBuffTypeIndex)
            if (entry == Buff_Entries[currBuffTypeIndex])
            {
                index -= currBuffTypeIndex;
                index += buff;
            }
    }

    SpawnBGObject(index, BUFF_RESPAWN_TIME);
}
void Battleground::HandleAreaTrigger(Player* player, uint32 trigger, bool entered)
{
    if (player->isGameMaster())
        player->GetSession()->SendNotification("Warning: Unhandled AreaTrigger in Battleground: %u, Entered: %u", trigger, entered);
}

void Battleground::HandleKillPlayer(Player* victim, Player* killer)
{
    // Keep in mind that for arena this will have to be changed a bit

    // Add +1 deaths
    UpdatePlayerScore(victim, SCORE_DEATHS, 1);
    // Add +1 kills to group and +1 killing_blows to killer
    if (killer)
    {
        // Don't reward credit for killing ourselves, like fall damage of hellfire (warlock)
        if (killer == victim)
            return;

        UpdatePlayerScore(killer, SCORE_HONORABLE_KILLS, 1);
        UpdatePlayerScore(killer, SCORE_KILLING_BLOWS, 1);

        for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        {
            Player* creditedPlayer = ObjectAccessor::FindPlayer(itr->first);
            if (!creditedPlayer || creditedPlayer == killer)
                continue;

            if (creditedPlayer->GetBGTeam() == killer->GetBGTeam() && creditedPlayer->IsAtGroupRewardDistance(victim))
                UpdatePlayerScore(creditedPlayer, SCORE_HONORABLE_KILLS, 1);
        }
    }

    if (!isArena())
    {
        // To be able to remove insignia -- ONLY IN Battlegrounds
        victim->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
        RewardXPAtKill(killer, victim);
    }
}

// Return the player's team based on battlegroundplayer info
// Used in same faction arena matches mainly
uint32 Battleground::GetPlayerTeam(ObjectGuid guid) const
{
    BattlegroundPlayerMap::const_iterator itr = m_Players.find(guid);
    if (itr != m_Players.end())
        return itr->second.Team;
    return 0;
}

uint32 Battleground::GetOtherTeam(uint32 teamId) const
{
    return teamId ? ((teamId == ALLIANCE) ? HORDE : ALLIANCE) : 0;
}

bool Battleground::IsPlayerInBattleground(ObjectGuid guid) const
{
    BattlegroundPlayerMap::const_iterator itr = m_Players.find(guid);
    if (itr != m_Players.end())
        return true;
    return false;
}

void Battleground::PlayerAddedToBGCheckIfBGIsRunning(Player* player)
{
    if (GetStatus() != STATUS_WAIT_LEAVE)
        return;

    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(GetTypeID(), GetJoinType());

    BlockMovement(player);

    WorldPackets::Battleground::PVPLogData pvpLogData;
    BuildPvPLogDataPacket(pvpLogData);
    player->GetSession()->SendPacket(pvpLogData.Write());

    WorldPackets::Battleground::BattlefieldStatusActive battlefieldStatus;
    sBattlegroundMgr->BuildBattlegroundStatusActive(&battlefieldStatus, this, player, player->GetBattlegroundQueueIndex(bgQueueTypeId), player->GetBattlegroundQueueJoinTime(GetTypeID()), GetJoinType());
    player->SendDirectMessage(battlefieldStatus.Write());
}

uint32 Battleground::GetAlivePlayersCountByTeam(uint32 Team) const
{
    int count = 0;
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        if (itr->second.Team == Team)
        {
            Player* player = ObjectAccessor::FindPlayer(itr->first);
            if (player && player->isAlive() && !player->HasByteFlag(UNIT_FIELD_BYTES_2, 3, FORM_SPIRITOFREDEMPTION))
                ++count;
        }
    }
    return count;
}

int32 Battleground::GetObjectType(ObjectGuid guid)
{
    for (size_t i = 0; i < BgObjects.size(); ++i)
        if (BgObjects[i] == guid)
            return i;
    sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::GetObjectType: player used gameobject (GUID: %u) which is not in internal data for BG (map: %u, instance id: %u), cheating?",
                   guid.GetCounter(), m_MapId, m_InstanceID);
    return -1;
}

void Battleground::BattlegroundTimedWin(uint32 type /*= 1*/)
{
    switch (type)
    {
        case 1:
            if (m_TeamScores[TEAM_ALLIANCE] > m_TeamScores[TEAM_HORDE])
                EndBattleground(ALLIANCE);
            else if (m_TeamScores[TEAM_HORDE] > m_TeamScores[TEAM_ALLIANCE])
                EndBattleground(HORDE);
            else
                EndBattleground(TEAM_OTHER);
            break;
        case 2:
            if (m_TeamScores[TEAM_ALLIANCE] == 0)
            {
                if (m_TeamScores[TEAM_HORDE] == 0)
                    EndBattleground(WINNER_NONE);
                else
                    EndBattleground(HORDE);
            }
            else if (m_TeamScores[TEAM_HORDE] == 0)
                EndBattleground(ALLIANCE);
            else if (m_TeamScores[TEAM_HORDE] == m_TeamScores[TEAM_ALLIANCE])
                EndBattleground(m_lastFlagCaptureTeam);
            else if (m_TeamScores[TEAM_HORDE] > m_TeamScores[TEAM_ALLIANCE])
                EndBattleground(HORDE);
            else
                EndBattleground(ALLIANCE);
            break;
        default:
            break;
    }
}

void Battleground::SetBgRaid(uint32 TeamID, Group* bg_raid)
{
    Group*& old_raid = TeamID == ALLIANCE ? m_BgRaids[TEAM_ALLIANCE] : m_BgRaids[TEAM_HORDE];
    if (old_raid)
        old_raid->SetBattlegroundGroup(nullptr);
    if (bg_raid)
        bg_raid->SetBattlegroundGroup(this);
    old_raid = bg_raid;
}

WorldSafeLocsEntry const* Battleground::GetClosestGraveYard(Player* player)
{
    return sObjectMgr->GetClosestGraveYard(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetMapId(), player->GetBGTeam());
}

bool Battleground::IsTeamScoreInRange(uint32 team, uint32 minScore, uint32 maxScore) const
{
    uint32 score = std::max(m_TeamScores[GetTeamIndexByTeamId(team)], 0);
    return score >= minScore && score <= maxScore;
}

void Battleground::StartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry)
{
    for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            player->GetAchievementMgr().StartTimedAchievement(type, entry);
}

void Battleground::SetBracket(PvPDifficultyEntry const* bracketEntry)
{
    m_BracketId = bracketEntry->GetBracketId();
    SetLevelRange(bracketEntry->minLevel, bracketEntry->maxLevel);
}

void Battleground::RewardXPAtKill(Player* killer, Player* victim)
{
    if (sWorld->getBoolConfig(CONFIG_BG_XP_FOR_KILL) && killer && victim)
        killer->RewardPlayerAndGroupAtKill(victim, true);
}

void Battleground::_ProcessPlayerPositionBroadcast(Milliseconds diff)
{
    if (m_LastPlayerPositionBroadcast > diff)
    {
        m_LastPlayerPositionBroadcast -= diff;
        return;
    }

    m_LastPlayerPositionBroadcast = PositionBroadcastUpdate;

    WorldPackets::Battleground::PlayerPositions playerPositions;
    GetPlayerPositionData(&playerPositions.FlagCarriers);
    SendPacketToAll(playerPositions.Write());
}
