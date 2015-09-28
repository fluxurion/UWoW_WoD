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

#ifndef CONVERSATIONOBJECT_H
#define CONVERSATIONOBJECT_H

#include "Object.h"
#include "UpdateMask.h"
#include "UpdateFieldFlags.h"

class Conversation : public WorldObject, public GridObject<Conversation>
{
    public:
        Conversation();
        ~Conversation();

        void AddToWorld();
        void RemoveFromWorld();

        bool CreateConversation(ObjectGuid::LowType guidlow, uint32 triggerEntry, Unit* caster, SpellInfo const* info, Position const& pos);
        void Update(uint32 p_time);
        void Remove();
        void SetDuration(int32 newDuration) { _duration = newDuration; }
        int32 GetDuration() const { return _duration; }

        Unit* GetCaster() const { return _caster; }
        void BindToCaster();
        void UnbindFromCaster();
        uint32 GetSpellId() const { return _spellId; }
        ObjectGuid GetCasterGUID() const { return casterGUID; }

        void BuildDynamicValuesUpdate(uint8 updatetype, ByteBuffer* data, Player* target) const override;

        ByteBuffer fieldBuffer;
        UpdateMask updateMask;

    protected:
        Unit* _caster;
        uint32 _spellId;
        uint32 _duration;
        ObjectGuid casterGUID;
};
#endif
