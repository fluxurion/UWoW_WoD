/*
    http://epicwow.com/
    Dungeon : The Everbloom 100
    Encounter: Ancient Protectors
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_everbloom.h"

enum Says
{
    //Gola
    SAY_GOLA_AGGRO         = 1,
    SAY_GOLA_REVITALIZING  = 2,
    SAY_GOLA_RAPID_TIDES   = 3,
    SAY_GOLA_DEATH         = 6,

    //Telu
    SAY_TELU_BRAMBLE       = 2,
    SAY_TELU_DEATH         = 4,
};

enum Spells
{
    //Life Warden Gola
    SPELL_WATER_COSMETIC_CHANNEL    = 173380,
    SPELL_WATER_BOLT                = 168092,
    SPELL_REVITALIZING_WATERS       = 168082,
    SPELL_RAPID_TIDES               = 168105,

    //Earthshaper Telu
    SPELL_NATURE_COSMETIC_CHANNEL   = 172325,
    SPELL_NATURE_WRATH              = 168040,
    SPELL_BRAMBLE_PATCH             = 177497,
    SPELL_BRIARSKIN                 = 168041,

    //Dulhu
    SPELL_RENDING_CHARGE            = 168186,
    SPELL_NOXIOUS_ERUPTION          = 175997,
    SPELL_GRASPING_VINE             = 168375,
    SPELL_GRASPING_VINE_VISUAL      = 168376,
    SPELL_GRASPING_VINE_LEAP        = 168378,
    SPELL_SLASH                     = 168383,
};

enum eEvents
{
    //Life Warden Gola
    EVENT_WATER_BOLT        = 1,
    EVENT_HEAL_PCT          = 2,
    EVENT_RAPID_TIDES       = 3,

    //Earthshaper Telu
    EVENT_NATURE_WRATH      = 1,
    EVENT_BRAMBLE_PATCH     = 2,
    EVENT_BRIARSKIN         = 3,

    //Dulhu
    EVENT_RENDING_CHARGE    = 1,
    EVENT_NOXIOUS_ERUPTION  = 2,
    EVENT_GRASPING_VINE     = 3,
};

uint32 const protectors[3] = { NPC_LIFE_WARDEN_GOLA, NPC_EARTHSHAPER_TELU, NPC_DULHU};

struct boss_encounter_ancient_protectors : public BossAI
{
    boss_encounter_ancient_protectors(Creature* creature) : BossAI(creature, DATA_PROTECTORS) {}

    uint8 bossDiedCount;

    void Reset()
    {
        _Reset();
        events.Reset();
    }

    void EnterCombat(Unit* who)
    {
        if (instance->GetBossState(DATA_PROTECTORS) != IN_PROGRESS)
            instance->SetBossState(DATA_PROTECTORS, IN_PROGRESS);
    }

    void JustDied(Unit* /*killer*/)
    {
        events.Reset();

        bossDiedCount = 0;
        for (uint8 i = 0; i < 3; i++)
            if (Creature* enfor = ObjectAccessor::GetCreature(*me, instance->GetGuidData(protectors[i])))
                if (!enfor->isAlive())
                    bossDiedCount++;

        if (bossDiedCount == 3)
            instance->SetBossState(DATA_PROTECTORS, DONE);
    }
};

//83892
class boss_life_warden_gola : public CreatureScript
{
public:
    boss_life_warden_gola() : CreatureScript("boss_life_warden_gola") { }

    struct boss_life_warden_golaAI : public boss_encounter_ancient_protectors
    {
        boss_life_warden_golaAI(Creature* creature) : boss_encounter_ancient_protectors(creature) {}

        void Reset()
        {
            boss_encounter_ancient_protectors::Reset();
            DoCast(SPELL_WATER_COSMETIC_CHANNEL);
        }

        void EnterCombat(Unit* who)
        {
            boss_encounter_ancient_protectors::EnterCombat(who);
            Talk(SAY_GOLA_AGGRO);
            events.ScheduleEvent(EVENT_WATER_BOLT, 0); //38:00
            events.ScheduleEvent(EVENT_HEAL_PCT, 18000);
            events.ScheduleEvent(EVENT_RAPID_TIDES, 22000);
        }

