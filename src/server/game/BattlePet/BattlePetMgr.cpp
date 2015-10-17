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

#include "WorldPacket.h"
#include "Player.h"
#include "BattlePetMgr.h"
#include "BattlePetPackets.h"

void BattlePetMgr::Initialize()
{ }

BattlePetMgr::BattlePetMgr(Player* owner)
{
    for (uint8 i = 0; i < MAX_ACTIVE_BATTLE_PETS; ++i)
    {
        WorldPackets::BattlePet::BattlePetSlot slot;
        slot.SlotIndex = i;
        _slots.push_back(slot);
    }

    _petBattle = nullptr;
    _player = owner;
    _queueTimer = 0;
    _waitTimesAvgStore = 0;
}

BattlePetMgr::~BattlePetMgr()
{
    _compatibleMapStore.clear();
    _queueDataStore.clear();
    _currentQueueStore.clear();
    _newToQueueStore.clear();
}

void BattlePetMgr::LoadFromDB(PreparedQueryResult pets, PreparedQueryResult slots)
{
    if (pets)
    {
        do
        {
            // SELECT guid, species, breed, level, exp, health, maxHealth, speed, power, collar, quality, flags, name, creatureID, spellID
            Field* fields = pets->Fetch();
            uint32 speciesID = fields[1].GetUInt32();

            if (BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(speciesID))
            {
                if (GetPetCount(speciesID) >= 3)
                    continue;

                BattlePet pet;
                pet.PacketInfo.BattlePetGUID = ObjectGuid::Create<HighGuid::BattlePet>(fields[0].GetUInt64());
                pet.PacketInfo.SpeciesID = speciesID;
                pet.PacketInfo.BreedID = fields[2].GetUInt16();
                pet.PacketInfo.Level = fields[3].GetUInt16();
                pet.PacketInfo.Xp = fields[4].GetUInt16();
                pet.PacketInfo.Health = fields[5].GetUInt32();
                pet.PacketInfo.MaxHealth = fields[6].GetUInt32();
                pet.PacketInfo.Speed = fields[7].GetUInt32();
                pet.PacketInfo.Power = fields[8].GetUInt32();
                pet.PacketInfo.CollarID = fields[9].GetUInt32();
                pet.PacketInfo.BreedQuality = fields[10].GetUInt8();
                pet.PacketInfo.BattlePetDBFlags = fields[11].GetUInt16();
                pet.PacketInfo.CustomName = fields[12].GetString();
                pet.PacketInfo.DisplayID = speciesEntry->CreatureEntry;
                pet.nonPacketData.CreatureID = fields[13].GetUInt32();
                pet.nonPacketData.SpellID = fields[14].GetUInt32();

                pet.PacketInfo.OwnerGuid = _player->GetGUID();
                pet.PacketInfo.NoRename = false;
                pet.SaveInfo = STATE_NORMAL;

                // recalculate stats after change breed
                if (!pet.PacketInfo.BreedID)
                {
                    pet.PacketInfo.BreedID = GetRandomBreedID(pet.PacketInfo.SpeciesID);
                    if (!pet.PacketInfo.BreedID)
                        continue;

                    float pct = float(pet.PacketInfo.Health * 100.0f) / pet.PacketInfo.MaxHealth;
                    CalculateStats(pet.PacketInfo.SpeciesID, pet.PacketInfo.BreedID, pet.PacketInfo.BreedQuality, pet.PacketInfo.Level, pet.PacketInfo.MaxHealth, pet.PacketInfo.Power, pet.PacketInfo.Speed);
                    if (pct != 100.0f)
                        pet.PacketInfo.Health = int32(pet.PacketInfo.MaxHealth * float(pct / 100.0f));
                    else
                        pet.PacketInfo.Health = pet.PacketInfo.MaxHealth;
                }

                // prevent some undefined behavoir
                if (pet.PacketInfo.Health > pet.PacketInfo.MaxHealth)
                    pet.PacketInfo.Health = pet.PacketInfo.MaxHealth;

                _pets[pet.PacketInfo.BattlePetGUID.GetCounter()] = pet;

                _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ADD_BATTLE_PET_JOURNAL, pet.nonPacketData.CreatureID);
            }
        }
        while (pets->NextRow());
    }

    if (slots)
    {
        uint8 i = 0;
        do
        {
            Field* fields = slots->Fetch();
            _slots[i].SlotIndex = fields[0].GetUInt8();
            auto itr = _pets.find(fields[1].GetUInt64());
            if (itr != _pets.end())
                _slots[i].Pet = itr->second.PacketInfo;
            _slots[i].Locked = fields[2].GetBool();
            i++;
        }
        while (slots->NextRow());
    }
}

void BattlePetMgr::SaveToDB(SQLTransaction& trans)
{
    PreparedStatement* stmt = nullptr;
    uint32 accountID = _player->GetSession()->GetAccountId();
    if (accountID == 0)
        return;

    for (auto itr = _pets.begin(); itr != _pets.end();)
    {
        switch (itr->second.SaveInfo)
        {
            case STATE_NEW:
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ACCOUNT_BATTLE_PET_LIST);
                stmt->setUInt64(0, itr->first);
                stmt->setUInt32(1, accountID);
                stmt->setUInt32(2, itr->second.PacketInfo.SpeciesID);
                stmt->setUInt16(3, itr->second.PacketInfo.BreedID);
                stmt->setUInt16(4, itr->second.PacketInfo.Level);
                stmt->setUInt16(5, itr->second.PacketInfo.Xp);
                stmt->setUInt32(6, itr->second.PacketInfo.Health);
                stmt->setUInt32(7, itr->second.PacketInfo.MaxHealth);
                stmt->setUInt32(8, itr->second.PacketInfo.Speed);
                stmt->setUInt32(9, itr->second.PacketInfo.Power);
                stmt->setUInt8(10, itr->second.PacketInfo.BreedQuality);
                stmt->setUInt16(11, itr->second.PacketInfo.BattlePetDBFlags);
                stmt->setString(12, itr->second.PacketInfo.CustomName);
                trans->Append(stmt);
                itr->second.SaveInfo = STATE_NORMAL;
                ++itr;
                break;
            case STATE_DELETED:
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ACCOUNT_BATTLE_PET_LIST);
                stmt->setUInt32(0, accountID);
                stmt->setUInt64(1, itr->first);
                trans->Append(stmt);
                itr = _pets.erase(itr);
                break;
            case STATE_UPDATED:
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ACCOUNT_BATTLE_PET_LIST);
                stmt->setUInt16(0, itr->second.PacketInfo.Level);
                stmt->setUInt16(1, itr->second.PacketInfo.Xp);
                stmt->setUInt32(2, itr->second.PacketInfo.Health);
                stmt->setUInt32(3, itr->second.PacketInfo.MaxHealth);
                stmt->setUInt32(4, itr->second.PacketInfo.Speed);
                stmt->setUInt32(5, itr->second.PacketInfo.Power);
                stmt->setUInt32(6, itr->second.PacketInfo.CollarID);
                stmt->setUInt8(7, itr->second.PacketInfo.BreedQuality);
                stmt->setUInt16(8, itr->second.PacketInfo.BattlePetDBFlags);
                stmt->setString(9, itr->second.PacketInfo.CustomName);
                stmt->setUInt32(10, accountID);
                stmt->setUInt64(11, itr->first);
                trans->Append(stmt);
                itr->second.SaveInfo = STATE_NORMAL;
                ++itr;
                break;
            default:
                ++itr;
                break;
        }
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ACCOUNT_BATTLE_PET_SLOTS);
    stmt->setUInt32(0, accountID);
    trans->Append(stmt);

    for (auto const& slot : _slots)
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ACCOUNT_BATTLE_PET_SLOTS);
        stmt->setUInt8(0, slot.SlotIndex);
        stmt->setUInt32(1, accountID);
        stmt->setUInt64(2, slot.Pet.BattlePetGUID.GetCounter());
        stmt->setBool(3, slot.Locked);
        trans->Append(stmt);
    }
}

int32 BattlePetMgr::BattlePet::GetBaseStateValue(BattlePetState state)
{
    int32 value = 0;

    auto breedState = sDB2Manager._battlePetBreedStateByBreedId.equal_range(PacketInfo.BreedID);
    for (auto itr = breedState.first; itr != breedState.second; ++itr)
        value = std::get<1>(itr->second);

    auto speciesState = sDB2Manager._battlePetBreedStateByBreedId.equal_range(PacketInfo.SpeciesID);
    for (auto itr1 = speciesState.first; itr1 != speciesState.second; ++itr1)
        value += std::get<1>(itr1->second);

    return value;
}

void BattlePetMgr::CalculateStats(uint32 speciesID, uint16 breedID, uint8 quality, uint8 lvl, uint32& maxHealth, uint32& power, uint32& speed)
{
    uint32 healthMod = 0;
    uint32 powerMod = 0;
    uint32 speedMod = 0;
    uint32 val = 0;

    auto bounds = sDB2Manager._battlePetSpeciesStateBySpecId.equal_range(speciesID);
    for (auto itr = bounds.first; itr != bounds.second; ++itr)
    {
        BattlePetStateEntry const* state = sBattlePetStateStore.LookupEntry(std::get<0>(itr->second));
        if (!state)
            break;

        switch (state->flags)
        {
            val = std::get<1>(itr->second);
            case STATE_STAT_SPEED:
                speedMod += val;
                break;
            case STATE_STAT_STAMINA:
                healthMod += val;
                break;
            case STATE_STAT_POWER:
                powerMod += val;
                break;
            default:
                break;
        }
    }

    auto bounds1 = sDB2Manager._battlePetBreedStateByBreedId.equal_range(breedID);
    for (auto itr1 = bounds1.first; itr1 != bounds1.second; ++itr1)
    {
        BattlePetStateEntry const* state = sBattlePetStateStore.LookupEntry(std::get<0>(itr1->second));
        if (!state)
            break;

        switch (state->flags)
        {
            val = std::get<1>(itr1->second);
            case STATE_STAT_SPEED:
                speedMod += val;
                break;
            case STATE_STAT_STAMINA:
                healthMod += val;
                break;
            case STATE_STAT_POWER:
                powerMod += val;
                break;
            default:
                break;
        }
    }

    float multiplier = sDB2Manager.CalcBattlePetQualityMuliplier(quality, lvl);

    maxHealth = (5.0f * healthMod * multiplier) + 100.0f + 0.5f;
    power = (speedMod * multiplier) + 0.5f;
    speed = (powerMod * multiplier) + 0.5f;
}

