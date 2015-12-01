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

#ifndef WORLD_STATES_H
#define WORLD_STATES_H

#include "Define.h"

enum class WorldStates : uint32
{
    WS_NONE                         = 0,

    WS_RATED_BG_ENABLED             = 5508,
    WS_ARENA_SEASON_ID              = 3901,
    WS_PVP_ARENA_ENABLED            = 3191,

    BG_WS_ENABLE_TIMER              = 4247,
    BG_WS_CURRENT_TIMER             = 4248,
    BG_WS_UNKNOWN                   = 4249, ///< Used after flag is captured (value: 1)
    BG_WS_FLAG_UNK_ALLIANCE         = 1545, ///< Value: -1 when alliance flag is dropped | 1 when alliance flag is on player | 0 On base | -2 ???
    BG_WS_FLAG_UNK_HORDE            = 1546, ///< Value: -1 when horde flag is dropped    | 1 when horde flag is on player    | 0 On base | -2 ???
    BG_WS_FLAG_UNKNOWN              = 1547, ///< -1 before capturing flag, 0 after both flags respawned
    BG_WS_FLAG_CAPTURES_ALLIANCE    = 1581,
    BG_WS_FLAG_CAPTURES_HORDE       = 1582,
    BG_WS_FLAG_CAPTURES_MAX         = 1601,
    BG_WS_FLAG_STATE_HORDE          = 2338, ///< 0 - hide, 1 - flag ok, 2 - flag picked up (flashing), 3 - flag picked up (not flashing)
    BG_WS_FLAG_STATE_ALLIANCE       = 2339, ///< 0 - hide, 1 - flag ok, 2 - flag picked up (flashing), 3 - flag picked up (not flashing)
    BG_WS_BATTLE_TIMER              = 5333,
    BG_WS_NEXT_BATTLE_TIMER         = 5332,
    
    OCCOPIED_BASES_HORDE                = 1778,
    OCCOPIED_BASES_ALLIANCE             = 1779,
    ALLIANCE_RESOUCES                   = 1776,
    HORDE_RESOUCES                      = 1777,
    MAX_TEAM_RESOUCES                   = 1780,
    BG_AB_OP_RESOURCES_WARNING          = 1955, GILNEAS_BG_OP_RESOURCES_WARNING          = 1955,

    NA_UI_HORDE_GUARDS_SHOW         = 2503,
    NA_UI_ALLIANCE_GUARDS_SHOW      = 2502,
    NA_UI_GUARDS_MAX                = 2493,
    NA_UI_GUARDS_LEFT               = 2491,

    NA_UI_TOWER_SLIDER_DISPLAY      = 2495,
    NA_UI_TOWER_SLIDER_POS          = 2494,
    NA_UI_TOWER_SLIDER_N            = 2497,

    NA_MAP_WYVERN_NORTH_NEU_H       = 2762,
    NA_MAP_WYVERN_NORTH_NEU_A       = 2662,
    NA_MAP_WYVERN_NORTH_H           = 2663,
    NA_MAP_WYVERN_NORTH_A           = 2664,

    NA_MAP_WYVERN_SOUTH_NEU_H       = 2760,
    NA_MAP_WYVERN_SOUTH_NEU_A       = 2670,
    NA_MAP_WYVERN_SOUTH_H           = 2668,
    NA_MAP_WYVERN_SOUTH_A           = 2669,

    NA_MAP_WYVERN_WEST_NEU_H        = 2761,
    NA_MAP_WYVERN_WEST_NEU_A        = 2667,
    NA_MAP_WYVERN_WEST_H            = 2665,
    NA_MAP_WYVERN_WEST_A            = 2666,

    NA_MAP_WYVERN_EAST_NEU_H        = 2763,
    NA_MAP_WYVERN_EAST_NEU_A        = 2659,
    NA_MAP_WYVERN_EAST_H            = 2660,
    NA_MAP_WYVERN_EAST_A            = 2661,

    NA_MAP_HALAA_NEUTRAL            = 2671,
    NA_MAP_HALAA_NEU_A              = 2676,
    NA_MAP_HALAA_NEU_H              = 2677,
    NA_MAP_HALAA_HORDE              = 2672,
    NA_MAP_HALAA_ALLIANCE           = 2673,


