#include <string>

#include "PauseMenu.h"

#include "PauseInterruptor.h"
#include "PauseAnimation.h"
#include "PauseSelector.h"

#include "Windy/Sprite.h"
#include "Windy/Label.h"
#include "Windy/Display.h"
#include "Windy/EntityFactory.h"
#include "Windy/AudioManager.h"

#include "Windy/Entities/Player.h"
#include "Windy/Entities/Browner.h"

#include "Game/GameManager.h"

#include "Game/Entities/UI/EnergyBar.h"
#include "Game/Entities/UI/GameGui.h"


using namespace game;

class PauseMenuResources {
public:
    static std::string backgroundPath;
    static std::string boxPath;
    static std::string font;
};

std::string PauseMenuResources::backgroundPath = "sprites/gameplay/screens/pause_menu/pause_background/pause_background";
std::string PauseMenuResources::boxPath = "sprites/gameplay/screens/pause_menu/pause_background/pause_box";
std::string PauseMenuResources::font = "fonts/megaman_2.ttf";


void PauseMenu::preloadResources() {
    windy::Sprite::cache(PauseMenuResources::backgroundPath);
    windy::Sprite::cache(PauseMenuResources::boxPath);
}

PauseMenu* PauseMenu::create(windy::Player* player, GameGui* gui, windy::Level* level) {

    PauseMenu* menu = new (std::nothrow) PauseMenu();

    if (menu) {
        menu->player = player;
        menu->gui = gui;
        menu->_level = level;
        menu->busy = false;
        menu->background = nullptr;
        menu->box = nullptr;
        menu->selector = nullptr;
        menu->eTank = nullptr;
        menu->mTank = nullptr;
        menu->livesLabel = nullptr;
        menu->eTanksLabel = nullptr;
        menu->mTanksLabel = nullptr;
        menu->exSwitch = nullptr;
        menu->helmetSwitch = nullptr;
        menu->exitSwitch = nullptr;
        menu->health = nullptr;
        menu->weaponAnimation = nullptr;
        menu->fillingMTank = false;
        menu->defaultBrowner = nullptr;
        menu->selectedBrowner = nullptr;
        menu->onETankTriggered = nullptr;
        menu->onMTankTriggered = nullptr;
        menu->onBrownerTriggered = nullptr;
        menu->onSwitchTriggered = nullptr;
    }

    if (menu && menu->init()) {
        menu->autorelease();
        return menu;
    }

    CC_SAFE_DELETE(menu);
    return nullptr;
}