void BattlePetMgr::AddPet(uint32 species, uint16 breed, uint8 quality, uint16 level /*= 1*/, BattlePetSaveInfo state /*= STATE_NEW*/)
{
    BattlePetSpeciesEntry const* speciesStore = sBattlePetSpeciesStore.LookupEntry(species);
    if (!speciesStore)
        return;

    BattlePet pet;
    pet.PacketInfo.BattlePetGUID = ObjectGuid::Create<HighGuid::BattlePet>(sObjectMgr->GetGenerator<HighGuid::BattlePet>()->Generate());
    pet.PacketInfo.SpeciesID = species;
    pet.nonPacketData.CreatureID = speciesStore->CreatureEntry;
    pet.PacketInfo.Level = level;
    pet.PacketInfo.BreedID = breed;
    pet.PacketInfo.BreedQuality = quality;
    pet.PacketInfo.CustomName.clear();
    pet.PacketInfo.Health = pet.PacketInfo.MaxHealth;
    CalculateStats(species, breed, quality, level, pet.PacketInfo.Health, pet.PacketInfo.Power, pet.PacketInfo.Speed);
    pet.SaveInfo = state;
    _pets[pet.PacketInfo.BattlePetGUID.GetCounter()] = pet;

    std::vector<BattlePet> updates;
    updates.push_back(pet);
    SendUpdatePets(updates, true);
}

BattlePetMgr::BattlePet* BattlePetMgr::GetPet(ObjectGuid guid)
{
    auto itr = _pets.find(guid.GetCounter());
    if (itr != _pets.end())
        return &itr->second;

    return nullptr;
}

void BattlePetMgr::UnlockSlot(uint8 slot)
{
    if (!_slots[slot].Locked)
        return;

    _slots[slot].Locked = false;

    WorldPackets::BattlePet::SlotUpdates updates;
    updates.Slots.push_back(_slots[slot]);
    updates.AutoSlotted = false; //@TODO: find some pet flags for this...
    updates.NewSlotUnlocked = true;
    _player->GetSession()->SendPacket(updates.Write());
}

bool BattlePetMgr::SlotIsLocked(uint8 index)
{
    if (index > MAX_ACTIVE_BATTLE_PETS)
        return true;

    switch (index)
    {
        case 0:
            return !_player->HasSpell(SPELL_BATTLE_PET_TRAINING);
        case 1:
            return !_player->HasAchieved(7433);
        case 2:
            return !_player->HasAchieved(6566);
    }

    return true;
}

void BattlePetMgr::ClosePetBattle()
{
    delete _petBattle;
    _petBattle = nullptr;
}

void BattlePetMgr::SendJournal()
{
    WorldPackets::BattlePet::BattlePetJournal battlePetJournal;
    battlePetJournal.TrapLevel = 0;
    battlePetJournal.HasJournalLock = _player->HasSpell(SPELL_BATTLE_PET_TRAINING);
   
    battlePetJournal.Slots = GetPetBattleSlots();

    for (auto i : _pets)
    {
        if (i.second.SaveInfo == STATE_DELETED)
            continue;

        WorldPackets::BattlePet::BattlePet pet;
        pet.BattlePetGUID = ObjectGuid::Create<HighGuid::BattlePet>(i.first);
        pet.SpeciesID = i.second.PacketInfo.SpeciesID;
        pet.DisplayID = i.second.PacketInfo.DisplayID;
        pet.CollarID = i.second.PacketInfo.CollarID;
        pet.BreedID = i.second.PacketInfo.BreedID;
        pet.Level = i.second.PacketInfo.Level;
        pet.Xp = i.second.PacketInfo.Xp;
        pet.BattlePetDBFlags = i.second.PacketInfo.BattlePetDBFlags;
        pet.Power = i.second.PacketInfo.Power;
        pet.Health = i.second.PacketInfo.Health;
        pet.MaxHealth = i.second.PacketInfo.MaxHealth;
        pet.Speed = i.second.PacketInfo.Speed;
        pet.BreedQuality = i.second.PacketInfo.BreedQuality;
        pet.CustomName = i.second.PacketInfo.CustomName;
        pet.NoRename = false; // what we should do?
        pet.OwnerGuid = _player->GetGUID();
        battlePetJournal.Pets.push_back(pet);
    }

    _player->GetSession()->SendPacket(battlePetJournal.Write());
}

void BattlePetMgr::SendUpdatePets(std::vector<BattlePet> updates, bool added /*= false*/)
{
    WorldPackets::BattlePet::Updates update;
    update.AddedPet = added;
    for (auto pet : updates)
    {
        if (pet.SaveInfo == STATE_DELETED)
            continue;
       
       update.Pets.push_back(pet.PacketInfo);
    }

    _player->GetSession()->SendPacket(update.Write());
}

void BattlePetMgr::HealBattlePetsPct(uint8 pct)
{
    std::vector<BattlePet> updates;

    for (auto& pet : _pets)
        if (pet.second.PacketInfo.Health < pet.second.PacketInfo.MaxHealth)
        {
            if (pet.second.IsDead() || pet.second.IsHurt())
            {
                pet.second.PacketInfo.Health += CalculatePct(pet.second.PacketInfo.MaxHealth, pct);
                if (pet.second.SaveInfo != STATE_NEW)
                    pet.second.SaveInfo = STATE_UPDATED;
                updates.push_back(pet.second);
            }
        }

    SendUpdatePets(updates, false);
}

uint32 BattlePetMgr::GetXPForNextLevel(uint8 level)
{
    switch (level)
    {
        case 1: return 50; break;
        case 2: return 110; break;
        case 3: return 120; break;
        case 4: return 195; break;
        case 5: return 280; break;
        case 6: return 450; break;
        case 7: return 560; break;
        case 8: return 595; break;
        case 9: return 720; break;
        case 10: return 760; break;
        case 11: return 900; break;
        case 12: return 945; break;
        case 13: return 990; break;
        case 14: return 1150; break;
        case 15: return 1200; break;
        case 16: return 1250; break;
        case 17: return 1430; break;
        case 18: return 1485; break;
        case 19: return 1555; break;
        case 20: return 1595; break;
        case 21: return 1800; break;
        case 22: return 1860; break;
        case 23: return 1920; break;
        case 24: return 1980; break;
        default: return 9999; break;
    }
}

uint8 BattlePetMgr::GetRandomQuailty()
{
    uint32 r = urand(0, 1000); // 42% - grey, 33% - white, 19% - green, 6% - rare
    uint8 quality = 0;
    if (r >= 420 && r < 750)
        quality = 1;
    else if (r >= 750 && r < 940)
        quality = 2;
    else if (r >= 940 && r <= 1000)
        quality = 3;

    return quality;
}

uint16 BattlePetMgr::GetRandomBreedID(uint32 speciesID)
{
    if (std::vector<uint32> const* breeds = sObjectMgr->GetPossibleBreedsForSpecies(speciesID))
    {
        uint32 sum = 0;
        for (auto itr : *breeds)
            sum += GetWeightForBreed(itr);

        uint32 r = urand(0, sum);
        uint32 current_sum = 0;

        for (auto itr : *breeds)
        {
            if (current_sum <= r && r < current_sum + GetWeightForBreed(itr))
                return itr;

            current_sum += GetWeightForBreed(itr);
        }
    }

    return 0;
}

uint8 BattlePetMgr::GetWeightForBreed(uint16 breedID)
{
    uint8 weight = 0;
    switch (breedID)
    {
        case 3:
            weight = 60;
            break;
        case 4:
        case 5:
        case 6:
            weight = 10;
            break;
        case 7:
        case 8:
        case 9:
            weight = 20;
            break;
        case 10:
        case 11:
        case 12:
            weight = 30;
            break;
        default:
            break;
    }

    return weight;
}

void BattlePetMgr::CreatePetBattle(Player* initiator, ObjectGuid wildCreatureGuid)
{
    delete _petBattle;
    _petBattle = new PetBattle(initiator);
    _petBattle->Init(wildCreatureGuid);
}

void BattlePetMgr::InitializePetBattle(WorldPackets::BattlePet::BattleRequest packet)
{
    uint8 reason = 0;
    if (AllSlotsEmpty())
        reason = 16;
    else if (AllSlotsDead())
        reason = 15;

    if (reason)
    {
        SendPetBattleRequestFailed(reason);
        return;
    }

    _player->GetSession()->SendPacket(WorldPackets::BattlePet::FinalizeLocation(packet.Location).Write());
    CreatePetBattle(_player, packet.TargetGUID);
}

void BattlePetMgr::SendPetBattleRequestFailed(uint8 reason)
{
    WorldPackets::BattlePet::RequestFailed request;
    request.Reason = reason;
    _player->GetSession()->SendPacket(request.Write());
}

void BattlePetMgr::SendBattleQueueProposeMatch()
{
    _player->GetSession()->SendPacket(WorldPackets::BattlePet::NullSMsg(SMSG_PET_BATTLE_QUEUE_PROPOSE_MATCH).Write());
}

