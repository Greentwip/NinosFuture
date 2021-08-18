#ifndef __GAME_DIRECTIONAL_BULLET_H__
#define __GAME_DIRECTIONAL_BULLET_H__

#include <memory>

#include "cocos2d.h"

#include "Windy/GameTags.h"
#include "Game/Entities/Resources.h"
#include "Windy/Entities/Weapon.h"

namespace game {
    class DirectionalBullet : public windy::Weapon
    {
    public:
        static game::Resources& getResources();

        static DirectionalBullet* create();

        virtual bool init() override;

        void setup();

        void fire(int power, int direction, windy::GameTags::Weapon tag);

        void fire(int power, const cocos2d::Point& playerPosition, windy::GameTags::Weapon tag);


        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void onFinished() override;

        virtual void onUpdate(float dt) override;
    };
}

#endif
