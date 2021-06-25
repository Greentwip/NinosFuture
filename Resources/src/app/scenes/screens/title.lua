-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local title = import("app.core.gameplay.control.layout_base").create("title")

local sprite    = import("app.core.graphical.sprite")
local label     = import("app.core.graphical.label")
local selector  = import("app.objects.gameplay.level.ui.selector")

function title:onLoad()
    if cc.platform_ == "mobile" then
        self:setPositionX(85)
    end
    
    self.background_ = sprite:create("sprites/gameplay/screens/title_screen/title_screen", cc.p(0, 0))
                             :setPosition(cc.p(0,0))
                             :addTo(self)

    self.city_ = sprite:create("sprites/gameplay/screens/title_screen/title_screen", cc.p(0, 0))
                             :setPosition(cc.p(0,0))
                             :addTo(self)

    self.silhouette_ = sprite:create("sprites/gameplay/screens/title_screen/title_screen", cc.p(0.5, 0.5))
                                  :setPosition(cc.p(256/2, 224/3.25))
                                  :addTo(self)

    self.logo_ = sprite:create("sprites/gameplay/screens/title_screen/title_screen", cc.p(0.5, 0.5))
                       :setPosition(cc.p(256/2, (224 * 0.25) * 3))
                       :addTo(self)

    self.abakura_ = sprite:create("sprites/gameplay/screens/title_screen/title_screen", cc.p(0, 0))
                       :setPosition(cc.p(0, 0))
                       :addTo(self)

    self.greentwip_ = sprite:create("sprites/gameplay/screens/title_screen/title_screen", cc.p(0, 0))
                       :setPosition(cc.p(256, 0))
                       :addTo(self)


    self.greentwip_:setPosition(cc.p(256 - self.greentwip_:getContentSize().width * 0.6, 0))



    self.background_:set_animation("background")

    self.city_:set_animation("city")

    self.silhouette_:set_animation("silhouette")

    self.logo_:set_animation("logo")
    self.abakura_:set_animation("abakura")
    self.greentwip_:set_animation("greentwip")


    --[[self.selector_ = selector:create("arrow", "right")
                             :setPosition(cc.p(64,96))
                             :addTo(self, 128)]]

    self.start_text_ = label:create("start game",
                                    "fonts/megaman_2.ttf",
                                    8,
                                    cc.TEXT_ALIGNMENT_LEFT,
                                    cc.VERTICAL_TEXT_ALIGNMENT_TOP)
                            :addTo(self, 128)

    self.options_text_ = label:create("options",
                                      "fonts/megaman_2.ttf",
                                      8,
                                      cc.TEXT_ALIGNMENT_LEFT,
                                      cc.VERTICAL_TEXT_ALIGNMENT_TOP)
                              :addTo(self, 128)

    local start_text_width = self.start_text_.label_:getContentSize().width
    local start_text_height = self.start_text_.label_:getContentSize().height


    local options_text_width = self.options_text_.label_:getContentSize().width
    local options_text_height = self.options_text_.label_:getContentSize().height


    self.start_text_:setPosition(cc.p((256 * 0.5) - (start_text_width * 0.5),
                                  (224 * 0.5) + (start_text_height * 2)))

    self.options_text_:setPosition(cc.p((256 * 0.5) - (options_text_width * 0.5),
                                   (224 * 0.5) + (options_text_height * 0.5) ))

    if cc.audio.current_track ~= "sounds/bgm_title.mp3" then
        cc.audio.play_bgm("sounds/bgm_title.mp3", true)
    end

    self.selected_option_ = self.start_text_

    self:select_start()

    -- self variables
    self.triggered_ = false
end

function title:select_start()
    --self.start_text_.label_:setTextColor(cc.c4b(0.5, 0.2, 0.92, 1.0))
    --self.options_text_.label_:setTextColor(cc.c4b(0.38, 0.02, 0.17, 1.0))

    self.start_text_.label_:setTextColor(cc.c4b(129, 51, 237, 255))
    self.options_text_.label_:setTextColor(cc.c4b(97, 6, 45, 255))

    self.selected_option_ = self.start_text_

end

function title:select_options()
    --self.start_text_.label_:setTextColor(cc.c4b(0.38, 0.02, 0.17, 1.0))
    --self.options_text_.label_:setTextColor(cc.c4b(0.5, 0.2, 0.92, 1.0))

    self.start_text_.label_:setTextColor(cc.c4b(97, 6, 45, 255))
    self.options_text_.label_:setTextColor(cc.c4b(129, 51, 237, 255))


    self.selected_option_ = self.options_text_

end

function title:step(dt)
    if not self.triggered_ then

        if cc.key_pressed(cc.key_code_.up) then
            if self.selected_option_ == self.start_text_ then
              self:select_options()
            else
              self:select_start()

            end

            cc.audio.play_sfx("sounds/sfx_select.mp3")
        end

        if cc.key_pressed(cc.key_code_.down) then
            if self.selected_option_ == self.start_text_ then
              self:select_options()
            else
              self:select_start()
            end

            cc.audio.play_sfx("sounds/sfx_select.mp3")
        end

        if cc.key_pressed(cc.key_code_.a) then
            self.triggered_ = true

            --ccexp.AudioEngine:stopAll()
            cc.audio.play_sfx("sounds/sfx_selected.mp3")

            if self.selected_option_ == self.start_text_ then
                local callback = cc.CallFunc:create(function()

                    self:getApp()
                        :enterScene("screens.save", "FADE", 0.5)
        
                end)
        
                local delay = cc.DelayTime:create(0.5)
        
                local sequence = cc.Sequence:create(delay, callback, nil)
        
                self:runAction(sequence)    
                
            else
                local callback = cc.CallFunc:create(function()
        
                    self:getApp()
                        :enterScene("screens.options", "FADE", 0.5)
        
                end)
        
                local delay = cc.DelayTime:create(0.5)
        
                local sequence = cc.Sequence:create(delay, callback, nil)
        
                self:runAction(sequence)    
    
    
    
            end

        end
    end

    self:post_step(dt)

    return self
end



return title