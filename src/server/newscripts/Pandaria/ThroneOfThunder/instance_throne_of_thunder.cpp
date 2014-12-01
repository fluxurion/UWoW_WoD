//UWoWCore
//Throne of Thunder

#include "NewScriptPCH.h"
#include "VMapFactory.h"
#include "throne_of_thunder.h"

const DoorData doorData[] =
{
    {GO_JINROKH_PRE_DOOR,    DATA_STORM_CALLER,      DOOR_TYPE_PASSAGE, 0},
    {GO_JINROKH_EX_DOOR,     DATA_JINROKH,           DOOR_TYPE_PASSAGE, 0},
    {GO_HORRIDON_PRE_DOOR,   DATA_STORMBRINGER,      DOOR_TYPE_PASSAGE, 0},
    {GO_HORRIDON_EX_DOOR,    DATA_HORRIDON,          DOOR_TYPE_PASSAGE, 0},
    {GO_COUNCIL_EX_DOOR,     DATA_COUNCIL_OF_ELDERS, DOOR_TYPE_PASSAGE, 0},
    {GO_COUNCIL_EX2_DOOR,    DATA_COUNCIL_OF_ELDERS, DOOR_TYPE_PASSAGE, 0},
    {GO_TORTOS_EX_DOOR,      DATA_TORTOS,            DOOR_TYPE_PASSAGE, 0},
    {GO_TORTOS_EX2_DOOR,     DATA_TORTOS,            DOOR_TYPE_PASSAGE, 0},
    {GO_MEGAERA_EX_DOOR,     DATA_MEGAERA,           DOOR_TYPE_PASSAGE, 0},
    {GO_JI_KUN_EX_DOOR,      DATA_JI_KUN,            DOOR_TYPE_PASSAGE, 0},
    {GO_DURUMU_EX_DOOR,      DATA_DURUMU,            DOOR_TYPE_PASSAGE, 0},
    {GO_PRIMORDIUS_EX_DOOR,  DATA_PRIMORDIUS,        DOOR_TYPE_PASSAGE, 0},
    {GO_DARK_ANIMUS_EX_DOOR, DATA_DARK_ANIMUS,       DOOR_TYPE_PASSAGE, 0},
    {GO_IRON_QON_EX_DOOR,    DATA_IRON_QON,          DOOR_TYPE_PASSAGE, 0},
    {GO_TWIN_EX_DOOR,        DATA_TWIN_CONSORTS,     DOOR_TYPE_PASSAGE, 0},
    {0,                      0,                      DOOR_TYPE_PASSAGE, 0},
};

class instance_throne_of_thunder : public InstanceMapScript
{
public:
    instance_throne_of_thunder() : InstanceMapScript("instance_throne_of_thunder", 1098) { }

    struct instance_throne_of_thunder_InstanceMapScript : public InstanceScript
    {
        instance_throne_of_thunder_InstanceMapScript(Map* map) : InstanceScript(map) {}
        
        //GameObjects
        ObjectGuid jinrokhpredoorGuid;
        ObjectGuid jinrokhentdoorGuid;
        ObjectGuid mogufont_sr_Guid;
        ObjectGuid mogufont_nr_Guid;
        ObjectGuid mogufont_nl_Guid;
        ObjectGuid mogufont_sl_Guid;
        ObjectGuid jinrokhexdoorGuid;
        ObjectGuid horridonpredoorGuid;
        ObjectGuid horridonentdoorGuid;
        ObjectGuid horridonexdoorGuid;
        ObjectGuid councilexdoorGuid;
        ObjectGuid councilex2doorGuid;
        ObjectGuid tortosexdoorGuid;
        ObjectGuid tortosex2doorGuid;
        ObjectGuid megaeraexdoorGuid;
        ObjectGuid jikunexdoorGuid;
        ObjectGuid durumuexdoorGuid;
        ObjectGuid primordiusentdoorGuid;
        ObjectGuid secretradendoorGuid;
        ObjectGuid primordiusexdoorGuid;
        ObjectGuid danimusentdoorGuid;
        ObjectGuid danimusexdoorGuid;
        ObjectGuid ironqonentdoorGuid;
        ObjectGuid ironqonexdoorGuid;
        ObjectGuid twinentdoorGuid;
        ObjectGuid twinexdoorGuid;
        ObjectGuid radenentdoorGuid;
        
