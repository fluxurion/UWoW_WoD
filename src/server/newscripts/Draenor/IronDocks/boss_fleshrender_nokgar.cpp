/*
    http://epicwow.com/
    Dungeon : Iron Docks 93-95
    Encounter: Fleshrender Nok'gar
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "iron_docks.h"

enum Says
{
    SAY_AGGRO                 = 0,
    SAY_BURNING_ARROWS        = 1,
    SAY_BARBED_ARROW_BARRAGE  = 2,
    SAY_RECKLESS_PROVOCATION  = 3,
    SAY_WARN_RECKLESS         = 4,
    SAY_KILLED_PLAYER         = 5,
    SAY_DEATH                 = 6,
};

enum Spells
{
    //Nokgar
    SPELL_SUMMON_DREADFANG      = 164049,
    SPELL_RIDE_VEHICLE          = 46598,
    SPELL_RECKLESS_PROVOCATION  = 164426,
    SPELL_BURNING_ARROWS        = 164635,
    SPELL_BURNING_ARROWS_SELECT = 166186,
    SPELL_BURNING_ARROWS_AT     = 164234,
    SPELL_BARBED_ARROW_BARRAGE  = 166923,
    SPELL_BARBED_ARROW_SELECT   = 166914,

    //Dreadfang
    SPELL_SAVAGE_MAULING        = 166290,
    SPELL_SAVAGE_MAULING_AURA   = 164837,
    SPELL_BLOODLETTING_HOWL     = 164835,
    SPELL_SHREDDING_SWIPES      = 164730,
    SPELL_SHREDDING_SWIPES_AT   = 164733,
    SPELL_SHREDDING_SWIP_REMOVE = 164735,
};

enum eEvents
{
    //Nokgar
    EVENT_RIDE_VEHICLE              = 1,
    EVENT_RECKLESS_PROVOCATION      = 2,
    EVENT_BURNING_ARROWS            = 3,
    EVENT_BARBED_ARROWS             = 4,

    //Dreadfang
    EVENT_SAVAGE_MAULING            = 1,
    EVENT_SHREDDING_SWIPES          = 2,
    EVENT_BLOODLETTING_HOWL         = 3,
};

class boss_fleshrender_nokgar : public CreatureScript
{
public:
    boss_fleshrender_nokgar() : CreatureScript("boss_fleshrender_nokgar") { }

    struct boss_fleshrender_nokgarAI : public BossAI
    {
        boss_fleshrender_nokgarAI(Creature* creature) : BossAI(creature, DATA_NOKGAR), summons(me) {}

        SummonList summons;
        Unit* dreadfang = NULL;

        void Reset()
        {
            _Reset();
            summons.DespawnAll();
            events.Reset();

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);

            DoCast(SPELL_SUMMON_DREADFANG);
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_RECKLESS_PROVOCATION, 34000);
            events.ScheduleEvent(EVENT_BURNING_ARROWS, 16000);
            events.ScheduleEvent(EVENT_BARBED_ARROWS, 44000);
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }
        
        void JustReachedHome()
        {
            DoCast(SPELL_SUMMON_DREADFANG);
        }

        void KilledUnit(Unit* victim)
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
                Talk(SAY_KILLED_PLAYER);
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            dreadfang = summon;
            events.ScheduleEvent(EVENT_RIDE_VEHICLE, 500);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (target->GetEntry() == NPC_GROMKAR_FLAMESLINGER)
            {
                if (spell->Id == SPELL_BURNING_ARROWS_SELECT)
                    if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                        target->CastSpell(pTarget, SPELL_BURNING_ARROWS_AT);

                if (spell->Id == SPELL_BARBED_ARROW_BARRAGE)
                    if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                        target->CastSpell(pTarget, SPELL_BARBED_ARROW_SELECT);
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() && me->isInCombat())
                return;

            if (me->GetDistance(me->GetHomePosition()) >= 90.0f)
            {
                EnterEvadeMode();
                return;
            }

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_RIDE_VEHICLE:
                        if (dreadfang)
                            me->CastSpell(dreadfang, SPELL_RIDE_VEHICLE, true);
                        break;
                    case EVENT_RECKLESS_PROVOCATION:
                        if (!me->HasAura(SPELL_RIDE_VEHICLE))
                        {
                            Talk(SAY_WARN_RECKLESS);
                            Talk(SAY_RECKLESS_PROVOCATION);
                            DoCast(SPELL_RECKLESS_PROVOCATION);
                        }
                        events.ScheduleEvent(EVENT_RECKLESS_PROVOCATION, 42000);
                        break;
                    case EVENT_BURNING_ARROWS:
                        Talk(SAY_BURNING_ARROWS);
                        DoCast(SPELL_BURNING_ARROWS);
                        events.ScheduleEvent(EVENT_BURNING_ARROWS, 18000);
                        break;
                    case EVENT_BARBED_ARROWS:
                        Talk(SAY_BARBED_ARROW_BARRAGE);
                        DoCast(SPELL_BARBED_ARROW_BARRAGE);
                        events.ScheduleEvent(EVENT_BARBED_ARROWS, 42000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_fleshrender_nokgarAI (creature);
    }
};

// 81297
class npc_dreadfang : public CreatureScript
{
public:
    npc_dreadfang() : CreatureScript("npc_dreadfang") { }

    struct npc_dreadfangAI : public ScriptedAI
    {
        npc_dreadfangAI(Creature* creature) : ScriptedAI(creature), summons(me)
        {
            instance = creature->GetInstanceScript();
            exitPassenger = false;
        }

        InstanceScript* instance;
        SummonList summons;
        EventMap events;

        Unit* nokgar = NULL;
        bool exitPassenger;

        void Reset()
        {
            events.Reset();
            me->SetReactState(REACT_AGGRESSIVE);
            summons.DespawnEntry(NPC_SHREDDING_SWIPES);
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (nokgar)
                nokgar->ToCreature()->AI()->DoZoneInCombat();

            events.ScheduleEvent(EVENT_SAVAGE_MAULING, 28000);
            events.ScheduleEvent(EVENT_SHREDDING_SWIPES, 36000);
            events.ScheduleEvent(EVENT_BLOODLETTING_HOWL, 44000);
        }

        void EnterEvadeMode()
        {
            ScriptedAI::EnterEvadeMode();

            if (nokgar)
                nokgar->ToCreature()->AI()->EnterEvadeMode();
        }

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            if (me->HealthBelowPct(50) && !exitPassenger)
            {
                exitPassenger = true;
                me->GetVehicleKit()->RemoveAllPassengers();
                
                if (nokgar)
                    nokgar->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE)
            {
                if (id == 1)
                {
                    summons.DespawnEntry(NPC_SHREDDING_SWIPES);
                    DoCast(SPELL_SHREDDING_SWIP_REMOVE);
                    me->SetReactState(REACT_AGGRESSIVE);
                }
            }
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            if (summon->GetEntry() == NPC_SHREDDING_SWIPES)
                me->GetMotionMaster()->MovePoint(1, summon->GetPositionX(), summon->GetPositionY(), summon->GetPositionZ());
        }

        void IsSummonedBy(Unit* summoner)
        {
            nokgar = summoner;
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

            if (Unit* target = me->getVictim())
                if (target->HasAura(SPELL_SAVAGE_MAULING_AURA))
                    return;

            if (me->HasAura(SPELL_SHREDDING_SWIPES_AT))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SAVAGE_MAULING:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 40, true))
                        {
                            me->AddThreat(pTarget, 10000.0f);
                            me->CastSpell(pTarget, SPELL_SAVAGE_MAULING_AURA);
                        }
                        events.ScheduleEvent(EVENT_SAVAGE_MAULING, 10000);
                        break;
                    case EVENT_SHREDDING_SWIPES:
                        DoStopAttack();
                        DoCast(SPELL_SHREDDING_SWIPES_AT);
                        DoCast(SPELL_SHREDDING_SWIPES);
                        events.ScheduleEvent(EVENT_SHREDDING_SWIPES, 36000);
                        break;
                    case EVENT_BLOODLETTING_HOWL:
                        DoCast(SPELL_BLOODLETTING_HOWL);
                        events.ScheduleEvent(EVENT_BLOODLETTING_HOWL, 44000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_dreadfangAI (creature);
    }
};

void AddSC_boss_fleshrender_nokgar()
{
    new boss_fleshrender_nokgar();
    new npc_dreadfang();
}
