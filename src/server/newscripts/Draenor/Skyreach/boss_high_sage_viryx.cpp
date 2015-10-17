/*
    http://epicwow.com/
    Dungeon : Skyreach 97 - 99
    Encounter: High Sage Viryx
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "skyreach.h"

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
        return new boss_high_sage_viryxAI (creature);
    }
};

void AddSC_boss_high_sage_viryx()
{
    new boss_high_sage_viryx();
}
