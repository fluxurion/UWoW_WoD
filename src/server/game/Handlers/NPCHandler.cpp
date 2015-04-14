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
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Player.h"
#include "GameObjectAI.h"
#include "GossipDef.h"
#include "UpdateMask.h"
#include "ObjectAccessor.h"
#include "Creature.h"
#include "Pet.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "ScriptMgr.h"
#include "CreatureAI.h"
#include "SpellInfo.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "NPCPackets.h"
#include "ItemPackets.h"
#include "GuildMgr.h"

void WorldSession::HandleTabardVendorActivateOpcode(WorldPacket & recvData)
{
    ObjectGuid guid;
    recvData >> guid.ReadAsPacked();

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_TABARDDESIGNER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleTabardVendorActivateOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(guid.GetCounter()));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendTabardVendorActivate(guid);
}

void WorldSession::SendTabardVendorActivate(ObjectGuid guid)
{
    WorldPacket data(SMSG_TABARDVENDOR_ACTIVATE, 8 + 1);
    data << guid;
    SendPacket(&data);
}

void WorldSession::HandleBankerActivateOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    recvData >> guid.ReadAsPacked();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_BANKER_ACTIVATE");

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_BANKER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleBankerActivateOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(guid.GetCounter()));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendShowBank(guid);
}

void WorldSession::SendShowBank(ObjectGuid guid)
{
    WorldPacket data(SMSG_SHOW_BANK, 8 + 1);
    data << guid;
    SendPacket(&data);
}

void WorldSession::HandleTrainerListOpcode(WorldPacket & recvData)
{
    ObjectGuid guid;
    recvData >> guid.ReadAsPacked();

    SendTrainerList(guid);
}

void WorldSession::SendTrainerList(ObjectGuid guid)
{
    std::string str = GetTrinityString(LANG_NPC_TAINER_HELLO);
    SendTrainerList(guid, str);
}

void WorldSession::SendTrainerList(ObjectGuid guid, const std::string& strTitle)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: SendTrainerList");

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_TRAINER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: SendTrainerList - Unit (GUID: %u) not found or you can not interact with him.", uint32(guid.GetCounter()));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // trainer list loaded at check;
    if (!unit->isCanTrainingOf(_player, true))
        return;

    CreatureTemplate const* ci = unit->GetCreatureTemplate();

    if (!ci)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: SendTrainerList - (GUID: %u) NO CREATUREINFO!", guid.GetCounter());
        return;
    }

    TrainerSpellData const* trainer_spells = unit->GetTrainerSpells();
    if (!trainer_spells)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: SendTrainerList - Training spells not found for creature (GUID: %u Entry: %u)",
            guid.GetCounter(), unit->GetEntry());
        return;
    }

    WorldPackets::NPC::TrainerList packet;
    packet.TrainerGUID = guid;
    packet.TrainerType = trainer_spells->trainerType;
    packet.Greeting = strTitle;

    // reputation discount
    float fDiscountMod = _player->GetReputationPriceDiscount(unit);
    bool can_learn_primary_prof = GetPlayer()->GetFreePrimaryProfessionPoints() > 0;

    packet.Spells.resize(trainer_spells->spellList.size());
    uint32 count = 0;
    for (TrainerSpellMap::const_iterator itr = trainer_spells->spellList.begin(); itr != trainer_spells->spellList.end(); ++itr)
    {
        TrainerSpell const* tSpell = &itr->second;

        bool valid = true;
        bool primary_prof_first_rank = false;
        for (uint8 i = 0; i < MAX_TRAINERSPELL_ABILITY_REQS; ++i)
        {
            if (!tSpell->learnedSpell[i])
                continue;

            if (!_player->IsSpellFitByClassAndRace(tSpell->learnedSpell[i]))
            {
                valid = false;
                break;
            }
            SpellInfo const* learnedSpellInfo = sSpellMgr->GetSpellInfo(tSpell->learnedSpell[i]);
            if (learnedSpellInfo && learnedSpellInfo->IsPrimaryProfessionFirstRank())
                primary_prof_first_rank = true;
        }

        if (!valid)
            continue;

        if(SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(tSpell->spell))
        {
            if(spellInfo->AttributesEx7 & SPELL_ATTR7_HORDE_ONLY && GetPlayer()->GetTeam() != HORDE)
                continue;
            if(spellInfo->AttributesEx7 & SPELL_ATTR7_ALLIANCE_ONLY && GetPlayer()->GetTeam() != ALLIANCE)
                continue;
        }

        TrainerSpellState state = _player->GetTrainerSpellState(tSpell);

        WorldPackets::NPC::TrainerListSpell& spell = packet.Spells[count];
        spell.SpellID = tSpell->spell;
        spell.MoneyCost = floor(tSpell->spellCost * fDiscountMod);
        spell.ReqSkillLine = tSpell->reqSkill;
        spell.ReqSkillRank = tSpell->reqSkillValue;
        spell.ReqLevel = tSpell->reqLevel;
        spell.Usable = (state == TRAINER_SPELL_GREEN_DISABLED ? TRAINER_SPELL_GREEN : state);

        uint8 maxReq = 0;
        for (uint8 i = 0; i < MAX_TRAINERSPELL_ABILITY_REQS; ++i)
        {
            if (!tSpell->learnedSpell[i])
                continue;

            if (maxReq > MAX_TRAINERSPELL_ABILITY_REQS)
                break;

            if (uint32 prevSpellId = sSpellMgr->GetPrevSpellInChain(tSpell->learnedSpell[i]))
            {
                spell.ReqAbility[maxReq++] = prevSpellId;
                continue;
            }
            SpellsRequiringSpellMapBounds spellsRequired = sSpellMgr->GetSpellsRequiredForSpellBounds(tSpell->learnedSpell[i]);
            for (SpellsRequiringSpellMap::const_iterator itr2 = spellsRequired.first; itr2 != spellsRequired.second; ++itr2)
            {
                spell.ReqAbility[maxReq++] = itr2->second;
                continue;
            }
        }
        ++count;
    }

    // Shrink to actual data size
    packet.Spells.resize(count);

    SendPacket(packet.Write());
}

