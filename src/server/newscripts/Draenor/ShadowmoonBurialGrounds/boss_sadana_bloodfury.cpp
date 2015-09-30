/*
    http://epicwow.com/
    Dungeon : Shadowmoon Burial Grounds 100
    Encounter: Sadana Bloodfury
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "shadowmoon_burial_grounds.h"

enum Says
{
    SAY_ENTER_ROOM      = 0,
    SAY_AGGRO           = 1,
    SAY_DEATH           = 2,
    SAY_KILL_PLAYER     = 3,
    SAY_SPELL_1         = 4,
    SAY_SPELL_2         = 5,
    SAY_SPELL_3         = 6,
    SAY_SPELL_4         = 7,
};

enum Spells
{
    //Runes
    SPELL_SHADOW_RUNE_1         = 152684,
    SPELL_SHADOW_RUNE_2         = 152691,
    //Sadana
    SPELL_SHADOW_RITUAL_VISUAL  = 152158,
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

class boss_sadana_bloodfury : public CreatureScript
{
public:
    boss_sadana_bloodfury() : CreatureScript("boss_sadana_bloodfury") { }

    struct boss_sadana_bloodfuryAI : public BossAI
    {
        boss_sadana_bloodfuryAI(Creature* creature) : BossAI(creature, DATA_SADANA) 
        {
            DoCast(SPELL_SHADOW_RITUAL_VISUAL);
            intro = true;
        }

        bool intro;

        void Reset()
        {
            events.Reset();
            _Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);
            _EnterCombat();
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

        void MoveInLineOfSight(Unit* who)
        {
            Player* player = who->ToPlayer();
            if (!player || !me->IsWithinDistInMap(who, 110.0f))
                return;

            if (intro)
            {
                intro = false;
                Talk(SAY_ENTER_ROOM);
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
                    case EVENT_1:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_sadana_bloodfuryAI (creature);
    }
};

class npc_sadana_shadow_rune : public CreatureScript
{
public:
    npc_sadana_shadow_rune() : CreatureScript("npc_sadana_shadow_rune") { }

    struct npc_sadana_shadow_runeAI : public Scripted_NoMovementAI
    {
        npc_sadana_shadow_runeAI(Creature* creature) : Scripted_NoMovementAI(creature) {}

        void Reset() 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void IsSummonedBy(Unit* summoner)
        {
            if (summoner->GetEntry() == NPC_WORD_TRIGGER)
                DoCast(SPELL_SHADOW_RUNE_1);

            if (summoner->GetEntry() == NPC_SADANA_BLOODFURY)
                DoCast(SPELL_SHADOW_RUNE_2);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (target->GetTypeId() == TYPEID_PLAYER)
                me->DespawnOrUnsummon();
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_sadana_shadow_runeAI (creature);
    }
};

void AddSC_boss_sadana_bloodfury()
{
    new boss_sadana_bloodfury();
    new npc_sadana_shadow_rune();
}
