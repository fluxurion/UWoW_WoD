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
#include "SharedDefines.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "Object.h"
#include "Creature.h"
#include "Player.h"
#include "Vehicle.h"
#include "ObjectMgr.h"
#include "UpdateData.h"
#include "UpdateMask.h"
#include "Util.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "Log.h"
#include "Transport.h"
#include "TargetedMovementGenerator.h"
#include "WaypointMovementGenerator.h"
#include "VMapFactory.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "SpellAuraEffects.h"
#include "UpdateFieldFlags.h"
#include "TemporarySummon.h"
#include "Totem.h"
#include "OutdoorPvPMgr.h"
#include "DynamicTree.h"
#include "Unit.h"
#include "Group.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "MovementPackets.h"
#include "GameObjectPackets.h"
#include "MiscPackets.h"
#include "Garrison.h"

Object::Object() : m_objectTypeId(TYPEID_OBJECT), m_objectType(TYPEMASK_OBJECT), m_uint32Values(NULL),
    _changedFields(NULL), m_valuesCount(0), _dynamicValuesCount(0), _fieldNotifyFlags(UF_FLAG_DYNAMIC), m_inWorld(false),
    m_objectUpdated(false), _dynamicValues(NULL), _dynamicChangesArrayMask(NULL)
{
}

WorldObject::~WorldObject()
{
    // this may happen because there are many !create/delete
    if (IsWorldObject() && m_currMap)
    {
        if (GetTypeId() == TYPEID_CORPSE)
        {
            sLog->outFatal(LOG_FILTER_GENERAL, "Object::~Object Corpse guid=" UI64FMTD ", type=%d, entry=%u deleted but still in map!!", GetGUID(), ((Corpse*)this)->GetType(), GetEntry());
            ASSERT(false);
        }
        ResetMap();
    }
}

Object::~Object()
{
    if (IsInWorld())
    {
        sLog->outFatal(LOG_FILTER_GENERAL, "Object::~Object - guid=" UI64FMTD ", typeid=%d, entry=%u deleted but still in world!!", GetGUID(), GetTypeId(), GetEntry());
        if (isType(TYPEMASK_ITEM))
            sLog->outFatal(LOG_FILTER_GENERAL, "Item slot %u", ((Item*)this)->GetSlot());
        //ASSERT(false);
        RemoveFromWorld();
    }

    if (m_objectUpdated)
    {
        sLog->outFatal(LOG_FILTER_GENERAL, "Object::~Object - guid=" UI64FMTD ", typeid=%d, entry=%u deleted but still in update list!!", GetGUID(), GetTypeId(), GetEntry());
        //ASSERT(false);
        sObjectAccessor->RemoveUpdateObject(this);
    }

    delete [] m_uint32Values;
    delete [] _changedFields;

    delete[] _dynamicValues;
    _dynamicValues = nullptr;

    delete[] _dynamicChangesArrayMask;
    _dynamicChangesArrayMask = nullptr;
}

void Object::_InitValues()
{
    m_uint32Values = new uint32[m_valuesCount];
    memset(m_uint32Values, 0, m_valuesCount*sizeof(uint32));

    _changedFields = new bool[m_valuesCount];
    memset(_changedFields, 0, m_valuesCount*sizeof(bool));

    _dynamicChangesMask.SetCount(_dynamicValuesCount);
    if (_dynamicValuesCount)
    {
        _dynamicValues = new std::vector<uint32>[_dynamicValuesCount];
        _dynamicChangesArrayMask = new UpdateMask[_dynamicValuesCount];
    }

    m_objectUpdated = false;
}

void Object::_Create(ObjectGuid const& guid)
{
    if (!m_uint32Values) _InitValues();

    SetGuidValue(OBJECT_FIELD_GUID, guid);
    SetUInt16Value(OBJECT_FIELD_TYPE, 0, m_objectType);
    m_PackGUID.Set(guid);
    if (GetVignetteId())
        vignetteGuid = ObjectGuid::Create<HighGuid::Vignette>(guid.GetMapId(), 0, guid.GetCounter());
}

std::string Object::_ConcatFields(uint16 startIndex, uint16 size) const
{
    std::ostringstream ss;
    for (uint16 index = 0; index < size; ++index)
        ss << GetUInt32Value(index + startIndex) << ' ';
    return ss.str();
}

void Object::AddToWorld()
{
    if (m_inWorld)
        return;

    ASSERT(m_uint32Values);

    m_inWorld = true;

    // synchronize values mirror with values array (changes will send in updatecreate opcode any way
    ClearUpdateMask(true);
}

void Object::RemoveFromWorld()
{
    if (!m_inWorld)
        return;

    m_inWorld = false;

    // if we remove from world then sending changes not required
    ClearUpdateMask(true);
}

void Object::BuildCreateUpdateBlockForPlayer(UpdateData* data, Player* target) const
{
    if (!target)
        return;

    uint8  updateType = UPDATETYPE_CREATE_OBJECT;
    uint16 flags      = m_updateFlag;

    uint32 valCount = m_valuesCount;

    /** lower flag1 **/
    if (target == this)                                      // building packet for yourself
        flags |= UPDATEFLAG_SELF;
    else if (GetTypeId() == TYPEID_PLAYER)
        valCount = PLAYER_END_NOT_SELF;

    switch (GetGUID().GetHigh())
    {
        case HighGuid::Player:
        case HighGuid::Pet:
        case HighGuid::Corpse:
        case HighGuid::DynamicObject:
        case HighGuid::AreaTrigger:
        case HighGuid::Conversation:
            updateType = UPDATETYPE_CREATE_OBJECT2;
            break;
        case HighGuid::Creature:
            if (ToUnit()->ToTempSummon() && ToUnit()->ToTempSummon()->GetSummonerGUID().IsPlayer())
                updateType = UPDATETYPE_CREATE_OBJECT2;
            break;
        case HighGuid::GameObject:
            if (ToGameObject()->GetOwnerGUID().IsPlayer())
                updateType = UPDATETYPE_CREATE_OBJECT2;
            break;
    }

    if (!(flags & UPDATEFLAG_LIVING))
        if (WorldObject const* worldObject = dynamic_cast<WorldObject const*>(this))
            if (!worldObject->m_movementInfo.transport.guid.IsEmpty())
                flags |= UPDATEFLAG_GO_TRANSPORT_POSITION;

    if (flags & UPDATEFLAG_STATIONARY_POSITION)
    {
        // UPDATETYPE_CREATE_OBJECT2 for some gameobject types...
        if (isType(TYPEMASK_GAMEOBJECT))
        {
            switch (((GameObject*)this)->GetGoType())
            {
                case GAMEOBJECT_TYPE_TRAP:
                case GAMEOBJECT_TYPE_DUEL_ARBITER:
                case GAMEOBJECT_TYPE_FLAGSTAND:
                case GAMEOBJECT_TYPE_FLAGDROP:
                    updateType = UPDATETYPE_CREATE_OBJECT2;
                    break;
                case GAMEOBJECT_TYPE_TRANSPORT:
                    flags |= UPDATEFLAG_TRANSPORT;
                    break;
                default:
                    break;
            }
        }
    }

    if (Unit const* unit = ToUnit())
    {
        if (unit->getVictim())
            flags |= UPDATEFLAG_HAS_TARGET;
    }

    if (ToWorldObject()->GetAIAnimKitId() || ToWorldObject()->GetMovementAnimKitId() || ToWorldObject()->GetMeleeAnimKitId())
        flags |= UPDATEFLAG_ANIMKITS;

    ByteBuffer buf(500);
    buf << uint8(updateType);
    buf << GetPackGUID();
    buf << uint8(m_objectTypeId);

    _BuildMovementUpdate(&buf, flags);
    _BuildValuesUpdate(updateType, &buf, target);
    BuildDynamicValuesUpdate(updateType, &buf, target);

    data->AddUpdateBlock(buf);
}

void Object::SendUpdateToPlayer(Player* player)
{
    // send create update to player
    UpdateData upd(player->GetMapId());
    WorldPacket packet;

    BuildCreateUpdateBlockForPlayer(&upd, player);
    upd.BuildPacket(&packet);
    player->GetSession()->SendPacket(&packet);
}

void Object::BuildValuesUpdateBlockForPlayer(UpdateData* data, Player* target) const
{
    ByteBuffer buf(500);

    buf << uint8(UPDATETYPE_VALUES);
    buf << GetPackGUID();

    _BuildValuesUpdate(UPDATETYPE_VALUES, &buf, target);
    BuildDynamicValuesUpdate(UPDATETYPE_VALUES, &buf, target);

    data->AddUpdateBlock(buf);
}

void Object::BuildOutOfRangeUpdateBlock(UpdateData* data) const
{
    data->AddOutOfRangeGUID(GetGUID());
}

void Object::DestroyForPlayer(Player* target, bool onDeath /*= false*/) const
{
    ASSERT(target);

    UpdateData updateData(target->GetMapId());
    BuildOutOfRangeUpdateBlock(&updateData);
    WorldPacket packet;
    updateData.BuildPacket(&packet);
    target->SendDirectMessage(&packet);
}

