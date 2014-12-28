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

#include "Opcodes.h"
#include "SharedDefines.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "AuthenticationPackets.h"
#include "ClientConfigPackets.h"
#include "SystemPackets.h"

void WorldSession::SendAuthResponse(uint8 code, bool hasAccountData, bool queued, uint32 queuePos)
{
    WorldPackets::Auth::AuthResponse response;
    response.SuccessInfo.HasValue = code == AUTH_OK;
    response.Result = code;
    response.WaitInfo.HasValue = queued;
    response.WaitInfo.Value.WaitCount = queuePos;
    if (code == AUTH_OK)
    {
        response.SuccessInfo.Value.AccountExpansionLevel = Expansion();
        response.SuccessInfo.Value.ActiveExpansionLevel = Expansion();
        response.SuccessInfo.Value.VirtualRealmAddress = GetVirtualRealmAddress();

        std::string realmName = sObjectMgr->GetRealmName(realmHandle.Index);

        // Send current home realm. Also there is no need to send it later in realm queries.
        response.SuccessInfo.Value.VirtualRealms.emplace_back(GetVirtualRealmAddress(), true, false, realmName, realmName);

        response.SuccessInfo.Value.AvailableClasses = &sObjectMgr->GetClassExpansionRequirements();
        response.SuccessInfo.Value.AvailableRaces = &sObjectMgr->GetRaceExpansionRequirements();
    }

    SendPacket(response.Write());
}

void WorldSession::SendClientCacheVersion(uint32 version)
{
    WorldPackets::ClientConfig::ClientCacheVersion cache;
    cache.CacheVersion = version;

    SendPacket(cache.Write());
}

void WorldSession::SendBattlePay()
{
    WorldPacket data(SMSG_BATTLEPAY_PRODUCT_ITEM, 7);
    data << uint32(0);
    data.WriteBits(0, 19);
    data.FlushBits();
    SendPacket(&data);
}

void WorldSession::SendDisplayPromo(int32 promo)
{
    WorldPacket data(SMSG_DISPLAY_PROMOTION, 7);
    data << promo;
    SendPacket(&data);
}