#ifndef __GAME_FADER_H__
#define __GAME_FADER_H__

#include <functional>

#include "cocos2d.h"

namespace windy {
    class Sprite;
}

namespace game {
    class Fader : public cocos2d::Node
    {
    public:
        static void preloadResources();

        static Fader* create(const cocos2d::Point& anchor);

        virtual bool init() override;

        void fadeIn(std::function<void()> onFadeInCallback);
        void fadeOut(std::function<void()> onFadeOutCallback);

        bool getFading();

    private:
        windy::Sprite* _sprite;
        bool _fading;
        cocos2d::Point _anchor;

        std::function<void()> _onFadeInCallback;
        std::function<void()> _onFadeOutCallback;
    };
}

#endif