void Object::_BuildMovementUpdate(ByteBuffer* data, uint16 flags) const
{
    bool NoBirthAnim = false;
    bool EnablePortals = false;
    bool PlayHoverAnim = false;
    bool IsSuppressingGreetings = false;
    bool HasMovementUpdate = (flags & UPDATEFLAG_LIVING) != 0;
    bool HasMovementTransport = (flags & UPDATEFLAG_GO_TRANSPORT_POSITION) != 0;
    bool Stationary = (flags & UPDATEFLAG_STATIONARY_POSITION) != 0;
    bool CombatVictim = (flags & UPDATEFLAG_HAS_TARGET) != 0;
    bool ServerTime = (flags & UPDATEFLAG_TRANSPORT) != 0;
    bool VehicleCreate = (flags & UPDATEFLAG_VEHICLE) != 0;
    bool AnimKitCreate = (flags & UPDATEFLAG_ANIMKITS) != 0;
    bool Rotation = (flags & UPDATEFLAG_ROTATION) != 0;
    bool HasAreaTrigger = (flags & UPDATEFLAG_AREA_TRIGGER) != 0;
    bool HasGameObject = (flags & UPDATEFLAG_HAS_WORLDEFFECTID) != 0;;
    bool ThisIsYou = (flags & UPDATEFLAG_SELF) != 0;
    bool ReplaceActive = false;
    bool SceneObjCreate = false;
    bool ScenePendingInstances = false;
    uint32 PauseTimesCount = 0;

    std::vector<uint32> transportFrames;
    if (GameObject const* go = ToGameObject())
    {
        if (go->HasManualAnim())
        {
            GameObjectTemplate const* goInfo = go->GetGOInfo();
            if (goInfo->type == GAMEOBJECT_TYPE_TRANSPORT)
            {
                if (goInfo->transport.Timeto2ndfloor)
                    transportFrames.push_back(goInfo->transport.Timeto2ndfloor);
                if (goInfo->transport.Timeto3rdfloor)
                    transportFrames.push_back(goInfo->transport.Timeto3rdfloor);
                //if (goInfo->transport.nextFrame2)
                //    transportFrames.push_back(goInfo->transport.nextFrame2);
                //if (goInfo->transport.nextFrame3)
                //    transportFrames.push_back(goInfo->transport.nextFrame3);
            }
            PauseTimesCount = transportFrames.size();
        }
    }

    data->WriteBit(NoBirthAnim);
    data->WriteBit(EnablePortals);
    data->WriteBit(PlayHoverAnim);
    data->WriteBit(IsSuppressingGreetings);
    data->WriteBit(HasMovementUpdate);
    data->WriteBit(HasMovementTransport);
    data->WriteBit(Stationary);
    data->WriteBit(CombatVictim);
    data->WriteBit(ServerTime);
    data->WriteBit(VehicleCreate);
    data->WriteBit(AnimKitCreate);
    data->WriteBit(Rotation);
    data->WriteBit(HasAreaTrigger);
    data->WriteBit(HasGameObject);
    data->WriteBit(ThisIsYou);
    data->WriteBit(ReplaceActive);
    data->WriteBit(SceneObjCreate);
    data->WriteBit(ScenePendingInstances);
    *data << uint32(PauseTimesCount);

    if (HasMovementUpdate)
    {
        Unit const* unit = ToUnit();
        uint32 movementFlags = unit->m_movementInfo.GetMovementFlags();
        uint16 movementFlagsExtra = unit->m_movementInfo.GetExtraMovementFlags();
        if (GetTypeId() == TYPEID_UNIT)
            movementFlags &= MOVEMENTFLAG_MASK_CREATURE_ALLOWED;
        // these break update packet
        {
            if (GetTypeId() == TYPEID_UNIT)
                movementFlags &= MOVEMENTFLAG_MASK_CREATURE_ALLOWED;
            else
            {
                if (movementFlags & (MOVEMENTFLAG_FLYING | MOVEMENTFLAG_CAN_FLY))
                    movementFlags &= ~(MOVEMENTFLAG_FALLING | MOVEMENTFLAG_FALLING_FAR | MOVEMENTFLAG_FALLING_SLOW);
                if ((movementFlagsExtra & MOVEMENTFLAG2_INTERPOLATED_TURNING) == 0)
                    movementFlags &= ~MOVEMENTFLAG_FALLING;
            }
        }

        bool HasFallDirection = unit->HasUnitMovementFlag(MOVEMENTFLAG_FALLING);
        bool HasFall = HasFallDirection || movementFlagsExtra & MOVEMENTFLAG2_INTERPOLATED_TURNING;
        bool HasSpline = unit->IsSplineEnabled();

        *data << GetPackGUID();                                         // MoverGUID

        *data << uint32(unit->m_movementInfo.time);                     // MoveIndex
        *data << float(unit->GetPositionX());
        *data << float(unit->GetPositionY());
        *data << float(unit->GetPositionZ());
        *data << float(G3D::fuzzyEq(unit->GetOrientation(), 0.0f) ? 0.0f : Position::NormalizeOrientation(unit->GetOrientation()));

                                                                        // Pitch
        if ((movementFlags & (MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_FLYING)) ||
            (movementFlagsExtra & MOVEMENTFLAG2_ALWAYS_ALLOW_PITCHING))
            *data << float(Position::NormalizePitch(unit->m_movementInfo.pitch));
        else
            *data << float(0.0f);

        if (movementFlags & MOVEMENTFLAG_SPLINE_ELEVATION)              // StepUpStartElevation
            *data << float(unit->m_movementInfo.splineElevation);
        else
            *data << float(0.0f);

        uint32 removeMovementForcesCount = 0;
        *data << uint32(removeMovementForcesCount);                     // Count of RemoveForcesIDs
        *data << uint32(0);                                             // Unknown

        //for (uint32 i = 0; i < removeMovementForcesCount; ++i)
        //    *data << ObjectGuid(RemoveForcesIDs);

        data->WriteBits(movementFlags, 30);
        data->WriteBits(movementFlagsExtra, 15);
        data->WriteBit(!unit->m_movementInfo.transport.guid.IsEmpty()); // HasTransport
        data->WriteBit(HasFall);                                        // HasFall
        data->WriteBit(HasSpline);                                      // HasSpline - marks that the unit uses spline movement
        data->WriteBit(0);                                              // HeightChangeFailed
        data->WriteBit(0);                                              // RemoteTimeValid

        if (!unit->m_movementInfo.transport.guid.IsEmpty())
        {
            *data << unit->m_movementInfo.transport.guid;                     // Transport Guid
            *data << float(unit->GetTransOffsetX());
            *data << float(unit->GetTransOffsetY());
            *data << float(unit->GetTransOffsetZ());
            *data << float(unit->GetTransOffsetO());
            *data << int8(unit->m_movementInfo.transport.seat);               // VehicleSeatIndex
            *data << uint32(unit->m_movementInfo.transport.time);             // MoveTime

            data->WriteBit(unit->m_movementInfo.transport.prevTime != 0);
            data->WriteBit(unit->m_movementInfo.transport.vehicleId != 0);

            if (unit->m_movementInfo.transport.prevTime)
                *data << uint32(unit->m_movementInfo.transport.prevTime);     // PrevMoveTime

            if (unit->m_movementInfo.transport.vehicleId)
                *data << uint32(unit->m_movementInfo.transport.vehicleId);    // VehicleRecID
        }

        if (HasFall)
        {
            *data << uint32(unit->m_movementInfo.fallTime);                   // Time
            *data << float(unit->m_movementInfo.jump.zspeed);                 // JumpVelocity

            if (data->WriteBit(HasFallDirection))
            {
                *data << float(unit->m_movementInfo.jump.sinAngle);           // Direction
                *data << float(unit->m_movementInfo.jump.cosAngle);
                *data << float(unit->m_movementInfo.jump.xyspeed);            // Speed
            }
        }

        *data << float(unit->GetSpeed(MOVE_WALK));
        *data << float(unit->GetSpeed(MOVE_RUN));
        *data << float(unit->GetSpeed(MOVE_RUN_BACK));
        *data << float(unit->GetSpeed(MOVE_SWIM));
        *data << float(unit->GetSpeed(MOVE_SWIM_BACK));
        *data << float(unit->GetSpeed(MOVE_FLIGHT));
        *data << float(unit->GetSpeed(MOVE_FLIGHT_BACK));
        *data << float(unit->GetSpeed(MOVE_TURN_RATE));
        *data << float(unit->GetSpeed(MOVE_PITCH_RATE));

        uint32 MovementForceCount = 0;
        *data << uint32(MovementForceCount);

        //for (uint32 i = 0; i < MovementForceCount; ++i)
        //{
        //    *data << ObjectGuid(ID);
        //    *data << Vector3(Direction);
        //    *data << int32(TransportID);
        //    *data << float(Magnitude);
        //    *data << uint8(Type);
        //}

        // HasMovementSpline - marks that spline data is present in packet

        if (data->WriteBit(HasSpline))
            WorldPackets::Movement::CommonMovement::WriteCreateObjectSplineDataBlock(*unit->movespline, *data);
    }

    if (HasMovementTransport)
    {
        WorldObject const* self = static_cast<WorldObject const*>(this);
        *data << self->m_movementInfo.transport.guid;                   // Transport Guid
        *data << float(self->GetTransOffsetX());
        *data << float(self->GetTransOffsetY());
        *data << float(self->GetTransOffsetZ());
        *data << float(self->GetTransOffsetO());
        *data << int8(self->m_movementInfo.transport.seat);             // VehicleSeatIndex
        *data << uint32(self->m_movementInfo.transport.time);           // MoveTime

        data->WriteBit(self->m_movementInfo.transport.prevTime != 0);
        data->WriteBit(self->m_movementInfo.transport.vehicleId != 0);

        if (self->m_movementInfo.transport.prevTime)
            *data << uint32(self->m_movementInfo.transport.prevTime);   // PrevMoveTime

        if (self->m_movementInfo.transport.vehicleId)
            *data << uint32(self->m_movementInfo.transport.vehicleId);  // VehicleRecID
    }

    if (Stationary)
    {
        WorldObject const* self = static_cast<WorldObject const*>(this);

        *data << float(self->GetPositionX());
        *data << float(self->GetPositionY());
        if (Unit const* unit = ToUnit())
            *data << float(unit->GetPositionZMinusOffset());
        else
            *data << float(self->GetPositionZ());
        *data << float(Position::NormalizeOrientation(self->GetOrientation()));
    }

    if (CombatVictim)
        *data << ToUnit()->getVictim()->GetGUID();                      // CombatVictim

    if (ServerTime)
    {
        //GameObject const* go = ToGameObject();
        ///** @TODO Use IsTransport() to also handle type 11 (TRANSPORT)
        //    Currently grid objects are not updated if there are no nearby players,
        //    this causes clients to receive different PathProgress
        //    resulting in players seeing the object in a different position
        //*/
        //if (go && go->ToTransport())                                    // ServerTime
        //    *data << uint32(go->GetGOValue()->Transport.PathProgress);
        //else
            *data << uint32(getMSTime());
    }

    if (VehicleCreate)
    {
        Unit const* unit = ToUnit();
        *data << uint32(unit->GetVehicleKit()->GetVehicleInfo()->ID); // RecID
        *data << float(Position::NormalizeOrientation(unit->GetOrientation())); // InitialRawFacing
    }

    if (AnimKitCreate)
    {
        WorldObject const* obj = ToWorldObject();
        *data << uint16(obj->GetAIAnimKitId());                        // AiID
        *data << uint16(obj->GetMovementAnimKitId());                  // MovementID
        *data << uint16(obj->GetMeleeAnimKitId());                     // MeleeID
    }

    if (Rotation)
        *data << uint64(ToGameObject()->GetRotation());                 // Rotation

    if (HasAreaTrigger)
    {
        AreaTrigger const* t = ToAreaTrigger();
        ASSERT(t);

        if(t->GetAreaTriggerInfo().ElapsedTime)
            *data << uint32(t->GetAreaTriggerInfo().ElapsedTime);       // Elapsed Time Ms
        else
            *data << uint32(1);                                         // Elapsed Time Ms

        *data << t->GetAreaTriggerInfo().RollPitchYaw1X << t->GetAreaTriggerInfo().RollPitchYaw1Y << t->GetAreaTriggerInfo().RollPitchYaw1Z;             //RollPitchYaw1

        data->WriteBit(t->GetAreaTriggerInfo().HasAbsoluteOrientation); // HasAbsoluteOrientation
        data->WriteBit(t->GetAreaTriggerInfo().HasDynamicShape);        // HasDynamicShape
        data->WriteBit(0/*t->GetAreaTriggerInfo().HasAttached*/);            // HasAttached is have transport, not complete
        data->WriteBit(t->GetAreaTriggerInfo().HasFaceMovementDir);     // HasFaceMovementDir
        data->WriteBit(t->GetAreaTriggerInfo().HasFollowsTerrain);      // HasFollowsTerrain
        data->WriteBit(t->HasTargetRollPitchYaw());                     // HasTargetRollPitchYaw
        data->WriteBit(t->GetAreaTriggerInfo().ScaleCurveID);           // HasScaleCurveID
        data->WriteBit(t->GetAreaTriggerInfo().MorphCurveID);           // HasMorphCurveID
        data->WriteBit(t->GetAreaTriggerInfo().FacingCurveID);          // HasFacingCurveID
        data->WriteBit(t->GetAreaTriggerInfo().MoveCurveID);            // hasMoveCurveID
        data->WriteBit(t->GetVisualScale());                            // HasAreaTriggerSphere
        data->WriteBit(t->GetAreaTriggerInfo().hasAreaTriggerBox);      // HasAreaTriggerBox
        data->WriteBit(t->isPolygon());                                 // areaTriggerPolygon
        data->WriteBit(t->GetAreaTriggerCylinder());                    // areaTriggerCylinder
        data->WriteBit(t->isMoving());                                  // areaTriggerSpline

        if (t->HasTargetRollPitchYaw())
            *data << t->GetAreaTriggerInfo().TargetRollPitchYawX << t->GetAreaTriggerInfo().TargetRollPitchYawY << t->GetAreaTriggerInfo().TargetRollPitchYawZ;             //RollPitchYaw1

        if (t->GetAreaTriggerInfo().ScaleCurveID)
            *data << uint32(t->GetAreaTriggerInfo().ScaleCurveID);

        if (t->GetAreaTriggerInfo().MorphCurveID)
            *data << uint32(t->GetAreaTriggerInfo().MorphCurveID);

        if (t->GetAreaTriggerInfo().FacingCurveID)
            *data << uint32(t->GetAreaTriggerInfo().FacingCurveID);

        if (t->GetAreaTriggerInfo().MoveCurveID)
            *data << uint32(t->GetAreaTriggerInfo().MoveCurveID);

        if (t->GetVisualScale())                                        // areaTriggerSphere
        {
            *data << t->GetVisualScale(true);                           // Radius
            *data << t->GetVisualScale();                               // RadiusTarget
        }

        if (t->GetAreaTriggerInfo().hasAreaTriggerBox)
        {
            *data << t->GetAreaTriggerInfo().Radius;                    // Radius
            *data << t->GetAreaTriggerInfo().Height;                    // Height
            *data << t->GetAreaTriggerInfo().Float4;                    // Float4
            *data << t->GetAreaTriggerInfo().RadiusTarget;              // RadiusTarget
            *data << t->GetAreaTriggerInfo().HeightTarget;              // HeightTarget
            *data << t->GetAreaTriggerInfo().Float5;                    // Float5
        }

        if (t->isPolygon())
        {
            *data << uint32(t->GetAreaTriggerInfo().verticesPoints.size()); // VerticesCount
            *data << uint32(t->GetAreaTriggerInfo().verticesTargetPoints.size()); // VerticesTargetCount

            *data << t->GetAreaTriggerInfo().Height;                    // Height
            *data << t->GetAreaTriggerInfo().HeightTarget;              // HeightTarget

            for (size_t i = 0; i < t->GetAreaTriggerInfo().verticesPoints.size(); ++i) // Vertices
            {
                *data << t->GetAreaTriggerInfo().verticesPoints[i].x;    // X
                *data << t->GetAreaTriggerInfo().verticesPoints[i].y;    // Y
            }

            for (uint16 i = 0; i < t->GetAreaTriggerInfo().verticesTargetPoints.size(); ++i) // VerticesTarget
            {
                *data << t->GetAreaTriggerInfo().verticesTargetPoints[i].x; // X
                *data << t->GetAreaTriggerInfo().verticesTargetPoints[i].y; // Y
            }
        }

        if (t->GetAreaTriggerCylinder())                                // areaTriggerCylinder
        {
            *data << t->GetAreaTriggerInfo().Radius;                    // Radius
            *data << t->GetAreaTriggerInfo().RadiusTarget;              // RadiusTarget
            *data << t->GetAreaTriggerInfo().Height;                    // Height
            *data << t->GetAreaTriggerInfo().HeightTarget;              // HeightTarget
            *data << t->GetAreaTriggerInfo().Float4;                    // Float4
            *data << t->GetAreaTriggerInfo().Float5;                    // Float5
        }

        if (t->isMoving())                                              // areaTriggerSpline
            t->PutObjectUpdateMovement(data);                           // Points
    }

    if (HasGameObject)
    {
        if(GameObject const* go = ToGameObject())
            *data << uint32(go->GetGOInfo()->visualData[0].WorldEffectID);
        else
            *data << uint32(0);

        data->WriteBit(false); // bit8
        //if (bit8)
            //*data << uint32(Int1);
    }

    //if (SceneObjCreate)
    //{
    //    packet.ResetBitReader();

    //    var CliSceneLocalScriptData = packet.ReadBit("CliSceneLocalScriptData", index);
    //    var PetBattleFullUpdate = packet.ReadBit("PetBattleFullUpdate", index);

    //    if (CliSceneLocalScriptData)
    //    {
    //        packet.ResetBitReader();
    //        var DataLength = packet.ReadBits(7);
    //        packet.ReadWoWString("Data", DataLength, index);
    //    }

    //    if (PetBattleFullUpdate)
    //    {
    //        for (var i = 0; i < 2; ++i)
    //        {
    //            packet.ReadPackedGuid128("CharacterID", index, i);

    //            packet.ReadInt32("TrapAbilityID", index, i);
    //            packet.ReadInt32("TrapStatus", index, i);

    //            packet.ReadInt16("RoundTimeSecs", index, i);

    //            packet.ReadByte("FrontPet", index, i);
    //            packet.ReadByte("InputFlags", index, i);

    //            packet.ResetBitReader();

    //            var PetBattlePetUpdateCount = packet.ReadBits("PetBattlePetUpdateCount", 2, index, i);

    //            for (var j = 0; j < PetBattlePetUpdateCount; ++j)
    //            {
    //                packet.ReadPackedGuid128("BattlePetGUID", index, i, j);

    //                packet.ReadInt32("SpeciesID", index, i, j);
    //                packet.ReadInt32("DisplayID", index, i, j);
    //                packet.ReadInt32("CollarID", index, i, j);

    //                packet.ReadInt16("Level", index, i, j);
    //                packet.ReadInt16("Xp", index, i, j);


    //                packet.ReadInt32("CurHealth", index, i, j);
    //                packet.ReadInt32("MaxHealth", index, i, j);
    //                packet.ReadInt32("Power", index, i, j);
    //                packet.ReadInt32("Speed", index, i, j);
    //                packet.ReadInt32("NpcTeamMemberID", index, i, j);

    //                packet.ReadInt16("BreedQuality", index, i, j);
    //                packet.ReadInt16("StatusFlags", index, i, j);

    //                packet.ReadByte("Slot", index, i, j);

    //                var PetBattleActiveAbility = packet.ReadInt32("PetBattleActiveAbility", index, i, j);
    //                var PetBattleActiveAura = packet.ReadInt32("PetBattleActiveAura", index, i, j);
    //                var PetBattleActiveState = packet.ReadInt32("PetBattleActiveState", index, i, j);

    //                for (var k = 0; k < PetBattleActiveAbility; ++k)
    //                {
    //                    packet.ReadInt32("AbilityID", index, i, j, k);
    //                    packet.ReadInt16("CooldownRemaining", index, i, j, k);
    //                    packet.ReadInt16("LockdownRemaining", index, i, j, k);
    //                    packet.ReadByte("AbilityIndex", index, i, j, k);
    //                    packet.ReadByte("Pboid", index, i, j, k);
    //                }

    //                for (var k = 0; k < PetBattleActiveAura; ++k)
    //                {
    //                    packet.ReadInt32("AbilityID", index, i, j, k);
    //                    packet.ReadInt32("InstanceID", index, i, j, k);
    //                    packet.ReadInt32("RoundsRemaining", index, i, j, k);
    //                    packet.ReadInt32("CurrentRound", index, i, j, k);
    //                    packet.ReadByte("CasterPBOID", index, i, j, k);
    //                }

    //                for (var k = 0; k < PetBattleActiveState; ++k)
    //                {
    //                    packet.ReadInt32("StateID", index, i, j, k);
    //                    packet.ReadInt32("StateValue", index, i, j, k);
    //                }

    //                packet.ResetBitReader();
    //                var bits57 = packet.ReadBits(7);
    //                packet.ReadWoWString("CustomName", bits57, index, i, j);
    //            }
    //        }

    //        for (var i = 0; i < 3; ++i)
    //        {
    //            var PetBattleActiveAura = packet.ReadInt32("PetBattleActiveAura", index, i);
    //            var PetBattleActiveState = packet.ReadInt32("PetBattleActiveState", index, i);

    //            for (var j = 0; j < PetBattleActiveAura; ++j)
    //            {
    //                packet.ReadInt32("AbilityID", index, i, j);
    //                packet.ReadInt32("InstanceID", index, i, j);
    //                packet.ReadInt32("RoundsRemaining", index, i, j);
    //                packet.ReadInt32("CurrentRound", index, i, j);
    //                packet.ReadByte("CasterPBOID", index, i, j);
    //            }

    //            for (var j = 0; j < PetBattleActiveState; ++j)
    //            {
    //                packet.ReadInt32("StateID", index, i, j);
    //                packet.ReadInt32("StateValue", index, i, j);
    //            }
    //        }

    //        packet.ReadInt16("WaitingForFrontPetsMaxSecs", index);
    //        packet.ReadInt16("PvpMaxRoundTime", index);

    //        packet.ReadInt32("CurRound", index);
    //        packet.ReadInt32("NpcCreatureID", index);
    //        packet.ReadInt32("NpcDisplayID", index);

    //        packet.ReadByte("CurPetBattleState");
    //        packet.ReadByte("ForfeitPenalty");

    //        packet.ReadPackedGuid128("InitialWildPetGUID");

    //        packet.ReadBit("IsPVP");
    //        packet.ReadBit("CanAwardXP");
    //    }
    //}

    //if (ScenePendingInstances)
    //{
    //    var SceneInstanceIDs = packet.ReadInt32("SceneInstanceIDsCount");

    //    for (var i = 0; i < SceneInstanceIDs; ++i)
    //        packet.ReadInt32("SceneInstanceIDs", index, i);
    //}

    for (size_t i = 0; i < transportFrames.size(); ++i)
        *data << uint32(transportFrames[i]);

    data->FlushBits();

    /* =========================
            P A N D A R I A
    ========================= */
/*    data->WriteBit(0);                              // byte2AC
    data->WriteBit(flags & UPDATEFLAG_AREA_TRIGGER);// byte29C
    data->WriteBit(0);                              // byte1
    data->WriteBit(flags & UPDATEFLAG_TRANSPORT);
    data->WriteBit(flags & UPDATEFLAG_HAS_WORLDEFFECTID); // WorldEffectID
    data->WriteBit(flags & UPDATEFLAG_SELF);
    data->WriteBit(0);                              // byte0
    data->WriteBit(flags & UPDATEFLAG_HAS_TARGET);

    std::vector<uint32> transportFrames;
    if (GameObject const* go = ToGameObject())
    {
        if (go->HasManualAnim())
        {
            GameObjectTemplate const* goInfo = go->GetGOInfo();
            if (goInfo->type == GAMEOBJECT_TYPE_TRANSPORT)
            {
                if (goInfo->transport.Timeto2ndfloor)
                    transportFrames.push_back(goInfo->transport.Timeto2ndfloor);
                if (goInfo->transport.Timeto3rdfloor)
                    transportFrames.push_back(goInfo->transport.Timeto3rdfloor);
                //if (goInfo->transport.Timeto4thfloor)
                //    transportFrames.push_back(goInfo->transport.Timeto4thfloor);
                //if (goInfo->transport.Timeto5thfloor)
                //    transportFrames.push_back(goInfo->transport.Timeto5thfloor);
            }
        }
    }
    data->WriteBits(transportFrames.size(), 22);   // transport animation frames                   

    data->WriteBit(0);                              // byte414
    data->WriteBit(0);                              // byte3
    data->WriteBit(0);                              // byte428
    data->WriteBit(0);                              // byte32A
    data->WriteBit(flags & UPDATEFLAG_STATIONARY_POSITION);
    data->WriteBit(0);                              // byte2
    data->WriteBit(flags & UPDATEFLAG_LIVING);
    data->WriteBit(flags & UPDATEFLAG_ANIMKITS);
    data->WriteBit(flags & UPDATEFLAG_VEHICLE);
    data->WriteBit(flags & UPDATEFLAG_GO_TRANSPORT_POSITION);
    data->WriteBit(0);                              // is scene object
    data->WriteBit(flags & UPDATEFLAG_ROTATION);    // has gameobject rotation

    if (flags & UPDATEFLAG_AREA_TRIGGER)
    {
        AreaTrigger const* t = ToAreaTrigger();
        ASSERT(t);

        data->WriteBit(t->isPolygon());  // areaTriggerPolygon
        if (t->isPolygon())
        {
            uint32 size = t->GetAreaTriggerInfo().polygonPoints.size();
            data->WriteBits(size, 21); // VerticesCount dword25C
            data->WriteBits(t->GetAreaTriggerInfo().polygon > 1 ? size : 0, 21); // VerticesTargetCount dword26C
        }
        data->WriteBit(t->GetAreaTriggerInfo().HasAbsoluteOrientation);   // HasAbsoluteOrientation?
        data->WriteBit(t->GetAreaTriggerInfo().HasFollowsTerrain);        // HasFollowsTerrain?
        data->WriteBit(t->GetVisualScale());                              // areaTriggerSphere
        data->WriteBit(t->isMoving());                                    // areaTriggerSpline
        data->WriteBit(t->GetAreaTriggerInfo().HasFaceMovementDir);       // HasFaceMovementDir?
        data->WriteBit(t->GetAreaTriggerInfo().HasAttached);              // HasAttached?
        data->WriteBit(t->GetAreaTriggerInfo().ScaleCurveID);             // hasScaleCurveID?
        data->WriteBit(t->GetAreaTriggerInfo().MorphCurveID);             // hasMorphCurveID?
        if (t->isMoving())
            data->WriteBits(t->GetObjectMovementParts(), 20);             // splinePointsCount
        data->WriteBit(t->GetAreaTriggerInfo().FacingCurveID);            // hasFacingCurveID?
        data->WriteBit(t->GetAreaTriggerInfo().HasDynamicShape);          // HasDynamicShape?
        data->WriteBit(t->GetAreaTriggerInfo().MoveCurveID);              // hasMoveCurveID
        data->WriteBit(t->GetAreaTriggerCylinder());                      // areaTriggerCylinder
    }

    if (flags & UPDATEFLAG_LIVING)
    {
        Unit const* self = ToUnit();
        ObjectGuid guid = GetGUID();
        uint32 movementFlags = self->m_movementInfo.GetMovementFlags();
        uint16 movementFlagsExtra = self->m_movementInfo.GetExtraMovementFlags();
        // these break update packet
        {
            if (GetTypeId() == TYPEID_UNIT)
                movementFlags &= MOVEMENTFLAG_MASK_CREATURE_ALLOWED;
            else
            {
                if (movementFlags & (MOVEMENTFLAG_FLYING | MOVEMENTFLAG_CAN_FLY))
                    movementFlags &= ~(MOVEMENTFLAG_FALLING | MOVEMENTFLAG_FALLING_FAR | MOVEMENTFLAG_FALLING_SLOW);
                if ((movementFlagsExtra & MOVEMENTFLAG2_INTERPOLATED_TURNING) == 0)
                    movementFlags &= ~MOVEMENTFLAG_FALLING;
            }
        }

        ObjectGuid transGuid = self->m_movementInfo.transport.guid;
        //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "BuildMovement GUID %u, Entry %u, t_guid %u, Flags %i, FlagsExtra %i, flags %i, xyz (%f %f %f) Zofset %f, T(%f %f %f)", 
        //GetGUID(), GetEntry(), self->m_movementInfo.t_guid, movementFlags, movementFlagsExtra, flags, self->GetPositionX(), self->GetPositionY(), self->GetPositionZ(), self->GetPositionZMinusOffset(), self->GetTransOffsetX(), self->GetTransOffsetY(), self->GetTransOffsetZ());

        //data->WriteGuidMask<4, 1>(guid);
        data->WriteBits(0, 19);                     // dword160
        //data->WriteGuidMask<5>(guid);
        data->WriteBit(G3D::fuzzyEq(self->GetOrientation(), 0.0f));         // has orientation
        //data->WriteGuidMask<7>(guid);
        data->WriteBits(0, 22);                     // dword90
        data->WriteBit(self->IsSplineEnabled());    // has spline data
        data->WriteBit(!((movementFlags & (MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_FLYING)) ||
            (movementFlagsExtra & MOVEMENTFLAG2_ALWAYS_ALLOW_PITCHING)));   // has pitch

        if (self->IsSplineEnabled())
            Movement::PacketBuilder::WriteCreateBits(*self->movespline, *data);

        data->WriteBit(1);                          // !dwordA0
        //data->WriteGuidMask<3>(guid);
        data->WriteBit(0);                          // byteA4
        data->WriteBit(!movementFlags);
        if (movementFlags)
            data->WriteBits(movementFlags, 30);
        data->WriteBit(0);                          // byte8C
        data->WriteBit(0);                          // byte8D
        //data->WriteGuidMask<2>(guid);
        data->WriteBit(0);                          // !hasTimeStamp
        //data->WriteGuidMask<0>(guid);
        //data->WriteBit(transGuid);                  // has transport data

        if (transGuid)
        {
            //data->WriteGuidMask<4, 7, 3, 1, 6>(transGuid);
            data->WriteBit(self->m_movementInfo.transport.prevTime);   // Has transport time 2
            //data->WriteGuidMask<2, 0, 5>(transGuid);
            data->WriteBit(self->m_movementInfo.transport.vehicleId);   // Has transport time 3
        }

        //data->WriteGuidMask<6>(guid);
        data->WriteBit(movementFlagsExtra & MOVEMENTFLAG2_INTERPOLATED_TURNING); // has fall data
        if (movementFlagsExtra & MOVEMENTFLAG2_INTERPOLATED_TURNING)
            data->WriteBit(movementFlags & MOVEMENTFLAG_FALLING);           // has fall direction
        data->WriteBit(!movementFlagsExtra);
        data->WriteBit(!(movementFlags & MOVEMENTFLAG_SPLINE_ELEVATION));   // has spline elevation
        if (movementFlagsExtra)
            data->WriteBits(movementFlagsExtra, 13);
    }

    if (flags & UPDATEFLAG_GO_TRANSPORT_POSITION)
    {
        WorldObject const* self = static_cast<WorldObject const*>(this);
        ObjectGuid transGuid = self->m_movementInfo.transport.guid;

        //data->WriteGuidMask<0, 7>(transGuid);
        data->WriteBit(self->m_movementInfo.transport.vehicleId);   // has go transport time 3
        //data->WriteGuidMask<1>(transGuid);
        data->WriteBit(self->m_movementInfo.transport.prevTime);   // has go transport time 2
        //data->WriteGuidMask<6, 5, 4, 3, 2>(transGuid);
    }

    if (flags & UPDATEFLAG_HAS_TARGET)
    {
        ObjectGuid victimGuid = ToUnit()->getVictim()->GetGUID();
        //data->WriteGuidMask<5, 4, 6, 0, 1, 7, 2, 3>(victimGuid);
    }

    if (flags & UPDATEFLAG_ANIMKITS)
    {
        data->WriteBit(1);                              // Missing AnimKit3
        data->WriteBit(1);                              // Missing AnimKit1
        data->WriteBit(1);                              // Missing AnimKit2
    }

    data->FlushBits();

    for (uint32 i = 0; i < transportFrames.size(); ++i)
        *data << uint32(transportFrames[i]);

    if (flags & UPDATEFLAG_AREA_TRIGGER)
    {
        AreaTrigger const* t = ToAreaTrigger();
        ASSERT(t);

        if (t->GetAreaTriggerCylinder())                // areaTriggerCylinder
        {
            *data << t->GetAreaTriggerInfo().Height; // Height (float250)
            *data << t->GetAreaTriggerInfo().Float4; // Float4 (float254)
            *data << t->GetAreaTriggerInfo().Float5; // Float5 (float248)
            *data << t->GetAreaTriggerInfo().Radius; // Radius (float24C)
            *data << t->GetAreaTriggerInfo().RadiusTarget; // RadiusTarget (float240)
            *data << t->GetAreaTriggerInfo().HeightTarget; // HeightTarget (float244)
        }

        if (t->isPolygon())
        {
            *data << t->GetAreaTriggerInfo().HeightTarget; // HeightTarget (float280)

            for (uint16 i = 0; i < t->GetAreaTriggerInfo().polygonPoints.size(); ++i)
            {
                *data << t->GetAreaTriggerInfo().polygonPoints[i].y; // Y
                *data << t->GetAreaTriggerInfo().polygonPoints[i].x; // X
            }

            *data << t->GetAreaTriggerInfo().Height; // Height (float27C)

            if(t->GetAreaTriggerInfo().polygon > 1)
            {
                for (uint16 i = 0; i < t->GetAreaTriggerInfo().polygonPoints.size(); ++i)
                {
                    *data << t->GetAreaTriggerInfo().polygonPoints[i].x; // X
                    *data << t->GetAreaTriggerInfo().polygonPoints[i].y; // Y
                }
            }
        }

        //if (t->GetAreaTriggerInfo().MoveCurveID)
        //    *data << uint32(t->GetAreaTriggerInfo().MoveCurveID);

        if (t->GetAreaTriggerInfo().MorphCurveID)
            *data << uint32(t->GetAreaTriggerInfo().MorphCurveID);

        //if (t->GetVisualScale())                // areaTriggerSphere
        //{
        //    *data << t->GetVisualScale(true);   // Radius
        //    *data << t->GetVisualScale();       // RadiusTarget
        //}

        //if (t->isMoving())                      // areaTriggerSpline
        //    t->PutObjectUpdateMovement(data);   // Points

        //if(t->GetAreaTriggerInfo().ElapsedTime)
        //    *data << uint32(t->GetAreaTriggerInfo().ElapsedTime);                     // Elapsed Time Ms
        //else
        //    *data << uint32(1);                     // Elapsed Time Ms

        if (t->GetAreaTriggerInfo().FacingCurveID)
            *data << uint32(t->GetAreaTriggerInfo().FacingCurveID);

        if (t->GetAreaTriggerInfo().ScaleCurveID)
            *data << uint32(t->GetAreaTriggerInfo().ScaleCurveID);
    }

    if (flags & UPDATEFLAG_LIVING)
    {
        Unit const* self = ToUnit();
        ObjectGuid guid = GetGUID();
        uint32 movementFlags = self->m_movementInfo.GetMovementFlags();
        uint16 movementFlagsExtra = self->m_movementInfo.GetExtraMovementFlags();
        if (GetTypeId() == TYPEID_UNIT)
            movementFlags &= MOVEMENTFLAG_MASK_CREATURE_ALLOWED;
        ObjectGuid transGuid = self->m_movementInfo.transport.guid;

        *data << float(self->GetPositionY());
        if (self->IsSplineEnabled())
            Movement::PacketBuilder::WriteCreateData(*self->movespline, *data);
        *data << self->GetSpeed(MOVE_FLIGHT);
        *data << self->GetSpeed(MOVE_RUN);
        //data->WriteGuidBytes<4>(guid);
        *data << self->GetSpeed(MOVE_WALK);

        if (movementFlagsExtra & MOVEMENTFLAG2_INTERPOLATED_TURNING)
        {
            if (movementFlags & MOVEMENTFLAG_FALLING)
            {
                *data << float(self->m_movementInfo.jump.xyspeed);
                *data << float(self->m_movementInfo.jump.sinAngle);
                *data << float(self->m_movementInfo.jump.cosAngle);
            }

            *data << uint32(self->m_movementInfo.fallTime);
            *data << float(self->m_movementInfo.jump.zspeed);
        }

        if (transGuid)
        {
            //data->WriteGuidBytes<5>(transGuid);
            *data << int8(self->GetTransSeat());
            //data->WriteGuidBytes<2>(transGuid);
            *data << float(Position::NormalizeOrientation(self->GetTransOffsetO()));
            //data->WriteGuidBytes<4, 7>(transGuid);
            if (uint32 time2 = self->m_movementInfo.transport.prevTime)
                *data << uint32(time2);
            *data << uint32(self->GetTransTime());
            *data << float(self->GetTransOffsetY());
            //data->WriteGuidBytes<3, 6>(transGuid);
            *data << float(self->GetTransOffsetX());
            //data->WriteGuidBytes<0>(transGuid);
            if (uint32 time3 = self->m_movementInfo.transport.vehicleId)
                *data << uint32(time3);
            //data->WriteGuidBytes<1>(transGuid);
            *data << float(self->GetTransOffsetZ());
        }

        //data->WriteGuidBytes<5>(guid);
        *data << uint32(getMSTime());
        //data->WriteGuidBytes<1>(guid);
        *data << self->GetSpeed(MOVE_SWIM_BACK);
        *data << self->GetSpeed(MOVE_FLIGHT_BACK);
        //data->WriteGuidBytes<6>(guid);
        *data << self->GetSpeed(MOVE_TURN_RATE);
        *data << float(self->GetPositionX());
        if (!G3D::fuzzyEq(self->GetOrientation(), 0.0f))
            *data << float(Position::NormalizeOrientation(self->GetOrientation()));
        *data << self->GetSpeed(MOVE_PITCH_RATE);
        *data << self->GetSpeed(MOVE_SWIM);
        if ((movementFlags & (MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_FLYING)) ||
            (movementFlagsExtra & MOVEMENTFLAG2_ALWAYS_ALLOW_PITCHING))
            *data << float(Position::NormalizePitch(self->m_movementInfo.pitch));
        //data->WriteGuidBytes<3>(guid);
        if (movementFlags & MOVEMENTFLAG_SPLINE_ELEVATION)
            *data << float(self->m_movementInfo.splineElevation);
        *data << self->GetSpeed(MOVE_RUN_BACK);
        //data->WriteGuidBytes<7, 2>(guid);
        *data << float(self->GetPositionZMinusOffset());
        //data->WriteGuidBytes<0>(guid);
    }

    if (flags & UPDATEFLAG_STATIONARY_POSITION)
    {
        WorldObject const* self = static_cast<WorldObject const*>(this);

        *data << float(self->GetPositionX());
        if (Unit const* unit = ToUnit())
            *data << float(unit->GetPositionZMinusOffset());
        else
            *data << float(self->GetPositionZ());
        *data << float(self->GetPositionY());
        *data << float(Position::NormalizeOrientation(self->GetOrientation()));
    }

    if (flags & UPDATEFLAG_GO_TRANSPORT_POSITION)
    {
        WorldObject const* self = static_cast<WorldObject const*>(this);
        ObjectGuid transGuid = self->m_movementInfo.transport.guid;

        if (uint32 time2 = self->m_movementInfo.transport.prevTime)
            *data << uint32(time2);
        //data->WriteGuidBytes<4, 2, 7, 3>(transGuid);
        *data << uint32(self->GetTransTime());
        *data << float(self->GetTransOffsetY());
        //data->WriteGuidBytes<1>(transGuid);
        *data << float(self->GetTransOffsetZ());
        *data << int8(self->GetTransSeat());
        if (uint32 time3 = self->m_movementInfo.transport.vehicleId)
            *data << uint32(time3);
        //data->WriteGuidBytes<6>(transGuid);
        *data << float(Position::NormalizeOrientation(self->GetTransOffsetO()));
        //data->WriteGuidBytes<5, 0>(transGuid);
        *data << float(self->GetTransOffsetX());
    }

    if (flags & UPDATEFLAG_ROTATION)
        *data << uint64(ToGameObject()->GetRotation());

    if (flags & UPDATEFLAG_HAS_TARGET)
    {
        ObjectGuid victimGuid = ToUnit()->getVictim()->GetGUID();   // checked in BuildCreateUpdateBlockForPlayer

        //data->WriteGuidBytes<4, 6, 3, 0, 7, 1, 2, 5>(victimGuid);
    }

    if (flags & UPDATEFLAG_ANIMKITS)
    {
        //if (hasAnimKit3)
        //    *data << uint16(0);
        //if (hasAnimKit1)
        //    *data << uint16(0);
        //if (hasAnimKit2)
        //    *data << uint16(0);
        
    }

    if (flags & UPDATEFLAG_TRANSPORT)
        *data << uint32(getMSTime());

    if (flags & UPDATEFLAG_VEHICLE)
    {
        Unit const* self = ToUnit();

        *data << uint32(self->GetVehicleKit()->GetVehicleInfo()->ID);
        *data << float(self->GetOrientation());
    }

    if (flags & UPDATEFLAG_HAS_WORLDEFFECTID)
    {
        if(GameObject const* go = ToGameObject())
            *data << uint32(go->GetGOInfo()->WorldEffectID);
        else
            *data << uint32(0);
    }

    if (flags & UPDATEFLAG_LIVING)
    {
        Unit const* self = ToUnit();

        if (self->IsSplineEnabled())
            Movement::PacketBuilder::WriteFacingData(*self->movespline, *data);
    }*/
}

