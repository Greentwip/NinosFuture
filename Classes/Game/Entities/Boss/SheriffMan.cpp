#include "SheriffMan.h"

#include "Windy/Entities/Browner.h"

#include "Game/GameManager.h"

using namespace game;

void SheriffMan::switchToBattleBrowner() {
    this->switchBrowner(GameManager::getInstance().browners.sheriff->id);
}

void SheriffMan::onSpawn() {
    this->state = BossState::Fighting;
}

void SheriffMan::onUpdate(float dt) {
    GameBoss::onUpdate(dt);

    if (this->state == BossState::Fighting) {


    }
    
}

