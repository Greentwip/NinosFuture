#ifndef __WINDY_DOOR_H__
#define __WINDY_DOOR_H__

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

        void reopen();
        void lock(std::function<void()> callback);
        void unlock(std::function<void()> callback);


        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;
        virtual void onUpdate(float dt) override;


    private:
        Sprite* sprite;
        bool isBossDoor;
        float lockTime;
        bool triggered;
    };
}

#endif
