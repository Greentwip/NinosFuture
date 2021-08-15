#ifndef _GAME_PAUSE_ANIMATION_H_
#define _GAME_PAUSE_ANIMATION_H_

#include <string>

#include "cocos2d.h"

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
        static void preloadResources();

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