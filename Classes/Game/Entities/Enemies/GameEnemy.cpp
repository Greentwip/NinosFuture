#include "GameEnemy.h"

#include "Game/Entities/Items/GameItem.h"

#include "Windy/AudioManager.h"
#include "Windy/EntityFactory.h"

#include "Windy/ObjectManager.h"

#include "Windy/Level.h"
#include "Windy/ObjectManager.h"



using namespace game;


void GameEnemy::onDefeated() {

    windy::AudioManager::playSfx(windy::Sounds::Explosion1);

    std::uniform_int_distribution<int> itemKindRandomDistribution(0, 400);
    std::uniform_int_distribution<int> itemVariantRandomDistribution(0, 1);
    int itemKind = itemKindRandomDistribution(Enemy::itemRandomEngine);

    std::string item = "";
    if (itemKind < 10) {
        item = "life";
    }
    else if (itemKind < 60) {
        std::string healthKind[2];
        healthKind[0] = "health_small";
        healthKind[1] = "health_big";
        int itemVariant = itemVariantRandomDistribution(Enemy::itemRandomEngine);
        item = healthKind[itemVariant];
    }
    else if (itemKind < 120) {
        std::string energyKind[2];
        energyKind[0] = "energy_small";
        energyKind[1] = "energy_big";
        int itemVariant = itemVariantRandomDistribution(Enemy::itemRandomEngine);
        item = energyKind[itemVariant];
    }

    if (item.compare("") != 0) {

        auto entryCollisionBox = Logical::getEntryCollisionRectangle<GameItem>(this->getPosition(), cocos2d::Size(8, 10));

        auto itemPosition = this->getPosition();
        auto entry = Logical::getEntry(entryCollisionBox, [=]() {

            auto itemGood = windy::EntityFactory::getInstance().create("item", itemPosition, cocos2d::Size(8, 10));

            dynamic_cast<windy::Item*>(itemGood)->setup("none", item, false, false);

            return itemGood;
            });

        this->level->objectManager->objectEntries.push_back(entry);
    }

    this->finish();

}