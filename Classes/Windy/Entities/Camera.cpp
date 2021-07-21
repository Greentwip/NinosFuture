#include <string>

#include "Camera.h"

#include "Bounds.h"
#include "Player.h"

#include "./../GameTags.h"

#include "./../Armature.h"


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

    this->collisionRectangles = armature.get("camera").collisionRectangles;

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(this, *this->collisionRectangles[i]);
    }

    this->collisionBox = this->collisionRectangles[0];

    this->setTag(GameTags::General::Camera);

    return true;
}

void Camera::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Camera::onUpdate(float dt) {


    if (this->cameraMode != CameraFlags::CameraMode::Shift) {
        // manual solve collisions
    }

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
        this->level->pause(true);

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

                if (this->bossDoorShift) {
                    this->level->player->setPositionY(playerPosition.y + this->protoShiftSpeed + 0.15f);
                }
                else {
                    this->level->player->setPositionY(playerPosition.y + this->protoShiftSpeed);
                }
            }


            if (this->shiftDirection == CameraFlags::CameraShift::ShiftDown) {
                this->level->bounds->setPositionY(boundsPosition.y - this->shiftSpeed);

                if (this->bossDoorShift) {
                    this->level->player->setPositionY(playerPosition.y - this->protoShiftSpeed - 0.15f);
                }
                else {
                    this->level->player->setPositionY(playerPosition.y - this->protoShiftSpeed);
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

                        this->level->pause(false);
                    });
                }
            }
            else {
                if (!this->bossDoorWorking && this->targetDoor == nullptr) {
                    this->cameraMode = CameraFlags::CameraMode::Screen;

                    this->level->pause(false);
                }
            }

        }

    }

    this->getScene()->getDefaultCamera()->setPosition(this->level->bounds->getPosition());

}