    HP_UI_TOWER_DISPLAY_A           = 0x9ba,
    HP_UI_TOWER_DISPLAY_H           = 0x9b9,

    HP_UI_TOWER_COUNT_H             = 0x9ae,
    HP_UI_TOWER_COUNT_A             = 0x9ac,

    HP_UI_TOWER_SLIDER_N            = 2475,
    HP_UI_TOWER_SLIDER_POS          = 2474,
    HP_UI_TOWER_SLIDER_DISPLAY      = 2473,

    WS_TB_BATTLE_TIMER_ENABLED                      = 5346,
    WS_TB_COUNTER_BUILDINGS                         = 5348,
    WS_TB_COUNTER_BUILDINGS_ENABLED                 = 5349,
    WS_TB_HORDE_DEFENCE                             = 5384,
    WS_TB_ALLIANCE_DEFENCE                          = 5385,
    WS_TB_NEXT_BATTLE_TIMER_ENABLED                 = 5387,

    WS_TB_SOUTH_CAPTURE_POINT                       = 5418,
    WS_TB_EAST_CAPTURE_POINT                        = 5428,
    WS_TB_WEST_CAPTURE_POINT                        = 5423,

    WS_TB_EAST_SPIRE                                = 5433,
    WS_TB_SOUTH_SPIRE                               = 5438,
    WS_TB_WEST_SPIRE                                = 5443,
    
    WS_TB_KEEP_HORDE_DEFENCE                        = 5469,
    WS_TB_KEEP_ALLIANCE_DEFENCE                     = 5470,
    
    WS_TB_ALLIANCE_ATTACK                           = 5546,
    WS_TB_HORDE_ATTACK                              = 5547,

    BATTLEFIELD_WG_WORLD_STATE_VEHICLE_H         = 3490,
    BATTLEFIELD_WG_WORLD_STATE_MAX_VEHICLE_H     = 3491,
    BATTLEFIELD_WG_WORLD_STATE_VEHICLE_A         = 3680,
    BATTLEFIELD_WG_WORLD_STATE_MAX_VEHICLE_A     = 3681,
    BATTLEFIELD_WG_WORLD_STATE_ACTIVE            = 3801,
    BATTLEFIELD_WG_WORLD_STATE_DEFENDER          = 3802,
    BATTLEFIELD_WG_WORLD_STATE_ATTACKER          = 3803,
    BATTLEFIELD_WG_WORLD_STATE_SHOW_WORLDSTATE   = 3710,

    BG_AB_OP_STABLE_ICON                = 1842,             //Stable map icon (NONE)
    BG_AB_OP_STABLE_STATE_ALIENCE       = 1767,             //Stable map state (ALIENCE)
    BG_AB_OP_STABLE_STATE_HORDE         = 1768,             //Stable map state (HORDE)
    BG_AB_OP_STABLE_STATE_CON_ALI       = 1769,             //Stable map state (CON ALIENCE)
    BG_AB_OP_STABLE_STATE_CON_HOR       = 1770,             //Stable map state (CON HORDE)

    BG_AB_OP_FARM_ICON                  = 1845,             //Farm map icon (NONE)
    BG_AB_OP_FARM_STATE_ALIENCE         = 1772,             //Farm state (ALIENCE)
    BG_AB_OP_FARM_STATE_HORDE           = 1773,             //Farm state (HORDE)
    BG_AB_OP_FARM_STATE_CON_ALI         = 1774,             //Farm state (CON ALIENCE)
    BG_AB_OP_FARM_STATE_CON_HOR         = 1775,             //Farm state (CON HORDE)

    BG_AB_OP_BLACKSMITH_ICON            = 1846,             //Blacksmith map icon (NONE)
    BG_AB_OP_BLACKSMITH_STATE_ALIENCE   = 1782,             //Blacksmith map state (ALIENCE)
    BG_AB_OP_BLACKSMITH_STATE_HORDE     = 1783,             //Blacksmith map state (HORDE)
    BG_AB_OP_BLACKSMITH_STATE_CON_ALI   = 1784,             //Blacksmith map state (CON ALIENCE)
    BG_AB_OP_BLACKSMITH_STATE_CON_HOR   = 1785,             //Blacksmith map state (CON HORDE)

