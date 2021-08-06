-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local weapon         = import("app.objects.weapons.base.weapon")
local claw_bullet    = class("claw_bullet", weapon)

function claw_bullet:onCreate()
    self.super:onCreate()
    self.speed_ = cc.p(0, 0)
    self.power_ = 4
    self.rotation_counter_ = 0
    self.straight_ = true
    self.normal_ = -1

end

function claw_bullet:setup_movement(point)

    local delta_y = point.y - self:getPositionY()
    local delta_x = point.x - self:getPositionX()

    self.angle_ = math.atan2(delta_y, delta_x) -- * 180 / math.pi
    self.velocity_ = 10000
    
    
    self.sprite_:setRotation(-(self.angle_ * 180 / math.pi) * 2)

    return self
end

function claw_bullet:fire_straight(point, normal)
    self.straight_ = true
    self.normal_ = normal
    self.velocity_ = 10000

    local delta_y = point.y - self:getPositionY()
    local delta_x = point.x - self:getPositionX()

    self.angle_ = math.atan2(delta_y, delta_x) -- * 180 / math.pi
    self.sprite_:setRotation(-(self.angle_ * 180 / math.pi) * 2)

    return self
end


function claw_bullet:step(dt)
    self.current_speed_ = self.speed_

    self.current_speed_.x = self.velocity_ * self.normal_ * dt
    self.current_speed_.y = 0

    self.sprite_:setRotation((-(self.angle_ * 180 / math.pi) * 2) + self.rotation_counter_)
    
    self.rotation_counter_ = self.rotation_counter_ + 18
    
    return self
end

return claw_bullet


