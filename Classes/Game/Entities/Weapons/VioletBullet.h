#ifndef __GAME_VIOLET_BULLET_H__
#define __GAME_VIOLET_BULLET_H__

#include "Windy/Entities/Browner.h"
#include "Windy/Entities/Resources.h"
#include "Windy/Entities/Weapon.h"
#include "Windy/GameTags.h"

#include "cocos2d.h"

#include <string>
#include <memory>

namespace game {
    class VioletBullet : public windy::Weapon
    {
    public:
        static windy::Resources& getResources(windy::Browner::ChargePower);

        static VioletBullet* create();

        virtual bool init() override;

        void setup(windy::Browner::ChargePower powerLevel);

        void fire(int power, int direction, windy::GameTags::Weapon tag);

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(windy::Browner::ChargePower powerLevel,
                                                                         const cocos2d::Point& position,
                                                                         const cocos2d::Size& size);

        virtual void onFinished() override;

        virtual void onUpdate(float dt) override;
    };
}

#endif
