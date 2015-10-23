/*
    http://epicwow.com/
    Dungeon : Skyreach 97 - 99
    Encounter: Rukhran
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "skyreach.h"

enum Spells
{
    SPELL_INVISIBILITY_DETECTION    = 141048,
    //Intro
    SPELL_MATURE_KALIRI             = 154180,
    SPELL_EMPOWER_WARBIRD_START     = 154181,
    SPELL_EMPOWER_WARBIRD_END       = 154182,
    //Battle
    SPELL_ENERGIZE                  = 178844,
    SPELL_SCREECH                   = 153896,
    SPELL_PIERCE_ARMOR              = 153794,
    SPELL_SUMMON_SOLAR_FLARE        = 153810,
    SPELL_SOLAR_FLARE_UPD_ENTRY     = 153827,
    SPELL_QUILLS                    = 159382,
    //Adds
    SPELL_DORMANT                   = 160641,
    SPELL_FIXATE                    = 176544,
    SPELL_SUNSTRIKE                 = 153828,
    SPELL_SUNBURST                  = 169810,
    SPELL_HEAL                      = 160650,
};

enum eEvents
{
    EVENT_PIERCE_ARMOR    = 1,
    EVENT_SUMMON_FLARE    = 2,
    EVENT_QUILLS          = 3,
    EVENT_CHECK_MELEE     = 4,
};

enum Display
{
    FLARE_DISPLAY   = 54030,
    ASH_DISPLAY     = 55141,
};

Position const addsPos[5] =
{
    {917.58f, 1901.29f, 213.95f, 2.36f}, //Skyreach Raven Whisperer
    {923.31f, 1903.61f, 214.86f, 1.22f}, //Dread Raven Hatchling
    {925.07f, 1903.30f, 214.86f, 4.88f},
    {914.19f, 1908.58f, 215.41f, 0.68f},
    {916.91f, 1894.43f, 214.86f, 5.53f},
};

class boss_rukhran : public CreatureScript
{
public:
    boss_rukhran() : CreatureScript("boss_rukhran") { }

    struct boss_rukhranAI : public BossAI
    {
        boss_rukhranAI(Creature* creature) : BossAI(creature, DATA_RUKHRAN) 
        {
            firstpull = true;
        }

        bool firstpull;
        uint8 summonDiedCount;

        void Init()
        {
            firstpull = false;
            SetCombatMovement(false);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            me->SetReactState(REACT_PASSIVE);

            DoCast(SPELL_INVISIBILITY_DETECTION);
            DoCast(SPELL_MATURE_KALIRI);

            me->SummonCreature(NPC_SKYREACH_RAVEN_WHISPERER, addsPos[0]);

            for (uint8 i = 1; i < 5; i++)
                me->SummonCreature(NPC_DREAD_RAVEN_HATCHLING, addsPos[i]);

            summonDiedCount = 0;
        }

        void Reset()
        {
            events.Reset();
            _Reset();

            if (firstpull)
                Init();

            me->SetPower(POWER_ENERGY, 0);
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();

            DoCast(SPELL_ENERGIZE);

            events.ScheduleEvent(EVENT_CHECK_MELEE, 2000);
            events.ScheduleEvent(EVENT_PIERCE_ARMOR, 10000); //39:21
            events.ScheduleEvent(EVENT_SUMMON_FLARE, 14000);
            events.ScheduleEvent(EVENT_QUILLS, 24000);
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            _JustDied();
        }

        void JustReachedHome()
        {
            me->RemoveAurasDueToSpell(SPELL_MATURE_KALIRI);
            me->SetReactState(REACT_AGGRESSIVE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_PREVENT_SELECT_NPC);
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            if (summon->GetEntry() == NPC_SKYREACH_RAVEN_WHISPERER || summon->GetEntry() == NPC_DREAD_RAVEN_HATCHLING)
            {
                summonDiedCount++;

                if (summonDiedCount == 5)
                {
                    summonDiedCount = 0;
                    DoCast(SPELL_EMPOWER_WARBIRD_START);
                }
            }
        }

        void SpellHit(Unit* caster, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_EMPOWER_WARBIRD_END)
                me->GetMotionMaster()->MoveTargetedHome();
        }

        void DamageTaken(Unit* who, uint32& damage)
        {
            //Hack. Not work Proc flag on creature
            if (me->HasAura(SPELL_SCREECH) && me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                if (me->GetDistance(who) <= 1.5f)
                    me->InterruptSpell(CURRENT_CHANNELED_SPELL);
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
                    case EVENT_CHECK_MELEE:
                    {
                        if (me->getVictim() && !me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                            if (me->GetDistance(me->getVictim()) >= 1.5f)
                                DoCast(SPELL_SCREECH);
                        events.ScheduleEvent(EVENT_CHECK_MELEE, 2000);
                        break;
                    }
                    case EVENT_PIERCE_ARMOR:
                        if (Unit* target = me->getVictim())
                            DoCast(target, SPELL_PIERCE_ARMOR);
                        events.ScheduleEvent(EVENT_PIERCE_ARMOR, 6000);
                        break;
                    case EVENT_SUMMON_FLARE:
                    {
                        std::list<Creature*> pileList;
                        me->GetCreatureListWithEntryInGrid(pileList, NPC_PILE_OF_ASH, 60.0f);
                        if (!pileList.empty())
                        {
                            Trinity::Containers::RandomResizeList(pileList, 1);
                            if (Creature* pTarget = pileList.front())
                            {
                                if (!me->IsWithinLOSInMap(pTarget))
                                {
                                    events.ScheduleEvent(EVENT_SUMMON_FLARE, 500);
                                    return;
                                }
                                DoCast(pTarget, SPELL_SUMMON_SOLAR_FLARE);
                            }
                        }
                        events.ScheduleEvent(EVENT_SUMMON_FLARE, 26000);
                        break;
                    }
                    case EVENT_QUILLS:
                        Talk(0); //Warning Message
                        DoCast(SPELL_QUILLS);
                        events.ScheduleEvent(EVENT_QUILLS, 24000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_rukhranAI (creature);
    }
};

//79505 (morph 76227)
class npc_pile_of_ash : public CreatureScript
{
public:
    npc_pile_of_ash() : CreatureScript("npc_pile_of_ash") { }

    struct npc_pile_of_ashAI : public ScriptedAI
    {
        npc_pile_of_ashAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISTRACT, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
        }

        EventMap events;
        bool active;

        Unit* plrTarget;

        void Reset() 
        {
            events.Reset();
            active = false;
            me->SetReactState(REACT_PASSIVE);
            DoCast(SPELL_DORMANT);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != FOLLOW_MOTION_TYPE)
                return;

            if (me->GetDistance(plrTarget) < 5.0f && active)
            {
                active = false;
                DoCast(SPELL_SUNSTRIKE);
                Ash();
            }
        }

        void SpellHit(Unit* caster, SpellInfo const* spell)
        {
            switch (spell->Id)
            {
                case SPELL_SOLAR_FLARE_UPD_ENTRY:
                case SPELL_SUNSTRIKE:
                case SPELL_SUNBURST:
                    active = true;
                    Flare();
                    break;
            }
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_FIXATE)
            {
                plrTarget = target;

                if (plrTarget)
                    me->GetMotionMaster()->MoveFollow(plrTarget, 1.0f, 0.0f);
            }
        }

        void DamageTaken(Unit* who, uint32& damage)
        {
            if (damage >= me->GetHealth())
            {
                damage = 0;

                if (active)
                {
                    active = false;
                    DoCast(SPELL_SUNBURST);
                    Ash();
                }
            }
        }

        void Ash()
        {
            me->StopMoving();
            me->GetMotionMaster()->Clear();
            DoCast(me, SPELL_DORMANT, true);
            me->UpdateEntry(NPC_PILE_OF_ASH);
            me->SetReactState(REACT_PASSIVE);
            me->SetDisplayId(ASH_DISPLAY);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
        }

        void Flare()
        {
            me->RemoveAurasDueToSpell(SPELL_DORMANT);
            me->UpdateEntry(NPC_SOLAR_FLARE);
            me->SetReactState(REACT_PASSIVE);
            me->SetDisplayId(FLARE_DISPLAY);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            plrTarget = NULL;
            events.ScheduleEvent(EVENT_1, 2000);
        }
        
        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_FIXATE);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_pile_of_ashAI (creature);
    }
};

//176544
class spell_rukhran_ash_fixate : public SpellScriptLoader
{
public:
    spell_rukhran_ash_fixate() : SpellScriptLoader("spell_rukhran_ash_fixate") { }

    class spell_rukhran_ash_fixate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rukhran_ash_fixate_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (targets.size() > 1)
                if (Unit* summoner = GetCaster()->ToTempSummon()->GetSummoner())
                    if (summoner->getVictim())
                        targets.remove(summoner->getVictim());
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rukhran_ash_fixate_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rukhran_ash_fixate_SpellScript();
    }
};

//159381
class spell_rukhran_quills : public SpellScriptLoader
{
    public:
        spell_rukhran_quills() : SpellScriptLoader("spell_rukhran_quills") { }

        class spell_rukhran_quills_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rukhran_quills_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                    {
                        caster->CastSpell(target, 156742, true);
                        caster->CastSpell(target, 160149, true);
                    }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rukhran_quills_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rukhran_quills_SpellScript();
        }
};

void AddSC_boss_rukhran()
{
    new boss_rukhran();
    new npc_pile_of_ash();
    new spell_rukhran_ash_fixate();
    new spell_rukhran_quills();
}
