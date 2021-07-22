#ifndef __WINDY_LANDSCAPE_TILE_H__
#define __WINDY_LANDSCAPE_TILE_H__

#include <memory>

#include "cocos2d.h"


namespace windy {

    class LandscapeTile : public cocos2d::Node
    {
    public:
        static LandscapeTile* create(cocos2d::Sprite* sprite, const cocos2d::Point& position, const cocos2d::Size& size);

        void setup(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual bool init();

    public:
        std::shared_ptr<cocos2d::Rect> collisionBox;

    private:
        cocos2d::Sprite* sprite;
    };
}
#endif
