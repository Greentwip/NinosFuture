#include "GameLevelController.h"

#include "Game/Entities/UI/GameGui.h"
#include "Game/Entities/UI//EnergyBar.h"
#include "Game/Entities/UI/Pause/PauseMenu.h"
#include "Game/Entities/UI/Pause/PauseInterruptor.h"
#include "Game/Entities/Player/GamePlayer.h"

#include "Game/GameManager.h"

#include "Game/GameStateMachine.h"

#include "Windy/GameTags.h"
#include "Windy/AudioManager.h"
#include "Windy/Input.h"

#include "Windy/Entities/Player.h"
#include "Windy/Entities/Boss.h"
#include "Windy/Entities/Bounds.h"

#if _DEBUG
#include "Windy/Entities/Camera.h"
#endif


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

	this->levelState = LevelState::Startup;

	this->pauseMenu = nullptr;

	this->manualPause = false;

	this->cameraPause = false;

	this->gui = dynamic_cast<GameGui*>(this->level->gui);

	this->_player = dynamic_cast<GamePlayer*>(this->level->player);


	return true;
}


void GameLevelController::restart() {
	this->levelState = LevelState::Restarting;
}


void GameLevelController::succeed() {
	this->levelState = LevelState::Succeeded;
}

void GameLevelController::onUpdate(float dt) {

	switch (this->levelState) {
		case LevelState::Startup:
		{
			//windy::AudioManager::stopAll();
			this->atVictory = false;
			this->fading = false;
			this->levelState = LevelState::Playing;
			this->gui->bossHealthBar->setVisible(false);
		}
		break;

		case LevelState::Restarting:
		{
			if (!this->fading) {
				this->fading = true;

				if (GameManager::getInstance().player.lives <= 0) {
					GameStateMachine::getInstance().pushState(GameState::GameOver);
					levelState = LevelState::GameOver;
				}
				else {
					this->levelState = LevelState::Startup;
				}

			}
			
		}
			
		break;

		case LevelState::Playing:
		{
			if (this->_player->spawning) {
				this->gui->healthBar->setVisible(false);
				this->gui->weaponBar->setVisible(false);
			}
			else {
				this->gui->healthBar->setVisible(true);

				if (this->_player->getAcquiringWeapon()) {
					this->gui->healthBar->setValue(-2);
					this->gui->weaponBar->setValue(-2);
					this->gui->healthBar->setVisible(false);
					this->gui->weaponBar->setVisible(false);
				}


			}

			if (this->level->boss != nullptr) {
				this->gui->bossHealthBar->setVisible(true);

				//windy::AudioManager::playBgm(windy::Sounds::BossTheme);

				this->levelState = LevelState::BossBattle;
			}
			else {
				//auto gameGui = dynamic_cast<GameGui*>(this->level->gui);
				//gameGui->bossHealthBar->setVisible(false);
			}
		}
		break;


		case LevelState::BossBattle:
		{
			
			if (this->level->boss != nullptr) {
				switch (this->level->boss->state) {
					case windy::Boss::BossState::Teleporting:
					case windy::Boss::BossState::Intro:
					case windy::Boss::BossState::Spawning:
					case windy::Boss::BossState::RestoringHealth:
						this->_player->canMove = false;
					break;
				}

				switch (this->level->boss->state) {
					case windy::Boss::BossState::Fighting:
						this->_player->canMove = true;
					break;
						
				}
				
			}
			else {
				
				this->_player->canMove = false;
				this->succeed();

			}

		}
		break;

		case LevelState::Succeeded:
		{	
			if (!this->atVictory) {
				this->atVictory = true;
				this->_player->cancelAttacks();

				windy::AudioManager::playSfx(windy::Sounds::Victory, false, [this]() {
					this->_player->exit();
				});
			}


			switch (this->_player->screenState) {
				case windy::Player::ScreenState::OffScreen:
					this->exitTimer = this->exitTimeDelay;
					this->levelState = LevelState::Exit;
					break;
			}
			
		} 
		break;

		case LevelState::Exit: {
			if (this->exitTimer <= 0) {
				this->exitTimer = 0;
				/*if (!this->fading) {
					this->fading = true;
				}*/

				GameStateMachine::getInstance().pushState(GameState::GetWeapon);
				levelState = LevelState::GameOver;

				//this->level->restart();

				
			}
			else {
				this->exitTimer -= dt;
			}
		}
		break;
	}


	
	switch (this->levelState) {
		case LevelState::Playing: 
		case LevelState::BossBattle:
		{
			if (!this->_player->spawning) {
				if (windy::Input::keyPressed(windy::InputKey::Select)) {
					bool pausedThisFrame = false;

					if (!this->level->getPaused() && this->pauseMenu == nullptr && this->_player->canMove) {

						auto gameGui = dynamic_cast<GameGui*>(this->level->gui);

						this->pauseMenu = PauseMenu::create(this->_player, gameGui);
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

					if (this->level->getPaused() && this->pauseMenu != nullptr && this->_player->canMove && !pausedThisFrame) {

						if (!this->pauseMenu->busy) {
							if (this->pauseMenu->selectedBrowner != nullptr) {
								this->_player->switchBrowner(this->pauseMenu->selectedBrowner->brownerId);
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

					if (!this->level->getPaused() && this->pauseMenu == nullptr && this->_player->canMove && !this->manualPause) {
						bool freezePlayer;
						this->level->setPaused(true, freezePlayer = true);

						pausedThisFrame = true;
						this->manualPause = true;
					}

					if (this->level->getPaused() && this->pauseMenu == nullptr && this->_player->canMove &&  !pausedThisFrame && this->manualPause) {
						bool freezePlayer;
						this->level->setPaused(false, freezePlayer = true);
						this->manualPause = false;
					}

				}

#if _DEBUG
				/*if (windy::Input::keyPressed(windy::InputKey::Start)) {

					bool pausedThisFrame = false;

					if (!this->cameraPause) {
						this->level->camera->pause();
						this->cameraPause = true;
						pausedThisFrame = true;
					}

					if (this->cameraPause && !pausedThisFrame) {
						this->level->camera->resume();
						this->cameraPause = false;
					}

				}*/
#endif
			}			

		}
		break;
	}


}