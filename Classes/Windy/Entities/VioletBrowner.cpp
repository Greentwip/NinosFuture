#include "VioletBrowner.h"

#include "Player.h"

#include "./../Sprite.h"

#include "./../AnimationAction.h"

#include "Game/GameManager.h"

using namespace windy;

void VioletBrowner::setBaseName() {
    this->baseName = "violet";
}

void VioletBrowner::loadActions() {

    std::vector<AnimationAction> actionSet = {
        AnimationAction("stand",        "violet_stand",         true,   0.10f),
        AnimationAction("slide",        "violet_slide",         true,   0.10f),
        AnimationAction("climb",        "violet_climb",         true,   0.16f),
        AnimationAction("jump",         "violet_jump",          true,   0.10f),
        AnimationAction("dashjump",     "violet_dashjump",      true,   0.10f),
        AnimationAction("walk",         "violet_walk",          true,   0.12f),
        AnimationAction("standshoot",   "violet_standshoot",    true,   0.10f),
        AnimationAction("walkshoot",    "violet_walkshoot",     true,   0.12f),
        AnimationAction("jumpshoot",    "violet_jumpshoot",     true,   0.10f),
        AnimationAction("climbshoot",   "violet_climbshoot",    true,   0.10f),
        AnimationAction("hurt",         "violet_hurt",          true,   0.02f),
        AnimationAction("morph",        "violet_morph",         true,   0.10f)
    };

    this->player->sprite->appendActionSet(actionSet, true, this->baseName);

    this->brownerId = game::GameManager::getInstance().browners.violet->id;
}

void VioletBrowner::spawn() {
    this->energy = -1;
}
