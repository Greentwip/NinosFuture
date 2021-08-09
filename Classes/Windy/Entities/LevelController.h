#ifndef __WINDY_LEVEL_CONTROLLER_H__
#define __WINDY_LEVEL_CONTROLLER_H__

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class LevelController : public Logical
    {
    public:
        virtual bool init() override;

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;

        virtual void restart() = 0;

        virtual void onUpdate(float dt) override;
    };
}

#endif
