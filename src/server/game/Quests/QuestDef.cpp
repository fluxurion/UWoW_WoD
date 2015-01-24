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

#include "QuestDef.h"
#include "Player.h"
#include "World.h"
#include "ObjectMgr.h"

Quest::Quest(Field* questRecord)
{
    uint8 index = 0;

    Id                      = questRecord[index++].GetUInt32();
    Type                    = questRecord[index++].GetUInt8();
    Level                   = questRecord[index++].GetInt16();
    PackageID               = questRecord[index++].GetUInt32();
    MinLevel                = uint32(questRecord[index++].GetInt16());
    MaxLevel                = uint32(questRecord[index++].GetInt16());
    QuestSortID              = questRecord[index++].GetInt16();
    QuestInfoID             = questRecord[index++].GetUInt16();
    SuggestedPlayers        = questRecord[index++].GetUInt8();
    RequiredTeam            = questRecord[index++].GetInt8();
    RequiredSkillId         = questRecord[index++].GetUInt16();
    RequiredSkillPoints     = questRecord[index++].GetUInt16();
    RequiredFactionId1      = questRecord[index++].GetUInt16();
    RequiredFactionId2      = questRecord[index++].GetUInt16();
    RequiredFactionValue1   = questRecord[index++].GetInt32();
    RequiredFactionValue2   = questRecord[index++].GetInt32();
    RequiredMinRepFaction   = questRecord[index++].GetUInt16();
    RequiredMaxRepFaction   = questRecord[index++].GetUInt16();
    RequiredMinRepValue     = questRecord[index++].GetInt32();
    RequiredMaxRepValue     = questRecord[index++].GetInt32();
    PrevQuestId             = questRecord[index++].GetInt32();
    NextQuestId             = questRecord[index++].GetInt32();
    ExclusiveGroup          = questRecord[index++].GetInt32();
    NextQuestIdChain        = questRecord[index++].GetUInt32();
    RewardXPDifficulty      = questRecord[index++].GetUInt8();
    Float10                 = questRecord[index++].GetFloat();
    RewardMoney             = questRecord[index++].GetInt32();
    RewardMoneyDifficulty   = questRecord[index++].GetUInt32();
    Float13                 = questRecord[index++].GetFloat();
    RewardBonusMoney        = questRecord[index++].GetUInt32();
    RewardDisplaySpell      = questRecord[index++].GetUInt32();
    RewardSpell             = questRecord[index++].GetInt32();
    RewardHonor             = questRecord[index++].GetUInt32();
    RewardKillHonor         = questRecord[index++].GetFloat();
    RewardMailTemplateId    = questRecord[index++].GetUInt32();
    RewardMailDelay         = questRecord[index++].GetUInt32();
    SourceItemId            = questRecord[index++].GetUInt32();
    SourceItemIdCount       = questRecord[index++].GetUInt8();
    SourceSpellid           = questRecord[index++].GetUInt32();
    Flags                   = questRecord[index++].GetUInt32();
    FlagsEx                 = questRecord[index++].GetUInt32();
    SpecialFlags            = questRecord[index++].GetUInt8();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
    {
        RewardItemId[i]     = questRecord[index++].GetUInt32();
        RewardItemCount[i]  = questRecord[index++].GetUInt32();    
    }

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
    {
        ItemDrop[i]         = questRecord[index++].GetUInt32();
        ItemDropQuantity[i] = questRecord[index++].GetUInt32();
    }

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
    {
        RewardChoiceItemId[i] = questRecord[index++].GetUInt32();
        RewardChoiceItemCount[i] = questRecord[index++].GetUInt32();
        RewardChoiceItemDisplayId[i] = questRecord[index++].GetUInt32();
    }

    POIContinent            = questRecord[index++].GetUInt16();
    POIx                    = questRecord[index++].GetFloat();
    POIy                    = questRecord[index++].GetFloat();
    POIPriority             = questRecord[index++].GetUInt32();
    RewardTitleId           = questRecord[index++].GetUInt8();
    RewardTalents           = questRecord[index++].GetUInt8();
    RewardArenaPoints       = questRecord[index++].GetUInt16();
    RewardSkillId           = questRecord[index++].GetUInt16();
    RewardSkillPoints       = questRecord[index++].GetUInt8();
    QuestGiverPortrait      = questRecord[index++].GetUInt32();
    QuestTurnInPortrait     = questRecord[index++].GetUInt32();
  
    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
    {
        RewardFactionId[i] = questRecord[index++].GetUInt32();
        RewardFactionValue[i] = questRecord[index++].GetInt32();
        RewardFactionOverride[i] = questRecord[index++].GetInt32();
    }
    RewardFactionFlags    = questRecord[index++].GetUInt32();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        RequirementType[i] = questRecord[index++].GetUInt8();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        RequiredId[i] = questRecord[index++].GetUInt32();
        RequiredIdBack[i] = RequiredId[i];
    }

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        RequiredIdCount[i] = questRecord[index++].GetUInt32();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        RequiredPOI[i] = questRecord[index++].GetUInt32();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        RequiredUnkFlag[i] = questRecord[index++].GetUInt32();

    RequiredSpell           = questRecord[index++].GetUInt32();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i) // To be removed
        RequiredSpellCast[i] = questRecord[index++].GetUInt32();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        ObjectiveText[i] = questRecord[index++].GetString();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        if (!RequiredIdCount[i])
            continue;
        QuestObjective obj;
        obj.ID = RequiredPOI[i];
        obj.Type = RequirementType[i];
        obj.StorageIndex = i;
        obj.ObjectID = RequiredId[i];
        obj.Amount = RequiredIdCount[i];
        obj.Flags = RequiredUnkFlag[i];
        obj.UnkFloat = 0.0f;
        obj.Description = ObjectiveText[i];

        Objectives.push_back(obj);
    }

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
    {
        RewardCurrencyId[i] = questRecord[index++].GetUInt32();
        RewardCurrencyCount[i] = questRecord[index++].GetUInt32();
    }

    SoundAccept             = questRecord[index++].GetUInt16();
    SoundTurnIn             = questRecord[index++].GetUInt16();
    AreaGroupID             = questRecord[index++].GetUInt32();
    LimitTime               = questRecord[index++].GetUInt32();
    AllowableRaces          = questRecord[index++].GetInt32();
    AllowableClasses        = questRecord[index++].GetInt32();

    LogTitle                = questRecord[index++].GetString();
    QuestDescription        = questRecord[index++].GetString();
    LogDescription          = questRecord[index++].GetString();
    AreaDescription         = questRecord[index++].GetString();
    QuestCompletionLog      = questRecord[index++].GetString();
    OfferRewardText         = questRecord[index++].GetString();
    PortraitGiverText    = questRecord[index++].GetString();
    PortraitGiverName    = questRecord[index++].GetString();
    PortraitTurnInText     = questRecord[index++].GetString();
    PortraitTurnInName     = questRecord[index++].GetString();
    RequestItemsText        = questRecord[index++].GetString();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmote[i] = questRecord[index++].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmoteDelay[i] = questRecord[index++].GetUInt32();

    EmoteOnIncomplete       = questRecord[index++].GetUInt16();
    EmoteOnComplete         = questRecord[index++].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmote[i] = questRecord[index++].GetInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmoteDelay[i] = questRecord[index++].GetInt32();

    StartScript             = questRecord[index++].GetUInt32();
    CompleteScript          = questRecord[index++].GetUInt32();

    // int32 WDBVerified = questRecord[index++].GetInt32();

    if (SpecialFlags & QUEST_SPECIAL_FLAGS_AUTO_ACCEPT)
        Flags |= QUEST_FLAGS_AUTO_ACCEPT;

    m_reqItemsCount = 0;
    m_reqNpcOrGoCount = 0;
    m_rewItemsCount = 0;
    m_rewChoiceItemsCount = 0;
    m_rewCurrencyCount = 0;
    m_reqCurrencyCount = 0;

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        RequiredNpcOrGo[i] = 0;
        RequiredNpcOrGoCount[i] = 0;
        RequiredItemId[i] = 0;
        RequiredItemCount[i] = 0;
        RequiredCurrencyId[i] = 0;
        RequiredCurrencyCount[i] = 0;
        if (RequiredId[i])
        {
            switch(RequirementType[i])
            {
                case 0:
                    ++m_reqNpcOrGoCount;
                    RequiredNpcOrGo[i] = RequiredId[i];
                    RequiredNpcOrGoCount[i] = RequiredIdCount[i];
                    break;
                case 1:
                    ++m_reqItemsCount;
                    RequiredItemId[i] = RequiredId[i];
                    RequiredItemCount[i] = RequiredIdCount[i];
                    break;
                case 2:
                    ++m_reqNpcOrGoCount;
                    RequiredNpcOrGo[i] = int32((-1)*RequiredId[i]);
                    RequiredNpcOrGoCount[i] = RequiredIdCount[i];
                    break;
                case 4:
                    ++m_reqCurrencyCount;
                    RequiredCurrencyId[i] = RequiredId[i];
                    RequiredCurrencyCount[i] = RequiredIdCount[i];
                    break;
            }
        }
    }

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        if (RewardItemId[i])
            ++m_rewItemsCount;

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        if (RewardChoiceItemId[i])
            ++m_rewChoiceItemsCount;

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        if (RewardCurrencyId[i])
            ++m_rewCurrencyCount;
}

