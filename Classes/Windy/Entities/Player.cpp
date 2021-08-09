#include <string>

#include "Player.h"

#include "Ladder.h"
#include "Browner.h"
#include "Camera.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Bounds.h"

#include "./../GameTags.h"
#include "./../Sprite.h"
#include "./../Armature.h"
#include "./../AnimationAction.h"
#include "./../Input.h"
#include "./../CameraFlags.h"
#include "./../AudioManager.h"
#include "./../EntityFactory.h"
#include "./../ObjectManager.h"
#include "./../GeometryExtensions.h"
#include "./../PhysicsWorld.h"

using namespace windy;

void Player::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Player::initVariables() {

    this->walking = false;
    this->jumping = true;
    this->dashJumping = false;
    this->sliding = false;
    this->climbing = false;
    this->attacking = false;
    this->charging = false;
    this->stunned = false;
    this->flashing = false;
    this->onGround = false;
    this->vulnerable = true;

    this->walkSpeed = 1;
    this->climbSpeed = 1;
    this->slideSpeed = 3;
    this->jumpSpeed = 5;
    this->dashJumpSpeed = 6.5f;
    this->stunTimer = 0;
    this->slideTimer = 0;
    this->climbCounter = 0;

    this->exitSpeed = 4;

    this->alive = true;
    this->canMove = false;
    this->spawning = true;

    this->health = 28;

    this->largeSlide = false;

    this->speed = cocos2d::Point(0, -4);

    this->cameraShiftSpeedBackup = false;
    this->shiftSpeedBackup = cocos2d::Point(0, 0);

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;
    
}

void Player::onRestart() {

    this->initVariables();

}

void Player::onSpawn() {

}


void Player::onCollisionEnter(Logical* collision) {

    if (collision->getTag() == GameTags::General::Door) {
        this->level->triggeringDoor = dynamic_cast<Door*>(collision);
    }
    else if (collision->getTag() == GameTags::General::Ladder) {
        this->activeLadder = dynamic_cast<Ladder*>(collision);
    }
    else if (collision->getTag() == GameTags::General::Teleporter) {

        if (this->spawning) {
            this->spawning = false;

            this->ignoreGravity = false;
            this->ignoreLandscapeCollision = false;

            this->canMove = true;
            this->onGround = true;

            this->speed = cocos2d::Point(0, 0);

            this->contacts[CollisionContact::Down] = true;

            this->level->physicsWorld->alignCollisions(this, collision, false);


            AudioManager::playSfx(windy::Sounds::Teleport1);

            this->onSpawn();
        }
    }
}

void Player::onCollision(Logical* collision) {

    if (collision->getTag() == GameTags::General::Enemy) {
        auto enemy = dynamic_cast<Enemy*>(collision);
        this->stun(enemy->power);
    }
    else if (collision->getTag() == GameTags::Weapon::WeaponEnemy) {
        auto weapon = dynamic_cast<Weapon*>(collision);
        this->stun(weapon->power);
    }

}



void Player::onCollisionExit(Logical* collision) {

    if (collision->getTag() == GameTags::General::Door) {
        this->level->triggeringDoor = nullptr;
    }
    else if (collision->getTag() == GameTags::General::Ladder) {
        this->activeLadder = nullptr;
    }

}

void Player::stun(int power) {
    if (!this->stunned && this->vulnerable) {
        windy::AudioManager::playSfx(windy::Sounds::PlayerHit);

        this->health -= power;

        if (this->health <= 0) {
            this->health = 0;
        }

        this->stunned = true;
        this->vulnerable = false;

        if (!this->largeSlide) {
            this->sliding = false;
            this->slideTimer = 0;
        }
        
        this->currentBrowner->chargePower = "low";
        this->charging = false;

        auto delay = cocos2d::DelayTime::create(this->currentBrowner->getActionDuration("hurt"));

        if (!this->sliding) {
            this->speed.x = -0.5f * this->currentBrowner->getSpriteNormal();
        }

        auto callback = cocos2d::CallFunc::create([this]() {
            this->stunned = false; 
        });

        auto blink = cocos2d::Blink::create(this->currentBrowner->getActionDuration("hurt"), 8);

        auto blinkCallback = cocos2d::CallFunc::create([this]() {
            if (!this->isVisible()) {
                this->setVisible(true);
            }

            if (!this->sprite->isVisible()) {
                this->sprite->setVisible(true);
            }

            this->vulnerable = true;
        });

        auto sequence = cocos2d::Sequence::create(delay, callback, blink, blink, blinkCallback, nullptr);

        sequence->setTag(windy::GameTags::Visibility);

        this->sprite->stopAllActionsByTag(windy::GameTags::Visibility);
        ((cocos2d::Node*)this->sprite)->runAction(sequence);

    }
}

