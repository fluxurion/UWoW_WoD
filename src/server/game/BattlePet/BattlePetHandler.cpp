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

    PetJournalInfo* petInfo = player->GetBattlePetMgr()->GetPetInfoByPetGUID(packet.BattlePetGUID);
    if (!petInfo || petInfo->GetState() == STATE_DELETED)
        return;

    uint32 spellId = petInfo->GetSummonSpell();
    if (spellId && !player->HasActiveSpell(spellId))
        return;

    if (!spellId)
        spellId = BATTLE_PET_SUMMON_SPELL;

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
            if (PetJournalInfo* petInfo = owner->GetBattlePetMgr()->GetPetInfoByPetGUID(packet.BattlePetID))
            {
                if (petInfo->GetState() == STATE_DELETED)
                    return;

                bool hasCustomName = petInfo->GetCustomName() == "" ? false : true;

                WorldPackets::BattlePet::QueryResponse response;
                response.BattlePetID = packet.BattlePetID;
                response.CreatureID = petInfo->GetCreatureEntry();
                response.Timestamp = hasCustomName ? summon->GetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP) : 0;
                response.Allow = hasCustomName;
                //response.DeclinedNames[MAX_DECLINED_NAME_CASES] = { };
                //response.HasDeclined = false;
                if (hasCustomName)
                    response.Name = petInfo->GetCustomName();
                SendPacket(response.Write());
            }
        }
    }
}

void WorldSession::HandleModifyName(WorldPackets::BattlePet::ModifyName& packet)
{
    PetJournalInfo* petInfo = _player->GetBattlePetMgr()->GetPetInfoByPetGUID(packet.BattlePetGUID);
    if (!petInfo || petInfo->GetState() == STATE_DELETED)
        return;

    petInfo->SetCustomName(packet.Name);
}

void WorldSession::HandleBattlePetSetFlags(WorldPackets::BattlePet::SetFlags& packet)
{
    ObjectGuid guid;
    PetJournalInfo* petInfo = _player->GetBattlePetMgr()->GetPetInfoByPetGUID(packet.BattlePetGUID);
    if (!petInfo || petInfo->GetState() == STATE_DELETED)
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

    if (PetJournalInfo* petInfo = _player->GetBattlePetMgr()->GetPetInfoByPetGUID(packet.BattlePetGUID))
    {
        BattlePetSpeciesEntry const* bp = sDB2Manager.GetBattlePetSpeciesEntry(petInfo->GetCreatureEntry());
        if (!bp || bp->flags & SPECIES_FLAG_CANT_TRADE)
            return;

        if (petInfo->GetState() == STATE_DELETED)
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
        dynData |= petInfo->GetBreedID();
        dynData |= uint32(petInfo->GetQuality() << 24);

        Item* item = _player->StoreNewItem(dest, itemId, true, 0);
        if (!item)                                               // prevent crash
            return;

        item->SetBattlePet(petInfo->GetSpeciesID(), dynData, petInfo->GetLevel());
        _player->SendNewItem(item, 1, false, true, petInfo);

        // at fourth - unlearn spell - TODO: fix it because more than one spell/battle pet of same type
        _player->removeSpell(petInfo->GetSummonSpell());
        _player->GetBattlePetMgr()->DeletePetByPetGUID(packet.BattlePetGUID);
        SendPacket(WorldPackets::BattlePet::BattlePetGuidWrite(packet.BattlePetGUID).Write()); // send packet twice? (sniff data) - check this
    }
}

void WorldSession::HandleBattlePetSetSlot(WorldPackets::BattlePet::SetBattleSlot& packet)
{
    if (packet.SlotIndex >= MAX_ACTIVE_BATTLE_PETS)
        return;

    PetBattleSlot* slot = _player->GetBattlePetMgr()->GetPetBattleSlot(packet.SlotIndex);
    if (!slot)
        return;

    PetJournalInfo* petInfo = _player->GetBattlePetMgr()->GetPetInfoByPetGUID(packet.BattlePetGUID);
    if (!petInfo || petInfo->GetState() == STATE_DELETED)
        return;

    if (!slot->IsEmpty())
    {
        for (uint8 i = 0; i < MAX_ACTIVE_BATTLE_PETS; ++i)
            if (PetBattleSlot* sourceSlot = _player->GetBattlePetMgr()->GetPetBattleSlot(i))
                if (sourceSlot->GetPet() == packet.BattlePetGUID)
                    sourceSlot->SetPet(slot->GetPet());
    }

    slot->SetPet(packet.BattlePetGUID);
}

void WorldSession::HandlePetBattleRequestWild(WorldPackets::BattlePet::RequestWild& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    uint8 reason = 0;
    if (player->GetBattlePetMgr()->AllSlotsEmpty())
        reason = 16;
    else if (player->GetBattlePetMgr()->AllSlotsDead())
        reason = 15;

    if (reason)
    {
        player->GetBattlePetMgr()->SendPetBattleRequestFailed(reason);
        return;
    }

    WorldPackets::BattlePet::FinalizeLocation finalizeLocation;
    finalizeLocation.Location.LocationResult = packet.Location.LocationResult;
    finalizeLocation.Location.BattleOrigin = packet.Location.BattleOrigin;
    finalizeLocation.Location.BattleFacing = packet.Location.BattleFacing;
    for (int i = 0; i < 2; ++i)
        finalizeLocation.Location.PlayerPositions[i] = packet.Location.PlayerPositions[i];
    SendPacket(finalizeLocation.Write());

    if (sWorld->getBoolConfig(CONFIG_PET_BATTLES_ENABLED))
        player->GetBattlePetMgr()->CreateWildBattle(player, packet.TargetGUID);
}