void Object::_BuildValuesUpdate(uint8 updateType, ByteBuffer* data, Player* target) const
{
    if (!target)
        return;

    ByteBuffer fieldBuffer;
    UpdateMask updateMask;
    updateMask.SetCount(m_valuesCount);

    bool IsActivateToQuest = false;
    if (updateType == UPDATETYPE_CREATE_OBJECT || updateType == UPDATETYPE_CREATE_OBJECT2)
    {
        if (isType(TYPEMASK_GAMEOBJECT) && !((GameObject*)this)->IsDynTransport())
        {
            if (((GameObject*)this)->ActivateToQuest(target))
                IsActivateToQuest = true;

            if (((GameObject*)this)->GetGoArtKit())
                _changedFields[GAMEOBJECT_FIELD_BYTES_1] = true;
        }
        else if (isType(TYPEMASK_UNIT))
        {
            if (((Unit*)this)->HasFlag(UNIT_FIELD_AURA_STATE, PER_CASTER_AURA_STATE_MASK))
                _changedFields[UNIT_FIELD_AURA_STATE] = true;
        }
    }
    else                                                    // case UPDATETYPE_VALUES
    {
        if (isType(TYPEMASK_GAMEOBJECT) && !((GameObject*)this)->IsTransport())
        {
            if (((GameObject*)this)->ActivateToQuest(target))
                IsActivateToQuest = true;

            _changedFields[GAMEOBJECT_FIELD_BYTES_1] = true;

            if (ToGameObject()->GetGoType() == GAMEOBJECT_TYPE_CHEST && ToGameObject()->GetGOInfo()->chest.usegrouplootrules &&
                ToGameObject()->HasLootRecipient())
                _changedFields[GAMEOBJECT_FIELD_FLAGS] = true;
        }
        else if (isType(TYPEMASK_UNIT))
        {
            if (((Unit*)this)->HasFlag(UNIT_FIELD_AURA_STATE, PER_CASTER_AURA_STATE_MASK))
                _changedFields[UNIT_FIELD_AURA_STATE] = true;
        }
    }

    uint32* flags = NULL;
    uint32 visibleFlag = GetUpdateFieldData(target, flags);

    uint32 valCount = m_valuesCount;
    if (GetTypeId() == TYPEID_PLAYER && target != this)
        valCount = PLAYER_END_NOT_SELF;

    if (target == this)
        visibleFlag |= UF_FLAG_PRIVATE;
    else if (GetTypeId() == TYPEID_PLAYER)
        valCount = PLAYER_END_NOT_SELF;
    
    for (uint16 index = 0; index < valCount; ++index)
    {
        if (_fieldNotifyFlags & flags[index] ||
            ((flags[index] & visibleFlag) & UF_FLAG_SPECIAL_INFO) ||
            ((updateType == UPDATETYPE_VALUES ? _changedFields[index] : m_uint32Values[index]) && (flags[index] & visibleFlag)) ||
            (index == UNIT_FIELD_AURA_STATE && isType(TYPEMASK_UNIT) && HasFlag(UNIT_FIELD_AURA_STATE, PER_CASTER_AURA_STATE_MASK)))
        {
            updateMask.SetBit(index);

            if (isType(TYPEMASK_UNIT))                               // unit (creature/player) case
            {
                if (index == UNIT_FIELD_NPC_FLAGS)
                {
                    // remove custom flag before sending
                    uint32 appendValue = m_uint32Values[index];

                    if (GetTypeId() == TYPEID_UNIT)
                    {
                        if (!target->canSeeSpellClickOn(this->ToCreature()))
                            appendValue &= ~UNIT_NPC_FLAG_SPELLCLICK;

                        if (appendValue & UNIT_NPC_FLAG_TRAINER)
                        {
                            if (!this->ToCreature()->isCanTrainingOf(target, false))
                                appendValue &= ~(UNIT_NPC_FLAG_TRAINER | UNIT_NPC_FLAG_TRAINER_CLASS | UNIT_NPC_FLAG_TRAINER_PROFESSION);
                        }
                    }

                    fieldBuffer << uint32(appendValue);
                    continue; //skip by custom write
                }
                else if (index == UNIT_FIELD_NPC_FLAGS2)
                {
                    uint32 appendValue = m_uint32Values[index];

                    if (appendValue & UNIT_NPC_FLAG2_GARRISON_ARCHITECT)
                    {
                        if (Garrison *garr = target->GetGarrison())
                            if (!garr->GetCountOfBluePrints())
                                appendValue &= ~UNIT_NPC_FLAG2_GARRISON_ARCHITECT;
                    }

                    if (appendValue & UNIT_NPC_FLAG2_GARRISON_MISSION_NPC)
                    {
                        if (Garrison *garr = target->GetGarrison())
                            if (!garr->GetCountOFollowers())
                                appendValue &= ~UNIT_NPC_FLAG2_GARRISON_MISSION_NPC;
                    }

                    fieldBuffer << uint32(appendValue);
                    continue; //skip by custom write
                }
                else if (index == UNIT_FIELD_AURA_STATE)
                {
                    // Check per caster aura states to not enable using a pell in client if specified aura is not by target
                    fieldBuffer << ((Unit*)this)->BuildAuraStateUpdateForTarget(target);
                    continue; //skip by custom write
                }
                else if (index == UNIT_FIELD_MAX_DAMAGE || index == UNIT_FIELD_MIN_DAMAGE || index == UNIT_FIELD_MIN_OFF_HAND_DAMAGE || index == UNIT_FIELD_MAX_OFF_HAND_DAMAGE)
                {
                    fieldBuffer << (m_floatValues[index] + CalculatePct(m_floatValues[index], ((Unit*)this)->GetTotalAuraModifier(SPELL_AURA_MOD_AUTOATTACK_DAMAGE)));
                    continue; //skip by custom write
                }
                // FIXME: Some values at server stored in float format but must be sent to client in uint32 format
                else if (index >= UNIT_FIELD_ATTACK_ROUND_BASE_TIME && index <= UNIT_FIELD_RANGED_ATTACK_ROUND_BASE_TIME)
                {
                    // convert from float to uint32 and send
                    fieldBuffer << uint32(m_floatValues[index] < 0 ? 0 : (RoundingFloatValue(m_floatValues[index] / 10) * 10));
                    continue; //skip by custom write
                }
                // there are some float values which may be negative or can't get negative due to other checks
                else if ((index >= UNIT_FIELD_STAT_NEG_BUFF && index <= UNIT_FIELD_STAT_NEG_BUFF+4) ||
                    (index >= UNIT_FIELD_RESISTANCE_BUFF_MODS_POSITIVE && index <= (UNIT_FIELD_RESISTANCE_BUFF_MODS_POSITIVE + 6)) ||
                    (index >= UNIT_FIELD_RESISTANCE_BUFF_MODS_NEGATIVE && index <= (UNIT_FIELD_RESISTANCE_BUFF_MODS_NEGATIVE + 6)) ||
                    (index >= UNIT_FIELD_STAT_POS_BUFF && index <= UNIT_FIELD_STAT_POS_BUFF + 4))
                {
                    fieldBuffer << uint32(m_floatValues[index]);
                    continue; //skip by custom write
                }
                // Gamemasters should be always able to select units - remove not selectable flag
                else if (index == UNIT_FIELD_FLAGS)
                {
                    if (target->isGameMaster())
                        fieldBuffer << (m_uint32Values[index] & ~UNIT_FLAG_NOT_SELECTABLE);
                    else
                        fieldBuffer << m_uint32Values[index];
                    continue; //skip by custom write
                }
                // use modelid_a if not gm, _h if gm for CREATURE_FLAG_EXTRA_TRIGGER creatures
                else if (index == UNIT_FIELD_DISPLAY_ID)
                {
                    if (GetTypeId() == TYPEID_UNIT)
                    {
                        CreatureModelInfo const* modelInfo = sObjectMgr->GetCreatureModelInfo(m_uint32Values[index]);
                        CreatureTemplate const* cinfo = ToCreature()->GetCreatureTemplate();

                        // this also applies for transform auras
                        if (SpellInfo const* transform = sSpellMgr->GetSpellInfo(ToUnit()->getTransForm()))
                            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                                if (transform->Effects[i].IsAura(SPELL_AURA_TRANSFORM))
                                    if (CreatureTemplate const* transformInfo = sObjectMgr->GetCreatureTemplate(transform->Effects[i].MiscValue))
                                    {
                                        cinfo = transformInfo;
                                        break;
                                    }

                        if(modelInfo && modelInfo->hostileId && ToUnit()->IsHostileTo(target))
                            fieldBuffer << modelInfo->hostileId;
                        else if (cinfo->flags_extra & CREATURE_FLAG_EXTRA_TRIGGER)
                        {
                            if (target->isGameMaster())
                            {
                                if (cinfo->Modelid1)
                                    fieldBuffer << cinfo->Modelid1;//Modelid1 is a visible model for gms
                                else
                                    fieldBuffer << 17519; // world invisible trigger's model
                            }
                            else
                            {
                                if (cinfo->Modelid2)
                                    fieldBuffer << cinfo->Modelid2;//Modelid2 is an invisible model for players
                                else
                                    fieldBuffer << 11686; // world invisible trigger's model
                            }
                            continue; //skip by custom write
                        }
                    }

                }
                // hide lootable animation for unallowed players
                else if (index == OBJECT_FIELD_DYNAMIC_FLAGS)
                {
                    uint32 dynamicFlags = m_uint32Values[index];

                    if (Creature const* creature = ToCreature())
                    {
                        if (creature->hasLootRecipient())
                        {
                            if(creature->IsPersonalLoot())
                            {
                                dynamicFlags |= (UNIT_DYNFLAG_TAPPED | UNIT_DYNFLAG_TAPPED_BY_PLAYER | UNIT_DYNFLAG_TAPPED_BY_ALL_THREAT_LIST);
                            }
                            else if (creature->isTappedBy(target))
                            {
                                dynamicFlags |= (UNIT_DYNFLAG_TAPPED | UNIT_DYNFLAG_TAPPED_BY_PLAYER);
                            }
                            else
                            {
                                dynamicFlags |= UNIT_DYNFLAG_TAPPED;
                                dynamicFlags &= ~UNIT_DYNFLAG_TAPPED_BY_PLAYER;
                            }
                        }
                        else
                        {
                            dynamicFlags &= ~UNIT_DYNFLAG_TAPPED;
                            dynamicFlags &= ~UNIT_DYNFLAG_TAPPED_BY_PLAYER;
                        }

                        if (!target->isAllowedToLoot(const_cast<Creature*>(creature)))
                            dynamicFlags &= ~UNIT_DYNFLAG_LOOTABLE;
                    }

                    // unit UNIT_DYNFLAG_TRACK_UNIT should only be sent to caster of SPELL_AURA_MOD_STALKED auras
                    if (Unit const* unit = ToUnit())
                        if (dynamicFlags & UNIT_DYNFLAG_TRACK_UNIT)
                            if (!unit->HasAuraTypeWithCaster(SPELL_AURA_MOD_STALKED, target->GetGUID()))
                                dynamicFlags &= ~UNIT_DYNFLAG_TRACK_UNIT;

                    fieldBuffer << dynamicFlags;
                    continue; //skip by custom write
                }
                // FG: pretend that OTHER players in own group are friendly ("blue")
                else if (index == UNIT_FIELD_BYTES_2 || index == UNIT_FIELD_FACTION_TEMPLATE)
                {
                    Unit const* unit = ToUnit();
                    if (!unit->HasAura(119626) && unit->IsControlledByPlayer() && target != this && sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP) && unit->IsInRaidWith(target))
                    {
                        FactionTemplateEntry const* ft1 = unit->getFactionTemplateEntry();
                        FactionTemplateEntry const* ft2 = target->getFactionTemplateEntry();
                        if (ft1 && ft2 && !ft1->IsFriendlyTo(*ft2))
                        {
                            if (index == UNIT_FIELD_BYTES_2)
                            {
                                // Allow targetting opposite faction in party when enabled in config
                                fieldBuffer << (m_uint32Values[index] & ((UNIT_BYTE2_FLAG_SANCTUARY /*| UNIT_BYTE2_FLAG_AURAS | UNIT_BYTE2_FLAG_UNK5*/) << 8)); // this flag is at uint8 offset 1 !!
                            }
                            else
                            {
                                // pretend that all other HOSTILE players have own faction, to allow follow, heal, rezz (trade wont work)
                                fieldBuffer << uint32(target->getFaction());
                            }
                            continue; //skip by custom write
                        }
                    }
                }
            }
            else if (isType(TYPEMASK_GAMEOBJECT))                    // gameobject case
            {
                // send in current format (float as float, uint32 as uint32)
                if (index == OBJECT_FIELD_DYNAMIC_FLAGS)
                {
                    if (IsActivateToQuest || target->isGameMaster())
                    {
                        switch (ToGameObject()->GetGoType())
                        {
                            case GAMEOBJECT_TYPE_CHEST:
                            case GAMEOBJECT_TYPE_GOOBER:
                            case GAMEOBJECT_TYPE_CAPTURE_POINT:
                                if (!IsActivateToQuest)
                                    fieldBuffer << uint16(GO_DYNFLAG_LO_ACTIVATE);
                                else
                                    fieldBuffer << uint16(GO_DYNFLAG_LO_ACTIVATE | GO_DYNFLAG_LO_SPARKLE);
                                break;
                            case GAMEOBJECT_TYPE_GENERIC:
                            case GAMEOBJECT_TYPE_SPELL_FOCUS:
                                if (!IsActivateToQuest)
                                    fieldBuffer << uint16(0);
                                else
                                    fieldBuffer << uint16(GO_DYNFLAG_LO_SPARKLE);
                                break;
                            default:
                                fieldBuffer << uint16(0); // unknown, not happen.
                                break;
                        }
                    }
                    else
                        fieldBuffer << uint16(0);         // disable quest object

                    fieldBuffer << uint16(-1);
                    continue; //skip by custom write
                }
                else if (index == GAMEOBJECT_FIELD_FLAGS)
                {
                    uint32 flags = m_uint32Values[index];
                    if (ToGameObject()->GetGoType() == GAMEOBJECT_TYPE_CHEST)
                        if (ToGameObject()->GetGOInfo()->chest.usegrouplootrules && !ToGameObject()->IsLootAllowedFor(target))
                            flags |= GO_FLAG_LOCKED | GO_FLAG_NOT_SELECTABLE;

                    fieldBuffer << flags;
                    continue; //skip by custom write
                }
                else if (index == GAMEOBJECT_FIELD_BYTES_1)
                {
                    if (((GameObject*)this)->GetGOInfo()->type == GAMEOBJECT_TYPE_TRANSPORT)
                    {
                        fieldBuffer << uint32(m_uint32Values[index] | GO_STATE_TRANSPORT_SPEC);
                        continue; //skip by custom write
                    }                    
                }
                else if (index == GAMEOBJECT_FIELD_STATE_WORLD_EFFECT_ID || index == GAMEOBJECT_FIELD_STATE_SPELL_VISUAL_ID)
                {
                    if (((GameObject*)this)->GetGOInfo()->visualQuestID)
                    {
                        fieldBuffer << target->GetGoVisualQuestData((GameObject*)this, index);
                        continue;
                    }
                }
            }
            else if (isType(TYPEMASK_DYNAMICOBJECT))                    // dynamiobject case
            {
                if (index == DYNAMICOBJECT_FIELD_BYTES)
                {
                    uint32 visualId = ((DynamicObject*)this)->GetVisualId();
                    DynamicObjectType dynType = ((DynamicObject*)this)->GetType();
                    Unit* caster = ((DynamicObject*)this)->GetCaster();
                    SpellVisualEntry const* visualEntry = sSpellVisualStore.LookupEntry(visualId);
                    if(caster && visualEntry && visualEntry->hostileId && caster->IsHostileTo(target))
                    {
                        fieldBuffer << ((dynType << 28) | visualEntry->hostileId);
                        continue; //skip by custom write
                    }
                }

            }
            else if (isType(TYPEMASK_AREATRIGGER))                    // AreaTrigger case
            {
                if (index == AREATRIGGER_FIELD_SPELL_VISUAL_ID)
                {
                    uint32 visualId = m_uint32Values[index];
                    Unit* caster = ((AreaTrigger*)this)->GetCaster();
                    SpellVisualEntry const* visualEntry = sSpellVisualStore.LookupEntry(visualId);
                    if(caster && visualEntry && visualEntry->hostileId && caster->IsHostileTo(target))
                    {
                        fieldBuffer << (visualEntry->hostileId);
                        continue; //skip by custom write
                    }
                }
            }

            fieldBuffer << m_uint32Values[index];
        }
    }

    *data << uint8(updateMask.GetBlockCount());
    updateMask.AppendToPacket(data);
    data->append(fieldBuffer);
}

