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

BattlePetMgr::BattlePetMgr(Player* owner) : m_player(owner), m_petBattleWild(NULL)
{
    // clear journal
    m_PetJournal.clear();
    // clear slots
    m_battleSlots.clear();
}

void BattlePetMgr::AddPetToList(ObjectGuid guid, uint32 speciesID, uint32 creatureEntry, uint8 level, uint32 display, uint16 power, uint16 speed, uint32 health, uint32 maxHealth, uint8 quality, uint16 xp, uint16 flags, uint32 spellID, std::string customName, int16 breedID, uint8 state)
{
    m_PetJournal[guid] = new PetJournalInfo(speciesID, creatureEntry, level, display, power, speed, health, maxHealth, quality, xp, flags, spellID, customName, breedID, state);
}

void BattlePetMgr::InitBattleSlot(ObjectGuid guid, uint8 index)
{
    m_battleSlots[index] = new PetBattleSlot(guid);
}

bool BattlePetMgr::SlotIsLocked(uint8 index)
{
    bool locked = true;

    switch (index)
    {
        case 0: locked = !m_player->HasSpell(119467); break;
        case 1: locked = !m_player->HasAchieved(7433), false; break;
        case 2: locked = !m_player->HasAchieved(6566), false; break;
        default: break;
    }

    return locked;
}

void BattlePetMgr::CloseWildPetBattle()
{
    // remove battle object
    delete m_petBattleWild;
    m_petBattleWild = NULL;
}

void BattlePetMgr::SendUpdatePets(std::list<ObjectGuid> &updates, bool added)
{
    WorldPackets::BattlePet::Updates update;
    update.AddedPet = added;
    
    for (auto const& i : updates)
    {
        PetJournalInfo* petInfo = GetPetInfoByPetGUID(i);
        if (!petInfo || petInfo->GetState() == STATE_DELETED)
            continue;

        WorldPackets::BattlePet::BattlePet petData;
        petData.Initialize(petInfo, i, m_player);
        update.Pets.push_back(petData);
    }

    m_player->GetSession()->SendPacket(update.Write());
}

// testing....
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
    // 42% - grey, 33% - white, 19% - green, 6% - rare
    uint32 r = urand(0, 1000);

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
        for (std::vector<uint32>::const_iterator itr = breeds->begin(); itr != breeds->end(); ++itr)
            sum += GetWeightForBreed((*itr));

        uint32 r = urand(0, sum);
        uint32 current_sum = 0;

        for (std::vector<uint32>::const_iterator itr = breeds->begin(); itr != breeds->end(); ++itr)
        {
            uint16 breedID = (*itr);
            if (current_sum <= r && r < current_sum + GetWeightForBreed(breedID))
                return breedID;

            current_sum += GetWeightForBreed(breedID);
        }
    }

    return 0;
}

void BattlePetMgr::CreateWildBattle(Player* initiator, ObjectGuid wildCreatureGuid)
{
    delete m_petBattleWild;
    m_petBattleWild = new PetBattleWild(initiator);
    m_petBattleWild->Init(wildCreatureGuid);
}

void BattlePetMgr::SendPetBattleRequestFailed(uint8 reason)
{
    WorldPackets::BattlePet::RequestFailed request;
    request.Reason = reason;
    m_player->GetSession()->SendPacket(request.Write());
}

// BattlePetStatAccumulator
BattlePetStatAccumulator::BattlePetStatAccumulator(uint32 _speciesID, uint16 _breedID) : healthMod(0), powerMod(0), speedMod(0), qualityMultiplier(0.0f)
{
    for (BattlePetSpeciesStateEntry const* speciesState : sBattlePetSpeciesStateStore)
        if (speciesState->SpeciesID == _speciesID)
            Accumulate(speciesState->State, speciesState->Value);

    for (BattlePetBreedStateEntry const* state : sBattlePetBreedStateStore)
        if (state->breedID == _breedID)
            Accumulate(state->stateID, state->stateModifier);
}

// PetBattleWild
PetBattleWild::PetBattleWild(Player* owner) : m_player(owner)
{
    battleInfo.clear();

    nextRoundFinal = false;
    abandoned = false;
}

