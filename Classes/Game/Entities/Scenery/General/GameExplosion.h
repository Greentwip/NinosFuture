#ifndef __GAME_EXPLOSION_H__
#define __GAME_EXPLOSION_H__

#include <memory>

#include "cocos2d.h"

#include "Windy/GameTags.h"
#include "Windy/Entities/Explosion.h"
#include "Game/Entities/Resources.h"

namespace game {
    class GameExplosion : public windy::Explosion
    {
    public:
        static game::Resources& getResources();

        static GameExplosion* create();

        virtual bool init() override;

        virtual void setup(cocos2d::Color3B spriteColor) override;

        virtual void fire(float angle) override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);
    };
}

#endif
