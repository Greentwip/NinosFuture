#include "DebugDrawNode.h"

#include "Entities/Logical.h"
#include "Entities/Bounds.h"

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

    auto levelEntities = this->level->entities;
    auto customEntities = this->customEntities;

    cocos2d::Vector<Logical*> screenEntities;

    for (int i = 0; i < levelEntities.size(); ++i) {
        auto entity = levelEntities.at(i);

        auto landscapeCollisionBox = entity->collisionBox;
        auto boundsCollisionBox = this->level->bounds->collisionBox;

        if (boundsCollisionBox->intersectsRect(*landscapeCollisionBox)) {
            screenEntities.pushBack(entity);
        }
    }

    cocos2d::Vector<Logical*> screenCustomEntities;

    for (int i = 0; i < customEntities.size(); ++i) {
        auto entity = customEntities.at(i);

        auto landscapeCollisionBox = entity->collisionBox;
        auto boundsCollisionBox = this->level->bounds->collisionBox;

        if (boundsCollisionBox->intersectsRect(*landscapeCollisionBox)) {
            screenCustomEntities.pushBack(entity);
        }
    }




    for (int i = 0; i < screenEntities.size(); ++i) {
        auto entity = screenEntities.at(i);

        auto collisionRectangle = entity->collisionBox;

        cocos2d::ccDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);

        float minX = collisionRectangle->getMinX();
        float maxX = collisionRectangle->getMaxX();

        float minY = collisionRectangle->getMinY();
        float maxY = collisionRectangle->getMaxY();


        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(minX, maxY), cocos2d::Point(maxX, maxY));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(maxX, maxY), cocos2d::Point(maxX, minY));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(maxX, minY), cocos2d::Point(minX, minY));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(minX, minY), cocos2d::Point(minX, maxY));

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

    for (int i = 0; i < screenCustomEntities.size(); ++i) {
        auto entity = screenCustomEntities.at(i);

        auto collisionRectangle = entity->collisionBox;

        cocos2d::ccDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);

        float minX = collisionRectangle->getMinX();
        float maxX = collisionRectangle->getMaxX();

        float minY = collisionRectangle->getMinY();
        float maxY = collisionRectangle->getMaxY();


        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(minX, maxY), cocos2d::Point(maxX, maxY));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(maxX, maxY), cocos2d::Point(maxX, minY));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(maxX, minY), cocos2d::Point(minX, minY));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(minX, minY), cocos2d::Point(minX, maxY));

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

    {
        auto bounds = this->level->bounds;

        auto collisionRectangle = bounds->collisionBox;

        cocos2d::ccDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);

        float minX = collisionRectangle->getMinX();
        float maxX = collisionRectangle->getMaxX();

        float minY = collisionRectangle->getMinY();
        float maxY = collisionRectangle->getMaxY();


        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(minX + 2, maxY - 2), cocos2d::Point(maxX - 2, maxY - 2));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(maxX - 2, maxY - 2), cocos2d::Point(maxX - 2, minY + 2));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(maxX - 2, minY + 2), cocos2d::Point(minX + 2, minY + 2));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(minX + 2, minY + 2), cocos2d::Point(minX + 2, maxY - 2));

        auto entityPosition = bounds->getPosition();
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
    
    /*{
        auto bounds = this->level->bounds;

        auto collisionRectangle = bounds->inflate(cocos2d::Size(16, 16));

        collisionRectangle.origin += cocos2d::Point(64, 64);

        cocos2d::ccDrawColor4F(0.0f, 0.0f, 1.0f, 1.0f);

        float minX = collisionRectangle.getMinX();
        float maxX = collisionRectangle.getMaxX();

        float minY = collisionRectangle.getMinY();
        float maxY = collisionRectangle.getMaxY();


        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(minX + 2, maxY - 2), cocos2d::Point(maxX - 2, maxY - 2));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(maxX - 2, maxY - 2), cocos2d::Point(maxX - 2, minY + 2));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(maxX - 2, minY + 2), cocos2d::Point(minX + 2, minY + 2));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(minX + 2, minY + 2), cocos2d::Point(minX + 2, maxY - 2));

        auto entityPosition = bounds->getPosition();
        float entityX = entityPosition.x;
        float entityY = entityPosition.y;
    }


    {
        auto bounds = this->level->bounds;

        auto collisionRectangle = bounds->inflate(cocos2d::Size(48, 48));

        collisionRectangle.origin += cocos2d::Point(64, 64);

        cocos2d::ccDrawColor4F(0.0f, 1.0f, 1.0f, 1.0f);

        float minX = collisionRectangle.getMinX();
        float maxX = collisionRectangle.getMaxX();

        float minY = collisionRectangle.getMinY();
        float maxY = collisionRectangle.getMaxY();


        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(minX + 2, maxY - 2), cocos2d::Point(maxX - 2, maxY - 2));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(maxX - 2, maxY - 2), cocos2d::Point(maxX - 2, minY + 2));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(maxX - 2, minY + 2), cocos2d::Point(minX + 2, minY + 2));
        cocos2d::DrawPrimitives::drawLine(cocos2d::Point(minX + 2, minY + 2), cocos2d::Point(minX + 2, maxY - 2));

        auto entityPosition = bounds->getPosition();
        float entityX = entityPosition.x;
        float entityY = entityPosition.y;
    }*/

    



	
}