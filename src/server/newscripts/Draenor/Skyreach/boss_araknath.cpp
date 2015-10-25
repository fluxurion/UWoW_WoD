/*
    http://epicwow.com/
    Dungeon : Skyreach 97 - 99
    Encounter: Araknath
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "skyreach.h"

enum Says
{
    SAY_DEATH           = 0,
};

enum Spells
{
    //Araknath
    SPELL_SUBMERGED_VISUAL    = 154163,
    SPELL_MELEE               = 154121,
    SPELL_SMASH_1             = 154113,
    SPELL_BURST               = 154135,

    //Arcanologist
    SPELL_SOLAR_DETONATION    = 160288,
    SPELL_SOLAR_STORM         = 159215,

    //Phase Visual
    SPELL_ENERGIZE_SINGLE     = 156382,
    SPELL_ENERGIZE_MASS       = 154177,
    SPELL_ENERGIZE_HEAL       = 154179,

    //Phase Battle
    SPELL_ENERGIZE_FORCE_CAST = 156384,
    SPELL_ENERGIZE_AURA       = 154159,
    SPELL_ENERGIZE_AURA_TICK  = 154139,
    SPELL_ENERGIZE_HEAL_2     = 154149,
    SPELL_ENERGIZE_DMG_PLR    = 154150,
};

enum eEvents
{
    EVENT_MELE_ATTACK    = 1,
    EVENT_SMASH          = 2,
    EVENT_BURST          = 3,
    EVENT_ENERGIZE_BEAM  = 4,
};

class boss_araknath : public CreatureScript
{
public:
    boss_araknath() : CreatureScript("boss_araknath") { }

    struct boss_araknathAI : public BossAI
    {
        boss_araknathAI(Creature* creature) : BossAI(creature, DATA_ARAKNATH), summons(me)
        {
            firstPull = true;
            me->SummonCreature(NPC_SKYREACH_ARCANOLOGIST, 1061.96f, 1800.92f, 200.20f, 2.65f);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            DoCast(me, SPELL_SUBMERGED_VISUAL, true);
        }

        SummonList summons;

        bool firstPull;

        void Reset()
        {
            events.Reset();
            _Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();

            events.ScheduleEvent(EVENT_MELE_ATTACK, 2000); //36:41
            events.ScheduleEvent(EVENT_SMASH, 6000);
            events.ScheduleEvent(EVENT_BURST, 20000);
            events.ScheduleEvent(EVENT_ENERGIZE_BEAM, 18000);
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            _JustDied();
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            if (summon->GetEntry() != NPC_SKYREACH_ARCANOLOGIST)
                summon->SetReactState(REACT_PASSIVE);

            if (!firstPull)
                return;

            switch (summon->GetEntry())
            {
                case NPC_INTERIOR_FOCUS:
                    summon->CastSpell(summon, SPELL_ENERGIZE_SINGLE);
                    break;
                case NPC_SUN_CONSTRUCT_ENERGIZER:
                    summon->CastSpell(summon, SPELL_ENERGIZE_MASS);
                    break;
                case NPC_SKYREACH_SUN_PROTOTYPE:
                    summon->CastSpell(me, SPELL_ENERGIZE_HEAL, true, 0, NULL, me->GetGUID());
                    break;
            }
        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_1:
                {
                    firstPull = false;
                    EntryCheckPredicate pred1(NPC_INTERIOR_FOCUS);
                    EntryCheckPredicate pred2(NPC_SUN_CONSTRUCT_ENERGIZER);
                    summons.DoAction(ACTION_1, pred1);
                    summons.DoAction(ACTION_1, pred2);
                }
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (me->GetDistance(me->GetHomePosition()) > 30.0f)
            {
                EnterEvadeMode();
                return;
            }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MELE_ATTACK:
                        if (Unit* target = me->getVictim())
                            DoCast(target, SPELL_MELEE);
                        events.ScheduleEvent(EVENT_MELE_ATTACK, 2000);
                        break;
                    case EVENT_SMASH:
                        if (Unit* target = me->getVictim())
                            DoCast(target, SPELL_SMASH_1);
                        events.ScheduleEvent(EVENT_SMASH, 8000);
                        break;
                    case EVENT_BURST:
                        DoCast(SPELL_BURST);
                        events.ScheduleEvent(EVENT_BURST, 22000);
                        break;
                    case EVENT_ENERGIZE_BEAM:
                        EntryCheckPredicate pred1(NPC_INTERIOR_FOCUS);
                        summons.DoAction(ACTION_2, pred1);
                        events.ScheduleEvent(EVENT_ENERGIZE_BEAM, 20000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_araknathAI (creature);
    }
};

//76376
class npc_skyreach_arcanologist : public CreatureScript
{
public:
    npc_skyreach_arcanologist() : CreatureScript("npc_skyreach_arcanologist") { }

    struct npc_skyreach_arcanologistAI : public ScriptedAI
    {
        npc_skyreach_arcanologistAI(Creature* creature) : ScriptedAI(creature) 
        {
            instance = me->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;

        ObjectGuid AraknathGUID;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_1, 2000); //36:31
            events.ScheduleEvent(EVENT_2, 8000);
        }

        void JustDied(Unit* killer)
        {
            Talk(SAY_DEATH);

            if (Creature* araknath = Unit::GetCreature(*me, AraknathGUID))
            {
                araknath->AI()->DoAction(ACTION_1);
                araknath->RemoveAllAuras();
                araknath->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                araknath->SetReactState(REACT_AGGRESSIVE);
            }
        }

        void IsSummonedBy(Unit* summoner)
        {
            AraknathGUID = summoner->GetGUID();
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
                    case EVENT_1:
                        DoCast(SPELL_SOLAR_DETONATION);
                        events.ScheduleEvent(EVENT_1, 10000);
                        break;
                    case EVENT_2:
                        DoCast(SPELL_SOLAR_STORM);
                        events.ScheduleEvent(EVENT_2, 12000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_skyreach_arcanologistAI (creature);
    }
};

//77543, 76367
class npc_araknath_energizer : public CreatureScript
{
public:
    npc_araknath_energizer() : CreatureScript("npc_araknath_energizer") { }

    struct npc_araknath_energizerAI : public ScriptedAI
    {
        npc_araknath_energizerAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void Reset() 
        {}

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_1:
                    if (me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                        me->InterruptSpell(CURRENT_CHANNELED_SPELL);
                    break;
                case ACTION_2:
                    DoCast(me, SPELL_ENERGIZE_FORCE_CAST, true);
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_araknath_energizerAI (creature);
    }
};

//76142
class npc_skyreach_sun_prototype : public CreatureScript
{
public:
    npc_skyreach_sun_prototype() : CreatureScript("npc_skyreach_sun_prototype") { }

    struct npc_skyreach_sun_prototypeAI : public ScriptedAI
    {
        npc_skyreach_sun_prototypeAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
            DoCast(SPELL_SUBMERGED_VISUAL);
        }

        EventMap events;

        void Reset() 
        {}

        void SpellHit(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_ENERGIZE_AURA)
            {
                me->RemoveAurasDueToSpell(SPELL_SUBMERGED_VISUAL);
                events.ScheduleEvent(EVENT_1, 3000);
                events.ScheduleEvent(EVENT_2, 15000);
            }
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_ENERGIZE_AURA_TICK);
                        break;
                    case EVENT_2:
                        if (me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                            me->InterruptSpell(CURRENT_CHANNELED_SPELL);
                        DoCast(SPELL_SUBMERGED_VISUAL);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_skyreach_sun_prototypeAI (creature);
    }
};

//154140
class spell_araknath_energize : public SpellScriptLoader
{
    public:
        spell_araknath_energize() : SpellScriptLoader("spell_araknath_energize") { }

        class spell_araknath_energize_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_araknath_energize_SpellScript);

            bool check = false;
 
            void Dummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        check = true;
                        if (!caster->FindCurrentSpellBySpellId(SPELL_ENERGIZE_DMG_PLR))
                            caster->CastSpell(target, SPELL_ENERGIZE_DMG_PLR, true);
                    }
                }
            }
 
            void HandleAfterCast()
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;
 
                if (!check && !caster->FindCurrentSpellBySpellId(SPELL_ENERGIZE_HEAL_2))
                    caster->CastSpell(caster, SPELL_ENERGIZE_HEAL_2, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_araknath_energize_SpellScript::HandleAfterCast);
                OnEffectHitTarget += SpellEffectFn(spell_araknath_energize_SpellScript::Dummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_araknath_energize_SpellScript();
        }
};

void AddSC_boss_araknath()
{
    new boss_araknath();
    new npc_skyreach_arcanologist();
    new npc_araknath_energizer();
    new npc_skyreach_sun_prototype();
    new spell_araknath_energize();
}
