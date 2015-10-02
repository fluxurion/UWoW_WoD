/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "bloodmaul_slag_mines.h"
#include "Garrison.h"

enum Texts
{
    TEXT_0, // 'Сломать вас будет удовольствием.'
    TEXT_1, // 'Сначала сломлю ваш дух, потом продам в рабство.'
    TEXT_2, // 'Горите!'
    TEXT_3, // 'Стихии повинуются мне!
    TEXT_4, // 'Вам… не видать... свободы…'
    TEXT_5, // '|TInterface\\Icons\\spell_fire_elementaldevastation.blp:20|t |cFFF00000|Hspell:150678|h[Огненные Недра]|h|r заставляют |3-3(%s) вызвать |cFFF00000|Hspell:150682|h[Лавовый залп]|h|r!'
    TEXT_6, // 'Вы будете рабами лавы!'
    TEXT_7, // '|TInterface\\Icons\\spell_fire_selfdestruct.blp:20|t %s: |cFFF00000|Hspell:150734|h[Необузданное Пламя]|h|r достигает максимальной силы!'
};

enum Spells
{
    SPELL_MOLTEN_BLAST              = 150677,

    SPELL_MAGMA_ERUPTION_COSMETIC   = 150776,
    SPELL_MAGMA_ERUPTION_SUMMON     = 150764,
    SPELL_MAGMA_ERUPTION_AURA       = 150784, //< casted by magma to players in 8 yards...
    SPELL_MAGMA_ERUPRION_A_TRIGGER  = 150783, //< triggered by SPELL_MAGMA_ERUPTION_SSC
    SPELL_MAGMA_ERUPTION_SSC        = 150777,

    SPELL_SUMMON_UNSTABLE_SLAG      = 150755,
    SPELL_MOLTEN_CORE               = 150678,
    SPELL_MOLTEN_BARRAGE            = 150682,
    SPELL_EMPOWERED_FLAMES          = 152091,
    SPELL_FLAME_BUFFET              = 163802,

    //< Unstable Slag
    SPELL_ENSLAVE_SLAG              = 150756, //< casted on self before start movement
    SPELL_SIPHON_FLAMES             = 150732,
    SPELL_UNLEASHED_FLAMES          = 150734, //< triggered by SPELL_SIPHON_FLAMES
};

enum NPCs
{
    NPC_UNSTABLE_SLAG               = 74927,
    NPC_SLG_GENERIC_MOP             = 68553,
    NPC_MAGMA_ERUPTION              = 74967,
};

class boss_gugrokk : public CreatureScript
{
public:
    boss_gugrokk() : CreatureScript("boss_gugrokk") { }

    struct boss_gugrokkAI : public BossAI
    {
        boss_gugrokkAI(Creature* creature) : BossAI(creature, DATA_GUGROKK)
        {
            instance = me->GetInstanceScript();
        }

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();

            events.ScheduleEvent(EVENT_1, 10 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_3, urand(5, 7) * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_4, 20 * IN_MILLISECONDS);

            if (IsHeroic())
                events.ScheduleEvent(EVENT_5, 6 * IN_MILLISECONDS);

            Talk(TEXT_0);
        }

        void JustDied(Unit* /*killer*/)
        {
            _JustDied();

            Talk(TEXT_4);

            if (instance->GetData(DATA_CROMAN_PROGRESS) == IN_PROGRESS)
            {
                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    if (Player* player = itr->getSource())
                        if (Garrison* garrison = player->GetGarrison())
                            garrison->AddFollower(177); //< Croman
                }
            }
        }

        void EnterEvadeMode()
        {
            _EnterEvadeMode();
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->GetDistance(me->GetHomePosition()) >= 120.0f)
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
                    case EVENT_5:
                        events.ScheduleEvent(EVENT_5, urand(10, 19) * IN_MILLISECONDS);
                        DoCastVictim(SPELL_FLAME_BUFFET);
                        Talk(TEXT_6);
                        break;
                    case EVENT_1:
                    {
                        Talk(TEXT_1);

                        events.ScheduleEvent(EVENT_1, urand(13, 18) * IN_MILLISECONDS);
                        if (Map* map = instance->instance)
                        {
                            Map::PlayerList const& players = map->GetPlayers();
                            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                                if (Player* player = itr->getSource())
                                    me->CastSpell(player->GetPosition(), SPELL_MAGMA_ERUPTION_SUMMON, false);
                        }

                        me->CastSpell(me, SPELL_MAGMA_ERUPTION_COSMETIC);
                        break;
                    }
                    case EVENT_3:
                        events.ScheduleEvent(EVENT_3, urand(8, 9) * IN_MILLISECONDS);
                        DoCastVictim(SPELL_MOLTEN_BLAST);
                        me->AddAura(SPELL_MOLTEN_CORE, me);
                        if (me->GetAura(SPELL_MOLTEN_CORE)->GetStackAmount() == 3)
                            events.ScheduleEvent(EVENT_7, 2 * IN_MILLISECONDS);
                        break;
                    case EVENT_7:
                        Talk(TEXT_5);
                        DoCastToAllHostilePlayers(SPELL_MOLTEN_BARRAGE);
                        me->RemoveAura(SPELL_MOLTEN_CORE);
                        break;
                    case EVENT_4:
                    {
                        events.ScheduleEvent(EVENT_4, 20 * IN_MILLISECONDS);
                        events.ScheduleEvent(EVENT_6, 3 * IN_MILLISECONDS);
                        
                        float x = 0.0f, y = 0.0f;
                        GetRandPosFromCenterInDist(me->GetPositionX(), me->GetPositionY(), 7.0f, x, y);
                        me->CastSpell(x, y, me->GetPositionZ(), SPELL_SUMMON_UNSTABLE_SLAG);
                        Talk(TEXT_2);
                        break;
                    }
                    case EVENT_6:
                        if (Creature* slag = me->FindNearestCreature(NPC_UNSTABLE_SLAG, 150.0f))
                        {
                            events.ScheduleEvent(EVENT_10, 10 * IN_MILLISECONDS);
                            slag->AI()->DoAction(ACTION_1);
                            Talk(TEXT_3);
                        }
                        break;
                    case EVENT_10:
                        Talk(TEXT_7);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* instance;
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_gugrokkAI(creature);
    }
};

