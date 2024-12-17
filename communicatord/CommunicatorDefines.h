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

#ifndef MANGOS_COMMUNICATOR_DEFINES_H
#define MANGOS_COMMUNICATOR_DEFINES_H

#define MAX_EFFECT_INDEX 3

enum ImportAccountTypes
{
    ACCOUNT_TYPE_CMANGOS_CLASSIC    = 0, // TODO
    ACCOUNT_TYPE_CMANGOS_TBC        = 1, // TODO
    ACCOUNT_TYPE_CMANGOS_WOTLK      = 2, // TODO
    ACCOUNT_TYPE_NOST_CORE          = 3,
    ACCOUNT_TYPE_TRINITYCORE_WOTLK  = 4, // TODO
    ACCOUNT_TYPE_MAX,
};

enum JsonOpcodes
{
    JSON_OPCODE_INPUT       = 0,
    JSON_OPCODE_INPUT_ACK   = 1,
    JSON_OPCODE_IMPORT_CHAR = 2,
    JSON_OPCODE_IMPORT_ACK  = 3,
    JSON_OPCODE_MAX,
};

#endif