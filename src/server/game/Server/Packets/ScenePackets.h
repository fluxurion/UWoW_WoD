/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
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

#ifndef ScenePackets_h__
#define ScenePackets_h__

#include "Packet.h"
#include "BattlePetPackets.h"
#include "AchievementPackets.h"
#include "Object.h"

namespace WorldPackets
{
    namespace Scene
    {
        //< CMSG_SCENE_PLAYBACK_COMPLETE
        //< CMSG_SCENE_PLAYBACK_CANCELED
        class SceneInstance final : public ClientPacket
        {
        public:
            SceneInstance(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

            void Read() override;

            uint32 SceneInstanceID = 0;
        };

        class CancelScene final : public ServerPacket
        {
        public:
            CancelScene(uint32 ID) : ServerPacket(SMSG_CANCEL_SCENE, 4), SceneInstanceID(ID) { }

            WorldPacket const* Write() override;

            uint32 SceneInstanceID = 0;
        };

        class PlayScene final : public ServerPacket
        {
        public:
            PlayScene() : ServerPacket(SMSG_PLAY_SCENE, 16 + 18 + 16) { }

            WorldPacket const* Write() override;

            uint32 SceneID = 0;
            uint32 PlaybackFlags = 0;
            uint32 SceneInstanceID = 0;
            uint32 SceneScriptPackageID = 0;
            ObjectGuid TransportGUID;
            Position Pos;
        };

        //< SMSG_SCENE_OBJECT_PET_BATTLE_FIRST_ROUND
        //< SMSG_SCENE_OBJECT_PET_BATTLE_ROUND_RESULT
        //< SMSG_SCENE_OBJECT_PET_BATTLE_REPLACEMENTS_MADE
        class PetBattleRound final : public ServerPacket
        {
        public:
            PetBattleRound(OpcodeServer opcode) : ServerPacket(opcode) { }

            WorldPacket const* Write() override;

            ObjectGuid SceneObjectGUID;
            WorldPackets::BattlePet::RoundResult MsgData;
        };

        class PetBattleFinalRound final : public ServerPacket
        {
        public:
            PetBattleFinalRound() : ServerPacket(SMSG_SCENE_OBJECT_PET_BATTLE_FINAL_ROUND) { }

            WorldPacket const* Write() override;

            ObjectGuid SceneObjectGUID;
            WorldPackets::BattlePet::FinalRound MsgData;
        };

        class PetBattleFinished final : public ServerPacket
        {
        public:
            PetBattleFinished() : ServerPacket(SMSG_SCENE_OBJECT_PET_BATTLE_FINISHED, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid SceneObjectGUID;
        };

        class QueryScenarioPOI final : public ClientPacket
        {
        public:
            QueryScenarioPOI(WorldPacket&& packet) : ClientPacket(CMSG_QUERY_SCENARIO_POI, std::move(packet)) { }

            void Read() override;

            std::vector<uint32> MissingScenarioPOITreeIDs;
        };

        class ScenarioPOIs final : public ServerPacket
        {
        public:
            ScenarioPOIs() : ServerPacket(SMSG_SCENARIO_PO_IS, 4) { }

            WorldPacket const* Write() override;

            struct POIData
            {
                struct BlobData
                {
                    struct POIPointData
                    {
                        uint32 X = 0;
                        uint32 Y = 0;
                    };

                    uint32 BlobID = 0;
                    uint32 MapID = 0;
                    uint32 WorldMapAreaID = 0;
                    uint32 Floor = 0;
                    uint32 Priority = 0;
                    uint32 Flags = 0;
                    uint32 WorldEffectID = 0;
                    uint32 PlayerConditionID = 0;
                    std::vector<POIPointData> Points;
                };

                uint32 CriteriaTreeID = 0;
                std::vector<BlobData> BlobDatas;
            };

            std::vector<POIData> PoiInfos;
        };

        //< SMSG_SCENARIO_BOOT
        class NullSMsg final : public ServerPacket
        {
        public:
            NullSMsg(OpcodeServer opcode) : ServerPacket(opcode) { }

            WorldPacket const* Write() override { return &_worldPacket; }
        };

        class ScenarioProgressUpdate final : public ServerPacket
        {
        public:
            ScenarioProgressUpdate() : ServerPacket(SMSG_SCENARIO_PROGRESS_UPDATE) { }

            WorldPacket const* Write() override;

            WorldPackets::Achievement::CriteriaTreeProgress Progress;
        };

        class ScenarioState final : public ServerPacket
        {
        public:
            ScenarioState() : ServerPacket(SMSG_SCENARIO_STATE, 33) { }

            WorldPacket const* Write() override;

            struct BonusObjective
            {
                uint32 BonusObjectiveID = 0;
                bool ObjectiveComplete = false;
            };

            uint32 ScenarioID = 0;
            uint32 CurrentStep = 0;
            uint32 DifficultyID = 0;
            uint32 WaveCurrent = 0;
            uint32 WaveMax = 0;
            uint32 TimerDuration = 0;
            bool ScenarioComplete = false;
            std::vector<BonusObjective> Objectives;
            std::vector<WorldPackets::Achievement::CriteriaTreeProgress> Progress;
        };

        class ScenarioCompleted final : public ServerPacket
        {
        public:
            ScenarioCompleted() : ServerPacket(SMSG_SCENARIO_COMPLETED, 4) { }

            WorldPacket const* Write() override;

            uint32 ScenarioID = 0;
        };

        class SceneTriggerEvent final : public ClientPacket
        {
        public:
            SceneTriggerEvent(WorldPacket&& packet) : ClientPacket(CMSG_SCENE_TRIGGER_EVENT, std::move(packet)) { }

            void Read() override;

            std::string Event;
            uint32 SceneInstanceID = 0;
        };

        class SceneObjectPetBattleInitialUpdate final : public ServerPacket
        {
        public:
            SceneObjectPetBattleInitialUpdate() : ServerPacket(SMSG_SCENE_OBJECT_PET_BATTLE_INITIAL_UPDATE) { }

            WorldPacket const* Write() override;

            ObjectGuid SceneObjectGUID;
            WorldPackets::BattlePet::PetBattleFullUpdate MsgData;
        };
    }
}

#endif // ScenePackets_h__
