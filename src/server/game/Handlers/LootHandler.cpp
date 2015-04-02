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
#include "WorldPacket.h"
#include "Log.h"
#include "Corpse.h"
#include "GameObject.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "WorldSession.h"
#include "LootMgr.h"
#include "Object.h"
#include "Group.h"
#include "World.h"
#include "Util.h"
#include "GuildMgr.h"
#include "LootPackets.h"

//! 6.0.3
void WorldSession::HandleAutostoreLootItemOpcode(WorldPackets::Loot::AutoStoreLootItem& packet)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_AUTOSTORE_LOOT_ITEM");
    Player* player = GetPlayer();
    Loot* loot = NULL;

    for (WorldPackets::Loot::LootRequest const& req : packet.Loot)
    {
        ObjectGuid lguid = req.Object;
        ObjectGuid lootObjectGUID = player->GetOwnerByLootGuid(lguid);

        if (!lootObjectGUID)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: CMSG_AUTOSTORE_LOOT_ITEM can't find owner of loot %u", lguid.GetCounter());
            player->SendLootRelease(lguid);
            return;
        }

        if (lootObjectGUID.IsGameObject())
        {
            GameObject* go = player->GetMap()->GetGameObject(lootObjectGUID);

            // not check distance for GO in case owned GO (fishing bobber case, for example) or Fishing hole GO
            if (!go || ((go->GetOwnerGUID() != _player->GetGUID() && go->GetGoType() != GAMEOBJECT_TYPE_FISHINGHOLE) && !go->IsWithinDistInMap(_player, INTERACTION_DISTANCE)))
            {
                player->SendLootRelease(lguid);
                return;
            }

            loot = &go->loot;
            if(go->GetGOInfo()->chest.chestPersonalLoot)
                if(TreasureData const* pData = sObjectMgr->GetTreasureData(go->GetGOInfo()->chest.chestPersonalLoot))
                    if(pData->type == GO_TYPE_RESPAWN || pData->type == GO_TYPE_DONTSPAWN)
                        loot = &player->personalLoot;

            //sLog->outDebug(LOG_FILTER_LOOT, "HandleAutostoreLootItemOpcode lguid %u, pguid %u lguid %u", lguid, player->personalLoot.GetGUID(), loot->GetGUID());
        }
        else if (lootObjectGUID.IsItem())
        {
            Item* pItem = player->GetItemByGuid(lootObjectGUID);

            if (!pItem)
            {
                player->SendLootRelease(lguid);
                return;
            }

            loot = &pItem->loot;
        }
        else if (lootObjectGUID.IsCorpse())
        {
            Corpse* bones = ObjectAccessor::GetCorpse(*player, lootObjectGUID);
            if (!bones)
            {
                player->SendLootRelease(lguid);
                return;
            }

            loot = &bones->loot;
        }
        else
        {
            Creature* creature = GetPlayer()->GetMap()->GetCreature(lootObjectGUID);

            bool lootAllowed = creature && creature->isAlive() == (player->getClass() == CLASS_ROGUE && creature->lootForPickPocketed);

            Unit *looter = creature ? creature->GetOtherRecipient() : NULL;
            if (!looter)
                looter = player;

            if (!lootAllowed || !creature->IsWithinDistInMap(looter, LOOT_DISTANCE))
            {
                player->SendLootError(lootObjectGUID, lguid, lootAllowed ? LOOT_ERROR_TOO_FAR : LOOT_ERROR_DIDNT_KILL);
                return;
            }

            loot = &player->personalLoot;
            if(!creature->IsPersonalLoot() || loot->isLooted() || creature->GetGUID() != loot->objGuid)
                loot = &creature->loot;
        }

        if(Group* group = player->GetGroup())
        {
            // Already rolled?
            if (group->isRolledSlot(req.LootListID-1))
                return;
        }

        // Since 6.x client sends loot starting from 1 hence the -1
        player->StoreLootItem(req.LootListID-1, loot);

        // If player is removing the last LootItem, delete the empty container.
        if (loot->isLooted() && lguid.IsItem())
            player->GetSession()->DoLootRelease(lguid);
    }
}

