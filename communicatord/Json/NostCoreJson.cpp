#include "NostCoreJson.h"
#include "Util.h"
#include "../game/Entities/Item.h"

void NostCoreJson::from_json(const json& inputData, CharacterAction& action)
{
    action.action = inputData["action"].get<uint32>();
    action.button = inputData["button"].get<uint8>();
    action.guid = inputData["guid"].get<uint32>();
    action.type = inputData["type"].get<uint8>();
}

void NostCoreJson::CharacterAction::SaveToDB(DatabaseType& database, uint32 guid)
{
    database.PExecute("INSERT INTO character_action VALUES(%u, %u, %u, %u)", guid, button, action, type);
}

void NostCoreJson::from_json(const json& inputData, CharacterAura& aura) // TODO: Fix this being an array
{
    aura.guid = inputData["guid"].get<uint32>();
    aura.casterGuid = inputData["caster_guid"].get<uint64>();
    aura.itemGuid = inputData["item_guid"].get<uint32>();
    aura.spellId = inputData["spell"].get<uint32>();
    aura.stackCount = inputData["stackcount"].get<uint32>();
    aura.remainingCharges = inputData["remaincharges"].get<uint32>();
    aura.basePoints[0] = inputData["basepoints0"].get<int32>();
    aura.basePoints[1] = inputData["basepoints1"].get<int32>();
    aura.basePoints[2] = inputData["basepoints2"].get<int32>();
    aura.periodicTime[0] = inputData["periodictime0"].get<uint32>();
    aura.periodicTime[1] = inputData["periodictime1"].get<uint32>();
    aura.periodicTime[2] = inputData["periodictime2"].get<uint32>();
    aura.maxDuration = inputData["maxduration"].get<int32>();
    aura.remainingTime = inputData["remaintime"].get<int32>();
    aura.effectMask = inputData["effIndexMask"].get<uint32>();
}

void NostCoreJson::CharacterAura::SaveToDB(DatabaseType& database, uint32 guid)
{
    database.PExecute("INSERT INTO character_aura "
        "VALUES(%u, %lu, %u, %u, %u, %u, %d, %d, %d, %d, %u, %u, %d, %d, %u)", guid, casterGuid, itemGuid, spellId, stackCount,
        remainingCharges, basePoints[0], basePoints[1], basePoints[2], periodicTime[0], periodicTime[1], periodicTime[2], maxDuration, remainingTime, effectMask);
}

void NostCoreJson::CharacterAura::LoadFromDB(Field* fields)
{
}

void NostCoreJson::from_json(const json& inputData, CharacterGifts& gifts)
{
    gifts.guid = inputData["guid"].get<uint32>();
    gifts.itemGuid = inputData["item_guid"].get<uint32>();
    gifts.entry = inputData["entry"].get<uint32>();
    gifts.flags = inputData["flags"].get<uint32>();
}

void NostCoreJson::CharacterGifts::SaveToDB(DatabaseType& database, uint32 guid)
{
    database.PExecute("INSERT INTO character_gifts VALUES(%u, %u, %u, %u)", guid, itemGuid, entry, flags);
}

void NostCoreJson::CharacterGifts::LoadFromDB(Field* fields)
{
}

void NostCoreJson::from_json(const json& inputData, CharacterHomebind& homebind)
{
    homebind.guid = inputData["guid"].get<uint32>();
    homebind.map = inputData["map"].get<uint32>();
    homebind.zone = inputData["zone"].get<uint32>();
    homebind.posX = inputData["position_x"].get<float>();
    homebind.posY = inputData["position_y"].get<float>();
    homebind.posZ = inputData["position_z"].get<float>();
}

void NostCoreJson::CharacterHomebind::SaveToDB(DatabaseType& database, uint32 guid)
{
    database.PExecute("INSERT INTO character_homebind VALUES(%u, %u, %u, %f, %f, %f)", guid, map, zone, posX, posY, posZ);
}

void NostCoreJson::CharacterHomebind::LoadFromDB(Field* fields)
{
}

void NostCoreJson::from_json(const json& inputData, CharacterInventory& inventory)
{
    inventory.guid = inputData["guid"].get<uint32>();
    inventory.bag = inputData["bag"].get<uint32>();
    inventory.slot = inputData["slot"].get<uint8>();
    inventory.item = inputData["item"].get<uint32>();
    inventory.itemEntry = inputData["item_template"].get<uint32>();
}

void NostCoreJson::CharacterInventory::SaveToDB(DatabaseType& database, uint32 guid, uint32 itemGuid, uint32 bagGuid)
{
    database.PExecute("INSERT INTO character_inventory VALUES(%u, %u, %u, %u, %u)", guid, bagGuid, slot, itemGuid, itemEntry);
}

void NostCoreJson::CharacterInventory::LoadFromDB(Field* fields)
{
}

void NostCoreJson::from_json(const json& inputData, CharacterMergeData& inventory)
{
    inventory.account = inputData["account"].get<uint32>();
    inventory.newGuid = inputData["new_guid"].get<uint32>();
    inventory.oldGuid = inputData["old_guid"].get<uint32>();
    inventory.realm = inputData["realm"].get<uint32>();
}

void NostCoreJson::CharacterMergeData::SaveToDB(DatabaseType& database, uint32 guid)
{
    // Intentionally unused
}

void NostCoreJson::CharacterMergeData::LoadFromDB(Field* fields)
{
    // Intentionally unused
}

