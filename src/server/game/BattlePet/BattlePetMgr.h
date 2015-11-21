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

#ifndef __TRINITY_BATTLEPETMGR_H
#define __TRINITY_BATTLEPETMGR_H

#include <map>
#include <string>

#include "Common.h"
#include "DatabaseEnv.h"
#include "DBCEnums.h"
#include "DBCStores.h"
#include "DB2Stores.h"
#include "Packets/BattlePetPackets.h"

enum BattlePetError
{
    BATTLEPETRESULT_TOO_HIGH_LEVEL_TO_UNCAGE         = 1,
    BATTLEPETRESULT_CANT_HAVE_MORE_PETS              = 9,
    BATTLEPETRESULT_CANT_HAVE_MORE_PETS_OF_THAT_TYPE = 12,

    // wrong order
    BATTLEPETRESULT_DUPLICATE_CONVERTED_PET,
    BATTLEPETRESULT_NEED_TO_UNLOCK,
    BATTLEPETRESULT_BAD_PARAM,
    BATTLEPETRESULT_LOCKED_PET_ALREADY_EXISTS,
    BATTLEPETRESULT_OK,
    BATTLEPETRESULT_UNCAPTURABLE,
    BATTLEPETRESULT_CANT_INVALID_CHARACTER_GUID
};

enum BattlePetsMiscData
{
    MAX_ACTIVE_BATTLE_PETS          = 3,
    MAX_ACTIVE_BATTLE_PET_ABILITIES = 3,
    MAX_BATTLE_PET_LEVEL            = 25,
    SPELL_SUMMON_BATTLE_PET         = 118301,
    SPELL_BATTLE_PET_TRAINING       = 119467,
};

enum BattlePetSaveInfo
{
    STATE_NORMAL  = 0,
    STATE_UPDATED = 1,
    STATE_NEW     = 2,
    STATE_DELETED = 3
};

enum BattlePetTeam
{
    TEAM_ALLY   = 0,
    TEAM_ENEMY  = 1,

    MAX_PETBATTLE_TEAMS
};

enum BattlePetBreeds
{
    BATTLE_PET_BREED_BB = 3,  // 25% health, 25% power, 25% speed
    BATTLE_PET_BREED_PP = 4,  // 100% power
    BATTLE_PET_BREED_SS = 5,  // 100% speed
    BATTLE_PET_BREED_HH = 6,  // 100% health
    BATTLE_PET_BREED_HP = 7,  // 45% health, 45% power
    BATTLE_PET_BREED_PS = 8,  // 45% power, 45% speed
    BATTLE_PET_BREED_HS = 9,  // 45% health, 45% speed
    BATTLE_PET_BREED_PB = 10, // 45% power, 20% health, 20% speed
    BATTLE_PET_BREED_SB = 11, // 45% speed, 20% power, 20% health
    BATTLE_PET_BREED_HB = 12, // 45% health, 20% power, 20% speed
};

enum BattlePetDBFlags
{
    BATTLE_PET_FLAG_FAVORITE            = 0x01,
    BATTLE_PET_FLAG_UNK                 = 0x02,
    BATTLE_PET_FLAG_REVOKED             = 0x04,
    BATTLE_PET_FLAG_LOCKED_FOR_CONVERT  = 0x08,
    BATTLE_PET_FLAG_CUSTOM_ABILITY_1    = 0x10,
    BATTLE_PET_FLAG_CUSTOM_ABILITY_2    = 0x20,
    BATTLE_PET_FLAG_CUSTOM_ABILITY_3    = 0x40
};

enum BattlePetDBSpeciesFlags
{
    SPECIES_FLAG_UNK1                   = 0x02,
    SPECIES_FLAG_UNK2                   = 0x04,
    SPECIES_FLAG_CAPTURABLE             = 0x08,
    SPECIES_FLAG_CANT_TRADE             = 0x10, // ~(unsigned __int8)(v3->speciesFlags >> 4) & 1 (cannot put in cage)
    SPECIES_FLAG_UNOBTAINABLE           = 0x20,
    SPECIES_FLAG_UNIQUE                 = 0x40, // (v2->speciesFlags >> 6) & 1)
    SPECIES_FLAG_CANT_BATTLE            = 0x80,
    SPECIES_FLAG_UNK3                   = 0x200,
    SPECIES_FLAG_ELITE                  = 0x400,
};

