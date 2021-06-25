-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local weapon                = import("app.objects.weapons.base.weapon")
local directional_bullet    = class("directional_bullet", weapon)

function directional_bullet:onCreate()
    self.super:onCreate()
    self.speed_ = cc.p(0, 0)
    self.power_ = 2
    self.straight_ = false
    self.normal_ = 0
end

function directional_bullet:setup_movement(point)

    local delta_y = point.y - self:getPositionY()
    local delta_x = point.x - self:getPositionX()

    self.angle_ = math.atan2(delta_y, delta_x) -- * 180 / math.pi
    self.velocity_ = 10000
    return self
end


function directional_bullet:fire_straight(normal)
    self.straight_ = true
    self.normal_ = normal
    self.velocity_ = 10000
    return self
end



function directional_bullet:step(dt)
    
    if not self.straight_ then
        
        self.current_speed_ = self.speed_

        self.current_speed_.x = self.velocity_ * math.cos(self.angle_) * dt
        self.current_speed_.y = self.velocity_ * math.sin(self.angle_) * dt

    else
        self.current_speed_ = self.speed_

        self.current_speed_.x = self.velocity_ * self.normal_ * dt
        self.current_speed_.y = 0
    end

    return self
end

return directional_bullet
