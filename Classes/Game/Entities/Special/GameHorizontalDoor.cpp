#include "GameHorizontalDoor.h"

#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"
#include "Windy/Sprite.h"


using namespace game;

class GameHorizontalDoorResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string GameHorizontalDoorResources::spritePath = "sprites/gameplay/level/special/general/horizontal_door/horizontal_door";
std::string GameHorizontalDoorResources::armaturePath = "physics/gameplay/level/special/general/horizontal_door/horizontal_door";

void GameHorizontalDoor::preloadResources() {
    windy::Armature::cache(GameHorizontalDoorResources::armaturePath);
    windy::Sprite::cache(GameHorizontalDoorResources::spritePath);
}

void GameHorizontalDoor::setup() {

    auto armature = windy::Armature(GameHorizontalDoorResources::armaturePath);

    auto newAnchor = armature.get("horizontal_door").anchor;

    this->sprite = windy::Sprite::create(GameHorizontalDoorResources::spritePath, newAnchor);
    this->addChild(this->sprite);

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = this->sprite->getContentSize();

    this->collisionRectangles = armature.get("horizontal_door").collisionRectangles;

    auto collisionBoxCenter = cocos2d::Point(this->collisionRectangles[0]->getMidX(), this->collisionRectangles[0]->getMidY());

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = windy::Logical::normalizeCollisionRectangle(this, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];


    this->sprite->setPosition(collisionBoxCenter + cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));



    auto action = windy::AnimationAction("lock", "horizontal_door_lock", false, this->lockTime / 8.0f);

    this->sprite->appendAction(action, false);
    this->sprite->setAnimation("horizontal_door_lock");
    this->sprite->setImageIndex(3);

    //this->close();
}

void GameHorizontalDoor::setupPrefix() {
    this->prefix = "horizontal";
}

std::shared_ptr<cocos2d::Rect> GameHorizontalDoor::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {

    auto armature = windy::Armature(GameHorizontalDoorResources::armaturePath);

    auto newAnchor = armature.get("horizontal_door").anchor;

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);

    auto collisionRectangles = armature.get("horizontal_door").collisionRectangles;

    for (int i = 0; i < collisionRectangles.size(); ++i) {
        collisionRectangles[i] = windy::Logical::normalizeCollisionRectangle(position, *collisionRectangles[i]);
    }

    return collisionRectangles[0];
}
