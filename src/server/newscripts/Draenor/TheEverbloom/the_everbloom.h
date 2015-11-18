/*
    http://epicwow.com/
    Dungeon : The Everbloom 100
*/

#ifndef THE_EVERBLOOM_H_
#define THE_EVERBLOOM_H_

enum eData
{
    DATA_WITHERBARK     = 0,
    DATA_PROTECTORS     = 1,
    DATA_XERITAC        = 2,
    DATA_ARCHMAGE_SOL   = 3,
    DATA_YALNU          = 4,
    MAX_ENCOUNTER,
};

enum eCreatures
{
    //WitherBark
    NPC_EVERBLOOM_NATURALIST    = 81819,
    NPC_EVERBLOOM_MENDER        = 81820,
    NPC_AQUEOUS_GLOBULE_TRIGGER = 81821,
    NPC_AQUEOUS_GLOBULE         = 81638,
    NPC_UNCHECKED_GROWTH        = 81564,
    //Defenders
    NPC_LIFE_WARDEN_GOLA        = 83892,
    NPC_EARTHSHAPER_TELU        = 83893,
    NPC_DULHU                   = 83894,
};

enum eGameObjects
{
    GO_WITHERBARK_DOOR_1    = 231966,
    GO_WITHERBARK_DOOR_2    = 231967,
};

#endif