enum BattlePetSpeciesSource
{
    SOURCE_LOOT                         = 0,
    SOURCE_QUEST                        = 1,
    SOURCE_VENDOR                       = 2,
    SOURCE_PROFESSION                   = 3,
    SOURCE_PET_BATTLE                   = 4,
    SOURCE_ACHIEVEMENT                  = 5,
    SOURCE_GAME_EVENT                   = 6,
    SOURCE_PROMO                        = 7,
    SOURCE_TCG                          = 8,
    SOURCE_NOT_AVALIABLE                = 0xFFFFFFFF,
};

enum BattlePetFamily
{
    BATTLE_PET_FAMILY_HUMANOID      = 0,
    BATTLE_PET_FAMILY_DRAGONKIN     = 1,
    BATTLE_PET_FAMILY_FLYING        = 2,
    BATTLE_PET_FAMILY_UNDEAD        = 3,
    BATTLE_PET_FAMILY_CRITTER       = 4,
    BATTLE_PET_FAMILY_MAGIC         = 5,
    BATTLE_PET_FAMILY_ELEMENTAL     = 6,
    BATTLE_PET_FAMILY_BEAST         = 7,
    BATTLE_PET_FAMILY_AQUATIC       = 8,
    BATTLE_PET_FAMILY_MECHANICAL    = 9,

    BATTLE_PET_FAMILY_MAX
};

BattlePetState const FamilyStates[BATTLE_PET_FAMILY_MAX] =
{
    STATE_PASSIVE_HUMANOID,
    STATE_PASSIVE_DRAGON,
    STATE_PASSIVE_FLYING,
    STATE_PASSIVE_UNDEAD,
    STATE_PASSIVE_CRITTER,
    STATE_PASSIVE_MAGIC,
    STATE_PASSIVE_ELEMENTAL,
    STATE_PASSIVE_BEAST,
    STATE_PASSIVE_AQUATIC,
    STATE_PASSIVE_MECHANICAL
};

enum BattlePetEffectProperties
{
    BASEPOINTS = 0,
    ACCURACY   = 1
};

enum PetBattleEffectFlags : uint16
{
    PETBATTLE_EFFECT_FLAG_INVALID_TARGET    = 0x0001,
    PETBATTLE_EFFECT_FLAG_MISS              = 0x0002,
    PETBATTLE_EFFECT_FLAG_CRIT              = 0x0004,
    PETBATTLE_EFFECT_FLAG_BLOCKED           = 0x0008,
    PETBATTLE_EFFECT_FLAG_DODGE             = 0x0010,
    PETBATTLE_EFFECT_FLAG_HEAL              = 0x0020,
    PETBATTLE_EFFECT_FLAG_UNKILLABLE        = 0x0040,
    PETBATTLE_EFFECT_FLAG_REFLECT           = 0x0080,
    PETBATTLE_EFFECT_FLAG_ABSORB            = 0x0100,
    PETBATTLE_EFFECT_FLAG_IMMUNE            = 0x0200,
    PETBATTLE_EFFECT_FLAG_STRONG            = 0x0400,
    PETBATTLE_EFFECT_FLAG_WEAK              = 0x0800,
    PETBATTLE_EFFECT_FLAG_BASE              = 0x1000
};

enum TrapStatus : int32
{
    PET_BATTLE_TRAP_ACTIVE                  = 1,  // active trap button
    PET_BATTLE_TRAP_ERR_2                   = 2,  // "Traps become available when one of your pets reaches level 3."
    PET_BATTLE_TRAP_ERR_3                   = 3,  // "You cannot trap a dead pet."
    PET_BATTLE_TRAP_ERR_4                   = 4,  // "The enemy pet's health is not low enough."
    PET_BATTLE_TRAP_ERR_5                   = 5,  // "You already have 3 of this pet or your journal is full."
    PET_BATTLE_TRAP_ERR_6                   = 6,  // "Your enemy pet is not caputurable."
    PET_BATTLE_TRAP_ERR_7                   = 7,  // "Can't trap an NPC's pets"
    PET_BATTLE_TRAP_ERR_8                   = 8   // "Can't trap twice in same battle"
};

uint32 const TrapSpells[4] = {427, 77, 135, 1368};
uint32 const MaxRoundTime[2] = {0, 30};