void NostCoreJson::from_json(const json& inputData, CharacterPet& inventory)
{
    inventory.id = inputData["id"].get<uint32>();
    inventory.entry = inputData["entry"].get<uint32>();
    inventory.owner = inputData["owner"].get<uint32>();
    inventory.modelId = inputData["modelid"].get<uint32>();
    inventory.createdBySpell = inputData["CreatedBySpell"].get<uint32>();
    inventory.petType = inputData["PetType"].get<uint8>();
    inventory.level = inputData["level"].get<uint32>();
    inventory.experience = inputData["exp"].get<uint32>();
    inventory.reactState = inputData["Reactstate"].get<uint8>();
    inventory.loyaltyPoints = inputData["loyaltypoints"].get<int32>();
    inventory.loyalty = inputData["loyalty"].get<uint32>();
    inventory.trainPoint = inputData["trainpoint"].get<int32>();
    inventory.name = inputData["name"].get<std::string>();
    inventory.renamed = inputData["renamed"].get<uint8>();
    inventory.slot = inputData["slot"].get<uint32>();
    inventory.curHealth = inputData["curhealth"].get<uint32>();
    inventory.curMana = inputData["curmana"].get<uint32>();
    inventory.curHappiness = inputData["curhappiness"].get<uint32>();
    inventory.savetime = inputData["savetime"].get<uint64>();
    inventory.resetTalentCost = inputData["resettalents_cost"].get<uint32>();
    inventory.resetTalentTime = inputData["resettalents_time"].get<uint64>();
    inventory.abData = inputData["abdata"].get<std::string>();
    inventory.teachSpellData = inputData["teachspelldata"].get<std::string>();
}

void NostCoreJson::CharacterPet::SaveToDB(DatabaseType& database, uint32 petGuid, uint32 ownerGuid)
{
    database.PExecute("INSERT INTO character_pet VALUES(%u, %u, %u, %u, %u, %u, %u, %u, %u, %d, %u, %d, \"%s\", %u, %u, %u, %u, %u, %lu, %u, %lu, '%s', '%s')",
        petGuid, entry, ownerGuid, modelId, createdBySpell, petType, level, experience, reactState, loyaltyPoints, loyalty, trainPoint, name.data(),
        renamed, slot, curHealth, curMana, curHappiness, savetime, resetTalentCost, resetTalentTime, abData.data(), teachSpellData.data());
}

void NostCoreJson::CharacterPet::LoadFromDB(Field* fields)
{
}

void NostCoreJson::from_json(const json& inputData, CharacterQueststatus& inventory)
{
    inventory.guid = inputData["guid"].get<uint32>();
    inventory.quest = inputData["quest"].get<uint32>();
    inventory.status = inputData["status"].get<uint32>();
    inventory.rewarded = inputData["rewarded"].get<uint8>();
    inventory.explored = inputData["explored"].get<uint8>();
    inventory.timer = inputData["timer"].get<uint64>();
    inventory.mobCount[0] = inputData["mobcount1"].get<uint32>();
    inventory.mobCount[1] = inputData["mobcount2"].get<uint32>();
    inventory.mobCount[2] = inputData["mobcount3"].get<uint32>();
    inventory.mobCount[3] = inputData["mobcount4"].get<uint32>();
    inventory.itemCount[0] = inputData["itemcount1"].get<uint32>();
    inventory.itemCount[1] = inputData["itemcount2"].get<uint32>();
    inventory.itemCount[2] = inputData["itemcount3"].get<uint32>();
    inventory.itemCount[3] = inputData["itemcount4"].get<uint32>();
    inventory.rewardChoice = inputData["reward_choice"].get<uint32>();
}

void NostCoreJson::CharacterQueststatus::SaveToDB(DatabaseType& database, uint32 guid)
{
    database.PExecute("INSERT INTO character_queststatus VALUES(%u, %u, %u, %u, %u, %lu, %u, %u, %u, %u, %u, %u, %u, %u, %u)",
        guid, quest, status, rewarded, explored, timer, mobCount[0], mobCount[1], mobCount[2], mobCount[3], itemCount[0], itemCount[1], itemCount[2], itemCount[3], rewardChoice);
}

void NostCoreJson::CharacterQueststatus::LoadFromDB(Field* fields)
{
}

void NostCoreJson::from_json(const json& inputData, CharacterReputation& inventory)
{
    inventory.guid = inputData["guid"].get<uint32>();
    inventory.faction = inputData["faction"].get<uint32>();
    inventory.standing = inputData["standing"].get<int32>();
    inventory.flags = inputData["flags"].get<int32>();
}

void NostCoreJson::CharacterReputation::SaveToDB(DatabaseType& database, uint32 guid)
{
    database.PExecute("INSERT INTO character_reputation VALUES(%u, %u, %d, %d)", guid, faction, standing, flags);
}

void NostCoreJson::CharacterReputation::LoadFromDB(Field* fields)
{
}

void NostCoreJson::from_json(const json& inputData, CharacterSkill& inventory)
{
    inventory.guid = inputData["guid"].get<uint32>();
    inventory.skill = inputData["skill"].get<uint32>();
    inventory.value = inputData["value"].get<uint32>();
    inventory.max = inputData["max"].get<uint32>();
}

void NostCoreJson::CharacterSkill::SaveToDB(DatabaseType& database, uint32 guid)
{
    database.PExecute("INSERT INTO character_skills VALUES(%u, %u, %u, %u)", guid, skill, value, max);
}

void NostCoreJson::CharacterSkill::LoadFromDB(Field* fields)
{
}

