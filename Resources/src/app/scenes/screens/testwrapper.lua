-- Copyright 2014-2015 Greentwip. All Rights Reserved.
require "config"


local testwrapper = import("app.core.gameplay.control.layout_base").create("testwrapper")

function testwrapper:onLoad()
    self.triggered_ = false
end

function testwrapper:step(dt)
    self:post_step(dt)

    if not self.triggered_ then
        self.triggered_ = true

    --cc.current_level_ = { mug_="sheriffmantest" }    cc.current_level_ = cc.levels_[10]

        if CC_DEBUG_MUG ~= "none" then

            -- Levels
            --level_mugs[1] = "freezerman"
            --level_mugs[2] = "sheriffman"
            --level_mugs[3] = "boomerman"
            --level_mugs[4] = "militaryman"
            --level_mugs[5] = "vineman"
            --level_mugs[6] = "shieldman"
            --level_mugs[7] = "nightman"
            --level_mugs[8] = "torchman"
            --level_mugs[9] = "test"
            --level_mugs[10] = "sheriffmantest"
            --level_mugs[11] = "militarymantest"
            --level_mugs[12] = "nightmantest"
            --level_mugs[13] = "vinemantest"
        

            cc.current_level_ = { mug_=CC_DEBUG_MUG }    

            local level = "levels.level"
            local physics = true
    
            local init = false
    
            local scene = self:getApp()
                              :enterScene(level, "FADE", 1, {physics = physics})
    
            if init then
                scene:prepare(self.exit_arguments_)
            end
    
        else
            print("Must define debug mug")
            
        end

    end
    return self
end

return testwrapper