bool PauseMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->background = windy::Sprite::create(PauseMenuResources::backgroundPath, cocos2d::Point(0, 1));
    this->background->setPosition(cocos2d::Point(0, 0));
    this->addChild(this->background);

    this->box = windy::Sprite::create(PauseMenuResources::boxPath, cocos2d::Point(0, 0));
    this->box->setPosition(cocos2d::Point(0, this->background->getContentSize().height * -1));
    this->addChild(this->box);

    this->selector = PauseSelector::create(PauseSelector::SelectorOrientation::OrientationDown);
    this->selector->setPosition(cocos2d::Point(0, 0));
    this->addChild(this->selector, 24);

    this->setupBrowners();

    this->eTank = PauseInterruptor::create("e_tank", nullptr);
    this->eTank->setPosition(cocos2d::Point(12, -198));
    this->addChild(this->eTank);

    this->mTank = PauseInterruptor::create("m_tank", nullptr);
    this->mTank->setPosition(cocos2d::Point(60, this->eTank->getPositionY()));
    this->addChild(this->mTank);

    auto lives = PauseInterruptor::create("life", nullptr);
    lives->setPosition(cocos2d::Point(112, this->mTank->getPositionY()));
    lives->visitInterruptor();
    this->addChild(lives);


    int numLives = GameManager::getInstance().player.lives;
    int numETanks = GameManager::getInstance().player.eTanks;
    int numMTanks = GameManager::getInstance().player.mTanks;


    this->livesLabel =
        windy::Label::create(
            "0" + std::to_string(numLives),
            PauseMenuResources::font,
            8,
            cocos2d::TextHAlignment::LEFT,
            cocos2d::TextVAlignment::TOP);

    this->livesLabel->setPosition(lives->sprite->getContentSize().width + 1, (lives->sprite->getContentSize().height * 0.5f * -1) + 1);
    lives->addChild(this->livesLabel);


    this->eTanksLabel =
        windy::Label::create(
            "0" + std::to_string(numETanks),
            PauseMenuResources::font,
            8,
            cocos2d::TextHAlignment::LEFT,
            cocos2d::TextVAlignment::TOP);

    this->eTanksLabel->setPosition(this->eTank->sprite->getContentSize().width + 1, (this->eTank->sprite->getContentSize().height * 0.5f * -1) + 1);
    this->eTank->addChild(this->eTanksLabel);

    this->mTanksLabel =
        windy::Label::create(
            "0" + std::to_string(numMTanks),
            PauseMenuResources::font,
            8,
            cocos2d::TextHAlignment::LEFT,
            cocos2d::TextVAlignment::TOP);

    this->mTanksLabel->setPosition(this->mTank->sprite->getContentSize().width + 1, (this->mTank->sprite->getContentSize().height * 0.5f * -1) + 1);
    this->mTank->addChild(this->mTanksLabel);

    auto head = PauseInterruptor::create("extreme_helmet", nullptr);
    head->setPosition(cocos2d::Point(192, -185));
    this->addChild(head);

    auto fist = PauseInterruptor::create("extreme_fist", nullptr);
    fist->setPosition(cocos2d::Point(head->getPositionX() - 18, head->getPositionY() - 18));
    this->addChild(fist);

    auto chest = PauseInterruptor::create("extreme_chest", nullptr);
    chest->setPosition(cocos2d::Point(fist->getPositionX() + 18, fist->getPositionY()));
    this->addChild(chest);

    auto boot = PauseInterruptor::create("extreme_boot", nullptr);
    boot->setPosition(cocos2d::Point(chest->getPositionX() + 18, chest->getPositionY()));
    this->addChild(boot);

    if (GameManager::getInstance().unlockables.head->acquired) {
        head->visitInterruptor();
    }

    if (GameManager::getInstance().unlockables.fist->acquired) {
        fist->visitInterruptor();
    }

    if (GameManager::getInstance().unlockables.chest->acquired) {
        chest->visitInterruptor();
    }

    if (GameManager::getInstance().unlockables.boot->acquired) {
        boot->visitInterruptor();
    }

    this->exSwitch = PauseInterruptor::create("ex", nullptr);
    this->exSwitch->setVisitable(false);
    this->exSwitch->setPosition(cocos2d::Point(64, -144));
    this->addChild(this->exSwitch);

    this->helmetSwitch = PauseInterruptor::create("helmet", nullptr);
    this->helmetSwitch->setVisitable(false);
    this->helmetSwitch->setPosition(cocos2d::Point(this->exSwitch->getPositionX() - 48, this->exSwitch->getPositionY()));
    this->addChild(this->helmetSwitch);

    this->exitSwitch = PauseInterruptor::create("exit", nullptr);
    this->exitSwitch->setPosition(cocos2d::Point(this->exSwitch->getPositionX() + 48, this->exSwitch->getPositionY()));
    this->addChild(this->exitSwitch);

    this->weaponAnimation = PauseAnimation::create();
    this->weaponAnimation->setPosition(
        static_cast<float>(windy::Display::getInstance().width / 4),
        static_cast<float>(((windy::Display::getInstance().height * -1) / 2) + 5));
    this->weaponAnimation->setAnimation(this->defaultBrowner->pauseItem);
    this->weaponAnimation->swapContents(this->defaultBrowner->pauseItem);
    this->addChild(this->weaponAnimation);

    this->menuItems.push_back(this->eTank);
    this->menuItems.push_back(this->mTank);
    this->menuItems.push_back(this->exSwitch);
    this->menuItems.push_back(this->helmetSwitch);
    this->menuItems.push_back(this->exitSwitch);

    this->health = PauseInterruptor::create("life", nullptr);
    this->health->setPosition(
        cocos2d::Point(
            this->weaponAnimation->getPositionX() - this->weaponAnimation->sprite->getContentSize().width * 0.5f, 
            this->weaponAnimation->getPositionY() - this->health->sprite->getContentSize().height * 0.25f));
    this->health->visitInterruptor();
    this->addChild(this->health);

    auto healthLabel =
        windy::Label::create(
            "HEALTH",
            PauseMenuResources::font,
            8,
            cocos2d::TextHAlignment::LEFT,
            cocos2d::TextVAlignment::TOP);

    healthLabel->setPosition(this->health->sprite->getContentSize().width + 1, 0);
    this->health->addChild(healthLabel);

    auto healthBar =
        dynamic_cast<EnergyBar*>
        (windy::EntityFactory::getInstance().create(
            "energy_bar",
            cocos2d::Point(0, 0),
            cocos2d::Size(16, 16)));

    auto healthBarPosition =
        cocos2d::Point(
            health->sprite->getContentSize().width + healthBar->sprite->getContentSize().height,
            health->sprite->getContentSize().height * 0.5f * -1);

    healthBar->setPosition(healthBarPosition);

    healthBar->setRotation(90);
    healthBar->setScaleY(1);

    this->health->energyBar = healthBar;
    this->health->addChild(healthBar);

    this->health->energyBar->setValue(this->player->health - 1);

    this->initCallbacks();
    this->setDefaultBrowner();
    this->prepare();


    return true;
}

