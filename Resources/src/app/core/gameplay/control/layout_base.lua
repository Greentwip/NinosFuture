-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local joypad = import("app.core.gameplay.ui.joypad")

local layout_base = {}

function layout_base.create(class_name)

    local layout = class(class_name, cc.load("mvc").ViewBase)

    function layout:onCreate()
        -- add keypad layer
--        self.keypad_layer_ = display.newLayer()
--                                    :onKeypad(handler(self, self.onKeypad))
--                                    :addTo(self)

        -- binding to the "event" component
        cc.bind(self, "event")

        -- schedule update
        self:start()

        if self.onLoad then
            self:onLoad()
        end
    end

    function layout:onAfterCreate()
        self.joypad_ = joypad:create(self):addTo(self, 4096)

        if cc.platform_ == "mobile" then
            self.blackout_a_ = cc.Sprite:create("sprites/core/blackout/bar.png")
            :setAnchorPoint(cc.p(0, 0))
            :setPosition(cc.p(display.left_bottom.x - 85, display.left_bottom.y))
            :addTo(self, 2048)

            self.blackout_b_ = cc.Sprite:create("sprites/core/blackout/bar.png")
            :setAnchorPoint(cc.p(1, 0))
            :setPosition(cc.p(display.right_bottom.x - 85, display.right_bottom.y))
            :addTo(self, 2048)                                    
        end

        if self.onAfterLoad then
            self:onAfterLoad()
        end
    end

    function layout:onAfterLoad()

    end

    function layout:start()
        self:scheduleUpdate(handler(self, self.step))
        return self
    end

    function layout:stop()
        self:unscheduleUpdate()
        return self
    end

    function layout:onCleanup()
        self:removeAllEventListeners()
    end

    function layout:post_step(dt)
        self.joypad_:step(dt)
    end


    return layout
end

return layout_base