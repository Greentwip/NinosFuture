#include "TremorDrill.h"

#include "Windy/AnimationAction.h"
#include "Windy/GameTags.h"

#include <vector>

using namespace game;

windy::Resources& TremorDrill::getResources() {
    static windy::Resources resources{windy::ResourceKind::Weapon, "tremor_tail"};
    return resources;
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

    Logical::composite<TremorDrill>(this);
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
    this->lastCollisionPosition = worldPosition;

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(worldPosition, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];

}

void TremorDrill::enableAttack() {
    this->setTag(windy::GameTags::Weapon::WeaponEnemy);
}

void TremorDrill::disableAttack() {
    this->setTag(windy::GameTags::Weapon::WeaponNone);
}

void TremorDrill::recomputeCollisionRectangles() {
    auto positionDifference = this->getParent()->getPosition() - lastCollisionPosition;
    this->lastCollisionPosition = this->getParent()->getPosition();

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

    this->lastPosition = this->getParent()->getPosition();

}
