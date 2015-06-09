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
#include "BattlePayPackets.h"

void WorldSession::SendAuthResponse(uint8 code, bool hasAccountData, bool queued, uint32 queuePos)
{
    //! WTF?
    WorldPacket data(SMSG_DANCE_STUDIO_CREATE_RESULT, 9);
    data << uint8(128);           //onebit
    data << uint32(795877);
    data << uint32(10848087);
    data << uint32(1084761);
    data << uint32(4665878);
    SendPacket(&data);

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
        response.SuccessInfo.Value.CurrencyID = CURRENCY_RUB;
        response.SuccessInfo.Value.TimeRemain = 0;
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

void WorldSession::SendDisplayPromo(int32 promo)
{
    WorldPacket data(SMSG_DISPLAY_PROMOTION, 7);
    data << promo;
    SendPacket(&data);
}

void WorldSession::SendFeatureSystemStatusGlueScreen()
{
    WorldPackets::System::FeatureSystemStatusGlueScreen features;
    features.BpayStoreAvailable = true;
    features.BpayStoreDisabledByParentalControls = false;
    features.CharUndeleteEnabled = HasAuthFlag(AT_AUTH_FLAG_RESTORE_DELETED_CHARACTER);
    features.BpayStoreEnabled = true/*sWorld->getBoolConfig(CONFIG_FEATURE_SYSTEM_BPAY_STORE_ENABLED)*/;
    features.CommerceSystemEnabled = true;
    features.Unk14 = true;
    features.TokenPollTimeSeconds = 300;
    SendPacket(features.Write());
}

//! 6.1.2
void WorldSession::HandleUndeleteCharacterCooldownnStatus(WorldPacket& /*recvPacket*/)
{
    //CMSG_GET_UNDELETE_CHARACTER_COOLDOWN_STATUS
    WorldPacket data(SMSG_UNDELETE_COOLDOWN_STATUS_RESPONSE, 9);
    data << uint8(0);           //onebit
    data << uint32(0);          //CD for free restore deleted character. 2592000
    data << uint32(0);
    SendPacket(&data);
}

//! 6.1.2
void WorldSession::HandleWowTokenMarketPrice(WorldPacket& /*recvPacket*/)
{
    WorldPacket data(SMSG_REQUEST_WOW_TOKEN_MARKET_PRICE_RESPONSE, 20);
    data << uint64(416460000);                                          //CurrentMarketPrice
    data << uint32(0);
    data << uint32(TOKEN_RESULT_SUCCESS);
    data << uint32(14400);
    SendPacket(&data);
}