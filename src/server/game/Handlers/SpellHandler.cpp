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

#include "Chat.h"
#include "Common.h"
#include "CreatureAI.h"
#include "DBCStores.h"
#include "GameObjectAI.h"
#include "GuildMgr.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "PetPackets.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellPackets.h"
#include "TemporarySummon.h"
#include "Totem.h"
#include "TotemPackets.h"
#include "WorldPacket.h"
#include "WorldSession.h"

//! 6.0.3
void WorldSession::HandleUseItemOpcode(WorldPackets::Spells::ItemUse& cast)
{
    // TODO: add targets.read() check
    Player* pUser = _player;

    // ignore for remote control state
    if (pUser->m_mover != pUser)
        return;

    // client provided targets
    SpellCastTargets targets(pUser, cast.Cast);

    Item* pItem = pUser->GetUseableItemByPos(cast.bagIndex, cast.slot);
    if (!pItem)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    if (pItem->GetGUID() != cast.itemGUID)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    ItemTemplate const* proto = pItem->GetTemplate();
    if (!proto)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, pItem, NULL);
        return;
    }

    // some item classes can be used only in equipped state
    if (proto->GetInventoryType() != INVTYPE_NON_EQUIP && !pItem->IsEquipped())
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, pItem, NULL);
        return;
    }

    InventoryResult msg = pUser->CanUseItem(pItem);
    if (msg != EQUIP_ERR_OK)
    {
        pUser->SendEquipError(msg, pItem, NULL);
        return;
    }

    // only allow conjured consumable, bandage, poisons (all should have the 2^21 item flag set in DB)
    if (proto->Class == ITEM_CLASS_CONSUMABLE && !(proto->Flags & ITEM_PROTO_FLAG_USEABLE_IN_ARENA) && pUser->InArena())
    {
        pUser->SendEquipError(EQUIP_ERR_NOT_DURING_ARENA_MATCH, pItem, NULL);
        return;
    }

    //Citron-infused bandages, hack fix
    if (pItem->GetEntry() == 82787 && pUser->GetMap()->IsDungeon())
    {
        pUser->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, pItem, NULL);
        return;
    }

    // don't allow items banned in arena
    if (proto->Flags & ITEM_PROTO_FLAG_NOT_USEABLE_IN_ARENA && pUser->InArena())
    {
        pUser->SendEquipError(EQUIP_ERR_NOT_DURING_ARENA_MATCH, pItem, NULL);
        return;
    }

    if (pUser->isInCombat())
    {
        for (int i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
        {
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(proto->Spells[i].SpellId))
            {
                if (!spellInfo->CanBeUsedInCombat())
                {
                    pUser->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
                    return;
                }
            }
        }
    }

    // check also  BIND_WHEN_PICKED_UP and BIND_QUEST_ITEM for .additem or .additemset case by GM (not binded at adding to inventory)
    if (pItem->GetTemplate()->Bonding == BIND_WHEN_USE || pItem->GetTemplate()->Bonding == BIND_WHEN_PICKED_UP || pItem->GetTemplate()->Bonding == BIND_QUEST_ITEM)
    {
        if (!pItem->IsSoulBound())
        {
            pItem->SetState(ITEM_CHANGED, pUser);
            pItem->SetBinding(true);
        }
    }

    // Note: If script stop casting it must send appropriate data to client to prevent stuck item in gray state.
    if (!sScriptMgr->OnItemUse(pUser, pItem, targets))
    {
        // no script or script not process request by self
        pItem->SetInUse();
        pUser->CastItemUseSpell(pItem, targets, cast.Cast.CastID, cast.Cast.Misc);
    }
}

void WorldSession::HandleGameObjectUseOpcode(WorldPacket & recvData)
{
    ObjectGuid guid;
    recvData >> guid;

    // ignore for remote control state
    if (_player->m_mover != _player)
        return;

    if (GameObject* obj = GetPlayer()->GetMap()->GetGameObject(guid))
        obj->Use(_player);
}