class npc_bloodmaul_magma_eruption : public CreatureScript
{
public:
    npc_bloodmaul_magma_eruption() : CreatureScript("npc_bloodmaul_magma_eruption") { }

    struct npc_bloodmaul_magma_eruptionAI : public ScriptedAI
    {
        npc_bloodmaul_magma_eruptionAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        void Reset()
        {
            me->SetReactState(REACT_AGGRESSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
            me->AddAura(SPELL_MAGMA_ERUPRION_A_TRIGGER, me);
            me->SetVisible(false);

            aura_timer = 1 * IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*attacker*/) { }

        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
            {
                if (me->IsWithinDistInMap(who, 5.0f) && !who->GetAuraEffect(SPELL_MAGMA_ERUPTION_AURA, EFFECT_0, me->GetGUID()))
                    me->AddAura(SPELL_MAGMA_ERUPTION_AURA, who);

                if (!me->IsWithinDistInMap(who, 5.0f) && who->GetAuraEffect(SPELL_MAGMA_ERUPTION_AURA, EFFECT_0, me->GetGUID()))
                    who->RemoveAura(SPELL_MAGMA_ERUPTION_AURA);
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (aura_timer <= diff)
            {
                me->AddAura(SPELL_MAGMA_ERUPTION_SSC, me);
                aura_timer = 2 * IN_MILLISECONDS;
            }
            else
                aura_timer -= diff;
        }

    private:
        InstanceScript* instance;
        EventMap events;
        uint32 aura_timer;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_bloodmaul_magma_eruptionAI(creature);
    }
};

class npc_bloodmaul_unstable_slag : public CreatureScript
{
public:
    npc_bloodmaul_unstable_slag() : CreatureScript("npc_bloodmaul_unstable_slag") { }

    struct npc_bloodmaul_unstable_slagAI : public ScriptedAI
    {
        npc_bloodmaul_unstable_slagAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);
            reached = false;

            checkTimer = 1 * IN_MILLISECONDS;
        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_1:
                    me->CastSpell(me, SPELL_ENSLAVE_SLAG);
                    me->SetReactState(REACT_PASSIVE);
                    reached = false;
                    events.ScheduleEvent(EVENT_8, 2 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            me->DespawnOrUnsummon(3 * IN_MILLISECONDS);
        }

        void EnterEvadeMode()
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            me->DespawnOrUnsummon(3 * IN_MILLISECONDS);
        }

        void EnterCombat(Unit* /*attacker*/)
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (checkTimer <= diff && !reached)
            {
                checkTimer = 2 * IN_MILLISECONDS;

                if (Creature* target = me->FindNearestCreature(NPC_SLG_GENERIC_MOP, 100.0f))
                    if (me->IsInDist2d(target->GetPositionX(), target->GetPositionY(), 4.0f) && !me->HasAura(SPELL_EMPOWERED_FLAMES))
                    {
                        me->AddAura(SPELL_EMPOWERED_FLAMES, me);
                        reached = true;

                        if (reached && (me->GetReactState() != REACT_AGGRESSIVE))
                            me->SetReactState(REACT_AGGRESSIVE);
                    }
            }
            else
                checkTimer -= diff;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_8:
                        if (Creature* target = me->FindNearestCreature(NPC_SLG_GENERIC_MOP, 100.0f))
                        {
                            me->GetMotionMaster()->MoveChase(target, 3.0f);
                            me->CastSpell(target, SPELL_SIPHON_FLAMES);
                            me->CastSpell(target, SPELL_UNLEASHED_FLAMES);
                        }
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* instance;
        bool reached;
        EventMap events;
        uint32 checkTimer;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_bloodmaul_unstable_slagAI(creature);
    }
};

class TargetFilter
{
public:
    TargetFilter(Unit* caster) : victim(caster->getVictim()) { }

    bool operator()(WorldObject* target)
    {
        if (target == victim && target->GetEntry() == NPC_SLG_GENERIC_MOP)
            return false;

        return true;
    }

private:
    Unit* victim;
};

class spell_unleashed_flames : public SpellScriptLoader
{
public:
    spell_unleashed_flames() : SpellScriptLoader("spell_unleashed_flames") { }

    class spell_unleashed_flames_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_unleashed_flames_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(TargetFilter(GetCaster()));
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_unleashed_flames_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_unleashed_flames_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_unleashed_flames_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_unleashed_flames_SpellScript();
    }
};

void AddSC_boss_gugrokk()
{
    new boss_gugrokk();

    new npc_bloodmaul_magma_eruption();
    new npc_bloodmaul_unstable_slag();

    new spell_unleashed_flames();
}
