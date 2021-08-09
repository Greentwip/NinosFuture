#include <string>

#include "EnergyBar.h"
#include "Windy/GameTags.h"

#include "Windy/Armature.h"
#include "Windy/Sprite.h"

using namespace game;

class EnergyBarResources {
public:
    static std::string spritePath;
    static std::string cellPath;
};

std::string EnergyBarResources::spritePath = "sprites/gameplay/level/ui/energy_bar/bar/bar";
std::string EnergyBarResources::cellPath = "sprites/gameplay/level/ui/energy_bar/meter/meter";


void EnergyBar::preloadResources() {
    windy::Sprite::cache(EnergyBarResources::spritePath);
    windy::Sprite::cache(EnergyBarResources::cellPath);
}

bool EnergyBar::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->sprite = windy::Sprite::create(EnergyBarResources::spritePath, cocos2d::Point(0, 1));

    this->sprite->setPosition(0, 0);

    this->addChild(this->sprite);

    float positionX = 1;

    float positionY = this->sprite->getContentSize().height * -1;

    for (int i = 0; i < 28; ++i) {
        positionY += 2;

        auto cell = windy::Sprite::create(EnergyBarResources::cellPath, cocos2d::Point(0, 1));

        cell->setPosition(positionX, positionY);

        this->addChild(cell);
        this->cells.pushBack(cell);
    }

    this->meter = 28;

    this->setTag(windy::GameTags::Gui::BarMeter);

    return true;
}


void EnergyBar::setValue(int value) {

    if (value != this->meter && value > -2) {
        if (!this->isVisible()) {
            this->setVisible(true);
        }

        if (value <= 0) {
            for (int i = 0; i < this->cells.size(); ++i) {
                this->cells.at(i)->setVisible(false);
            }
        }
        else if (value >= this->cells.size()) {
            for (int i = 0; i < this->cells.size(); ++i) {
                this->cells.at(i)->setVisible(true);
            }
        }
        else {
            for (int i = 0; i < this->cells.size(); ++i) {
                if (i <= value) {
                    this->cells.at(i)->setVisible(true);
                }
                else {
                    this->cells.at(i)->setVisible(false);
                }
                
            }
        }

        this->meter = value;
    }
    else if(value <= -2) {
        this->setVisible(false);
        this->meter = value;
    }
}

int EnergyBar::getValue() {
    return this->meter;
}