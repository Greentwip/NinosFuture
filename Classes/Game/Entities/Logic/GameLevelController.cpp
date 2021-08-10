#include "GameLevelController.h"

#include "Game/Entities/UI/GameGui.h"
#include "Game/Entities/UI//EnergyBar.h"

#include "Windy/GameTags.h"

#include "Windy/AudioManager.h"

#include "Windy/Entities/Player.h"
#include "Windy/Entities/Boss.h"


using namespace game;


bool GameLevelController::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Node::init())
	{
		return false;
	}

	this->setTag(windy::GameTags::General::None);

	this->atVictory = false;

	this->exitTimer = 0;
	this->exitTimeDelay = 2;

	this->fading = false;

	this->gameState = GameState::Startup;



	return true;
}


void GameLevelController::restart() {
	this->gameState = GameState::Restarting;
}


void GameLevelController::succeed() {
	this->gameState = GameState::Finishing;
}

void GameLevelController::onUpdate(float dt) {

	switch (this->gameState) {
		case GameState::Startup:
		{
			//windy::AudioManager::stopAll();
			this->atVictory = false;
			this->fading = false;
			this->gameState = GameState::Playing;
			auto gameGui = dynamic_cast<GameGui*>(this->level->gui);
			gameGui->bossHealthBar->setVisible(false);
		}
		break;

		case GameState::Restarting:
		{
			if (!this->fading) {
				this->fading = true;

				this->gameState = GameState::Startup;
			}
			
		}
			
		break;

		case GameState::Playing: 
		{
			if (this->level->boss != nullptr) {
				auto gameGui = dynamic_cast<GameGui*>(this->level->gui);
				gameGui->bossHealthBar->setVisible(true);

				//windy::AudioManager::playBgm(windy::Sounds::BossTheme);

				this->gameState = GameState::BossBattle;
			}
			else {
				//auto gameGui = dynamic_cast<GameGui*>(this->level->gui);
				//gameGui->bossHealthBar->setVisible(false);
			}
			break;
		}
			

		case GameState::BossBattle: 
		{
			
			if (this->level->boss != nullptr) {
				switch (this->level->boss->state) {
					case windy::Boss::BossState::Teleporting:
					case windy::Boss::BossState::Intro:
					case windy::Boss::BossState::Spawning:
					case windy::Boss::BossState::RestoringHealth:
						this->level->player->canMove = false;
					break;
				}

				switch (this->level->boss->state) {
					case windy::Boss::BossState::Fighting:
						this->level->player->canMove = true;
					break;
						
				}
				
			}
			else {
				
				this->level->player->canMove = false;
				this->succeed();

			}

		}
		break;

		case GameState::Finishing: 
		{	
			if (!this->atVictory) {
				this->atVictory = true;
				this->level->player->cancelAttacks();

				windy::AudioManager::playSfx(windy::Sounds::Victory, false, [this]() {
					this->level->player->exit();
				});
			}


			switch (this->level->player->screenState) {
				case windy::Player::ScreenState::OffScreen:
					this->exitTimer = this->exitTimeDelay;
					this->gameState = GameState::Exit;
					break;
			}
			


			
		} 
		break;

		case GameState::Exit: {
			if (this->exitTimer <= 0) {
				this->exitTimer = 0;
				/*if (!this->fading) {
					this->fading = true;
				}*/

				this->level->restart();

				
			}
			else {
				this->exitTimer -= dt;
			}
		}

			break;
	}


	


}