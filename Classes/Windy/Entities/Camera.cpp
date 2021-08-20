#include <string>

#include "Camera.h"

#include "Bounds.h"
#include "Player.h"
#include "Scroll.h"

#include "./../GameTags.h"

#include "./../Armature.h"

#include "Windy/PhysicsWorld.h"


using namespace windy;

class CameraResources {
public:
    static std::string armaturePath;
};

std::string CameraResources::armaturePath = "physics/gameplay/level/internal/camera/camera";


bool Camera::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->cameraMode = CameraFlags::CameraMode::Screen;
    this->scroll = CameraFlags::CameraScroll::ScrollNone;
    this->shiftDirection = CameraFlags::CameraShift::ShiftNone;

    this->shiftSpeed = 4;
    this->bossDoorShift = false;

    this->targetDoor = nullptr;

    this->bossDoorWorking = false;

    this->protoShiftSpeed = 0.3f;
    this->shiftMoveCount = 0;

    this->speed = cocos2d::Point(0, 0);
    this->tolerance = 16;
    this->freeScroll = nullptr;
    this->fixedScroll = nullptr;

    /*this->staticMode = false;
    this->staticPosition = cocos2d::Point(0, 0);*/

    auto armature = Armature(CameraResources::armaturePath);

    Logical::setup(this->getPosition(), armature.get("camera").collisionRectangles[0]->size);

    this->collisionRectangles = armature.get("camera").collisionRectangles;


    this->setTag(GameTags::General::Camera);

    this->ignoreGravity = true;
    this->ignoreLandscapeCollision = true;

    return true;
}

void Camera::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Camera::onCollision(Logical* collision) {
    if (collision->getTag() == GameTags::General::Scroll) {
        auto scrollCollision = dynamic_cast<Scroll*>(collision);

        if (this->scroll == CameraFlags::CameraScroll::ScrollMoving) {
            this->scroll = CameraFlags::CameraScroll::ScrollNone;
            this->cameraMode = CameraFlags::CameraMode::Screen;
        }

        if (this->cameraMode == CameraFlags::CameraMode::Screen) {
            this->cameraMode = CameraFlags::CameraMode::Scroll;
            
        }

        if (this->cameraMode == CameraFlags::CameraMode::Scroll) {
            this->scroll = scrollCollision->scroll;
        }

        this->level->physicsWorld->alignCollisions(this, scrollCollision, true);
        this->level->bounds->setPositionX(this->collisionBox->getMidX());
    }
}

void Camera::normalizeCollisionRectangles() {
    auto worldPosition = this->getParent()->convertToWorldSpace(this->getPosition());
    this->lastCollisionPosition = worldPosition;
    this->childLastPosition = this->getPosition();

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(worldPosition, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];

}

void Camera::recomputeCollisionRectangles() {
    auto parentPositionDifference = this->getParent()->getPosition() - lastCollisionPosition;
    auto currentPositionDifference = this->getPosition() - childLastPosition;

    this->lastCollisionPosition = this->getParent()->getPosition();
    this->childLastPosition = this->getPosition();

    auto positionDifference = parentPositionDifference + currentPositionDifference;

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        float differenceX = positionDifference.x;
        float differenceY = positionDifference.y;

        this->collisionRectangles[i]->origin.x += differenceX;
        this->collisionRectangles[i]->origin.y += differenceY;
    }
}

void Camera::synchronizeWithBounds() {
    this->getScene()->getDefaultCamera()->setPosition(this->level->bounds->getPosition());
}



void Camera::update(float dt) {
    this->recomputeCollisionRectangles();

    this->setPosition(0, 0);

    this->onUpdate(dt);
}

