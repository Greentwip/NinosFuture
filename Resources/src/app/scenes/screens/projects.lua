-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local credits = import("app.core.gameplay.control.layout_base").create("credits")

function credits:onLoad()

    if cc.platform_ == "mobile" then
        self:setPositionX(85)
    end
    
    local root = cc.CSLoader:createNode("sprites/gameplay/screens/projects/data.csb")
    root:addTo(self)

    self.triggered_ = false
    self.done_ = true


    root:getChildByName("text"):getLabel():getFontAtlas():setAliasTexParameters()
end

function credits:step(dt)

    if not self.triggered_ and self.done_ then
        if cc.key_pressed(cc.key_code_.a) then
            self.triggered_ = true
            cc.audio.play_sfx("sounds/sfx_selected.mp3")

            self:getApp()
            :enterScene("screens.title", "FADE", 0.5)
        end
       
    end

    self:post_step(dt)

    return self
end

function credits:prepare(args)

end

return credits