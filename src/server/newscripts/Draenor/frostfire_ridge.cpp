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

// 230527
class go_wod_q34375 : public GameObjectScript
{
public:
    go_wod_q34375() : GameObjectScript("go_wod_q34375") { }

    struct go_wod_q34375AI : public GameObjectAI
    {
        go_wod_q34375AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            SPELL_SUMMON = 160657,
            NPC_TREE = 79525,
            __QUEST = 34375,
        };

        EventMap events;
        ObjectGuid treeGUID;

        void InitializeAI()
        { 
            Reset();
            events.ScheduleEvent(TEXT_GENERIC_2, 1000);
        }

        bool GossipUse(Player* player) override
        {
            if (player->GetQuestStatus(__QUEST) != QUEST_STATUS_INCOMPLETE)
                return true;

            Creature * tree = go->GetMap()->GetCreature(treeGUID);

            if (!tree 
                || !tree->isAlive()
                || tree->GetUInt32Value(UNIT_FIELD_STATE_SPELL_VISUAL_ID))
                return true;
            player->KillCreditGO(go->GetEntry(), go->GetGUID());

            tree->SetUInt32Value(UNIT_FIELD_STATE_SPELL_VISUAL_ID, 39303);
            player->CastSpell(player, go->GetGOInfo()->GetSpell(), true);
            events.ScheduleEvent(TEXT_GENERIC_1, 15000);

            go->UseDoorOrButton(30, false, player);
            go->SetUInt32Value(GAMEOBJECT_FIELD_STATE_SPELL_VISUAL_ID, 8743);
            return true;
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                if (eventId == TEXT_GENERIC_1)
                {
                    if (Creature * tree = go->GetMap()->GetCreature(treeGUID))
                    {
                        tree->SetUInt32Value(UNIT_FIELD_STATE_SPELL_VISUAL_ID, 0);
                        tree->SetCorpseDelay(0);
                        tree->SetRespawnDelay(30);
                        tree->ForcedDespawn();
                    }
                }else
                { 
                    if (Creature *tree = go->FindNearestCreature(NPC_TREE, 5.0f))
                    {
                        tree->SetUInt32Value(UNIT_FIELD_STATE_SPELL_VISUAL_ID, 0);
                        treeGUID = tree->GetGUID();
                        if (!tree->isAlive())
                            tree->Respawn();
                    }
                    else
                    {
                        Position pos;
                        go->GetRandomNearPosition(pos, 5.0f);
                        if (TempSummon* summon = go->SummonCreature(NPC_TREE, pos))
                            treeGUID = summon->GetGUID();
                    }
                }
            }
        }
    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_wod_q34375AI(go);
    }
};

// 79526
class mob_wod_q34375 : public CreatureScript
{
public:
    mob_wod_q34375() : CreatureScript("mob_wod_q34375") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_q34375AI(creature);
    }

    struct mob_wod_q34375AI : public ScriptedAI
    {

        mob_wod_q34375AI(Creature* creature) : ScriptedAI(creature)
        {
        }

        enum data
        {
            SOUNDID = 7514,
            NPC_TREE = 79525
        };

        ObjectGuid treeGUID;
        void MovementInform(uint32 type, uint32 /*id*/)
        {
            if (type != POINT_MOTION_TYPE)
                return;
            if (Creature * tree = me->GetMap()->GetCreature(treeGUID))
                me->SetFacingToObject(tree);
            me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_2);
            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_WORK_CHOPWOOD_2);
        }

        void IsSummonedBy(Unit* summoner)
        {
            Player *player = summoner->ToPlayer();
            if (!player)
            {
                me->MonsterSay("SCRIPT::mob_wod_q34375 summoner is not player", LANG_UNIVERSAL, ObjectGuid::Empty);
                return;
            }
            me->PlayDirectSound(SOUNDID, player);
            if (Creature *tree = player->FindNearestCreature(NPC_TREE, 10.0f))
            {
                Position pos;
                tree->GetRandomNearPosition(pos, 5.0f);
                me->GetMotionMaster()->MovePoint(1, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ());
                treeGUID = tree->GetGUID();
            }
        }
    };
};

void AddSC_wod_frostfire_ridge()
{
    new sceneTrigger_q33815();
    new go_wod_q34375();
    new mob_wod_q34375();
}