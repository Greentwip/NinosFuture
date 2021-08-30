#include "GameWater.h"

#include "GameSplash.h"

#include "Windy/AudioManager.h"
#include "Windy/GameTags.h"

using namespace game;

game::Resources& GameWater::getResources() {
    static game::Resources resources{ game::ResourceKind::LevelSpecial, "water" };
    return resources;
}

void GameWater::setup()
{
    Logical::composite<GameWater>(this);
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    auto action = windy::AnimationAction("play", "water_play", true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("play");
    this->sprite->setAnimation("water_play");

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    this->splash = GameSplash::create();

    this->splash->setPosition(cocos2d::Point(0, 0));

    this->splash->setPositionY(this->splash->sprite->getContentSize().height * 0.5f);

    this->addChild(this->splash);

}

std::shared_ptr<cocos2d::Rect> GameWater::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<GameWater>(position, size);
}


void GameWater::onCollisionEnter(windy::Logical* collision) {
    if (collision->getTag() == windy::GameTags::General::Player ||
        collision->getTag() == windy::GameTags::General::Enemy ||
        collision->getTag() == windy::GameTags::General::Boss ||
        collision->getTag() == windy::GameTags::General::Item) {
        if (collision->getPositionY() > this->getPositionY()) {
            windy::AudioManager::playSfx(windy::Sounds::Splash);

            this->splash->triggerSplash();
        }
    }
}

void GameWater::onCollisionExit(windy::Logical* collision) {
    if (collision->getTag() == windy::GameTags::General::Player ||
        collision->getTag() == windy::GameTags::General::Enemy ||
        collision->getTag() == windy::GameTags::General::Boss ||
        collision->getTag() == windy::GameTags::General::Item) {
        if (collision->getPositionY() > this->getPositionY()) {
            windy::AudioManager::playSfx(windy::Sounds::Splash);

            this->splash->triggerSplash();
        }
    }
}


