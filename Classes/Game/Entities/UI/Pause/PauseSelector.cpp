#include <cmath>
#include <algorithm>
#include <map>

#include "PauseSelector.h"
#include "PauseInterruptor.h"

#include "Windy/Sprite.h"
#include "Windy/Input.h"
#include "Windy/AudioManager.h"

using namespace game;

class PauseSelectorResources {
public:
    static std::string spritePath;
};

std::string PauseSelectorResources::spritePath = "sprites/gameplay/level/ui/selector/selector";


void PauseSelector::preloadResources() {
    windy::Sprite::cache(PauseSelectorResources::spritePath);
}

PauseSelector* PauseSelector::create(PauseSelector::SelectorOrientation orientation) {

    PauseSelector* selector = new (std::nothrow) PauseSelector();

    if (selector) {
        selector->orientation = orientation;
        selector->sprite = nullptr;
        selector->selectedItem = nullptr;
    }

    if (selector && selector->init()) {
        selector->autorelease();
        return selector;
    }

    CC_SAFE_DELETE(selector);
    return nullptr;
}

bool PauseSelector::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->sprite = windy::Sprite::create(PauseSelectorResources::spritePath, cocos2d::Point(0, 0));
    this->sprite->setPosition(cocos2d::Point(0, 0));
    this->addChild(this->sprite);

    /*if (this->orientation == SelectorOrientation::Up) {
        this->sprite->setAnimation("arrow_down");
        this->sprite->setFlippedY(true);
    }
    else */if (this->orientation == SelectorOrientation::OrientationDown) {
        this->sprite->setAnimation("arrow_down");
    }
    /*else if (this->orientation == SelectorOrientation::Left) {
        this->sprite->setAnimation("arrow_right");
        this->sprite->setFlippedX(true);
    }*/
    else if (this->orientation == SelectorOrientation::OrientationRight) {
        this->sprite->setAnimation("arrow_right");
    }

    return true;
}

PauseInterruptor* PauseSelector::getSelectedItem() {
    return this->selectedItem;
}

void PauseSelector::setSelectedItem(PauseInterruptor* item) {
    this->setPosition(item->getPosition());

    if (this->orientation == SelectorOrientation::OrientationDown) {
        this->sprite->setPosition(cocos2d::Point(0, 3));
    }

    if (this->selectedItem != nullptr) {
        this->selectedItem->leaveInterruptor();
    }

    this->selectedItem = item;

    this->selectedItem->visitInterruptor();
}

float PauseSelector::calculateDistance(const cocos2d::Point& a, const cocos2d::Point& b) {
    return static_cast<float>(std::pow(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2), 0.5));
}

PauseInterruptor* PauseSelector::findNearest(const std::vector<PauseInterruptor*>& items, SelectorDirection direction) {
    auto pointA = this->getPosition();


    std::vector<std::pair<float, PauseInterruptor*>> itemsByDistance;

    for (int i = 0; i < items.size(); ++i) {
        auto item = items[i];

        auto pointB = item->getPosition();

        float distance = 0.0f;

        if (item->isVisible()) {
            switch (direction) {
                case SelectorDirection::DirectionUp: 
                {
                    if (pointB.y > pointA.y) {
                        distance = calculateDistance(pointA, pointB);
                    }
                }
                break;

                case SelectorDirection::DirectionDown: 
                {
                    if (pointB.y < pointA.y) {
                        distance = calculateDistance(pointA, pointB);
                    }
                }
                break;

                case SelectorDirection::DirectionLeft: 
                {
                    if (pointB.x < pointA.x) {
                        distance = calculateDistance(pointA, pointB);
                    }
                }
                break;

                case SelectorDirection::DirectionRight: 
                {
                    if (pointB.x > pointA.x) {
                        distance = calculateDistance(pointA, pointB);
                    }

                }
                break;
            }

            if (distance != 0) {
                itemsByDistance.push_back({ distance, item });
            }
        }
    }

    if (itemsByDistance.size() > 0) {
        std::sort(itemsByDistance.begin(),
            itemsByDistance.end(),
            [=](const std::pair<float, PauseInterruptor*>& a, const std::pair<float, PauseInterruptor*>& b) {
                return a.first < b.first;
            });

        return itemsByDistance[0].second;        
    }
    else {
        return nullptr;
    }

}

void PauseSelector::selectFromItems(const std::vector<PauseInterruptor*>& items) {
    PauseInterruptor* nearestItem = nullptr;

    if (windy::Input::keyPressed(windy::InputKey::Up)) {
        nearestItem = this->findNearest(items, SelectorDirection::DirectionUp);
    }
    else if (windy::Input::keyPressed(windy::InputKey::Down)) {
        nearestItem = this->findNearest(items, SelectorDirection::DirectionDown);
    }
    else if (windy::Input::keyPressed(windy::InputKey::Left)) {
        nearestItem = this->findNearest(items, SelectorDirection::DirectionLeft);
    }
    else if (windy::Input::keyPressed(windy::InputKey::Right)) {
        nearestItem = this->findNearest(items, SelectorDirection::DirectionRight);
    }
    else if (windy::Input::keyPressed(windy::InputKey::A)) {
        if (this->selectedItem != nullptr) {
            this->selectedItem->triggerInterruptor();
        }
    }

    if (nearestItem != nullptr) {
        windy::AudioManager::playSfx(windy::Sounds::Select);
        this->setSelectedItem(nearestItem);
    }

}

