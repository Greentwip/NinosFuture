#ifndef __WINDY_GAME_MANAGER__
#define __WINDY_GAME_MANAGER__

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "Windy/SaveManager.h"

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

    private:
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
        UnlockableData helmet;
        UnlockableData head;
        UnlockableData chest;
        UnlockableData fist;
        UnlockableData boot;

        bool helmetAcquired() {
            return helmet.acquired;
        }

        bool extremeAcquired() {
            return helmet.acquired && head.acquired && chest.acquired && fist.acquired && boot.acquired;
        }

    private:
        UnlockablesData() :
            helmet(UnlockableData(2, false, "helmet")),
            head(UnlockableData(3, false, "head")),
            chest(UnlockableData(4, false, "chest")),
            fist(UnlockableData(5, false, "fist")),
            boot(UnlockableData(6, false, "boot")) {

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
            teleport(std::make_shared<BrownerData>(1, false, "")),
            violet(std::make_shared<BrownerData>(2, false, "violet")),
            fuzzy(std::make_shared<BrownerData>(3, false, "fuzzy", "", 28)),
            freezer(std::make_shared<BrownerData>(4, false, "freezer", "freezerman", 28)),
            sheriff(std::make_shared<BrownerData>(5, false, "sheriff", "sheriffman", 28)),
            boomer(std::make_shared<BrownerData>(6, false, "boomer", "boomerman", 28)),
            military(std::make_shared<BrownerData>(7, false, "military", "militaryman", 28)),
            vine(std::make_shared<BrownerData>(8, false, "vine", "vineman", 28)),
            shield(std::make_shared<BrownerData>(9, false, "shield", "shieldman", 28)),
            night(std::make_shared<BrownerData>(10, false, "night", "nightman", 28)),
            torch(std::make_shared<BrownerData>(11, false, "torch", "torchman", 28)),
            helmet(std::make_shared<BrownerData>(12, false, "helmet")),
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
            life(std::shared_ptr<ItemData>(new ItemData(1, "life"))),
            helmet(std::shared_ptr<ItemData>(new ItemData(2, "helmet"))),
            head(std::shared_ptr<ItemData>(new ItemData(3, "head"))),
            chest(std::shared_ptr<ItemData>(new ItemData(4, "chest"))),
            fist(std::shared_ptr<ItemData>(new ItemData(5, "fist"))),
            boot(std::shared_ptr<ItemData>(new ItemData(6, "boot"))),
            healthSmall(std::shared_ptr<ItemData>(new ItemData(7, "health_small"))),
            healthBig(std::shared_ptr<ItemData>(new ItemData(8, "health_big"))),
            energySmall(std::shared_ptr<ItemData>(new ItemData(9, "energy_small"))),
            energyBig(std::shared_ptr<ItemData>(new ItemData(10, "energy_big"))),
            eTank(std::shared_ptr<ItemData>(new ItemData(11, "e_tank"))),
            mTank(std::shared_ptr<ItemData>(new ItemData(12, "m_tank"))) {

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

    private:
        OptionsData(bool helmetActivated = true) {
            this->helmetActivated = helmetActivated;
        }

        OptionsData(OptionsData const& other) {
            this->helmetActivated = other.helmetActivated;
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
        GameManager() : slot(1) { 
        }

    public:
        GameManager(GameManager const&) = delete;
        void operator=(GameManager const&) = delete;

    public:

        windy::Slot getDefaultSlot() {
            return windy::SaveManager::readSlot(this->slot);
        }

        void saveDefaultSlot(const windy::Slot& data) {
            windy::SaveManager::saveSlot(this->slot, data);
        }

        UnlockablesData unlockables;
        BrownersData browners;

        PlayerData player;

        LevelsData levels;

        ItemsData items;


        std::shared_ptr<LevelData> currentLevel;

        OptionsData options;

        int slot;

        std::vector<std::string> collectibles;
    };

}

#endif