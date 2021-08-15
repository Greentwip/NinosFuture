#ifndef __WINDY_GAME_MANAGER__
#define __WINDY_GAME_MANAGER__

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Windy/SaveManager.h"

#include "Game/Entities/Items/ItemFlags.h"

namespace game {
    class GameManager;
    class UnlockablesData;
    class BrownersData;
    class LevelsData;

    class UnlockableData {
    public:
        int id;
        bool acquired;
        std::string name;

    public:
        UnlockableData(int id, bool acquired, std::string name) {
            this->id = id;
            this->acquired = acquired;
            this->name = name;
        }

        UnlockableData(UnlockableData const& other) {
            this->id = other.id;
            this->acquired = other.acquired;
            this->name = other.name;
        }

        friend class GameManager;
        friend class UnlockablesData;
    };

    class UnlockablesData {
    public:
        std::shared_ptr<UnlockableData> helmet;
        std::shared_ptr<UnlockableData> head;
        std::shared_ptr<UnlockableData> chest;
        std::shared_ptr<UnlockableData> fist;
        std::shared_ptr<UnlockableData> boot;

        std::vector<std::shared_ptr<UnlockableData>> collection;


        bool helmetAcquired() {
            return helmet->acquired;
        }

        bool extremeAcquired() {
            return helmet->acquired && head->acquired && chest->acquired && fist->acquired && boot->acquired;
        }

    private:
        UnlockablesData() :
            helmet(std::make_shared<UnlockableData>(ItemId::Helmet, false, "helmet")),
            head(std::make_shared<UnlockableData>(ItemId::Head, false, "head")),
            chest(std::make_shared<UnlockableData>(ItemId::Chest, false, "chest")),
            fist(std::make_shared<UnlockableData>(ItemId::Fist, false, "fist")),
            boot(std::make_shared<UnlockableData>(ItemId::Boot, false, "boot")) {

            collection.push_back(helmet);
            collection.push_back(head);
            collection.push_back(chest);
            collection.push_back(fist);
            collection.push_back(boot);

        }

        friend class GameManager;

    };


    class BrownerData {
    public:
        int id;
        bool acquired;
        std::string pauseItem;

        std::string level;
        int energy;

    public:
        BrownerData(int id, bool acquired, std::string pauseItem, std::string level = "", int energy = -1) {
            this->id = id;
            this->acquired = acquired;
            this->pauseItem = pauseItem;
            this->level = level;
            this->energy = energy;
        }

        BrownerData(BrownerData const& other) {
            this->id = other.id;
            this->acquired = other.acquired;
            this->pauseItem = other.pauseItem;
            this->level = other.level;
            this->energy = other.energy;
        }

    };

    class BrownersData {
    public:
        std::shared_ptr<BrownerData> teleport;
        std::shared_ptr<BrownerData> violet;
        std::shared_ptr<BrownerData> fuzzy;
        std::shared_ptr<BrownerData> freezer;
        std::shared_ptr<BrownerData> sheriff;
        std::shared_ptr<BrownerData> boomer;
        std::shared_ptr<BrownerData> military;
        std::shared_ptr<BrownerData> vine;
        std::shared_ptr<BrownerData> shield;
        std::shared_ptr<BrownerData> night;
        std::shared_ptr<BrownerData> torch;
        std::shared_ptr<BrownerData> helmet;
        std::shared_ptr<BrownerData> extreme;
        std::shared_ptr<BrownerData> boss;
        std::shared_ptr<BrownerData> subBoss;
        std::shared_ptr<BrownerData> newNightManBoss;

        std::vector<std::shared_ptr<BrownerData>> collection;

    private:
        BrownersData() :
            teleport(std::make_shared<BrownerData>(1, true, "")),
            violet(std::make_shared<BrownerData>(2, true, "violet")),
            fuzzy(std::make_shared<BrownerData>(3, false, "fuzzy", "", 28)),
            freezer(std::make_shared<BrownerData>(4, false, "freezer", "freezerman", 28)),
            sheriff(std::make_shared<BrownerData>(5, true, "sheriff", "sheriffman", 28)),
            boomer(std::make_shared<BrownerData>(6, false, "boomer", "boomerman", 28)),
            military(std::make_shared<BrownerData>(7, false, "military", "militaryman", 28)),
            vine(std::make_shared<BrownerData>(8, false, "vine", "vineman", 28)),
            shield(std::make_shared<BrownerData>(9, false, "shield", "shieldman", 28)),
            night(std::make_shared<BrownerData>(10, false, "night", "nightman", 28)),
            torch(std::make_shared<BrownerData>(11, false, "torch", "torchman", 28)),
            helmet(std::make_shared<BrownerData>(12, true, "helmet")),
            extreme(std::make_shared<BrownerData>(13, false, "ex", "", 28)),
            boss(std::make_shared<BrownerData>(14, false, "")),
            subBoss(std::make_shared<BrownerData>(15, false, "")),
            newNightManBoss(std::make_shared<BrownerData>(16, false, "")) {

            collection.push_back(teleport);
            collection.push_back(violet);
            collection.push_back(fuzzy);
            collection.push_back(freezer);
            collection.push_back(sheriff);
            collection.push_back(boomer);
            collection.push_back(military);
            collection.push_back(vine);
            collection.push_back(shield);
            collection.push_back(night);
            collection.push_back(torch);
            collection.push_back(helmet);
            collection.push_back(extreme);
            collection.push_back(subBoss);
            collection.push_back(newNightManBoss);
        }