void WorldSession::HandleGameobjectReportUse(WorldPacket& recvPacket)
{
    ObjectGuid guid;
    recvPacket >> guid;

    if (GameObject* go = GetPlayer()->GetMap()->GetGameObject(guid))
    {
        // ignore for remote control state
        if (_player->m_mover != _player)
            if (!(_player->IsOnVehicle(_player->m_mover) || _player->IsMounted()) && !go->GetGOInfo()->IsUsableMounted())
                return;

        if (!go->IsWithinDistInMap(_player, INTERACTION_DISTANCE))
            return;

        if (go->GetEntry() == 193905 || go->GetEntry() == 193967 || //Chest Alexstrasza's Gift
            go->GetEntry() == 194158 || go->GetEntry() == 194159)   //Chest Heart of Magic
            _player->CastSpell(go, 6247, true);
        else
            go->AI()->GossipHello(_player);

        _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT, go->GetEntry());
    }
}

void WorldSession::HandleCastSpellOpcode(WorldPackets::Spells::CastSpell& cast)
{
    bool replaced = false;
    // ignore for remote control state (for player case)
    Unit* mover = _player->m_mover;
    if (mover != _player && mover->GetTypeId() == TYPEID_PLAYER)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: mover != _player id %u", cast.Cast.SpellID);
        return;
    }

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: got cast spell packet, castCount: %u, spellId: %u, glyphIndex %u", cast.Cast.CastID, cast.Cast.SpellID, cast.Cast.Misc);

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(cast.Cast.SpellID);
    if (!spellInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown spell id %u", cast.Cast.SpellID);
        return;
    }

    if (spellInfo->IsPassive())
        return;

    if (mover->GetTypeId() == TYPEID_PLAYER)
    {
        // not have spell in spellbook or spell passive and not casted by client
        if ((!mover->ToPlayer()->HasActiveSpell(cast.Cast.SpellID) || spellInfo->IsPassive()) && !spellInfo->ResearchProject && cast.Cast.SpellID != 101054 && !spellInfo->HasEffect(SPELL_EFFECT_OPEN_LOCK) && !spellInfo->HasEffect(SPELL_EFFECT_LOOT_BONUS) &&
            !(spellInfo->AttributesEx8 & SPELL_ATTR8_RAID_MARKER))
        {
            if (cast.Cast.SpellID == 101603)
            {
                mover->RemoveAurasDueToSpell(107837);
                mover->RemoveAurasDueToSpell(101601);
            }
            if (cast.Cast.SpellID == 119393)
            {
                mover->RemoveAurasDueToSpell(119388);
                mover->RemoveAurasDueToSpell(119386);
            }
            else
            {
                //cheater? kick? ban?
                sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: cheater? kick? ban? TYPEID_PLAYER spell id %u", cast.Cast.SpellID);
                return;
            }
        }
    }
    else
    {
        // spell passive and not casted by client
        if (spellInfo->IsPassive())
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: spell passive and not casted by client id %u", cast.Cast.SpellID);
            return;
        }
        // not have spell in spellbook or spell passive and not casted by client
        if ((mover->GetTypeId() == TYPEID_UNIT && !mover->ToCreature()->HasSpell(cast.Cast.SpellID)) || spellInfo->IsPassive())
            if (mover->GetTypeId() == TYPEID_UNIT && !mover->ToCreature()->HasSpell(cast.Cast.SpellID))
            {
                if (_player->HasActiveSpell(cast.Cast.SpellID))
                    mover = (Unit*)_player;
                else
                {
                    //cheater? kick? ban?
                    sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: not have spell in spellbook id %u", cast.Cast.SpellID);
                    return;
                }
            }
    }

    spellInfo = _player->GetCastSpellInfo(spellInfo);

    // Client is resending autoshot cast opcode when other spell is casted during shoot rotation
    // Skip it to prevent "interrupt" message
    if (spellInfo->IsAutoRepeatRangedSpell() && _player->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL)
        && _player->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL)->m_spellInfo == spellInfo)
        return;

    // can't use our own spells when we're in possession of another unit,
    if (_player->isPossessing())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: can't use our own spells when we're in possession id %u", cast.Cast.SpellID);
        return;
    }

    // Check possible spell cast overrides
    //603 TODO
    //spellInfo = caster->GetCastSpellInfo(spellInfo);

    // client provided targets
    SpellCastTargets targets(mover, cast.Cast);
    // auto-selection buff level base at target level (in spellInfo)
    if (targets.GetUnitTarget())
    {
        SpellInfo const* actualSpellInfo = spellInfo->GetAuraRankForLevel(targets.GetUnitTarget()->getLevel());

        // if rank not found then function return NULL but in explicit cast case original spell can be casted and later failed with appropriate error message
        if (actualSpellInfo)
            spellInfo = actualSpellInfo;
    }

    targets.m_weights.resize(cast.Cast.Weight.size());
    for (size_t i = 0; i < cast.Cast.Weight.size(); ++i)
    {
        targets.m_weights[i].type = cast.Cast.Weight[i].Type;
        switch (targets.m_weights[i].type)
        {
            case WEIGHT_KEYSTONE:
                targets.m_weights[i].keystone.itemId = cast.Cast.Weight[i].ID;
                targets.m_weights[i].keystone.itemCount = cast.Cast.Weight[i].Quantity;
                break;
            case WEIGHT_FRAGMENT:
                targets.m_weights[i].fragment.currencyId = cast.Cast.Weight[i].ID;
                targets.m_weights[i].fragment.currencyCount = cast.Cast.Weight[i].Quantity;
                break;
            default:
                targets.m_weights[i].raw.id = cast.Cast.Weight[i].ID;
                targets.m_weights[i].raw.count = cast.Cast.Weight[i].Quantity;
                break;
        }
    }

    Spell* spell = new Spell(mover, spellInfo, TRIGGERED_NONE, ObjectGuid::Empty, false, replaced);
    spell->m_cast_count = cast.Cast.CastID;
    spell->m_misc.Data = cast.Cast.Misc;
    spell->prepare(&targets);
}

