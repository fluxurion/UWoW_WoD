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

#include "NewScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "CreatureTextMgr.h"
#include "GameObjectAI.h"

class mob_wod_intro_guldan : public CreatureScript
{
public:
    mob_wod_intro_guldan() : CreatureScript("mob_wod_intro_guldan") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_intro_guldanAI(creature);
    }

    struct mob_wod_intro_guldanAI : public ScriptedAI
    {

        enum data
        {
            SPELL_SCENE_INTRO       = 163807,
            SPELL_PORTAL_CREDIT     = 166319,
            QUEST_PORTAL_ENERGY     = 34393,
            OBJECTIVE_PORTAL_CREDIT = 82573,
        };

        mob_wod_intro_guldanAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void Reset()
        {
        }

        void MoveInLineOfSight(Unit* who)
        {
            Player *player = who->ToPlayer();
            if (!player || !me->IsWithinDistInMap(who, 51.0f) || who->GetPositionZ() > 79.0f)
                return;

            if (player->GetQuestStatus(QUEST_PORTAL_ENERGY) != QUEST_STATUS_INCOMPLETE)
                return;

            if (player->HasAura(SPELL_SCENE_INTRO) || player->GetQuestObjectiveData(QUEST_PORTAL_ENERGY, OBJECTIVE_PORTAL_CREDIT))
                return;

            player->CastSpell(player, SPELL_PORTAL_CREDIT, true);
            player->CastSpell(player, SPELL_SCENE_INTRO, false);
            return;
        }
    };
};

class mob_wod_frostwolf_slave : public CreatureScript
{
public:
    mob_wod_frostwolf_slave() : CreatureScript("mob_wod_frostwolf_slave") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_frostwolf_slaveAI(creature);
    }

    struct mob_wod_frostwolf_slaveAI : public ScriptedAI
    {

        mob_wod_frostwolf_slaveAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        enum data
        {
            QUEST1 = 34421,
            QUEST2 = 35240,
        };

        GuidSet players;

        void Reset()
        {
        }

        void SetGUID(ObjectGuid const& guid, int32 /*id*/ = 0)
        {
            players.insert(guid);
        }

        void MoveInLineOfSight(Unit* who)
        {
            Player *player = who->ToPlayer();
            if (!player || !me->IsWithinDistInMap(who, 51.0f) || who->GetPositionZ() > 79.0f)
                return;

            if (player->GetQuestStatus(QUEST1) != QUEST_STATUS_INCOMPLETE && player->GetQuestStatus(QUEST2) != QUEST_STATUS_INCOMPLETE)
                return;

            auto data = players.find(who->GetGUID());
            if (data != players.end())
                return;

            players.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, 0, who->GetGUID());

            // already trigered player for nearest
            std::list<Creature*> creatureList;
            me->GetCreatureListWithEntryInGrid(creatureList, me->GetEntry(), 50);
            for (auto c : creatureList)
                c->AI()->SetGUID(who->GetGUID(), 0);
        }
    };
};

const uint32 npc[4] = { 82871, 85142, 78529, 85141 };
// 159126 - eastern
class go_wod_slaves_cage : public GameObjectScript
{
public:
    go_wod_slaves_cage() : GameObjectScript("go_wod_slaves_cage") { }

    struct go_wod_slaves_cageAI : public GameObjectAI
    {
        go_wod_slaves_cageAI(GameObject* go) : GameObjectAI(go)
        {

        }
        
        enum data
        {
            SOUTH_SCENE = 159127,
            EASTERN_SCENE = 159126
        };
        bool GossipHello(Player* player) override
        {
            if (go->GetEntry() == 229353)
                player->CastSpell(player, EASTERN_SCENE, true);
            else
                player->CastSpell(player, SOUTH_SCENE, true);

            for (uint8 i = 0; i != 4; ++i)
            {
                if (Creature* target = go->FindNearestCreature(npc[i], 50.0f, true))
                    player->KilledMonsterCredit(npc[i], ObjectGuid::Empty);
            }

            return true;
        }

    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_wod_slaves_cageAI(go);
    }
};

class mob_wod_ariok : public CreatureScript
{
public:
    mob_wod_ariok() : CreatureScript("mob_wod_ariok") { }

