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
#include "WorldPacket.h"
#include "WorldSession.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Spell.h"
#include "ObjectAccessor.h"
#include "CreatureAI.h"
#include "Util.h"
#include "Pet.h"
#include "World.h"
#include "Group.h"
#include "SpellInfo.h"
#include "SpellAuraEffects.h"
#include "SpellPackets.h"
#include "QueryPackets.h"
#include "PetPackets.h"

enum StableResultCode
{
    STABLE_ERR_NONE = 0x00,                         // does nothing, just resets stable states
    STABLE_ERR_MONEY = 0x01,                         // "you don't have enough money"
    STABLE_ERR_INVALID_SLOT = 0x03,
    STABLE_ERR_STABLE = 0x06,                         // currently used in most fail cases
    STABLE_SUCCESS_STABLE = 0x08,                         // stable success
    STABLE_SUCCESS_UNSTABLE = 0x09,                         // unstable/swap success
    STABLE_SUCCESS_BUY_SLOT = 0x0A,                         // buy slot success
    STABLE_ERR_EXOTIC = 0x0B,                         // "you are unable to control exotic creatures"
    STABLE_ERR_INTERNAL = 0x0C,
};

void WorldSession::HandleDismissCritter(WorldPackets::PetPackets::DismissCritter& packet)
{
    Unit* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, packet.CritterGUID);
    if (!pet)
        return;

    if (_player->GetCritterGUID() == pet->GetGUID())
    {
         if (pet->GetTypeId() == TYPEID_UNIT && pet->ToCreature()->isSummon())
             pet->ToTempSummon()->UnSummon();
    }
}

//! 6.0.3
void WorldSession::HandlePetAction(WorldPacket & recvData)
{
    ObjectGuid guid1;   //pet guid
    ObjectGuid guid2;   //tag guid
    uint32 data;
    float x, y, z;

    recvData >> guid1 >> data >> guid2 >> x >> y >> z;

    uint32 spellid = UNIT_ACTION_BUTTON_ACTION(data);
    uint8 flag = UNIT_ACTION_BUTTON_TYPE(data);             //delete = 0x07 CastSpell = C1

    // used also for charmed creature
    Unit* pet= ObjectAccessor::GetUnit(*_player, guid1);
    sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetAction: Pet %u - flag: %u, spellid: %u, target: %u.", guid1.GetGUIDLow(), uint32(flag), spellid, guid2.GetGUIDLow());

    if (!pet)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetAction: Pet (GUID: %u) doesn't exist for player '%s'", guid1.GetGUIDLow(), GetPlayer()->GetName());
        return;
    }

    if (pet != GetPlayer()->GetFirstControlled())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetAction: Pet (GUID: %u) does not belong to player '%s'", guid1.GetGUIDLow(), GetPlayer()->GetName());
        return;
    }

    if (!pet->isAlive())
    {
        SpellInfo const* spell = (flag == ACT_ENABLED || flag == ACT_PASSIVE) ? sSpellMgr->GetSpellInfo(spellid) : NULL;
        if (!spell)
            return;
        if (!(spell->Attributes & SPELL_ATTR0_CASTABLE_WHILE_DEAD))
            return;
    }

    //TODO: allow control charmed player?
    if (pet->GetTypeId() == TYPEID_PLAYER && !(flag == ACT_COMMAND && spellid == COMMAND_ATTACK))
        return;

    if (GetPlayer()->m_Controlled.size() == 1)
        HandlePetActionHelper(pet, guid1, spellid, flag, guid2, x, y ,z);
    else
    {
        //If a pet is dismissed, m_Controlled will change
        std::vector<Unit*> controlled;
        for (Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
            if ((*itr)->GetEntry() == pet->GetEntry() && (*itr)->isAlive())
            {
                if((*itr)->ToCreature())
                {
                    if(!(*itr)->ToCreature()->m_Stampeded && (*itr)->HasUnitTypeMask(UNIT_MASK_CONTROLABLE_GUARDIAN))
                        controlled.push_back(*itr);
                }
                else
                    controlled.push_back(*itr);
            }
        for (std::vector<Unit*>::iterator itr = controlled.begin(); itr != controlled.end(); ++itr)
            HandlePetActionHelper(*itr, guid1, spellid, flag, guid2, x, y, z);
    }
}

void WorldSession::HandleStopAttack(WorldPackets::PetPackets::StopAttack& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Unit* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*player, packet.PetGUID);
    if (!pet)
        return;

    if (pet != player->GetPet() && pet != player->GetCharm())
        return;

    if (!pet->isAlive())
        return;

    pet->AttackStop();
}

