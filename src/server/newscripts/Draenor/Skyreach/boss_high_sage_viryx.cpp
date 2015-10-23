/*
    http://epicwow.com/
    Dungeon : Skyreach 97 - 99
    Encounter: High Sage Viryx
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "skyreach.h"

enum Says
{
    SAY_AGGRO               = 0,
    SAY_SUMM_VEHICLE        = 1,
    SAY_WARN_LENS_FLARE     = 2,
    SAY_SUMM_ZEALOT         = 3,
    SAY_SUMM_DEFENDER       = 4,
    SAY_KILL_PLAYER         = 5,
    SAY_DEATH               = 6,
};

enum Spells
{
    //Viryx
    SPELL_SOLAR_BURST     = 154396,
    SPELL_CAST_DOWN       = 153954,
    SPELL_LENS_FLARE      = 154032,
    SPELL_CALL_ADDS       = 154049,
    SPELL_CAST_DOWN_SUMM  = 153955,
    //Zealot
    SPELL_JUMP_CREATOR    = 165834,
    SPELL_CREATOR_RIDE_ME = 136522,
    SPELL_LENS_FLARE_AT   = 154044,
    //Defender
    SPELL_SHIELDING       = 154055,
};

enum eEvents
{
    EVENT_SOLAR_BURST    = 1,
    EVENT_CAST_DOWN      = 2,
    EVENT_LENS_FLARE     = 3,
    EVENT_CALL_ADDS      = 4,
};

Position const centrPos = {1063.10f, 1799.82f, 262.17f};

Position const linePos[4] =
{
    {1063.10f, 1799.82f, 262.17f}, //Right line
    {1072.62f, 1733.22f, 262.17f},
    {1063.10f, 1799.82f, 262.17f}, //Left line
    {1125.21f, 1820.63f, 262.17f},
};

Position const lineEndPos[2] =
{
    {1088.75f, 1722.93f, 262.17f},
    {1137.63f, 1820.51f, 262.17f},
};

class boss_high_sage_viryx : public CreatureScript
{
public:
    boss_high_sage_viryx() : CreatureScript("boss_high_sage_viryx") { }

    struct boss_high_sage_viryxAI : public BossAI
    {
        boss_high_sage_viryxAI(Creature* creature) : BossAI(creature, DATA_VIRYX) {}

        void Reset()
        {
            events.Reset();
            _Reset();

            me->RemoveAreaObject(SPELL_LENS_FLARE_AT);
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_SOLAR_BURST, 8000); //43:42
            events.ScheduleEvent(EVENT_CAST_DOWN, 16000);
            events.ScheduleEvent(EVENT_LENS_FLARE, 26000);
            events.ScheduleEvent(EVENT_CALL_ADDS, 32000);
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();

            me->RemoveAreaObject(SPELL_LENS_FLARE_AT);
        }

        void KilledUnit(Unit* victim)
        {
            if (victim->GetTypeId() != TYPEID_PLAYER)
                return;

            uint8 rand = urand(0, 1);
            if (rand)
                Talk(SAY_KILL_PLAYER);
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
                    case EVENT_SOLAR_BURST:
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_SOLAR_BURST);
                        events.ScheduleEvent(EVENT_SOLAR_BURST, 12000);
                        break;
                    case EVENT_CAST_DOWN:
                        Talk(SAY_SUMM_ZEALOT);
                        DoCast(SPELL_CAST_DOWN);
                        events.ScheduleEvent(EVENT_CAST_DOWN, 36000);
                        break;
                    case EVENT_LENS_FLARE:
                        Talk(SAY_WARN_LENS_FLARE);
                        DoCast(SPELL_LENS_FLARE);
                        events.ScheduleEvent(EVENT_LENS_FLARE, 36000);
                        break;
                    case EVENT_CALL_ADDS:
                        DoCast(156791); //SPELL_CALL_ADDS
                        events.ScheduleEvent(EVENT_CALL_ADDS, 60000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_high_sage_viryxAI (creature);
    }
};

//76267
class npc_solar_zealot : public CreatureScript
{
public:
    npc_solar_zealot() : CreatureScript("npc_solar_zealot") { }

    struct npc_solar_zealotAI : public ScriptedAI
    {
        npc_solar_zealotAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->setFaction(16);
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

        void Reset() 
        {
            events.Reset();
            me->SetReactState(REACT_PASSIVE);
        }

        void IsSummonedBy(Unit* summoner)
        {
            events.ScheduleEvent(EVENT_1, 1000);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == 1)
            {
                me->GetVehicleKit()->RemoveAllPassengers();
                me->DespawnOrUnsummon(1000);
            }
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_CREATOR_RIDE_ME)
            {
                if (target->GetPositionZ() > 270.0f || target->GetPositionZ() < 255.0f)
                {
                    me->DespawnOrUnsummon();
                    return;
                }
                target->EnterVehicle(me, 0);
                events.ScheduleEvent(EVENT_2, 1000);
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
                        if (Unit* summoner = me->ToTempSummon()->GetSummoner())
                            if (summoner)
                                DoCast(summoner, SPELL_JUMP_CREATOR, true);
                        break;
                    case EVENT_2:
                        float x, y, z;
                        float angle = centrPos.GetAngle(me);
                        centrPos.SimplePosXYRelocationByAngle(x, y, z, 90.0f, angle);
                        Position moveTo {x, y, z};
                        if (IsLinesCross(centrPos, moveTo, linePos[0], linePos[1])) //Right
                            me->GetMotionMaster()->MovePoint(1, lineEndPos[0]);
                        else if (IsLinesCross(centrPos, moveTo, linePos[2], linePos[3])) //Left
                            me->GetMotionMaster()->MovePoint(1, lineEndPos[1]);
                        else
                            me->GetMotionMaster()->MovePoint(1, x, y, me->GetPositionZ());
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_solar_zealotAI (creature);
    }
};

//76083
class npc_viryx_lens_flare : public CreatureScript
{
public:
    npc_viryx_lens_flare() : CreatureScript("npc_viryx_lens_flare") { }

    struct npc_viryx_lens_flareAI : public ScriptedAI
    {
        npc_viryx_lens_flareAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void IsSummonedBy(Unit* summoner)
        {
            me->GetMotionMaster()->MoveFollow(summoner, 1.0f, 0.0f);
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_viryx_lens_flareAI (creature);
    }
};

//76292
class npc_skyreach_shield_construct : public CreatureScript
{
public:
    npc_skyreach_shield_construct() : CreatureScript("npc_skyreach_shield_construct") { }

    struct npc_skyreach_shield_constructAI : public ScriptedAI
    {
        npc_skyreach_shield_constructAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void Reset() 
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_1, 4000);
        }

        void IsSummonedBy(Unit* summoner)
        {
            DoZoneInCombat(me, 100.0f);
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
                        DoCast(SPELL_SHIELDING);
                        events.ScheduleEvent(EVENT_1, 15000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_skyreach_shield_constructAI(creature);
    }
};

//153954
class spell_viryx_cast_down : public SpellScriptLoader
{
public:
    spell_viryx_cast_down() : SpellScriptLoader("spell_viryx_cast_down") { }

    class spell_viryx_cast_down_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_viryx_cast_down_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (targets.size() > 1)
                if (GetCaster()->getVictim())
                    targets.remove(GetCaster()->getVictim());
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_viryx_cast_down_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_viryx_cast_down_SpellScript();
    }
};

//153955
class spell_viryx_cast_down_summ : public SpellScriptLoader
{
    public:
        spell_viryx_cast_down_summ() : SpellScriptLoader("spell_viryx_cast_down_summ") { }

        class spell_viryx_cast_down_summ_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_viryx_cast_down_summ_SpellScript);

            void ModDestHeight(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(EFFECT_0);

                switch (GetSpellInfo()->Id)
                {
                    case SPELL_CAST_DOWN_SUMM:
                    {
                        static Position const offset = {0.0f, 0.0f, 12.0f, 0.0f};
                        GetHitDest()->RelocateOffset(offset);
                        break;
                    }
                    /* case SPELL_DAGGERFALL_DAMAGE_FALL:
                    {
                        static Position const offset = {0.0f, 0.0f, -12.0f, 0.0f};
                        WorldLocation* dest = const_cast<WorldLocation*>(GetExplTargetDest());
                        dest->RelocateOffset(offset);
                        break;
                    } */
                }
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_viryx_cast_down_summ_SpellScript::ModDestHeight, EFFECT_0, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_viryx_cast_down_summ_SpellScript();
        }
};
void AddSC_boss_high_sage_viryx()
{
    new boss_high_sage_viryx();
    new npc_solar_zealot();
    new npc_viryx_lens_flare();
    new npc_skyreach_shield_construct();
    new spell_viryx_cast_down();
    new spell_viryx_cast_down_summ();
}
