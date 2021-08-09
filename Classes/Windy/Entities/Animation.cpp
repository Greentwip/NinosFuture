#include "Animation.h"

using namespace windy;

bool Animation::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}


void Animation::parseBehavior(const cocos2d::ValueMap& behavior) {

}
