#include <cmath>

#include "Input.h"

#include "cocos/base/CCController.h"

using namespace windy;

static float inputDeadZone = 0.30f;

std::vector<InputKey> windy::Input::joyKeys = {
    InputKey::Right,
    InputKey::Left,
    InputKey::Up,
    InputKey::Down
};

std::map<InputKey, Key> windy::Input::delayedKeys = {
    {InputKey::Up, Key() },
    {InputKey::Down, Key() },
    {InputKey::Left, Key() },
    {InputKey::Right, Key() },
    {InputKey::Start, Key() },
    {InputKey::A, Key() },
    {InputKey::B, Key() },
    {InputKey::LB, Key() },
    {InputKey::RB, Key() }
};

std::map<InputKey, Key> windy::Input::keys = {
    {InputKey::Up, Key() },
    {InputKey::Down, Key() },
    {InputKey::Left, Key() },
    {InputKey::Right, Key() },
    {InputKey::Start, Key() },
    {InputKey::A, Key() },
    {InputKey::B, Key() },
    {InputKey::LB, Key() },
    {InputKey::RB, Key() }
};

bool windy::Input::takeInputs = true;


static void ResetKeys() {
    windy::Input::takeInputs = true;

    windy::Input::joyKeys.clear();
    windy::Input::delayedKeys.clear();
    windy::Input::keys.clear();
    
    windy::Input::joyKeys = {
        InputKey::Right,
        InputKey::Left,
        InputKey::Up,
        InputKey::Down
    };

    windy::Input::delayedKeys = {
        {InputKey::Up, Key() },
        {InputKey::Down, Key() },
        {InputKey::Left, Key() },
        {InputKey::Right, Key() },
        {InputKey::Start, Key() },
        {InputKey::A, Key() },
        {InputKey::B, Key() },
        {InputKey::LB, Key() },
        {InputKey::RB, Key() }
    };

    windy::Input::keys = {
        {InputKey::Up, Key() },
        {InputKey::Down, Key() },
        {InputKey::Left, Key() },
        {InputKey::Right, Key() },
        {InputKey::Start, Key() },
        {InputKey::A, Key() },
        {InputKey::B, Key() },
        {InputKey::LB, Key() },
        {InputKey::RB, Key() }
    };
}



Input* Input::create() {

    Input* input = new (std::nothrow) Input();

    if (input && input->init()) {
        input->autorelease();
        input->deadZone = inputDeadZone;
        input->canMove = true;
        input->inHorizontalRange = false;
        input->inVerticalRange = false;
        input->horizontalRangeKey = InputKey::None;
        input->verticalRangeKey = InputKey::None;
        ResetKeys();
        return input;
    }

    CC_SAFE_DELETE(input);
    return nullptr;
}

bool Input::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }
    return true;
}

void Input::setup() {
    auto controllerListener = cocos2d::EventListenerController::create();

    controllerListener->onKeyDown = [this](cocos2d::Controller* controller, int keyCode, cocos2d::Event* event) {
        this->onControllerKey(controller, keyCode, true);
    };

    controllerListener->onKeyUp = [this](cocos2d::Controller* controller, int keyCode, cocos2d::Event* event) {
        this->onControllerKey(controller, keyCode, false);
    };

    controllerListener->onAxisEvent = [this](cocos2d::Controller* controller, int keyCode, cocos2d::Event* event) {

        this->onAxis(controller->getKeyStatus(ControllerKey::JOYSTICK_LEFT_X).value, controller->getKeyStatus(ControllerKey::JOYSTICK_LEFT_Y).value);

    };


    getScene()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(controllerListener, getScene());
}

bool Input::keyDown(InputKey key) {
    return Input::keys[key].status == KeyStatus::Down;

}
bool Input::keyPressed(InputKey key) {
    return Input::keys[key].pressed;
}

void Input::onControllerKey(cocos2d::Controller* controller, int keyCode, bool pressed) {

    int key = keyCode;

    InputKey translatedKey = InputKey::None;

    if (key == 1) {
        translatedKey = InputKey::Start;
    }
    else if (key == 64) {
        translatedKey = InputKey::Up;
    }        
    else if (key == 128) {
        translatedKey = InputKey::Down;
    }  
    else if (key == 256) {
        translatedKey = InputKey::Left;
    }
    else if (key == 512) {
        translatedKey = InputKey::Right;
    }
    else if (key == 4 || key == 16) {
        translatedKey = InputKey::A;
    }
    else if (key == 8 || key == 32) {
        translatedKey = InputKey::B;
    }
    else if (key == 1024) {
        translatedKey = InputKey::LB;
    }
    else if (key == 2048) {
        translatedKey = InputKey::RB;
    }
       
    if (translatedKey != InputKey::None) {
        if (pressed) {
            Input::delayedKeys[translatedKey].status = KeyStatus::Down;
            Input::delayedKeys[translatedKey].pressed = true;
        }
        else {
            Input::delayedKeys[translatedKey].status = KeyStatus::Up;
            Input::delayedKeys[translatedKey].released = true;
        }
    }

    if (!Input::takeInputs) {
        return;
    }

    if (translatedKey != InputKey::None) {
        if (pressed) {
            Input::keys[translatedKey].status = KeyStatus::Down;
            Input::keys[translatedKey].pressed = true;
        }
        else {
            Input::keys[translatedKey].status = KeyStatus::Up;
            Input::keys[translatedKey].released = true;
        }
    }
}

