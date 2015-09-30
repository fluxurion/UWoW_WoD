/*
    http://epicwow.com/
    Dungeon : Shadowmoon Burial Grounds 100
    Encounter: Ner'zhul
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

class boss_nerzhul : public CreatureScript
{
public:
    boss_nerzhul() : CreatureScript("boss_nerzhul") { }

    struct boss_nerzhulAI : public BossAI
    {
        boss_nerzhulAI(Creature* creature) : BossAI(creature, DATA_NERZHUL) {}

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
        return new boss_nerzhulAI (creature);
    }
};

void AddSC_boss_nerzhul()
{
    new boss_nerzhul();
}
