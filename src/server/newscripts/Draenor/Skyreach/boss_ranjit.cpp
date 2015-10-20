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
    SAY_SPELL_F_WINDS   = 3,
    SAY_WTF             = 4,
    SAY_WARN_WTF        = 5,
};

enum Spells
{
    SPELL_SPINNING_BLADE     = 153544, //<
    SPELL_SPINNING_BLADE_AT  = 153588, // АТ бумеранг. дмг 153544
    SPELL_WINDWALL           = 153315, //краш клиента от АТ
    SPELL_FAN_OF_BLADES      = 153757,
    SPELL_PIERCING_RUSH      = 165733,
    SPELL_FOUR_WINDS         = 156793,
};

enum eEvents
{
    EVENT_SPINNING_BLADE    = 1,
    EVENT_WINDWALL          = 2,
    EVENT_FAN_BLADES        = 3,
    EVENT_PIERCING_RUSH     = 4,
    EVENT_FOUR_WINDS        = 5,
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

            events.ScheduleEvent(EVENT_SPINNING_BLADE, 4000); //34:51
            events.ScheduleEvent(EVENT_WINDWALL, 10000);
            events.ScheduleEvent(EVENT_FAN_BLADES, 10000);
            events.ScheduleEvent(EVENT_PIERCING_RUSH, 16000);
            events.ScheduleEvent(EVENT_FOUR_WINDS, 30000);
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

        void KilledUnit(Unit* victim)
        {
            if (victim->GetTypeId() != TYPEID_PLAYER)
                return;

            uint8 rand = urand(0, 1);
            if (rand)
                Talk(SAY_KILL_PLAYER);
        }

        void SpellHit(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_SPINNING_BLADE)
                DoCast(SPELL_SPINNING_BLADE_AT);
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
                    case EVENT_SPINNING_BLADE:
                        DoCast(SPELL_SPINNING_BLADE);
                        events.ScheduleEvent(EVENT_SPINNING_BLADE, 26000);
                        break;
                    case EVENT_WINDWALL:
                        DoCast(SPELL_WINDWALL);
                        events.ScheduleEvent(EVENT_WINDWALL, 14000);
                        break;
                    case EVENT_FAN_BLADES:
                        DoCast(SPELL_FAN_OF_BLADES);
                        events.ScheduleEvent(EVENT_FAN_BLADES, 16000);
                        break;
                    case EVENT_PIERCING_RUSH:
                        DoCast(SPELL_PIERCING_RUSH);
                        events.ScheduleEvent(EVENT_PIERCING_RUSH, 60000);
                        break;
                    case EVENT_FOUR_WINDS:
                        Talk(SAY_SPELL_F_WINDS);
                        DoCast(SPELL_FOUR_WINDS);
                        events.ScheduleEvent(EVENT_FOUR_WINDS, 46000);
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
