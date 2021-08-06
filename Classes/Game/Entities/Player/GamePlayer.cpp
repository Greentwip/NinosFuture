#include "GamePlayer.h"

#include "Windy/Entities/Browner.h"

#include "VioletBrowner.h"

using namespace game;

void GamePlayer::setupBrowners() {
    this->currentBrowner = windy::Browner::create<VioletBrowner>(this->level, this);
    this->currentBrowner->runAction("jump");

    this->addChild(this->currentBrowner);
}
