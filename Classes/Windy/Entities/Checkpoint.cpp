#include "Checkpoint.h"
#include "./../GameTags.h"

using namespace windy;

bool Checkpoint::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    this->setTag(GameTags::General::Checkpoint);

    return true;
}


void Checkpoint::parseBehavior(const cocos2d::ValueMap& behavior) {
	std::string checkpointType = behavior.at("type").asString();

	if (checkpointType.compare("first") == 0) {
        this->setTag(GameTags::Logic::Checkpoint::First);
	}
    else if (checkpointType.compare("last") == 0) {
        this->setTag(GameTags::Logic::Checkpoint::Last);
    }
}

void Checkpoint::onUpdate(float dt) {

}