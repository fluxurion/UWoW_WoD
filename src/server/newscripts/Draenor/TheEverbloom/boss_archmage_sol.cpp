/*
    http://epicwow.com/
    Dungeon : The Everbloom 100
    Encounter: Archmage Sol
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_everbloom.h"

enum Says
{
    SAY_INTRO               = 0,
    SAY_AGGRO               = 1,
    SAY_FIREBLOOM           = 3,
    SAY_WARN_PHASE_FROST    = 4,
    SAY_PHASE_FROST         = 5,
    SAY_WARN_PHASE_ARCANE   = 6,
    SAY_PHASE_ARCANE        = 7,
    SAY_DEATH               = 8,
};

enum Spells
{
    SPELL_BARRIER             = 170150,
    SPELL_THOUGHT_CHRYSALIS   = 166474,
    SPELL_LIVING_ORGANISM     = 166719,
    SPELL_FIRE_AURA           = 166475,
    SPELL_FROST_AURA          = 166476,
    SPELL_ARCANE_AURA         = 166477,
    //Phase Fire
    SPELL_FIREBALL            = 166464,
    SPELL_FIREBLOOM           = 166572,
    SPELL_FIREBLOOM_AT        = 166560,
    //Phase Frost
    SPELL_FROSTBALL           = 166465,
    SPELL_FROZEN_RAIN         = 166723,
    SPELL_FROZEN_RAIN_AT      = 166913,
    //Phase Arcane
    SPELL_ARCANE_BURST        = 166466,
    SPELL_ARCANE_BLOSSOM      = 166751, //??

    SPELL_PARASITIC_GROWTH    = 168885,
    SPELL_PARASITIC_INJECTION = 166891,
};

enum eEvents
{
    EVENT_FIREBALL          = 1,
    EVENT_FIREBLOOM         = 2,
    EVENT_PARASITIC_GROWTH  = 3,
    EVENT_FROSTBALL         = 4,
    EVENT_FROZEN_RAIN       = 5,
    EVENT_ARCANE_BURST      = 6,
};

enum Phases
{
    PHASE_FIRE      = 1,
    PHASE_FROST     = 2,
    PHASE_ARCANE    = 3,
};

Position const introPos[3] =
{
    {611.17f, 1542.52f, 115.49f, 2.63f},
    {641.26f, 1557.85f, 115.49f, 0.42f},
    {638.05f, 1557.63f, 115.88f, 2.99f},
};

class boss_archmage_sol : public CreatureScript
{
public:
    boss_archmage_sol() : CreatureScript("boss_archmage_sol") { }

    struct boss_archmage_solAI : public BossAI
    {
        boss_archmage_solAI(Creature* creature) : BossAI(creature, DATA_ARCHMAGE_SOL), summons(me)
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            DoCast(SPELL_BARRIER);
            for (uint8 i = 0; i < 3; i++)
                me->SummonCreature(NPC_PUTRID_PYROMANCER, introPos[i]);
            
            introSum = 0;
        }

        SummonList summons;
        uint8 introSum;

        void Reset()
        {
            events.Reset();
            _Reset();

            summons.DespawnEntry(NPC_FROZEN_RAIN);
            me->RemoveAreaObject(SPELL_FIREBLOOM_AT);
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);
            _EnterCombat();

            DoCast(me, SPELL_THOUGHT_CHRYSALIS, true);
            DoCast(me, SPELL_LIVING_ORGANISM, true);

            events.SetPhase(PHASE_FIRE);
            events.ScheduleEvent(EVENT_FIREBALL, 0, 0, PHASE_FIRE); //43:24
            events.ScheduleEvent(EVENT_FIREBLOOM, 10000, 0, PHASE_FIRE);
            events.ScheduleEvent(EVENT_PARASITIC_GROWTH, 30000);
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            if (summon->GetEntry() == NPC_FROZEN_RAIN)
            {
                summon->SetReactState(REACT_PASSIVE);
                summon->CastSpell(summon, SPELL_FROZEN_RAIN);
            }
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            if (summon->GetEntry() == NPC_PUTRID_PYROMANCER)
                introSum++;
            
            if (introSum == 3)
            {
                Talk(SAY_INTRO);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                me->SetReactState(REACT_DEFENSIVE);
                me->RemoveAurasDueToSpell(SPELL_BARRIER);
            }
        }

        void OnInterruptCast(Unit* /*caster*/, uint32 /*spellId*/, uint32 curSpellID, uint32 /*schoolMask*/)
        {
            if (curSpellID != SPELL_PARASITIC_GROWTH)
                return;

            if (events.IsInPhase(PHASE_FIRE))
            {
                Talk(SAY_WARN_PHASE_FROST);
                Talk(SAY_PHASE_FROST);
                events.SetPhase(PHASE_FROST);
                me->RemoveAurasDueToSpell(SPELL_FIRE_AURA);
                DoCast(me, SPELL_FROST_AURA, true); //43:56
                DoCast(me, SPELL_THOUGHT_CHRYSALIS, true);
                events.ScheduleEvent(EVENT_FROSTBALL, 0, 0, PHASE_FROST);
                events.ScheduleEvent(EVENT_FROZEN_RAIN, 8000, 0, PHASE_FROST); //44:04
            }
            else if (events.IsInPhase(PHASE_FROST))
            {
                Talk(SAY_WARN_PHASE_ARCANE);
                Talk(SAY_PHASE_ARCANE);
                events.SetPhase(PHASE_ARCANE);
                me->RemoveAurasDueToSpell(SPELL_FROST_AURA);
                DoCast(me, SPELL_ARCANE_AURA, true);
                DoCast(me, SPELL_THOUGHT_CHRYSALIS, true);
                summons.DespawnEntry(NPC_FROZEN_RAIN);
                events.CancelEvent(EVENT_PARASITIC_GROWTH);
                events.ScheduleEvent(EVENT_ARCANE_BURST, 1000, 0, PHASE_ARCANE);
            }
            DoCast(me, SPELL_PARASITIC_INJECTION, true);
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
                    case EVENT_FIREBALL:
                        DoCast(SPELL_FIREBALL);
                        events.ScheduleEvent(EVENT_FIREBALL, 3100, 0, PHASE_FIRE);
                        break;
                    case EVENT_FIREBLOOM:
                        DoCast(SPELL_FIREBLOOM);
                        events.ScheduleEvent(EVENT_FIREBLOOM, 12000, 0, PHASE_FIRE);
                        break;
                    case EVENT_PARASITIC_GROWTH:
                        DoCast(SPELL_PARASITIC_GROWTH);
                        events.ScheduleEvent(EVENT_PARASITIC_GROWTH, 30000);
                        break;
                    case EVENT_FROSTBALL:
                        DoCast(SPELL_FROSTBALL);
                        events.ScheduleEvent(EVENT_FROSTBALL, 3100, 0, PHASE_FROST);
                        break;
                    case EVENT_FROZEN_RAIN:
                    {
                        float range = float(urand(10, 30));
                        float ang = float(urand(0, 5));
                        Position pos;
                        me->GetNearPosition(pos, range, ang);
                        me->SummonCreature(NPC_FROZEN_RAIN, pos);
                        events.ScheduleEvent(EVENT_FROZEN_RAIN, 10000, 0, PHASE_FROST);
                        break;
                    }
                    case EVENT_ARCANE_BURST:
                        DoCast(SPELL_ARCANE_BURST);
                        events.ScheduleEvent(EVENT_ARCANE_BURST, 3000, 0, PHASE_ARCANE);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_archmage_solAI (creature);
    }
};

//166723
class spell_sol_frozen_rain : public SpellScriptLoader
{
    public:
        spell_sol_frozen_rain() : SpellScriptLoader("spell_sol_frozen_rain") { }

        class spell_sol_frozen_rain_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sol_frozen_rain_SpellScript);

            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                caster->CastSpell(caster, SPELL_FROZEN_RAIN_AT);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sol_frozen_rain_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sol_frozen_rain_SpellScript();
        }
};

void AddSC_boss_archmage_sol()
{
    new boss_archmage_sol();
    new spell_sol_frozen_rain();
}
