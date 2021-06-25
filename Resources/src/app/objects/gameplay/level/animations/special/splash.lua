-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local animation = import ("app.objects.gameplay.level.animations.base.animation")

local splash = class("splash", animation)

function splash:onAfterAnimate(args)
    self.start_position_ = args.anchored_position_
    self.start_position_.x = self.start_position_.x  - 8
    self.start_position_.y = self.start_position_.y  + 16
    self:setPosition(self.start_position_)

    cc.level_controller_.scene_components_[#cc.level_controller_.scene_components_ + 1] = self

end

function splash:animate(cname)
    local spin =  { name = "play", animation = { name = cname .. "_" .. "play", forever = true, delay = 0.10} }

    self.sprite_:load_action(spin, false)

    self.sprite_:run_action("play")

    print("animate")
    return self
end

function splash:step(dt)

    if self.sprite_.cycles_ >= 1 then
        self.disposed_ = true
    end

end

return splash
