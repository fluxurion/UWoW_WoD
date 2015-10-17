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
#include "DBCEnums.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "BattlePetMgr.h"
#include "Player.h"
#include "BattlePetPackets.h"

void WorldSession::HandleBattlePetSummon(WorldPackets::BattlePet::BattlePetGuidRead& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    auto* petInfo = player->GetBattlePetMgr()->GetPet(packet.BattlePetGUID);
    if (petInfo->SaveInfo == STATE_DELETED)
        return;

    uint32 spellId = petInfo->nonPacketData.SpellID;
    if (spellId && !player->HasActiveSpell(spellId))
        return;

    if (!spellId)
        spellId = SPELL_SUMMON_BATTLE_PET;

    if (player->m_SummonSlot[SUMMON_SLOT_MINIPET])
    {
        Creature* oldSummon = player->GetMap()->GetCreature(player->m_SummonSlot[SUMMON_SLOT_MINIPET]);
        if (oldSummon && oldSummon->isSummon() && oldSummon->GetGuidValue(UNIT_FIELD_BATTLE_PET_COMPANION_GUID) == packet.BattlePetGUID)
            oldSummon->ToTempSummon()->UnSummon();
        else
        {
            player->SetGuidValue(PLAYER_FIELD_SUMMONED_BATTLE_PET_GUID, packet.BattlePetGUID);
            player->CastSpell(player, spellId, true);
        }
    }
    else
    {
        player->SetGuidValue(PLAYER_FIELD_SUMMONED_BATTLE_PET_GUID, packet.BattlePetGUID);
        player->CastSpell(player, spellId, true);
    }
}

void WorldSession::HandleBattlePetNameQuery(WorldPackets::BattlePet::Query& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (Creature* summon = player->GetMap()->GetCreature(packet.UnitGUID))
    {
        if (summon->GetGuidValue(UNIT_FIELD_BATTLE_PET_COMPANION_GUID) != packet.BattlePetID)
            return;

        if (Player* owner = summon->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            if (auto petInfo = owner->GetBattlePetMgr()->GetPet(packet.BattlePetID))
            {
                if (petInfo->SaveInfo == STATE_DELETED)
                    return;

                bool hasCustomName = petInfo->PacketInfo.CustomName == "" ? false : true;

                WorldPackets::BattlePet::QueryResponse response;
                response.BattlePetID = packet.BattlePetID;
                response.CreatureID = petInfo->nonPacketData.CreatureID;
                response.Timestamp = hasCustomName ? summon->GetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP) : 0;
                response.Allow = hasCustomName;
                //response.DeclinedNames[MAX_DECLINED_NAME_CASES] = { };
                //response.HasDeclined = false;
                if (hasCustomName)
                    response.Name = petInfo->PacketInfo.CustomName;
                SendPacket(response.Write());
            }
        }
    }
}

void WorldSession::HandleModifyName(WorldPackets::BattlePet::ModifyName& packet)
{
    auto petInfo = _player->GetBattlePetMgr()->GetPet(packet.BattlePetGUID);
    if (petInfo->SaveInfo == STATE_DELETED)
        return;

    petInfo->PacketInfo.CustomName = packet.Name;
}

void WorldSession::HandleBattlePetSetFlags(WorldPackets::BattlePet::SetFlags& packet)
{
    auto petInfo = _player->GetBattlePetMgr()->GetPet(packet.BattlePetGUID);
    if (petInfo->SaveInfo == STATE_DELETED)
        return;

    if (!packet.ControlType)
        petInfo->RemoveFlag(packet.Flags);
    else
        petInfo->SetFlag(packet.Flags);
}

void WorldSession::HandleCageBattlePet(WorldPackets::BattlePet::BattlePetGuidRead& packet)
{
    if (_player->m_SummonSlot[SUMMON_SLOT_MINIPET])
    {
        Creature* oldSummon = _player->GetMap()->GetCreature(_player->m_SummonSlot[SUMMON_SLOT_MINIPET]);
        if (oldSummon && oldSummon->isSummon() && oldSummon->GetGuidValue(UNIT_FIELD_BATTLE_PET_COMPANION_GUID) == packet.BattlePetGUID)
            oldSummon->ToTempSummon()->UnSummon();
    }

    if (auto petInfo = _player->GetBattlePetMgr()->GetPet(packet.BattlePetGUID))
    {
        BattlePetSpeciesEntry const* bp = sDB2Manager.GetBattlePetSpeciesEntry(petInfo->nonPacketData.CreatureID);
        if (!bp || bp->flags & SPECIES_FLAG_CANT_TRADE)
            return;

        if (petInfo->SaveInfo == STATE_DELETED)
            return;

        uint32 itemId = ITEM_BATTLE_PET_CAGE_ID;
        uint32 count = 1;
        uint32 _noSpaceForCount = 0;
        ItemPosCountVec dest;
        InventoryResult msg = _player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, count, &_noSpaceForCount);
        if (msg != EQUIP_ERR_OK)
            count -= _noSpaceForCount;

        if (count == 0 || dest.empty())
            return;

        uint32 dynData = 0;
        dynData |= petInfo->PacketInfo.BreedQuality;
        dynData |= uint32(petInfo->PacketInfo.BreedQuality << 24);

        Item* item = _player->StoreNewItem(dest, itemId, true, 0);
        if (!item)                                               // prevent crash
            return;

        item->SetBattlePet(petInfo->PacketInfo.SpeciesID, dynData, petInfo->PacketInfo.Level);
        _player->SendNewItem(item, 1, false, true, petInfo);

        // at fourth - unlearn spell - TODO: fix it because more than one spell/battle pet of same type
        _player->removeSpell(petInfo->nonPacketData.SpellID);
        _player->GetBattlePetMgr()->DeletePetByPetGUID(packet.BattlePetGUID);
        SendPacket(WorldPackets::BattlePet::BattlePetDeleted(packet.BattlePetGUID).Write());
    }
}

