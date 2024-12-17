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

#ifndef MANGOS_JSONREADER_H
#define MANGOS_JSONREADER_H

#include "Platform/Define.h"
#include "Import/ImportStructures.h"

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>

#include "json.hpp"

#include "../CommunicatorDefines.h"
#include "../Database/DatabaseManager.h"
#include "../Network/CoreSender.h"
#include "../Network/JsonSocket.h"

class AccountPrototype
{
    public:
        AccountPrototype(ImportAccountTypes accountType) : m_accountType(accountType) {}

        virtual void SaveToDB(DatabaseType* database, uint32 accountId) = 0;
        virtual void LoadFromDB(DatabaseType* database, uint32 guid, MaNGOS::Account& account) = 0;
        virtual bool CheckDuplicity(DatabaseType* database) = 0;
    private:
        ImportAccountTypes m_accountType;
};

struct JsonReader
{
    public:
        void RunParsing();

        void LoadJsonsFromDirectory();

        bool LoadAccountFromJson(ImportAccountTypes accountType, uint32 accountId, const nlohmann::json& inputData);
        MaNGOS::Account* LoadAccountFromDB(ImportAccountTypes accountType, uint32 guid);
        void SetDatabaseManager(DatabaseManager* manager) { m_manager = manager; }
        void SetCoreSender(CoreSender* sender) { m_sender = sender; }
        void AddJsonToQueue(ImportAccountTypes type, uint32 accountId, nlohmann::json& inputData, boost::shared_ptr<JsonSocket> socket);
        void AddCharacterGuidForExport(ImportAccountTypes type, uint32 guid, uint32 serverId);
    private:
        std::map<uint32, AccountPrototype*> m_account[ACCOUNT_TYPE_MAX];
        DatabaseManager* m_manager;
        CoreSender* m_sender;

        std::mutex m_jsonQueueMutex;
        std::vector<std::tuple<ImportAccountTypes, uint32, nlohmann::json, boost::shared_ptr<JsonSocket>>> m_jsonQueue;

        std::mutex m_charQueueMutex;
        std::vector<std::tuple<ImportAccountTypes, uint32, uint32>> m_charQueue;
};

#endif