-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local sprite    = import("app.core.graphical.sprite")

local weapon        = import("app.objects.weapons.base.weapon")
local tremor_tail  = class("tremor_laser", weapon)

function tremor_tail:animate(cname)
    self.sprite_:setAnchorPoint(cc.p(0.5, 0.5))
                :setPosition(cc.p(0, 0))

    local actions = {}
    actions[#actions + 1]  =  { name = "tail_a",  animation = { name = "tremor_tail_a",  forever = false, delay = 0.04 } }
    actions[#actions + 1]  =  { name = "tail_b",  animation = { name = "tremor_tail_b",  forever = true, delay = 0.04 } }
    actions[#actions + 1]  =  { name = "tail_c",  animation = { name = "tremor_tail_c",  forever = false, delay = 0.04 } }
    actions[#actions + 1]  =  { name = "tail_idle",  animation = { name = "tremor_tail_idle",  forever = false, delay = 0.04 } }

    self.sprite_:load_actions_set(actions, false)

    self.sprite_:run_action("tail_idle")

    self.sprite_:set_animation("tremor_tail_idle")

    self.power_ = 8

    return self
end

function tremor_tail:enable_attack()
    self:getPhysicsBody():getShapes()[1]:setTag(cc.tags.weapon.enemy)
    return self
end

function tremor_tail:disable_attack()
    self:getPhysicsBody():getShapes()[1]:setTag(cc.tags.weapon.none)
    return self
end


return tremor_tail

