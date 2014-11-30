/*
    Dungeon : Stormstout Brewery 85-87
    Instance General Script
*/

#include "NewScriptPCH.h"
#include "stormstout_brewery.h"

class instance_stormstout_brewery : public InstanceMapScript
{
public:
    instance_stormstout_brewery() : InstanceMapScript("instance_stormstout_brewery", 961) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_stormstout_brewery_InstanceMapScript(map);
    }

    struct instance_stormstout_brewery_InstanceMapScript : public InstanceScript
    {
        ObjectGuid ookookGuid;
        ObjectGuid hoptallusGuid;
        ObjectGuid yanzhuGuid;
        ObjectGuid ookexitdoorGuid;
        ObjectGuid doorGuid;
        ObjectGuid door2Guid;
        ObjectGuid door3Guid;
        ObjectGuid door4Guid;
        ObjectGuid lastdoorGuid;
        ObjectGuid carrotdoorGuid;

        instance_stormstout_brewery_InstanceMapScript(Map* map) : InstanceScript(map)
        {}

        void Initialize()
        {
            SetBossNumber(3);
            ookookGuid.Clear();
            hoptallusGuid.Clear();
            yanzhuGuid.Clear();
            ookexitdoorGuid.Clear();
            doorGuid.Clear();
            door2Guid.Clear();
            door3Guid.Clear();
            door4Guid.Clear();
            lastdoorGuid.Clear();
            carrotdoorGuid.Clear();
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
            case GO_EXIT_OOK_OOK:
                ookexitdoorGuid = go->GetGUID();
                break;
            case GO_DOOR:
                doorGuid = go->GetGUID();
                break;
            case GO_DOOR2:
                door2Guid = go->GetGUID();
                break;
            case GO_DOOR3:
                door3Guid = go->GetGUID();
                break;
            case GO_DOOR4:
                door4Guid = go->GetGUID();
                break;
            case GO_LAST_DOOR:
                lastdoorGuid = go->GetGUID();
                break;
            case GO_CARROT_DOOR:
                carrotdoorGuid = go->GetGUID();
                break;
            }
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_OOK_OOK:
                    ookookGuid = creature->GetGUID();
                    break;
                case NPC_HOPTALLUS:
                    hoptallusGuid = creature->GetGUID();
                    break;
                case NPC_YAN_ZHU:
                    yanzhuGuid = creature->GetGUID();
                    break;
            }
        }
        
        bool SetBossState(uint32 id, EncounterState state)
        {
            if (!InstanceScript::SetBossState(id, state))
                return false;

            switch (id)
            {
            case DATA_OOK_OOK:
                {
                    if (state == DONE)
                    {
                         HandleGameObject(ookexitdoorGuid, true);
                         HandleGameObject(doorGuid, true);
                         HandleGameObject(door2Guid, true);
                    }
                }
                break;
            case DATA_HOPTALLUS:
                {
                    switch (state)
                    {
                    case NOT_STARTED:
                        HandleGameObject(door2Guid, true);
                        break;
                    case IN_PROGRESS:
                        HandleGameObject(door2Guid, false);
                        break;
                    case DONE:
                        {
                            HandleGameObject(door2Guid, true);
                            HandleGameObject(door3Guid, true);
                            HandleGameObject(door4Guid, true);
                            if (GameObject* go = instance->GetGameObject(carrotdoorGuid))
                                go->Delete();
                        }
                        break;
                    }
                }
                break;
            case DATA_YAN_ZHU:
                {
                    switch (state)
                    {
                    case NOT_STARTED:
                    case DONE:
                        HandleGameObject(lastdoorGuid, true);
                        break;
                    case IN_PROGRESS:
                        HandleGameObject(lastdoorGuid, false);
                        break;
                    }
                }
                break;
            }

            return true;
        }

        void SetData(uint32 type, uint32 data){}

        uint32 GetData(uint32 type)
        {
            return 0;
        }

        ObjectGuid GetGuidData(uint32 type)
        {
            switch (type)
            {
                case NPC_OOK_OOK:
                    return ookookGuid;
                case NPC_HOPTALLUS:
                    return hoptallusGuid;
                case NPC_YAN_ZHU:
                    return yanzhuGuid;
            }

            return ObjectGuid::Empty;
        }

        void Update(uint32 diff) 
        {
            // Challenge
            InstanceScript::Update(diff);
        }
    };

};

void AddSC_instance_stormstout_brewery()
{
    new instance_stormstout_brewery();
}
