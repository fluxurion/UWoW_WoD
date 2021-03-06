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

#ifndef LOG_H
#define LOG_H

#include "Define.h"
#include "Appender.h"
#include "Logger.h"
#include <stdarg.h>
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>

#include <unordered_map>
#include <string>

#include <string>
#include <set>

class Log
{
    typedef std::unordered_map<uint8, Logger> LoggerMap;
    typedef std::unordered_map<uint8, Logger const*> CachedLoggerContainer;

    private:
        Log();
        ~Log();

    public:

        static Log* instance(boost::asio::io_service* ioService = nullptr)
        {
            static Log instance;

            if (ioService != nullptr)
            {
                instance._ioService = ioService;
                instance._strand = new boost::asio::strand(*ioService);
            }

            return &instance;
        }

        void LoadFromConfig();
        void Close();
        bool ShouldLog(LogFilterType type, LogLevel level) const;
        bool SetLogLevel(std::string const& name, char const* level, bool isLogger = true);

        void outTrace(LogFilterType f, char const* str, ...) ATTR_PRINTF(3,4);
        void outDebug(LogFilterType f, char const* str, ...) ATTR_PRINTF(3,4);
        void outInfo (LogFilterType f, char const* str, ...) ATTR_PRINTF(3,4);
        void outWarn (LogFilterType f, char const* str, ...) ATTR_PRINTF(3,4);
        void outError(LogFilterType f, char const* str, ...) ATTR_PRINTF(3,4);
        void outFatal(LogFilterType f, char const* str, ...) ATTR_PRINTF(3,4);
        void outArena(const char * str, ...)                 ATTR_PRINTF(2,3);
        void outSpamm( const char * str, ... )               ATTR_PRINTF(2, 3);
        void outDiff( const char * str, ... )                ATTR_PRINTF(2, 3);
        void outCommand(uint32 account, const char * str, ...) ATTR_PRINTF(3, 4);
        void outCharDump(char const* str, uint32 account_id, uint64 guid, char const* name);

        void EnableDBAppenders();
        static std::string GetTimestampStr();
        
        void SetRealmID(uint32 id);
        static void outTimestamp(FILE* file);
        uint32 GetRealmID() const { return realm; }

        void outU(const char* str, ...);
    private:
        void vlog(LogFilterType f, LogLevel level, char const* str, va_list argptr);
        void write(LogMessage* msg);

        Logger* GetLoggerByType(LogFilterType filter);
        Appender* GetAppenderByName(std::string const& name);
        uint8 NextAppenderId();
        void CreateAppenderFromConfig(const char* name);
        void CreateLoggerFromConfig(const char* name);
        void ReadAppendersFromConfig();
        void ReadLoggersFromConfig();

        AppenderMap appenders;
        LoggerMap loggers;
        uint8 AppenderId;
        LogLevel lowestLogLevel;
        FILE* openLogFile(char const* configFileName, char const* configTimeStampFlag, char const* mode);
        FILE* arenaLogFile;
        FILE* spammLogFile;
        FILE* diffLogFile;

        std::string m_logsDir;
        std::string m_logsTimestamp;

        uint32 realm;
        boost::asio::io_service* _ioService;
        boost::asio::strand* _strand;
};

#define sLog Log::instance()

#endif
