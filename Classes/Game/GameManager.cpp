#include <algorithm>

#include "GameManager.h"

void GameManager::buildHighScoreTable() {

	scoreTable = std::vector<std::pair<std::string, uint64_t>>{
		{"Mony", 15},
		{"Edgar", 30},
		{"Alan", 60},
		{"Mike", 90},
		{"Johny", 120},
		{"Joseph", 150},
		{"Bob", 180},
		{"Dylan", 210},
		{"Jimmy", 240},
		{"Heimdal", 360}
	};

	std::sort(scoreTable.begin(), scoreTable.end(), [](const auto& a, const auto& b) { return a.second > b.second; });

}

void GameManager::setSessionPoints(const uint64_t& points) {
	sessionPoints = points;
}

uint64_t GameManager::getSessionPoints() {
	return sessionPoints;
}

void GameManager::tryPostHighScore(std::string name, uint64_t score) {

	std::vector<std::pair<std::string, uint64_t>>::iterator scoreIterator;

	bool brokeMark;
	for (auto it = scoreTable.begin(); it != scoreTable.end(); ++it) {
		if (score >= it->second) {
			scoreIterator = it;
			brokeMark = true;
			break;
		}
	}

	if (brokeMark) {
		scoreTable.insert(scoreIterator, { name, score });
		scoreTable.pop_back();
	}
}

std::vector<std::pair<std::string, uint64_t>> const GameManager::getHighScoreTable() {
	return scoreTable;
}

void GameManager::setPlayerLives(uint8_t lives) {
	playerLives = lives;
}

uint8_t GameManager::getPlayerLives() {
	return playerLives;
}
