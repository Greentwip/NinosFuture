#ifndef _GAME_SOUND_TEST_MENU_H_
#define _GAME_SOUND_TEST_MENU_H_

#include <string>
#include <vector>
#include <map>

#include "cocos2d.h"

namespace game {
    class SoundTestMenu : public cocos2d::Node
    {
    public:
        static void preloadResources();

        static SoundTestMenu* create();

        virtual bool init();

        virtual void onEnter();
        virtual void onExit();

        virtual void update(float dt);


    private:
        cocos2d::Node* _root;

        int _currentTrackId;

        std::vector<std::string> _tracks;
        std::vector<std::string> _trackIdentifiers;

        std::map<std::string, float> _amplitudes;

        bool _playing;
    };

}

#endif
