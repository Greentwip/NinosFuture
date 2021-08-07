#ifndef _GAME_TREMOR_TAIL_H_
#define _GAME_TREMOR_TAIL_H_

#include "cocos2d.h"

#include "Windy/Entities/Logical.h"

namespace game {
    class TremorDrill;
}

namespace game {
    class TremorTail : public windy::Logical
    {
    public:
        static TremorTail* create();

        virtual bool init();

        virtual void onEnter();
        virtual void onExit();

        virtual void onFinished() override;

        void parseBehavior(const cocos2d::ValueMap& behavior);

        void onUpdate(float dt);

        TremorDrill* drill;

    };

}

#endif