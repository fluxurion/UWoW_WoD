/*
    http://epicwow.com/
    Dungeon : Iron Docks 93-95
    Encounter: Skulloc <Son of Gruul>
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "iron_docks.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
    SAY_EVADE           = ,
}; */

enum Spells
{
    //Skulloc
    SPELL_GRONN_SMASH           = 168227,
    SPELL_CANNON_BARRAGE        = 168929,
    SPELL_CANNON_BARRAGE_LOSS   = 168822,
    SPELL_BACKDRAFT             = 169131,
    SPELL_BACKDRAFT_DMG_MIN     = 169129,
    SPELL_BACKDRAFT_DMG_MAX     = 169132,
    SPELL_CHECK_FOR_PLAYERS     = 169071,
    SPELL_CHECK_FOR_PLAYERS_HIT = 168926,
    SPELL_CANCEL_CANNON_BARRAGE = 169072,
    //Koramar
    SPELL_SHATTERING_BLADE      = 169073,
    SPELL_BERSERKER_LEAP        = 168970,
    SPELL_BLADESTORM            = 168402,
    //Zaggosh
    SPELL_RIDE_VEHICLE          = 93970,
    SPELL_DUMMY_NUKE            = 172498, //?
    SPELL_RAPID_FIRE_TARGETING  = 168398,
    SPELL_RAPID_FIRE            = 168394,
};

enum eEvents
{
    //Skulloc
    EVENT_GRONN_SMASH         = 1,
    EVENT_CANNON_BARRAGE_1    = 2,
    EVENT_CANNON_BARRAGE_2    = 3,
    //Koramar
    EVENT_SHATTERING_BLADE    = 1,
    EVENT_BERSERKER_LEAP      = 2,
    //Turrnet
    EVENT_RAPID_FIRE          = 1,
};

enum Actions
{
    ACTION_KORMAR_START_BLADESTORM  = 1,
    ACTION_KORMAR_STOP_BLADESTORM   = 2,
};

uint32 const bossSkulloc[3] = { NPC_SKULLOC, NPC_KORAMAR, NPC_ZOGGOSH};

struct boss_encounter_skulloc : public BossAI
{
    boss_encounter_skulloc(Creature* creature) : BossAI(creature, DATA_SKULLOC) {}

    uint8 bossDiedCount;

    void Reset()
    {
        _Reset();
        events.Reset();
    }

    void EnterCombat(Unit* who)
    {
        if (instance->GetBossState(DATA_SKULLOC) != IN_PROGRESS)
            instance->SetBossState(DATA_SKULLOC, IN_PROGRESS);
    }

    void JustDied(Unit* /*killer*/)
    {
        events.Reset();

        bossDiedCount = 0;
        for (uint8 i = 0; i < 3; i++)
            if (Creature* enfor = ObjectAccessor::GetCreature(*me, instance->GetGuidData(bossSkulloc[i])))
                if (!enfor->isAlive())
                    bossDiedCount++;

        if (bossDiedCount == 3)
            instance->SetBossState(DATA_SKULLOC, DONE);
    }
};

class boss_skulloc : public CreatureScript
{
public:
    boss_skulloc() : CreatureScript("boss_skulloc") { }

    struct boss_skullocAI : public boss_encounter_skulloc
    {
        boss_skullocAI(Creature* creature) : boss_encounter_skulloc(creature), summons(me) {}

        SummonList summons;

        bool draftMin;
        bool lowHp;