void Player::walk() {

    if (!this->climbing && !this->sliding && !this->stunned) {
        bool walkLeftCondition = Input::keyDown(InputKey::Left) && !Input::keyDown(InputKey::Right);
        bool walkRightCondition = Input::keyDown(InputKey::Right) && !Input::keyDown(InputKey::Left);

        if (walkRightCondition) {
            this->sprite->setFlippedX(false);
            this->speed.x = static_cast<float>(this->walkSpeed);
            this->walking = true;
        }
        else if (walkLeftCondition) {
            this->sprite->setFlippedX(true);
            this->speed.x = static_cast<float>(-this->walkSpeed);
            this->walking = true;
        }
        else {
            this->speed.x = 0;
            this->walking = false;
        }
    }
    else {
        this->walking = false;
    }
}

void Player::jump() {

    bool jumpCondition =
        Input::keyPressed(InputKey::A) &&
        !Input::keyDown(InputKey::Up) &&
        !Input::keyDown(InputKey::Down);

    bool stopJumpCondition = !Input::keyDown(InputKey::A);

    if (jumpCondition && this->onGround && !this->sliding && !this->stunned) {
        this->speed.y = static_cast<float>(this->jumpSpeed);
        this->onGround = false;
        this->jumping = true;
    }

    if (stopJumpCondition && this->speed.y >= 0 && !this->climbing && !this->onGround) {
        this->speed.y = 0;
    }
}

void Player::dashJump() {
    bool jumpCondition =
        Input::keyPressed(InputKey::A) &&
        Input::keyDown(InputKey::Up) &&
        !Input::keyDown(InputKey::Down);

    bool stopJumpCondition = !Input::keyDown(InputKey::A);

    if (jumpCondition && this->onGround && !this->sliding && !this->stunned) {
        this->speed.y = static_cast<float>(this->dashJumpSpeed);
        this->onGround = false;
        this->jumping = true;
        this->dashJumping = true;
    }

    if (stopJumpCondition && this->speed.y >= 0 && !this->climbing && !this->onGround) {
        this->speed.y = 0;
        this->dashJumping = false;
    }
    else if (this->speed.y <= 0 && !this->climbing && !this->onGround) {
        this->dashJumping = false;
    }
}

