#include "BarMeter.h"
#include "./../GameTags.h"

using namespace windy;

bool BarMeter::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->setTag(GameTags::Gui::BarMeter);

    return true;
}


void BarMeter::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void BarMeter::onUpdate(float dt) {

}