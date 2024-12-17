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

#ifndef MANGOS_JSONSOCKET_H
#define MANGOS_JSONSOCKET_H

#include "Network/AsyncSocket.hpp"

class JsonSocket : public MaNGOS::AsyncSocket<JsonSocket>
{
    public:
        JsonSocket(boost::asio::io_service& io_service);

        void SendJsonAckStatus(bool failed);
        void SendImportAckStatus(bool failed);
    private:
        bool _HandleJsonReceive();
        bool _HandleImportReceive();

        virtual bool ProcessIncomingData() override;
        void ProcessDataCallback(const boost::system::error_code& error, std::size_t written);
};

#endif