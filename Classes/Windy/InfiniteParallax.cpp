#include "InfiniteParallax.h"

#include "Sprite.h"


using namespace windy;

InfiniteParallax* InfiniteParallax::create(const std::string& filePath, const cocos2d::Point& speed) {

    InfiniteParallax* infiniteParallax = new (std::nothrow) InfiniteParallax();

    if (infiniteParallax && infiniteParallax->init()) {
        infiniteParallax->autorelease();

        infiniteParallax->parallax = Sprite::create(filePath, cocos2d::Point(1, 0.5f));
        infiniteParallax->scroller = Sprite::create(filePath, cocos2d::Point(1, 0.5f));

        infiniteParallax->scroller->setPositionY(-infiniteParallax->parallax->getContentSize().height);

        infiniteParallax->addChild(infiniteParallax->parallax);
        infiniteParallax->addChild(infiniteParallax->scroller);

        infiniteParallax->speed = speed;

        infiniteParallax->spriteLimit = cocos2d::Point(0, infiniteParallax->parallax->getContentSize().height);
        infiniteParallax->scrollerStartup = cocos2d::Point(0, -infiniteParallax->parallax->getContentSize().height);


        return infiniteParallax;
    }

    CC_SAFE_DELETE(infiniteParallax);
    return nullptr;
}

bool InfiniteParallax::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }
    return true;
}


void InfiniteParallax::onEnter()
{
    Node::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void InfiniteParallax::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Node::onExit();
}

void InfiniteParallax::horizontalScroll() {

}

void InfiniteParallax::verticalScroll() {

    this->parallax->setPositionY(this->parallax->getPositionY() + this->speed.y);
    this->scroller->setPositionY(this->scroller->getPositionY() + this->speed.y);

    if (this->parallax->getPositionY() >= this->spriteLimit.y) {
        this->parallax->setPositionY(this->scrollerStartup.y);
    }

    if (this->scroller->getPositionY() >= this->spriteLimit.y) {
        this->scroller->setPositionY(this->scrollerStartup.y);
    }
}

void InfiniteParallax::update(float dt)
{
    if (this->speed.x != 0) {
        this->horizontalScroll();
    }

    if (this->speed.y != 0) {
        this->verticalScroll();
    }
}
