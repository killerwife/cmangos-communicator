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

#include "JsonReader.h"
#include "Config/Config.h"
#include "NostCoreJson.h"
#include <filesystem>
#include <atomic>

extern std::atomic<bool>& GetStopEvent();

using json = nlohmann::json;

void JsonReader::RunParsing()
{
    do
    {
        LoadJsonsFromDirectory();
        {
            std::lock_guard<std::mutex> guard(m_jsonQueueMutex);
            for (auto& inputData : m_jsonQueue)
            {
                bool success = LoadAccountFromJson(std::move(std::get<0>(inputData)), std::get<1>(inputData), std::get<2>(inputData));
                std::get<3>(inputData)->SendJsonAckStatus(!success);
            }
            m_jsonQueue.clear();
        }
        {
            std::lock_guard<std::mutex> guard(m_charQueueMutex);
            for (auto& inputData : m_charQueue)
            {
                MaNGOS::Account* account = LoadAccountFromDB(std::get<0>(inputData), std::get<1>(inputData));
                if (!account)
                    continue;
                uint32 serverId = std::get<2>(inputData);
                if (DatabaseType* db = m_manager->GetDatabaseType(std::get<0>(inputData)))
                    db->PExecute("UPDATE characters SET inserted=1,insertedTo=%u WHERE guid=%u", serverId, std::get<1>(inputData));
                DETAIL_LOG("Character guid %u loaded from db", std::get<1>(inputData));
                m_sender->QueueAccountForSend(account, serverId, std::get<0>(inputData));
            }
            m_charQueue.clear();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    while (!GetStopEvent());
}

void JsonReader::LoadJsonsFromDirectory()
{
    std::vector<std::filesystem::path> doneFiles;
    for (auto& entry : std::filesystem::directory_iterator(sConfig.GetStringDefault("JsonFolder")))
    {
        if (entry.path().extension() == ".json")
        {
            std::ifstream inputFile(entry.path().string());
            json characterData;
            inputFile >> characterData;
            LoadAccountFromJson(ACCOUNT_TYPE_NOST_CORE, 0, characterData);
            doneFiles.push_back(entry.path());
        }
    }
    for (auto& path : doneFiles)
        std::filesystem::remove(path);
}

bool JsonReader::LoadAccountFromJson(ImportAccountTypes accountType, uint32 accountId, const nlohmann::json& inputData)
{
    std::vector<std::unique_ptr<AccountPrototype>> accounts;
    if (accountType == ACCOUNT_TYPE_NOST_CORE)
    {
        for (auto& data : inputData)
            accounts.push_back(std::unique_ptr<AccountPrototype>(data["account"].get<NostCoreJson::Account*>()));
    }
    for (auto& account : accounts)
    {
        auto database = m_manager->GetDatabaseType(accountType);
        if (account->CheckDuplicity(database))
            account->SaveToDB(database, accountId);
        else
        {
            sLog.outError("Attempt at duplicitous insert for account ID: %u", accountId);
            return false;
        }
    }
    DETAIL_LOG("Json processed for account %u.", accountId);
    return true;
}

MaNGOS::Account* JsonReader::LoadAccountFromDB(ImportAccountTypes accountType, uint32 guid)
{
    if (accountType == ACCOUNT_TYPE_NOST_CORE)
    {
        auto database = m_manager->GetDatabaseType(accountType);
        std::unique_ptr<QueryResult> result(database->PQuery("SELECT inserted FROM characters WHERE guid='%u'", guid));
        if (result && result->Fetch()[0].GetUInt32() == 2) // already inserted in db
        {
            sLog.outError("Attempt at duplicitous import of character guid: %u", guid);
            return nullptr;
        }
        auto mangosAccount = new MaNGOS::Account();
        NostCoreJson::Account account;
        account.LoadFromDB(database, guid, *mangosAccount);
        return mangosAccount;
    }
    return nullptr;
}

void JsonReader::AddJsonToQueue(ImportAccountTypes type, uint32 accountId, nlohmann::json& inputData, boost::shared_ptr<JsonSocket> socket)
{
    std::lock_guard<std::mutex> guard(m_jsonQueueMutex);
    m_jsonQueue.push_back({ type, accountId, inputData, socket });
}

void JsonReader::AddCharacterGuidForExport(ImportAccountTypes type, uint32 guid, uint32 serverId)
{
    {
        std::lock_guard<std::mutex> guard(m_charQueueMutex);
        m_charQueue.push_back({ type, guid, serverId });
    }
}
