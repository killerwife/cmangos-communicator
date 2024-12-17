/*
* This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "DatabaseManager.h"
#include "Config/Config.h"
#include "Log.h"

DatabaseManager::DatabaseManager() : m_databases(ACCOUNT_TYPE_MAX)
{
}

DatabaseManager::~DatabaseManager()
{
    for (DatabaseType* db : m_databases)
        delete db;
}

bool DatabaseManager::StartDB()
{
    std::string dbstring = sConfig.GetStringDefault("CommunicatorDatabaseInfo");
    if (dbstring.empty())
    {
        sLog.outError("Database not specified");
        return false;
    }

    sLog.outString("Communicator Database total connections: %i", 1 + 1);

    if (!CommunicatorDatabase.Initialize(dbstring.c_str()))
    {
        sLog.outError("Cannot connect to database");
        return false;
    }

    {
        std::unique_ptr<QueryResult> result(CommunicatorDatabase.Query("SELECT Type, ConnectionString FROM `database_connections`"));
        if (!result)
        {
            sLog.outError("No databases configured for import. Import will not function.");
            return true;
        }

        do
        {
            Field* fields = result->Fetch();
            ImportAccountTypes type = ImportAccountTypes(fields[0].GetUInt32());
            std::string connectionString = fields[1].GetCppString();
            DatabaseType* newDatabase = new DatabaseType();
            if (!newDatabase->Initialize(connectionString.data()))
            {
                delete newDatabase;
                sLog.outError("Could not connect to DB type %u.", uint32(type));
            }
            else
                m_databases[type] = newDatabase;
        }
        while (result->NextRow());
    }

    return true;
}

void DatabaseManager::BeforeLoopStart()
{
    // server has started up successfully => enable async DB requests
    CommunicatorDatabase.AllowAsyncTransactions();
    for (DatabaseType* type : m_databases)
        if (type)
            type->AllowAsyncTransactions();
}

void DatabaseManager::AfterLoopEnd()
{
    // Wait for the delay thread to exit
    for (DatabaseType* type : m_databases)
        if (type)
            type->HaltDelayThread();
    CommunicatorDatabase.HaltDelayThread();
}
