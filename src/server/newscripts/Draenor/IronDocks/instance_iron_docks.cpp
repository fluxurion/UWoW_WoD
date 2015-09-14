/*
    http://epicwow.com/
    Dungeon : Iron Docks 93-95
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "iron_docks.h"

DoorData const doorData[] =
{
    //{GO_,       DATA_,         DOOR_TYPE_PASSAGE,    BOUNDARY_NONE},
    {0,                   0,                  DOOR_TYPE_ROOM,       BOUNDARY_NONE}, // END
};

class instance_iron_docks : public InstanceMapScript
{
public:
    instance_iron_docks() : InstanceMapScript("instance_iron_docks", 1195) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_iron_docks_InstanceMapScript(map);
    }

    struct instance_iron_docks_InstanceMapScript : public InstanceScript
    {
        instance_iron_docks_InstanceMapScript(Map* map) : InstanceScript(map) {}

        std::list<ObjectGuid> oshirGUIDconteiner;
        std::map<uint32, ObjectGuid> skullocGUIDconteiner;
        std::map<uint32, ObjectGuid> goCage;
        std::map<uint32, std::list<ObjectGuid> > goCageMobs;
        ObjectGuid enforcerGUID[3];
        uint8 enforCount;

        void Initialize()
        {
            SetBossNumber(MAX_ENCOUNTER);
            LoadDoorData(doorData);
            
            enforCount = 0;

            for (uint8 i = 0; i < 3; ++i)
                enforcerGUID[i].Clear();
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_MAKOGG_EMBERBLADE:
                    enforcerGUID[0] = creature->GetGUID();
                    break;
                case NPC_NEESA_NOX:
                    enforcerGUID[1] = creature->GetGUID();
                    break;
                case NPC_AHRIOK_DUGRU:
                    enforcerGUID[2] = creature->GetGUID();
                    break;
                case NPC_RYLAK_CAGE:
                case NPC_WOLF_CAGE:
                case NPC_RYLAK_SKYTERROR:
                case NPC_RAVENOUS_WOLF:
                    oshirGUIDconteiner.push_back(creature->GetGUID());
                    break;
                case NPC_SKULLOC:
                case NPC_KORAMAR:
                case NPC_BLACKHAND_TURRET:
                case NPC_ZOGGOSH:
                    skullocGUIDconteiner[creature->GetEntry()] = creature->GetGUID();
                    break;
            }
            for (uint8 i = 0; i < 19; ++i)
                if(creature->GetDistance(cageSpawn[i]) < 10.0f)
                    goCageMobs[i].push_back(creature->GetGUID());
        }

        void OnGameObjectCreate(GameObject* go)
        {
            /* switch (go->GetEntry())
            {
                case GO_:
                    break;
                default:
                    break;
            } */
            for (uint8 i = 0; i < 19; ++i)
                if(go->GetDistance(cageSpawn[i]) < 1.0f)
                    goCage[i] = go->GetGUID();
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;
            
            switch (type)
            {
                case DATA_G_ENFORCERS:
                {
                    switch (state)
                    {
                        case NOT_STARTED:
                        {
                            for (uint8 i = 0; i < 3; i++)
                                if (Creature* gfor = instance->GetCreature(enforcerGUID[i]))
                                {
                                    gfor->Respawn();
                                    gfor->AI()->EnterEvadeMode();
                                }
                                break;
                        }
                        case DONE:
                            if (Creature* dugu = instance->GetCreature(enforcerGUID[2]))
                                dugu->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                            break;
                    }
                    break;
                }
                case DATA_OSHIR:
                {
                    switch (state)
                    {
                        case NOT_STARTED:
                        {
                            for (std::list<ObjectGuid>::iterator itr = oshirGUIDconteiner.begin(); itr != oshirGUIDconteiner.end(); ++itr)
                                if (Creature* oshTrash = instance->GetCreature(*itr))
                                {
                                    oshTrash->Respawn();
                                    oshTrash->AI()->EnterEvadeMode();
                                    oshTrash->NearTeleportTo(oshTrash->GetHomePosition().GetPositionX(), oshTrash->GetHomePosition().GetPositionY(), 
                                            oshTrash->GetHomePosition().GetPositionZ(), oshTrash->GetHomePosition().GetOrientation());
                                }
                            for (uint8 i = 0; i < 19; ++i)
                                if (GameObject* cage = instance->GetGameObject(goCage[i]))
                                    cage->SetGoState(GO_STATE_READY);
                            break;
                        }
                        case DONE:
                            break;
                    }
                    break;
                }
                case DATA_SKULLOC:
                {
                    switch (state)
                    {
                        case NOT_STARTED:
                            for (std::map<uint32, ObjectGuid>::iterator itr = skullocGUIDconteiner.begin(); itr != skullocGUIDconteiner.end(); ++itr)
                                DoBossDespawn(itr->second, FIVE_SECONDS);
                            break;
                        case IN_PROGRESS:
                            for (std::map<uint32, ObjectGuid>::iterator itr = skullocGUIDconteiner.begin(); itr != skullocGUIDconteiner.end(); ++itr)
                                if (Creature* skullocs = instance->GetCreature(itr->second))
                                    skullocs->AI()->DoZoneInCombat(skullocs, 150.0f);
                            break;
                        case DONE:
                            if (Creature* skulloc = instance->GetCreature(skullocGUIDconteiner[NPC_SKULLOC]))
                                skulloc->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                            break;
                    }
                    break;
                }
            }
            return true;
        }

        void SetData(uint32 type, uint32 data)
        {
            switch (type)
            {
                case DATA_G_ENFOR_DIED:
                {
                    enforCount = data;
                    if (enforCount == 3)
                        SetBossState(DATA_G_ENFORCERS, DONE);
                    break;
                }
                case DATA_OSHIR_CAGE:
                {
                    if (GameObject* cage = instance->GetGameObject(goCage[data]))
                        cage->SetGoState(GO_STATE_ACTIVE);

                    for (std::list<ObjectGuid>::iterator itr = goCageMobs[data].begin(); itr != goCageMobs[data].end(); ++itr)
                        if (Creature* cageMobs = instance->GetCreature(*itr))
                            cageMobs->AI()->DoZoneInCombat(cageMobs, 100.0f);
                    break;
                }
                default:
                    break;
            }
        }

        ObjectGuid GetGuidData(uint32 type) const
        {
            switch (type)
            {
                case NPC_MAKOGG_EMBERBLADE:   
                    return enforcerGUID[0];
                case NPC_NEESA_NOX:
                    return enforcerGUID[1];
                case NPC_AHRIOK_DUGRU:
                    return enforcerGUID[2];
            }
            
            std::map<uint32, ObjectGuid>::const_iterator itr = skullocGUIDconteiner.find(type);
            if (itr != skullocGUIDconteiner.end())
                return itr->second;

            return ObjectGuid::Empty;
        }

        uint32 GetData(uint32 type) const
        {
            if (type == DATA_G_ENFOR_DIED)
                return enforCount;
            return 0;
        }

        /* void Update(uint32 diff) 
        {
            // Challenge
            InstanceScript::Update(diff);
        } */
    };
};

void AddSC_instance_iron_docks()
{
    new instance_iron_docks();
}