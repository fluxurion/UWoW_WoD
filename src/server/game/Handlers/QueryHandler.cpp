/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Common.h"
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "MapManager.h"
#include "GossipDef.h"
#include "CharacterPackets.h"
#include "QueryPackets.h"
#include "BattlenetAccountMgr.h"
#include "CharacterPackets.h"
#include "QueryPackets.h"

void WorldSession::SendNameQueryOpcode(ObjectGuid guid)
{
    Player* player = ObjectAccessor::FindPlayer(guid);

    WorldPackets::Query::QueryPlayerNameResponse response;
    response.Player = guid;

    if (response.Data.Initialize(guid, player))
        response.Result = RESPONSE_SUCCESS; // name known
    else
        response.Result = RESPONSE_FAILURE; // name unknown

    SendPacket(response.Write());
}


void WorldSession::HandleNameQueryOpcode(WorldPackets::Query::QueryPlayerName& packet)
{
    SendNameQueryOpcode(packet.Player);
}

void WorldSession::HandleQueryTimeOpcode(WorldPacket & /*recvData*/)
{
    SendQueryTimeResponse();
}

//603
void WorldSession::SendQueryTimeResponse()
{
    WorldPacket data(SMSG_QUERY_TIME_RESPONSE, 4+4);
    data << uint32(time(NULL));
    data << uint32(sWorld->GetNextDailyQuestsResetTime() - time(NULL));
    SendPacket(&data);
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleCreatureQuery(WorldPackets::Query::QueryCreature& packet)
{
    WorldPackets::Query::QueryCreatureResponse response;

    CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(packet.CreatureID);

    response.CreatureID = packet.CreatureID;

    if (creatureInfo)
    {
        response.Allow = true;

        WorldPackets::Query::CreatureStats& stats = response.Stats;

        stats.Title = creatureInfo->SubName;
        stats.CursorName = creatureInfo->IconName;
        stats.CreatureType = creatureInfo->type;
        stats.CreatureFamily = creatureInfo->family;
        stats.Classification = creatureInfo->rank;
        stats.HpMulti = creatureInfo->ModHealth;
        stats.EnergyMulti = creatureInfo->ModMana;
        stats.Leader = creatureInfo->RacialLeader;
        for (uint8 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            if (creatureInfo->questItems[i])
                stats.QuestItems.push_back(creatureInfo->questItems[i]);
        stats.CreatureMovementInfoID = creatureInfo->movementId;
        stats.RequiredExpansion = creatureInfo->expansionUnknown;
        stats.Flags[0] = creatureInfo->type_flags;
        stats.Flags[1] = creatureInfo->type_flags2;
        for (uint32 i = 0; i < MAX_KILL_CREDIT; ++i)
            stats.ProxyCreatureID[i] = creatureInfo->KillCredit[i];
        stats.CreatureDisplayID[0] = creatureInfo->Modelid1;
        stats.CreatureDisplayID[1] = creatureInfo->Modelid2;
        stats.CreatureDisplayID[2] = creatureInfo->Modelid3;
        stats.CreatureDisplayID[3] = creatureInfo->Modelid4;
        if (CreatureLocale const* cl = sObjectMgr->GetCreatureLocale(response.CreatureID))
        {
            if (cl->Name.size() > GetSessionDbLocaleIndex() && !cl->Name[GetSessionDbLocaleIndex()].empty())
                stats.Name[0] = cl->Name[GetSessionDbLocaleIndex()];

            if (cl->SubName.size() > GetSessionDbLocaleIndex() && !cl->SubName[GetSessionDbLocaleIndex()].empty())
                stats.Title = cl->SubName[GetSessionDbLocaleIndex()];
        }

        {
            if (stats.Name[0].empty())
                stats.Name[0] = creatureInfo->Name;
            if (stats.NameAlt[0].empty())
                stats.NameAlt[0] = creatureInfo->SubName;
        }
    }
    else
        response.Allow = false;

    SendPacket(response.Write());
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleGameObjectQueryOpcode(WorldPackets::Query::QueryGameObject& packet)
{
    WorldPackets::Query::QueryGameObjectResponse response;

    response.GameObjectID = packet.GameObjectID;

    if (GameObjectTemplate const* gameObjectInfo = sObjectMgr->GetGameObjectTemplate(packet.GameObjectID))
    {
        response.Allow = true;
        WorldPackets::Query::GameObjectStats& stats = response.Stats;

        stats.CastBarCaption = gameObjectInfo->castBarCaption;
        stats.DisplayID = gameObjectInfo->displayId;

        if (GameObjectLocale const* cl = sObjectMgr->GetGameObjectLocale(response.GameObjectID))
        {
            if (cl->Name.size() > GetSessionDbLocaleIndex() && !cl->Name[GetSessionDbLocaleIndex()].empty())
                stats.Name[0] = cl->Name[GetSessionDbLocaleIndex()];
        }

        stats.IconName = gameObjectInfo->IconName;
        stats.Name[0] = gameObjectInfo->name;

        for (uint8 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; i++)
            if (gameObjectInfo->questItems[i])
                stats.QuestItems.push_back(gameObjectInfo->questItems[i]);
        for (uint32 i = 0; i < MAX_GAMEOBJECT_DATA; i++)
            stats.Data[i] = gameObjectInfo->raw.data[i];

        stats.Size = gameObjectInfo->size;
        stats.Type = gameObjectInfo->type;
        stats.UnkString = gameObjectInfo->unk1;
        stats.Expansion = 0;
    }
    else
        response.Allow = false;

    SendPacket(response.Write());
}

void WorldSession::HandleQueryCorpseLocation(WorldPackets::Query::QueryCorpseLocationFromClient& /*packet*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_CORPSE_QUERY");

    Corpse* corpse = GetPlayer()->GetCorpse();

    if (!corpse)
    {
        WorldPackets::Query::CorpseLocation packet;
        packet.Valid = false;                               // corpse not found
        SendPacket(packet.Write());
        return;
    }

    uint32 mapid = corpse->GetMapId();
    float x = corpse->GetPositionX();
    float y = corpse->GetPositionY();
    float z = corpse->GetPositionZ();
    uint32 corpsemapid = mapid;

    // if corpse at different map
    if (mapid != _player->GetMapId())
    {
        // search entrance map for proper show entrance
        if (MapEntry const* corpseMapEntry = sMapStore.LookupEntry(mapid))
        {
            if (corpseMapEntry->IsDungeon() && corpseMapEntry->entrance_map >= 0)
            {
                // if corpse map have entrance
                if (Map const* entranceMap = sMapMgr->CreateBaseMap(corpseMapEntry->entrance_map))
                {
                    mapid = corpseMapEntry->entrance_map;
                    x = corpseMapEntry->entrance_x;
                    y = corpseMapEntry->entrance_y;
                    z = entranceMap->GetHeight(GetPlayer()->GetPhaseMask(), x, y, MAX_HEIGHT);
                }
            }
        }
    }

    WorldPackets::Query::CorpseLocation packet;
    packet.Valid = true;
    packet.MapID = corpsemapid;
    packet.ActualMapID = mapid;
    packet.Position = G3D::Vector3(x, y, z);
    packet.Transport = corpse->GetTransGUID();
    SendPacket(packet.Write());
}

void WorldSession::HandleNpcTextQueryOpcode(WorldPackets::Query::QueryNPCText& packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_QUERY_NPC_TEXT ID '%u'", packet.TextID);

    GossipText const* gossip = sObjectMgr->GetGossipText(packet.TextID);

    WorldPackets::Query::QueryNPCTextResponse response;
    response.TextID = packet.TextID;

    if (gossip)
    {
        for (uint8 i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            response.Probabilities[i] = i == 0 ? 1 : 0/*gossip->Options[i].Probability*/;
            response.BroadcastTextID[i] = i == 0 ? packet.TextID : 0/*gossip->Options[i].BroadcastTextID*/;
        }

        response.Allow = true;
    }

    SendPacket(response.Write());

    /*if (!pGossip)
    {
        for (uint32 i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << float(0);
            data << "Greetings $N";
            data << "Greetings $N";
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
        }
    }
    else
    {
        std::string Text_0[MAX_LOCALES], Text_1[MAX_LOCALES];
        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            Text_0[i]=pGossip->Options[i].Text_0;
            Text_1[i]=pGossip->Options[i].Text_1;
        }

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (NpcTextLocale const* nl = sObjectMgr->GetNpcTextLocale(textID))
            {
                for (int i = 0; i < MAX_LOCALES; ++i)
                {
                    ObjectMgr::GetLocaleString(nl->Text_0[i], loc_idx, Text_0[i]);
                    ObjectMgr::GetLocaleString(nl->Text_1[i], loc_idx, Text_1[i]);
                }
            }
        }

        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << pGossip->Options[i].Probability;

            if (Text_0[i].empty())
                data << Text_1[i];
            else
                data << Text_0[i];

            if (Text_1[i].empty())
                data << Text_0[i];
            else
                data << Text_1[i];

            data << pGossip->Options[i].Language;

            for (int j = 0; j < MAX_GOSSIP_TEXT_EMOTES; ++j)
            {
                data << pGossip->Options[i].Emotes[j]._Delay;
                data << pGossip->Options[i].Emotes[j]._Emote;
            }
        }
    }*/

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUERY_NPC_TEXT_RESPONSE");
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandlePageTextQueryOpcode(WorldPackets::Query::QueryPageText& packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUERY_PAGE_TEXT");

    uint32 pageID = packet.PageTextID;

    while (pageID)
    {
        PageText const* pageText = sObjectMgr->GetPageText(pageID);

        WorldPackets::Query::QueryPageTextResponse response;
        response.PageTextID = pageID;

        if (!pageText)
        {
            response.Allow = false;
            pageID = 0;
        }
        else
        {
            response.Allow = true;
            response.Info.ID = pageID;
            response.Info.Text = pageText->Text;

            int loc_idx = GetSessionDbLocaleIndex();
            if (loc_idx >= 0)
                if (PageTextLocale const* player = sObjectMgr->GetPageTextLocale(pageID))
                    ObjectMgr::GetLocaleString(player->Text, loc_idx, response.Info.Text);

            response.Info.NextPageID = pageText->NextPage;
            pageID = pageText->NextPage;
        }

        SendPacket(response.Write());

        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUERY_PAGE_TEXT_RESPONSE");
    }
}

//! ToDo: neew system of it.
void WorldSession::HandleQueryCorpseTransport(WorldPackets::Query::QueryCorpseTransport& packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recv CMSG_CORPSE_MAP_POSITION_QUERY");

    Corpse* corpse = _player->GetCorpse();

    WorldPackets::Query::CorpseTransportQuery response;
    /*if (!corpse || corpse->GetTransGUID().IsEmpty() || corpse->GetTransGUID() != packet.Transport)*/
    {
        response.Position = G3D::Vector3(0.0f, 0.0f, 0.0f);
        response.Facing = 0.0f;
    }
    /*else
    {
        response.Position = G3D::Vector3(corpse->GetTransOffsetX(), corpse->GetTransOffsetY(), corpse->GetTransOffsetZ());
        response.Facing = corpse->GetTransOffsetO();
    }*/

    SendPacket(response.Write());
}

//! 6.1.2
void WorldSession::HandleQuestPOIQuery(WorldPackets::Query::QuestPOIQuery& packet)
{
    if (packet.MissingQuestCount > MAX_QUEST_LOG_SIZE)
        return;

    // Read quest ids and add the in a unordered_set so we don't send POIs for the same quest multiple times
    std::set<int32> questIds;
    for (int32 i = 0; i < packet.MissingQuestCount; ++i)
        questIds.insert(packet.MissingQuestPOIs[i]); // QuestID

    WorldPackets::Query::QuestPOIQueryResponse response;

    for (auto itr = questIds.begin(); itr != questIds.end(); ++itr)
    {
        int32 QuestID = *itr;

        bool questOk = false;

        uint16 questSlot = _player->FindQuestSlot(uint32(QuestID));

        if (questSlot != MAX_QUEST_LOG_SIZE)
            questOk = _player->GetQuestSlotQuestId(questSlot) == uint32(QuestID);

        if (questOk)
        {
            QuestPOIVector const* poiData = sObjectMgr->GetQuestPOIVector(QuestID);
            if (poiData)
            {
                WorldPackets::Query::QuestPOIData questPOIData;

                questPOIData.QuestID = QuestID;

                for (auto data = poiData->begin(); data != poiData->end(); ++data)
                {
                    WorldPackets::Query::QuestPOIBlobData questPOIBlobData;

                    questPOIBlobData.BlobIndex          = data->BlobIndex;
                    questPOIBlobData.ObjectiveIndex     = data->ObjectiveIndex;
                    questPOIBlobData.QuestObjectiveID   = data->QuestObjectiveID;
                    questPOIBlobData.QuestObjectID      = data->QuestObjectID;
                    questPOIBlobData.MapID              = data->MapID;
                    questPOIBlobData.WorldMapAreaID     = data->WorldMapAreaID;
                    questPOIBlobData.Floor              = data->Floor;
                    questPOIBlobData.Priority           = data->Priority;
                    questPOIBlobData.Flags              = data->Flags;
                    questPOIBlobData.WorldEffectID      = data->WorldEffectID;
                    questPOIBlobData.PlayerConditionID  = data->PlayerConditionID;
                    questPOIBlobData.UnkWoD1            = data->UnkWoD1;

                    for (auto points = data->points.begin(); points != data->points.end(); ++points)
                    {
                        WorldPackets::Query::QuestPOIBlobPoint questPOIBlobPoint;

                        questPOIBlobPoint.X = points->X;
                        questPOIBlobPoint.Y = points->Y;

                        sLog->outDebug(LOG_FILTER_NETWORKIO, "Quest: %i BlobIndex: %i X/Y: %i/%i", QuestID, data->BlobIndex, points->X, points->Y);

                        questPOIBlobData.QuestPOIBlobPointStats.push_back(questPOIBlobPoint);
                    }

                    questPOIData.QuestPOIBlobDataStats.push_back(questPOIBlobData);
                }

                response.QuestPOIDataStats.push_back(questPOIData);
            }
        }
    }

    SendPacket(response.Write());
}

void WorldSession::HandleDBQueryBulk(WorldPackets::Query::DBQueryBulk& packet)
{
    DB2StorageBase const* store = GetDB2Storage(packet.TableHash);
    if (!store)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_DB_QUERY_BULK: Received unknown hotfix type: %u", packet.TableHash);
        return;
    }

    for (WorldPackets::Query::DBQueryBulk::DBQueryRecord const& rec : packet.Queries)
    {
        WorldPackets::Query::DBReply response;
        response.TableHash = packet.TableHash;

        if (store->HasRecord(rec.RecordID))
        {
            response.RecordID = rec.RecordID;
            response.Locale = GetSessionDbcLocale();
            response.Timestamp = sObjectMgr->GetHotfixDate(rec.RecordID, packet.TableHash);
            response.Data = store;
        }
        else
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_DB_QUERY_BULK: Entry %u does not exist in datastore: %u", rec.RecordID, packet.TableHash);
            response.RecordID = -int32(rec.RecordID);
            response.Timestamp = time(NULL);
        }
        
        SendPacket(response.Write());
    }
}