void WorldSession::HandleQueryPetName(WorldPackets::Query::QueryPetName& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    WorldPackets::Query::QueryPetNameResponse response;
    response.UnitGUID = packet.UnitGUID;

    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*player, packet.UnitGUID);
    if (!pet)
    {
        player->GetSession()->SendPacket(response.Write());
        return;
    }

    response.Allow = pet->isPet() ? 1 : 0;
    
    response.DeclinedNames;
    response.Name = pet->GetName();

    if (Pet* playerPet = pet->ToPet())
    {
        DeclinedName const* declinedNames = playerPet->GetDeclinedNames();
        if (declinedNames)
        {
            response.HasDeclined = true;

            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                if (declinedNames->name[i].size())
                    response.DeclinedNames.name[i] = declinedNames->name[i];
        }
        else
            response.HasDeclined = false;

        response.Timestamp = playerPet->GetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP);
    }

    player->GetSession()->SendPacket(response.Write());
}

//! 6.0.3
void WorldSession::HandlePetSetAction(WorldPacket & recvData)
{
    //sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetSetAction. CMSG_PET_SET_ACTION");

    uint32 position;
    uint32 data;
    ObjectGuid petguid;

    recvData >> petguid >> position >> data;

    Unit* pet = ObjectAccessor::GetUnit(*_player, petguid);

    if (!pet || pet != _player->GetFirstControlled())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetSetAction: Unknown pet (GUID: %u) or pet owner (GUID: %u)", petguid.GetGUIDLow(), _player->GetGUIDLow());
        return;
    }

    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandlePetSetAction: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    bool move_command = false;

    uint32 spell_id = UNIT_ACTION_BUTTON_ACTION(data);
    uint8 act_state = UNIT_ACTION_BUTTON_TYPE(data);

    //ignore invalid position
    if (position >= MAX_UNIT_ACTION_BAR_INDEX)
        return;

    // remove batton
    if (act_state == ACT_DECIDE)
    {
        UnitActionBarEntry const* actionEntry = charmInfo->GetActionBarEntry(position);
        if (!actionEntry)
            return;

        //charmInfo->RemoveSpellFromActionBar(actionEntry->GetAction());
        //return;
    }

    sLog->outInfo(LOG_FILTER_NETWORKIO, "Player %s has changed pet spell action. Position: %u, Spell: %u, State: 0x%X HasSpell %i", _player->GetName(), position, spell_id, uint32(act_state), pet->HasSpell(spell_id));

    //if it's act for spell (en/disable/cast) and there is a spell given (0 = remove spell) which pet doesn't know, don't add
    if (!((act_state == ACT_ENABLED || act_state == ACT_DISABLED || act_state == ACT_PASSIVE) && spell_id && !pet->HasSpell(spell_id)))
    {
        if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell_id))
        {
            //sign for autocast
            if (act_state == ACT_ENABLED)
            {
                if (pet->GetCharmInfo())
                    ((Pet*)pet)->ToggleAutocast(spellInfo, true);
                else
                    for (Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
                        if ((*itr)->GetEntry() == pet->GetEntry())
                            (*itr)->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, true);
            }
            //sign for no/turn off autocast
            else if (act_state == ACT_DISABLED)
            {
                if (pet->GetCharmInfo())
                    ((Pet*)pet)->ToggleAutocast(spellInfo, false);
                else
                    for (Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
                        if ((*itr)->GetEntry() == pet->GetEntry())
                            (*itr)->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, false);
            }
        }

        charmInfo->SetActionBar(position, spell_id, ActiveStates(act_state));
    }
}

