/*
* Copyright (C) 2008-2015 Uwow.Biz <http://uwow.biz/>
* Copyright (C) 2014-2015 Epicwow.com <http://epicwow.com/>
*/

#include "NewScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "CreatureTextMgr.h"
#include "GameObjectAI.h"
#include "Garrison.h"

Position const hearhstoneHorde[3] = {
    {5560.4f, 4507.7f, 132.67f, 206.1f},         // Horde lvl1 map 1152
    {5565.86f, 4593.5f, 140.893f, 2.332632f},    // Horde lvl2 spell 171325 map 1330
    {5563.7f, 4599.9f, 141.71f, 131.75f},        // Horde lvl3 spell 171325 map 1153
};
Position const hearhstoneAlliance[3] = {
    {1850.71f, 254.43f, 78.083f, 1.886526f},     // ALLiance lvl1 spell 158825
    {1936.9f, 341.35f, 90.28f, 123.479f},        // Alliance lvl2 
    {1947.0f, 324.88f, 90.28f, 118.664f},        // Alliance lvl3
};

//http://www.wowhead.com/spell=171253/garrison-hearthstone
class spell_garrison_hearthstone : public SpellScriptLoader
{
public:
    spell_garrison_hearthstone() : SpellScriptLoader("spell_garrison_hearthstone") { }

    class spell_garrison_hearthstone_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_garrison_hearthstone_SpellScript);

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if (Unit* caster = GetCaster())
            {
                Player* plr = caster->ToPlayer();
                if (!plr)
                    return;

                Garrison *garr = plr->GetGarrison();
                if (!garr || garr->GetGarrisonMapID() == -1)
                    return;
                uint8 idx = garr->GetGarrisonLevel() - 1;

                if (plr->GetTeam() == ALLIANCE)
                    plr->TeleportTo(garr->GetGarrisonMapID(), hearhstoneAlliance[idx].m_positionX, hearhstoneAlliance[idx].m_positionY, hearhstoneAlliance[idx].m_positionZ, hearhstoneAlliance[idx].m_orientation, TELE_TO_SPELL, 171253);
                else
                    plr->TeleportTo(garr->GetGarrisonMapID(), hearhstoneHorde[idx].m_positionX, hearhstoneHorde[idx].m_positionY, hearhstoneHorde[idx].m_positionZ, hearhstoneHorde[idx].m_orientation, TELE_TO_SPELL, 171253);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_garrison_hearthstone_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_TELEPORT_UNITS);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_garrison_hearthstone_SpellScript();
    }
};

//http://www.wowhead.com/spell=173847/loot
//! HORDE Q: 34824 ALLIANCE Q: 35176
class spell_garrison_cache_loot : public SpellScriptLoader
{
public:
    spell_garrison_cache_loot() : SpellScriptLoader("spell_garrison_cache_loot") { }

    class spell_garrison_cache_loot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_garrison_cache_loot_SpellScript);

        enum data
        {
            NPC__ = 80223,
            QUESTA = 35176,
            QUESTH = 34824,
        };

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if (Unit* caster = GetCaster())
            {
                Player* plr = caster->ToPlayer();
                if (!plr)
                    return;

                Garrison *garr = plr->GetGarrison();
                if (!garr || !garr->GetResNumber())
                    return;
                
                plr->ModifyCurrency(CURRENCY_TYPE_GARRISON_RESOURCES, garr->GetResNumber(), false, false, true, true, true);
                garr->UpdateResTakenTime();

                if (plr->GetQuestStatus(QUESTA) == QUEST_STATUS_INCOMPLETE ||
                    plr->GetQuestStatus(QUESTH) == QUEST_STATUS_INCOMPLETE)
                if (Creature *c = plr->FindNearestCreature(NPC__, 100.0f))
                    sCreatureTextMgr->SendChat(c, TEXT_GENERIC_0, plr->GetGUID());
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_garrison_cache_loot_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_garrison_cache_loot_SpellScript();
    }
};

//162714
class spell_q34824 : public SpellScriptLoader
{
public:
    spell_q34824() : SpellScriptLoader("spell_q34824") { }

    class spell_q34824_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_q34824_SpellScript);


        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if (Unit* caster = GetCaster())
            {
                std::list<Player*> playerList;
                caster->GetPlayerListInGrid(playerList, 10.0f);
                for (auto player : playerList)
                    player->KilledMonsterCredit(caster->GetEntry(), ObjectGuid::Empty);

                if (Creature *c = caster->ToCreature())
                    sCreatureTextMgr->SendChat(c, TEXT_GENERIC_0);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_q34824_SpellScript::HandleScriptEffect, 2, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_q34824_SpellScript();
    }
};

//161033 for the gord cast 160767 too
//! HORDE Q: 34378, ALLIANCE Q: 34586
class spell_161033 : public SpellScriptLoader
{
public:
    spell_161033() : SpellScriptLoader("spell_161033") { }

    class spell_161033_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_161033_SpellScript);

#define __spell 160767
        void Handle(SpellEffIndex effIndex)
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->ToPlayer())
                return;

            if (caster->ToPlayer()->GetTeam() == ALLIANCE)
                return;

            caster->CastSpell(caster, __spell, true);
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(spell_161033_SpellScript::Handle, 0, SPELL_EFFECT_KILL_CREDIT2);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_161033_SpellScript();
    }
};

//- 155071
/*
UPDATE `npc_spellclick_spells` set cast_flags = 1 WHERE`spell_id` = 155071; -- need plr caster to creature
*/

class spell_interract : public SpellScriptLoader
{
public:
    spell_interract() : SpellScriptLoader("spell_interract") { }

    class spell_interract_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_interract_SpellScript);

        enum data
        {
            NPC__ = 79446,

            SPELL_CREDIT_Q34646 = 177492,

        };

        void HandleEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if (Unit* caster = GetCaster())
            {
                Player* plr = caster->ToPlayer();
                if (!plr)
                    return;

                Unit * target = GetOriginalTarget(); 
                if (!target)
                    return;

                volatile uint32 entry = target->GetEntry();
                switch (target->GetEntry())
                {
                    case NPC__:
                        //cast 164640
                        //cast 164649
                        plr->TalkedToCreature(target->GetEntry(), target->GetGUID());
                        caster->CastSpell(caster, SPELL_CREDIT_Q34646, true);
                        break;
                    default:
                        break;
                }

            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_interract_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_interract_SpellScript();
    }
};
void AddSC_garrison_general()
{
    new spell_garrison_hearthstone();
    new spell_garrison_cache_loot();
    new spell_q34824();
    new spell_161033();
    new spell_interract();
}