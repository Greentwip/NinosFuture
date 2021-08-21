#include <string>

#include "ReadyIndicator.h"
#include "Windy/GameTags.h"

#include "Windy/Sprite.h"

using namespace game;

class ReadyIndicatorResources {
public:
    static std::string spritePath;
};

std::string ReadyIndicatorResources::spritePath = "sprites/gameplay/level/ui/ready_object/ready_object";

void ReadyIndicator::preloadResources() {
    windy::Sprite::cache(ReadyIndicatorResources::spritePath);
}


ReadyIndicator* ReadyIndicator::create(std::function<void()> onReadyCallback) {

    ReadyIndicator* entity = new (std::nothrow) ReadyIndicator();

    if (entity) {
        entity->_onReadyCallback = onReadyCallback;
    }

    if (entity && entity->init()) {
        entity->autorelease();
        return entity;
    }

    CC_SAFE_DELETE(entity);
    return nullptr;
}

bool ReadyIndicator::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->sprite = windy::Sprite::create(ReadyIndicatorResources::spritePath, cocos2d::Point(0.5f, 0.5f));

    this->sprite->setPosition(0, 0);

    this->addChild(this->sprite);

    auto action = windy::AnimationAction("ready", "spr_ready", true, 0.10f);

    this->sprite->appendAction(action, false);

    this->sprite->runAction("ready");

    this->setTag(windy::GameTags::Gui::Interface);

    auto delay = cocos2d::DelayTime::create(2);

    auto callback = cocos2d::CallFunc::create([this]() {
        if (_onReadyCallback != nullptr) {
            _onReadyCallback();
        }
    });


    auto sequence = cocos2d::Sequence::create(delay, callback, nullptr);

    runAction(sequence);


    return true;
}