void WorldSession::HandleCancelCast(WorldPackets::Spells::CancelCast& packet)
{
    if (_player->IsNonMeleeSpellCasted(false))
        _player->InterruptNonMeleeSpells(false, packet.SpellID, false);
}

void WorldSession::HandleCancelAura(WorldPackets::Spells::CancelAura& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(packet.SpellID);
    if (!spellInfo)
        return;

    if (spellInfo->Attributes & SPELL_ATTR0_CANT_CANCEL)
        return;

    if (spellInfo->IsChanneled())
    {
        if (Spell* curSpell = player->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
            if (curSpell->m_spellInfo->Id == packet.SpellID)
                player->InterruptSpell(CURRENT_CHANNELED_SPELL);

        return;
    }

    if (!spellInfo->IsPositive() || spellInfo->IsPassive())
        return;

    player->RemoveOwnedAura(packet.SpellID, ObjectGuid::Empty, 0, AURA_REMOVE_BY_CANCEL);
}

void WorldSession::HandlePetCancelAura(WorldPackets::PetPackets::PetCancelAura& packet)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(packet.SpellID);
    if (!spellInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET spell id %u", packet.SpellID);
        return;
    }

    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, packet.PetGUID);
    if (!pet)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCancelAura: Attempt to cancel an aura for non-existant pet %u by player '%s'", uint32(packet.PetGUID.GetGUIDLow()), GetPlayer()->GetName());
        return;
    }

    if (pet != GetPlayer()->GetGuardianPet() && pet != GetPlayer()->GetCharm())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCancelAura: Pet %u is not a pet of player '%s'", uint32(packet.PetGUID.GetGUIDLow()), GetPlayer()->GetName());
        return;
    }

    if (!pet->isAlive())
    {
        pet->SendPetActionFeedback(FEEDBACK_PET_DEAD);
        return;
    }

    pet->RemoveOwnedAura(packet.SpellID, ObjectGuid::Empty, 0, AURA_REMOVE_BY_CANCEL);

    pet->AddCreatureSpellCooldown(packet.SpellID);
}