void Object::BuildDynamicValuesUpdate(uint8 updateType, ByteBuffer *data, Player* target) const
{
    if (!target)
        return;

    ByteBuffer fieldBuffer;
    UpdateMask updateMask;
    updateMask.SetCount(_dynamicValuesCount);

    uint32* flags = nullptr;
    uint32 visibleFlag = GetDynamicUpdateFieldData(target, flags);

    for (uint16 index = 0; index < _dynamicValuesCount; ++index)
    {
        ByteBuffer buffer;
        std::vector<uint32> const& values = _dynamicValues[index];
        if (_fieldNotifyFlags & flags[index] ||
            ((updateType == UPDATETYPE_VALUES ? _dynamicChangesMask.GetBit(index) : !values.empty()) && (flags[index] & visibleFlag)))
        {
            updateMask.SetBit(index);

            UpdateMask arrayMask;
            arrayMask.SetCount(values.size());
            for (size_t v = 0; v < values.size(); ++v)
            {
                if (updateType != UPDATETYPE_VALUES || _dynamicChangesArrayMask[index].GetBit(v))
                {
                    arrayMask.SetBit(v);
                    buffer << uint32(values[v]);
                }
            }

            fieldBuffer << uint8(arrayMask.GetBlockCount());
            arrayMask.AppendToPacket(&fieldBuffer);
            fieldBuffer.append(buffer);
        }
    }

    *data << uint8(updateMask.GetBlockCount());
    updateMask.AppendToPacket(data);
    data->append(fieldBuffer);
    //// Crashfix, prevent use of bag with dynamic field
    //if (/*isType(TYPEMAST_BAG) || */
    //    (updatetype == UPDATETYPE_VALUES && GetTypeId() == TYPEID_PLAYER && this != target))
    //{
    //    *data << uint8(0);
    //    return;
    //}

    //uint32 dynamicTabMask = 0;
    //std::vector<uint32> dynamicFieldsMask;
    //dynamicFieldsMask.resize(m_dynamicTab.size());

    //for (size_t i = 0; i < m_dynamicTab.size(); ++i)
    //{
    //    dynamicFieldsMask[i] = 0;
    //    for (int index = 0; index < 32; ++index)
    //    {
    //        if ((updatetype == UPDATETYPE_CREATE_OBJECT || updatetype == UPDATETYPE_CREATE_OBJECT2) ||
    //            updatetype == UPDATETYPE_VALUES && m_dynamicChange[i])
    //        {
    //            dynamicTabMask |= 1 << i;
    //            if (m_dynamicTab[i][index] != 0)
    //                dynamicFieldsMask[i] |= 1 << index;
    //        }
    //    }
    //}

    //*data << uint8(dynamicTabMask ? 1 : 0); // count of dynamic tab masks
    //if (dynamicTabMask)
    //{
    //    *data << uint32(dynamicTabMask);

    //    for (size_t i = 0; i < m_dynamicTab.size(); ++i)
    //    {
    //        if (dynamicTabMask & (1 << i))
    //        {
    //            *data << uint8(bool(dynamicFieldsMask[i]));      // count of dynamic field masks
    //            if (dynamicFieldsMask[i])
    //            {
    //                *data << uint32(dynamicFieldsMask[i]);

    //                for (int index = 0; index < 32; ++index)
    //                {
    //                    if (dynamicFieldsMask[i] & (1 << index))
    //                        *data << uint32(m_dynamicTab[i][index]);
    //                }
    //            }
    //        }
    //    }
    //}
}

void Object::ClearUpdateMask(bool remove)
{
    _dynamicChangesMask.Clear();
    for (uint32 i = 0; i < _dynamicValuesCount; ++i)
        _dynamicChangesArrayMask[i].Clear();

    memset(_changedFields, 0, m_valuesCount*sizeof(bool));

    if (m_objectUpdated)
    {
        if (remove)
            sObjectAccessor->RemoveUpdateObject(this);
        m_objectUpdated = false;
    }
}

void Object::BuildFieldsUpdate(Player* player, UpdateDataMapType& data_map) const
{
    UpdateDataMapType::iterator iter = data_map.find(player);

    if (iter == data_map.end())
    {
        std::pair<UpdateDataMapType::iterator, bool> p = data_map.insert(UpdateDataMapType::value_type(player, UpdateData(player->GetMapId())));
        ASSERT(p.second);
        iter = p.first;
    }

    BuildValuesUpdateBlockForPlayer(&iter->second, iter->first);
}

void Object::_LoadIntoDataField(char const* data, uint32 startOffset, uint32 count)
{
    if (!data)
        return;

    Tokenizer tokens(data, ' ', count);

    if (tokens.size() != count)
        return;

    for (uint32 index = 0; index < count; ++index)
    {
        m_uint32Values[startOffset + index] = atol(tokens[index]);
        _changedFields[startOffset + index] = true;
    }
}

