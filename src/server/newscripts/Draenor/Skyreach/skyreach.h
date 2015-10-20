/*
    http://epicwow.com/
    Dungeon : Skyreach 97 - 99
*/

#ifndef UPPER_SKYREACH_H_
#define UPPER_SKYREACH_H_

enum eData
{
    DATA_RANJIT     = 0,
    DATA_ARAKNATH   = 1,
    DATA_RUKHRAN    = 2,
    DATA_VIRYX      = 3,
    MAX_ENCOUNTER,
};

enum eCreatures
{
    //Araknath
    NPC_SKYREACH_ARCANOLOGIST   = 76376,
    NPC_INTERIOR_FOCUS          = 77543,
    NPC_SUN_CONSTRUCT_ENERGIZER = 76367,
    NPC_SKYREACH_SUN_PROTOTYPE  = 76142,
};

enum eGameObjects
{
    GO_RANJIT_ENTER_DOOR      = 234311,
    GO_RANJIT_EXIT_DOOR       = 234310,
    GO_ARAKNATH_EXIT_DOOR_1   = 234312,
    GO_ARAKNATH_EXIT_DOOR_2   = 234313,
    GO_ARAKNATH_ENTER_DOOR_1  = 234314,
    GO_ARAKNATH_ENTER_DOOR_2  = 234315,
};

#endif