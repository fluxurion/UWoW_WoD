/*
    http://epicwow.com/
    Dungeon : Upper Blackrock Spire 90-100
    Encounter: Ragewing the Untamed
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "upper_blackrock_spire.h"

enum Spells
{
    SPELL_MAGMA_SPIT            = 155050,
    SPELL_RIPPING_CLAW          = 155065,
    SPELL_ENGULFING_FIRE        = 154996,
    SPELL_ENGULFING_FIRE_AURA   = 155025,
    SPELL_ENGULFING_FIRE_AT     = 155029,
    SPELL_SWIRLING_WINDS        = 167203,
    SPELL_FIRE_STORM            = 155070,
    SPELL_FIRE_STORM_DMG        = 155073,
    SPELL_FIRE_BREATH_CHECK     = 155605,
    SPELL_FIRE_BREATH_DMG       = 155606,
    SPELL_BURNING_RAGE          = 155620,
};

enum eEvents
{
    EVENT_MAGMA_SPIT            = 1,
    EVENT_RIPPING_CLAW          = 2,
    EVENT_ENGULFING_FIRE        = 3,
    EVENT_FLY                   = 4,
    EVENT_FIRE_STORM            = 5,
    EVENT_FIRE_BREATH           = 6,
    EVENT_FIRE_BREATH_DMG       = 7,
    EVENT_GROUND                = 8,
    EVENT_BURNING_RAGE          = 9,
    EVENT_ENGULFING_FIRE_END    = 10,
};

enum eSays
{
    SAY_WARN_ENGULFING_FIRE     = 0,
};

enum ePath
{
    PATH_FIRE_R  = 7681300,
    PATH_FIRE_L  = 7683700,
};

enum ePhase
{
    PHASE_BATTLE    = 1,
    PHASE_FLY       = 2,
};

class boss_ragewing_untamed : public CreatureScript
{
public:
    boss_ragewing_untamed() : CreatureScript("boss_ragewing_untamed") { }

    struct boss_ragewing_untamedAI : public BossAI
    {
        boss_ragewing_untamedAI(Creature* creature) : BossAI(creature, DATA_RAGEWING), summons(me) {}

        SummonList summons;

        uint8 healthPct;
        bool groundPhase;
        bool engulfing;

        void Reset()
        {
            events.Reset();
            _Reset();
            summons.DespawnAll();

            me->SetCanFly(true);
            me->SetDisableGravity(true);
            me->SetReactState(REACT_AGGRESSIVE);
            me->RemoveAurasDueToSpell(SPELL_BURNING_RAGE);
            SetCombatMovement(false);

            healthPct = 70;
            groundPhase = false;
            engulfing = false;
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();

            BattleGroundEvents();
        }

        void BattleGroundEvents()
        {
            events.SetPhase(PHASE_BATTLE);
            events.ScheduleEvent(EVENT_MAGMA_SPIT, 8000, 0, PHASE_BATTLE); //10:04
            events.ScheduleEvent(EVENT_RIPPING_CLAW, 10000, 0, PHASE_BATTLE);
            events.ScheduleEvent(EVENT_FIRE_BREATH, 2000, 0, PHASE_BATTLE);
            if (!groundPhase)
                events.ScheduleEvent(EVENT_ENGULFING_FIRE, 18000, 0, PHASE_BATTLE);
            else
                events.ScheduleEvent(EVENT_BURNING_RAGE, 12000, 0, PHASE_BATTLE);
        }
        
        void BattleFlyEvents()
        {
            events.SetPhase(PHASE_FLY);
            events.ScheduleEvent(EVENT_FLY, 1000, 0, PHASE_FLY);
            events.ScheduleEvent(EVENT_FIRE_STORM, 3000, 0, PHASE_FLY);
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

            summons.DoZoneInCombat();
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            switch (spell->Id)
            {
                case SPELL_ENGULFING_FIRE:
                    Talk(SAY_WARN_ENGULFING_FIRE);
                    me->SetFacingToObject(target);
                    break;
                case SPELL_ENGULFING_FIRE_AURA:
                {
                    if (target->GetEntry() == NPC_ENGULFING_FIRE_R)
                        target->GetMotionMaster()->MovePath(PATH_FIRE_R, false);
                    else if (target->GetEntry() == NPC_ENGULFING_FIRE_L)
                        target->GetMotionMaster()->MovePath(PATH_FIRE_L, false);
                    break;
                }
                case SPELL_FIRE_BREATH_CHECK:
                    events.CancelEvent(EVENT_FIRE_BREATH_DMG);
                    break;
            }
        }

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            if (damage >= me->GetHealth() && !groundPhase)
                damage = 0;

            if (me->HealthBelowPct(healthPct))
            {
                if (healthPct == 70)
                    healthPct = 40;
                else
                    healthPct = 0;

                for (uint8 i = 0; i < 10; i++)
                    me->SummonCreature(NPC_RAGEWING_WHELP, ragewingAdds[i]);

                BattleFlyEvents();
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == WAYPOINT_MOTION_TYPE)
            {
                switch (id)
                {
                    case 0:
                        DoCast(SPELL_SWIRLING_WINDS);
                        break;
                    case 2:
                    {
                        if (healthPct < 40)
                        {
                            groundPhase = true;
                            events.ScheduleEvent(EVENT_GROUND, 1000);
                        }
                        else
                            me->SetReactState(REACT_AGGRESSIVE);

                        BattleGroundEvents();
                        break;
                    }
                }
            }
            if (type == POINT_MOTION_TYPE)
                if (id == 1)
                {
                    me->SetCanFly(false);
                    me->SetDisableGravity(false);
                    me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_HOVER);
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
                    case EVENT_MAGMA_SPIT:
                        DoCast(SPELL_MAGMA_SPIT);
                        events.ScheduleEvent(EVENT_MAGMA_SPIT, 16000, 0, PHASE_BATTLE);
                        break;
                    case EVENT_RIPPING_CLAW:
                        if (Unit* target = me->getVictim())
                            DoCast(target, SPELL_RIPPING_CLAW);
                        events.ScheduleEvent(EVENT_RIPPING_CLAW, 10000, 0, PHASE_BATTLE);
                        break;
                    case EVENT_ENGULFING_FIRE:
                        engulfing = true;
                        DoStopAttack();
                        DoCast(SPELL_ENGULFING_FIRE);
                        events.ScheduleEvent(EVENT_ENGULFING_FIRE, 24000, 0, PHASE_BATTLE);
                        events.ScheduleEvent(EVENT_ENGULFING_FIRE_END, 4000, 0, PHASE_BATTLE);
                        break;
                    case EVENT_ENGULFING_FIRE_END:
                        me->SetReactState(REACT_AGGRESSIVE);
                        engulfing = false;
                        break;
                    case EVENT_FLY:
                        DoStopAttack();
                        me->GetMotionMaster()->MovePath(me->GetEntry() * 100, false);
                        break;
                    case EVENT_FIRE_STORM:
                        DoCast(SPELL_FIRE_STORM);
                        events.ScheduleEvent(EVENT_FIRE_STORM, 1000, 0, PHASE_FLY);
                        break;
                    case EVENT_FIRE_BREATH:
                        if (!engulfing)
                            DoCast(me, SPELL_FIRE_BREATH_CHECK, true);
                        events.ScheduleEvent(EVENT_FIRE_BREATH, 2000, 0, PHASE_BATTLE);
                        events.ScheduleEvent(EVENT_FIRE_BREATH_DMG, 2000, 0, PHASE_BATTLE);
                        break;
                    case EVENT_FIRE_BREATH_DMG:
                        if (!engulfing)
                            DoCast(me, SPELL_FIRE_BREATH_DMG, true);
                        break;
                    case EVENT_GROUND:
                        me->GetMotionMaster()->MovePoint(1, 33.63f, -405.50f, 110.7f);
                        break;
                    case EVENT_BURNING_RAGE:
                        DoCast(SPELL_BURNING_RAGE);
                        events.ScheduleEvent(EVENT_BURNING_RAGE, 12000, 0, PHASE_BATTLE);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ragewing_untamedAI (creature);
    }
};

//155070
class spell_fire_storm : public SpellScriptLoader
{
    public:
        spell_fire_storm() : SpellScriptLoader("spell_fire_storm") { }

        class spell_fire_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_fire_storm_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, SPELL_FIRE_STORM_DMG, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_fire_storm_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_fire_storm_SpellScript();
        }
};

void AddSC_boss_ragewing_untamed()
{
    new boss_ragewing_untamed();
    new spell_fire_storm();
}
