
#include "SaveScene.h"
#include "GameStateMachine.h"

#include "cocos/ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "Windy/AudioManager.h"
#include "Windy/SaveManager.h"
#include "Windy/Input.h"

#include "GameManager.h"

#include "Windy/Armature.h"
#include "Windy/Sprite.h"

#include "Game/Entities/UI/Fader.h"


using namespace game;
using namespace cocos2d;

class SaveSceneResources {
public:
    static std::string dataFile;
    static cocos2d::Color3B activeColor;
    static cocos2d::Color3B inactiveColor;

};

std::string SaveSceneResources::dataFile = "sprites/gameplay/screens/save/data.csb";
cocos2d::Color3B SaveSceneResources::activeColor = cocos2d::Color3B(255, 255, 255);
cocos2d::Color3B SaveSceneResources::inactiveColor = cocos2d::Color3B(127, 127, 127);


Scene* SaveScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    SaveScene* layer = SaveScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool SaveScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();

    auto root = cocos2d::CSLoader::createNode(SaveSceneResources::dataFile);

    addChild(root);

    this->slot1 = root->getChildByName("file_1");
    this->slot2 = root->getChildByName("file_2");
    this->slot3 = root->getChildByName("file_3");

    this->deleteButton = root->getChildByName("delete");
    this->deleteConfirm = root->getChildByName("confirm");

    this->slot1Customs = this->slot1->getChildByName("customs");
    this->slot2Customs = this->slot2->getChildByName("customs");
    this->slot3Customs = this->slot3->getChildByName("customs");

    this->slot1Customs->setOpacity(0);
    this->slot2Customs->setOpacity(0);
    this->slot3Customs->setOpacity(0);

    this->slot1->setColor(SaveSceneResources::activeColor);
    this->slot2->setColor(SaveSceneResources::inactiveColor);
    this->slot3->setColor(SaveSceneResources::inactiveColor);

    this->deleteButton->setColor(SaveSceneResources::inactiveColor);

    this->deleteConfirm->setVisible(false);

    this->selecting = 0;
    this->deletingConfirmation = 1;
    this->state = this->selecting;

    this->activeSlot = this->slot1;

    for (int i = 1; i < 4; ++i) {
        if (this->slotExists(i)) {
            this->loadSlot(i);
            this->populateSlot(i);
        }
    }

    if (windy::AudioManager::getCurrentTrack() != windy::Sounds::Title) {
        windy::AudioManager::stopAll();
        windy::AudioManager::playBgm(windy::Sounds::Title);
    }

    this->triggered = false;

    _ready = false;

    auto fader = Fader::create(cocos2d::Point(0, 0));

    fader->setPosition(cocos2d::Point(0, 0));

    fader->setOpacity(255);

    addChild(fader, 4096);

    _fader = fader;

    _fader->fadeOut([this]() {
        _ready = true;


    });


    return true;
}


void SaveScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void SaveScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

bool SaveScene::slotExists(int slot) {
    return windy::SaveManager::slotExists(slot);
}

windy::Slot SaveScene::readSlot(int slot) {
    return windy::SaveManager::readSlot(slot);
}