uint32 BattlePetMgr::GetPetCount(uint32 creatureEntry)
{
    uint32 count = 0;

    for (auto pet : _pets)
    {
        if (pet.second.SaveInfo == STATE_DELETED)
            continue;

        if (pet.second.nonPacketData.CreatureID == creatureEntry)
            ++count;
    }

    return count;
}

void BattlePetMgr::DeletePetByPetGUID(ObjectGuid guid)
{
    BattlePet* pet = GetPet(guid);
    if (!pet)
        return;

    pet->SaveInfo = STATE_DELETED;
}

ObjectGuid::LowType BattlePetMgr::GetPetGUIDBySpell(uint32 spell)
{
    for (auto v : _pets)
    {
        if (v.second.SaveInfo == STATE_DELETED)
            continue;

        if (v.second.nonPacketData.SpellID == spell)
            return v.first;
    }

    return ObjectGuid::LowType();
}

bool BattlePetMgr::AllSlotsEmpty()
{
    for (auto s : _slots)
        if (!s.Pet.BattlePetGUID.IsEmpty())
            return false;

    return true;
}

bool BattlePetMgr::AllSlotsDead()
{
    uint32 x = 0;
    for (auto s : _slots)
    {
        if (!s.Pet.BattlePetGUID.IsEmpty())
        {
            if (s.Pet.Health <= 0)
            {
                x++;
                if (x = MAX_ACTIVE_BATTLE_PETS)
                    return true;
            }
        }
        else
            return false;
    }

    return true;
}

bool BattlePetMgr::CanAwardXP()
{
    for (auto s : _slots)
        if (auto pet = GetPet(s.Pet.BattlePetGUID))
            if (pet->PacketInfo.Level < 25)
                return true;

    return false;
}

uint32 BattlePetMgr::CalcAvengerPetsLevelInSlots()
{
    uint32 lvl = 1;
    uint8 count = 1;
    {
        for (auto const x : _slots)
        {
            if (auto const p = GetPet(x.Pet.BattlePetGUID))
            {
                lvl += p->PacketInfo.Level;
                count++;
                if (count > MAX_ACTIVE_BATTLE_PETS)
                    break;
            }
        }
    }

    return lvl / count;
}

bool BattlePetMgr::PetIsSlotted(ObjectGuid guid)
{
    for (auto s : _slots)
        if (s.Pet.BattlePetGUID == guid)
            return true;

    return false;
}

void BattlePetMgr::SendError(BattlePetError error, uint32 creatureId /*= 0*/)
{
    WorldPackets::BattlePet::BattlePetError battlePetError;
    battlePetError.Result = error;
    battlePetError.CreatureID = creatureId;
    _player->GetSession()->SendPacket(battlePetError.Write());
}

void BattlePetMgr::SendPetBattleQueueStatus(QueueStatusData data)
{
    if (!_player || data.status == PET_BATTLE_QUEUE_STATUS_INIT)
        return;

    auto* session = _player->GetSession();
    if (session)
    {
        WorldPackets::BattlePet::PetBattleQueueStatus status;
        status.Msg.Status = data.status;
        status.Msg.SlotResult.clear();
        status.Msg.ClientWaitTime = time(nullptr) - data.joinTime;
        status.Msg.AverageWaitTime = _waitTimesAvgStore;
        status.Msg.Ticket.RequesterGuid = session->GetBattlenetAccountGUID();
        status.Msg.Ticket.Id = uint32(sObjectMgr->GetGenerator<HighGuid::PetBattle>()->Generate());
        status.Msg.Ticket.Type = 5; // always 5 ?
        status.Msg.Ticket.Time = time(nullptr);
        session->SendPacket(status.Write());
    }
}

void BattlePetMgr::JoinPvpBattleQueue()
{
    AddQueueData(_player->GetGUID(), time(nullptr), CalcAvengerPetsLevelInSlots(), PET_BATTLE_QUEUE_STATUS_JOINED);
    SendPetBattleQueueStatus(GetQueueData(_player->GetGUID()));
}

void BattlePetMgr::LeavePvpBattleQueue()
{
    QueueStatusData data;
    data.avgLevel = CalcAvengerPetsLevelInSlots();
    //data.joinTime = time(nullptr); not needed?
    data.status = PET_BATTLE_QUEUE_STATUS_LEAVE;
    SendPetBattleQueueStatus(data);
    RemoveQueueData(_player->GetGUID());
}

BattlePetMgr::QueueStatusData::QueueStatusData() : joinTime(time_t(time(nullptr))), avgLevel(0), status(PET_BATTLE_QUEUE_STATUS_INIT)
{ }

BattlePetMgr::QueueStatusData::QueueStatusData(time_t _joinTime, int32 _avgLevel, PetBattleQueueStatus _status)
{
    joinTime = _joinTime;
    avgLevel = _avgLevel;
    status = _status;
}

void BattlePetMgr::AddQueueData(ObjectGuid guid, time_t joinTime, int32 avgLevel, PetBattleQueueStatus status)
{
    _queueDataStore[guid] = QueueStatusData(joinTime, avgLevel, status);
    AddToQueue(guid);
}

void BattlePetMgr::AddToQueue(ObjectGuid guid)
{
    auto itr = _queueDataStore.find(guid);
    if (itr == _queueDataStore.end())
        return;

    _newToQueueStore.push_back(guid);
}

BattlePetMgr::QueueStatusData BattlePetMgr::GetQueueData(ObjectGuid guid)
{
    auto it = _queueDataStore.find(guid);
    if (it != _queueDataStore.end())
        return it->second;

    return QueueStatusData();
}

void BattlePetMgr::RemoveQueueData(ObjectGuid guid)
{
    auto it = _queueDataStore.find(guid);
    if (it != _queueDataStore.end())
        _queueDataStore.erase(it);
}

void BattlePetMgr::UpdateQueueTimers(time_t currTime)
{
    QueueStatusData data;
    data.avgLevel = CalcAvengerPetsLevelInSlots();
    data.joinTime -= currTime;
    SendPetBattleQueueStatus(data);
}

uint8 BattlePetMgr::FindPvpBattle()
{
    uint8 proposals = 0;
    GuidList firstNew;
    while (!_newToQueueStore.empty())
    {
        ObjectGuid frontguid = _newToQueueStore.front();
        firstNew.clear();
        firstNew.push_back(frontguid);
        _newToQueueStore.remove(frontguid);

        GuidList temporalList = _currentQueueStore;
        auto compatibles = FindNewBattles(firstNew, temporalList);
        if (compatibles == PET_PVP_BATTLE_MATCH)
            ++proposals;
        else
            _currentQueueStore.push_back(frontguid);
    }

    return proposals;
}

std::string ConcatenateGuids(GuidList const& check)
{
    if (check.empty())
        return "";

    GuidSet guids(check.begin(), check.end());
    std::ostringstream o;
    GuidSet::const_iterator it = guids.begin();
    o << *it;
    for (++it; it != guids.end(); ++it)
        o << '|' << *it;

    return o.str();
}

PetPvpBattleCompatibility BattlePetMgr::GetCompatibles(std::string const& key)
{
    auto itr = _compatibleMapStore.find(key);
    if (itr != _compatibleMapStore.end())
        return itr->second.compatibility;

    return PET_PVP_BATTLE_PENDING;
}

void BattlePetMgr::SetCompatibles(std::string const& key, PetPvpBattleCompatibility compatibles)
{
    auto& data = _compatibleMapStore[key];
    data.compatibility = compatibles;
}

PetPvpBattleCompatibility BattlePetMgr::CheckCompatibility(GuidList check)
{
    std::string strGuids = ConcatenateGuids(check);
    SetCompatibles(strGuids, PET_PVP_BATTLE_MATCH);
    return PET_PVP_BATTLE_MATCH;
}

PetPvpBattleCompatibility BattlePetMgr::FindNewBattles(GuidList& check, GuidList& all)
{
    std::string strGuids = ConcatenateGuids(check);
    auto compatibles = GetCompatibles(strGuids);
    if (compatibles == PET_PVP_BATTLE_PENDING)
        compatibles = CheckCompatibility(check);

    if (compatibles == PET_PVP_BATTLE_TOO_DIFFERENT_LEVELS)
        return compatibles;

    while (!all.empty())
    {
        check.push_back(all.front());
        all.pop_front();
        auto subcompatibility = FindNewBattles(check, all);
        if (subcompatibility == PET_PVP_BATTLE_MATCH)
            return PET_PVP_BATTLE_MATCH;
        check.pop_back();
    }
    return compatibles;
}

void BattlePetMgr::Update(uint32 diff)
{
    FindPvpBattle();
    time_t currTime = time(nullptr);
    if (_queueTimer > 10 * IN_MILLISECONDS)
    {
        _queueTimer = 0;
        UpdateQueueTimers(currTime);
        UpdateWaitTimeAvg(currTime);
    }
    else
        _queueTimer += diff;
}

void BattlePetMgr::UpdateWaitTimeAvg(int32 waitTime)
{
    _waitTimesAvgStore += waitTime;
}

PetBattle::PetBattle(Player* owner) : _player(owner)
{
    battleInfo.clear();
    nextRoundFinal = false;
    abandoned = false;
}

BattlePetFamily BattlePetMgr::BattlePet::GetFamily()
{
    if (BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(PacketInfo.SpeciesID))
        return BattlePetFamily(speciesEntry->petType);

    return BATTLE_PET_FAMILY_MAX;
}

