#include <string>

#include "GameGui.h"
#include "EnergyBar.h"

#include "Windy/Level.h"
#include "Windy/Entities/Bounds.h"

#include "Windy/EntityFactory.h"

#include "Game/Entities/Player/GamePlayer.h"

using namespace game;

void GameGui::build()
{
    auto barBasePosition = cocos2d::Point(this->level->bounds->collisionBox->size.width * -0.5f, this->level->bounds->collisionBox->size.height * 0.5f);

    auto healthBarPosition = barBasePosition + cocos2d::Point(16, -16);
    auto weaponBarPosition = barBasePosition + cocos2d::Point(8, -16);

    this->healthBar = dynamic_cast<EnergyBar*>(windy::EntityFactory::getInstance().create("energy_bar", healthBarPosition, cocos2d::Size(16, 16)));
    this->weaponBar = dynamic_cast<EnergyBar*>(windy::EntityFactory::getInstance().create("energy_bar", weaponBarPosition, cocos2d::Size(16, 16)));

    this->addChild(this->healthBar);
    this->addChild(this->weaponBar);


    auto gamePlayer = dynamic_cast<GamePlayer*>(this->level->player);

    gamePlayer->gui = this;
}

