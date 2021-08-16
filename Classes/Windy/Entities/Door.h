#ifndef __WINDY_DOOR_H__
#define __WINDY_DOOR_H__

#include <string>
#include <functional>

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class Sprite;
}

namespace windy {
    class Door : public Logical
    {
    public:
        virtual bool init() override;

        virtual void setup() = 0;

        virtual void setupPrefix() = 0;

        void setTraversable(bool animate = true);
        void reset();
        void close(bool animate = true);
        void open(bool animate = true);
        void lock(std::function<void()> callback);
        void unlock(std::function<void()> callback);

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;
        virtual void onUpdate(float dt) override;


    protected:
        Sprite* sprite;
        bool isBossDoor;

        float lockTime;
        bool triggered;

        std::string prefix;
    };
}

#endif
