#include "PauseAnimation.h"

#include "Windy/Sprite.h"

using namespace game;

class PauseAnimationResources {
public:
    static std::string spritePath;
};

std::string PauseAnimationResources::spritePath = "sprites/gameplay/screens/pause_menu/pause_animation/pause_animation";


void PauseAnimation::preloadResources() {
    windy::Sprite::cache(PauseAnimationResources::spritePath);
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

    this->sprite = windy::Sprite::create(PauseAnimationResources::spritePath, cocos2d::Point(0.5f, 0));
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
