#ifndef __WINDY_GAME_MANAGER__
#define __WINDY_GAME_MANAGER__

#include <cstdint>
#include <string>
#include <vector>

class GameManager {
public:
    static GameManager& getInstance()
    {
        static GameManager instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
        return instance;
    }
private:
    GameManager() :playerLives(3), sessionPoints(0) { buildHighScoreTable(); }

public:
    GameManager(GameManager const&) = delete;
    void operator=(GameManager const&) = delete;

public:
    void setSessionPoints(const uint64_t& points);

    uint64_t getSessionPoints();

    void tryPostHighScore(std::string name, uint64_t score);

    std::vector<std::pair<std::string, uint64_t>> const getHighScoreTable();

    void setPlayerLives(uint8_t lives);

    uint8_t getPlayerLives();
private:
    void buildHighScoreTable();

private:
    uint64_t sessionPoints;
    uint8_t playerLives;
    std::vector<std::pair<std::string, uint64_t>> scoreTable;
};


#endif