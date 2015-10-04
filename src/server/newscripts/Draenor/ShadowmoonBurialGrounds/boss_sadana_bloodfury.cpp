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
    SAY_DARK_COMMUNION  = 4,
    SAY_DARK_ECLIPSE    = 5,
};

enum Spells
{
    //Sadana
    SPELL_SHADOW_RITUAL_VISUAL    = 152158,
    SPELL_DARK_ECLIPSE_CHECK      = 164710,
    SPELL_DARK_ECLIPSE_HIT_RUNE   = 164705,
    SPELL_DEATHSPIKE              = 153079,
    SPELL_DAGGERFALL_SUMMON       = 153200,
    SPELL_DAGGERFALL_TARGETING    = 153240,
    SPELL_WHISPERS_DARK_STAR      = 153094,
    SPELL_DARK_COMMUNION          = 153153,
    SPELL_DARK_ECLIPSE_VISUAL     = 164685,
    SPELL_DARK_ECLIPSE_CHANNEL    = 164974,
    SPELL_DARK_ECLIPSE_AT         = 164704,
    SPELL_DARK_ECLIPSE_DMG        = 164686,
    SPELL_TELEPORT_TO_HOME        = 155689,
    //Daggerfall
    SPELL_DAGGERFALL_VISUAL       = 153216,
    SPELL_DAGGERFALL_DAMAGE_FALL  = 153225,
    SPELL_DAGGERFALL_DAMAGE_TICK  = 153236,
    //Spirit
    SPELL_INVISIBLE_MAN_TRANSFORM = 152821,
    SPELL_DARK_COMMUNION_HEAL     = 153164,
    SPELL_PURPLE_SHADOWY_DROWN    = 153136,
    SPELL_PURPLE_SHADOWY          = 152311,
    SPELL_FEIGN_DEATH             = 114371,
    //Runes
    SPELL_SHADOW_RUNE_1_AT        = 152684,
    SPELL_SHADOW_RUNE_2_AT        = 152691,
    SPELL_SHADOW_RUNE_3_AT        = 152695,
};

enum eEvents
{
    EVENT_DEATHSPIKE        = 1,
    EVENT_DAGGERFALL_SUMM   = 2,
    EVENT_DARK_STAR         = 3,
    EVENT_DARK_COMMUNION    = 4,
    EVENT_DARK_ECLIPSE_1    = 5,
    EVENT_DARK_ECLIPSE_2    = 6,
    EVENT_DARK_ECLIPSE_3    = 7,

    //Dagger
    EVENT_MOVE_FALL_1       = 1,
    EVENT_MOVE_FALL_2       = 2,
};

