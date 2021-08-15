#include "GameItem.h"

#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"
#include "Windy/Sprite.h"

#include "Windy/GameTags.h"

#include "Game/GameManager.h"


using namespace game;

class GameItemResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string GameItemResources::spritePath = "sprites/gameplay/level/goods/item/item";
std::string GameItemResources::armaturePath = "physics/gameplay/level/goods/item/item";


void GameItem::preloadResources() {
    windy::Armature::cache(GameItemResources::armaturePath);
    windy::Sprite::cache(GameItemResources::spritePath);
}

void GameItem::setup(const std::string& name, const std::string& content, bool forever, bool collectible) {

    auto armature = windy::Armature(GameItemResources::armaturePath);

    Logical::composite<GameItem>(this, GameItemResources::armaturePath, GameItemResources::spritePath, "item");

    this->setTag(windy::GameTags::General::Item);

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("life",         "life",         true,   0.10f),
        windy::AnimationAction("helmet",       "helmet",       true,   0.10f),
        windy::AnimationAction("head",         "head",         true,   0.10f),
        windy::AnimationAction("chest",        "chest",        true,   0.10f),
        windy::AnimationAction("fist",         "fist",         true,   0.10f),
        windy::AnimationAction("boot",         "boot",         true,   0.10f),
        windy::AnimationAction("health_small", "health_small", true,   0.10f),
        windy::AnimationAction("health_big",   "health_big",   true,   0.10f),
        windy::AnimationAction("energy_small", "energy_small", true,   0.10f),
        windy::AnimationAction("energy_big",   "energy_big",   true,   0.10f),
        windy::AnimationAction("e_tank",       "e_tank",       true,   0.10f),
        windy::AnimationAction("m_tank",       "m_tank",       true,   0.10f)
    };

    this->sprite->appendActionSet(actionSet, false);

    for (int i = 0; i < GameManager::getInstance().items.collection.size(); ++i) {
        auto item = GameManager::getInstance().items.collection[i];
        if (content == item->string) {
            this->id = item->id;
        }
    }

    this->sprite->setAnimation(content);

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

    this->permanent = forever;

    this->name = name;
    this->collectible = collectible;
}

std::shared_ptr<cocos2d::Rect> GameItem::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle(position, size, GameItemResources::armaturePath, "item");
}

void GameItem::onFinished() {
    if (!this->permanent) {
        this->finishForever();
    }
    //this->finish();    
}
