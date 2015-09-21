/*
    http://epicwow.com/
    Dungeon : Upper Blackrock Spire 90-100
*/

#ifndef UPPER_BLACKROCK_SPIRE_H_
#define UPPER_BLACKROCK_SPIRE_H_

enum eData
{
    DATA_GORASHAN   = 0,
    DATA_KYRAK      = 1,
    DATA_THARBEK    = 2,
    DATA_RAGEWING   = 3,
    DATA_ZAELA      = 4,
    MAX_ENCOUNTER,
    
    DATA_FIRST_DOOR,
};

enum eCreatures
{
    //Firt room creature
    NPC_RUNE_GLOW               = 76396,
    NPC_BLACK_IRON_BERSERKER    = 76176,
    NPC_BLACK_IRON_LEADBELCHER  = 76157,
    NPC_BLACK_IRON_WARCASTER    = 76151,
    NPC_BLACK_IRON_GRUNT        = 76179,
    NPC_RAGEMAW_WORG            = 76181,
    //Goroshan
    NPC_OREBENDER_GORASHAN      = 76413,
    NPC_POWER_RUNE              = 76417,
    NPC_LIGHTNING_FIELD         = 76464,
    //Kyrak
    NPC_KYRAK                   = 76021,
    NPC_DRAKONID_MONSTROSITY    = 82556,
};

enum eGameObjects
{
    GO_ENTRANCE_DOOR     = 175244,
    //Goroshan
    GO_ENTER_ROOM_DOOR   = 175705,
    GO_EXIT_ROOM_DOOR    = 175153,
    GO_CONDUIT           = 226704,
};

Position const runeSpawn[5] = 
{
    {155.349f, -353.102f, 70.92f},
    {155.297f, -286.07f,  71.03f},
    {215.2f,   -334.706f, 76.95f},
    {228.828f, -301.516f, 77.02f},
    {192.704f, -258.461f, 77.02f},
};

Position const runeSummons[18] = 
{
    {161.78f, -357.51f, 70.93f, 3.3f},
    {157.65f, -359.49f, 70.93f, 1.0f},
    {156.01f, -343.43f, 70.92f, 1.5f},
    {152.67f, -285.48f, 70.95f, 0.0f},
    {159.03f, -280.85f, 70.96f, 3.1f},
    {158.73f, -286.21f, 70.95f, 3.1f},
    {211.27f, -336.71f, 76.88f, 0.4f},
    {219.12f, -338.92f, 76.91f, 3.9f},
    {217.18f, -336.59f, 76.87f, 2.3f},
    {217.63f, -332.20f, 76.87f, 3.9f},
    {224.01f, -303.97f, 76.96f, 1.2f},
    {231.58f, -302.81f, 76.93f, 3.1f},
    {228.65f, -298.58f, 76.93f, 4.7f},
    {224.15f, -299.42f, 76.93f, 4.6f},
    {197.27f, -262.31f, 76.93f, 3.1f},
    {196.88f, -259.71f, 76.94f, 3.0f},
    {189.03f, -259.28f, 76.94f, 3.0f},
    {190.38f, -270.88f, 76.94f, 5.1f},
};

Position const goroshanAdds[9] = 
{
    {162.36f, -258.90f, 91.63f}, //Power Rune
    {162.48f, -276.88f, 91.63f},
    {126.18f, -276.84f, 91.69f},
    {126.18f, -258.89f, 91.63f},
    {144.36f, -240.79f, 91.63f},
    {126.18f, -240.79f, 91.63f},
    {162.36f, -240.79f, 91.63f},
    {162.24f, -276.85f, 95.0f}, //Lightning Field
    {126.11f, -240.43f, 95.0f},
};

#endif
