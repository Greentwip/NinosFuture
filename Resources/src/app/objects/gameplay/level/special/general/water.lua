-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local special   = import("app.core.physics.static_character").create("water")

local splash = import ("app.objects.gameplay.level.animations.special.splash")


function special:onCreate(args)
    self.splash_in_flag_ = false
    self.splash_out_flag_ = false
end


function special:onAfterAnimate(args)
    self.start_position_ = args.anchored_position_
    self.start_position_.x = self.start_position_.x + 8
    self.start_position_.y = self.start_position_.y - 8
    self:setPosition(self.start_position_)
end

function special:create_splash()

    local creation_args = {}
    creation_args.real_position_     = cc.p(0, 0)

    local special_element = splash:create(creation_args)
                                  :setup("gameplay", "level", "special" .. "/" .. "general", "splash")
                                  :addTo(self)

    creation_args.anchored_position_ = creation_args.real_position_


    print("////////////////")
    print(self:getPositionX())
    print(self:getPositionY())
    print("////////////////")

    print("////////////////")
    print(self.sprite_:getPositionX())
    print(self.sprite_:getPositionY())
    print("////////////////")

    if special_element.onAfterAnimate then
        special_element:onAfterAnimate(creation_args)
    end


end


function special:solve_collisions()
    local collisions = self.kinematic_body_:get_collisions()

    local found_collision = false 

    for _, collision in pairs(collisions) do
        if collision:getPhysicsBody():getShapes()[1]:getTag() == cc.tags.player then
            found_collision = true
        end
    end

    if found_collision and cc.level_controller_.player_.current_browner_.speed_.y <= 0 then
        
        if not self.splash_in_flag_ then
            self:create_splash()

            cc.audio.play_sfx("sounds/sfx_splash.mp3", false)


            cc.physics_world_:setGravity(cc.p(0, -500))
            self.splash_in_flag_ = true
        end

    elseif found_collision and cc.level_controller_.player_.current_browner_.speed_.y > 0 then
        if not self.splash_out_flag_ then
            self:create_splash()

            cc.audio.play_sfx("sounds/sfx_splash.mp3", false)

            cc.physics_world_:setGravity(cc.p(0, -1000))
            self.splash_out_flag_ = true
        end
    else 
        self.splash_in_flag_ = false
        self.splash_out_flag_ = false
    end

end



function special:step(dt)
    
    self:static_step(dt)
    self:solve_collisions()

    return self
end

return special