    BG_AB_OP_LUMBERMILL_ICON            = 1844,             //Lumber Mill map icon (NONE)
    BG_AB_OP_LUMBERMILL_STATE_ALIENCE   = 1792,             //Lumber Mill map state (ALIENCE)
    BG_AB_OP_LUMBERMILL_STATE_HORDE     = 1793,             //Lumber Mill map state (HORDE)
    BG_AB_OP_LUMBERMILL_STATE_CON_ALI   = 1794,             //Lumber Mill map state (CON ALIENCE)
    BG_AB_OP_LUMBERMILL_STATE_CON_HOR   = 1795,             //Lumber Mill map state (CON HORDE)

    BG_AB_OP_GOLDMINE_ICON              = 1843,             //Gold Mine map icon (NONE)
    BG_AB_OP_GOLDMINE_STATE_ALIENCE     = 1787,             //Gold Mine map state (ALIENCE)
    BG_AB_OP_GOLDMINE_STATE_HORDE       = 1788,             //Gold Mine map state (HORDE)
    BG_AB_OP_GOLDMINE_STATE_CON_ALI     = 1789,             //Gold Mine map state (CON ALIENCE
    BG_AB_OP_GOLDMINE_STATE_CON_HOR     = 1790,             //Gold Mine map state (CON HORDE)

    AV_Alliance_Score               = 3127,
    AV_Horde_Score                  = 3128,
    AV_SHOW_H_SCORE                 = 3133,
    AV_SHOW_A_SCORE                 = 3134,
    AV_AID_A_C                      = 1325,
    AV_AID_A_A                      = 1326,
    AV_AID_H_C                      = 1327,
    AV_AID_H_A                      = 1328,
    AV_PIKEGRAVE_A_C                = 1333,
    AV_PIKEGRAVE_A_A                = 1335,
    AV_PIKEGRAVE_H_C                = 1334,
    AV_PIKEGRAVE_H_A                = 1336,
    AV_STONEHEART_A_C               = 1302,
    AV_STONEHEART_A_A               = 1304, //over hc
    AV_STONEHEART_H_C               = 1301, //over ac
    AV_STONEHEART_H_A               = 1303, //over aa
    AV_SNOWFALL_N                   = 1966, //over aa
    AV_SNOWFALL_A_C                 = 1341, //over hc
    AV_SNOWFALL_A_A                 = 1343, //over ha
    AV_SNOWFALL_H_C                 = 1342,
    AV_SNOWFALL_H_A                 = 1344, //over ac
    AV_ICEBLOOD_A_C                 = 1346, //over hc
    AV_ICEBLOOD_A_A                 = 1348, //over ac
    AV_ICEBLOOD_H_C                 = 1347,
    AV_ICEBLOOD_H_A                 = 1349, //over aa
    AV_FROSTWOLF_A_C                = 1337, //over hc
    AV_FROSTWOLF_A_A                = 1339, //over ac
    AV_FROSTWOLF_H_C                = 1338,
    AV_FROSTWOLF_H_A                = 1340, //over aa
    AV_FROSTWOLFHUT_A_C             = 1329, //over hc
    AV_FROSTWOLFHUT_A_A             = 1331, //over ha
    AV_FROSTWOLFHUT_H_C             = 1330,
    AV_FROSTWOLFHUT_H_A             = 1332, //over ac
    AV_DUNS_CONTROLLED              = 1361,
    AV_DUNS_DESTROYED               = 1370,
    AV_DUNS_ASSAULTED               = 1378,
    AV_DUNN_CONTROLLED              = 1362,
    AV_DUNN_DESTROYED               = 1371,
    AV_DUNN_ASSAULTED               = 1379,
    AV_ICEWING_CONTROLLED           = 1363,
    AV_ICEWING_DESTROYED            = 1372,
    AV_ICEWING_ASSAULTED            = 1380,
    AV_STONEH_CONTROLLED            = 1364,
    AV_STONEH_DESTROYED             = 1373,
    AV_STONEH_ASSAULTED             = 1381,
    AV_ICEBLOOD_CONTROLLED          = 1385,
    AV_ICEBLOOD_DESTROYED           = 1368,
    AV_ICEBLOOD_ASSAULTED           = 1390,
    AV_TOWERPOINT_CONTROLLED        = 1384,
    AV_TOWERPOINT_DESTROYED         = 1367, //goes over controlled
    AV_TOWERPOINT_ASSAULTED         = 1389, //goes over destroyed
    AV_FROSTWOLFW_CONTROLLED        = 1382,
    AV_FROSTWOLFW_DESTROYED         = 1365, //over controlled
    AV_FROSTWOLFW_ASSAULTED         = 1387, //over destroyed
    AV_FROSTWOLFE_CONTROLLED        = 1383,
    AV_FROSTWOLFE_DESTROYED         = 1366,
    AV_FROSTWOLFE_ASSAULTED         = 1388,
    AV_N_MINE_N              = 1360,
    AV_N_MINE_A              = 1358,
    AV_N_MINE_H              = 1359,
    AV_S_MINE_N                     = 1357,
    AV_S_MINE_A                     = 1355,
    AV_S_MINE_H                     = 1356,
    AV_STONEH_UNUSED                = 1377,
    AV_ICEWING_UNUSED               = 1376,
    AV_DUNS_UNUSED                  = 1375,
    AV_DUNN_UNUSED                  = 1374,

