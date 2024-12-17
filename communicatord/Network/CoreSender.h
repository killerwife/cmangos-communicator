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

#ifndef MANGOS_CORESENDER_H
#define MANGOS_CORESENDER_H

#include "Import/ImportStructures.h"
#include "Database/DatabaseManager.h"
#include <boost/asio.hpp>
#include <mutex>
#include <atomic>

class Connection
{
    public:
        Connection(boost::asio::io_service& ios, std::string ip, int port);
        Connection(const Connection& other) = delete;
        Connection(Connection&& other) = default;
        ~Connection();
        void Connect();
        void Reconnect();
        boost::asio::ip::tcp::socket& GetSocket() { return *(m_socket.get()); }
        std::atomic<bool>& GetOpenStatus() { return m_open; }
    private:
        boost::asio::io_service& m_ios;
        boost::asio::ip::tcp::endpoint m_endpoint;
        boost::shared_ptr<boost::asio::ip::tcp::socket> m_socket;
        std::atomic<bool> m_open;
};

class CoreSender
{
    public:
        ~CoreSender();
        void LoadServerInfo();
        void RunSender();

        void SetDatabaseManager(DatabaseManager* manager) { m_manager = manager; }
        void SetIoService(boost::asio::io_service* ios) { m_ios = ios; }
        void QueueAccountForSend(MaNGOS::Account* account, uint32 serverId, ImportAccountTypes type);
        DatabaseManager* GetManager() { return m_manager; }
    private:
        bool SendAccountData(std::tuple<MaNGOS::Account*, uint32, ImportAccountTypes>& data);

        DatabaseManager* m_manager;
        std::map<uint32, std::pair<std::string, int>> m_serverInfo;

        std::mutex m_accountMutex;
        std::vector<std::tuple<MaNGOS::Account*, uint32, ImportAccountTypes>> m_accounts;

        std::map<uint32, Connection*> m_connections;

        boost::asio::io_service* m_ios;
};

#endif