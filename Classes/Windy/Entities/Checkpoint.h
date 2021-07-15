#ifndef __WINDY_CHECKPOINT_H__
#define __WINDY_CHECKPOINT_H__

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class Checkpoint : public Logical
    {
    public:
        virtual bool init() override;
        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;
        virtual void onUpdate(float dt) override;
    };
}

#endif