void Input::onAxis(float axisX, float axisY) {
    if (!Input::takeInputs) {
        return;
    }

    auto axisPoint = cocos2d::Point(axisX, axisY);

    float circleDistance = axisPoint.getDistance(cocos2d::Point(0, 0));

    if (circleDistance >= this->deadZone) {
        this->canMove = true;
    }
    else {
        if (this->canMove) {
            this->inHorizontalRange = false;
            this->inVerticalRange = false;

            for (int i = 0; i < Input::joyKeys.size(); ++i) {
                Input::keys[Input::joyKeys[i]].status = KeyStatus::Up;

                if (Input::keys[Input::joyKeys[i]].pressed) {
                    Input::keys[Input::joyKeys[i]].pressed = false;
                    Input::keys[Input::joyKeys[i]].released = true;
                }

            }
        }

        this->canMove = false;
    }

    float deltaX = axisX;
    float deltaY = axisY;

    float angle = std::atan2(deltaY, deltaX) * 180.0f / 3.1416f;

    if (this->canMove) {
        if (angle >= -60 && angle <= 60) {
            if (!this->inHorizontalRange) {
                Input::keys[InputKey::Right].status = KeyStatus::Down;
                Input::keys[InputKey::Right].pressed = true;
                this->inHorizontalRange = true;
                this->horizontalRangeKey = InputKey::Right;
            }
        }
            
        else {
            Input::keys[InputKey::Right].status = KeyStatus::Up;
            Input::keys[InputKey::Right].released = true;
            
            if (this->horizontalRangeKey == InputKey::Right) {
                this->inHorizontalRange = false;
            }

        }

        if (std::abs(angle) >= 120 && std::abs(angle) <= 180) {
            if (!this->inHorizontalRange) {
                Input::keys[InputKey::Left].status = KeyStatus::Down;
                Input::keys[InputKey::Left].pressed = true;
                this->inHorizontalRange = true;
                this->horizontalRangeKey = InputKey::Left;
            }
        }

        else {
            Input::keys[InputKey::Left].status = KeyStatus::Up;
            Input::keys[InputKey::Left].released = true;

            if (this->horizontalRangeKey == InputKey::Left) {
                this->inHorizontalRange = false;
            }

        }

        if (angle >= 30 && angle <= 150) {
            if (!this->inVerticalRange) {
                Input::keys[InputKey::Up].status = KeyStatus::Down;
                Input::keys[InputKey::Up].pressed = true;
                this->inVerticalRange = true;
                this->verticalRangeKey = InputKey::Up;
            }
        }

        else {
            Input::keys[InputKey::Up].status = KeyStatus::Up;
            Input::keys[InputKey::Up].released = true;

            if (this->verticalRangeKey == InputKey::Up) {
                this->inVerticalRange = false;
            }

        }

        if (angle <= -30 && angle >= -150) {
            if (!this->inVerticalRange) {
                Input::keys[InputKey::Down].status = KeyStatus::Down;
                Input::keys[InputKey::Down].pressed = true;
                this->inVerticalRange = true;
                this->verticalRangeKey = InputKey::Down;
            }
        }

        else {
            Input::keys[InputKey::Down].status = KeyStatus::Up;
            Input::keys[InputKey::Down].released = true;

            if (this->verticalRangeKey == InputKey::Down) {
                this->inVerticalRange = false;
            }
        }
    }

}

void Input::onEnter()
{
    Node::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void Input::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Node::onExit();
}

void Input::update(float dt)
{
    if (!Input::takeInputs) {
        return;
    }
        
    for (auto kp : Input::keys) {
        if (Input::keys[kp.first].pressed) {
            Input::keys[kp.first].pressed = false;
        }

        if (Input::keys[kp.first].released) {
            Input::keys[kp.first].released = false;
        }
    }

    for (auto kp : Input::delayedKeys) {
        if (Input::delayedKeys[kp.first].pressed) {
            Input::delayedKeys[kp.first].pressed = false;
        }

        if (Input::delayedKeys[kp.first].released) {
            Input::delayedKeys[kp.first].released = false;
        }
    }
}