    AV_ICEBLOOD_UNUSED              = 1395,
    AV_TOWERPOINT_UNUSED            = 1394,
    AV_FROSTWOLFE_UNUSED            = 1393,
    AV_FROSTWOLFW_UNUSED            = 1392,

    DRAENEI_RUINS_HORDE_CONTROL     = 2733,
    DRAENEI_RUINS_ALLIANCE_CONTROL  = 2732,
    DRAENEI_RUINS_UNCONTROL         = 2731,
    MAGE_TOWER_ALLIANCE_CONTROL     = 2730,
    MAGE_TOWER_HORDE_CONTROL        = 2729,
    MAGE_TOWER_UNCONTROL            = 2728,
    FEL_REAVER_HORDE_CONTROL        = 2727,
    FEL_REAVER_ALLIANCE_CONTROL     = 2726,
    FEL_REAVER_UNCONTROL            = 2725,
    BLOOD_ELF_HORDE_CONTROL         = 2724,
    BLOOD_ELF_ALLIANCE_CONTROL      = 2723,
    BLOOD_ELF_UNCONTROL             = 2722,
    PROGRESS_BAR_PERCENT_GREY       = 2720,                 //100 = empty (only grey), 0 = blue|red (no grey)
    PROGRESS_BAR_STATUS             = 2719,                 //50 init!, 48 ... hordak bere .. 33 .. 0 = full 100% hordacky, 100 = full alliance
    PROGRESS_BAR_SHOW               = 2718,                 //1 init, 0 druhy send - bez messagu, 1 = controlled aliance
    NETHERSTORM_FLAG                = 2757,
    //set to 2 when flag is picked up, and to 1 if it is dropped
    NETHERSTORM_FLAG_STATE_ALLIANCE = 2769,
    NETHERSTORM_FLAG_STATE_HORDE    = 2770,
    NETHERSTROM_FLAG_UI_ALLIANCE    = 9808,
    NETHERSTROM_FLAG_UI_HORDE       = 9809,


    
    BG_IC_ALLIANCE_RENFORT_SET      = 4221,
    BG_IC_HORDE_RENFORT_SET         = 4222,
    BG_IC_ALLIANCE_RENFORT          = 4226,
    BG_IC_HORDE_RENFORT             = 4227,
    BG_IC_GATE_FRONT_H_WS_CLOSED    = 4317,
    BG_IC_GATE_WEST_H_WS_CLOSED     = 4318,
    BG_IC_GATE_EAST_H_WS_CLOSED     = 4319,
    BG_IC_GATE_FRONT_A_WS_CLOSED    = 4328,
    BG_IC_GATE_WEST_A_WS_CLOSED     = 4327,
    BG_IC_GATE_EAST_A_WS_CLOSED     = 4326,
    BG_IC_GATE_FRONT_H_WS_OPEN      = 4322,
    BG_IC_GATE_WEST_H_WS_OPEN       = 4321,
    BG_IC_GATE_EAST_H_WS_OPEN       = 4320,
    BG_IC_GATE_FRONT_A_WS_OPEN      = 4323,
    BG_IC_GATE_WEST_A_WS_OPEN       = 4324,
    BG_IC_GATE_EAST_A_WS_OPEN       = 4325,