uint32 Quest::XPValue(Player* player) const
{
    if (player)
    {
        int32 quest_level = (Level == -1 ? player->getLevel() : Level);
        const QuestXPEntry* xpentry = sQuestXPStore.LookupEntry(quest_level);
        if (!xpentry)
            return 0;

        int32 diffFactor = 2 * (quest_level - player->getLevel()) + 20;
        if (diffFactor < 1)
            diffFactor = 1;
        else if (diffFactor > 10)
            diffFactor = 10;

        uint32 xp = diffFactor * xpentry->Exp[RewardXPDifficulty] / 10;
        if (xp <= 100)
            xp = 5 * ((xp + 2) / 5);
        else if (xp <= 500)
            xp = 10 * ((xp + 5) / 10);
        else if (xp <= 1000)
            xp = 25 * ((xp + 12) / 25);
        else
            xp = 50 * ((xp + 25) / 50);

        return xp;
    }

    return 0;
}

uint32 Quest::GetRewMoney() const
{
    if (RewardMoney > 0)
        return RewardMoney * sWorld->getRate(RATE_DROP_MONEY);
    else
        return 0;
}

void Quest::BuildExtraQuestInfo(WorldPacket& data, Player* player) const
{
    data << uint32(GetRewChoiceItemsCount());
    for (uint8 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
    {
        data << uint32(RewardChoiceItemId[i]);
        data << uint32(RewardChoiceItemCount[i]);
        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(RewardChoiceItemId[i]))
            data << uint32(itemTemplate->DisplayInfoID);
        else
            data << uint32(0);
    }

    data << uint32(GetReqItemsCount());
    for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        data << uint32(RewardItemId[i]);
    for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        data << uint32(RewardItemCount[i]);
    for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
    {
        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(RewardItemId[i]))
            data << uint32(itemTemplate->DisplayInfoID);
        else
            data << uint32(0);
    }

    data << int32(GetRewMoney());

    float QuestXpRate = 1;
    if(player->GetPersonnalXpRate())
        QuestXpRate = player->GetPersonnalXpRate();
    else
        QuestXpRate = sWorld->getRate(RATE_XP_QUEST);

    data << uint32(XPValue(player) * QuestXpRate);

    data << uint32(0);                                      // unk
    data << uint32(GetBonusTalents());
    data << uint32(GetRewardFactionFlags());

    /* Pre cata struct, some of these unks might be the missing values in cata:
    // rewarded honor points. Multiply with 10 to satisfy client
    data << uint32(10 * quest->CalculateHonorGain(_session->GetPlayer()->GetQuestLevel(quest)));
    data << float(0.0f);                                    // unk, honor multiplier?
    data << uint32(0x08);                                   // unused by client?
    data << uint32(quest->GetRewDisplaySpell());                   // reward spell, this spell will display (icon) (casted if RewSpellCast == 0)
    data << int32(quest->GetRewSpell());                // casted spell
    data << uint32(0);                                      // unknown
    data << uint32(quest->GetBonusTalents());               // bonus talents
    data << uint32(quest->GetRewArenaPoints());             // arena points
    data << uint32(0);
    */

    for (uint8 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)    // reward factions ids
        data << uint32(RewardFactionId[i]);

    for (uint8 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)    // columnid in QuestFactionReward.dbc (zero based)?
        data << int32(RewardFactionValue[i]);

    for (uint8 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)    // reward reputation override?
        data << uint32(RewardFactionOverride[i]);

    data << uint32(GetRewDisplaySpell());
    data << uint32(GetRewSpell());

    for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        data << uint32(RewardCurrencyId[i]);

    for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        data << uint32(RewardCurrencyCount[i]);

    data << uint32(GetRewardSkillId());
    data << uint32(GetRewardSkillPoints());
}