void NostCoreJson::from_json(const json& inputData, CharacterSpell& inventory)
{
    inventory.guid = inputData["guid"].get<uint32>();
    inventory.spell = inputData["spell"].get<uint32>();
    inventory.active = inputData["active"].get<uint8>();
    inventory.disabled = inputData["disabled"].get<uint8>();
}

void NostCoreJson::CharacterSpell::SaveToDB(DatabaseType& database, uint32 guid)
{
    database.PExecute("INSERT INTO character_spell VALUES(%u, %u, %u, %u)", guid, spell, active, disabled);
}

void NostCoreJson::CharacterSpell::LoadFromDB(Field* fields)
{
}

void NostCoreJson::from_json(const json& inputData, CharacterSpellCooldown& inventory)
{
    inventory.guid = inputData["guid"].get<uint32>();
    inventory.spellId = inputData["spell"].get<uint32>();
    inventory.item = inputData["item"].get<uint32>();
    inventory.time = inputData["time"].get<uint64>();
    inventory.categoryTime = inputData["catTime"].get<uint64>();
}

void NostCoreJson::CharacterSpellCooldown::SaveToDB(DatabaseType& database, uint32 guid)
{
    database.PExecute("INSERT INTO character_spell_cooldown VALUES(%u, %u, %u, %lu, %lu)", guid, spellId, item, time, categoryTime);
}

void NostCoreJson::CharacterSpellCooldown::LoadFromDB(Field* fields)
{
}

void NostCoreJson::from_json(const json& inputData, CharacterStats& inventory)
{
    inventory.guid = inputData["guid"].get<uint32>();
    inventory.maxHealth = inputData["maxhealth"].get<uint32>();
    for (uint32 i = 0; i < 7; ++i)
        inventory.maxPower[i] = inputData["maxpower" + std::to_string(i + 1)].get<uint32>();
    inventory.strength = inputData["strength"].get<uint32>();
    inventory.agility = inputData["agility"].get<uint32>();
    inventory.stamina = inputData["stamina"].get<uint32>();
    inventory.intellect = inputData["intellect"].get<uint32>();
    inventory.spirit = inputData["spirit"].get<uint32>();
    inventory.armor = inputData["armor"].get<uint32>();
    inventory.resistances[0] = inputData["resHoly"].get<uint32>();
    inventory.resistances[1] = inputData["resFire"].get<uint32>();
    inventory.resistances[2] = inputData["resNature"].get<uint32>();
    inventory.resistances[3] = inputData["resFrost"].get<uint32>();
    inventory.resistances[4] = inputData["resShadow"].get<uint32>();
    inventory.resistances[5] = inputData["resArcane"].get<uint32>();
    inventory.blockPct = inputData["blockPct"].get<float>();
    inventory.dodgePct = inputData["dodgePct"].get<float>();
    inventory.parryPct = inputData["parryPct"].get<float>();
    inventory.critPct = inputData["critPct"].get<float>();
    inventory.rangedCritPct = inputData["rangedCritPct"].get<float>();
    inventory.attackPower = inputData["attackPower"].get<uint32>();
    inventory.rangedAttackPower = inputData["rangedAttackPower"].get<uint32>();
}

void NostCoreJson::CharacterStats::SaveToDB(DatabaseType& database, uint32 guid)
{
    database.PExecute("INSERT INTO character_stats VALUES(%u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %f, %f, %f, %f, %f, %u, %u)",
        guid, maxHealth, maxPower[0], maxPower[1], maxPower[2], maxPower[3], maxPower[4], maxPower[5], maxPower[6], strength, agility, stamina, intellect, spirit, armor,
        resistances[0], resistances[1], resistances[2], resistances[3], resistances[4], resistances[5], blockPct, dodgePct, parryPct, critPct, rangedCritPct, attackPower, rangedAttackPower);
}

void NostCoreJson::CharacterStats::LoadFromDB(Field* fields)
{
}

