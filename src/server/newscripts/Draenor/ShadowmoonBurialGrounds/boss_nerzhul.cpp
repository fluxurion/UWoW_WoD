/*
    http://epicwow.com/
    Dungeon : Shadowmoon Burial Grounds 100
    Encounter: Ner'zhul
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "shadowmoon_burial_grounds.h"
#include "CreatureGroups.h"

#define MAX_DIST    60

enum Says
{
    SAY_AGGRO           = 0,
    SAY_BONE            = 1,
    SAY_DEATH           = 2,
};

enum Spells
{
    //Intro scene
    SPELL_FLOATING_DEAD            = 160467,
    SPELL_CLIENT_SCENE_TELEPORT    = 178093,
    //Nerzhul
    SPELL_MALEVOLENCE_SUM          = 154439,
    SPELL_MALEVOLENCE_DMG          = 154442,
    SPELL_OMEN_OF_DEATH            = 177691,
    //Omen
    SPELL_OMEN_OF_DEATH_VISUAL     = 154351,
    SPELL_OMEN_OF_DEATH_DMG        = 154352,
    //Bones
    SPELL_RITUAL_OF_BONES          = 154559,
    SPELL_RITUAL_OF_BONES_VISUAL   = 156312,
    SPELL_RITUAL_OF_BONES_DMG_FIN  = 160537,
};

enum eEvents
{
    EVENT_MALEVOLENCE      = 1,
    EVENT_OMEN_OF_DEATH    = 2,
    EVENT_RITUAL_OF_BONES  = 3,
};

Position const bonePos[6] =
{
    {1679.81f, -829.47f, 73.85f, 0.1f},
    {1679.28f, -815.14f, 73.85f, 0.1f},
    {1681.24f, -843.43f, 73.85f, 0.1f},
    {1679.23f, -802.71f, 73.85f, 0.1f},
    {1693.66f, -854.61f, 73.85f, 0.1f},
    {1689.09f, -790.68f, 73.85f, 0.1f},
};

class boss_nerzhul : public CreatureScript
{
public:
    boss_nerzhul() : CreatureScript("boss_nerzhul") { }

    struct boss_nerzhulAI : public BossAI
    {
        boss_nerzhulAI(Creature* creature) : BossAI(creature, DATA_NERZHUL), summons(me) {}

        SummonList summons;
        uint8 bonesCount;

        void Reset()
        {
            events.Reset();
            _Reset();

            summons.DespawnAll();
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_MALEVOLENCE, 6000); //45:07
            events.ScheduleEvent(EVENT_OMEN_OF_DEATH, 10000);
            events.ScheduleEvent(EVENT_RITUAL_OF_BONES, 20000);
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();

            me->SummonCreature(NPC_PORTAL, 1727.25f, -810.65f, 73.80f, 3.56f);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
            summons.SetReactState(REACT_PASSIVE);

            switch (summon->GetEntry())
            {
                case NPC_OMEN_OF_DEATH:
                    summon->CastSpell(summon, SPELL_OMEN_OF_DEATH_VISUAL, true);
                    summon->CastSpell(summon, SPELL_OMEN_OF_DEATH_DMG, true);
                    break;
                case NPC_RITUAL_OF_BONES:
                    if (bonesCount < 4)
                        summon->AI()->DoAction(1);
                    else
                        summon->AI()->DoAction(2);
                    bonesCount++;
                    break;
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
                    case EVENT_MALEVOLENCE:
                        DoCast(me, SPELL_MALEVOLENCE_SUM, true);
                        DoCast(SPELL_MALEVOLENCE_DMG);
                        events.ScheduleEvent(EVENT_MALEVOLENCE, 10000);
                        break;
                    case EVENT_OMEN_OF_DEATH:
                        DoCast(SPELL_OMEN_OF_DEATH);
                        events.ScheduleEvent(EVENT_OMEN_OF_DEATH, 12000);
                        break;
                    case EVENT_RITUAL_OF_BONES:
                        bonesCount = 0;
                        Talk(SAY_BONE);
                        for (uint8 i = 0; i < 6; i++)
                            me->SummonCreature(NPC_RITUAL_OF_BONES, bonePos[i]);
                        events.ScheduleEvent(EVENT_RITUAL_OF_BONES, 50000);
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

//79497
class npc_shadowmoon_nerzhul_intro : public CreatureScript
{
public:
    npc_shadowmoon_nerzhul_intro() : CreatureScript("npc_shadowmoon_nerzhul_intro") { }

    struct npc_shadowmoon_nerzhul_introAI : public ScriptedAI
    {
        npc_shadowmoon_nerzhul_introAI(Creature* creature) : ScriptedAI(creature) 
        {
            SetCombatMovement(false);
        }

        void Reset() {}

        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
            {
                if (me->IsWithinDistInMap(who, 70.0f))
                {
                    who->CastSpell(who, SPELL_FLOATING_DEAD, true);
                    who->CastSpell(who, SPELL_CLIENT_SCENE_TELEPORT, true);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_shadowmoon_nerzhul_introAI (creature);
    }
};

//76518
class npc_nerzhul_ritual_bones : public CreatureScript
{
public:
    npc_nerzhul_ritual_bones() : CreatureScript("npc_nerzhul_ritual_bones") { }

    struct npc_nerzhul_ritual_bonesAI : public ScriptedAI
    {
        npc_nerzhul_ritual_bonesAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetWalk(true);
        }

        EventMap events;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(SPELL_RITUAL_OF_BONES_VISUAL);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->RemoveAllAuras();
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == 1)
            {
                DoCast(SPELL_RITUAL_OF_BONES_DMG_FIN);
                events.ScheduleEvent(EVENT_2, 5000);
            }
        }

        void DoAction(int32 const action)
        {
            if (action == 1)
                events.ScheduleEvent(EVENT_1, 1000);
            if (action == 2)
                events.ScheduleEvent(EVENT_1, 4000);
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_RITUAL_OF_BONES);
                        float x, y, z;
                        me->GetClosePoint(x, y, z, me->GetObjectSize(), 68.0f);
                        me->GetMotionMaster()->MovePoint(1, x, y, z);
                        break;
                    case EVENT_2:
                        me->RemoveAreaObject(154462);
                        me->DespawnOrUnsummon();
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_nerzhul_ritual_bonesAI (creature);
    }
};

//154353
class spell_omen_of_death : public SpellScriptLoader
{
    public:
        spell_omen_of_death() : SpellScriptLoader("spell_omen_of_death") { }

        class spell_omen_of_death_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_omen_of_death_SpellScript);

            void DealDamage()
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;

                float distance = caster->GetExactDist2d(target);

                if (distance >= 0 && distance <= 60)
                    SetHitDamage(GetHitDamage() * ((MAX_DIST - distance) / 100));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_omen_of_death_SpellScript::DealDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_omen_of_death_SpellScript();
        }
};

void AddSC_boss_nerzhul()
{
    new boss_nerzhul();
    new npc_shadowmoon_nerzhul_intro();
    new npc_nerzhul_ritual_bones();
    new spell_omen_of_death();
}
