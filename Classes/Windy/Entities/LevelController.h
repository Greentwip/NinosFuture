#ifndef __WINDY_LEVEL_CONTROLLER_H__
#define __WINDY_LEVEL_CONTROLLER_H__

#include <functional>

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class LevelController : public Logical
    {
    public:
        virtual bool init() override;

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        virtual void startup() = 0;

        virtual void restart(std::function<void()> onLevelRestarted) = 0;

        virtual void succeed() = 0;

        virtual void onUpdate(float dt) override;

    protected:
        std::function<void()> _onLevelRestarted;
    };
}

#endif
