/*
    http://epicwow.com/
    Dungeon : The Everbloom 100
    Encounter: Witherbark
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_everbloom.h"

enum Says
{
    SAY_AGGRO           = 0,
    SAY_BRITTLE_BARK    = 1,
    SAY_REVIV           = 2,
    SAY_WARN_REVIV      = 3,
    SAY_DEATH           = 4,
    SAY_KILL_PLAYER     = 5,
};

enum Spells
{
    SPELL_PETRIFIED_BARK         = 164713,
    SPELL_CANCEL_PETRIFIED_BARK  = 164719,
    SPELL_AGITATED_WATER         = 177733,
    SPELL_SUM_NOXIOUS_VINES      = 164712,
    SPELL_PARCHED_GASP           = 164357,
    SPELL_BRITTLE_BARK           = 164275,
    SPELL_CANCEL_BRITTLE_BARK    = 164718,
    SPELL_SUM_AQUEOUS_GLOBULE    = 164437,

    //Aqueous intro              
    SPELL_GLOBULE_COSMETICS      = 164391,
    SPELL_COSMETIC_ROOTS         = 164715,
    SPELL_ENERGIZE               = 164438,
    SPELL_AQUEOUS_BURST          = 164990,

    //Aqueous                    
    SPELL_COSMETIC_SWIRL_STATE   = 164988,
    SPELL_JUMP_FROM_WATER        = 164434,
    SPELL_AQUEOUS_BURST_DMG      = 164538,

    //Unchecked Growth
    SPELL_NOXIOUS_VINES          = 164302,
    SPELL_NOXIOUS_VINES_VISUAL   = 164296,
    SPELL_NOXIOUS_CANCEL_AURA    = 164375,
    SPELL_NOXIOUS_VINES_AT       = 164290,
    SPELL_NOXIOUS_CANCEL_AURA_AT = 164554,
    SPELL_UNCHECKED_GROWTH       = 164556,
};

enum eEvents
{
    EVENT_PARCHED_GASP      = 1,
    EVENT_AGITATED_WATER    = 2,
    EVENT_SUM_AQUEOUS       = 3,
    EVENT_SUM_NOXIOUS_VINES = 4,
};

enum Stage
{
    STAGE_BATTLE    = 0,
    STAGE_AQUEOUS   = 1,
};

Position const aquaPos[3] =
{
    {403.84f, 1628.38f, 88.06f},
    {439.88f, 1609.58f, 88.70f},
    {410.45f, 1588.57f, 88.07f},
};

Position const addsPos[9] =
{
                                    //1
    {398.96f, 1634.14f, 87.77f, 5.41f}, //81820
    {402.14f, 1620.07f, 88.45f, 1.36f}, //81819
    {412.57f, 1629.21f, 88.67f, 3.23f}, //81819
                                    //2
    {447.00f, 1606.71f, 88.90f, 2.86f}, //81820
    {438.13f, 1617.35f, 88.14f, 4.93f}, //81819
    {433.75f, 1605.23f, 88.47f, 0.61f}, //81819
                                    //3
    {407.83f, 1581.26f, 87.89f, 1.22f}, //81820
    {416.87f, 1590.81f, 88.25f, 3.47f}, //81819
    {405.34f, 1595.57f, 88.65f, 5.34f}, //81819
};

Position const aquaSummPos[12] =
{
    {458.76f, 1648.69f, 86.56f},
    {434.33f, 1661.59f, 86.56f},
    {474.81f, 1638.34f, 86.56f},
    {422.24f, 1652.27f, 86.56f},
    {443.61f, 1649.08f, 86.56f},
    {404.96f, 1654.05f, 86.56f},
    {366.53f, 1614.20f, 86.56f},
    {376.22f, 1636.73f, 86.56f},
    {369.49f, 1628.34f, 86.56f},
    {387.49f, 1650.12f, 86.56f},
    {362.70f, 1599.98f, 86.56f},
    {349.98f, 1594.23f, 86.56f},
};

class boss_witherbark : public CreatureScript
{
public:
    boss_witherbark() : CreatureScript("boss_witherbark") { }

    struct boss_witherbarkAI : public BossAI
    {
        boss_witherbarkAI(Creature* creature) : BossAI(creature, DATA_WITHERBARK), summons(me)
        {
            for (uint8 i = 0; i < 3; i++)
                me->SummonCreature(NPC_AQUEOUS_GLOBULE_TRIGGER, aquaPos[i]);

            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            DoCast(me, SPELL_PETRIFIED_BARK, true);
            intro = true;
        }

        SummonList summons;
        bool intro;
        uint8 Stage;
        uint8 power;
        uint8 checkPower;
        uint8 aquaRand;
        uint16 tickPower_Timer;

        void Reset()
        {
            events.Reset();
            _Reset();

            Stage = 0;
            checkPower = 0;
            tickPower_Timer = 1000;
            if (!intro)
            {
                me->RemoveAllAuras();
                me->SetPower(POWER_MANA, 100);
                me->SetReactState(REACT_AGGRESSIVE);
                me->GetMotionMaster()->MoveTargetedHome();
                summons.DespawnAll();
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);
            _EnterCombat();

            Stage = STAGE_BATTLE;

            events.ScheduleEvent(EVENT_PARCHED_GASP, 8000); //29:47
            events.ScheduleEvent(EVENT_AGITATED_WATER, 5000);
            events.ScheduleEvent(EVENT_SUM_NOXIOUS_VINES, 5000);
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

            Talk(SAY_KILL_PLAYER);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            switch (summon->GetEntry())
            {
                case NPC_AQUEOUS_GLOBULE:
                    summon->GetMotionMaster()->MovePoint(1, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                    break;
            }
        }

        void SpellHit(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_ENERGIZE)
                if (me->GetPower(POWER_MANA) >= 100)
                {
                    if (intro)
                    {
                        intro = false;
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        DoCast(me, SPELL_CANCEL_PETRIFIED_BARK, true);
                    }
                    else if (Stage != STAGE_BATTLE)
                    {
                        Stage = STAGE_BATTLE;
                        Talk(SAY_REVIV);
                        Talk(SAY_WARN_REVIV);
                        me->SetReactState(REACT_AGGRESSIVE);
                        DoCast(me, SPELL_CANCEL_BRITTLE_BARK, true);
                        summons.DespawnEntry(NPC_AQUEOUS_GLOBULE);
                    }
                }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (Stage != STAGE_AQUEOUS)
                if (tickPower_Timer <= diff)
                {
                    tickPower_Timer = 1000;
                    power = me->GetPower(POWER_MANA);
                    if (power > 0)
                    {
                        if (checkPower > 0)
                        {
                            checkPower--;
                            me->SetPower(POWER_MANA, power - 3);
                        }
                        else
                        {
                            checkPower = 2;
                            me->SetPower(POWER_MANA, power - 4);
                        }
                    }
                    if (power <= 0)
                    {
                        Talk(SAY_BRITTLE_BARK);
                        Stage = STAGE_AQUEOUS;
                        DoStopAttack();
                        DoCast(me, SPELL_BRITTLE_BARK, true);
                        events.RescheduleEvent(EVENT_SUM_AQUEOUS, 1000, 0, STAGE_AQUEOUS);
                    }
                    if (me->GetDistance(me->GetHomePosition()) >= 45.0f)
                    {
                        EnterEvadeMode();
                        return;
                    }
                }
                else tickPower_Timer -=diff;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_PARCHED_GASP:
                        if (!Stage)
                            DoCast(SPELL_PARCHED_GASP);
                        events.ScheduleEvent(EVENT_PARCHED_GASP, 12000);
                        break;
                    case EVENT_AGITATED_WATER:
                        DoCast(me, SPELL_AGITATED_WATER, true);
                        events.ScheduleEvent(EVENT_AGITATED_WATER, 6000);
                        break;
                    case EVENT_SUM_AQUEOUS:
                        if (aquaRand = urand(0,3))
                            me->CastSpell(aquaSummPos[aquaRand], SPELL_SUM_AQUEOUS_GLOBULE, true);
                        if (aquaRand = urand(4,7))
                            me->CastSpell(aquaSummPos[aquaRand], SPELL_SUM_AQUEOUS_GLOBULE, true);
                        if (aquaRand = urand(8,11))
                            me->CastSpell(aquaSummPos[aquaRand], SPELL_SUM_AQUEOUS_GLOBULE, true);
                        events.RescheduleEvent(EVENT_SUM_AQUEOUS, 15000, 0, STAGE_AQUEOUS);
                        break;
                    case EVENT_SUM_NOXIOUS_VINES:
                        DoCast(me, SPELL_SUM_NOXIOUS_VINES);
                        events.ScheduleEvent(EVENT_SUM_NOXIOUS_VINES, 11000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_witherbarkAI (creature);
    }
};

//81821
class npc_witherbark_aqueous_globule : public CreatureScript
{
public:
    npc_witherbark_aqueous_globule() : CreatureScript("npc_witherbark_aqueous_globule") { }

    struct npc_witherbark_aqueous_globuleAI : public ScriptedAI
    {
        npc_witherbark_aqueous_globuleAI(Creature* creature) : ScriptedAI(creature) 
        {
            DoCast(SPELL_GLOBULE_COSMETICS);
            DoCast(SPELL_COSMETIC_ROOTS);
        }

        uint8 sDiedCount;

        void Reset() 
        {
            for (uint8 i = 0; i < 3; ++i)
                if (me->GetDistance(aquaPos[i]) < 5.0f)
                {
                    switch (i)
                    {
                        case 0:
                            me->SummonCreature(NPC_EVERBLOOM_MENDER, addsPos[0]);
                            me->SummonCreature(NPC_EVERBLOOM_NATURALIST, addsPos[1]);
                            me->SummonCreature(NPC_EVERBLOOM_NATURALIST, addsPos[2]);
                            break;
                        case 1:
                            me->SummonCreature(NPC_EVERBLOOM_MENDER, addsPos[3]);
                            me->SummonCreature(NPC_EVERBLOOM_NATURALIST, addsPos[4]);
                            me->SummonCreature(NPC_EVERBLOOM_NATURALIST, addsPos[5]);
                            break;
                        case 2:
                            me->SummonCreature(NPC_EVERBLOOM_MENDER, addsPos[6]);
                            me->SummonCreature(NPC_EVERBLOOM_NATURALIST, addsPos[7]);
                            me->SummonCreature(NPC_EVERBLOOM_NATURALIST, addsPos[8]);
                            break;
                    }
                }

            sDiedCount = 0;
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            sDiedCount++;
            if (sDiedCount == 3)
            {
                me->RemoveAurasDueToSpell(SPELL_COSMETIC_ROOTS);
                me->GetMotionMaster()->MovePoint(1, 417.79f, 1615.38f, 89.37f);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == 1)
            {
                DoCast(me, SPELL_ENERGIZE, true);
                DoCast(me, SPELL_AQUEOUS_BURST, true);

                me->DespawnOrUnsummon(500);
            }
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_witherbark_aqueous_globuleAI (creature);
    }
};

//81638
class npc_witherbark_aqueous_globule_follower : public CreatureScript
{
public:
    npc_witherbark_aqueous_globule_follower() : CreatureScript("npc_witherbark_aqueous_globule_follower") { }

    struct npc_witherbark_aqueous_globule_followerAI : public ScriptedAI
    {
        npc_witherbark_aqueous_globule_followerAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
            DoCast(SPELL_GLOBULE_COSMETICS);
            DoCast(me, SPELL_COSMETIC_SWIRL_STATE, true);
            DoCast(me, SPELL_JUMP_FROM_WATER, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        }

        void Reset() {}

        void JustDied(Unit* /*killer*/)
        {
            me->CastSpell(me, SPELL_AQUEOUS_BURST_DMG, true);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == 1)
            {
                me->CastSpell(me, SPELL_AQUEOUS_BURST_DMG, true);
                me->CastSpell(me, SPELL_ENERGIZE, true);
                me->DespawnOrUnsummon();
            }
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_witherbark_aqueous_globule_followerAI (creature);
    }
};