//! 6.0.3
void WorldSession::HandlePetRename(WorldPacket & recvData)
{
    //sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetRename. CMSG_PET_RENAME");

    ObjectGuid PetGUID;
    uint32 petnumber;

    std::string name;
    DeclinedName declinedname;

    recvData >> PetGUID >> petnumber;

    int32 len = recvData.ReadBits(8);
    uint8 isdeclined  = recvData.ReadBit();

    uint32 declname[MAX_DECLINED_NAME_CASES];
    if (isdeclined)
    {
        for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            declname[i] = recvData.ReadBits(7);

        for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            declinedname.name[i] = recvData.ReadString(declname[i]);
    }
    name = recvData.ReadString(len);

    Pet* pet = _player->GetPet();
    if (!pet)
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetRename pet not found");
        return;
    }
                                                            // check it!
    if (!pet || !pet->isPet() || ((Pet*)pet)->getPetType()!= HUNTER_PET ||
        !pet->HasByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED) ||
        pet->GetOwnerGUID() != _player->GetGUID() || !pet->GetCharmInfo())
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetRename error pet");
        return;
    }

    if (pet->GetCharmInfo()->GetPetNumber() != petnumber ||
        pet->GetGUID() != PetGUID)
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetRename petnumber not correct");
        return;
    }

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    //if (namepart)
    {
        PetNameInvalidReason res = ObjectMgr::CheckPetName(name);
        if (res != PET_NAME_SUCCESS)
        {
            SendPetNameInvalid(res, PetGUID, name, NULL);
            sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetRename CheckPetName res %i", res);
            return;
        }

        if (sObjectMgr->IsReservedName(name))
        {
            SendPetNameInvalid(PET_NAME_RESERVED, PetGUID, name, NULL);
            return;
        }

        pet->SetName(name);

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_NAME);
        stmt->setString(0, name);
        stmt->setUInt64(1, _player->GetGUIDLow());
        stmt->setUInt32(2, pet->GetCharmInfo()->GetPetNumber());
        trans->Append(stmt);
    }

    if (isdeclined)
    {
        if (sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
        {
            std::wstring wname;
            Utf8toWStr(name, wname);
            if (!ObjectMgr::CheckDeclinedNames(wname, declinedname))
            {
                SendPetNameInvalid(PET_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME, PetGUID, name, &declinedname);
                return;
            }

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_PET_DECLINEDNAME);
            stmt->setUInt32(0, pet->GetCharmInfo()->GetPetNumber());
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_ADD_CHAR_PET_DECLINEDNAME);
            stmt->setUInt64(0, _player->GetGUIDLow());

            for (uint8 i = 0; i < 5; i++)
                stmt->setString(i+1, declinedname.name[i]);

            trans->Append(stmt);
        }
    }

    CharacterDatabase.CommitTransaction(trans);

    Unit* owner = pet->GetOwner();
    if (owner && (owner->GetTypeId() == TYPEID_PLAYER) && owner->ToPlayer()->GetGroup())
        owner->ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_NAME);

    pet->RemoveByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED);
    pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(time(NULL))); // cast can't be helped
}

//! 6.0.3
void WorldSession::HandlePetAbandon(WorldPacket& recvData)
{
    ObjectGuid guid;
    recvData >> guid;

    //sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetAbandon. CMSG_PET_ABANDON pet guid is %u", guid.GetGUIDLow());

    if (!_player->IsInWorld())
        return;

    // pet/charmed
    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, guid);
    if (pet)
    {
        if (pet->isPet())
        {
            _player->RemovePet((Pet*)pet, true);
            _player->GetSession()->SendStablePet(ObjectGuid::Empty);
        }
        else if (pet->GetGUID() == _player->GetCharmGUID())
            _player->StopCastingCharm();
    }
}

//! 6.0.3
void WorldSession::HandlePetSpellAutocastOpcode(WorldPacket& recvPacket)
{
    ObjectGuid guid;
    uint32 spellid;
    uint32  state;                                           //1 for on, 0 for off
    recvPacket >> guid >> spellid;
    state = recvPacket.ReadBit();

    if (!_player->GetGuardianPet() && !_player->GetCharm())
        return;

    if (ObjectAccessor::FindPlayer(guid))
        return;

    Creature* pet=ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, guid);

    if (!pet || (pet != _player->GetGuardianPet() && pet != _player->GetCharm()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetSpellAutocastOpcode.Pet %u isn't pet of player %s .", uint32(guid.GetGUIDLow()), GetPlayer()->GetName());
        return;
    }

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellid);
    // do not add not learned spells/ passive spells
    if (!pet->HasSpell(spellid) || spellInfo->IsAutocastable())
        return;

    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandlePetSpellAutocastOpcod: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    if (pet->isPet())
        ((Pet*)pet)->ToggleAutocast(spellInfo, state);
    else
        pet->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, state != 0);

    charmInfo->SetSpellAutocast(spellInfo, state != 0);
}