enum PetPvpBattleCompatibility
{
    PET_PVP_BATTLE_PENDING,
    PET_PVP_BATTLE_HAS_IGNORES,
    PET_PVP_BATTLE_TOO_DIFFERENT_LEVELS,
    PET_PVP_BATTLE_MATCH
};

// demo
enum DeathPetResult
{
    HAVE_ALIVE_PETS_MORE_ONE_ALLY           = 0,
    LAST_ALIVE_PET_ALLY                     = 1,
    HAVE_ALIVE_PETS_MORE_ONE_ENEMY          = 2,
    NO_ALIVE_PETS_ALLY                      = 3,
    NO_ALIVE_PETS_ENEMY                     = 4
};

enum PetBattleQueueStatus : int32
{
    PET_BATTLE_QUEUE_STATUS_INIT,
    PET_BATTLE_QUEUE_STATUS_JOINED,

    PET_BATTLE_QUEUE_STATUS_LEAVE = 12,
};

enum MoveType
{
    MOVE_TYPE_NONE                  = 0,
    MOVE_TYPE_USE_ABILITY           = 1,
    MOVE_TYPE_SWAP_FRONT_PET        = 2,
    MOVE_TYPE_USE_TRAP              = 3,
    MOVE_TYPE_REJECTION_FROM_BATTLE = 4,
};

enum PetBattleEffectType
{
    PETBATTLE_EFFECT_TYPE_INVALID               = -1,
    PETBATTLE_EFFECT_TYPE_SET_HEALTH            = 0,
    PETBATTLE_EFFECT_TYPE_AURA_APPLY            = 1,
    PETBATTLE_EFFECT_TYPE_AURA_CANCEL           = 2,
    PETBATTLE_EFFECT_TYPE_AURA_CHANGE           = 3,
    PETBATTLE_EFFECT_TYPE_PET_SWAP              = 4,
    PETBATTLE_EFFECT_TYPE_STATUS_CHANGE         = 5,
    PETBATTLE_EFFECT_TYPE_SET_STATE             = 6,
    PETBATTLE_EFFECT_TYPE_SET_MAX_HEALTH        = 7,
    PETBATTLE_EFFECT_TYPE_SET_SPEED             = 8,
    PETBATTLE_EFFECT_TYPE_SET_POWER             = 9,
    PETBATTLE_EFFECT_TYPE_TRIGGER_ABILITY       = 10,
    PETBATTLE_EFFECT_TYPE_ABILITY_CHANGE        = 11,
    PETBATTLE_EFFECT_TYPE_NPC_EMOTE             = 12,
    PETBATTLE_EFFECT_TYPE_AURA_PROCESSING_BEGIN = 13,
    PETBATTLE_EFFECT_TYPE_AURA_PROCESSING_END   = 14
};

enum PetBattleState
{
    PETBATTLE_STATE_INVALID                 = -1,
    PETBATTLE_STATE_CREATED                 = 0,
    PETBATTLE_STATE_WAITING_PRE_BATTLE      = 1,
    PETBATTLE_STATE_ROUND_IN_PROGRESS       = 2,
    PETBATTLE_STATE_WAITING_FOR_FRONT_PETS  = 3,
    PETBATTLE_STATE_CREATED_FAILED          = 4,
    PETBATTLE_STATE_FINAL_ROUND             = 5,
    PETBATTLE_STATE_FINISHED                = 6
};

enum EffectTarget //@TODO: fixup enum names...
{
    EFFECT_TARGET_NONE,
    EFFECT_TARGET_1 = 1,
    EFFECT_TARGET_3 = 3,
    EFFECT_TARGET_4 = 4,
    EFFECT_TARGET_6 = 6,
};


class PetBattleInfo
{
public:
    PetBattleInfo() { }
    void SetGUID(ObjectGuid _guid) { guid = _guid; }
    ObjectGuid GetGUID() { return guid; }
    void SetPetID(uint8 petNumber) { petX = petNumber; }
    uint8 GetPetID() { return petX; }
    void CopyPetInfo(Player* player, ObjectGuid petGuid/*BattlePetMgr::BattlePet* petInfo*/);
    void SetTeam(uint8 _team) { team = _team; }
    uint8 GetTeam() { return team; }
    void SetFrontPet(bool apply) { frontPet = apply; }
    bool IsFrontPet() { return frontPet; }
    void SetAbilityID(uint32 abilityID, uint8 index) { abilities[index] = abilityID; }

