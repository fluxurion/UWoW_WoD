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

#ifndef _ITEM_ENCHANTMENT_MGR_H
#define _ITEM_ENCHANTMENT_MGR_H

#include "Common.h"

enum enchantmentRandomType
{
	ENCHANTMENT_RANDOM_SUFFIX,
	ENCHANTMENT_RANDOM_PROPERTY,
};

void LoadRandomEnchantmentsTable();
uint32 GetItemEnchantMod(int32 entry, uint32 type, uint32 SpecID);
uint32 GenerateEnchSuffixFactor(ItemTemplate const* proto, uint32 level = 0);
uint32 GetRandomPropertyPoints(uint32 itemLevel, uint32 quality, uint32 inventoryType, uint32 subclass);
bool CheckSpecProp(uint32 ench, uint32 type, uint32 SpecID);
bool CheckStatsSpec(uint32 StatType, uint32 SpecID);
#endif