void SaveScene::loadSlot(int slot) {
    auto dataSlot = windy::SaveManager::readSlot(slot);

    GameManager::getInstance().unlockables.helmet->acquired = dataSlot.helmet;
    GameManager::getInstance().unlockables.head->acquired = dataSlot.head;
    GameManager::getInstance().unlockables.chest->acquired = dataSlot.chest;
    GameManager::getInstance().unlockables.fist->acquired = dataSlot.fist;
    GameManager::getInstance().unlockables.boot->acquired = dataSlot.boot;

    GameManager::getInstance().player.eTanks = dataSlot.e;
    GameManager::getInstance().player.mTanks = dataSlot.m;

    GameManager::getInstance().player.lives = dataSlot.lives;

    GameManager::getInstance().browners.freezer->acquired = dataSlot.freezer;
    GameManager::getInstance().browners.sheriff->acquired = dataSlot.sheriff;
    GameManager::getInstance().browners.boomer->acquired = dataSlot.boomer;
    GameManager::getInstance().browners.military->acquired = dataSlot.military;
    GameManager::getInstance().browners.vine->acquired = dataSlot.vine;
    GameManager::getInstance().browners.shield->acquired = dataSlot.shield;
    GameManager::getInstance().browners.night->acquired = dataSlot.night;
    GameManager::getInstance().browners.torch->acquired = dataSlot.torch;


    GameManager::getInstance().browners.helmet->acquired = GameManager::getInstance().unlockables.helmetAcquired();
    GameManager::getInstance().browners.extreme->acquired = GameManager::getInstance().unlockables.extremeAcquired();

    GameManager::getInstance().levels.freezer->defeated = GameManager::getInstance().browners.freezer->acquired;
    GameManager::getInstance().levels.sheriff->defeated = GameManager::getInstance().browners.sheriff->acquired;
    GameManager::getInstance().levels.boomer->defeated = GameManager::getInstance().browners.boomer->acquired;
    GameManager::getInstance().levels.military->defeated = GameManager::getInstance().browners.military->acquired;
    GameManager::getInstance().levels.vine->defeated = GameManager::getInstance().browners.vine->acquired;
    GameManager::getInstance().levels.shield->defeated = GameManager::getInstance().browners.shield->acquired;
    GameManager::getInstance().levels.night->defeated = GameManager::getInstance().browners.night->acquired;
    GameManager::getInstance().levels.torch->defeated = GameManager::getInstance().browners.torch->acquired;

    GameManager::getInstance().options.helmetActivated = dataSlot.helmetActivated;
    GameManager::getInstance().collectibles = dataSlot.collectibles;
}


void SaveScene::saveSlot(int slot) {
    auto slotData = windy::SaveManager::createSlot();

    windy::SaveManager::saveSlot(slot, slotData);
}

void SaveScene::deleteSlot(int slot) {
    windy::SaveManager::deleteSlot(slot);
}

void SaveScene::resetDeleting() {
    this->activeSlot = this->slot1;
    this->activeSlot->setColor(SaveSceneResources::activeColor);
    this->slot2->setColor(SaveSceneResources::inactiveColor);
    this->slot3->setColor(SaveSceneResources::inactiveColor);

    this->deleteButton->setColor(SaveSceneResources::inactiveColor);
    this->deleteConfirm->setVisible(false);

}

void SaveScene::uncolorWeapon(cocos2d::Node* custom, std::string weaponName) {
    custom->getChildByName(weaponName)->setColor(SaveSceneResources::inactiveColor);
}

void SaveScene::colorWeapon(cocos2d::Node* custom, std::string weaponName) {
    custom->getChildByName(weaponName)->setColor(SaveSceneResources::activeColor);
}