        //Creature
        ObjectGuid stormcallerGuid;
        ObjectGuid jinrokhGuid;
        ObjectGuid stormbringerGuid;
        ObjectGuid horridonGuid;
        ObjectGuid jalakGuid;
        ObjectGuid mallakGuid;
        ObjectGuid marliGuid;
        ObjectGuid kazrajinGuid;
        ObjectGuid sulGuid;
        ObjectGuid tortosGuid;
        ObjectGuid flameheadGuid;
        ObjectGuid frozenheadGuid;
        ObjectGuid venousheadGuid;
        ObjectGuid jikunGuid;
        ObjectGuid durumuGuid;
        ObjectGuid primordiusGuid;
        ObjectGuid darkanimusGuid;
        ObjectGuid ironqonGuid;
        ObjectGuid roshakGuid;
        ObjectGuid quetzalGuid;
        ObjectGuid damrenGuid;
        ObjectGuid sulinGuid;
        ObjectGuid lulinGuid;
        ObjectGuid leishenGuid;
        ObjectGuid radenGuid;
        ObjectGuid canimaGuid;
        ObjectGuid cvitaGuid;

        GuidVector councilGuids;
        GuidVector mogufontsGuids;
        GuidVector councilentdoorGuids;
        GuidVector jikunfeatherGuids;
        GuidVector massiveanimagolemGuids;
        GuidVector twinfencedoorGuids;
        