Position const spiritPos[4] =
{
    {1795.85f,  2.427f, 282.14f, 4.67f},
    {1772.43f, -44.29f, 281.39f, 0.74f},
    {1812.56f, -49.17f, 280.96f, 1.99f},
    {1795.8f,  -26.65f, 261.39f, 3.14f},
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
            me->SetReactState(REACT_AGGRESSIVE);

            for (uint8 i = 0; i < 3; i++)
                me->SummonCreature(NPC_DEFILED_SPIRIT, spiritPos[i]);

            me->SummonCreature(NPC_DARK_ECLIPSE, spiritPos[3]);
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);
            _EnterCombat();

            me->RemoveAurasDueToSpell(SPELL_SHADOW_RITUAL_VISUAL);
            DoCast(SPELL_DARK_ECLIPSE_CHECK);

            events.ScheduleEvent(EVENT_DEATHSPIKE, 10000); //33:12
            events.ScheduleEvent(EVENT_DAGGERFALL_SUMM, 10000);
            events.ScheduleEvent(EVENT_DARK_STAR, 14000);
            events.ScheduleEvent(EVENT_DARK_COMMUNION, 24000);
            events.ScheduleEvent(EVENT_DARK_ECLIPSE_1, 44000);
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

        void MoveInLineOfSight(Unit* who)
        {
            if (Player* player = who->ToPlayer())
            {
                if (me->IsWithinDistInMap(who, 110.0f) && intro)
                {
                    intro = false;
                    Talk(SAY_ENTER_ROOM);
                }
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->GetDistance(me->GetHomePosition()) >= 25.0f)
            {
                EnterEvadeMode();
                return;
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_DEATHSPIKE:
                        DoCast(me, SPELL_DEATHSPIKE, true);
                        break;
                    case EVENT_DAGGERFALL_SUMM:
                        DoCast(SPELL_DAGGERFALL_TARGETING);
                        events.ScheduleEvent(EVENT_DAGGERFALL_SUMM, 28000);
                        break;
                    case EVENT_DARK_STAR:
                        DoCast(SPELL_WHISPERS_DARK_STAR);
                        events.ScheduleEvent(EVENT_DARK_STAR, 28000);
                        break;
                    case EVENT_DARK_COMMUNION:
                        DoStopAttack();
                        DoCast(SPELL_DARK_COMMUNION);
                        Talk(SAY_DARK_COMMUNION);
                        events.ScheduleEvent(EVENT_DARK_COMMUNION, 60000);
                        break;
                    case EVENT_DARK_ECLIPSE_1:
                        DoCast(SPELL_DARK_ECLIPSE_VISUAL);
                        events.ScheduleEvent(EVENT_DARK_ECLIPSE_1, 44000);
                        events.ScheduleEvent(EVENT_DARK_ECLIPSE_2, 4000);
                        break;
                    case EVENT_DARK_ECLIPSE_2:
                        DoStopAttack();
                        DoCast(me, SPELL_TELEPORT_TO_HOME, true);
                        DoCast(me, SPELL_DARK_ECLIPSE_AT, true);
                        DoCast(SPELL_DARK_ECLIPSE_CHANNEL);
                        Talk(SAY_DARK_ECLIPSE);
                        events.ScheduleEvent(EVENT_DARK_ECLIPSE_3, 6000);
                        break;
                    case EVENT_DARK_ECLIPSE_3:
                        DoCast(SPELL_DARK_ECLIPSE_DMG);
                        me->SetReactState(REACT_AGGRESSIVE);
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
                DoCast(SPELL_SHADOW_RUNE_1_AT);

            if (summoner->GetEntry() == NPC_SADANA_BLOODFURY)
                DoCast(SPELL_SHADOW_RUNE_2_AT);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (target->GetTypeId() == TYPEID_PLAYER)
            {
                me->RemoveAurasDueToSpell(SPELL_SHADOW_RUNE_1_AT);
                me->RemoveAurasDueToSpell(SPELL_SHADOW_RUNE_2_AT);
            }
        }

        void SpellHit(Unit* target, const SpellInfo* spell)
        {
            if (target->GetEntry() == NPC_SADANA_BLOODFURY)
                if (spell->Id == SPELL_DARK_ECLIPSE_HIT_RUNE)
                    if (!me->HasAura(SPELL_SHADOW_RUNE_2_AT) && target->HasAura(SPELL_DARK_ECLIPSE_CHANNEL))
                        DoCast(SPELL_SHADOW_RUNE_2_AT);
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_sadana_shadow_runeAI (creature);
    }
};

class npc_sadana_daggerfall : public CreatureScript
{
public:
    npc_sadana_daggerfall() : CreatureScript("npc_sadana_daggerfall") { }

    struct npc_sadana_daggerfallAI : public ScriptedAI
    {
        npc_sadana_daggerfallAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void Reset() 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_DAGGERFALL_VISUAL, true);

            events.ScheduleEvent(EVENT_MOVE_FALL_1, 2000);
            events.ScheduleEvent(EVENT_MOVE_FALL_2, 5000);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != EFFECT_MOTION_TYPE)
                return;

            DoCast(me, SPELL_DAGGERFALL_DAMAGE_TICK, true);
        }

        void UpdateAI(uint32 diff) 
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MOVE_FALL_1:
                        DoCast(me, SPELL_DAGGERFALL_DAMAGE_FALL, true);
                        break;
                    case EVENT_MOVE_FALL_2:
                        me->GetMotionMaster()->MoveFall();
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_sadana_daggerfallAI (creature);
    }
};

//75966
class npc_sadana_defiled_spirit : public CreatureScript
{
public:
    npc_sadana_defiled_spirit() : CreatureScript("npc_sadana_defiled_spirit") { }

    struct npc_sadana_defiled_spiritAI : public ScriptedAI
    {
        npc_sadana_defiled_spiritAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISTRACT, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
        }

        EventMap events;
        Creature* owner;

        void Reset() 
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetDisableGravity(true);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            DoCast(SPELL_INVISIBLE_MAN_TRANSFORM);

