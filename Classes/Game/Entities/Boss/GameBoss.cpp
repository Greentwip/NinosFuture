#include "GameBoss.h"

#include "Windy/Entities/Browner.h"
#include "Windy/Armature.h"
#include "Windy/Sprite.h"
#include "Windy/GameTags.h"

#include "Game/Entities/UI/GameGui.h"
#include "Game/Entities/UI/EnergyBar.h"
#include "Game/GameManager.h"

#include "Game/Entities/Player/TeleportBrowner.h"
#include "Game/Entities/Player/SheriffBrowner.h"

using namespace game;

class GameBossResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string GameBossResources::spritePath = "sprites/characters/player/regular/browners/browners";
std::string GameBossResources::armaturePath = "physics/characters/player/regular/browners/browners";

void GameBoss::preloadResources() {
    windy::Armature::cache(GameBossResources::armaturePath);
    windy::Sprite::cache(GameBossResources::spritePath);
}


bool GameBoss::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->gui = nullptr;
    auto armature = windy::Armature(GameBossResources::armaturePath);

    auto newAnchor = armature.get("browners").anchor;

    this->sprite = windy::Sprite::create(GameBossResources::spritePath, newAnchor);
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

    this->initVariables();

    this->health = 0;

    this->setupBrowners();

    this->level->boss = this;

    this->gui = dynamic_cast<GameGui*>(this->level->gui);

    this->weaponTag = windy::GameTags::WeaponEnemy;

    this->sprite->setFlippedX(true);
    
    return true;
}

std::shared_ptr<cocos2d::Rect> GameBoss::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return windy::Logical::buildEntryCollisionRectangle(position, size, GameBossResources::armaturePath, "browners");
}

void GameBoss::onFinished() {
    this->level->boss = nullptr;
}

void GameBoss::switchBrowner(int brownerId) {
    for (int i = 0; i < this->browners.size(); ++i) {
        auto browner = this->browners.at(i);
        if (browner->brownerId == brownerId) {
            this->currentBrowner = browner;
        }
    }
}

void GameBoss::setupBrowners() {
    auto teleportBrowner = windy::Browner::create<TeleportBrowner>(this->level, this);
    auto sheriffBrowner = windy::Browner::create<SheriffBrowner>(this->level, this);

    this->addChild(teleportBrowner);
    this->addChild(sheriffBrowner);

    this->browners.pushBack(teleportBrowner);
    this->browners.pushBack(sheriffBrowner);

    this->currentBrowner = teleportBrowner;

    this->currentBrowner->runAction("jump");
}

windy::Browner* GameBoss::getBrowner(int brownerId) {
    windy::Browner* brownerFound = nullptr;
    for (int i = 0; i < this->browners.size(); ++i) {
        auto browner = this->browners.at(i);
        if (browner->brownerId == brownerId) {
            brownerFound = browner;
            break;
        }
    }

    return brownerFound;
}


void GameBoss::walk() {

    if (this->speed.y == 0) {
        if (this->speed.x != 0) {
            this->walking = true;
        }
        else {
            this->walking = false;
        }
    }
}

void GameBoss::jump() {
    if (this->speed.y != 0) {
        this->jumping = true;
    }
    else {
        this->jumping = false;
    }

}


void GameBoss::kill(bool killAnimation) {

    if (killAnimation) {

        auto explosionA = cocos2d::CallFunc::create([this]() {
            this->explode(0);
            });

        auto delay = cocos2d::DelayTime::create(0.40f);

        auto explosionB = cocos2d::CallFunc::create([this]() {
            this->explode(22.5f);
            });

        auto killDelay = cocos2d::DelayTime::create(2);

        auto lifeCallback = cocos2d::CallFunc::create([=]() {

            this->finish();
            //this->level->restart();
            });

        auto sequence = cocos2d::Sequence::create(explosionA, delay, explosionB, killDelay, lifeCallback, nullptr);

        this->runAction(sequence);
    }
    else {
        this->sprite->setVisible(false);


        auto killDelay = cocos2d::DelayTime::create(2);

        auto lifeCallback = cocos2d::CallFunc::create([=]() {
            this->finish();
            //this->level->restart();
            });

        auto sequence = cocos2d::Sequence::create(killDelay, lifeCallback, nullptr);

        this->runAction(sequence);

    }

}


void GameBoss::restoreHealth(int amount) {
    this->gui->restoreBossHealth(amount);
}

void GameBoss::checkHealth() {
    
    switch (this->state) {
        case BossState::Teleporting:
        case BossState::Intro:
        case BossState::Spawning:
            this->gui->bossHealthBar->setValue(-1);
            break;

        case BossState::RestoringHealth:
        {
            if (!this->gui->fillingBossHealthBar) {
                this->health = this->maxHealth;
            }
        }       
        break;

        case BossState::Fighting: 
        {
            this->gui->bossHealthBar->setValue(this->health - 1);
        }
        break;
    }

    Boss::checkHealth();
}

void GameBoss::onUpdate(float dt) {
    Boss::onUpdate(dt);
}

