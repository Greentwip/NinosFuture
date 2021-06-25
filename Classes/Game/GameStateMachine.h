#ifndef __WINDY_GAME_FLOW__
#define __WINDY_GAME_FLOW__

#include <vector>

#include "GameState.h"

namespace game {

    class GameStateMachine {
    public:
        static GameStateMachine& getInstance()
        {
            static GameStateMachine instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
    private:
        GameStateMachine() {}

    public:
        GameStateMachine(GameStateMachine const&) = delete;
        void operator=(GameStateMachine const&) = delete;

    public:
        void pushState(GameState state);
        void popState();

        GameState getState();

    private:
        void loadCurrentState();

    private:
        GameState previousState;
        GameState state;

        std::vector<GameState> stateStack;
    };


}

#endif