/*
    http://epicwow.com/
    Dungeon : Upper Blackrock Spire 90-100
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "upper_blackrock_spire.h"

DoorData const doorData[] =
{
    //{GO_,       DATA_,         DOOR_TYPE_PASSAGE,    BOUNDARY_NONE},
    {0,                   0,                  DOOR_TYPE_ROOM,       BOUNDARY_NONE}, // END
};

class instance_upper_blackrock_spire : public InstanceMapScript
{
public:
    instance_upper_blackrock_spire() : InstanceMapScript("instance_upper_blackrock_spire", 1358) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_upper_blackrock_spire_InstanceMapScript(map);
    }

    struct instance_upper_blackrock_spire_InstanceMapScript : public InstanceScript
    {
        instance_upper_blackrock_spire_InstanceMapScript(Map* map) : InstanceScript(map) 
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

void AddSC_instance_upper_blackrock_spire()
{
    new instance_upper_blackrock_spire();
}