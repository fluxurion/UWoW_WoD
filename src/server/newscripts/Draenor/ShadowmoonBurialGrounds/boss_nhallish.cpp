/*
    http://epicwow.com/
    Dungeon : Shadowmoon Burial Grounds 100
    Encounter: Nhallish
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "shadowmoon_burial_grounds.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
    SAY_EVADE           = ,
}; */

/* enum Spells
{
    SPELL_    = ,
    SPELL_    = ,
    SPELL_    = ,
    SPELL_    = ,
}; */

/* enum eEvents
{
    EVENT_    = 1,
    EVENT_    = 2,
    EVENT_    = 3,
    EVENT_    = 4,
    EVENT_    = 5,
}; */

class boss_nhallish : public CreatureScript
{
public:
    boss_nhallish() : CreatureScript("boss_nhallish") { }

    struct boss_nhallishAI : public BossAI
    {
        boss_nhallishAI(Creature* creature) : BossAI(creature, DATA_NHALLISH) {}

        void Reset()
        {
            events.Reset();
            _Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();
        }

        void EnterEvadeMode()
        {
            //Talk(SAY_EVADE);
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
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
        return new boss_nhallishAI (creature);
    }
};

void AddSC_boss_nhallish()
{
    new boss_nhallish();
}