        void Initialize()
        {
            SetBossNumber(16);
            LoadDoorData(doorData);

            //GameObject
            jinrokhentdoorGuid.Clear();
            mogufont_sr_Guid.Clear();
            mogufont_nr_Guid.Clear();
            mogufont_nl_Guid.Clear();
            mogufont_sl_Guid.Clear();
            jinrokhexdoorGuid.Clear();
            horridonpredoorGuid.Clear();
            horridonentdoorGuid.Clear();
            horridonexdoorGuid.Clear();
            councilexdoorGuid.Clear();
            councilex2doorGuid.Clear();
            tortosexdoorGuid.Clear();
            tortosex2doorGuid.Clear();
            megaeraexdoorGuid.Clear();
            jikunexdoorGuid.Clear();
            durumuexdoorGuid.Clear();
            primordiusentdoorGuid.Clear();
            secretradendoorGuid.Clear();
            primordiusexdoorGuid.Clear();
            danimusentdoorGuid.Clear();
            danimusexdoorGuid.Clear();
            ironqonentdoorGuid.Clear();
            ironqonexdoorGuid.Clear();
            twinentdoorGuid.Clear();
            twinexdoorGuid.Clear();
            radenentdoorGuid.Clear();
           
            //Creature
            stormcallerGuid.Clear();
            jinrokhGuid.Clear();
            stormbringerGuid.Clear();
            horridonGuid.Clear();
            jalakGuid.Clear();
            mallakGuid.Clear();
            marliGuid.Clear();
            kazrajinGuid.Clear();
            sulGuid.Clear();
            tortosGuid.Clear();
            flameheadGuid.Clear();
            frozenheadGuid.Clear();
            venousheadGuid.Clear();
            jikunGuid.Clear();
            durumuGuid.Clear();
            primordiusGuid.Clear();
            darkanimusGuid.Clear();
            ironqonGuid.Clear();
            roshakGuid.Clear();
            quetzalGuid.Clear();
            damrenGuid.Clear();
            sulinGuid.Clear();
            lulinGuid.Clear();
            leishenGuid.Clear();
            radenGuid.Clear();
            canimaGuid.Clear();
            cvitaGuid.Clear();

            councilGuids.clear();
            mogufontsGuids.clear();
            councilentdoorGuids.clear();
            jikunfeatherGuids.clear();
            massiveanimagolemGuids.clear();
            twinfencedoorGuids.clear();
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
            case NPC_STORM_CALLER:
                stormcallerGuid = creature->GetGUID();
                break;
            case NPC_JINROKH:
                jinrokhGuid = creature->GetGUID();
                break;
            case NPC_STORMBRINGER:
                stormbringerGuid = creature->GetGUID();
                break;
            case NPC_HORRIDON: 
                horridonGuid = creature->GetGUID();
                break;
            case NPC_JALAK:
                jalakGuid = creature->GetGUID();
                break;
            //Council of Elders
            case NPC_FROST_KING_MALAKK:
                mallakGuid = creature->GetGUID();
                councilGuids.push_back(creature->GetGUID());
                break;
            case NPC_PRINCESS_MARLI:
                marliGuid = creature->GetGUID();
                councilGuids.push_back(creature->GetGUID());
                break;  
            case NPC_KAZRAJIN:  
                kazrajinGuid = creature->GetGUID();
                councilGuids.push_back(creature->GetGUID());
                break;
            case NPC_SUL_SANDCRAWLER: 
                sulGuid = creature->GetGUID();
                councilGuids.push_back(creature->GetGUID());
                break;
            //
            case NPC_TORTOS: 
                tortosGuid = creature->GetGUID();
                break;
            //Megaera
            case NPC_FLAMING_HEAD: 
                flameheadGuid = creature->GetGUID();
                break;
            case NPC_FROZEN_HEAD:
                frozenheadGuid = creature->GetGUID();
                break;
            case NPC_VENOMOUS_HEAD:
                venousheadGuid = creature->GetGUID();
                break;
            //
            case NPC_JI_KUN:  
                jikunGuid = creature->GetGUID();
                break;
            case NPC_DURUMU:  
                durumuGuid = creature->GetGUID();
                break;
            case NPC_PRIMORDIUS: 
                primordiusGuid = creature->GetGUID();
                break;
            case NPC_MASSIVE_ANIMA_GOLEM:
                massiveanimagolemGuids.push_back(creature->GetGUID());
                break;
            case NPC_DARK_ANIMUS:  
                darkanimusGuid = creature->GetGUID();
                break;
            case NPC_IRON_QON:
                ironqonGuid = creature->GetGUID();
                break;
            case NPC_ROSHAK:
                roshakGuid = creature->GetGUID();
                break;
            case NPC_QUETZAL:
                quetzalGuid = creature->GetGUID();
                break;
            case NPC_DAMREN:
                damrenGuid = creature->GetGUID();
                break;
            //Twin consorts
            case NPC_SULIN:   
                sulinGuid = creature->GetGUID();
                break;
            case NPC_LULIN: 
                lulinGuid = creature->GetGUID();
                break;
            //
            case NPC_LEI_SHEN:
                leishenGuid = creature->GetGUID();
                break;
            case NPC_RA_DEN:
                radenGuid = creature->GetGUID();
                break;
            case NPC_CORRUPTED_ANIMA:
                canimaGuid = creature->GetGUID();
                break;
            case NPC_CORRUPTED_VITA:
                cvitaGuid = creature->GetGUID();
                break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {    
            switch (go->GetEntry())
            {
            case GO_JINROKH_PRE_DOOR:
                AddDoor(go, true);
                jinrokhpredoorGuid = go->GetGUID();
                break;
            case GO_JINROKH_ENT_DOOR:
                jinrokhentdoorGuid = go->GetGUID();
                break;
            //Mogu Fonts
            case GO_MOGU_SR:
                mogufont_sr_Guid = go->GetGUID();
                mogufontsGuids.push_back(go->GetGUID());
                break;
            case GO_MOGU_NR:
                mogufont_nr_Guid = go->GetGUID();
                mogufontsGuids.push_back(go->GetGUID());
                break;
            case GO_MOGU_NL:
                mogufont_nl_Guid = go->GetGUID();
                mogufontsGuids.push_back(go->GetGUID());
                break;
            case GO_MOGU_SL:
                mogufont_sl_Guid = go->GetGUID();
                mogufontsGuids.push_back(go->GetGUID());
                break;
            //
            case GO_JINROKH_EX_DOOR:
                AddDoor(go, true);
                jinrokhexdoorGuid = go->GetGUID();
                break;
            case GO_HORRIDON_PRE_DOOR:
                AddDoor(go, true);
                horridonpredoorGuid = go->GetGUID();
                break;
            case GO_HORRIDON_ENT_DOOR:
                horridonentdoorGuid = go->GetGUID();
                break;
            case GO_HORRIDON_EX_DOOR:
                AddDoor(go, true);
                horridonexdoorGuid = go->GetGUID();
                break;
            case GO_COUNCIL_LENT_DOOR:
                councilentdoorGuids.push_back(go->GetGUID());
                break;
            case GO_COUNCIL_RENT_DOOR:
                councilentdoorGuids.push_back(go->GetGUID());
                break;
            case GO_COUNCIL_EX_DOOR:
                AddDoor(go, true);
                councilexdoorGuid = go->GetGUID();
                break;
            case GO_COUNCIL_EX2_DOOR:
                AddDoor(go, true);
                councilex2doorGuid = go->GetGUID();
                break;
            case GO_TORTOS_EX_DOOR:
                AddDoor(go, true);
                tortosexdoorGuid = go->GetGUID();
                break;
            case GO_TORTOS_EX2_DOOR:
                AddDoor(go, true);
                tortosex2doorGuid = go->GetGUID();
                break;
            case GO_MEGAERA_EX_DOOR:
                AddDoor(go, true);
                megaeraexdoorGuid = go->GetGUID();
                break;
            case GO_JI_KUN_FEATHER:
                jikunfeatherGuids.push_back(go->GetGUID());
                break;
            case GO_JI_KUN_EX_DOOR:
                AddDoor(go, true);
                jikunexdoorGuid = go->GetGUID();
                break;
            case GO_DURUMU_EX_DOOR:
                AddDoor(go, true);
                durumuexdoorGuid = go->GetGUID();
                break;
            case GO_PRIMORDIUS_ENT_DOOR:
                primordiusentdoorGuid = go->GetGUID();
                break;
            case GO_S_RA_DEN_ENT_DOOR:
                LoadSecretRaDenDoor(go);
                secretradendoorGuid = go->GetGUID();
            case GO_PRIMORDIUS_EX_DOOR:
                AddDoor(go, true);
                primordiusexdoorGuid = go->GetGUID();
                break;
            case GO_DARK_ANIMUS_ENT_DOOR:
                danimusentdoorGuid = go->GetGUID();
                break;
            case GO_DARK_ANIMUS_EX_DOOR:
                AddDoor(go, true);
                danimusexdoorGuid = go->GetGUID();
                break;
            case GO_IRON_QON_ENT_DOOR:
                ironqonentdoorGuid = go->GetGUID();
                break;
            case GO_IRON_QON_EX_DOOR:
                AddDoor(go, true);
                ironqonexdoorGuid = go->GetGUID();
                break;
            case GO_TWIN_ENT_DOOR:
                twinentdoorGuid = go->GetGUID();
                break;
            case GO_TWIN_FENCE_DOOR:
                twinfencedoorGuids.push_back(go->GetGUID());
                break;
            case GO_TWIN_FENCE_DOOR_2:
                twinfencedoorGuids.push_back(go->GetGUID());
                break;
            case GO_TWIN_EX_DOOR:
                AddDoor(go, true);
                twinexdoorGuid = go->GetGUID();
                break;
            case GO_RA_DEN_ENT_DOOR:
                radenentdoorGuid = go->GetGUID();
                break;
            default:
                break;
            }
        }

        bool SetBossState(uint32 id, EncounterState state)
        {
            if (!InstanceScript::SetBossState(id, state))
                return false;

            switch (id)
            {
            case DATA_STORM_CALLER:
                if (state == DONE)
                    HandleGameObject(jinrokhpredoorGuid, true);
                break;
            case DATA_JINROKH:
                {
                    switch (state)
                    {
                    case NOT_STARTED:
                        for (GuidVector::const_iterator guid = mogufontsGuids.begin(); guid != mogufontsGuids.end(); guid++)
                            HandleGameObject(*guid, false);
                        HandleGameObject(jinrokhentdoorGuid, true);
                        break;
                    case IN_PROGRESS:
                        HandleGameObject(jinrokhentdoorGuid, false);
                        break;
                    case DONE:
                        HandleGameObject(jinrokhentdoorGuid, true);
                        HandleGameObject(jinrokhexdoorGuid, true); 
                        break;
                    }
                }
                break;
            case DATA_STORMBRINGER:
                if (state == DONE)
                    HandleGameObject(horridonpredoorGuid, true);
                break;
            case DATA_HORRIDON:
                {
                    switch (state)
                    {
                    case NOT_STARTED:
                        HandleGameObject(horridonentdoorGuid, true);
                        break;
                    case IN_PROGRESS:
                        HandleGameObject(horridonentdoorGuid, false);
                        break;
                    case DONE:
                        HandleGameObject(horridonentdoorGuid, true);
                        HandleGameObject(horridonexdoorGuid, true);
                        break;
                    }
                }
                break;
            case DATA_COUNCIL_OF_ELDERS:
                {
                    switch (state)
                    {
                    case NOT_STARTED:
                        //Reset all council
                        for (GuidVector::const_iterator guid = councilGuids.begin(); guid != councilGuids.end(); guid++)
                        {
                            if (Creature* council = instance->GetCreature(*guid))
                            {
                                if (council->isAlive() && council->isInCombat())
                                    council->AI()->EnterEvadeMode();
                                else if (!council->isAlive())
                                {
                                    council->Respawn();
                                    council->GetMotionMaster()->MoveTargetedHome();
                                }
                                SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, council);
                            }
                        }

                        for (GuidVector::const_iterator guids = councilentdoorGuids.begin(); guids != councilentdoorGuids.end(); guids++)
                            HandleGameObject(*guids, true);
                        break;
                    case IN_PROGRESS:
                        //Call all council 
                        for (GuidVector::const_iterator guid = councilGuids.begin(); guid != councilGuids.end(); guid++)
                        {
                            if (Creature* council = instance->GetCreature(*guid))
                            {
                                if (council->isAlive() && !council->isInCombat())
                                    council->AI()->DoZoneInCombat(council, 150.0f);
                                SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, council);
                            }
                        }

                        for (GuidVector::const_iterator guids = councilentdoorGuids.begin(); guids != councilentdoorGuids.end(); guids++)
                            HandleGameObject(*guids, false);
                        break;
                    case DONE:
                        for (GuidVector::const_iterator guids = councilentdoorGuids.begin(); guids != councilentdoorGuids.end(); guids++)
                            HandleGameObject(*guids, true);
                        HandleGameObject(councilexdoorGuid, true);
                        HandleGameObject(councilex2doorGuid, true);
                        break;
                    }
                }
                break;
            case DATA_TORTOS:
                if (state == DONE)
                {
                    HandleGameObject(tortosexdoorGuid, true);
                    HandleGameObject(tortosex2doorGuid, true);
                }
                break;
            case DATA_MEGAERA:
                if (state == DONE)
                    HandleGameObject(megaeraexdoorGuid, true);
                break;
            case DATA_JI_KUN:
                {
                    switch (state)
                    {
                    case NOT_STARTED:
                    case DONE:
                        HandleGameObject(megaeraexdoorGuid, true);
                        HandleGameObject(jikunexdoorGuid, true);
                        for (GuidVector::const_iterator guid = jikunfeatherGuids.begin(); guid != jikunfeatherGuids.end(); guid++)
                            if (GameObject* feather = instance->GetGameObject(*guid))
                                feather->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);
                        break;
                    case IN_PROGRESS:
                        HandleGameObject(megaeraexdoorGuid, false);
                        for (GuidVector::const_iterator guid = jikunfeatherGuids.begin(); guid != jikunfeatherGuids.end(); guid++)
                            if (GameObject* feather = instance->GetGameObject(*guid))
                                feather->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);
                        break;
                    }
                }
                break;
            case DATA_DURUMU: 
                if (state == DONE)
                    HandleGameObject(durumuexdoorGuid, true);
                break;
            case DATA_PRIMORDIUS:
                {
                    switch (state)
                    {
                    case NOT_STARTED:
                        HandleGameObject(primordiusentdoorGuid, true);
                        break;
                    case IN_PROGRESS:
                        HandleGameObject(primordiusentdoorGuid, false);
                        break;
                    case DONE:
                        HandleGameObject(primordiusentdoorGuid, true);
                        HandleGameObject(primordiusexdoorGuid, true);
                        break;
                    }
                }
                break;
            case DATA_DARK_ANIMUS:
                {
                    switch (state)
                    {
                    case NOT_STARTED:
                        for (GuidVector::const_iterator guid = massiveanimagolemGuids.begin(); guid != massiveanimagolemGuids.end(); guid++)
                        {
                            if (Creature* mag = instance->GetCreature(*guid))
                            {
                                if (mag->isAlive() && mag->isInCombat())
                                    mag->AI()->EnterEvadeMode();
                                else if (!mag->isAlive())
                                {
                                    mag->Respawn();
                                    mag->GetMotionMaster()->MoveTargetedHome();
                                }
                            }
                        }
                        HandleGameObject(danimusentdoorGuid, true);
                        break;
                    case IN_PROGRESS:
                        if (Creature* animus = instance->GetCreature(darkanimusGuid))
                        {
                            if (animus->isAlive() && !animus->isInCombat())
                                animus->AI()->DoZoneInCombat(animus, 150.0f);
                        }

                        for (GuidVector::const_iterator guid = massiveanimagolemGuids.begin(); guid != massiveanimagolemGuids.end(); guid++)
                        {
                            if (Creature* mag = instance->GetCreature(*guid))
                            {
                                if (mag->isAlive() && !mag->isInCombat())
                                    mag->AI()->DoZoneInCombat(mag, 150.0f);
                            }
                        }
                        HandleGameObject(danimusentdoorGuid, false);
                        break;
                    case DONE:
                        HandleGameObject(danimusentdoorGuid, true);
                        HandleGameObject(danimusexdoorGuid, true);
                        break;
                    }
                }
                break;
            case DATA_IRON_QON:
                {
                    switch (state)
                    {
                    case NOT_STARTED:
                        HandleGameObject(ironqonentdoorGuid, true);
                        break;
                    case IN_PROGRESS:
                        HandleGameObject(ironqonentdoorGuid, false);
                        break;
                    case DONE:
                        HandleGameObject(ironqonentdoorGuid, true);
                        HandleGameObject(ironqonexdoorGuid, true);
                        break;
                    }
                }
                break;
            case DATA_TWIN_CONSORTS:
                {
                    switch (state)
                    {
                        case NOT_STARTED:
                            for (GuidVector::const_iterator guid = twinfencedoorGuids.begin(); guid != twinfencedoorGuids.end(); guid++)
                                HandleGameObject(*guid, true);
                            HandleGameObject(twinentdoorGuid, true);
                            break;
                        case IN_PROGRESS:
                            for (GuidVector::const_iterator guid = twinfencedoorGuids.begin(); guid != twinfencedoorGuids.end(); guid++)
                                HandleGameObject(*guid, false);
                            HandleGameObject(twinentdoorGuid, false);
                            break;
                        case DONE:
                            for (GuidVector::const_iterator guid = twinfencedoorGuids.begin(); guid != twinfencedoorGuids.end(); guid++)
                                HandleGameObject(*guid, true);
                            HandleGameObject(twinentdoorGuid, true);
                            HandleGameObject(twinexdoorGuid, true);
                            break;                         
                    }
                }
                break;
            case DATA_RA_DEN:
                {
                    switch (state)
                    {
                    case NOT_STARTED:
                    case DONE:
                        HandleGameObject(radenentdoorGuid, true);
                        break;
                    case IN_PROGRESS:
                        HandleGameObject(radenentdoorGuid, false);
                        break;
                    }
                }
                break;
            default:
                break;
            }
           
            if (state == DONE && id != DATA_RA_DEN)
            {
                if (GameObject* go = instance->GetGameObject(secretradendoorGuid))
                    LoadSecretRaDenDoor(go);
            }
            return true;
        }

        void LoadSecretRaDenDoor(GameObject* go)
        {
            if (!go || !instance->IsHeroic())
                return;

            if (InstanceMap* im = instance->ToInstanceMap())
            {
                InstanceScript* pinstance = im->GetInstanceScript();
                for (uint8 n = DATA_STORM_CALLER; n <= DATA_LEI_SHEN; n++)
                {
                    if (pinstance->GetBossState(n) != DONE)
                        return;
                }
                go->SetGoState(GO_STATE_ACTIVE);
            }
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
            case NPC_JINROKH:
                return jinrokhGuid;
            case NPC_HORRIDON: 
                return horridonGuid;
            case NPC_JALAK:
                return jalakGuid;
            //Council of Elders
            case NPC_FROST_KING_MALAKK:
                return mallakGuid;
            case NPC_PRINCESS_MARLI:
                return marliGuid;
            case NPC_KAZRAJIN:  
                return kazrajinGuid;
            case NPC_SUL_SANDCRAWLER: 
                return sulGuid;
            //
            case NPC_TORTOS: 
                return tortosGuid;
            //Megaera
            case NPC_FLAMING_HEAD: 
                return flameheadGuid;
            case NPC_FROZEN_HEAD:
                return frozenheadGuid;
            case NPC_VENOMOUS_HEAD:
                return venousheadGuid;
            //
            case NPC_JI_KUN:  
                return jikunGuid;
            case NPC_DURUMU:  
                return durumuGuid;
            case NPC_PRIMORDIUS: 
                return primordiusGuid;
            case NPC_DARK_ANIMUS:  
                return darkanimusGuid;
            case NPC_IRON_QON:
                return ironqonGuid;
            //Iron Qon Maunts
            case NPC_ROSHAK:
                return roshakGuid;
            case NPC_QUETZAL:
                return quetzalGuid;
            case NPC_DAMREN:
                return damrenGuid;
            //
            //Twin consorts
            case NPC_SULIN:   
                return sulinGuid;
            case NPC_LULIN: 
                return lulinGuid;
            //
            case NPC_LEI_SHEN:
                return leishenGuid;
            case NPC_RA_DEN:
                return radenGuid;
            case NPC_CORRUPTED_ANIMA:
                return canimaGuid;
            case NPC_CORRUPTED_VITA:
                return cvitaGuid;
            }
            return ObjectGuid::Empty;
        }
        
        bool IsWipe()
        {
            Map::PlayerList const& PlayerList = instance->GetPlayers();

            if (PlayerList.isEmpty())
                return true;

            for (Map::PlayerList::const_iterator Itr = PlayerList.begin(); Itr != PlayerList.end(); ++Itr)
            {
                Player* player = Itr->getSource();

                if (!player)
                    continue;

                if (player->isAlive() && !player->isGameMaster() && !player->HasAura(115877)) // Aura 115877 = Totaly Petrified
                    return false;
            }

            return true;
        }

        std::string GetSaveData()
        {
            std::ostringstream saveStream;
            saveStream << GetBossSaveData() << " ";
            return saveStream.str();
        }

        void Load(const char* data)
        {
            std::istringstream loadStream(LoadBossState(data));
            uint32 buff;
            for (uint32 i = 0; i < 16; ++i)
                loadStream >> buff;
        }
    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_throne_of_thunder_InstanceMapScript(map);
    }
};