void NostCoreJson::from_json(const json& inputData, CharacterData& inventory)
{
    inventory.guid = inputData["guid"].get<uint32>();
    inventory.account = inputData["account"].get<uint32>();

    inventory.name = inputData["name"].get<std::string>();

    inventory.race = inputData["race"].get<uint8>();
    inventory.playerClass = inputData["class"].get<uint8>();
    inventory.gender = inputData["gender"].get<uint8>();
    inventory.level = inputData["level"].get<uint8>();

    inventory.experience = inputData["xp"].get<uint32>();
    inventory.money = inputData["money"].get<uint32>();
    inventory.playerBytes = inputData["playerBytes"].get<uint32>();
    inventory.playerBytes2 = inputData["playerBytes2"].get<uint32>();
    inventory.playerFlags = inputData["playerFlags"].get<uint32>();

    inventory.posX = inputData["position_x"].get<float>();
    inventory.posY = inputData["position_y"].get<float>();
    inventory.posZ = inputData["position_z"].get<float>();
    inventory.map = inputData["map"].get<uint32>();
    inventory.ori = inputData["orientation"].get<float>();

    inventory.taxiMask = inputData["taximask"].get<std::string>();

    inventory.online = inputData["online"].get<uint8>();
    inventory.cinematic = inputData["cinematic"].get<uint8>();

    inventory.totalTime = inputData["totaltime"].get<uint32>();
    inventory.levelTime = inputData["leveltime"].get<uint32>();

    inventory.logoutTime = inputData["logout_time"].get<uint64>();

    inventory.isLogoutResting = inputData["is_logout_resting"].get<uint8>();

    inventory.restBonus = inputData["rest_bonus"].get<float>();
    inventory.resetTalentsMultiplier = inputData["resettalents_multiplier"].get<uint32>();
    inventory.resetTalentsTime = inputData["resettalents_time"].get<uint64>();

    inventory.transportX = inputData["trans_x"].get<float>();
    inventory.transportY = inputData["trans_y"].get<float>();
    inventory.transportZ = inputData["trans_z"].get<float>();
    inventory.transportOri = inputData["trans_o"].get<float>();

    inventory.transportGuid = inputData["transguid"].get<uint64>();
    inventory.extraFlags = inputData["extra_flags"].get<uint32>();
    inventory.stableSlots = inputData["stable_slots"].get<uint8>();

    inventory.atLogin = inputData["at_login"].get<uint32>();
    inventory.zone = inputData["zone"].get<uint32>();

    inventory.deathExpireTime = inputData["death_expire_time"].get<uint64>();

    inventory.taxiPath = inputData["taxi_path"].get<std::string>();
    inventory.honorRankPoints = inputData["honorRankPoints"].get<float>();

    inventory.honorHighestRank = inputData["honorHighestRank"].get<int32>();
    inventory.honorStanding = inputData["honorStanding"].get<int32>();
    inventory.honorLastWeekHK = inputData["honorLastWeekHK"].get<int32>();
    inventory.honorLastWeekCP = inputData["honorLastWeekCP"].get<int32>();

    inventory.honorStoredHK = inputData["honorStoredHK"].get<uint32>();
    inventory.honorStoredDK = inputData["honorStoredDK"].get<uint32>();
    inventory.watchedFaction = inputData["watchedFaction"].get<uint32>();
    inventory.drunk = inputData["drunk"].get<uint32>();
    inventory.health = inputData["health"].get<uint32>();

    for (uint32 i = 0; i < 5; ++i)
        inventory.power[i] = inputData["power" + std::to_string(i + 1)].get<uint32>();

    inventory.exploredZones = inputData["exploredZones"].get<std::string>();
    inventory.equipmentCache = inputData["equipmentCache"].get<std::string>();

    inventory.ammoId = inputData["ammoId"].get<uint32>();
    inventory.actionBars = inputData["actionBars"].get<uint8>();
    inventory.deleteInfosAccount = 0;

    inventory.deleteInfosName = "";
    inventory.deleteDate = 0;
    inventory.area = inputData["area"].get<uint32>();

    inventory.worldPhaseMask = 0;
    inventory.customFlags = inputData["customFlags"].get<int32>();
}

void NostCoreJson::CharacterData::SaveToDB(DatabaseType& database, uint32 guid, uint32 accountId)
{
    uint32 inserted = 0;
    if (level < 15)
        inserted = 3; // unavailable for insert
    database.PExecute("INSERT INTO characters VALUES(%u, %u, \"%s\", %u, %u, %u, %u, %u, %u, %u, %u, %u, %f, %f, %f, %u, %f, '%s', %u, %u, %u, %u, %lu, %u, %f, %u, %lu, %f, %f, %f, %f, %lu, %u, %u, %u, %u, %lu, '%s', %f, %d, %d,"
        " %d, %d, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, '%s', '%s', %u, %u, %u, '%s', %lu, %u, %d, %d, %u, NULL)",
        guid, accountId, name.data(), race, playerClass, gender, level, experience, money, playerBytes, playerBytes2, playerFlags, posX, posY, posZ, map, ori, taxiMask.data(), online, cinematic, totalTime, levelTime,
        logoutTime, isLogoutResting, restBonus, resetTalentsMultiplier, resetTalentsTime, transportX, transportY, transportZ, transportOri, transportGuid, extraFlags, stableSlots, atLogin, zone, deathExpireTime,
        taxiPath.data(), honorRankPoints, honorHighestRank, honorStanding, honorLastWeekHK, honorLastWeekCP, honorStoredHK, honorStoredDK, watchedFaction, drunk, health, power[0], power[1], power[2],
        power[3], power[4], exploredZones.data(), equipmentCache.data(), ammoId, actionBars, deleteInfosAccount, deleteInfosName.data(), deleteDate, area, worldPhaseMask, customFlags, inserted);
}

void NostCoreJson::CharacterData::LoadFromDB(Field* fields)
{
}

void NostCoreJson::from_json(const json& inputData, ItemInstance& inventory)
{
    inventory.guid = inputData["guid"].get<uint32>();
    inventory.entry = inputData["itemEntry"].get<uint32>();
    inventory.ownerGuid = inputData["owner_guid"].get<uint32>();
    inventory.creatorGuid = 0;
    inventory.giftCreatorGuid = inputData["giftCreatorGuid"].get<uint32>(); // unused due to us not having all data
    inventory.count = inputData["count"].get<uint32>();
    inventory.duration = inputData["duration"].get<int32>();
    inventory.charges = inputData["charges"].get<std::string>();
    inventory.flags = inputData["flags"].get<uint32>();
    inventory.enchantments = inputData["enchantments"].get<std::string>();
    inventory.randomPropertyId = inputData["randomPropertyId"].get<int16>();
    inventory.durability = inputData["durability"].get<uint16>();
    inventory.text = inputData["text"].get<uint32>();
    inventory.generatedLoot = inputData["generated_loot"].get<int8>();
}

void NostCoreJson::ItemInstance::SaveToDB(DatabaseType& database, uint32 itemGuid, uint32 ownerGuid)
{
    database.PExecute("INSERT INTO item_instance VALUES(%u, %u, %u, %u, %u, %u, %d, '%s', %u, '%s', %d, %u, %u, %d)",
        itemGuid, entry, ownerGuid, 0, 0, count, duration, charges.data(), flags, enchantments.data(), randomPropertyId, durability, text, generatedLoot);
}