bool PetBattle::PrepareBattleInfo(ObjectGuid creatureGuid)
{
    int8 petID = 0;

    // PLAYER (copying data from journal and other sources)
    for (uint8 i = 0; i < MAX_ACTIVE_BATTLE_PETS; ++i)
    {
        if (auto x = _player->GetBattlePetMgr()->GetPetBattleSlot(i))
        {
            if (auto petInfo = _player->GetBattlePetMgr()->GetPet(x->Pet.BattlePetGUID))
            {
                // dead or deleted pets are not valid
                if (petInfo->IsDead() || petInfo->SaveInfo == STATE_DELETED)
                    continue;

                PetBattleInfo* pbInfo = new PetBattleInfo();

                pbInfo->SetPetID(petID);
                pbInfo->SetTeam(TEAM_ALLY);
                pbInfo->SetGUID(x->Pet.BattlePetGUID);
                pbInfo->CopyPetInfo(_player, x->Pet.BattlePetGUID); //petInfo);

                for (uint8 j = 0; j < MAX_ACTIVE_BATTLE_PET_ABILITIES; ++j)
                    pbInfo->SetAbilityID(petInfo->GetAbilityID(j), j);

                ++petID;
                battleInfo.push_back(pbInfo);
            }
        }
    }

    // NPC (creating new data)
    Creature* wildPet = _player->GetMap()->GetCreature(creatureGuid);
    if (!wildPet)
        return false;

    uint8 wildPetLevel = wildPet->GetUInt32Value(UNIT_FIELD_WILD_BATTLE_PET_LEVEL);
    BattlePetSpeciesEntry const* s = sDB2Manager.GetBattlePetSpeciesEntry(wildPet->GetEntry());
    if (!s)
        return false;

    CreatureTemplate const* t = sObjectMgr->GetCreatureTemplate(wildPet->GetEntry());
    if (!t)
        return false;

    // roll creature count
    uint32 creatureCount = 1;
    if (wildPetLevel > 5)
        creatureCount = 2;
    else if (wildPetLevel > 10)
        creatureCount = 3;

    // set enemy PetID
    petID = 3;

    for (uint8 i = 0; i < creatureCount; ++i)
    {
        // roll random quality and breed
        //uint8 quality = _player->GetBattlePetMgr()->GetRandomQuailty();
        //uint16 breedID = _player->GetBattlePetMgr()->GetRandomBreedID(s->ID);
        //uint32 health = 0;
        //uint32 power = 0;
        //uint32 speed = 0;
        //_player->GetBattlePetMgr()->CalculateStats(s->ID, breedID, quality, wildPetLevel, health, power, speed);

        //BattlePetSlot* slot = new BattlePetSlot(ObjectGuid::Empty);
        //PetJournalInfo* petInfo = new PetJournalInfo(s->ID, wildPet->GetEntry(), wildPetLevel, t->Modelid1, power, speed, health, health, quality, 0, 0, s->spellId, "", breedID, STATE_NORMAL);
        //PetBattleInfo* pbInfo = new PetBattleInfo();

        //pbInfo->SetPetID(petID);
        //pbInfo->SetTeam(TEAM_ENEMY);
        //pbInfo->SetGUID(slot->GetPet());
        //pbInfo->CopyPetInfo(petInfo);

        //for (uint8 j = 0; j < MAX_ACTIVE_BATTLE_PET_ABILITIES; ++j)
        //    pbInfo->SetAbilityID(petInfo->GetAbilityID(j), j);

        //delete slot;
        //delete petInfo;

        ++petID;

        // added to array
        //battleInfo.push_back(pbInfo);
    }

    SetBattleState(PETBATTLE_STATE_WAITING_PRE_BATTLE);
    SetCurrentRoundID(0);

    return true;
}

uint8 PetBattle::GetTotalPetCountInTeam(uint8 team, bool onlyActive)
{
    uint8 count = 0;

    for (auto const& x : battleInfo)
    {
        if (x->GetTeam() != team)
            continue;

        if (onlyActive)
            if (x->IsDead() || x->Captured() || x->Caged())
                continue;

        ++count;
    }

    return count;
}

PetBattleInfo* PetBattle::GetPet(uint8 petNumber)
{
    for (auto v : battleInfo)
    {
        if (v->GetPetID() != petNumber)
            continue;

        return v;
    }

    return nullptr;
}

PetBattleInfo* PetBattle::GetFrontPet(uint8 team)
{
    for (auto const& x : battleInfo)
    {
        if (x->GetTeam() != team || !x->IsFrontPet())
            continue;

        return x;
    }

    return nullptr;
}

PetBattleInfo* PetBattle::GetNotFrontPet(uint8 team)
{
    for (auto const& x : battleInfo)
    {
        if (x->GetTeam() != team || x->IsFrontPet())
            continue;

        return x;
    }

    return nullptr;
}

void PetBattle::SetFrontPet(uint8 team, uint8 petNumber)
{
    if (PetBattleInfo* pb = GetFrontPet(team))
        pb->SetFrontPet(false);

    if (PetBattleInfo* pb1 = GetPet(petNumber))
        pb1->SetFrontPet(true);
}

void PetBattle::SetWinner(uint8 team)
{
    for (uint8 i = 0; i < MAX_TEAMS; ++i)
    {
        if (i == team)
            winners[i] = 1;
        else
            winners[i] = 0;
    }
}

uint8 PetBattle::GetWinner()
{
    for (uint8 i = 0; i < MAX_TEAMS; ++i)
    {
        if (winners[i] == 1)
            return i;
    }

    return 0;
}

uint8 PetBattle::GetPetTeamIndex(uint8 petNumber)
{
    switch (petNumber)
    {
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
        case 3: return 0;
        case 4: return 1;
        case 5: return 2;
        default: return 0;
    }
}

uint8 PetBattle::GetTeamByPetID(uint8 petNumber)
{
    switch (petNumber)
    {
        case 0:
        case 1:
        case 2:
            return 0;
        case 3:
        case 4:
        case 5:
            return 1;
        default:
            return 0;
    }
}

void PetBattle::InitializePetBattle(ObjectGuid targetGuid)
{
    auto mgr = _player->GetBattlePetMgr();

    bool isPvP = targetGuid.IsPlayer();

    teamGuids[0] = _player->GetGUID();
    if (isPvP)
        teamGuids[1] = targetGuid;

    WorldPackets::BattlePet::PetBattleInitialUpdate initial;
    initial.MsgData.WaitingForFrontPetsMaxSecs = MaxRoundTime[isPvP ? 1 : 0];
    initial.MsgData.PvpMaxRoundTime = MaxRoundTime[isPvP ? 1 : 0];
    initial.MsgData.CurRound = 0;
    initial.MsgData.NpcCreatureID = 0;
    initial.MsgData.NpcDisplayID = 0;
    initial.MsgData.CurPetBattleState = GetBattleState();
    initial.MsgData.ForfeitPenalty = 10;
    initial.MsgData.InitialWildPetGUID = targetGuid;
    initial.MsgData.IsPVP = isPvP;
    initial.MsgData.CanAwardXP = mgr->CanAwardXP();

    for (uint8 i = 0; i < MAX_TEAMS; ++i)
    {
        initial.MsgData.Players[i].CharacterID = teamGuids[i];
        initial.MsgData.Players[i].TrapAbilityID = TrapSpells[0];
        initial.MsgData.Players[i].TrapStatus = PET_BATTLE_TRAP_ERR_2;
        initial.MsgData.Players[i].RoundTimeSecs = MaxRoundTime[isPvP ? 1 : 0];
        initial.MsgData.Players[i].FrontPet = 0;
        initial.MsgData.Players[i].InputFlags = 6;

        for (auto const& x : battleInfo)
        {
            if (x->GetTeam() != i)
                continue;

            WorldPackets::BattlePet::PetBattlePetUpdate petUpdate;
            petUpdate.BattlePetGUID = x->GetGUID();
            petUpdate.SpeciesID = x->GetSpeciesID();
            petUpdate.DisplayID = x->GetDisplayID();
            petUpdate.CollarID = 0;
            petUpdate.Level = x->GetLevel();
            petUpdate.Xp = x->GetXP();
            petUpdate.CurHealth = x->GetHealth();
            petUpdate.MaxHealth = x->GetMaxHealth();
            petUpdate.Power = x->GetPower();
            petUpdate.Speed = x->GetSpeed();
            petUpdate.NpcTeamMemberID = 0;
            petUpdate.BreedQuality = x->GetQuality();
            petUpdate.StatusFlags = 0;
            petUpdate.Slot = GetPetTeamIndex(x->GetPetID());
            petUpdate.CustomName = x->GetCustomName();

            for (uint8 k = 0; k < MAX_ACTIVE_BATTLE_PET_ABILITIES; ++k)
            {
                WorldPackets::BattlePet::PetBattleActiveAbility activeAbility;
                activeAbility.AbilityID = x->GetAbilityID(k);
                activeAbility.CooldownRemaining = 0;
                activeAbility.LockdownRemaining = 0;
                activeAbility.AbilityIndex = k;
                activeAbility.Pboid = 0;
                petUpdate.Abilities.push_back(activeAbility);
            }

            for (auto const& x : {0})
            {
                WorldPackets::BattlePet::PetBattleActiveAura activeAura;
                activeAura.AbilityID = 0;
                activeAura.InstanceID = 0;
                activeAura.RoundsRemaining = 0;
                activeAura.CurrentRound = 0;
                activeAura.CasterPBOID = 0;
                //petUpdate.Auras.push_back(activeAura);
            }


            if (auto pet = mgr->GetPet(x->GetGUID()))
            {
                petUpdate.States[STATE_STAT_STAMINA] = pet->GetBaseStateValue(STATE_STAT_STAMINA); // 1700;
                petUpdate.States[STATE_STAT_SPEED] = pet->GetBaseStateValue(STATE_STAT_SPEED); // 1600;
                petUpdate.States[STATE_STAT_CRIT_CHANCE] = pet->GetBaseStateValue(STATE_STAT_CRIT_CHANCE); // 5;
                petUpdate.States[STATE_STAT_POWER] = pet->PacketInfo.Power;
                petUpdate.States[FamilyStates[pet->GetFamily()]] = 1;
                initial.MsgData.Players[i].Pets.push_back(petUpdate);
            }
        }
    }

    for (uint8 i = 0; i < MAX_ACTIVE_BATTLE_PET_ABILITIES; ++i)
    {
        for (auto const& x : {0})
        {
            WorldPackets::BattlePet::PetBattleActiveAura activeAura;
            activeAura.AbilityID = 0;
            activeAura.InstanceID = 0;
            activeAura.RoundsRemaining = 0;
            activeAura.CurrentRound = 0;
            activeAura.CasterPBOID = 0;
            //initial.MsgData.Enviros[i].Auras.push_back(activeAura);
        }

        //for (auto const& v : {0})
        //    initial.MsgData.Enviros[i].States.push_back({0, 0});
    }

    _player->GetSession()->SendPacket(initial.Write());
}