void PauseMenu::setupBrowners() {
    float offsetY = -24;

    std::vector<std::shared_ptr<BrownerData>> pauseBrowners;
    
    auto gameBrowners = GameManager::getInstance().browners.collection;

    for (int i = 0; i < gameBrowners.size(); ++i) {
        auto browner = gameBrowners[i];

        if (browner->pauseItem.compare("") != 0) {
            pauseBrowners.push_back(browner);
        }
    }

    for (int i = 0; i < pauseBrowners.size(); ++i) {
        auto browner = pauseBrowners[i];

        if (browner->acquired) {
            auto brownerLocation = cocos2d::Point(160, offsetY);

            auto brownerInterruptor = PauseInterruptor::create(browner->pauseItem + "_" + "weapon", nullptr);
            brownerInterruptor->setPosition(brownerLocation);
            this->addChild(brownerInterruptor);

            auto brownerLabel =
                windy::Label::create(
                    browner->pauseItem,
                    PauseMenuResources::font,
                    8,
                    cocos2d::TextHAlignment::LEFT,
                    cocos2d::TextVAlignment::TOP);

            brownerLabel->setPosition(cocos2d::Point(brownerInterruptor->sprite->getContentSize().width + 1, 0));

            brownerInterruptor->addChild(brownerLabel);

            brownerInterruptor->brownerId = browner->id;
            brownerInterruptor->pauseItem = browner->pauseItem;


            auto energyBar = 
                dynamic_cast<EnergyBar*>
                (windy::EntityFactory::getInstance().create(
                    "energy_bar", 
                    cocos2d::Point(0, 0), 
                    cocos2d::Size(16, 16)));

            auto energyBarPosition =
                cocos2d::Point(brownerInterruptor->sprite->getContentSize().width + energyBar->sprite->getContentSize().height,
                    brownerInterruptor->sprite->getContentSize().height * 0.5f * -1);

            energyBar->setPosition(energyBarPosition);

            energyBar->setRotation(90);
            energyBar->setScaleY(1);
                        
            brownerInterruptor->energyBar = energyBar;
            brownerInterruptor->addChild(energyBar);

            brownerInterruptor->energyBar->setValue(this->player->getBrowner(brownerInterruptor->brownerId)->energy - 1);


            this->menuItems.push_back(brownerInterruptor);
            this->menuBrowners.push_back(brownerInterruptor);

            offsetY -= 24;

            if (brownerInterruptor->brownerId == this->player->currentBrowner->brownerId) {
                this->defaultBrowner = brownerInterruptor;
            }

        }
    }


}

