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

#include "SpellPackets.h"
#include "SpellAuraEffects.h"
#include "MovementPackets.h"

void WorldPackets::Spells::CancelAura::Read()
{
    _worldPacket >> SpellID;
    _worldPacket >> CasterGUID;
}

WorldPacket const* WorldPackets::Spells::CategoryCooldown::Write()
{
    _worldPacket.reserve(4 + 8 * CategoryCooldowns.size());

    _worldPacket << static_cast<uint32>(CategoryCooldowns.size());

    for (CategoryCooldownInfo const& cooldown : CategoryCooldowns)
    {
        _worldPacket << uint32(cooldown.Category);
        _worldPacket << int32(cooldown.ModCooldown);
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::SendKnownSpells::Write()
{
    _worldPacket.reserve(1 + 4 * KnownSpells.size());

    _worldPacket.WriteBit(InitialLogin);
    _worldPacket << static_cast<uint32>(KnownSpells.size());

    for (uint32 const& spellId : KnownSpells)
        _worldPacket << uint32(spellId);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::UpdateActionButtons::Write()
{
    for (uint32 i = 0; i < MAX_ACTION_BUTTONS; ++i)
        _worldPacket << ActionButtons[i];

    _worldPacket << Reason;

    return &_worldPacket;
}

void WorldPackets::Spells::SetActionButton::Read()
{
    _worldPacket >> Action;
    _worldPacket >> Type;
    _worldPacket >> Index;
}

WorldPacket const* WorldPackets::Spells::SendUnlearnSpells::Write()
{
    _worldPacket << static_cast<uint32>(Spells.size());
    for (uint32 const& spellId : Spells)
        _worldPacket << uint32(spellId);

    return &_worldPacket;
}

void WorldPackets::Spells::SpellCastLogData::Initialize(Unit* unit)
{
    Health = unit->GetHealth();
    AttackPower = unit->GetTotalAttackPowerValue(unit->getClass() == CLASS_HUNTER ? RANGED_ATTACK : BASE_ATTACK);
    SpellPower = unit->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SPELL);
    PowerData.emplace_back(int32(unit->getPowerType()), unit->GetPower(unit->getPowerType()));
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellCastLogData const& spellCastLogData)
{
    data << spellCastLogData.Health;
    data << spellCastLogData.AttackPower;
    data << spellCastLogData.SpellPower;
    data << static_cast<int32>(spellCastLogData.PowerData.size());
    for (WorldPackets::Spells::SpellLogPowerData const& powerData : spellCastLogData.PowerData)
    {
        data << powerData.PowerType;
        data << powerData.Amount;
    }
    data.WriteBit(false);
    // data << float // Unk data if bit is true
    data.FlushBits();

    return data;
}

WorldPacket const* WorldPackets::Spells::AuraUpdate::Write()
{
    _worldPacket.WriteBit(UpdateAll);
    _worldPacket << UnitGUID;
    _worldPacket << static_cast<uint32>(Auras.size());
    for (auto const& aura : Auras)
    {
        _worldPacket << aura.Slot;
        if (_worldPacket.WriteBit(aura.AuraData.is_initialized()))
        {
            AuraDataInfo const& data = *aura.AuraData;
            _worldPacket << uint32(data.SpellID);
            _worldPacket << uint8(data.Flags);
            _worldPacket << uint32(data.ActiveFlags);
            _worldPacket << uint16(data.CastLevel);
            _worldPacket << uint8(data.Applications);
            _worldPacket << static_cast<uint32>(data.EstimatedPoints.size());
            _worldPacket << static_cast<uint32>(data.Points.size());

            if (!data.EstimatedPoints.empty())
                _worldPacket.append(data.EstimatedPoints.data(), data.EstimatedPoints.size());

            if (!data.Points.empty())
                _worldPacket.append(data.Points.data(), data.Points.size());

            _worldPacket.WriteBit(data.CastUnit.is_initialized());
            _worldPacket.WriteBit(data.Duration.is_initialized());
            _worldPacket.WriteBit(data.Remaining.is_initialized());

            if (data.CastUnit)
                _worldPacket << *data.CastUnit;

            if (data.Duration)
                _worldPacket << uint32(*data.Duration);

            if (data.Remaining)
                _worldPacket << uint32(*data.Remaining);
        }

        _worldPacket.FlushBits();
    }

    return &_worldPacket;
}

ByteBuffer& operator>>(ByteBuffer& buffer, WorldPackets::Spells::TargetLocation& location)
{
    buffer >> location.Transport;
    buffer >> location.Location.m_positionX;
    buffer >> location.Location.m_positionY;
    buffer >> location.Location.m_positionZ;

    return buffer;
}

ByteBuffer& operator>>(ByteBuffer& buffer, Optional<WorldPackets::Spells::TargetLocation>& location)
{
    location = boost::in_place();
    buffer >> location->Transport;
    buffer >> location->Location.m_positionX;
    buffer >> location->Location.m_positionY;
    buffer >> location->Location.m_positionZ;

    return buffer;
}

ByteBuffer& operator>>(ByteBuffer& buffer, WorldPackets::Spells::SpellTargetData& targetData)
{
    buffer.ResetBitPos();

    targetData.Flags = buffer.ReadBits(23);
    bool const hasSrcLocation = buffer.ReadBit();
    bool const hasDstLocation = buffer.ReadBit();
    bool const hasOrientation = buffer.ReadBit();
    uint32 nameLength = buffer.ReadBits(7);

    buffer >> targetData.Unit;
    buffer >> targetData.Item;

    if (hasSrcLocation)
        buffer >> targetData.SrcLocation;

    if (hasDstLocation)
        buffer >> targetData.DstLocation;

    if (hasOrientation)
        targetData.Orientation = buffer.read<float>();

    targetData.Name = buffer.ReadString(nameLength);

    return buffer;
}

ByteBuffer& operator>>(ByteBuffer& buffer, WorldPackets::Spells::MissileTrajectoryRequest& trajectory)
{
    buffer >> trajectory.Pitch;
    buffer >> trajectory.Speed;

    return buffer;
}

ByteBuffer& operator>>(ByteBuffer& buffer, WorldPackets::Spells::SpellCastRequest& request)
{
    buffer >> request.CastID;
    buffer >> request.SpellID;
    buffer >> request.Misc;
    buffer >> request.Target;
    buffer >> request.MissileTrajectory;
    buffer >> request.Charmer;

    buffer.ResetBitPos();
    request.SendCastFlags = buffer.ReadBits(5);
    bool const hasMoveUpdate = buffer.ReadBit();
    request.Weight.resize(buffer.ReadBits(2));

    if (hasMoveUpdate)
    {
        MovementInfo movementInfo;
        buffer >> movementInfo;
        request.MoveUpdate = movementInfo;
    }

    for (WorldPackets::Spells::SpellWeight& weight : request.Weight)
    {
        buffer.ResetBitPos();
        weight.Type = buffer.ReadBits(2);
        buffer >> weight.ID;
        buffer >> weight.Quantity;
    }

    return buffer;
}

void WorldPackets::Spells::CastSpell::Read()
{
    _worldPacket >> Cast;
}

void WorldPackets::Spells::PetCastSpell::Read()
{
    _worldPacket >> PetGUID;
    _worldPacket >> Cast;
}

void WorldPackets::Spells::ItemUse::Read()
{
    _worldPacket >> bagIndex;
    _worldPacket >> slot;
    _worldPacket >> itemGUID;
    _worldPacket >> Cast;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::TargetLocation const& targetLocation)
{
    data << targetLocation.Transport;
    data << targetLocation.Location.m_positionX;
    data << targetLocation.Location.m_positionY;
    data << targetLocation.Location.m_positionZ;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellTargetData const& spellTargetData)
{
    data.WriteBits(spellTargetData.Flags, 23);
    data.WriteBit(spellTargetData.SrcLocation.is_initialized());
    data.WriteBit(spellTargetData.DstLocation.is_initialized());
    data.WriteBit(spellTargetData.Orientation.is_initialized());
    data.WriteBits(spellTargetData.Name.size(), 7);
    data.FlushBits();

    data << spellTargetData.Unit;
    data << spellTargetData.Item;

    if (spellTargetData.SrcLocation)
        data << *spellTargetData.SrcLocation;

    if (spellTargetData.DstLocation)
        data << *spellTargetData.DstLocation;

    if (spellTargetData.Orientation)
        data << *spellTargetData.Orientation;

    data.WriteString(spellTargetData.Name);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellMissStatus const& spellMissStatus)
{
    data.WriteBits(spellMissStatus.Reason, 4);
    data.WriteBits(spellMissStatus.ReflectStatus, 4);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellPowerData const& spellPowerData)
{
    data << spellPowerData.Cost;
    data << spellPowerData.Type;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::RuneData const& runeData)
{
    data << runeData.Start;
    data << runeData.Count;

    data.WriteBits(runeData.Cooldowns.size(), 3);
    data.FlushBits();

    for (uint8 const& cd : runeData.Cooldowns)
        data << cd;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::MissileTrajectoryResult const& missileTrajectory)
{
    data << missileTrajectory.TravelTime;
    data << missileTrajectory.Pitch;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellAmmo const& spellAmmo)
{
    data << spellAmmo.DisplayID;
    data << spellAmmo.InventoryType;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::ProjectileVisualData const& projectileVisual)
{
    data << projectileVisual.ID[0];
    data << projectileVisual.ID[1];

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::CreatureImmunities const& immunities)
{
    data << immunities.School;
    data << immunities.Value;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellHealPrediction const& spellPred)
{
    data << spellPred.Points;
    data << spellPred.Type;
    data << spellPred.BeaconGUID;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellCastData const& spellCastData)
{
    data << spellCastData.CasterGUID;
    data << spellCastData.CasterUnit;
    data << uint8(spellCastData.CastID);
    data << int32(spellCastData.SpellID);
    data << uint32(spellCastData.CastFlags);
    data << uint32(spellCastData.CastTime);
    data << static_cast<uint32>(spellCastData.HitTargets.size());
    data << static_cast<uint32>(spellCastData.MissTargets.size());
    data << static_cast<uint32>(spellCastData.MissStatus.size());
    data << spellCastData.Target;
    data << static_cast<uint32>(spellCastData.RemainingPower.size());
    data << spellCastData.MissileTrajectory;
    data << spellCastData.Ammo;
    data << uint8(spellCastData.DestLocSpellCastIndex);
    data << static_cast<uint32>(spellCastData.TargetPoints.size());
    data << spellCastData.Immunities;
    data << spellCastData.Predict;

    for (ObjectGuid const& target : spellCastData.HitTargets)
        data << target;

    for (ObjectGuid const& target : spellCastData.MissTargets)
        data << target;

    for (WorldPackets::Spells::SpellMissStatus const& status : spellCastData.MissStatus)
        data << status;

    for (WorldPackets::Spells::SpellPowerData const& power : spellCastData.RemainingPower)
        data << power;

    for (WorldPackets::Spells::TargetLocation const& targetLoc : spellCastData.TargetPoints)
        data << targetLoc;

    data.WriteBits(spellCastData.CastFlagsEx, 18);
    data.WriteBit(spellCastData.RemainingRunes.is_initialized());
    data.WriteBit(spellCastData.ProjectileVisual.is_initialized());
    data.FlushBits();

    if (spellCastData.RemainingRunes)
        data << *spellCastData.RemainingRunes;

    if (spellCastData.ProjectileVisual)
        data << *spellCastData.ProjectileVisual;

    return data;
}

WorldPacket const* WorldPackets::Spells::SpellStart::Write()
{
    _worldPacket << Cast;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::SpellGo::Write()
{
    _worldPacket << Cast;

    _worldPacket.WriteBit(LogData.is_initialized());
    _worldPacket.FlushBits();

    if (LogData)
        _worldPacket << *LogData;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::LearnedSpells::Write()
{
    _worldPacket << static_cast<uint32>(SpellID.size());
    for (int32 const& spell : SpellID)
        _worldPacket << spell;

    _worldPacket.WriteBit(SuppressMessaging);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::SpellFailure::Write()
{
    _worldPacket << CasterUnit;
    _worldPacket << CastID;
    _worldPacket << SpellID;
    _worldPacket << Reason;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::SpellFailedOther::Write()
{
    _worldPacket << CasterUnit;
    _worldPacket << CastID;
    _worldPacket << SpellID;
    _worldPacket << Reason;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::CastFailed::Write()
{
    _worldPacket << SpellID;
    _worldPacket << Reason;
    _worldPacket << FailedArg1;
    _worldPacket << FailedArg2;
    _worldPacket << CastID;

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellModifierData const& spellModifierData)
{
    data << spellModifierData.ModifierValue;
    data << spellModifierData.ClassIndex;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellModifier const& spellModifier)
{
    data << spellModifier.ModIndex;
    data << static_cast<uint32>(spellModifier.ModifierData.size());
    for (WorldPackets::Spells::SpellModifierData const& modData : spellModifier.ModifierData)
        data << modData;

    return data;
}

WorldPacket const* WorldPackets::Spells::SetSpellModifier::Write()
{
    _worldPacket << static_cast<uint32>(Modifiers.size());
    for (WorldPackets::Spells::SpellModifier const& spellMod : Modifiers)
        _worldPacket << spellMod;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::UnlearnedSpells::Write()
{
    _worldPacket << uint32(SpellID.size());
    for (uint32 const& spellId : SpellID)
        _worldPacket << uint32(spellId);

    _worldPacket.WriteBit(SuppressMessaging);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::SupercededSpells::Write()
{
    _worldPacket << SpellCount1;
    _worldPacket << SpellCount2;
    _worldPacket << SpellID;
    _worldPacket << SupercededSpellID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::CooldownEvent::Write()
{
    _worldPacket << int32(SpellID);
    _worldPacket.WriteBit(IsPet);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::ClearCooldowns::Write()
{
    _worldPacket << static_cast<uint32>(SpellID.size());
    if (!SpellID.empty())
        _worldPacket.append(SpellID.data(), SpellID.size());

    _worldPacket.WriteBit(IsPet);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::ClearCooldown::Write()
{
    _worldPacket << uint32(SpellID);
    _worldPacket.WriteBit(ClearOnHold);
    _worldPacket.WriteBit(IsPet);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::ModifyCooldown::Write()
{
    _worldPacket << int32(SpellID);
    _worldPacket << int32(DeltaTime);
    _worldPacket.WriteBit(IsPet);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellCooldownStruct const& cooldown)
{
    data << uint32(cooldown.SrecID);
    data << uint32(cooldown.ForcedCooldown);
    return data;
}

WorldPacket const* WorldPackets::Spells::SpellCooldown::Write()
{
    _worldPacket << Caster;
    _worldPacket << uint8(Flags);
    _worldPacket << static_cast<uint32>(SpellCooldowns.size());
    for (SpellCooldownStruct const& cooldown : SpellCooldowns)
        _worldPacket << cooldown;

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellHistoryEntry const& historyEntry)
{
    data << uint32(historyEntry.SpellID);
    data << uint32(historyEntry.ItemID);
    if (historyEntry.OnHold)
    {
        data << uint32(historyEntry.Category);
        data << int32(historyEntry.RecoveryTime);
        data << int32(historyEntry.CategoryRecoveryTime);
    }
    else
    {
        data << uint32(0);
        data << int32(historyEntry.RecoveryTime);
        data << int32(0);
    }
    data.WriteBit(historyEntry.OnHold);
    data.FlushBits();

    return data;
}

WorldPacket const* WorldPackets::Spells::SendSpellHistory::Write()
{
    _worldPacket << static_cast<uint32>(Entries.size());
    for (SpellHistoryEntry const& historyEntry : Entries)
        _worldPacket << historyEntry;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::RefreshSpellHistory::Write()
{
    _worldPacket << static_cast<uint32>(Entries.size());
    for (SpellHistoryEntry const& historyEntry : Entries)
        _worldPacket << historyEntry;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::ClearAllSpellCharges::Write()
{
    _worldPacket.WriteBit(IsPet);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::ClearSpellCharges::Write()
{
    _worldPacket << int32(Category);
    _worldPacket.WriteBit(IsPet);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::SetSpellCharges::Write()
{
    _worldPacket << int32(Category);
    _worldPacket << uint32(NextRecoveryTime);
    _worldPacket << uint8(ConsumedCharges);
    _worldPacket.WriteBit(IsPet);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellChargeEntry const& chargeEntry)
{
    data << uint32(chargeEntry.Category);
    data << uint32(chargeEntry.NextRecoveryTime);
    data << uint8(chargeEntry.ConsumedCharges);

    return data;
}

WorldPacket const* WorldPackets::Spells::SendSpellCharges::Write()
{
    _worldPacket << static_cast<uint32>(Entries.size());
    for (SpellChargeEntry const& chargeEntry : Entries)
        _worldPacket << chargeEntry;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::ClearTarget::Write()
{
    _worldPacket << Guid;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::CancelOrphanSpellVisual::Write()
{
    _worldPacket << int32(SpellVisualID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::CancelSpellVisual::Write()
{
    _worldPacket << Source;
    _worldPacket << int32(SpellVisualID);

    return &_worldPacket;
}

void WorldPackets::Spells::CancelCast::Read()
{
    _worldPacket >> SpellID;
    _worldPacket >> CastID;
}

void WorldPackets::Spells::OpenItem::Read()
{
    _worldPacket >> Slot;
    _worldPacket >> PackSlot;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellChannelStartInterruptImmunities const& interruptImmunities)
{
    data << int32(interruptImmunities.SchoolImmunities);
    data << int32(interruptImmunities.Immunities);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::SpellTargetedHealPrediction const& targetedHealPrediction)
{
    data << targetedHealPrediction.TargetGUID;
    data << targetedHealPrediction.Predict;

    return data;
}

WorldPacket const* WorldPackets::Spells::SpellChannelStart::Write()
{
    _worldPacket << CasterGUID;
    _worldPacket << int32(SpellID);
    _worldPacket << uint32(ChannelDuration);
    _worldPacket.WriteBit(InterruptImmunities.is_initialized());
    _worldPacket.WriteBit(HealPrediction.is_initialized());
    _worldPacket.FlushBits();

    if (InterruptImmunities)
        _worldPacket << *InterruptImmunities;

    if (HealPrediction)
        _worldPacket << *HealPrediction;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::SpellChannelUpdate::Write()
{
    _worldPacket << CasterGUID;
    _worldPacket << int32(TimeRemaining);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::ResurrectRequest::Write()
{
    _worldPacket << ResurrectOffererGUID;
    _worldPacket << ResurrectOffererVirtualRealmAddress;

    _worldPacket << PetNumber;
    _worldPacket << SpellID;

    _worldPacket.WriteBits(Name.length(), 6);
    _worldPacket.WriteBit(UseTimer);
    _worldPacket.WriteBit(Sickness);
    _worldPacket.WriteString(Name);

    return &_worldPacket;
}

void WorldPackets::Spells::UnlearnSkill::Read()
{
    _worldPacket >> SkillLine;
}

void WorldPackets::Spells::GetMirrorImageData::Read()
{
    _worldPacket >> UnitGUID;
    _worldPacket >> DisplayID;
}

WorldPacket const* WorldPackets::Spells::MirrorImageComponentedData::Write()
{
    _worldPacket << UnitGUID;
    _worldPacket << DisplayID;
    _worldPacket << RaceID;
    _worldPacket << Gender;
    _worldPacket << ClassID;
    _worldPacket << SkinColor;
    _worldPacket << FaceVariation;
    _worldPacket << HairVariation;
    _worldPacket << HairColor;
    _worldPacket << BeardVariation;
    _worldPacket << GuildGUID;

    _worldPacket << static_cast<uint32>(ItemDisplayID.size());

    for (auto const& itemDisplayId : ItemDisplayID)
        _worldPacket << itemDisplayId;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::MirrorImageCreatureData::Write()
{
    _worldPacket << UnitGUID;
    _worldPacket << DisplayID;

    return &_worldPacket;
}

void WorldPackets::Spells::SpellClick::Read()
{
    _worldPacket >> SpellClickUnitGuid;
    TryAutoDismount = _worldPacket.ReadBit();
}

WorldPacket const* WorldPackets::Spells::ConvertRune::Write()
{
    _worldPacket << uint8(Index);
    _worldPacket << uint8(Rune);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::ResyncRunes::Write()
{
    _worldPacket << static_cast<uint32>(Runes.size());
    for (auto const& rune : Runes)
    {
        _worldPacket << uint8(rune.RuneType);
        _worldPacket << uint8(rune.Cooldown);
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::ScriptCast::Write()
{
    _worldPacket << SpellID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::PlaySpellVisual::Write()
{
    _worldPacket << Source;
    _worldPacket << Target;
    _worldPacket << TargetPosition.PositionXYZStream();
    _worldPacket << SpellVisualID;
    _worldPacket << TravelSpeed;
    _worldPacket << MissReason;
    _worldPacket << ReflectStatus;
    _worldPacket << TargetPosition.GetOrientation();
    _worldPacket.WriteBit(SpeedAsTime);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::PlaySpellVisualKit::Write()
{
    _worldPacket << Unit;
    _worldPacket << KitType;
    _worldPacket << Duration;
    _worldPacket << KitRecID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::PlayOrphanSpellVisual::Write()
{
    _worldPacket << SourceLocation.PositionXYZStream();
    _worldPacket << SourceOrientation.PositionXYZStream();
    _worldPacket << TargetLocation.PositionXYZStream();
    _worldPacket << Target;
    _worldPacket << SpellVisualID;
    _worldPacket << TravelSpeed;
    _worldPacket << UnkFloat;
    _worldPacket.WriteBit(SpeedAsTime);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::WeeklySpellUsage::Write()
{
    _worldPacket << static_cast<uint32>(SpellUsage.size());
    for (auto const& x : SpellUsage)
    {
        _worldPacket << x.Category;
        _worldPacket << x.Uses;
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::UpdateChainTargets::Write()
{
    _worldPacket << CasterGUID;
    _worldPacket << SpellID;
    _worldPacket << static_cast<uint32>(Targets.size());
    for (auto const& x : Targets)
        _worldPacket << x;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::SpellMultistrikeEffect::Write()
{
    _worldPacket << Caster;
    _worldPacket << Target;
    _worldPacket << SpellID;
    _worldPacket << ProcCount;
    _worldPacket << ProcNum;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::ResumeCastBar::Write()
{
    _worldPacket << Guid;
    _worldPacket << Target;
    _worldPacket << SpellID;
    _worldPacket << TimeRemaining;
    _worldPacket << TotalTime;
    _worldPacket.WriteBit(Immunities.is_initialized());
    if (Immunities)
        _worldPacket << *Immunities;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::NotifyMissileTrajectoryCollision::Write()
{
    _worldPacket << Caster;
    _worldPacket << CastID;
    _worldPacket << CollisionPos.PositionXYZStream();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::NotifyDestLocSpellCast::Write()
{
    _worldPacket << Caster;
    _worldPacket << DestTransport;
    _worldPacket << SpellID;
    _worldPacket << SourceLoc.PositionXYZStream();
    _worldPacket << DestLoc.PositionXYZStream();
    _worldPacket << MissileTrajectoryPitch;
    _worldPacket << MissileTrajectorySpeed;
    _worldPacket << TravelTime;
    _worldPacket << DestLocSpellCastIndex;
    _worldPacket << CastID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::LossOfControlAuraUpdate::Write()
{
    _worldPacket << static_cast<uint32>(Infos.size());
    for (auto const& x : Infos)
    {
        _worldPacket << x.AuraSlot;
        _worldPacket << x.EffectIndex;
        _worldPacket.WriteBits(x.Type, 8);
        _worldPacket.WriteBits(x.Mechanic, 8);
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::CancelSpellVisualKit::Write()
{
    _worldPacket << Source;
    _worldPacket << SpellVisualKitID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::AuraPointsDepleted::Write()
{
    _worldPacket << Unit;
    _worldPacket << Slot;
    _worldPacket << EffectIndex;

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Spells::AreaTriggerSpline& spline)
{
    data << spline.TimeToTarget;
    data << spline.ElapsedTimeForMovement;
    data << static_cast<uint32>(spline.VerticesPoints.size());
    for (auto& x : spline.VerticesPoints)
        data << x.PositionXYZStream();

    return data;
}

WorldPacket const* WorldPackets::Spells::AreaTriggerRePath::Write()
{
    _worldPacket << TriggerGUID;
    _worldPacket << Spline;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::AreaTriggerDenied::Write()
{
    _worldPacket << AreaTriggerID;
    _worldPacket.WriteBit(Entered);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::SpellDelayed::Write()
{
    _worldPacket << Caster;
    _worldPacket << ActualDelay;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::MissileCancel::Write()
{
    _worldPacket << OwnerGUID;
    _worldPacket << SpellID;
    _worldPacket.WriteBit(Reverse);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::DispelFailed::Write()
{
    _worldPacket << CasterGUID;
    _worldPacket << VictimGUID;
    _worldPacket << SpellID;
    _worldPacket << static_cast<uint32>(FailedSpellIDs.size());
    for (uint32 const& x : FailedSpellIDs)
        _worldPacket << x;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::SpellDispellLog::Write()
{
    _worldPacket.WriteBit(IsSteal);
    _worldPacket.WriteBit(IsBreak);
    _worldPacket.FlushBits();
    _worldPacket << TargetGUID;
    _worldPacket << CasterGUID;
    _worldPacket << SpellID;
    _worldPacket << static_cast<uint32>(Dispell.size());
    for (auto const& x : Dispell)
    {
        _worldPacket << x.SpellID;
        _worldPacket.WriteBit(x.IsHarmful);
        _worldPacket.WriteBit(x.Rolled.is_initialized());
        _worldPacket.WriteBit(x.Needed.is_initialized());
        _worldPacket.FlushBits();
        if (x.Rolled)
            _worldPacket << *x.Rolled;
        if (x.Needed)
            _worldPacket << *x.Needed;
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Spells::AddLossOfControl::Write()
{
    _worldPacket.WriteBits(Mechanic, 8);
    _worldPacket.WriteBits(Type, 8);
    _worldPacket << SpellID;
    _worldPacket << Caster;
    _worldPacket << Duration;
    _worldPacket << DurationRemaining;
    _worldPacket << LockoutSchoolMask;

    return &_worldPacket;
}
