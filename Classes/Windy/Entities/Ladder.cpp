#include "Ladder.h"

#include "Block.h"

#include "./../GameTags.h"



using namespace windy;

bool Ladder::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    auto ceilingSize = cocos2d::Size(this->collisionBox->size.width, 12.0f);
    auto ceilingPosition = cocos2d::Point(this->getPositionX(), this->collisionBox->getMaxY() - (ceilingSize.height * 0.5f));

    this->ceiling = dynamic_cast<Block*>(Logical::create<Block>(this->level, ceilingPosition, ceilingSize, 32));

    this->level->addChild(this->ceiling);

    this->level->entities.pushBack(this->ceiling);

    this->solidified = false;

    this->solidify();
    
    this->setTag(GameTags::General::Ladder);

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    return true;
}


void Ladder::parseBehavior(const cocos2d::ValueMap& behavior) {
}

void Ladder::solidify() {
    this->ceiling->setTag(GameTags::General::Block);
    this->solidified = true;
}

void Ladder::unsolidify() {
    this->ceiling->setTag(GameTags::General::None);
    this->solidified = false;
}

void Ladder::onUpdate(float dt) {

}