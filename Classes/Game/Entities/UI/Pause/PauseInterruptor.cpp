#include "PauseInterruptor.h"

#include "Windy/Sprite.h"

using namespace game;

game::Resources& PauseInterruptor::getResources() {
    static game::Resources resources{game::ResourceKind::PauseMenu, "pause_interruptor", false};
    return resources;
}

PauseInterruptor* PauseInterruptor::create(const std::string& animation, std::function<void(PauseInterruptor* sender)> onTriggered) {

    PauseInterruptor* interruptor = new (std::nothrow) PauseInterruptor();

    if (interruptor) {
        interruptor->animation = animation;
        interruptor->onTriggered = onTriggered;
    }

    if (interruptor && interruptor->init()) {
        interruptor->autorelease();
        return interruptor;
    }

    CC_SAFE_DELETE(interruptor);
    return nullptr;
}

bool PauseInterruptor::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->sprite = windy::Sprite::create(PauseInterruptor::getResources()._spritePath, cocos2d::Point(0, 1));
    this->sprite->setPosition(cocos2d::Point(0, 0));
    this->addChild(this->sprite);

    this->sprite->setAnimation(this->animation + "_" + "interruptor");

    this->visitable = true;

    this->brownerId = BrownerIdentifier::Invalid;
    this->pauseItem = "";
    this->energyBar = nullptr;
    
    return true;
}

void PauseInterruptor::setVisitable(bool visitable) {
    this->visitable = visitable;
    this->sprite->setImageIndex(0);
}

void PauseInterruptor::visitInterruptor() {
    if (this->visitable) {
        this->sprite->setImageIndex(1);
    }
}

void PauseInterruptor::leaveInterruptor() {
    if (this->visitable) {
        this->sprite->setImageIndex(0);
    }
}

void PauseInterruptor::triggerInterruptor() {
    if (this->onTriggered) {
        this->onTriggered(this);
    }
}

void PauseInterruptor::setTriggerCallback(std::function<void(PauseInterruptor* sender)> onTriggered) {
    this->onTriggered = onTriggered;
}