    enum data
    {
        QUEST = 34423,
        SPELL_CREDIT = 159278,
        SPELL_SUMMON = 161625,
        PHASE_ARIOK_DB = 3394,
    };
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == 1)
        {
            player->CLOSE_GOSSIP_MENU();
            if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE)
            {
                player->CastSpell(player, SPELL_CREDIT, true);
                player->CastSpell(player, SPELL_SUMMON, true);
                std::set<uint32> phase = player->GetPhases();
                phase.erase(PHASE_ARIOK_DB);
                player->SetPhaseId(phase, true);
            }
        }

        return true;
    }
};

class mob_wod_ariok_mover : public CreatureScript
{
public:
    mob_wod_ariok_mover() : CreatureScript("mob_wod_ariok_mover") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_ariok_moverAI(creature);
    }

    struct mob_wod_ariok_moverAI : public ScriptedAI
    {
        EventMap events;
        ObjectGuid playerGuid;

        mob_wod_ariok_moverAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        enum data
        {
            SPELL_ON_ALTAR_CREDIT = 161637,
            SPELL_SCENE_EYE_CILLROG = 164877,

            NPC_ORB = 83670,

            EVENT_1 = 1,
            EVENT_2,
            EVENT_3,
            EVENT_4,
            EVENT_5,
            EVENT_CHECK_PHASE_1
        };

        void OnCharmed(bool /*apply*/)
        {
        }

        void IsSummonedBy(Unit* summoner)
        {
            Player *player = summoner->ToPlayer();
            if (!player)
            {
                me->MonsterSay("SCRIPT::mob_wod_ariok_moverAI summoner is not player", LANG_UNIVERSAL, ObjectGuid::Empty);
                return;
            }

            playerGuid = summoner->GetGUID();
            me->AddPlayerInPersonnalVisibilityList(summoner->GetGUID());

            uint32 t = 0;                                                       //
            events.ScheduleEvent(EVENT_1, t += 2000);               //09:14:54.000
            events.ScheduleEvent(EVENT_2, t += 6000);               //09:15:00.000
            events.ScheduleEvent(EVENT_3, t += 7000);               //09:15:07.000

            events.ScheduleEvent(EVENT_CHECK_PHASE_1, t += 1000);
        }

        void UpdateAI(uint32 diff)
        {
            UpdateVictim();

            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_4:
                    if (Player* player = sObjectAccessor->FindPlayer(playerGuid))
                    {
                        player->CastSpell(player, SPELL_ON_ALTAR_CREDIT, true);
                        player->CastSpell(player, SPELL_SCENE_EYE_CILLROG, true);
                    }
                    //break;    no break;
                case EVENT_5:
                case EVENT_1:
                case EVENT_2:
                case EVENT_3:
                    sCreatureTextMgr->SendChat(me, /*TEXT_GENERIC_0*/eventId-1, playerGuid);
                    break;
                case EVENT_CHECK_PHASE_1:

                    if (me->GetDistance(3979.26f, -2918.31f, 60.8725f) < 80.0f)
                    {
                        uint32 t = 0;
                        events.ScheduleEvent(EVENT_4, t += 1000);               //09:16:18.000
                        events.ScheduleEvent(EVENT_5, t += 5000);               //09:16:23.000
                    }else
                        events.ScheduleEvent(EVENT_CHECK_PHASE_1, 5000);
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

// Destroy - 167955
class spell_wod_destroying : public SpellScriptLoader
{
public:
    spell_wod_destroying() : SpellScriptLoader("spell_wod_destroying") { }

    class spell_wod_destroying_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_wod_destroying_SpellScript);

        enum data
        {
            SPELL_SCENE = 165061,
        };
        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(EFFECT_1);

            if (Unit* caster = GetCaster())
            {
                Player *player = caster->ToPlayer();
                if (!player)
                    return;

                if (Unit * target = GetHitUnit())
                {
                    target->AddToHideList(caster->GetGUID());
                    caster->CastSpell(caster, SPELL_SCENE, true);
                    target->DestroyForPlayer(player, false);
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_wod_destroying_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_wod_destroying_SpellScript();
    }
};
void AddSC_wod_dark_portal()
{
    new mob_wod_intro_guldan();
    new mob_wod_frostwolf_slave();
    new go_wod_slaves_cage();
    new mob_wod_ariok();
    new mob_wod_ariok_mover();
    new spell_wod_destroying();
}
