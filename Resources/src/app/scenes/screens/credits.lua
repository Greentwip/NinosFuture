-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local credits = import("app.core.gameplay.control.layout_base").create("credits")

function credits:onLoad() -- weird bug when using onLoad

    if cc.platform_ == "mobile" then
        self:setPositionX(85)
    end
    
    local root = cc.CSLoader:createNode("sprites/gameplay/screens/credits/data.csb")
    root:addTo(self)

    self.triggered_ = false
    self.done_ = false


    root:getChildByName("credits"):getLabel():getFontAtlas():setAliasTexParameters()

    local root_timeline = cc.CSLoader:createTimeline("sprites/gameplay/screens/credits/data.csb")

    root:runAction(root_timeline)

    root_timeline:gotoFrameAndPause(0)


    local fly_primary_delay = cc.DelayTime:create((1.0 / 60.0) * 1200)


    local fly_in_primary_callback = cc.CallFunc:create(function()
        self.done_ = true
    end)

    local fly_sequence  = cc.Sequence:create(fly_primary_delay, 
                                             fly_in_primary_callback,
                                             nil)


    root_timeline:play("scroll", false)    


    self:runAction(fly_sequence)
                                      
    --cc.audio.play_bgm("sounds/bgm_gameover.mp3", false)
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