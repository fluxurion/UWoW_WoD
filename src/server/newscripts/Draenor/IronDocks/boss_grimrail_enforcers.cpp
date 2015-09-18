/*
    http://epicwow.com/
    Dungeon : Iron Docks / Железные доки 93-95
    Encounter: Grimrail Enforcers / Головорезы Мрачных Путей
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "iron_docks.h"

enum Says
{
    //Makogg
    SAY_MAKOGG_INTRO       = 0,
    SAY_MAKOGG_F_SLASH     = 1,
    SAY_MAKOGG_DEATH       = 2,
    //Neesa
    SAY_NEESA_INTRO        = 0,
    SAY_NEESA_BOMBSQUAD    = 1,
    SAY_NEESA_DEATH        = 2,
    //Dugru
    SAY_DUGRU_INTRO        = 0,
    SAY_DUGRU_BLOOD_SURGES = 1,
    SAY_DUGRU_WARN_SPHERE  = 2,
    SAY_DUGRU_DEATH        = 3,
};

enum Spells
{
    //Makogg
    SPELL_FLAMING_SLASH      = 163665,
    SPELL_LAVA_SWEEP         = 164956,
    //Neesa
    SPELL_GUTSHOT            = 163334,
    SPELL_OGRE_TRAPS         = 163390,
    SPELL_OGRE_TRAPS_1       = 163275,
    SPELL_OGRE_TRAPS_2       = 163278,
    SPELL_OGRE_TRAPS_3       = 163279,
    SPELL_OGRE_TRAPS_4       = 163303,
    SPELL_OGRE_TRAPS_5       = 163304,
    SPELL_OGRE_TRAPS_6       = 163305,
    SPELL_OGRE_TRAPS_7       = 163306,
    SPELL_OGRE_TRAPS_8       = 163307,
    SPELL_OGRE_TRAP_VISUAL_1 = 177391,
    SPELL_OGRE_TRAP_AT       = 177392,
    SPELL_OGRE_TRAP_VISUAL_2 = 177396,
    SPELL_SHREDDED_TENDONS   = 163276,
    SPELL_JUMPER_CABLES      = 163376,
    SPELL_SUMMON_BOMBSQUAD   = 163362,
    SPELL_BOMB_CHARGE        = 163635,
    //Dugru
    SPELL_BLOOD_BOLT         = 165122,
    SPELL_TAINTED_BLOOD      = 163740,
    SPELL_SANGUINE_SPHERE    = 163689,
    SPELL_ABRUPT_RESTORATION = 163705,
};

enum eEvents
{
    //Makogg
    EVENT_FLAMING_SLASH    = 1,
    EVENT_LAVA_SWEEP       = 2,
    //Neesa
    EVENT_GUTSHOT          = 1,
    EVENT_OGRE_TRAPS       = 2,
    EVENT_JUMPER_CABLES    = 3,
    EVENT_SUMMON_BOMBSQUAD = 4,
    EVENT_SWITCH_TARGET    = 5,
    //Dugru
    EVENT_BLOOD_BOLT       = 1,
    EVENT_TAINTED_BLOOD    = 2,
    EVENT_SANGUINE_SPHERE  = 3,
};

uint32 const enforcers[3] = { NPC_MAKOGG_EMBERBLADE, NPC_NEESA_NOX, NPC_AHRIOK_DUGRU };

struct boss_grimrail_enforcers : public BossAI
{
    boss_grimrail_enforcers(Creature* creature) : BossAI(creature, DATA_G_ENFORCERS) {}

    void Reset()
    {
        _Reset();
        instance->SetData(DATA_G_ENFOR_DIED, 0);
    }

    void EnterCombat(Unit* who)
    {
        instance->SetBossState(DATA_G_ENFORCERS, IN_PROGRESS);

        for (int32 i = 0; i < 3; ++i)
        {
            if (me->GetEntry() == enforcers[i])
                continue;

            if (Creature* enfor = ObjectAccessor::GetCreature(*me, instance->GetGuidData(enforcers[i])))
                DoZoneInCombat(enfor, 150.0f);
        }
    }

    void JustDied(Unit* /*killer*/)
    {
        events.Reset();
        uint32 enforCount = instance->GetData(DATA_G_ENFOR_DIED) + 1;
        instance->SetData(DATA_G_ENFOR_DIED, enforCount);
    }
};

