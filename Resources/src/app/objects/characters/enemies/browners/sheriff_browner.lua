-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local browner                   = import("app.objects.characters.enemies.browners.base.browner")
local directional_star_bullet     = import("app.objects.weapons.browners.sheriff.directional_star_bullet")

local sheriff_browner = class("sheriff_browner-enemy", browner)

function sheriff_browner:ctor(sprite, args)
    self.super:ctor(sprite, args)

    -- constraints
    self.can_slide_         = false
    self.can_charge_        = false
    self.can_dash_jump_     = false
    self.can_walk_shoot_    = false
    self.can_jump_shoot_    = false

    self.base_name_ = "sheriff"

    local actions = {}
    actions[#actions + 1] = {name = "intro",       animation = {name = "sheriff_intro",      forever = false, delay = 0.10} }
    actions[#actions + 1] = {name = "stand",      animation = {name = "sheriff_stand",       forever = false, delay = 0.10} }
    actions[#actions + 1] = {name = "jump",       animation = {name = "sheriff_jump",        forever = false, delay = 0.10} }
    actions[#actions + 1] = {name = "walk",       animation = {name = "sheriff_walk",        forever = true,  delay = 0.12} }
    actions[#actions + 1] = {name = "standshoot", animation = {name = "sheriff_standshoot",  forever = false, delay = 0.10} }
    actions[#actions + 1] = {name = "climb",      animation = {name = "sheriff_climb",       forever = true,  delay = 0.16} }
    actions[#actions + 1] = {name = "hurt",       animation = {name = "sheriff_hurt",        forever = false, delay = 0.02} }

    self.sprite_:load_actions_set(actions, true, self.base_name_)

    self.browner_id_ = cc.browners_.sheriff_.id_       -- overriden from parent


    self.state_nothing_ = 0
    self.state_initial_shoot_prepare_ = 1
    self.state_initial_shooting_ = 2

    self.state_initial_jumping_prepare_ = 3

    self.state_dash_prepare_ = 4
    self.state_dash_ = 5
    self.state_dash_end_ = 6

    self.state_diagonal_attack_prepare_ = 7
    self.state_diagonal_slash_prepare_ = 8
    self.state_diagonal_slash_ = 9
    self.state_diagonal_slash_reset_ = 10

    self:reset_flags()
    
end

function sheriff_browner:reset_flags()
    self.ticks_ = 0

    self.jump_flag_ = false

    self.jump_counter_ = 0

    self.dash_flag_ = false

    self.dash_direction_ = 1

    self.rounds_ = 0

    self.state_ = self.state_nothing_
end

function sheriff_browner:update(dt)
    self.ticks_ = self.ticks_ + dt

    if self.state_ == self.state_nothing_ then
        self.state_ = self.state_initial_shoot_prepare_
        self.jump_flag_ = false
        self.dash_flag_ = false
        self.jump_counter_ = 0
        self.ticks_ = 0
        self.dash_direction_ = -self.dash_direction_

        if self.rounds_ == 2 then
            self.state_ = self.state_diagonal_attack_prepare_
        end

    elseif self.state_ == self.state_initial_shoot_prepare_ then
        self.state_ = self.state_initial_shooting_

        self.attacking_ = true

        local pre_delay = cc.DelayTime:create(self:get_action_duration("standshoot"))

        local pre_callback = cc.CallFunc:create(function()
            self:fire()
        end)

        local post_delay = cc.DelayTime:create(self:get_action_duration("standshoot") * 0.50)

        local post_callback = cc.CallFunc:create(function()
            self.attacking_ = false
            self.state_ = self.state_initial_jumping_prepare_
            self.ticks_ = 0
        end)

        local sequence = cc.Sequence:create(pre_delay, pre_callback, post_delay, post_callback, nil)

        self:runAction(sequence)
    
    elseif self.state_ == self.state_initial_shooting_ then
        return self

    elseif self.state_ == self.state_initial_jumping_prepare_ then
        
        if self.ticks_ > 0.5 and self.ticks_ < 2 then
            if not self.jump_flag_ then
                self.jump_flag_ = true
                self.speed_.y  = self.jump_speed_ * 1.5
                
                self.on_ground_ = false
                self.jumping_ = true
                self.jump_counter_ = self.jump_counter_ + 1
            end
        end
        
        if self.ticks_ > 0.5 and self.ticks_ < 2 then
            if not self.on_ground_ then
                self.speed_.x = -self.walk_speed_ * -self.dash_direction_
            else
                self.speed_.x = 0
                self.ticks_ = 0
                self.jump_flag_ = false
            end
        end

        if self.jump_counter_ == 3 then

            if not self.dash_flag_ then
                self.dash_flag_ = true

                local post_delay = cc.DelayTime:create(1)

                local post_callback = cc.CallFunc:create(function()
                    self.state_ = self.state_dash_prepare_
                end)
        
                local sequence = cc.Sequence:create(post_delay, post_callback, nil)
        
                self:runAction(sequence)
            end

        end

    elseif self.state_ == self.state_dash_prepare_ then
        self.state_ = self.state_dash_

        local dash_delay_1 = cc.DelayTime:create(1)

        local post_callback = cc.CallFunc:create(function()
            self.state_ = self.state_dash_end_
        end)
        

        local dash_delay_2 = cc.DelayTime:create(1.5)

        local dash_callback = cc.CallFunc:create(function()
            self.dash_direction_ = -self.dash_direction_
        end)


        local sequence = cc.Sequence:create(dash_delay_1, dash_callback, dash_delay_2, dash_callback, post_callback, nil)

        self:runAction(sequence)


    elseif self.state_ == self.state_dash_ then

        if self.dash_direction_ == -1 then
            self.sprite_:setFlippedX(false)
        else
            self.sprite_:setFlippedX(true)
        end
        
        self.speed_.x = self.walk_speed_ * 2 * -self.dash_direction_
        self.walking_ = true
    elseif self.state_ == self.state_dash_end_ then
        self.speed_.x = 0
        self.walking_ = false
        self.state_ = self.state_nothing_

        self.rounds_ = self.rounds_ + 1

        if self.dash_direction_ == -1 then
            self.sprite_:setFlippedX(false)
        else
            self.sprite_:setFlippedX(true)
        end

                        --[==[
                



        local post_delay = cc.DelayTime:create(2.5)

        local post_callback = cc.CallFunc:create(function()
            self.state_ = self.state_nothing_
        end)

        local sequence = cc.Sequence:create(post_delay, post_callback, nil)
        
        self:runAction(sequence)
                        ]==]

    elseif self.state_ == self.state_diagonal_attack_prepare_ then

        --print("diagonal")
        if self.ticks_ > 0.5 and self.ticks_ < 2 then
            if not self.jump_flag_ then
                print("Jumping")
                self.jump_flag_ = true
                self.speed_.y  = self.jump_speed_ * 1.25
                
                self.on_ground_ = false
                self.jumping_ = true
                self.jump_counter_ = self.jump_counter_ + 1
            end
        end
        
        if self.ticks_ > 0.5 and self.ticks_ < 2 then
            if not self.on_ground_ then
                --self.speed_.x = -self.walk_speed_ * -self.dash_direction_

                if self.speed_.y < 0 then
                    self.boss_.kinematic_body_.body_:setGravityEnable(false)
                    --self.boss_.kinematic_body_.body_:setVelocity(cc.p(0, 0))
                    self.speed_.y = 0

                    local positionBackup = cc.p(self.boss_.sprite_:getPosition())
                    local anchorBackup = self.boss_.sprite_:getAnchorPoint()
                    local size = self.boss_.sprite_:getContentSize()

                    local delay = cc.DelayTime:create(0.25)
                    
                    local pre_callback = cc.CallFunc:create(function()
                        self.boss_.sprite_:setPosition(cc.exports.UnanchoredPosition(cc.p(0.5, 0.5), self.boss_.sprite_))
                        self.boss_.sprite_:setAnchorPoint(cc.p(0.5, 0.5))

                    end)

                    local rotateBy = cc.RotateBy:create(0.5, 360 * self.dash_direction_)

                    local post_callback = cc.CallFunc:create(function()
                        self.boss_.sprite_:setAnchorPoint(anchorBackup)
                        self.boss_.sprite_:setPosition(positionBackup)
                        self.state_ = self.state_diagonal_slash_prepare_
                    end)
                
                    local sequence = cc.Sequence:create(delay, pre_callback, rotateBy, post_callback, nil)
            
                    self.boss_.sprite_:runAction(sequence)
                end
        

            else
                self.speed_.x = 0
                self.ticks_ = 0
                self.jump_flag_ = false
            end
        end

    elseif self.state_ == self.state_diagonal_slash_prepare_ then
        self.jump_flag_ = false
        self.dash_flag_ = false
        self.state_ = self.state_diagonal_slash_

    elseif self.state_ == self.state_diagonal_slash_ then
        self.speed_.y = -self.jump_speed_ * 1.25
        self.speed_.x = self.walk_speed_ * 14 * self.dash_direction_

        if self.on_ground_ then
            self.speed_.y = 0
            self.speed_.x = 0

            print("flip")
            if self.dash_direction_ == -1 then
                self.sprite_:setFlippedX(false)
            else
                self.sprite_:setFlippedX(true)
            end

            self.dash_flag_ = false
            self.dash_direction_ = -self.dash_direction_

            self.boss_.kinematic_body_.body_:setGravityEnable(true)
            self.ticks_ = 0

            if self.jump_counter_ == 2 then

                if not self.dash_flag_ then
                    self.dash_flag_ = true

                    self.dash_direction_ = -self.dash_direction_

                    self.state_ = self.state_diagonal_slash_reset_
    
                    local post_delay = cc.DelayTime:create(0.25)
    
                    local post_callback = cc.CallFunc:create(function()
                        self.rounds_ = 0
                        self.state_ = self.state_nothing_
                    end)
            
                    local sequence = cc.Sequence:create(post_delay, post_callback, nil)
            
                    self:runAction(sequence)
                end

            else
                self.state_ = self.state_diagonal_attack_prepare_   
            end            
        elseif self.state_ == self.state_diagonal_slash_reset_ then 
            return self
        end
    end

end

function sheriff_browner:walk()    --@TODO implement walk_condition()
    return self    
end

function sheriff_browner:jump()
    return self
end


function sheriff_browner:attack()
    return self
end

function sheriff_browner:fire()

    local bullet_offset = 24

    cc.audio.play_sfx("sounds/sfx_buster_shoot_mid.mp3", false)

    local bullet_position_a = cc.p(self:getParent():getPositionX() + (bullet_offset * self:get_sprite_normal().x),
                                   self:getParent():getPositionY() + 14)

    local bullet_position_b = cc.p(self:getParent():getPositionX() + (bullet_offset * self:get_sprite_normal().x * -1),
                                   self:getParent():getPositionY() + 14)


    local bullet_a = directional_star_bullet:create()
                                         :setPosition(bullet_position_a)
                                         :setup("weapons", "browners", "sheriff", "directional_star_bullet")
                                         :init_weapon(self:get_sprite_normal().x, self.weapon_tag_)
                                         :addTo(self:getParent():getParent())

    local bullet_b = directional_star_bullet:create()
                                            :setPosition(bullet_position_b)
                                            :setup("weapons", "browners", "sheriff", "directional_star_bullet")
                                            :init_weapon(self:get_sprite_normal().x * -1, self.weapon_tag_)
                                            :addTo(self:getParent():getParent())

    self:getParent():getParent().bullets_[bullet_a] = bullet_a
    self:getParent():getParent().bullets_[bullet_b] = bullet_b

end

return sheriff_browner





