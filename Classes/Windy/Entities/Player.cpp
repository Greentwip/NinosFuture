#include <string>

#include "Player.h"

#include "Ladder.h"
#include "Browner.h"
#include "Camera.h"

#include "./../GameTags.h"

#include "./../Sprite.h"

#include "./../Armature.h"
#include "./../AnimationAction.h"

#include "./../Input.h"

#include "./../CameraFlags.h"

using namespace windy;

void Player::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Player::initVariables() {

    this->walking = false;
    this->jumping = false;
    this->dashJumping = false;
    this->sliding = false;
    this->climbing = false;
    this->attacking = false;
    this->charging = false;
    this->stunned = false;
    this->flashing = false;
    this->onGround = false;

    this->walkSpeed = 1;
    this->climbSpeed = 1;
    this->slideSpeed = 3;
    this->jumpSpeed = 5;
    this->dashJumpSpeed = 6.5f;
    this->stunTimer = 0;
    this->slideTimer = 0;
    this->climbCounter = 0;

    this->largeSlide = false;

    this->speed = cocos2d::Point(0, 0);

    this->cameraShiftSpeedBackup = false;
    this->shiftSpeedBackup = cocos2d::Point(0, 0);
    
}


void Player::onCollisionEnter(Logical* collision) {

    if (collision->getTag() == GameTags::General::Door) {
        this->level->triggeringDoor = dynamic_cast<Door*>(collision);
    }
    else if (collision->getTag() == GameTags::General::Ladder) {
        this->activeLadder = dynamic_cast<Ladder*>(collision);
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

void Player::triggerActions() {

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


void Player::onUpdate(float dt) {

    if (this->canMove) {
        this->move();

        if (this->spawning && this->alive) {
            // solve collisions, old code, remove
            //this->solveCollisions();
        }
        else if (this->alive) {
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
                //this->checkHealth();
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
        }
    }
    else {
        if (!this->demoMode) {
            if (!this->atExit) {
                // solve collisions, old code, remove
                //this->solveCollisions();
                this->speed.x = 0;
                this->speed.y = 0;
            }
            else {
                this->speed.x = 0;
                this->speed.y = 320;
            }
        }
        else {
            if (this->atExit) {
                this->speed.x = 0;
                this->speed.y = 320;
            }
        }
    }

    this->triggerActions();
}