    uint32 GetAbilityID(uint8 index) { return abilities[index]; }
    void SetCaptured(bool apply) { captured = apply; caged = apply; }

    bool Captured() { return captured; }
    bool Caged() { return caged; }

    std::string GetCustomName() { return customName; }
    uint16 GetXP() { return xp; }
    void SetTotalXP(uint16 _xp) { totalXP = _xp; }
    uint16 GetTotalXP() { return totalXP; }
    void SetLevel(uint8 _level) { level = _level; }
    uint8 GetLevel() { return level; }
    void SetNewLevel(uint8 _level) { newLevel = _level; }
    uint8 GetNewLevel() { return newLevel; }
    int32 GetHealth() { return health; }
    float GetHealthPct() { return maxHealth ? 100.f * health / maxHealth : 0.0f; }
    void SetHealth(int32 _health) { health = _health; }
    int32 GetMaxHealth() { return maxHealth; }
    void SetMaxHealth(int32 _maxHealth) { maxHealth = _maxHealth; }
    void SetPower(uint16 _power) { power = _power; }
    void SetSpeed(uint16 _speed) { speed = _speed; }
    uint16 GetSpeed() { return speed; }
    uint16 GetPower() { return power; }
    uint16 GetBreedID() { return breedID; }
    uint8 GetQuality() { return quality; }
    uint32 GetSpeciesID() { return speciesID; }
    uint32 GetSummonSpell() { return summonSpellID; }
    uint32 GetCreatureEntry() { return creatureEntry; }
    bool IsDead() { return health <= 0; }
    bool HasAbility(uint32 abilityID);

private:
    Player* _player;

    ObjectGuid guid;
    uint8 petX;
    uint8 team;
    uint32 speciesID;
    uint32 creatureEntry;
    uint16 power;
    uint16 speed;
    int32 health;
    int32 maxHealth;
    uint8 quality;
    uint16 xp;
    uint16 totalXP;
    uint8 level;
    uint8 newLevel;
    uint16 flags;
    int16 breedID;
    uint32 summonSpellID;
    std::string customName;
    uint32 abilities[MAX_ACTIVE_BATTLE_PET_ABILITIES];
    std::map<uint32, uint8> abilityCooldowns;
    uint8 status;
    bool frontPet;
    bool captured;
    bool caged;
};

typedef std::list<PetBattleInfo*> BattleInfo;

class PetBattle
{
public:
    struct RoundInfo
    {
        WorldPackets::BattlePet::PetBattleFullUpdate InitialUpdate;
        WorldPackets::BattlePet::FinalRound FinalRound;
    };

    struct RoundResults
    {
        struct Effect
        {
            struct Target
            {
                WorldPackets::BattlePet::EffectTarget PacketInfo;
            };

            WorldPackets::BattlePet::Effect PacketInfo;

            BattlePetAbilityEntry const* abilityEntry;
            int16 CalculateHitResult(PetBattleInfo* victim);
            int32 GetBaseDamage(PetBattleInfo* attacker, uint32 turnIndex = 1);
            int32 CalculateDamage(PetBattleInfo* attacker, PetBattleInfo* victim, bool crit);
            float GetAttackModifier(uint8 attackType, uint8 defenseType);
            uint32 GetProperties(uint8 properties, uint32 turnIndex = 1);
        };

        struct AbilityData
        {
            WorldPackets::BattlePet::ActiveAbility PacketInfo;
        };

        WorldPackets::BattlePet::RoundResult PacketInfo;

        void ProcessAbilityDamage(PetBattleInfo* caster, PetBattleInfo* target, uint32 abilityID, uint32 effectID, uint8 TurnInstanceID);
        void ProcessPetSwap(uint8 oldPetNumber, uint8 newPetNumber);
        void ProcessSkipTurn(uint8 petNumber);
        void ProcessSetState(PetBattleInfo* caster, PetBattleInfo* target, uint32 effectID, uint8 stateID, uint32 stateValue, uint8 TurnInstanceID);
        void AuraProcessingBegin();
        void AuraProcessingEnd();
    };

    PetBattle(Player* owner);

    bool CreateBattleInfo();
    bool PrepareBattleInfo(ObjectGuid creatureGuid);
    bool InitiPvEBattle(ObjectGuid creatureGuid);
    bool InitiPvPBattle(ObjectGuid creatureGuid);

    void Init(ObjectGuid creatureGuid);

