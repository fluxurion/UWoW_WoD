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
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Log.h"
#include "Opcodes.h"
#include "UpdateData.h"
#include "Player.h"
#include "SocialMgr.h"
#include "ScriptMgr.h"
#include "GameObject.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "DuelPackets.h"

void WorldSession::HandleDuelResponse(WorldPackets::Duel::DuelResponse& packet)
{
    if (packet.Accepted)
        HandleDuelAccepted();
    else
        HandleDuelCancelled();
}

void WorldSession::HandleDuelAccepted()
{
    if (!GetPlayer()->duel)                                  // ignore accept from duel-sender
        return;

    Player* player = GetPlayer();
    Player* plTarget = player->duel->opponent;

    if (player == player->duel->initiator || !plTarget || player == plTarget || player->duel->startTime != 0 || plTarget->duel->startTime != 0)
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Player 1 is: %s (%s)", player->GetGUID().ToString().c_str(), player->GetName());
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Player 2 is: %s (%s)", plTarget->GetGUID().ToString().c_str(), plTarget->GetName());

    time_t now = time(NULL);
    player->duel->startTimer = now;
    plTarget->duel->startTimer = now;

    if (sWorld->getBoolConfig(CONFIG_FUN_OPTION_ENABLED))
    {
        // reset cooldowns and HP/Mana
        player->SetHealth(player->GetMaxHealth());
        plTarget->SetHealth(plTarget->GetMaxHealth());

        if (player->getPowerType() == POWER_MANA)
            player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
        if (plTarget->getPowerType() == POWER_MANA)
            plTarget->SetPower(POWER_MANA, plTarget->GetMaxPower(POWER_MANA));

        //only for cooldowns which < 15 min
        if (!player->GetMap()->IsDungeon())
        {
            player->RemoveArenaSpellCooldowns();
            plTarget->RemoveArenaSpellCooldowns();
        }
    }

    player->SendDuelCountdown(3000);
    player->OnEnterPvPCombat();

    plTarget->SendDuelCountdown(3000);
    plTarget->OnEnterPvPCombat();
}

void WorldSession::HandleDuelCancelled()
{
    Player* player = GetPlayer();
    if (!player)
        return;

    // no duel requested
    if (!player->duel)
        return;

    // player surrendered in a duel using /forfeit
    if (player->duel->startTime != 0)
    {
        player->CombatStopWithPets(true);
        if (player->duel->opponent)
            player->duel->opponent->CombatStopWithPets(true);

        player->CastSpell(GetPlayer(), 7267, true);    // beg
        player->DuelComplete(DUEL_WON);
        return;
    }

    player->DuelComplete(DUEL_INTERRUPTED);
}

void WorldSession::HandleCanDuel(WorldPackets::Duel::CanDuel& packet)
{
    Player* player = ObjectAccessor::FindPlayer(packet.TargetGUID);
    if (!player)
        return;

    WorldPackets::Duel::CanDuelResult response;
    response.TargetGUID = packet.TargetGUID;
    response.Result = !player->duel;
    SendPacket(response.Write());

    if (response.Result)
    {
        if (_player->IsMounted())
            _player->CastSpell(player, 62875);
        else
            _player->CastSpell(player, 7266);
    }
}
