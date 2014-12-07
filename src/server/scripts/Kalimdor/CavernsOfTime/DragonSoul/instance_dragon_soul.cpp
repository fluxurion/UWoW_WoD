#include "ScriptPCH.h"
#include "dragon_soul.h"

#define MAX_ENCOUNTER 8

class instance_dragon_soul : public InstanceMapScript
{
    public:
        instance_dragon_soul() : InstanceMapScript("instance_dragon_soul", 967) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_dragon_soul_InstanceMapScript(map);
        }

        struct instance_dragon_soul_InstanceMapScript : public InstanceScript
        {
            instance_dragon_soul_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(MAX_ENCOUNTER);

                uiMorchokGUID.Clear();
                uiKohcromGUID.Clear();
                uiValeeraGUID.Clear();
                uiEiendormiGUID.Clear();
            }

            void OnPlayerEnter(Player* pPlayer)
            {
                if (!uiTeamInInstance)
                    uiTeamInInstance = pPlayer->GetTeam();
            }

            void OnCreatureCreate(Creature* pCreature)
            {
                switch (pCreature->GetEntry())
                {
                    case NPC_MORCHOK:
                        uiMorchokGUID = pCreature->GetGUID();
                        break;
                    case NPC_KOHCROM:
                        uiKohcromGUID = pCreature->GetGUID();
                        break;
                    case NPC_TRAVEL_TO_WYRMREST_TEMPLE:
                        teleportGUIDs.push_back(pCreature->GetGUID());
                        break;
                    case NPC_VALEERA:
                        uiValeeraGUID = pCreature->GetGUID();
                        break;
                    case NPC_EIENDORMI:
                        uiEiendormiGUID = pCreature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void SetData(uint32 type, uint32 data)
            {
            }

            uint32 GetData(uint32 type)
            {
                return 0;
            }

            ObjectGuid GetGuidData(uint32 type)
            {
                switch (type)
                {
                    case DATA_MORCHOK: return uiMorchokGUID;
                    case DATA_KOHCROM: return uiKohcromGUID;
                    case NPC_VALEERA: return uiValeeraGUID;
                    case NPC_EIENDORMI: return uiEiendormiGUID;
                    default: return ObjectGuid::Empty;
                }
                return ObjectGuid::Empty;
            }

            bool SetBossState(uint32 type, EncounterState state)
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                if (state == IN_PROGRESS)
                {
                    if (!teleportGUIDs.empty())
                        for (GuidVector::const_iterator itr = teleportGUIDs.begin(); itr != teleportGUIDs.end(); ++itr)
                            if (Creature* pTeleport = instance->GetCreature((*itr)))
                            {
                                pTeleport->RemoveAura(SPELL_TELEPORT_VISUAL_ACTIVE);
                                pTeleport->CastSpell(pTeleport, SPELL_TELEPORT_VISUAL_DISABLED, true);
                            }
                }
                else
                {
                    if (!teleportGUIDs.empty())
                        for (GuidVector::const_iterator itr = teleportGUIDs.begin(); itr != teleportGUIDs.end(); ++itr)
                            if (Creature* pTeleport = instance->GetCreature((*itr)))
                            {
                                pTeleport->RemoveAura(SPELL_TELEPORT_VISUAL_DISABLED);
                                pTeleport->CastSpell(pTeleport, SPELL_TELEPORT_VISUAL_ACTIVE, true);
                            }
                }


                return true;
            }

            std::string GetSaveData()
            {
                OUT_SAVE_INST_DATA;

                std::string str_data;

                std::ostringstream saveStream;
                saveStream << "D S " << GetBossSaveData();

                str_data = saveStream.str();

                OUT_SAVE_INST_DATA_COMPLETE;
                return str_data;
            }

            void Load(const char* in)
            {
                if (!in)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(in);

                char dataHead1, dataHead2;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'D' && dataHead2 == 'S')
                {
                    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }} else OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

            private:
                uint32 uiTeamInInstance;

                ObjectGuid uiMorchokGUID;
                ObjectGuid uiKohcromGUID;
                ObjectGuid uiValeeraGUID;
                ObjectGuid uiEiendormiGUID;

                GuidVector teleportGUIDs;
               
        };
};


void WhisperToAllPlayerInZone(int32 TextId, Creature* sender)
{
    Map::PlayerList const &players = sender->GetMap()->GetPlayers();
    for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
        if (Player* player = i->getSource())
            DoScriptText(TextId, sender, player);
}
            
void AddSC_instance_dragon_soul()
{
    new instance_dragon_soul();
}