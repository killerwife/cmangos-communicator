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

#include "JsonSocket.h"
#include "Log.h"
#include "ByteBuffer.h"
#include "Json/JsonReader.h"
#include <exception>

extern JsonReader* GetReader();
extern std::atomic<bool>& GetStopEvent();

#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

struct PacketJsonInput
{
    uint8 type;
    uint32 accountId;
    uint32 size;
};

struct PacketImportChar
{
    uint8 type;
    uint32 guid;
    uint32 serverId;
};

#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

struct JsonHandler
{
    JsonOpcodes opcode;
    bool (JsonSocket::* handler)(void);
};

JsonSocket::JsonSocket(boost::asio::io_service& io_service)
    : AsyncSocket(io_service)
{
}

bool JsonSocket::_HandleJsonReceive()
{
    auto self(shared_from_this());
    std::shared_ptr<PacketJsonInput> input(new PacketJsonInput);
    DETAIL_LOG("Received packet JSON_OPCODE_INPUT");
    Read((char*)&(*input), sizeof(PacketJsonInput), [input,self](const boost::system::error_code& error, std::size_t read)
    {
        DETAIL_LOG("During JSON_OPCODE_INPUT data receive: %s Received %u", error.message().data(), uint32(read));
        if (error)
            return;
        DETAIL_LOG("Received packet JSON_OPCODE_INPUT PacketJsonInput data Account ID %u Size %u Type %u", input->accountId, input->size, input->type);
        std::shared_ptr<std::vector<uint8>> buffer(new std::vector<uint8>);
        buffer->resize(input->size);
        self->Read((char*)&(*buffer)[0], input->size, [buffer,input,self](const boost::system::error_code& error, std::size_t read)
        {
            DETAIL_LOG("During JSON_OPCODE_INPUT json receive: %s Received %u", error.message().data(), uint32(read));
            if (error)
                return;
            DETAIL_LOG("Received packet JSON_OPCODE_INPUT json string data");
            const char* jsonString = (const char*)&(*buffer)[0];
            {
                std::stringstream stream(jsonString);
                try
                {
                    nlohmann::json json;
                    stream >> json;
                    GetReader()->AddJsonToQueue(ImportAccountTypes(input->type), input->accountId, json, self);
                }
                catch (std::exception& e)
                {
                    sLog.outError("Json loading exception occured: %s for account %u", e.what(), input->accountId);
                }
            }

            DETAIL_LOG("Json for account %u parsed", input->accountId);
            self->ProcessIncomingData();
        });
    });

    return true;
}

void JsonSocket::SendJsonAckStatus(bool failed)
{
    std::shared_ptr<ByteBuffer> pkt(new ByteBuffer);
    *pkt << uint8(JSON_OPCODE_INPUT_ACK);
    *pkt << uint8(failed);
    auto self(shared_from_this());
    Write((const char*)pkt->contents(), pkt->size(), [self, pkt](const boost::system::error_code& error, std::size_t read) {});
}

bool JsonSocket::_HandleImportReceive()
{
    auto self(shared_from_this());
    std::shared_ptr<PacketImportChar> input(new PacketImportChar);
    DETAIL_LOG("Received packet JSON_OPCODE_IMPORT_CHAR");
    std::function<void(const boost::system::error_code&, std::size_t)> callback = [self,input](const boost::system::error_code& error, std::size_t read)
    {
        DETAIL_LOG("During JSON_OPCODE_IMPORT_CHAR receive: %s Received %u", error.message().data(), uint32(read));
        if (error)
            return;

        DETAIL_LOG("Received packet JSON_OPCODE_IMPORT_CHAR PacketImportChar data: type %u, guid %u, server ID %u", input->type, input->guid, input->serverId);
        {
            GetReader()->AddCharacterGuidForExport(ImportAccountTypes(input->type), input->guid, input->serverId);
        }

        self->SendImportAckStatus(false);
        self->ProcessIncomingData();
    };
    Read((char*)&(*input), sizeof(PacketImportChar), callback);
    return true;
}

void JsonSocket::SendImportAckStatus(bool failed)
{
    ByteBuffer* pkt = new ByteBuffer;
    *pkt << uint8(JSON_OPCODE_IMPORT_ACK);
    *pkt << uint8(failed);
    auto self(shared_from_this());
    Write((const char*)pkt->contents(), pkt->size(), [self, pkt](const boost::system::error_code& error, std::size_t read) { delete pkt; });
}

bool JsonSocket::ProcessIncomingData()
{
    if (GetStopEvent())
        return false;

    const static JsonHandler table[] =
    {
        { JSON_OPCODE_INPUT, &JsonSocket::_HandleJsonReceive },
        { JSON_OPCODE_INPUT_ACK, nullptr},
        { JSON_OPCODE_IMPORT_CHAR, &JsonSocket::_HandleImportReceive },
        { JSON_OPCODE_IMPORT_ACK, nullptr},
    };

    sLog.outBasic("Established connection.");
    std::shared_ptr<uint8> packetId(new uint8);
    auto self(shared_from_this());
    std::function<void(const boost::system::error_code&, std::size_t)> callback = [self,packetId](const boost::system::error_code& error, std::size_t read)
    {
        DETAIL_LOG("During opcode receive: %s\n", error.message().data());
        if (error)
            return;
        const JsonOpcodes opcode = static_cast<JsonOpcodes>(*packetId);
        if (opcode < JSON_OPCODE_MAX && table[opcode].handler)
        {
            if (!(*self.*table[opcode].handler)())
            {
                DETAIL_LOG("[Json] Command handler failed for cmd %u", opcode);
            }
        }
    };
    Read((char*)&(*packetId), sizeof(uint8), callback);
    return true;
}

void JsonSocket::ProcessDataCallback(const boost::system::error_code& error, std::size_t written)
{
    if (error)
        return;
    ProcessIncomingData();
}