void WorldSession::HandleTrainerBuySpellOpcode(WorldPacket & recvData)
{
    ObjectGuid guid;
    uint32 spellId;
    int32 trainerId;

    recvData >> guid >> trainerId >> spellId;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_TRAINER_BUY_SPELL NpcGUID=%u, learn spell id is: %u", uint32(guid.GetCounter()), spellId);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_TRAINER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleTrainerBuySpellOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(guid.GetCounter()));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (!unit->isCanTrainingOf(_player, true))
    { 
        SendTrainerService(guid, spellId, 0);
        return;
    }

    // check present spell in trainer spell list
    TrainerSpellData const* trainer_spells = unit->GetTrainerSpells();
    if (!trainer_spells)
    {
        SendTrainerService(guid, spellId, 0);
        return;
    }

    // not found, cheat?
    TrainerSpell const* trainer_spell = trainer_spells->Find(spellId);
    if (!trainer_spell)
    { 
        SendTrainerService(guid, spellId, 0);
        return;
    }

    // can't be learn, cheat? Or double learn with lags...
    if (_player->GetTrainerSpellState(trainer_spell) != TRAINER_SPELL_GREEN)
    { 
        SendTrainerService(guid, spellId, 0);
        return;
    }

    // apply reputation discount
    uint32 nSpellCost = uint32(floor(trainer_spell->spellCost * _player->GetReputationPriceDiscount(unit)));

    // check money requirement
    if (!_player->HasEnoughMoney(uint64(nSpellCost)))
    { 
        SendTrainerService(guid, spellId, 1);
        return;
    }

    _player->ModifyMoney(-int64(nSpellCost));

    unit->SendPlaySpellVisualKit(179, 0);       // 53 SpellCastDirected
    _player->SendPlaySpellVisualKit(362, 1);    // 113 EmoteSalute

    // learn explicitly or cast explicitly
    if (trainer_spell->IsCastable())
        _player->CastSpell(_player, trainer_spell->spell, true);
    else
        _player->learnSpell(spellId, false);

    SendTrainerService(guid, spellId, 2);
}

void WorldSession::SendTrainerService(ObjectGuid guid, uint32 spellId, uint32 result)
{ 
    WorldPacket data(SMSG_TRAINER_SERVICE, 16);
    data << guid;
    data << uint32(spellId);        // should be same as in packet from client
    data << uint32(result);         // 2 == Success. 1 == "Not enough money for trainer service." 0 == "Trainer service %d unavailable."
    SendPacket(&data);
}