void WorldSession::HandleCancelAutoRepeatSpellOpcode(WorldPacket& /*recvPacket*/)
{
    // may be better send SMSG_CANCEL_AUTO_REPEAT?
    // cancel and prepare for deleting
    _player->InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
}

void WorldSession::HandleCancelChanneling(WorldPacket& recvData)
{
    recvData.read_skip<uint32>();                          // spellid, not used

    // ignore for remote control state (for player case)
    Unit* mover = _player->m_mover;
    if (mover != _player && mover->GetTypeId() == TYPEID_PLAYER)
        return;

    mover->InterruptSpell(CURRENT_CHANNELED_SPELL);
}

void WorldSession::HandleTotemDestroyed(WorldPackets::Totem::TotemDestroyed& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (player->m_mover != player)
        return;

    ++packet.Slot;

    if (packet.Slot >= MAX_TOTEM_SLOT)
        return;

    if (!player->m_SummonSlot[packet.Slot])
        return;

    if (Creature* summon = player->GetMap()->GetCreature(player->m_SummonSlot[packet.Slot]))
    {
        if (uint32 spellId = summon->GetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL))
            if (AreaTrigger* arTrigger = player->GetAreaObject(spellId))
                arTrigger->SetDuration(0);

        summon->DespawnOrUnsummon();
    }
}

void WorldSession::HandleSelfRes(WorldPackets::Spells::SelfRes& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (player->HasAuraType(SPELL_AURA_PREVENT_RESURRECTION))
        return;

    if (player->GetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL))
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(player->GetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL));
        if (spellInfo)
            player->CastSpell(player, spellInfo, false, 0);

        player->SetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL, 0);
    }
}

void WorldSession::HandleSpellClick(WorldPackets::Spells::SpellClick& packet)
{
    // this will get something not in world. crash
    Creature* unit = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, packet.SpellClickUnitGuid);
    if (!unit)
        return;

    // TODO: Unit::SetCharmedBy: 28782 is not in world but 0 is trying to charm it! -> crash
    if (!unit->IsInWorld())
        return;

    // flags in Deepwind Gorge
    if (unit->GetEntry() == 53194)
    {
        _player->CastSpell(unit, unit->GetInt32Value(UNIT_FIELD_INTERACT_SPELL_ID));
        return;
    }

    unit->HandleSpellClick(_player);
}

