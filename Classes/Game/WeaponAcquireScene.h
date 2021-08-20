#ifndef __WEAPON_ACQUIRE_SCENE_H__
#define __WEAPON_ACQUIRE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace game {
    class GamePlayer;
    class Fader;
}

namespace game {
    class WeaponAcquireScene : public cocos2d::Layer
    {
    public:
        virtual bool init();

        static cocos2d::Scene* scene();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);

        CREATE_FUNC(WeaponAcquireScene);

    private:
        GamePlayer* _player;

        int _targetBrownerId;
        bool _morphing;
        bool _playerActionsFinished;

        float _exitTimer;
        float _exitTimeDelay;
        bool _exitingScreen;
        bool _transitioning;

        Fader* _fader;
    };

}
#endif