void WorldSession::HandleGossipHelloOpcode(WorldPackets::NPC::Hello& packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GOSSIP_HELLO");

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(packet.Unit, UNIT_NPC_FLAG_NONE);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGossipHelloOpcode - Unit (GUID: %s) not found or you can not interact with him.", packet.Unit.ToString().c_str());
        return;
    }

    // set faction visible if needed
    if (FactionTemplateEntry const* factionTemplateEntry = sFactionTemplateStore.LookupEntry(unit->getFaction()))
        _player->GetReputationMgr().SetVisible(factionTemplateEntry);

    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_TALK);
    // remove fake death
    //if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
    //    GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (unit->isArmorer() || unit->isCivilian() || unit->isQuestGiver() || unit->isServiceProvider() || unit->isGuard())
        unit->StopMoving();

    // If spiritguide, no need for gossip menu, just put player into resurrect queue
    if (unit->isSpiritGuide())
    {
        Battleground* bg = _player->GetBattleground();
        if (bg)
        {
            bg->AddPlayerToResurrectQueue(unit->GetGUID(), _player->GetGUID());
            sBattlegroundMgr->SendAreaSpiritHealerQueryOpcode(_player, bg, unit->GetGUID());
            return;
        }
    }

    if (!sScriptMgr->OnGossipHello(_player, unit))
    {
//        _player->TalkedToCreature(unit->GetEntry(), unit->GetGUID());
        _player->PrepareGossipMenu(unit, unit->GetCreatureTemplate()->GossipMenuId, true);
        _player->SendPreparedGossip(unit);
    }

    unit->AI()->sGossipHello(_player);
}

void WorldSession::HandleGossipSelectOptionOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_GOSSIP_SELECT_OPTION");

    ObjectGuid guid;
    uint32 gossipListId;
    uint32 menuId;

    recvData >> guid >> menuId >> gossipListId;

    std::string code = recvData.ReadString(recvData.ReadBits(8));

    Creature* unit = NULL;
    GameObject* go = NULL;
    if (guid.IsCreatureOrVehicle())
    {
        unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
        if (!unit)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGossipSelectOptionOpcode - Unit (GUID: %u) not found or you can't interact with him.", uint32(guid.GetCounter()));
            return;
        }
    }
    else if (guid.IsGameObject())
    {
        go = _player->GetMap()->GetGameObject(guid);
        if (!go)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGossipSelectOptionOpcode - GameObject (GUID: %u) not found.", uint32(guid.GetCounter()));
            return;
        }
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGossipSelectOptionOpcode - unsupported GUID type for highguid %u. lowpart %u.", guid.GetHigh(), guid.GetCounter());
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if ((unit && unit->GetCreatureTemplate()->ScriptID != unit->LastUsedScriptID) || (go && go->GetGOInfo()->ScriptId != go->LastUsedScriptID))
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGossipSelectOptionOpcode - Script reloaded while in use, ignoring and set new scipt id");
        if (unit)
            unit->LastUsedScriptID = unit->GetCreatureTemplate()->ScriptID;
        if (go)
            go->LastUsedScriptID = go->GetGOInfo()->ScriptId;
        _player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    if (!code.empty())
    {
        if (unit)
        {
            unit->AI()->sGossipSelectCode(_player, menuId, gossipListId, code.c_str());
            if (!sScriptMgr->OnGossipSelectCode(_player, unit, _player->PlayerTalkClass->GetGossipOptionSender(gossipListId), _player->PlayerTalkClass->GetGossipOptionAction(gossipListId), code.c_str()))
                _player->OnGossipSelect(unit, gossipListId, menuId);
        }
        else
        {
            go->AI()->GossipSelectCode(_player, menuId, gossipListId, code.c_str());
            sScriptMgr->OnGossipSelectCode(_player, go, _player->PlayerTalkClass->GetGossipOptionSender(gossipListId), _player->PlayerTalkClass->GetGossipOptionAction(gossipListId), code.c_str());
        }
    }
    else
    {
        if (unit)
        {
            unit->AI()->sGossipSelect(_player, menuId, gossipListId);
            if (!sScriptMgr->OnGossipSelect(_player, unit, _player->PlayerTalkClass->GetGossipOptionSender(gossipListId), _player->PlayerTalkClass->GetGossipOptionAction(gossipListId)))
                _player->OnGossipSelect(unit, gossipListId, menuId);
        }
        else
        {
            go->AI()->GossipSelect(_player, menuId, gossipListId);
            if (!sScriptMgr->OnGossipSelect(_player, go, _player->PlayerTalkClass->GetGossipOptionSender(gossipListId), _player->PlayerTalkClass->GetGossipOptionAction(gossipListId)))
                _player->OnGossipSelect(go, gossipListId, menuId);
        }
    }
}

void WorldSession::HandleSpiritHealerActivateOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_SPIRIT_HEALER_ACTIVATE");

    ObjectGuid guid;
    recvData >> guid;

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_SPIRITHEALER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleSpiritHealerActivateOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(guid.GetCounter()));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendSpiritResurrect();
}

void WorldSession::SendSpiritResurrect()
{
    _player->ResurrectPlayer(0.5f, true);

    _player->DurabilityLossAll(0.25f, true, false);

    // get corpse nearest graveyard
    WorldSafeLocsEntry const* corpseGrave = NULL;
    Corpse* corpse = _player->GetCorpse();
    if (corpse)
        corpseGrave = sObjectMgr->GetClosestGraveYard(
            corpse->GetPositionX(), corpse->GetPositionY(), corpse->GetPositionZ(), corpse->GetMapId(), _player->GetTeam());

    // now can spawn bones
    _player->SpawnCorpseBones();

    // teleport to nearest from corpse graveyard, if different from nearest to player ghost
    if (corpseGrave)
    {
        WorldSafeLocsEntry const* ghostGrave = sObjectMgr->GetClosestGraveYard(
            _player->GetPositionX(), _player->GetPositionY(), _player->GetPositionZ(), _player->GetMapId(), _player->GetTeam());

        if (corpseGrave != ghostGrave)
            _player->TeleportTo(corpseGrave->MapID, corpseGrave->Loc.X, corpseGrave->Loc.Y, corpseGrave->Loc.Z, _player->GetOrientation());
        // or update at original position
        else
            _player->UpdateObjectVisibility();
    }
    // or update at original position
    else
        _player->UpdateObjectVisibility();
}

void WorldSession::HandleBinderActivateOpcode(WorldPacket& recvData)
{
    ObjectGuid npcGUID;
    recvData >> npcGUID;

    if (!GetPlayer()->IsInWorld() || !GetPlayer()->isAlive())
        return;

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(npcGUID, UNIT_NPC_FLAG_INNKEEPER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleBinderActivateOpcode - Unit (GUID: %u) not found or you can not interact with him.", npcGUID.GetCounter());
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendBindPoint(unit);
}

void WorldSession::SendBindPoint(Creature* npc)
{
    // prevent set homebind to instances in any case
    if (GetPlayer()->GetMap()->Instanceable())
        return;

    uint32 bindspell = 3286;

    // update sql homebind
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_PLAYER_HOMEBIND);
    stmt->setUInt16(0, _player->GetMapId());
    stmt->setUInt16(1, _player->GetAreaId());
    stmt->setFloat (2, _player->GetPositionX());
    stmt->setFloat (3, _player->GetPositionY());
    stmt->setFloat (4, _player->GetPositionZ());
    stmt->setUInt64(5, _player->GetGUID().GetCounter());
    CharacterDatabase.Execute(stmt);

    _player->m_homebindMapId = _player->GetMapId();
    _player->m_homebindAreaId = _player->GetAreaId();
    _player->m_homebindX = _player->GetPositionX();
    _player->m_homebindY = _player->GetPositionY();
    _player->m_homebindZ = _player->GetPositionZ();

    // send spell for homebinding (3286)
    _player->CastSpell(_player, bindspell, true);

    SendTrainerService(npc->GetGUID(), bindspell, 2);
    _player->PlayerTalkClass->SendCloseGossip();
}