class boss_makogg_emberblade : public CreatureScript
{
public:
    boss_makogg_emberblade() : CreatureScript("boss_makogg_emberblade") { }

    struct boss_makogg_emberbladeAI : public boss_grimrail_enforcers
    {
        boss_makogg_emberbladeAI(Creature* creature) : boss_grimrail_enforcers(creature) {}

        void Reset()
        {
            boss_grimrail_enforcers::Reset();
        }

        void EnterCombat(Unit* who)
        {
            boss_grimrail_enforcers::EnterCombat(who);
            
            events.ScheduleEvent(EVENT_FLAMING_SLASH, 4000);
            events.ScheduleEvent(EVENT_LAVA_SWEEP, 14000);
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_MAKOGG_DEATH);
            boss_grimrail_enforcers::JustDied(NULL);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->GetDistance(me->GetHomePosition()) >= 90.0f)
            {
                EnterEvadeMode();
                return;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FLAMING_SLASH:
                        Talk(SAY_MAKOGG_F_SLASH);
                        Position pos;
                        me->GetNearPosition(pos, 20.0f, 0.0f);
                        me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_FLAMING_SLASH);
                        events.ScheduleEvent(EVENT_FLAMING_SLASH, 28000);
                        break;
                    case EVENT_LAVA_SWEEP:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                            DoCast(pTarget, SPELL_LAVA_SWEEP);
                        events.ScheduleEvent(EVENT_LAVA_SWEEP, 28000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_makogg_emberbladeAI (creature);
    }
};

class boss_neesa_nox : public CreatureScript
{
public:
    boss_neesa_nox() : CreatureScript("boss_neesa_nox") { }

    struct boss_neesa_noxAI : public boss_grimrail_enforcers
    {
        boss_neesa_noxAI(Creature* creature) : boss_grimrail_enforcers(creature), summons(me) {}

        SummonList summons;

        void Reset()
        {
            boss_grimrail_enforcers::Reset();
            summons.DespawnAll();
        }

        void EnterCombat(Unit* who)
        {
            boss_grimrail_enforcers::EnterCombat(who);

            events.ScheduleEvent(EVENT_GUTSHOT, 0);
            events.ScheduleEvent(EVENT_OGRE_TRAPS, 12000);
            events.ScheduleEvent(EVENT_JUMPER_CABLES, 28000);
            events.ScheduleEvent(EVENT_SUMMON_BOMBSQUAD, 0);
            events.ScheduleEvent(EVENT_SWITCH_TARGET, 10000);
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_NEESA_DEATH);
            boss_grimrail_enforcers::JustDied(NULL);
            summons.DespawnAll();
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            if (summon->GetEntry() == NPC_BOMBSQUAD)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80, true))
                {
                    summon->AddThreat(target, 10000.0f);
                    summon->CastSpell(target, SPELL_BOMB_CHARGE);
                }
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->GetDistance(me->GetHomePosition()) >= 90.0f)
            {
                EnterEvadeMode();
                return;
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SWITCH_TARGET:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 90, true))
                        {
                            DoResetThreat();
                            me->AddThreat(pTarget, 100000.0f);
                        }
                        events.ScheduleEvent(EVENT_SWITCH_TARGET, 10000);
                        break;
                    case EVENT_GUTSHOT:
                        if (Unit* pTarget = me->getVictim())
                            DoCast(pTarget, SPELL_GUTSHOT);
                        events.ScheduleEvent(EVENT_GUTSHOT, 3000);
                        break;
                    case EVENT_OGRE_TRAPS:
                        DoCast(SPELL_OGRE_TRAPS);
                        events.ScheduleEvent(EVENT_OGRE_TRAPS, 30000);
                        break;
                    case EVENT_JUMPER_CABLES:
                        if (Unit* pTarget = me->getVictim())
                            DoCast(pTarget, SPELL_JUMPER_CABLES);
                        events.ScheduleEvent(EVENT_JUMPER_CABLES, 28000);
                        break;
                    case EVENT_SUMMON_BOMBSQUAD:
                        Talk(SAY_NEESA_BOMBSQUAD);
                        DoCast(SPELL_SUMMON_BOMBSQUAD);
                        events.ScheduleEvent(EVENT_SUMMON_BOMBSQUAD, 14000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_neesa_noxAI (creature);
    }
};