uint32 Quest::GetRewMoneyMaxLevel() const
{
    // If Quest has flag to not give money on max level, it's 0
    if (HasFlag(QUEST_FLAGS_NO_MONEY_FROM_XP))
        return 0;

    // Else, return the rewarded copper sum modified by the rate
    return uint32(RewardBonusMoney/* * sWorld->getRate(RATE_MONEY_MAX_LEVEL_QUEST)*/);
}

bool Quest::IsAutoAccept() const
{
    return !sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_AUTO_ACCEPT) && HasFlag(QUEST_FLAGS_AUTO_ACCEPT);
}

bool Quest::IsAutoComplete() const
{
    return !sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_AUTO_COMPLETE) && (Type == 0/* || HasFlag(QUEST_FLAGS_AUTOCOMPLETE)*/);
}

bool Quest::IsAllowedInRaid() const
{
    if (IsRaidQuest())
        return true;

    return sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_RAID);
}

uint32 Quest::CalculateHonorGain(uint8 level) const
{
    if (level > GT_MAX_LEVEL)
        level = GT_MAX_LEVEL;

    uint32 honor = 0;

    /*if (GetRewHonor() > 0 || GetRewKillHonor() > 0.0f)
    {
        // values stored from 0.. for 1...
        TeamContributionPointsEntry const* tc = sTeamContributionPointsStore.LookupEntry(level);
        if (!tc)
            return 0;
        honor = uint32(tc->value * GetRewKillHonor() * 0.1f);
        honor += GetRewHonor();
    }*/

    return honor;
}

uint32 Quest::GetItemFromPakage(uint32 packItemId) const
{
    if(GetQuestPackageID() == 0)
        return 0;

    std::list<uint32> tempList = GetPackageItemList(GetQuestPackageID());
    for (std::list<uint32>::const_iterator itr = tempList.begin(); itr != tempList.end(); ++itr)
        if (QuestPackageItem const* sp = sQuestPackageItemStore.LookupEntry((*itr)))
            if(sp->ItemID == packItemId)
                return (*itr);

    return 0;
}

uint32 Quest::GetRewItemDisplayId(uint8 index) const
{
    uint32 itemId = RewardItemId[index];
    if (!itemId)
        return 0;

    ItemTemplate const* item = sObjectMgr->GetItemTemplate(itemId);
    return item ? item->DisplayInfoID : 0;
}

uint32 Quest::GetRewChoiceItemDisplayId(uint8 index) const
{
    uint32 itemId = RewardChoiceItemId[index];
    if (!itemId)
        return 0;

    ItemTemplate const* item = sObjectMgr->GetItemTemplate(itemId);
    return item ? item->DisplayInfoID : 0;
}