void SaveScene::populateSlot(int slot) {
    cocos2d::Node* saveSlot = nullptr;

    switch (slot) {
        case 1:
            saveSlot = this->slot1Customs;
            break;

        case 2:
            saveSlot = this->slot2Customs;
            break;

        case 3:
            saveSlot = this->slot3Customs;
            break;

        default:
            saveSlot = this->slot1Customs;
            break;

    }

    saveSlot->setOpacity(255);

    
    cocos2d::ui::Text* eTank = dynamic_cast<cocos2d::ui::Text*>(saveSlot->getChildByName("etank")->getChildByName("label"));
    cocos2d::ui::Text* mTank = dynamic_cast<cocos2d::ui::Text*>(saveSlot->getChildByName("mtank")->getChildByName("label"));
    cocos2d::ui::Text* lives = dynamic_cast<cocos2d::ui::Text*>(saveSlot->getChildByName("life")->getChildByName("label"));

    eTank->setString(std::to_string(GameManager::getInstance().player.eTanks));
    mTank->setString(std::to_string(GameManager::getInstance().player.mTanks));
    lives->setString(std::to_string(GameManager::getInstance().player.lives));

    auto weapons = saveSlot->getChildByName("weapons");

    if (GameManager::getInstance().unlockables.helmet->acquired) {
        this->colorWeapon(weapons, "helmet");
    }
    
    if (GameManager::getInstance().unlockables.head->acquired) {
        this->colorWeapon(weapons, "ex_helmet");
    }

    if (GameManager::getInstance().unlockables.chest->acquired) {
        this->colorWeapon(weapons, "chest");
    }

    if (GameManager::getInstance().unlockables.fist->acquired) {
        this->colorWeapon(weapons, "fist");
    }


    if (GameManager::getInstance().unlockables.boot->acquired) {
        this->colorWeapon(weapons, "boot");
    }

    if (GameManager::getInstance().browners.freezer->acquired) {
        this->colorWeapon(weapons, "freezer");
    }

    if (GameManager::getInstance().browners.sheriff->acquired) {
        this->colorWeapon(weapons, "sheriff");
    }

    if (GameManager::getInstance().browners.boomer->acquired) {
        this->colorWeapon(weapons, "boomer");
    }

    if (GameManager::getInstance().browners.military->acquired) {
        this->colorWeapon(weapons, "military");
    }

    if (GameManager::getInstance().browners.vine->acquired) {
        this->colorWeapon(weapons, "vine");
    }

    if (GameManager::getInstance().browners.shield->acquired) {
        this->colorWeapon(weapons, "shield");
    }

    if (GameManager::getInstance().browners.night->acquired) {
        this->colorWeapon(weapons, "night");
    }

    if (GameManager::getInstance().browners.torch->acquired) {
        this->colorWeapon(weapons, "torch");
    }

    if (GameManager::getInstance().browners.extreme->acquired) {
        this->colorWeapon(weapons, "ex");
    }

}

void SaveScene::populateSlotCheat(int slot) {

    GameManager::getInstance().unlockables.helmet->acquired = true;
    GameManager::getInstance().unlockables.head->acquired = true;
    GameManager::getInstance().unlockables.chest->acquired = true;
    GameManager::getInstance().unlockables.fist->acquired = true;
    GameManager::getInstance().unlockables.boot->acquired = true;

    GameManager::getInstance().browners.freezer->acquired = true;
    GameManager::getInstance().browners.sheriff->acquired = true;
    GameManager::getInstance().browners.boomer->acquired = true;
    GameManager::getInstance().browners.military->acquired = true;
    GameManager::getInstance().browners.vine->acquired = true;
    GameManager::getInstance().browners.shield->acquired = true;
    GameManager::getInstance().browners.night->acquired = true;
    GameManager::getInstance().browners.torch->acquired = true;


    GameManager::getInstance().browners.helmet->acquired = true;
    GameManager::getInstance().browners.extreme->acquired = true;

    for (auto level : GameManager::getInstance().levels.collection) {
        level->defeated = false;
    }

    this->populateSlot(slot);
}

void SaveScene::clearSlot(int slot) {
    cocos2d::Node* saveSlot = nullptr;

    switch (slot) {
    case 1:
        saveSlot = this->slot1Customs;
        break;

    case 2:
        saveSlot = this->slot2Customs;
        break;

    case 3:
        saveSlot = this->slot3Customs;
        break;

    default:
        saveSlot = this->slot1Customs;
        break;

    }

    saveSlot->setOpacity(0);

    cocos2d::ui::Text* eTank = dynamic_cast<cocos2d::ui::Text*>(saveSlot->getChildByName("etank")->getChildByName("label"));
    cocos2d::ui::Text* mTank = dynamic_cast<cocos2d::ui::Text*>(saveSlot->getChildByName("mtank")->getChildByName("label"));
    cocos2d::ui::Text* lives = dynamic_cast<cocos2d::ui::Text*>(saveSlot->getChildByName("life")->getChildByName("label"));

    eTank->setString(std::to_string(0));
    mTank->setString(std::to_string(0));
    lives->setString(std::to_string(0));

    auto weapons = saveSlot->getChildByName("weapons");

    this->uncolorWeapon(weapons, "helmet");
    this->uncolorWeapon(weapons, "ex_helmet");
    this->uncolorWeapon(weapons, "chest");
    this->uncolorWeapon(weapons, "fist");
    this->uncolorWeapon(weapons, "boot");
    this->uncolorWeapon(weapons, "freezer");
    this->uncolorWeapon(weapons, "sheriff");
    this->uncolorWeapon(weapons, "boomer");
    this->uncolorWeapon(weapons, "military");
    this->uncolorWeapon(weapons, "vine");
    this->uncolorWeapon(weapons, "shield");
    this->uncolorWeapon(weapons, "night");
    this->uncolorWeapon(weapons, "torch");
    this->uncolorWeapon(weapons, "ex");

}

