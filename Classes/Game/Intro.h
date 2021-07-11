#ifndef __INTRO_H__
#define __INTRO_H__

#include <string>
#include <functional>

#include "cocos2d.h"

#include "Windy/AudioManager.h"

namespace windy {
    class InfiniteParallax;
    class Sprite;
}

namespace game {
    class Intro : public cocos2d::Node
    {
    public:
        static Intro* create(const std::string& mug,
                             windy::Sounds bgm, 
                             std::function<void()> onIntroEnd);

        virtual bool init();

    private:
        void onRingMoveComplete();

        std::string mug;
        windy::Sounds bgm;
        std::function<void()> onIntroEnd;

        windy::InfiniteParallax* parallax;

        windy::Sprite* shadow;

        windy::Sprite* beltLeft;
        windy::Sprite* beltRight;

        int ringCount;


    };
}

#endif