uint32 Object::GetUpdateFieldData(Player const* target, uint32*& flags) const
{
    uint32 visibleFlag = UF_FLAG_PUBLIC;

    if (target == this)
        visibleFlag |= UF_FLAG_PRIVATE;

    // This function assumes updatefield index is always valid
    switch (GetTypeId())
    {
        case TYPEID_ITEM:
        case TYPEID_CONTAINER:
            flags = ItemUpdateFieldFlags;
            if (((Item const*)this)->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER | UF_FLAG_ITEM_OWNER;
            break;
        case TYPEID_UNIT:
        case TYPEID_PLAYER:
        {
            Player* plr = ToUnit()->GetCharmerOrOwnerPlayerOrPlayerItself();
            flags = UnitUpdateFieldFlags;
            if (ToUnit()->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;

            if (HasFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_SPECIALINFO))
                if (ToUnit()->HasAuraTypeWithCaster(SPELL_AURA_EMPATHY, target->GetGUID()))
                    visibleFlag |= UF_FLAG_SPECIAL_INFO;
            
            if (plr && plr->IsInSameRaidWith(target))
                visibleFlag |= UF_FLAG_PARTY_MEMBER;
            break;
        }
        case TYPEID_GAMEOBJECT:
            flags = GameObjectUpdateFieldFlags;
            if (ToGameObject()->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;
            break;
        case TYPEID_DYNAMICOBJECT:
            flags = DynamicObjectUpdateFieldFlags;
            if (ToDynObject()->GetCasterGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;
            break;
        case TYPEID_CORPSE:
            flags = CorpseUpdateFieldFlags;
            if (ToCorpse()->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;
            break;
        case TYPEID_AREATRIGGER:
            flags = AreaTriggerUpdateFieldFlags;
            if (ToAreaTrigger()->GetCasterGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;
            break;
        case TYPEID_CONVERSATION:
            flags = ConversationUpdateFieldFlags;
            break;
        case TYPEID_OBJECT:
            break;
    }
    return visibleFlag;
}

uint32 Object::GetDynamicUpdateFieldData(Player const* target, uint32*& flags) const
{
    uint32 visibleFlag = UF_FLAG_PUBLIC;

    if (target == this)
        visibleFlag |= UF_FLAG_PRIVATE;

    switch (GetTypeId())
    {
        case TYPEID_ITEM:
        case TYPEID_CONTAINER:
            flags = ItemDynamicFieldFlags;
            if (((Item const*)this)->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER | UF_FLAG_ITEM_OWNER;
            break;
        case TYPEID_UNIT:
        case TYPEID_PLAYER:
        {
            Player* plr = ToUnit()->GetCharmerOrOwnerPlayerOrPlayerItself();
            flags = UnitDynamicFieldFlags;
            if (ToUnit()->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;

            if (HasFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_SPECIALINFO))
                if (ToUnit()->HasAuraTypeWithCaster(SPELL_AURA_EMPATHY, target->GetGUID()))
                    visibleFlag |= UF_FLAG_SPECIAL_INFO;

            if (plr && plr->IsInSameRaidWith(target))
                visibleFlag |= UF_FLAG_PARTY_MEMBER;
            break;
        }
        case TYPEID_CONVERSATION:
            flags = ConversationDynamicFieldFlags;
            break;
        default:
            flags = nullptr;
            break;
    }

    return visibleFlag;
}


void Object::SetInt32Value(uint16 index, int32 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (m_int32Values[index] != value)
    {
        m_int32Values[index] = value;
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::UpdateInt32Value(uint16 index, int32 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    m_int32Values[index] = value;
}

void Object::SetUInt32Value(uint16 index, uint32 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (m_uint32Values[index] != value)
    {
        m_uint32Values[index] = value;
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::UpdateUInt32Value(uint16 index, uint32 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    m_uint32Values[index] = value;
}

void Object::SetUInt64Value(uint16 index, uint64 value)
{
    ASSERT(index + 1 < m_valuesCount || PrintIndexError(index, true));
    if (*((uint64*)&(m_uint32Values[index])) != value)
    {
        m_uint32Values[index] = PAIR64_LOPART(value);
        m_uint32Values[index + 1] = PAIR64_HIPART(value);
        _changedFields[index] = true;
        _changedFields[index + 1] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

bool Object::AddGuidValue(uint16 index, ObjectGuid const& value)
{
    ASSERT(index + 3 < m_valuesCount || PrintIndexError(index, true));
    if (!value.IsEmpty() && ((ObjectGuid*)&(m_uint32Values[index]))->IsEmpty())
    {
        *((ObjectGuid*)&(m_uint32Values[index])) = value;
        _changedFields[index] = true;
        _changedFields[index + 1] = true;
        _changedFields[index + 2] = true;
        _changedFields[index + 3] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }

        return true;
    }

    return false;
}

bool Object::RemoveGuidValue(uint16 index, ObjectGuid const& value)
{
    ASSERT(index + 3 < m_valuesCount || PrintIndexError(index, true));
    if (!value.IsEmpty() && *((ObjectGuid*)&(m_uint32Values[index])) == value)
    {
        m_uint32Values[index] = 0;
        m_uint32Values[index + 1] = 0;
        m_uint32Values[index + 2] = 0;
        m_uint32Values[index + 3] = 0;
        _changedFields[index] = true;
        _changedFields[index + 1] = true;
        _changedFields[index + 2] = true;
        _changedFields[index + 3] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }

        return true;
    }

    return false;
}

void Object::SetFloatValue(uint16 index, float value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (m_floatValues[index] != value)
    {
        m_floatValues[index] = value;
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetByteValue(uint16 index, uint8 offset, uint8 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (offset > 4)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Object::SetByteValue: wrong offset %u", offset);
        return;
    }

    if (uint8(m_uint32Values[index] >> (offset * 8)) != value)
    {
        m_uint32Values[index] &= ~uint32(uint32(0xFF) << (offset * 8));
        m_uint32Values[index] |= uint32(uint32(value) << (offset * 8));
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetUInt16Value(uint16 index, uint8 offset, uint16 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (offset > 2)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Object::SetUInt16Value: wrong offset %u", offset);
        return;
    }

    if (uint16(m_uint32Values[index] >> (offset * 16)) != value)
    {
        m_uint32Values[index] &= ~uint32(uint32(0xFFFF) << (offset * 16));
        m_uint32Values[index] |= uint32(uint32(value) << (offset * 16));
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetGuidValue(uint16 index, ObjectGuid const& value)
{
    ASSERT(index + 3 < m_valuesCount || PrintIndexError(index, true));
    if (*((ObjectGuid*)&(m_uint32Values[index])) != value)
    {
        *((ObjectGuid*)&(m_uint32Values[index])) = value;
        _changedFields[index] = true;
        _changedFields[index + 1] = true;
        _changedFields[index + 2] = true;
        _changedFields[index + 3] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetStatFloatValue(uint16 index, float value)
{
    if (value < 0)
        value = 0.0f;

    SetFloatValue(index, value);
}

void Object::SetStatInt32Value(uint16 index, int32 value)
{
    if (value < 0)
        value = 0;

    SetUInt32Value(index, uint32(value));
}

void Object::ApplyModUInt32Value(uint16 index, int32 val, bool apply)
{
    int32 cur = GetUInt32Value(index);
    cur += (apply ? val : -val);
    if (cur < 0)
        cur = 0;
    SetUInt32Value(index, cur);
}

void Object::ApplyModInt32Value(uint16 index, int32 val, bool apply)
{
    int32 cur = GetInt32Value(index);
    cur += (apply ? val : -val);
    SetInt32Value(index, cur);
}

void Object::ApplyModSignedFloatValue(uint16 index, float  val, bool apply)
{
    float cur = GetFloatValue(index);
    cur += (apply ? val : -val);
    SetFloatValue(index, cur);
}

void Object::ApplyModPositiveFloatValue(uint16 index, float  val, bool apply)
{
    float cur = GetFloatValue(index);
    cur += (apply ? val : -val);
    if (cur < 0)
        cur = 0;
    SetFloatValue(index, cur);
}

void Object::SetFlag(uint16 index, uint32 newFlag)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));
    uint32 oldval = m_uint32Values[index];
    uint32 newval = oldval | newFlag;

    if (oldval != newval)
    {
        m_uint32Values[index] = newval;
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::RemoveFlag(uint16 index, uint32 oldFlag)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));
    ASSERT(m_uint32Values);

    uint32 oldval = m_uint32Values[index];
    uint32 newval = oldval & ~oldFlag;

    if (oldval != newval)
    {
        m_uint32Values[index] = newval;
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetByteFlag(uint16 index, uint8 offset, uint8 newFlag)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (offset > 4)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Object::SetByteFlag: wrong offset %u", offset);
        return;
    }

    if (!(uint8(m_uint32Values[index] >> (offset * 8)) & newFlag))
    {
        m_uint32Values[index] |= uint32(uint32(newFlag) << (offset * 8));
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::RemoveByteFlag(uint16 index, uint8 offset, uint8 oldFlag)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (offset > 4)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Object::RemoveByteFlag: wrong offset %u", offset);
        return;
    }

    if (uint8(m_uint32Values[index] >> (offset * 8)) & oldFlag)
    {
        m_uint32Values[index] &= ~uint32(uint32(oldFlag) << (offset * 8));
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

std::vector<uint32> const& Object::GetDynamicValues(uint16 index) const
{
    ASSERT(index < _dynamicValuesCount || PrintIndexError(index, false));
    return _dynamicValues[index];
}

void Object::AddDynamicValue(uint16 index, uint32 value)
{
    ASSERT(index < _dynamicValuesCount || PrintIndexError(index, false));

    std::vector<uint32>& values = _dynamicValues[index];

    UpdateMask& mask = _dynamicChangesArrayMask[index];

    _dynamicChangesMask.SetBit(index);
    if (values.size() >= values.capacity())
        values.reserve(values.capacity() + 32);

    values.push_back(value);
    if (mask.GetCount() < values.size())
        mask.AddBlock();

    mask.SetBit(values.size()-1);

    if (m_inWorld && !m_objectUpdated)
    {
        sObjectAccessor->AddUpdateObject(this);
        m_objectUpdated = true;
    }
}

void Object::ClearDynamicValue(uint16 index)
{
    ASSERT(index < _dynamicValuesCount || PrintIndexError(index, false));

    if (!_dynamicValues[index].empty())
    {
        _dynamicValues[index].clear();
        _dynamicChangesMask.SetBit(index);
        _dynamicChangesArrayMask[index].SetCount(0);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetDynamicUInt32Value(uint32 index, uint16 offset, uint32 value)
{
    ASSERT(index < _dynamicValuesCount || PrintIndexError(index, false));

    std::vector<uint32>& values = _dynamicValues[index];

    //if (offset >= values.capacity())
    //    values.reserve(offset + 32);

    ASSERT(offset < values.size());

    if (values[offset] != value)
    {
        values[offset] = value;
        _dynamicChangesMask.SetBit(index);
        _dynamicChangesArrayMask[index].SetBit(offset);

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

ObjectGuid const& Object::GetGuidValue(uint16 index) const
{
    ASSERT(index + 1 < m_valuesCount || PrintIndexError(index, false));
    return *((ObjectGuid*)&(m_uint32Values[index]));
}

bool Object::PrintIndexError(uint32 index, bool set) const
{
    sLog->outError(LOG_FILTER_GENERAL, "Attempt %s non-existed value field: %u (count: %u) for object typeid: %u type mask: %u", (set ? "set value to" : "get value from"), index, m_valuesCount, GetTypeId(), m_objectType);

    // ASSERT must fail after function call
    return false;
}

bool Position::HasInLine(WorldObject const* target, float width) const
{
    if (!HasInArc(M_PI, target))
        return false;
    width += target->GetObjectSize();
    float angle = GetRelativeAngle(target);
    return fabs(sin(angle)) * GetExactDist2d(target->GetPositionX(), target->GetPositionY()) < width;
}

bool Position::IsInDegreesRange(float x, float y, float degresA, float degresB, bool relative/* = false*/) const
{
    float angel = GetDegreesAngel(x, y, relative);
    return angel >= degresA && angel <= degresB;
}

float Position::GetDegreesAngel(float x, float y, bool relative) const
{
    float angel = relative ? GetRelativeAngle(x, y) : GetAngle(x, y);
    return NormalizeOrientation(angel) * M_RAD;
}

Position Position::GetRandPointBetween(const Position &B) const
{
    float Lambda = urand(0.0f, 100.0f) / 100.0f;
    float X = (B.GetPositionX() + Lambda * GetPositionX()) / (1 + Lambda);
    float Y = (B.GetPositionY() + Lambda * GetPositionY()) / (1 + Lambda);
    //Z should be updated by Vmap
    float Z = (B.GetPositionZ() + Lambda * GetPositionZ()) / (1 + Lambda);

    Position result;
    result.Relocate(X, Y, Z);
    return result;
}

void Position::SimplePosXYRelocationByAngle(Position &pos, float dist, float angle, bool relative) const
{
    if(!relative)
        angle += GetOrientation();

    pos.m_positionX = m_positionX + dist * std::cos(angle);
    pos.m_positionY = m_positionY + dist * std::sin(angle);
    pos.m_positionZ = m_positionZ;

    // Prevent invalid coordinates here, position is unchanged
    if (!Trinity::IsValidMapCoord(pos.m_positionX, pos.m_positionY))
    {
        pos.Relocate(this);
        sLog->outFatal(LOG_FILTER_GENERAL, "Position::SimplePosXYRelocationByAngle invalid coordinates X: %f and Y: %f were passed!", pos.m_positionX, pos.m_positionY);
        return;
    }

    Trinity::NormalizeMapCoord(pos.m_positionX);
    Trinity::NormalizeMapCoord(pos.m_positionY);
    pos.SetOrientation(GetOrientation());
}

void Position::SimplePosXYRelocationByAngle(float &x, float &y, float &z, float dist, float angle, bool relative) const
{
    if(!relative)
        angle += GetOrientation();

    x = m_positionX + dist * std::cos(angle);
    y = m_positionY + dist * std::sin(angle);
    z = m_positionZ;

    // Prevent invalid coordinates here, position is unchanged
    if (!Trinity::IsValidMapCoord(x, y))
    {
        x = m_positionX;
        y = m_positionY;
        z = m_positionZ;
        sLog->outFatal(LOG_FILTER_GENERAL, "Position::SimplePosXYRelocationByAngle invalid coordinates X: %f and Y: %f were passed!", x, y);
        return;
    }

    Trinity::NormalizeMapCoord(x);
    Trinity::NormalizeMapCoord(y);
}

bool Position::IsLinesCross(Position const &pos11, Position const &pos12, Position const &pos21, Position const &pos22) const
{
    //Line 1
    G3D::Vector2 p11(pos11.GetPositionX(), pos11.GetPositionY());
    G3D::Vector2 p12(pos12.GetPositionX(), pos12.GetPositionY());
    LineSegment2D line1 = LineSegment2D::fromTwoPoints(p11, p12);
    //Line 2
    G3D::Vector2 p21(pos21.GetPositionX(), pos21.GetPositionY());
    G3D::Vector2 p22(pos22.GetPositionX(), pos22.GetPositionY());
    LineSegment2D line2 = LineSegment2D::fromTwoPoints(p21, p22);

    //check line
    if(line1.intersection(line2) == G3D::Vector2::inf())
        return false;
    else
        return true;
}

std::string Position::ToString() const
{
    std::stringstream sstr;
    sstr << "X: " << m_positionX << " Y: " << m_positionY << " Z: " << m_positionZ << " O: " << m_orientation;
    return sstr.str();
}

ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYStreamer const& streamer)
{
    buf << streamer.Pos->GetPositionX();
    buf << streamer.Pos->GetPositionY();

    return buf;
}

ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYStreamer const& streamer)
{
    float x, y;
    buf >> x >> y;
    streamer.Pos->Relocate(x, y);

    return buf;
}

ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer)
{
    float x, y, z;
    streamer.m_pos->GetPosition(x, y, z);
    buf << x << y << z;

    return buf;
}

ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer)
{
    float x, y, z;
    buf >> x >> y >> z;
    streamer.m_pos->Relocate(x, y, z);

    return buf;
}

ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer)
{
    buf << streamer.m_pos->GetPositionX();
    buf << streamer.m_pos->GetPositionY();
    buf << streamer.m_pos->GetPositionZ();
    buf << streamer.m_pos->GetOrientation();

    return buf;
}

ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer)
{
    float x, y, z, o;
    buf >> x >> y >> z >> o;
    streamer.m_pos->Relocate(x, y, z, o);

    return buf;
}

void MovementInfo::OutDebug()
{
    sLog->outInfo(LOG_FILTER_NETWORKIO, "MOVEMENT INFO");
    sLog->outInfo(LOG_FILTER_NETWORKIO, "guid " UI64FMTD, guid);
    sLog->outInfo(LOG_FILTER_NETWORKIO, "flags %u", flags);
    sLog->outInfo(LOG_FILTER_NETWORKIO, "flags2 %u", flags2);
    sLog->outInfo(LOG_FILTER_NETWORKIO, "time %u current time " UI64FMTD "", flags2, uint64(::time(NULL)));
    sLog->outInfo(LOG_FILTER_NETWORKIO, "position: `%s`", pos.ToString().c_str());
    if (transport.guid)
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "TRANSPORT:");
        sLog->outInfo(LOG_FILTER_NETWORKIO, "guid: " UI64FMTD, transport.guid);
        sLog->outInfo(LOG_FILTER_NETWORKIO, "position: `%s`", transport.pos.ToString().c_str());
        sLog->outInfo(LOG_FILTER_NETWORKIO, "seat: %i", transport.seat);
        sLog->outInfo(LOG_FILTER_NETWORKIO, "time: %u", transport.time);
        if (flags2 & MOVEMENTFLAG2_INTERPOLATED_MOVEMENT)
            sLog->outInfo(LOG_FILTER_NETWORKIO, "time2: %u", transport.prevTime);
    }

    if ((flags & (MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_FLYING)) || (flags2 & MOVEMENTFLAG2_ALWAYS_ALLOW_PITCHING))
        sLog->outInfo(LOG_FILTER_NETWORKIO, "pitch: %f", pitch);

    sLog->outInfo(LOG_FILTER_NETWORKIO, "fallTime: %u", fallTime);
    if (flags & MOVEMENTFLAG_FALLING)
        sLog->outInfo(LOG_FILTER_NETWORKIO, "jump.zspeed: %f jump.sinAngle: %f jump.cosAngle: %f jump.xyspeed: %f", jump.zspeed, jump.sinAngle, jump.cosAngle, jump.xyspeed);

    if (flags & MOVEMENTFLAG_SPLINE_ELEVATION)
        sLog->outInfo(LOG_FILTER_NETWORKIO, "splineElevation: %f", splineElevation);
}

WorldObject::WorldObject(bool isWorldObject): WorldLocation(),
m_name(""), m_isActive(false), m_isWorldObject(isWorldObject), m_zoneScript(NULL),
m_transport(NULL), m_currMap(NULL), m_InstanceId(0),
m_phaseMask(PHASEMASK_NORMAL), m_ignorePhaseIdCheck(false)
{
    m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_ALIVE | GHOST_VISIBILITY_GHOST);
    m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_ALIVE);
}

void WorldObject::SetWorldObject(bool on)
{
    if (!IsInWorld())
        return;

    GetMap()->AddObjectToSwitchList(this, on);
}

bool WorldObject::IsWorldObject() const
{
    if (m_isWorldObject)
        return true;

    if (ToCreature() && ToCreature()->m_isTempWorldObject)
        return true;

    return false;
}

void WorldObject::setActive(bool on)
{
    if (m_isActive == on)
        return;

    if (GetTypeId() == TYPEID_PLAYER)
        return;

    m_isActive = on;

    if (!IsInWorld())
        return;

    Map* map = FindMap();
    if (!map)
        return;

    if (on)
    {
        if (GetTypeId() == TYPEID_UNIT)
            map->AddToActive(this->ToCreature());
        else if (GetTypeId() == TYPEID_DYNAMICOBJECT)
            map->AddToActive((DynamicObject*)this);
        else if (GetTypeId() == TYPEID_GAMEOBJECT)
            map->AddToActive((GameObject*)this);
    }
    else
    {
        if (GetTypeId() == TYPEID_UNIT)
            map->RemoveFromActive(this->ToCreature());
        else if (GetTypeId() == TYPEID_DYNAMICOBJECT)
            map->RemoveFromActive((DynamicObject*)this);
        else if (GetTypeId() == TYPEID_GAMEOBJECT)
            map->RemoveFromActive((GameObject*)this);
    }
}

void WorldObject::CleanupsBeforeDelete(bool /*finalCleanup*/)
{
    if (IsInWorld())
        RemoveFromWorld();
}

uint32 WorldObject::GetZoneId() const
{
    return GetBaseMap()->GetZoneId(m_positionX, m_positionY, m_positionZ);
}

uint32 WorldObject::GetPZoneId() const
{
    return GetParentZoneOrSelf(GetZoneId());
}

uint32 WorldObject::GetAreaId() const
{
    return GetBaseMap()->GetAreaId(m_positionX, m_positionY, m_positionZ);
}

void WorldObject::GetZoneAndAreaId(uint32& zoneid, uint32& areaid) const
{
    GetBaseMap()->GetZoneAndAreaId(zoneid, areaid, m_positionX, m_positionY, m_positionZ);
}

InstanceScript* WorldObject::GetInstanceScript()
{
    Map* map = GetMap();
    return map->IsDungeon() ? ((InstanceMap*)map)->GetInstanceScript() : NULL;
}

float WorldObject::GetDistanceZ(const WorldObject* obj) const
{
    float dz = fabs(GetPositionZ() - obj->GetPositionZ());
    float sizefactor = GetObjectSize() + obj->GetObjectSize();
    float dist = dz - sizefactor;
    return (dist > 0 ? dist : 0);
}

bool WorldObject::_IsWithinDist(WorldObject const* obj, float dist2compare, bool is3D) const
{
    float sizefactor = GetObjectSize() + obj->GetObjectSize();
    float maxdist = dist2compare + sizefactor;

    if (m_transport && obj->GetTransport() &&  obj->GetTransport()->GetGUIDLow() == m_transport->GetGUIDLow())
    {
        float dtx = m_movementInfo.transport.pos.m_positionX - obj->m_movementInfo.transport.pos.m_positionX;
        float dty = m_movementInfo.transport.pos.m_positionY - obj->m_movementInfo.transport.pos.m_positionY;
        float disttsq = dtx * dtx + dty * dty;
        if (is3D)
        {
            float dtz = m_movementInfo.transport.pos.m_positionZ - obj->m_movementInfo.transport.pos.m_positionZ;
            disttsq += dtz * dtz;
        }
        return disttsq < (maxdist * maxdist);
    }

    float dx = GetPositionX() - obj->GetPositionX();
    float dy = GetPositionY() - obj->GetPositionY();
    float distsq = dx*dx + dy*dy;
    if (is3D)
    {
        float dz = GetPositionZ() - obj->GetPositionZ();
        distsq += dz*dz;
    }

    return distsq < maxdist * maxdist;
}

bool WorldObject::IsWithinLOSInMap(const WorldObject* obj) const
{
    if (!IsInMap(obj))
        return false;

    float ox, oy, oz;
    obj->GetPosition(ox, oy, oz);
    return IsWithinLOS(ox, oy, oz);
}

bool WorldObject::IsWithinLOS(float ox, float oy, float oz) const
{
    /*float x, y, z;
    GetPosition(x, y, z);
    VMAP::IVMapManager* vMapManager = VMAP::VMapFactory::createOrGetVMapManager();
    return vMapManager->isInLineOfSight(GetMapId(), x, y, z+2.0f, ox, oy, oz+2.0f);*/
    if (IsInWorld())
        return GetMap()->isInLineOfSight(GetPositionX(), GetPositionY(), GetPositionZ()+2.f, ox, oy, oz+2.f, GetPhaseMask());

    return true;
}

bool WorldObject::GetDistanceOrder(WorldObject const* obj1, WorldObject const* obj2, bool is3D /* = true */) const
{
    float dx1 = GetPositionX() - obj1->GetPositionX();
    float dy1 = GetPositionY() - obj1->GetPositionY();
    float distsq1 = dx1*dx1 + dy1*dy1;
    if (is3D)
    {
        float dz1 = GetPositionZ() - obj1->GetPositionZ();
        distsq1 += dz1*dz1;
    }

    float dx2 = GetPositionX() - obj2->GetPositionX();
    float dy2 = GetPositionY() - obj2->GetPositionY();
    float distsq2 = dx2*dx2 + dy2*dy2;
    if (is3D)
    {
        float dz2 = GetPositionZ() - obj2->GetPositionZ();
        distsq2 += dz2*dz2;
    }

    return distsq1 < distsq2;
}

bool WorldObject::IsInRange(WorldObject const* obj, float minRange, float maxRange, bool is3D /* = true */) const
{
    float dx = GetPositionX() - obj->GetPositionX();
    float dy = GetPositionY() - obj->GetPositionY();
    float distsq = dx*dx + dy*dy;
    if (is3D)
    {
        float dz = GetPositionZ() - obj->GetPositionZ();
        distsq += dz*dz;
    }

    float sizefactor = GetObjectSize() + obj->GetObjectSize();

    // check only for real range
    if (minRange > 0.0f)
    {
        float mindist = minRange + sizefactor;
        if (distsq < mindist * mindist)
            return false;
    }

    float maxdist = maxRange + sizefactor;
    return distsq < maxdist * maxdist;
}

bool WorldObject::IsInRange2d(float x, float y, float minRange, float maxRange) const
{
    float dx = GetPositionX() - x;
    float dy = GetPositionY() - y;
    float distsq = dx*dx + dy*dy;

    float sizefactor = GetObjectSize();

    // check only for real range
    if (minRange > 0.0f)
    {
        float mindist = minRange + sizefactor;
        if (distsq < mindist * mindist)
            return false;
    }

    float maxdist = maxRange + sizefactor;
    return distsq < maxdist * maxdist;
}

bool WorldObject::IsInRange3d(float x, float y, float z, float minRange, float maxRange) const
{
    float dx = GetPositionX() - x;
    float dy = GetPositionY() - y;
    float dz = GetPositionZ() - z;
    float distsq = dx*dx + dy*dy + dz*dz;

    float sizefactor = GetObjectSize();

    // check only for real range
    if (minRange > 0.0f)
    {
        float mindist = minRange + sizefactor;
        if (distsq < mindist * mindist)
            return false;
    }

    float maxdist = maxRange + sizefactor;
    return distsq < maxdist * maxdist;
}

void Position::RelocateOffset(const Position & offset)
{
    m_positionX = GetPositionX() + (offset.GetPositionX() * std::cos(GetOrientation()) + offset.GetPositionY() * std::sin(GetOrientation() + M_PI));
    m_positionY = GetPositionY() + (offset.GetPositionY() * std::cos(GetOrientation()) + offset.GetPositionX() * std::sin(GetOrientation()));
    m_positionZ = GetPositionZ() + offset.GetPositionZ();
    SetOrientation(GetOrientation() + offset.GetOrientation());
}

void Position::GetPositionOffsetTo(const Position & endPos, Position & retOffset) const
{
    float dx = endPos.GetPositionX() - GetPositionX();
    float dy = endPos.GetPositionY() - GetPositionY();

    retOffset.m_positionX = dx * std::cos(GetOrientation()) + dy * std::sin(GetOrientation());
    retOffset.m_positionY = dy * std::cos(GetOrientation()) - dx * std::sin(GetOrientation());
    retOffset.m_positionZ = endPos.GetPositionZ() - GetPositionZ();
    retOffset.SetOrientation(endPos.GetOrientation() - GetOrientation());
}

float Position::GetAngle(const Position* obj) const
{
    if (!obj)
        return 0;

    return GetAngle(obj->GetPositionX(), obj->GetPositionY());
}

// Return angle in range 0..2*pi
float Position::GetAngle(const float x, const float y) const
{
    float dx = x - GetPositionX();
    float dy = y - GetPositionY();

    float ang = atan2(dy, dx);
    ang = (ang >= 0) ? ang : 2 * M_PI + ang;
    return ang;
}

void Position::GetSinCos(const float x, const float y, float &vsin, float &vcos) const
{
    float dx = GetPositionX() - x;
    float dy = GetPositionY() - y;

    if (fabs(dx) < 0.001f && fabs(dy) < 0.001f)
    {
        float angle = (float)rand_norm()*static_cast<float>(2*M_PI);
        vcos = std::cos(angle);
        vsin = std::sin(angle);
    }
    else
    {
        float dist = sqrt((dx*dx) + (dy*dy));
        vcos = dx / dist;
        vsin = dy / dist;
    }
}

bool Position::HasInArc(float arc, const Position* obj) const
{
    // always have self in arc
    if (obj == this)
        return true;

    // move arc to range 0.. 2*pi
    arc = NormalizeOrientation(arc);

    float angle = GetAngle(obj);
    angle -= m_orientation;

    // move angle to range -pi ... +pi
    angle = NormalizeOrientation(angle);
    if (angle > M_PI)
        angle -= 2.0f*M_PI;

    float lborder = -1 * (arc/2.0f);                        // in range -pi..0
    float rborder = (arc/2.0f);                             // in range 0..pi
    return ((angle >= lborder) && (angle <= rborder));
}

bool WorldObject::IsInBetween(const Position* obj1, const Position* obj2, float size) const
{
    if (!obj1 || !obj2)
        return false;

    float dist = GetExactDist2d(obj1->GetPositionX(), obj1->GetPositionY());

    // not using sqrt() for performance
    if ((dist * dist) >= obj1->GetExactDist2dSq(obj2->GetPositionX(), obj2->GetPositionY()))
        return false;

    if (!size)
        size = GetObjectSize() / 2;

    float angle = obj1->GetAngle(obj2);

    // not using sqrt() for performance
    return (size * size) >= GetExactDist2dSq(obj1->GetPositionX() + cos(angle) * dist, obj1->GetPositionY() + sin(angle) * dist);
}

bool WorldObject::IsInBetweenShift(const Position* obj1, const Position* obj2, float size, float shift, float angleShift) const
{
    if (!obj1 || !obj2)
        return false;

    angleShift += obj1->GetOrientation();
    float destx = obj1->GetPositionX() + shift * std::cos(angleShift);
    float desty = obj1->GetPositionY() + shift * std::sin(angleShift);

    float dist = GetExactDist2d(destx, desty);

    // not using sqrt() for performance
    if ((dist * dist) >= obj1->GetExactDist2dSq(obj2->GetPositionX(), obj2->GetPositionY()))
        return false;

    if (!size)
        size = GetObjectSize() / 2;

    float angle = obj1->GetAngle(obj2);

    // not using sqrt() for performance
    return (size * size) >= GetExactDist2dSq(destx + cos(angle) * dist, desty + sin(angle) * dist);
}

bool WorldObject::IsInBetween(const WorldObject* obj1, float x2, float y2, float size) const
{
    if (!obj1)
        return false;

    float dist = GetExactDist2d(obj1->GetPositionX(), obj1->GetPositionY());

    // not using sqrt() for performance
    if ((dist * dist) >= obj1->GetExactDist2dSq(x2, y2))
        return false;

    if (!size)
        size = GetObjectSize() / 2;

    float angle = obj1->GetAngle(x2, y2);

    // not using sqrt() for performance
    return (size * size) >= GetExactDist2dSq(obj1->GetPositionX() + std::cos(angle) * dist, obj1->GetPositionY() + std::sin(angle) * dist);
}

bool WorldObject::isInFront(WorldObject const* target,  float arc) const
{
    return HasInArc(arc, target);
}

bool WorldObject::isInBack(WorldObject const* target, float arc) const
{
    return !HasInArc(2 * M_PI - arc, target);
}

void WorldObject::GetRandomPoint(const Position &pos, float distance, float &rand_x, float &rand_y, float &rand_z) const
{
    if (!distance)
    {
        pos.GetPosition(rand_x, rand_y, rand_z);
        return;
    }

    // angle to face `obj` to `this`
    float angle = (float)rand_norm()*static_cast<float>(2*M_PI);
    float new_dist = (float)rand_norm()*static_cast<float>(distance);

    rand_x = pos.m_positionX + new_dist * std::cos(angle);
    rand_y = pos.m_positionY + new_dist * std::sin(angle);
    rand_z = pos.m_positionZ;

    Trinity::NormalizeMapCoord(rand_x);
    Trinity::NormalizeMapCoord(rand_y);
    UpdateGroundPositionZ(rand_x, rand_y, rand_z);            // update to LOS height if available
}

void WorldObject::UpdateGroundPositionZ(float x, float y, float &z) const
{
    float new_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true);
    if (new_z > INVALID_HEIGHT)
        z = new_z+ 0.05f;                                   // just to be sure that we are not a few pixel under the surface
}

void WorldObject::UpdateAllowedPositionZ(float x, float y, float &z) const
{
    switch (GetTypeId())
    {
        case TYPEID_UNIT:
        {
            Unit* victim = ToCreature()->getVictim();
            if (victim)
            {
                // anyway creature move to victim for thinly Z distance (shun some VMAP wrong ground calculating)
                if (fabs(GetPositionZ() - victim->GetPositionZ()) < 5.0f)
                    return;
            }
            // non fly unit don't must be in air
            // non swim unit must be at ground (mostly speedup, because it don't must be in water and water level check less fast
            if (!ToCreature()->CanFly())
            {
                bool canSwim = ToCreature()->CanSwim();
                float ground_z = z;
                float max_z = canSwim
                    ? GetBaseMap()->GetWaterOrGroundLevel(x, y, z, &ground_z, !ToUnit()->HasAuraType(SPELL_AURA_WATER_WALK))
                    : ((ground_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true)));
                if (max_z > INVALID_HEIGHT)
                {
                    if (z > max_z)
                        z = max_z;
                    else if (z < ground_z)
                        z = ground_z;
                }
            }
            else
            {
                float ground_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true);
                if (z < ground_z)
                    z = ground_z;
            }
            break;
        }
        case TYPEID_PLAYER:
        {
            // for server controlled moves playr work same as creature (but it can always swim)
            if (!ToPlayer()->CanFly())
            {
                float ground_z = z;
                float max_z = GetBaseMap()->GetWaterOrGroundLevel(x, y, z, &ground_z, !ToUnit()->HasAuraType(SPELL_AURA_WATER_WALK));
                if (max_z > INVALID_HEIGHT)
                {
                    if (z > max_z)
                        z = max_z;
                    else if (z < ground_z)
                        z = ground_z;
                }
            }
            else
            {
                float ground_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true);
                if (z < ground_z)
                    z = ground_z;
            }
            break;
        }
        default:
        {
            float ground_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true);
            if (ground_z > INVALID_HEIGHT)
                z = ground_z;
            break;
        }
    }
}

bool Position::IsPositionValid() const
{
    return Trinity::IsValidMapCoord(m_positionX, m_positionY, m_positionZ, m_orientation);
}

float WorldObject::GetGridActivationRange() const
{
    if (Player const* thisPlayer = ToPlayer())
        return GetMap()->GetVisibilityRange(thisPlayer->getCurrentUpdateZoneID(), thisPlayer->getCurrentUpdateAreaID());
    else if (ToCreature())
        return ToCreature()->m_SightDistance;
    else
        return 0.0f;
}

float WorldObject::GetVisibilityRange() const
{
    if (isActiveObject() && !ToPlayer())
        return MAX_VISIBILITY_DISTANCE;
    else
        if (GetMap())
        {
            if (Player const* thisPlayer = ToPlayer())
                return GetMap()->GetVisibilityRange(thisPlayer->getCurrentUpdateZoneID(), thisPlayer->getCurrentUpdateAreaID());
            else if (Creature const* creature = ToCreature())
                return GetMap()->GetVisibilityRange(creature->getCurrentUpdateZoneID(), creature->getCurrentUpdateAreaID());
            else
                return GetMap()->GetVisibilityRange();
        }

    return MAX_VISIBILITY_DISTANCE;
}

float WorldObject::GetSightRange(const WorldObject* target) const
{
    if (ToUnit())
    {
        if (Player const* thisPlayer = ToPlayer())
        {
            if (target && target->isActiveObject() && !target->ToPlayer())
                return MAX_VISIBILITY_DISTANCE;
            else
                return GetMap()->GetVisibilityRange(thisPlayer->getCurrentUpdateZoneID(), thisPlayer->getCurrentUpdateAreaID());
        }
        else if (ToCreature())
            return ToCreature()->m_SightDistance;
        else
            return SIGHT_RANGE_UNIT;
    }

    return 0.0f;
}

void WorldObject::SetVisible(bool x)
{
    if (!x)
        m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, SEC_GAMEMASTER);
    else
        m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, SEC_PLAYER);

    UpdateObjectVisibility();
}

bool WorldObject::canSeeOrDetect(WorldObject const* obj, bool ignoreStealth, bool distanceCheck) const
{
    if (this == obj)
        return true;

    if (GetGUID().IsPlayer())
    {
        if (obj->MustBeVisibleOnlyForSomePlayers())
        {
            Player const* thisPlayer = ToPlayer();

            if (!thisPlayer)
                return false;

            Group const* group = thisPlayer->GetGroup();

            if (!obj->IsInPersonnalVisibilityList(thisPlayer->GetGUID()) &&
                (!group || !obj->IsInPersonnalVisibilityList(group->GetGUID())))
                return false;
        }

        if (!obj->HideForSomePlayers())
        {
            if (obj->ShouldHideFor(GetGUID()))
                return false;
        }
    }

    if (GetGUID().IsPlayer() && obj->GetGUID().IsGameObject())
    {
        Player const* thisPlayer = ToPlayer();
        if (thisPlayer && thisPlayer->IsPlayerLootCooldown(obj->GetEntry()))
            return false;
    }

    if (obj->IsNeverVisible() || CanNeverSee(obj))
        return false;

    if (obj->IsAlwaysVisibleFor(this) || CanAlwaysSee(obj))
        return true;

    bool corpseVisibility = false;
    if (distanceCheck)
    {
        bool corpseCheck = false;
        bool onArena = false;   //on arena we have always see all

        if (Player const* thisPlayer = ToPlayer())
        {
            if (thisPlayer->HaveExtraLook(obj->GetGUID()))
                return true;

            //not see befor enter vehicle.
            if (Creature const* creature = obj->ToCreature())
                if (creature->onVehicleAccessoryInit())
                    return false;

            onArena = thisPlayer->InArena();

            if (thisPlayer->isDead() && thisPlayer->GetHealth() > 0 && // Cheap way to check for ghost state
                !(obj->m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GHOST) & m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GHOST) & GHOST_VISIBILITY_GHOST))
            {
                if (Corpse* corpse = thisPlayer->GetCorpse())
                {
                    corpseCheck = true;
                    if (corpse->IsWithinDist(thisPlayer, GetSightRange(obj), false))
                        if (corpse->IsWithinDist(obj, GetSightRange(obj), false))
                            corpseVisibility = true;
                }
            }
        }

        WorldObject const* viewpoint = this;
        if (Player const* player = this->ToPlayer())
            viewpoint = player->GetViewpoint();

        if (!viewpoint)
            viewpoint = this;
        
        if (!corpseCheck && !onArena && !viewpoint->IsWithinDist(obj, GetSightRange(obj), false))
            return false;
    }

    // GM visibility off or hidden NPC
    if (!obj->m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GM))
    {
        // Stop checking other things for GMs
        if (m_serverSideVisibilityDetect.GetValue(SERVERSIDE_VISIBILITY_GM))
            return true;
    }
    else
        return m_serverSideVisibilityDetect.GetValue(SERVERSIDE_VISIBILITY_GM) >= obj->m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GM);

    // Ghost players, Spirit Healers, and some other NPCs
    if (!corpseVisibility && !(obj->m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GHOST) & m_serverSideVisibilityDetect.GetValue(SERVERSIDE_VISIBILITY_GHOST)))
    {
        // Alive players can see dead players in some cases, but other objects can't do that
        if (Player const* thisPlayer = ToPlayer())
        {
            if (Player const* objPlayer = obj->ToPlayer())
            {
                if (thisPlayer->GetTeam() != objPlayer->GetTeam() || !thisPlayer->IsGroupVisibleFor(objPlayer))
                    return false;
            }
            else
                return false;
        }
        else
            return false;
    }

    if (obj->IsInvisibleDueToDespawn())
        return false;

    if (!CanDetect(obj, ignoreStealth))
        return false;

    return true;
}