void Player::slide() {
    bool slideCondition =
        Input::keyPressed(InputKey::A) &&
        Input::keyDown(InputKey::Down);

    bool leftSlideCondition =
        Input::keyPressed(InputKey::LB);

    bool rightSlideCondition =
        Input::keyPressed(InputKey::RB);

    if (leftSlideCondition && this->onGround && !this->sliding && !this->stunned && !this->attacking) {
        if (!(this->walking && !this->sprite->isFlippedX())) {
            this->sliding = true;
            this->slideTimer = 32;
        }

        this->collisionBox = this->collisionRectangles[1];
        this->contacts[CollisionContact::Left] = false;
        this->contacts[CollisionContact::Right] = false;

        this->sprite->setFlippedX(true);
    }


    if (rightSlideCondition && this->onGround && !this->sliding && !this->stunned && !this->attacking) {
        if (!(this->walking && this->sprite->isFlippedX())) {
            this->sliding = true;
            this->slideTimer = 32;
        }

        this->collisionBox = this->collisionRectangles[1];
        this->contacts[CollisionContact::Left] = false;
        this->contacts[CollisionContact::Right] = false;

        this->sprite->setFlippedX(false);
    }

    if (slideCondition && this->onGround && !this->sliding && !this->stunned && !this->attacking) {
        this->sliding = true;
        this->slideTimer = 32;

        this->collisionBox = this->collisionRectangles[1];
        this->contacts[CollisionContact::Left] = false;
        this->contacts[CollisionContact::Right] = false;
    }

    if (this->slideTimer > 0) {
        this->slideTimer -= 1;

        this->largeSlide = false;

        if (this->contacts[CollisionContact::Up]) {
            this->slideTimer += 1;
            this->sliding = true;
            this->largeSlide = true;

            if (Input::keyDown(InputKey::Left) && !Input::keyDown(InputKey::Right)) {
                this->sprite->setFlippedX(true);
                this->speed.x = static_cast<float>(-this->slideSpeed);
            }
            else if (Input::keyDown(InputKey::Right) && !Input::keyDown(InputKey::Left)) {
                this->sprite->setFlippedX(false);
                this->speed.x = static_cast<float>(this->slideSpeed);
            }
        }

        if (this->sprite->isFlippedX()) {
            if (this->contacts[CollisionContact::Left]) {
                this->speed.x = 0;

                if (!this->largeSlide) {
                    this->slideTimer = 0;
                    this->sliding = false;
                }
            }
            else {
                this->speed.x = static_cast<float>(-this->slideSpeed);
            }
        }
        else {
            if (this->contacts[CollisionContact::Right]) {
                this->speed.x = 0;

                if (!this->largeSlide) {
                    this->slideTimer = 0;
                    this->sliding = false;
                }
            }
            else {
                this->speed.x = static_cast<float>(this->slideSpeed);
            }
        }

        if (!this->onGround) {
            this->slideTimer = 0;
            this->sliding = false;
            this->speed.x = 0;
        }
        else if ((this->sprite->isFlippedX() && Input::keyDown(InputKey::Right) && !Input::keyDown(InputKey::Left))
                 || (!this->sprite->isFlippedX() && Input::keyDown(InputKey::Left) && !Input::keyDown(InputKey::Right))
                 || (this->onGround && Input::keyDown(InputKey::A) && this->slideTimer < 8)) {
            this->slideTimer = 0;
            this->sliding = false;
            this->speed.x = 0;
        }
    }
    else {
        if (this->sliding) {
            this->sliding = false;
        }
    }
    
    if (!this->sliding) {
        this->collisionBox = this->collisionRectangles[0];
    }

}

void Player::climbShift() {

    if (this->activeLadder != nullptr) {
        this->climbCounter += 1;
    }

    if (this->activeLadder != nullptr) {
        if (this->climbCounter >
            ((this->currentBrowner->getActionDuration("climb") /
                this->currentBrowner->getCurrentAnimationNumberOfFrames()) * 60)) {
            this->currentBrowner->increaseOrLoopFrame();
            this->climbCounter = 0;
        }
    }
}

