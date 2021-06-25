-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local level_complete = import("app.core.gameplay.control.layout_base").create("level_complete")

function level_complete:onLoad() -- weird bug when using onLoad

    if cc.platform_ == "mobile" then
        self:setPositionX(85)
    end
    
    local root = cc.CSLoader:createNode("sprites/gameplay/screens/gameover/data.csb")
    root:addTo(self)

    self.yes_button_ = root:getChildByName("yes_button")
    self.no_button_ = root:getChildByName("no_button")

    root:getChildByName("game_over_background"):setVisible(false)
    root:getChildByName("level_clear_background"):setVisible(false)

    if cc.is_level_clear_ then
        root:getChildByName("level_clear_background"):setVisible(true)
    else
        root:getChildByName("game_over_background"):setVisible(true)
    end

    self.current_option_ = self.yes_button_

    self.inactive_color_ = cc.c3b(127, 127, 127)
    self.active_color_ = cc.c3b(255, 255, 255)

    self.triggered_ = false
    self.ready_ = false

    local root_timeline = cc.CSLoader:createTimeline("sprites/gameplay/screens/gameover/data.csb")

    root:runAction(root_timeline)

    root_timeline:gotoFrameAndPause(0)


    local fly_primary_delay = cc.DelayTime:create((1.0 / 60.0) * 30)
    local fly_secondary_delay = cc.DelayTime:create((1.0 / 60.0) * 45)

    local pre_fly_callback = cc.CallFunc:create(function()
        root_timeline:play("fly_in_primary", false)    
    end)

    local fly_in_primary_callback = cc.CallFunc:create(function()
        root_timeline:play("fly_out_primary", false)
    end)

    local fly_out_primary_callback = cc.CallFunc:create(function()
        root_timeline:play("fly_in_secondary", false)
    end)


    local fly_in_secondary_callback = cc.CallFunc:create(function()
        self.yes_button_:setVisible(true)
        self.no_button_:setVisible(true)
        self.ready_ = true

        self.yes_button_:setColor(self.active_color_)
        self.no_button_:setColor(self.inactive_color_)
    end)

    local fly_sequence  = cc.Sequence:create(fly_primary_delay, 
                                             fly_primary_delay,
                                             fly_primary_delay,
                                             fly_primary_delay,
                                             pre_fly_callback,
                                             fly_primary_delay,
                                             fly_in_primary_callback, 
                                             fly_primary_delay,
                                             fly_out_primary_callback,
                                             fly_secondary_delay,
                                             fly_in_secondary_callback,
                                             nil)

    self:runAction(fly_sequence)
                                      
    cc.audio.play_bgm("sounds/bgm_gameover.mp3", false)
end

function level_complete:step(dt)
    if not self.triggered_ and self.ready_ then    
        if cc.key_pressed(cc.key_code_.left) then
            if self.current_option_ == self.yes_button_ then
                self.yes_button_:setColor(self.inactive_color_)
                self.no_button_:setColor(self.active_color_)
                self.current_option_ = self.no_button_
            else 
                self.no_button_:setColor(self.inactive_color_)
                self.yes_button_:setColor(self.active_color_)
                self.current_option_ = self.yes_button_
            end
            cc.audio.play_sfx("sounds/sfx_select.mp3", false)
        end
    
        if cc.key_pressed(cc.key_code_.right) then
            if self.current_option_ == self.yes_button_ then
                self.yes_button_:setColor(self.inactive_color_)
                self.no_button_:setColor(self.active_color_)
                self.current_option_ = self.no_button_
            else 
                self.no_button_:setColor(self.inactive_color_)
                self.yes_button_:setColor(self.active_color_)
                self.current_option_ = self.yes_button_
            end
            cc.audio.play_sfx("sounds/sfx_select.mp3", false)
        end
    
    end


    if not self.triggered_ and self.ready_ then
        if cc.key_pressed(cc.key_code_.a) then
            self.triggered_ = true
            cc.audio.play_sfx("sounds/sfx_selected.mp3")

            if self.current_option_ == self.yes_button_ then
                if cc.is_level_clear_ then
                    self:getApp():enterScene("screens.stage_select", "FADE", 0.5)
                else
                    cc.player_.lives_ = 3
                    self:getApp():enterScene("levels.level", "FADE", 1, {physics = true})
                end

            else
                if cc.is_level_clear_ then
                    self:getApp():enterScene("screens.title", "FADE", 0.5)
                else
                    self:getApp():enterScene("screens.stage_select", "FADE", 0.5)
                end
            end

        end
       
    end

    self:post_step(dt)

    return self
end

function level_complete:prepare(args)

end

return level_complete