/*
    http://epicwow.com/
    Dungeon : The Everbloom 100
    Encounter: Ancient Protectors
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_everbloom.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
    SAY_EVADE           = ,
}; */

enum Spells
{
    //Life Warden Gola
    SPELL_WATER_COSMETIC_CHANNEL    = 173380,
    SPELL_WATER_BOLT                = 168092,
    SPELL_REVITALIZING_WATERS       = 168082,

    //Earthshaper Telu
    SPELL_NATURE_COSMETIC_CHANNEL   = 172325,

    //Dulhu
    //SPELL_    = ,
};

enum eEvents
{
    //Life Warden Gola
    EVENT_WATER_BOLT        = 1,
    EVENT_HEAL_PCT          = 2,
    //EVENT_    = 3,
    //EVENT_    = 4,
    //EVENT_    = 5,
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
            events.ScheduleEvent(EVENT_WATER_BOLT, 0); //38:00
            events.ScheduleEvent(EVENT_HEAL_PCT, 18000);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            boss_encounter_ancient_protectors::JustDied(NULL);
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
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 90, true))
                        {
                            DoResetThreat();
                            me->AddThreat(pTarget, 10000.0f);
                            DoCast(SPELL_WATER_BOLT);
                        }
                        events.ScheduleEvent(EVENT_WATER_BOLT, 3000);
                        break;
                    case EVENT_HEAL_PCT:
                        Creature* targetA = ObjectAccessor::GetCreature(*me, instance->GetGuidData(NPC_EARTHSHAPER_TELU))
                        if (!targetA)
                            return;
                        Creature* targetB = ObjectAccessor::GetCreature(*me, instance->GetGuidData(NPC_DULHU))
                        if (!targetB)
                            return;

                        if (targetA->GetHealthPct() > targetB->GetHealthPct())
                            DoCast(targetB, SPELL_REVITALIZING_WATERS);
                        else
                            DoCast(targetA, SPELL_REVITALIZING_WATERS);

                        events.ScheduleEvent(EVENT_HEAL_PCT, 22000);
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
        }

        void JustDied(Unit* /*killer*/)
        {
            boss_encounter_ancient_protectors::JustDied(NULL);
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
            //events.ScheduleEvent(EVENT_, 30000); //
        }

        void JustDied(Unit* /*killer*/)
        {
            boss_encounter_ancient_protectors::JustDied(NULL);
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
