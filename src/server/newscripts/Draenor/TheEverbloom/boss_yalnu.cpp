/*
    http://epicwow.com/
    Dungeon : The Everbloom 100
    Encounter: Yalnu
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_everbloom.h"

enum Says
{
    SAY_WARN_GENESIS    = 0,

    SAY_LADY_AGGRO      = 1,
};

enum Spells
{
    SPELL_COLOSSAL_BLOW         = 169756,
    SPELL_COLOSSAL_BLOW_DMG     = 169179,

    SPELL_ENTANGLEMENT          = 169251,
    SPELL_ENTANGLEMENT_SELECT   = 169247,
    SPELL_ENTANGLEMENT_SUM      = 169237,
    SPELL_ENTANGLEMENT_VISUAL   = 169192,
    SPELL_ENTANGLEMENT_STUN     = 169240,

    SPELL_ENTANGLEMENT_PLR      = 170124,
    SPELL_ENTANGLEMENT_SELECT_P = 170126,
    SPELL_ENTANGLEMENT_SUM_P    = 170127,
    SPELL_ENTANGLEMENT_STUN_P   = 170132,

    SPELL_DISARM                = 169320,
    SPELL_GERMINATE_ARBORBLADE  = 169265,

    SPELL_FONT_OF_LIFE          = 169120,
    SPELL_FONT_OF_LIFE_VISUAL   = 169123,

    SPELL_GENESIS               = 169125,
    SPELL_GENESIS_CH            = 169613,
    SPELL_GENESIS_SUBMERGED     = 169126,
    SPELL_GENESIS_VISUAL        = 173539,
    SPELL_GENESIS_AT            = 173537,
    SPELL_SUBMERGED             = 175123,

    //Feral Lasher
    SPELL_LASHER_VENOM          = 173563,

    //Mages
    SPELL_FIREBALL              = 168666,
    SPELL_FLAMESTRIKE           = 169094,

    SPELL_FROSTBOLT             = 170028,
    SPELL_ICE_COMET             = 170032,

    SPELL_ARCANE_BLAST          = 170035,
    SPELL_ARCANE_ORB            = 170040,
};

enum eEvents
{
    EVENT_C_BLOW_1          = 1,
    EVENT_C_BLOW_2          = 2,
    EVENT_C_BLOW_3          = 3,
    EVENT_ENTANGLEMENT      = 4,
    EVENT_ENTANGLEMENT_PLR  = 5,
    EVENT_FONT_OF_LIFE      = 6,
    EVENT_GENESIS           = 7,
};

enum MageClass
{
    MAGE_FIRE    = 1,
    MAGE_FROST   = 2,
    MAGE_ARCANE  = 3,
};

Position const magePos[7] =
{
    {947.38f, -1235.11f, 181.25f, 0.68f},
    {966.46f, -1214.60f, 181.25f, 4.11f},
    {960.97f, -1208.64f, 181.24f, 4.22f},
    {970.09f, -1236.66f, 181.25f, 2.47f},
    {959.63f, -1238.97f, 181.25f, 1.62f},
    {942.67f, -1212.62f, 181.35f, 5.44f},
    {976.43f, -1223.22f, 181.33f, 3.08f},
};

class boss_yalnu : public CreatureScript
{
public:
    boss_yalnu() : CreatureScript("boss_yalnu") { }

    struct boss_yalnuAI : public BossAI
    {
        boss_yalnuAI(Creature* creature) : BossAI(creature, DATA_YALNU), summons(me) {}

        SummonList summons;
        bool find;
        uint16 checkTimer;

        void Reset()
        {
            events.Reset();
            _Reset();

            summons.DespawnAll();
            me->SetReactState(REACT_AGGRESSIVE);
            checkTimer = 2000;
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();

            for (uint8 i = 0; i < 6; i++)
                me->SummonCreature(NPC_KIRIN_TOR_MAGE, magePos[i]);

            me->SummonCreature(NPC_LADY_BAIHU, magePos[6]);

            events.ScheduleEvent(EVENT_C_BLOW_1, 5000); //45:47
            events.ScheduleEvent(EVENT_ENTANGLEMENT, 10000);
            if (me->GetMap()->GetDifficultyID() != DIFFICULTY_NORMAL)
                events.ScheduleEvent(EVENT_ENTANGLEMENT_PLR, 50000);
            events.ScheduleEvent(EVENT_FONT_OF_LIFE, 12000);
            events.ScheduleEvent(EVENT_GENESIS, 30000);
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            _JustDied();
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            if (attacker->ToCreature() && !attacker->ToCreature()->isPet())
                damage /= 2;
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            switch (spell->Id)
            {
                case SPELL_ENTANGLEMENT_SELECT:
                    DoCast(me, SPELL_DISARM, true);
                    DoCast(SPELL_GERMINATE_ARBORBLADE);
                    break;
                case SPELL_ENTANGLEMENT_SELECT_P:
                    target->CastSpell(target, SPELL_ENTANGLEMENT_SUM_P, true);
                    DoCast(me, SPELL_DISARM, true);
                    DoCast(SPELL_GERMINATE_ARBORBLADE);
                    break;
            }
        }

        void CheckPlrInBattleZone()
        {
            find = false;
            const Map::PlayerList &PlayerList = me->GetMap()->GetPlayers();
            if (PlayerList.isEmpty())
                return;
            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                if (Player* plr = i->getSource())
                {
                    if (!plr->isAlive() || plr->isGameMaster())
                        break;

                    if (plr->GetAreaId() == 7330)
                    {
                        find = true;
                    }
                }
            if (find && !me->isInCombat())
            {
                DoZoneInCombat();
            }
            else if(!find && me->isInCombat())
            {
                EnterEvadeMode();
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (checkTimer <= diff)
            {
                checkTimer = 2000;
                CheckPlrInBattleZone();
            }
            else 
                checkTimer -=diff;

            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_C_BLOW_1:
                        DoStopAttack();
                        DoCast(SPELL_COLOSSAL_BLOW);
                        events.ScheduleEvent(EVENT_C_BLOW_1, 20000);
                        events.ScheduleEvent(EVENT_C_BLOW_2, 100);
                        break;
                    case EVENT_C_BLOW_2:
                        DoCast(SPELL_COLOSSAL_BLOW_DMG);
                        if (Creature* blow = me->FindNearestCreature(NPC_COLOSSAL_BLOW, 100))
                            me->SetFacingToObject(blow);
                        events.ScheduleEvent(EVENT_C_BLOW_3, 100);
                        break;
                    case EVENT_C_BLOW_3:
                        me->SetReactState(REACT_AGGRESSIVE);
                        break;
                    case EVENT_ENTANGLEMENT:
                    {
                        std::list<Creature*> mageList;
                        me->GetCreatureListWithEntryInGrid(mageList, NPC_KIRIN_TOR_MAGE, 100.0f);
                        if (!mageList.empty())
                        {
                            Trinity::Containers::RandomResizeList(mageList, 1);
                            if (Creature* target = mageList.front())
                                DoCast(target, SPELL_ENTANGLEMENT);
                        }
                        events.ScheduleEvent(EVENT_ENTANGLEMENT, 60000);
                        break;
                    }
                    case EVENT_ENTANGLEMENT_PLR:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            DoCast(pTarget, SPELL_ENTANGLEMENT_PLR);
                        events.ScheduleEvent(EVENT_ENTANGLEMENT_PLR, 60000);
                        break;
                    case EVENT_FONT_OF_LIFE:
                        DoCast(SPELL_FONT_OF_LIFE);
                        events.ScheduleEvent(EVENT_FONT_OF_LIFE, 15000);
                        break;
                    case EVENT_GENESIS:
                        for (uint8 i = 0; i < 22; i++)
                        {
                            float x = frand(930, 983);
                            float y = frand(-1267, -1201);
                            me->CastSpell(x, y, me->GetPositionZ() + 2, SPELL_GENESIS, true);
                        }
                        DoCast(SPELL_GENESIS_CH);
                        Talk(SAY_WARN_GENESIS);
                        events.ScheduleEvent(EVENT_GENESIS, 60000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_yalnuAI (creature);
    }
};

//84329, 84358
class npc_yalnu_kirin_tor_mage : public CreatureScript
{
public:
    npc_yalnu_kirin_tor_mage() : CreatureScript("npc_yalnu_kirin_tor_mage") { }

    struct npc_yalnu_kirin_tor_mageAI : public ScriptedAI
    {
        npc_yalnu_kirin_tor_mageAI(Creature* creature) : ScriptedAI(creature), summons(me) 
        {
            SetCombatMovement(false);
        }

        EventMap events;
        SummonList summons;
        uint8 classMage;

        void Reset() 
        {
            summons.DespawnAll();
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (me->GetEntry() == NPC_LADY_BAIHU)
                Talk(SAY_LADY_AGGRO);

            switch (classMage)
            {
                case MAGE_FIRE:
                    events.ScheduleEvent(EVENT_1, 0); //45:47
                    events.ScheduleEvent(EVENT_2, urand(18, 35) * IN_MILLISECONDS);
                    break;
                case MAGE_FROST:
                    events.ScheduleEvent(EVENT_3, 0); //45:47
                    events.ScheduleEvent(EVENT_4, urand(18, 30) * IN_MILLISECONDS);
                    break;
                case MAGE_ARCANE:
                    events.ScheduleEvent(EVENT_5, 0); //45:47
                    events.ScheduleEvent(EVENT_6, urand(18, 35) * IN_MILLISECONDS);
                    break;
            }
        }

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(142193); //Visual Teleport
            classMage = urand(MAGE_FIRE, MAGE_ARCANE); //1-3
        }

        void SpellHit(Unit* /*caster*/, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_ENTANGLEMENT_SELECT)
                DoCast(me, SPELL_ENTANGLEMENT_SUM, true);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            if (summon->GetEntry() == NPC_ENTANGLEMENT)
            {
                summon->SetReactState(REACT_PASSIVE);
                summon->CastSpell(summon, SPELL_ENTANGLEMENT_VISUAL, true);
                summon->CastSpell(me, SPELL_ENTANGLEMENT_STUN, true);
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            summons.DespawnAll();
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
                    case EVENT_1: //Fireball
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_FIREBALL);
                        events.ScheduleEvent(EVENT_1, 3000);
                        break;
                    case EVENT_2: //Flamestrike
                        DoCast(SPELL_FLAMESTRIKE);
                        events.ScheduleEvent(EVENT_2, urand(18, 35) * IN_MILLISECONDS);
                        break;
                    case EVENT_3: //Frostball
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_FROSTBOLT);
                        events.ScheduleEvent(EVENT_3, 3000);
                        break;
                    case EVENT_4: //Ice Comet
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_ICE_COMET);
                        events.ScheduleEvent(EVENT_4, urand(18, 30) * IN_MILLISECONDS);
                        break;
                    case EVENT_5: //Arcane Blast
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_ARCANE_BLAST);
                        events.ScheduleEvent(EVENT_5, 3000);
                        break;
                    case EVENT_6: //Arcane Orb
                        DoCast(SPELL_ARCANE_ORB);
                        events.ScheduleEvent(EVENT_6, urand(18, 35) * IN_MILLISECONDS);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_yalnu_kirin_tor_mageAI (creature);
    }
};

