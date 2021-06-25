-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local browner                = import("app.objects.characters.enemies.browners.base.subboss_browner")
local linear_missile_bullet     = import("app.objects.weapons.browners.military.linear_missile_bullet")
local item    = import("app.objects.gameplay.level.goods.item")


local newnightmanboss_browner = class("newnightmanboss_browner-enemy", browner)

function newnightmanboss_browner:ctor(sprite, args)
    self.skip_intro_ = true
    self.simple_stun_ = true
    self.default_health_ = 1
    self.super:ctor(sprite, args)

    -- constraints
    self.can_slide_         = false
    self.can_charge_        = false
    self.can_dash_jump_     = false
    self.can_walk_shoot_    = false
    self.can_jump_shoot_    = false

    self.base_name_ = "newnightboss"

    local actions = {}
    actions[#actions + 1] = {name = "stand",      animation = {name = "newnightboss_stand",       forever = false, delay = 0.10} }
    actions[#actions + 1] = {name = "jump",       animation = {name = "newnightboss_jump",        forever = false, delay = 0.07} }
    actions[#actions + 1] = {name = "walk",       animation = {name = "newnightboss_walk",        forever = true,  delay = 0.12} }
    actions[#actions + 1] = {name = "walkshoot",  animation = {name = "newnightboss_walkshoot",        forever = true,  delay = 0.12} }
    actions[#actions + 1] = {name = "standshoot", animation = {name = "newnightboss_standshoot",  forever = false, delay = 0.10} }

    self.sprite_:load_actions_set(actions, true, self.base_name_)

    self.browner_id_ = cc.browners_.newnightmanboss_.id_       -- overriden from parent

    self.dash_direction_ = 1
    
    self.state_nothing_ = 0
    self.state_initial_shoot_prepare_ = 1
    self.state_initial_shooting_ = 2

    self.state_dash_prepare_ = 3
    self.state_dash_ = 4
    self.state_dash_end_ = 5

    self.state_dash_prepare_no_jump_ = 6

    self.state_dash_end_idle_ = 7
    self.state_jump_flags_ = 8
        
    self:reset_flags()
end

function newnightmanboss_browner:reset_flags()
    self.ticks_ = 0

    self.jump_flag_ = false
    self.double_jump_flag_ = false
    self.rounds_ = 0

    self.double_jump_counter_ = 0
    self.jump_air_counter_ = 0

    self.freeze_time_ticks_ = 0

    self.state_ = self.state_jump_flags_
end

function newnightmanboss_browner:update(dt)
    self.ticks_ = self.ticks_ + dt

    local self_position = self.sprite_:convertToWorldSpace(cc.p(self.sprite_:getPositionX(), self.sprite_:getPositionY()))
    local bounds_position = cc.bounds_:center()
    --print("self position x")
    --print(self_position.x)

    --print("bounds center x")
    --print(bounds_position.x)  
    if self.dash_direction_  == 1 then
        if self.rounds_ == 3 then
            if self_position.x <= bounds_position.x + 16 and self_position.x >= bounds_position.x then
                if self.double_jump_counter_ == 0 then
                    self.double_jump_flag_ = true    
                end
            end                    
        end
    else
        if self.rounds_ == 3 then
            if self_position.x >= bounds_position.x - 80 and self_position.x <= bounds_position.x then
                if self.on_ground_ then
                    if self.double_jump_counter_ == 0 then
                        self.double_jump_flag_ = true    
                    end
                end
            end
        end
    end

    if self.dash_direction_  == 1 then

        if self.rounds_ < 3 then
        
            if self_position.x <= bounds_position.x + 16 and self_position.x >= bounds_position.x then
                if self.on_ground_ then
                    self.jump_flag_ = true    
                end
            end
        end
    else
        if self.rounds_ < 3 then
            if self_position.x >= bounds_position.x - 64 and self_position.x <= bounds_position.x then
                if self.on_ground_ then
                    self.jump_flag_ = true    
                end
            end
        end
    end

    if self.state_ == self.state_jump_flags_ then
        self.state_ = self.state_nothing_    
    elseif self.state_ == self.state_nothing_ then
        self.state_ = self.state_initial_shoot_prepare_
        self.jump_flag_ = false

    elseif self.state_ == self.state_initial_shoot_prepare_ then
        self.speed_.x = 0
        self.walking_ = false

        self.jump_air_counter_ = 0
        self.double_jump_counter_ = 0

        self.rounds_ = self.rounds_ + 1

        self.state_ = self.state_initial_shooting_

        local pre_delay = cc.DelayTime:create(self:get_action_duration("standshoot"))

        local pre_callback = cc.CallFunc:create(function()
            self.attacking_ = true

            self:fire()
        end)

        local post_delay = cc.DelayTime:create(0.280)

        local post_callback = cc.CallFunc:create(function()
            self.attacking_ = false
        end)

        local final_delay = cc.DelayTime:create(0.5)

        local final_callback = cc.CallFunc:create(function()
            self.state_ = self.state_dash_prepare_
        end)

        local time_between = cc.DelayTime:create(0.5)

        local sequence1 = cc.Sequence:create(pre_delay, pre_callback, post_delay, post_callback, nil)
        local sequence2 = cc.Sequence:create(pre_delay, pre_callback, post_delay, post_callback, nil)
        local sequence3 = cc.Sequence:create(pre_delay, pre_callback, post_delay, post_callback, nil)

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
        self.speed_.x = 0
        self.walking_ = false
        
        return self
    elseif self.state_ == self.state_dash_prepare_ then
        self.state_ = self.state_dash_
    elseif self.state_ == self.state_dash_prepare_no_jump_ then
        return self
    elseif self.state_ == self.state_dash_ then       

        if self.rounds_ >= 3 then
            --self.speed_.x = self.walk_speed_ * 2 * -self.dash_direction_
            self.speed_.x = (self.walk_speed_ * 2 * -self.dash_direction_) / 1.50
        else
            self.speed_.x = self.walk_speed_ * 2 * -self.dash_direction_
        end

        if self.jump_flag_ then
            self.jump_flag_ = false

            self.speed_.y  = self.jump_speed_
            self.on_ground_ = false
            self.jumping_ = true            
        end

        if self.on_ground_ then
            if self.double_jump_flag_  then
                if self.double_jump_counter_ == 1 then
                    self.double_jump_flag_ = false
                    self.speed_.y  = self.jump_speed_ * 2
                    self.on_ground_ = false
                    self.jumping_ = true 

                    self.rounds_ = 0
                    self.freeze_time_ticks_ = 0
                elseif self.double_jump_counter_ == 0 then
                    self.double_jump_counter_ = self.double_jump_counter_ + 1
                    self.speed_.y  = self.jump_speed_ * 4
                    self.on_ground_ = false
                    self.jumping_ = true 
                end
            end
        end

        if self.on_ground_ then
            self.walking_ = true
        else
            self.jumping_ = true            
        end

        local self_position = self.sprite_:convertToWorldSpace(cc.p(self.sprite_:getPositionX(), self.sprite_:getPositionY()))
        local bounds_left = cc.bounds_:left()
        local bounds_right = cc.bounds_:right()

        if self.dash_direction_  == 1 then
            if self_position.x <= bounds_left + 16 then
                self.dash_direction_ = -1
                self.state_ = self.state_initial_shoot_prepare_
                self.walking_ = false
                self.speed_.x = 0
            end
        else
            if self_position.x >= bounds_right - 64 then
                self.dash_direction_ = 1
                self.state_ = self.state_initial_shoot_prepare_
                self.walking_ = false
                self.speed_.x = 0
            end
        end

        if self.dash_direction_ == -1 then
            self.sprite_:setFlippedX(false)
        else
            self.sprite_:setFlippedX(true)
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

    if self.double_jump_flag_  then
        if self.jumping_ then
            self.freeze_time_ticks_ = self.freeze_time_ticks_ + 1

            if self.double_jump_counter_ > 0 then

                if self.freeze_time_ticks_ < 80 then
                    if self.speed_.y <= 0 then
                        self.speed_.y = 0
                        self.speed_.x = 0
                    end

                end
            end
        end
    end
    
    if self.jumping_ and not self.double_jump_flag_  then
        self.jump_air_counter_ = self.jump_air_counter_ + 1
        if self.jump_air_counter_ < 120 and self.double_jump_counter_ == 0 then
            if self.speed_.y <= 0 then
                self.speed_.y = 0
                self.speed_.x = 0
            end
        end
    end
    
end


function newnightmanboss_browner:walk()    --@TODO implement walk_condition()
    return self
end

function newnightmanboss_browner:jump()
    return self
end

function newnightmanboss_browner:attack()
    return self
end

function newnightmanboss_browner:fire()

    local bullet_offset = 24

    cc.audio.play_sfx("sounds/sfx_buster_shoot_mid.mp3", false)

    local bullet_position = cc.p(self:getParent():getPositionX() + (bullet_offset * self:get_sprite_normal().x),
                                 self:getParent():getPositionY() + 26)

        local bullet = linear_missile_bullet:create()
                                            :setPosition(bullet_position)
                                            :setup("weapons", "browners", "military", "linear_missile_bullet")
                                            :init_weapon(self:get_sprite_normal().x, self.weapon_tag_)
                                            :addTo(self:getParent():getParent())

        self:getParent():getParent().bullets_[bullet] = bullet

end


function newnightmanboss_browner:onDefeated()

    print("On defeated")

    self:stopAllActions()

    cc.is_boss_area_ = false
    
    local item_array = {}

    if cc.unlockables_.items_.fist_.acquired_ then
        item_array[1] = "health_big"
    else
        item_array[1] = "fist"
    end

    if item_array[1] ~= nil then
        local item_good = item:create()
                              :setup("gameplay", "level", "goods", "item")
                              :setPosition(cc.p(self:getParent():getPositionX(), self:getParent():getPositionY()))

        item_good:swap(item_array[1], true)

        if item_array[1] == "fist" then
            item_good:set_name("fist")
        end

        self:getParent():getParent():schedule_component(item_good)
    end

end


return newnightmanboss_browner





