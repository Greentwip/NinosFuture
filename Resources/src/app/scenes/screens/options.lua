-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local options = import("app.core.gameplay.control.layout_base").create("title")

function options:onLoad()
    -- self variables
    if cc.platform_ == "mobile" then
        self:setPositionX(85)
    end
    
    local root = cc.CSLoader:createNode("sprites/gameplay/screens/options/data.csb")
    root:addTo(self)
    

    self.bgm_ = root:getChildByName("bgm");
    self.sfx_ = root:getChildByName("sfx");
    --self.controls_ = root:getChildByName("controls");

    self.current_option_ = self.bgm_

    self.inactive_color_ = cc.c3b(127, 127, 127)
    self.active_color_ = cc.c3b(255, 255, 255)

    self.bgm_slider_ = self.bgm_:getChildByName("slider")
    self.sfx_slider_ = self.sfx_:getChildByName("slider")

    self.bgm_volume_ = cc.bgm_volume_ * 100
    self.sfx_volume_ = cc.sfx_volume_ * 100

    self.bgm_slider_:setPercent(self.bgm_volume_)
    self.sfx_slider_:setPercent(self.sfx_volume_)

    self.triggered_ = false

end

function options:save(bgm_volume, sfx_volume)

    cc.bgm_volume_ = bgm_volume
    cc.sfx_volume_ = sfx_volume

    local volume_options = { bgm_volume = cc.bgm_volume_, sfx_volume = cc.sfx_volume_ }

    local encoded = cc.json.encode(volume_options)

    cc.file.write("options.json", encoded)

    cc.audio.set_bgm_volume(cc.bgm_volume_)
end

function options:step(dt)

    --[[
    if cc.key_pressed(cc.key_code_.up) then

        if self.current_option_ == self.bgm_ then
            self.bgm_:setColor(self.inactive_color_)
            self.controls_:setColor(self.active_color_)
            self.current_option_ = self.controls_
        elseif self.current_option_ == self.sfx_ then
            self.bgm_:setColor(self.active_color_)
            self.sfx_:setColor(self.inactive_color_)
            self.current_option_ = self.bgm_
        else
            self.sfx_:setColor(self.active_color_)
            self.controls_:setColor(self.inactive_color_)
            self.current_option_ = self.sfx_
        end

    end

    if cc.key_pressed(cc.key_code_.down) then
        if self.current_option_ == self.bgm_ then
            self.bgm_:setColor(self.inactive_color_)
            self.sfx_:setColor(self.active_color_)
            self.current_option_ = self.sfx_
        elseif self.current_option_ == self.sfx_ then
            self.sfx_:setColor(self.inactive_color_)
            self.controls_:setColor(self.active_color_)
            self.current_option_ = self.controls_
        else
            self.controls_:setColor(self.inactive_color_)
            self.bgm_:setColor(self.active_color_)
            self.current_option_ = self.bgm_
        end
    end
    ]]

    if not self.triggered_ then
        if cc.key_pressed(cc.key_code_.up) then
            if self.current_option_ == self.bgm_ then
                self.bgm_:setColor(self.inactive_color_)
                self.sfx_:setColor(self.active_color_)
                self.current_option_ = self.sfx_
            else 
                self.bgm_:setColor(self.active_color_)
                self.sfx_:setColor(self.inactive_color_)
                self.current_option_ = self.bgm_     
            end
            cc.audio.play_sfx("sounds/sfx_select.mp3", false)
        end
    
        if cc.key_pressed(cc.key_code_.down) then
            if self.current_option_ == self.bgm_ then
                self.bgm_:setColor(self.inactive_color_)
                self.sfx_:setColor(self.active_color_)
                self.current_option_ = self.sfx_
            else 
                self.bgm_:setColor(self.active_color_)
                self.sfx_:setColor(self.inactive_color_)
                self.current_option_ = self.bgm_     
            end
            cc.audio.play_sfx("sounds/sfx_select.mp3", false)
        end
    
        if cc.key_pressed(cc.key_code_.left) then
            if self.current_option_ == self.bgm_ then
                if self.bgm_volume_ > 0 then 
                    self.bgm_volume_ = self.bgm_volume_ - 10
                    self.bgm_slider_:setPercent(self.bgm_volume_)
                    self:save(self.bgm_volume_ / 100, self.sfx_volume_ / 100)
                end
            else
                if self.sfx_volume_ > 0 then 
                    self.sfx_volume_ = self.sfx_volume_ - 10
                    self.sfx_slider_:setPercent(self.sfx_volume_)
                    self:save(self.bgm_volume_ / 100, self.sfx_volume_ / 100)
                    cc.audio.play_sfx("sounds/sfx_select.mp3")
                end
            end
        end
    
        if cc.key_pressed(cc.key_code_.right) then
            if self.current_option_ == self.bgm_ then
                if self.bgm_volume_ < 100 then 
                    self.bgm_volume_ = self.bgm_volume_ + 10
                    self.bgm_slider_:setPercent(self.bgm_volume_)
                    self:save(self.bgm_volume_ / 100, self.sfx_volume_ / 100)
                end
            else
                if self.sfx_volume_ < 100 then 
                    self.sfx_volume_ = self.sfx_volume_ + 10
                    self.sfx_slider_:setPercent(self.sfx_volume_)
                    self:save(self.bgm_volume_ / 100, self.sfx_volume_ / 100)
                    cc.audio.play_sfx("sounds/sfx_select.mp3")
                end
            end
        end
    end

    if not self.triggered_ then

        if cc.key_pressed(cc.key_code_.b) then
            self.triggered_ = true

            self:getApp()
            :enterScene("screens.title", "FADE", 0.5)
        end
    end

    self:post_step(dt)

    return self
end

return options