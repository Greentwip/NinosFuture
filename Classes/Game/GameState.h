#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

namespace game {

    enum class GameState {
        Intro,
        Abakura,
        Title,
        Options,
        Save,
        StageSelect,
        BossIntro,
        Game,
        GetWeapon,
        GameOver,
    };

}

#endif 