    BG_IC_DOCKS_UNCONTROLLED = 4301,
    BG_IC_DOCKS_CONFLICT_A = 4305,
    BG_IC_DOCKS_CONFLICT_H = 4302,
    BG_IC_DOCKS_CONTROLLED_A = 4304,
    BG_IC_DOCKS_CONTROLLED_H = 4303,

    BG_IC_HANGAR_UNCONTROLLED = 4296,
    BG_IC_HANGAR_CONFLICT_A = 4300,
    BG_IC_HANGAR_CONFLICT_H = 4297,
    BG_IC_HANGAR_CONTROLLED_A = 4299,
    BG_IC_HANGAR_CONTROLLED_H = 4298,

    BG_IC_QUARRY_UNCONTROLLED = 4306,
    BG_IC_QUARRY_CONFLICT_A = 4310,
    BG_IC_QUARRY_CONFLICT_H = 4307,
    BG_IC_QUARRY_CONTROLLED_A = 4309,
    BG_IC_QUARRY_CONTROLLED_H = 4308,

    BG_IC_REFINERY_UNCONTROLLED = 4311,
    BG_IC_REFINERY_CONFLICT_A = 4315,
    BG_IC_REFINERY_CONFLICT_H = 4312,
    BG_IC_REFINERY_CONTROLLED_A = 4314,
    BG_IC_REFINERY_CONTROLLED_H = 4313,

    BG_IC_WORKSHOP_UNCONTROLLED = 4294,
    BG_IC_WORKSHOP_CONFLICT_A = 4228,
    BG_IC_WORKSHOP_CONFLICT_H = 4293,
    BG_IC_WORKSHOP_CONTROLLED_A = 4229,
    BG_IC_WORKSHOP_CONTROLLED_H = 4230,

    BG_IC_ALLIANCE_KEEP_UNCONTROLLED = 4341,
    BG_IC_ALLIANCE_KEEP_CONFLICT_A = 4342,
    BG_IC_ALLIANCE_KEEP_CONFLICT_H = 4343,
    BG_IC_ALLIANCE_KEEP_CONTROLLED_A = 4339,
    BG_IC_ALLIANCE_KEEP_CONTROLLED_H = 4340,

    BG_IC_HORDE_KEEP_UNCONTROLLED = 4346,
    BG_IC_HORDE_KEEP_CONFLICT_A = 4347,
    BG_IC_HORDE_KEEP_CONFLICT_H = 4348,
    BG_IC_HORDE_KEEP_CONTROLLED_A = 4344,
    BG_IC_HORDE_KEEP_CONTROLLED_H = 4345,

    BG_KT_ICON_BLUE_ORB_ICON    = 6714,
    BG_KT_ICON_PURPLE_ORB_ICON  = 6715,
    BG_KT_ICON_GREEN_ORB_ICON   = 6716,
    BG_KT_ICON_ORANGE_ORB_ICON  = 6717,

    BG_KT_ICON_A                = 6308,
    BG_KT_ICON_H                = 6307,
    BG_KT_ORB_POINTS_A          = 6303,
    BG_KT_ORB_POINTS_H          = 6304,
    BG_KT_ORB_STATE             = 6309,

    BG_KT_PURPLE_ORB_C          = 6967,
    BG_KT_PURPLE_ORB_X          = 6968,
    
    BG_KT_GREEN_ORB_C           = 6965,
    BG_KT_GREEN_ORB_X           = 6966,
    
