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

#ifndef AuthenticationPacketsWorld_h__
#define AuthenticationPacketsWorld_h__

#include "Packet.h"
#include "ObjectMgr.h"
#include "Common.h"
#include "BigNumber.h"
#include "SHA1.h"
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

namespace WorldPackets
{
    namespace Auth
    {
        class AuthChallenge final : public ServerPacket
        {
        public:
            AuthChallenge() : ServerPacket(SMSG_AUTH_CHALLENGE, 4 + 32 + 1) { }

            WorldPacket const* Write() override;

            uint32 Challenge = 0;
            uint32 DosChallenge[8];
            uint8 DosZeroBits = 0;
        };

        class AuthSession final : public ClientPacket
        {
        public:
            AuthSession(WorldPacket&& packet) : ClientPacket(CMSG_AUTH_SESSION, std::move(packet))
            {
                memset(Digest, 0, SHA_DIGEST_LENGTH);
            }

            void Read() override;

            uint32 BattlegroupID = 0;
            int8 LoginServerType = 0;
            int8 BuildType = 0;
            uint32 RealmID = 0;
            uint16 Build = 0;
            uint32 LocalChallenge = 0;
            int32 LoginServerID = 0;
            uint32 RegionID = 0;
            uint64 DosResponse = 0;
            uint8 Digest[SHA_DIGEST_LENGTH];
            std::string Account;
            bool UseIPv6 = false;
            ByteBuffer AddonInfo;
        };

        class AuthResponse final : public ServerPacket
        {
        public:
            struct RealmInfo
            {
                RealmInfo(uint32 realmAddress, bool isHomeRealm, bool isInternalRealm, std::string const& realmNameActual, std::string const& realmNameNormalized) :
                    RealmAddress(realmAddress), IsLocal(isHomeRealm), IsInternalRealm(isInternalRealm), RealmNameActual(realmNameActual), RealmNameNormalized(realmNameNormalized)
                { }

                uint32 RealmAddress;
                bool IsLocal;
                bool IsInternalRealm;
                std::string RealmNameActual;
                std::string RealmNameNormalized;
            };

            struct CharacterTemplate
            {
                struct TemplateClass
                {
                    uint8 Class;
                    uint8 FactionGroup;
                };

                uint32 TemplateSetId;
                std::list<TemplateClass> Classes;
                std::string Name;
                std::string Description;
            };

            struct AuthSuccessInfo
            {
                uint32 TimeRemain = 0;
                uint8 AccountExpansionLevel = 0;
                uint8 ActiveExpansionLevel = 0;
                uint32 TimeRested = 0;
                uint8 TimeOptions = 0;

                uint32 VirtualRealmAddress = 0;
                uint32 RealmNamesCount = 0;
                uint32 TimeSecondsUntilPCKick = 0;
                uint32 CurrencyID = 0;

                std::vector<RealmInfo> VirtualRealms;
                std::vector<CharacterTemplate> Templates;

                ExpansionRequirementContainer const* AvailableClasses = nullptr;
                ExpansionRequirementContainer const* AvailableRaces = nullptr;

                bool IsExpansionTrial = false;
                bool ForceCharacterTemplate = false;
                Optional<uint16> NumPlayersHorde;
                Optional<uint16> NumPlayersAlliance;
                bool IsVeteranTrial = false;
            };

            struct AuthWaitInfo
            {
                uint32 WaitCount = 0;
                bool HasFCM = false;
            };

            AuthResponse();

            WorldPacket const* Write() override;

            Optional<AuthSuccessInfo> SuccessInfo;
            Optional<AuthWaitInfo> WaitInfo;
            uint8 Result = 0;
        };

        enum class ConnectToSerial : uint32
        {
            None = 0,
            Realm = 14,
            WorldAttempt1 = 17,
            WorldAttempt2 = 35,
            WorldAttempt3 = 53,
            WorldAttempt4 = 71,
            WorldAttempt5 = 89
        };

        class ConnectTo final : public ServerPacket
        {
            static std::string const Haiku;
            static uint8 const PiDigits[130];

            struct ConnectPayload
            {
                tcp::endpoint Where;
                uint32 Adler32 = 0;
                uint8 XorMagic = 0x2A;
                uint8 PanamaKey[32];
            };

        public:
            ConnectTo();

            WorldPacket const* Write() override;

            uint64 Key = 0;
            ConnectToSerial Serial = ConnectToSerial::None;
            ConnectPayload Payload;
            uint8 Con = 0;

        private:
            BigNumber p;
            BigNumber q;
            BigNumber dmp1;
            BigNumber dmq1;
            BigNumber iqmp;
        };

        class AuthContinuedSession final : public ClientPacket
        {
        public:
            AuthContinuedSession(WorldPacket&& packet) : ClientPacket(CMSG_AUTH_CONTINUED_SESSION, std::move(packet))
            {
                memset(Digest, 0, SHA_DIGEST_LENGTH);
            }

            void Read() override;

            uint64 DosResponse = 0;
            uint64 Key = 0;
            uint8 Digest[SHA_DIGEST_LENGTH];
        };

        class ResumeComms final : public ServerPacket
        {
        public:
            ResumeComms(ConnectionType connection) : ServerPacket(SMSG_RESUME_COMMS, 0) { }

            WorldPacket const* Write() override { return &_worldPacket; }
        };

        class ConnectToFailed final : public ClientPacket
        {
        public:
            ConnectToFailed(WorldPacket&& packet) : ClientPacket(CMSG_CONNECT_TO_FAILED, std::move(packet)) { }

            void Read() override;

            ConnectToSerial Serial = ConnectToSerial::None;
            uint8 Con = 0;
        };
    }
}

#endif // AuthenticationPacketsWorld_h__
