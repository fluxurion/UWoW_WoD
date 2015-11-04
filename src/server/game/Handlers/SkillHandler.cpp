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

    if (packet.SpecGroupIndex >= 4)
        return;

    ChrSpecializationsEntry const* chrSpec = sChrSpecializationByIndexStore[player->getClass()][packet.SpecGroupIndex];
    if (!chrSpec)
        return;

    if (chrSpec->ClassID != player->getClass())
        return;

    if (player->getLevel() < 10)
        return;

    player->LearnTalentSpecialization(chrSpec->ID);
}

void WorldSession::HandleLearnTalent(WorldPackets::Talent::LearnTalent& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (packet.Talents.size() > 7)
        return;

    Battleground* bg = player->GetBattleground();
    if (bg && bg->GetStatus() != STATUS_WAIT_JOIN)
        return;

    bool anythingLearned = false;
    for (uint32 const& talentId : packet.Talents)
        if (_player->LearnTalent(talentId))
            anythingLearned = true;

    if (anythingLearned)
        _player->SendTalentsInfoData(false);
}

void WorldSession::HandleConfirmRespecWipe(WorldPackets::Misc::ConfirmRespecWipe& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    // Hack
    if (player->HasAura(33786))
        return;

    if (player->HasUnitState(UNIT_STATE_DIED))
        player->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    switch(packet.respecType)
    {
        case RESPEC_TYPE_TALENTS:
            if (!player->ResetTalents())
                player->SendTalentWipeConfirm(packet.RespecMaster, RESPEC_TYPE_TALENTS);
            break;
        case RESPEC_TYPE_SPEC:
            player->ResetSpec();
            break;
        case RESPEC_TYPE_GLYPH:
            break;
        default:
            break;
    }

    player->SendTalentsInfoData(false);

    // spell: "Untalent Visual Effect"
    if (Unit* unit = player->GetSelectedUnit())
        unit->CastSpell(player, 14867, true);  
}

void WorldSession::HandleShowTradeSkill(WorldPackets::Misc::ShowTradeSkill& packet)
{
    if (!sSkillLineStore.LookupEntry(packet.SkillLineID) || !sSpellMgr->GetSpellInfo(packet.SpellID))
        return;

    Player* player = sObjectAccessor->FindPlayer(packet.PlayerGUID);
    if (!player)
        return;

    std::set<uint32> relatedSkills;
    relatedSkills.insert(packet.SkillLineID);

    for (SkillLineEntry const* skillLine : sSkillLineStore)
    {
        if (skillLine->parentSkillId != packet.SkillLineID)
            continue;

        if (!player->HasSkill(skillLine->parentSkillId))
            continue;

        relatedSkills.insert(skillLine->parentSkillId);
    }

    std::set<uint32> profSpells;
    for (auto const& v : player->GetSpellMap())
    {
        if (v.second->state == PLAYERSPELL_REMOVED)
            continue;

        if (!v.second->active || v.second->disabled)
            continue;

        for (auto const& s : relatedSkills)
            if (IsPartOfSkillLine(s, v.first))
                profSpells.insert(v.first);
    }

    if (profSpells.empty())
        return;

    WorldPackets::Misc::ShowTradeSkillResponse response;
    response.PlayerGUID = packet.PlayerGUID;
    response.SpellId = packet.SpellID;

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
