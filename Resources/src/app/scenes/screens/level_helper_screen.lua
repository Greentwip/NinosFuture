-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local level_helper_screen = import("app.core.gameplay.control.layout_base").create("level_helper_screen")

function level_helper_screen:onLoad()

    self.triggered_ = false

end


function level_helper_screen:step(dt)

    if not self.triggered_ then
        self.triggered_ = true
        local callback = cc.CallFunc:create(function()

            print("Callback")

            self:getApp()
                :enterScene("levels.level", "FADE", 1, {physics = true})

        end)

        local delay = cc.DelayTime:create(1)

        local sequence = cc.Sequence:create(delay, callback, nil)

        self:runAction(sequence)    
    end

    self:post_step(dt)
    return self


end


return level_helper_screen