bool WorldObject::CanDetect(WorldObject const* obj, bool ignoreStealth) const
{
    const WorldObject* seer = this;

    // Pets don't have detection, they use the detection of their masters
    if (const Unit* thisUnit = ToUnit())
        if (Unit* controller = thisUnit->GetCharmerOrOwner())
            seer = controller;

    if (obj->IsAlwaysDetectableFor(seer))
        return true;

    if (!ignoreStealth && !seer->CanDetectInvisibilityOf(obj))
        return false;

    if (!ignoreStealth && !seer->CanDetectStealthOf(obj))
        return false;

    return true;
}

bool WorldObject::CanDetectInvisibilityOf(WorldObject const* obj) const
{
    uint32 mask = obj->m_invisibility.GetFlags() & m_invisibilityDetect.GetFlags();

    // Check for not detected types
    if (mask != obj->m_invisibility.GetFlags())
        return false;

    if (obj->ToUnit())
        if ((m_invisibility.GetFlags() & obj->m_invisibilityDetect.GetFlags()) != m_invisibility.GetFlags())
        {
            if (obj->m_invisibility.GetFlags() != 0 || !isType(TYPEMASK_UNIT) || !ToUnit()->HasAuraType(SPELL_AURA_SEE_WHILE_INVISIBLE))
                return false;
        }

    for (uint32 i = 0; i < TOTAL_INVISIBILITY_TYPES; ++i)
    {
        if (!(mask & (1 << i)))
            continue;

        int32 objInvisibilityValue = obj->m_invisibility.GetValue(InvisibilityType(i));
        int32 ownInvisibilityDetectValue = m_invisibilityDetect.GetValue(InvisibilityType(i));

        // Too low value to detect
        if (ownInvisibilityDetectValue < objInvisibilityValue)
            return false;
    }

    return true;
}

bool WorldObject::CanDetectStealthOf(WorldObject const* obj) const
{
    // Combat reach is the minimal distance (both in front and behind),
    //   and it is also used in the range calculation.
    // One stealth point increases the visibility range by 0.3 yard.

    if (!obj->m_stealth.GetFlags())
        return true;

    float distance = GetExactDist(obj);
    float combatReach = 0.0f;

    if (isType(TYPEMASK_UNIT))
        combatReach = ((Unit*)this)->GetCombatReach();

//     if (distance < combatReach)
//         return true;
// 
//     if (Player const* player = ToPlayer())
//         if(player->HaveAtClient(obj) && distance < (ATTACK_DISTANCE * 2))
//             return true;

    if (!HasInArc(M_PI, obj))
        return false;

    for (uint32 i = 0; i < TOTAL_STEALTH_TYPES; ++i)
    {
        if (!(obj->m_stealth.GetFlags() & (1 << i)))
            continue;

        if (isType(TYPEMASK_UNIT))
            if (((Unit*)this)->HasAuraTypeWithMiscvalue(SPELL_AURA_DETECT_STEALTH, i))
                return true;

        // Starting points
        int32 detectionValue = 30;

        // Level difference: 5 point / level, starting from level 1.
        // There may be spells for this and the starting points too, but
        // not in the DBCs of the client.
        detectionValue += int32(getLevelForTarget(obj) - 1) * 5;

        // Apply modifiers
        detectionValue += m_stealthDetect.GetValue(StealthType(i));
        if (obj->isType(TYPEMASK_GAMEOBJECT))
            if (Unit* owner = ((GameObject*)obj)->GetOwner())
                detectionValue -= int32(owner->getLevelForTarget(this) - 1) * 5;

        detectionValue -= obj->m_stealth.GetValue(StealthType(i));

        // Calculate max distance
        float visibilityRange = float(detectionValue) * 0.3f + combatReach;

        if (visibilityRange > MAX_PLAYER_STEALTH_DETECT_RANGE)
            visibilityRange = MAX_PLAYER_STEALTH_DETECT_RANGE;

        if (distance > visibilityRange)
            return false;
    }

    return true;
}

bool WorldObject::IsInPersonnalVisibilityList(ObjectGuid const& guid) const
{
    return _visibilityPlayerList.find(guid) != _visibilityPlayerList.end();
}

void WorldObject::AddPlayersInPersonnalVisibilityList(GuidUnorderedSet const& viewerList)
{
    for (auto guid : viewerList)
    {
        if (!guid.IsPlayer())
            continue;

        _visibilityPlayerList.insert(guid);
    }
}

void WorldObject::SendPlaySound(uint32 soundKitID, bool OnlySelf)
{
    WorldPackets::Misc::PlaySound  sound;
    sound.SoundKitID = soundKitID;
    sound.SourceObjectGuid = GetGUID();

    if (OnlySelf && GetTypeId() == TYPEID_PLAYER)
        this->ToPlayer()->GetSession()->SendPacket(sound.Write());
    else
        SendMessageToSet(sound.Write(), true); // ToSelf ignored in this case
}

void Object::ForceValuesUpdateAtIndex(uint32 i)
{
    _changedFields[i] = true;
    if (m_inWorld && !m_objectUpdated)
    {
        sObjectAccessor->AddUpdateObject(this);
        m_objectUpdated = true;
    }
}

namespace Trinity
{
    class MonsterChatBuilder
    {
        public:
            MonsterChatBuilder(WorldObject const& obj, ChatMsg msgtype, int32 textId, uint32 language, ObjectGuid targetGUID)
                : i_object(obj), i_msgtype(msgtype), i_textId(textId), i_language(language), i_targetGUID(targetGUID) {}
            void operator()(WorldPacket& data, LocaleConstant loc_idx)
            {
                char const* text = sObjectMgr->GetTrinityString(i_textId, loc_idx);

                // TODO: i_object.GetName() also must be localized?
                i_object.BuildMonsterChat(&data, i_msgtype, text, i_language, i_object.GetNameForLocaleIdx(loc_idx), i_targetGUID);
            }

        private:
            WorldObject const& i_object;
            ChatMsg i_msgtype;
            int32 i_textId;
            uint32 i_language;
            ObjectGuid i_targetGUID;
    };

    class MonsterCustomChatBuilder
    {
        public:
            MonsterCustomChatBuilder(WorldObject const& obj, ChatMsg msgtype, const char* text, uint32 language, ObjectGuid targetGUID)
                : i_object(obj), i_msgtype(msgtype), i_text(text), i_language(language), i_targetGUID(targetGUID) {}
            void operator()(WorldPacket& data, LocaleConstant loc_idx)
            {
                // TODO: i_object.GetName() also must be localized?
                i_object.BuildMonsterChat(&data, i_msgtype, i_text, i_language, i_object.GetNameForLocaleIdx(loc_idx), i_targetGUID);
            }

        private:
            WorldObject const& i_object;
            ChatMsg i_msgtype;
            const char* i_text;
            uint32 i_language;
            ObjectGuid i_targetGUID;
    };
}                                                           // namespace Trinity

void WorldObject::MonsterSay(const char* text, uint32 language, ObjectGuid TargetGuid)
{
    CellCoord p = Trinity::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    Trinity::MonsterCustomChatBuilder say_build(*this, CHAT_MSG_MONSTER_SAY, text, language, TargetGuid);
    Trinity::LocalizedPacketDo<Trinity::MonsterCustomChatBuilder> say_do(say_build);
    Trinity::PlayerDistWorker<Trinity::LocalizedPacketDo<Trinity::MonsterCustomChatBuilder> > say_worker(this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY), say_do);
    TypeContainerVisitor<Trinity::PlayerDistWorker<Trinity::LocalizedPacketDo<Trinity::MonsterCustomChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY));
}

void WorldObject::MonsterSay(int32 textId, uint32 language, ObjectGuid TargetGuid)
{
    CellCoord p = Trinity::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    Trinity::MonsterChatBuilder say_build(*this, CHAT_MSG_MONSTER_SAY, textId, language, TargetGuid);
    Trinity::LocalizedPacketDo<Trinity::MonsterChatBuilder> say_do(say_build);
    Trinity::PlayerDistWorker<Trinity::LocalizedPacketDo<Trinity::MonsterChatBuilder> > say_worker(this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY), say_do);
    TypeContainerVisitor<Trinity::PlayerDistWorker<Trinity::LocalizedPacketDo<Trinity::MonsterChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY));
}

void WorldObject::MonsterYell(const char* text, uint32 language, ObjectGuid TargetGuid)
{
    CellCoord p = Trinity::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    Trinity::MonsterCustomChatBuilder say_build(*this, CHAT_MSG_MONSTER_YELL, text, language, TargetGuid);
    Trinity::LocalizedPacketDo<Trinity::MonsterCustomChatBuilder> say_do(say_build);
    Trinity::PlayerDistWorker<Trinity::LocalizedPacketDo<Trinity::MonsterCustomChatBuilder> > say_worker(this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_YELL), say_do);
    TypeContainerVisitor<Trinity::PlayerDistWorker<Trinity::LocalizedPacketDo<Trinity::MonsterCustomChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_YELL));
}

void WorldObject::MonsterYell(int32 textId, uint32 language, ObjectGuid TargetGuid)
{
    CellCoord p = Trinity::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    Trinity::MonsterChatBuilder say_build(*this, CHAT_MSG_MONSTER_YELL, textId, language, TargetGuid);
    Trinity::LocalizedPacketDo<Trinity::MonsterChatBuilder> say_do(say_build);
    Trinity::PlayerDistWorker<Trinity::LocalizedPacketDo<Trinity::MonsterChatBuilder> > say_worker(this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_YELL), say_do);
    TypeContainerVisitor<Trinity::PlayerDistWorker<Trinity::LocalizedPacketDo<Trinity::MonsterChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_YELL));
}

void WorldObject::MonsterYellToZone(int32 textId, uint32 language, ObjectGuid TargetGuid)
{
    Trinity::MonsterChatBuilder say_build(*this, CHAT_MSG_MONSTER_YELL, textId, language, TargetGuid);
    Trinity::LocalizedPacketDo<Trinity::MonsterChatBuilder> say_do(say_build);

    uint32 zoneid = GetZoneId();

    Map::PlayerList const& pList = GetMap()->GetPlayers();
    for (Map::PlayerList::const_iterator itr = pList.begin(); itr != pList.end(); ++itr)
        if (itr->getSource()->GetZoneId() == zoneid)
            say_do(itr->getSource());
}

void WorldObject::MonsterTextEmote(const char* text, ObjectGuid TargetGuid, bool IsBossEmote)
{
    WorldPacket data(SMSG_CHAT, 200);
    BuildMonsterChat(&data, IsBossEmote ? CHAT_MSG_RAID_BOSS_EMOTE : CHAT_MSG_MONSTER_EMOTE, text, LANG_UNIVERSAL, GetName(), TargetGuid);
    SendMessageToSetInRange(&data, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_TEXTEMOTE), true);
}

void WorldObject::MonsterTextEmote(int32 textId, ObjectGuid TargetGuid, bool IsBossEmote)
{
    CellCoord p = Trinity::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    Trinity::MonsterChatBuilder say_build(*this, IsBossEmote ? CHAT_MSG_RAID_BOSS_EMOTE : CHAT_MSG_MONSTER_EMOTE, textId, LANG_UNIVERSAL, TargetGuid);
    Trinity::LocalizedPacketDo<Trinity::MonsterChatBuilder> say_do(say_build);
    Trinity::PlayerDistWorker<Trinity::LocalizedPacketDo<Trinity::MonsterChatBuilder> > say_worker(this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_TEXTEMOTE), say_do);
    TypeContainerVisitor<Trinity::PlayerDistWorker<Trinity::LocalizedPacketDo<Trinity::MonsterChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_TEXTEMOTE));
}

void WorldObject::MonsterWhisper(const char* text, ObjectGuid receiver, bool IsBossWhisper)
{
    Player* player = ObjectAccessor::FindPlayer(receiver);
    if (!player || !player->GetSession())
        return;

    LocaleConstant loc_idx = player->GetSession()->GetSessionDbLocaleIndex();

    WorldPacket data(SMSG_CHAT, 200);
    BuildMonsterChat(&data, IsBossWhisper ? CHAT_MSG_RAID_BOSS_WHISPER : CHAT_MSG_MONSTER_WHISPER, text, LANG_UNIVERSAL, GetNameForLocaleIdx(loc_idx), receiver);

    player->GetSession()->SendPacket(&data);
}

void WorldObject::MonsterWhisper(int32 textId, ObjectGuid receiver, bool IsBossWhisper)
{
    Player* player = ObjectAccessor::FindPlayer(receiver);
    if (!player || !player->GetSession())
        return;

    LocaleConstant loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
    char const* text = sObjectMgr->GetTrinityString(textId, loc_idx);

    WorldPacket data(SMSG_CHAT, 200);
    BuildMonsterChat(&data, IsBossWhisper ? CHAT_MSG_RAID_BOSS_WHISPER : CHAT_MSG_MONSTER_WHISPER, text, LANG_UNIVERSAL, GetNameForLocaleIdx(loc_idx), receiver);

    player->GetSession()->SendPacket(&data);
}

void WorldObject::BuildMonsterChat(WorldPacket* data, uint8 msgtype, char const* text, uint32 language, char const* name, ObjectGuid targetGuid) const
{
    Trinity::ChatData c;
    c.sourceGuid = GetGUID();
    c.targetGuid = targetGuid;
    c.message = text;
    c.sourceName = name;
    c.language = language;
    c.chatType = msgtype;

    Trinity::BuildChatPacket(*data, c);
}

void WorldObject::SendMessageToSet(WorldPacket const* data, bool self)
{
    if (IsInWorld())
        SendMessageToSetInRange(data, GetVisibilityRange(), self);
}

void WorldObject::SendMessageToSetInRange(WorldPacket const* data, float dist, bool /*self*/)
{
    Trinity::MessageDistDeliverer notifier(this, data, dist);
    VisitNearbyWorldObject(dist, notifier);
}

void WorldObject::SendMessageToSet(WorldPacket const* data, Player const* skipped_rcvr)
{
    Trinity::MessageDistDeliverer notifier(this, data, GetVisibilityRange(), false, skipped_rcvr);
    VisitNearbyWorldObject(GetVisibilityRange(), notifier);
}

void WorldObject::SendObjectDeSpawnAnim(ObjectGuid guid)
{
    SendMessageToSet(WorldPackets::GameObject::GameObjectDespawn(guid).Write(), true);
}

void WorldObject::SetMap(Map* map)
{
    ASSERT(map);
    ASSERT(!IsInWorld() || GetTypeId() == TYPEID_CORPSE);
    if (m_currMap == map) // command add npc: first create, than loadfromdb
        return;
    if (m_currMap)
    {
        sLog->outFatal(LOG_FILTER_GENERAL, "WorldObject::SetMap: obj %u new map %u %u, old map %u %u", (uint32)GetTypeId(), map->GetId(), map->GetInstanceId(), m_currMap->GetId(), m_currMap->GetInstanceId());
        ASSERT(false);
    }
    m_currMap = map;
    m_mapId = map->GetId();
    m_InstanceId = map->GetInstanceId();
    if (IsWorldObject())
        m_currMap->AddWorldObject(this);
}

void WorldObject::ResetMap()
{
    ASSERT(m_currMap);
    ASSERT(!IsInWorld());
    if (IsWorldObject())
        m_currMap->RemoveWorldObject(this);
    m_currMap = NULL;
    //maybe not for corpse
    //m_mapId = 0;
    //m_InstanceId = 0;
}

Map const* WorldObject::GetBaseMap() const
{
    ASSERT(m_currMap);
    return m_currMap->GetParent();
}

void WorldObject::AddObjectToRemoveList()
{
    ASSERT(m_uint32Values);

    Map* map = FindMap();
    if (!map)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Object (TypeId: %u Entry: %u GUID: %u) at attempt add to move list not have valid map (Id: %u).", GetTypeId(), GetEntry(), GetGUIDLow(), GetMapId());
        return;
    }

    map->AddObjectToRemoveList(this);
}

TempSummon* Map::SummonCreature(uint32 entry, Position const& pos, SummonPropertiesEntry const* properties /*= NULL*/, uint32 duration /*= 0*/, Unit* summoner /*= NULL*/, ObjectGuid targetGuid /*= 0*/, uint32 spellId /*= 0*/, int32 vehId /*= 0*/, ObjectGuid viewerGuid /*= 0*/, GuidUnorderedSet* viewersList /*= NULL*/)
{
    if(summoner)
    {
        std::list<Creature*> creatures;
        summoner->GetAliveCreatureListWithEntryInGrid(creatures, entry, 110.0f);
        if(creatures.size() > (GetInstanceId() ? 100 : 50))
            return NULL;
    }

    uint32 mask = UNIT_MASK_SUMMON;
    if (properties)
    {
        switch (properties->Category)
        {
            case SUMMON_CATEGORY_PET:
                mask = UNIT_MASK_GUARDIAN;
                break;
            case SUMMON_CATEGORY_PUPPET:
                mask = UNIT_MASK_PUPPET;
                break;
            case SUMMON_CATEGORY_VEHICLE:
                if (properties->Id == 3384) //hardfix despawn npc 63872
                    mask = UNIT_MASK_SUMMON;
                else
                    mask = UNIT_MASK_MINION;
                break;
            case SUMMON_CATEGORY_WILD:
            case SUMMON_CATEGORY_ALLY:
            case SUMMON_CATEGORY_UNK:
            {
                switch (properties->Type)
                {
                    case SUMMON_TYPE_MINION:
                    case SUMMON_TYPE_GUARDIAN:
                    case SUMMON_TYPE_GUARDIAN2:
                    case SUMMON_TYPE_OBJECT:
                        mask = UNIT_MASK_GUARDIAN;
                        break;
                    case SUMMON_TYPE_TOTEM:
                    case SUMMON_TYPE_BANNER:
                    case SUMMON_TYPE_STATUE:
                        mask = UNIT_MASK_TOTEM;
                        break;
                    case SUMMON_TYPE_VEHICLE:
                    case SUMMON_TYPE_VEHICLE2:
                    case SUMMON_TYPE_GATE:
                        mask = UNIT_MASK_SUMMON;
                        break;
                    case SUMMON_TYPE_MINIPET:
                        mask = UNIT_MASK_MINION;
                        break;
                    default:
                    {
                        if (properties->Flags & 512 ||
                            properties->Id == 2921 ||
                            properties->Id == 3459 ||
                            properties->Id == 3097) // Mirror Image, Summon Gargoyle
                            mask = UNIT_MASK_GUARDIAN;
                            break;
                    }
                }
                break;
            }
            default:
                return NULL;
        }
    }

    uint32 phase = PHASEMASK_NORMAL;
    uint32 team = 0;
    std::set<uint32> phaseIds;

    if (summoner)
    {
        phase = summoner->GetPhaseMask();
        phaseIds = summoner->GetPhases();
        if (summoner->GetTypeId() == TYPEID_PLAYER)
            team = summoner->ToPlayer()->GetTeam();
    }

    TempSummon* summon = NULL;
    switch (mask)
    {
        case UNIT_MASK_SUMMON:
            summon = new TempSummon(properties, summoner, false);
            break;
        case UNIT_MASK_GUARDIAN:
            summon = new Guardian(properties, summoner, false);
            break;
        case UNIT_MASK_PUPPET:
            summon = new Puppet(properties, summoner);
            break;
        case UNIT_MASK_TOTEM:
            summon = new Totem(properties, summoner);
            break;
        case UNIT_MASK_MINION:
            summon = new Minion(properties, summoner, false);
            break;
        default:
            return NULL;
    }

    if (!summon->Create(sObjectMgr->GetGenerator<HighGuid::Creature>()->Generate(), this, phase, entry, vehId, team, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation()))
    {
        delete summon;
        return NULL;
    }

    summon->SetPhaseId(phaseIds, false);

    summon->SetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL, spellId);
    if (summoner)
        summon->SetGuidValue(UNIT_FIELD_DEMON_CREATOR, summoner->GetGUID());

    summon->SetTargetGUID(targetGuid);

    summon->SetHomePosition(pos);

    summon->InitStats(duration);

    if (!viewerGuid.IsEmpty())
        summon->AddPlayerInPersonnalVisibilityList(viewerGuid);

    if (viewersList)
        summon->AddPlayersInPersonnalVisibilityList(*viewersList);

    AddToMap(summon->ToCreature());
    summon->InitSummon();
    summon->CastPetAuras(true);

    //sLog->outDebug(LOG_FILTER_PETS, "Map::SummonCreature summoner %u entry %i mask %i", summoner ? summoner->GetGUID() : 0, entry, mask);

    //ObjectAccessor::UpdateObjectVisibility(summon);

    return summon;
}

/**
* Summons group of creatures.
*
* @param group Id of group to summon.
* @param list  List to store pointers to summoned creatures.
*/

void Map::SummonCreatureGroup(uint8 group, std::list<TempSummon*>* list /*= NULL*/)
{
    std::vector<TempSummonData> const* data = sObjectMgr->GetSummonGroup(GetId(), SUMMONER_TYPE_MAP, group);
    if (!data)
        return;

    for (std::vector<TempSummonData>::const_iterator itr = data->begin(); itr != data->end(); ++itr)
        if (TempSummon* summon = SummonCreature(itr->entry, itr->pos, NULL, itr->time))
            if (list)
                list->push_back(summon);
}

void WorldObject::SetZoneScript()
{
    if (Map* map = FindMap())
    {
        if (map->IsDungeon() || map->IsGarrison())
            m_zoneScript = (ZoneScript*)((InstanceMap*)map)->GetInstanceScript();
        else if (!map->IsBattlegroundOrArena())
        {
            if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(GetZoneId()))
                m_zoneScript = bf;
            else
            {
                if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(GetZoneId()))
                    m_zoneScript = bf;
                else
                    m_zoneScript = sOutdoorPvPMgr->GetZoneScript(GetZoneId());
            }
        }
    }
}

