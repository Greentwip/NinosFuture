#include "Water.h"

#include "./../GameTags.h"

using namespace windy;

bool Water::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    this->setTag(GameTags::General::Water);

    this->setup();

    return true;
}

