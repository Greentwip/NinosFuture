-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local about = import("app.core.gameplay.control.layout_base").create("about")

function about:onLoad() -- weird bug when using onLoad

    if cc.platform_ == "mobile" then
        self:setPositionX(85)
    end
    
    local root = cc.CSLoader:createNode("sprites/gameplay/screens/about/data.csb")
    root:addTo(self)

    self.root_ = root

    self.next_button_ = root:getChildByName("next_button")
    self.prev_button_ = root:getChildByName("prev_button")

    self.current_option_ = self.next_button_

    self.inactive_color_ = cc.c3b(127, 127, 127)
    self.active_color_ = cc.c3b(255, 255, 255)

    self.triggered_ = false
    
    self.next_button_:setColor(self.active_color_)
    self.prev_button_:setColor(self.inactive_color_)

    self.prev_button_:getChildByName("label"):setText("exit")

    self.menu_index_ = 0

    self.root_:getChildByName("about" .. "_" .. tostring(self.menu_index_)):setVisible(true)


    for i = 0, 8 do
        self.root_:getChildByName("about" .. "_" .. tostring(i)):getLabel():getFontAtlas():setAliasTexParameters()
    end

    --cc.audio.play_bgm("sounds/bgm_gameover.mp3", false)
end

function about:disable_antialiasing()

end

function about:step(dt)
    if cc.key_pressed(cc.key_code_.left) then
        if self.current_option_ == self.next_button_ then
            self.next_button_:setColor(self.inactive_color_)
            self.prev_button_:setColor(self.active_color_)
            self.current_option_ = self.prev_button_
        else 
            self.prev_button_:setColor(self.inactive_color_)
            self.next_button_:setColor(self.active_color_)
            self.current_option_ = self.next_button_
        end
        cc.audio.play_sfx("sounds/sfx_select.mp3", false)
    end

    if cc.key_pressed(cc.key_code_.right) then
        if self.current_option_ == self.next_button_ then
            self.next_button_:setColor(self.inactive_color_)
            self.prev_button_:setColor(self.active_color_)
            self.current_option_ = self.prev_button_
        else 
            self.prev_button_:setColor(self.inactive_color_)
            self.next_button_:setColor(self.active_color_)
            self.current_option_ = self.next_button_
        end
        cc.audio.play_sfx("sounds/sfx_select.mp3", false)
    end

    if self.menu_index_ == 0 then
        self.prev_button_:getChildByName("label"):setString("exit")
        self.next_button_:getChildByName("label"):setString("next")
    elseif self.menu_index_ == 8 then
        self.prev_button_:getChildByName("label"):setString("prev")
        self.next_button_:getChildByName("label"):setString("exit")
    end

    if self.menu_index_ > 0 and self.menu_index_ < 8 then
        self.prev_button_:getChildByName("label"):setString("prev")
        self.next_button_:getChildByName("label"):setString("next")
    end

    if not self.triggered_ and (self.menu_index_ == 8 and self.current_option_ == self.next_button_ or self.menu_index_ == 0 and self.current_option_ == self.prev_button_) then
        if cc.key_pressed(cc.key_code_.a) then
            self.triggered_ = true
            cc.audio.play_sfx("sounds/sfx_selected.mp3")

            self:getApp()
            :enterScene("screens.title", "FADE", 0.5)

        end
    end


    if self.current_option_ == self.prev_button_ and self.menu_index_ > 0 then
        if cc.key_pressed(cc.key_code_.a) then
            self.root_:getChildByName("about" .. "_" .. tostring(self.menu_index_)):setVisible(false)
            self.menu_index_ = self.menu_index_ - 1
            self.root_:getChildByName("about" .. "_" .. tostring(self.menu_index_)):setVisible(true)
        end
    end

    if self.current_option_ == self.next_button_ and self.menu_index_ < 8 then
        if cc.key_pressed(cc.key_code_.a) then
            self.root_:getChildByName("about" .. "_" .. tostring(self.menu_index_)):setVisible(false)
            self.menu_index_ = self.menu_index_ + 1
            self.root_:getChildByName("about" .. "_" .. tostring(self.menu_index_)):setVisible(true)
        end
    end

    --self.root_:getChildByName("about" .. "_" .. tostring(0)):setString(self.text_data_[self.menu_index_ + 1])



    self:post_step(dt)

    return self
end

function about:prepare(args)

end

return about