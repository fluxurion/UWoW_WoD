/*
    http://epicwow.com/
    Dungeon : Upper Blackrock Spire 90-100
    Encounter: Commander Tharbek
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "upper_blackrock_spire.h"

#define MIN_X 111.0f
#define MAX_X 194.0f
#define MIN_Y -454.0f   
#define MAX_Y -385.0f
#define MIN_Z 100.0f
#define MAX_Z 120.0f

enum Says
{
    SAY_DEATH           = 0,
};

enum Spells
{
    //Tharbek
    SPELL_DIALOG_1              = 174129,
    SPELL_DIALOG_2              = 174138,
    SPELL_DIALOG_3              = 174142,
    SPELL_IRON_RAGE             = 161985,
    SPELL_IRON_REAVER           = 161987,
    SPELL_IMBUED_IRON_AXE       = 162085,
    SPELL_IMBUED_IRON_AXE_AURA  = 162092,
    SPELL_IRON_AXE              = 161765,
    //Skyreaver
    SPELL_RIDE_VEHICLE          = 43671,
    SPELL_NOXIOUS_SPIT          = 161811,
    SPELL_INCINERATING_BREATH   = 161883,
};

enum eEvents
{
    EVENT_SUMMON             = 1,
    EVENT_SUMMON_2,
    EVENT_SUMMON_3,
    EVENT_SUMMON_4,
    EVENT_SUMMONS_ATTACK     = 5,
    EVENT_IRON_REAVER        = 6,
    EVENT_IMBUED_AXE         = 7,
    EVENT_IRON_AXE           = 8,
    //Skyreaver
    EVENT_NOXIOUS_SPIT       = 1,
    EVENT_BREATH             = 2,
    //Other
    EVENT_RANDOM_POS         = 1,
};

class boss_commander_tharbek : public CreatureScript
{
public:
    boss_commander_tharbek() : CreatureScript("boss_commander_tharbek") { }

    struct boss_commander_tharbekAI : public BossAI
    {
        boss_commander_tharbekAI(Creature* creature) : BossAI(creature, DATA_THARBEK), summons(me) {}

        SummonList summons;

        ObjectGuid zaelaGUID;
        uint32 checkTimer;
        uint8 summonCount;
        uint8 waveCount;
        bool find;
        bool rage;

        void Reset()
        {
            events.Reset();
            _Reset();
            summons.DespawnAll();

            checkTimer = 1000;
            summonCount = 0;
            waveCount = 0;
            rage = false;

            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);

            me->SummonCreature(NPC_WARLORD_ZAELA, tharbekAdds[12]);
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();

            events.ScheduleEvent(EVENT_SUMMON, 2000); //Aggro 04:40. Attack 04:50
            DoCast(SPELL_DIALOG_1);
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
            DespawnOnRespawn(FIVE_SECONDS);
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();

            summons.DespawnEntry(NPC_IRON_AXE_STALKER);
            if (Creature* zaela = me->GetCreature(*me, zaelaGUID))
                zaela->DespawnOrUnsummon();
        }

        void JustSummoned(Creature* summon)
        {
            if (summon->GetEntry() == NPC_WARLORD_ZAELA)
            {
                summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
                summon->SetReactState(REACT_PASSIVE);
                zaelaGUID = summon->GetGUID();
                return;
            }

            summons.Summon(summon);
            summons.SetReactState(REACT_PASSIVE);

            if (waveCount > 2)
                return;

            summonCount++;
        }
        
        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            if (waveCount > 2)
                return;

            summonCount--;
            if (summonCount == 0)
            {
                waveCount++;
                events.ScheduleEvent(EVENT_SUMMON + waveCount, 2000);
            }
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case 1:
                    events.ScheduleEvent(EVENT_IRON_REAVER, 10000);
                    events.ScheduleEvent(EVENT_IRON_AXE, 24000);
                    break;
                case 2:
                    events.ScheduleEvent(EVENT_IMBUED_AXE, 18000);
                    break;
            }
        }

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            if (me->HealthBelowPct(70) && !rage)
            {
                rage = true;
                DoCast(me, SPELL_IRON_RAGE, true);
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

                    if (plr->GetPositionX() > MIN_X && plr->GetPositionX() < MAX_X &&
                        plr->GetPositionY() > MIN_Y && plr->GetPositionY() < MAX_Y &&
                        plr->GetPositionZ() > MIN_Z && plr->GetPositionZ() < MAX_Z)
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
                checkTimer = 1000;
                CheckPlrInBattleZone();
            }
            else 
                checkTimer -=diff;

            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SUMMON:
                        for (uint8 i = 0; i < 2; i++)
                        {
                            me->SummonCreature(NPC_BLACK_IRON_LEADBELCHER, tharbekAdds[i]);
                            me->SummonCreature(NPC_RAGEMAW_WORG, tharbekAdds[i+2]);
                        }
                        for (uint8 i = 4; i < 12; i++)
                            me->SummonCreature(NPC_VILEMAW_HATCHLING, tharbekAdds[i]);
                        events.ScheduleEvent(EVENT_SUMMONS_ATTACK, 1000);
                        break;
                    case EVENT_SUMMON_2:
                        DoCast(SPELL_DIALOG_2);
                        for (uint8 i = 0; i < 2; i++)
                        {
                            me->SummonCreature(NPC_BLACK_IRON_BERSERKER, tharbekAdds[i]);
                            me->SummonCreature(NPC_BLACK_IRON_DREADWEAVER, tharbekAdds[i+2]);
                        }
                        for (uint8 i = 4; i < 12; i++)
                            me->SummonCreature(NPC_VILEMAW_HATCHLING, tharbekAdds[i]);
                        events.ScheduleEvent(EVENT_SUMMONS_ATTACK, 1000);
                        break;
                    case EVENT_SUMMON_3:
                        DoCast(SPELL_DIALOG_3);
                        me->SummonCreature(NPC_BLACK_IRON_ENGINEER, tharbekAdds[0]);
                        for (uint8 i = 1; i < 3; i++)
                        {
                            me->SummonCreature(NPC_BLACK_IRON_SUMMONER, tharbekAdds[i]);
                        }
                        for (uint8 i = 3; i < 12; i++)
                        {
                            me->SummonCreature(NPC_VILEMAW_HATCHLING, tharbekAdds[i]);
                        }
                        events.ScheduleEvent(EVENT_SUMMONS_ATTACK, 1000);
                        break;
                    case EVENT_SUMMON_4:
                        me->SummonCreature(NPC_IRONBARB_SKYREAVER, tharbekAdds[0]);
                        instance->SetData(DATA_THARBEK_OPENGATE, IN_PROGRESS);
                        break;
                    case EVENT_SUMMONS_ATTACK:
                        instance->SetData(DATA_THARBEK_OPENGATE, IN_PROGRESS);
                        summons.SetReactState(REACT_AGGRESSIVE);
                        summons.DoZoneInCombat();
                        break;
                    case EVENT_IRON_REAVER:
                        DoCast(SPELL_IRON_REAVER);
                        events.ScheduleEvent(EVENT_IRON_REAVER, 20000);
                        break;
                    case EVENT_IMBUED_AXE:
                        DoCast(SPELL_IMBUED_IRON_AXE);
                        events.ScheduleEvent(EVENT_IMBUED_AXE, 30000);
                        break;
                    case EVENT_IRON_AXE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0, true))
                            DoCast(target, SPELL_IRON_AXE);
                        events.ScheduleEvent(EVENT_IRON_AXE, 10000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_commander_tharbekAI (creature);
    }
};

//80098
class npc_ironbarb_skyreaver : public CreatureScript
{
public:
    npc_ironbarb_skyreaver() : CreatureScript("npc_ironbarb_skyreaver") { }

    struct npc_ironbarb_skyreaverAI : public ScriptedAI
    {
        npc_ironbarb_skyreaverAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            exitPassenger = false;
            me->SetSpeed(MOVE_FLIGHT, 2.0f);
        }

        InstanceScript* instance;
        EventMap events;

        Unit* tharbek = NULL;
        uint8 healthPct;
        bool exitPassenger;

        void Reset()
        {
            events.Reset();
            if (me->GetMap()->GetDifficultyID() != DIFFICULTY_NORMAL)
                healthPct = 50;
            else
                healthPct = 30;
        }

        void EnterCombat(Unit* /*who*/) {}

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            if (me->HealthBelowPct(healthPct) && !exitPassenger)
            {
                exitPassenger = true;
                me->GetVehicleKit()->RemoveAllPassengers();

                if (tharbek)
                {
                    tharbek->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
                    tharbek->ToCreature()->AI()->DoAction(1);
                }
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != WAYPOINT_MOTION_TYPE)
                return;

            switch (id)
            {
                case 1:
                    tharbek->CastSpell(me, SPELL_RIDE_VEHICLE, true);
                    break;
                case 6:
                    me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_HOVER);
                    me->SetReactState(REACT_AGGRESSIVE);
                    tharbek->ToCreature()->SetReactState(REACT_AGGRESSIVE);
                    tharbek->ToCreature()->AI()->DoAction(2);
                    events.ScheduleEvent(EVENT_NOXIOUS_SPIT, 2000);
                    events.ScheduleEvent(EVENT_BREATH, 8000);
                    break;
            }
        }

        void IsSummonedBy(Unit* summoner)
        {
            if (!me->isInCombat())
                DoZoneInCombat();

            tharbek = summoner;

            me->GetMotionMaster()->MovePath((me->GetEntry() * 100), false);
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
                    case EVENT_NOXIOUS_SPIT:
                        DoCast(SPELL_NOXIOUS_SPIT);
                        events.ScheduleEvent(EVENT_NOXIOUS_SPIT, 14000);
                        break;
                    case EVENT_BREATH:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                        {
                            DoStopAttack();
                            DoCast(pTarget, SPELL_INCINERATING_BREATH);
                            me->SetFacingToObject(pTarget);
                        }
                        events.ScheduleEvent(EVENT_BREATH, 12000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ironbarb_skyreaverAI (creature);
    }
};

//80307
class npc_imbued_iron_axe_stalker : public CreatureScript
{
public:
    npc_imbued_iron_axe_stalker() : CreatureScript("npc_imbued_iron_axe_stalker") { }

    struct npc_imbued_iron_axe_stalkerAI : public ScriptedAI
    {
        npc_imbued_iron_axe_stalkerAI(Creature* creature) : ScriptedAI(creature) 
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;

        void Reset()
        {
            DoCast(me, SPELL_IMBUED_IRON_AXE_AURA, true);
        }

        void IsSummonedBy(Unit* summoner)
        {
            events.ScheduleEvent(EVENT_RANDOM_POS, 1000);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == 1)
                events.ScheduleEvent(EVENT_RANDOM_POS, 1000);
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_RANDOM_POS:
                    {
                        float x = frand(MIN_X, MAX_X);
                        float y = frand(MIN_Y, MAX_Y);
                        me->GetMotionMaster()->MovePoint(1, x, y, me->GetPositionZ());
                        break;
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_imbued_iron_axe_stalkerAI (creature);
    }
};

void AddSC_boss_commander_tharbek()
{
    new boss_commander_tharbek();
    new npc_ironbarb_skyreaver();
    new npc_imbued_iron_axe_stalker();
}
