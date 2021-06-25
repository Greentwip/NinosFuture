-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local enemy   = import("app.objects.characters.enemies.base.enemy")
local mob     = class("lyric", enemy)

function mob:onCreate()
    self.super:onCreate()

    self.default_health_ = 2
    self.moving_    = false
    self.idle_time_counter_ = 0
end

function mob:animate(cname)

    local idle =  { name = "idle",    animation = { name = cname .. "_" .. "idle", forever = true, delay = 0.10} }
    local walk =  { name = "walk",    animation = { name = cname .. "_" .. "walk", forever = true, delay = 0.10} }

    self.sprite_:load_action(idle, false)
    self.sprite_:load_action(walk, false)
    self.sprite_:set_animation(idle.animation.name)

    return self
end

function mob:onRespawn()
    self.idle_time_counter_ = 0
    self.moving_ = false
    self.sprite_:run_action("idle")
end

function mob:update(dt)

    self.idle_time_counter_ = self.idle_time_counter_ + dt

    if self.idle_time_counter_ >= self.sprite_:get_action_duration("idle") then

        if self.sprite_:current_action() ~= self.sprite_:get_action("walk") then
            self.sprite_:run_action("walk")
        end

        if self.player_:getPositionX() > self:getPositionX() then
            self.current_speed_.x = 25
        elseif self.player_:getPositionX() < self:getPositionX() then
            self.current_speed_.x = -25
        end
    end
end

return mob