bool PetBattleWild::PrepareBattleInfo(ObjectGuid creatureGuid)
{
    int8 petID = 0;

    // PLAYER (copying data from journal and other sources)
    for (uint8 i = 0; i < MAX_ACTIVE_BATTLE_PETS; ++i)
    {
        if (PetBattleSlot* _slot = m_player->GetBattlePetMgr()->GetPetBattleSlot(i))
        {
            if (!_slot->IsEmpty())
            {
                if (PetJournalInfo* petInfo = m_player->GetBattlePetMgr()->GetPetInfoByPetGUID(_slot->GetPet()))
                {
                    // dead or deleted pets are not valid
                    if (petInfo->IsDead() || petInfo->GetState() == STATE_DELETED)
                        continue;

                    PetBattleInfo* pbInfo = new PetBattleInfo();

                    pbInfo->SetPetID(petID);
                    pbInfo->SetTeam(TEAM_ALLY);
                    pbInfo->SetGUID(_slot->GetPet());

                    pbInfo->CopyPetInfo(petInfo);

                    for (uint8 j = 0; j < MAX_ACTIVE_BATTLE_PET_ABILITIES; ++j)
                    {
                        uint32 ID = petInfo->GetAbilityID(j); 
                        pbInfo->SetAbilityID(ID, j);
                    }

                    ++petID;

                    // added to array
                    battleInfo.push_back(pbInfo);
                }
            }
        }
    }

    // NPC (creating new data)
    Creature * wildPet = m_player->GetMap()->GetCreature(creatureGuid);

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
        uint8 quality = m_player->GetBattlePetMgr()->GetRandomQuailty();
        uint16 breedID = m_player->GetBattlePetMgr()->GetRandomBreedID(s->ID);

        BattlePetStatAccumulator* accumulator = new BattlePetStatAccumulator(s->ID, breedID);
        accumulator->CalcQualityMultiplier(quality, wildPetLevel);
        uint32 health = accumulator->CalculateHealth();
        uint32 power = accumulator->CalculatePower();
        uint32 speed = accumulator->CalculateSpeed();
        delete accumulator;

        PetBattleSlot* slot = new PetBattleSlot(ObjectGuid::Empty);
        PetJournalInfo* petInfo = new PetJournalInfo(s->ID, wildPet->GetEntry(), wildPetLevel, t->Modelid1, power, speed, health, health, quality, 0, 0, s->spellId, "", breedID, STATE_NORMAL);
        PetBattleInfo* pbInfo = new PetBattleInfo();

        pbInfo->SetPetID(petID);
        pbInfo->SetTeam(TEAM_ENEMY);
        pbInfo->SetGUID(slot->GetPet());

        pbInfo->CopyPetInfo(petInfo);

        for (uint8 j = 0; j < MAX_ACTIVE_BATTLE_PET_ABILITIES; ++j)
        {
            uint32 ID = petInfo->GetAbilityID(j);
            pbInfo->SetAbilityID(ID, j);
        }

        delete slot;
        delete petInfo;

        ++petID;

        // added to array
        battleInfo.push_back(pbInfo);
    }

    SetBattleState(1);
    SetCurrentRoundID(0);

    return true;
}

uint8 PetBattleWild::GetTotalPetCountInTeam(uint8 team, bool onlyActive)
{
    uint8 count = 0;

    for (BattleInfo::const_iterator itr = battleInfo.begin(); itr != battleInfo.end(); ++itr)
    {
        PetBattleInfo * pb = (*itr);

        if (!pb || pb->GetTeam() != team)
            continue;

        if (onlyActive)
        {
            if (pb->IsDead() || pb->Captured() || pb->Caged())
                continue;
        }

        ++count;
    }

    return count;
}

PetBattleInfo* PetBattleWild::GetFrontPet(uint8 team)
{
    for (BattleInfo::const_iterator itr = battleInfo.begin(); itr != battleInfo.end(); ++itr)
    {
        PetBattleInfo * pb = (*itr);

        if (!pb || pb->GetTeam() != team || !pb->IsFrontPet())
            continue;

        return pb;
    }

    return NULL;
}

void PetBattleWild::SetFrontPet(uint8 team, uint8 petNumber)
{
    for (BattleInfo::const_iterator itr = battleInfo.begin(); itr != battleInfo.end(); ++itr)
    {
        PetBattleInfo * pb = (*itr);

        if (!pb || pb->GetTeam() != team)
            continue;

        pb->SetFrontPet(false);
    }

    for (BattleInfo::const_iterator itr = battleInfo.begin(); itr != battleInfo.end(); ++itr)
    {
        PetBattleInfo * pb = (*itr);

        if (!pb || pb->GetTeam() != team || pb->GetPetID() != petNumber)
            continue;

        pb->SetFrontPet(true);
    }
}

