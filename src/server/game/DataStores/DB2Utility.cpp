/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
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

#include "DB2Utility.h"
#include "ObjectMgr.h"
#include "GossipDef.h"

inline bool ItemExists(uint32 id)
{
    return sObjectMgr->GetItemTemplate(id) != NULL;
}

bool DB2Utilities::HasItemEntry(DB2Storage<ItemEntry> const& /*store*/, uint32 id)
{
    return ItemExists(id);
}

bool DB2Utilities::HasItemSparseEntry(DB2Storage<ItemSparseEntry> const& /*store*/, uint32 id)
{
    return ItemExists(id);
}

bool DB2Utilities::HasBroadcastTextEntry(DB2Storage<BroadcastTextEntry> const& /*store*/, uint32 id)
{
    return true;
}

void DB2Utilities::WriteItemDbReply(DB2Storage<ItemEntry> const& /*store*/, uint32 id, uint32 /*locale*/, ByteBuffer& buffer)
{
    ItemTemplate const* proto = sObjectMgr->GetItemTemplate(id);
    ASSERT(proto);

    buffer << uint32(proto->ItemId);
    buffer << uint32(proto->Class);
    buffer << uint32(proto->SubClass);
    buffer << int32(proto->SoundOverrideSubclass);
    buffer << int32(proto->Material);
    buffer << uint32(proto->GetInventoryType());
    buffer << uint32(proto->Sheath);
    buffer << uint32(proto->FileDataID);
    buffer << uint32(proto->GroupSoundsID);
}

void DB2Utilities::WriteItemSparseDbReply(DB2Storage<ItemSparseEntry> const& /*store*/, uint32 id, uint32 locale, ByteBuffer& buffer)
{
    ItemTemplate const* proto = sObjectMgr->GetItemTemplate(id);
    ASSERT(proto);

    ItemLocale const* localeData = locale ? sObjectMgr->GetItemLocale(id) : NULL;

    buffer << uint32(proto->ItemId);
    buffer << uint32(proto->Quality);

    buffer << uint32(proto->Flags);
    buffer << uint32(proto->Flags2);
    buffer << uint32(proto->Flags3);

    buffer << float(proto->Unk430_1);
    buffer << float(proto->Unk430_2);
    buffer << uint32(proto->BuyCount);
    buffer << int32(proto->BuyPrice);
    buffer << uint32(proto->SellPrice);
    buffer << uint32(proto->GetInventoryType());
    buffer << int32(proto->AllowableClass);
    buffer << int32(proto->AllowableRace);
    buffer << uint32(proto->ItemLevel);
    buffer << uint32(proto->RequiredLevel);
    buffer << uint32(proto->RequiredSkill);
    buffer << uint32(proto->RequiredSkillRank);
    buffer << uint32(proto->RequiredSpell);
    buffer << uint32(proto->RequiredHonorRank);
    buffer << uint32(proto->RequiredCityRank);
    buffer << uint32(proto->RequiredReputationFaction);
    buffer << uint32(proto->RequiredReputationRank);
    buffer << int32(proto->MaxCount);
    buffer << int32(proto->Stackable);
    buffer << uint32(proto->ContainerSlots);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_STATS; ++x)
        buffer << uint32(proto->ItemStat[x].ItemStatType);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_STATS; ++x)
        buffer << int32(proto->ItemStat[x].ItemStatValue);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_STATS; ++x)
        buffer << int32(proto->ItemStat[x].ItemStatAllocation);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_STATS; ++x)
        buffer << float(proto->ItemStat[x].ItemStatSocketCostMultiplier);

    buffer << uint32(proto->ScalingStatDistribution);
    buffer << uint32(proto->DamageType);
    buffer << uint32(proto->Delay);
    buffer << float(proto->RangedModRange);
    buffer << uint32(proto->Bonding);

    // item name
    std::string name = proto->Name1;
    if (localeData)
        ObjectMgr::GetLocaleString(localeData->Name, locale, name);

    buffer << uint16(name.length());
    if (name.length())
        buffer.WriteString(name);

    for (uint32 i = 1; i < 4; ++i) // other 3 names
        buffer << uint16(0);

    std::string desc = proto->Description;
    if (localeData)
        ObjectMgr::GetLocaleString(localeData->Description, locale, desc);

    buffer << uint16(desc.length());
    if (desc.length())
        buffer.WriteString(desc);

    buffer << uint32(proto->PageText);
    buffer << uint32(proto->LanguageID);
    buffer << uint32(proto->PageMaterial);
    buffer << uint32(proto->StartQuest);
    buffer << uint32(proto->LockID);
    buffer << int32(proto->Material);
    buffer << uint32(proto->Sheath);
    buffer << uint32(proto->RandomProperty);
    buffer << uint32(proto->RandomSuffix);
    buffer << uint32(proto->ItemSet);

    buffer << uint32(proto->Area);
    buffer << uint32(proto->Map);
    buffer << uint32(proto->BagFamily);
    buffer << uint32(proto->TotemCategory);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_SOCKETS; ++x)
        buffer << uint32(proto->Socket[x].Color);

    buffer << uint32(proto->socketBonus);
    buffer << uint32(proto->GemProperties);
    buffer << float(proto->ArmorDamageModifier);
    buffer << int32(proto->Duration);
    buffer << uint32(proto->ItemLimitCategory);
    buffer << uint32(proto->HolidayId);
    buffer << float(proto->StatScalingFactor);    // StatScalingFactor
    buffer << uint32(proto->CurrencySubstitutionId);
    buffer << uint32(proto->CurrencySubstitutionCount);
    buffer << uint32(proto->ItemNameDescriptionID);
}

//ToDo
void DB2Utilities::WriteBroadcastTextDbReply(DB2Storage<BroadcastTextEntry> const& store, uint32 entry, uint32 locale, ByteBuffer& buff)
{
    GossipText const* pGossip = sObjectMgr->GetGossipText(entry);
    uint32 localeEntry = entry;
    if (!pGossip)
    {
        pGossip = sObjectMgr->GetGossipText(DEFAULT_GOSSIP_MESSAGE);
        localeEntry = DEFAULT_GOSSIP_MESSAGE;
    }

    std::string Text_0, Text_1;
    if(pGossip)
    {
        Text_0 = pGossip->Options[0].Text_0;
        Text_1 = pGossip->Options[1].Text_1;
        
        if (locale >= 0)
        {
            if (NpcTextLocale const* nl = sObjectMgr->GetNpcTextLocale(localeEntry))
            {
                ObjectMgr::GetLocaleString(nl->Text_0[0], locale, Text_0);
                ObjectMgr::GetLocaleString(nl->Text_1[1], locale, Text_1);
            }
        }
    }
    
    uint16 size1 = Text_0.length();
    uint16 size2 = 0/*Text_1.length()*/; // no send.

    buff << uint32(entry);
    buff << uint32(0);
    buff << uint16(size1);
    if (size1)
        buff.WriteString(Text_0);
    buff << uint16(size2);
    if (size2)
        buff.WriteString(Text_1);
    buff << uint32(pGossip ? pGossip->Options[0].Emotes[0]._Emote : 0); // Emote
    buff << uint32(0);
    buff << uint32(0);
    buff << uint32(pGossip ? pGossip->Options[0].Emotes[0]._Delay : 0); // Delay
    buff << uint32(0);
    buff << uint32(0);
    buff << uint32(0); //SoundID
    buff << uint32(0); //UnkEmoteID
    buff << uint32(0); //Type Id
}