//! 6.0.3
void WorldSession::HandlePetCastSpellOpcode(WorldPackets::Spells::PetCastSpell& cast)
{
    if (!_player->GetGuardianPet() && !_player->GetCharm())
        return;

    Unit* caster = ObjectAccessor::GetUnit(*_player, cast.PetGUID);

    if (!caster || (caster != _player->GetGuardianPet() && caster != _player->GetCharm()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCastSpellOpcode: Pet %u isn't pet of player %s .", cast.PetGUID.GetGUIDLow(), GetPlayer()->GetName());
        return;
    }

    SpellCastTargets targets(caster, cast.Cast);

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(cast.Cast.SpellID);
    if (!spellInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET spell id %i", cast.Cast.SpellID);
        return;
    }

    bool triggered = false;
    for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (spellInfo->Effects[i].TargetA.GetTarget() == TARGET_DEST_TRAJ || spellInfo->Effects[i].TargetB.GetTarget() == TARGET_DEST_TRAJ || spellInfo->Effects[i].Effect == SPELL_EFFECT_TRIGGER_MISSILE)
            triggered = true;
    }

    if (spellInfo->StartRecoveryCategory > 0) // Check if spell is affected by GCD
        if (caster->GetTypeId() == TYPEID_UNIT && caster->GetCharmInfo() && caster->GetCharmInfo()->GetGlobalCooldownMgr().HasGlobalCooldown(spellInfo))
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCastSpellOpcode: Check if spell is affected by GCD");
            Spell::SendCastResult(GetPlayer(), spellInfo, 0, SPELL_FAILED_NOT_READY, SPELL_CUSTOM_ERROR_NONE, 0, true);
            return;
        }

    // do not cast not learned spells
    if (!triggered && (!caster->HasSpell(cast.Cast.SpellID) || spellInfo->IsPassive()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCastSpellOpcode: !HasSpell or IsPassive");
        return;
    }

    caster->ClearUnitState(UNIT_STATE_FOLLOW);

    uint32 triggeredCastFlags = triggered ? TRIGGERED_FULL_MASK : TRIGGERED_NONE;
    triggeredCastFlags &= ~TRIGGERED_IGNORE_POWER_AND_REAGENT_COST;

    Spell* spell = new Spell(caster, spellInfo, TriggerCastFlags(triggeredCastFlags));
    spell->m_cast_count = cast.Cast.CastID;                    // probably pending spell cast
    spell->m_targets = targets;

    // TODO: need to check victim?
    SpellCastResult result;
    if (caster->m_movedPlayer)
        result = spell->CheckPetCast(caster->m_movedPlayer->GetSelectedUnit());
    else
        result = spell->CheckPetCast(NULL);
    if (result == SPELL_CAST_OK)
    {
        if (caster->GetTypeId() == TYPEID_UNIT)
        {
            Creature* pet = caster->ToCreature();
            pet->AddCreatureSpellCooldown(cast.Cast.SpellID);
            if (pet->isPet())
            {
                Pet* p = (Pet*)pet;
                // 10% chance to play special pet attack talk, else growl
                // actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
                if (p->getPetType() == SUMMON_PET && (urand(0, 100) < 10))
                    pet->SendPetTalk((uint32)PET_TALK_SPECIAL_SPELL);
                else
                    pet->SendPetAIReaction(cast.PetGUID);
            }
        }

        spell->prepare(&(spell->m_targets));
    }
    else
    {
        Creature* pet = caster->ToCreature();
        bool sendPet = !pet || !(pet->isPossessed() || pet->IsVehicle());
        Spell::SendCastResult(GetPlayer(), spellInfo, cast.Cast.CastID, result, SPELL_CUSTOM_ERROR_NONE, 0, sendPet);

        if (caster->GetTypeId() == TYPEID_PLAYER)
        {
            if (!caster->ToPlayer()->HasSpellCooldown(cast.Cast.SpellID))
                GetPlayer()->SendClearCooldown(cast.Cast.SpellID, caster);
        }
        else
        {
            if (!caster->ToCreature()->HasSpellCooldown(cast.Cast.SpellID))
                GetPlayer()->SendClearCooldown(cast.Cast.SpellID, caster);
        }

        spell->finish(false);
        delete spell;
    }
}

