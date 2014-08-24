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

#include "ByteBuffer.h"
#include "MessageBuffer.h"
#include "Common.h"
#include "Log.h"
#include <sstream>

ByteBuffer::ByteBuffer(MessageBuffer&& buffer) : _rpos(0), _wpos(0), _bitpos(InitialBitPos), _curbitval(0), _storage(buffer.Move())
{
}

ByteBufferPositionException::ByteBufferPositionException(bool add, size_t pos, size_t size, size_t valueSize)
: ByteBufferException(pos, size, valueSize), _add(add)
{
    PrintError();
}

void ByteBufferPositionException::PrintError() const
{
    sLog->outError(LOG_FILTER_GENERAL, "Attempted to %s value with size: %zu in ByteBuffer (pos: %zu size: %zu)",
        (_add ? "put" : "get"), ValueSize, Pos, Size);
}

ByteBufferSourceException::ByteBufferSourceException(size_t pos, size_t size, size_t valueSize)
    : ByteBufferException(pos, size, valueSize)
{
    PrintError();
}

void ByteBufferSourceException::PrintError() const
{
    sLog->outError(LOG_FILTER_GENERAL, "Attempted to put a %s in ByteBuffer (pos: %zu size: %zu)",
        (ValueSize > 0 ? "NULL-pointer" : "zero-sized value"), Pos, Size);
}

void ByteBuffer::print_storage() const
{
    if (!sLog->ShouldLog(LOG_FILTER_NETWORKIO, LOG_LEVEL_TRACE)) // optimize disabled debug output
        return;

    std::ostringstream o;
    o << "STORAGE_SIZE: " << size();
    for (uint32 i = 0; i < size(); ++i)
        o << read<uint8>(i) << " - ";
    o << " ";

    sLog->outTrace(LOG_FILTER_NETWORKIO, "%s", o.str().c_str());
}

void ByteBuffer::textlike() const
{
    if (!sLog->ShouldLog(LOG_FILTER_NETWORKIO, LOG_LEVEL_TRACE)) // optimize disabled debug output
        return;

    std::ostringstream o;
    o << "STORAGE_SIZE: " << size();
    for (uint32 i = 0; i < size(); ++i)
    {
        char buf[2];
        snprintf(buf, 2, "%c", read<uint8>(i));
        o << buf;
    }
    o << " ";
    sLog->outTrace(LOG_FILTER_NETWORKIO, "%s", o.str().c_str());
}

void ByteBuffer::hexlike() const
{
    if (!sLog->ShouldLog(LOG_FILTER_NETWORKIO, LOG_LEVEL_TRACE)) // optimize disabled debug output
        return;

    uint32 j = 1, k = 1;

    std::ostringstream o;
    o << "STORAGE_SIZE: " << size();

    for (uint32 i = 0; i < size(); ++i)
    {
        char buf[3];
        snprintf(buf, 3, "%2X ", read<uint8>(i));
        if ((i == (j * 8)) && ((i != (k * 16))))
        {
            o << "| ";
            ++j;
        }
        else if (i == (k * 16))
        {
            o << "\n";
            ++k;
            ++j;
        }

        o << buf;
    }
    o << " ";
    sLog->outTrace(LOG_FILTER_NETWORKIO, "%s", o.str().c_str());
}
