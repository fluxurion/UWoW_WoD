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
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Log.h"
#include "Player.h"
#include "World.h"
#include "ObjectMgr.h"
#include "WorldSession.h"
#include "ChallengeMgr.h"
#include "ChallengeModePackets.h"

void WorldSession::HandleRequestLeaders(WorldPackets::ChallengeMode::RequestLeaders& packet)
{
    WorldPackets::ChallengeMode::RequestLeadersResult result;

    result.MapID = packet.MapId;
    result.LastGuildUpdate = 624653;
    result.LastRealmUpdate = getMSTime();

    if (Challenge* bestGuild = sChallengeMgr->BestGuildChallenge(_player->GetGuildId(), packet.MapId))
    {
        WorldPackets::ChallengeMode::ModeAttempt guildLeaders = result.GuildLeaders[bestGuild->member.size()];

        for(ChallengeMemberList::iterator itr = bestGuild->member.begin(); itr != bestGuild->member.end(); ++itr)
        {
            WorldPackets::ChallengeMode::ModeAttempt::Member memberData = guildLeaders.Members[bestGuild->member.size()];
            for(ChallengeMemberList::iterator itr = bestGuild->member.begin(); itr != bestGuild->member.end(); ++itr)
            {
                ChallengeMember member = *itr;

                memberData.VirtualRealmAddress = GetVirtualRealmAddress();
                memberData.NativeRealmAddress = GetVirtualRealmAddress();
                memberData.Guid = member.guid;
                memberData.SpecializationID = member.specId;
            }

            guildLeaders.InstanceRealmAddress = GetVirtualRealmAddress();
            guildLeaders.AttemptID = bestGuild->guildId;
            guildLeaders.CompletionTime = bestGuild->recordTime;
            guildLeaders.CompletionDate = bestGuild->date;
            guildLeaders.MedalEarned = bestGuild->medal;
        }
    }

    if (Challenge* bestServer = sChallengeMgr->BestServerChallenge(packet.MapId))
    {
        WorldPackets::ChallengeMode::ModeAttempt realmLeaders = result.RealmLeaders[bestServer ? bestServer->member.size() : 0];

        for(ChallengeMemberList::iterator itr = bestServer->member.begin(); itr != bestServer->member.end(); ++itr)
        {
            WorldPackets::ChallengeMode::ModeAttempt::Member memberData = realmLeaders.Members[bestServer->member.size()];
            for(ChallengeMemberList::iterator itr = bestServer->member.begin(); itr != bestServer->member.end(); ++itr)
            {
                ChallengeMember member = *itr;

                memberData.VirtualRealmAddress = GetVirtualRealmAddress();
                memberData.NativeRealmAddress = GetVirtualRealmAddress();
                memberData.Guid = member.guid;
                memberData.SpecializationID = member.specId;
            }

            realmLeaders.InstanceRealmAddress = GetVirtualRealmAddress();
            realmLeaders.AttemptID = bestServer->guildId;
            realmLeaders.CompletionTime = bestServer->recordTime;
            realmLeaders.CompletionDate = bestServer->date;
            realmLeaders.MedalEarned = bestServer->medal;
        }
    }

    SendPacket(result.Write());
}

void WorldSession::HandleGetChallengeModeRewards(WorldPackets::ChallengeMode::Misc& /*packet*/)
{
    WorldPackets::ChallengeMode::Rewards rewards;
    WorldPackets::ChallengeMode::MapChallengeModeReward mapRewards = rewards.MapChallengeModeRewards[sMapChallengeModeStore.GetFieldCount() - 1]; // -1 really needed?
    WorldPackets::ChallengeMode::ItemReward itemReward = rewards.ItemRewards[0];
    
    for (MapChallengeModeEntry const* challenge : sMapChallengeModeStore)
    {
        mapRewards.MapId = challenge->map;

        WorldPackets::ChallengeMode::MapChallengeModeReward::ChallengeModeReward mapReward = mapRewards.Rewards[CHALLENGE_MEDAL_PLAT];

        for (int32 i = CHALLENGE_MEDAL_NONE; i < CHALLENGE_MEDAL_PLAT; ++i)
        {
            mapReward.Money = 198450; // "best time in seconds." - wat? what is it ? best time? 
            //for (bla bla bla)
            mapReward.CurrencyRewards.emplace_back(uint32(CURRENCY_TYPE_VALOR_POINTS), sChallengeMgr->GetValorPointsReward(i));
        }
    }

    SendPacket(rewards.Write());
}

void WorldSession::HandleChallengeModeRequestMapStats(WorldPackets::ChallengeMode::Misc& /*packet*/)
{
    WorldPackets::ChallengeMode::AllMapStats stats;
    if (ChallengeByMap* best = sChallengeMgr->BestForMember(_player->GetGUID()))
    {
        WorldPackets::ChallengeMode::ChallengeModeMap modeMap = stats.ChallengeModeMaps[best ? best->size() : 0];
        for(ChallengeByMap::iterator itr = best->begin(); itr != best->end(); ++itr)
        {
            for(ChallengeMemberList::iterator i = itr->second->member.begin(); i != itr->second->member.end(); ++i)
                modeMap.BestSpecID.emplace_back(i->specId);

            modeMap.BestMedalDate = itr->second->date;
            
            modeMap.MapId = itr->second->mapID;
            modeMap.BestCompletionMilliseconds = itr->second->recordTime;
            modeMap.LastCompletionMilliseconds = itr->second->recordTime; //ToDo: need create one more holder on challenge mgr with last record. Is it trully need?
            modeMap.BestMedal = itr->second->medal;
        }
    }

    SendPacket(stats.Write());
}