void WorldSession::HandleRepairItemOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_REPAIR_ITEM");

    ObjectGuid npcGUID, itemGUID;
    bool guildBank;                                         // new in 2.3.2, bool that means from guild bank money
    recvData >> npcGUID >> itemGUID;
    guildBank = recvData.ReadBit();

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(npcGUID, UNIT_NPC_FLAG_REPAIR);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleRepairItemOpcode - Unit (GUID: %u) not found or you can not interact with him.", npcGUID.GetCounter());
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // reputation discount
    float discountMod = _player->GetReputationPriceDiscount(unit);

    if (itemGUID)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "ITEM: Repair item, itemGUID = %u, npcGUID = %u", itemGUID.GetCounter(), npcGUID.GetCounter());

        Item* item = _player->GetItemByGuid(itemGUID);
        if (item)
            _player->DurabilityRepair(item->GetPos(), true, discountMod, guildBank != 0);
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "ITEM: Repair all items, npcGUID = %u", npcGUID.GetCounter());
        _player->DurabilityRepairAll(true, discountMod, guildBank != 0);
    }
}

void WorldSession::HandleListInventoryOpcode(WorldPacket & recvData)
{
    ObjectGuid guid;
    recvData >> guid;

    if (!GetPlayer()->isAlive())
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_LIST_INVENTORY");

    SendListInventory(guid);
}