void PauseMenu::initCallbacks() {
    this->fillingMTank = false;

    this->onETankTriggered = [this](PauseInterruptor* sender) {

        if (this->player->health < 28 && GameManager::getInstance().player.eTanks > 0) {
            this->busy = true;

            this->gui->restoreGenericBar(this->health->energyBar, this->player->maxHealth, [this]() {
                this->player->health = this->player->maxHealth;

                this->busy = false;
                GameManager::getInstance().player.eTanks -= 1;

                auto slot = GameManager::getInstance().getDefaultSlot();

                slot.e = GameManager::getInstance().player.eTanks;

                windy::SaveManager::saveSlot(windy::SaveManager::defaultSlot, slot);

                int numETanks = GameManager::getInstance().player.eTanks;
                
                this->eTanksLabel->setString("0" + std::to_string(numETanks));

            });
        }
        else {
            windy::AudioManager::playSfx(windy::Sounds::Error);
        }

    };

    

    this->onMTankTriggered = [this](PauseInterruptor* sender) {
        if (GameManager::getInstance().player.mTanks > 0) {
            this->fillingMTank = true;
            this->menuItems = this->menuBrowners;
            this->selector->setSelectedItem(this->defaultBrowner);
        }
        else {
            windy::AudioManager::playSfx(windy::Sounds::Error);
        }
    };

    this->onBrownerTriggered = [=](PauseInterruptor* sender) {

        if (this->fillingMTank) {
            auto selectedBrowner = this->selector->getSelectedItem();

            if (this->player->getBrowner(selectedBrowner->brownerId)->brownerId != 
                GameManager::getInstance().browners.teleport->id &&
                this->player->getBrowner(selectedBrowner->brownerId)->brownerId !=
                GameManager::getInstance().browners.violet->id &&
                this->player->getBrowner(selectedBrowner->brownerId)->brownerId !=
                GameManager::getInstance().browners.helmet->id) {
                if (this->player->getBrowner(selectedBrowner->brownerId)->energy < 28) {
                    this->busy = true;

                    this->gui->restoreGenericBar(selectedBrowner->energyBar, this->player->maxWeaponEnergy, [=]() {
                        this->player->getBrowner(selectedBrowner->brownerId)->energy = this->player->maxWeaponEnergy;
                        this->busy = false;

                        GameManager::getInstance().player.mTanks -= 1;

                        auto slot = GameManager::getInstance().getDefaultSlot();

                        slot.e = GameManager::getInstance().player.mTanks;

                        windy::SaveManager::saveSlot(windy::SaveManager::defaultSlot, slot);

                        int numMTanks = GameManager::getInstance().player.mTanks;

                        this->mTanksLabel->setString("0" + std::to_string(numMTanks));
                    });
                }
                else {
                    windy::AudioManager::playSfx(windy::Sounds::Error);
                }
            }
            else {
                windy::AudioManager::playSfx(windy::Sounds::Error);
            }

            this->fillingMTank = false;
            this->menuItems = this->allMenuItems;
        }  
    };

    this->onSwitchTriggered = [=](PauseInterruptor* sender) {

        bool visitTarget = false;
        bool visitSender = false;

        int targetValue = 0;
        int senderValue = 0;

        PauseInterruptor* target = nullptr;

        if (sender == this->helmetSwitch) {
            if (GameManager::getInstance().browners.helmet->acquired) {
                if (GameManager::getInstance().options.helmetActivated) {
                    GameManager::getInstance().options.helmetActivated = false;
                    visitSender = true;
                }
                else {
                    if (GameManager::getInstance().options.extremeActivated) {
                        GameManager::getInstance().options.extremeActivated = false;
                        visitTarget = true;
                        target = this->exSwitch;
                    }


                    GameManager::getInstance().options.helmetActivated = true;
                    visitSender = true;
                    senderValue = 1;
                }

                auto slot = GameManager::getInstance().getDefaultSlot();

                slot.helmetActivated = GameManager::getInstance().options.helmetActivated;

                windy::SaveManager::saveSlot(windy::SaveManager::defaultSlot, slot);

            }
            else {
                windy::AudioManager::playSfx(windy::Sounds::Error);
            }
        }
        else if (sender == this->exSwitch) {
            if (GameManager::getInstance().browners.extreme->acquired) {
                if (GameManager::getInstance().options.extremeActivated) {
                    GameManager::getInstance().options.extremeActivated = false;
                    visitSender = true;
                }
                else {
                    if (GameManager::getInstance().options.helmetActivated) {
                        GameManager::getInstance().options.helmetActivated = false;
                        visitTarget = true;
                        target = this->helmetSwitch;
                    }


                    GameManager::getInstance().options.extremeActivated = true;
                    visitSender = true;
                    senderValue = 1;
                }
            }
            else {
                windy::AudioManager::playSfx(windy::Sounds::Error);
            }
        }
        else if (sender == this->exitSwitch) {
            windy::AudioManager::playSfx(windy::Sounds::Selected);
            this->level->exitLevel();
        }

        if (visitTarget) {
            target->sprite->setImageIndex(targetValue);
        }

        if (visitSender) {
            sender->sprite->setImageIndex(senderValue);
        }

        this->setDefaultBrowner();

        this->validateWeapons();
    };

    this->eTank->setTriggerCallback(this->onETankTriggered);
    this->mTank->setTriggerCallback(this->onMTankTriggered);

    for (int i = 0; i < this->menuBrowners.size(); ++i) {
        auto browner = this->menuBrowners[i];
        browner->setTriggerCallback(this->onBrownerTriggered);
    }

    this->exSwitch->setTriggerCallback(this->onSwitchTriggered);
    this->helmetSwitch->setTriggerCallback(this->onSwitchTriggered);
    this->exitSwitch->setTriggerCallback(this->onSwitchTriggered);
}

