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
#include "ScriptedEscortAI.h"


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

void AddSC_wod_dark_portal()
{
    new mob_wod_intro_guldan();
}