enum sSpells
{
    SPELL_STORM_WEAPON   = 139319,
    SPELL_STORM_ENERGY   = 139322,
    SPELL_CHAIN_LIGHTNIG = 139903,
    SPELL_STORMCLOUD     = 139900,
};

enum sEvent
{
    EVENT_STORM_ENERGY   = 1,
    EVENT_CHAIN_LIGHTNIG = 2,
};

//Mini boss, guard Jinrokh entrance
class npc_storm_caller : public CreatureScript
{
    public:
        npc_storm_caller() : CreatureScript("npc_storm_caller") { }
        
        struct npc_storm_callerAI : public BossAI
        {
            npc_storm_callerAI(Creature* pCreature) : BossAI(pCreature, DATA_STORM_CALLER)
            {
                pInstance = pCreature->GetInstanceScript();
            }
            
            InstanceScript* pInstance;

            void Reset()
            {
                _Reset();
                me->RemoveAurasDueToSpell(SPELL_STORM_WEAPON);
            }

            void EnterCombat(Unit* who)
            {
                _EnterCombat();
                me->AddAura(SPELL_STORM_WEAPON, me);
                events.ScheduleEvent(EVENT_STORM_ENERGY, urand(15000, 20000));
            }

            void JustDied(Unit* killer)
            {
                _JustDied();
            }
            
