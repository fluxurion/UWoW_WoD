/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
#include "mogu_shan_vault.h"

enum eSpells
{
    // Shared
    SPELL_SPIRIT_BOLT                   = 118530,
    SPELL_STRENGHT_OF_SPIRIT            = 116363,

    // Visuals
    SPELL_SPIRIT_FIST                   = 115743,
    SPELL_SPIRIT_SPEAR                  = 115740,
    SPELL_SPIRIT_STAFF                  = 115742,
    SPELL_SPIRIT_SHIELD                 = 115741,

    // Spirit of the Fist
    SPELL_LIGHTNING_LASH                = 131788,
    SPELL_LIGHTNING_FISTS               = 116157,
    SPELL_EPICENTER                     = 116018,

    // Spirit of the Spear
    SPELL_FLAMING_SPEAR                 = 116942,
    SPELL_WILDFIRE_SPARK                = 116784,
    SPELL_DRAW_FLAME                    = 116711,

    // Spirit of the Staff 
    SPELL_ARCANE_SHOCK                  = 131790,
    SPELL_ARCANE_VELOCITY               = 116364,
    SPELL_ARCANE_RESONANCE              = 116417,

    // Spirit of the Shield ( Heroic )
    SPELL_SHADOWBURN                    = 17877,
    SPELL_SIPHONING_SHIELD              = 118071,
    SPELL_CHAINS_OF_SHADOW              = 118783,

    // Stolen Essences of Stone
    SPELL_NULLIFICATION_BARRIER         = 115817,
    SPELL_SHROUD_OF_REVERSAL            = 115911,

    // Controler Visual
    SPELL_VISUAL_FIST                   = 105032,
    SPELL_VISUAL_SPEAR                  = 118485,
    SPELL_VISUAL_STAFF                  = 118486,
    SPELL_VISUAL_SHIELD                 = 117303,

    // Inversions Spell
    SPELL_INVERSION                     = 115972,

    SPELL_EPICENTER_INVERSION           = 118300,
    SPELL_LIGHTNING_FISTS_INVERSION     = 118302,
    SPELL_ARCANE_RESONANCE_INVERSION    = 118304,
    SPELL_ARCANE_VELOCITY_INVERSION     = 118305,
    SPELL_WILDFIRE_SPARK_INVERSION      = 118307,
    SPELL_FLAMING_SPEAR_INVERSION       = 118308,
    // Inversion bouclier siphon        = 118471,
    SPELL_SHADOWBURN_INVERSION          = 132296,
    SPELL_LIGHTNING_LASH_INVERSION      = 132297,
    SPELL_ARCANE_SHOCK_INVERSION        = 132298
};

enum eEvents
{
    EVENT_DOT_ATTACK            = 1,
    EVENT_RE_ATTACK             = 2,

    EVENT_LIGHTNING_FISTS       = 3,
    EVENT_EPICENTER             = 4,

    EVENT_WILDFIRE_SPARK        = 5,
    EVENT_DRAW_FLAME            = 6,

    EVENT_ARCANE_VELOCITY       = 7,
    EVENT_ARCANE_RESONANCE      = 8,
};

enum ePhases
{
    PHASE_NONE                  = 0,
    PHASE_FIST                  = 1,
    PHASE_SPEAR                 = 2,
    PHASE_STAFF                 = 3,
    PHASE_SHIELD                = 4
};

Position modPhasePositions[4] =
{
    {4063.26f, 1320.50f, 466.30f, 5.5014f}, // Phase Fist
    {4021.17f, 1320.50f, 466.30f, 3.9306f}, // Phase Spear
    {4021.17f, 1362.80f, 466.30f, 2.0378f}, // Phase Staff
    {4063.26f, 1362.80f, 466.30f, 0.7772f}, // Phase Shield
};

uint32 statueEntryInOrder[4] = {GOB_FIST_STATUE,   GOB_SPEAR_STATUE,   GOB_STAFF_STATUE,   GOB_SHIELD_STATUE};
uint32 controlerVisualId[4]  = {SPELL_VISUAL_FIST, SPELL_VISUAL_SPEAR, SPELL_VISUAL_STAFF, SPELL_VISUAL_SHIELD};
uint32 fengVisualId[4]       = {SPELL_SPIRIT_FIST, SPELL_SPIRIT_SPEAR, SPELL_SPIRIT_STAFF, SPELL_SPIRIT_SHIELD};

#define MAX_INVERSION_SPELLS    9
uint32 inversionMatching[MAX_INVERSION_SPELLS][2] =
{
    {SPELL_EPICENTER,        SPELL_EPICENTER_INVERSION},
    {SPELL_LIGHTNING_FISTS,  SPELL_LIGHTNING_FISTS_INVERSION},
    {SPELL_ARCANE_RESONANCE, SPELL_ARCANE_RESONANCE_INVERSION},
    {SPELL_ARCANE_VELOCITY,  SPELL_ARCANE_VELOCITY_INVERSION},
    {SPELL_WILDFIRE_SPARK,   SPELL_WILDFIRE_SPARK_INVERSION},
    {SPELL_FLAMING_SPEAR,    SPELL_FLAMING_SPEAR_INVERSION},
    {SPELL_SHADOWBURN,       SPELL_SHADOWBURN_INVERSION},
    {SPELL_LIGHTNING_LASH,   SPELL_LIGHTNING_LASH_INVERSION},
    {SPELL_ARCANE_SHOCK,     SPELL_ARCANE_SHOCK_INVERSION}
};

