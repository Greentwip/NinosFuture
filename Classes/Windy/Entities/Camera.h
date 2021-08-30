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
        virtual void update(float dt) override;
        virtual void onUpdate(float dt) override;


        virtual void recomputeCollisionRectangles() override;

        virtual void onCollisionEnter(Logical* collision) override;

        virtual void onCollision(Logical* collision) override;

        void normalizeCollisionRectangles();

        void synchronizeWithBounds();

        enum CameraFlags::CameraMode cameraMode;
        enum CameraFlags::CameraScroll scroll;
        enum CameraFlags::CameraShift shiftDirection;

        int tolerance;

        float catchupSpeed;

    private:
        int shiftSpeed;
        bool bossDoorShift;
        Door* targetDoor;
        bool bossDoorWorking;
        float protoShiftSpeed;
        int shiftMoveCount;

        //Player* player;
        cocos2d::Point childLastPosition;

        cocos2d::Point speed;
        Logical* freeScroll;
        CameraScroll* fixedScroll;
    };
}

#endif
