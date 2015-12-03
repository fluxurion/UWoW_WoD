/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
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
#include "BracketMgr.h"
#include "GuildMgr.h"
#include "ArenaScore.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "World.h"
#include "WorldSession.h"

Arena::Arena()
{
    for (uint8 i = BG_STARTING_EVENT_FIRST; i < BG_STARTING_EVENT_COUNT; ++i)
    {
        m_broadcastMessages[i] = ArenaBroadcastTexts[i];
        m_hasBroadcasts[i] = true;
    }
}

void Arena::Update(uint32 diff)
{
    Battleground::Update(diff);

    if (GetStatus() != STATUS_IN_PROGRESS || !isArena())
        return;

    Milliseconds elapsedTime = GetElapsedTime();
    if (elapsedTime >= Minutes(25))
    {
        UpdateArenaWorldState();
        EndBattleground(WINNER_NONE);
        return;
    }
    else if (elapsedTime > Minutes(2))
        UpdateWorldState(WorldStates::ARENA_END_TIMER, int32(time(nullptr) + std::chrono::duration_cast<Seconds>(Minutes(25) - elapsedTime).count()));
    else if (elapsedTime >= Minutes(10))
    {
        ModifyStartDelayTime(Milliseconds(diff));
        if (GetStartDelayTime() <= Seconds(0) || GetStartDelayTime() > Seconds(10))
        {
            SetStartDelayTime(Seconds(10));
            for (auto const& v : GetPlayers())
                if (Player* player = sObjectAccessor->FindPlayer(v.first))
                    if (!player->HasAura(SPELL_BG_ARENA_DUMPENING))
                        player->CastSpell(player, SPELL_BG_ARENA_DUMPENING, true);
        }
    }
}

inline void Arena::_ProcessJoin(uint32 diff)
{
    Battleground::_ProcessJoin(diff);

    if (!isArena())
        return;

    if (GetStartDelayTime() <= m_messageTimer[BG_STARTING_EVENT_FOURTH] && !(m_Events & BG_STARTING_EVENT_4))
    {
        BattlegroundQueueTypeId bgQueueTypeId = sBattlegroundMgr->BGQueueTypeId(m_TypeID, GetJoinType());

        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                uint32 queueSlot = player->GetBattlegroundQueueIndex(bgQueueTypeId);

                WorldPackets::Battleground::BattlefieldStatusActive battlefieldStatus;
                sBattlegroundMgr->BuildBattlegroundStatusActive(&battlefieldStatus, this, player, queueSlot, player->GetBattlegroundQueueJoinTime(BATTLEGROUND_AA), GetJoinType());
                player->SendDirectMessage(battlefieldStatus.Write());

                player->RemoveAurasDueToSpell(SPELL_ARENA_PREPARATION);
                player->ResetAllPowers();

                Unit::AuraApplicationMap & auraMap = player->GetAppliedAuras();
                for (Unit::AuraApplicationMap::iterator iter = auraMap.begin(); iter != auraMap.end();)
                {
                    AuraApplication* aurApp = iter->second;
                    Aura* aura = aurApp->GetBase();
                    if (!aura->IsPermanent()
                        && aura->GetDuration() <= 30 * IN_MILLISECONDS
                        && aurApp->IsPositive()
                        && (!(aura->GetSpellInfo()->Attributes & SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY))
                        && (!aura->HasEffectType(SPELL_AURA_MOD_INVISIBILITY)))
                        player->RemoveAura(iter);
                    else
                        ++iter;
                }
            }

        CheckWinConditions();
    }
}