TempSummon* WorldObject::SummonCreature(uint32 entry, const Position &pos, ObjectGuid targetGuid, TempSummonType spwtype, uint32 duration, uint32 spellId /*= 0*/, SummonPropertiesEntry const* properties /*= NULL*/) const
{
    if (Map* map = FindMap())
    {
        if(!ToUnit())
        {
            std::list<Creature*> creatures;
            GetAliveCreatureListWithEntryInGrid(creatures, entry, 110.0f);
            if(creatures.size() > (map->GetInstanceId() ? 100 : 50))
                return NULL;
        }
        if (TempSummon* summon = map->SummonCreature(entry, pos, properties, duration, isType(TYPEMASK_UNIT) ? (Unit*)this : NULL, targetGuid, spellId))
        {
            summon->SetTempSummonType(spwtype);
            return summon;
        }
    }

    return NULL;
}

TempSummon* WorldObject::SummonCreature(uint32 entry, const Position &pos, TempSummonType spwtype, uint32 duration, int32 vehId, ObjectGuid viewerGuid, GuidUnorderedSet* viewersList) const
{
    if (Map* map = FindMap())
    {
        if(!ToUnit())
        {
            std::list<Creature*> creatures;
            GetAliveCreatureListWithEntryInGrid(creatures, entry, 110.0f);
            if(creatures.size() > (map->GetInstanceId() ? 100 : 50))
                return NULL;
        }
        if (TempSummon* summon = map->SummonCreature(entry, pos, NULL, duration, isType(TYPEMASK_UNIT) ? (Unit*)this : NULL, ObjectGuid::Empty, 0, vehId, viewerGuid, viewersList))
        {
            summon->SetTempSummonType(spwtype);
            return summon;
        }
    }

    return NULL;
}

Pet* Player::SummonPet(uint32 entry, float x, float y, float z, float ang, PetType petType, uint32 duration, uint32 spellId, bool stampeded)
{
    if (getClass() == CLASS_HUNTER)
        petType = HUNTER_PET;

    Pet* pet = new Pet(this, petType);

    pet->Relocate(x, y, z, ang);

    //summoned pets always non-curent!
    if (petType == SUMMON_PET || petType == HUNTER_PET)
    {
        // This check done in LoadPetFromDB, but we should not continue this function if pet not alowed
        if (!CanSummonPet(entry))
        {
            delete pet;
            return NULL;
        }

        if (pet->LoadPetFromDB(this, entry, 0, stampeded))
        {
            if (duration > 0)
                pet->SetDuration(duration);

            return pet;
        }
    }

    // petentry == 0 for hunter "call pet" (current pet summoned if any)
    if (!entry)
    {
        sLog->outError(LOG_FILTER_PETS, "no such entry %u", entry);
        delete pet;
        return NULL;
    }

    if (!pet->IsPositionValid())
    {
        sLog->outError(LOG_FILTER_PETS, "Pet (guidlow %d, entry %d) not summoned. Suggested coordinates isn't valid (X: %f Y: %f)", pet->GetGUIDLow(), pet->GetEntry(), pet->GetPositionX(), pet->GetPositionY());
        delete pet;
        return NULL;
    }

    Map* map = GetMap();
    uint32 pet_number = sObjectMgr->GeneratePetNumber();
    if (!pet->Create(sObjectMgr->GetGenerator<HighGuid::Pet>()->Generate(), map, GetPhaseMask(), entry, pet_number))
    {
        sLog->outError(LOG_FILTER_PETS, "no such creature entry %u", entry);
        delete pet;
        return NULL;
    }

    pet->SetCreatorGUID(GetGUID());
    pet->SetUInt32Value(UNIT_FIELD_FACTION_TEMPLATE, getFaction());
    pet->SetUInt32Value(UNIT_FIELD_NPC_FLAGS, 0);
    pet->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
    pet->SetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL, spellId);
    pet->InitStatsForLevel(getLevel());

    if(petType == SUMMON_PET)
        pet->GetCharmInfo()->SetPetNumber(pet_number, true);

    // After SetPetNumber
    SetMinion(pet, true, stampeded);

    map->AddToMap(pet->ToCreature());

    if(petType == SUMMON_PET)
    {
        pet->InitPetCreateSpells();
        pet->SynchronizeLevelWithOwner();
        pet->LearnPetPassives();
        pet->SavePetToDB();
        PetSpellInitialize();
        SendTalentsInfoData(true);
    }

    if (getClass() == CLASS_WARLOCK)
        if (HasAura(108503))
            RemoveAura(108503);

    if (pet->isControlled() && !pet->isTemporarySummoned())
        SetLastPetEntry(entry);

    if (duration > 0)
        pet->SetDuration(duration);

    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "SummonPet entry %i, petType %i, spellId %i", entry, petType, spellId);

    pet->CastPetAuras(true);
    return pet;
}

GameObject* WorldObject::SummonGameObject(uint32 entry, float x, float y, float z, float ang, float rotation0, float rotation1, float rotation2, float rotation3, uint32 respawnTime, ObjectGuid viewerGuid, GuidUnorderedSet* viewersList)
{
    if (!IsInWorld())
        return NULL;

    GameObjectTemplate const* goinfo = sObjectMgr->GetGameObjectTemplate(entry);
    if (!goinfo)
    {
        sLog->outError(LOG_FILTER_SQL, "Gameobject template %u not found in database!", entry);
        return NULL;
    }
    Map* map = GetMap();
    GameObject* go = new GameObject();
    if (!go->Create(sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate(), entry, map, GetPhaseMask(), x, y, z, ang, rotation0, rotation1, rotation2, rotation3, 100, GO_STATE_READY))
    {
        delete go;
        return NULL;
    }
    go->SetRespawnTime(respawnTime);

    // If we summon go by creature at despown - we will see deleted go.
    // If we summon go by creature with ownership in some cases we couldn't use it
    if (GetTypeId() == TYPEID_PLAYER || (GetTypeId() == TYPEID_UNIT && !respawnTime)) //not sure how to handle this
        ToUnit()->AddGameObject(go);
    else
        go->SetSpawnedByDefault(false);

    if (!viewerGuid.IsEmpty())
        go->AddPlayerInPersonnalVisibilityList(viewerGuid);

    if (viewersList)
        go->AddPlayersInPersonnalVisibilityList(*viewersList);

    map->AddToMap(go);

    return go;
}

Creature* WorldObject::SummonTrigger(float x, float y, float z, float ang, uint32 duration, CreatureAI* (*GetAI)(Creature*))
{
    TempSummonType summonType = (duration == 0) ? TEMPSUMMON_DEAD_DESPAWN : TEMPSUMMON_TIMED_DESPAWN;
    Creature* summon = SummonCreature(WORLD_TRIGGER, x, y, z, ang, summonType, duration);
    if (!summon)
        return NULL;

    //summon->SetName(GetName());
    if (GetTypeId() == TYPEID_PLAYER || GetTypeId() == TYPEID_UNIT)
    {
        summon->setFaction(((Unit*)this)->getFaction());
        summon->SetLevel(((Unit*)this)->getLevel());
    }

    if (GetAI)
        summon->AIM_Initialize(GetAI(summon));
    return summon;
}

/**
* Summons group of creatures. Should be called only by instances of Creature and GameObject classes.
*
* @param group Id of group to summon.
* @param list  List to store pointers to summoned creatures.
*/
void WorldObject::SummonCreatureGroup(uint8 group, std::list<TempSummon*>* list /*= NULL*/)
{
    ASSERT((GetTypeId() == TYPEID_GAMEOBJECT || GetTypeId() == TYPEID_UNIT) && "Only GOs and creatures can summon npc groups!");

    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "WorldObject:SummonCreatureGroup");

    std::vector<TempSummonData> const* data = sObjectMgr->GetSummonGroup(GetEntry(), GetTypeId() == TYPEID_GAMEOBJECT ? SUMMONER_TYPE_GAMEOBJECT : SUMMONER_TYPE_CREATURE, group);
    if (!data)
        return;

    TempSummonGroupKey groupKey = TempSummonGroupKey(GetEntry(), GetTypeId() == TYPEID_GAMEOBJECT ? SUMMONER_TYPE_GAMEOBJECT : SUMMONER_TYPE_CREATURE, group);

    //If group exist derespawn
    if(!tempSummonGroupList[groupKey].empty())
    {
        for (std::list<ObjectGuid>::const_iterator iter = tempSummonGroupList[groupKey].begin(); iter != tempSummonGroupList[groupKey].end(); ++iter)
        {
            if(Creature* temp = ObjectAccessor::GetCreature(*this, (*iter)))
                temp->DespawnOrUnsummon();
        }
        tempSummonGroupList[groupKey].clear();
    }

    for (std::vector<TempSummonData>::const_iterator itr = data->begin(); itr != data->end(); ++itr)
    {
        switch (itr->actionType)
        {
            case SUMMON_ACTION_TYPE_DEFAULT:
            {
                if (TempSummon* summon = SummonCreature(itr->entry, itr->pos, itr->sumType, itr->time))
                {
                    if (list)
                        list->push_back(summon);

                    tempSummonGroupList[groupKey].push_back(summon->GetGUID());
                }
                break;
            }
            case SUMMON_ACTION_TYPE_ROUND_HOME_POS:
            {
                float stepbyangle = 2*M_PI / itr->count;
                for (uint8 i = 0; i < itr->count; ++i)
                {
                    float x = 0.0f, y = 0.0f;
                    Position posHome{GetPositionX(), GetPositionY(), GetPositionZ()};
                    if(ToCreature())
                        posHome = ToCreature()->GetHomePosition();

                    x = posHome.GetPositionX() + (GetObjectSize() + itr->distance) * std::cos(stepbyangle*i);
                    y = posHome.GetPositionY() + (GetObjectSize() + itr->distance) * std::sin(stepbyangle*i);
                    Trinity::NormalizeMapCoord(x);
                    Trinity::NormalizeMapCoord(y);
                    posHome.Relocate(x, y, GetPositionZ());

                    if (TempSummon* summon = SummonCreature(itr->entry, posHome, itr->sumType, itr->time))
                    {
                        if (list)
                            list->push_back(summon);

                        tempSummonGroupList[groupKey].push_back(summon->GetGUID());
                    }
                }
                break;
            }
            case SUMMON_ACTION_TYPE_ROUND_SUMMONER:
            {
                float stepbyangle = 2*M_PI / itr->count;
                for (uint8 i = 0; i < itr->count; ++i)
                {
                    float x = 0.0f, y = 0.0f;
                    GetNearPoint2D(x, y, itr->distance, stepbyangle*i);
                    Position pos {x, y, GetPositionZ()};

                    if (TempSummon* summon = SummonCreature(itr->entry, pos, itr->sumType, itr->time))
                    {
                        if (list)
                            list->push_back(summon);

                        tempSummonGroupList[groupKey].push_back(summon->GetGUID());
                    }
                }
                break;
            }
        }
    }
}

void WorldObject::SummonCreatureGroupDespawn(uint8 group, std::list<TempSummon*>* list /*= NULL*/)
{
    if(list)
    {
        for (std::list<TempSummon*>::const_iterator iter = list->begin(); iter != list->end(); ++iter)
            if(TempSummon *const temp = *iter)
                temp->DespawnOrUnsummon();
        return;
    }

    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "WorldObject:SummonCreatureGroupDespawn");

    std::vector<TempSummonData> const* data = sObjectMgr->GetSummonGroup(GetEntry(), GetTypeId() == TYPEID_GAMEOBJECT ? SUMMONER_TYPE_GAMEOBJECT : SUMMONER_TYPE_CREATURE, group);
    if (!data)
        return;

    TempSummonGroupKey groupKey = TempSummonGroupKey(GetEntry(), GetTypeId() == TYPEID_GAMEOBJECT ? SUMMONER_TYPE_GAMEOBJECT : SUMMONER_TYPE_CREATURE, group);

    if(tempSummonGroupList[groupKey].empty())
        return;

    for (std::list<ObjectGuid>::const_iterator iter = tempSummonGroupList[groupKey].begin(); iter != tempSummonGroupList[groupKey].end(); ++iter)
    {
        if(Creature* temp = ObjectAccessor::GetCreature(*this, *iter))
            temp->DespawnOrUnsummon();
    }
    tempSummonGroupList[groupKey].clear();
}

void WorldObject::GetAttackableUnitListInRange(std::list<Unit*> &list, float fMaxSearchRange) const
{
    CellCoord p(Trinity::ComputeCellCoord(GetPositionX(), GetPositionY()));
    Cell cell(p);
    cell.SetNoCreate();

    Trinity::AnyUnitInObjectRangeCheck u_check(this, fMaxSearchRange);
    Trinity::UnitListSearcher<Trinity::AnyUnitInObjectRangeCheck> searcher(this, list, u_check);

    TypeContainerVisitor<Trinity::UnitListSearcher<Trinity::AnyUnitInObjectRangeCheck>, WorldTypeMapContainer > world_unit_searcher(searcher);
    TypeContainerVisitor<Trinity::UnitListSearcher<Trinity::AnyUnitInObjectRangeCheck>, GridTypeMapContainer >  grid_unit_searcher(searcher);

    cell.Visit(p, world_unit_searcher, *GetMap(), *this, fMaxSearchRange);
    cell.Visit(p, grid_unit_searcher, *GetMap(), *this, fMaxSearchRange);
}

void WorldObject::GetAreaTriggersWithEntryInRange(std::list<AreaTrigger*>& list, uint32 entry, ObjectGuid casterGuid, float fMaxSearchRange) const
{
    Trinity::AreaTriggerWithEntryInObjectRangeCheck checker(this, entry, casterGuid, fMaxSearchRange);
    Trinity::AreaTriggerListSearcher<Trinity::AreaTriggerWithEntryInObjectRangeCheck> searcher(this, list, checker);
    VisitNearbyObject(fMaxSearchRange, searcher);
}

Creature* WorldObject::FindNearestCreature(uint32 entry, float range, bool alive) const
{
    Creature* creature = NULL;
    Trinity::NearestCreatureEntryWithLiveStateInObjectRangeCheck checker(*this, entry, alive, range);
    Trinity::CreatureLastSearcher<Trinity::NearestCreatureEntryWithLiveStateInObjectRangeCheck> searcher(this, creature, checker);
    VisitNearbyObject(range, searcher);
    return creature;
}

GameObject* WorldObject::FindNearestGameObject(uint32 entry, float range) const
{
    GameObject* go = NULL;
    Trinity::NearestGameObjectEntryInObjectRangeCheck checker(*this, entry, range);
    Trinity::GameObjectLastSearcher<Trinity::NearestGameObjectEntryInObjectRangeCheck> searcher(this, go, checker);
    VisitNearbyGridObject(range, searcher);
    return go;
}

Player* WorldObject::FindNearestPlayer(float range, bool alive)
{
    Player* player = NULL;
    Trinity::AnyPlayerInObjectRangeCheck check(this, GetVisibilityRange());
    Trinity::PlayerSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(this, player, check);
    VisitNearbyWorldObject(range, searcher);
    return player;
}

GameObject* WorldObject::FindNearestGameObjectOfType(GameobjectTypes type, float range) const
{ 
    GameObject* go = NULL;
    Trinity::NearestGameObjectTypeInObjectRangeCheck checker(*this, type, range);
    Trinity::GameObjectLastSearcher<Trinity::NearestGameObjectTypeInObjectRangeCheck> searcher(this, go, checker);
    VisitNearbyGridObject(range, searcher);
    return go;
}

void WorldObject::GetGameObjectListWithEntryInGrid(std::list<GameObject*>& gameobjectList, uint32 entry, float maxSearchRange) const
{
    CellCoord pair(Trinity::ComputeCellCoord(this->GetPositionX(), this->GetPositionY()));
    Cell cell(pair);
    cell.SetNoCreate();

    Trinity::AllGameObjectsWithEntryInRange check(this, entry, maxSearchRange);
    Trinity::GameObjectListSearcher<Trinity::AllGameObjectsWithEntryInRange> searcher(this, gameobjectList, check);
    TypeContainerVisitor<Trinity::GameObjectListSearcher<Trinity::AllGameObjectsWithEntryInRange>, GridTypeMapContainer> visitor(searcher);

    cell.Visit(pair, visitor, *(this->GetMap()), *this, maxSearchRange);
}

void WorldObject::GetCreatureListWithEntryInGrid(std::list<Creature*>& creatureList, uint32 entry, float maxSearchRange) const
{
    CellCoord pair(Trinity::ComputeCellCoord(this->GetPositionX(), this->GetPositionY()));
    Cell cell(pair);
    cell.SetNoCreate();

    Trinity::AllCreaturesOfEntryInRange check(this, entry, maxSearchRange);
    Trinity::CreatureListSearcher<Trinity::AllCreaturesOfEntryInRange> searcher(this, creatureList, check);
    TypeContainerVisitor<Trinity::CreatureListSearcher<Trinity::AllCreaturesOfEntryInRange>, GridTypeMapContainer> visitor(searcher);

    cell.Visit(pair, visitor, *(this->GetMap()), *this, maxSearchRange);
}

void WorldObject::GetAreaTriggerListWithEntryInGrid(std::list<AreaTrigger*>& atList, uint32 entry, float maxSearchRange) const
{
    CellCoord pair(Trinity::ComputeCellCoord(this->GetPositionX(), this->GetPositionY()));
    Cell cell(pair);
    cell.SetNoCreate();

    Trinity::AllAreaTriggeresOfEntryInRange check(this, entry, maxSearchRange);
    Trinity::AreaTriggerListSearcher<Trinity::AllAreaTriggeresOfEntryInRange> searcher(this, atList, check);
    TypeContainerVisitor<Trinity::AreaTriggerListSearcher<Trinity::AllAreaTriggeresOfEntryInRange>, GridTypeMapContainer> visitor(searcher);

    cell.Visit(pair, visitor, *(this->GetMap()), *this, maxSearchRange);
}

void WorldObject::GetPlayerListInGrid(std::list<Player*>& playerList, float maxSearchRange) const
{    
    Trinity::AnyPlayerInObjectRangeCheck checker(this, maxSearchRange);
    Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(this, playerList, checker);
    this->VisitNearbyWorldObject(maxSearchRange, searcher);
}

void WorldObject::GetGameObjectListWithEntryInGridAppend(std::list<GameObject*>& gameobjectList, uint32 entry, float maxSearchRange) const
{
    std::list<GameObject*> tempList;
    GetGameObjectListWithEntryInGrid(tempList, entry, maxSearchRange);
    gameobjectList.sort();
    tempList.sort();
    gameobjectList.merge(tempList);
}

void WorldObject::GetCreatureListWithEntryInGridAppend(std::list<Creature*>& creatureList, uint32 entry, float maxSearchRange) const
{
    std::list<Creature*> tempList;
    GetCreatureListWithEntryInGrid(tempList, entry, maxSearchRange);
    creatureList.sort();
    tempList.sort();
    creatureList.merge(tempList);
}

void WorldObject::GetAliveCreatureListWithEntryInGrid(std::list<Creature*>& creatureList, uint32 entry, float maxSearchRange) const
{
    CellCoord pair(Trinity::ComputeCellCoord(this->GetPositionX(), this->GetPositionY()));
    Cell cell(pair);
    cell.SetNoCreate();

    Trinity::AllAliveCreaturesOfEntryInRange check(this, entry, maxSearchRange);
    Trinity::CreatureListSearcher<Trinity::AllAliveCreaturesOfEntryInRange> searcher(this, creatureList, check);
    TypeContainerVisitor<Trinity::CreatureListSearcher<Trinity::AllAliveCreaturesOfEntryInRange>, GridTypeMapContainer> visitor(searcher);

    cell.Visit(pair, visitor, *(this->GetMap()), *this, maxSearchRange);
}

void WorldObject::GetCorpseCreatureInGrid(std::list<Creature*>& creatureList, float maxSearchRange) const
{
    CellCoord pair(Trinity::ComputeCellCoord(this->GetPositionX(), this->GetPositionY()));
    Cell cell(pair);
    cell.SetNoCreate();

    Trinity::SearchCorpseCreatureCheck check(this, maxSearchRange);
    Trinity::CreatureListSearcher<Trinity::SearchCorpseCreatureCheck> searcher(this, creatureList, check);
    TypeContainerVisitor<Trinity::CreatureListSearcher<Trinity::SearchCorpseCreatureCheck>, GridTypeMapContainer> visitor(searcher);

    cell.Visit(pair, visitor, *(this->GetMap()), *this, maxSearchRange);
}

/*
namespace Trinity
{
    class NearUsedPosDo
    {
        public:
            NearUsedPosDo(WorldObject const& obj, WorldObject const* searcher, float angle, ObjectPosSelector& selector)
                : i_object(obj), i_searcher(searcher), i_angle(angle), i_selector(selector) {}

            void operator()(Corpse*) const {}
            void operator()(DynamicObject*) const {}

            void operator()(Creature* c) const
            {
                // skip self or target
                if (c == i_searcher || c == &i_object)
                    return;

                float x, y, z;

                if (!c->isAlive() || c->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED | UNIT_STATE_DISTRACTED) ||
                    !c->GetMotionMaster()->GetDestination(x, y, z))
                {
                    x = c->GetPositionX();
                    y = c->GetPositionY();
                }

                add(c, x, y);
            }

            template<class T>
                void operator()(T* u) const
            {
                // skip self or target
                if (u == i_searcher || u == &i_object)
                    return;

                float x, y;

                x = u->GetPositionX();
                y = u->GetPositionY();

                add(u, x, y);
            }

            // we must add used pos that can fill places around center
            void add(WorldObject* u, float x, float y) const
            {
                // u is too nearest/far away to i_object
                if (!i_object.IsInRange2d(x, y, i_selector.m_dist - i_selector.m_size, i_selector.m_dist + i_selector.m_size))
                    return;

                float angle = i_object.GetAngle(u)-i_angle;

                // move angle to range -pi ... +pi
                while (angle > M_PI)
                    angle -= 2.0f * M_PI;
                while (angle < -M_PI)
                    angle += 2.0f * M_PI;

                // dist include size of u
                float dist2d = i_object.GetDistance2d(x, y);
                i_selector.AddUsedPos(u->GetObjectSize(), angle, dist2d + i_object.GetObjectSize());
            }
        private:
            WorldObject const& i_object;
            WorldObject const* i_searcher;
            float              i_angle;
            ObjectPosSelector& i_selector;
    };
}                                                           // namespace Trinity
*/

//===================================================================================================

void WorldObject::GetNearPoint2D(float &x, float &y, float distance2d, float absAngle) const
{
    x = GetPositionX() + (GetObjectSize() + distance2d) * std::cos(absAngle);
    y = GetPositionY() + (GetObjectSize() + distance2d) * std::sin(absAngle);

    Trinity::NormalizeMapCoord(x);
    Trinity::NormalizeMapCoord(y);
}