void WorldSession::HandleLootMoneyOpcode(WorldPacket& /*recvData*/)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_LOOT_MONEY");

    Player* player = GetPlayer();
    ObjectGuid guid = player->GetLootGUID();
    AoeMap* listloot = player->GetAoeLootList();
    AoeMap* temploot;
    AoeMap templootguid;
    if (!guid && listloot->empty())
        return;

    if(!listloot->empty())
        temploot = listloot;
    else
    {
        templootguid[guid] = guid/*should be loot guid. but no metter*/;
        temploot = &templootguid;
    }

    for (AoeMap::const_iterator itr = temploot->begin(); itr != temploot->end(); ++itr)
    {
        Loot* loot = NULL;
        bool shareMoney = true;
        ObjectGuid lootguid = itr->first;

        switch (lootguid.GetHigh())
        {
            case HighGuid::GameObject:
            {
                GameObject* go = GetPlayer()->GetMap()->GetGameObject(lootguid);

                // do not check distance for GO if player is the owner of it (ex. fishing bobber)
                if (go && ((go->GetOwnerGUID() == player->GetGUID() || go->IsWithinDistInMap(player, INTERACTION_DISTANCE))))
                    loot = &go->loot;

                break;
            }
            case HighGuid::Corpse:                               // remove insignia ONLY in BG
            {
                Corpse* bones = ObjectAccessor::GetCorpse(*player, lootguid);

                if (bones && bones->IsWithinDistInMap(player, LOOT_DISTANCE))
                {
                    loot = &bones->loot;
                    shareMoney = false;
                }

                break;
            }
            case HighGuid::Item:
            {
                if (Item* item = player->GetItemByGuid(lootguid))
                {
                    loot = &item->loot;
                    shareMoney = false;
                }
                break;
            }
            case HighGuid::Creature:
            case HighGuid::Vehicle:
            {
                Creature* creature = player->GetMap()->GetCreature(lootguid);
                bool lootAllowed = creature && creature->isAlive() == (player->getClass() == CLASS_ROGUE && creature->lootForPickPocketed);
                if (lootAllowed && creature->IsWithinDistInMap(player, LOOT_DISTANCE))
                {
                    loot = &creature->loot;
                    if (creature->isAlive())
                        shareMoney = false;
                }
                break;
            }
            default:
                return;                                         // unlootable type
        }

        if (loot)
        {
            if (shareMoney && player->GetGroup())      //item, pickpocket and players can be looted only single player
            {
                Group* group = player->GetGroup();

                std::vector<Player*> playersNear;
                for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                {
                    Player* member = itr->getSource();
                    if (!member)
                        continue;

                    if (player->IsWithinDistInMap(member, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                        playersNear.push_back(member);
                }
                 /*@todo: check me for 5.0.5*/
                uint32 goldPerPlayer = uint32((loot->gold) / (playersNear.size()));
                
                loot->NotifyMoneyRemoved(goldPerPlayer);

                //! 6.0.3 SMSG_LOOT_MONEY_NOTIFY
                WorldPackets::Loot::LootMoneyNotify packet;
                packet.Money = goldPerPlayer;
                packet.SoleLooter = playersNear.size() <= 1 ? true : false;
  
                for (std::vector<Player*>::const_iterator i = playersNear.begin(); i != playersNear.end(); ++i)
                {
                    (*i)->ModifyMoney(goldPerPlayer);
                    (*i)->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY, goldPerPlayer);

                    if (Guild* guild = sGuildMgr->GetGuildById((*i)->GetGuildId()))
                        if (uint32 guildGold = CalculatePct(goldPerPlayer, (*i)->GetTotalAuraModifier(SPELL_AURA_DEPOSIT_BONUS_MONEY_IN_GUILD_BANK_ON_LOOT)))
                            guild->HandleMemberDepositMoney(this, guildGold, true);

                    (*i)->GetSession()->SendPacket(packet.Write());
                }
            }
            else
            {
                player->ModifyMoney(loot->gold);
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY, loot->gold);

                if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId()))
                    if (uint32 guildGold = CalculatePct(loot->gold, player->GetTotalAuraModifier(SPELL_AURA_DEPOSIT_BONUS_MONEY_IN_GUILD_BANK_ON_LOOT)))
                        guild->HandleMemberDepositMoney(this, guildGold, true);

                loot->NotifyMoneyRemoved(loot->gold);

                //! 6.0.3 SMSG_LOOT_MONEY_NOTIFY
                WorldPackets::Loot::LootMoneyNotify packet;
                packet.Money = loot->gold;
                packet.SoleLooter = true; // "You loot..."
                SendPacket(packet.Write());
            }

            loot->gold = 0;
        }
    }
}

