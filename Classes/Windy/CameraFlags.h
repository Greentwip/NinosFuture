#ifndef __WINDY_CAMERA_FLAGS_H__
#define __WINDY_CAMERA_FLAGS_H__


namespace windy {
    class CameraFlags {
    public:
        enum CameraMode {
            Screen = 1,
            Scroll,
            Shift
        };

        enum CameraScroll {
            ScrollUp = 1,
            ScrollDown,
            ScrollLeft,
            ScrollRight,
            ScrollMoving,
            ScrollNone
        };

        enum CameraShift {
            ShiftUp = 1,
            ShiftDown,
            ShiftLeft,
            ShiftRight,
            ShiftNone
        };
    };
    
}

#endif