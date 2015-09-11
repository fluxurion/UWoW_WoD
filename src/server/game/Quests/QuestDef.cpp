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
#include "QuestPackets.h"

Quest::Quest(Field* questRecord)
{
    uint8 index = 0;

    Id                      = questRecord[index++].GetUInt32();
    Type                    = questRecord[index++].GetUInt8();
    Level                   = questRecord[index++].GetInt16();
    PackageID               = questRecord[index++].GetUInt32();
    MinLevel                = uint32(questRecord[index++].GetInt16());
    QuestSortID             = questRecord[index++].GetInt16();
    QuestInfoID             = questRecord[index++].GetUInt16();
    SuggestedPlayers        = questRecord[index++].GetUInt8();
    NextQuestIdChain        = questRecord[index++].GetUInt32(); //RewardNextQuest on DB
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
    SourceItemId            = questRecord[index++].GetUInt32();
    Flags                   = questRecord[index++].GetUInt32();
    FlagsEx                 = questRecord[index++].GetUInt32();

    for (int i = 0; i < QUEST_REWARD_ITEM_COUNT; ++i)
    {
        RewardItemId[i]     = questRecord[index++].GetUInt32();
        RewardItemCount[i]  = questRecord[index++].GetUInt32();    
    }

    for (int i = 0; i < QUEST_ITEM_DROP_COUNT; ++i)
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
  
    for (int i = 0; i < QUEST_REWARD_REPUTATIONS_COUNT; ++i)
    {
        RewardFactionId[i] = questRecord[index++].GetUInt32();
        RewardFactionValue[i] = questRecord[index++].GetInt32();
        RewardFactionOverride[i] = questRecord[index++].GetInt32();
    }
    RewardFactionFlags    = questRecord[index++].GetUInt32();

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

    LogTitle                = questRecord[index++].GetString();
    QuestDescription        = questRecord[index++].GetString();
    LogDescription          = questRecord[index++].GetString();
    AreaDescription         = questRecord[index++].GetString();
    QuestCompletionLog      = questRecord[index++].GetString();
    PortraitGiverText       = questRecord[index++].GetString();
    PortraitGiverName       = questRecord[index++].GetString();
    PortraitTurnInText      = questRecord[index++].GetString();
    PortraitTurnInName      = questRecord[index++].GetString();

    StartScript             = questRecord[index++].GetUInt32();
    CompleteScript          = questRecord[index++].GetUInt32();

    // int32 WDBVerified = questRecord[index++].GetInt32();

    if (SpecialFlags & QUEST_SPECIAL_FLAGS_AUTO_ACCEPT)
        Flags |= QUEST_FLAGS_AUTO_ACCEPT;

    m_rewItemsCount = 0;
    m_rewChoiceItemsCount = 0;
    m_rewCurrencyCount = 0;
    m_reqCurrencyCount = 0;

    for (int i = 0; i < QUEST_REWARD_ITEM_COUNT; ++i)
        if (RewardItemId[i])
            ++m_rewItemsCount;

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        if (RewardChoiceItemId[i])
            ++m_rewChoiceItemsCount;

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        if (RewardCurrencyId[i])
            ++m_rewCurrencyCount;
}

void Quest::LoadQuestDetails(Field* fields)
{
    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmote[i] = fields[1+i].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmoteDelay[i] = fields[5+i].GetUInt32();
}

void Quest::LoadQuestRequestItems(Field* fields)
{
    EmoteOnComplete = fields[1].GetUInt16();
    EmoteOnIncomplete = fields[2].GetUInt16();
    EmoteOnCompleteDelay = fields[3].GetUInt32();
    EmoteOnIncompleteDelay = fields[4].GetUInt32();
    RequestItemsText = fields[5].GetString();
}


void Quest::LoadQuestOfferReward(Field* fields)
{
    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmote[i] = fields[1+i].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmoteDelay[i] = fields[5+i].GetUInt32();

    OfferRewardText = fields[9].GetString();
}

void Quest::LoadQuestTemplateAddon(Field* fields)
{
    MaxLevel = fields[1].GetUInt8();
    AllowableClasses = fields[2].GetUInt32();
    SourceSpellID = fields[3].GetUInt32();
    PrevQuestID = fields[4].GetInt32();
    NextQuestID = fields[5].GetInt32();
    ExclusiveGroup = fields[6].GetInt32();
    RewardMailTemplateId = fields[7].GetUInt32();
    RewardMailDelay = fields[8].GetUInt32();
    RequiredSkillId = fields[9].GetUInt16();
    RequiredSkillPoints = fields[10].GetUInt16();
    RequiredMinRepFaction = fields[11].GetUInt16();
    RequiredMaxRepFaction = fields[12].GetUInt16();
    RequiredMinRepValue = fields[13].GetInt32();
    RequiredMaxRepValue = fields[14].GetInt32();
    SourceItemIdCount = fields[15].GetUInt8();
    SpecialFlags = fields[16].GetUInt8();

    if (SpecialFlags & QUEST_SPECIAL_FLAGS_AUTO_ACCEPT)
        Flags |= QUEST_FLAGS_AUTO_ACCEPT;
}

