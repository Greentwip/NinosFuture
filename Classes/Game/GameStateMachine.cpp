#include <string>

#include "GameStateMachine.h"

#include "IntroScene.h"
#include "AbakuraScene.h"
#include "TitleScene.h"

#include "Windy/Input.h"

using namespace game;
using namespace cocos2d;

template <typename T, typename U>
class SceneFactory
{
public:
    template <typename TDerived>
    void registerType(U name)
    {
        _createFuncs[name] = &createFunc<TDerived>;
    }

    T create(GameState name) {
        typename std::map<U, PCreateFunc>::const_iterator it = _createFuncs.find(name);
        if (it != _createFuncs.end()) {
            return it->second();
        }
        return nullptr;
    }

private:
    template <typename TDerived>
    static T createFunc()
    {
        return TDerived::scene();
    }

    typedef T (*PCreateFunc)();
    std::map<U, PCreateFunc> _createFuncs;
};

static SceneFactory<Scene*, GameState> sceneFactory;

GameStateMachine::GameStateMachine() {
    sceneFactory.registerType<IntroScene>(GameState::Intro);
    sceneFactory.registerType<AbakuraScene>(GameState::Abakura);
    sceneFactory.registerType<TitleScene>(GameState::Title);
}

GameState GameStateMachine::getState() {
    return this->state;
}


void GameStateMachine::pushState(GameState state) {
    this->previousState = this->state;
    this->state = state;
    this->loadCurrentState();
}

void GameStateMachine::popState() {
    auto currentState = this->state;
    this->state = this->previousState;
    this->previousState = currentState;
    this->loadCurrentState();
}

void GameStateMachine::loadCurrentState() {
    auto scene = sceneFactory.create(this->state);

    auto input = windy::Input::create();
    scene->addChild(input);
    input->setup();

    Director::getInstance()->replaceScene(scene);


}
