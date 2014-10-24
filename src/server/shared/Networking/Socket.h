/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
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

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "ByteBuffer.h"
#include "MessageBuffer.h"
#include "Log.h"
#include <atomic>
#include <vector>
#include <mutex>
#include <queue>
#include <memory>
#include <functional>
#include <type_traits>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>


#define GetHeaderByType(x) (x ? _authReadHeaderBuffer : _worldReadHeaderBuffer )

using boost::asio::ip::tcp;

#define READ_BLOCK_SIZE 4096

template<class T, class PacketType>
class Socket : public std::enable_shared_from_this<T>
{
    typedef typename std::conditional<std::is_pointer<PacketType>::value, PacketType, PacketType const&>::type WritePacketType;

public:
    Socket(tcp::socket&& socket, std::size_t const& authHeaderSize, std::size_t const& worldHeaderSize) : _socket(std::move(socket)), _remoteAddress(_socket.remote_endpoint().address()),
        _remotePort(_socket.remote_endpoint().port()), _authReadHeaderBuffer(), _worldReadHeaderBuffer(), _readDataBuffer(), _closed(false), _closing(false)
    {
        _authReadHeaderBuffer.Grow(authHeaderSize);
        _worldReadHeaderBuffer.Grow(worldHeaderSize);
    }

    virtual ~Socket()
    {
        boost::system::error_code error;
        _socket.close(error);

        while (!_writeQueue.empty())
        {
            DeletePacket(_writeQueue.front());
            _writeQueue.pop();
        }
    }

    virtual void Start() = 0;

    boost::asio::ip::address GetRemoteIpAddress() const
    {
        return _remoteAddress;
    }

    uint16 GetRemotePort() const
    {
        return _remotePort;
    }

    void AsyncReadHeader(bool auth)
    {
        if (!IsOpen())
            return;

        GetHeaderByType(auth).ResetWritePointer();
        _readDataBuffer.Reset();

        AsyncReadMissingHeaderData(auth);
    }

    void AsyncReadData(std::size_t size, bool auth)
    {
        if (!IsOpen())
            return;

        if (!size)
        {
            // if this is a packet with 0 length body just invoke handler directly
            ReadDataHandler(auth);
            return;
        }

        _readDataBuffer.Grow(size);
        AsyncReadMissingData(auth);
    }

    void ReadData(std::size_t size)
    {
        if (!IsOpen())
            return;

        boost::system::error_code error;

        _readDataBuffer.Grow(size);

        std::size_t bytesRead = boost::asio::read(_socket, boost::asio::buffer(_readDataBuffer.GetWritePointer(), size), error);

        _readDataBuffer.WriteCompleted(bytesRead);

        if (error || !_readDataBuffer.IsMessageReady())
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Socket::ReadData: %s errored with: %i (%s)", GetRemoteIpAddress().to_string().c_str(), error.value(),
                error.message().c_str());

            CloseSocket();
        }
    }

    void AsyncWrite(WritePacketType data)
    {
        boost::asio::async_write(_socket, boost::asio::buffer(data), std::bind(&Socket<T, PacketType>::WriteHandler, this->shared_from_this(),
            std::placeholders::_1, std::placeholders::_2));
    }

    bool IsOpen() const { return !_closed && !_closing; }

    virtual void CloseSocket()
    {
        if (_closed.exchange(true))
            return;

        boost::system::error_code shutdownError;
        _socket.shutdown(boost::asio::socket_base::shutdown_send, shutdownError);
        if (shutdownError)
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Socket::CloseSocket: %s errored when shutting down socket: %i (%s)", GetRemoteIpAddress().to_string().c_str(),
                shutdownError.value(), shutdownError.message().c_str());
    }

    /// Marks the socket for closing after write buffer becomes empty
    void DelayedCloseSocket() { _closing = true; }

    virtual bool IsHeaderReady(bool auth) const { return GetHeaderByType(auth).IsMessageReady(); }
    virtual bool IsDataReady() const { return _readDataBuffer.IsMessageReady(); }

    uint8* GetHeaderBuffer(bool auth) { return GetHeaderByType(auth).Data(); }
    uint8* GetDataBuffer() { return _readDataBuffer.Data(); }

    size_t GetHeaderSize(bool auth) const { return GetHeaderByType(auth).GetReadyDataSize(); }
    size_t GetDataSize() const { return _readDataBuffer.GetReadyDataSize(); }

    MessageBuffer&& MoveHeader(bool auth) { return std::move(GetHeaderByType(auth)); }
    MessageBuffer&& MoveData() { return std::move(_readDataBuffer); }

