#include "GameVerticalDoor.h"

#include "Windy/AnimationAction.h"
#include "Windy/Armature.h"
#include "Windy/Sprite.h"

using namespace game;

game::Resources& GameVerticalDoor::getResources() {
    static game::Resources resources{game::ResourceKind::LevelSpecial, "vertical_door"};
    return resources;
}

void GameVerticalDoor::setup() {
    const auto& resources = GameVerticalDoor::getResources();
    auto armature = windy::Armature(resources._armaturePath);
    auto newAnchor = armature.get("door").anchor;
    this->sprite = windy::Sprite::create(resources._spritePath, newAnchor);
    this->addChild(this->sprite);
    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = this->sprite->getContentSize();

    this->collisionRectangles = armature.get("door").collisionRectangles;

    auto collisionBoxCenter = cocos2d::Point(this->collisionRectangles[0]->getMidX(), this->collisionRectangles[0]->getMidY());

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = windy::Logical::normalizeCollisionRectangle(this, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];


    this->sprite->setPosition(collisionBoxCenter + cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));

    auto lockAction = windy::AnimationAction("lock", "vertical_door_lock", false, this->lockTime / 8.0f);
    auto unlockAction = windy::AnimationAction("unlock", "vertical_door_unlock", false, this->lockTime / 8.0f);

    this->sprite->appendAction(lockAction, false);
    this->sprite->appendAction(unlockAction, false);
    this->sprite->setAnimation("vertical_door_lock");
    this->sprite->setImageIndex(3);
}

void GameVerticalDoor::setupPrefix() {
    this->prefix = "vertical";
}

std::shared_ptr<cocos2d::Rect> GameVerticalDoor::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    const auto& resources = GameVerticalDoor::getResources();
    auto armature = windy::Armature(resources._armaturePath);
    auto newAnchor = armature.get("door").anchor;
    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto collisionRectangles = armature.get("door").collisionRectangles;

    for (int i = 0; i < collisionRectangles.size(); ++i) {
        collisionRectangles[i] = windy::Logical::normalizeCollisionRectangle(position, *collisionRectangles[i]);
    }

    return collisionRectangles[0];
}