    BG_KT_ORANGE_ORB_C          = 6963,
    BG_KT_ORANGE_ORB_X          = 6964,
    
    BG_KT_BLUE_ORB_C            = 6969,
    BG_KT_BLUE_ORB_X            = 6970,

    BG_RV_WORLD_STATE                            = 3610,

    
    BG_SA_ALLY_ATTACKS              = 4352,
    BG_SA_HORDE_ATTACKS             = 4353,
    BG_SA_PURPLE_GATEWS             = 3614,
    BG_SA_RED_GATEWS                = 3617,
    BG_SA_BLUE_GATEWS               = 3620,
    BG_SA_GREEN_GATEWS              = 3623,
    BG_SA_YELLOW_GATEWS             = 3638,
    BG_SA_ANCIENT_GATEWS            = 3849,
    BG_SA_LEFT_GY_ALLIANCE          = 3635,
    BG_SA_RIGHT_GY_ALLIANCE         = 3636,
    BG_SA_CENTER_GY_ALLIANCE        = 3637,
    BG_SA_RIGHT_ATT_TOKEN_ALL       = 3627,
    BG_SA_LEFT_ATT_TOKEN_ALL        = 3626,
    BG_SA_LEFT_ATT_TOKEN_HRD        = 3629,
    BG_SA_RIGHT_ATT_TOKEN_HRD       = 3628,
    BG_SA_HORDE_DEFENCE_TOKEN       = 3631,
    BG_SA_ALLIANCE_DEFENCE_TOKEN    = 3630,
    BG_SA_RIGHT_GY_HORDE            = 3632,
    BG_SA_LEFT_GY_HORDE             = 3633,
    BG_SA_CENTER_GY_HORDE           = 3634,
    BG_SA_BONUS_TIMER               = 3571,
    BG_SA_ENABLE_TIMER              = 3564, //3565 second round timer?
    BG_SA_TIMER                     = 3557, //cur time? 1386402226 | 1386402896

    
    SSM_PROGRESS_BAR_PERCENT_GREY       = 6877,                 //100 = empty (only grey), 0 = blue|red (no grey)
    SSM_PROGRESS_BAR_STATUS             = 6876,                 //50 init!, 48 ... hordak bere .. 33 .. 0 = full 100% hordacky, 100 = full alliance
    SSM_PROGRESS_BAR_SHOW               = 6875,                 //1 init, 0 druhy send - bez messagu, 1 = controlled aliance
    SSM_EAST_TRACK_SWITCH               = 6467,
    SSM_NORTH_TRACK_SWITCH              = 6468,

    SSM_INIT_POINTS_ALLIANCE            = 6441,
    SSM_INIT_POINTS_HORDE               = 6443,
    SSM_POINTS_ALLIANCE                 = 6437,
    SSM_POINTS_HORDE                    = 6438,

    WS_ARENA_BE_UNK2547                 = 2547,

    WORLDSTATE_SHOW_CRATES          = 3479,
    WORLDSTATE_CRATES_REVEALED      = 3480,
    WORLDSTATE_WAVE_COUNT           = 3504,
    WORLDSTATE_TIME_GUARDIAN        = 3931,
    WORLDSTATE_TIME_GUARDIAN_SHOW   = 3932,

    WORLDSTATE_SHOW_FRAGMENTS       = 6046,
    WORLDSTATE_FRAGMENTS_COLLECTED  = 6025,

    WORLDSTATE_CORPOREALITY_MATERIAL = 5049,
    WORLDSTATE_CORPOREALITY_TWILIGHT = 5050,
    WORLDSTATE_CORPOREALITY_TOGGLE   = 5051,

    
    WORLD_STATE_HOR_WAVES_ENABLED                 = 4884,
    WORLD_STATE_HOR_WAVE_COUNT                    = 4882,
    