void NostCoreJson::ItemInstance::LoadFromDB(Field* fields)
{
}

void NostCoreJson::from_json(const json& inputData, Character& character)
{
    for (auto& data : inputData["character_action"])
        character.actions.push_back(data.get<CharacterAction>());
    for (auto& data : inputData["character_aura"])
        character.auras.push_back(data.get<CharacterAura>());
    for (auto& data : inputData["character_gifts"])
        character.gifts.push_back(data.get<CharacterGifts>());
    for (auto& data : inputData["character_homebind"])
        character.homebinds.push_back(data.get<CharacterHomebind>());
    for (auto& data : inputData["character_inventory"])
        character.inventory.push_back(data.get<CharacterInventory>());
    if (inputData.contains("character_merge_data"))
        for (auto& data : inputData["character_merge_data"])
            character.mergeData.push_back(data.get<CharacterMergeData>());
    for (auto& data : inputData["character_pet"])
        character.pets.push_back(data.get<CharacterPet>());
    for (auto& data : inputData["character_queststatus"])
        character.questStatus.push_back(data.get<CharacterQueststatus>());
    for (auto& data : inputData["character_reputation"])
        character.reputations.push_back(data.get<CharacterReputation>());
    for (auto& data : inputData["character_skills"])
        character.skills.push_back(data.get<CharacterSkill>());
    for (auto& data : inputData["character_spell"])
        character.spells.push_back(data.get<CharacterSpell>());
    for (auto& data : inputData["character_spell_cooldown"])
        character.cooldowns.push_back(data.get<CharacterSpellCooldown>());
    for (auto& data : inputData["character_stats"])
        character.stats.push_back(data.get<CharacterStats>());
    for (auto& data : inputData["characters"])
        character.character.push_back(data.get<CharacterData>());
    for (auto& data : inputData["item_instance"])
        character.items.push_back(data.get<ItemInstance>());
}

void NostCoreJson::Character::SaveToDB(DatabaseType& database, uint32 guid, uint32 accountId)
{
    for (auto& data : actions)
        data.SaveToDB(database, guid);
    for (auto& data : auras)
        data.SaveToDB(database, guid);
    for (auto& data : gifts)
        data.SaveToDB(database, guid);
    for (auto& data : homebinds)
        data.SaveToDB(database, guid);
    // inventory done later
    for (auto& data : mergeData)
        data.SaveToDB(database, guid);
    {
        uint32 petGuid = 0;
        std::unique_ptr<QueryResult> result(database.PQuery("SELECT MAX(id) FROM character_pet"));
        if (result)
            petGuid = result->Fetch()[0].GetUInt32() + 1;
        for (auto& data : pets)
            data.SaveToDB(database, petGuid++, guid);
    }
    for (auto& data : questStatus)
        data.SaveToDB(database, guid);
    for (auto& data : reputations)
        data.SaveToDB(database, guid);
    for (auto& data : skills)
        data.SaveToDB(database, guid);
    for (auto& data : spells)
        data.SaveToDB(database, guid);
    for (auto& data : cooldowns)
        data.SaveToDB(database, guid);
    for (auto& data : stats)
        data.SaveToDB(database, guid);
    for (auto& data : character)
        data.SaveToDB(database, guid, accountId);
    {
        uint32 itemGuid = 0;
        std::unique_ptr<QueryResult> result(database.PQuery("SELECT MAX(guid) FROM item_instance"));
        if (result)
            itemGuid = result->Fetch()[0].GetUInt32() + 1;
        std::unordered_map<uint32, uint32> oldGuids;
        for (auto& data : items)
        {
            oldGuids.emplace(data.guid, itemGuid);
            data.SaveToDB(database, itemGuid++, guid);
        }
        for (auto& data : inventory)
            data.SaveToDB(database, guid, oldGuids[data.item], data.bag ? oldGuids[data.bag] : 0);
    }
}

void NostCoreJson::Character::LoadFromDB(DatabaseType& database, uint32 guid, uint32 accountId)
{

}

void NostCoreJson::from_json(const json& inputData, RealmData& realm)
{
    auto& data = inputData["realm"];
    for (auto& charData : data["characters"])
        realm.characters.push_back(charData["character"].get<Character>());
    realm.realmName = data["name"].get<std::string>();
}

void NostCoreJson::RealmData::SaveToDB(DatabaseType& database, uint32& guid, uint32 accountId)
{
    for (auto& charData : characters)
    {
        database.PExecute("INSERT INTO character_realm VALUES('%u','%s')", guid, realmName.data());
        charData.SaveToDB(database, guid++, accountId);
    }
}

void NostCoreJson::RealmData::LoadFromDB(DatabaseType& database, uint32 guid, uint32 accountId)
{

}

void NostCoreJson::from_json(const json& inputData, Account*& account)
{
    account = new Account();
    account->accountId = inputData["id"].get<uint32>();
    for (auto& data : inputData["realms"])
        account->realmData.push_back(data.get<RealmData>());
}

void NostCoreJson::Account::SaveToDB(DatabaseType* database, uint32 accountId)
{
    if (!database)
    {
        sLog.outError("Nost Account Failed due to nonexistant database.");
        return;
    }
    uint32 guid = 0;
    {
        std::unique_ptr<QueryResult> result(database->PQuery("SELECT MAX(guid) FROM characters"));
        if (result)
            guid = result->Fetch()[0].GetUInt32() + 1;
    }
    for (auto& data : realmData)
        data.SaveToDB(*database, guid, accountId ? accountId : this->accountId);
}

