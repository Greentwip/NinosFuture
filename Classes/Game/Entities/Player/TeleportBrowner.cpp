#include "TeleportBrowner.h"

#include "Windy/Entities/Player.h"

#include "Windy/Sprite.h"

#include "Windy/AnimationAction.h"

#include "Game/GameManager.h"

using namespace game;

void TeleportBrowner::initConstraints() {
    this->canWalk = false;
    this->canJump = true;
    this->canAttack = false;
    this->canWalkShoot = false;
    this->canJumpShoot = false;
    this->canClimb = false;
    this->canCharge = false;
    this->canSlide = false;
    this->canDashJump = false;
}

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
    this->energy = -2;
}

void TeleportBrowner::restoreWeaponEnergy(int amount) {
    this->energy = -2;
}

void TeleportBrowner::fire() {

}