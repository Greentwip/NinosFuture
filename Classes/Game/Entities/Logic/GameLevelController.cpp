#include "GameLevelController.h"

#include "Game/Entities/UI/GameGui.h"
#include "Game/Entities/UI//EnergyBar.h"
#include "Game/Entities/UI/Pause/PauseMenu.h"
#include "Game/Entities/UI/Pause/PauseInterruptor.h"
#include "Game/Entities/Player/GamePlayer.h"

#include "Game/GameManager.h"

#include "Windy/GameTags.h"
#include "Windy/AudioManager.h"
#include "Windy/Input.h"

#include "Windy/Entities/Player.h"
#include "Windy/Entities/Boss.h"
#include "Windy/Entities/Bounds.h"


using namespace game;


bool GameLevelController::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Node::init())
	{
		return false;
	}

#if _DEBUG
	GameManager::getInstance().loadGameDataFromDefaultSlot();
	GameManager::getInstance().browners.sheriff->acquired = true;
#endif


	this->setTag(windy::GameTags::General::None);

	this->atVictory = false;

	this->exitTimer = 0;
	this->exitTimeDelay = 2;

	this->fading = false;

	this->gameState = GameState::Startup;

	this->pauseMenu = nullptr;

	this->manualPause = false;

	this->gui = dynamic_cast<GameGui*>(this->level->gui);


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
			this->gui->bossHealthBar->setVisible(false);
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
			if (this->level->player->spawning) {
				this->gui->healthBar->setVisible(false);
			}
			else {
				this->gui->healthBar->setVisible(true);
			}

			if (this->level->boss != nullptr) {
				this->gui->bossHealthBar->setVisible(true);

				//windy::AudioManager::playBgm(windy::Sounds::BossTheme);

				this->gameState = GameState::BossBattle;
			}
			else {
				//auto gameGui = dynamic_cast<GameGui*>(this->level->gui);
				//gameGui->bossHealthBar->setVisible(false);
			}
		}
		break;


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


	
	switch (this->gameState) {
		case GameState::Playing: 
		{
			if (!this->level->player->spawning) {
				if (windy::Input::keyPressed(windy::InputKey::Select)) {
					bool pausedThisFrame = false;

					if (!this->level->getPaused() && this->pauseMenu == nullptr && this->level->player->canMove) {

						auto gameGui = dynamic_cast<GameGui*>(this->level->gui);

						this->pauseMenu = PauseMenu::create(this->level->player, gameGui);
						this->pauseMenu->setVisible(true);

						auto pauseMenuPosition =
							cocos2d::Point(
								this->level->bounds->collisionBox->size.width * 0.5f * -1,
								this->level->bounds->collisionBox->size.height * 0.5f);

						this->pauseMenu->setPosition(pauseMenuPosition);
						this->level->bounds->addChild(this->pauseMenu, 4096);

						bool freezePlayer;

						this->level->setPaused(true, freezePlayer = true);

						pausedThisFrame = true;

					}

					if (this->level->getPaused() && this->pauseMenu != nullptr && this->level->player->canMove && !pausedThisFrame) {

						if (!this->pauseMenu->busy) {
							if (this->pauseMenu->selectedBrowner != nullptr) {
								auto gamePlayer = dynamic_cast<GamePlayer*>(this->level->player);

								gamePlayer->switchBrowner(this->pauseMenu->selectedBrowner->brownerId);
							}

							this->pauseMenu->removeFromParent();
							this->pauseMenu = nullptr;

							bool freezePlayer;

							this->level->setPaused(false, freezePlayer = true);
						}
						
					}

				}

				if (windy::Input::keyPressed(windy::InputKey::Start)) {
					bool pausedThisFrame = false;

					if (!this->level->getPaused() && this->pauseMenu == nullptr && this->level->player->canMove && !this->manualPause) {
						bool freezePlayer;
						this->level->setPaused(true, freezePlayer = true);

						pausedThisFrame = true;
						this->manualPause = true;
					}

					if (this->level->getPaused() && this->pauseMenu == nullptr && this->level->player->canMove &&  !pausedThisFrame && this->manualPause) {
						bool freezePlayer;
						this->level->setPaused(false, freezePlayer = true);
						this->manualPause = false;
					}

				}
			}
			

		}
		break;

		case GameState::BossBattle:
		{
			if (this->level->boss != nullptr) {
				switch (this->level->boss->state) {
					case windy::Boss::BossState::Fighting:
						
					break;

				}

			}
		}
		break;


	}


}