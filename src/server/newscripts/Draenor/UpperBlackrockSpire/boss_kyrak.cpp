/*
    http://epicwow.com/
    Dungeon : Upper Blackrock Spire 90-100
    Encounter: Kyrak
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "upper_blackrock_spire.h"
#include "CreatureGroups.h"

enum Says
{
    SAY_AGGRO           = 0,
    SAY_VILEBLOOD       = 1,
    SAY_FIXATION        = 2,
    SAY_DEATH           = 3,
};

enum Spells
{
    //Kyrak
    SPELL_DEBILITATING_FIXATION    = 177660,
    SPELL_VILEBLOOD_SERUM_TARGET   = 161207,
    SPELL_VILEBLOOD_SERUM_FIENDLY  = 161235,
    SPELL_VILEBLOOD_SERUM_AT       = 161210,
    SPELL_REJUVENATING_SERUM       = 161203,
    SPELL_SALVE_OF_TOXIC_FUMES     = 162589,
    //Trash
    SPELL_MONSTROUS_CLAWS          = 155032,
    SPELL_ERUPTION                 = 155037,
};

enum eEvents
{
    //Kyrak
    EVENT_FIXATION      = 1,
    EVENT_VILEBLOOD     = 2,
    EVENT_REJUVENATING  = 3,
    EVENT_TOXIC_FUMES   = 4,
    //Trash
    EVENT_ERUPTION      = 1,
};

Position const drakonidSpawn[2] = 
{
    {86.39f, -328.90f, 91.52f, 1.9f},
    {69.96f, -328.22f, 91.54f, 0.5f},
};

class boss_kyrak : public CreatureScript
{
public:
    boss_kyrak() : CreatureScript("boss_kyrak") { }

    struct boss_kyrakAI : public BossAI
    {
        boss_kyrakAI(Creature* creature) : BossAI(creature, DATA_KYRAK), summons(me) 
        {
            group_member = sFormationMgr->CreateCustomFormation(me);
        }

        SummonList summons;
        FormationInfo* group_member;

        void Reset()
        {
            events.Reset();
            _Reset();
            summons.DespawnAll();
            me->RemoveAreaObject(SPELL_VILEBLOOD_SERUM_AT);

            for (uint8 i = 0; i < 2; i++)
                me->SummonCreature(NPC_DRAKONID_MONSTROSITY, drakonidSpawn[i]);
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_FIXATION, 14000); //54:37
            events.ScheduleEvent(EVENT_VILEBLOOD, 10000);
            events.ScheduleEvent(EVENT_REJUVENATING, 20000);
            if (me->GetMap()->GetDifficultyID() != DIFFICULTY_NORMAL)
                events.ScheduleEvent(EVENT_TOXIC_FUMES, 16000);
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

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            if (CreatureGroup* f = me->GetFormation())
                f->AddMember(summon, group_member);
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            if (CreatureGroup* f = me->GetFormation())
                f->RemoveMember(summon);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_DEBILITATING_FIXATION)
                me->AddThreat(target, 10000.0f);

            if (spell->Id == SPELL_VILEBLOOD_SERUM_FIENDLY)
                target->CastSpell(target, SPELL_VILEBLOOD_SERUM_TARGET, true);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            EnterEvadeIfOutOfCombatArea(diff);
            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FIXATION:
                        DoResetThreat();
                        Talk(SAY_FIXATION);
                        DoCast(SPELL_DEBILITATING_FIXATION);
                        events.ScheduleEvent(EVENT_FIXATION, 18000);
                        break;
                    case EVENT_VILEBLOOD:
                        Talk(SAY_VILEBLOOD);
                        DoCast(SPELL_VILEBLOOD_SERUM_FIENDLY);
                        events.ScheduleEvent(EVENT_VILEBLOOD, 16000);
                        break;
                    case EVENT_REJUVENATING:
                        DoCast(SPELL_REJUVENATING_SERUM);
                        events.ScheduleEvent(EVENT_REJUVENATING, 30000);
                        break;
                    case EVENT_TOXIC_FUMES:
                        DoCast(SPELL_SALVE_OF_TOXIC_FUMES);
                        events.ScheduleEvent(EVENT_TOXIC_FUMES, 12000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_kyrakAI (creature);
    }
};

//82556
class npc_drakonid_monstrosity : public CreatureScript
{
public:
    npc_drakonid_monstrosity() : CreatureScript("npc_drakonid_monstrosity") { }

    struct npc_drakonid_monstrosityAI : public CreatureAI
    {
        npc_drakonid_monstrosityAI(Creature* creature) : CreatureAI(creature) {}

        uint32 eruptionTimer;

        void EnterCombat(Unit* /*who*/) {}

        void Reset() 
        {
            eruptionTimer = 10000;
            DoCast(SPELL_MONSTROUS_CLAWS);
        }

        void UpdateAI(uint32 diff) 
        {
            if (!UpdateVictim())
                return;

            if (eruptionTimer <= diff)
            {
                DoCast(SPELL_ERUPTION);
                eruptionTimer = 18000;
            }
            else
                eruptionTimer -=diff;

            if (!me->HasUnitState(UNIT_STATE_CASTING))
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_drakonid_monstrosityAI (creature);
    }
};

void AddSC_boss_kyrak()
{
    new boss_kyrak();
    new npc_drakonid_monstrosity();
}