void WorldSession::HandleGetMirrorImageData(WorldPackets::Spells::GetMirrorImageData& packet)
{
    // Get unit for which data is needed by client
    Unit* unit = ObjectAccessor::GetObjectInWorld(packet.UnitGUID, (Unit*)NULL);
    if (!unit)
        return;

    if (!unit->HasAuraType(SPELL_AURA_CLONE_CASTER))
        return;

    // Get creator of the unit (SPELL_AURA_CLONE_CASTER does not stack)
    Unit* creator = unit->GetAuraEffectsByType(SPELL_AURA_CLONE_CASTER).front()->GetCaster();
    if (!creator)
        return;

    if (Player* player = creator->ToPlayer())
    {
        WorldPackets::Spells::MirrorImageComponentedData mirrorImageComponentedData;
        mirrorImageComponentedData.UnitGUID = packet.UnitGUID;
        mirrorImageComponentedData.DisplayID = creator->GetDisplayId();
        mirrorImageComponentedData.RaceID = creator->getRace();
        mirrorImageComponentedData.Gender = creator->getGender();
        mirrorImageComponentedData.ClassID = creator->getClass();

        Guild* guild = player->GetGuild();

        mirrorImageComponentedData.SkinColor = player->GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_SKIN_ID);
        mirrorImageComponentedData.FaceVariation = player->GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_FACE_ID);
        mirrorImageComponentedData.HairVariation = player->GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_STYLE_ID);
        mirrorImageComponentedData.HairColor = player->GetByteValue(PLAYER_BYTES, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID);
        mirrorImageComponentedData.BeardVariation = player->GetByteValue(PLAYER_BYTES_2, PLAYER_BYTES_2_OFFSET_FACIAL_STYLE);
        mirrorImageComponentedData.GuildGUID = (guild ? guild->GetGUID() : ObjectGuid::Empty);

        mirrorImageComponentedData.ItemDisplayID.reserve(11);

        static EquipmentSlots const itemSlots[] =
        {
            EQUIPMENT_SLOT_HEAD,
            EQUIPMENT_SLOT_SHOULDERS,
            EQUIPMENT_SLOT_BODY,
            EQUIPMENT_SLOT_CHEST,
            EQUIPMENT_SLOT_WAIST,
            EQUIPMENT_SLOT_LEGS,
            EQUIPMENT_SLOT_FEET,
            EQUIPMENT_SLOT_WRISTS,
            EQUIPMENT_SLOT_HANDS,
            EQUIPMENT_SLOT_TABARD,
            EQUIPMENT_SLOT_BACK,
            EQUIPMENT_SLOT_END
        };

        // Display items in visible slots
        for (auto const& slot : itemSlots)
        {
            uint32 itemDisplayId;
            if ((slot == EQUIPMENT_SLOT_HEAD && player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM)) ||
                (slot == EQUIPMENT_SLOT_BACK && player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_CLOAK)))
                itemDisplayId = 0;
            else if (Item const* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                itemDisplayId = item->GetDisplayId();
            else
                itemDisplayId = 0;

            mirrorImageComponentedData.ItemDisplayID.push_back(itemDisplayId);
        }
        SendPacket(mirrorImageComponentedData.Write());
    }
    else
    {
        WorldPackets::Spells::MirrorImageCreatureData mirrorImageCreatureData;
        mirrorImageCreatureData.UnitGUID = packet.UnitGUID;
        mirrorImageCreatureData.DisplayID = creator->GetDisplayId();
        SendPacket(mirrorImageCreatureData.Write());
    }
}

void WorldSession::HandleUpdateProjectilePosition(WorldPacket& recvPacket)
{
    ObjectGuid casterGuid;
    uint32 spellId;
    uint8 castID;
    Position CollisionPos;

    recvPacket >> casterGuid;
    recvPacket >> spellId;
    recvPacket >> castID;
    recvPacket >> CollisionPos.PositionXYZStream();

    Unit* caster = ObjectAccessor::GetUnit(*_player, casterGuid);
    if (!caster)
        return;

    Spell* spell = caster->FindCurrentSpellBySpellId(spellId);
    if (!spell || !spell->m_targets.HasDst())
        return;

    Position pos = *spell->m_targets.GetDstPos();
    pos.Relocate(CollisionPos);
    spell->m_targets.ModDst(pos);


    WorldPackets::Spells::NotifyMissileTrajectoryCollision response;
    response.Caster = casterGuid;
    response.CastID = castID;
    response.CollisionPos = CollisionPos;
    caster->SendMessageToSet(response.Write(), true);
}

void WorldSession::HandleUpdateMissileTrajectory(WorldPacket& recvPacket)
{
    ObjectGuid guid;
    uint32 spellId;
    float elevation, speed;
    float curX, curY, curZ;
    float targetX, targetY, targetZ;
    uint8 moveStop;

    recvPacket >> guid >> spellId >> elevation >> speed;
    recvPacket >> curX >> curY >> curZ;
    recvPacket >> targetX >> targetY >> targetZ;
    recvPacket >> moveStop;

    Unit* caster = ObjectAccessor::GetUnit(*_player, guid);
    Spell* spell = caster ? caster->GetCurrentSpell(CURRENT_GENERIC_SPELL) : NULL;
    if (!spell || spell->m_spellInfo->Id != spellId || !spell->m_targets.HasDst() || !spell->m_targets.HasSrc())
    {
        recvPacket.rfinish();
        return;
    }

    Position pos = *spell->m_targets.GetSrcPos();
    pos.Relocate(curX, curY, curZ);
    spell->m_targets.ModSrc(pos);

    pos = *spell->m_targets.GetDstPos();
    pos.Relocate(targetX, targetY, targetZ);
    spell->m_targets.ModDst(pos);

    spell->m_targets.SetPitch(elevation);
    spell->m_targets.SetSpeed(speed);

    if (moveStop)
    {
        uint32 opcode;
        recvPacket >> opcode;
        recvPacket.SetOpcode(CMSG_MOVE_STOP); // always set to MSG_MOVE_STOP in client SetOpcode
        //HandleMovementOpcodes(recvPacket);
    }
}

