#include <string>

#include "Fader.h"
#include "Windy/GameTags.h"

#include "Windy/Armature.h"
#include "Windy/Sprite.h"

using namespace game;

class FaderResources {
public:
    static std::string spritePath;
};

std::string FaderResources::spritePath = "sprites/gameplay/level/ui/fade/fade_nes";


void Fader::preloadResources() {
    windy::Sprite::cache(FaderResources::spritePath);
}


Fader* Fader::create(const cocos2d::Point& anchor) {

    Fader* entity = new (std::nothrow) Fader();

    if (entity) {
        entity->_anchor = anchor;
        entity->_fading = false;
        entity->_onFadeInCallback = nullptr;
        entity->_onFadeOutCallback = nullptr;
    }

    if (entity && entity->init()) {
        entity->autorelease();
        return entity;
    }

    CC_SAFE_DELETE(entity);
    return nullptr;
}

bool Fader::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    setCascadeOpacityEnabled(true);

    _sprite = windy::Sprite::create(FaderResources::spritePath, _anchor);

    _sprite->setPosition(0, 0);

    addChild(_sprite);

    _fading = false;


    setTag(windy::GameTags::Gui::Interface);


    return true;
}

void Fader::fadeIn(std::function<void()> onFadeInCallback) {

    _fading = true;
    _onFadeInCallback = onFadeInCallback;

    setOpacity(0);

    auto fadeIn = cocos2d::FadeIn::create(0.5f);

    auto callback = cocos2d::CallFunc::create([this]() {
        if (_onFadeInCallback != nullptr) {
            _onFadeInCallback();
        }
        
        _fading = false;
    });

    auto graphicsDelay = cocos2d::DelayTime::create(0.25f);

    auto sequence = cocos2d::Sequence::create(fadeIn, graphicsDelay, callback, nullptr);

    runAction(sequence);
}

void Fader::fadeOut(std::function<void()> onFadeOutCallback) {

    _fading = true;
    _onFadeOutCallback = onFadeOutCallback;

    setOpacity(255);

    auto fadeOut = cocos2d::FadeOut::create(0.5f);

    auto callback = cocos2d::CallFunc::create([this]() {
        if (_onFadeOutCallback != nullptr) {
            _onFadeOutCallback();
        }
        
        _fading = false;
    });

    auto graphicsDelay = cocos2d::DelayTime::create(0.25f);

    auto sequence = cocos2d::Sequence::create(fadeOut, graphicsDelay, callback, nullptr);

    runAction(sequence);
}

bool Fader::getFading() {
    return _fading;
}
