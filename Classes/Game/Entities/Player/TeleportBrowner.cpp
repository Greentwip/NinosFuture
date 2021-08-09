#include "TeleportBrowner.h"

#include "Windy/Entities/Player.h"

#include "Windy/Sprite.h"

#include "Windy/AnimationAction.h"

#include "Game/GameManager.h"

using namespace game;

void TeleportBrowner::setBaseName() {
    this->baseName = "teleport";
}

void TeleportBrowner::loadActions() {

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("stand",        "teleport_stand",         true,   0.12f),
        windy::AnimationAction("jump",         "teleport_jump",          true,   0.12f)
    };

    this->player->sprite->appendActionSet(actionSet, true, this->baseName);

    this->brownerId = game::GameManager::getInstance().browners.teleport->id;
}

void TeleportBrowner::spawn() {
    this->energy = -1;
}


void TeleportBrowner::fire() {

}