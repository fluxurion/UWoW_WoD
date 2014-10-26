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

#ifndef __SERVERPKTHDR_H__
#define __SERVERPKTHDR_H__

#pragma pack(push, 1)

union ServerPktHeader
{
    static uint32 const SizeOf[2];

    ServerPktHeader() { memset(data, 0, sizeof(data)); }

    char data[6];
    struct
    {
        char raw[6];
        void Set(uint32 size, uint32 cmd)
        {
            memcpy(raw, &size, 2);
            memcpy(&raw[2], &cmd, 4);
        }
    } Setup;

    struct
    {
        uint32 Command : 13;
        uint32 Size : 19;
    } Normal;
};

#pragma pack(pop)

#endif