void PetBattleWild::SendFullUpdate(ObjectGuid creatureGuid)
{
    WorldPacket data(SMSG_PET_BATTLE_INITIAL_UPDATE); // it's right opcode?
    // BITPACK
    // enviro states and auras count and bitpack handle (weather?) (some enviroment variables?)
    for (uint8 i = 0; i < 3; ++i)
    {
        data.WriteBits(0, 21);
        data.WriteBits(0, 21);
    }

    data.WriteBit(0);
    data.WriteBit(0);
    data.WriteBit(0);

    //teamGuids[0] = m_player->GetBattlePetMgr()->InverseGuid(m_player->GetObjectGuid());
    //teamGuids[1] = 0;

    for (uint8 i = 0; i < 2; ++i)
    {
        //data.WriteGuidMask<2>(teamGuids[i]);
        data.WriteBit(1);
        //data.WriteGuidMask<5, 7, 4>(teamGuids[i]);
        data.WriteBit(0);
        //data.WriteGuidMask<0>(teamGuids[i]);
        data.WriteBits(GetTotalPetCountInTeam(i), 2);
        //data.WriteGuidMask<1>(teamGuids[i]);

        for (BattleInfo::const_iterator itr = battleInfo.begin(); itr != battleInfo.end(); ++itr)
        {
            PetBattleInfo * pb = (*itr);

            // check vaild
            if (!pb || pb->GetTeam() != i)
                continue;

            ObjectGuid guid = pb->GetGUID();

            //data.WriteGuidMask<3, 4, 0>(guid);
            data.WriteBit(0);
            //data.WriteGuidMask<6>(guid);
            data.WriteBits(4, 21);                   // state count
            data.WriteBits(1, 20);                   // abilities count
            //data.WriteGuidMask<5>(guid);

            data.WriteBit(0);

            //data.WriteGuidMask<2, 1, 7>(guid);
            data.WriteBit(1);

            uint8 len = pb->GetCustomName() == "" ? 0 : pb->GetCustomName().length();
            data.WriteBits(len, 7);
            data.WriteBits(0, 21);                   // auras count
        }

        //data.WriteGuidMask<6>(teamGuids[i]);
        data.WriteBit(0);
        //data.WriteGuidMask<3>(teamGuids[i]);
    }

    data.WriteBit(1);
    data.WriteBit(0);
    data.WriteBit(1);

    //data.WriteGuidMask<6, 0, 1, 4, 2, 5, 3, 7>(creatureGuid);

    data.WriteBit(0);
    data.WriteBit(1);
    data.WriteBit(0);

    for (uint8 i = 0; i < 2; ++i)
    {
        for (BattleInfo::const_iterator itr = battleInfo.begin(); itr != battleInfo.end(); ++itr)
        {
            PetBattleInfo * pb = (*itr);

            // check vaild
            if (!pb || pb->GetTeam() != i)
                continue;

            ObjectGuid guid = pb->GetGUID();

            // abilities
            for (uint8 k = 0; k < MAX_ACTIVE_BATTLE_PET_ABILITIES; ++k)
            {
                if (k != 0)
                    break;

                data << uint8(0);
                data << uint16(0);
                data << uint8(0);
                data << uint16(0);
                data << uint32(pb->GetAbilityID(k));
                /*if (i == 0)
                    data << uint32(204);
                else
                    data << uint32(battleInfo[i][j]->GetAbilityInfoByIndex(k)->GetID());*/
            }

            data << uint16(pb->GetXP());
            data << uint32(pb->GetMaxHealth());

            //data.WriteGuidBytes<1>(guid);

            // states - same for testing
            data << uint32(1600); // some fucking strange value!
            data << uint32(20);   // stateID from BattlePetState.db2 -> Stat_Speed
            data << uint32(1800); // some fucking strange value1!
            data << uint32(18);   // stateID from BattlePetState.db2 -> Stat_Power
            data << uint32(1700); // some fucking strange value2!
            data << uint32(19);   // stateID from BattlePetState.db2 -> Stat_Stamina
            data << uint32(5);    // crit chance % value
            data << uint32(40);   // stateID from BattlePetState.db2 -> Stat_CritChance

            data << uint32(0);
            data << uint32(pb->GetSpeed()); // speed

            // auras
            //

            //data.WriteGuidBytes<4>(guid);
            uint8 len = pb->GetCustomName() == "" ? 0 : pb->GetCustomName().length();
            if (len > 0)
                data.WriteString(pb->GetCustomName());
            data << uint16(pb->GetQuality());
            //data.WriteGuidBytes<0>(guid);
            data << uint32(pb->GetHealth());
            //data.WriteGuidBytes<3>(guid);
            data << uint32(pb->GetSpeciesID());
            data << uint32(0);
            //data.WriteGuidBytes<2, 6>(guid);
            data << uint32(pb->GetDisplayID());
            data << uint16(pb->GetLevel());
            //data.WriteGuidBytes<7, 5>(guid);
            data << uint32(pb->GetPower());
            uint8 slot = GetTeamIndex(pb->GetPetID());
            data << uint8(slot);                       // Slot
        }

        data << uint32(427);                           // TrapSpellID
        //data.WriteGuidBytes<2, 5>(teamGuids[i]);
        data << uint32(2);                             // TrapStatus

        //data.WriteGuidBytes<4, 0, 7, 6>(teamGuids[i]);
        data << uint8(6);                              // InputFlags
        data << uint8(0);                              // FrontPet
        //data.WriteGuidBytes<1, 3>(teamGuids[i]);
    }

    //data.WriteGuidBytes<6, 2, 1, 3, 0, 4, 7, 5>(creatureGuid);

    data << uint16(30);                                // WaitingForFrontPetsMaxSecs | PvpMaxRoundTime
    data << uint16(30);                                // WaitingForFrontPetsMaxSecs | PvpMaxRoundTime
    data << uint8(GetBattleState());                   // CurPetBattleState
    data << uint32(0);                                 // CurRound
    data << uint8(10);                                 // ForfeitPenalty

    m_player->GetSession()->SendPacket(&data);
}

void PetBattleInfo::CopyPetInfo(PetJournalInfo* petInfo)
{
    // copying from journal
    speciesID = petInfo->GetSpeciesID();
    summonSpellID = petInfo->GetSummonSpell();
    creatureEntry = petInfo->GetCreatureEntry();
    displayID = petInfo->GetDisplayID();
    customName = petInfo->GetCustomName();
    breedID = petInfo->GetBreedID();
    health = petInfo->GetHealth();
    maxHealth = petInfo->GetMaxHealth();
    speed = petInfo->GetSpeed();
    power = petInfo->GetPower();
    flags = petInfo->GetFlags();
    quality = petInfo->GetQuality();
    level = petInfo->GetLevel();
    xp = petInfo->GetXP();
    // for final stage
    totalXP = petInfo->GetXP();
    newLevel = petInfo->GetLevel();

    frontPet = false;
    captured = false;
    caged = false;
}

void PetBattleWild::Init(ObjectGuid creatureGuid)
{
    if (!PrepareBattleInfo(creatureGuid))
        return;

    SendFullUpdate(creatureGuid);
}

bool PetBattleWild::FirstRoundHandler(uint8 allyFrontPetID, uint8 enemyFrontPetID)
{
    PetBattleRoundResults* firstRound = new PetBattleRoundResults(0);

    // set active pets for team
    SetFrontPet(TEAM_ALLY, allyFrontPetID);
    SetFrontPet(TEAM_ENEMY, enemyFrontPetID);

    PetBattleEffect* effect = new PetBattleEffect(0, 0, 4, 0, 0, 0, 0);
    PetBattleEffectTarget* target = new PetBattleEffectTarget(allyFrontPetID, 3);

    effect->targets.push_back(target);
    firstRound->effects.push_back(effect);

    PetBattleEffect* effect1 = new PetBattleEffect(3, 0, 4, 0, 0, 0, 0);
    PetBattleEffectTarget* target1 = new PetBattleEffectTarget(enemyFrontPetID, 3);

    effect1->targets.push_back(target1);
    firstRound->effects.push_back(effect1);

    SendFirstRound(firstRound);
    delete firstRound;

    return true;
}

