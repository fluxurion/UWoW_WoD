/*
    http://epicwow.com/
    Dungeon : Shadowmoon Burial Grounds 100
*/

#ifndef SHADOWMOON_BURIAL_GROUNDS_H_
#define SHADOWMOON_BURIAL_GROUNDS_H_

enum eData
{
    DATA_SADANA     = 0,
    DATA_NHALLISH   = 1,
    DATA_BONEMAW    = 2,
    DATA_NERZHUL    = 3,
    MAX_ENCOUNTER,
};

enum eCreatures
{
    NPC_WORD_TRIGGER        = 83816,
    //Sadana
    NPC_SADANA_BLOODFURY    = 75509,
    NPC_SHADOW_RUNE         = 75778,
    NPC_DARK_ECLIPSE        = 76052,
    NPC_DAGGERFALL          = 75981,
    NPC_DEFILED_SPIRIT      = 75966,
    //Nhallish
    NPC_POSSESSED_SOUL      = 75899,
};

enum eGameObjects
{
    GO_SADANA_DOOR_ENTER    = 233921,
    GO_SADANA_DOOR_EXIT     = 233919,
    GO_NHALLISH_DOOR        = 227851,
};

#endif