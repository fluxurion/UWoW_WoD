/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
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

#include "WorldSession.h"
#include "GuildMgr.h"
#include "InspectPackets.h"
#include "Player.h"
#include "Bracket.h"

void WorldSession::HandleInspect(WorldPackets::Inspect::Inspect& packet)
{
    Player* player = ObjectAccessor::FindPlayer(packet.Target);
    if (!player)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSession::HandleInspect: Target %s not found.", packet.Target.ToString().c_str());
        return;
    }

    uint8 activeSpec = player->GetActiveSpec();
    Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId());

    if (GetPlayer()->IsValidAttackTarget(player))
        return;

    WorldPackets::Inspect::InspectResult inspectResult;

    for (uint8 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            inspectResult.Items.emplace_back(item, i);
    }

    inspectResult.ClassID = player->getClass();
    inspectResult.GenderID = player->getGender();

    if (sWorld->getBoolConfig(CONFIG_TALENTS_INSPECTING) || GetPlayer()->isGameMaster())
    {
        PlayerTalentMap const* talents = player->GetTalentMap(activeSpec);
        for (PlayerTalentMap::value_type const& v : *talents)
        {
            SpellInfo const* spell = sSpellMgr->GetSpellInfo(v.first);
            if (spell && spell->talentId)
                inspectResult.Talents.push_back(spell->talentId);
        }

        for (uint8 i = 0; i < MAX_GLYPH_SLOT_INDEX; ++i)
        {
            uint32 glyph = player->GetGlyph(activeSpec, i);
            if (!glyph)
                continue;
            
            inspectResult.Glyphs.push_back(glyph);
        }
    }

    if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId()))
    {
        inspectResult.GuildData = boost::in_place();
        inspectResult.GuildData->GuildGUID = guild->GetGUID();
        inspectResult.GuildData->NumGuildMembers = guild->GetMembersCount();
        inspectResult.GuildData->AchievementPoints = guild->GetAchievementMgr().GetAchievementPoints();
    }

    inspectResult.InspecteeGUID = packet.Target;
    inspectResult.SpecializationID = player->GetSpecializationId(activeSpec);

    SendPacket(inspectResult.Write());
}

void WorldSession::HandleRequestHonorStats(WorldPackets::Inspect::RequestHonorStats& packet)
{
    Player* player = ObjectAccessor::FindPlayer(packet.TargetGUID);
    if (!player)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSession::HandleRequestHonorStats: Target %s not found.", packet.TargetGUID.ToString().c_str());
        return;
    }

    WorldPackets::Inspect::InspectHonorStats honorStats;
    honorStats.PlayerGUID  = packet.TargetGUID;
    honorStats.LifetimeHK  = player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);
    honorStats.YesterdayHK = player->GetUInt16Value(PLAYER_FIELD_YESTERDAY_HONORABLE_KILLS, 1);
    honorStats.TodayHK     = player->GetUInt16Value(PLAYER_FIELD_YESTERDAY_HONORABLE_KILLS, 0);
    honorStats.LifetimeMaxRank = 0; /// @todo
    SendPacket(honorStats.Write());
}

void WorldSession::HandleInspectPVP(WorldPackets::Inspect::InspectPVPRequest& packet)
{
    Player* player = ObjectAccessor::FindPlayer(packet.InspectTarget);
    if (!player)
        return;
        
    WorldPackets::Inspect::InspectPVPResponse response;
    WorldPackets::Inspect::PVPBracketData data;
    response.ClientGUID = packet.InspectTarget;
   
    for (BracketType i = BRACKET_TYPE_ARENA_2; i < BRACKET_TYPE_MAX; ++i)
    {
        Bracket* bracket = player->getBracket(i);
        ASSERT(bracket);

        data.Rating           = bracket->getRating();
        data.Rank             = 0;
        data.WeeklyPlayed     = bracket->GetBracketInfo(BRACKET_WEEK_GAMES);
        data.WeeklyWon        = bracket->GetBracketInfo(BRACKET_WEEK_WIN);
        data.SeasonPlayed     = bracket->GetBracketInfo(BRACKET_SEASON_GAMES);
        data.SeasonWon        = bracket->GetBracketInfo(BRACKET_SEASON_WIN);
        data.WeeklyBestRating = 0;
        data.Bracket          = i;
    }

    response.Bracket.push_back(data);
    SendPacket(response.Write());
}

void WorldSession::HandleQueryInspectAchievements(WorldPackets::Inspect::QueryInspectAchievements& inspect)
{
    if (Player* player = ObjectAccessor::FindPlayer(inspect.Guid))
        player->GetAchievementMgr().SendAchievementInfo(_player);
}
