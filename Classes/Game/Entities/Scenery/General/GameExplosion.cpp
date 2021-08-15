#include <cmath>

#include "GameExplosion.h"

#include "Windy/Sprite.h"
#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"

using namespace game;

class GameExplosionResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string GameExplosionResources::spritePath = "sprites/gameplay/level/fx/explosion/explosion";
std::string GameExplosionResources::armaturePath = "physics/gameplay/level/fx/explosion/explosion";


void GameExplosion::preloadResources() {
    windy::Armature::cache(GameExplosionResources::armaturePath);
    windy::Sprite::cache(GameExplosionResources::spritePath);
}


GameExplosion* GameExplosion::create() {
    GameExplosion* explosion = new (std::nothrow) GameExplosion();

    if (explosion && explosion->init()) {
        explosion->autorelease();
        return explosion;
    }

    CC_SAFE_DELETE(explosion);
    return nullptr;

}

bool GameExplosion::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->setTag(windy::GameTags::Scenery::Particle);


    return true;
}

void GameExplosion::setup(cocos2d::Color3B spriteColor) {

    windy::Logical::composite<windy::Explosion>(this,
                                                GameExplosionResources::armaturePath,
                                                GameExplosionResources::spritePath,
                                                "explosion");

    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    auto action = windy::AnimationAction("shoot", "explosion_death", true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("shoot");

    this->sprite->setColor(spriteColor);

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

}


std::shared_ptr<cocos2d::Rect> GameExplosion::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle(position, size, GameExplosionResources::armaturePath, "explosion");
}


void GameExplosion::fire(float angle) {

    //float deltaY = targetPosition.y - this->getPositionY();
    //float deltaX = targetPosition.x - this->getPositionX();

    //float angle = (float)std::atan2(deltaY, deltaX);

    float velocity = 2;
    float speedX = static_cast<float>(velocity * (float)std::cos(angle));
    float speedY = static_cast<float>(velocity * (float)std::sin(angle));
    this->speed = cocos2d::Point(speedX, speedY);

    this->setTag(windy::GameTags::Scenery::Particle);
}

