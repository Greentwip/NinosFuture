#include "Spikes.h"
#include "./../GameTags.h"

using namespace windy;

void Spikes::setup() {
    this->power = 1024;
    this->maxHealth = 1025;
    this->health = this->maxHealth;

    this->sprite = nullptr;

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

}

std::shared_ptr<cocos2d::Rect> Spikes::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return std::make_shared<cocos2d::Rect>(position.x - size.width * 0.5f, position.y - size.height * 0.5f, size.width, size.height);
}

void Spikes::onDefeated() {

}

void Spikes::setOrientation() {

}

void Spikes::attack(float dt) {

}

void Spikes::onCollisionEnter(Logical* collision) {

}

void Spikes::onUpdate(float dt) {

}