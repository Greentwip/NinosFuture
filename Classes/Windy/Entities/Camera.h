#ifndef __WINDY_CAMERA_H__
#define __WINDY_CAMERA_H__

#include "cocos2d.h"

#include "Logical.h"

#include "Door.h"

#include "CameraScroll.h"

#include "./../CameraFlags.h"

namespace windy {
    class Camera : public Logical
    {
    public:
        virtual bool init() override;

        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;
        virtual void onUpdate(float dt) override;

        enum CameraFlags::CameraMode cameraMode;
        enum CameraFlags::CameraScroll scroll;
        enum CameraFlags::CameraShift shiftDirection;

    private:
        int shiftSpeed;
        bool bossDoorShift;
        Door* targetDoor;
        bool bossDoorWorking;
        float protoShiftSpeed;
        int shiftMoveCount;

        //Player* player;
        cocos2d::Point speed;
        int tolerance;
        Logical* freeScroll;
        CameraScroll* fixedScroll;
    };
}

#endif