void SaveScene::update(float dt)
{

    if (this->state == this->deletingConfirmation && _ready && !triggered) {
        if (windy::Input::keyPressed(windy::InputKey::Up)) {
            if (this->activeSlot == this->slot1) {
                if (this->slotExists(3)) {
                    this->activeSlot = this->slot3;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot1->setColor(SaveSceneResources::inactiveColor);
                }
                else if (this->slotExists(2)) {
                    this->activeSlot = this->slot2;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot1->setColor(SaveSceneResources::inactiveColor);
                }
            }
            else if (this->activeSlot == this->slot2) {
                if (this->slotExists(1)) {
                    this->activeSlot = this->slot1;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot2->setColor(SaveSceneResources::inactiveColor);
                }
                else if (this->slotExists(3)) {
                    this->activeSlot = this->slot3;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot2->setColor(SaveSceneResources::inactiveColor);
                }

            }
            else {
                if (this->slotExists(2)) {
                    this->activeSlot = this->slot2;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot3->setColor(SaveSceneResources::inactiveColor);
                }
                else if (this->slotExists(1)) {
                    this->activeSlot = this->slot1;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot3->setColor(SaveSceneResources::inactiveColor);
                }

            }

            windy::AudioManager::playSfx(windy::Sounds::Select);
        }

        if (windy::Input::keyPressed(windy::InputKey::Down)) {
            if (this->activeSlot == this->slot1) {
                if (this->slotExists(2)) {
                    this->activeSlot = this->slot2;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot1->setColor(SaveSceneResources::inactiveColor);
                }
                else if (this->slotExists(3)) {
                    this->activeSlot = this->slot3;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot1->setColor(SaveSceneResources::inactiveColor);
                }
            }
            else if (this->activeSlot == this->slot2) {
                if (this->slotExists(3)) {
                    this->activeSlot = this->slot3;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot2->setColor(SaveSceneResources::inactiveColor);
                }
                else if (this->slotExists(1)) {
                    this->activeSlot = this->slot1;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot2->setColor(SaveSceneResources::inactiveColor);
                }

            }
            else {
                if (this->slotExists(1)) {
                    this->activeSlot = this->slot1;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot3->setColor(SaveSceneResources::inactiveColor);
                }
                else if (this->slotExists(2)) {
                    this->activeSlot = this->slot2;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot3->setColor(SaveSceneResources::inactiveColor);
                }

            }

            windy::AudioManager::playSfx(windy::Sounds::Select);
        }

    }

    if (this->state == this->selecting && _ready && !triggered) {
        if (windy::Input::keyPressed(windy::InputKey::Up)) {
            if (this->activeSlot == this->slot1) {
                if (this->slotExists(1) || this->slotExists(2) || this->slotExists(3)) {
                    this->activeSlot = this->deleteButton;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot1->setColor(SaveSceneResources::inactiveColor);
                }
                else {
                    this->activeSlot = this->slot3;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot1->setColor(SaveSceneResources::inactiveColor);
                }
            }
            else if (this->activeSlot == this->slot2) {
                this->activeSlot = this->slot1;
                this->activeSlot->setColor(SaveSceneResources::activeColor);
                this->slot2->setColor(SaveSceneResources::inactiveColor);
            }
            else if (this->activeSlot == this->slot3) {
                this->activeSlot = this->slot2;
                this->activeSlot->setColor(SaveSceneResources::activeColor);
                this->slot3->setColor(SaveSceneResources::inactiveColor);
            }
            else {
                this->activeSlot = this->slot3;
                this->activeSlot->setColor(SaveSceneResources::activeColor);
                this->deleteButton->setColor(SaveSceneResources::inactiveColor);
            }

            windy::AudioManager::playSfx(windy::Sounds::Select);

        }

        if (windy::Input::keyPressed(windy::InputKey::Down)) {
            if (this->activeSlot == this->slot1) {
                this->activeSlot = this->slot2;
                this->activeSlot->setColor(SaveSceneResources::activeColor);
                this->slot1->setColor(SaveSceneResources::inactiveColor);
            }
            else if (this->activeSlot == this->slot2) {
                this->activeSlot = this->slot3;
                this->activeSlot->setColor(SaveSceneResources::activeColor);
                this->slot2->setColor(SaveSceneResources::inactiveColor);
            }
            else if (this->activeSlot == this->slot3) {
                if (this->slotExists(1) || this->slotExists(2) || this->slotExists(3)) {
                    this->activeSlot = this->deleteButton;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot3->setColor(SaveSceneResources::inactiveColor);
                }
                else {
                    this->activeSlot = this->slot1;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->slot3->setColor(SaveSceneResources::inactiveColor);
                }
            }
            else {
                this->activeSlot = this->slot1;
                this->activeSlot->setColor(SaveSceneResources::activeColor);
                this->deleteButton->setColor(SaveSceneResources::inactiveColor);
            }
            windy::AudioManager::playSfx(windy::Sounds::Select);
        }
    }

    if (!this->triggered && _ready) {
        if (windy::Input::keyPressed(windy::InputKey::A)) {
            bool deleteState = false;

            int selectedSlot = 0;

            if (this->activeSlot == this->slot1) {
                selectedSlot = 1;
            }
            else if(this->activeSlot == this->slot2) {
                selectedSlot = 2;
            }
            else if (this->activeSlot == this->slot3) {
                selectedSlot = 3;
            }
            else {
                deleteState = true;
            }

            if (this->state == this->deletingConfirmation) {
                deleteState = true;
            }

            if (deleteState) {
                if (this->state == this->selecting) {
                    this->deleteConfirm->setOpacity(255);
                    this->deleteConfirm->setVisible(true);
                    this->activeSlot = this->slot1;
                    this->activeSlot->setColor(SaveSceneResources::activeColor);
                    this->state = this->deletingConfirmation;
                }
                else {
                    this->deleteConfirm->setOpacity(0);
                    this->deleteSlot(selectedSlot);
                    this->clearSlot(selectedSlot);
                    this->resetDeleting();
                    this->state = this->selecting;
                }
            }
            else {
                if (!this->slotExists(selectedSlot)) {
                    this->saveSlot(selectedSlot);
                }

                this->loadSlot(selectedSlot);

                if (GameManager::getInstance().player.lives <= 0) {
                    GameManager::getInstance().player.lives = 3;
                }

                if (selectedSlot == 3) {
                    this->populateSlotCheat(selectedSlot);
                }
                else {
                    this->populateSlot(selectedSlot);
                }

                windy::SaveManager::defaultSlot = selectedSlot;

                auto slot = GameManager::getInstance().getDefaultSlot();

                slot.lives = GameManager::getInstance().player.lives;

                GameManager::getInstance().saveDefaultSlot(slot);

                this->triggered = true;

                windy::AudioManager::playSfx(windy::Sounds::Selected);

                _fader->fadeIn([this]() {
                    GameStateMachine::getInstance().pushState(GameState::StageSelect);
                });

            }
        }
        else if (windy::Input::keyPressed(windy::InputKey::B)) {
            if (this->state == this->deletingConfirmation) {
                this->state = this->selecting;
                this->resetDeleting();
            }
            else {
                this->triggered = true;

                _fader->fadeIn([this]() {
                    GameStateMachine::getInstance().pushState(GameState::Title);
                });
            }
        }
    }
}