void WorldSession::HandleRequestCategoryCooldowns(WorldPackets::Spells::RequestCategoryCooldowns& /*packet*/)
{
    _player->SendCategoryCooldownMods();
}

void WorldSession::HandleUnlearnSkill(WorldPackets::Spells::UnlearnSkill& packet)
{
    SkillRaceClassInfoEntry const* rcEntry = GetSkillRaceClassInfo(packet.SkillLine, GetPlayer()->getRace(), GetPlayer()->getClass());
    if (!rcEntry || !(rcEntry->Flags & SKILL_FLAG_UNLEARNABLE))
        return;

    GetPlayer()->SetSkill(packet.SkillLine, 0, 0, 0);
}

void WorldSession::HandleCancelMountAura(WorldPackets::Spells::CancelMountAura& /*packet*/)
{
    _player->RemoveAurasByType(SPELL_AURA_MOUNTED, [](AuraApplication const* aurApp)
    {
        SpellInfo const* spellInfo = aurApp->GetBase()->GetSpellInfo();
        return !spellInfo->HasAttribute(SPELL_ATTR0_CANT_CANCEL) && spellInfo->IsPositive() && !spellInfo->IsPassive();
    });
}

void WorldSession::HandleCancelGrowthAura(WorldPackets::Spells::CancelGrowthAura& /*packet*/)
{
    _player->RemoveAurasByType(SPELL_AURA_MOD_SCALE, [](AuraApplication const* aurApp)
    {
        SpellInfo const* spellInfo = aurApp->GetBase()->GetSpellInfo();
        return !spellInfo->HasAttribute(SPELL_ATTR0_CANT_CANCEL) && spellInfo->IsPositive() && !spellInfo->IsPassive();
    });
}

void WorldSession::HandleSetActionButtonOpcode(WorldPackets::Spells::SetActionButton& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    uint32 action = packet.Action;
    uint8 type = uint8(packet.Type >> 24);

    sLog->outInfo(LOG_FILTER_NETWORKIO, "BUTTON: %u ACTION: %u TYPE: %u", packet.Index, action, type);

    if (!packet.Action)
        player->removeActionButton(packet.Index);
    else
    {
        switch (type)
        {
            case ACTION_BUTTON_MACRO:
            case ACTION_BUTTON_CMACRO:
                sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added Macro %u into button %u", action, packet.Index);
                break;
            case ACTION_BUTTON_EQSET:
                sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added EquipmentSetInfo %u into button %u", action, packet.Index);
                break;
            case ACTION_BUTTON_SPELL:
                sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added Spell %u into button %u", action, packet.Index);
                break;
            case ACTION_BUTTON_SUB_BUTTON:
                sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added sub buttons %u into button %u", action, packet.Index);
                break;
            case ACTION_BUTTON_ITEM:
                sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added Item %u into button %u", action, packet.Index);
                break;
            case ACTION_BUTTON_PET:
                sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added Pet Spell %u into button %u", action, packet.Index);
                break;
            default:
                sLog->outError(LOG_FILTER_NETWORKIO, "MISC: Unknown action button type %u for action %u into button %u for player %s (GUID: %u)", type, action, packet.Index, _player->GetName(), _player->GetGUIDLow());
                break;
        }

        player->addActionButton(packet.Index, action, type);
    }
}
