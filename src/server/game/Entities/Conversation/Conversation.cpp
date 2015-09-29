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

#include "Common.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "Conversation.h"
#include "ObjectMgr.h"

Conversation::Conversation() : WorldObject(false), _caster(NULL), _duration(0), _spellId(0)
{
    m_objectType |= TYPEMASK_CONVERSATION;
    m_objectTypeId = TYPEID_CONVERSATION;

    m_updateFlag = UPDATEFLAG_STATIONARY_POSITION;

    m_valuesCount = CONVERSATION_END;
    _dynamicValuesCount = CONVERSATION_DYNAMIC_END;

    _fieldNotifyFlags = UF_FLAG_PUBLIC | UF_FLAG_UNK0X100;
    updateMask.SetCount(_dynamicValuesCount);
}

Conversation::~Conversation()
{
}

void Conversation::AddToWorld()
{
    ///- Register the Conversation for guid lookup and for caster
    if (!IsInWorld())
    {
        sObjectAccessor->AddObject(this);
        WorldObject::AddToWorld();
        BindToCaster();
    }
}

void Conversation::RemoveFromWorld()
{
    ///- Remove the Conversation from the accessor and from all lists of objects in world
    if (IsInWorld())
    {
        // dynobj could get removed in Aura::RemoveAura
        if (!IsInWorld())
            return;

        UnbindFromCaster();
        WorldObject::RemoveFromWorld();
        sObjectAccessor->RemoveObject(this);
    }
}

bool Conversation::CreateConversation(ObjectGuid::LowType guidlow, uint32 triggerEntry, Unit* caster, SpellInfo const* info, Position const& pos)
{
    std::vector<ConversationData> const* conversationData = sObjectMgr->GetConversationData(triggerEntry);
    std::vector<ConversationCreature> const* conversationCreature = sObjectMgr->GetConversationCreature(triggerEntry);

    if(!conversationData || !conversationCreature || conversationData->empty() || conversationCreature->empty())
        return false;

    SetMap(caster->GetMap());
    Relocate(pos);
    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_GENERAL, "Conversation (spell %u) not created. Suggested coordinates isn't valid (X: %f Y: %f)", info->Id, GetPositionX(), GetPositionY());
        return false;
    }

    for (uint16 index = 0; index < _dynamicValuesCount; ++index)
    {
        ByteBuffer buffer;
        if (_fieldNotifyFlags & ConversationDynamicFieldFlags[index])
        {
            updateMask.SetBit(index);

            UpdateMask arrayMask;
            if (index == CONVERSATION_DYNAMIC_FIELD_ACTORS)
            {
                uint32 count = 0;
                arrayMask.SetCount(conversationCreature->size());
                for (std::vector<ConversationCreature>::const_iterator itr = conversationCreature->begin(); itr != conversationCreature->end(); ++itr)
                {
                    if (Creature* creature = caster->FindNearestCreature(itr->creatureId, caster->GetVisibilityRange()))
                    {
                        arrayMask.SetBit(count++);
                        buffer << uint32(PAIR64_LOPART(creature->GetGUID().GetLowPart()));
                        arrayMask.SetBit(count++);
                        buffer << uint32(PAIR64_HIPART(creature->GetGUID().GetLowPart()));
                        arrayMask.SetBit(count++);
                        buffer << uint32(PAIR64_LOPART(creature->GetGUID().GetHighPart()));
                        arrayMask.SetBit(count++);
                        buffer << uint32(PAIR64_HIPART(creature->GetGUID().GetHighPart()));
                    }
                    else
                        return false;
                }
            }
            if (index == CONVERSATION_DYNAMIC_FIELD_LINES)
            {
                uint32 count = 0;
                arrayMask.SetCount(conversationData->size());
                for (std::vector<ConversationData>::const_iterator itr = conversationData->begin(); itr != conversationData->end(); ++itr)
                {
                    arrayMask.SetBit(count++);
                    buffer << uint32(itr->id);
                    arrayMask.SetBit(count++);
                    buffer << uint32(itr->textId);
                    arrayMask.SetBit(count++);
                    buffer << uint32(itr->unk1);
                    arrayMask.SetBit(count++);
                    buffer << uint32(itr->unk2);
                    arrayMask.SetBit(count++);
                    buffer << uint32(itr->flags);
                }
            }

            fieldBuffer << uint8(arrayMask.GetBlockCount());
            arrayMask.AppendToPacket(&fieldBuffer);
            fieldBuffer.append(buffer);
        }
    }

    Object::_Create(ObjectGuid::Create<HighGuid::Conversation>(GetMapId(), 0, guidlow));
    SetPhaseMask(caster->GetPhaseMask(), false);
    SetPhaseId(caster->GetPhases(), false);

    SetEntry(triggerEntry);
    SetObjectScale(1.0f);
    casterGUID = caster->GetGUID();
    SetDuration(30000);

    setActive(true);

    if (!GetMap()->AddToMap(this))
        return false;

    return true;
}

void Conversation::Update(uint32 p_time)
{
    bool expired = false;

    if (GetDuration() > int32(p_time))
        _duration -= p_time;
    else
        expired = true;

    if (expired)
        Remove();
}

void Conversation::Remove()
{
    if (IsInWorld())
    {
        RemoveFromWorld();
        AddObjectToRemoveList();
    }
}

void Conversation::BindToCaster()
{
    ASSERT(!_caster);
    _caster = ObjectAccessor::GetUnit(*this, GetCasterGUID());
    ASSERT(_caster);
    ASSERT(_caster->GetMap() == GetMap());
}

void Conversation::UnbindFromCaster()
{
    ASSERT(_caster);
    _caster = NULL;
}

void Conversation::BuildDynamicValuesUpdate(uint8 updateType, ByteBuffer* data, Player* target) const
{
    UpdateMask updateMaskTemp;
    updateMaskTemp = updateMask;

    *data << uint8(updateMaskTemp.GetBlockCount());
    updateMaskTemp.AppendToPacket(data);
    data->append(fieldBuffer);
}
