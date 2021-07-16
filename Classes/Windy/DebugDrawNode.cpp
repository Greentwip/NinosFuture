#include "DebugDrawNode.h"

#include "Entities/Logical.h"

#include "Level.h"

using namespace windy;

DebugDrawNode* DebugDrawNode::create(windy::Level* level) {
    DebugDrawNode* drawNode = new (std::nothrow) DebugDrawNode();

    if (drawNode) {
        drawNode->level = level;
    }

    if (drawNode && drawNode->init()) {
        drawNode->autorelease();
        return drawNode;
    }

    CC_SAFE_DELETE(drawNode);
    return nullptr;

}

bool DebugDrawNode::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    return true;
}


void DebugDrawNode::onEnter()
{
    Node::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void DebugDrawNode::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Node::onExit();
}

void DebugDrawNode::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) {
    renderCommand.init(16384);
    renderCommand.func = CC_CALLBACK_0(DebugDrawNode::onDraw, this, transform, flags);
	renderer->addCommand(&renderCommand);
}

void DebugDrawNode::onDraw(const cocos2d::Mat4& transform, uint32_t flags)
{
	cocos2d::Director* director = cocos2d::Director::getInstance();
	director->pushMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

    auto entities = this->level->entities;
    for (int i = 0; i < entities.size(); ++i) {
        auto entity = entities.at(i);

        auto collisionRectangles = entity->collisionRectangles;

        cocos2d::ccDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);


        for (int j = 0; j < collisionRectangles.size(); ++j) {
            auto collisionRectangle = collisionRectangles[j];

            float minX = collisionRectangle.getMinX();
            float maxX = collisionRectangle.getMaxX();

            float minY = collisionRectangle.getMinY();
            float maxY = collisionRectangle.getMaxY();


            cocos2d::DrawPrimitives::drawLine(cocos2d::Point(minX, maxY), cocos2d::Point(maxX, maxY));
            cocos2d::DrawPrimitives::drawLine(cocos2d::Point(maxX, maxY), cocos2d::Point(maxX, minY));
            cocos2d::DrawPrimitives::drawLine(cocos2d::Point(maxX, minY), cocos2d::Point(minX, minY));
            cocos2d::DrawPrimitives::drawLine(cocos2d::Point(minX, minY), cocos2d::Point(minX, maxY));
        }

        auto entityPosition = entity->getPosition();
        float entityX = entityPosition.x;
        float entityY = entityPosition.y;


        cocos2d::ccDrawColor4F(1.0f, 0.0f, 1.0f, 1.0f);

        //cocos2d::DrawPrimitives::drawCircle(entityPosition, 2, 0, 0, false);
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(entityX - 2, entityY + 2),
                                          cocos2d::Point(entityX + 2, entityY + 2));

        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(entityX + 2, entityY + 2), 
                                          cocos2d::Point(entityX + 2, entityY - 2));

        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(entityX + 2, entityY - 2), 
                                          cocos2d::Point(entityX - 2, entityY - 2));

        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(entityX - 2, entityY - 2), 
                                          cocos2d::Point(entityX - 2, entityY + 2));

    }
	
}