class boss_ahriok_dugru : public CreatureScript
{
public:
    boss_ahriok_dugru() : CreatureScript("boss_ahriok_dugru") { }

    struct boss_ahriok_dugruAI : public boss_grimrail_enforcers
    {
        boss_ahriok_dugruAI(Creature* creature) : boss_grimrail_enforcers(creature) {}

        void Reset()
        {
            boss_grimrail_enforcers::Reset();
        }

        void EnterCombat(Unit* who)
        {
            boss_grimrail_enforcers::EnterCombat(who);

            events.ScheduleEvent(EVENT_BLOOD_BOLT, 0);
            events.ScheduleEvent(EVENT_TAINTED_BLOOD, 20000);
            events.ScheduleEvent(EVENT_SANGUINE_SPHERE, 30000);
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DUGRU_DEATH);
            me->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            boss_grimrail_enforcers::JustDied(NULL);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->GetDistance(me->GetHomePosition()) >= 90.0f)
            {
                EnterEvadeMode();
                return;
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_BLOOD_BOLT:
                        if (Unit* pTarget = me->getVictim())
                            DoCast(pTarget, SPELL_BLOOD_BOLT);
                        events.ScheduleEvent(EVENT_BLOOD_BOLT, 4000);
                        break;
                    case EVENT_TAINTED_BLOOD:
                        if (Unit* pTarget = me->getVictim())
                            DoCast(pTarget, SPELL_TAINTED_BLOOD);
                        events.ScheduleEvent(EVENT_TAINTED_BLOOD, 28000);
                        break;
                    case EVENT_SANGUINE_SPHERE:
                        Talk(SAY_DUGRU_WARN_SPHERE);
                        DoCast(SPELL_SANGUINE_SPHERE);
                        events.ScheduleEvent(EVENT_SANGUINE_SPHERE, 60000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ahriok_dugruAI (creature);
    }
};

class npc_ogre_trap : public CreatureScript
{
public:
    npc_ogre_trap() : CreatureScript("npc_ogre_trap") { }

    struct npc_ogre_trapAI : public Scripted_NoMovementAI
    {
        npc_ogre_trapAI(Creature* creature) : Scripted_NoMovementAI(creature) 
        {
            DoCast(me, SPELL_OGRE_TRAP_VISUAL_1, true);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
            activate = false;
        }

        bool activate;

        void MoveInLineOfSight(Unit* pTarget)
        {
            if (pTarget->GetTypeId() != TYPEID_PLAYER)
                return;

            if (me->GetDistance(pTarget) < 1.5f && !activate)
            {
                activate = true;
                me->RemoveAurasDueToSpell(SPELL_OGRE_TRAP_VISUAL_1);
                DoCast(me, SPELL_OGRE_TRAP_VISUAL_2, true);
                pTarget->CastSpell(pTarget, SPELL_SHREDDED_TENDONS, true);
                me->DespawnOrUnsummon(3000);
            }
        }

        void Reset() {}

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ogre_trapAI (creature);
    }
};

// 163390
uint32 spellTrap[8] = 
{   SPELL_OGRE_TRAPS_1, SPELL_OGRE_TRAPS_2, 
    SPELL_OGRE_TRAPS_3, SPELL_OGRE_TRAPS_4, 
    SPELL_OGRE_TRAPS_5, SPELL_OGRE_TRAPS_6, 
    SPELL_OGRE_TRAPS_7, SPELL_OGRE_TRAPS_8
};
class spell_neesa_ogre_traps : public SpellScriptLoader
{
    public:
        spell_neesa_ogre_traps() : SpellScriptLoader("spell_neesa_ogre_traps") { }

        class spell_neesa_ogre_traps_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_neesa_ogre_traps_AuraScript);

            uint8 Idx;

            bool Load()
            {
                Idx = 0;
                return true;
            }

            void OnPeriodic(AuraEffect const* aurEff)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                Unit* pTarget = GetCaster()->getVictim();
                if (!pTarget)
                    return;

