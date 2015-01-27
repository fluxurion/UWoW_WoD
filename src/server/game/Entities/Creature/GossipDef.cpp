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
#include "GossipDef.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Formulas.h"
#include "QuestPackets.h"
#include "NPCPackets.h"

GossipMenu::GossipMenu()
{
    _menuId = 0;
}

GossipMenu::~GossipMenu()
{
    ClearMenu();
}

void GossipMenu::AddMenuItem(int32 menuItemId, uint8 icon, std::string const& message, uint32 sender, uint32 action, std::string const& boxMessage, uint32 boxMoney, bool coded /*= false*/)
{
    if(_menuItems.size() > GOSSIP_MAX_MENU_ITEMS)
        return;
    //ASSERT(_menuItems.size() <= GOSSIP_MAX_MENU_ITEMS);

    // Find a free new id - script case
    if (menuItemId == -1)
    {
        menuItemId = 0;
        if (!_menuItems.empty())
        {
            for (GossipMenuItemContainer::const_iterator itr = _menuItems.begin(); itr != _menuItems.end(); ++itr)
            {
                if (int32(itr->first) > menuItemId)
                    break;

                menuItemId = itr->first + 1;
            }
        }
    }

    GossipMenuItem& menuItem = _menuItems[menuItemId];

    menuItem.MenuItemIcon    = icon;
    menuItem.Message         = message;
    menuItem.IsCoded         = coded;
    menuItem.Sender          = sender;
    menuItem.OptionType      = action;
    menuItem.BoxMessage      = boxMessage;
    menuItem.BoxMoney        = boxMoney;
}

void GossipMenu::AddGossipMenuItemData(uint32 menuItemId, uint32 gossipActionMenuId, uint32 gossipActionPoi)
{
    GossipMenuItemData& itemData = _menuItemData[menuItemId];

    itemData.GossipActionMenuId  = gossipActionMenuId;
    itemData.GossipActionPoi     = gossipActionPoi;
}

uint32 GossipMenu::GetMenuItemSender(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return 0;

    return itr->second.Sender;
}

uint32 GossipMenu::GetMenuItemAction(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return 0;

    return itr->second.OptionType;
}

bool GossipMenu::IsMenuItemCoded(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return false;

    return itr->second.IsCoded;
}

void GossipMenu::ClearMenu()
{
    _menuItems.clear();
    _menuItemData.clear();
}

PlayerMenu::PlayerMenu(WorldSession* session) : _session(session)
{
}

PlayerMenu::~PlayerMenu()
{
    ClearMenus();
}

void PlayerMenu::ClearMenus()
{
    _gossipMenu.ClearMenu();
    _questMenu.ClearMenu();
}