void PetBattleWild::SendFirstRound(PetBattleRoundResults* firstRound)
{
    WorldPackets::BattlePet::BattleRound round(SMSG_PET_BATTLE_FIRST_ROUND);
    round.MsgData.Initialize(firstRound);
    m_player->GetSession()->SendPacket(round.Write());
}

void PetBattleWild::ForceReplacePetHandler(uint32 roundID, uint8 newFrontPet, uint8 team)
{
    PetBattleRoundResults* round = new PetBattleRoundResults(roundID);

    // check active pets
    PetBattleInfo* frontPet = GetFrontPet(team);

    if (!frontPet)
        return;

    // paranoia check
    if (GetTeamByPetID(newFrontPet) != team)
        return;

    round->ProcessPetSwap(frontPet->GetPetID(), newFrontPet);
    SendForceReplacePet(round);

    SetFrontPet(team, newFrontPet);

    delete round;
}

void PetBattleWild::SendForceReplacePet(PetBattleRoundResults* round)
{
    WorldPackets::BattlePet::BattleRound roundResult(SMSG_PET_BATTLE_REPLACEMENTS_MADE);
    roundResult.MsgData.Initialize(round);
    m_player->GetSession()->SendPacket(roundResult.Write());
}

bool PetBattleWild::UseAbilityHandler(uint32 abilityID, uint32 roundID)
{
    PetBattleRoundResults* round = new PetBattleRoundResults(roundID);
    // default state - need system for control it
    SetBattleState(2);

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

        uint32 effectID = GetVisualEffectIDByAbilityID(castAbilityID);
        round->ProcessAbilityDamage(attacker, victim, castAbilityID, effectID, 1);

        // second turn: response - TODO: script battle for wild pets (death check in some process)
        castAbilityID = (victim->GetTeam() == TEAM_ALLY) ? abilityID : victim->GetAbilityID(0);

        if (!castAbilityID)
            return false;

        effectID = GetVisualEffectIDByAbilityID(castAbilityID);
        round->ProcessAbilityDamage(victim, attacker, castAbilityID, effectID, 2);

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
            SetBattleState(3);
    }
    else
        return false;

    round->AuraProcessingBegin();
    round->AuraProcessingEnd();

    // increase round number
    round->roundID++;
    SetCurrentRoundID(round->roundID);

    CheckTrapStatuses(round);
    CheckInputFlags(round);

    SendRoundResults(round);

    // find right place for some handler
    if (enemyPet->IsDead() && GetTotalPetCountInTeam(TEAM_ENEMY, true))
        ForceReplacePetHandler(round->roundID, enemyPet->GetPetID() + 1, TEAM_ENEMY);
    else if (allyPet->IsDead() && GetTotalPetCountInTeam(TEAM_ALLY, true) == 1)
        ForceReplacePetHandler(round->roundID, GetLastAlivePetID(TEAM_ALLY), TEAM_ALLY);

    // return to default state - need system for control it
    petBattleState = 2;

    delete round;
    return true;
}

int8 PetBattleWild::GetLastAlivePetID(uint8 team)
{
    for (BattleInfo::const_iterator itr = battleInfo.begin(); itr != battleInfo.end(); ++itr)
    {
        PetBattleInfo * pb = (*itr);

        if (!pb || pb->GetTeam() != team)
            continue;

        if (!pb->IsDead() && !pb->Captured() && !pb->Caged())
            return pb->GetPetID();
    }

    return -1;
}

uint32 PetBattleWild::GetCurrentRoundID()
{
    return currentRoundID;
}

bool PetBattleWild::SkipTurnHandler(uint32 _roundID)
{
    PetBattleRoundResults* round = new PetBattleRoundResults(_roundID);
    // default state - need system for control it
    petBattleState = 2;

    // check active pets
    PetBattleInfo* allyPet = GetFrontPet(TEAM_ALLY);
    PetBattleInfo* enemyPet = GetFrontPet(TEAM_ENEMY);

    if (!allyPet || !enemyPet)
        return false;

    round->ProcessSkipTurn(allyPet->GetPetID());

    // response enemy after player pet skip turn
    uint32 castAbilityID = enemyPet->GetAbilityID(0);

    if (!castAbilityID)
        return false;

    uint32 effectID = GetVisualEffectIDByAbilityID(castAbilityID);
    round->ProcessAbilityDamage(enemyPet, allyPet, castAbilityID, effectID, 1);

    // find right place for some handler
    if (allyPet->IsDead() && !GetTotalPetCountInTeam(allyPet->GetTeam(), true))
    {
        SetWinner(enemyPet->GetTeam());
        nextRoundFinal = true;
    }
    else if (allyPet->IsDead() && GetTotalPetCountInTeam(allyPet->GetTeam(), true) > 1)
        SetBattleState(3);

    round->AuraProcessingBegin();
    round->AuraProcessingEnd();

    // increase round number
    round->roundID++;
    SetCurrentRoundID(round->roundID);

    CheckTrapStatuses(round);
    CheckInputFlags(round);

    SendRoundResults(round);

    if (allyPet->IsDead() && GetTotalPetCountInTeam(TEAM_ALLY, true) == 1)
        ForceReplacePetHandler(round->roundID, GetLastAlivePetID(TEAM_ALLY), TEAM_ALLY);

    delete round;
    return true;
}