void PauseMenu::prepare() {
    this->validateWeapons();
    this->allMenuItems = this->menuItems;
    this->selector->setSelectedItem(this->defaultBrowner);
}

PauseInterruptor* PauseMenu::getBrowner(int brownerId) {
    PauseInterruptor* brownerFound = nullptr;
    for (int i = 0; i < this->menuBrowners.size(); ++i) {
        auto browner = this->menuBrowners.at(i);
        if (browner->brownerId == brownerId) {
            brownerFound = browner;
            break;
        }
    }

    return brownerFound;
}

void PauseMenu::setDefaultBrowner() {

    auto violetBrowner = this->getBrowner(GameManager::getInstance().browners.violet->id);
    auto helmetBrowner = this->getBrowner(GameManager::getInstance().browners.helmet->id);
    auto extremeBrowner = this->getBrowner(GameManager::getInstance().browners.extreme->id);

    PauseInterruptor* defaultBrowner = nullptr;
    if (GameManager::getInstance().options.helmetActivated) {
        if (this->defaultBrowner == violetBrowner || this->defaultBrowner == extremeBrowner) {
            if (GameManager::getInstance().options.extremeActivated) {
                defaultBrowner = extremeBrowner;
            }
            else {
                defaultBrowner = helmetBrowner;
            }
        }
        else {
            if (GameManager::getInstance().options.extremeActivated) {
                if (this->defaultBrowner == violetBrowner || this->defaultBrowner == helmetBrowner) {
                    defaultBrowner = extremeBrowner;
                }
            }
        }
    }
    else {
        if (GameManager::getInstance().options.extremeActivated) {
            if (this->defaultBrowner == violetBrowner || this->defaultBrowner == helmetBrowner) {
                defaultBrowner = extremeBrowner;
            }
        }
        else {
            if (this->defaultBrowner == helmetBrowner || this->defaultBrowner == extremeBrowner) {
                defaultBrowner = violetBrowner;
            }
        }
    }

    if (defaultBrowner != nullptr) {
        this->defaultBrowner = defaultBrowner;
    }

}

void PauseMenu::validateWeapons() {
    int helmetSwitchIndex = 0;
    int extremeSwitchIndex = 0;

    if (GameManager::getInstance().options.helmetActivated) {
        helmetSwitchIndex = 1;
    }

    if (GameManager::getInstance().options.extremeActivated) {
        extremeSwitchIndex = 1;
    }

    this->helmetSwitch->sprite->setImageIndex(helmetSwitchIndex);
    this->exSwitch->sprite->setImageIndex(extremeSwitchIndex);

    auto violetBrowner = this->getBrowner(GameManager::getInstance().browners.violet->id);
    auto helmetBrowner = this->getBrowner(GameManager::getInstance().browners.helmet->id);
    auto extremeBrowner = this->getBrowner(GameManager::getInstance().browners.extreme->id);

    violetBrowner->setVisible(!(GameManager::getInstance().options.helmetActivated || GameManager::getInstance().options.extremeActivated));

    if (helmetBrowner != nullptr) {
        helmetBrowner->setVisible(GameManager::getInstance().options.helmetActivated && !GameManager::getInstance().options.extremeActivated);
        helmetBrowner->setPosition(violetBrowner->getPosition());
    }

    if (extremeBrowner != nullptr) {
        extremeBrowner->setVisible(GameManager::getInstance().options.extremeActivated);
        extremeBrowner->setPosition(violetBrowner->getPosition());
    }
}


void PauseMenu::onEnter()
{
    Node::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void PauseMenu::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Node::onExit();
}

void PauseMenu::update(float dt)
{

    if (!this->busy) {
        
        for (int i = 0; i < this->menuBrowners.size(); ++i) {
            auto browner = this->menuBrowners[i];

            browner->energyBar->setValue(this->player->getBrowner(browner->brownerId)->energy - 1);
        }

        this->selector->selectFromItems(this->menuItems);
        
        auto selectedItem = this->selector->getSelectedItem();

        if (selectedItem->pauseItem.compare("") != 0) {
            this->weaponAnimation->swapContents(selectedItem->pauseItem);
            this->selector->sprite->setVisible(false);
        }
        else {
            this->weaponAnimation->swapContents(this->defaultBrowner->pauseItem);
            this->selector->sprite->setVisible(true);
        }

        if (selectedItem->brownerId == PauseInterruptor::BrownerIdentifier::Invalid) {
            this->selectedBrowner = this->defaultBrowner;
        }
        else {
            this->selectedBrowner = selectedItem;
        }

    }

}