    WORLDSTATE_SHOW_TIMER           = 4903,
    WORLDSTATE_EXECUTION_TIME       = 4904,
    WORLDSTATE_SHOW_ATTEMPTS        = 4940,
    WORLDSTATE_ATTEMPTS_REMAINING   = 4941,
    WORLDSTATE_ATTEMPTS_MAX         = 4942,

    
    WORLD_STATE_CENTRIFUGE_CONSTRUCT_SHOW   = 3524,
    WORLD_STATE_CENTRIFUGE_CONSTRUCT_AMOUNT = 3486,

    
    SI_GATHERED_A           = 2313,
    SI_GATHERED_H           = 2314,
    SI_SILITHYST_MAX        = 2317,

        ZM_UI_TOWER_SLIDER_N_W          = 2529,
    ZM_UI_TOWER_SLIDER_POS_W        = 2528,
    ZM_UI_TOWER_SLIDER_DISPLAY_W    = 2527,

    ZM_UI_TOWER_SLIDER_N_E          = 2535,
    ZM_UI_TOWER_SLIDER_POS_E        = 2534,
    ZM_UI_TOWER_SLIDER_DISPLAY_E    = 2533,

    ZM_WORLDSTATE_UNK_1             = 2653,

    ZM_UI_TOWER_EAST_N              = 2560,
    ZM_UI_TOWER_EAST_H              = 2559,
    ZM_UI_TOWER_EAST_A              = 2558,
    ZM_UI_TOWER_WEST_N              = 2557,
    ZM_UI_TOWER_WEST_H              = 2556,
    ZM_UI_TOWER_WEST_A              = 2555,

    ZM_MAP_TOWER_EAST_N             = 2652,
    ZM_MAP_TOWER_EAST_H             = 2651,
    ZM_MAP_TOWER_EAST_A             = 2650,
    ZM_MAP_GRAVEYARD_H              = 2649,
    ZM_MAP_GRAVEYARD_A              = 2648,
    ZM_MAP_GRAVEYARD_N              = 2647,
    ZM_MAP_TOWER_WEST_N             = 2646,
    ZM_MAP_TOWER_WEST_H             = 2645,
    ZM_MAP_TOWER_WEST_A             = 2644,

    ZM_MAP_HORDE_FLAG_READY         = 2658,
    ZM_MAP_HORDE_FLAG_NOT_READY     = 2657,
    ZM_MAP_ALLIANCE_FLAG_NOT_READY  = 2656,
    ZM_MAP_ALLIANCE_FLAG_READY      = 2655,

    WORLD_STATE_BM          = 2541,
    WORLD_STATE_BM_SHIELD   = 2540,
    WORLD_STATE_BM_RIFT     = 2784,

    WS_SHOW_SOUTH_TOWER             = 8545,
    WS_SHOW_NORTH_TOWER             = 8547,
    WS_SHOW_CAPTURE_SOUTH_TOWER     = 8546,
    WS_SHOW_CAPTURE_NORTH_TOWER     = 8548,
    WS_SOUTH_TOWER                  = 8461,
    WS_NORTH_TOWER                  = 8462,
    WS_CAPTURE_SOUTH_TOWER          = 8468,
    WS_CAPTURE_NORTH_TOWER          = 8469,
    WS_SHOW_KORKRON_CANNON          = 8414,
    WS_KORKRON_CANNON_COUNT         = 8373,

    WORLD_STATE_VH              = 3816,
    WORLD_STATE_VH_PRISON_STATE = 3815,
    WORLD_STATE_VH_WAVE_COUNT   = 3810,
    
    TF_UI_TOWER_SLIDER_POS                  = 0xa41,
    TF_UI_TOWER_SLIDER_N                    = 0xa40,
    TF_UI_TOWER_SLIDER_DISPLAY              = 0xa3f,

    TF_UI_TOWER_COUNT_H                     = 0xa3e,
    TF_UI_TOWER_COUNT_A                     = 0xa3d,
    TF_UI_TOWERS_CONTROLLED_DISPLAY         = 0xa3c,

    TF_UI_LOCKED_TIME_MINUTES_FIRST_DIGIT   = 0x9d0,
    TF_UI_LOCKED_TIME_MINUTES_SECOND_DIGIT  = 0x9ce,
    TF_UI_LOCKED_TIME_HOURS                 = 0x9cd,
    TF_UI_LOCKED_DISPLAY_NEUTRAL            = 0x9cc,
    TF_UI_LOCKED_DISPLAY_HORDE              = 0xad0,
    TF_UI_LOCKED_DISPLAY_ALLIANCE           = 0xacf,