void WorldSession::HandleBattlePetSetSlot(WorldPackets::BattlePet::SetBattleSlot& packet)
{
    if (packet.SlotIndex >= MAX_ACTIVE_BATTLE_PETS)
        return;

    if (BattlePetMgr::BattlePet* pet = _player->GetBattlePetMgr()->GetPet(packet.BattlePetGUID))
        _player->GetBattlePetMgr()->GetPetBattleSlot(packet.SlotIndex)->Pet = pet->PacketInfo;
}

void WorldSession::HandlePetBattleRequestWild(WorldPackets::BattlePet::RequestWild& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();
    if (!battlePetMgr)
        return;

    battlePetMgr->InitializePetBattle(packet.Battle);
}

void WorldSession::HandleReplaceFrontPet(WorldPackets::BattlePet::ReplaceFrontPet& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    PetBattle* petBattle = player->GetBattlePetMgr()->GetPetBattle();
    if (!petBattle)
        return;

    if (!packet.FrontPet)
    {
        if (!petBattle->FirstRoundHandler(packet.FrontPet, 3))
            petBattle->FinishPetBattle(true);
    }
    else
        petBattle->ForceReplacePetHandler(petBattle->GetCurrentRoundID(), packet.FrontPet, TEAM_ALLY);
}

void WorldSession::HandlePetBattleRequestUpdate(WorldPackets::BattlePet::RequestUpdate& /*packet*/)
{ }

void WorldSession::HandlePetBattleInput(WorldPackets::BattlePet::PetBattleInput& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    PetBattle* petBattle = player->GetBattlePetMgr()->GetPetBattle();
    if (!petBattle)
        return;

    if (packet.AbilityID && packet.MoveType == MOVE_TYPE_USE_ABILITY)
    {
        if (!petBattle->UseAbilityHandler(packet.AbilityID, packet.Round))
            petBattle->FinishPetBattle(true);
    }
    else if (packet.MoveType == MOVE_TYPE_SWAP_FRONT_PET && packet.NewFrontPet != -1)
    {
        if (!petBattle->SwapPetHandler(packet.NewFrontPet, packet.Round))
            petBattle->FinishPetBattle(true);
    }
    else if (packet.MoveType == MOVE_TYPE_USE_TRAP)
    {
        if (!petBattle->UseTrapHandler(packet.Round))
            petBattle->FinishPetBattle(true);
    }

    if (petBattle->NextRoundFinal() && packet.MoveType != MOVE_TYPE_REJECTION_FROM_BATTLE)
    {
        if (!petBattle->FinalRoundHandler(false))
            petBattle->FinishPetBattle(true);
    }
}

void WorldSession::HandlePetBattleFinalNotify(WorldPackets::BattlePet::NullCmsg& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (PetBattle* petBattle = player->GetBattlePetMgr()->GetPetBattle())
        petBattle->FinishPetBattle();
}

void WorldSession::HandlePetBattleQuitNotify(WorldPackets::BattlePet::NullCmsg& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (PetBattle* petBattle = player->GetBattlePetMgr()->GetPetBattle())
    {
        petBattle->SetAbandoned(true);
        petBattle->SetWinner(TEAM_ENEMY);

        if (!petBattle->FinalRoundHandler(true))
            petBattle->FinishPetBattle();
    }
}

void WorldSession::HandleBattlePetDelete(WorldPackets::BattlePet::BattlePetGuidRead& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();
    if (!battlePetMgr)
        return;

    auto petInfo = battlePetMgr->GetPet(packet.BattlePetGUID);
    if (petInfo->SaveInfo == STATE_DELETED)
        return;

    if (petInfo->PacketInfo.BattlePetDBFlags & 0xC)
        return;
        
    if (battlePetMgr->PetIsSlotted(packet.BattlePetGUID))
        return;

    battlePetMgr->DeletePetByPetGUID(packet.BattlePetGUID);
}

void WorldSession::HandleBattlePetRequestJournal(WorldPackets::BattlePet::NullCmsg& /*packet*/)
{
    _player->GetBattlePetMgr()->SendJournal();
}

void WorldSession::HandleBattlePetJournalLock(WorldPackets::BattlePet::NullCmsg& /*packet*/)
{
    // seems unused in client - there's no answer for this
}

void WorldSession::HandleJoinPetBattleQueue(WorldPackets::BattlePet::NullCmsg& /*packet*/)
{
    Player* player = GetPlayer();
    if (player)
        player->GetBattlePetMgr()->JoinPvpBattleQueue();
}

void WorldSession::HandlePetBattleScriptErrorNotify(WorldPackets::BattlePet::NullCmsg& /*packet*/)
{ }

void WorldSession::HandleBattlePetDeletePetCheat(WorldPackets::BattlePet::BattlePetGuidRead& /*packet*/)
{ }

void WorldSession::HandlePetBattleRequestPVP(WorldPackets::BattlePet::RequestPVP& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();
    if (!battlePetMgr)
        return;

    battlePetMgr->InitializePetBattle(packet.Battle);
}

void WorldSession::HanldeQueueProposeMatchResult(WorldPackets::BattlePet::QueueProposeMatchResult& /*packet*/)
{ }

void WorldSession::HandleLeaveQueue(WorldPackets::BattlePet::LeaveQueue& /*packet*/)
{
    Player* player = GetPlayer();
    if (player)
        player->GetBattlePetMgr()->LeavePvpBattleQueue();
}