void WorldSession::HandlePetBattleInputFirstPet(WorldPacket& recvData)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    uint8 firstPetID;
    recvData >> firstPetID;

    PetBattleWild* petBattle = player->GetBattlePetMgr()->GetPetBattleWild();
    if (!petBattle)
        return;

    if (!firstPetID)
    {
        if (!petBattle->FirstRoundHandler(firstPetID, 3))
            petBattle->FinishPetBattle(true);
    }
    else
        petBattle->ForceReplacePetHandler(petBattle->GetCurrentRoundID(), firstPetID, TEAM_ALLY);
}

void WorldSession::HandlePetBattleRequestUpdate(WorldPackets::BattlePet::RequestUpdate& /*packet*/) { }

void WorldSession::HandlePetBattleInput(WorldPacket& recvData)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    bool bit = recvData.ReadBit();
    bool bit1 = recvData.ReadBit();
    bool bit2 = recvData.ReadBit();
    bool ignoreAbandonPenalty = recvData.ReadBit();
    bool bit4 = recvData.ReadBit();
    bool bit5 = recvData.ReadBit();
    bool bit6 = recvData.ReadBit();

    uint32 abilityID = 0;
    uint32 roundID = 0;
    uint8 moveType = 0;
    int8 newFrontPet = -1;
    uint8 battleInterrupted = 0;

    if (!bit5)
        recvData >> battleInterrupted;
    if (!bit6)
        recvData >> roundID;
    if (!bit1)
        recvData >> moveType;
    if (!bit)
        recvData >> abilityID;
    if (!bit2)
        recvData.read_skip<uint8>(); // debugFlags
    if (!bit4)
        recvData >> newFrontPet;

    PetBattleWild* petBattle = player->GetBattlePetMgr()->GetPetBattleWild();
    if (!petBattle)
        return;

    if (abilityID && moveType == 1)
    {
        if (!petBattle->UseAbilityHandler(abilityID, roundID))
            petBattle->FinishPetBattle(true);
    }
    else if (moveType == 2 && newFrontPet != -1)
    {
        if (!petBattle->SwapPetHandler(newFrontPet, roundID))
            petBattle->FinishPetBattle(true);
    }
    else if (moveType == 3)
    {
        if (!petBattle->UseTrapHandler(roundID))
            petBattle->FinishPetBattle(true);
    }

    if (petBattle->NextRoundFinal() && moveType != 4)
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

    if (PetBattleWild* petBattle = player->GetBattlePetMgr()->GetPetBattleWild())
        petBattle->FinishPetBattle();
}

void WorldSession::HandlePetBattleQuitNotify(WorldPacket& recvData)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (PetBattleWild* petBattle = player->GetBattlePetMgr()->GetPetBattleWild())
    {
        petBattle->SetAbandoned(true);
        petBattle->SetWinner(TEAM_ENEMY);

        if (!petBattle->FinalRoundHandler(true))
            petBattle->FinishPetBattle();
    }
}

void WorldSession::HandleBattlePetDelete(WorldPackets::BattlePet::BattlePetGuidRead& packet)
{
    PetJournalInfo* petInfo = _player->GetBattlePetMgr()->GetPetInfoByPetGUID(packet.BattlePetGUID);
    if (!petInfo || petInfo->GetState() == STATE_DELETED)
        return;

    if (petInfo->GetFlags() & 0xC)
        return;

    _player->GetBattlePetMgr()->DeletePetByPetGUID(packet.BattlePetGUID);
}

void WorldSession::HandleBattlePetRequestJournal(WorldPackets::BattlePet::NullCmsg& /*packet*/) { }

void WorldSession::HandleBattlePetJournalLock(WorldPackets::BattlePet::NullCmsg& /*packet*/) { }

void WorldSession::HandleJoinPetBattleQueue(WorldPackets::BattlePet::NullCmsg& /*packet*/) { }

void WorldSession::HandlePetBattleScriptErrorNotify(WorldPackets::BattlePet::NullCmsg& /*packet*/) { }

void WorldSession::HandleBattlePetDeletePetCheat(WorldPackets::BattlePet::BattlePetGuidRead& /*packet*/) { }

void WorldSession::HandlePetBattleRequestPVP(WorldPackets::BattlePet::RequestPVP& /*packet*/) { }

void WorldSession::HandleReplaceFrontPet(WorldPackets::BattlePet::ReplaceFrontPet& /*packet*/) { }

void WorldSession::HanldeQueueProposeMatchResult(WorldPackets::BattlePet::QueueProposeMatchResult& /*packet*/) { }

void WorldSession::HandleLeaveQueue(WorldPackets::BattlePet::LeaveQueue& /*packet*/) { }
