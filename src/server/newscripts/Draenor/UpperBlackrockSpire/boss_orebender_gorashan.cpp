/*
    http://epicwow.com/
    Dungeon : Upper Blackrock Spire 90-100
    Encounter: Orebender Gor'ashan
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "upper_blackrock_spire.h"

enum Says
{
    SAY_AGGRO                = 0,
    SAY_WARN_ACTIVE_CONDUIT  = 1,
    SAY_ACTIVE_CONDUIT_2     = 2,
    SAY_ACTIVE_CONDUIT_4     = 3,
    SAY_ACTIVE_CONDUIT_6     = 4,
    SAY_KILL_PLAYER          = 5,
    SAY_DESTROY              = 6,
    SAY_DEATH                = 7,
};

enum Spells
{
    //intoRune
    SPELL_RUNE_GLOW              = 154226,
    //Gorashan
    SPELL_SEAL_CONDUIT_1         = 154302,
    SPELL_SEAL_CONDUIT_2         = 154900,
    SPELL_SEAL_CONDUIT_3         = 154901,
    SPELL_UNHARNESSED_POWER      = 154265,
    SPELL_POWER_CONDUIT          = 154294,
    SPELL_POWER_CONDUIT_AURA     = 166168,
    SPELL_POWER_CONDUIT_HANGOVER = 166169,
    //lighting sphere
    SPELL_ELECTRIC_PULSE         = 154335,
    //Phase 1
    SPELL_BLADES_OF_STEEL        = 154417,
    SPELL_SHRAPNEL_NOVA          = 154448,
    SPELL_LODESTONE_SPIKE        = 154435,
    SPELL_LODESTONE_SPIKE_AT     = 154428,
    //Phase 2
    SPELL_THUNDER_CACOPHONY      = 154899,
    //Open Cage
    SPELL_DISRUPTING_CONDUIT     = 154296,
};

enum eEvents
{
    EVENT_BLADES_OF_STEEL       = 1,
    EVENT_SHRAPNEL_NOVA         = 2,
    EVENT_THUNDER_CACOPHONY     = 3,
    EVENT_LODESTONE_SPIKE       = 4,
};

enum eActions
{
    ACTION_REMOVE_CONDUIT    = 1,
};

enum Phases
{
    PHASE_BATTLE_1   = 1,
    PHASE_BATTLE_2   = 2,
};

enum lightingPath
{
    LIGHTNING_PATH_1 = 7646400,
    LIGHTNING_PATH_2 = 7646401,
};

class boss_orebender_gorashan : public CreatureScript
{
public:
    boss_orebender_gorashan() : CreatureScript("boss_orebender_gorashan") { }

    struct boss_orebender_gorashanAI : public BossAI
    {
        boss_orebender_gorashanAI(Creature* creature) : BossAI(creature, DATA_GORASHAN), summons(me) {}

        SummonList summons;

        uint8 healthPct;
        uint8 lightCount;
        bool intro;

        void Reset()
        {
            events.Reset();
            _Reset();
            summons.DespawnAll();
            summonAdds();
            intro = true;
            lightCount = 0;

            me->SetReactState(REACT_PASSIVE);
            me->RemoveAreaObject(SPELL_LODESTONE_SPIKE_AT);

            healthPct = 75;
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();

            //Lightning Field
            me->SummonCreature(NPC_LIGHTNING_FIELD, goroshanAdds[7]); 
            if (me->GetMap()->GetDifficultyID() != DIFFICULTY_NORMAL)
                me->SummonCreature(NPC_LIGHTNING_FIELD, goroshanAdds[8]);
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

        void summonAdds()
        {
            for (uint8 i = 0; i < 7; i++)
                me->SummonCreature(NPC_POWER_RUNE, goroshanAdds[i]);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
            
            summons.SetReactState(REACT_PASSIVE);

            if (summon->GetEntry() == NPC_LIGHTNING_FIELD)
            {
                if (!lightCount)
                    summon->GetMotionMaster()->MovePath(LIGHTNING_PATH_1, true);
                else
                    summon->GetMotionMaster()->MovePath(LIGHTNING_PATH_2, true);

                summon->CastSpell(summon, SPELL_ELECTRIC_PULSE);

                lightCount++;
            }
        }

        void SpellHit(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_POWER_CONDUIT)
            {
                DoCast(SPELL_POWER_CONDUIT_AURA);
                events.SetPhase(PHASE_BATTLE_2);
                events.RescheduleEvent(EVENT_THUNDER_CACOPHONY, 3000, 0, PHASE_BATTLE_2);
            }
        }

        void DoAction(const int32 action)
        {
            if (action == ACTION_REMOVE_CONDUIT)
            {
                if (intro)
                {
                    intro = false;
                    Talk(SAY_AGGRO);
                    me->SetReactState(REACT_AGGRESSIVE);
                }
                events.SetPhase(PHASE_BATTLE_1);
                events.RescheduleEvent(EVENT_BLADES_OF_STEEL, 0, 0, PHASE_BATTLE_1);
                events.RescheduleEvent(EVENT_SHRAPNEL_NOVA, 16000, 0, PHASE_BATTLE_1);
                events.RescheduleEvent(EVENT_LODESTONE_SPIKE, 4000, 0, PHASE_BATTLE_1);
            }
        }

        void DamageTaken(Unit* attacker, uint32 &damage)
        {
            if (HealthBelowPct(healthPct))
            {
                Talk(SAY_WARN_ACTIVE_CONDUIT);

                switch (healthPct)
                {
                    case 75:
                        healthPct -= 25;
                        Talk(SAY_ACTIVE_CONDUIT_2);
                        DoCast(me, SPELL_SEAL_CONDUIT_1, true);
                        break;
                    case 50:
                        healthPct -= 25;
                        Talk(SAY_ACTIVE_CONDUIT_4);
                        DoCast(me, SPELL_SEAL_CONDUIT_2, true);
                        break;
                    case 25:
                        healthPct -= 15;
                        Talk(SAY_ACTIVE_CONDUIT_6);
                        DoCast(me, SPELL_SEAL_CONDUIT_3, true);
                        break;
                    case 10:
                        healthPct -= 10;
                        Talk(SAY_DESTROY);
                        break;
                }
            }
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
                    case EVENT_BLADES_OF_STEEL:
                        if (Unit* target = me->getVictim())
                            DoCast(target, SPELL_BLADES_OF_STEEL);
                        events.RescheduleEvent(EVENT_BLADES_OF_STEEL, 2000, 0, PHASE_BATTLE_1);
                        break;
                    case EVENT_SHRAPNEL_NOVA:
                        DoCast(SPELL_SHRAPNEL_NOVA);
                        events.RescheduleEvent(EVENT_SHRAPNEL_NOVA, 16000, 0, PHASE_BATTLE_1);
                        break;
                    case EVENT_THUNDER_CACOPHONY:
                        DoCast(SPELL_THUNDER_CACOPHONY);
                        events.RescheduleEvent(EVENT_THUNDER_CACOPHONY, 3000, 0, PHASE_BATTLE_2);
                        break;
                    case EVENT_LODESTONE_SPIKE:
                        DoCast(me, SPELL_LODESTONE_SPIKE, true);
                        events.RescheduleEvent(EVENT_LODESTONE_SPIKE, 10000, 0, PHASE_BATTLE_1);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_orebender_gorashanAI (creature);
    }
};

//76417
class npc_gorashan_power_rune : public CreatureScript
{
public:
    npc_gorashan_power_rune() : CreatureScript("npc_gorashan_power_rune") { }

    struct npc_gorashan_power_runeAI : public ScriptedAI
    {
        npc_gorashan_power_runeAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() 
        {
            DoCast(me, SPELL_POWER_CONDUIT, true);

            if (GameObject* cage = me->FindNearestGameObject(GO_CONDUIT, 3.0f))
                if (cage->GetGoState() != GO_STATE_READY)
                    cage->ResetDoorOrButton();
        }

        void SpellHit(Unit* target, const SpellInfo* spell)
        {
            if (target->GetEntry() == NPC_OREBENDER_GORASHAN)
            {
                DoCast(target, SPELL_POWER_CONDUIT, true);

                if (GameObject* cage = me->FindNearestGameObject(GO_CONDUIT, 3.0f))
                    if (cage->GetGoState() != GO_STATE_READY)
                        cage->ResetDoorOrButton();
            }

            if (spell->Id == SPELL_DISRUPTING_CONDUIT)
            {
                DoCast(me, SPELL_UNHARNESSED_POWER, true);
                me->InterruptSpell(CURRENT_CHANNELED_SPELL);
            }
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_gorashan_power_runeAI (creature);
    }
};

// First dungeon room
class npc_rune_glow : public CreatureScript
{
public:
    npc_rune_glow() : CreatureScript("npc_rune_glow") { }

    struct npc_rune_glowAI : public ScriptedAI
    {
        npc_rune_glowAI(Creature* creature) : ScriptedAI(creature) 
        {
            spawnCount = 0;
        }

        uint8 spawnCount;

        void Reset() 
        {
            DoCast(SPELL_RUNE_GLOW);
            summonAdds();
        }

        void summonAdds()
        {
            for (uint8 i = 0; i < 5; ++i)
                if (me->GetDistance(runeSpawn[i]) < 1.0f)
                {
                    switch (i)
                    {
                        case 0:
                            me->SummonCreature(NPC_BLACK_IRON_BERSERKER, runeSummons[0]);
                            me->SummonCreature(NPC_BLACK_IRON_BERSERKER, runeSummons[1]);
                            me->SummonCreature(NPC_BLACK_IRON_LEADBELCHER, runeSummons[2]);
                            spawnCount = 3;
                            break;
                        case 1:
                            me->SummonCreature(NPC_BLACK_IRON_BERSERKER, runeSummons[3]);
                            me->SummonCreature(NPC_BLACK_IRON_WARCASTER, runeSummons[4]);
                            me->SummonCreature(NPC_BLACK_IRON_GRUNT, runeSummons[5]);
                            spawnCount = 3;
                            break;
                        case 2:
                            me->SummonCreature(NPC_BLACK_IRON_LEADBELCHER, runeSummons[6]);
                            me->SummonCreature(NPC_BLACK_IRON_BERSERKER, runeSummons[7]);
                            me->SummonCreature(NPC_BLACK_IRON_LEADBELCHER, runeSummons[8]);
                            me->SummonCreature(NPC_BLACK_IRON_GRUNT, runeSummons[9]);
                            spawnCount = 4;
                            break;
                        case 3:
                            me->SummonCreature(NPC_BLACK_IRON_GRUNT, runeSummons[10]);
                            me->SummonCreature(NPC_BLACK_IRON_WARCASTER, runeSummons[11]);
                            me->SummonCreature(NPC_BLACK_IRON_LEADBELCHER, runeSummons[12]);
                            me->SummonCreature(NPC_BLACK_IRON_BERSERKER, runeSummons[13]);
                            spawnCount = 4;
                            break;
                        case 4:
                            me->SummonCreature(NPC_RAGEMAW_WORG, runeSummons[14]);
                            me->SummonCreature(NPC_BLACK_IRON_LEADBELCHER, runeSummons[15]);
                            me->SummonCreature(NPC_BLACK_IRON_WARCASTER, runeSummons[16]);
                            me->SummonCreature(NPC_BLACK_IRON_GRUNT, runeSummons[17]);
                            spawnCount = 4;
                            break;
                    }
                }
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            spawnCount--;

            if (spawnCount == 0)
            {
                me->RemoveAurasDueToSpell(SPELL_RUNE_GLOW);
                me->Kill(me);
            }
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_rune_glowAI (creature);
    }
};

class spell_gorashan_power_conduit : public SpellScriptLoader
{
    public:
        spell_gorashan_power_conduit() : SpellScriptLoader("spell_gorashan_power_conduit") { }

        class spell_gorashan_power_conduit_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gorashan_power_conduit_AuraScript);

            uint8 stack;

            bool Load()
            {
                stack = 0;
                return true;
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Creature* target = GetTarget()->ToCreature();
                if (!target)
                    return;

                if (Aura* aura = target->GetAura(SPELL_POWER_CONDUIT_AURA))
                {
                    stack = aura->GetStackAmount();

                    if (stack > 1)
                        aura->SetStackAmount(stack - 1);
                    else
                    {
                        aura->Remove();
                        target->CastSpell(target, SPELL_POWER_CONDUIT_HANGOVER, true);
                        target->AI()->DoAction(ACTION_REMOVE_CONDUIT);
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_gorashan_power_conduit_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gorashan_power_conduit_AuraScript();
        }
};
class CheckPos
{
    public:
        CheckPos() { }

        bool operator() (WorldObject* player) const
        {
            if (player->GetPositionX() > 138.4f && player->GetPositionX() < 150.3f && 
                player->GetPositionY() > -264.3f && player->GetPositionY() < -252.3f &&
                player->GetPositionZ() > 94.0f && player->GetPositionZ() < 97.0f)
                return true;
            return false;
        }
};
//154435
class spell_lodestone_spike : public SpellScriptLoader
{
    public:
        spell_lodestone_spike() : SpellScriptLoader("spell_lodestone_spike") { }

        class spell_lodestone_spike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lodestone_spike_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(CheckPos());
            }

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, SPELL_LODESTONE_SPIKE_AT, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_lodestone_spike_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnHit += SpellHitFn(spell_lodestone_spike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lodestone_spike_SpellScript();
        }
};

//154345
class spell_electric_pulse : public SpellScriptLoader
{
    public:
        spell_electric_pulse() : SpellScriptLoader("spell_electric_pulse") { }

        class spell_electric_pulse_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_electric_pulse_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(CheckPos());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_electric_pulse_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_electric_pulse_SpellScript();
        }
};

void AddSC_boss_orebender_gorashan()
{
    new boss_orebender_gorashan();
    new npc_gorashan_power_rune();
    new npc_rune_glow();
    new spell_gorashan_power_conduit();
    new spell_lodestone_spike();
    new spell_electric_pulse();
}
