#include "GameStateMachine.h"

using namespace game;
using namespace cocos2d;


GameState GameStateMachine::getState() {
    return this->state;
}


void GameStateMachine::pushState(GameState state) {
    this->previousState = this->state;
    this->state = state;
}

void GameStateMachine::popState() {
    auto currentState = this->state;
    this->state = this->previousState;
    this->previousState = currentState;
}

void GameStateMachine::loadCurrentState() {
    /*auto scene = ModeSelectScene::scene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));*/
}
