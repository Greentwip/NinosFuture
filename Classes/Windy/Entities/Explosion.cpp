#include "Explosion.h"

#include "./../GameTags.h"

using namespace windy;

bool Explosion::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->setTag(GameTags::Scenery::Particle);

    return true;
}


void Explosion::onFinished() {
    this->finishForever();
}


void Explosion::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Explosion::onUpdate(float dt) {

}