void PlayerMenu::SendGossipMenu(uint32 titleTextId, ObjectGuid objectGUID) const
{
    WorldPacket data(SMSG_GOSSIP_MESSAGE, 100);         // guess size

    data.WriteBits(_gossipMenu.GetMenuItemCount(), 20);
    //data.WriteGuidMask<5, 1, 7, 2>(objectGUID);
    data.WriteBits(_questMenu.GetMenuItemCount(), 19);
    //data.WriteGuidMask<6, 4, 0>(objectGUID);

    ByteBuffer buffer;
    for (uint32 iI = 0; iI < _questMenu.GetMenuItemCount(); ++iI)
    {
        QuestMenuItem const& item = _questMenu.GetItem(iI);
        uint32 questID = item.QuestId;
        Quest const* quest = sObjectMgr->GetQuestTemplate(questID);

        data.WriteBit(0);                               // 3.3.3 changes icon: blue question or yellow exclamation
        std::string title = quest->GetLogTitle();
        int locale = _session->GetSessionDbLocaleIndex();
        if (locale >= 0)
            if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(questID))
                ObjectMgr::GetLocaleString(localeData->LogTitle, locale, title);
        data.WriteBits(title.size(), 9);

        buffer << uint32(quest->GetFlags());            // 3.3.3 quest flags
        buffer << uint32(item.QuestIcon);
        buffer << int32(quest->GetQuestLevel());
        buffer.WriteString(title);
        buffer << uint32(questID);
        buffer << uint32(0);                            // quest flags2
    }

    //buffer.WriteGuidBytes<2, 1>(objectGUID);
    buffer << uint32(_gossipMenu.GetMenuId());          // new 2.4.0

    for (GossipMenuItemContainer::const_iterator itr = _gossipMenu.GetMenuItems().begin(); itr != _gossipMenu.GetMenuItems().end(); ++itr)
    {
        GossipMenuItem const& item = itr->second;
        data.WriteBits(item.BoxMessage.size(), 12);
        data.WriteBits(item.Message.size(), 12);

        buffer.WriteString(item.Message);               // text for gossip item
        buffer << uint8(item.IsCoded);                  // makes pop up box password
        buffer << uint8(item.MenuItemIcon);
        buffer << uint32(item.BoxMoney);                // money required to open menu, 2.0.3
        buffer.WriteString(item.BoxMessage);            // accept text (related to money) pop up box, 2.0.3
        buffer << uint32(itr->first);
    }

    //data.WriteGuidMask<3>(objectGUID);
    data.FlushBits();
    if (!buffer.empty())
        data.append(buffer);

    //data.WriteGuidBytes<7, 4, 6>(objectGUID);
    data << uint32(0);                                  // friendship faction
    //data.WriteGuidBytes<0, 5>(objectGUID);
    data << uint32(titleTextId);
    //data.WriteGuidBytes<3>(objectGUID);

    _session->SendPacket(&data);
}

void PlayerMenu::SendCloseGossip() const
{
    WorldPacket data(SMSG_GOSSIP_COMPLETE, 0);
    _session->SendPacket(&data);
}

void PlayerMenu::SendPointOfInterest(uint32 poiId) const
{
    PointOfInterest const* poi = sObjectMgr->GetPointOfInterest(poiId);
    if (!poi)
    {
        sLog->outError(LOG_FILTER_SQL, "Request to send non-existing POI (Id: %u), ignored.", poiId);
        return;
    }

    std::string iconText = poi->icon_name;
    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
        if (PointOfInterestLocale const* localeData = sObjectMgr->GetPointOfInterestLocale(poiId))
            ObjectMgr::GetLocaleString(localeData->IconName, locale, iconText);

    WorldPacket data(SMSG_GOSSIP_POI, 4 + 4 + 4 + 4 + 4 + 10);  // guess size
    data << uint32(poi->flags);
    data << float(poi->x);
    data << float(poi->y);
    data << uint32(poi->icon);
    data << uint32(poi->data);
    data << iconText;

    _session->SendPacket(&data);
}

/*********************************************************/
/***                    QUEST SYSTEM                   ***/
/*********************************************************/

QuestMenu::QuestMenu()
{
    _questMenuItems.reserve(16);                                   // can be set for max from most often sizes to speedup push_back and less memory use
}

QuestMenu::~QuestMenu()
{
    ClearMenu();
}

void QuestMenu::AddMenuItem(uint32 QuestId, uint8 Icon)
{
    if (!sObjectMgr->GetQuestTemplate(QuestId))
        return;

    ASSERT(_questMenuItems.size() <= GOSSIP_MAX_MENU_ITEMS);

    QuestMenuItem questMenuItem;

    questMenuItem.QuestId        = QuestId;
    questMenuItem.QuestIcon      = Icon;

    _questMenuItems.push_back(questMenuItem);
}

bool QuestMenu::HasItem(uint32 questId) const
{
    for (QuestMenuItemList::const_iterator i = _questMenuItems.begin(); i != _questMenuItems.end(); ++i)
        if (i->QuestId == questId)
            return true;

    return false;
}

void QuestMenu::ClearMenu()
{
    _questMenuItems.clear();
}