void Quest::LoadQuestObjective(Field* fields)
{
    QuestObjective obj;
    obj.ID = fields[0].GetUInt32();
    obj.Type = fields[2].GetUInt8();
    obj.StorageIndex = fields[3].GetInt8();
    obj.ObjectID = fields[4].GetInt32();
    obj.Amount = fields[5].GetInt32();
    obj.Flags = fields[6].GetUInt32();
    obj.UnkFloat = fields[7].GetFloat();
    obj.Description = fields[8].GetString();

    Objectives.push_back(obj);
}

void Quest::LoadQuestObjectiveVisualEffect(Field* fields)
{
    uint32 objID = fields[1].GetUInt32();

    for (QuestObjective& obj : Objectives)
    {
        if (obj.ID == objID)
        {
            uint8 effectIndex = fields[3].GetUInt8();
            if (effectIndex >= obj.VisualEffects.size())
                obj.VisualEffects.resize(effectIndex+1, 0);

            obj.VisualEffects[effectIndex] = fields[4].GetInt32();
            break;
        }
    }
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

void Quest::BuildQuestRewards(WorldPackets::Quest::QuestRewards& rewards, Player* player) const
{
    rewards.ChoiceItemCount         = GetRewChoiceItemsCount();
    rewards.ItemCount               = GetRewItemsCount();
    rewards.Money                   = GetRewMoney();
    float QuestXpRate = 1;
    if (player->GetPersonnalXpRate())
        QuestXpRate = player->GetPersonnalXpRate();
    else
        QuestXpRate = sWorld->getRate(RATE_XP_QUEST);

    rewards.XP                      = uint32(XPValue(player) * QuestXpRate);
    rewards.Title                   = GetRewTitle();
    rewards.Talents                 = GetBonusTalents();
    rewards.FactionFlags            = GetRewardFactionFlags();
    rewards.SpellCompletionDisplayID = GetRewDisplaySpell();
    rewards.SpellCompletionID       = GetRewSpell();
    rewards.SkillLineID             = GetRewardSkillId();
    rewards.NumSkillUps             = GetRewardSkillPoints();

    for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
    {
        rewards.ChoiceItems[i].ItemID = RewardChoiceItemId[i];
        rewards.ChoiceItems[i].Quantity = RewardChoiceItemCount[i];
    }

    for (uint32 i = 0; i < QUEST_REWARD_ITEM_COUNT; ++i)
    {
        rewards.ItemID[i] = RewardItemId[i];
        rewards.ItemQty[i] = RewardItemCount[i];
    }

    for (uint32 i = 0; i < QUEST_REWARD_REPUTATIONS_COUNT; ++i)
    {
        rewards.FactionID[i] = RewardFactionId[i];
        rewards.FactionValue[i] = RewardFactionValue[i];
        rewards.FactionOverride[i] = RewardFactionOverride[i];
    }

    for (uint32 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
    {
        rewards.CurrencyID[i] = RewardCurrencyId[i];
        rewards.CurrencyQty[i] = RewardCurrencyCount[i];
    }
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

bool Quest::IsRaidQuest(Difficulty difficulty) const
{
    switch (Type)
    {
        case QUEST_INFO_RAID:
            return true;
        case QUEST_INFO_RAID_10:
            return difficulty == DIFFICULTY_10_N || difficulty == DIFFICULTY_10_HC;
        case QUEST_INFO_RAID_25:
            return difficulty == DIFFICULTY_25_N || difficulty == DIFFICULTY_25_HC;
        default:
            break;
    }

    return false;
}

bool Quest::IsAllowedInRaid(Difficulty difficulty) const
{
    if (IsRaidQuest(difficulty))
        return true;

    return sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_RAID);
}

uint32 Quest::CalculateHonorGain(uint8 level) const
{
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

    return GetItemDisplayId(itemId, 0);
}

uint32 Quest::GetRewChoiceItemDisplayId(uint8 index) const
{
    uint32 itemId = RewardChoiceItemId[index];
    if (!itemId)
        return 0;

    return GetItemDisplayId(itemId, 0);
}
