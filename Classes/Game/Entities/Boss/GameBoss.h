#ifndef __GAME_BOSS_H__
#define __GAME_BOSS_H__

#include "cocos2d.h"

#include "Windy/Entities/Boss.h"
#include "Windy/Entities/Browner.h"
#include "Game/Entities/Resources.h"

namespace game {
    class GameGui;
}

namespace game {
    class GameBoss : public windy::Boss
    {
    public:
        static game::Resources& getResources();

        virtual bool init() override;

        static std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size);

        virtual void onFinished() override;

        void switchBrowner(int brownerId);

        virtual void setupBrowners() override;

        virtual windy::Browner* getBrowner(int brownerId) override;       

        virtual void walk() override;

        virtual void jump() override;

        virtual void kill(bool killAnimation) override;

        virtual void restoreHealth(int amount) override;

        virtual void checkHealth() override;

        virtual void onUpdate(float dt) override;

        GameGui* gui;

        bool restoringHealth;

    private:
        cocos2d::Vector<windy::Browner*> browners;
    };
}

#endif
