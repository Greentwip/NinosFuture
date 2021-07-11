#ifndef _WINDY_INFINITE_PARALLAX_H_
#define _WINDY_INFINITE_PARALLAX_H_

#include <string>

#include "cocos2d.h"

namespace windy {
    class Sprite;
}

namespace windy {
    class InfiniteParallax : public cocos2d::Node
    {
    public:
        static InfiniteParallax* create(const std::string& filePath, const cocos2d::Point& speed);
        
        virtual bool init();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

    private:

        void horizontalScroll();
        void verticalScroll();

        windy::Sprite* parallax;
        windy::Sprite* scroller;

        cocos2d::Point speed;
        cocos2d::Point spriteLimit;
        cocos2d::Point scrollerStartup;
    };

}


#endif