constexpr uint32 racemaskAlliance =
((1 << (1 - 1)) | (1 << (3 - 1)) | (1 << (4 - 1)) |
(1 << (7 - 1)) | (1 << (11 - 1)));

void NostCoreJson::Account::LoadFromDB(DatabaseType* database, uint32 guid, MaNGOS::Account& account)
{
    std::unique_ptr<QueryResult> result(database->PQuery("SELECT button,action,type FROM character_action WHERE guid=%u", guid));
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            account.m_actions.push_back({ fields[0].GetUInt8(), fields[1].GetUInt32(), fields[2].GetUInt8() });
        } while (result->NextRow());
    }
    result.reset(database->PQuery("SELECT caster_guid,item_guid,spell,stackcount,remaincharges,basepoints0,basepoints1,basepoints2,periodictime0,periodictime1,periodictime2,maxduration,remaintime,effIndexMask FROM character_aura WHERE guid=%u", guid));
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            account.m_auras.push_back({ fields[0].GetUInt64(), fields[1].GetUInt32(), fields[2].GetUInt32(), fields[3].GetUInt32(), fields[4].GetUInt32(), fields[5].GetInt32(), fields[6].GetInt32(), fields[7].GetInt32(),
                fields[8].GetUInt32(), fields[9].GetUInt32(), fields[10].GetUInt32(), fields[11].GetInt32(), fields[12].GetInt32(), fields[13].GetUInt32() });
        } while (result->NextRow());
    }
    result.reset(database->PQuery("SELECT item_guid,entry,flags FROM character_gifts WHERE guid=%u", guid));
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            account.m_gifts.push_back({ fields[0].GetUInt32(), fields[1].GetUInt32(), fields[2].GetUInt32() });
        } while (result->NextRow());
    }
    result.reset(database->PQuery("SELECT map,zone,position_x,position_y,position_z FROM character_homebind WHERE guid=%u", guid));
    if (result)
    {
        Field* fields = result->Fetch();
        account.m_homebind = { fields[0].GetUInt32(), fields[1].GetUInt32(), fields[2].GetFloat(), fields[3].GetFloat(), fields[4].GetFloat() };
    }
    result.reset(database->PQuery("SELECT bag,slot,item,item_template FROM character_inventory WHERE guid=%u", guid));
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint8 slot = fields[1].GetUInt8();
            if (slot >= 63 && slot < 69) // recalculate bank slots
                slot = slot - 63 + 67;
            if (slot >= 81 && slot < 97) // recalculate keychain slots
                slot = slot - 81 + 86;
            account.m_inventory.push_back({ fields[0].GetUInt32(), slot, fields[2].GetUInt32(), fields[3].GetUInt32() });
        } while (result->NextRow());
    }
    result.reset(database->PQuery("SELECT id,entry,modelid,CreatedBySpell,PetType,level,exp,Reactstate,loyaltypoints,loyalty,trainpoint,name,renamed,slot,curhealth,curmana,curhappiness,savetime,resettalents_cost,resettalents_time,abdata,teachspelldata FROM character_pet WHERE owner=%u", guid));
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 level = fields[5].GetUInt32();
            int32 trainPoint = fields[10].GetInt32();
            trainPoint = level * 5;
            account.m_pets.push_back({ fields[0].GetUInt32(), fields[1].GetUInt32(), fields[2].GetUInt32(), fields[3].GetUInt32(), fields[4].GetUInt8(),
                level, fields[6].GetUInt32(), fields[7].GetUInt8(), fields[8].GetInt32(), fields[9].GetUInt32(), 6u, trainPoint, fields[12].GetUInt8(),
                fields[13].GetUInt32(), fields[14].GetUInt32(), fields[15].GetUInt32(), fields[16].GetUInt32(), fields[17].GetUInt64(),
                fields[18].GetUInt32(), fields[19].GetUInt64() });
            account.m_strings.petStrings.push_back({ fields[0].GetUInt32(), fields[11].GetCppString(), fields[20].GetCppString(), fields[21].GetCppString() });
        } while (result->NextRow());
    }
    result.reset(database->PQuery("SELECT quest,status,rewarded,explored,timer,mobcount1,mobcount2,mobcount3,mobcount4,itemcount1,itemcount2,itemcount3,itemcount4,reward_choice FROM character_queststatus WHERE guid=%u", guid));
    if (result)
    {
        do
        {
            Field* fields = result->Fetch(); // reward_choice unused
            account.m_quests.push_back({ fields[0].GetUInt32(), fields[1].GetUInt32(), fields[2].GetUInt8(), fields[3].GetUInt8(), fields[4].GetUInt64(), fields[5].GetUInt32(), fields[6].GetUInt32(),
                fields[7].GetUInt32(), fields[8].GetUInt32(), fields[9].GetUInt32(), fields[10].GetUInt32(), fields[11].GetUInt32(), fields[12].GetUInt32() });
        } while (result->NextRow());
    }
    {
        bool found = false;
        bool properFound = false;
        const std::set<uint32> naxxAttunement = { 9121,9122,9123 };
        for (auto& data : account.m_quests)
        {
            if (naxxAttunement.find(data.quest) != naxxAttunement.end())
                found = true;
            else if (data.quest == 9378)
                properFound = true;
        }
        // readd proper naxx attunement flag quest
        if (found && !properFound)
        {
            MaNGOS::CharacterQueststatus status;
            status.quest = 9378;
            status.status = 1;
            status.rewarded = 1;
            account.m_quests.push_back(status);
        }
    }
    result.reset(database->PQuery("SELECT faction,standing,flags FROM character_reputation WHERE guid=%u", guid));
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            account.m_reputation.push_back({ fields[0].GetUInt32(), fields[1].GetInt32(), fields[2].GetInt32() });
        } while (result->NextRow());
    }
    result.reset(database->PQuery("SELECT spell,active,disabled FROM character_spell WHERE guid=%u", guid));
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            account.m_spells.push_back({ fields[0].GetUInt32(), fields[1].GetUInt8(), fields[2].GetUInt8() });
        } while (result->NextRow());
    }
    result.reset(database->PQuery("SELECT spell,item,time,catTime FROM character_spell_cooldown WHERE guid=%u", guid));
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            //                                                                          cat
            account.m_cooldowns.push_back({ fields[0].GetUInt32(), fields[2].GetUInt64(), 0, fields[3].GetUInt64(), fields[1].GetUInt32() });
        } while (result->NextRow());
    }
    result.reset(database->PQuery("SELECT maxhealth,maxpower1,maxpower2,maxpower3,maxpower4,maxpower5,maxpower6,maxpower7,strength,agility,stamina,intellect,spirit,armor,resHoly,resFire,resNature,resFrost,resShadow,resArcane,blockPct,dodgePct,parryPct,critPct,rangedCritPct,attackPower,rangedAttackPower FROM character_stats WHERE guid=%u", guid));
    if (result)
    {
        Field* fields = result->Fetch();
        account.m_stats = { fields[0].GetUInt32(), fields[1].GetUInt32(), fields[2].GetUInt32() , fields[3].GetUInt32() , fields[4].GetUInt32() , fields[5].GetUInt32() , fields[6].GetUInt32(),
        fields[7].GetUInt32(), fields[8].GetUInt32(), fields[9].GetUInt32(), fields[10].GetUInt32(), fields[11].GetUInt32(), fields[12].GetUInt32(), fields[13].GetUInt32(), fields[14].GetUInt32() ,
        fields[15].GetUInt32(), fields[16].GetUInt32(), fields[17].GetUInt32(), fields[18].GetUInt32(), fields[19].GetUInt32(), fields[20].GetFloat(), fields[21].GetFloat(), fields[22].GetFloat(),
        fields[23].GetFloat(), fields[24].GetFloat(), 0.f, fields[25].GetUInt32(), fields[26].GetUInt32() };
    }
    //                                          0    1     2      3     4  5     6           7            8           9         10         11         12  13          14        15        16          17                18         19                      20                21      22      23      24      25        26          27           28       29   30                31              32               33            34              35              36            37            38             39    40     41     42     43     44     45     46     47         48   49       50        51            52             53                     
    result.reset(database->PQuery("SELECT account,race,class,gender,level,xp,money,playerBytes,playerBytes2,playerFlags,position_x,position_y,position_z,map,orientation,totaltime,leveltime,logout_time,is_logout_resting,rest_bonus,resettalents_multiplier,resettalents_time,trans_x,trans_y,trans_z,trans_o,transguid,extra_flags,stable_slots,at_login,zone,death_expire_time,honorRankPoints,honorHighestRank,honorStanding,honorLastWeekHK,honorLastWeekCP,honorStoredHK,honorStoredDK,watchedFaction,drunk,health,power1,power2,power3,power4,power5,ammoId,actionBars,name,taximask,taxi_path,exploredZones,equipmentCache FROM characters WHERE guid=%u", guid));
    if (result)
    {
        Field* fields = result->Fetch();
        MaNGOS::CharacterData& data = account.m_data;
        data.oldGuid = guid;

        data.account = fields[0].GetUInt32();
        data.race = fields[1].GetUInt8();
        data.playerClass = fields[2].GetUInt8();
        data.gender = fields[3].GetUInt8();
        data.level = fields[4].GetUInt8();
        data.experience = fields[5].GetUInt32();
        data.money = fields[6].GetUInt32();
        data.playerBytes = fields[7].GetUInt32();
        data.playerBytes2 = fields[8].GetUInt32();
        data.playerFlags = fields[9].GetUInt32();

        data.posX = fields[10].GetFloat();
        data.posY = fields[11].GetFloat();
        data.posZ = fields[12].GetFloat();
        data.map = fields[13].GetUInt32();
        data.dungeonDifficulty = 0;
        data.ori = fields[14].GetFloat();
        data.online = 0;
        data.cinematic = 1;
        data.totalTime = fields[15].GetUInt32();
        data.levelTime = fields[16].GetUInt32();

        data.logoutTime = fields[17].GetUInt64();
        data.isLogoutResting = fields[18].GetUInt8();
        data.restBonus = fields[19].GetFloat();
        data.resetTalentsMultiplier = fields[20].GetUInt32();
        data.resetTalentsTime = fields[21].GetUInt64();
        data.transportX = fields[22].GetFloat();
        data.transportY = fields[23].GetFloat();
        data.transportZ = fields[24].GetFloat();
        data.transportOri = fields[25].GetFloat();
        data.transportGuid = fields[26].GetUInt64();

        data.extraFlags = fields[27].GetUInt32();
        data.stableSlots = fields[28].GetUInt8();
        data.atLogin = fields[29].GetUInt32();
        data.zone = fields[30].GetUInt32();
        data.deathExpireTime = fields[31].GetUInt64();
        data.arenaPoints = 0;
        data.totalHonor = 0;
        data.todayHonor = 0;
        data.yesterdayHonor = 0;
        data.totalKills = fields[37].GetUInt32();

        data.todayKills = 0;
        data.yesterdayKills = 0;
        data.chosenTitle = 0;
        data.watchedFaction = fields[39].GetUInt32();
        data.drunk = fields[40].GetUInt32();
        data.health = fields[41].GetUInt32();
        data.power[0] = fields[42].GetUInt32();
        data.power[1] = fields[43].GetUInt32();
        data.power[2] = fields[44].GetUInt32();
        data.power[3] = fields[45].GetUInt32();
        data.power[4] = fields[46].GetUInt32();

        data.ammoId = fields[47].GetUInt32();
        data.actionBars = fields[48].GetUInt8();

        account.m_strings.name = fields[49].GetCppString();
        account.m_strings.taxiMask = fields[50].GetCppString();
        for (uint32 i = 0; i < 8; ++i) // add fields added in tbc
            account.m_strings.taxiMask += " 0";
        try
        {
            account.m_strings.taxiPath = std::to_string(std::stoi(fields[51].GetCppString()));
        }
        catch (std::exception&)
        {
            account.m_strings.taxiPath = "";
        }
        account.m_strings.exploredZones = fields[52].GetCppString();
        for (uint32 i = 0; i < 64; ++i) // add fields added in tbc
            account.m_strings.exploredZones += " 0";
        account.m_strings.equipmentCache = fields[53].GetCppString();
        uint32 maxRank = fields[33].GetUInt32();
        if (maxRank != 0)
        {
            if (maxRank < 5)
                maxRank = 0; // tbc doesnt support negative ranks
            else
                maxRank -= 4;
        }
        uint32 titleMask = 0;
        if (maxRank)
            titleMask = (1 << (maxRank + (((1 << data.race) & racemaskAlliance) == 0 ? 14 : 0)));

        bool scarabLord = false;
        std::unique_ptr<QueryResult> scarabResult(database->PQuery("SELECT guid FROM item_instance WHERE owner_guid=%u AND itemEntry=21176", guid));
        if (scarabResult)
            scarabLord = true;
        account.m_strings.knownTitles = std::to_string(titleMask) + (scarabLord ? " 2" : " 0");
    }
    result.reset(database->PQuery("SELECT skill,value,max FROM character_skills WHERE guid=%u", guid));
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 skillId = fields[0].GetUInt32();
            uint32 value = fields[1].GetUInt32();
            uint32 max = fields[2].GetUInt32();
            const std::set<uint32> ridingSkills = { 148,149,150,152,533,554,553,713 };
            if (ridingSkills.find(skillId) != ridingSkills.end()) // fix vanilla riding skills to use generic one
            {
                skillId = 762;
                value = max;
                uint32 spellId;
                if (value == 75)
                    spellId = 33388;
                else
                    spellId = 33391;
                account.m_spells.push_back({ spellId, 1, 0 });
            }
            const std::set<uint32> weaponSkills = { 43, 44, 45, 46, 54, 55, 160, 172 };
            if (weaponSkills.find(skillId) != weaponSkills.end())
            {
                value = account.m_data.level * 5;
            }
            account.m_skills.push_back({ skillId, value, max });
        } while (result->NextRow());
    }
    result.reset(database->PQuery("SELECT guid,itemEntry,owner_guid,creatorGuid,giftCreatorGuid,count,duration,charges,flags,enchantments,randomPropertyId,durability,text FROM item_instance WHERE owner_guid=%u", guid));
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            std::map<uint32, uint32> enchantMap =
            {
                {0, PERM_ENCHANTMENT_SLOT},
                {1, TEMP_ENCHANTMENT_SLOT},
                {2, BONUS_ENCHANTMENT_SLOT},
                {3, PROP_ENCHANTMENT_SLOT_0},
                {4, PROP_ENCHANTMENT_SLOT_1},
                {5, PROP_ENCHANTMENT_SLOT_2},
                {6, PROP_ENCHANTMENT_SLOT_3},
            };
            std::string enchantments;
            std::vector<uint32> enchValues(33, 0);
            Tokens tokens = StrSplit(fields[9].GetCppString(), " ");
            if (tokens.size() == 21)
            {
                Tokens::iterator iter;
                uint32 index;
                for (iter = tokens.begin(), index = 0; index < 21; ++iter, ++index)
                {
                    uint32 slot = index / MAX_ENCHANTMENT_OFFSET;
                    uint32 offset = index % MAX_ENCHANTMENT_OFFSET;
                    enchValues[enchantMap[slot] * MAX_ENCHANTMENT_OFFSET + offset] = std::stoul(*iter);
                }
            }
            for (uint32 value : enchValues)
                enchantments += std::to_string(value) + " ";
            account.m_items.push_back({ fields[0].GetUInt32(), fields[1].GetUInt32(), fields[2].GetUInt32(), fields[3].GetUInt32(), fields[4].GetUInt32(), fields[5].GetUInt32(), fields[6].GetInt32(),
                fields[8].GetUInt32(), 0, fields[10].GetInt16(), fields[11].GetUInt16(), fields[12].GetUInt32() });
            account.m_strings.itemStrings.push_back({ fields[0].GetUInt32(), fields[7].GetCppString(), enchantments });
        } while (result->NextRow());
    }
}

bool NostCoreJson::Account::CheckDuplicity(DatabaseType* database)
{
    for (auto& data : this->realmData)
    {
        for (auto& innerData : data.characters)
        {
            std::unique_ptr<QueryResult> result(database->PQuery("SELECT inserted FROM characters a JOIN character_realm b ON(a.guid=b.guid) WHERE name='%s' AND realmname='%s'",
                innerData.character[0].name.data(), data.realmName.data()));
            if (result)
                return false;
        }
    }
    return true;
}
