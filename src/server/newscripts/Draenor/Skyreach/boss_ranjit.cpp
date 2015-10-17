/*
    http://epicwow.com/
    Dungeon : Skyreach 97 - 99
    Encounter: Ranjit
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "skyreach.h"

enum Says
{
    SAY_AGGRO           = 0,
    SAY_DEATH           = 1,
    SAY_KILL_PLAYER     = 2,
    SAY_SPELL           = 3,
    SAY_WTF             = 4,
    SAY_WARN_WTF        = 5,
};

enum Spells
{
    //SPELL_    = ,
    //SPELL_    = ,
    //SPELL_    = ,
    //SPELL_    = ,
};

enum eEvents
{
    //EVENT_    = 1,
    //EVENT_    = 2,
    //EVENT_    = 3,
    //EVENT_    = 4,
    //EVENT_    = 5,
};

class boss_ranjit : public CreatureScript
{
public:
    boss_ranjit() : CreatureScript("boss_ranjit") { }

    struct boss_ranjitAI : public BossAI
    {
        boss_ranjitAI(Creature* creature) : BossAI(creature, DATA_RANJIT) {}

        void Reset()
        {
            events.Reset();
            _Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);
            _EnterCombat();
            
            //events.ScheduleEvent(EVENT_1, ); //34:51
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
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
        return new boss_ranjitAI (creature);
    }
};

void AddSC_boss_ranjit()
{
    new boss_ranjit();
}
