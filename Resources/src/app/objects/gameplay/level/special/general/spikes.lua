-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local special   = import("app.core.physics.static_character").create("spikes")

function special:onCreate(args)
    self.spike_flag_ = false
end

function special:onAfterAnimate(args)
    self.start_position_ = args.anchored_position_
    self.start_position_.x = self.start_position_.x 
    self.start_position_.y = self.start_position_.y 
    self:setPosition(self.start_position_)
end

function special:solve_collisions()
    local collisions = self.kinematic_body_:get_collisions()

    local found_collision = false 

    for _, collision in pairs(collisions) do
        if collision:getPhysicsBody():getShapes()[1]:getTag() == cc.tags.player then
            found_collision = true
        end
    end

    if found_collision then
        if not self.spike_flag_ then
            self.spike_flag_ = true
            cc.level_controller_.player_:kill()
        end
    end

end

function special:step(dt)
    
    self:static_step(dt)
    self:solve_collisions()

    return self
end

return special




