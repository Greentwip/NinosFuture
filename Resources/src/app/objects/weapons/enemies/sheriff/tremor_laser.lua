-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local sprite    = import("app.core.graphical.sprite")

local weapon        = import("app.objects.weapons.base.weapon")
local tremor_laser  = class("tremor_laser", weapon)

function tremor_laser:animate(cname)
    self.sprite_ :setPosition(cc.p(0,0))

    local actions = {}
    actions[#actions + 1] = {name = "charge",  animation = { name = "tremor_laser_charge", forever = true,  delay = 0.05} }
    actions[#actions + 1] = {name = "shot",   animation = { name = "tremor_laser_shot",    forever = false, delay = 0.10} }

    self.sprite_:load_actions_set(actions, false)

    self.sprite_:set_animation("tremor_laser_shot")

    self.power_ = 1

    self.kinematic_body_size_ = cc.size(self.sprite_:getContentSize().width, self.sprite_:getContentSize().height)

    self.sprite_:setPositionX(0 + self.sprite_:getContentSize().width * 0.5)
    
    self.sprite_:set_animation("tremor_laser_charge")
    self.sprite_:run_action("charge")


    self.moving_ = false

    self.speed_ = {}

    self.speed_.x = 0
    self.speed_.y = 0

    return self
end

function tremor_laser:disable()
    self:getPhysicsBody():getShapes()[1]:setTag(cc.tags.weapon.none)
end

function tremor_laser:enable()
    local charge_delay = cc.DelayTime:create(1)

    local weaponize_callback = cc.CallFunc:create(function()
                                                        self.speed_ = cc.p(260 * self.x_normal_, 0)
                                                        self:getPhysicsBody():getShapes()[1]:setTag(cc.tags.weapon.enemy)
                                                        self.moving_ = true
                                                  end)

    local animation_callback = cc.CallFunc:create(function() self.sprite_:run_action("shot") end)

    local sequence = cc.Sequence:create(charge_delay, weaponize_callback, animation_callback, nil)

    self:runAction(sequence)

end

function tremor_laser:step(dt)
    if self.moving_ then
        self.current_speed_ = self.speed_
    else
        self.current_speed_ = cc.p(0, 0)
    end
    return self
end


return tremor_laser