void PlayerMenu::SendQuestGiverQuestList(QEmote eEmote, const std::string& Title, ObjectGuid npcGUID)
{
    WorldPacket data(SMSG_QUESTGIVER_QUEST_LIST, 200);      // guess size
    data.WriteBits(Title.size(), 11);
    //data.WriteGuidMask<0, 4>(npcGUID);

    uint32 menuCount = 0;
    for (uint32 i = 0; i < _questMenu.GetMenuItemCount(); ++i)
        if (sObjectMgr->GetQuestTemplate(_questMenu.GetItem(i).QuestId))
            ++menuCount;
    data.WriteBits(menuCount, 19);
    //data.WriteGuidMask<7>(npcGUID);

    ByteBuffer buff;
    for (uint32 i = 0; i < _questMenu.GetMenuItemCount(); ++i)
    {
        QuestMenuItem const& qmi = _questMenu.GetItem(i);

        uint32 questID = qmi.QuestId;

        if (Quest const* quest = sObjectMgr->GetQuestTemplate(questID))
        {
            std::string title = quest->GetLogTitle();

            Player* plr = _session->GetPlayer();

            int loc_idx = _session->GetSessionDbLocaleIndex();
            if (loc_idx >= 0)
                if (QuestLocale const* ql = sObjectMgr->GetQuestLocale(questID))
                    ObjectMgr::GetLocaleString(ql->LogTitle, loc_idx, title);

            uint32 questStatus = plr ? plr->GetQuestStatus(questID) : 0;

            if (questStatus == QUEST_STATUS_COMPLETE)
                questStatus = 3;
            else if (questStatus == QUEST_STATUS_NONE)
                questStatus = 1;
            else if (questStatus == QUEST_STATUS_INCOMPLETE)
                questStatus = 2;

            data.WriteBit(0);                               // 3.3.3 changes icon: blue question or yellow exclamation
            data.WriteBits(title.size(), 9);

            buff << uint32(0);                              // quest flags 2
            buff.WriteString(title);
            buff << int32(quest->GetQuestLevel());
            buff << uint32(questID);
            buff << uint32(quest->GetFlags());              // 3.3.3 quest flags
            buff << uint32(questStatus);
        }
    }

    //data.WriteGuidMask<2, 6, 3, 5, 1>(npcGUID);
    if (!buff.empty())
    {
        data.FlushBits();
        data.append(buff);
    }

    //data.WriteGuidBytes<5, 4, 0, 7, 1, 6>(npcGUID);
    data << uint32(eEmote._Emote);                         // NPC emote
    //data.WriteGuidBytes<3>(npcGUID);
    data << uint32(eEmote._Delay);                         // player emote
    data.WriteString(Title);
    //data.WriteGuidBytes<2>(npcGUID);

    _session->SendPacket(&data);
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUESTGIVER_QUEST_LIST NPC Guid=%u", npcGUID.GetCounter());
}

void PlayerMenu::SendQuestGiverStatus(uint32 questStatus, ObjectGuid npcGUID) const
{
    WorldPacket data(SMSG_QUESTGIVER_STATUS, 4 + 8 + 1);
    //data.WriteGuidMask<3, 6, 7, 1, 4, 0, 2, 5>(npcGUID);
    //data.WriteGuidBytes<1, 3>(npcGUID);
    data << uint32(questStatus);
    //data.WriteGuidBytes<7, 0, 4, 6, 5, 2>(npcGUID);

    _session->SendPacket(&data);
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUESTGIVER_STATUS NPC Guid=%u, status=%u", npcGUID.GetCounter(), questStatus);
}

