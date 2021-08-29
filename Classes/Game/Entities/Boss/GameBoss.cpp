#include "GameBoss.h"

#include "Game/Entities/Player/TeleportBrowner.h"
#include "Game/Entities/Player/SheriffBrowner.h"
#include "Game/Entities/Player/MilitaryBrowner.h"
#include "Game/Entities/Player/VineBrowner.h"
#include "Game/Entities/Player/NightBrowner.h"
#include "Game/Entities/UI/EnergyBar.h"
#include "Game/Entities/UI/GameGui.h"
#include "Windy/GameTags.h"
#include "Windy/Entities/Browner.h"

using namespace game;

game::Resources& GameBoss::getResources() {
    static game::Resources resources{game::ResourceKind::Player, "browners"};
    return resources;
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

    Logical::composite<GameBoss>(this); //@TODO

    this->collisionRectangles[1]->origin = this->collisionRectangles[0]->origin;

    this->setTag(windy::GameTags::General::Boss);

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
    return windy::Logical::buildEntryCollisionRectangle<GameBoss>(position, size);
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
    auto militaryBrowner = windy::Browner::create<MilitaryBrowner>(this->level, this);
    auto vineBrowner = windy::Browner::create<VineBrowner>(this->level, this);
    auto nightBrowner = windy::Browner::create<NightBrowner>(this->level, this);

    this->addChild(teleportBrowner);
    this->addChild(sheriffBrowner);
    this->addChild(militaryBrowner);
    this->addChild(vineBrowner);
    this->addChild(nightBrowner);

    this->browners.pushBack(teleportBrowner);
    this->browners.pushBack(sheriffBrowner);
    this->browners.pushBack(militaryBrowner);
    this->browners.pushBack(vineBrowner);
    this->browners.pushBack(nightBrowner);

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