        friend class GameManager;
    };

    class LevelData {
    public:
        bool defeated;
        std::string mug;

    private:
        LevelData(bool defeated, std::string mug) {
            this->defeated = defeated;
            this->mug = mug;
        }

        LevelData(LevelData const& other) {
            this->defeated = other.defeated;
            this->mug = other.mug;
        }

        friend class GameManager;
        friend class LevelsData;
    };

    class LevelsData {
    public:
        std::shared_ptr<LevelData> freezer;
        std::shared_ptr<LevelData> sheriff;
        std::shared_ptr<LevelData> boomer;
        std::shared_ptr<LevelData> military;
        std::shared_ptr<LevelData> vine;
        std::shared_ptr<LevelData> shield;
        std::shared_ptr<LevelData> night;
        std::shared_ptr<LevelData> torch;

        std::vector<std::shared_ptr<LevelData>> collection;

    private:
        LevelsData() :
            freezer(std::shared_ptr<LevelData>(new LevelData(false, "freezerman"))),
            sheriff(std::shared_ptr<LevelData>(new LevelData(false, "sheriffman"))),
            boomer(std::shared_ptr<LevelData>(new LevelData(false, "boomerman"))),
            military(std::shared_ptr<LevelData>(new LevelData(false, "militaryman"))),
            vine(std::shared_ptr<LevelData>(new LevelData(false, "vineman"))),
            shield(std::shared_ptr<LevelData>(new LevelData(false, "shieldman"))),
            night(std::shared_ptr<LevelData>(new LevelData(false, "nightman"))),
            torch(std::shared_ptr<LevelData>(new LevelData(false, "torchman"))){

            collection.push_back(freezer);
            collection.push_back(sheriff);
            collection.push_back(boomer);
            collection.push_back(military);
            collection.push_back(vine);
            collection.push_back(shield);
            collection.push_back(night);
            collection.push_back(torch);

        }

        friend class GameManager;
    };

    class ItemData {
    public:
        int id;
        std::string string;

    private:
        ItemData(int id, std::string string) {
            this->id = id;
            this->string = string;
        }

        ItemData(ItemData const& other) {
            this->id = other.id;
            this->string = other.string;
        }

        friend class GameManager;
        friend class ItemsData;
    };

    class ItemsData {
    public:
        std::shared_ptr<ItemData> life;
        std::shared_ptr<ItemData> helmet;
        std::shared_ptr<ItemData> head;
        std::shared_ptr<ItemData> chest;
        std::shared_ptr<ItemData> fist;
        std::shared_ptr<ItemData> boot;
        std::shared_ptr<ItemData> healthSmall;
        std::shared_ptr<ItemData> healthBig;
        std::shared_ptr<ItemData> energySmall;
        std::shared_ptr<ItemData> energyBig;
        std::shared_ptr<ItemData> eTank;
        std::shared_ptr<ItemData> mTank;

        std::vector<std::shared_ptr<ItemData>> collection;

