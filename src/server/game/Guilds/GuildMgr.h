/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#ifndef _GUILDMGR_H
#define _GUILDMGR_H

#include "Guild.h"

class GuildMgr
{
private:
    GuildMgr();
    ~GuildMgr();

public:
    typedef std::unordered_map<ObjectGuid::LowType, Guild*> GuildContainer;

    static GuildMgr* instance()
    {
        static GuildMgr instance;
        return &instance;
    }

    Guild* GetGuildByLeader(ObjectGuid const& guid) const;
    Guild* GetGuildById(ObjectGuid::LowType guildId) const;
    Guild* GetGuildByGuid(ObjectGuid const& guid) const;
    Guild* GetGuildByName(const std::string& guildName) const;
    std::string GetGuildNameById(ObjectGuid::LowType const& guildId) const;

    void LoadGuildXpForLevel();
    void LoadGuildRewards();

    void LoadGuilds();
    void AddGuild(Guild* guild);
    void RemoveGuild(ObjectGuid::LowType guildId);

    void SaveGuilds();

    ObjectGuid::LowType GenerateGuildId();
    void SetNextGuildId(ObjectGuid::LowType Id) { NextGuildId = Id; }

    uint32 GetXPForGuildLevel(uint8 level) const;
    std::vector<GuildReward> const& GetGuildRewards() const { return GuildRewards; }

protected:
    ObjectGuid::LowType NextGuildId;
    GuildContainer GuildStore;
    std::vector<uint64> GuildXPperLevel;
    std::vector<GuildReward> GuildRewards;
};

#define sGuildMgr GuildMgr::instance()

#endif