bool PetBattleWild::UseTrapHandler(uint32 _roundID)
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

    PetBattleRoundResults* round = new PetBattleRoundResults(_roundID);
    // default state - need system for control it
    petBattleState = 2;

    // demo
    PetBattleEffect* effect = new PetBattleEffect(allyPet->GetPetID(), 698, 5, 0, 0, 1, 1);
    PetBattleEffectTarget * target = new PetBattleEffectTarget(enemyPet->GetPetID(), 1);
    target->status = trapped;

    effect->AddTarget(target);
    round->AddEffect(effect);

    // demo
    if (!trapped)
    {
        // in response cast ability
    }
    else
    {
        enemyPet->SetCaptured(true);
        round->petXDiedNumbers.push_back(enemyPet->GetPetID());

        if (!GetTotalPetCountInTeam(TEAM_ENEMY, true))
        {
            SetWinner(TEAM_ALLY);
            nextRoundFinal = true;
        }
    }

    round->AuraProcessingBegin();
    round->AuraProcessingEnd();

    // set trap status
    round->SetTrapStatus(TEAM_ALLY, PET_BATTLE_TRAP_ERR_8);

    // increase round
    round->roundID++;
    SetCurrentRoundID(round->roundID);

    SendRoundResults(round);

    if (GetTotalPetCountInTeam(TEAM_ENEMY, true))
        ForceReplacePetHandler(round->roundID, enemyPet->GetPetID() + 1, TEAM_ENEMY);

    delete round;
    return true;
}

bool PetBattleWild::SwapPetHandler(uint8 newFrontPet, uint32 _roundID)
{
    PetBattleRoundResults* round = new PetBattleRoundResults(_roundID);
    // default state - need system for control it
    petBattleState = 2;

    // check active pets
    PetBattleInfo* allyPet = GetFrontPet(TEAM_ALLY);
    PetBattleInfo* enemyPet = GetFrontPet(TEAM_ENEMY);

    if (!allyPet || !enemyPet)
        return false;

    if (newFrontPet < 0 || newFrontPet > 2)
        return false;

    if (allyPet->GetPetID() == newFrontPet)
    {
        SkipTurnHandler(_roundID);
        return true;
    }

    round->ProcessPetSwap(allyPet->GetPetID(), newFrontPet);

    SetFrontPet(TEAM_ALLY, newFrontPet);
    // check front pet
    allyPet = GetFrontPet(TEAM_ALLY);

    if (!allyPet)
        return false;

    // response enemy after player pet swap
    uint32 castAbilityID = enemyPet->GetAbilityID(0);

    if (!castAbilityID)
        return false;

    uint32 effectID = GetVisualEffectIDByAbilityID(castAbilityID);
    round->ProcessAbilityDamage(enemyPet, allyPet, castAbilityID, effectID, 1);

    // find right place for some handler
    if (allyPet->IsDead() && !GetTotalPetCountInTeam(allyPet->GetTeam(), true))
    {
        SetWinner(enemyPet->GetTeam());
        nextRoundFinal = true;
    }
    else if (allyPet->IsDead() && GetTotalPetCountInTeam(allyPet->GetTeam(), true) > 1)
        SetBattleState(3);

    round->AuraProcessingBegin();
    round->AuraProcessingEnd();

    // increase round number
    round->roundID++;
    SetCurrentRoundID(round->roundID);

    CheckTrapStatuses(round);
    SendRoundResults(round);

    if (allyPet->IsDead() && GetTotalPetCountInTeam(TEAM_ALLY, true) == 1)
        ForceReplacePetHandler(round->roundID, GetLastAlivePetID(TEAM_ALLY), TEAM_ALLY);

    delete round;
    return true;
}

void PetBattleWild::CheckTrapStatuses(PetBattleRoundResults* round)
{
    // check active pets
    PetBattleInfo* allyPet = GetFrontPet(TEAM_ALLY);
    PetBattleInfo* enemyPet = GetFrontPet(TEAM_ENEMY);

    if (!allyPet || !enemyPet)
        return;

    // default for wild pets and NPC
    round->SetTrapStatus(TEAM_ENEMY, PET_BATTLE_TRAP_ERR_2);

    if (enemyPet->Caged() || enemyPet->Captured())
        return;

    uint8 allyTrapStatus = PET_BATTLE_TRAP_ERR_4;

    if (enemyPet->GetHealthPct() < 20)
        allyTrapStatus = PET_BATTLE_TRAP_ACTIVE;

    // some checks
    uint32 creatureEntry = enemyPet->GetCreatureEntry();
    if (m_player->GetBattlePetMgr()->GetPetCount(creatureEntry) >= 3)
        allyTrapStatus = PET_BATTLE_TRAP_ERR_5;

    if (allyPet->IsDead() || enemyPet->IsDead())
        allyTrapStatus = PET_BATTLE_TRAP_ERR_3;

    round->SetTrapStatus(TEAM_ALLY, allyTrapStatus);
}

void PetBattleWild::CheckInputFlags(PetBattleRoundResults* round)
{
    for (uint8 i = 0; i < 2; ++i)
        round->SetInputFlags(i, 0);

    // check special state
    if (GetBattleState() == 3)
    {
        round->SetInputFlags(TEAM_ALLY, 8);
        round->SetInputFlags(TEAM_ENEMY, 6);
    }
}

void PetBattleWild::SendRoundResults(PetBattleRoundResults* result)
{
    WorldPackets::BattlePet::BattleRound round(SMSG_PET_BATTLE_ROUND_RESULT);
    round.MsgData.Initialize(result);
    m_player->GetSession()->SendPacket(round.Write());
}