    private:
        ItemsData() :
            life(std::shared_ptr<ItemData>(new ItemData(ItemId::Life, "life"))),
            helmet(std::shared_ptr<ItemData>(new ItemData(ItemId::Helmet, "helmet"))),
            head(std::shared_ptr<ItemData>(new ItemData(ItemId::Head, "head"))),
            chest(std::shared_ptr<ItemData>(new ItemData(ItemId::Chest, "chest"))),
            fist(std::shared_ptr<ItemData>(new ItemData(ItemId::Fist, "fist"))),
            boot(std::shared_ptr<ItemData>(new ItemData(ItemId::Boot, "boot"))),
            healthSmall(std::shared_ptr<ItemData>(new ItemData(ItemId::HealthSmall, "health_small"))),
            healthBig(std::shared_ptr<ItemData>(new ItemData(ItemId::HealthBig, "health_big"))),
            energySmall(std::shared_ptr<ItemData>(new ItemData(ItemId::EnergySmall, "energy_small"))),
            energyBig(std::shared_ptr<ItemData>(new ItemData(ItemId::EnergyBig, "energy_big"))),
            eTank(std::shared_ptr<ItemData>(new ItemData(ItemId::eTank, "e_tank"))),
            mTank(std::shared_ptr<ItemData>(new ItemData(ItemId::mTank, "m_tank"))) {

            collection.push_back(life);
            collection.push_back(helmet);
            collection.push_back(head);
            collection.push_back(chest);
            collection.push_back(fist);
            collection.push_back(boot);
            collection.push_back(healthSmall);
            collection.push_back(healthBig);
            collection.push_back(energySmall);
            collection.push_back(energyBig);
            collection.push_back(eTank);
            collection.push_back(mTank);

        }

        friend class GameManager;
    };

    class PlayerData {
    public:
        enum ClimbDirection {
            Up = 0,
            Down,
            None
        };

        int lives;

        int eTanks = 0;
        int mTanks = 0;

    private:
        PlayerData(int lives = 3, int eTanks = 0, int mTanks = 0) {
            this->lives = lives;
            this->eTanks = eTanks;
            this->mTanks = mTanks;
        }

        PlayerData(PlayerData const& other) {
            this->lives = other.lives;
            this->eTanks = other.eTanks;
            this->mTanks = other.mTanks;
        }

        friend class GameManager;

    };

    class OptionsData {
    public:
        bool helmetActivated;
        bool extremeActivated;

    private:
        OptionsData(bool helmetActivated = true, bool extremeActivated = false) {
            this->helmetActivated = helmetActivated;
            this->extremeActivated = extremeActivated;
        }

        OptionsData(OptionsData const& other) {
            this->helmetActivated = other.helmetActivated;
            this->extremeActivated = other.extremeActivated;
        }

        friend class GameManager;

    };

    class GameManager {
    public:
        static GameManager& getInstance()
        {
            static GameManager instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
    private:
        GameManager() { 
        }

    public:
        GameManager(GameManager const&) = delete;
        void operator=(GameManager const&) = delete;

    public:

        void loadGameDataFromDefaultSlot() {
            auto dataSlot = windy::SaveManager::readSlot(windy::SaveManager::defaultSlot);

            this->unlockables.helmet->acquired = dataSlot.helmet;
            this->unlockables.head->acquired = dataSlot.head;
            this->unlockables.chest->acquired = dataSlot.chest;
            this->unlockables.fist->acquired = dataSlot.fist;
            this->unlockables.boot->acquired = dataSlot.boot;

            this->player.eTanks = dataSlot.e;
            this->player.mTanks = dataSlot.m;

            this->player.lives = dataSlot.lives;

            this->browners.freezer->acquired = dataSlot.freezer;
            this->browners.sheriff->acquired = dataSlot.sheriff;
            this->browners.boomer->acquired = dataSlot.boomer;
            this->browners.military->acquired = dataSlot.military;
            this->browners.vine->acquired = dataSlot.vine;
            this->browners.shield->acquired = dataSlot.shield;
            this->browners.night->acquired = dataSlot.night;
            this->browners.torch->acquired = dataSlot.torch;


            this->browners.helmet->acquired = this->unlockables.helmetAcquired();
            this->browners.extreme->acquired = this->unlockables.extremeAcquired();

            this->levels.freezer->defeated = this->browners.freezer->acquired;
            this->levels.sheriff->defeated = this->browners.sheriff->acquired;
            this->levels.boomer->defeated = this->browners.boomer->acquired;
            this->levels.military->defeated = this->browners.military->acquired;
            this->levels.vine->defeated = this->browners.vine->acquired;
            this->levels.shield->defeated = this->browners.shield->acquired;
            this->levels.night->defeated = this->browners.night->acquired;
            this->levels.torch->defeated = this->browners.torch->acquired;

            this->options.helmetActivated = dataSlot.helmetActivated;
            this->collectibles = dataSlot.collectibles;
        }

        windy::Slot getDefaultSlot() {
            return windy::SaveManager::readSlot(windy::SaveManager::defaultSlot);
        }

        void saveDefaultSlot(const windy::Slot& data) {
            windy::SaveManager::saveSlot(windy::SaveManager::defaultSlot, data);
        }

        UnlockablesData unlockables;
        BrownersData browners;

        PlayerData player;

        LevelsData levels;

        ItemsData items;


        std::shared_ptr<LevelData> currentLevel;

        OptionsData options;

        std::vector<std::pair<std::string, bool>> collectibles;
    };

}

#endif