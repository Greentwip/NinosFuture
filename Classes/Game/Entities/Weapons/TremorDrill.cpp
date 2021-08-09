#include <vector>

#include "TremorDrill.h"

#include "Windy/Sprite.h"
#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"

#include "Windy/GameTags.h"

using namespace game;

class TremorDrillResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string TremorDrillResources::spritePath = "sprites/gameplay/level/weapon/tremor_tail/tremor_tail";
std::string TremorDrillResources::armaturePath = "physics/gameplay/level/weapon/tremor_tail/tremor_tail";

void TremorDrill::preloadResources() {
    windy::Armature::cache(TremorDrillResources::armaturePath);
    windy::Sprite::cache(TremorDrillResources::spritePath);
}

TremorDrill* TremorDrill::create() {
    TremorDrill* tremorDrill = new (std::nothrow) TremorDrill();

    if (tremorDrill && tremorDrill->init()) {
        tremorDrill->autorelease();
        return tremorDrill;
    }

    CC_SAFE_DELETE(tremorDrill);
    return nullptr;

}

bool TremorDrill::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    Logical::composite<windy::Weapon>(this,
                                      TremorDrillResources::armaturePath,
                                      TremorDrillResources::spritePath,
                                      "tremor_tail");


    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("tail_a",        "tremor_tail_a",         false,   0.04f),
        windy::AnimationAction("tail_b",        "tremor_tail_b",         true,    0.04f),
        windy::AnimationAction("tail_c",        "tremor_tail_c",         false,   0.04f),
        windy::AnimationAction("tail_idle",     "tremor_tail_idle",      false,   0.04f)
    };


    this->sprite->appendActionSet(actionSet, false);

    this->sprite->runAction("tail_idle");
    this->sprite->setAnimation("tremor_tail_idle");

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    this->power = 8;

    this->disableAttack();

    return true;
}

void TremorDrill::normalizeCollisionRectangles() {
    auto worldPosition = this->getParent()->convertToWorldSpace(this->getPosition()); 
    this->lastPosition = worldPosition;

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(worldPosition, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];

}

/*std::shared_ptr<cocos2d::Rect> TremorDrill::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {

    auto armature = windy::Armature(TremorDrillResources::armaturePath);

    auto newAnchor = armature.get("tremor_tail").anchor;

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);

    auto collisionRectangles = armature.get("tremor_tail").collisionRectangles;

    for (int i = 0; i < collisionRectangles.size(); ++i) {
        collisionRectangles[i] = Logical::normalizeCollisionRectangle(position, *collisionRectangles[i]);
    }

    return collisionRectangles[0];
}*/


void TremorDrill::enableAttack() {
    this->setTag(windy::GameTags::Weapon::WeaponEnemy);
}

void TremorDrill::disableAttack() {
    this->setTag(windy::GameTags::Weapon::WeaponNone);
}

void TremorDrill::recomputeCollisionRectangles() {
    auto positionDifference = this->getParent()->getPosition() - lastPosition;
    this->lastPosition = this->getParent()->getPosition();

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        float differenceX = positionDifference.x;
        float differenceY = positionDifference.y;

        this->collisionRectangles[i]->origin.x += differenceX;
        this->collisionRectangles[i]->origin.y += differenceY;
    }

}


void TremorDrill::update(float dt) {
    this->recomputeCollisionRectangles();

    this->onUpdate(dt);
}

void TremorDrill::onUpdate(float dt) {

}