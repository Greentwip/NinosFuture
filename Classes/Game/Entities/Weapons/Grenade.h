#ifndef __GAME_GRENADE_H__
#define __GAME_GRENADE_H__

#include "cocos2d.h"

#include "Game/Entities/Resources.h"
#include "Windy/Entities/Weapon.h"

#include <memory>

namespace game {

    class Grenade : public windy::Weapon {
        bool _rising = false;
        float _riseSpeed = 8.0f;
        float _moveSpeed = 1.0f;
    public:
        static game::Resources& getResources();
        static Grenade* create();
        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        bool init() override;
        void setup();
        void onFinished() override;
        void onUpdate(float dt) override;
        void fire(bool flipped);
    };

}

#endif
