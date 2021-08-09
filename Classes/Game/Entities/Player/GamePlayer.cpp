#include "GamePlayer.h"

#include "Windy/Entities/Browner.h"

#include "Windy/Armature.h"
#include "Windy/Sprite.h"

#include "Windy/GameTags.h"

#include "Game/Entities/UI/GameGui.h"

#include "VioletBrowner.h"



using namespace game;


class PlayerResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string PlayerResources::spritePath = "sprites/characters/player/regular/browners/browners";
std::string PlayerResources::armaturePath = "physics/characters/player/regular/browners/browners";

void GamePlayer::preloadResources() {
    windy::Armature::cache(PlayerResources::armaturePath);
    windy::Sprite::cache(PlayerResources::spritePath);
}


bool GamePlayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->gui = nullptr;

    auto armature = windy::Armature(PlayerResources::armaturePath);

    auto newAnchor = armature.get("browners").anchor;

    this->sprite = windy::Sprite::create(PlayerResources::spritePath, newAnchor);
    this->addChild(this->sprite);

    auto anchorChange = newAnchor - cocos2d::Point(0.5f, 0.5f);
    auto contentSize = this->sprite->getContentSize();

    this->collisionRectangles = armature.get("browners").collisionRectangles;

    auto collisionBoxCenter = cocos2d::Point(this->collisionRectangles[0]->getMidX(), this->collisionRectangles[0]->getMidY());

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(this, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];


    this->sprite->setPosition(collisionBoxCenter + cocos2d::Point(contentSize.width * anchorChange.x, contentSize.height * anchorChange.y));

    this->setTag(windy::GameTags::General::Player);

    this->alive = true;
    this->canMove = true;
    this->spawning = false;

    this->initVariables();

    this->setupBrowners();


    return true;
}


void GamePlayer::setupBrowners() {
    this->currentBrowner = windy::Browner::create<VioletBrowner>(this->level, this);
    this->currentBrowner->runAction("jump");

    this->addChild(this->currentBrowner);
}
