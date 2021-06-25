-- Copyright 2014-2015 Greentwip. All Rights Reserved.


local fade          = class("fade", cc.Node)
local sprite        = import("app.core.graphical.sprite")

function fade:ctor(duration, on_fade_begin, on_fade_in, on_fade_out, settings, anchor, delay)

    if cc.platform_ == "mobile" then
        if anchor == nil then
            --        self.sprite_ = sprite:create("sprites/core/fade/fade", cc.p(1,1))
            --                             :addTo(self)
            self.sprite_ = cc.Sprite:create("sprites/core/fade/fade_wide.png")
                                    :setAnchorPoint(cc.p(1,1))
                                    :addTo(self)
    
        else
    --        self.sprite_ = sprite:create("sprites/core/fade/fade", anchor)
    --                             :addTo(self)
            self.sprite_ = cc.Sprite:create("sprites/core/fade/fade_wide.png")
                                    :setAnchorPoint(anchor)
                                    :addTo(self)
    
        end

    else
        if anchor == nil then
    --        self.sprite_ = sprite:create("sprites/core/fade/fade", cc.p(1,1))
    --                             :addTo(self)
            self.sprite_ = cc.Sprite:create("sprites/core/fade/fade.png")
                                    :setAnchorPoint(cc.p(1,1))
                                    :addTo(self)
    
        else
    --        self.sprite_ = sprite:create("sprites/core/fade/fade", anchor)
    --                             :addTo(self)
            self.sprite_ = cc.Sprite:create("sprites/core/fade/fade.png")
                                    :setAnchorPoint(anchor)
                                    :addTo(self)
    
        end
    end

    local actions = {}

    if delay ~= nil then
        actions[#actions +1] = cc.DelayTime:create(delay)
    end

    if on_fade_begin ~= nil then
        actions[#actions +1] = cc.CallFunc:create(on_fade_begin)
    end

    if settings.fade_in then
        if settings.fade_out then
            actions[#actions +1] = cc.FadeIn:create(duration * 0.5)
        else
            actions[#actions +1] = cc.FadeIn:create(duration)
        end
    end

    if on_fade_in ~= nil then
        actions[#actions +1] = cc.CallFunc:create(on_fade_in)
    end

    if settings.fade_out then
        if settings.fade_in then
            actions[#actions +1] = cc.FadeOut:create(duration * 0.5)
        else
            actions[#actions +1] = cc.FadeOut:create(duration)
        end
    end

    if on_fade_out ~= nil then
        actions[#actions +1] = cc.CallFunc:create(on_fade_out)
    end

    if settings.fade_out then
        self.sprite_:setOpacity(250)
    end

    if settings.fade_in then
        self.sprite_:setOpacity(0)
    end

    local sequence = cc.Sequence:create(actions)
    self.sprite_:runAction(sequence)
end

function fade:start()

end

return fade