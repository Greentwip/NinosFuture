#ifndef WINDY_DISPLAY_H
#define WINDY_DISPLAY_H

namespace windy {
    class Display {
    public:
        cocos2d::Size size;
        float contentScaleFactor;
        int width;
        int height;
        int cx;
        int cy;
        int c_left;
        int c_right;
        int c_top;
        int c_bottom;
        int left;
        int right;
        int top;
        int bottom;
        cocos2d::Point center;
        cocos2d::Point left_top;
        cocos2d::Point left_bottom;
        cocos2d::Point left_center;
        cocos2d::Point right_top;
        cocos2d::Point right_bottom;
        cocos2d::Point right_center;
        cocos2d::Point top_center;
        cocos2d::Point top_bottom;

    public:
        static Display& getInstance()
        {
            static Display instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }

    public:
        Display(Display const&) = delete;
        void operator=(Display const&) = delete;

    private:
        Display() {
            size = cocos2d::Director::getInstance()->getWinSize();
            contentScaleFactor = cocos2d::Director::getInstance()->getContentScaleFactor();
            width = (int)size.width;
            height = (int)size.height;
            cx = (int)(width / 2);
            cy = (int)(height / 2);
            c_left = (int)(-width / 2);
            c_right = (int)(width / 2);
            c_top = (int)(height / 2);
            c_bottom = (int)(-height / 2);
            left = 0;
            right = width;
            top = height;
            bottom = 0;
            center = cocos2d::Point((float)cx, (float)cy);
            left_top = cocos2d::Point((float)left, (float)top);
            left_bottom = cocos2d::Point((float)left, (float)bottom);
            left_center = cocos2d::Point((float)left, (float)cy);
            right_top = cocos2d::Point((float)right, (float)top);
            right_bottom = cocos2d::Point((float)right, (float)bottom);
            right_center = cocos2d::Point((float)right, (float)cy);
            top_center = cocos2d::Point((float)cx, (float)top);;
            top_bottom = cocos2d::Point((float)cx, (float)bottom);
        }
    };
}

#endif
