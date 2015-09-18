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

#include "CombatLogPackets.h"
#include "SpellPackets.h"

WorldPacket const* WorldPackets::CombatLog::SpellNonMeleeDmgLog::Write()
{
    _worldPacket << Me;
    _worldPacket << CasterGUID;

    _worldPacket << SpellID;
    _worldPacket << Damage;
    _worldPacket << OverKill;

    _worldPacket << SchoolMask;

    _worldPacket << ShieldBlock;
    _worldPacket << Resisted;
    _worldPacket << Absorbed;

    _worldPacket.WriteBit(Periodic);
    _worldPacket.WriteBits(Flags, 9);
    _worldPacket.WriteBit(LogData.is_initialized());
    _worldPacket.WriteBit(DebugInfo.is_initialized());

    _worldPacket.FlushBits();

    if (DebugInfo)
    {
        _worldPacket << DebugInfo->CritRoll;
        _worldPacket << DebugInfo->CritNeeded;
        _worldPacket << DebugInfo->HitRoll;
        _worldPacket << DebugInfo->HitNeeded;
        _worldPacket << DebugInfo->MissChance;
        _worldPacket << DebugInfo->DodgeChance;
        _worldPacket << DebugInfo->ParryChance;
        _worldPacket << DebugInfo->BlockChance;
        _worldPacket << DebugInfo->GlanceChance;
        _worldPacket << DebugInfo->CrushChance;
    }

    if (LogData)
        _worldPacket << *LogData;

    return &_worldPacket;
}
