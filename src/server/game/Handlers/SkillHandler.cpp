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
#include "DatabaseEnv.h"
#include "Log.h"
#include "MiscPackets.h"
#include "ObjectAccessor.h"
#include "Opcodes.h"
#include "Player.h"
#include "TalentPackets.h"
#include "UpdateMask.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void WorldSession::HandleSetSpecialization(WorldPackets::Talent::SetSpecialization& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    uint8 classId = player->getClass();

    if (player->GetSpecializationId(player->GetActiveSpec()))
        return;

    uint32 specializationId = 0;
    uint32 specializationSpell = 0;

    for (ChrSpecializationsEntry const* specialization : sChrSpecializationsStore)
    {
        if (specialization->ClassID == classId && specialization->OrderIndex == packet.SpecGroupIndex)
        {
            specializationId = specialization->ID;
            specializationSpell = specialization->MasterySpellID[0];
            break;
        }
    }

    if (specializationId)
    {
        player->SetSpecializationId(player->GetActiveSpec(), specializationId);
        player->SendTalentsInfoData(false);
        player->InitSpellForLevel();
        player->InitialPowers();
        player->_ApplyOrRemoveItemEquipDependentAuras(ObjectGuid::Empty, false);
    }
}

void WorldSession::HandleLearnTalent(WorldPackets::Talent::LearnTalent& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (packet.Talents.size() > 7)
        return;

    if (packet.Talents.size() > player->GetFreeTalentPoints())
        return;

    Battleground* bg = player->GetBattleground();
    if (bg && bg->GetStatus() != STATUS_WAIT_JOIN)
        return;

    for (uint32 const& talentID : packet.Talents)
        player->LearnTalent(talentID);

    player->SendTalentsInfoData(false);
}

//! 6.0.3
void WorldSession::HandleTalentWipeConfirmOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    recvData >> guid;
    uint8 specializationReset = recvData.read<uint8>();

    // Hack
    if (GetPlayer()->HasAura(33786))
        return;

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if(!specializationReset)
    {
        if (!_player->ResetTalents())
        {
            WorldPacket data(SMSG_RESPEC_WIPE_CONFIRM, 8+4);    //you have not any talent
            data << uint8(0);
            data << uint32(0);
            data << ObjectGuid::Empty;
            SendPacket(&data);
            return;
        }
    }
    else
    {
        _player->ResetSpec();
    }

    _player->SendTalentsInfoData(false);

    if(Unit* unit = _player->GetSelectedUnit())
        unit->CastSpell(_player, 14867, true);                  //spell: "Untalent Visual Effect"
}

void WorldSession::HandleQueryPlayerRecipes(WorldPacket& recvPacket)
{
    uint32 spellId, skillId;
    ObjectGuid guid;

    recvPacket >> spellId >> skillId;
    //recvPacket.ReadGuidMask<6, 0, 2, 3, 5, 7, 1, 4>(guid);
    //recvPacket.ReadGuidBytes<5, 6, 1, 3, 4, 0, 7, 2>(guid);

    if (!sSkillLineStore.LookupEntry(skillId) || !sSpellMgr->GetSpellInfo(spellId))
        return;

    Player* player = sObjectAccessor->FindPlayer(guid);
    if (!player)
        return;

    std::set<uint32> relatedSkills;
    relatedSkills.insert(skillId);

    for (SkillLineEntry const* skillLine : sSkillLineStore)
    {
        if (skillLine->parentSkillId != skillId)
            continue;

        if (!player->HasSkill(skillLine->parentSkillId))
            continue;

        relatedSkills.insert(skillLine->parentSkillId);
    }

    std::set<uint32> profSpells;
    PlayerSpellMap const& spellMap = player->GetSpellMap();
    for (PlayerSpellMap::const_iterator itr = spellMap.begin(); itr != spellMap.end(); ++itr)
    {
        if (itr->second->state == PLAYERSPELL_REMOVED)
            continue;

        if (!itr->second->active || itr->second->disabled)
            continue;

        for (std::set<uint32>::const_iterator itr2 = relatedSkills.begin(); itr2 != relatedSkills.end(); ++itr2)
            if (IsPartOfSkillLine(*itr2, itr->first))
                profSpells.insert(itr->first);
    }

    if (profSpells.empty())
        return;

    WorldPackets::Misc::ShowTradeSkillResponse response;
    response.PlayerGUID = guid;
    response.SpellId = spellId;

    for (uint32 const& x : profSpells)
        response.KnownAbilitySpellIDs.push_back(x);

    for (uint32 const& v : relatedSkills)
    {
        response.SkillLineIDs.push_back(v);
        response.SkillRanks.push_back(player->GetSkillValue(v));
        response.SkillMaxRanks.push_back(player->GetMaxSkillValue(v));
    }

    _player->SendDirectMessage(response.Write());
}