void WorldObject::GetNearPoint(WorldObject const* searcher, float &x, float &y, float &z, float searcher_size, float distance2d, float absAngle) const
{
    GetNearPoint2D(x, y, distance2d+searcher_size, absAngle);
    z = GetPositionZ();
    if (!searcher || !searcher->ToCreature() || !searcher->GetMap()->Instanceable())
        UpdateAllowedPositionZ(x, y, z);
    /*
    // if detection disabled, return first point
    if (!sWorld->getIntConfig(CONFIG_DETECT_POS_COLLISION))
    {
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available
        return;
    }

    // or remember first point
    float first_x = x;
    float first_y = y;
    bool first_los_conflict = false;                        // first point LOS problems

    // prepare selector for work
    ObjectPosSelector selector(GetPositionX(), GetPositionY(), GetObjectSize(), distance2d+searcher_size);

    // adding used positions around object
    {
        CellCoord p(Trinity::ComputeCellCoord(GetPositionX(), GetPositionY()));
        Cell cell(p);
        cell.SetNoCreate();

        Trinity::NearUsedPosDo u_do(*this, searcher, absAngle, selector);
        Trinity::WorldObjectWorker<Trinity::NearUsedPosDo> worker(this, u_do);

        TypeContainerVisitor<Trinity::WorldObjectWorker<Trinity::NearUsedPosDo>, GridTypeMapContainer  > grid_obj_worker(worker);
        TypeContainerVisitor<Trinity::WorldObjectWorker<Trinity::NearUsedPosDo>, WorldTypeMapContainer > world_obj_worker(worker);

        CellLock<GridReadGuard> cell_lock(cell, p);
        cell_lock->Visit(cell_lock, grid_obj_worker,  *GetMap(), *this, distance2d);
        cell_lock->Visit(cell_lock, world_obj_worker, *GetMap(), *this, distance2d);
    }

    // maybe can just place in primary position
    if (selector.CheckOriginal())
    {
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available

        if (IsWithinLOS(x, y, z))
            return;

        first_los_conflict = true;                          // first point have LOS problems
    }

    float angle;                                            // candidate of angle for free pos

    // special case when one from list empty and then empty side preferred
    if (selector.FirstAngle(angle))
    {
        GetNearPoint2D(x, y, distance2d, absAngle+angle);
        z = GetPositionZ();
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available

        if (IsWithinLOS(x, y, z))
            return;
    }

    // set first used pos in lists
    selector.InitializeAngle();

    // select in positions after current nodes (selection one by one)
    while (selector.NextAngle(angle))                        // angle for free pos
    {
        GetNearPoint2D(x, y, distance2d, absAngle+angle);
        z = GetPositionZ();
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available

        if (IsWithinLOS(x, y, z))
            return;
    }

    // BAD NEWS: not free pos (or used or have LOS problems)
    // Attempt find _used_ pos without LOS problem

    if (!first_los_conflict)
    {
        x = first_x;
        y = first_y;

        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available
        return;
    }

    // special case when one from list empty and then empty side preferred
    if (selector.IsNonBalanced())
    {
        if (!selector.FirstAngle(angle))                     // _used_ pos
        {
            GetNearPoint2D(x, y, distance2d, absAngle+angle);
            z = GetPositionZ();
            UpdateGroundPositionZ(x, y, z);                   // update to LOS height if available

            if (IsWithinLOS(x, y, z))
                return;
        }
    }

    // set first used pos in lists
    selector.InitializeAngle();

    // select in positions after current nodes (selection one by one)
    while (selector.NextUsedAngle(angle))                    // angle for used pos but maybe without LOS problem
    {
        GetNearPoint2D(x, y, distance2d, absAngle+angle);
        z = GetPositionZ();
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available

        if (IsWithinLOS(x, y, z))
            return;
    }

    // BAD BAD NEWS: all found pos (free and used) have LOS problem :(
    x = first_x;
    y = first_y;

    UpdateGroundPositionZ(x, y, z);                           // update to LOS height if available
    */
}

void WorldObject::MovePosition(Position &pos, float dist, float angle)
{
    angle += GetOrientation();
    float destx, desty, destz, ground, floor;
    destx = pos.m_positionX + dist * std::cos(angle);
    desty = pos.m_positionY + dist * std::sin(angle);

    // Prevent invalid coordinates here, position is unchanged
    if (!Trinity::IsValidMapCoord(destx, desty))
    {
        sLog->outFatal(LOG_FILTER_GENERAL, "WorldObject::MovePosition invalid coordinates X: %f and Y: %f were passed!", destx, desty);
        return;
    }

    ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
    floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
    destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;

    float step = dist/10.0f;

    for (uint8 j = 0; j < 10; ++j)
    {
        // do not allow too big z changes
        if (fabs(pos.m_positionZ - destz) > 6)
        {
            destx -= step * std::cos(angle);
            desty -= step * std::sin(angle);
            ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
            floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
            destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;
        }
        // we have correct destz now
        else
        {
            pos.Relocate(destx, desty, destz);
            break;
        }
    }

    Trinity::NormalizeMapCoord(pos.m_positionX);
    Trinity::NormalizeMapCoord(pos.m_positionY);
    UpdateGroundPositionZ(pos.m_positionX, pos.m_positionY, pos.m_positionZ);
    pos.SetOrientation(GetOrientation());
}

void WorldObject::MovePositionToFirstCollision(Position &pos, float dist, float angle)
{
    angle += GetOrientation();
    float destx, desty, destz, ground, floor;
    pos.m_positionZ += 2.0f;
    destx = pos.m_positionX + dist * std::cos(angle);
    desty = pos.m_positionY + dist * std::sin(angle);

    // Prevent invalid coordinates here, position is unchanged
    if (!Trinity::IsValidMapCoord(destx, desty))
    {
        sLog->outFatal(LOG_FILTER_GENERAL, "WorldObject::MovePositionToFirstCollision invalid coordinates X: %f and Y: %f were passed!", destx, desty);
        return;
    }

    ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
    floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
    destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;

    bool col = VMAP::VMapFactory::createOrGetVMapManager()->getObjectHitPos(GetMapId(), pos.m_positionX, pos.m_positionY, pos.m_positionZ+0.5f, destx, desty, destz+0.5f, destx, desty, destz, -0.5f);

    // collision occurred
    if (col)
    {
        // move back a bit
        destx -= CONTACT_DISTANCE * std::cos(angle);
        desty -= CONTACT_DISTANCE * std::sin(angle);
        dist = sqrt((pos.m_positionX - destx)*(pos.m_positionX - destx) + (pos.m_positionY - desty)*(pos.m_positionY - desty));
    }

    // check dynamic collision
    col = GetMap()->getObjectHitPos(GetPhaseMask(), pos.m_positionX, pos.m_positionY, pos.m_positionZ+0.5f, destx, desty, destz+0.5f, destx, desty, destz, -0.5f);

    // Collided with a gameobject
    if (col)
    {
        destx -= CONTACT_DISTANCE * std::cos(angle);
        desty -= CONTACT_DISTANCE * std::sin(angle);
        dist = sqrt((pos.m_positionX - destx)*(pos.m_positionX - destx) + (pos.m_positionY - desty)*(pos.m_positionY - desty));
    }

    float step = dist/10.0f;

    for (uint8 j = 0; j < 10; ++j)
    {
        // do not allow too big z changes
        if (fabs(pos.m_positionZ - destz) > 6)
        {
            destx -= step * std::cos(angle);
            desty -= step * std::sin(angle);
            ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
            floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
            destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;
        }
        // we have correct destz now
        else
        {
            pos.Relocate(destx, desty, destz);
            break;
        }
    }

    Trinity::NormalizeMapCoord(pos.m_positionX);
    Trinity::NormalizeMapCoord(pos.m_positionY);
    UpdateAllowedPositionZ(pos.m_positionX, pos.m_positionY, pos.m_positionZ);
    pos.SetOrientation(GetOrientation());
}

void WorldObject::MovePositionToCollisionBetween(Position &pos, float distMin, float distMax, float angle)
{
    angle += GetOrientation();
    float destx, desty, destz, tempDestx, tempDesty, ground, floor;
    pos.m_positionZ += 2.0f;

    tempDestx = pos.m_positionX + distMin * std::cos(angle);
    tempDesty = pos.m_positionY + distMin * std::sin(angle);

    destx = pos.m_positionX + distMax * std::cos(angle);
    desty = pos.m_positionY + distMax * std::sin(angle);

    // Prevent invalid coordinates here, position is unchanged
    if (!Trinity::IsValidMapCoord(destx, desty))
    {
        sLog->outFatal(LOG_FILTER_GENERAL, "WorldObject::MovePositionToFirstCollision invalid coordinates X: %f and Y: %f were passed!", destx, desty);
        return;
    }

    ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
    floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
    destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;

    bool col = VMAP::VMapFactory::createOrGetVMapManager()->getObjectHitPos(GetMapId(), tempDestx, tempDesty, pos.m_positionZ+0.5f, destx, desty, destz+0.5f, destx, desty, destz, -0.5f);

    // collision occurred
    if (col)
    {
        // move back a bit
        destx -= CONTACT_DISTANCE * std::cos(angle);
        desty -= CONTACT_DISTANCE * std::sin(angle);
        distMax = sqrt((pos.m_positionX - destx)*(pos.m_positionX - destx) + (pos.m_positionY - desty)*(pos.m_positionY - desty));
    }

    // check dynamic collision
    col = GetMap()->getObjectHitPos(GetPhaseMask(), tempDestx, tempDesty, pos.m_positionZ+0.5f, destx, desty, destz+0.5f, destx, desty, destz, -0.5f);

    // Collided with a gameobject
    if (col)
    {
        destx -= CONTACT_DISTANCE * std::cos(angle);
        desty -= CONTACT_DISTANCE * std::sin(angle);
        distMax = sqrt((pos.m_positionX - destx)*(pos.m_positionX - destx) + (pos.m_positionY - desty)*(pos.m_positionY - desty));
    }

    float step = distMax/10.0f;

    for (uint8 j = 0; j < 10; ++j)
    {
        // do not allow too big z changes
        if (fabs(pos.m_positionZ - destz) > 6)
        {
            destx -= step * std::cos(angle);
            desty -= step * std::sin(angle);
            ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
            floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
            destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;
        }
        // we have correct destz now
        else
        {
            pos.Relocate(destx, desty, destz);
            break;
        }
    }

    Trinity::NormalizeMapCoord(pos.m_positionX);
    Trinity::NormalizeMapCoord(pos.m_positionY);
    UpdateAllowedPositionZ(pos.m_positionX, pos.m_positionY, pos.m_positionZ);
    pos.SetOrientation(GetOrientation());
}

void WorldObject::SetPhaseMask(uint32 newPhaseMask, bool update)
{
    m_phaseMask = newPhaseMask;

    if (update && IsInWorld())
        UpdateObjectVisibility();
}

void WorldObject::PlayDistanceSound(uint32 soundID, Player* target /*= nullptr*/)
{
    WorldPackets::GameObject::PlayObjectSound objectSound;
    objectSound.SourceObjectGUID = GetGUID();
    if (target)
        objectSound.TargetObjectGUID = target->GetGUID();
    objectSound.Pos = GetPosition();
    objectSound.SoundId = soundID;

    if (target)
        target->SendDirectMessage(objectSound.Write());
    else
        SendMessageToSet(objectSound.Write(), true);
}

void WorldObject::PlayDirectSound(uint32 soundKitID, Player* target /*= NULL*/)
{
    WorldPackets::Misc::PlaySound  sound;
    sound.SoundKitID = soundKitID;
    sound.SourceObjectGuid = GetGUID();

    if (target)
        target->SendDirectMessage(sound.Write());
    else
        SendMessageToSet(sound.Write(), true);
}

void WorldObject::DestroyForNearbyPlayers()
{
    if (!IsInWorld())
        return;

    std::list<Player*> targets;
    Trinity::AnyPlayerInObjectRangeCheck check(this, GetVisibilityRange(), false);
    Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(this, targets, check);
    VisitNearbyWorldObject(GetVisibilityRange(), searcher);
    for (std::list<Player*>::const_iterator iter = targets.begin(); iter != targets.end(); ++iter)
    {
        Player* player = (*iter);

        if (player == this)
            continue;

        if (!player->HaveAtClient(this))
            continue;

        if (isType(TYPEMASK_UNIT) && ((Unit*)this)->GetCharmerGUID() == player->GetGUID()) // TODO: this is for puppet
            continue;

        DestroyForPlayer(player);
        player->m_clientGUIDs.erase(GetGUID());
    }
}

void WorldObject::DestroyVignetteForNearbyPlayers()
{
    if (!IsInWorld() || !GetVignetteId())
        return;

    std::list<Player*> targets;
    Trinity::AnyPlayerInObjectRangeCheck check(this, GetVisibilityRange(), false);
    Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(this, targets, check);
    VisitNearbyWorldObject(GetVisibilityRange(), searcher);
    for (std::list<Player*>::const_iterator iter = targets.begin(); iter != targets.end(); ++iter)
    {
        Player* player = (*iter);

        if (player == this)
            continue;

        if (!player->HaveAtClient(this))
            continue;

        if (isType(TYPEMASK_UNIT) && ((Unit*)this)->GetCharmerGUID() == player->GetGUID()) // TODO: this is for puppet
            continue;

        player->RemoveVignette(this, true);
    }
}

void WorldObject::UpdateObjectVisibility(bool /*forced*/)
{
    //updates object's visibility for nearby players
    Trinity::VisibleChangesNotifier notifier(*this);
    VisitNearbyWorldObject(GetVisibilityRange(), notifier);
}

struct WorldObjectChangeAccumulator
{
    UpdateDataMapType& i_updateDatas;
    WorldObject& i_object;
    GuidSet plr_list;
    WorldObjectChangeAccumulator(WorldObject &obj, UpdateDataMapType &d) : i_updateDatas(d), i_object(obj) {}
    void Visit(PlayerMapType &m)
    {
        Player* source = NULL;
        for (PlayerMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
        {
            source = iter->getSource();

            BuildPacket(source);

            if (!source->GetSharedVisionList().empty())
            {
                SharedVisionList::const_iterator it = source->GetSharedVisionList().begin();
                for (; it != source->GetSharedVisionList().end(); ++it)
                    BuildPacket(*it);
            }
        }
    }

    void Visit(CreatureMapType &m)
    {
        Creature* source = NULL;
        for (CreatureMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
        {
            source = iter->getSource();
            if (!source->GetSharedVisionList().empty())
            {
                SharedVisionList::const_iterator it = source->GetSharedVisionList().begin();
                for (; it != source->GetSharedVisionList().end(); ++it)
                    BuildPacket(*it);
            }
        }
    }

    void Visit(DynamicObjectMapType &m)
    {
        DynamicObject* source = NULL;
        for (DynamicObjectMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
        {
            source = iter->getSource();
            ObjectGuid guid = source->GetCasterGUID();

            if (guid.IsPlayer())
            {
                //Caster may be NULL if DynObj is in removelist
                if (Player* caster = ObjectAccessor::FindPlayer(guid))
                    if (caster->GetGuidValue(PLAYER_FIELD_FARSIGHT_OBJECT) == source->GetGUID())
                        BuildPacket(caster);
            }
        }
    }

    void BuildPacket(Player* player)
    {
        // Only send update once to a player
        if (plr_list.find(player->GetGUID()) == plr_list.end() && player->HaveAtClient(&i_object))
        {
            i_object.BuildFieldsUpdate(player, i_updateDatas);
            plr_list.insert(player->GetGUID());
        }
    }

    template<class SKIP> void Visit(GridRefManager<SKIP> &) {}
};

void WorldObject::BuildUpdate(UpdateDataMapType& data_map)
{
    CellCoord p = Trinity::ComputeCellCoord(GetPositionX(), GetPositionY());
    Cell cell(p);
    cell.SetNoCreate();
    WorldObjectChangeAccumulator notifier(*this, data_map);
    TypeContainerVisitor<WorldObjectChangeAccumulator, WorldTypeMapContainer > player_notifier(notifier);
    Map& map = *GetMap();
    //we must build packets for all visible players
    cell.Visit(p, player_notifier, map, *this, GetVisibilityRange());

    ClearUpdateMask(false);
}

ObjectGuid WorldObject::GetTransGUID() const
{
    if (GetTransport())
        return GetTransport()->GetGUID();
    return ObjectGuid::Empty;
}

//! if someone has phaseID but enother has empty - not see any! YES! NOT SEE! FIX2. WHY SHOULD? 
//      FOR SUPPORT OLD STYLE NEED ALLOW TO SEE. FOR SUPER HIDE PHASE ALL SHOULD HAVE SOME PHASEIDs
//! If some have 1 2 enother has 1 = see each other.
//! ir some have 1 2 enorther has 3 - not see.
//! if some has ignorePhase id - see each.
bool WorldObject::InSamePhaseId(std::set<uint32> const& phase) const
{
    if (IgnorePhaseId())
        return true;

    //- speed up case.
    if (phase.empty() || m_phaseId.empty())
        return true;

    //! speed up case. should be done in any way. 
    // As iteration not check empty data but it should be done.
    if (phase.empty() && !m_phaseId.empty() ||
        !phase.empty() && m_phaseId.empty())
        return false;

    //! check target phases
    for (auto PhaseID : phase)
    {
        //- if not found
        if (m_phaseId.find(PhaseID) == m_phaseId.end())
        {
            //- lets check our phases
            for (auto _phase : m_phaseId)
            {
                //- if not foud
                if (phase.find(_phase) == phase.end())
                    return false;   //fail
            }
            //- else - one of targets has all phases - so it can see eatch other.
            return true;
        }
    }
    return true;
}

void WorldObject::SetAIAnimKitId(uint16 animKitID)
{
    if (_aiAnimKitId == animKitID)
        return;

    _aiAnimKitId = animKitID;

    if (!IsInWorld())
        return;

    WorldPackets::Misc::SetAIAnimKit aiAnimKit;
    aiAnimKit.AnimKitID = animKitID;
    aiAnimKit.Unit = GetGUID();
    SendMessageToSet(aiAnimKit.Write(), true);
}

void WorldObject::SetMovementAnimKitId(uint16 animKitID)
{
    if (_movementAnimKitId == animKitID)
        return;

    _movementAnimKitId = animKitID;

    if (!IsInWorld())
        return;

    WorldPackets::Misc::SetMovementAnimKit movementAnimKit;
    movementAnimKit.AnimKitID = animKitID;
    movementAnimKit.Unit = GetGUID();
    SendMessageToSet(movementAnimKit.Write(), true);
}

void WorldObject::SetMeleeAnimKitId(uint16 animKitID)
{
    if (_meleeAnimKitId == animKitID)
        return;

    _meleeAnimKitId = animKitID;

    if (!IsInWorld())
        return;

    WorldPackets::Misc::SetMeleeAnimKit meleeAnimKit;
    meleeAnimKit.AnimKitID = animKitID;
    meleeAnimKit.Unit = GetGUID();
    SendMessageToSet(meleeAnimKit.Write(), true);
}

C_PTR Object::get_ptr()
{
    if (ptr.numerator && ptr.numerator->ready)
        return ptr.shared_from_this();

    ptr.InitParent(this);
    ASSERT(ptr.numerator);  // It's very bad. If it hit nothing work.
    return ptr.shared_from_this();
}

void WorldObject::RebuildTerrainSwaps()
{
    // Clear all terrain swaps, will be rebuilt below
    // Reason for this is, multiple phases can have the same terrain swap, we should not remove the swap if another phase still use it
    _terrainSwaps.clear();
    /*ConditionList conditions;

    // Check all applied phases for terrain swap and add it only once
    for (uint32 phaseId : _phases)
    {
        std::list<uint32>& swaps = sObjectMgr->GetPhaseTerrainSwaps(phaseId);

        for (uint32 swap : swaps)
        {
            // only add terrain swaps for current map
            MapEntry const* mapEntry = sMapStore.LookupEntry(swap);
            if (!mapEntry || mapEntry->ParentMapID != int32(GetMapId()))
                continue;

            conditions = sConditionMgr->GetConditionsForNotGroupedEntry(CONDITION_SOURCE_TYPE_TERRAIN_SWAP, swap);

            if (sConditionMgr->IsObjectMeetToConditions(this, conditions))
                _terrainSwaps.insert(swap);
        }
    }

    // get default terrain swaps, only for current map always
    std::list<uint32>& mapSwaps = sObjectMgr->GetDefaultTerrainSwaps(GetMapId());

    for (uint32 swap : mapSwaps)
    {
        conditions = sConditionMgr->GetConditionsForNotGroupedEntry(CONDITION_SOURCE_TYPE_TERRAIN_SWAP, swap);

        if (sConditionMgr->IsObjectMeetToConditions(this, conditions))
            _terrainSwaps.insert(swap);
    }*/

    // online players have a game client with world map display
    if (GetTypeId() == TYPEID_PLAYER)
        RebuildWorldMapAreaSwaps();
}

void WorldObject::RebuildWorldMapAreaSwaps()
{
    // Clear all world map area swaps, will be rebuilt below
    _worldMapAreaSwaps.clear();

    // get ALL default terrain swaps, if we are using it (condition is true)
    // send the worldmaparea for it, to see swapped worldmaparea in client from other maps too, not just from our current
    /*TerrainPhaseInfo defaults = sObjectMgr->GetDefaultTerrainSwapStore();
    for (TerrainPhaseInfo::const_iterator itr = defaults.begin(); itr != defaults.end(); ++itr)
    {
        for (uint32 swap : itr->second)
        {
            ConditionList conditions = sConditionMgr->GetConditionsForNotGroupedEntry(CONDITION_SOURCE_TYPE_TERRAIN_SWAP, swap);
            if (sConditionMgr->IsObjectMeetToConditions(this, conditions))
            {
                for (uint32 map : sObjectMgr->GetTerrainWorldMaps(swap))
                    _worldMapAreaSwaps.insert(map);
            }
        }
    }

    // Check all applied phases for world map area swaps
    for (uint32 phaseId : _phases)
    {
        std::list<uint32>& swaps = sObjectMgr->GetPhaseTerrainSwaps(phaseId);

        for (uint32 swap : swaps)
        {
            // add world map swaps for ANY map

            ConditionList conditions = sConditionMgr->GetConditionsForNotGroupedEntry(CONDITION_SOURCE_TYPE_TERRAIN_SWAP, swap);

            if (sConditionMgr->IsObjectMeetToConditions(this, conditions))
            {
                for (uint32 map : sObjectMgr->GetTerrainWorldMaps(swap))
                    _worldMapAreaSwaps.insert(map);
            }
        }
    }*/
}