    void InitializePetBattle(ObjectGuid opponentGuid);

    void ForceReplacePetHandler(uint32 roundID, uint8 newFrontPet, uint8 team);
    void ReplaceFrontPet(uint8 frontPet = 0);

    bool FirstRoundHandler(uint8 allyFrontPetID, uint8 enemyFrontPetID);
    bool UseAbilityHandler(uint32 abilityID, uint32 roundID);
    bool SkipTurnHandler(uint32 roundID);
    bool UseTrapHandler(uint32 roundID);
    bool SwapPetHandler(uint8 newFrontPet, uint32 roundID);
    void SendFirstRound(RoundResults* firstRound);
    void SendRoundResults(RoundResults* round);
    void SendForceReplacePet(RoundResults* round);
    bool FinalRoundHandler(bool abandoned);
    void SendFinalRound(bool pvpBattle = false);
    void CheckTrapStatuses(RoundResults* round);
    void CheckInputFlags(RoundResults* round);
    void UpdatePetsAfterBattle();

    void FinishPetBattle(bool error = false);
    void SendFinishPetBattle();

    PetBattleInfo* GetPet(uint8 petNumber);

    PetBattleInfo* GetFrontPet(uint8 team);
    PetBattleInfo* GetNotFrontPet(uint8 team);
    void SetFrontPet(uint8 team, uint8 petNumber);

    bool NextRoundFinal() { return nextRoundFinal; }
    void SetAbandoned(bool apply) { abandoned = apply; }

    void SetWinner(uint8 team);
    uint8 GetWinner();

    uint8 GetTotalPetCountInTeam(uint8 team, bool onlyActive = false);

    // only demo
    int8 GetLastAlivePetID(uint8 team);

    uint8 GetPetTeamIndex(uint8 petNumber);
    uint8 GetTeamByPetID(uint8 petNumber);

    uint32 GetCurrentRoundID();
    void SetCurrentRoundID(uint32 roundID) { currentRoundID = roundID; }
    void SetBattleState(uint8 state) { petBattleState = state; }
    uint8 GetBattleState() { return petBattleState; }

private:
    Player* _player;

protected:
    BattleInfo battleInfo;

    uint32 currentRoundID;      // CurRound             _initialInfo
    ObjectGuid teamGuids[MAX_TEAMS]; // Players              _initialInfo
    uint8 petBattleState;       // CurPetBattleState    _initialInfo
    bool nextRoundFinal;        // CurRound is next final check ? _initialInfo

    uint8 winners[MAX_TEAMS]; // Winner               _finalRoundInfo
    bool abandoned;             // Abandoned            _finalRoundInfo
};

class BattlePetMgr
{
public:
    static BattlePetMgr* instance()
    {
        static BattlePetMgr instance;
        return &instance;
    }

    struct PetPvpBattleCompatibilityData
    {
        PetPvpBattleCompatibilityData(): compatibility(PET_PVP_BATTLE_PENDING) { }
        PetPvpBattleCompatibilityData(PetPvpBattleCompatibility _compatibility): compatibility(_compatibility) { }
        PetPvpBattleCompatibilityData(PetPvpBattleCompatibility _compatibility, int32 _avgLevels):
            compatibility(_compatibility), avgLevels(_avgLevels) { }

        PetPvpBattleCompatibility compatibility;
        int32 avgLevels;
    };

    struct BattlePet
    {
        WorldPackets::BattlePet::BattlePetJournalInfo JournalInfo;

        BattlePetFamily GetFamily();

        int32 GetBaseStateValue(BattlePetState state);
        void CalculateStats();

        struct
        {
            uint32 SpellID = 0;
        } serverSideData;

        BattlePetSaveInfo SaveInfo;

        bool HasFlag(uint16 flag) { return (JournalInfo.BattlePetDBFlags & flag) != 0; }
        void SetFlag(uint16 flag) { if (!HasFlag(flag)) JournalInfo.BattlePetDBFlags |= flag; }
        void RemoveFlag(uint16 flag) { JournalInfo.BattlePetDBFlags &= ~flag; }
        float GetHealthPct() { return JournalInfo.MaxHealth ? 100.f * JournalInfo.Health / JournalInfo.MaxHealth : 0.0f; }
        bool IsDead() { return JournalInfo.Health <= 0; }
        bool IsHurt() { return !IsDead() && JournalInfo.Health < JournalInfo.MaxHealth; }
        uint32 GetAbilityID(uint8 rank);
    };

