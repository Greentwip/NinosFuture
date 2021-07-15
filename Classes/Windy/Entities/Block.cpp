#include "Block.h"
#include "./../GameTags.h"

using namespace windy;

bool Block::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->setTag(GameTags::General::Block);

    return true;
}


void Block::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Block::onUpdate(float dt) {

}