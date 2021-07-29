#include "Item.h"

#include "./../Sprite.h"
#include "./../AnimationAction.h"
#include "./../Armature.h"
#include "./../GameTags.h"



#include "Game/GameManager.h"

using namespace windy;

class ItemResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string ItemResources::spritePath = "sprites/gameplay/level/goods/item/item";
std::string ItemResources::armaturePath = "physics/gameplay/level/goods/item/item";

bool Item::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->id = -1;

    this->setTag(GameTags::General::Item);

    return true;
}


void Item::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Item::setup(const std::string& content, bool forever) {

    auto armature = Armature(ItemResources::armaturePath);

    auto newAnchor = armature.get("item").anchor;

    this->sprite = Sprite::create(ItemResources::spritePath, newAnchor);
    this->addChild(this->sprite);

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = this->sprite->getContentSize();

    this->collisionRectangles = armature.get("item").collisionRectangles;

    auto collisionBoxCenter = cocos2d::Point(this->collisionRectangles[0]->getMidX(), this->collisionRectangles[0]->getMidY());

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(this, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];


    this->sprite->setPosition(collisionBoxCenter + cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));

    this->setTag(GameTags::General::Item);

    std::vector<AnimationAction> actionSet = {
        AnimationAction("life",         "life",         true,   0.10f),
        AnimationAction("helmet",       "helmet",       true,   0.10f),
        AnimationAction("head",         "head",         true,   0.10f),
        AnimationAction("chest",        "chest",        true,   0.10f),
        AnimationAction("fist",         "fist",         true,   0.10f),
        AnimationAction("boot",         "boot",         true,   0.10f),
        AnimationAction("health_small", "health_small", true,   0.10f),
        AnimationAction("health_big",   "health_big",   true,   0.10f),
        AnimationAction("energy_small", "energy_small", true,   0.10f),
        AnimationAction("energy_big",   "energy_big",   true,   0.10f),
        AnimationAction("e_tank",       "e_tank",       true,   0.10f),
        AnimationAction("m_tank",       "m_tank",       true,   0.10f)
    };

    this->sprite->appendActionSet(actionSet, false);


    for (int i = 0; i < game::GameManager::getInstance().items.collection.size(); ++i) {
        auto item = game::GameManager::getInstance().items.collection[i];
        if (content == item->string) {
            this->id = item->id;
        }
    }

    if (forever) {
        this->sprite->runAction(content);
    }
    else {
        this->sprite->runAction(content);

        auto delay = cocos2d::DelayTime::create(4);

        auto blink = cocos2d::Blink::create(2, 20);

        auto callback = cocos2d::CallFunc::create([this]() { this->finishForever(); });

        auto sequence = cocos2d::Sequence::create(delay, blink, callback, nullptr);

        ((cocos2d::Node*)this)->runAction(sequence);
    }
}

std::shared_ptr<cocos2d::Rect> Item::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {

    auto armature = Armature(ItemResources::armaturePath);

    auto newAnchor = armature.get("item").anchor;

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);

    auto collisionRectangles = armature.get("item").collisionRectangles;

    for (int i = 0; i < collisionRectangles.size(); ++i) {
        collisionRectangles[i] = Logical::normalizeCollisionRectangle(position, *collisionRectangles[i]);
    }

    return collisionRectangles[0];
}


void Item::onUpdate(float dt) {

}