void Player::climb() {

    int climbDirection = -2;

    if (this->activeLadder != nullptr && !this->sliding && !this->stunned) {
        if (Input::keyDown(InputKey::Up) && !Input::keyDown(InputKey::Down)) {
            climbDirection = 1;
            this->climbCounter += 1;
        }
        else if (Input::keyDown(InputKey::Down) && !Input::keyDown(InputKey::Up)) {
            climbDirection = -1;
            this->climbCounter += 1;
        }
        else {
            if (this->climbing) {
                climbDirection = 0;
            }
            else {
                climbDirection = -2;
            }
            
        }

        if (this->speed.y > 0 && !this->climbing) {
            climbDirection = -2;
        }
    }
    else if (this->stunned) {
        if (this->climbing) {
            climbDirection = 0;
        }
    }

    if (climbDirection != -2 && this->activeLadder != nullptr) {
        float actionDuration = this->currentBrowner->getActionDuration("climb");
        int animationNumberOfFrames = this->currentBrowner->getCurrentAnimationNumberOfFrames();
        if (this->climbCounter > ((actionDuration / animationNumberOfFrames) * 60)) {
            this->currentBrowner->increaseOrLoopFrame();
            this->climbCounter = 0;
        }

        float speedXBackup = this->speed.x;
        bool onGroundBackup = this->onGround;

        this->climbing = true;
        this->onGround = false;
        this->ignoreGravity = true;
        this->ignoreLandscapeCollision = true;

        if (this->speed.x != 0) {
            this->speed.x = 0;
        }

        if (climbDirection == 1) {
            if (this->getPositionY() >= this->activeLadder->collisionBox->getMaxY()) {
                if (this->collisionBox->getMinY() < this->activeLadder->collisionBox->getMaxY()) {
                    this->climbing = false;
                    this->ignoreGravity = false;
                    this->ignoreLandscapeCollision = false;
                    this->climbCounter = 0;
                    this->setPositionY(this->activeLadder->collisionBox->getMaxY() + 16);
                    this->activeLadder->solidify();
                }
                else {
                    this->climbing = false;
                    this->ignoreGravity = false;
                    this->ignoreLandscapeCollision = false;
                    this->climbCounter = 0;
                    this->speed.x = speedXBackup;

                    this->onGround = onGroundBackup;
                }
            }
            else {
                if (this->activeLadder->solidified) { // pull the player up
                    this->setPositionY(this->getPositionY() + 4);
                }
                this->activeLadder->unsolidify();
                this->speed.y = static_cast<float>(this->climbSpeed);
            }
        }
        else if (climbDirection == -1) {
            if (this->collisionBox->getMinY() < this->activeLadder->collisionBox->getMinY()) {
                this->climbing = false;
                this->ignoreGravity = false;
                this->ignoreLandscapeCollision = false;
                this->climbCounter = 0;
                this->speed.x = speedXBackup;
                this->onGround = onGroundBackup;
                this->activeLadder->solidify();
            }
            else {
                this->speed.y = static_cast<float>(-this->climbSpeed);
                this->activeLadder->unsolidify();
            }
        }
        else {
            if (this->stunned) {
                this->currentBrowner->resumeActions();
                this->speed.y = -12;
            }
            else {
                this->speed.y = 0;
                this->currentBrowner->pauseActions();
            }
        }

        if (this->attacking) {
            this->speed.y = 0;
        }
    }
    else {
        this->climbing = false;
        this->ignoreGravity = false;
        this->ignoreLandscapeCollision = false;
        this->climbCounter = 0;
        this->currentBrowner->resumeActions();
    }

    if (this->climbing) {
        this->setPositionX(this->activeLadder->getPositionX());
    }
}

void Player::move() {

    if (this->contacts[CollisionContact::Down]) {
        this->speed.y = 0;

        if (!this->onGround && !this->climbing && !this->spawning) {
            this->onGround = true;
            this->dashJumping = false;
            this->jumping = false;

            AudioManager::playSfx(Sounds::Land);
        }
    }
    else {
        this->onGround = false;
        this->jumping = true;
    }
    
    if (this->contacts[CollisionContact::Up]) {
        if (this->speed.y > 0) {
            this->speed.y = -1;
        }
    }

    if (!this->level->paused()) {
        this->walk();
        this->jump();
        this->dashJump();
    }
    else {
        this->speed.x = 0;
        this->speed.y = 0;
    }

    if (this->contacts[CollisionContact::Right]) {
        if (this->speed.x > 0) {
            this->speed.x = 0;
        }
    }
    else if (this->contacts[CollisionContact::Left]) {
        if (this->speed.x < 0) {
            this->speed.x = 0;
        }
    }
}

void Player::explode(float angleOffset) {

    for (int i = 0; i < 8; ++i) {

        auto explosionPosition = this->getPosition();

        auto entryCollisionBox = EntityFactory::getInstance().getEntryCollisionRectangle("explosion", explosionPosition, cocos2d::Size(16, 16));

        auto entry = Logical::getEntry(entryCollisionBox, [=]() {
            auto explosion = dynamic_cast<Explosion*>(EntityFactory::getInstance().create("explosion", explosionPosition, cocos2d::Size(16, 16)));

            explosion->setPosition(explosionPosition);
            explosion->setup(cocos2d::Color3B(153, 153, 255));

            explosion->fire(GeometryExtensions::degreesToRadians(static_cast<float>(angleOffset)));

            return explosion;
            });

        this->level->objectManager->objectEntries.push_back(entry);

        angleOffset -= 45;
    }
}

void Player::kill(bool killAnimation) {
}

