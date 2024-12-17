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
#include "ImportStructures.h"

template <typename T>
void SerializeVector(ByteBuffer& buffer, std::vector<T>& vector)
{
    buffer << uint64(vector.size());
    for (auto& data : vector)
        buffer.append(&data, 1);
}

template <typename T>
void DeseralizeVector(ByteBuffer& buffer, std::vector<T>& vector)
{
    uint64 size;
    buffer >> size;
    vector.resize(size);
    for (auto& data : vector)
        data = buffer.read<T>();
}

uint64 MaNGOS::Account::GetSize()
{
    uint64 size = 0;
    size += m_actions.size() * sizeof(CharacterAction) + sizeof(uint64);
    size += m_auras.size() * sizeof(CharacterAura) + sizeof(uint64);
    size += m_gifts.size() * sizeof(CharacterGifts) + sizeof(uint64);
    size += sizeof(CharacterHomebind);
    size += m_inventory.size() * sizeof(CharacterInventory) + sizeof(uint64);
    size += m_pets.size() * sizeof(CharacterPet) + sizeof(uint64);
    size += m_quests.size() * sizeof(CharacterQueststatus) + sizeof(uint64);
    size += m_reputation.size() * sizeof(CharacterReputation) + sizeof(uint64);
    size += m_skills.size() * sizeof(CharacterSkill) + sizeof(uint64);
    size += m_spells.size() * sizeof(CharacterSpell) + sizeof(uint64);
    size += m_cooldowns.size() * sizeof(CharacterSpellCooldown) + sizeof(uint64);
    size += sizeof(CharacterStats);
    size += sizeof(CharacterData);
    size += m_items.size() * sizeof(ItemInstance) + sizeof(uint64);
    size += m_strings.GetSize();
    return size;
}

void MaNGOS::Account::Serialize(ByteBuffer& buffer)
{
    SerializeVector(buffer, m_actions);
    SerializeVector(buffer, m_auras);
    SerializeVector(buffer, m_gifts);
    buffer.append(&m_homebind, 1);
    SerializeVector(buffer, m_inventory);
    SerializeVector(buffer, m_pets);
    SerializeVector(buffer, m_quests);
    SerializeVector(buffer, m_reputation);
    SerializeVector(buffer, m_skills);
    SerializeVector(buffer, m_spells);
    SerializeVector(buffer, m_cooldowns);
    buffer.append(&m_stats, 1);
    buffer.append(&m_data, 1);
    SerializeVector(buffer, m_items);

    buffer << m_strings.name;
    buffer << m_strings.taxiMask;
    buffer << m_strings.taxiPath;
    buffer << m_strings.exploredZones;
    buffer << m_strings.equipmentCache;
    buffer << m_strings.knownTitles;
    buffer << uint64(m_strings.petStrings.size());
    for (auto& data : m_strings.petStrings)
    {
        buffer << data.id;
        buffer << data.name;
        buffer << data.abData;
        buffer << data.teachSpellData;
    }
    buffer << uint64(m_strings.itemStrings.size());
    for (auto& data : m_strings.itemStrings)
    {
        buffer << data.guid;
        buffer << data.charges;
        buffer << data.enchantments;
    }
}

void MaNGOS::Account::Deserialize(ByteBuffer& buffer)
{
    DeseralizeVector(buffer, m_actions);
    DeseralizeVector(buffer, m_auras);
    DeseralizeVector(buffer, m_gifts);
    m_homebind = buffer.read<MaNGOS::CharacterHomebind>();
    DeseralizeVector(buffer, m_inventory);
    DeseralizeVector(buffer, m_pets);
    DeseralizeVector(buffer, m_quests);
    DeseralizeVector(buffer, m_reputation);
    DeseralizeVector(buffer, m_skills);
    DeseralizeVector(buffer, m_spells);
    DeseralizeVector(buffer, m_cooldowns);
    m_stats = buffer.read<MaNGOS::CharacterStats>();
    m_data = buffer.read<MaNGOS::CharacterData>();
    DeseralizeVector(buffer, m_items);

    buffer >> m_strings.name;
    buffer >> m_strings.taxiMask;
    buffer >> m_strings.taxiPath;
    buffer >> m_strings.exploredZones;
    buffer >> m_strings.equipmentCache;
    buffer >> m_strings.knownTitles;
    uint64 size;
    buffer >> size;
    for (uint64 i = 0; i < size; ++i)
    {
        PetStrings data;
        buffer >> data.id;
        buffer >> data.name;
        buffer >> data.abData;
        buffer >> data.teachSpellData;
        m_strings.petStrings.push_back(data);
    }
    buffer >> size;
    for (uint64 i = 0; i < size; ++i)
    {
        ItemStrings data;
        buffer >> data.guid;
        buffer >> data.charges;
        buffer >> data.enchantments;
        m_strings.itemStrings.push_back(data);
    }
}

uint64 MaNGOS::CharacterStrings::GetSize()
{
    uint64 size = 0;
    size += strlen(name.data()) + sizeof(uint8);
    size += strlen(taxiMask.data()) + sizeof(uint8);
    size += strlen(taxiPath.data()) + sizeof(uint8);
    size += strlen(exploredZones.data()) + sizeof(uint8);
    size += strlen(equipmentCache.data()) + sizeof(uint8);
    size += strlen(knownTitles.data()) + sizeof(uint8);
    size += sizeof(uint64);
    for (auto& data : petStrings)
        size += data.GetSize();
    size += sizeof(uint64);
    for (auto& data : itemStrings)
        size += data.GetSize();
    return size;
}

uint64 MaNGOS::PetStrings::GetSize()
{
    uint64 size = sizeof(uint32);
    size += strlen(name.data()) + sizeof(uint8);
    size += strlen(abData.data()) + sizeof(uint8);
    size += strlen(teachSpellData.data()) + sizeof(uint8);
    return size;
}

uint64 MaNGOS::ItemStrings::GetSize()
{
    uint64 size = sizeof(uint32);
    size += strlen(charges.data()) + sizeof(uint8);
    size += strlen(enchantments.data()) + sizeof(uint8);
    return size;
}
