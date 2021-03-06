/*
* Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
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

#ifndef __WORLDSOCKET_H__
#define __WORLDSOCKET_H__

#include "Common.h"
#include "MessageBuffer.h"
#include "WorldPacketCrypt.h"
#include "ServerPktHeader.h"
#include "Socket.h"
#include "Util.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include <chrono>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/buffer.hpp>

using boost::asio::ip::tcp;
struct z_stream_s;

namespace WorldPackets
{
    class ServerPacket;
    namespace Auth
    {
        class AuthSession;
        class AuthContinuedSession;
        class ConnectToFailed;
    }
}

#pragma pack(push, 1)

union ClientPktHeader
{
    struct
    {
        uint16 Size;
        uint32 Command;
    } Setup;

    struct
    {
        uint32 Command : 13;
        uint32 Size : 19;
    } Normal;


    static bool IsValidSize(uint32 size) { return size < 10240; }
    static bool IsValidOpcode(uint32 opcode) { return opcode < OPCODE_COUNT; }
};

#pragma pack(pop)

class WorldSocket : public Socket<WorldSocket>
{
    static std::string const ServerConnectionInitialize;
    static std::string const ClientConnectionInitialize;
    static uint32 const MinSizeForCompression;

public:
    WorldSocket(tcp::socket&& socket);
    ~WorldSocket();

    WorldSocket(WorldSocket const& right) = delete;
    WorldSocket& operator=(WorldSocket const& right) = delete;

    void Start() override;

    void SendPacket(WorldPacket const& packet);

protected:
    void ReadHandler() override;
    bool ReadHeaderHandler();
    bool ReadDataHandler();

private:
    void HandleSendAuthSession();
    void HandleAuthSession(WorldPackets::Auth::AuthSession& authSession);
    void SendAuthResponseError(uint8 code);
    void HandleConnectToFailed(WorldPackets::Auth::ConnectToFailed& connectToFailed);

    void WritePacketToBuffer(WorldPacket const& packet, MessageBuffer& buffer);
    uint32 CompressPacket(uint8* buffer, WorldPacket const& packet);

    void HandlePing(WorldPacket& recvPacket);

    void ExtractOpcodeAndSize(ClientPktHeader const* header, uint32& opcode, uint32& size) const;

    uint32 _authSeed;
    WorldPacketCrypt _authCrypt;
    BigNumber _encryptSeed;
    BigNumber _decryptSeed;

    TimePoint _LastPingTime;
    uint32 _OverSpeedPings;

    WorldSession* _worldSession;

    MessageBuffer _headerBuffer;
    MessageBuffer _packetBuffer;

    z_stream_s* _compressionStream;

    bool _initialized;
};

#endif
