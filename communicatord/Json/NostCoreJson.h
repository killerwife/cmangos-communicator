#include "Platform/Define.h"
#include "../CommunicatorDefines.h"
#include "JsonReader.h"
#include "json.hpp"

namespace NostCoreJson
{
    using json = nlohmann::json;

    struct CharacterAction
    {
        uint32 action;
        uint8 button;
        uint32 guid;
        uint8 type;
        void SaveToDB(DatabaseType& database, uint32 guid);
    };

    void from_json(const json& inputData, CharacterAction& action);

    struct CharacterAura
    {
        uint32 guid;
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
        void SaveToDB(DatabaseType& database, uint32 guid);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, CharacterAura& aura);

    struct CharacterGifts
    {
        uint32 guid;
        uint32 itemGuid;
        uint32 entry;
        uint32 flags;
        void SaveToDB(DatabaseType& database, uint32 guid);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, CharacterGifts& gifts);

    struct CharacterHomebind
    {
        uint32 guid;
        uint32 map;
        uint32 zone;
        float posX;
        float posY;
        float posZ;
        void SaveToDB(DatabaseType& database, uint32 guid);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, CharacterHomebind& homebind);

    struct CharacterInventory
    {
        uint32 guid;
        uint32 bag;
        uint8 slot;
        uint32 item;
        uint32 itemEntry;
        void SaveToDB(DatabaseType& database, uint32 guid, uint32 itemGuid, uint32 bagGuid);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, CharacterInventory& inventory);

    struct CharacterMergeData
    {
        uint32 account;
        uint32 newGuid;
        uint32 realm;
        uint32 oldGuid;
        void SaveToDB(DatabaseType& database, uint32 guid);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, CharacterMergeData& inventory);

    struct CharacterPet
    {
        uint32 id; // requires reguid
        uint32 entry;
        uint32 owner;
        uint32 modelId;
        uint32 createdBySpell;
        uint8 petType;
        uint32 level;
        uint32 experience;
        uint8 reactState;
        int32 loyaltyPoints;
        uint32 loyalty;
        int32 trainPoint;
        std::string name;
        uint8 renamed;
        uint32 slot;
        uint32 curHealth;
        uint32 curMana;
        uint32 curHappiness;
        uint64 savetime;
        uint32 resetTalentCost;
        uint64 resetTalentTime;
        std::string abData;
        std::string teachSpellData;
        void SaveToDB(DatabaseType& database, uint32 petGuid, uint32 ownerGuid);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, CharacterPet& inventory);

    struct CharacterQueststatus
    {
        uint32 guid;
        uint32 quest;
        uint32 status;
        uint8 rewarded;
        uint8 explored;
        uint64 timer;
        uint32 mobCount[4];
        uint32 itemCount[4];
        uint32 rewardChoice; // not imported atm
        void SaveToDB(DatabaseType& database, uint32 guid);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, CharacterQueststatus& inventory);

    struct CharacterReputation
    {
        uint32 guid;
        uint32 faction;
        int32 standing;
        int32 flags;
        void SaveToDB(DatabaseType& database, uint32 guid);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, CharacterReputation& inventory);

    struct CharacterSkill
    {
        uint32 guid;
        uint32 skill;
        uint32 value;
        uint32 max;
        void SaveToDB(DatabaseType& database, uint32 guid);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, CharacterSkill& inventory);

    struct CharacterSpell
    {
        uint32 guid;
        uint32 spell;
        uint8 active;
        uint8 disabled;
        void SaveToDB(DatabaseType& database, uint32 guid);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, CharacterSpell& inventory);

    struct CharacterSpellCooldown
    {
        uint32 guid;
        uint32 spellId;
        uint32 item;
        uint64 time;
        uint64 categoryTime;
        void SaveToDB(DatabaseType& database, uint32 guid);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, CharacterSpellCooldown& inventory);

    struct CharacterStats
    {
        uint32 guid;
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
        uint32 attackPower;
        uint32 rangedAttackPower;
        void SaveToDB(DatabaseType& database, uint32 guid);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, CharacterStats& inventory);

    struct CharacterData
    {
        uint32 guid;
        uint32 account;
        std::string name;
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
        float ori;
        std::string taxiMask;
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
        std::string taxiPath; // TODO: Conversion
        float honorRankPoints;
        int32 honorHighestRank;
        int32 honorStanding;
        int32 honorLastWeekHK;
        int32 honorLastWeekCP;
        uint32 honorStoredHK;
        uint32 honorStoredDK;
        uint32 watchedFaction;
        uint32 drunk;
        uint32 health;
        uint32 power[5];
        std::string exploredZones;
        std::string equipmentCache;
        uint32 ammoId;
        uint8 actionBars;
        uint32 deleteInfosAccount;
        std::string deleteInfosName;
        uint64 deleteDate;
        uint32 area;
        int32 worldPhaseMask;
        int32 customFlags;
        void SaveToDB(DatabaseType& database, uint32 guid, uint32 accountId);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, CharacterData& inventory);

    struct ItemInstance
    {
        uint32 guid; // requires reguid
        uint32 entry;
        uint32 ownerGuid; // requires reguid
        uint32 creatorGuid;
        uint32 giftCreatorGuid;
        uint32 count;
        int32 duration;
        std::string charges;
        uint32 flags;
        std::string enchantments;
        int16 randomPropertyId;
        uint16 durability;
        uint32 text;
        int8 generatedLoot;
        void SaveToDB(DatabaseType& database, uint32 itemGuid, uint32 ownerGuid);
        void LoadFromDB(Field* fields);
    };

    void from_json(const json& inputData, ItemInstance& inventory);

    struct Character
    {
        std::vector<CharacterAction> actions;
        std::vector<CharacterAura> auras;
        std::vector<CharacterGifts> gifts;
        std::vector<CharacterHomebind> homebinds; // there can only be one...
        std::vector<CharacterInventory> inventory;
        std::vector<CharacterMergeData> mergeData;
        std::vector<CharacterPet> pets;
        std::vector<CharacterQueststatus> questStatus;
        std::vector<CharacterReputation> reputations;
        std::vector<CharacterSkill> skills;
        std::vector<CharacterSpell> spells;
        std::vector<CharacterSpellCooldown> cooldowns;
        std::vector<CharacterStats> stats;
        std::vector<CharacterData> character;
        std::vector<ItemInstance> items;
        void SaveToDB(DatabaseType& database, uint32 guid, uint32 accountId);
        void LoadFromDB(DatabaseType& database, uint32 guid, uint32 accountId);
    };

    void from_json(const json& inputData, Character& character);

    struct RealmData
    {
        std::string realmName;
        std::vector<Character> characters;
        void SaveToDB(DatabaseType& database, uint32& guid, uint32 accountId);
        void LoadFromDB(DatabaseType& database, uint32 guid, uint32 accountId);
    };

    void from_json(const json& inputData, RealmData& realm);

    struct Account : AccountPrototype
    {
        public:
            Account() : AccountPrototype(ACCOUNT_TYPE_NOST_CORE), accountId(0) {}

            virtual void SaveToDB(DatabaseType* database, uint32 accountId) override;
            virtual void LoadFromDB(DatabaseType* database, uint32 guid, MaNGOS::Account& account) override;
            virtual bool CheckDuplicity(DatabaseType* database) override;

            uint32 accountId;
            std::vector<RealmData> realmData;
    };

    void from_json(const json& inputData, Account*& account);
}