                caster->CastSpell(pTarget, spellTrap[Idx], true);
                ++Idx;
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_neesa_ogre_traps_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_neesa_ogre_traps_AuraScript();
        }
};

//163689
class spell_sanguine_sphere : public SpellScriptLoader
{
    public:
        spell_sanguine_sphere() : SpellScriptLoader("spell_sanguine_sphere") { }

        class spell_sanguine_sphere_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sanguine_sphere_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.sort(CheckHealthState());

                if (targets.size() > 1)
                    targets.resize(1);

                if (targets.empty())
                    targets.push_front(GetCaster());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sanguine_sphere_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        class spell_sanguine_sphere_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sanguine_sphere_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetTarget())
                    return;

                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_ENEMY_SPELL)
                    GetTarget()->CastSpell(GetTarget(), SPELL_ABRUPT_RESTORATION, true);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_sanguine_sphere_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sanguine_sphere_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_sanguine_sphere_AuraScript();
        }
private:
    class CheckHealthState
    {
        public:
            CheckHealthState() { }

            bool operator() (WorldObject* a, WorldObject* b) const
            {
                Unit* unita = a->ToUnit();
                Unit* unitb = b->ToUnit();
                if(!unita)
                    return true;
                if(!unitb)
                    return false;
                return unita->GetHealthPct() < unitb->GetHealthPct();
            }
    };
};

// Lava Swipe - 164956
class spell_boss_grimrail_lava_swipe1 : public SpellScriptLoader
{
public:
    spell_boss_grimrail_lava_swipe1() : SpellScriptLoader("spell_boss_grimrail_lava_swipe1") { }

    class spell_boss_grimrail_lava_swipe1_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_boss_grimrail_lava_swipe1_SpellScript);

        void HandleBeforeCast()
        {
            if(Unit* caster = GetOriginalCaster())
            {
                Unit* target = GetExplTargetUnit();
                float x, y, o;
                if(target)
                {
                    o = caster->GetAngle(target);
                    caster->CastSpell(target, 164901);
                }
                else
                    o = caster->GetOrientation();
                WorldLocation loc;
                caster->GetNearPoint2D(x, y, 43.55f, o + 0.25f);
                loc.Relocate(x, y, caster->GetPositionZ());
                GetSpell()->destAtTarget = loc;
            }
        }

        void HandleTriggerEffect(SpellEffIndex /*effIndex*/)
        {
            PreventHitDefaultEffect(EFFECT_2);
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(spell_boss_grimrail_lava_swipe1_SpellScript::HandleTriggerEffect, EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL);
            BeforeCast += SpellCastFn(spell_boss_grimrail_lava_swipe1_SpellScript::HandleBeforeCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_boss_grimrail_lava_swipe1_SpellScript();
    }
};

// Lava Swipe - 164901
class spell_boss_grimrail_lava_swipe2 : public SpellScriptLoader
{
public:
    spell_boss_grimrail_lava_swipe2() : SpellScriptLoader("spell_boss_grimrail_lava_swipe2") { }

    class spell_boss_grimrail_lava_swipe2_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_boss_grimrail_lava_swipe2_SpellScript);

        void HandleBeforeCast()
        {
            if(Unit* caster = GetOriginalCaster())
            {
                Unit* target = GetExplTargetUnit();
                float x, y, o;
                if(target)
                    o = caster->GetAngle(target);
                else
                    o = caster->GetOrientation();
                WorldLocation loc;
                caster->GetNearPoint2D(x, y, 43.55f, o - 0.25f);
                loc.Relocate(x, y, caster->GetPositionZ());
                GetSpell()->destAtTarget = loc;
            }
        }

        void Register()
        {
            BeforeCast += SpellCastFn(spell_boss_grimrail_lava_swipe2_SpellScript::HandleBeforeCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_boss_grimrail_lava_swipe2_SpellScript();
    }
};

void AddSC_boss_grimrail_enforcers()
{
    new boss_makogg_emberblade();
    new boss_neesa_nox();
    new boss_ahriok_dugru();
    new npc_ogre_trap();
    new spell_neesa_ogre_traps();
    new spell_sanguine_sphere();
    new spell_boss_grimrail_lava_swipe1();
    new spell_boss_grimrail_lava_swipe2();
}
