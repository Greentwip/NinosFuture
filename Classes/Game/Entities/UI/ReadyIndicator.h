#ifndef __GAME_READY_INDICATOR_H__
#define __GAME_READY_INDICATOR_H__

#include <functional>

#include "cocos2d.h"

namespace windy {
    class Sprite;
}

namespace game {
    class ReadyIndicator : public cocos2d::Node
    {
    public:
        static void preloadResources();

        static ReadyIndicator* create(std::function<void()> onReadyCallback);

        virtual bool init() override;

    private:
        windy::Sprite* sprite;

        std::function<void()> _onReadyCallback;
    };
}

#endif