//85194, 85107
class npc_yalnu_eventer : public CreatureScript
{
public:
    npc_yalnu_eventer() : CreatureScript("npc_yalnu_eventer") { }

    struct npc_yalnu_eventerAI : public ScriptedAI
    {
        npc_yalnu_eventerAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;

        void Reset() 
        {
            events.Reset();
        }

        void IsSummonedBy(Unit* summoner)
        {
            switch (me->GetEntry())
            {
                case NPC_ENTANGLEMENT_PLR:
                    DoCast(me, SPELL_ENTANGLEMENT_VISUAL, true);
                    DoCast(summoner, SPELL_ENTANGLEMENT_STUN_P, true);
                    break;
                case NPC_FONT_LIFE_STALKER:
                    DoCast(me, SPELL_FONT_OF_LIFE_VISUAL, true);
                    events.ScheduleEvent(EVENT_1 + urand(0, 2), 1000);
                    me->DespawnOrUnsummon(3000);
                    break;
            }
        }

        void UpdateAI(uint32 diff) 
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        me->SummonCreature(NPC_VICIOUS_MANDRAGORA, me->GetPositionX() + 5, me->GetPositionY() - 5, me->GetPositionZ() + 2);
                        me->SummonCreature(NPC_VICIOUS_MANDRAGORA, me->GetPositionX() - 5, me->GetPositionY() + 5, me->GetPositionZ() + 2);
                        break;
                    case EVENT_2:
                        me->SummonCreature(NPC_GNARLED_ANCIENT, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        break;
                    case EVENT_3:
                        for (uint8 i = 0; i < 6; i++)
                            me->SummonCreature(NPC_SWIFT_SPROUTLING, me->GetPositionX() + i, me->GetPositionY() - i, me->GetPositionZ() + 2);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_yalnu_eventerAI (creature);
    }
};

//86684
class npc_yalnu_feral_lasher : public CreatureScript
{
public:
    npc_yalnu_feral_lasher() : CreatureScript("npc_yalnu_feral_lasher") { }