void WorldSession::HandleLearnPetSpecializationGroup(WorldPackets::PetPackets::LearnPetSpecializationGroup& packet)
{
    if (_player->isInCombat() || _player->getClass() != CLASS_HUNTER)
        return;

    uint32 specializationId = 0;
    bool adaptation = false;

    if (_player->HasAura(152244)) //Adaptation Spec
        adaptation = true;

    switch(packet.SpecGroupId)
    {
        case 0:
            if(adaptation)
                specializationId = SPEC_PET_ADAPTATION_FEROCITY;
            else
                specializationId = SPEC_PET_FEROCITY;
            break;
        case 1:
            if(adaptation)
                specializationId = SPEC_PET_ADAPTATION_TENACITY;
            else
                specializationId = SPEC_PET_TENACITY;
            break;
        case 2:
            if(adaptation)
                specializationId = SPEC_PET_ADAPTATION_CUNNING;
            else
                specializationId = SPEC_PET_CUNNING;
            break;
        default:
            break;
    }

    if (!specializationId)
        return;

    Pet* pet = _player->GetPet();
    if (!pet || !pet->isAlive())
        return;

    if (pet->GetSpecializationId())
        pet->UnlearnSpecializationSpell();

    pet->SetSpecializationId(specializationId);
    pet->LearnSpecializationSpell();
    _player->SendTalentsInfoData(true);
}

//! 6.0.3
void WorldSession::HandleListStabledPetsOpcode(WorldPacket & recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recv CMSG_REQUEST_STABLED_PETS");

    ObjectGuid npcGUID;
    recvData >> npcGUID;

    if (!CheckStableMaster(npcGUID))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // remove mounts this fix bug where getting pet from stable while mounted deletes pet.
    if (GetPlayer()->IsMounted())
        GetPlayer()->RemoveAurasByType(SPELL_AURA_MOUNTED);

    SendStablePet(npcGUID);
}

//! 6.0.3
void WorldSession::HandleStableChangeSlot(WorldPacket & recv_data)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recv CMSG_SET_PET_SLOT.");
    uint32 pet_number;
    ObjectGuid npcGUID;
    uint8 new_slot;

    recv_data >> pet_number >> new_slot >> npcGUID;

    if (!CheckStableMaster(npcGUID))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    if (new_slot > MAX_PET_STABLES)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    Pet* pet = _player->GetPet();

    //If we move the pet already summoned...
    if (pet && pet->GetCharmInfo() && pet->GetCharmInfo()->GetPetNumber() == pet_number)
        _player->RemovePet(pet);

    //If we move to the pet already summoned...
    PetSlot curentSlot = GetPlayer()->GetSlotForPetId(GetPlayer()->m_currentPetNumber);
    if (pet && curentSlot == new_slot)
        _player->RemovePet(pet);


    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PET_BY_ID);

    stmt->setUInt64(0, _player->GetGUIDLow());
    stmt->setUInt32(1, pet_number);

    _stableChangeSlotCallback.SetParam(new_slot);
    _stableChangeSlotCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::HandleStableChangeSlotCallback(PreparedQueryResult result, uint8 new_slot)
{
    if (!GetPlayer())
        return;

    if (!result)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    Field *fields = result->Fetch();

    uint32 pet_entry = fields[0].GetUInt32();
    uint32 pet_number  = fields[1].GetUInt32();
    bool isHunter = fields[2].GetUInt8() == HUNTER_PET;

    PetSlot slot = GetPlayer()->GetSlotForPetId(pet_number);

    if (!pet_entry)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(pet_entry);
    if (!creatureInfo || !creatureInfo->isTameable(_player->CanTameExoticPets()))
    {
        // if problem in exotic pet
        if (creatureInfo && creatureInfo->isTameable(true))
            SendStableResult(STABLE_ERR_EXOTIC);
        else
            SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // Update if its a Hunter pet
    if (new_slot != 100)
    {
        // We need to remove and add the new pet to there diffrent slots
        GetPlayer()->SwapPetSlot(slot, (PetSlot)new_slot);
    }

    SendStableResult(STABLE_SUCCESS_STABLE);
}

void WorldSession::HandleStableRevivePet(WorldPacket & recvData )
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleStableRevivePet: Not implemented");
}

bool WorldSession::CheckStableMaster(ObjectGuid const& guid)
{
    // spell case or GM
    if (guid == GetPlayer()->GetGUID())
    {
        if (!GetPlayer()->isGameMaster() && !GetPlayer()->HasAuraType(SPELL_AURA_OPEN_STABLE))
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Player (GUID:%u) attempt open stable in cheating way.", guid.GetGUIDLow());
            return false;
        }
    }
    // stable master case
    else
    {
        if (!GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_STABLEMASTER))
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Stablemaster (GUID:%u) not found or you can't interact with him.", guid.GetGUIDLow());
            return false;
        }
    }
    return true;
}

