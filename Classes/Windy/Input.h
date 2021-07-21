#ifndef _WINDY_INPUT_H_
#define _WINDY_INPUT_H_

#include <string>
#include <vector>

#include "cocos2d.h"
#include "cocos/base/CCController.h"

#include "InputKeys.h"


namespace windy {
    class Input : public cocos2d::Node
    {
    public:
        static Input* create();
        void setup();

        virtual bool init();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

        static bool takeInputs;

        static bool keyDown(InputKey key);
        static bool keyPressed(InputKey key);


    public:
        static std::map<InputKey, Key> delayedKeys;
        static std::map<InputKey, Key> keys;
        static std::vector<InputKey> joyKeys;


    private:
        void onKeyPad(cocos2d::EventKeyboard::KeyCode keyCode, bool pressed);
        void onControllerKey(cocos2d::Controller* controller, int keyCode, bool pressed);
        void onAxis(float axisX, float axisY);
        

        float deadZone;
        bool canMove;

        bool inHorizontalRange;
        bool inVerticalRange;
        InputKey horizontalRangeKey;
        InputKey verticalRangeKey;

    };

}

#endif