void Arena::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    PlayerScores[player->GetGUID()] = new ArenaScore(player->GetGUID(), player->GetBGTeamId());

    player->ResummonPetTemporaryUnSummonedIfAny();

    if (Pet* pet = player->GetPet())
    {
        if (!pet->isAlive())
            pet->setDeathState(ALIVE);

        pet->SetHealth(pet->GetMaxHealth());
        pet->RemoveAllAuras();
    }

    player->RemoveArenaEnchantments(TEMP_ENCHANTMENT_SLOT);

    uint32 team = player->GetTeam();
    if (team == ALLIANCE)
        player->CastSpell(player, player->GetBGTeam() == HORDE ? SPELL_BG_HORDE_GOLD_FLAG : SPELL_BG_ALLIANCE_GOLD_FLAG, true);
    else
        player->CastSpell(player, player->GetBGTeam() == HORDE ? SPELL_BG_HORDE_GREEN_FLAG : SPELL_BG_ALLIANCE_GREEN_FLAG, true);

    player->CastSpell(player, SPELL_BATTLE_FATIGUE, true);

    player->DestroyConjuredItems(true);
    player->UnsummonPetTemporaryIfAny();

    if (GetStatus() == STATUS_WAIT_JOIN)
    {
        player->CastSpell(player, SPELL_ARENA_PREPARATION, true);
        player->CastSpell(player, SPELL_ARENA_PEREODIC_AURA, true);
        player->CastSpell(player, SPELL_ENTERING_BATTLEGROUND, true);
        if (isRated())
            player->CastSpell(player, SPELL_RATED_PVP_TRANSFORM_SUPPRESSION, true);

        player->ResetAllPowers(true);
    }

    player->SetByteValue(PLAYER_BYTES_3, PLAYER_BYTES_3_OFFSET_ARENA_FACTION, team == HORDE ? 0 : 1);

    SendOpponentSpecialization(team);
    SendOpponentSpecialization(GetOtherTeam(team));

    UpdateArenaWorldState();
}

void Arena::RemovePlayer(Player* /*player*/, ObjectGuid /*guid*/, uint32 /*team*/)
{
    if (GetStatus() == STATUS_WAIT_LEAVE)
        return;

    UpdateArenaWorldState();
    CheckWinConditions();
}

void Arena::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(WorldStates::ARENA_SHOW_END_TIMER, GetStatus() == STATUS_IN_PROGRESS);
    packet.Worldstates.emplace_back(WorldStates::ARENA_END_TIMER, int32(time(nullptr) + std::chrono::duration_cast<Seconds>(Minutes(25) - GetElapsedTime()).count()));
    packet.Worldstates.emplace_back(WorldStates::ARENA_ALIVE_PLAYERS_GREEN, GetAlivePlayersCountByTeam(HORDE));
    packet.Worldstates.emplace_back(WorldStates::ARENA_ALIVE_PLAYERS_GOLD, GetAlivePlayersCountByTeam(ALLIANCE));
}

void Arena::UpdateArenaWorldState()
{
    UpdateWorldState(WorldStates::ARENA_ALIVE_PLAYERS_GREEN, GetAlivePlayersCountByTeam(HORDE));
    UpdateWorldState(WorldStates::ARENA_ALIVE_PLAYERS_GOLD, GetAlivePlayersCountByTeam(ALLIANCE));
}

void Arena::HandleKillPlayer(Player* player, Player* killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    Battleground::HandleKillPlayer(player, killer);

    UpdateArenaWorldState();
    CheckWinConditions();
}

void Arena::StartingEventCloseDoors()
{
    Battleground::StartingEventCloseDoors();

    UpdateWorldState(WorldStates::ARENA_SHOW_END_TIMER, 0);
}

void Arena::StartingEventOpenDoors()
{
    Battleground::StartingEventOpenDoors();

    UpdateWorldState(WorldStates::ARENA_SHOW_END_TIMER, 1);

    for (auto const& v : GetPlayers())
        if (Player* player = ObjectAccessor::FindPlayer(v.first))
            player->RemoveAurasDueToSpell(SPELL_ARENA_PREPARATION);
}

void Arena::RemovePlayerAtLeave(ObjectGuid guid, bool transport, bool sendPacket)
{
    Battleground::RemovePlayerAtLeave(guid, transport, sendPacket);

    Player* player = ObjectAccessor::FindPlayer(guid);
    if (!player)
        return;

    player->RemoveAurasDueToSpell(SPELL_ARENA_PEREODIC_AURA);
    player->RemoveAurasDueToSpell(SPELL_ENTERING_BATTLEGROUND);
    if (isRated())
        player->RemoveAurasDueToSpell(SPELL_RATED_PVP_TRANSFORM_SUPPRESSION);
}