bool PetBattleWild::FinalRoundHandler(bool abandoned)
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
        
        Unit::AuraEffectList const& auras = m_player->GetAuraEffectsByType(SPELL_AURA_MOD_BATTLE_PET_XP_GAIN);
        for (Unit::AuraEffectList::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
            rewardXp *= (*itr)->GetAmount();

        if (allyPet->GetLevel() == MAX_BATTLE_PET_LEVEL)
            rewardXp = 0;

        newXp = oldXp + rewardXp;
        uint32 totalXp = m_player->GetBattlePetMgr()->GetXPForNextLevel(allyPet->GetLevel());

        // check level-up
        if (newXp >= totalXp)
        {
            allyPet->SetNewLevel(allyPet->GetLevel() + 1);
            uint16 remXp = newXp - totalXp;
            allyPet->SetTotalXP(remXp);

            // recalculate stats
            BattlePetStatAccumulator* accumulator = new BattlePetStatAccumulator(allyPet->GetSpeciesID(), allyPet->GetBreedID());
            accumulator->CalcQualityMultiplier(allyPet->GetQuality(), allyPet->GetNewLevel());
            uint32 health = accumulator->CalculateHealth();
            uint32 power = accumulator->CalculatePower();
            uint32 speed = accumulator->CalculateSpeed();
            delete accumulator;

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
            for (BattleInfo::const_iterator itr = battleInfo.begin(); itr != battleInfo.end(); ++itr)
            {
                PetBattleInfo * pb = (*itr);

                if (!pb || pb->GetTeam() != TEAM_ALLY)
                    continue;

                if (!pb->IsDead())
                {
                    int32 percent10 = pb->GetHealth() / 10;
                    int32 newHealth = pb->GetHealth() - percent10;
                    pb->SetHealth(newHealth);
                }
            }
        }
    }

    SendFinalRound();

    return true;
}

void PetBattleWild::SendFinalRound()
{
    WorldPackets::BattlePet::SceneObjectFinalRound round;
    round.MsgData.Abandoned = abandoned;
    round.MsgData.PvpBattle = false;

    for (uint8 i = 0; i < 2; i++)
    {
        round.MsgData.Winner[i] = winners[i];
        round.MsgData.NpcCreatureID[i] = 0;

        for (auto const& n : battleInfo)
        {
            PetBattleInfo* pb = n;
            if (!pb || pb->GetTeam() != i)
                continue;

            WorldPackets::BattlePet::FinalPet pet;
            pet.Guid = pb->GetGUID();
            pet.Level = pb->GetNewLevel();
            pet.Health = pb->GetHealth();
            pet.MaxHealth = pb->GetMaxHealth();
            pet.InitialLevel = pb->GetLevel();
            pet.Pboid = pb->GetPetID();
            pet.Captured = pb->Captured();
            pet.Caged = pb->Caged();
            pet.SeenAction = false;
            pet.AwardedXP = pb->GetTotalXP() || (pb->GetLevel() < pb->GetNewLevel());
            if (pb->GetTotalXP())
                pet.Xp = pb->GetTotalXP();

            round.MsgData.Pets.push_back(pet);
        }
    }

    m_player->GetSession()->SendPacket(round.Write());
}

void PetBattleWild::SendFinishPetBattle()
{
    m_player->GetSession()->SendPacket(WorldPackets::BattlePet::NullSMsg(SMSG_PET_BATTLE_FINISHED).Write());
}

void PetBattleWild::UpdatePetsAfterBattle()
{
    std::list<ObjectGuid> updates;
    updates.clear();

    // find trapped pets
    for (uint8 i = 0; i < 2; ++i)
    {
        for (BattleInfo::const_iterator itr = battleInfo.begin(); itr != battleInfo.end(); ++itr)
        {
            PetBattleInfo * pb = (*itr);

            if (!pb || pb->GetTeam() != i)
                continue;

            if (i == TEAM_ENEMY && !pb->Captured())
                continue;

            // update loadout
            if (i == TEAM_ALLY)
            {
                PetJournalInfo* loadoutInfo = m_player->GetBattlePetMgr()->GetPetInfoByPetGUID(pb->GetGUID());

                if (!loadoutInfo)
                    continue;

                // recalculate stats
                BattlePetStatAccumulator* accumulator = new BattlePetStatAccumulator(pb->GetSpeciesID(), pb->GetBreedID());
                accumulator->CalcQualityMultiplier(pb->GetQuality(), pb->GetNewLevel());
                uint32 health = accumulator->CalculateHealth();
                uint32 power = accumulator->CalculatePower();
                uint32 speed = accumulator->CalculateSpeed();
                delete accumulator;

                // update
                loadoutInfo->SetLevel(pb->GetNewLevel());
                if (pb->IsDead())
                    loadoutInfo->SetHealth(0);
                else
                    loadoutInfo->SetHealth(pb->GetHealth());
                loadoutInfo->SetQuality(pb->GetQuality());
                loadoutInfo->SetXP(pb->GetTotalXP());
                loadoutInfo->SetPower(power);
                loadoutInfo->SetSpeed(speed);
                loadoutInfo->SetMaxHealth(health);
                loadoutInfo->SetState(STATE_UPDATED);

                updates.push_back(pb->GetGUID());
            }
            // update trapped pets and added in journal
            else
            {
                ObjectGuid petguid = ObjectGuid::Create<HighGuid::BattlePet>(sObjectMgr->GetGenerator<HighGuid::BattlePet>()->Generate());

                BattlePetStatAccumulator* accumulator = new BattlePetStatAccumulator(pb->GetSpeciesID(), pb->GetBreedID());
                accumulator->CalcQualityMultiplier(pb->GetQuality(), pb->GetLevel());
                uint32 health = accumulator->CalculateHealth();
                uint32 power = accumulator->CalculatePower();
                uint32 speed = accumulator->CalculateSpeed();
                delete accumulator;

                m_player->GetBattlePetMgr()->AddPetToList(petguid, pb->GetSpeciesID(), pb->GetCreatureEntry(), pb->GetLevel(), pb->GetDisplayID(), power, speed, pb->GetHealth(), health, pb->GetQuality(), 0, 0, pb->GetSummonSpell(), "", pb->GetBreedID(), STATE_UPDATED);
                // hack, fix it!
                if (pb->GetSummonSpell())
                    m_player->learnSpell(pb->GetSummonSpell(), false);

                std::list<ObjectGuid> newPets;
                newPets.clear();
                newPets.push_back(petguid);

                m_player->GetBattlePetMgr()->SendUpdatePets(newPets, true);
                m_player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ADD_BATTLE_PET_JOURNAL, pb->GetCreatureEntry());
                m_player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COLLECT_BATTLEPET);
                m_player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_PET_IN_BATTLE, pb->GetSpeciesID(), pb->GetQuality(), pb->GetType());
                m_player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLE_PET_CREDIT);
            }

            if (GetWinner() == TEAM_ALLY)
            {
                m_player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BATTLEPET_WIN, 0, 0, pb->GetType());

                if (pb->GetNewLevel() > pb->GetLevel())
                    m_player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BATTLEPET_LEVEL_UP, pb->GetNewLevel());
            }
            else
                m_player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BATTLEPET_WIN, ACHIEVEMENT_CRITERIA_CONDITION_NO_LOSE_PET_BATTLE);
        }
    }

    if (!updates.empty())
        m_player->GetBattlePetMgr()->SendUpdatePets(updates, false);
}

