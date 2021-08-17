#include "Tank.h"

using namespace game;

game::Resources& Tank::getResources() {
    static game::Resources resources{game::ResourceKind::EnemyMilitary, "tank"};
    return resources;
}

void Tank::setup() {
    maxHealth = 24;
    health = maxHealth;
    power = 6;

    Logical::composite<Tank>(this);
}

std::shared_ptr<cocos2d::Rect> Tank::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return std::shared_ptr<cocos2d::Rect>();
}

void Tank::attack(float dt) {

}
