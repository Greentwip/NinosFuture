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

#include "Windy/Display.h"

#if _DEBUG
#include "Windy/Entities/Camera.h"
#endif

#include "Game/Entities/UI/Fader.h"
#include "Game/Entities/UI/ReadyIndicator.h"


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

	this->levelState = LevelState::Startup;

	this->pauseMenu = nullptr;

	this->manualPause = false;

	this->cameraPause = false;

	this->gui = dynamic_cast<GameGui*>(this->level->gui);

	this->_player = dynamic_cast<GamePlayer*>(this->level->player);

	_restartFader = nullptr;
	_pauseFader = nullptr;
	
	_onLevelRestarted = nullptr;

	return true;
}


void GameLevelController::startup() {
	this->levelState = LevelState::Startup;
}

void GameLevelController::restart(std::function<void()> onLevelRestarted) {
	_onLevelRestarted = onLevelRestarted;
	this->levelState = LevelState::Restarting;
}

void GameLevelController::succeed() {
	this->levelState = LevelState::Succeeded;
}

void GameLevelController::exitLevel() {
	this->exitTimer = this->exitTimeDelay;
	this->levelState = LevelState::Exit;
}

void GameLevelController::onUpdate(float dt) {

	switch (this->levelState) {
		case LevelState::Startup:
		{
			this->level->setPaused(true, true);

			this->gui->healthBar->setVisible(false);
			this->gui->weaponBar->setVisible(false);

			//windy::AudioManager::stopAll();
			this->atVictory = false;
			this->gui->bossHealthBar->setVisible(false);

			if (_restartFader == nullptr) {

				windy::AudioManager::stopAll();
				windy::AudioManager::playBgm(this->level->bgm);

				auto fader = Fader::create(cocos2d::Point(0.5f, 0.5f));

				fader->setPosition(cocos2d::Point(0, 0));

				fader->setOpacity(255);

				this->level->bounds->addChild(fader, 4096);

				_restartFader = fader;

				_restartFader->fadeOut([this]() {
					_readyIndicator = ReadyIndicator::create([this]() {
						levelState = LevelState::Playing;
						this->level->setPaused(false, true);

						_restartFader->removeFromParent();
						_restartFader = nullptr;

						_readyIndicator->removeFromParent();
						_readyIndicator = nullptr;
					});

					float indicatorPositionY = windy::Display::getInstance().height * 0.125f;
					_readyIndicator->setPosition(cocos2d::Point(0, indicatorPositionY));

					this->level->bounds->addChild(_readyIndicator, 2048);
					
				});
			}
		}
		break;

		case LevelState::Restarting:
		{
			if (_restartFader == nullptr) {

				this->level->setPaused(true, true);

				auto fader = Fader::create(cocos2d::Point(0.5f, 0.5f));

				fader->setPosition(cocos2d::Point(0, 0));

				fader->setOpacity(0);

				this->level->bounds->addChild(fader, 4096);

				_restartFader = fader;

				_restartFader->fadeIn([=]() {
					if (GameManager::getInstance().player.lives <= 0) {
						GameStateMachine::getInstance().pushState(GameState::GameOver);
						this->levelState = LevelState::GameOver;
					}
					else {
						this->levelState = LevelState::Startup;

						if (this->_onLevelRestarted) {
							this->_onLevelRestarted();
							this->_onLevelRestarted = nullptr;
						}
					}

					this->_restartFader->removeFromParent();
					this->_restartFader = nullptr;
				});

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

				switch (this->level->boss->state) 
				{
					case windy::Boss::BossState::Defeated:
						this->_player->vulnerable = false;
					break;
				}
				
			}
			else {
				
				this->_player->canMove = false;
				this->_player->vulnerable = false;
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
					this->levelState = LevelState::ExitToWeaponScene;
					break;
			}
			
		} 
		break;

		case LevelState::ExitToWeaponScene: 
		{
			if (this->exitTimer <= 0) {
				this->exitTimer = 0;
				if (_restartFader == nullptr) {
					auto fader = Fader::create(cocos2d::Point(0.5f, 0.5f));

					fader->setPosition(cocos2d::Point(0, 0));

					fader->setOpacity(255);

					this->level->bounds->addChild(fader, 4096);

					_restartFader = fader;

					_restartFader->fadeIn([this]() {
						GameStateMachine::getInstance().pushState(GameState::GetWeapon);
						levelState = LevelState::GameOver;

						_restartFader->removeFromParent();
						_restartFader = nullptr;
					});
				}


				//this->level->restart();

				
			}
			else {
				this->exitTimer -= dt;
			}
		}
		break;

		case LevelState::Exit: 
		{
			if (this->exitTimer <= 0) {
				this->exitTimer = 0;
				if (_restartFader == nullptr) {
					auto fader = Fader::create(cocos2d::Point(0.5f, 0.5f));

					fader->setPosition(cocos2d::Point(0, 0));

					fader->setOpacity(255);

					this->level->bounds->addChild(fader, 4096);

					_restartFader = fader;

					_restartFader->fadeIn([this]() {
						GameStateMachine::getInstance().pushState(GameState::StageSelect);
						levelState = LevelState::GameOver;

						_restartFader->removeFromParent();
						_restartFader = nullptr;
					});
				}


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

					if (!this->level->getPaused() && 
						this->pauseMenu == nullptr && 
						_pauseFader == nullptr &&
						this->_player->canMove) {

						windy::AudioManager::playSfx(windy::Sounds::Pause);

						bool freezePlayer;

						this->level->setPaused(true, freezePlayer = true);


						auto fader = Fader::create(cocos2d::Point(0.5f, 0.5f));

						fader->setPosition(cocos2d::Point(0, 0));

						fader->setOpacity(0);

						this->level->bounds->addChild(fader, 4096);

						_pauseFader = fader;

						_pauseFader->fadeIn([this]() {
							this->pauseMenu = PauseMenu::create(this->_player, gui);
							this->pauseMenu->setVisible(true);
							auto pauseMenuPosition =
								cocos2d::Point(
									this->level->bounds->collisionBox->size.width * 0.5f * -1,
									this->level->bounds->collisionBox->size.height * 0.5f);

							this->pauseMenu->setPosition(pauseMenuPosition);
							this->level->bounds->addChild(this->pauseMenu, 2048);

							_pauseFader->fadeOut([this]() {

								_pauseFader->removeFromParent();
								_pauseFader = nullptr;
								});
														
						});


						pausedThisFrame = true;


					}

					if (this->level->getPaused() && 
						this->pauseMenu != nullptr && 
						_pauseFader == nullptr &&
						this->_player->canMove && 
						!pausedThisFrame) {

						if (!this->pauseMenu->busy) {

							this->pauseMenu->busy = true;

							windy::AudioManager::playSfx(windy::Sounds::Pause);

							auto fader = Fader::create(cocos2d::Point(0.5f, 0.5f));

							fader->setPosition(cocos2d::Point(0, 0));

							fader->setOpacity(0);

							this->level->bounds->addChild(fader, 4096);

							_pauseFader = fader;

							_pauseFader->fadeIn([this]() {

								if (this->pauseMenu->selectedBrowner != nullptr) {
									this->_player->switchBrowner(this->pauseMenu->selectedBrowner->brownerId);
								}

								this->pauseMenu->removeFromParent();
								this->pauseMenu = nullptr;

								bool freezePlayer;

								this->level->setPaused(false, freezePlayer = true);


								_pauseFader->fadeOut([this]() {

									_pauseFader->removeFromParent();
									_pauseFader = nullptr;
									});

								});


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