        void Reset()
        {
            draftMin = true;
            lowHp = false;
            boss_encounter_skulloc::Reset();
            summons.DespawnAll();
            events.Reset();
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void EnterCombat(Unit* who)
        {
            boss_encounter_skulloc::EnterCombat(who);
            events.ScheduleEvent(EVENT_GRONN_SMASH, 30000); //26:16

            me->SummonCreature(NPC_BACKDRAFT, 6861.77f, -990.17f, 23.13f, 3.0f);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            summons.DespawnAll();
            boss_encounter_skulloc::JustDied(NULL);
        }

        void JustReachedHome()
        {
            DoCast(SPELL_CHECK_FOR_PLAYERS);
            events.ScheduleEvent(EVENT_CANNON_BARRAGE_2, 1000);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }

        void DamageTaken(Unit* who, uint32 &damage)
        {
            if (me->HealthBelowPct(25) && !lowHp)
            {
                lowHp = true;
                if (Creature* turret = instance->instance->GetCreature(instance->GetGuidData(NPC_BLACKHAND_TURRET)))
                {
                    turret->RemoveAllAuras();
                    turret->SetReactState(REACT_PASSIVE);
                    turret->AttackStop();
                    turret->AI()->Reset();
                }
                
                if (Creature* zaggosh = instance->instance->GetCreature(instance->GetGuidData(NPC_ZOGGOSH)))
                {
                    /* if (Creature* koramar = instance->instance->GetCreature(instance->GetGuidData(NPC_KORAMAR)))
                        if (koramar->isAlive())
                        {
                            koramar->AI()->Talk(0);
                            zaggosh->AI()->Talk(0);
                        }
                        else
                            zaggosh->AI()->Talk(1); */

                    zaggosh->SetReactState(REACT_AGGRESSIVE);
                }
            }
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (target->GetTypeId() == TYPEID_PLAYER)
            {
                if (spell->Id == SPELL_CHECK_FOR_PLAYERS_HIT)
                {
                    events.CancelEvent(EVENT_CANNON_BARRAGE_2);
                    draftMin = true;
                    DoCast(SPELL_CANCEL_CANNON_BARRAGE);
                    DoResetThreat();
                    me->SetReactState(REACT_AGGRESSIVE);
                    if (Creature* koramar = instance->instance->GetCreature(instance->GetGuidData(NPC_KORAMAR)))
                        koramar->AI()->DoAction(ACTION_KORMAR_STOP_BLADESTORM);
                    events.ScheduleEvent(EVENT_GRONN_SMASH, 60000);
                }
            }
            if (target->GetEntry() == NPC_BACKDRAFT)
                if (spell->Id == SPELL_BACKDRAFT)
                {
                    if (draftMin)
                    {
                        draftMin = false;
                        target->CastSpell(target, SPELL_BACKDRAFT_DMG_MIN);
                    }
                    else
                        target->CastSpell(target, SPELL_BACKDRAFT_DMG_MAX);
                }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_GRONN_SMASH:
                        DoCast(SPELL_GRONN_SMASH);
                        DoStopAttack();
                        if (Creature* koramar = instance->instance->GetCreature(instance->GetGuidData(NPC_KORAMAR)))
                            koramar->AI()->DoAction(ACTION_KORMAR_START_BLADESTORM);
                        events.ScheduleEvent(EVENT_CANNON_BARRAGE_1, 3000);
                        break;
                    case EVENT_CANNON_BARRAGE_1:
                        me->GetMotionMaster()->MoveTargetedHome();
                        break;
                    case EVENT_CANNON_BARRAGE_2:
                        DoCast(SPELL_CANNON_BARRAGE);
                        DoCast(me, SPELL_BACKDRAFT, true);
                        events.ScheduleEvent(EVENT_CANNON_BARRAGE_2, 3000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_skullocAI (creature);
    }
};

class boss_koramar : public CreatureScript
{
public:
    boss_koramar() : CreatureScript("boss_koramar") { }

    struct boss_koramarAI : public boss_encounter_skulloc
    {
        boss_koramarAI(Creature* creature) : boss_encounter_skulloc(creature) {}

        bool patch;

        void Reset()
        {
            boss_encounter_skulloc::Reset();
            patch = false;
            me->GetMotionMaster()->Clear(false);
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void EnterCombat(Unit* who)
        {
            boss_encounter_skulloc::EnterCombat(who);
            events.ScheduleEvent(EVENT_SHATTERING_BLADE, 4000); // 26:16
            events.ScheduleEvent(EVENT_BERSERKER_LEAP, 10000);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->GetMotionMaster()->Clear(false);
            boss_encounter_skulloc::JustDied(NULL);
        }

        void DoAction(const int32 action)
        {
            if (action == ACTION_KORMAR_START_BLADESTORM && !patch)
            {
                patch = true;
                DoStopAttack();
                DoCast(SPELL_BLADESTORM);
                me->GetMotionMaster()->MovePath((me->GetEntry() * 100), true);
            }
            if (action == ACTION_KORMAR_STOP_BLADESTORM && patch)
            {
                me->GetMotionMaster()->Clear(false);
                me->RemoveAurasDueToSpell(SPELL_BLADESTORM);
                me->SetReactState(REACT_AGGRESSIVE);
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SHATTERING_BLADE:
                        DoCast(SPELL_SHATTERING_BLADE);
                        events.ScheduleEvent(EVENT_SHATTERING_BLADE, 4000);
                        break;
                    case EVENT_BERSERKER_LEAP:
                        DoCast(SPELL_BERSERKER_LEAP);
                        events.ScheduleEvent(EVENT_BERSERKER_LEAP, 12000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_koramarAI (creature);
    }
};

class boss_zoggosh : public CreatureScript
{
public:
    boss_zoggosh() : CreatureScript("boss_zoggosh") { }

    struct boss_zoggoshAI : public boss_encounter_skulloc
    {
        boss_zoggoshAI(Creature* creature) : boss_encounter_skulloc(creature) {}

        void Reset()
        {
            boss_encounter_skulloc::Reset();
            me->SetReactState(REACT_PASSIVE);
        }

        void EnterCombat(Unit* who)
        {
            boss_encounter_skulloc::EnterCombat(who);

            if (!me->HasAura(SPELL_RIDE_VEHICLE))
                if (Creature* turret = instance->instance->GetCreature(instance->GetGuidData(NPC_BLACKHAND_TURRET)))
                    DoCast(turret, SPELL_RIDE_VEHICLE);
        }

        void JustDied(Unit* /*killer*/)
        {
            boss_encounter_skulloc::JustDied(NULL);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_zoggoshAI (creature);
    }
};

class npc_blackhand_might_turret : public CreatureScript
{
public:
    npc_blackhand_might_turret() : CreatureScript("npc_blackhand_might_turret") { }

    struct npc_blackhand_might_turretAI : public Scripted_NoMovementAI
    {
        npc_blackhand_might_turretAI(Creature* creature) : Scripted_NoMovementAI(creature) 
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;

        void Reset() 
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_RAPID_FIRE, 0);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_RAPID_FIRE_TARGETING)
                DoCast(SPELL_RAPID_FIRE);
        }

        void UpdateAI(uint32 diff) 
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_RAPID_FIRE:
                        DoResetThreat();
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                        {
                            pTarget->AddThreat(me, 10000.0f);
                            DoCast(pTarget, SPELL_RAPID_FIRE_TARGETING);
                        }
                        events.ScheduleEvent(EVENT_RAPID_FIRE, 10000);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_blackhand_might_turretAI (creature);
    }
};

class at_cannon_barrage_loss : public AreaTriggerScript
{
public:
    at_cannon_barrage_loss() : AreaTriggerScript("at_cannon_barrage_loss") { }

    bool OnTrigger(Player* pPlayer, const AreaTriggerEntry* /*pAt*/, bool enter)
    {
        if (enter)
            pPlayer->CastSpell(pPlayer, SPELL_CANNON_BARRAGE_LOSS);
        else
            pPlayer->RemoveAurasDueToSpell(SPELL_CANNON_BARRAGE_LOSS);

        return true;
    }
};

void AddSC_boss_skulloc()
{
    new boss_skulloc();
    new boss_koramar();
    new boss_zoggosh();
    new npc_blackhand_might_turret();
    new at_cannon_barrage_loss();
}