        void JustDied(Unit* /*killer*/)
        {
            boss_encounter_ancient_protectors::JustDied(NULL);
            Talk(SAY_GOLA_DEATH);
        }

        void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_RAPID_TIDES)
            {
                events.ScheduleEvent(EVENT_HEAL_PCT, 2000);
                events.ScheduleEvent(EVENT_RAPID_TIDES, 2000);
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
                    case EVENT_WATER_BOLT:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        {
                            DoResetThreat();
                            me->AddThreat(pTarget, 10000.0f);
                            DoCast(pTarget, SPELL_WATER_BOLT);
                        }
                        events.ScheduleEvent(EVENT_WATER_BOLT, 3100);
                        break;
                    case EVENT_HEAL_PCT:
                    {
                        Creature* targetA = instance->instance->GetCreature(instance->GetGuidData(NPC_EARTHSHAPER_TELU));
                        Creature* targetB = instance->instance->GetCreature(instance->GetGuidData(NPC_DULHU));
                        if (!targetA || !targetB)
                            return;

                        Unit* target = NULL;
                        if (targetA->isAlive() && targetB->isAlive())
                            target = targetA->GetHealthPct() < targetB->GetHealthPct() ? targetA : targetB;

                        else if (!targetA->isAlive() && !targetB->isAlive())
                            target = me;
                        else if (!targetA->isAlive() || !targetB->isAlive())
                            target = targetA->isAlive() ? targetA : targetB;

                        if (target)
                            DoCast(target, SPELL_REVITALIZING_WATERS);

                        if (me->HasAura(SPELL_RAPID_TIDES))
                            events.ScheduleEvent(EVENT_HEAL_PCT, 2000);
                        else
                        {
                            Talk(SAY_GOLA_REVITALIZING);
                            events.ScheduleEvent(EVENT_HEAL_PCT, 22000);
                        }
                        break;
                    }
                    case EVENT_RAPID_TIDES:
                        DoCast(SPELL_RAPID_TIDES);
                        if (me->HasAura(SPELL_RAPID_TIDES))
                            events.ScheduleEvent(EVENT_RAPID_TIDES, 2000);
                        else
                        {
                            Talk(SAY_GOLA_RAPID_TIDES);
                            events.ScheduleEvent(EVENT_RAPID_TIDES, 22000);
                        }
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_life_warden_golaAI (creature);
    }
};

//83893
class boss_earthshaper_telu : public CreatureScript
{
public:
    boss_earthshaper_telu() : CreatureScript("boss_earthshaper_telu") { }

    struct boss_earthshaper_teluAI : public boss_encounter_ancient_protectors
    {
        boss_earthshaper_teluAI(Creature* creature) : boss_encounter_ancient_protectors(creature) {}

        void Reset()
        {
            boss_encounter_ancient_protectors::Reset();
            DoCast(SPELL_NATURE_COSMETIC_CHANNEL);
        }

        void EnterCombat(Unit* who)
        {
            boss_encounter_ancient_protectors::EnterCombat(who);
            events.ScheduleEvent(EVENT_NATURE_WRATH, 0); //38:00
            events.ScheduleEvent(EVENT_BRAMBLE_PATCH, 12000);
            events.ScheduleEvent(EVENT_BRIARSKIN, 26000);
        }

        void JustDied(Unit* /*killer*/)
        {
            boss_encounter_ancient_protectors::JustDied(NULL);
            Talk(SAY_TELU_DEATH);
        }

