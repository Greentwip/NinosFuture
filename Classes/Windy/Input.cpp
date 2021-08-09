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
    {InputKey::Select, Key() },
    {InputKey::A, Key() },
    {InputKey::B, Key() },
    {InputKey::LB, Key() },
    {InputKey::RB, Key() },
    {InputKey::LT, Key() },
    {InputKey::RT, Key() }
};

std::map<InputKey, Key> windy::Input::keys = {
    {InputKey::Up, Key() },
    {InputKey::Down, Key() },
    {InputKey::Left, Key() },
    {InputKey::Right, Key() },
    {InputKey::Start, Key() },
    {InputKey::Select, Key() },
    {InputKey::A, Key() },
    {InputKey::B, Key() },
    {InputKey::LB, Key() },
    {InputKey::RB, Key() },
    {InputKey::LT, Key() },
    {InputKey::RT, Key() }
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
        {InputKey::Select, Key() },
        {InputKey::A, Key() },
        {InputKey::B, Key() },
        {InputKey::LB, Key() },
        {InputKey::RB, Key() },
        {InputKey::LT, Key() },
        {InputKey::RT, Key() }
    };

    windy::Input::keys = {
        {InputKey::Up, Key() },
        {InputKey::Down, Key() },
        {InputKey::Left, Key() },
        {InputKey::Right, Key() },
        {InputKey::Start, Key() },
        {InputKey::Select, Key() },
        {InputKey::A, Key() },
        {InputKey::B, Key() },
        {InputKey::LB, Key() },
        {InputKey::RB, Key() },
        {InputKey::LT, Key() },
        {InputKey::RT, Key() }
    };
}



