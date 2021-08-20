#ifndef __SAVE_SCENE_H__
#define __SAVE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <string>

#include "Windy/SaveManager.h"

namespace game {
    class Fader;
}

namespace game {
    class SaveScene : public cocos2d::LayerColor
    {
    public:
        virtual bool init();

        static cocos2d::Scene* scene();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

        CREATE_FUNC(SaveScene);

    private:
        bool slotExists(int slot);
        windy::Slot readSlot(int slot);
        void loadSlot(int slot);
        void saveSlot(int slot);
        void deleteSlot(int slot);
        void resetDeleting();
        void uncolorWeapon(cocos2d::Node* custom, std::string weaponName);
        void colorWeapon(cocos2d::Node* custom, std::string weaponName);
        void populateSlot(int slot);
        void populateSlotCheat(int slot);
        void clearSlot(int slot);

        cocos2d::Node* slot1;
        cocos2d::Node* slot2;
        cocos2d::Node* slot3;

        cocos2d::Node* slot1Customs;
        cocos2d::Node* slot2Customs;
        cocos2d::Node* slot3Customs;

        cocos2d::Node* deleteButton;

        cocos2d::Node* deleteConfirm;

        int selecting;
        int deletingConfirmation;
        int state;

        cocos2d::Node* activeSlot;


        bool triggered;
        bool _ready;

        Fader* _fader;


    };
}

#endif
