#include "Logical.h"

#include "ObjectEntry.h"

using namespace windy;


void Logical::setup(const cocos2d::Point& position, const cocos2d::Size& size) {
    this->setPosition(position);
    this->lastPosition = position;
    this->collisionRectangles.push_back(std::make_shared<cocos2d::Rect>(position.x - size.width * 0.5f, position.y + size.height * 0.5f, size.width, size.height));
    this->collisionBox = this->collisionRectangles[0];
    this->ignoreGravity = false;
    this->ignoreLandscapeCollision = false;
}


bool Logical::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }
    return true;
}

std::shared_ptr<cocos2d::Rect> Logical::normalizeCollisionRectangle(const cocos2d::Point& nodePosition, cocos2d::Rect rectangle) {
    auto rectangleOrigin = rectangle.origin;

    auto normalizedRectangle = std::make_shared<cocos2d::Rect>(rectangle);
    normalizedRectangle->origin.x = nodePosition.x + rectangleOrigin.x;
    normalizedRectangle->origin.y = nodePosition.y + rectangleOrigin.y;

    return normalizedRectangle;
}



std::shared_ptr<cocos2d::Rect> Logical::normalizeCollisionRectangle(cocos2d::Node* node, cocos2d::Rect rectangle) {
    auto nodePosition = node->getPosition();
    auto rectangleOrigin = rectangle.origin;

    auto normalizedRectangle = std::make_shared<cocos2d::Rect>(rectangle);
    normalizedRectangle->origin.x = nodePosition.x + rectangleOrigin.x;
    normalizedRectangle->origin.y = nodePosition.y + rectangleOrigin.y;

    return normalizedRectangle;
}


void Logical::setEntry(std::shared_ptr<ObjectEntry> entry) {
    this->entry = entry;
}


std::shared_ptr<ObjectEntry> Logical::getEntry(std::shared_ptr<cocos2d::Rect> collisionRectangle,
                                                      std::function<Logical* ()> createFunction){

    return std::make_shared<ObjectEntry>(collisionRectangle, createFunction);
}


void Logical::onEnter()
{
    Node::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void Logical::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Node::onExit();
}

void Logical::recomputeCollisionRectangles() {
    auto positionDifference = this->getPosition() - lastPosition;
    lastPosition = this->getPosition();

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        float differenceX = positionDifference.x;
        float differenceY = positionDifference.y;

        this->collisionRectangles[i]->origin.x += differenceX;
        this->collisionRectangles[i]->origin.y += differenceY;
    }

}

void Logical::finish() {
    this->stopAllActions();
    this->entry->finished = true;
}

void Logical::finishForever() {
    this->stopAllActions();
    this->entry->finishedForever = true;
}

void Logical::onFinished() {

}

void Logical::onCollisionEnter(Logical* collision) {

}

void Logical::onCollision(Logical* collision) {

}

void Logical::onCollisionExit(Logical* collision) {

}

void Logical::update(float dt)
{
    recomputeCollisionRectangles();

    this->onUpdate(dt);
}
