#ifndef __GAME_VINE_BALL_H__
#define __GAME_VINE_BALL_H__

#include "cocos2d.h"

#include "Game/Entities/Resources.h"
#include "Windy/Entities/Weapon.h"
#include "Windy/GameTags.h"

#include <memory>

namespace game {

    class VineBall : public windy::Weapon {
    public:
        static game::Resources& getResources();
        static VineBall* create();
        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        bool init() override;
        void setup();
        void onFinished() override;
        void onUpdate(float dt) override;
        void fire(int power, int direction, windy::GameTags::Weapon tag, const cocos2d::Point& weaponSpeed = cocos2d::Point(1.2f, 4.0f));

    private:
        bool _onGround;
    };

}

#endif