void PetBattleWild::FinishPetBattle(bool error)
{
    SendFinishPetBattle();

    if (!error)
        UpdatePetsAfterBattle();

    m_player->GetBattlePetMgr()->CloseWildPetBattle();
}

uint32 PetBattleWild::GetVisualEffectIDByAbilityID(uint32 abilityID, uint8 turnIndex)
{
    for (BattlePetAbilityTurnEntry const* tEntry : sBattlePetAbilityTurnStore)
    {
        if (tEntry->AbilityID == abilityID && tEntry->turnIndex == turnIndex)
        {
            for (BattlePetAbilityEffectEntry const* eEntry : sBattlePetAbilityEffectStore)
                if (eEntry->TurnEntryID == tEntry->ID)
                    return eEntry->ID;
        }
    }

    return 0;
}

// PetBattleAbility
PetBattleAbilityInfo::PetBattleAbilityInfo(uint32 _ID, uint32 _speciesID)
{
    BattlePetAbilityEntry const *abilityEntry = sBattlePetAbilityStore.LookupEntry(_ID);

    if (abilityEntry)
    {
        // get data from abilityEntry
        ID = abilityEntry->ID;
        Type = abilityEntry->Type;
        turnCooldown = abilityEntry->turnCooldown;
        auraAbilityID = abilityEntry->auraAbilityID;
        auraDuration = abilityEntry->auraDuration;
        // get data from xAbilityEntry
        // fucking blizzard...
        for (BattlePetSpeciesXAbilityEntry const* xEntry : sBattlePetSpeciesXAbilityStore)
        {
            if (xEntry->abilityID == abilityEntry->ID && xEntry->speciesID == _speciesID)
            {
                requiredLevel = xEntry->requiredLevel;
                rank = xEntry->rank;
                break;
            }
        }
    }
}

