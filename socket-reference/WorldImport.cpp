void World::ImportPlayer(MaNGOS::Account* playerData)
{
    Player::Validate(*playerData);
    Player::SaveToDB(sObjectMgr.GeneratePlayerLowGuid(), *playerData);
    delete playerData;
    UpdateRealmCharCount(playerData->m_data.account);
}