#include "PauseAnimation.h"

#include "Windy/Sprite.h"

using namespace game;

windy::Resources& PauseAnimation::getResources() {
    static windy::Resources resources{windy::ResourceKind::PauseMenu, "pause_animation", false};
    return resources;
}

PauseAnimation* PauseAnimation::create() {

    PauseAnimation* animation = new (std::nothrow) PauseAnimation();

    if (animation && animation->init()) {
        animation->autorelease();
        return animation;
    }

    CC_SAFE_DELETE(animation);
    return nullptr;
}

bool PauseAnimation::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->sprite = windy::Sprite::create(PauseAnimation::getResources()._spritePath, cocos2d::Point(0.5f, 0));
    this->sprite->setPosition(cocos2d::Point(0, 0));
    this->addChild(this->sprite);


    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("violet",    "violet_animation",     true,   0.10f),
        windy::AnimationAction("ex",        "ex_animation",         true,   0.10f),
        windy::AnimationAction("helmet",    "helmet_animation",     true,   0.10f),
        windy::AnimationAction("fuzzy",     "fuzzy_animation",      true,   0.10f),
        windy::AnimationAction("sheriff",   "sheriff_animation",    true,   0.10f),
        windy::AnimationAction("military",  "military_animation",   true,   0.10f),
        windy::AnimationAction("vine",      "vine_animation",       true,   0.10f),
        windy::AnimationAction("night",     "night_animation",      true,   0.10f)
    };

    this->sprite->appendActionSet(actionSet, false);

    return true;
}

void PauseAnimation::setAnimation(const std::string& animationName) {
    this->sprite->setAnimation(animationName + "_" + "animation");
}

void PauseAnimation::swapContents(const std::string& contents) {
    this->sprite->runAction(contents);
}
