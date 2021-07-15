#include "CameraScroll.h"

#include "./../GameTags.h"

using namespace windy;

bool CameraScroll::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }
    this->scroll = CameraFlags::CameraScroll::ScrollNone;

    this->setTag(GameTags::General::Scroll);

    return true;
}


void CameraScroll::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void CameraScroll::onUpdate(float dt) {

}