        void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_RAPID_TIDES)
            {
                events.ScheduleEvent(EVENT_BRAMBLE_PATCH, 2000);
                events.ScheduleEvent(EVENT_BRIARSKIN, 2000);
            }
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
                    case EVENT_NATURE_WRATH:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        {
                            DoResetThreat();
                            me->AddThreat(pTarget, 10000.0f);
                            DoCast(pTarget, SPELL_NATURE_WRATH);
                        }
                        events.ScheduleEvent(EVENT_NATURE_WRATH, 3100);
                        break;
                    case EVENT_BRAMBLE_PATCH:
                        DoCast(SPELL_BRAMBLE_PATCH);
                        if (me->HasAura(SPELL_RAPID_TIDES))
                            events.ScheduleEvent(EVENT_BRAMBLE_PATCH, 2000);
                        else
                        {
                            Talk(SAY_TELU_BRAMBLE);
                            events.ScheduleEvent(EVENT_BRAMBLE_PATCH, 20000);
                        }
                        break;
                    case EVENT_BRIARSKIN:
                        Creature* targetA = instance->instance->GetCreature(instance->GetGuidData(NPC_LIFE_WARDEN_GOLA));
                        Creature* targetB = instance->instance->GetCreature(instance->GetGuidData(NPC_DULHU));
                        if (!targetA || !targetB)
                            return;

                        Unit* target = NULL;
                        if (targetA->isAlive() && targetB->isAlive())
                            target = targetA->GetHealthPct() < targetB->GetHealthPct() ? targetA : targetB;

                        else if (!targetA->isAlive() && !targetB->isAlive())
                            target = me;
                        else if (!targetA->isAlive() || !targetB->isAlive())
                            target = targetA->isAlive() ? targetA : targetB;

                        if (target)
                            DoCast(target, SPELL_BRIARSKIN);

                        if (me->HasAura(SPELL_RAPID_TIDES))
                            events.ScheduleEvent(EVENT_BRIARSKIN, 2000);
                        else
                            events.ScheduleEvent(EVENT_BRIARSKIN, 22000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_earthshaper_teluAI (creature);
    }
};

//83894
class boss_dulhu : public CreatureScript
{
public:
    boss_dulhu() : CreatureScript("boss_dulhu") { }

    struct boss_dulhuAI : public boss_encounter_ancient_protectors
    {
        boss_dulhuAI(Creature* creature) : boss_encounter_ancient_protectors(creature) {}

        void Reset()
        {
            boss_encounter_ancient_protectors::Reset();
        }

        void EnterCombat(Unit* who)
        {
            boss_encounter_ancient_protectors::EnterCombat(who);
            events.ScheduleEvent(EVENT_RENDING_CHARGE, 6000); //38:00
            events.ScheduleEvent(EVENT_NOXIOUS_ERUPTION, 7000);
            events.ScheduleEvent(EVENT_GRASPING_VINE, 22000);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            boss_encounter_ancient_protectors::JustDied(NULL);
        }

        void SpellHit(Unit* caster, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_RAPID_TIDES)
            {
                events.ScheduleEvent(EVENT_NOXIOUS_ERUPTION, 2000);
                events.ScheduleEvent(EVENT_GRASPING_VINE, 2000);
            }
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_GRASPING_VINE_VISUAL)
            {
                target->CastSpell(me, SPELL_GRASPING_VINE_LEAP, true);
                DoCast(target, SPELL_SLASH);
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
                    case EVENT_RENDING_CHARGE:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 60, true))
                            DoCast(pTarget, SPELL_RENDING_CHARGE);
                        events.ScheduleEvent(EVENT_RENDING_CHARGE, 20000);
                        break;
                    case EVENT_NOXIOUS_ERUPTION:
                        DoCast(SPELL_NOXIOUS_ERUPTION);
                        if (me->HasAura(SPELL_RAPID_TIDES))
                            events.ScheduleEvent(EVENT_NOXIOUS_ERUPTION, 2000);
                        else
                            events.ScheduleEvent(EVENT_NOXIOUS_ERUPTION, 18000);
                        break;
                    case EVENT_GRASPING_VINE:
                        DoCast(SPELL_GRASPING_VINE);
                        if (me->HasAura(SPELL_RAPID_TIDES))
                            events.ScheduleEvent(EVENT_GRASPING_VINE, 2000);
                        else
                            events.ScheduleEvent(EVENT_GRASPING_VINE, 22000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_dulhuAI (creature);
    }
};

void AddSC_boss_ancient_protectors()
{
    new boss_life_warden_gola();
    new boss_earthshaper_telu();
    new boss_dulhu();
}
