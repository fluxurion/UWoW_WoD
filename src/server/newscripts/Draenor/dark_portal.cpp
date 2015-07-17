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

void AddSC_wod_dark_portal()
{
    new mob_wod_intro_guldan();
    new mob_wod_frostwolf_slave();
    new go_wod_slaves_cage();
}
