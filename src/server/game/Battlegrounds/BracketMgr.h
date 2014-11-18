/*
uwow.biz
*/

#ifndef _BRACKETMGR_H
#define _BRACKETMGR_H

#include "Bracket.h"
#include "Player.h"

class BracketMgr
{
    private:
        BracketMgr() { }
        ~BracketMgr();

    public:
        static BracketMgr* instance()
        {
            static BracketMgr instance;
            return &instance;
        }

        typedef UNORDERED_MAP<ObjectGuid, BracketList> BracketContainer;

        void LoadCharacterBrackets();

        Bracket* TryGetOrCreateBracket(ObjectGuid guid, BracketType bType);
        void DeleteBracketInfo(ObjectGuid guid);

    private:
        BracketContainer m_conteiner;
};

#define sBracketMgr BracketMgr::instance()
#endif