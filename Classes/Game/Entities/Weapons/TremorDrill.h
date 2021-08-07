#ifndef __GAME_TREMOR_DRILL_H__
#define __GAME_TREMOR_DRILL_H__

#include "cocos2d.h"

#include "Windy/Entities/Weapon.h"

namespace game {
    class TremorDrill : public windy::Weapon
    {
    public:
        static void preloadResources();

        static TremorDrill* create();

        virtual bool init() override;

        void normalizeCollisionRectangles();

        //static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);


        void enableAttack();
        void disableAttack();

        virtual void onUpdate(float dt) override;

    };
}

#endif