void WorldSession::SendStableResult(uint8 res)
{
    WorldPackets::PetPackets::StableResult stableResult;
    stableResult.Result = res;
    SendPacket(stableResult.Write());
}

void WorldSession::HandlePetActionHelper(Unit* pet, ObjectGuid guid1, uint32 spellid, uint16 flag, ObjectGuid guid2, float x, float y, float z)
{
    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandlePetAction(petGuid: " UI64FMTD ", tagGuid: " UI64FMTD ", spellId: %u, flag: %u): object (entry: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!",
            guid1, guid2, spellid, flag, pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    switch (flag)
    {
        case ACT_COMMAND:                                   //0x07
            switch (spellid)
            {
                case COMMAND_STAY:                          //flat=1792  //STAY
                    pet->StopMoving();
                    pet->GetMotionMaster()->Clear(false);
                    pet->GetMotionMaster()->MoveIdle();
                    charmInfo->SetCommandState(COMMAND_STAY);

                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(true);
                    charmInfo->SetIsFollowing(false);
                    charmInfo->SetIsReturning(false);
                    charmInfo->SaveStayPosition();
                    break;
                case COMMAND_FOLLOW:                        //spellid=1792  //FOLLOW
                    pet->AttackStop();
                    pet->InterruptNonMeleeSpells(false);
                    pet->GetMotionMaster()->MoveFollow(_player, PET_FOLLOW_DIST, pet->GetFollowAngle());
                    charmInfo->SetCommandState(COMMAND_FOLLOW);

                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(false);
                    charmInfo->SetIsReturning(true);
                    charmInfo->SetIsFollowing(false);
                    break;
                case COMMAND_ATTACK:                        //spellid=1792  //ATTACK
                {
                    // Can't attack if owner is pacified
                    if (_player->HasAuraType(SPELL_AURA_MOD_PACIFY))
                    {
                        //pet->SendPetCastFail(spellid, SPELL_FAILED_PACIFIED);
                        //TODO: Send proper error message to client
                        return;
                    }

                    // only place where pet can be player
                    Unit* TargetUnit = ObjectAccessor::GetUnit(*_player, guid2);
                    if (!TargetUnit)
                        return;

                    if (Unit* owner = pet->GetOwner())
                        if (!owner->IsValidAttackTarget(TargetUnit))
                            return;

                    // Not let attack through obstructions
                    if (sWorld->getBoolConfig(CONFIG_PET_LOS))
                    {
                        if (!pet->IsWithinLOSInMap(TargetUnit))
                            return;
                    }

                    pet->ClearUnitState(UNIT_STATE_FOLLOW);
                    // This is true if pet has no target or has target but targets differs.
                    if (pet->getVictim() != TargetUnit || (pet->getVictim() == TargetUnit && !pet->GetCharmInfo()->IsCommandAttack()))
                    {
                        if (pet->getVictim())
                            pet->AttackStop();

                        if (pet->GetTypeId() != TYPEID_PLAYER && pet->ToCreature()->IsAIEnabled)
                        {
                            charmInfo->SetIsCommandAttack(true);
                            charmInfo->SetIsAtStay(false);
                            charmInfo->SetIsFollowing(false);
                            charmInfo->SetIsReturning(false);

                            pet->ToCreature()->AI()->AttackStart(TargetUnit);

                            //10% chance to play special pet attack talk, else growl
                            if (pet->ToCreature()->isPet() && ((Pet*)pet)->getPetType() == SUMMON_PET && pet != TargetUnit && urand(0, 100) < 10)
                                pet->SendPetTalk((uint32)PET_TALK_ATTACK);
                            else
                            {
                                // 90% chance for pet and 100% chance for charmed creature
                                pet->SendPetAIReaction(guid1);
                            }
                        }
                        else                                // charmed player
                        {
                            if (pet->getVictim() && pet->getVictim() != TargetUnit)
                                pet->AttackStop();

                            charmInfo->SetIsCommandAttack(true);
                            charmInfo->SetIsAtStay(false);
                            charmInfo->SetIsFollowing(false);
                            charmInfo->SetIsReturning(false);

                            pet->Attack(TargetUnit, true);
                            pet->SendPetAIReaction(guid1);
                        }
                    }
                    break;
                }
                case COMMAND_ABANDON:                       // abandon (hunter pet) or dismiss (summoned pet)
                    if (pet->GetCharmerGUID() == GetPlayer()->GetGUID())
                        _player->StopCastingCharm();
                    else if (pet->GetOwnerGUID() == GetPlayer()->GetGUID())
                    {
                        ASSERT(pet->GetTypeId() == TYPEID_UNIT);
                        if (pet->isPet())
                            GetPlayer()->RemovePet((Pet*)pet);
                        else if (pet->HasUnitTypeMask(UNIT_MASK_MINION))
                        {
                            ((Minion*)pet)->UnSummon();
                        }
                    }
                    break;
                case COMMAND_MOVE_TO:
                    pet->StopMoving();
                    pet->GetMotionMaster()->Clear(false);
                    pet->GetMotionMaster()->MovePoint(0, x, y, z);
                    charmInfo->SetCommandState(COMMAND_MOVE_TO);
                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(true);
                    charmInfo->SetIsFollowing(false);
                    charmInfo->SetIsReturning(false);
                    charmInfo->SaveStayPosition();
                    break;

                default:
                    sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
            }
            break;
        case ACT_REACTION:                                  // 0x6
            switch (spellid)
            {
                case REACT_PASSIVE:                         //passive
                    pet->AttackStop();
                    //pet->GetMotionMaster()->Clear();
                    pet->GetMotionMaster()->MoveFollow(_player, PET_FOLLOW_DIST, pet->GetFollowAngle());
                    charmInfo->SetIsReturning(true);
                case REACT_DEFENSIVE:                       //recovery
                case REACT_HELPER:
                case REACT_AGGRESSIVE:
                    if (pet->GetTypeId() == TYPEID_UNIT)
                        pet->ToCreature()->SetReactState(ReactStates(spellid));
                    break;
                default:
                    break;
            }
            break;
        case ACT_DISABLED:                                  // 0x81    spell (disabled), ignore
        case ACT_PASSIVE:                                   // 0x01
        case ACT_ENABLED:                                   // 0xC1    spell
        {
            Unit* unit_target = NULL;

            if (guid2)
                unit_target = ObjectAccessor::GetUnit(*_player, guid2);

            // do not cast unknown spells
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellid);
            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET spell id %i", spellid);
                return;
            }

            if (spellInfo->StartRecoveryCategory > 0)
                if (pet->GetCharmInfo() && pet->GetCharmInfo()->GetGlobalCooldownMgr().HasGlobalCooldown(spellInfo))
                    return;

            for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                if (spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_SRC_AREA_ENEMY || spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_DEST_AREA_ENEMY || spellInfo->Effects[i].TargetA.GetTarget() == TARGET_DEST_DYNOBJ_ENEMY)
                    return;
            }

            // do not cast not learned spells
            if (!pet->HasSpell(spellid) || spellInfo->IsPassive())
                return;

            //  Clear the flags as if owner clicked 'attack'. AI will reset them
            //  after AttackStart, even if spell failed
            if (pet->GetCharmInfo())
            {
                pet->GetCharmInfo()->SetIsAtStay(false);
                pet->GetCharmInfo()->SetIsCommandAttack(true);
                pet->GetCharmInfo()->SetIsReturning(false);
                pet->GetCharmInfo()->SetIsFollowing(false);
            }

            Spell* spell = new Spell(pet, spellInfo, TRIGGERED_NONE);

            SpellCastResult result = spell->CheckPetCast(unit_target);

            //auto turn to target unless possessed
            if (result == SPELL_FAILED_UNIT_NOT_INFRONT && !pet->isPossessed() && !pet->IsVehicle())
            {
                if (unit_target)
                {
                    pet->SetInFront(unit_target);
                    if (unit_target->GetTypeId() == TYPEID_PLAYER)
                        pet->SendUpdateToPlayer((Player*)unit_target);
                }
                else if (Unit* unit_target2 = spell->m_targets.GetUnitTarget())
                {
                    pet->SetInFront(unit_target2);
                    if (unit_target2->GetTypeId() == TYPEID_PLAYER)
                        pet->SendUpdateToPlayer((Player*)unit_target2);
                }

                if (Unit* powner = pet->GetCharmerOrOwner())
                    if (powner->GetTypeId() == TYPEID_PLAYER)
                        pet->SendUpdateToPlayer(powner->ToPlayer());

                result = SPELL_CAST_OK;
            }

            if (result == SPELL_CAST_OK)
            {
                pet->ToCreature()->AddCreatureSpellCooldown(spellid);

                unit_target = spell->m_targets.GetUnitTarget();

                //10% chance to play special pet attack talk, else growl
                //actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
                if (pet->ToCreature()->isPet() && (((Pet*)pet)->getPetType() == SUMMON_PET) && (pet != unit_target) && (urand(0, 100) < 10))
                    pet->SendPetTalk((uint32)PET_TALK_SPECIAL_SPELL);
                else
                {
                    pet->SendPetAIReaction(guid1);
                }

                if (unit_target && !GetPlayer()->IsFriendlyTo(unit_target) && !pet->isPossessed() && !pet->IsVehicle())
                {
                    // This is true if pet has no target or has target but targets differs.
                    if (pet->getVictim() != unit_target)
                    {
                        if (pet->getVictim())
                            pet->AttackStop();
                        pet->GetMotionMaster()->Clear();
                        if (pet->ToCreature()->IsAIEnabled)
                            pet->ToCreature()->AI()->AttackStart(unit_target);
                    }
                }

                spell->prepare(&(spell->m_targets));
            }
            else
            {
                Spell::SendCastResult(GetPlayer(), spellInfo, 0, result, SPELL_CUSTOM_ERROR_NONE, 0, !(pet->isPossessed() || pet->IsVehicle()));
                if (!pet->ToCreature()->HasSpellCooldown(spellid))
                    GetPlayer()->SendClearCooldown(spellid, pet);

                spell->finish(false);
                delete spell;

                // reset specific flags in case of spell fail. AI will reset other flags
                if (pet->GetCharmInfo())
                    pet->GetCharmInfo()->SetIsCommandAttack(false);
            }
            break;
        }
        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
    }
}

