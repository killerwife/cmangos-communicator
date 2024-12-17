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

#include "Import/ImportStructures.h"
#include "ImportSocket.h"
#include "World/World.h"
#include "Log.h"

struct ImportHandler
{
    CoreCommunicatorOpcodes opcode;
    bool (ImportSocket::* handler)(void);
};

ImportSocket::ImportSocket(boost::asio::io_service& io_service)
    : AsyncSocket(io_service)
{
}

bool ImportSocket::_HandleImportReceive()
{
    auto self(shared_from_this());
    std::shared_ptr<uint64> input(new uint64);
    Read((char*)&(*input), sizeof(uint64), [input, self](const boost::system::error_code& error, std::size_t read)
    {
        if (error)
            return;
        std::shared_ptr<std::vector<uint8>> buffer(new std::vector<uint8>);
        buffer->resize(*input);
        self->Read((char*)&(*buffer)[0], *input, [buffer, input, self](const boost::system::error_code& error, std::size_t read)
        {
            if (error)
                return;
            ByteBuffer pkt(*buffer);
            MaNGOS::Account* account = new MaNGOS::Account();
            account->Deserialize(pkt);
            sWorld.GetMessager().AddMessage([=](World* world)
            {
                world->ImportPlayer(account);
            });
            std::shared_ptr<ByteBuffer> pktTwo(new ByteBuffer);
            *pktTwo << uint8(CORE_IMPORT_ACK);
            *pktTwo << account->m_data.oldGuid;
            self->Write((const char*)pktTwo->contents(), pktTwo->size(), [self, pktTwo](const boost::system::error_code& error, std::size_t read) { });
            self->ProcessIncomingData();
        });
    });
    return true;
}

bool ImportSocket::ProcessIncomingData()
{
    if (World::IsStopped())
        return false;

    const static ImportHandler table[] =
    {
        { CORE_IMPORT_DATA, &ImportSocket::_HandleImportReceive },
    };

    std::shared_ptr<uint8> packetId(new uint8);
    auto self(shared_from_this());
    std::function<void(const boost::system::error_code&, std::size_t)> callback = [self, packetId](const boost::system::error_code& error, std::size_t read)
    {
        printf("%s\n", error.message().data());
        if (error)
            return;
        const CoreCommunicatorOpcodes opcode = static_cast<CoreCommunicatorOpcodes>(*packetId);
        if (opcode < CORE_COMMUNICATOR_MAX && table[opcode].handler)
        {
            if (!(*self.*table[opcode].handler)())
            {
                DEBUG_LOG("[Json] Command handler failed for cmd %u", opcode);
            }
        }
    };
    Read((char*)&(*packetId), sizeof(uint8), callback);
    return true;
}