class boss_feng : public CreatureScript
{
    public:
        boss_feng() : CreatureScript("boss_feng") {}

        struct boss_fengAI : public BossAI
        {
            boss_fengAI(Creature* creature) : BossAI(creature, DATA_FENG)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;

            std::list<uint32> phaseList;
            uint8 actualPhase;

            uint32 nextPhasePct;
            uint32 dotSpellId;

            void Reset()
            {
                _Reset();

                phaseList.clear();
                for (int i = 1; i <= (IsHeroic() ? 4: 3); ++i)
                    phaseList.push_back(i);

                //std::random_shuffle(phaseList.begin(), phaseList.end());

                for (auto visualSpellId: fengVisualId)
                    me->RemoveAurasDueToSpell(visualSpellId);

                actualPhase  = PHASE_NONE;
                nextPhasePct = 95;
                dotSpellId = 0;
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (id >= 1 && id <= 4)
                    PrepareNewPhase(id);
            }

            void PrepareNewPhase(uint8 newPhase)
            {
                events.Reset();
                events.ScheduleEvent(EVENT_DOT_ATTACK, 30000);
                events.ScheduleEvent(EVENT_RE_ATTACK,  1000);
                me->GetMotionMaster()->Clear();

                if (Creature* controler = GetClosestCreatureWithEntry(me, NPC_PHASE_CONTROLER, 20.0f))
                    controler->DespawnOrUnsummon();

                if (GameObject* statue = pInstance->instance->GetGameObject(pInstance->GetData64(statueEntryInOrder[newPhase - 1])))
                {
                    statue->SetLootState(GO_READY);
                    statue->UseDoorOrButton();
                }

                for (auto visualSpellId: fengVisualId)
                    me->RemoveAurasDueToSpell(visualSpellId);

                me->AddAura(fengVisualId[newPhase - 1], me);

                switch (newPhase)
                {
                    case PHASE_FIST:
                    {
                        dotSpellId = SPELL_LIGHTNING_LASH;
                        events.ScheduleEvent(EVENT_LIGHTNING_FISTS,  20000, PHASE_FIST);
                        events.ScheduleEvent(EVENT_EPICENTER,        35000, PHASE_FIST);
                        break;
                    }
                    case PHASE_SPEAR:
                    {
                        dotSpellId = SPELL_FLAMING_SPEAR;
                        events.ScheduleEvent(EVENT_WILDFIRE_SPARK,   35000, PHASE_SPEAR);
                        events.ScheduleEvent(EVENT_DRAW_FLAME,       40000, PHASE_SPEAR);
                        break;
                    }
                    case PHASE_STAFF:
                    {
                        dotSpellId = SPELL_ARCANE_SHOCK;
                        events.ScheduleEvent(EVENT_ARCANE_VELOCITY,  25000, PHASE_STAFF);
                        events.ScheduleEvent(EVENT_ARCANE_RESONANCE, 40000, PHASE_STAFF);
                        break;
                    }
                    case PHASE_SHIELD:
                    {
                        dotSpellId = SPELL_SHADOWBURN;
                        // Todo
                        break;
                    }
                    default:
                        break;
                }
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                if (AuraPtr inversion = target->GetAura(115911))
                {
                    if (Unit* caster = inversion->GetCaster())
                    {
                        for (uint8 i = 0; i < MAX_INVERSION_SPELLS; ++i)
                        {
                            if (spell->Id == inversionMatching[i][0])
                            {
                                bool alreadyHaveAnInversion = false;

                                for (uint8 j = 0; j < MAX_INVERSION_SPELLS; ++j)
                                    if (caster->HasAura(inversionMatching[j][1]))
                                    {
                                        alreadyHaveAnInversion = true;
                                        break;
                                    }

                                if (!alreadyHaveAnInversion)
                                    caster->CastSpell(caster, inversionMatching[i][1], true);

                                break;
                            }
                        }
                    }
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (!pInstance)
                    return;

                if (nextPhasePct)
                {
                    if (me->HealthBelowPctDamaged(nextPhasePct, damage))
                    {
                        events.Reset();
                        uint8  newPhase = *phaseList.begin();
                        phaseList.pop_front();

                        if (GameObject* statue = pInstance->instance->GetGameObject(pInstance->GetData64(statueEntryInOrder[newPhase - 1])))
                        {
                            statue->SetLootState(GO_READY);
                            statue->UseDoorOrButton();
                        }

                        if (Creature* controler = me->SummonCreature(NPC_PHASE_CONTROLER, modPhasePositions[newPhase - 1].GetPositionX(), modPhasePositions[newPhase - 1].GetPositionY(), modPhasePositions[newPhase - 1].GetPositionZ()))
                            controler->AddAura(controlerVisualId[newPhase - 1], controler);

                        me->GetMotionMaster()->MovePoint(newPhase, modPhasePositions[newPhase - 1].GetPositionX(), modPhasePositions[newPhase - 1].GetPositionY(), modPhasePositions[newPhase - 1].GetPositionZ());

                        uint32 reduction = IsHeroic() ? 25: 32;
                        nextPhasePct >= reduction ? nextPhasePct-= reduction: nextPhasePct = 0;
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch(events.ExecuteEvent())
                {
                    // All Phases
                    case EVENT_DOT_ATTACK:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, dotSpellId, false);

                        events.ScheduleEvent(EVENT_DOT_ATTACK, 30000);
                        break;
                    }
                    case EVENT_RE_ATTACK:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->GetMotionMaster()->MoveChase(target);
                        break;
                    }
                    // Fist Phase
                    case EVENT_LIGHTNING_FISTS:
                    {
                        me->CastSpell(me, SPELL_LIGHTNING_FISTS, false);
                        events.ScheduleEvent(EVENT_LIGHTNING_FISTS, 20000);
                        break;
                    }
                    case EVENT_EPICENTER:
                    {
                        me->CastSpell(me, SPELL_EPICENTER, false);
                        events.ScheduleEvent(EVENT_EPICENTER, 35000);
                        break;
                    }
                    // Spear Phase
                    case EVENT_WILDFIRE_SPARK: // Todo
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_WILDFIRE_SPARK, false);
                        events.ScheduleEvent(EVENT_WILDFIRE_SPARK, 35000);
                        break;
                    }
                    case EVENT_DRAW_FLAME: // Todo
                    {
                        // Todo
                        events.ScheduleEvent(EVENT_DRAW_FLAME, 10000);
                        break;
                    }
                    // Staff Phase
                    case EVENT_ARCANE_VELOCITY:
                    {
                        me->CastSpell(me, SPELL_ARCANE_VELOCITY, false);
                        events.ScheduleEvent(EVENT_ARCANE_VELOCITY, 15000);
                        break;
                    }
                    case EVENT_ARCANE_RESONANCE:
                    {
                        me->CastSpell(me, SPELL_ARCANE_RESONANCE, false);
                        events.ScheduleEvent(EVENT_ARCANE_RESONANCE, 40000);
                        break;
                    }
                    // Shield Phase : TODO
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_fengAI(creature);
        }
};