//! 6.0.3
void WorldSession::SendPetNameInvalid(uint32 error, ObjectGuid const& guid, const std::string& name, DeclinedName *declinedName)
{
    WorldPacket data(SMSG_PET_NAME_INVALID, 4 + name.size() + 1 + 1);
    data << uint8(error);    // unk

    data << guid;
    data << uint32(0);
    data.WriteBits(name.size(), 8);
    data.WriteBit(declinedName ? 1 : 0);

    if (declinedName)
    {
        for (uint32 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data.WriteBits(declinedName->name[i].size(), 7);

        data.FlushBits();

        for (uint32 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data.WriteString(declinedName->name[i]);
    }

    data.FlushBits();

    data.WriteString(name);

    SendPacket(&data);
}

void WorldSession::SendStablePet(ObjectGuid const& guid)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PET_DETAIL);

    stmt->setUInt64(0, _player->GetGUIDLow());

    _sendStabledPetCallback.SetParam(guid);
    _sendStabledPetCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::SendStablePetCallback(PreparedQueryResult result, ObjectGuid const& guid)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    WorldPackets::PetPackets::StableList list;
    ObjectGuid StableMaster = guid;

    std::set<uint32> stableNumber;
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            uint32 petNumber = fields[0].GetUInt32();
            PetSlot petSlot = player->GetSlotForPetId(petNumber);
            stableNumber.insert(petNumber);

            if (petSlot > PET_SLOT_STABLE_LAST)
                continue;

            if (petSlot == PET_SLOT_FULL_LIST)
                petSlot = (PetSlot)player->SetOnAnyFreeSlot(petNumber);

            if (petSlot >= PET_SLOT_HUNTER_FIRST &&  petSlot < PET_SLOT_STABLE_LAST)
            {
                WorldPackets::PetPackets::StableInfo info;
                info.PetSlot = petSlot;
                info.PetNumber = petNumber;
                info.CreatureID = fields[1].GetUInt32();
                info.DisplayID = fields[4].GetUInt32();
                info.ExperienceLevel = fields[2].GetUInt16();
                info.PetFlags = petSlot < PET_SLOT_STABLE_FIRST ? 1 : 3;
                info.PetName = fields[3].GetString();
                list.Stables.push_back(info);
            }
        }
        while (result->NextRow());
    }

    if (player->getClass() == CLASS_HUNTER)
    {
        SendPacket(list.Write());
        SendStableResult(STABLE_ERR_NONE);
    }

    PlayerPetSlotList const& petSlots = player->GetPetSlotList();
    for (uint32 i = uint32(PET_SLOT_HUNTER_FIRST); i < uint32(PET_SLOT_STABLE_LAST); ++i)
    {
        if (!petSlots[i])
            continue;

        std::set<uint32>::iterator find = stableNumber.find(petSlots[i]);
        if (find == stableNumber.end())
            player->cleanPetSlotForMove(PetSlot(i), petSlots[i]);
    }
}