void PetBattleInfo::CopyPetInfo(Player* player, ObjectGuid petGuid /*BattlePetMgr::BattlePet* petInfo*/)
{
    auto petInfo = player->GetBattlePetMgr()->GetPet(petGuid);
    speciesID = petInfo->PacketInfo.SpeciesID;
    summonSpellID = petInfo->nonPacketData.SpellID;
    creatureEntry = petInfo->nonPacketData.CreatureID;
    displayID = petInfo->PacketInfo.DisplayID;
    customName = petInfo->PacketInfo.CustomName;
    breedID = petInfo->PacketInfo.BreedID;
    health = petInfo->PacketInfo.Health;
    maxHealth = petInfo->PacketInfo.MaxHealth;
    speed = petInfo->PacketInfo.Speed;
    power = petInfo->PacketInfo.Power;
    flags = petInfo->PacketInfo.BattlePetDBFlags;
    quality = petInfo->PacketInfo.BreedQuality;
    level = petInfo->PacketInfo.Level;
    xp = petInfo->PacketInfo.Xp;
    totalXP = petInfo->PacketInfo.Xp;
    newLevel = petInfo->PacketInfo.Level;

    frontPet = false;
    captured = false;
    caged = false;
}

bool PetBattleInfo::HasAbility(uint32 abilityID)
{
    for (uint8 i = 0; i < MAX_ACTIVE_BATTLE_PET_ABILITIES; ++i)
        if (GetAbilityID(i) == abilityID)
            return true;

    return false;
}

void PetBattle::Init(ObjectGuid creatureGuid)
{
    if (PrepareBattleInfo(creatureGuid))
        InitializePetBattle(creatureGuid);
}

bool PetBattle::FirstRoundHandler(uint8 allyFrontPetID, uint8 enemyFrontPetID)
{
    RoundResults firstRound;
    firstRound.PacketInfo.CurRound = 0;

    // set active pets for team
    SetFrontPet(TEAM_ALLY, allyFrontPetID);
    SetFrontPet(TEAM_ENEMY, enemyFrontPetID);

    PetBattle::RoundResults::Effect effect;
    effect.PacketInfo.PetBattleEffectType = PETBATTLE_EFFECT_TYPE_PET_SWAP;
    PetBattle::RoundResults::Effect::Target target;
    target.PacketInfo.Petx = allyFrontPetID;
    effect.PacketInfo.EffectTargetData.emplace_back(target.PacketInfo);
    firstRound.PacketInfo.EffectData.emplace_back(effect.PacketInfo);

    PetBattle::RoundResults::Effect effect1;
    effect1.PacketInfo.PetBattleEffectType = PETBATTLE_EFFECT_TYPE_PET_SWAP;
    effect1.PacketInfo.CasterPBOID = 3;
    PetBattle::RoundResults::Effect::Target target1;
    target1.PacketInfo.Petx = enemyFrontPetID;
    target1.PacketInfo.Type = EFFECT_TARGET_3;
    effect1.PacketInfo.EffectTargetData.emplace_back(target1.PacketInfo);
    firstRound.PacketInfo.EffectData.emplace_back(effect1.PacketInfo);

    CheckTrapStatuses(&firstRound);
    SendFirstRound(&firstRound);

    return true;
}

void PetBattle::SendFirstRound(RoundResults* roundResult)
{
    WorldPackets::BattlePet::BattleRound round(SMSG_PET_BATTLE_FIRST_ROUND);

    round.MsgData.NextPetBattleState = GetBattleState();
    round.MsgData.CurRound = roundResult->PacketInfo.CurRound;

    for (uint8 i = 0; i < MAX_TEAMS; ++i)
    {
        round.MsgData.NextInputFlags[i] = roundResult->PacketInfo.NextInputFlags[i];
        round.MsgData.NextTrapStatus[i] = roundResult->PacketInfo.NextTrapStatus[i];
        round.MsgData.RoundTimeSecs[i] = roundResult->PacketInfo.RoundTimeSecs[i];
    }

    round.MsgData.EffectData = roundResult->PacketInfo.EffectData;
    round.MsgData.Ability = roundResult->PacketInfo.Ability;
    round.MsgData.PetXDied = roundResult->PacketInfo.PetXDied;

    _player->GetSession()->SendPacket(round.Write());
}

void PetBattle::ForceReplacePetHandler(uint32 roundID, uint8 newFrontPet, uint8 team)
{
    RoundResults round;
    round.PacketInfo.CurRound = roundID;

    PetBattleInfo* frontPet = GetFrontPet(team);
    if (!frontPet)
        return;

    // paranoia check
    if (GetTeamByPetID(newFrontPet) != team)
        return;

    round.ProcessPetSwap(frontPet->GetPetID(), newFrontPet);
    SendForceReplacePet(&round);

    SetFrontPet(team, newFrontPet);
}

void PetBattle::SendForceReplacePet(RoundResults* roundResult)
{
    WorldPackets::BattlePet::BattleRound round(SMSG_PET_BATTLE_REPLACEMENTS_MADE);
    round.MsgData.NextPetBattleState = GetBattleState();
    round.MsgData.CurRound = roundResult->PacketInfo.CurRound;

    for (uint8 i = 0; i < MAX_TEAMS; ++i)
    {
        round.MsgData.NextInputFlags[i] = roundResult->PacketInfo.NextInputFlags[i];
        round.MsgData.NextTrapStatus[i] = roundResult->PacketInfo.NextTrapStatus[i];
        round.MsgData.RoundTimeSecs[i] = roundResult->PacketInfo.RoundTimeSecs[i];
    }

    round.MsgData.EffectData = roundResult->PacketInfo.EffectData;
    round.MsgData.Ability = roundResult->PacketInfo.Ability;
    round.MsgData.PetXDied = roundResult->PacketInfo.PetXDied;

    _player->GetSession()->SendPacket(round.Write());
}

bool PetBattle::UseAbilityHandler(uint32 abilityID, uint32 roundID)
{
    RoundResults round;
    round.PacketInfo.CurRound = roundID;

    // default state - need system for control it
    SetBattleState(PETBATTLE_STATE_ROUND_IN_PROGRESS);

    // check front pets
    PetBattleInfo* allyPet = GetFrontPet(TEAM_ALLY);
    PetBattleInfo* enemyPet = GetFrontPet(TEAM_ENEMY);
    if (!allyPet || !enemyPet)
        return false;

    // check on cheats
    if (!allyPet->HasAbility(abilityID))
        return false;

    // check speed and set attacker/victim of round
    // TODO: check some speed states and auras
    PetBattleInfo* attacker = allyPet->GetSpeed() > enemyPet->GetSpeed() ? allyPet : enemyPet;
    PetBattleInfo* victim = allyPet->GetSpeed() > enemyPet->GetSpeed() ? enemyPet : allyPet;

    bool damage = true;
    // TODO: script battle for wild pets
    if (damage)
    {
        // first turn: attack - TODO: script battle for wild pets (death check in some process)
        uint32 castAbilityID = (attacker->GetTeam() == TEAM_ALLY) ? abilityID : attacker->GetAbilityID(0);
        if (!castAbilityID)
            return false;

        uint32 effectID = sDB2Manager.GetBattlePetTurnByAbility(castAbilityID);
        round.ProcessAbilityDamage(attacker, victim, castAbilityID, effectID, 1);

        // second turn: response - TODO: script battle for wild pets (death check in some process)
        castAbilityID = (victim->GetTeam() == TEAM_ALLY) ? abilityID : victim->GetAbilityID(0);
        if (!castAbilityID)
            return false;

        effectID = sDB2Manager.GetBattlePetTurnByAbility(castAbilityID);
        round.ProcessAbilityDamage(victim, attacker, castAbilityID, effectID, 2);

        // find right place for some handler
        if (allyPet->IsDead() && !GetTotalPetCountInTeam(allyPet->GetTeam(), true))
        {
            SetWinner(enemyPet->GetTeam());
            nextRoundFinal = true;
        }
        else if (enemyPet->IsDead() && !GetTotalPetCountInTeam(enemyPet->GetTeam(), true))
        {
            SetWinner(allyPet->GetTeam());
            nextRoundFinal = true;
        }
        else if (allyPet->IsDead() && GetTotalPetCountInTeam(allyPet->GetTeam(), true) > 1)
            SetBattleState(PETBATTLE_STATE_WAITING_FOR_FRONT_PETS);
    }
    else
        return false;

    round.AuraProcessingBegin();
    round.AuraProcessingEnd();

    // increase round number
    round.PacketInfo.CurRound++;
    SetCurrentRoundID(round.PacketInfo.CurRound);

    CheckTrapStatuses(&round);
    CheckInputFlags(&round);

    SendRoundResults(&round);

    // find right place for some handler
    if (enemyPet->IsDead() && GetTotalPetCountInTeam(TEAM_ENEMY, true))
        ForceReplacePetHandler(round.PacketInfo.CurRound, enemyPet->GetPetID() + 1, TEAM_ENEMY);
    else if (allyPet->IsDead() && GetTotalPetCountInTeam(TEAM_ALLY, true) == 1)
        ForceReplacePetHandler(round.PacketInfo.CurRound, GetLastAlivePetID(TEAM_ALLY), TEAM_ALLY);

    // return to default state - need system for control it
    SetBattleState(PETBATTLE_STATE_ROUND_IN_PROGRESS);

    return true;
}