enum eLightningFistSpell
{
    SPELL_FIST_BARRIER      = 115856,
    SPELL_FIST_CHARGE       = 116374
};

class mob_lightning_fist : public CreatureScript
{
    public:
        mob_lightning_fist() : CreatureScript("mob_lightning_fist") {}

        struct mob_lightning_fistAI : public ScriptedAI
        {
            mob_lightning_fistAI(Creature* creature) : ScriptedAI(creature)
            {}

            uint32 checkNearPlayerTimer;

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->AddAura(SPELL_FIST_BARRIER, me);

                float x = 0, y = 0;
                GetPositionWithDistInOrientation(me, 100.0f, me->GetOrientation(), x, y);

                me->GetMotionMaster()->MoveCharge(x, y, me->GetPositionZ(), 24.0f, 1);

                checkNearPlayerTimer = 500;
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (id == 1)
                    me->DespawnOrUnsummon();
            }

            void UpdateAI(const uint32 diff)
            {
                if (checkNearPlayerTimer)
                {
                    if (checkNearPlayerTimer <= diff)
                    {
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 5.0f);

                        for (auto player: playerList)
                            me->CastSpell(player, SPELL_FIST_CHARGE, true);

                        checkNearPlayerTimer = 500;
                    }
                    else
                        checkNearPlayerTimer -= diff;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_lightning_fistAI(creature);
        }
};

class spell_mogu_epicenter : public SpellScriptLoader
{
    public:
        spell_mogu_epicenter() : SpellScriptLoader("spell_mogu_epicenter") { }

        class spell_mogu_epicenter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mogu_epicenter_SpellScript);

            void DealDamage()
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;
                
                float distance = caster->GetDistance(target);

                if (distance > 1)
                    SetHitDamage(GetHitDamage() * (100 - (distance / 60 * 100)));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mogu_epicenter_SpellScript::DealDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mogu_epicenter_SpellScript();
        }
};

class spell_mogu_inversion : public SpellScriptLoader
{
    public:
        spell_mogu_inversion() : SpellScriptLoader("spell_mogu_inversion") { }

        class spell_mogu_inversion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mogu_inversion_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget())
                    GetTarget()->RemoveAurasDueToSpell(SPELL_INVERSION);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget())
                    GetTarget()->CastSpell(GetTarget(), SPELL_INVERSION, true);
            }

            void Register()
            {
                OnEffectApply     += AuraEffectApplyFn(spell_mogu_inversion_AuraScript::OnApply,   EFFECT_0, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                AfterEffectRemove += AuraEffectRemoveFn(spell_mogu_inversion_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mogu_inversion_AuraScript();
        }
};

void AddSC_boss_feng()
{
    new boss_feng();
    new mob_lightning_fist();
    new spell_mogu_epicenter();
    new spell_mogu_inversion();
}