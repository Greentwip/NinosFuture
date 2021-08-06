-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local opening = import("app.core.gameplay.control.layout_base").create("opening")

local sprite    = import("app.core.graphical.sprite")
local fade      = import("app.core.graphical.fade")
local label     = import("app.core.graphical.label")

function opening:onLoad()

    local initiate = cc.CallFunc:create(function()
                        self:opening_intro_a()
                     end)

    local sequence = cc.Sequence:create(initiate, nil)
    self:runAction(sequence)

    -- self variables
    self.triggered_ = false

    if cc.platform_ == "mobile" then
        self:setPositionX(85)
    end
end

function opening:opening_intro_a()

    local root = cc.CSLoader:createNode("sprites/gameplay/screens/license/data.csb")
    root:addTo(self)

    root:getChildByName("text"):getLabel():getFontAtlas():setAliasTexParameters()


    local duration = cc.DelayTime:create(3)
    local post_callback = cc.CallFunc:create(function()

        self:getApp():enterScene("screens.title", "FADE", 1)

        
    end)

    local sequence = cc.Sequence:create(duration, post_callback, nil)

    self:runAction(sequence)
end

function opening:step(dt)
--    if not self.triggered_ then
--        if cc.key_pressed(cc.key_code_.start) then
--            self.triggered_ = true
--            cc.audio.play_sfx("sounds/sfx_selected.mp3")
--            self:getApp():enterScene("gameplay.stage_select", "FADE", 1)
--        end
--    end
--   self:getApp():enterScene("screens.testwrapper", "FADE", 1)

    self:post_step(dt)

    return self
end



return opening