int8 PetBattle::GetLastAlivePetID(uint8 team)
{
    for (auto const& x : battleInfo)
    {
        if (x->GetTeam() != team)
            continue;

        if (!x->IsDead() && !x->Captured() && !x->Caged())
            return x->GetPetID();
    }

    return -1;
}

uint32 PetBattle::GetCurrentRoundID()
{
    return currentRoundID;
}

bool PetBattle::SkipTurnHandler(uint32 roundID)
{
    RoundResults round;
    round.PacketInfo.CurRound = roundID;

    SetBattleState(PETBATTLE_STATE_ROUND_IN_PROGRESS);

    PetBattleInfo* allyPet = GetFrontPet(TEAM_ALLY);
    PetBattleInfo* enemyPet = GetFrontPet(TEAM_ENEMY);
    if (!allyPet || !enemyPet)
        return false;

    round.ProcessSkipTurn(allyPet->GetPetID());

    // response enemy after player pet skip turn
    uint32 castAbilityID = enemyPet->GetAbilityID(0);
    if (!castAbilityID)
        return false;

    round.ProcessAbilityDamage(enemyPet, allyPet, castAbilityID, sDB2Manager.GetBattlePetTurnByAbility(castAbilityID), 1);

    // find right place for some handler
    if (allyPet->IsDead() && !GetTotalPetCountInTeam(allyPet->GetTeam(), true))
    {
        SetWinner(enemyPet->GetTeam());
        nextRoundFinal = true;
    }
    else if (allyPet->IsDead() && GetTotalPetCountInTeam(allyPet->GetTeam(), true) > 1)
        SetBattleState(PETBATTLE_STATE_WAITING_FOR_FRONT_PETS);

    round.AuraProcessingBegin();
    round.AuraProcessingEnd();

    // increase round number
    round.PacketInfo.CurRound++;
    SetCurrentRoundID(round.PacketInfo.CurRound);

    CheckTrapStatuses(&round);
    CheckInputFlags(&round);

    SendRoundResults(&round);

    if (allyPet->IsDead() && GetTotalPetCountInTeam(TEAM_ALLY, true) == 1)
        ForceReplacePetHandler(round.PacketInfo.CurRound, GetLastAlivePetID(TEAM_ALLY), TEAM_ALLY);

    return true;
}

bool PetBattle::UseTrapHandler(uint32 roundID)
{
    // check active pets
    PetBattleInfo* allyPet = GetFrontPet(TEAM_ALLY);
    PetBattleInfo* enemyPet = GetFrontPet(TEAM_ENEMY);
    if (!allyPet || !enemyPet)
        return false;

    // cheater checks, TODO:
    if (allyPet->IsDead())
        return false;

    if (enemyPet->IsDead() || enemyPet->GetHealthPct() > 20)
        return false;

    // check chance - base implemented
    uint8 trapped = 1;
    //if (roll_chance_i(70))
    //trapped = 1;

    RoundResults round;
    round.PacketInfo.CurRound = roundID;

    SetBattleState(PETBATTLE_STATE_ROUND_IN_PROGRESS);

    // demo
    PetBattle::RoundResults::Effect effect;
    effect.PacketInfo.CasterPBOID = allyPet->GetPetID();
    effect.PacketInfo.AbilityEffectID = 698;
    effect.PacketInfo.PetBattleEffectType = PETBATTLE_EFFECT_TYPE_STATUS_CHANGE;
    effect.PacketInfo.TurnInstanceID = 1;
    effect.PacketInfo.StackDepth = 1;
    PetBattle::RoundResults::Effect::Target target;
    target.PacketInfo.Petx = enemyPet->GetPetID();
    target.PacketInfo.Type = EFFECT_TARGET_1;
    target.PacketInfo.NewStatValue = trapped;
    effect.PacketInfo.EffectTargetData.emplace_back(target.PacketInfo);
    round.PacketInfo.EffectData.emplace_back(effect.PacketInfo);

    // demo
    if (!trapped)
    {
        // in response cast ability
    }
    else
    {
        enemyPet->SetCaptured(true);
        round.PacketInfo.PetXDied.push_back(enemyPet->GetPetID());

        if (!GetTotalPetCountInTeam(TEAM_ENEMY, true))
        {
            SetWinner(TEAM_ALLY);
            nextRoundFinal = true;
        }
    }

    round.AuraProcessingBegin();
    round.AuraProcessingEnd();

    // set trap status
    round.PacketInfo.NextTrapStatus[TEAM_ALLY] = PET_BATTLE_TRAP_ERR_8;

    // increase round
    round.PacketInfo.CurRound++;
    SetCurrentRoundID(round.PacketInfo.CurRound);

    SendRoundResults(&round);

    if (GetTotalPetCountInTeam(TEAM_ENEMY, true))
        ForceReplacePetHandler(round.PacketInfo.CurRound, enemyPet->GetPetID() + 1, TEAM_ENEMY);

    return true;
}

bool PetBattle::SwapPetHandler(uint8 newFrontPet, uint32 roundID)
{
    RoundResults round;
    round.PacketInfo.CurRound = roundID;

    SetBattleState(PETBATTLE_STATE_ROUND_IN_PROGRESS);

    // check active pets
    PetBattleInfo* allyPet = GetFrontPet(TEAM_ALLY);
    PetBattleInfo* enemyPet = GetFrontPet(TEAM_ENEMY);
    if (!allyPet || !enemyPet)
        return false;

    if (newFrontPet < 0 || newFrontPet > 2)
        return false;

    if (allyPet->GetPetID() == newFrontPet)
    {
        SkipTurnHandler(roundID);
        return true;
    }

    // cheater check
    PetBattleInfo* _newFrontPet = GetPet(newFrontPet);
    if (!_newFrontPet || _newFrontPet->IsDead())
        return false;

    round.ProcessPetSwap(allyPet->GetPetID(), newFrontPet);

    SetFrontPet(TEAM_ALLY, newFrontPet);

    allyPet = GetFrontPet(TEAM_ALLY);
    if (!allyPet)
        return false;

    // response enemy after player pet swap
    uint32 castAbilityID = enemyPet->GetAbilityID(0);
    if (!castAbilityID)
        return false;

    round.ProcessAbilityDamage(enemyPet, allyPet, castAbilityID, sDB2Manager.GetBattlePetTurnByAbility(castAbilityID), 1);

    // find right place for some handler
    if (allyPet->IsDead() && !GetTotalPetCountInTeam(allyPet->GetTeam(), true))
    {
        SetWinner(enemyPet->GetTeam());
        nextRoundFinal = true;
    }
    else if (allyPet->IsDead() && GetTotalPetCountInTeam(allyPet->GetTeam(), true) > 1)
        SetBattleState(PETBATTLE_STATE_WAITING_FOR_FRONT_PETS);

    round.AuraProcessingBegin();
    round.AuraProcessingEnd();

    // increase round number
    round.PacketInfo.CurRound++;
    SetCurrentRoundID(round.PacketInfo.CurRound);

    CheckTrapStatuses(&round);
    CheckInputFlags(&round);

    SendRoundResults(&round);

    if (allyPet->IsDead() && GetTotalPetCountInTeam(TEAM_ALLY, true) == 1)
        ForceReplacePetHandler(round.PacketInfo.CurRound, GetLastAlivePetID(TEAM_ALLY), TEAM_ALLY);

    return true;
}

void PetBattle::CheckTrapStatuses(RoundResults* round)
{
    PetBattleInfo* allyPet = GetFrontPet(TEAM_ALLY);
    PetBattleInfo* enemyPet = GetFrontPet(TEAM_ENEMY);
    if (!allyPet || !enemyPet)
        return;

    // default for wild pets and NPC
    round->PacketInfo.NextTrapStatus[TEAM_ENEMY] = PET_BATTLE_TRAP_ERR_2;

    if (enemyPet->Caged() || enemyPet->Captured())
        return;

    uint8 allyTrapStatus = PET_BATTLE_TRAP_ERR_4;

    if (enemyPet->GetHealthPct() < 20)
        allyTrapStatus = PET_BATTLE_TRAP_ACTIVE;

    // some checks
    uint32 creatureEntry = enemyPet->GetCreatureEntry();
    if (_player->GetBattlePetMgr()->GetPetCount(creatureEntry) >= MAX_ACTIVE_BATTLE_PETS)
        allyTrapStatus = PET_BATTLE_TRAP_ERR_5;

    if (allyPet->IsDead() || enemyPet->IsDead())
        allyTrapStatus = PET_BATTLE_TRAP_ERR_3;

    round->PacketInfo.NextTrapStatus[TEAM_ALLY] = allyTrapStatus;
}

void PetBattle::CheckInputFlags(RoundResults* round)
{
    for (uint8 i = 0; i < MAX_TEAMS; ++i)
        round->PacketInfo.NextInputFlags[i] = 0;

    // check special state
    if (GetBattleState() == 3)
    {
        round->PacketInfo.NextInputFlags[TEAM_ALLY] = 8;
        round->PacketInfo.NextInputFlags[TEAM_ENEMY] = 6;
    }
}