    struct QueueStatusData
    {
        QueueStatusData();
        QueueStatusData(time_t _joinTime, int32 avgLevel, PetBattleQueueStatus _status);

        time_t joinTime;
        int32 avgLevel;
        PetBattleQueueStatus status;
    };

    explicit BattlePetMgr(Player* owner);

    void Initialize();

    BattlePetMgr() { }
    ~BattlePetMgr();

    void LoadFromDB(PreparedQueryResult pets, PreparedQueryResult slots);
    void SaveToDB(SQLTransaction& trans);

    void AddPet(uint32 species, uint16 breed, uint8 quality, uint16 level = 1);
    BattlePet* GetPet(ObjectGuid guid);
    WorldPackets::BattlePet::BattlePetJournalInfo* GetPetInfo(ObjectGuid guid);

    void UnlockSlot(uint8 slot);

    void SendJournal();
    void ClosePetBattle();
    void SendUpdatePets(std::vector<BattlePet> updates, bool added = false);
    void HealBattlePetsPct(uint8 pct);

    void CreatePetBattle(Player* initiator, ObjectGuid wildCreatureGuid);
    void InitializePetBattle(Player* player, WorldPackets::BattlePet::BattleRequest packet);
    void SendPetBattleRequestFailed(uint8 reason);

    uint32 GetPetCount(uint32 speciesID);
    void DeletePetByPetGUID(ObjectGuid guid);

    ObjectGuid::LowType GetPetGUIDBySpell(uint32 spell);
    std::unordered_map<ObjectGuid::LowType /*battlePetGuid*/, BattlePetMgr::BattlePet> const& GetPetJournal() { return _pets; }

    std::vector<BattlePetMgr::BattlePet> GetLearnedPets() const;
    WorldPackets::BattlePet::BattlePetSlot* GetPetBattleSlot(uint8 slot) { return &_slots[slot]; }
    std::vector<WorldPackets::BattlePet::BattlePetSlot> GetPetBattleSlots() const { return _slots; }

    bool SlotIsLocked(uint8 index);

    bool AllSlotsEmpty();
    bool AllSlotsDead();
    bool CanAwardXP();
    uint32 CalcAvengerPetsLevelInSlots();
    bool PetIsSlotted(ObjectGuid guid);
    PetBattle* GetPetBattle() { return _petBattle; }
    uint32 GetXPForNextLevel(uint8 level);
    uint8 GetRandomQuailty();
    uint16 GetRandomBreedID(uint32 speciesID);
    uint8 GetWeightForBreed(uint16 breedID);

    void SendError(BattlePetError error, uint32 creatureId = 0);

    void UpdateWaitTimeAvg(int32 waitTime);
    void Update(uint32 diff);
    void SendPetBattleQueueStatus(QueueStatusData data);
    void AddQueueData(ObjectGuid guid, time_t joinTime, int32 avgLevel, PetBattleQueueStatus _status);
    void AddToQueue(ObjectGuid guid);
    void RemoveQueueData(ObjectGuid guid);
    QueueStatusData GetQueueData(ObjectGuid guid);
    void UpdateQueueTimers(time_t currTime);
    uint8 FindPvpBattle();
    PetPvpBattleCompatibility FindNewBattles(GuidList& check, GuidList& all);
    PetPvpBattleCompatibility GetCompatibles(std::string const& key);
    PetPvpBattleCompatibility CheckCompatibility(GuidList check);
    void SetCompatibles(std::string const& key, PetPvpBattleCompatibility compatibles);
    void JoinPvpBattleQueue();
    void LeavePvpBattleQueue();
    void SendBattleQueueProposeMatch();

private:
    Player* _player;
    std::unordered_map<ObjectGuid::LowType /*battlePetGuid*/, BattlePetMgr::BattlePet> _pets;
    std::vector<WorldPackets::BattlePet::BattlePetSlot> _slots;
    PetBattle* _petBattle;

    uint32 _queueTimer;
    uint32 _waitTimesAvgStore;

    GuidList _newToQueueStore;
    GuidList _currentQueueStore;
    std::map<ObjectGuid, QueueStatusData> _queueDataStore;
    std::map<std::string, PetPvpBattleCompatibilityData> _compatibleMapStore;
};

#define sBattlePetMgr BattlePetMgr::instance()

#endif