    WS_ENABLE               = 6489,
    WS_HEALER_COUNT         = 6488,

    WORLD_STATE_ALGALON_DESPAWN_TIMER   = 4131,
    WORLD_STATE_ALGALON_TIMER_ENABLED   = 4132,

    WORLD_STATE_WAVES         = 2842,
    WORLD_STATE_ENEMY         = 2453,
    WORLD_STATE_ENEMYCOUNT    = 2454,
    
    ARENA_ALIVE_PLAYERS_GREEN           = 3600,
    ARENA_ALIVE_PLAYERS_GOLD            = 3601,

    ARENA_END_TIMER                     = 8529,
    ARENA_SHOW_END_TIMER                = 8524,

    DG_ALLIANCE_POINTS                  = 7880,
    DG_HORDE_POINTS                     = 7881,
    DG_ALLIANCE_NODES                   = 8230,
    DG_HORDE_NODES                      = 8231,
    DG_SHOW_MIDDLE_MINE_ICON            = 7939,
    DG_SHOW_GOBLIN_MINE_ICON            = 7938,
    DG_SHOW_PANDAREN_MINE_ICON          = 7935,
    DG_MIDDLE_MINE_ALLIANCE_ASSAULT     = 7934,
    DG_MIDDLE_MINE_HORDE_CAPTURED       = 7932,

    DG_GOBLIN_MINE_ALLIANCE_ASSAULT     = 7864,
    DG_GOBLIN_MINE_CAPTURED_BY_TEAM     = 7856,

    DG_ALLIANCE_CART_ASSAULT            = 7904, // 1 default to show alliance points; 2 cart assault; 3 cart dropeed
    DG_HORDE_CART_ASSAULT               = 7887, // 1 default to show alliance points; 2 cart assault; 3 cart dropeed

    //< Ashran
    
    ASHRAN_CURRENT_KILLS_ALLIANCE       = 8933,
    ASHRAN_CURRENT_KILLS_HORDE          = 8934,
    ASHRAN_TIME_TO_NEXT_BATTLE          = 8945,
    ASHRAN_SHOW_TIME_TO_NEXT_BATTLE     = 8946,
    
    ASHRAN_HORDE_BASE_ICON              = 9100,
    ASHRAN_HORDE_TOWER_ICON             = 9101,
    ASHRAN_HORDE_OUTPOST_ICON           = 9102,
    ASHRAN_CROSSROAD_ICON               = 9103,
    ASHRAN_ALLIANCE_OUTPOST_ICON        = 9104,
    ASHRAN_ALLIANCE_TOWER_ICON          = 9105,
    ASHRAN_ALLIANCE_BASE_ICON           = 9106,

    ASHRAN_DISPLAY_CROSSROADS_GRAVEYARD = 9214, // 0 - hide 1 - display

    ASHRAN_TIME_TO_KILL_VOLRAT          = 9316,
    ASHRAN_SHOW_TIME_TO_KILL_VOLRAT     = 9317,
    ASHRAN_SHOW_TIME_TO_KILL_TREMBLADE  = 9326,

    ASHRAN_HIGH_WARLORD_VOLRAT          = 9713,
    ASHRAN_CARBON_TOWER                 = 9714,
    ASHRAN_ADVANCED_GATE                = 9715,
    ASHRAN_CROSSROAD_UI                 = 9716,
    ASHRAN_TREMBLADE_AVANGARD_STATE     = 9717,
    ASHRAN_BASTION_SUPREME_MAGE         = 9718,
    ASHRAN_COMMANDER_TREMBLADE          = 9719,

    ASHRAN_CAPTURE_FLAG_UI              = 9785,

    ASHRAN_KILLS_CAP_ALLIANCE           = 9801,
    ASHRAN_KILLS_CAP_HORDE              = 9802,

    ASHRAN_MINE_EVENT                   = 10287,
};

#endif // WORLD_STATES_H
