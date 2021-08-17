#ifndef _GAME_PAUSE_ANIMATION_H_
#define _GAME_PAUSE_ANIMATION_H_

#include "cocos2d.h"

#include "Game/Entities/Resources.h"

#include <string>

namespace windy {
    class Sprite;
}

namespace game {
    class PauseMenu;
}

namespace game {
    class PauseAnimation : public cocos2d::Node
    {
    public:
        static game::Resources& getResources();

        static PauseAnimation* create();

        virtual bool init();

        void setAnimation(const std::string& animationName);
        void swapContents(const std::string& contents);

    private:
        friend class PauseMenu;

        windy::Sprite* sprite;
        
    };

}

#endif