            owner = NULL;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE)
            {
                if (id == 1)
                {
                    me->RemoveAurasDueToSpell(SPELL_PURPLE_SHADOWY_DROWN);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    DoCast(SPELL_PURPLE_SHADOWY);
                    me->SetAIAnimKitId(4843); //GripMoveAnim
                    me->GetMotionMaster()->MoveFollow(owner, 0.0f, owner->GetAngle(me));
                }
            }
            if (type == FOLLOW_MOTION_TYPE)
                if (me->GetDistance(owner) < 3.0f)
                {
                    me->SetAIAnimKitId(0);
                    me->CastSpell(owner, SPELL_DARK_COMMUNION_HEAL);
                    me->RemoveAurasDueToSpell(SPELL_DARK_COMMUNION);
                    owner->SetReactState(REACT_AGGRESSIVE);
                    EnterEvadeMode();
                }
        }

        void DamageTaken(Unit* /*who*/, uint32& damage)
        {
            if (damage >= me->GetHealth())
            {
                damage = 0;

                me->SetAIAnimKitId(0);
                me->RemoveAurasDueToSpell(SPELL_DARK_COMMUNION);
                owner->SetReactState(REACT_AGGRESSIVE);
                DoCast(me, SPELL_FEIGN_DEATH, true);
                events.ScheduleEvent(EVENT_1, 5000);
            }
        }

        void SpellHit(Unit* target, const SpellInfo* spell)
        {
            if (target->GetEntry() == NPC_SADANA_BLOODFURY)
                if (spell->Id == SPELL_DARK_COMMUNION)
                {
                    owner = target->ToCreature();
                    me->RemoveAurasDueToSpell(SPELL_INVISIBLE_MAN_TRANSFORM);
                    me->CastSpell(me, SPELL_PURPLE_SHADOWY_DROWN, true);
                    me->GetMotionMaster()->MovePoint(1, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() - 20.0f);
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
                        EnterEvadeMode();
                        me->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_sadana_defiled_spiritAI (creature);
    }
};

//153240
class spell_sadana_daggerfall_filter : public SpellScriptLoader
{
    public:
        spell_sadana_daggerfall_filter() : SpellScriptLoader("spell_sadana_daggerfall_filter") { }

        class spell_sadana_daggerfall_filter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sadana_daggerfall_filter_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                uint8 count;
                switch (GetCaster()->GetMap()->GetDifficultyID())
                {
                    case DIFFICULTY_NORMAL:
                        count = 1;
                        break;
                    case DIFFICULTY_HEROIC:
                        count = 2;
                        break;
                    case DIFFICULTY_CHALLENGE:
                        count = 3;
                        break;
                }
                Trinity::Containers::RandomResizeList(targets, count);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sadana_daggerfall_filter_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sadana_daggerfall_filter_SpellScript();
        }
};

//153200, 153225
class spell_sadana_daggerfall : public SpellScriptLoader
{
    public:
        spell_sadana_daggerfall() : SpellScriptLoader("spell_sadana_daggerfall") { }

        class spell_sadana_daggerfall_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sadana_daggerfall_SpellScript);

            void ModDestHeight(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(EFFECT_0);

                switch (GetSpellInfo()->Id)
                {
                    case SPELL_DAGGERFALL_SUMMON:
                    {
                        static Position const offset = {0.0f, 0.0f, 12.0f, 0.0f};
                        GetHitDest()->RelocateOffset(offset);
                        break;
                    }
                    case SPELL_DAGGERFALL_DAMAGE_FALL:
                    {
                        static Position const offset = {0.0f, 0.0f, -12.0f, 0.0f};
                        WorldLocation* dest = const_cast<WorldLocation*>(GetExplTargetDest());
                        dest->RelocateOffset(offset);
                        break;
                    }
                }
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_sadana_daggerfall_SpellScript::ModDestHeight, EFFECT_0, SPELL_EFFECT_SUMMON);
                OnEffectLaunch += SpellEffectFn(spell_sadana_daggerfall_SpellScript::ModDestHeight, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sadana_daggerfall_SpellScript();
        }
};

//153153
class spell_sadana_dark_communion : public SpellScriptLoader
{
    public:
        spell_sadana_dark_communion() : SpellScriptLoader("spell_sadana_dark_communion") { }

        class spell_sadana_dark_communion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sadana_dark_communion_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));
                WorldObject* target = targets.back();
                targets.clear();
                targets.push_back(target);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sadana_dark_communion_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sadana_dark_communion_SpellScript();
        }
};

void AddSC_boss_sadana_bloodfury()
{
    new boss_sadana_bloodfury();
    new npc_sadana_shadow_rune();
    new npc_sadana_daggerfall();
    new npc_sadana_defiled_spirit();
    new spell_sadana_daggerfall_filter();
    new spell_sadana_daggerfall();
    new spell_sadana_dark_communion();
}
