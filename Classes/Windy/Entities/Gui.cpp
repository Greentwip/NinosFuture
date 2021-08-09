#include "Gui.h"
#include "./../GameTags.h"

using namespace windy;

bool Gui::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->build();

    this->setTag(GameTags::Gui::Interface);

    return true;
}


void Gui::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Gui::onUpdate(float dt) {

}