//! 6.0.3
void WorldSession::HandleLootOpcode(WorldPackets::Loot::LootUnit& packet)
{
    // Check possible cheat
    if (!GetPlayer()->isAlive() || !packet.Unit.IsCreatureOrVehicle())
        return;

    LootCorps(packet.Unit);

    // interrupt cast
    if (GetPlayer()->IsNonMeleeSpellCasted(false))
        GetPlayer()->InterruptNonMeleeSpells(false);
}

void WorldSession::LootCorps(ObjectGuid corpsGUID, WorldObject* lootedBy)
{
    // Check possible cheat
    if (!_player->isAlive())
        return;

    WorldObject* _looted = lootedBy ? lootedBy : _player;

    Creature* _creature = _player->GetMap()->GetCreature(corpsGUID);
    if (!_creature)
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_LOOT corpsGUID %u", corpsGUID);

    std::list<Creature*> corpesList;
    _looted->GetCorpseCreatureInGrid(corpesList, LOOT_DISTANCE);

    WorldPacket data(SMSG_AE_LOOT_TARGETS);
    data << uint32(corpesList.size());                             //aoe counter
    _player->SendDirectMessage(&data);

    _creature->SetOtherLootRecipient(lootedBy ? lootedBy->GetGUID() : ObjectGuid::Empty);
    _player->SendLoot(corpsGUID, LOOT_CORPSE, true);

    data.Initialize(SMSG_AE_LOOT_TARGET_ACK);
    _player->SendDirectMessage(&data);

    for (std::list<Creature*>::const_iterator itr = corpesList.begin(); itr != corpesList.end(); ++itr)
    {
        if(Creature* creature = (*itr))
        {
            creature->SetOtherLootRecipient(lootedBy ? lootedBy->GetGUID() : ObjectGuid::Empty);

            if(corpsGUID != creature->GetGUID())
            {
                _player->SendLoot(creature->GetGUID(), LOOT_CORPSE, true, 1);
                _player->SendDirectMessage(&data); //SMSG_AE_LOOT_TARGET_ACK
            }
        }
    }

    //clear aoe loot state
    if (Group* group = GetPlayer()->GetGroup())
        group->ClearAoeSlots();

    // interrupt cast
    if (_player->IsNonMeleeSpellCasted(false))
        _player->InterruptNonMeleeSpells(false);
}

//! 6.0.3
void WorldSession::HandleLootReleaseOpcode(WorldPacket& recvData)
{
    //sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_LOOT_RELEASE");

    // cheaters can modify lguid to prevent correct apply loot release code and re-loot
    // use internal stored guid
    ObjectGuid guid;
    recvData >> guid;

    ObjectGuid lguid = GetPlayer()->GetLootGUID();
    if (!lguid.IsEmpty())
        if (lguid == guid)
            DoLootRelease(guid);
}

