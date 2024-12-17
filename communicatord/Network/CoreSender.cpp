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

#include "CoreSender.h"
#include "Database/DatabaseManager.h"
#include "ByteBuffer.h"
#include <iostream>
#include <fstream>
#include <atomic>
#include <thread>

extern std::atomic<bool>& GetStopEvent();

#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

struct ImportAck
{
    uint8 opcode;
    uint32 guid;
};

#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

CoreSender::~CoreSender()
{
    for (auto& conn : m_connections)
        delete conn.second;
}

void CoreSender::LoadServerInfo()
{
    std::unique_ptr<QueryResult> result(m_manager->CommunicatorDatabase.Query("SELECT Id, Ip, Port FROM `servers`"));
    if (!result)
    {
        sLog.outError("No servers configured for import. Import to live database will not function.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();
        uint32 id = fields[0].GetUInt32();
        std::string ip = fields[1].GetCppString();
        int port = fields[2].GetInt32();
        m_serverInfo[id] = { ip, port };
    } while (result->NextRow());
}

void CoreSender::RunSender()
{
    for (auto& data : m_serverInfo)
    {
        Connection* conn = new Connection(*m_ios, data.second.first, data.second.second);
        m_connections.emplace(data.first, conn);
        conn->Connect();
    }
    do
    {
        {
            std::lock_guard<std::mutex> m_guard(m_accountMutex);
            std::vector<std::tuple<MaNGOS::Account*, uint32, ImportAccountTypes>> unprocessed;
            for (auto& data : m_accounts)
            {
                if (SendAccountData(data))
                    delete std::get<0>(data);
                else
                    unprocessed.push_back(data);
            }
            m_accounts = unprocessed;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (!GetStopEvent());
}

void CoreSender::QueueAccountForSend(MaNGOS::Account* account, uint32 serverId, ImportAccountTypes type)
{
    std::lock_guard<std::mutex> m_guard(m_accountMutex);
    m_accounts.emplace_back(account, serverId, type);
}

bool CoreSender::SendAccountData(std::tuple<MaNGOS::Account*, uint32, ImportAccountTypes>& data)
{
    uint32 connectionId = std::get<1>(data);
    auto itr = m_connections.find(connectionId);
    if (itr == m_connections.end())
    {
        sLog.outError("Error in sending to core - connection index too high.");
        return true; // complete failure - get rid of it
    }

    Connection& connection = *(*itr).second;
    if (connection.GetOpenStatus() != true || !connection.GetSocket().is_open()) // reconnect and try again
    {
        connection.Reconnect();
        return false;
    }

    ByteBuffer pkt;
    pkt << uint8(CORE_IMPORT_DATA);
    pkt << std::get<0>(data)->GetSize();
    std::get<0>(data)->Serialize(pkt);
    boost::system::error_code error;
    boost::asio::write(connection.GetSocket(), boost::asio::buffer(pkt.contents(), pkt.size()), error);
    if (error)
    {
        DETAIL_LOG("During CORE_IMPORT_DATA data send to server id %u: %s\n", connectionId, error.message().data());
        connection.GetSocket().close();
        return false;
    }

    ImportAck ackData;
    boost::asio::read(connection.GetSocket(), boost::asio::buffer((char*)&(ackData), sizeof(ImportAck)), error);
    if (error)
    {
        DETAIL_LOG("During CORE_IMPORT_ACK data receive: %s\n", error.message().data());
        connection.GetSocket().close();
        return false;
    }

    if (DatabaseType* db = this->GetManager()->GetDatabaseType(std::get<2>(data)))
        db->PExecute("UPDATE characters SET inserted=2 WHERE guid=%u", ackData.guid);
    return true;
}

Connection::Connection(boost::asio::io_service& ios, std::string ip, int port) : m_ios(ios), m_endpoint(boost::asio::ip::address::from_string(ip), port),
    m_socket(new boost::asio::ip::tcp::socket(m_ios)), m_open(false)
{

}

Connection::~Connection()
{
    m_socket->close();
}

void Connection::Connect()
{
    m_socket->async_connect(m_endpoint, [this](const boost::system::error_code& error)
    {
        auto& open = GetOpenStatus();
        open = true;
        if (error)
        {
            sLog.outError("Core connection failed on: %s", error.message().data());
            return;
        }
    });
}

void Connection::Reconnect()
{
    if (!m_open)
        return;
    m_socket.reset(new boost::asio::ip::tcp::socket(m_ios));
    m_open = false;
    Connect();
}
