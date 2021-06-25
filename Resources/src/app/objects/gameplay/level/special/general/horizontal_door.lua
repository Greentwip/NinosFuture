-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local special   = import("app.core.physics.static_character").create("horizontal_door")

function special:onCreate(args)
    self.lock_time_ = 1.0
    self.is_boss_door_ = false
end

function special:onAfterAnimate(args)
    self.start_position_ = args.anchored_position_
    self:setPosition(self.start_position_)

    self.triggered_ = false

    self.kinematic_body_.current_shape_:setTag(cc.tags.block)
end

function special:animate(cname)

    local lock   =  { name = "lock", animation = { name = cname .. "_" .. "lock", forever = false, delay = self.lock_time_ / 8} }
    local idle   =  { name = "idle", animation = { name = cname .. "_" .. "idle", forever = false, delay = 8 } }

    self.sprite_:load_action(lock, false)
    self.sprite_:load_action(idle, false)
    self.sprite_:set_animation(cname .. "_" .. "idle")
    self.sprite_:set_image_index(1)

end

function special:reclose()
    self.kinematic_body_.current_shape_:setTag(cc.tags.block)
    self.triggered_ = false

    self.sprite_:run_action("idle")
end


function special:unlock(callback)

    local delay = cc.DelayTime:create(self.lock_time_ * 0.5)

    local unlock = cc.CallFunc:create(function()
        cc.audio.play_sfx("sounds/sfx_door.mp3", false)
        self.sprite_:run_action("lock")
        self.sprite_:reverse_action()
    end)

    local callback = cc.CallFunc:create(function() self.kinematic_body_.current_shape_:setTag(cc.tags.none) end)

    local sequence = cc.Sequence:create(unlock, delay, callback, nil)

    self:stopAllActions()
    self:runAction(sequence)

    self.triggered_ = true
end

return special