void Arena::CheckWinConditions()
{
    if (!GetAlivePlayersCountByTeam(ALLIANCE) && GetPlayersCountByTeam(HORDE))
        EndBattleground(HORDE);
    else if (GetPlayersCountByTeam(ALLIANCE) && !GetAlivePlayersCountByTeam(HORDE))
        EndBattleground(ALLIANCE);
}

void Arena::EndBattleground(uint32 winner)
{
    Battleground::EndBattleground(winner);

    if (!isArena())
        return;

    UpdateWorldState(WorldStates::ARENA_SHOW_END_TIMER, 0);

    bool guildAwarded = false;
    uint8 aliveWinners = GetAlivePlayersCountByTeam(winner);
    std::ostringstream info;
    BracketType bType = BattlegroundMgr::BracketByJoinType(GetJoinType());

    for (BattlegroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        uint32 team = itr->second.Team;

        if (itr->second.OfflineRemoveTime)
        {
            if (isRated() && winner != WINNER_NONE)
            {
                Bracket* bracket = sBracketMgr->TryGetOrCreateBracket(itr->first, bType);
                ASSERT(bracket);    //unreal
                uint32 gain = bracket->FinishGame(team == winner, GetMatchmakerRating(team == winner ? GetOtherTeam(winner) : winner));
                info << " >> Plr. Not in game at the end of match. GUID: " << itr->first << " mmr gain: " << gain << " state:" << (team == winner) ? "WINER" : "LOSER";
            }
            continue;
        }

        Player* player = GetPlayer(itr, "EndBattleground");
        if (!player)
            continue;

        Bracket* bracket = GetJoinType() ? player->getBracket(bType) : nullptr;
        uint16 bracketRating = bracket->getRating();
        bool isWinnerTeam = team == winner;

        if (isWinnerTeam && isRated() && GetJoinType() == JOIN_TYPE_ARENA_5v5 && aliveWinners == 1 && player->isAlive())
            player->CastSpell(player, SPELL_BG_THE_LAST_STANDING, true);

        if (isRated() && winner != WINNER_NONE)
        {
            uint32 gain = bracket->FinishGame(isWinnerTeam, GetMatchmakerRating(isWinnerTeam ? GetOtherTeam(winner) : winner));
            info << " >> Plr: " << player->ToString().c_str() << " mmr gain: " << gain << " state:" << (isWinnerTeam) ? "WINER" : "LOSER";
            if (isWinnerTeam)
            {
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA, 1);
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA, GetMapId());
                player->ModifyCurrency(CURRENCY_TYPE_CONQUEST_ARENA_META, sWorld->getIntConfig(CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD));

                _arenaTeamScores[team].Assign(bracketRating, bracketRating + gain, bracket->getLastMMRChange());

                if (!guildAwarded)
                {
                    guildAwarded = true;
                    if (ObjectGuid::LowType guildId = GetBgMap()->GetOwnerGuildId(player->GetTeam()))
                        if (Guild* guild = sGuildMgr->GetGuildById(guildId))
                            guild->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA, std::max<uint32>(bracketRating, 1), 0, 0, nullptr, player);
                }
            }
            else
            {
                player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA, ACHIEVEMENT_CRITERIA_CONDITION_NO_LOSE);
                _arenaTeamScores[team].Assign(bracketRating, bracketRating - gain, bracket->getLastMMRChange());
            }

            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA, GetMapId());
        }
        else if (isRated())
        {
            _arenaTeamScores[TEAM_ALLIANCE].Assign(bracketRating, bracketRating - 16, bracket->getLastMMRChange());
            _arenaTeamScores[TEAM_HORDE].Assign(bracketRating, bracketRating - 16, bracket->getLastMMRChange());

            bracket->FinishGame(false, -16);
        }
    }

    if (isRated() && winner != WINNER_NONE)
    {
        const char* winnerTeam[5];
        const char*  loserTeam[5];
        for (uint8 i = 0; i < 5; ++i)
        {
            winnerTeam[i] = "";
            loserTeam[i] = "";
        }
        uint8 w = 0;
        uint8 l = 0;

        for (std::list<const char*>::iterator itr = m_nameList[winner].begin(); itr != m_nameList[winner].end(); ++itr)
            if (const char* winnerName = (*itr))
            {
                winnerTeam[w] = winnerName;
                w++;
            }

        for (std::list<const char*>::iterator itr = m_nameList[GetOtherTeam(winner)].begin(); itr != m_nameList[GetOtherTeam(winner)].end(); ++itr)
            if (const char* loserName = (*itr))
            {
                loserTeam[l] = loserName;
                l++;
            }

        uint32 _arenaTimer = GetElapsedTime().count();
        uint32 _min = _arenaTimer / 60;
        uint32 _sec = _arenaTimer - (_min * 60);
        const char* att = "";
        if (GetElapsedTime() < Seconds(75))
            att = "--- ATTENTION!";

        switch (m_JoinType)
        {
            case JOIN_TYPE_ARENA_2v2:
            {
                sLog->outArena("FINISH: Arena match Type: 2v2 --- Winner[%s, %s]: old rating: %u --- Loser[%s, %s]: old rating: %u --- Duration: %u min. %u sec. %s",
                               winnerTeam[0], winnerTeam[1], GetMatchmakerRating(winner), loserTeam[0], loserTeam[1], GetMatchmakerRating(GetOtherTeam(winner)), _min, _sec, att);
                break;
            }
            case JOIN_TYPE_ARENA_3v3:
            {
                sLog->outArena("FINISH: Arena match Type: 3v3 --- Winner[%s, %s, %s]: old rating: %u --- Loser[%s, %s, %s]: old rating: %u --- Duration: %u min. %u sec. %s",
                               winnerTeam[0], winnerTeam[1], winnerTeam[2], GetMatchmakerRating(winner), loserTeam[0], loserTeam[1], loserTeam[2], GetMatchmakerRating(GetOtherTeam(winner)), _min, _sec, att);
                break;
            }
            case JOIN_TYPE_ARENA_5v5:
            {
                sLog->outArena("FINISH: Arena match Type: 5v5 --- Winner[%s, %s, %s, %s, %s]: old rating: %u --- Loser[%s, %s, %s, %s, %s]: old rating: %u --- Duration: %u min. %u sec. %s",
                               winnerTeam[0], winnerTeam[1], winnerTeam[2], winnerTeam[3], winnerTeam[4], GetMatchmakerRating(winner), loserTeam[0], loserTeam[1], loserTeam[2], loserTeam[3], loserTeam[4], GetMatchmakerRating(GetOtherTeam(winner)), _min, _sec, att);
                break;
            }
            default:
                sLog->outArena("match Type: %u --- Winner: old rating: %u  --- Loser: old rating: %u| DETAIL: %s",
                               m_JoinType, GetMatchmakerRating(winner), GetMatchmakerRating(GetOtherTeam(winner)), info.str().c_str());
                break;
        }
    }
}