void PlayerMenu::SendQuestGiverQuestDetails(Quest const* quest, ObjectGuid npcGUID, bool activateAccept, bool isAreaTrigger /*=false*/) const
{
    std::string questTitle           = quest->GetLogTitle();
    std::string questDetails         = quest->GetQuestDescription();
    std::string questObjectives      = quest->GetLogDescription();
    std::string questEndText         = quest->GetAreaDescription();
    std::string questGiverTextWindow = quest->GetPortraitGiverText();
    std::string questGiverTargetName = quest->GetPortraitGiverName();
    std::string questTurnTextWindow  = quest->GetPortraitTurnInText();
    std::string questTurnTargetName  = quest->GetPortraitTurnInName();

    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->LogTitle, locale, questTitle);
            ObjectMgr::GetLocaleString(localeData->QuestDescription, locale, questDetails);
            ObjectMgr::GetLocaleString(localeData->LogDescription, locale, questObjectives);
            ObjectMgr::GetLocaleString(localeData->AreaDescription, locale, questEndText);
            ObjectMgr::GetLocaleString(localeData->PortraitGiverText, locale, questGiverTextWindow);
            ObjectMgr::GetLocaleString(localeData->PortraitGiverName, locale, questGiverTargetName);
            ObjectMgr::GetLocaleString(localeData->PortraitTurnInText, locale, questTurnTextWindow);
            ObjectMgr::GetLocaleString(localeData->PortraitTurnInName, locale, questTurnTargetName);
        }
    }

    WorldPacket data(SMSG_QUESTGIVER_QUEST_DETAILS, 200);           // guess size
    data << uint32(quest->GetQuestGiverPortrait());                 // 4.x
    data << uint32(quest->GetRewItemDisplayId(1));

    for (uint8 i = 0; i < QUEST_REWARD_REPUTATIONS_COUNT; ++i)
    {
        data << uint32(quest->RewardFactionId[i]);                  // reward factions ids
        data << int32(quest->RewardFactionValue[i]);              // columnid in QuestFactionReward.dbc (zero based)?
        data << uint32(quest->RewardFactionOverride[i]);     // reward reputation override?
    }

    data << uint32(quest->GetRewItemDisplayId(2));
    data << int32(quest->GetRewMoney());
    data << uint32(quest->RewardItemId[0]);
    data << uint32(quest->RewardChoiceItemCount[5]);
    data << uint32(quest->GetQuestPackageID());
    data << uint32(quest->GetRewItemDisplayId(3));

    for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
    {
        data << uint32(quest->RewardCurrencyId[i]);
        data << uint32(quest->RewardCurrencyCount[i]);
    }

    data << uint32(0);
    data << uint32(quest->GetRewChoiceItemDisplayId(0));
    data << uint32(quest->GetRewChoiceItemsCount());
    data << uint32(quest->RewardChoiceItemId[3]);
    data << uint32(quest->RewardChoiceItemCount[0]);
    data << uint32(0);
    data << uint32(quest->RewardItemCount[1]);
    data << uint32(0);                                              // quest flags 2
    data << uint32(quest->RewardChoiceItemCount[4]);
    data << uint32(quest->RewardItemCount[3]);
    data << uint32(quest->RewardItemId[2]);
    data << uint32(quest->GetRewDisplaySpell());
    data << uint32(quest->GetRewChoiceItemDisplayId(2));
    data << uint32(0);
    data << uint32(quest->RewardItemId[1]);
    data << uint32(quest->GetFlags());
    data << uint32(quest->GetQuestTurnInPortrait());                // 4.x
    data << uint32(quest->GetQuestId());
    data << uint32(quest->GetSuggestedPlayers());
    data << uint32(quest->RewardChoiceItemId[2]);
    data << uint32(quest->RewardChoiceItemCount[2]);
    data << uint32(0);
    data << uint32(quest->RewardChoiceItemId[0]);
    data << uint32(quest->GetRewItemDisplayId(0));
    data << uint32(quest->RewardChoiceItemCount[3]);
    data << uint32(quest->RewardItemCount[0]);
    data << uint32(quest->GetRewChoiceItemDisplayId(1));
    data << uint32(quest->RewardChoiceItemId[4]);
    data << uint32(quest->GetRewItemsCount());
    data << uint32(quest->RewardChoiceItemCount[1]);
    data << uint32(quest->GetRewChoiceItemDisplayId(5));
    data << uint32(quest->GetRewChoiceItemDisplayId(3));
    data << uint32(0);
    data << uint32(quest->GetRewChoiceItemDisplayId(4));
    data << uint32(quest->GetRewSpell());
    data << uint32(quest->RewardChoiceItemId[1]);
    data << uint32(quest->RewardChoiceItemId[5]);
    data << uint32(0);
    data << uint32(quest->RewardItemId[3]);
    data << uint32(quest->RewardItemCount[2]);

    bool isAutoLaunched = true;                             //activateAccept ? 1 : 0
    ObjectGuid guid2 = isAutoLaunched ? ObjectGuid::Empty : npcGUID;

    if (isAreaTrigger)
    {
        isAutoLaunched = false;
        guid2 = npcGUID;
    }

    //data.WriteGuidMask<0>(npcGUID);
    data.WriteBits(questGiverTextWindow.size(), 10);
    //data.WriteGuidMask<0>(guid2);
    data.WriteBit(isAreaTrigger);                           // from areatrigger
    //data.WriteGuidMask<6>(guid2);
    //data.WriteGuidMask<1, 7>(npcGUID);
    data.WriteBits(questTurnTargetName.size(), 8);
    //data.WriteGuidMask<2>(guid2);
    //data.WriteGuidMask<2>(npcGUID);
    data.WriteBit(isAutoLaunched);
    data.WriteBit(isAreaTrigger);                          // IsFinished? value is sent back to server in quest accept packet
    //data.WriteGuidMask<1>(guid2);
    data.WriteBits(questTitle.size(), 9);
    data.WriteBits(questGiverTargetName.size(), 8);
    //data.WriteGuidMask<4, 6, 3>(npcGUID);
    data.WriteBits(questObjectives.size(), 12);
    //data.WriteGuidMask<7>(guid2);
    data.WriteBits(questTurnTextWindow.size(), 10);
    //data.WriteGuidMask<4>(guid2);
    data.WriteBits(QUEST_EMOTE_COUNT, 21);
    //data.WriteGuidMask<3, 5>(guid2);

    uint8 objCount = 0;
    for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        if (quest->RequiredIdBack[i] != 0)
            ++objCount;
    data.WriteBits(objCount, 20);

    data.WriteBits(0, 22);                                  // counter
    data.WriteBits(questDetails.size(), 12);
    //data.WriteGuidMask<5>(npcGUID);

    //data.WriteGuidBytes<7>(npcGUID);
    data.WriteString(questGiverTextWindow);
    //data.WriteGuidBytes<2>(npcGUID);
    //data.WriteGuidBytes<3>(guid2);
    data.WriteString(questDetails);
    data.WriteString(questObjectives);
    //data.WriteGuidBytes<2, 1>(guid2);

    for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        if (!quest->RequiredIdBack[i])
            continue;

        data << uint32(quest->RequiredIdBack[i]);
        data << uint32(quest->RequiredPOI[i]);
        data << uint32(quest->RequiredIdCount[i]);
        data << uint8(quest->RequirementType[i]);
    }

    //data.WriteGuidBytes<4>(npcGUID);
    //data.WriteGuidBytes<5>(guid2);
    data.WriteString(questGiverTargetName);
    //data.WriteGuidBytes<0, 5>(npcGUID);
    data.WriteString(questTurnTargetName);
    //data.WriteGuidBytes<6>(npcGUID);

    for (uint8 i = 0; i < QUEST_EMOTE_COUNT; ++i)
    {
        data << uint32(quest->DetailsEmote[i]);
        data << uint32(quest->DetailsEmoteDelay[i]);       // DetailsEmoteDelay (in ms)
    }

    //data.WriteGuidBytes<1, 3>(npcGUID);
    //data.WriteGuidBytes<6, 4>(guid2);
    data.WriteString(questTitle);
    data.WriteString(questTurnTextWindow);

    //data.WriteGuidBytes<7, 0>(guid2);

    _session->SendPacket(&data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUESTGIVER_QUEST_DETAILS NPCGuid=%u, questid=%u", npcGUID.GetCounter(), quest->GetQuestId());
}