protected:
    virtual void ReadHeaderHandler(bool auth) = 0;
    virtual void ReadDataHandler(bool auth) = 0;

    std::mutex _writeLock;
    std::queue<PacketType> _writeQueue;

private:
    void AsyncReadMissingHeaderData(bool auth)
    {
        auto& header = GetHeaderByType(auth);

        try
        {
            _socket.async_read_some(boost::asio::buffer(header.GetWritePointer(), std::min<std::size_t>(READ_BLOCK_SIZE, header.GetMissingSize())),
                std::bind(&Socket<T, PacketType>::ReadHeaderHandlerInternal, this->shared_from_this(), auth, std::placeholders::_1, std::placeholders::_2));
        }
        catch (std::exception const& e)
        {

        }
    }

    void AsyncReadMissingData(bool auth)
    {
        try
        {
            _socket.async_read_some(boost::asio::buffer(_readDataBuffer.GetWritePointer(), std::min<std::size_t>(READ_BLOCK_SIZE, _readDataBuffer.GetMissingSize())),
                std::bind(&Socket<T, PacketType>::ReadDataHandlerInternal, this->shared_from_this(), auth, std::placeholders::_1, std::placeholders::_2));
        }
        catch (std::exception const& e)
        {

        }
    }

    void ReadHeaderHandlerInternal(bool auth, boost::system::error_code error, size_t transferredBytes)
    {
        if (error)
        {
            CloseSocket();
            return;
        }

        GetHeaderByType(auth).WriteCompleted(transferredBytes);

        if (!IsHeaderReady(auth))
        {
            // incomplete, read more
            AsyncReadMissingHeaderData(auth);
            return;
        }

        ReadHeaderHandler(auth);
    }

    void ReadDataHandlerInternal(bool auth, boost::system::error_code error, size_t transferredBytes)
    {
        if (error)
        {
            CloseSocket();
            return;
        }

        _readDataBuffer.WriteCompleted(transferredBytes);

        if (!IsDataReady())
        {
            // incomplete, read more
            AsyncReadMissingData(auth);
            return;
        }

        ReadDataHandler(auth);
    }

    void WriteHandler(boost::system::error_code error, size_t /*transferedBytes*/)
    {
        if (!error)
        {
            std::lock_guard<std::mutex> deleteGuard(_writeLock);

            DeletePacket(_writeQueue.front());
            _writeQueue.pop();

            if (!_writeQueue.empty())
                AsyncWrite(_writeQueue.front());
            else if (_closing)
                CloseSocket();
        }
        else
            CloseSocket();
    }

    template<typename Q = PacketType>
    typename std::enable_if<std::is_pointer<Q>::value>::type DeletePacket(PacketType& packet) { delete packet; }

    template<typename Q = PacketType>
    typename std::enable_if<!std::is_pointer<Q>::value>::type DeletePacket(PacketType const& /*packet*/) { }

    tcp::socket _socket;

    boost::asio::ip::address _remoteAddress;
    uint16 _remotePort;

    //inline MessageBuffer& GetHeaderByType(bool auth) { return auth ? _authReadHeaderBuffer : _worldReadHeaderBuffer; }
    //inline MessageBuffer const& GetHeaderByType(bool auth) const { return auth ? _authReadHeaderBuffer : _worldReadHeaderBuffer; }

    MessageBuffer _authReadHeaderBuffer;
    MessageBuffer _worldReadHeaderBuffer;
    MessageBuffer _readDataBuffer;

    std::atomic<bool> _closed;
    std::atomic<bool> _closing;
};

#endif // __SOCKET_H__
