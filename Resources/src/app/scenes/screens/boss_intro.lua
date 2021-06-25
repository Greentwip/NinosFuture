-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local boss_intro = import("app.core.gameplay.control.layout_base").create("boss_intro")

local label  = import("app.core.graphical.label")
local sprite = import("app.core.graphical.sprite")

local intro = import("app.scenes.special.intro")
local fade  = import("app.core.graphical.fade")

function boss_intro:onLoad()
    self.boss_ = sprite:create("sprites/gameplay/screens/boss_intro/boss_animation/boss_animation", cc.p(0.5, 0.0))
                       :setPosition(display.center)
                       :setVisible(false)
                       :addTo(self, 256)

    self.boss_:setPositionY(self.boss_:getPositionY() - 16)

    local boss_animation = {name = "animate",  animation = { name = "boss_animation_" .. cc.current_level_.mug_,  forever = false, delay = 0.10} }

    self.boss_:load_action(boss_animation, false)


    self.text_ = label:create(string.gsub(cc.current_level_.mug_, "man", " man"),
                              "fonts/megaman_2.ttf",
                              8,
                              cc.TEXT_ALIGNMENT_CENTER,
                              cc.VERTICAL_TEXT_ALIGNMENT_TOP,
                              cc.p(0.5, 0.5),
                              {delay_ = 0.2, callback_ = nil})
                       :setPosition(display.center)
                       :addTo(self, 256)

    self.text_:setPositionY((self.text_:getPositionY() - self.text_:getContentSize().height * 0.5) -
                             self.boss_:getContentSize().height * 0.5)

    local parallax_arguments = { category_ = "gameplay",
                                 sub_category_ = "screens",
                                 package_ = "boss_intro",
                                 cname_ = cc.current_level_.mug_,
                                 bgm_ = nil,
                                 on_end_callback_ = self.on_intro_complete,
                                 sender_ = self}

    self.intro_ = intro:create(parallax_arguments)
                       :setPosition(display.left_bottom)
                       :addTo(self)

    local callback = cc.CallFunc:create(function()
        if ccexp.AudioEngine:getState(self.bgm_id_) == -1 and not self.triggered_  then
            ccexp.AudioEngine:stopAll()
        end
    end)

    local delay = cc.DelayTime:create(10)

    local sequence = cc.Sequence:create(delay, callback, nil)

    self:runAction(sequence)    

    self.bgm_id_ = cc.audio.play_bgm("sounds/bgm_boss_intro.mp3", false)

end

function boss_intro:onAfterLoad()
    if cc.platform_ == "mobile" then
        self.joypad_:setVisible(false)
        self.joypad_:setPositionX(85)
    end

    if cc.platform_ == "mobile" then
        self.blackout_a_:setPosition(cc.p(display.left_bottom.x, display.left_bottom.y))              
        self.blackout_b_:setPosition(cc.p(display.right_bottom.x, display.right_bottom.y))              
    end

end

function boss_intro:on_intro_complete()

    local boss_animate = cc.CallFunc:create(function()
        self.boss_:setVisible(true)
        self.boss_:run_action("animate")
    end)

    local delay = cc.DelayTime:create(self.boss_:get_action_duration("animate"))

    local text_animation = cc.CallFunc:create(self.on_boss_intro_complete)

    local sequence = cc.Sequence:create(boss_animate, delay, text_animation, nil)

    self:runAction(sequence)
end

function boss_intro:on_boss_intro_complete()
    self.text_:start_animation()
end

function boss_intro:step(dt)

    self.intro_:step(dt)

    if ccexp.AudioEngine:getState(self.bgm_id_) == -1 and not self.triggered_  then
        self.triggered_ = true

            ccexp.AudioEngine:stopAll()

            if cc.platform_ == "mobile" then
                fade:create(1.0, function() end, function()
                    self:getApp()
                        :enterScene("levels.level", "FADE", 1, {physics = true})
                
                end, function() end, {fade_in = true, fade_out = false}, cc.p(1, 0.5))
                                    :setPosition(0, 0)
                                    :addTo(self, 1024)
            else

                fade:create(1.0, function() end, function()
                    self:getApp()
                        :enterScene("levels.level", "FADE", 1, {physics = true})
                            
                            end, function() end, {fade_in = true, fade_out = false}, cc.p(0, 1.0))
                                                :setPosition(cc.p(display.left_top.x - 16, 
                                                                display.left_top.y + 16))
                                                :addTo(self, 1024)
                            
            end
                            
    

    end

    self:post_step(dt)
    return self
end

return boss_intro