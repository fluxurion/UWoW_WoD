/*
    http://epicwow.com/
    Dungeon : Skyreach 97 - 99
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "skyreach.h"

DoorData const doorData[] =
{
    //{GO_,       DATA_,         DOOR_TYPE_PASSAGE,    BOUNDARY_NONE},
    {0,                   0,                  DOOR_TYPE_ROOM,       BOUNDARY_NONE}, // END
};

class instance_skyreach : public InstanceMapScript
{
public:
    instance_skyreach() : InstanceMapScript("instance_skyreach", 1209) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_skyreach_InstanceMapScript(map);
    }

    struct instance_skyreach_InstanceMapScript : public InstanceScript
    {
        instance_skyreach_InstanceMapScript(Map* map) : InstanceScript(map) 
        {
            SetBossNumber(MAX_ENCOUNTER);
        }

        //ObjectGuid BraunGUID;

        void Initialize()
        {
            LoadDoorData(doorData);
            //BraunGUID.Clear();
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;
            
            return true;
        }

        void OnGameObjectCreate(GameObject* go)
        {
            /* switch (go->GetEntry())
            {
                case GO_HARLAN_DOOR:
                    AddDoor(go, true);
                    break;
                default:
                    break;
            } */
        }

        void OnCreatureCreate(Creature* creature)
        {
            /* switch (creature->GetEntry())
            {
                case NPC_HOUNDMASTER_BRAUN:    
                    BraunGUID = creature->GetGUID(); 
                    break;
            } */
        }

        void SetData(uint32 type, uint32 data)
        {
            /*switch (type)
            {
                default:
                    break;
            }*/
        }

        /* ObjectGuid GetGuidData(uint32 type) const
        {
            switch (type)
            {
                case NPC_HOUNDMASTER_BRAUN:   
                    return BraunGUID;
            }
            return ObjectGuid::Empty;
        } */

        uint32 GetData(uint32 type) const
        {
            return 0;
        }

        /* void Update(uint32 diff) 
        {
            // Challenge
            InstanceScript::Update(diff);
        } */
    };
};

void AddSC_instance_skyreach()
{
    new instance_skyreach();
}