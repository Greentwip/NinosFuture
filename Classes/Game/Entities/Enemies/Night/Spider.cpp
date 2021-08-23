#include "Spider.h"

#include "Windy/AnimationAction.h"

#include <vector>

using namespace game;

game::Resources& Spider::getResources() {
    static game::Resources resources{ game::ResourceKind::EnemyNight, "wall_spider" };
    return resources;
}

void Spider::setup() {
    maxHealth = 3;
    health = maxHealth;
    power = 2;

    Logical::composite<Spider>(this);

    std::vector<windy::AnimationAction> actionSet = {
            windy::AnimationAction{"stand",  "wall_spider_stand",  false, 0.10f},
    };

    sprite->appendActionSet(actionSet, false);
    sprite->setAnimation("wall_spider_stand");

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;
}

std::shared_ptr<cocos2d::Rect> Spider::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<Spider>(position, size);
}

void Spider::attack(float dt) {
   
}
