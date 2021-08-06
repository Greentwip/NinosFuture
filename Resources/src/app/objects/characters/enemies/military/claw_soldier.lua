-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local enemy     = import("app.objects.characters.enemies.base.enemy")
local mob       = class("claw_soldier", enemy)

function mob:onCreate()
    self.super:onCreate()
    self.default_health_ = 12
    self.moving_    = false
    self.orientation_set_ = false

    self.weapon_ = import("app.objects.weapons.enemies.military.claw_bullet")
    self.weapon_parameters_ = {
        category_ = "weapons",
        sub_category_ = "enemies",
        package_ = "military",
        cname_ = "claw_bullet"
    }

end

function mob:animate(cname)

    local stand =   { name = "stand",  animation = {name = cname .. "_" .. "stand", forever = false, delay = 0.10}   }
    local attack =  { name = "attack", animation = {name = cname .. "_" .. "attack", forever = false, delay = 0.10}  }

    self.sprite_:load_action(stand, false)
    self.sprite_:load_action(attack, false)
    self.sprite_:set_animation(stand.animation.name)

    return self
end

function mob:onRespawn()
    self.attacking_ = false
    self.sprite_:run_action("stand")
end

function mob:attack()

    local player_position = cc.p(self.player_:getPositionX(), self.player_:getPositionY())
    local self_position = cc.p(self:getPositionX(), self:getPositionY())
    
    player_position = self:convertToWorldSpace(player_position)
    self_position = self:convertToWorldSpace(self_position)
    
    local delta_y = player_position.y - self_position.y
    local delta_x = player_position.x - self_position.x

    local angle = math.atan2(delta_y, delta_x) -- * 180 / math.pi
    
    local euler_angle = angle * 180 / math.pi
        
    if  (euler_angle <= 60 and euler_angle >= -60) or
        (euler_angle >= -180 and euler_angle <= -120) or
        (euler_angle <= 180 and euler_angle >= 120) then
        if not self.attacking_ then
            self.attacking_ = true
            
            --[[local facing = 0
            if  (euler_angle >= -180 and euler_angle <= -120) or
                (euler_angle <= 180 and euler_angle >= 120) then
                  facing = 1
            else
                  facing = -1
            end
            ]]
        
            
            local action_delay = cc.DelayTime:create(1)

            local attack  = cc.CallFunc:create(function()
                self.sprite_:run_action("attack")
            end)

            local callback = cc.CallFunc:create(function()
                local player_x_distance = cc.pGetDistance(cc.p(self:getPositionX(), 0), cc.p(self.player_:getPositionX(), 0))
                local player_y_distance = cc.pGetDistance(cc.p(0, self:getPositionY()), cc.p(0, self.player_:getPositionY()))
    
                self.player_position_ = cc.p(self.player_:getPositionX(), self.player_:getPositionY())

                local bullet = self:fire({  sfx = nil,
                    offset = cc.p(20, 16),
                    weapon = self.weapon_,
                    parameters = self.weapon_parameters_})

                    local x_normal = 0
                    if self.sprite_:isFlippedX() then
                        x_normal = 1
                    else
                        x_normal = -1
                    end

                    bullet:fire_straight(player_position, x_normal)

            end)


            local on_end = cc.CallFunc:create(function()
                self.attacking_ = false
                self.sprite_:run_action("stand")
            end)

            local sequence = cc.Sequence:create(action_delay, attack, callback, action_delay, on_end, nil)

            self:stopAllActions()
            self:runAction(sequence)
    end

    end


end

return mob





