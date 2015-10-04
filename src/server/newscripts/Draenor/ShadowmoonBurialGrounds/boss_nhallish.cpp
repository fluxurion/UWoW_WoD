/*
    http://epicwow.com/
    Dungeon : Shadowmoon Burial Grounds 100
    Encounter: Nhallish
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "shadowmoon_burial_grounds.h"

enum Says
{
    SAY_AGGRO            = 0,
    SAY_DEATH            = 1,
    SAY_INTRO_1          = 2,
    SAY_INTRO_2          = 3,
    SAY_INTRO_3          = 4,
    SAY_KILLED_PLAYER    = 5,
    SAY_VOID_VORTEX      = 6,
    SAY_VOID_DEVASTATION = 7,
    SAY_SOUL_STEAL       = 8,
};

enum Spells
{
    //intro
    SPELL_SHACKLED_SOUL             = 154025, //summon npc 76269
    SPELL_VOID_CHAINS               = 153624, //hit npc 76269

    SPELL_VOID_BLAST                = 152792,
    SPELL_PLANAR_SHIFT              = 153623,
    SPELL_VOID_VORTEX               = 152801, //поправить притягивалку ат и настройки в дате
    SPELL_SOUL_STEAL                = 152962,
    SPELL_TEMPORAL_DISTORTION       = 158382,
    SPELL_TEMPORAL_DISTORTION_STUN  = 158372,
    SPELL_SOUL_STEAL_CHANNEL        = 156755,
    SPELL_TEMPORAL_DISPELL          = 158379,
    SPELL_VOID_DEVASTATION          = 153067,
    SPELL_TEMPORAL_DISTORTION_CHECK = 158432,

    //Possessed Soul
    SPELL_UNORTHODOX_EXISTENCE      = 152976,
    SPELL_SOUL_SHRED                = 152979,
    SPELL_RETURNED_SOUL             = 153033,
    SPELL_MIRROR_IMAGE              = 153493,
    SPELL_RECLAIMING_SOUL_KILL      = 153486,
    SPELL_RECLAIMING_SOUL           = 154921, //???
    SPELL_RECLAIMING_SOUL_REMOVE    = 154925, //spellclick
    SPELL_SOULLESS_SCREAN           = 154947,
    SPELL_FEIGN_DEATH               = 114371,

    SPELL_PHASE_SHIFT_1             = 155005,
    SPELL_PHASE_SHIFT_2             = 155006,
    SPELL_PHASE_SHIFT_3             = 155007,
    SPELL_PHASE_SHIFT_4             = 155009,
    SPELL_PHASE_SHIFT_5             = 155010,
    
    SPELL_DEFIILED_BURIAL_SITE      = 153238,
};

enum eEvents
{
    EVENT_VOID_BLAST        = 1,
    EVENT_PLANAR_SHIFT      = 2,
    EVENT_SOUL_STEAL        = 3,
    EVENT_VOID_DEVASTATION  = 4,
};

uint32 phaseSpell[5] =
{
    SPELL_PHASE_SHIFT_1,
    SPELL_PHASE_SHIFT_2,
    SPELL_PHASE_SHIFT_3,
    SPELL_PHASE_SHIFT_4,
    SPELL_PHASE_SHIFT_5,
};

class boss_nhallish : public CreatureScript
{
public:
    boss_nhallish() : CreatureScript("boss_nhallish") { }

    struct boss_nhallishAI : public BossAI
    {
        boss_nhallishAI(Creature* creature) : BossAI(creature, DATA_NHALLISH) {}

        uint16 checkTimer;
        bool soulsEvent;

        void Reset()
        {
            events.Reset();
            _Reset();
            me->SetReactState(REACT_AGGRESSIVE);

            soulsEvent = false;
            checkTimer = 1000;
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);
            _EnterCombat();
            
            events.ScheduleEvent(EVENT_VOID_BLAST, 12000); //36:06
            events.ScheduleEvent(EVENT_PLANAR_SHIFT, 24000);
            events.ScheduleEvent(EVENT_SOUL_STEAL, 34000);
            events.ScheduleEvent(EVENT_VOID_DEVASTATION, 66000);
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
                Talk(SAY_KILLED_PLAYER);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (target->GetTypeId() == TYPEID_PLAYER)
            {
                switch (spell->Id)
                {
                    case SPELL_PLANAR_SHIFT:
                        Talk(SAY_VOID_VORTEX);
                        DoCast(SPELL_VOID_VORTEX);
                        break;
                    case SPELL_SOUL_STEAL:
                        Talk(SAY_SOUL_STEAL);
                        DoCast(me, SPELL_TEMPORAL_DISTORTION, true);
                        DoCast(me, SPELL_TEMPORAL_DISTORTION_STUN, true);
                        soulsEvent = true;
                        break;
                }
            }
            else
            {
                if (spell->Id == SPELL_TEMPORAL_DISPELL)
                    target->RemoveAurasDueToSpell(SPELL_TEMPORAL_DISTORTION_STUN);
            }
        }

        void CheckPlrSoul()
        {
            const Map::PlayerList &PlayerList = me->GetMap()->GetPlayers();
            if (PlayerList.isEmpty())
                return;
            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                if (Player* plr = i->getSource())
                {
                    if (!plr->isAlive() || plr->isGameMaster())
                        return;

                    if (!plr->HasAura(SPELL_SOULLESS_SCREAN))
                    {
                        soulsEvent = false;
                        DoCast(me, SPELL_TEMPORAL_DISPELL, true);
                        me->RemoveAurasDueToSpell(SPELL_TEMPORAL_DISTORTION);
                        me->SetReactState(REACT_AGGRESSIVE);
                    }
                }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (soulsEvent)
            {
                if (checkTimer <= diff)
                {
                    CheckPlrSoul();
                    
                    if (me->GetDistance(me->GetHomePosition()) >= 30.0f)
                    {
                        EnterEvadeMode();
                        return;
                    }
                    checkTimer = 1000;
                }
                else
                    checkTimer -=diff;
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_VOID_BLAST:
                        DoCast(SPELL_VOID_BLAST);
                        events.ScheduleEvent(EVENT_VOID_BLAST, 24000);
                        break;
                    case EVENT_PLANAR_SHIFT:
                        DoCast(SPELL_PLANAR_SHIFT);
                        events.ScheduleEvent(EVENT_PLANAR_SHIFT, 70000);
                        break;
                    case EVENT_SOUL_STEAL:
                        DoStopAttack();
                        DoCast(SPELL_SOUL_STEAL);
                        events.ScheduleEvent(EVENT_SOUL_STEAL, 70000);
                        break;
                    case EVENT_VOID_DEVASTATION:
                        Talk(SAY_VOID_DEVASTATION);
                        DoCast(SPELL_VOID_DEVASTATION);
                        events.ScheduleEvent(EVENT_VOID_DEVASTATION, 70000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_nhallishAI (creature);
    }
};

//75899
class npc_nhallish_possessed_soul : public CreatureScript
{
public:
    npc_nhallish_possessed_soul() : CreatureScript("npc_nhallish_possessed_soul") { }

    struct npc_nhallish_possessed_soulAI : public ScriptedAI
    {
        npc_nhallish_possessed_soulAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            DoZoneInCombat(me, 50.0f);
            DoCast(me, SPELL_UNORTHODOX_EXISTENCE, true);
            DoCast(summoner, SPELL_MIRROR_IMAGE, true);
            DoCast(summoner, SPELL_SOULLESS_SCREAN, true);
            DoCast(summoner, SPELL_SOUL_SHRED);
            me->GetMotionMaster()->MoveRandom(10.0f);
        }

        void DamageTaken(Unit* who, uint32& damage)
        {
            if (damage >= me->GetHealth())
            {
                if (who->GetEntry() != NPC_POSSESSED_SOUL)
                    damage = 0;

                me->setFaction(35);
                me->RemoveAurasDueToSpell(111232); //mirror image
                DoCast(me, SPELL_FEIGN_DEATH, true);
            }
        }

        void SpellHit(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_RECLAIMING_SOUL_REMOVE)
            {
                DoCast(target, SPELL_RETURNED_SOUL, true);
                DoCast(me, SPELL_RECLAIMING_SOUL_KILL, true);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_nhallish_possessed_soulAI (creature);
    }
};

//75977
class npc_nhallish_defiled_burial_site : public CreatureScript
{
public:
    npc_nhallish_defiled_burial_site() : CreatureScript("npc_nhallish_defiled_burial_site") { }

    struct npc_nhallish_defiled_burial_siteAI : public ScriptedAI
    {
        npc_nhallish_defiled_burial_siteAI(Creature* creature) : ScriptedAI(creature) 
        {
            activate = false;
        }

        bool activate;

        void Reset() {}
        
        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
            {
                if (me->IsWithinDistInMap(who, 2.0f) && !activate)
                {
                    activate = true;
                    DoCast(SPELL_DEFIILED_BURIAL_SITE);
                    me->DespawnOrUnsummon(5000);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_nhallish_defiled_burial_siteAI (creature);
    }
};

//153493
class spell_nhallish_mirror_image : public SpellScriptLoader
{
    public:
        spell_nhallish_mirror_image() : SpellScriptLoader("spell_nhallish_mirror_image") { }

        class spell_nhallish_mirror_image_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_nhallish_mirror_image_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetHitUnit()->CastSpell(GetCaster(), GetSpellInfo()->Effects[EFFECT_0].BasePoints, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_nhallish_mirror_image_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_nhallish_mirror_image_SpellScript();
        }
};

//152962
class spell_nhallish_soul_summon : public SpellScriptLoader
{
    public:
        spell_nhallish_soul_summon() : SpellScriptLoader("spell_nhallish_soul_summon") { }

        class spell_nhallish_soul_summon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_nhallish_soul_summon_SpellScript);

            uint8 soulCount;

            bool Load()
            {
                soulCount = 0;
                return true;
            }

            void HandleScript()
            {
                Player* pCaster = GetHitUnit()->ToPlayer();
                if (!pCaster)
                    return;

                pCaster->CastSpell(pCaster, phaseSpell[soulCount], true);
                soulCount++;
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_nhallish_soul_summon_SpellScript::HandleScript);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_nhallish_soul_summon_SpellScript();
        }
};

void AddSC_boss_nhallish()
{
    new boss_nhallish();
    new npc_nhallish_possessed_soul();
    new npc_nhallish_defiled_burial_site();
    new spell_nhallish_mirror_image();
    new spell_nhallish_soul_summon();
}
