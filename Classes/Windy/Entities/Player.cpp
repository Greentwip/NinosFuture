#include <string>

#include "Player.h"

#include "./../GameTags.h"

#include "./../Sprite.h"

#include "./../Armature.h"
#include "./../AnimationAction.h"


using namespace windy;

class PlayerResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string PlayerResources::spritePath = "sprites/characters/player/regular/browners/browners";
std::string PlayerResources::armaturePath = "physics/characters/player/regular/browners/browners";


bool Player::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->sprite = Sprite::create(PlayerResources::spritePath);

    auto armature = Armature(PlayerResources::armaturePath);

    auto newAnchor = armature.get("browners").anchor;
    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = this->sprite->getContentSize();
    this->sprite->setPosition(cocos2d::Point(0, 0) + cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));

    this->collisionRectangles = armature.get("browners").collisionRectangles;

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(this, this->collisionRectangles[i]);
    }

    this->collisionRectangle = this->collisionRectangles[0];


    this->setTag(GameTags::General::Player);

    this->alive = true;

    return true;
}

void Player::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Player::onUpdate(float dt) {

}