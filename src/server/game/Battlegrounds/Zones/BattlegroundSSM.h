/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#ifndef __BATTLEGROUNDSSM_H
#define __BATTLEGROUNDSSM_H

#include "Battleground.h"

class BattleGroundSSMScore : public BattlegroundScore
{
public:
    BattleGroundSSMScore() : CartsTaken(0) { }

    virtual ~BattleGroundSSMScore() { }

    uint8 CartsTaken;
};

enum BG_SSM_ObjectTypes
{
    BG_SSM_CART_EAST,
    BG_SSM_CART_SOUTH,
    BG_SSM_CART_NORTH,

    BG_SSM_SPIRIT_MAIN_ALLIANCE,
    BG_SSM_SPIRIT_MAIN_HORDE,

    BG_DOOR_1,
    BG_DOOR_2,
    BG_DOOR_3,
    BG_DOOR_4,

    BG_SSM_OBJECT_SPEEDBUFF,
    BG_SSM_OBJECT_REGENBUFF,
    BG_SSM_OBJECT_BERSERKBUFF,

    BG_SSM_OBJECT_MAX
};

enum BG_SSM_SPELLS
{
    BG_SSM_SPELL_CONTROL_NEUTRAL                        = 118001,
    BG_SSM_SPELL_CONTROL_ALLIANCE                       = 116086,
    BG_SSM_SPELL_CONTROL_HORDE                          = 116085,

    BG_SSM_SPELL_CART_CONTROL_CAPTURE_POINT_UNIT_SOUTH  = 125696,
    BG_SSM_SPELL_CART_CONTROL_CAPTURE_POINT_UNIT_NORTH  = 125695,
    BG_SSM_SPELL_CART_CONTROL_CAPTURE_POINT_UNIT_EAST   = 125620,

    BG_SSM_SPELL_DEFENDING_CART_AURA                    = 128646,
    BG_SSM_SPELL_CART_CAP                               = 115904,
};

enum BG_SSM_Greveyards
{
    BG_SSM_HORDE_GRAVEYARD      = 4061,
    BG_SSM_ALLIANCE_GRAVEYARD   = 4062
};

Milliseconds const SSM_SCORE_UPDATE_TIMER = Seconds(5);
Milliseconds const SSM_CARTS_UPDATE_TIMER = Seconds(2);

enum BG_SSM_UnitEntry
{
    BG_SSM_CART = 60140,
};

static const uint16 SSM_MAX_TEAM_POINTS = 1500;

enum BG_SSM_ObjectEntry
{
    BG_SSM_CHEST_1 = 212080,
    BG_SSM_CHEST_2 = 212081,

    BG_SSM_DOOR    = 212941
};

enum BG_SSM_ControlState
{
    SSM_CONTROL_NEUTRAL,
    SSM_CONTROL_ALLIANCE,
    SSM_CONTROL_HORDE
};

static const uint32 BG_SSM_MAX_CARTS = 3;

enum BG_SSM_ProgressBarConsts
{
    BG_SSM_POINT_MAX_CAPTURERS_COUNT     = 5,
    BG_SSM_POINT_RADIUS                  = 22,
    BG_SSM_PROGRESS_BAR_DONT_SHOW        = 0,
    BG_SSM_PROGRESS_BAR_SHOW             = 1,
    BG_SSM_PROGRESS_BAR_PERCENT_GREY     = 0,
    BG_SSM_PROGRESS_BAR_STATE_MIDDLE     = 50,
    BG_SSM_PROGRESS_BAR_HORDE_CONTROLLED = 0,
    BG_SSM_PROGRESS_BAR_NEUTRAL_LOW      = 30,
    BG_SSM_PROGRESS_BAR_NEUTRAL_HIGH     = 70,
    BG_SSM_PROGRESS_BAR_ALI_CONTROLLED   = 100
};

Position const WayEastBase[] =
{
    {744.5086f, 183.1756f, 319.5443f, 4.338116f},
    {744.9193f, 184.1136f, 319.5439f},
    {744.5174f, 183.1979f, 319.5439f},
    {742.8542f, 178.9635f, 319.6017f},
    {739.8906f, 171.5191f, 319.3651f},
    {735.5955f, 163.5434f, 319.2191f},
    {730.4774f, 155.1944f, 319.1304f},
    {728.8577f, 152.4063f, 319.6251f},
    {723.9236f, 143.1042f, 319.6667f},
    {719.3646f, 131.0938f, 319.5531f},
    {717.6007f, 122.7222f, 320.0978f},
    {716.9514f, 113.5174f, 320.8443f},
    {716.9514f, 113.5174f, 320.8443f},
};

Position const ExtraWayEast1[] =
{
    {715.8229f, 109.0373f, 321.0153f},
    {713.3229f, 104.2873f, 320.2653f},
    {706.8229f, 99.28732f, 319.0153f},
    {701.8229f, 97.28732f, 317.7653f},
    {695.0729f, 96.53732f, 316.2653f},
    {686.5729f, 95.53732f, 314.5153f},
    {680.3229f, 94.53732f, 312.7653f},
    {670.3229f, 90.03732f, 309.5153f},
    {663.8229f, 86.53732f, 307.0153f},
    {660.3229f, 84.53732f, 305.7653f},
    {647.3229f, 82.03732f, 302.0153f},
    {638.5729f, 81.53732f, 299.5153f},
};

Position const ExtraWayEast2[] =
{
    {0.0f, 0.0f, 0.0f},
};

