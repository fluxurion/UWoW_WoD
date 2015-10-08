/*
* Copyright (C) 2008-2015 Uwow.Biz <http://uwow.biz/>
* Copyright (C) 2014-2015 Epicwow.com <http://epicwow.com/>
*/

// DRAENOR START LOCATION FOR THE HORDE + GARRIZONE

#include "NewScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "CreatureTextMgr.h"
#include "GameObjectAI.h"

class sceneTrigger_q33815 : public SceneTriggerScript
{
public:
    sceneTrigger_q33815() : SceneTriggerScript("sceneTrigger_q33815")
    {}

    enum data
    {
        CREDIT_ = 78272,
        NPC__ = 78272,
    };

    bool OnTrigger(Player* player, SpellScene const* trigger, std::string type) override
    {
        if (type == "durotanIntroduced")
        {
            player->KilledMonsterCredit(CREDIT_, ObjectGuid::Empty);
            if (Creature *c = player->FindNearestCreature(NPC__, 100.0f))
                sCreatureTextMgr->SendChat(c, TEXT_GENERIC_0, player->GetGUID());
        }
        return true;
    }
};

void AddSC_wod_frostfire_ridge()
{
    new sceneTrigger_q33815();
}