    struct npc_yalnu_feral_lasherAI : public ScriptedAI
    {
        npc_yalnu_feral_lasherAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
            activated = false;
        }

        EventMap events;
        bool activated;

        void Reset() 
        {
            events.Reset();
        }

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_SUBMERGED, true);
            DoCast(me, SPELL_GENESIS_AT, true);
        }

        void SpellHit(Unit* /*caster*/, const SpellInfo* spell)
        {
            switch (spell->Id)
            {
                case SPELL_GENESIS_SUBMERGED:
                    me->RemoveAurasDueToSpell(SPELL_SUBMERGED);
                    me->SetReactState(REACT_AGGRESSIVE);
                    DoZoneInCombat(me, 100.0f);
                    events.ScheduleEvent(EVENT_1, 2000);
                    break;
                case SPELL_GENESIS_VISUAL:
                    if (!activated)
                    {
                        activated = true;
                        me->DespawnOrUnsummon(500);
                    }
                    break;
            }
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
                        DoCast(SPELL_LASHER_VENOM);
                        events.ScheduleEvent(EVENT_1, 3000 * urand(1,3));
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_yalnu_feral_lasherAI (creature);
    }
};

//68553
class npc_yalnu_teleporter : public CreatureScript
{
public:
    npc_yalnu_teleporter() : CreatureScript("npc_yalnu_teleporter") { }

    struct npc_yalnu_teleporterAI : public ScriptedAI
    {
        npc_yalnu_teleporterAI(Creature* creature) : ScriptedAI(creature) 
        {
            instance = me->GetInstanceScript();
        }

        InstanceScript* instance;

        void Reset() 
        {
            if (me->GetAreaId() == 7384) //The Everbloom
                DoCast(169147); //Visual Teleport To Stormwind
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetTypeId() != TYPEID_PLAYER || me->GetDistance(who) > 5.0f)
                return;

            switch (me->GetAreaId())
            {
                case 7384: //Soul Area
                    if (instance->GetBossState(DATA_ARCHMAGE_SOL) == DONE)
                        who->ToPlayer()->TeleportTo(1279, 887.16f, -1221.76f, 194.33f, 0.03f); //Teleport to Yalnu
                    break;
            }
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_yalnu_teleporterAI (creature);
    }
};

void AddSC_boss_yalnu()
{
    new boss_yalnu();
    new npc_yalnu_kirin_tor_mage();
    new npc_yalnu_eventer();
    new npc_yalnu_feral_lasher();
    new npc_yalnu_teleporter();
}