void Arena::SendOpponentSpecialization(uint32 team)
{
    WorldPackets::Battleground::ArenaPrepOpponentSpecializations spec;
    WorldPackets::Battleground::ArenaPrepOpponentSpecializations::OpponentSpecData data;

    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        if (Player* opponent = GetPlayerForTeam(team, itr, "SendOponentSpecialization"))
        {
            data.Guid = opponent->GetGUID();
            data.SpecializationID = opponent->GetSpecializationId(opponent->GetActiveSpec());
        }
    }

    SendPacketToTeam(GetOtherTeam(team), spec.Write());
}

Player* Arena::GetPlayer(ObjectGuid guid, bool offlineRemove, const char* context) const
{
    return Battleground::GetPlayer(guid, offlineRemove, context);
}

Player* Arena::GetPlayer(BattlegroundPlayerMap::iterator itr, const char* context)
{
    return Battleground::GetPlayer(itr, context);
}

Player* Arena::GetPlayer(BattlegroundPlayerMap::const_iterator itr, const char* context) const
{
    return Battleground::GetPlayer(itr, context);
}

Player* Arena::GetPlayerForTeam(uint32 teamId, BattlegroundPlayerMap::const_iterator itr, const char* context) const
{
    return Battleground::GetPlayerForTeam(teamId, itr, context);
}