            void UpdateAI(uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_STORM_ENERGY)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.0f, true))
                            DoCast(target, SPELL_STORM_ENERGY);
                        events.ScheduleEvent(EVENT_STORM_ENERGY, urand(15000, 20000));
                    }
                }
                DoMeleeAttackIfReady();
            }
        };
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_storm_callerAI(pCreature);
        }
};

//Mini boss, guard Horridon entrance
class npc_stormbringer : public CreatureScript
{
    public:
        npc_stormbringer() : CreatureScript("npc_stormbringer") { }
        
        struct npc_stormbringerAI : public BossAI
        {
            npc_stormbringerAI(Creature* pCreature) : BossAI(pCreature, DATA_STORMBRINGER)
            {
                pInstance = pCreature->GetInstanceScript();
            }
            
            InstanceScript* pInstance;

            void Reset()
            {
                _Reset();
                me->RemoveAurasDueToSpell(SPELL_STORMCLOUD);
            }

            void EnterCombat(Unit* who)
            {
                _EnterCombat();
                DoCast(me, SPELL_STORMCLOUD);
                events.ScheduleEvent(EVENT_CHAIN_LIGHTNIG, urand(15000, 20000));
            }

            void JustDied(Unit* killer)
            {
                _JustDied();
            }
            
            void UpdateAI(uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_CHAIN_LIGHTNIG)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.0f, true))
                            DoCast(target, SPELL_CHAIN_LIGHTNIG);
                        events.ScheduleEvent(EVENT_STORM_ENERGY, urand(15000, 20000));
                    }
                }
                DoMeleeAttackIfReady();
            }
        };
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_stormbringerAI(pCreature);
        }
};