//81564
class npc_witherbark_unchecked_growth : public CreatureScript
{
public:
    npc_witherbark_unchecked_growth() : CreatureScript("npc_witherbark_unchecked_growth") { }

    struct npc_witherbark_unchecked_growthAI : public ScriptedAI
    {
        npc_witherbark_unchecked_growthAI(Creature* creature) : ScriptedAI(creature) 
        {
            //me->SetReactState(REACT_PASSIVE);
        }

        bool vineComplete;
        uint16 checkTimer;
        Unit* plr;

        void Reset() 
        {
            plr = NULL;
        }

        void IsSummonedBy(Unit* summoner)
        {
            vineComplete = false;
            checkTimer = 1000;
            DoZoneInCombat(me, 100.0f);
            DoCast(me, SPELL_NOXIOUS_VINES_VISUAL, true);
            DoCast(me, SPELL_NOXIOUS_VINES, true);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != CHASE_MOTION_TYPE)
                return;

            if (!vineComplete && me->GetDistance(plr) < 3.0f)
                Vine();
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetEntry() != NPC_AQUEOUS_GLOBULE)
                return;

            if (vineComplete && me->GetDistance(who) < 5.0f)
            {
                DoCast(me, SPELL_UNCHECKED_GROWTH, true);
                me->Kill(who);
                me->DespawnOrUnsummon();
            }
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_NOXIOUS_VINES)
            {
                AttackStart(target);
                me->AddThreat(target, 10000.0f);
                plr = target;
            }
        }

        void Vine()
        {
            vineComplete = true;
            me->AttackStop();
            me->GetMotionMaster()->Clear();
            me->RemoveAurasDueToSpell(SPELL_NOXIOUS_VINES);
            DoCast(me, SPELL_NOXIOUS_CANCEL_AURA, true);
            DoCast(me, SPELL_NOXIOUS_VINES_AT);
        }

        void UpdateAI(uint32 diff) 
        {
            if (!UpdateVictim())
                return;

            if (!vineComplete && !me->HasAura(SPELL_NOXIOUS_VINES_VISUAL))
            {
                if (checkTimer <= diff)
                {
                    Vine();
                }
                else 
                    checkTimer -=diff;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_witherbark_unchecked_growthAI(creature);
    }
};

void AddSC_boss_witherbark()
{
    new boss_witherbark();
    new npc_witherbark_aqueous_globule();
    new npc_witherbark_aqueous_globule_follower();
    new npc_witherbark_unchecked_growth();
}