static std::vector<std::string> s_keyCodeKey =
{
    "KEY_NONE",
    "KEY_PAUSE",
    "KEY_SCROLL_LOCK",
    "KEY_PRINT",
    "KEY_SYSREQ",
    "KEY_BREAK",
    "KEY_ESCAPE",
    "KEY_BACKSPACE",
    "KEY_TAB",
    "KEY_BACK_TAB",
    "KEY_RETURN",
    "KEY_CAPS_LOCK",
    "KEY_SHIFT",
    "KEY_RIGHT_SHIFT",
    "KEY_CTRL",
    "KEY_RIGHT_CTRL",
    "KEY_ALT",
    "KEY_RIGHT_ALT",
    "KEY_MENU",
    "KEY_HYPER",
    "KEY_INSERT",
    "KEY_HOME",
    "KEY_PG_UP",
    "KEY_DELETE",
    "KEY_END",
    "KEY_PG_DOWN",
    "KEY_LEFT_ARROW",
    "KEY_RIGHT_ARROW",
    "KEY_UP_ARROW",
    "KEY_DOWN_ARROW",
    "KEY_NUM_LOCK",
    "KEY_KP_PLUS",
    "KEY_KP_MINUS",
    "KEY_KP_MULTIPLY",
    "KEY_KP_DIVIDE",
    "KEY_KP_ENTER",
    "KEY_KP_HOME",
    "KEY_KP_UP",
    "KEY_KP_PG_UP",
    "KEY_KP_LEFT",
    "KEY_KP_FIVE",
    "KEY_KP_RIGHT",
    "KEY_KP_END",
    "KEY_KP_DOWN",
    "KEY_KP_PG_DOWN",
    "KEY_KP_INSERT",
    "KEY_KP_DELETE",
    "KEY_F1",
    "KEY_F2",
    "KEY_F3",
    "KEY_F4",
    "KEY_F5",
    "KEY_F6",
    "KEY_F7",
    "KEY_F8",
    "KEY_F9",
    "KEY_F10",
    "KEY_F11",
    "KEY_F12",
    "KEY_SPACE",
    "KEY_EXCLAM",
    "KEY_QUOTE",
    "KEY_NUMBER",
    "KEY_DOLLAR",
    "KEY_PERCENT",
    "KEY_CIRCUMFLEX",
    "KEY_AMPERSAND",
    "KEY_APOSTROPHE",
    "KEY_LEFT_PARENTHESIS",
    "KEY_RIGHT_PARENTHESIS",
    "KEY_ASTERISK",
    "KEY_PLUS",
    "KEY_COMMA",
    "KEY_MINUS",
    "KEY_PERIOD",
    "KEY_SLASH",
    "KEY_0",
    "KEY_1",
    "KEY_2",
    "KEY_3",
    "KEY_4",
    "KEY_5",
    "KEY_6",
    "KEY_7",
    "KEY_8",
    "KEY_9",
    "KEY_COLON",
    "KEY_SEMICOLON",
    "KEY_LESS_THAN",
    "KEY_EQUAL",
    "KEY_GREATER_THAN",
    "KEY_QUESTION",
    "KEY_AT",
    "KEY_CAPITAL_A",
    "KEY_CAPITAL_B",
    "KEY_CAPITAL_C",
    "KEY_CAPITAL_D",
    "KEY_CAPITAL_E",
    "KEY_CAPITAL_F",
    "KEY_CAPITAL_G",
    "KEY_CAPITAL_H",
    "KEY_CAPITAL_I",
    "KEY_CAPITAL_J",
    "KEY_CAPITAL_K",
    "KEY_CAPITAL_L",
    "KEY_CAPITAL_M",
    "KEY_CAPITAL_N",
    "KEY_CAPITAL_O",
    "KEY_CAPITAL_P",
    "KEY_CAPITAL_Q",
    "KEY_CAPITAL_R",
    "KEY_CAPITAL_S",
    "KEY_CAPITAL_T",
    "KEY_CAPITAL_U",
    "KEY_CAPITAL_V",
    "KEY_CAPITAL_W",
    "KEY_CAPITAL_X",
    "KEY_CAPITAL_Y",
    "KEY_CAPITAL_Z",
    "KEY_LEFT_BRACKET",
    "KEY_BACK_SLASH",
    "KEY_RIGHT_BRACKET",
    "KEY_UNDERSCORE",
    "KEY_GRAVE",
    "KEY_A",
    "KEY_B",
    "KEY_C",
    "KEY_D",
    "KEY_E",
    "KEY_F",
    "KEY_G",
    "KEY_H",
    "KEY_I",
    "KEY_J",
    "KEY_K",
    "KEY_L",
    "KEY_M",
    "KEY_N",
    "KEY_O",
    "KEY_P",
    "KEY_Q",
    "KEY_R",
    "KEY_S",
    "KEY_T",
    "KEY_U",
    "KEY_V",
    "KEY_W",
    "KEY_X",
    "KEY_Y",
    "KEY_Z",
    "KEY_LEFT_BRACE",
    "KEY_BAR",
    "KEY_RIGHT_BRACE",
    "KEY_TILDE",
    "KEY_EURO",
    "KEY_POUND",
    "KEY_YEN",
    "KEY_MIDDLE_DOT",
    "KEY_SEARCH",
    "KEY_DPAD_LEFT",
    "KEY_DPAD_RIGHT",
    "KEY_DPAD_UP",
    "KEY_DPAD_DOWN",
    "KEY_DPAD_CENTER",
    "KEY_ENTER",
    "KEY_PLAY",
};


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

    auto onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
        this->onKeyPad(keyCode, true);
    };

    auto onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
        this->onKeyPad(keyCode, false);
    };
        
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = onKeyPressed;
    keyboardListener->onKeyReleased = onKeyReleased;

    getScene()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, getScene());
}

bool Input::keyDown(InputKey key) {
    return Input::keys[key].status == KeyStatus::Down;

}
bool Input::keyPressed(InputKey key) {
    return Input::keys[key].pressed;
}

void Input::onKeyPad(cocos2d::EventKeyboard::KeyCode key, bool pressed) {

    InputKey translatedKey = InputKey::None;


    if (key == cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER || key == cocos2d::EventKeyboard::KeyCode::KEY_ENTER) {
        translatedKey = InputKey::Start;
    }
    else if (key == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW) {
        translatedKey = InputKey::Up;
    }
    else if (key == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
        translatedKey = InputKey::Down;
    }
    else if (key == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
        translatedKey = InputKey::Left;
    }
    else if (key == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
        translatedKey = InputKey::Right;
    }
    else if (key == cocos2d::EventKeyboard::KeyCode::KEY_Z) {
        translatedKey = InputKey::A;
    }
    else if (key == cocos2d::EventKeyboard::KeyCode::KEY_X) {
        translatedKey = InputKey::B;
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

void Input::onControllerKey(cocos2d::Controller* controller, int keyCode, bool pressed) {

    int key = keyCode;

    InputKey translatedKey = InputKey::None;

    if (key == 1) {
        translatedKey = InputKey::Start;
    }
    else if (key == 2) {
        translatedKey = InputKey::Select;
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