void PetBattle::SendRoundResults(RoundResults* roundResult)
{
    WorldPackets::BattlePet::BattleRound round(SMSG_PET_BATTLE_ROUND_RESULT);
    round.MsgData.NextPetBattleState = GetBattleState();
    round.MsgData.CurRound = roundResult->PacketInfo.CurRound;

    for (uint8 i = 0; i < MAX_TEAMS; ++i)
    {
        round.MsgData.NextInputFlags[i] = roundResult->PacketInfo.NextInputFlags[i];
        round.MsgData.NextTrapStatus[i] = roundResult->PacketInfo.NextTrapStatus[i];
        round.MsgData.RoundTimeSecs[i] = roundResult->PacketInfo.RoundTimeSecs[i];
    }

    round.MsgData.EffectData = roundResult->PacketInfo.EffectData;
    round.MsgData.Ability = roundResult->PacketInfo.Ability;
    round.MsgData.PetXDied = roundResult->PacketInfo.PetXDied;

    _player->GetSession()->SendPacket(round.Write());
}

bool PetBattle::FinalRoundHandler(bool abandoned)
{
    PetBattleInfo* allyPet = GetFrontPet(TEAM_ALLY);
    PetBattleInfo* enemyPet = GetFrontPet(TEAM_ENEMY);
    if (!allyPet || !enemyPet)
        return false;

    // rewardXP only for winner
    if (GetWinner() == TEAM_ALLY)
    {
        uint16 oldXp = allyPet->GetXP();
        uint16 newXp = 0;

        int8 levelDiff = allyPet->GetLevel() - enemyPet->GetLevel();
        // some checks
        if (levelDiff > 2)
            levelDiff = 2;
        if (levelDiff < -5)
            levelDiff = -5;

        // formula
        uint16 rewardXp = (enemyPet->GetLevel() + 9) * (levelDiff + 5);

        Unit::AuraEffectList const& auras = _player->GetAuraEffectsByType(SPELL_AURA_MOD_BATTLE_PET_XP_GAIN);
        for (Unit::AuraEffectList::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
            rewardXp *= (*itr)->GetAmount();

        if (allyPet->GetLevel() == MAX_BATTLE_PET_LEVEL)
            rewardXp = 0;

        newXp = oldXp + rewardXp;
        uint32 totalXp = _player->GetBattlePetMgr()->GetXPForNextLevel(allyPet->GetLevel());

        // check level-up
        if (newXp >= totalXp)
        {
            allyPet->SetNewLevel(allyPet->GetLevel() + 1);
            allyPet->SetTotalXP(newXp - totalXp);

            uint32 health = 0;
            uint32 power = 0;
            uint32 speed = 0;
            _player->GetBattlePetMgr()->CalculateStats(allyPet->GetSpeciesID(), allyPet->GetBreedID(), allyPet->GetQuality(), allyPet->GetNewLevel(), health, power, speed);
            allyPet->SetHealth(health);
            allyPet->SetMaxHealth(health);
            allyPet->SetPower(power);
            allyPet->SetSpeed(speed);
        }
        else
            allyPet->SetTotalXP(newXp);
    }
    else
    {
        if (abandoned)
        {
            // all alive pets penalty
            for (auto const& x : battleInfo)
            {
                if (x->GetTeam() != TEAM_ALLY)
                    continue;

                if (!x->IsDead())
                {
                    int32 percent10 = x->GetHealth() / 10;
                    int32 newHealth = x->GetHealth() - percent10;
                    x->SetHealth(newHealth);
                }
            }
        }
    }

    SendFinalRound();

    return true;
}

void PetBattle::SendFinalRound(bool pvpBattle /*= false*/)
{
    WorldPackets::BattlePet::SceneObjectFinalRound round;
    round.MsgData.Abandoned = abandoned;
    round.MsgData.PvpBattle = pvpBattle;
    for (uint8 i = 0; i < MAX_TEAMS; ++i)
    {
        for (auto const& x : battleInfo)
        {
            if (x->GetTeam() != i)
                continue;

            WorldPackets::BattlePet::FinalPet pet;
            pet.Guid = x->GetGUID();
            pet.Level = x->GetNewLevel();
            pet.Health = x->GetHealth();
            pet.MaxHealth = x->GetMaxHealth();
            pet.InitialLevel = x->GetLevel();
            pet.Pboid = x->GetPetID();
            pet.Captured = x->Captured();
            pet.Caged = x->Caged();
            pet.SeenAction = false;
            pet.AwardedXP = x->GetTotalXP() || (x->GetLevel() < x->GetNewLevel());
            pet.Xp = x->GetTotalXP();
            round.MsgData.Pets.push_back(pet);
        }

        round.MsgData.NpcCreatureID[i] = 0;
    }

    _player->GetSession()->SendPacket(round.Write());
}

void PetBattle::SendFinishPetBattle()
{
    _player->GetSession()->SendPacket(WorldPackets::BattlePet::NullSMsg(SMSG_PET_BATTLE_FINISHED).Write());
}

void PetBattle::UpdatePetsAfterBattle()
{
    GuidList updates;
    updates.clear();
    std::vector<BattlePetMgr::BattlePet> pets;

    for (uint8 i = 0; i < MAX_TEAMS; ++i)
    {
        for (auto const& v : battleInfo)
        {
            if (v->GetTeam() != i)
                continue;

            if (i == TEAM_ENEMY && !v->Captured())
                continue;

            // update loadout
            if (i == TEAM_ALLY)
            {
                auto loadoutInfo = _player->GetBattlePetMgr()->GetPet(v->GetGUID());
                if (!loadoutInfo)
                    continue;

                uint32 health = 0;
                uint32 power = 0;
                uint32 speed = 0;
                _player->GetBattlePetMgr()->CalculateStats(v->GetSpeciesID(), v->GetBreedID(), v->GetQuality(), v->GetNewLevel(), health, power, speed);
                // update
                loadoutInfo->PacketInfo.Level = v->GetNewLevel();
                if (v->IsDead())
                    loadoutInfo->PacketInfo.Health = 0;
                else
                    loadoutInfo->PacketInfo.Health = v->GetHealth();
                loadoutInfo->PacketInfo.BreedQuality = v->GetQuality();
                loadoutInfo->PacketInfo.Xp = v->GetTotalXP();
                loadoutInfo->PacketInfo.Power = power;
                loadoutInfo->PacketInfo.Speed = speed;
                loadoutInfo->PacketInfo.MaxHealth = health;
                loadoutInfo->SaveInfo = STATE_UPDATED;
                loadoutInfo->PacketInfo.BattlePetGUID = v->GetGUID();

                pets.push_back(*loadoutInfo);
            }
            // update trapped pets and added in journal
            else
            {
                _player->GetBattlePetMgr()->AddPet(v->GetSpeciesID(), v->GetBreedID(), v->GetQuality(), v->GetLevel(), STATE_NORMAL);
                // hack, fix it!
                if (v->GetSummonSpell())
                    _player->learnSpell(v->GetSummonSpell(), false);

                _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ADD_BATTLE_PET_JOURNAL, v->GetCreatureEntry());
                _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COLLECT_BATTLEPET);
                _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_PET_IN_BATTLE, v->GetSpeciesID(), v->GetQuality(), sDB2Manager.GetBattlePetSpeciesBySpellID(v->GetCreatureEntry()));
                _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLE_PET_CREDIT);
            }

            if (GetWinner() == TEAM_ALLY)
            {
                _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BATTLEPET_WIN, 0, 0, sDB2Manager.GetBattlePetSpeciesBySpellID(v->GetCreatureEntry()));

                if (v->GetNewLevel() > v->GetLevel())
                    _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BATTLEPET_LEVEL_UP, v->GetNewLevel());
            }
            else
                _player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BATTLEPET_WIN, ACHIEVEMENT_CRITERIA_CONDITION_NO_LOSE_PET_BATTLE);
        }
    }

    if (!pets.empty())
        _player->GetBattlePetMgr()->SendUpdatePets(pets, false);
}

void PetBattle::FinishPetBattle(bool error)
{
    SendFinishPetBattle();

    if (!error)
        UpdatePetsAfterBattle();

    _player->GetBattlePetMgr()->ClosePetBattle();
}

uint32 BattlePetMgr::BattlePet::GetAbilityID(uint8 rank)
{
    for (BattlePetSpeciesXAbilityEntry const* xEntry : sBattlePetSpeciesXAbilityStore)
    {
        if (xEntry->speciesID == PacketInfo.SpeciesID && xEntry->rank == rank)
        {
            if (rank == 0)
            {
                if (PacketInfo.BattlePetDBFlags & BATTLE_PET_FLAG_CUSTOM_ABILITY_1)
                {
                    if (xEntry->requiredLevel == 10)
                        return xEntry->abilityID;
                    else
                        continue;
                }
                else
                {
                    if (xEntry->requiredLevel < 10)
                        return xEntry->abilityID;
                }
            }

            if (rank == 1)
            {
                if (PacketInfo.BattlePetDBFlags & BATTLE_PET_FLAG_CUSTOM_ABILITY_2)
                {
                    if (xEntry->requiredLevel == 15)
                        return xEntry->abilityID;
                    else
                        continue;
                }
                else
                {
                    if (xEntry->requiredLevel < 15)
                        return xEntry->abilityID;
                }
            }

            if (rank == 2)
            {
                if (PacketInfo.BattlePetDBFlags & BATTLE_PET_FLAG_CUSTOM_ABILITY_3)
                {
                    if (xEntry->requiredLevel == 20)
                        return xEntry->abilityID;
                    else
                        continue;
                }
                else
                {
                    if (xEntry->requiredLevel < 20)
                        return xEntry->abilityID;
                }
            }
        }
    }

    return 0;

    //bool customAbility = false;

    //switch (rank)
    //{
    //    case 0:
    //        customAbility = (PacketInfo.BattlePetDBFlags & BATTLE_PET_FLAG_CUSTOM_ABILITY_1) ? true : false;
    //        break;
    //    case 1:
    //        customAbility = (PacketInfo.BattlePetDBFlags & BATTLE_PET_FLAG_CUSTOM_ABILITY_2) ? true : false;
    //        break;
    //    case 2:
    //        customAbility = (PacketInfo.BattlePetDBFlags & BATTLE_PET_FLAG_CUSTOM_ABILITY_3) ? true : false;
    //        break;
    //    default:
    //        break;
    //}
    //
    //return sDB2Manager.GetBattlePetXAbilityEntryBySpec(speciesID, customAbility, rank);
}