void PlayerMenu::SendQuestQueryResponse(uint32 questId) const
{
    Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
    if (!quest)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUEST_QUERY_RESPONSE questid=%u, does not exist", questId);
        return;
    }

    bool hideRewards = quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS);
    std::string questTitle = quest->GetLogTitle();
    std::string questDetails = quest->GetQuestDescription();
    std::string questObjectives = quest->GetLogDescription();
    std::string questEndText = quest->GetAreaDescription();
    std::string questQuestCompletionLog = quest->GetQuestCompletionLog();
    std::string portraitGiverText = quest->GetPortraitGiverText();
    std::string portraitGiverName = quest->GetPortraitGiverName();
    std::string portraitTurnInText = quest->GetPortraitTurnInText();
    std::string portraitTurnInName = quest->GetPortraitTurnInName();

    std::string questObjectiveText[QUEST_OBJECTIVES_COUNT];
    for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        questObjectiveText[i] = quest->ObjectiveText[i];

    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->LogTitle, locale, questTitle);
            ObjectMgr::GetLocaleString(localeData->QuestDescription, locale, questDetails);
            ObjectMgr::GetLocaleString(localeData->LogDescription, locale, questObjectives);
            ObjectMgr::GetLocaleString(localeData->AreaDescription, locale, questEndText);
            ObjectMgr::GetLocaleString(localeData->QuestCompletionLog, locale, questQuestCompletionLog);
            ObjectMgr::GetLocaleString(localeData->PortraitGiverText, locale, portraitGiverText);
            ObjectMgr::GetLocaleString(localeData->PortraitGiverName, locale, portraitGiverName);
            ObjectMgr::GetLocaleString(localeData->PortraitTurnInText, locale, portraitTurnInText);
            ObjectMgr::GetLocaleString(localeData->PortraitTurnInName, locale, portraitTurnInName);

            for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
                ObjectMgr::GetLocaleString(localeData->ObjectiveText[i], locale, questObjectiveText[i]);
        }
    }

    WorldPackets::Quest::QueryQuestInfoResponse packet;

    packet.Allow = true;
    packet.QuestID = quest->GetQuestId();

    packet.Info.QuestID = quest->GetQuestId();
    packet.Info.QuestType = quest->GetQuestType();
    packet.Info.QuestLevel = quest->GetQuestLevel();
    packet.Info.QuestPackageID = quest->GetQuestPackageID();
    packet.Info.QuestMinLevel = quest->GetMinLevel();
    packet.Info.QuestSortID = quest->GetZoneOrSort();
    packet.Info.QuestInfoID = quest->GetQuestInfoID();
    packet.Info.SuggestedGroupNum = quest->GetSuggestedPlayers();
    packet.Info.RewardNextQuest = quest->GetNextQuestInChain();
    packet.Info.RewardXPDifficulty = quest->GetXPDifficulty();
    packet.Info.Float10 = quest->Float10; // Unk
    packet.Info.Float13 = quest->Float13; // Unk

    if (quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS))
        packet.Info.RewardMoney = quest->RewardMoney;

    packet.Info.RewardMoneyDifficulty = quest->GetRewMoneyDifficulty();
    packet.Info.RewardBonusMoney = quest->GetRewMoneyMaxLevel();
    packet.Info.RewardDisplaySpell = quest->GetRewDisplaySpell();
    packet.Info.RewardSpell = quest->GetRewDisplaySpell();

    packet.Info.RewardHonor = quest->GetRewHonor();
    packet.Info.RewardKillHonor = quest->GetRewKillHonor();

    packet.Info.StartItem = quest->GetSrcItemId();
    packet.Info.Flags = quest->GetFlags();
    packet.Info.FlagsEx = quest->GetFlagsEx();
    packet.Info.RewardTitle = quest->GetRewTitle();
    packet.Info.RewardTalents = quest->GetBonusTalents();
    packet.Info.RewardArenaPoints = quest->GetRewArenaPoints();
    packet.Info.RewardSkillLineID = quest->GetRewardSkillId();
    packet.Info.RewardNumSkillUps = quest->GetRewardSkillPoints();
    packet.Info.RewardFactionFlags = quest->GetRewardFactionFlags();
    packet.Info.PortraitGiver = quest->GetQuestGiverPortrait();
    packet.Info.PortraitTurnIn = quest->GetQuestTurnInPortrait();

    if (!quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS))
    {
        for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
        {
            packet.Info.RewardItems[i] = quest->RewardItemId[i];
            packet.Info.RewardAmount[i] = quest->RewardItemCount[i];
        }
        for (uint8 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        {
            packet.Info.UnfilteredChoiceItems[i].ItemID = quest->RewardChoiceItemId[i];
            packet.Info.UnfilteredChoiceItems[i].Quantity = quest->RewardChoiceItemCount[i];
        }
    }

    for (uint8 i = 0; i < QUEST_REWARD_REPUTATIONS_COUNT; ++i)
    {
        packet.Info.RewardFactionID[i] = quest->RewardFactionId[i];
        packet.Info.RewardFactionValue[i] = quest->RewardFactionValue[i];
        packet.Info.RewardFactionOverride[i] = quest->RewardFactionOverride[i];
    }

    packet.Info.POIContinent = quest->GetPOIContinent();
    packet.Info.POIx = quest->GetPOIx();
    packet.Info.POIy = quest->GetPOIy();
    packet.Info.POIPriority = quest->GetPOIPriority();

    /*if (sWorld->getBoolConfig(CONFIG_UI_QUESTLEVELS_IN_DIALOGS))
        AddQuestLevelToTitle(questTitle, quest->GetQuestLevel());*/

    packet.Info.LogTitle = questTitle;
    packet.Info.LogDescription = questDetails;
    packet.Info.QuestDescription = questObjectives;
    packet.Info.AreaDescription = questEndText;
    packet.Info.QuestCompletionLog = questQuestCompletionLog;
    packet.Info.AllowableRaces = quest->GetAllowableRaces();

    for (QuestObjective const& obj : quest->Objectives)
    {
        packet.Info.Objectives.push_back(obj);
        // @todo update quets objective locales
        //packet.Info.Objectives.back().Description = questObjectiveText[i];
    }

    for (uint32 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
    {
        packet.Info.RewardCurrencyID[i] = quest->RewardCurrencyId[i];
        packet.Info.RewardCurrencyQty[i] = quest->RewardCurrencyCount[i];
    }

    packet.Info.PortraitGiverText = portraitGiverText;
    packet.Info.PortraitGiverName = portraitGiverName;
    packet.Info.PortraitTurnInText = portraitTurnInText;
    packet.Info.PortraitTurnInName = portraitTurnInName;

    packet.Info.AcceptedSoundKitID = quest->GetSoundAccept();
    packet.Info.CompleteSoundKitID = quest->GetSoundTurnIn();
    packet.Info.AreaGroupID = quest->GetAreaGroupID();
    packet.Info.TimeAllowed = quest->GetLimitTime();

    _session->SendPacket(packet.Write());

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUEST_QUERY_RESPONSE questid=%u", quest->GetQuestId());
}

void PlayerMenu::SendQuestGiverOfferReward(Quest const* quest, ObjectGuid npcGUID, bool enableNext) const
{
    Player* player = _session->GetPlayer();
    std::string questTitle = quest->GetLogTitle();
    std::string questOfferRewardText = quest->GetOfferRewardText();
    std::string portraitGiverText = quest->GetPortraitGiverText();
    std::string portraitGiverName = quest->GetPortraitGiverName();
    std::string portraitTurnInText = quest->GetPortraitTurnInText();
    std::string portraitTurnInName = quest->GetPortraitTurnInName();

    int locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->LogTitle, locale, questTitle);
            ObjectMgr::GetLocaleString(localeData->OfferRewardText, locale, questOfferRewardText);
            ObjectMgr::GetLocaleString(localeData->PortraitGiverText, locale, portraitGiverText);
            ObjectMgr::GetLocaleString(localeData->PortraitGiverName, locale, portraitGiverName);
            ObjectMgr::GetLocaleString(localeData->PortraitTurnInText, locale, portraitTurnInText);
            ObjectMgr::GetLocaleString(localeData->PortraitTurnInName, locale, portraitTurnInName);
        }
    }

    WorldPackets::Quest::QuestGiverOfferRewardMessage packet;
    WorldPackets::Quest::QuestGiverOfferReward& offer = packet.QuestData;

    quest->BuildQuestRewards(offer.Rewards, _session->GetPlayer());
    offer.QuestGiverGUID = npcGUID;

    // Is there a better way? what about game objects?
    if (Creature const* creature = sObjectAccessor->GetCreature(*_session->GetPlayer(), npcGUID))
        offer.QuestGiverCreatureID = creature->GetCreatureTemplate()->Entry;

    offer.QuestID = quest->GetQuestId();
    offer.AutoLaunched = enableNext;
    offer.SuggestedPartyMembers = quest->GetSuggestedPlayers();

    for (uint32 i = 0; i < QUEST_EMOTE_COUNT && quest->OfferRewardEmote[i]; ++i)
        offer.Emotes.push_back(WorldPackets::Quest::QuestDescEmote(quest->OfferRewardEmote[i], quest->OfferRewardEmoteDelay[i]));

    offer.QuestFlags[0] = quest->GetFlags();
    offer.QuestFlags[1] = quest->GetFlagsEx();

    packet.QuestTitle = questTitle;
    packet.RewardText = questOfferRewardText;
    packet.PortraitTurnIn = quest->GetQuestTurnInPortrait();
    packet.PortraitGiver = quest->GetQuestGiverPortrait();
    packet.PortraitGiverText = portraitGiverText;
    packet.PortraitGiverName = portraitGiverName;
    packet.PortraitTurnInText = portraitTurnInText;
    packet.PortraitTurnInName = portraitTurnInName;
    packet.QuestPackageID = quest->GetQuestPackageID();

    _session->SendPacket(packet.Write());

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUESTGIVER_OFFER_REWARD NPCGuid=%u, questid=%u", npcGUID.GetCounter(), quest->GetQuestId());
}