uint32 PetBattleAbilityInfo::GetEffectProperties(uint8 properties, uint32 effectIdx, uint32 turnIndex)
{
    char* desc = "Points";
    // get string for properties
    switch (properties)
    {
        case 1: desc = "Accuracy"; break;
        default: break;
    }

    // get turn data entry
    for (BattlePetAbilityTurnEntry const* tEntry : sBattlePetAbilityTurnStore)
    {
        if (tEntry->AbilityID == ID && tEntry->turnIndex == turnIndex)
        {
            // get effect data
            for (BattlePetAbilityEffectEntry const* eEntry : sBattlePetAbilityEffectStore)
            {
                if (eEntry->TurnEntryID == tEntry->ID)
                {
                    // get effect properties data
                    for (BattlePetEffectPropertiesEntry const* pEntry : sBattlePetEffectPropertiesStore)
                    {
                        if (eEntry->propertiesID == pEntry->ID)
                        {
                            for (uint8 l = 0; l < MAX_EFFECT_PROPERTIES; ++l)
                            {
                                if (!strcmp(pEntry->propertyDescs[DEFAULT_LOCALE][l].Str[DEFAULT_LOCALE], desc))
                                    return eEntry->propertyValues[l];
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}

float PetBattleAbilityInfo::GetAttackModifier(uint8 attackType, uint8 defenseType)
{
    // maybe number of attack types?
    uint32 formulaValue = 0xA;
    uint32 modId = defenseType * formulaValue + Type;

    //! TODO: check IT
    if (GtBattlePetTypeDamageModEntry const* gt = sGtBattlePetTypeDamageModStore.EvaluateTable(defenseType-1, Type-1))
        return gt->value;

    //for (GtBattlePetTypeDamageModEntry const* gt : sGtBattlePetTypeDamageModStore)
    //{
    //    if (gt->Id == modId)
    //        return gt->value;
    //}

    return 0.0f;
}

// PetJournalInfo
uint32 PetJournalInfo::GetAbilityID(uint8 rank)
{
    for (BattlePetSpeciesXAbilityEntry const* xEntry : sBattlePetSpeciesXAbilityStore)
    {
        if (xEntry->speciesID == speciesID && xEntry->rank == rank)
        {
            if (rank == 0)
            {
                if (flags & BATTLE_PET_FLAG_CUSTOM_ABILITY_1)
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
                if (flags & BATTLE_PET_FLAG_CUSTOM_ABILITY_2)
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
                if (flags & BATTLE_PET_FLAG_CUSTOM_ABILITY_3)
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
}

// PetBattleRoundResults
PetBattleRoundResults::~PetBattleRoundResults()
{
    // delete targets
    for (std::list<PetBattleEffect*>::iterator itr = effects.begin(); itr != effects.end(); ++itr)
        for (std::list<PetBattleEffectTarget*>::iterator itr2 = (*itr)->targets.begin(); itr2 != (*itr)->targets.end(); ++itr2)
            delete (*itr2);

    // delete effects
    for (std::list<PetBattleEffect*>::iterator itr = effects.begin(); itr != effects.end(); ++itr)
        delete (*itr);
}

void PetBattleRoundResults::ProcessAbilityDamage(PetBattleInfo* caster, PetBattleInfo* target, uint32 abilityID, uint32 effectID, uint8 turnInstanceID)
{
    // check on dead
    if (caster->IsDead() || target->IsDead())
        return;

    // base pre-calculate damage and flags
    PetBattleAbilityInfo* ainfo = new PetBattleAbilityInfo(abilityID, caster->GetSpeciesID());
    uint16 flags = ainfo->CalculateHitResult(caster, target);
    bool crit = false;
    if (flags & PETBATTLE_EFFECT_FLAG_CRIT)
        crit = true;
    uint32 damage = ainfo->CalculateDamage(caster, target, crit);

    // simple combination of effect 0 and target type 6
    PetBattleEffect* effect = new PetBattleEffect(caster->GetPetID(), effectID, 0, flags, 0, turnInstanceID, 1);
    PetBattleEffectTarget* t = new PetBattleEffectTarget(target->GetPetID(), 6);

    // process in battle
    int32 newHealth = target->GetHealth() - damage;
    target->SetHealth(newHealth);

    // process for round result
    t->remainingHealth = newHealth;

    effect->AddTarget(t);
    AddEffect(effect);

    // process if victim dead
    if (target->IsDead())
    {
        // added petID to died ID
        AddDeadPet(target->GetPetID());
        // set state DEAD (testing!)
        ProcessSetState(caster, target, effectID, 1, 1, turnInstanceID);
    }
}

void PetBattleRoundResults::AddDeadPet(uint8 petNumber)
{
    petXDiedNumbers.push_back(petNumber);
}

void PetBattleRoundResults::ProcessSetState(PetBattleInfo* caster, PetBattleInfo* target, uint32 effectID, uint8 stateID, uint32 stateValue, uint8 turnInstanceID)
{
    PetBattleEffect* effect = new PetBattleEffect(caster->GetPetID(), effectID, 6, 0, 0, turnInstanceID, 1);
    PetBattleEffectTarget* t = new PetBattleEffectTarget(target->GetPetID(), 4);

    t->stateID = stateID;
    t->stateValue = stateValue;

    effect->AddTarget(t);
    AddEffect(effect);
}

void PetBattleRoundResults::ProcessPetSwap(uint8 oldPetNumber, uint8 newPetNumber)
{
    // simple combination of effect 4 and target type 3
    PetBattleEffect* effect = new PetBattleEffect(oldPetNumber, 0, 4, 0, 0, 0, 0);
    PetBattleEffectTarget* target = new PetBattleEffectTarget(newPetNumber, 3);

    effect->AddTarget(target);
    AddEffect(effect);
}

void PetBattleRoundResults::ProcessSkipTurn(uint8 petNumber)
{
    // simple combination of effect 4 and target type 3
    PetBattleEffect* effect = new PetBattleEffect(petNumber, 0, 4, 1, 0, 0, 0);
    PetBattleEffectTarget* target = new PetBattleEffectTarget(petNumber, 3);

    effect->AddTarget(target);
    AddEffect(effect);
}

void PetBattleRoundResults::AuraProcessingBegin()
{
    PetBattleEffect* effect = new PetBattleEffect(-1, 0, 13, 0, 0, 0, 0);
    PetBattleEffectTarget* target = new PetBattleEffectTarget(-1, 3);

    effect->AddTarget(target);
    AddEffect(effect);
}

void PetBattleRoundResults::AuraProcessingEnd()
{
    PetBattleEffect* effect = new PetBattleEffect(-1, 0, 14, 0, 0, 0, 0);
    PetBattleEffectTarget* target = new PetBattleEffectTarget(-1, 3);

    effect->AddTarget(target);
    AddEffect(effect);
}

int32 PetBattleAbilityInfo::GetBaseDamage(PetBattleInfo* attacker, uint32 effectIdx, uint32 turnIndex)
{
    uint32 basePoints = GetEffectProperties(BASEPOINTS, effectIdx, turnIndex);
    return basePoints * (1 + attacker->GetPower() * 0.05f);
}

int32 PetBattleAbilityInfo::CalculateDamage(PetBattleInfo* attacker, PetBattleInfo* victim, bool crit)
{
    int32 baseDamage = GetBaseDamage(attacker);
    int32 cleanDamage = urand(baseDamage - 5, baseDamage + 5);
    // mods
    float mod = GetAttackModifier(GetType(), victim->GetType());
    int32 finalDamage = cleanDamage * mod;
    if (crit)
        finalDamage *= 2;

    return finalDamage;
}

int16 PetBattleAbilityInfo::CalculateHitResult(PetBattleInfo* attacker, PetBattleInfo* victim)
{
    uint16 flags = PETBATTLE_EFFECT_FLAG_BASE;
    // mods
    float mod = GetAttackModifier(GetType(), victim->GetType());

    if (roll_chance_i(5))
        flags |= PETBATTLE_EFFECT_FLAG_CRIT;

    if (mod > 1.0f)
        flags |= PETBATTLE_EFFECT_FLAG_STRONG;
    else if (mod < 1.0f)
        flags |= PETBATTLE_EFFECT_FLAG_WEAK;

    return flags;
}