Position const onbridge    = {6045.42f, 5163.28f, 148.1146f, 1.548f};
Position const underbridge = {6042.31f, 5088.96f,  -43.152f, 4.654f};

//Te;eport to Tortos, and back
class npc_teleporter : public CreatureScript
{
    public:
        npc_teleporter() : CreatureScript("npc_teleporter") {}

        struct npc_teleporterAI : public CreatureAI
        {
            npc_teleporterAI(Creature* creature) : CreatureAI(creature)
            {
                instance = creature->GetInstanceScript();
                me->AddAura(126493, me); //Visual
            }

            InstanceScript* instance;

            void Reset(){}
            
            void OnSpellClick(Unit* clicker)
            {
                if (instance)
                {
                   if (clicker->GetTypeId() == TYPEID_PLAYER)
                   {
                       if (me->GetPositionZ() > 140.0f)
                           clicker->NearTeleportTo(underbridge.GetPositionX(), underbridge.GetPositionY(), underbridge.GetPositionZ(), underbridge.GetOrientation());
                       else
                           clicker->NearTeleportTo(onbridge.GetPositionX(), onbridge.GetPositionY(), onbridge.GetPositionZ(), onbridge.GetOrientation());
                   }
                }
            }
            
            void EnterEvadeMode(){}

            void EnterCombat(Unit* who){}

            void UpdateAI(uint32 diff){}
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_teleporterAI(creature);
        }
};

void AddSC_instance_throne_of_thunder()
{
    new instance_throne_of_thunder();
    new npc_storm_caller();
    new npc_stormbringer();
    new npc_teleporter();
}
