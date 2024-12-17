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

#ifndef MANGOS_IMPORT_STRUCTS_H
#define MANGOS_IMPORT_STRUCTS_H

#include "Platform/Define.h"
#include "Util/ByteBuffer.h"
#include <string>
#include <vector>

#define MAX_EFFECT_INDEX 3

enum CoreCommunicatorOpcodes
{
    CORE_IMPORT_DATA = 0,
    CORE_IMPORT_ACK = 1,
    CORE_COMMUNICATOR_MAX,
};

namespace MaNGOS
{
#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

    struct CharacterAction
    {
        uint8 button;
        uint32 action;
        uint8 type;
    };

    struct CharacterAura
    {
        uint64 casterGuid;
        uint32 itemGuid;
        uint32 spellId;
        uint32 stackCount;
        uint32 remainingCharges;
        int32 basePoints[MAX_EFFECT_INDEX];
        uint32 periodicTime[MAX_EFFECT_INDEX];
        int32 maxDuration;
        int32 remainingTime;
        uint32 effectMask;
    };

    struct CharacterGifts
    {
        uint32 itemGuid;
        uint32 entry;
        uint32 flags;
    };

    struct CharacterHomebind
    {
        uint32 map;
        uint32 zone;
        float posX;
        float posY;
        float posZ;
    };

    struct CharacterInventory
    {
        uint32 bag;
        uint8 slot;
        uint32 item;
        uint32 itemEntry;
    };

    struct CharacterPet
    {
        uint32 id;
        uint32 entry;
        uint32 modelId;
        uint32 createdBySpell;
        uint8 petType;
        uint32 level;
        uint32 experience;
        uint8 reactState;
        int32 loyaltyPoints;
        uint32 loyalty;
        uint32 xpForNextLoyalty;
        int32 trainPoint;
        uint8 renamed;
        uint32 slot;
        uint32 curHealth;
        uint32 curMana;
        uint32 curHappiness;
        uint64 savetime;
        uint32 resetTalentCost;
        uint64 resetTalentTime;
    };

    struct CharacterQueststatus
    {
        uint32 quest;
        uint32 status;
        uint8 rewarded;
        uint8 explored;
        uint64 timer;
        uint32 mobCount[4];
        uint32 itemCount[4];
    };

    struct CharacterReputation
    {
        uint32 faction;
        int32 standing;
        int32 flags;
    };

    struct CharacterSkill
    {
        uint32 skill;
        uint32 value;
        uint32 max;
    };

    struct CharacterSpell
    {
        uint32 spell;
        uint8 active;
        uint8 disabled;
    };

    struct CharacterSpellCooldown
    {
        uint32 spellId;
        uint64 time;
        uint32 category; // Fill on import
        uint64 categoryTime;
        uint32 itemEntry;
    };

    struct CharacterStats
    {
        uint32 maxHealth;
        uint32 maxPower[7];
        uint32 strength;
        uint32 agility;
        uint32 stamina;
        uint32 intellect;
        uint32 spirit;
        uint32 armor;
        uint32 resistances[6];
        float blockPct;
        float dodgePct;
        float parryPct;
        float critPct;
        float rangedCritPct;
        float spellCritPct;
        uint32 attackPower;
        uint32 rangedAttackPower;
        uint32 spellPower;
    };

    struct CharacterData
    {
        uint32 oldGuid;

        uint32 account;
        uint8 race;
        uint8 playerClass;
        uint8 gender;
        uint8 level;
        uint32 experience;
        uint32 money;
        uint32 playerBytes;
        uint32 playerBytes2;
        uint32 playerFlags;

        float posX;
        float posY;
        float posZ;
        uint32 map;
        uint8 dungeonDifficulty;
        float ori;
        uint8 online;
        uint8 cinematic;
        uint32 totalTime;
        uint32 levelTime;

        uint64 logoutTime;
        uint8 isLogoutResting;
        float restBonus;
        uint32 resetTalentsMultiplier;
        uint64 resetTalentsTime;
        float transportX;
        float transportY;
        float transportZ;
        float transportOri;
        uint64 transportGuid;

        uint32 extraFlags;
        uint8 stableSlots;
        uint32 atLogin;
        uint32 zone;
        uint64 deathExpireTime;
        uint32 arenaPoints;
        uint32 totalHonor;
        uint32 todayHonor;
        uint32 yesterdayHonor;
        uint32 totalKills;

        uint32 todayKills;
        uint32 yesterdayKills;
        uint32 chosenTitle;
        uint32 watchedFaction;
        uint32 drunk;
        uint32 health;
        uint32 power[5];
        uint32 ammoId;
        uint8 actionBars;
    };

    struct ItemInstance
    {
        uint32 guid;
        uint32 entry;
        uint32 ownerGuid;
        uint32 creatorGuid;
        uint32 giftCreatorGuid;
        uint32 stackCount;
        int32 duration;        
        uint32 flags;        
        int16 randomPropertySeed;
        int16 randomPropertyId;
        uint16 durability;
        uint32 textId;
    };

    struct PetStrings
    {
        uint32 id;
        std::string name;
        std::string abData;
        std::string teachSpellData;
        uint64 GetSize();
    };

    struct ItemStrings
    {
        uint32 guid;
        std::string charges;
        std::string enchantments;
        uint64 GetSize();
    };

    struct CharacterStrings
    {
        std::string name;
        std::string taxiMask;
        std::string taxiPath;
        std::string exploredZones;
        std::string equipmentCache;
        std::string knownTitles;
        std::vector<PetStrings> petStrings;
        std::vector<ItemStrings> itemStrings;
        uint64 GetSize();
    };

    struct Account
    {
        std::vector<CharacterAction> m_actions;
        std::vector<CharacterAura> m_auras;
        std::vector<CharacterGifts> m_gifts;
        CharacterHomebind m_homebind;
        std::vector<CharacterInventory> m_inventory;
        std::vector<CharacterPet> m_pets;
        std::vector<CharacterQueststatus> m_quests;
        std::vector<CharacterReputation> m_reputation;
        std::vector<CharacterSkill> m_skills;
        std::vector<CharacterSpell> m_spells;
        std::vector<CharacterSpellCooldown> m_cooldowns;
        CharacterStats m_stats;
        CharacterData m_data;
        std::vector<ItemInstance> m_items;
        CharacterStrings m_strings;
        uint64 GetSize();
        void Serialize(ByteBuffer& buffer);
        void Deserialize(ByteBuffer& buffer);
    };

#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif
}

#endif
