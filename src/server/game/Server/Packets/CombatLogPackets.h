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

#ifndef CombatLogPackets_h__
#define CombatLogPackets_h__

#include "Packet.h"
#include "SpellPackets.h"

namespace WorldPackets
{
    namespace CombatLog
    {
        class SpellNonMeleeDmgLog final : public ServerPacket
        {
        public:
            SpellNonMeleeDmgLog() : ServerPacket(SMSG_SPELL_NON_MELEE_DAMAGE_LOG, 60) { }

            WorldPacket const* Write() override;

            struct SpellNonMeleeDamageLogDebugInfo
            {
                float CritRoll = 0.f;
                float CritNeeded = 0.f;
                float HitRoll = 0.f;
                float HitNeeded = 0.f;
                float MissChance = 0.f;
                float DodgeChance = 0.f;
                float ParryChance = 0.f;
                float BlockChance = 0.f;
                float GlanceChance = 0.f;
                float CrushChance = 0.f;
            };

            ObjectGuid Me;
            ObjectGuid CasterGUID;
            uint32 SpellID = 0;
            uint32 Damage = 0;
            uint32 OverKill = 0;
            uint8 SchoolMask = 0;
            uint32 ShieldBlock = 0;
            uint32 Resisted = 0;
            uint32 Absorbed = 0;
            bool Periodic = false;
            uint16 Flags = 0;

            Optional<Spells::SpellCastLogData> LogData;
            Optional<SpellNonMeleeDamageLogDebugInfo> DebugInfo;
        };
    }
}

#endif // CombatLogPackets_h__
