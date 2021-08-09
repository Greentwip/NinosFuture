#include "LevelController.h"
#include "./../GameTags.h"

using namespace windy;

bool LevelController::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->setTag(GameTags::General::None);

    return true;
}


void LevelController::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void LevelController::onUpdate(float dt) {

}