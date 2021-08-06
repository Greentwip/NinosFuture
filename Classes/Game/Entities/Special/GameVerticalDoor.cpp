#include "GameVerticalDoor.h"

#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"
#include "Windy/Sprite.h"


using namespace game;

class GameVerticalDoorResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string GameVerticalDoorResources::spritePath = "sprites/gameplay/level/special/general/vertical_door/vertical_door";
std::string GameVerticalDoorResources::armaturePath = "physics/gameplay/level/special/general/door/door";

void GameVerticalDoor::setup() {

    auto armature = windy::Armature(GameVerticalDoorResources::armaturePath);

    auto newAnchor = armature.get("door").anchor;

    this->sprite = windy::Sprite::create(GameVerticalDoorResources::spritePath, newAnchor);
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



    auto action = windy::AnimationAction("lock", "vertical_door_lock", false, this->lockTime / 8.0f);

    this->sprite->appendAction(action, false);
    this->sprite->setAnimation("vertical_door_lock");
    this->sprite->setImageIndex(3);
}

void GameVerticalDoor::setupPrefix() {
    this->prefix = "vertical";
}

std::shared_ptr<cocos2d::Rect> GameVerticalDoor::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {

    auto armature = windy::Armature(GameVerticalDoorResources::armaturePath);

    auto newAnchor = armature.get("door").anchor;

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);

    auto collisionRectangles = armature.get("door").collisionRectangles;

    for (int i = 0; i < collisionRectangles.size(); ++i) {
        collisionRectangles[i] = windy::Logical::normalizeCollisionRectangle(position, *collisionRectangles[i]);
    }

    return collisionRectangles[0];
}