void PetBattle::RoundResults::ProcessAbilityDamage(PetBattleInfo* caster, PetBattleInfo* target, uint32 abilityID, uint32 effectID, uint8 TurnInstanceID)
{
    // check on dead
    if (caster->IsDead() || target->IsDead())
        return;

    // simple combination of effect 0 and target type 6

    PetBattle::RoundResults::Effect effect;
    effect.PacketInfo.CasterPBOID = caster->GetPetID();
    effect.PacketInfo.AbilityEffectID = effectID;
    effect.PacketInfo.TurnInstanceID = TurnInstanceID;
    effect.PacketInfo.StackDepth = 1;
    PetBattle::RoundResults::Effect::Target t;
    t.PacketInfo.Petx = target->GetPetID();
    t.PacketInfo.Type = EFFECT_TARGET_6;

    // get ability data
    if (BattlePetAbilityEntry const* entry = sBattlePetAbilityStore.LookupEntry(abilityID))
        effect.abilityEntry = entry;
    if (!effect.abilityEntry)
        return;

    // base pre-calculate damage and flags
    uint16 flags = effect.CalculateHitResult(target);
    bool crit = false;
    if (flags & PETBATTLE_EFFECT_FLAG_CRIT)
        crit = true;
    effect.PacketInfo.Flags = flags;

    // process in battle
    t.PacketInfo.Health = target->GetHealth() - effect.CalculateDamage(caster, target, crit);
    target->SetHealth(t.PacketInfo.Health);

    // process for round result
    effect.PacketInfo.EffectTargetData.emplace_back(t.PacketInfo);
    PacketInfo.EffectData.emplace_back(effect.PacketInfo);

    // process if victim dead
    if (target->IsDead())
    {
        PacketInfo.PetXDied.push_back(target->GetPetID());
        ProcessSetState(caster, target, effectID, 1, 1, TurnInstanceID);
    }
}

void PetBattle::RoundResults::ProcessSetState(PetBattleInfo* caster, PetBattleInfo* target, uint32 effectID, uint8 stateID, uint32 stateValue, uint8 TurnInstanceID)
{
    PetBattle::RoundResults::Effect effect;
    effect.PacketInfo.CasterPBOID = caster->GetPetID();
    effect.PacketInfo.AbilityEffectID = effectID;
    effect.PacketInfo.PetBattleEffectType = PETBATTLE_EFFECT_TYPE_SET_STATE;
    effect.PacketInfo.SourceAuraInstanceID = TurnInstanceID;
    effect.PacketInfo.StackDepth = 1;
    PetBattle::RoundResults::Effect::Target t;
    t.PacketInfo.Petx = target->GetPetID();
    t.PacketInfo.Type = EFFECT_TARGET_4;
    t.PacketInfo.StateID = stateID;
    t.PacketInfo.StateValue = stateValue;
    effect.PacketInfo.EffectTargetData.emplace_back(t.PacketInfo);
    PacketInfo.EffectData.emplace_back(effect.PacketInfo);
}

void PetBattle::RoundResults::ProcessPetSwap(uint8 oldPetNumber, uint8 newPetNumber)
{
    // simple combination of effect 4 and target type 3
    PetBattle::RoundResults::Effect effect;
    effect.PacketInfo.CasterPBOID = oldPetNumber;
    effect.PacketInfo.PetBattleEffectType = PETBATTLE_EFFECT_TYPE_PET_SWAP;
    PetBattle::RoundResults::Effect::Target target;
    target.PacketInfo.Petx = newPetNumber;
    target.PacketInfo.Type = EFFECT_TARGET_3;
    effect.PacketInfo.EffectTargetData.emplace_back(target.PacketInfo);
    PacketInfo.EffectData.emplace_back(effect.PacketInfo);
}

void PetBattle::RoundResults::ProcessSkipTurn(uint8 petNumber)
{
    // simple combination of effect 4 and target type 3
    PetBattle::RoundResults::Effect effect;
    effect.PacketInfo.CasterPBOID = petNumber;
    effect.PacketInfo.PetBattleEffectType = PETBATTLE_EFFECT_TYPE_PET_SWAP;
    effect.PacketInfo.Flags = PETBATTLE_EFFECT_FLAG_INVALID_TARGET; //@TODO check flag...
    PetBattle::RoundResults::Effect::Target target;
    target.PacketInfo.Petx = petNumber;
    target.PacketInfo.Type = EFFECT_TARGET_3;
    effect.PacketInfo.EffectTargetData.emplace_back(target.PacketInfo);
    PacketInfo.EffectData.emplace_back(effect.PacketInfo);
}

void PetBattle::RoundResults::AuraProcessingBegin()
{
    PetBattle::RoundResults::Effect effect;
    effect.PacketInfo.CasterPBOID = -1;
    effect.PacketInfo.PetBattleEffectType = PETBATTLE_EFFECT_TYPE_AURA_PROCESSING_BEGIN;
    PetBattle::RoundResults::Effect::Target target;
    target.PacketInfo.Petx = -1;
    target.PacketInfo.Type = EFFECT_TARGET_3;
    effect.PacketInfo.EffectTargetData.emplace_back(target.PacketInfo);
    PacketInfo.EffectData.emplace_back(effect.PacketInfo);
}

void PetBattle::RoundResults::AuraProcessingEnd()
{
    PetBattle::RoundResults::Effect effect;
    effect.PacketInfo.CasterPBOID = -1;
    effect.PacketInfo.PetBattleEffectType = PETBATTLE_EFFECT_TYPE_AURA_PROCESSING_END;
    PetBattle::RoundResults::Effect::Target target;
    target.PacketInfo.Petx = -1;
    target.PacketInfo.Type = EFFECT_TARGET_3;
    effect.PacketInfo.EffectTargetData.emplace_back(target.PacketInfo);
    PacketInfo.EffectData.emplace_back(effect.PacketInfo);
}

int32 PetBattle::RoundResults::Effect::GetBaseDamage(PetBattleInfo* attacker, uint32 turnIndex)
{
    uint32 basePoints = GetProperties(BASEPOINTS, turnIndex);
    return basePoints * (1 + attacker->GetPower() * 0.05f);
}

int32 PetBattle::RoundResults::Effect::CalculateDamage(PetBattleInfo* attacker, PetBattleInfo* victim, bool crit)
{
    int32 baseDamage = GetBaseDamage(attacker);
    int32 cleanDamage = urand(baseDamage - 5, baseDamage + 5);
    float mod = GetAttackModifier(abilityEntry->Type, sDB2Manager.GetBattlePetSpeciesBySpellID(victim->GetCreatureEntry()));
    int32 finalDamage = cleanDamage * mod;
    if (crit)
        finalDamage *= 2;

    return finalDamage;
}

int16 PetBattle::RoundResults::Effect::CalculateHitResult(PetBattleInfo* victim)
{
    uint16 flags = PETBATTLE_EFFECT_FLAG_BASE;
    float mod = GetAttackModifier(abilityEntry->Type, sDB2Manager.GetBattlePetSpeciesBySpellID(victim->GetCreatureEntry()));

    if (roll_chance_i(5))
        flags |= PETBATTLE_EFFECT_FLAG_CRIT;

    if (mod > 1.0f)
        flags |= PETBATTLE_EFFECT_FLAG_STRONG;
    else if (mod < 1.0f)
        flags |= PETBATTLE_EFFECT_FLAG_WEAK;

    return flags;
}

uint32 PetBattle::RoundResults::Effect::GetProperties(uint8 properties, uint32 turnIndex)
{
    LocalizedString desc;
    desc.Str[LOCALE_enUS] = "Points";

    switch (properties)
    {
        case 1:
            desc.Str[LOCALE_enUS] = "Accuracy";
            break;
        default:
            break;
    }

    for (BattlePetAbilityTurnEntry const* tEntry : sBattlePetAbilityTurnStore)
        if (tEntry->AbilityID == abilityEntry->ID && tEntry->turnIndex == turnIndex)
            for (BattlePetAbilityEffectEntry const* eEntry : sBattlePetAbilityEffectStore)
                if (eEntry->TurnEntryID == tEntry->ID)
                    for (BattlePetEffectPropertiesEntry const* entry : sBattlePetEffectPropertiesStore)
                        if (eEntry->propertiesID == entry->ID)
                            for (uint8 l = 0; l < MAX_EFFECT_PROPERTIES; ++l)
                                if (!strcmp(entry->propertyDescs[LOCALE_enUS][l].Str[LOCALE_enUS], desc.Str[LOCALE_enUS]))
                                    return eEntry->propertyValues[l];

    return 0;
}

float PetBattle::RoundResults::Effect::GetAttackModifier(uint8 attackType, uint8 defenseType)
{
    if (GtBattlePetTypeDamageModEntry const* gt = sGtBattlePetTypeDamageModStore.EvaluateTable(defenseType - 1, attackType - 1))
        return gt->value;

    return 1.0f;
}