void Camera::onUpdate(float dt) {

    /*if (this->cameraMode != CameraFlags::CameraMode::Shift) {
        // manual solve collisions
    }*/

    if (!this->level->player->alive) {
        return;
    }

    /*if (this->staticMode) {
        auto defaultCamera = this->getScene()->getDefaultCamera();
        defaultCamera->setPosition(this->staticPosition.x, this->staticPosition.y);
    }*/

    if (this->cameraMode == CameraFlags::CameraMode::Scroll) {

        if (this->scroll == CameraFlags::CameraScroll::ScrollLeft && this->level->player->getPositionX() < this->level->bounds->center().x) {
            this->scroll = CameraFlags::CameraScroll::ScrollMoving;
        }

        if (this->scroll == CameraFlags::CameraScroll::ScrollRight && this->level->player->getPositionX() > this->level->bounds->center().x) {
            this->scroll = CameraFlags::CameraScroll::ScrollMoving;
        }

        if (this->scroll == CameraFlags::CameraScroll::ScrollMoving) {
            auto worldPosition = this->convertToWorldSpace(this->getPosition());
            auto playerPosition = this->level->player->getPosition();
            auto boundsPosition = this->level->bounds->getPosition();

            auto positionDifference = playerPosition - worldPosition;
            
            if (playerPosition.x > worldPosition.x + this->tolerance) {
                if (playerPosition.x - this->tolerance > worldPosition.x) {
                    this->level->bounds->setPositionX(boundsPosition.x + positionDifference.x - tolerance);
                }
            }
            else if (playerPosition.x < worldPosition.x - this->tolerance) {
                if (playerPosition.x + this->tolerance < worldPosition.x) {
                    this->level->bounds->setPositionX(boundsPosition.x + positionDifference.x + tolerance);
                }
            }
        }
    }

    if (this->cameraMode != CameraFlags::CameraMode::Shift) {

        auto playerPosition = this->level->player->getPosition();

        float playerCollisionTop = this->level->player->collisionBox->getMaxY();
        float playerCollisionBottom = this->level->player->collisionBox->getMinY();

        int boundsTop = this->level->bounds->top();
        int boundsBottom = this->level->bounds->bottom();
        int boundsLeft = this->level->bounds->left();
        int boundsRight = this->level->bounds->right();

        if (playerPosition.x >= boundsRight) {
            this->shiftDirection = CameraFlags::CameraShift::ShiftRight;
        } 
        else if (playerPosition.x <= boundsLeft) {
            this->shiftDirection = CameraFlags::CameraShift::ShiftLeft;
        } 
        else if (playerCollisionBottom < boundsBottom) {
            this->shiftDirection = CameraFlags::CameraShift::ShiftDown;
        }
        else if (playerCollisionTop > boundsTop && this->level->player->climbing) {
            this->shiftDirection = CameraFlags::CameraShift::ShiftUp;
        }
        else {
            this->shiftDirection = CameraFlags::CameraShift::ShiftNone;
        }

        if (this->level->triggeringDoor != nullptr) {
            auto triggeringDoorPosition = this->level->triggeringDoor->getPosition();
            
            if (triggeringDoorPosition.x > playerPosition.x && !this->bossDoorShift) {
                this->shiftDirection = CameraFlags::CameraShift::ShiftRight;
            }
            else if (triggeringDoorPosition.x < playerPosition.x && !this->bossDoorShift) {
                this->shiftDirection = CameraFlags::CameraShift::ShiftLeft;
            }
        }

        if (this->shiftDirection != CameraFlags::CameraShift::ShiftNone) {
            this->cameraMode = CameraFlags::CameraMode::Shift;
        }

        if (this->shiftDirection == CameraFlags::CameraShift::ShiftLeft || this->shiftDirection == CameraFlags::CameraShift::ShiftRight) {
            this->shiftMoveCount = 256;
        } 
        else if (this->shiftDirection == CameraFlags::CameraShift::ShiftUp || this->shiftDirection == CameraFlags::CameraShift::ShiftDown) {
            this->shiftMoveCount = 224;
        }
    }


    if (this->fixedScroll != nullptr && this->cameraMode != CameraFlags::CameraMode::Shift) {
        auto cameraSize = this->collisionBox->size;
        auto fixedScrollSize = this->fixedScroll->collisionBox->size;

        auto boundsPosition = this->level->bounds->getPosition();
        auto scrollPosition = this->fixedScroll->getPosition();

        if (this->fixedScroll->scroll == CameraFlags::CameraScroll::ScrollRight) {
            if (boundsPosition.x - cameraSize.width * 0.5f < scrollPosition.x + fixedScrollSize.width * 0.5f) {
                this->level->bounds->setPositionX(scrollPosition.x + cameraSize.width * 0.5f + fixedScrollSize.width * 0.5f);
            }
        }

        if (this->fixedScroll->scroll == CameraFlags::CameraScroll::ScrollLeft) {
            if (boundsPosition.x + cameraSize.width * 0.5f > scrollPosition.x - fixedScrollSize.width * 0.5f) {
                this->level->bounds->setPositionX(scrollPosition.x - cameraSize.width * 0.5f - fixedScrollSize.width * 0.5f);
            }
        }
    }

    if (this->cameraMode == CameraFlags::CameraMode::Shift) {
        this->level->setPaused(true);

        if (this->targetDoor == nullptr) {
            if (this->level->triggeringDoor != nullptr && !this->bossDoorShift) {
                this->targetDoor = this->level->triggeringDoor;
                this->bossDoorShift = true;
                this->bossDoorWorking = true;

                this->targetDoor->unlock([this]() {
                    this->bossDoorWorking = false;
                });
            }
        }

        if (!this->bossDoorWorking && this->shiftMoveCount > 0) {
            
            auto boundsPosition = this->level->bounds->getPosition();
            auto playerPosition = this->level->player->getPosition();

            if (this->shiftDirection == CameraFlags::CameraShift::ShiftRight) {
                this->level->bounds->setPositionX(boundsPosition.x + this->shiftSpeed);

                if (this->bossDoorShift) {
                    this->level->player->setPositionX(playerPosition.x + this->protoShiftSpeed + 0.5f);
                }
                else {
                    this->level->player->setPositionX(playerPosition.x + this->protoShiftSpeed);
                }
            }

            if (this->shiftDirection == CameraFlags::CameraShift::ShiftLeft) {
                this->level->bounds->setPositionX(boundsPosition.x - this->shiftSpeed);

                if (this->bossDoorShift) {
                    this->level->player->setPositionX(playerPosition.x - this->protoShiftSpeed - 0.5f);
                }
                else {
                    this->level->player->setPositionX(playerPosition.x - this->protoShiftSpeed);
                }
            }

            if (this->shiftDirection == CameraFlags::CameraShift::ShiftUp) {

                this->level->bounds->setPositionY(boundsPosition.y + this->shiftSpeed);

                if (this->level->player->climbing) {
                    if (this->bossDoorShift) {
                        this->level->player->setPositionY(playerPosition.y + this->protoShiftSpeed + 0.25f);
                    }
                    else {
                        this->level->player->setPositionY(playerPosition.y + this->protoShiftSpeed + 0.15f);
                    }
                }
                else {
                    if (this->bossDoorShift) {
                        this->level->player->setPositionY(playerPosition.y + this->protoShiftSpeed + 0.15f);
                    }
                    else {
                        this->level->player->setPositionY(playerPosition.y + this->protoShiftSpeed);
                    }
                }
                
            }


            if (this->shiftDirection == CameraFlags::CameraShift::ShiftDown) {
                this->level->bounds->setPositionY(boundsPosition.y - this->shiftSpeed);

                if (this->level->player->climbing) {
                    if (this->bossDoorShift) {
                        this->level->player->setPositionY(playerPosition.y - this->protoShiftSpeed - 0.25f);
                    }
                    else {
                        this->level->player->setPositionY(playerPosition.y - this->protoShiftSpeed - 0.15f);
                    }
                }
                else {
                    if (this->bossDoorShift) {
                        this->level->player->setPositionY(playerPosition.y - this->protoShiftSpeed - 0.15f);
                    }
                    else {
                        this->level->player->setPositionY(playerPosition.y - this->protoShiftSpeed);
                    }
                }
                
            }


            this->shiftMoveCount -= this->shiftSpeed;
        }
        

        if (this->shiftMoveCount <= 0) {
            
            if (this->bossDoorShift) {
                this->bossDoorShift = false;
                if (this->targetDoor != nullptr) {
                    this->targetDoor->lock([this]() {

                        this->cameraMode = CameraFlags::CameraMode::Screen;
                        this->bossDoorShift = false;
                        this->targetDoor = nullptr;
                        this->bossDoorWorking = false;
                        this->level->triggeringDoor = nullptr;

                        this->level->setPaused(false);
                    });
                }
            }
            else {
                if (!this->bossDoorWorking && this->targetDoor == nullptr) {
                    this->cameraMode = CameraFlags::CameraMode::Screen;

                    this->level->setPaused(false);
                }
            }

        }

    }

    this->getScene()->getDefaultCamera()->setPosition(this->level->bounds->getPosition());

}