#include "Intro.h"

#include "Windy/InfiniteParallax.h"
#include "Windy/Sprite.h"
#include "Windy/Display.h"
#include "Windy/AnimationAction.h"

using namespace game;
using namespace cocos2d;


class IntroResources {
public:
    static std::string introPath;
    static std::string beltPath;

};

std::string IntroResources::introPath = "sprites/gameplay/screens/boss_intro";
std::string IntroResources::beltPath = "sprites/gameplay/screens/common/belt";


Intro* Intro::create(const std::string& mug, 
                     std::function<void()> onIntroEnd) {

    Intro* intro = new (std::nothrow) Intro();

    if (intro) {
        intro->mug = mug;
        intro->parallax = nullptr;
        intro->shadow = nullptr;
        intro->beltLeft = nullptr;
        intro->beltRight = nullptr;
    }

    if (intro && intro->init()) {
        intro->autorelease();
        intro->mug = mug;
        intro->onIntroEnd = onIntroEnd;
        intro->ringCount = 0;

        return intro;
    }

    CC_SAFE_DELETE(intro);
    return nullptr;
}


bool Intro::init()
{
    if (!Node::init())
    {
        return false;
    }

    auto parallaxPath = IntroResources::introPath + "/parallax/" + this->mug + "_" + "parallax" + "/" + this->mug + "_" + "parallax";

    this->parallax = windy::InfiniteParallax::create(parallaxPath, cocos2d::Point(0, 4));
    this->parallax->setPosition(windy::Display::getInstance().center);
    this->addChild(this->parallax);

    auto shadowPath = IntroResources::introPath + "/shadow/" + this->mug + "_" + "shadow" + "/" + this->mug + "_" + "shadow";

    this->shadow = windy::Sprite::create(shadowPath, cocos2d::Point(1, 0));
    this->shadow->setPosition(windy::Display::getInstance().left_bottom);
    this->addChild(this->shadow, 64);


    this->beltLeft = windy::Sprite::create(IntroResources::beltPath + "/belt_left/belt_left", cocos2d::Point(1, 0.5));
    this->beltLeft->setPosition(windy::Display::getInstance().left_center);
    this->addChild(beltLeft, 96);

    this->beltRight = windy::Sprite::create(IntroResources::beltPath + "/belt_right/belt_right", cocos2d::Point(0, 0.5));
    this->beltRight->setPosition(windy::Display::getInstance().right_center);
    this->beltRight->setFlippedY(true);
    this->addChild(beltRight, 72);

    auto beltLeftShine = windy::AnimationAction("shine", "belt_left_shine", true, 0.20f);
    auto beltRightShine = windy::AnimationAction("shine", "belt_right_shine", true, 0.20f);

    this->beltLeft->appendAction(beltLeftShine, false);
    this->beltRight->appendAction(beltRightShine, false);

    auto shadowMove = cocos2d::MoveTo::create(2, 
                                              cocos2d::Point(windy::Display::getInstance().center.x + this->shadow->getContentSize().width * 0.5f,
                                                             this->shadow->getPositionY()));

    auto shadowCallback = [=]() {
        auto beltMove = [=]() {
            this->ringCount = 0;

            auto bossCallback = [=]() {
                this->ringCount++;

                if (this->ringCount >= 2) {
                    this->onRingMoveComplete();
                }
                
            };

            auto startShineLeft = [=]() {
                this->beltLeft->runAction("shine");
            };

            auto startShineRight = [=]() {
                this->beltRight->runAction("shine");
            };

            auto beltLeftMove = cocos2d::MoveTo::create(1, cocos2d::Point(windy::Display::getInstance().center.x + 13, this->beltLeft->getPositionY()));
            auto beltRightMove = cocos2d::MoveTo::create(1, cocos2d::Point(windy::Display::getInstance().center.x - 18, this->beltRight->getPositionY()));

            auto ringASequence =
                cocos2d::Sequence::create(beltLeftMove, cocos2d::CallFunc::create(startShineLeft), cocos2d::CallFunc::create(bossCallback), nullptr);

            auto ringBSequence =
                cocos2d::Sequence::create(beltRightMove, cocos2d::CallFunc::create(startShineRight), cocos2d::CallFunc::create(bossCallback), nullptr);

            ((cocos2d::Node*)this->beltLeft)->runAction(ringASequence);
            ((cocos2d::Node*)this->beltRight)->runAction(ringBSequence);
        };

        auto audioDelay = cocos2d::DelayTime::create(0.8f);
        auto audioCallback = cocos2d::CallFunc::create([=]() { windy::AudioManager::playSfx(windy::Sounds::BeltJoin); });

        auto audioSequence = cocos2d::Sequence::create(audioDelay, audioCallback, nullptr);

        auto audioSpawn = cocos2d::Spawn::create(cocos2d::CallFunc::create(beltMove), audioSequence, nullptr);

        this->runAction(audioSpawn);
    };

    auto shadowSequence = cocos2d::Sequence::create(shadowMove, cocos2d::DelayTime::create(0.5f), cocos2d::CallFunc::create(shadowCallback), nullptr);

    ((cocos2d::Node*)this->shadow)->runAction(shadowSequence);

    return true;
}

void Intro::onRingMoveComplete() {
    this->onIntroEnd();
}