void Player::checkHealth() {
    bool killAnimation = true;

    auto playerCollisionBox = this->collisionBox;
    auto boundsCollisionBox = this->level->bounds->collisionBox;

    if (playerCollisionBox->getMinY() < boundsCollisionBox->getMinY()) {
        this->health = 0;
        killAnimation = false;
    }

    if (this->health <= 0 && this->alive) {
        this->level->pause(true);

        this->currentBrowner->deactivate();

        this->health = 0;

        this->alive = false;
        this->vulnerable = false;

        AudioManager::stopAll();

        AudioManager::playSfx(windy::Sounds::Death);

        this->kill(killAnimation);
    }
}

void Player::triggerActions() {

    if (this->alive) {

        if (!this->stunned) {
            if (this->onGround) {
                if (this->walking) {
                    if (this->attacking) {
                        this->currentBrowner->runAction("walkshoot");
                    }
                    else {
                        this->currentBrowner->runAction("walk");
                    }
                }
                else {
                    if (this->attacking) {
                        this->currentBrowner->runAction("standshoot");
                    }
                    else if (this->sliding) {
                        this->currentBrowner->runAction("slide");
                    }
                    /*else if (this->morphing) {
                        this->currentBrowner->runAction("morph");
                    }*/
                    else {
                        this->currentBrowner->runAction("stand");
                    }
                }
            }
            else {
                if (this->climbing) {
                    if (this->attacking) {
                        this->currentBrowner->runAction("climbshoot");
                    }
                    else {
                        this->currentBrowner->runAction("climb");
                    }
                }
                else {
                    if (this->attacking) {
                        this->currentBrowner->runAction("jumpshoot");
                    }
                    else {
                        if (this->dashJumping) {
                            this->currentBrowner->runAction("dashjump");
                        }
                        else if (this->jumping) {
                            this->currentBrowner->runAction("jump");
                        }
                    }
                }
            }
        }
        else {
            this->currentBrowner->runAction("hurt");
        }
    }

}


void Player::onUpdate(float dt) {

    /*if (Input::keyPressed(InputKey::Select)) {
        this->explode(0);
        this->explode(22.5f);
    }*/

    this->triggerActions();

    if (this->canMove) {

        if (this->level->paused()) {
            if (this->level->camera->shiftDirection != CameraFlags::CameraShift::ShiftNone) {
                if (!cameraShiftSpeedBackup) {
                    cameraShiftSpeedBackup = true;
                    this->shiftSpeedBackup = this->speed;
                    this->ignoreGravity = true;
                    this->speed.y = 0;
                    this->speed.x = 0;
                }
            }

        }


        this->move();


        /*if (this->spawning && this->alive) {
            // solve collisions, old code, remove
            //this->solveCollisions();
        }
        else */
        if (this->alive) {
            if (!this->level->paused()) {

                if (cameraShiftSpeedBackup) {
                    cameraShiftSpeedBackup = false;
                    this->speed = this->shiftSpeedBackup;
                    this->ignoreGravity = false;
                }

                // solve collisions, old code, remove
                //this->solveCollisions();
                this->currentBrowner->attack();
                this->currentBrowner->charge();
                this->slide();
                this->climb();
                this->checkHealth();
            }
            else {
                if ((this->level->camera->shiftDirection == CameraFlags::CameraShift::ShiftUp ||
                    this->level->camera->shiftDirection == CameraFlags::CameraShift::ShiftDown) &&
                    this->climbing) {
                    this->climbShift();
                }

                if ((this->level->camera->shiftDirection == CameraFlags::CameraShift::ShiftLeft ||
                    this->level->camera->shiftDirection == CameraFlags::CameraShift::ShiftRight) &&
                    (this->onGround && !this->sliding && !this->stunned && !this->attacking)) {
                    this->currentBrowner->runAction("walk");
                }
            }
        }
    }
    else {
        if (!this->demoMode) {
            if (!this->atExit) {
                // solve collisions, old code, remove
                //this->solveCollisions();
                this->speed.x = 0;

                if (!this->spawning) {
                    this->speed.y = 0;
                }
                
            }
            else {
                this->speed.x = 0;
                this->speed.y = this->exitSpeed;
            }
        }
        else {
            if (this->atExit) {
                this->speed.x = 0;
                this->speed.y = this->exitSpeed;
            }
        }
    }

}