void PlayerMenu::SendQuestGiverRequestItems(Quest const* quest, ObjectGuid npcGUID, bool canComplete, bool closeOnCancel) const
{
    // We can always call to RequestItems, but this packet only goes out if there are actually
    // items.  Otherwise, we'll skip straight to the OfferReward

    std::string questTitle = quest->GetLogTitle();
    std::string requestItemsText = quest->GetRequestItemsText();

    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->LogTitle, locale, questTitle);
            ObjectMgr::GetLocaleString(localeData->RequestItemsText, locale, requestItemsText);
        }
    }

    if (!quest->GetReqItemsCount() && canComplete)
    {
        SendQuestGiverOfferReward(quest, npcGUID, true);
        return;
    }

    WorldPackets::Quest::QuestGiverRequestItems packet;
    packet.QuestGiverGUID = npcGUID;

    // Is there a better way? what about game objects?
    if (Creature const* creature = sObjectAccessor->GetCreature(*_session->GetPlayer(), npcGUID))
        packet.QuestGiverCreatureID = creature->GetCreatureTemplate()->Entry;

    packet.QuestID = quest->GetQuestId();

    if (canComplete)
    {
        packet.CompEmoteDelay = quest->EmoteOnCompleteDelay;
        packet.CompEmoteType = quest->EmoteOnComplete;
    }
    else
    {
        packet.CompEmoteDelay = quest->EmoteOnIncompleteDelay;
        packet.CompEmoteType = quest->EmoteOnIncomplete;
    }

    packet.QuestFlags[0] = quest->GetFlags();
    packet.QuestFlags[1] = quest->GetFlagsEx();
    packet.SuggestPartyMembers = quest->GetSuggestedPlayers();
    packet.StatusFlags = 0xDF; // Unk, send common value

    packet.MoneyToGet = 0;
    for (QuestObjective const& obj : quest->GetObjectives())
    {
        switch (obj.Type)
        {
            case QUEST_OBJECTIVE_ITEM:
                packet.Collect.push_back(WorldPackets::Quest::QuestObjectiveCollect(obj.ObjectID, obj.Amount));
                break;
            case QUEST_OBJECTIVE_CURRENCY:
                packet.Currency.push_back(WorldPackets::Quest::QuestCurrency(obj.ObjectID, obj.Amount));
                break;
            case QUEST_OBJECTIVE_MONEY:
                packet.MoneyToGet += obj.Amount;
                break;
            default:
                break;
        }
    }

    packet.AutoLaunched = closeOnCancel;
    packet.QuestTitle = questTitle;
    packet.CompletionText = requestItemsText;

    _session->SendPacket(packet.Write());

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS NPCGuid=%u, questid=%u", npcGUID.GetEntry(), quest->GetQuestId());
}