Position const WaySouth[] =
{
    {738.8957f, 204.154f,  319.6037f, 2.248367f},
    {738.9358f, 204.1042f, 319.6031f},
    {736.0469f, 207.6944f, 319.6429f},
    {732.5243f, 213.625f,  320.1088f},
    {730.5608f, 219.1163f, 320.544f},
    {728.0052f, 231.5764f, 321.037f},
    {725.6285f, 236.9444f, 321.0317f},
    {722.8368f, 241.6771f, 320.7483f},
    {720.7118f, 244.5208f, 321.2489f},
    {715.6458f, 251.4236f, 321.1337f},
    {710.8594f, 261.0868f, 320.8995f},
    {707.2899f, 270.0538f, 320.633f},
    {703.6528f, 278.1771f, 320.5885f},
    {696.5243f, 290.1788f, 320.6527f},
    {686.2604f, 301.6042f, 321.0385f},
    {667.9496f, 317.1371f, 323.1469f},
    {660.3976f, 323.9063f, 324.7097f},
    {651.4028f, 331.2066f, 327.3729f},
    {638.6805f, 342.3385f, 332.2758f},
    {633.8316f, 345.2587f, 334.5666f},
    {626.217f,  345.4913f, 337.4414f},
    {616.7413f, 343.224f,  340.5497f},
    {601.7222f, 340.3073f, 344.5738f},
    {592.059f,  339.2743f, 346.0716f},
    {585.4739f, 337.6736f, 346.3017f},
    {585.4739f, 337.6736f, 346.3017f},
};

Position const WayNorthBase[] =
{
    {759.3847f, 198.3586f, 319.5317f, 0.4215083f},
    {758.3997f, 197.9515f, 319.5306f},
    {759.3246f, 198.3316f, 319.5306f},
    {762.0156f, 199.5382f, 319.5831f},
    {765.5938f, 202.5295f, 319.9603f},
    {770.7864f, 210.3819f, 321.3461f},
    {776.9566f, 222.9167f, 323.3075f},
    {780.1962f, 232.6215f, 324.9792f},
    {783.5833f, 241.6458f, 327.9297f},
    {787.6268f, 249.401f,  330.7884f},
    {794.5851f, 259.5799f, 336.815f},
    {803.3455f, 269.5903f, 341.3372f},
    {816.4271f, 282.5642f, 344.849f},
    {823.8924f, 289.4271f, 345.9665f},
    {830.2726f, 294.8576f, 346.5565f},
    {835.592f,  300.4184f, 346.956f},
    {835.592f,  300.4184f, 346.956f},
};

Position const WayNorthLeft[] =
{
    {837.7413f, 303.2179f, 347.3937f},
    {840.7413f, 307.4679f, 347.3937f},
    {844.7413f, 314.2179f, 347.3937f},
    {847.9913f, 321.7179f, 347.6437f},
    {848.7413f, 326.9679f, 347.3937f},
    {846.9913f, 342.7179f, 347.3937f},
    {843.4913f, 350.9679f, 347.6437f},
    {831.9913f, 372.4679f, 347.3937f},
    {822.4913f, 397.4679f, 347.1437f},
    {818.7413f, 406.9679f, 348.1437f},
    {817.9913f, 410.4679f, 348.8937f},
    {816.7413f, 419.2179f, 350.6437f},
    {817.2413f, 422.7179f, 351.3937f},
    {815.9913f, 429.2179f, 353.3937f},
    {814.2413f, 434.7179f, 355.1437f},
    {810.9913f, 442.2179f, 357.1437f},
    {807.4913f, 457.7179f, 358.8937f},
    {806.7413f, 466.4679f, 359.1437f},
    {803.9913f, 472.9679f, 359.3937f},
    {800.9913f, 476.9679f, 359.6437f},
    {793.7413f, 485.2179f, 359.6437f} 
};
Position const WayNorthRight[] =
{
    {0.0f, 0.0f, 0.0f},
};

static uint32 WaysSize[] = {13, 26, 17};
static uint32 ExtraWaysSize[] = {21, 1, 12, 1}; //< WayNorthLeft WayNorthRight | ExtraWayEast1 ExtraWayEast2

class BattlegroundSSM : public Battleground
{
    public:
        BattlegroundSSM();
        ~BattlegroundSSM();

        bool SetupBattleground() override;
        void PostUpdateImpl(Milliseconds diff) override;

        void HandleAreaTrigger(Player* player, uint32 trigger, bool entered) override;

        void AddPlayer(Player* player) override;
        void RemovePlayer(Player* player, ObjectGuid guid, uint32 team) override;
        void HandleKillPlayer(Player* player, Player* killer) override;

        void StartingEventOpenDoors() override;
        void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet) override;
        WorldSafeLocsEntry const* GetClosestGraveYard(Player* player) override;

    private:
        Creature* _AddCart(uint32 type, Position const loc);
        Creature* _UpdateCart(uint32 type);

        void _CheckPlayersAtCars();

        void _UpdatePoints();
        void _UpdateScore();

        void _SetCartControl(uint32 type, TeamId teamID);

        void _AddScore(TeamId team, int32 points);

        Creature* _cart[BG_SSM_MAX_CARTS];
        uint32 _waysStep[BG_SSM_MAX_CARTS];
        bool _isWaysStep[BG_SSM_MAX_CARTS];

        std::map<uint8, Position const* /*pos*/> _cartWaypointsMap;

        uint32 _cartsState[BG_SSM_MAX_CARTS];
        int32 _cartsCapturePoints[BG_SSM_MAX_CARTS];

        Milliseconds _timerPointsUpdate;
        Milliseconds _timerCartsUpdate;

        bool _northController;
        bool _eastController;

        GuidVector _playersNearPoint[BG_SSM_MAX_CARTS + 1];
};

#endif
