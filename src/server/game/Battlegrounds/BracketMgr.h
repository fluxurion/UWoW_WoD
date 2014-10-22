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
            static BracketMgr* instance = new BracketMgr();
            return instance;
        }

        typedef UNORDERED_MAP<uint64, BracketList> BracketContainer;

        void LoadCharacterBrackets();

        Bracket* TryGetOrCreateBracket(uint64 guid, BracketType bType);
        void DeleteBracketInfo(uint64 guid);

    private:
        BracketContainer m_conteiner;
};

#define sBracketMgr BracketMgr::instance()
#endif