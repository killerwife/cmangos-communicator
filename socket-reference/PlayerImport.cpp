void Player::SaveToDB(uint32 guid, MaNGOS::Account& accountData)
{
    DEBUG_LOG("Inserting character guid %u for character %s\n", guid, accountData.m_strings.name.data());
    if (ObjectMgr::CheckPlayerName(accountData.m_strings.name, true) != CHAR_NAME_SUCCESS || sObjectMgr.IsReservedName(accountData.m_strings.name) || sObjectMgr.GetPlayerGuidByName(accountData.m_strings.name))
        accountData.m_data.atLogin |= uint32(AT_LOGIN_RENAME);

    accountData.m_data.atLogin |= uint32(AT_LOGIN_RESET_TALENTS);

    // custom balance import changes
    accountData.m_data.money = accountData.m_data.money > 20000000 ? 20000000 : accountData.m_data.money;

    {
        auto& data = accountData.m_data;
        CharacterDatabase.PExecute("INSERT INTO characters VALUES(%u, %u, \"%s\", %u, %u, %u, %u, %u, %u, %u,"
            "%u, %u, %f, %f, %f, %u, %u, %f, '%s', %u,"
            "%u, %u, %u, %lu, %u, %f, %u, %lu, %f, %f,"
            "%f, %f, %lu, %u, %u, %u, %u, %lu, '%s', %u,"
            "%u, %u, %u, %u, %u, %u, %u, %u, %u, %u,"
            "%u, %u, %u, %u, %u, '%s', '%s', %u, '%s', %u, 0,"
            "NULL, NULL, NULL)",
            guid, data.account, accountData.m_strings.name.data(), data.race, data.playerClass, data.gender, data.level, data.experience, data.money, data.playerBytes,
            data.playerBytes2, data.playerFlags, data.posX, data.posY, data.posZ, data.map, data.dungeonDifficulty, data.ori, accountData.m_strings.taxiMask.data(), data.online,
            data.cinematic, data.totalTime, data.levelTime, data.logoutTime, data.isLogoutResting, data.restBonus, data.resetTalentsMultiplier, data.resetTalentsTime, data.transportX, data.transportY,
            data.transportZ, data.transportOri, data.transportGuid, data.extraFlags, data.stableSlots, data.atLogin, data.zone, data.deathExpireTime, accountData.m_strings.taxiPath.data(), data.arenaPoints,
            data.totalHonor, data.todayHonor, data.yesterdayHonor, data.totalKills, data.todayKills, data.yesterdayKills, data.chosenTitle, data.watchedFaction, data.drunk, data.health,
            data.power[0], data.power[1], data.power[2], data.power[3], data.power[4], accountData.m_strings.exploredZones.data(), accountData.m_strings.equipmentCache.data(), data.ammoId, accountData.m_strings.knownTitles.data(), data.actionBars);

        CharacterDatabase.PExecute("INSERT INTO characters_imported VALUES(%u, %u)", guid, data.oldGuid);
    }
    
    for (auto& data : accountData.m_actions)
        CharacterDatabase.PExecute("INSERT INTO character_action(guid,button,action,type) VALUES(%u, %u, %u, %u)", guid, data.button, data.action, data.type);

    for (auto& data : accountData.m_auras)
        CharacterDatabase.PExecute("INSERT INTO character_aura "
            "VALUES(%u, %lu, %u, %u, %u, %u, %d, %d, %d, %d, %u, %u, %d, %d, %u)", guid, data.casterGuid, data.itemGuid, data.spellId, data.stackCount,
            data.remainingCharges, data.basePoints[0], data.basePoints[1], data.basePoints[2], data.periodicTime[0], data.periodicTime[1], data.periodicTime[2], data.maxDuration, data.remainingTime, data.effectMask);

    for (auto& data : accountData.m_gifts)
        CharacterDatabase.PExecute("INSERT INTO character_gifts VALUES(%u, %u, %u, %u)", guid, data.itemGuid, data.entry, data.flags);

    CharacterDatabase.PExecute("INSERT INTO character_homebind VALUES(%u, %u, %u, %f, %f, %f)", guid, accountData.m_homebind.map, accountData.m_homebind.zone, accountData.m_homebind.posX, accountData.m_homebind.posY, accountData.m_homebind.posZ);

    for (uint32 i = 0; i < accountData.m_pets.size(); ++i)
    {
        auto& data = accountData.m_pets[i];
        CharacterDatabase.PExecute("INSERT INTO character_pet VALUES(%u, %u, %u, %u, %u, %u, %u, %u, %u, %d, %u, %u, %d, \"%s\", %u, %u, %u, %u, %u, %lu, %u, %lu, '%s', '%s')",
            sObjectMgr.GeneratePetNumber(), data.entry, guid, data.modelId, data.createdBySpell, data.petType, data.level, data.experience, data.reactState, data.loyaltyPoints, data.loyalty, data.xpForNextLoyalty, data.trainPoint, accountData.m_strings.petStrings[i].name.data(),
            data.renamed, data.slot, data.curHealth, data.curMana, data.curHappiness, data.savetime, data.resetTalentCost, data.resetTalentTime, accountData.m_strings.petStrings[i].abData.data(), accountData.m_strings.petStrings[i].teachSpellData.data());
    }

    for (auto& data : accountData.m_quests)
    {
        Quest const* quest = sObjectMgr.GetQuestTemplate(data.quest);
        uint32 questStatus = data.status; // nost core uses this field differently and this causes a plethora of quests to be added even tho they are repeatable
        if (quest && quest->IsRepeatable() && questStatus == 1 && data.rewarded == 1)
            questStatus = 0; // everyone can pick it up again
        CharacterDatabase.PExecute("INSERT INTO character_queststatus VALUES(%u, %u, %u, %u, %u, %lu, %u, %u, %u, %u, %u, %u, %u, %u)",
            guid, data.quest, questStatus, data.rewarded, data.explored, data.timer, data.mobCount[0], data.mobCount[1], data.mobCount[2], data.mobCount[3], data.itemCount[0], data.itemCount[1], data.itemCount[2], data.itemCount[3]);
    }

    for (auto& data : accountData.m_reputation)
        CharacterDatabase.PExecute("INSERT INTO character_reputation VALUES(%u, %u, %d, %d)", guid, data.faction, data.standing, data.flags);

    for (auto& data : accountData.m_skills)
        CharacterDatabase.PExecute("INSERT INTO character_skills VALUES(%u, %u, %u, %u)", guid, data.skill, data.value, data.max);

    for (auto& data : accountData.m_spells)
        CharacterDatabase.PExecute("INSERT INTO character_spell VALUES(%u, %u, %u, %u)", guid, data.spell, data.active, data.disabled);

    for (auto& data : accountData.m_cooldowns)
        CharacterDatabase.PExecute("INSERT INTO character_spell_cooldown VALUES(%u, %u, %lu, %u, %lu, %u)", guid, data.spellId, data.time, data.category, data.categoryTime, data.itemEntry);

    // stats - unused

    std::unordered_map<uint32, uint32> oldGuids; // TODO: convert this monster to item_instance without string
    for (uint32 i = 0; i < accountData.m_items.size(); ++i)
    {
        auto& data = accountData.m_items[i];
        uint32 itemGuid = sObjectMgr.GenerateItemLowGuid();
        oldGuids.emplace(data.guid, itemGuid);
        ItemPrototype const* proto = ObjectMgr::GetItemPrototype(data.entry);
        std::unique_ptr<Item> item(NewItemOrBag(proto));
        item->Create(itemGuid, data.entry, nullptr);
        item->SetUInt32Value(ITEM_FIELD_CREATOR, data.creatorGuid);
        item->SetUInt32Value(ITEM_FIELD_GIFTCREATOR, data.giftCreatorGuid);
        item->SetUInt32Value(ITEM_FIELD_STACK_COUNT, data.stackCount);
        item->SetUInt32Value(ITEM_FIELD_DURATION, data.duration);
        Tokens tokens = StrSplit(accountData.m_strings.itemStrings[i].charges, " ");
        if (tokens.size() == 5)
        {
            Tokens::iterator iter;
            uint32 index;
            for (iter = tokens.begin(), index = 0; index < 5; ++iter, ++index)
                item->SetUInt32Value(ITEM_FIELD_SPELL_CHARGES + index, std::stoul(*iter));
        }
        item->SetUInt32Value(ITEM_FIELD_FLAGS, data.flags);
        tokens = StrSplit(accountData.m_strings.itemStrings[i].enchantments, " ");
        if (tokens.size() == 33)
        {
            Tokens::iterator iter;
            uint32 index;
            for (iter = tokens.begin(), index = 0; index < 33; ++iter, ++index)
            {
                uint32 slot = index / MAX_ENCHANTMENT_OFFSET;
                uint32 offset = index % MAX_ENCHANTMENT_OFFSET;
                item->SetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + slot * MAX_ENCHANTMENT_OFFSET + offset, std::stoul(*iter));
            }
        }
        if (data.randomPropertyId)
        {
            for (uint32 i = PROP_ENCHANTMENT_SLOT_0; i <= PROP_ENCHANTMENT_SLOT_4; ++i)
            {
                item->SetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + i * MAX_ENCHANTMENT_OFFSET + 0, 0);
                item->SetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + i * MAX_ENCHANTMENT_OFFSET + 1, 0);
                item->SetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + i * MAX_ENCHANTMENT_OFFSET + 2, 0);
            }
        }
        item->SetItemRandomProperties(data.randomPropertyId);
        item->SetUInt32Value(ITEM_FIELD_ITEM_TEXT_ID, data.textId);
        item->SetUInt32Value(ITEM_FIELD_DURABILITY, data.durability);
        std::ostringstream totallyStupidObjectDataEmulation;
        for (uint16 i = 0; i < item->GetValuesCount(); ++i)
            totallyStupidObjectDataEmulation << item->GetUInt32Value(i) << " ";
        CharacterDatabase.PExecute("INSERT INTO item_instance(guid, owner_guid, data) VALUES(%u, %u, '%s')", itemGuid, guid, totallyStupidObjectDataEmulation.str().data());
    }

    for (auto& data : accountData.m_inventory)
        CharacterDatabase.PExecute("INSERT INTO character_inventory VALUES(%u, %u, %u, %u, %u)", guid, data.bag ? oldGuids[data.bag] : 0, data.slot, oldGuids[data.item], data.itemEntry);
}

