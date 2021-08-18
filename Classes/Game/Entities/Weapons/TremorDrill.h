#ifndef __GAME_TREMOR_DRILL_H__
#define __GAME_TREMOR_DRILL_H__

#include "cocos2d.h"

#include "Game/Entities/Resources.h"
#include "Windy/Entities/Weapon.h"

namespace game {
    class TremorDrill : public windy::Weapon
    {
    public:
        static game::Resources& getResources();

        static TremorDrill* create();

        virtual bool init() override;

        void normalizeCollisionRectangles();

        virtual void recomputeCollisionRectangles() override;


        //static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);


        void enableAttack();
        void disableAttack();

        virtual void update(float dt) override;
        virtual void onUpdate(float dt) override;

    };
}

#endif