void WorldSession::SendListInventory(ObjectGuid vendorGuid)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_LIST_INVENTORY");

    Creature* vendor = GetPlayer()->GetNPCIfCanInteractWith(vendorGuid, UNIT_NPC_FLAG_VENDOR);
    if (!vendor)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: SendListInventory - Unit (GUID: %u) not found or you can not interact with him.", vendorGuid.GetCounter());
        _player->SendSellError(SELL_ERR_VENDOR_HATES_YOU, NULL, ObjectGuid::Empty);
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // Stop the npc if moving
    if (vendor->HasUnitState(UNIT_STATE_MOVING))
        vendor->StopMoving();

    VendorItemData const* vendorItems = vendor->GetVendorItems();
    uint32 rawItemCount = vendorItems ? vendorItems->GetItemCount() : 0;

    //if (rawItemCount > 300),
    //    rawItemCount = 300; // client cap but uint8 max value is 255

    WorldPackets::NPC::VendorInventory packet;
    packet.Vendor = vendor->GetGUID();

    packet.Items.resize(rawItemCount);

    const float discountMod = _player->GetReputationPriceDiscount(vendor);
    uint32 realCount = 0;
    for (uint32 slot = 0; slot < rawItemCount; ++slot)
    {
        if (realCount >= MAX_VENDOR_ITEMS)
            break;

        VendorItem const* vendorItem = vendorItems->GetItem(slot);
        if (!vendorItem)
            continue;

        WorldPackets::NPC::VendorItem& item = packet.Items[realCount];

        if (vendorItem->Type == ITEM_VENDOR_TYPE_ITEM)
        {
            ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(vendorItem->item);
            if (!itemTemplate) 
                continue;

            uint32 leftInStock = vendorItem->maxcount <= 0 ? 0xFFFFFFFF : vendor->GetVendorItemCurrentCount(vendorItem);
            if (!_player->isGameMaster()) // ignore conditions if GM on
            {
                // Items sold out are not displayed in list
                if (leftInStock == 0)
                    continue;

                ConditionList conditions = sConditionMgr->GetConditionsForNpcVendorEvent(vendor->GetEntry(), vendorItem->item);
                if (!sConditionMgr->IsObjectMeetToConditions(_player, vendor, conditions))
                {
                    sLog->outDebug(LOG_FILTER_CONDITIONSYS, "SendListInventory: conditions not met for creature entry %u item %u", vendor->GetEntry(), vendorItem->item);
                    continue;
                }

                // Respect allowed class
                if (!(itemTemplate->AllowableClass & _player->getClassMask()) && itemTemplate->Bonding == BIND_WHEN_PICKED_UP)
                    continue;

                // Custom MoP Script for Pandarens Mounts (Alliance)
                if (itemTemplate->Class == 15 && itemTemplate->SubClass == 5 && _player->getRace() != RACE_PANDAREN_ALLI
                    && _player->getRace() != RACE_PANDAREN_HORDE && _player->getRace() != RACE_PANDAREN_NEUTRAL
                    && vendor->GetEntry() == 65068 && _player->GetReputationRank(1353) != REP_EXALTED)
                    continue;

                // Custom MoP Script for Pandarens Mounts (Horde)
                if (itemTemplate->Class == 15 && itemTemplate->SubClass == 5 && _player->getRace() != RACE_PANDAREN_ALLI
                    && _player->getRace() != RACE_PANDAREN_HORDE && _player->getRace() != RACE_PANDAREN_NEUTRAL
                    && vendor->GetEntry() == 66022 && _player->GetReputationRank(1352) != REP_EXALTED)
                    continue;

                // Only display items in vendor lists for the team the player is on
                if ((itemTemplate->Flags2 & ITEM_FLAGS_EXTRA_HORDE_ONLY && _player->GetTeam() == ALLIANCE) ||
                    (itemTemplate->Flags2 & ITEM_FLAGS_EXTRA_ALLIANCE_ONLY && _player->GetTeam() == HORDE))
                    continue;

                std::vector<GuildReward> const& rewards = sGuildMgr->GetGuildRewards();
                bool guildRewardCheckPassed = true;

                for (std::vector<GuildReward>::const_iterator reward = rewards.begin(); reward != rewards.end(); ++reward)
                {
                    if (itemTemplate->ItemId != reward->Entry)
                        continue;

                    Guild* guild = sGuildMgr->GetGuildById(_player->GetGuildId());

                    if (!guild)
                    {
                        guildRewardCheckPassed = false;
                        break;
                    }

                    if (reward->Standing && _player->GetReputationRank(REP_GUILD) < reward->Standing)
                    {
                        guildRewardCheckPassed = false;
                        break;
                    }

                    if (reward->AchievementId && !guild->GetAchievementMgr().HasAchieved(reward->AchievementId))
                    {
                        guildRewardCheckPassed = false;
                        break;
                    }

                    if (reward->Racemask)
                    {
                        if (!(_player->getRaceMask() & reward->Racemask))
                        {
                            guildRewardCheckPassed = false;
                            break;
                        }
                    }
                }

                if (!guildRewardCheckPassed)
                    continue;
            }

            // reputation discount
            int32 price = vendorItem->IsGoldRequired(itemTemplate) ? uint32(itemTemplate->BuyPrice/* * discountMod*/) : 0;

            //if (int32 priceMod = _player->GetTotalAuraModifier(SPELL_AURA_MOD_VENDOR_ITEMS_PRICES))
                 //price -= CalculatePct(price, priceMod);

            // if (!unk "enabler") data << uint32(something);
            if (vendorItem->ExtendedCost != 0)
            {
                //Hack for donate
                if(vendorItem->ExtendedCost > 14999)
                {
                    if(ItemExtendedCostEntry const* iece = sItemExtendedCostStore.LookupEntry(vendorItem->ExtendedCost))
                        price = uint32(iece->RequiredItemCount[0] * 10000 * sWorld->getRate(RATE_DONATE));
                }
            }

            item.MuID = slot + 1; // client expects counting to start at 1
            item.Durability = itemTemplate->MaxDurability ? itemTemplate->MaxDurability : -1;
            item.ExtendedCostID = vendorItem->ExtendedCost;
            item.Type = vendorItem->Type;
            item.Quantity = leftInStock;
            item.StackCount = itemTemplate->BuyCount;
            item.Price = price;

            item.Item.ItemID = vendorItem->item;
        }
        else if (vendorItem->Type == ITEM_VENDOR_TYPE_CURRENCY)
        {
            CurrencyTypesEntry const* currencyTemplate = sCurrencyTypesStore.LookupEntry(vendorItem->item);

            if (!currencyTemplate)
                continue;

            if (vendorItem->ExtendedCost == 0)
                continue; // there's no price defined for currencies, only extendedcost is used

            uint32 precision = currencyTemplate->GetPrecision();

            item.MuID = slot + 1; // client expects counting to start at 1
            item.ExtendedCostID = vendorItem->ExtendedCost;
            item.Item.ItemID = vendorItem->item;
            item.Type = vendorItem->Type;
            item.StackCount = vendorItem->maxcount * precision;
        }

        if (++realCount >= MAX_VENDOR_ITEMS)
            break;
    }

    // It doesn't matter what value is used here (PROBABLY its full vendor size)
    // What matters is that if count of items we can see is 0 and this field is 1
    // then client will open the vendor list, otherwise it won't
    //if (rawItemCount)
    //    data << uint8(rawItemCount);
    //else
    //    data << uint8(vendor->isArmorer());

    // Resize vector to real size (some items can be skipped due to checks)
    packet.Items.resize(realCount);

    SendPacket(packet.Write());
}