void WorldSession::DoLootRelease(ObjectGuid lguid)
{
    Player  *player = GetPlayer();
    Loot    *loot;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: DoLootRelease lguid %u", lguid);

    player->DelAoeLootList(lguid);
    player->SetLootGUID(ObjectGuid::Empty);
    player->SendLootRelease(lguid);

    player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOOTING);

    if (!player->IsInWorld())
        return;

    if (lguid.IsGameObject())
    {
        GameObject* go = GetPlayer()->GetMap()->GetGameObject(lguid);

        // not check distance for GO in case owned GO (fishing bobber case, for example) or Fishing hole GO
        if (!go || ((go->GetOwnerGUID() != _player->GetGUID() && go->GetGoType() != GAMEOBJECT_TYPE_FISHINGHOLE) && !go->IsWithinDistInMap(_player, INTERACTION_DISTANCE)))
            return;

        loot = &go->loot;

        if (go->GetGoType() == GAMEOBJECT_TYPE_DOOR)
        {
            // locked doors are opened with spelleffect openlock, prevent remove its as looted
            go->UseDoorOrButton();
        }
        else if (loot->isLooted() || go->GetGoType() == GAMEOBJECT_TYPE_FISHINGNODE)
        {
            // GO is mineral vein? so it is not removed after its looted
            if (go->GetGoType() == GAMEOBJECT_TYPE_CHEST)
            {
                uint32 go_min = go->GetGOInfo()->chest.minRestock;
                uint32 go_max = go->GetGOInfo()->chest.maxRestock;

                // only vein pass this check
                if (go_min != 0 && go_max > go_min)
                {
                    float amount_rate = sWorld->getRate(RATE_MINING_AMOUNT);
                    float min_amount = go_min*amount_rate;
                    float max_amount = go_max*amount_rate;

                    go->AddUse();
                    float uses = float(go->GetUseCount());

                    if (uses < max_amount)
                    {
                        if (uses >= min_amount)
                        {
                            float chance_rate = sWorld->getRate(RATE_MINING_NEXT);

                            int32 ReqValue = 175;
                            LockEntry const* lockInfo = sLockStore.LookupEntry(go->GetGOInfo()->GetLockId());
                            if (lockInfo)
                                ReqValue = lockInfo->Skill[0];
                            float skill = float(player->GetSkillValue(SKILL_MINING))/(ReqValue+25);
                            double chance = pow(0.8*chance_rate, 4*(1/double(max_amount))*double(uses));
                            if (roll_chance_f((float)(100*chance+skill)))
                            {
                                go->SetLootState(GO_READY);
                            }
                            else                            // not have more uses
                                go->SetLootState(GO_JUST_DEACTIVATED);
                        }
                        else                                // 100% chance until min uses
                            go->SetLootState(GO_READY);
                    }
                    else                                    // max uses already
                        go->SetLootState(GO_JUST_DEACTIVATED);
                }
                else                                        // not vein
                    go->SetLootState(GO_JUST_DEACTIVATED);
            }
            else if (go->GetGoType() == GAMEOBJECT_TYPE_FISHINGHOLE)
            {                                               // The fishing hole used once more
                go->AddUse();                               // if the max usage is reached, will be despawned in next tick
                if (go->GetUseCount() >= urand(go->GetGOInfo()->fishingHole.minRestock, go->GetGOInfo()->fishingHole.maxRestock))
                {
                    go->SetLootState(GO_JUST_DEACTIVATED);
                }
                else
                    go->SetLootState(GO_READY);
            }
            else // not chest (or vein/herb/etc)
                go->SetLootState(GO_JUST_DEACTIVATED);

            loot->clear();
        }
        else
        {
            // not fully looted object
            go->SetLootState(GO_ACTIVATED, player);

            // if the round robin player release, reset it.
            if (player->GetGUID() == loot->roundRobinPlayer)
            {
                if (Group* group = player->GetGroup())
                {
                    if (group->GetLootMethod() != MASTER_LOOT)
                    {
                        loot->roundRobinPlayer.Clear();
                    }
                }
                else
                    loot->roundRobinPlayer.Clear();
            }
        }
    }
    else if (lguid.IsCorpse())        // ONLY remove insignia at BG
    {
        Corpse* corpse = ObjectAccessor::GetCorpse(*player, lguid);
        if (!corpse || !corpse->IsWithinDistInMap(_player, LOOT_DISTANCE))
            return;

        loot = &corpse->loot;

        if (loot->isLooted())
        {
            loot->clear();
            corpse->RemoveFlag(CORPSE_FIELD_DYNAMIC_FLAGS, CORPSE_DYNFLAG_LOOTABLE);
        }
    }
    else if (lguid.IsItem())
    {
        Item* pItem = player->GetItemByGuid(lguid);
        if (!pItem)
            return;

        ItemTemplate const* proto = pItem->GetTemplate();

        // destroy only 5 items from stack in case prospecting and milling
        if (proto->Flags & (ITEM_PROTO_FLAG_PROSPECTABLE | ITEM_PROTO_FLAG_MILLABLE))
        {
            pItem->m_lootGenerated = false;
            pItem->loot.clear();

            uint32 count = pItem->GetCount();

            // >=5 checked in spell code, but will work for cheating cases also with removing from another stacks.
            if (count > 5)
                count = 5;

            player->DestroyItemCount(pItem, count, true);
        }
        else
        {
            pItem->m_lootGenerated = false;
            pItem->loot.clear();

            uint32 count = 1;

            player->DestroyItemCount(pItem, count, true);
            // FIXME: item must not be deleted in case not fully looted state. But this pre-request implement loot saving in DB at item save. Or cheating possible.
            // player->DestroyItem(pItem->GetBagSlot(), pItem->GetSlot(), true);
        }
        return;                                             // item can be looted only single player
    }
    else
    {
        Creature* creature = player->GetMap()->GetCreature(lguid);

        bool lootAllowed = creature && creature->isAlive() == (player->getClass() == CLASS_ROGUE && creature->lootForPickPocketed);

        Unit *looter = creature ? creature->GetOtherRecipient() : NULL;
        if (!looter)
            looter = player;

        // Restore Fetch state for pet.
        if (looter->isPet())
        {
            if(Unit* _petowner = looter->GetOwner())
                if (_petowner == player)
                    looter->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, looter->GetFollowAngle());
        }

        if (!lootAllowed || !creature->IsWithinDistInMap(looter, LOOT_DISTANCE))
            return;

        loot = &player->personalLoot;
        if(creature->IsPersonalLoot() && loot->isLooted() && creature->GetGUID() == loot->objGuid)
        {
            loot->clear();
            creature->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            creature->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            return;
        }

        loot = &creature->loot;
        if (loot->isLooted())
        {
            // skip pickpocketing loot for speed, skinning timer reduction is no-op in fact
            if (!creature->isAlive())
                creature->AllLootRemovedFromCorpse();

            creature->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            loot->clear();
        }
        else
        {
            // if the round robin player release, reset it.
            if (player->GetGUID() == loot->roundRobinPlayer)
            {
                if (Group* group = player->GetGroup())
                {
                    if (group->GetLootMethod() != MASTER_LOOT)
                    {
                        loot->roundRobinPlayer.Clear();
                        group->SendLooter(creature, NULL);

                        // force update of dynamic flags, otherwise other group's players still not able to loot.
                        creature->ForceValuesUpdateAtIndex(OBJECT_FIELD_DYNAMIC_FLAGS);
                    }
                }
                else
                    loot->roundRobinPlayer.Clear();
            }
        }
    }

    //Player is not looking at loot list, he doesn't need to see updates on the loot list
    loot->RemoveLooter(player->GetGUID());
}

void WorldSession::HandleLootSpecIdOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_LOOT_METHOD_FOR_SPECIALIZATION");

    uint32 specID;
    recvData >> specID;

    if (specID == 0)
    {
        _player->SetLootSpecID(0);
        return;
    }

    uint8 classId = _player->getClass();
    ChrSpecializationsEntry const* specialization = sChrSpecializationsStore.LookupEntry(specID);
    if (specialization && specialization->ClassID == classId)
        _player->SetLootSpecID(specID);   
}