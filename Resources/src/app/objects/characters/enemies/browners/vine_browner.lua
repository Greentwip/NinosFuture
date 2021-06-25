-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local browner       = import("app.objects.characters.enemies.browners.base.browner")
local vine_bullet   = import("app.objects.weapons.browners.vine.vine_bullet")

local vine_browner = class("vine_browner-enemy", browner)

function vine_browner:ctor(sprite, args)
    self.super:ctor(sprite, args)

    self.base_name_ = "vine"

    local actions = {}
    actions[#actions + 1] = {name = "intro",      animation = {name = "vine_intro",      forever = false, delay = 0.10} }
    actions[#actions + 1] = {name = "stand",      animation = {name = "vine_stand",       forever = false, delay = 0.10} }
    actions[#actions + 1] = {name = "jump",       animation = {name = "vine_jump",        forever = false, delay = 0.10} }
    actions[#actions + 1] = {name = "walk",       animation = {name = "vine_walk",        forever = true,  delay = 0.12} }
    actions[#actions + 1] = {name = "standshoot", animation = {name = "vine_standshoot",  forever = false, delay = 0.10} }
    actions[#actions + 1] = {name = "climb",      animation = {name = "vine_climb",       forever = true,  delay = 0.16} }
    actions[#actions + 1] = {name = "hurt",       animation = {name = "vine_hurt",        forever = false, delay = 0.02} }

    self.sprite_:load_actions_set(actions, true, self.base_name_)

    self.browner_id_ = cc.browners_.vine_.id_       -- overriden from parent

    self.dash_direction_ = 1
    
    self.state_nothing_ = 0
    self.state_initial_shoot_prepare_ = 1
    self.state_initial_shooting_ = 2

    self.state_dash_prepare_ = 3
    self.state_dash_ = 4
    self.state_dash_end_ = 5

    self.state_dash_prepare_no_jump_ = 6

    self.state_dash_end_idle_ = 7
    
    self:reset_flags()
end

function vine_browner:reset_flags()
    self.ticks_ = 0
    
    self.jump_flag_ = false
    self.rounds_ = 0

    self.state_ = self.state_nothing_
end

function vine_browner:init_constraints()
    self.super:init_constraints()
    self.can_slide_         = false
    self.can_charge_        = false
    self.can_dash_jump_     = false
    self.can_walk_shoot_    = false
    self.can_jump_shoot_    = false
end

function vine_browner:init_variables()
    self.super:init_variables(self)
end


function vine_browner:update(dt)
    self.ticks_ = self.ticks_ + dt

    if self.state_ == self.state_nothing_ then
        self.state_ = self.state_initial_shoot_prepare_
        self.jump_flag_ = false

        if self.rounds_ == 1 then
            print("rounds 1")
            self.state_ = self.state_dash_prepare_no_jump_
        end

    elseif self.state_ == self.state_initial_shoot_prepare_ then
        self.state_ = self.state_initial_shooting_

        local speed = 1

        local init_callback = cc.CallFunc:create(function()
            self.attacking_ = true
        end)

        local pre_delay = cc.DelayTime:create(self:get_action_duration("standshoot") * 0.20)

        local pre_callback = cc.CallFunc:create(function()
            self.attacking_ = true
            self:fire(speed)
            speed = speed + 1.5
        end)

        local post_delay = cc.DelayTime:create(0.25)

        local post_callback = cc.CallFunc:create(function()
            self.attacking_ = false
        end)

        local final_delay = cc.DelayTime:create(0.5)

        local final_callback = cc.CallFunc:create(function()
            self.state_ = self.state_dash_prepare_
        end)

        local time_between = cc.DelayTime:create(0.5)

        local sequence1 = cc.Sequence:create(init_callback, pre_delay, pre_callback, post_delay, post_callback, nil)
        local sequence2 = cc.Sequence:create(init_callback, pre_delay, pre_callback, post_delay, post_callback, nil)
        local sequence3 = cc.Sequence:create(init_callback, pre_delay, pre_callback, post_delay, post_callback, nil)

        local sequence = cc.Sequence:create(sequence1,
                                            time_between,
                                            sequence2,
                                            time_between, 
                                            sequence3, 
                                            time_between, 
                                            final_delay, 
                                            final_callback, 
                                            nil)

        self:runAction(sequence)
    
    elseif self.state_ == self.state_initial_shooting_ then
        return self
    elseif self.state_ == self.state_dash_prepare_ then
        self.state_ = self.state_dash_

        local dash_delay = cc.DelayTime:create(0.5)

        local jump_callback = cc.CallFunc:create(function()
            self.jump_flag_ = true
        end)

        local post_callback = cc.CallFunc:create(function()
            self.state_ = self.state_dash_end_
        end)
        

        local dash_delay_2 = cc.DelayTime:create(2)

        local dash_callback = cc.CallFunc:create(function()
            self.dash_direction_ = -self.dash_direction_
        end)


        local sequence = cc.Sequence:create(dash_delay, 
                                            jump_callback,
                                            dash_delay,
                                            dash_delay,
                                            dash_delay,
                                            dash_callback,
                                            dash_delay, 
                                            jump_callback,
                                            dash_delay,
                                            dash_delay,
                                            dash_delay, 
                                            dash_callback,
                                            post_callback, 
                                            nil)

        self:runAction(sequence)        
    elseif self.state_ == self.state_dash_prepare_no_jump_ then
        self.state_ = self.state_dash_

        local dash_delay = cc.DelayTime:create(0.5)

        local jump_callback = cc.CallFunc:create(function()
            --self.jump_flag_ = true
        end)

        local post_callback = cc.CallFunc:create(function()
            self.state_ = self.state_dash_end_
        end)

        local dash_delay_2 = cc.DelayTime:create(2)

        local dash_callback = cc.CallFunc:create(function()
            self.dash_direction_ = -self.dash_direction_

            if self.dash_direction_ == -1 then
                self.sprite_:setFlippedX(false)
            else
                self.sprite_:setFlippedX(true)
            end
    
        end)


        local sequence = cc.Sequence:create(dash_delay, 
                                            jump_callback,
                                            dash_delay,
                                            dash_delay,
                                            dash_delay,
                                            dash_callback,
                                            post_callback,
                                            nil)

        self:runAction(sequence)


    elseif self.state_ == self.state_dash_ then

        if self.dash_direction_ == -1 then
            self.sprite_:setFlippedX(false)
        else
            self.sprite_:setFlippedX(true)
        end
        
        self.speed_.x = self.walk_speed_ * 2 * -self.dash_direction_

        if self.jump_flag_ then
            self.jump_flag_ = false

            self.speed_.y  = self.jump_speed_
            self.on_ground_ = false
            self.jumping_ = true            
        end

        if self.on_ground_ then
            self.walking_ = true
        else
            self.jumping_ = true            
        end

        
    elseif self.state_ == self.state_dash_end_ then
        self.speed_.x = 0
        self.walking_ = false
        self.state_ = self.state_dash_end_idle_

        if self.rounds_ == 0 then
            print("Rounds 0")
            self.rounds_ = self.rounds_ + 1
        else
            print("Rounds 1")
            self.rounds_ = 0
        end

        if self.dash_direction_ == -1 then
            self.sprite_:setFlippedX(false)
        else
            self.sprite_:setFlippedX(true)
        end

        local dash_delay = cc.DelayTime:create(1)

        local dash_callback = cc.CallFunc:create(function()
            self.state_ = self.state_nothing_
        end)


        local sequence = cc.Sequence:create(dash_delay, 
                                            dash_callback, 
                                            nil)

        self:runAction(sequence)    
    elseif self.state_ == self.state_dash_end_idle_ then
        return self
    end
end

function vine_browner:walk()    --@TODO implement walk_condition()
    return self
end

function vine_browner:jump()
    return self
end

function vine_browner:fire(speed)

    local bullet_offset = 0

    cc.audio.play_sfx("sounds/sfx_buster_shoot_mid.mp3", false)

    local bullet_position = cc.p(self:getParent():getPositionX() + (bullet_offset * self:get_sprite_normal().x),
                                 self:getParent():getPositionY() + 26)


    local bullet = vine_bullet:create()
                              :setPosition(bullet_position)
                              :setup("gameplay", "level", "weapon", "vine_bullet")
                              :init_weapon(self:get_sprite_normal().x, self.weapon_tag_)
                              :addTo(self:getParent():getParent())

    bullet.move_speed_ = bullet.move_speed_ * speed

    self:getParent():getParent().bullets_[bullet] = bullet
end

return vine_browner





