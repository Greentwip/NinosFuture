-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local boss = import("app.core.physics.kinematic_character").create("boss")

local energy_bar        = import("app.objects.gameplay.level.ui.energy_bar")


local teleport_browner  = import("app.objects.characters.enemies.browners.teleport_browner")

function boss:onCreate(args)
    self.player_ = args.player_
    self.is_sub_boss_ = args.is_sub_boss_
    self.power_ = 10
    self.health_bar_ = nil -- this will be set up on teleporter collision
    self:init_variables()
    self.battle_status_ = cc.battle_status_.startup_
end

function boss:onAfterAnimate(args)
    self.start_position_ = args.anchored_position_
    self:setPosition(self.start_position_)
    self:init_browners(args)
end


function boss:center()
    local center = self.kinematic_body_:center()
    return center
end

function boss:top()
    local shape  = self.kinematic_body_.body_:getShapes()[1]
    local top    = self:center().y + shape.size_.height * 0.5
    return top
end

function boss:bottom()
    local shape     = self.kinematic_body_.body_:getShapes()[1]
    local bottom    = self:center().y - shape.size_.height * 0.5
    return bottom
end

function boss:init_variables()
    self.spawning_          = false
    self.vulnerable_        = true
    self.max_fall_speed_    = 400
    self.health_            = 0
    self.alive_             = false         -- level starts up player
    self.spawn_flag_        = false
    self.defeated_          = false
end

function boss:init_browners(args)

    self.browners_ = {}

    local browner      = import("app.objects.characters.enemies.browners." .. args.type_ .. "_" .. "browner")

    self.type_ = args.type_

    args.boss_ = self

    local boss_browner = browner:create(self.sprite_, args)
                                :addTo(self)

    boss_browner:deactivate()

    if not self.is_sub_boss_ then
        self.browners_[cc.browners_.boss_.id_] = boss_browner
    else
        self.browners_[cc.browners_.sub_boss_.id_] = boss_browner
    end

    self.demo_browner_id_ = boss_browner.browner_id_

    for _, v in pairs(self.browners_) do
        v:setPosition(cc.p(0, 0))
        v:run_action("stand")
        v.weapon_tag_ = cc.tags.weapon.enemy
    end

    -- teleport browner has no stand action 

    self.current_browner_ = boss_browner

    if not self.current_browner_.skip_intro_ then
        self.browners_[cc.browners_.teleport_.id_] = teleport_browner:create(self.sprite_)
                                                                     :setPosition(cc.p(0, 0))
                                                                     :addTo(self)

        self.current_browner_ = self.browners_[cc.browners_.teleport_.id_]

        self:switch_browner(cc.browners_.teleport_.id_)
    else
        if not self.is_sub_boss_ then
            self:switch_browner(cc.browners_.boss_.id_)
        else
            self:switch_browner(cc.browners_.sub_boss_.id_)
        end
        
    end

    self.current_browner_:run_action("stand") 
end

function boss:reset()
    for k, v in pairs(self.browners_) do
        local browner = self.browners_[k]
        browner:stop_actions()
        browner:spawn()
        browner:run_action("stand")
        browner.weapon_tag_ = cc.tags.weapon.enemy
    end
    self:getPhysicsBody():getShapes()[1]:setTag(cc.tags.weapon.enemy)
end

function boss:spawn()

    self.spawning_ = true
    self:setPosition(self.start_position_)

    if self.current_browner_.default_health_ ~= nil then
        self.health_            = self.current_browner_.default_health_
    else
        self.health_            = 0
    end


    --self:switch_browner(cc.browners_.teleport_.id_)
    --self.current_browner_ = self.browners_[cc.browners_.teleport_.id_]

    self:reset()

    if self.current_browner_.skip_intro_ == nil then
        self:switch_browner(cc.browners_.teleport_.id_)
    else
        if not self.is_sub_boss_ then
            self:switch_browner(cc.browners_.boss_.id_)
        else
            self:switch_browner(cc.browners_.sub_boss_.id_)
        end
    end

    self.current_browner_:run_action("stand")

    self.vulnerable_ = true

    self.sprite_:setFlippedX(true)
    self.sprite_:setVisible(true)

    self.alive_ = true
    
    self.current_browner_:reset_flags()
end

function boss:switch_browner(id)

    self.current_browner_:stop_actions()
    self.current_browner_:deactivate()

    local new_browner = self.browners_[id]

    new_browner.on_ground_ = self.current_browner_.on_ground_

    self.current_browner_ = new_browner
    self.current_browner_:activate()

    self.current_browner_:reset_flags()

    print("Switching browner to")
    print(tostring(id))
    print(tostring(self.browners_[id].browner_id_))
    

end

function boss:walk()

    if self.current_browner_.can_walk_ then
        self.current_browner_:walk()
    end

end

function boss:jump()

    if self.current_browner_.can_jump_ then
        self.current_browner_:jump()
    end

end

function boss:dash_jump()

    if self.current_browner_.can_dash_jump_ then
        self.current_browner_:dash_jump()
    end

end

function boss:slide()

    if self.current_browner_.can_slide_ then
        self.current_browner_:slide()
    end

end

function boss:climb()
    if self.current_browner_.can_climb_ then
        self.current_browner_:climb()
    end
end

function boss:attack()
    if self.current_browner_.can_attack_ then
        self.current_browner_:attack()
    end
end

function boss:charge()
    if self.current_browner_.can_charge_ then
        self.current_browner_:charge()
    end
end


function boss:restore_health(amount, callback)
    cc.callbacks_.energy_fill(self, self, amount, {health_ = true, energy_ = false}, callback)
end

function boss:restore_energy(amount)
    cc.callbacks_.energy_fill(self, self.current_browner_, amount, {health_ = false, energy_ = true})
end

function boss:pause_actions()
    self.current_browner_:pause_actions()
end

function boss:resume_actions()
    self.current_browner_:resume_actions()
end


function boss:stun(damage)
    if not self.current_browner_.stunned_ and self.vulnerable_ or self.current_browner_.simple_stun_ ~= nil  then
        
        self.health_ = self.health_ - damage

        self.current_browner_.stunned_ = true
        self.vulnerable_ = false

        self.current_browner_.charge_power_ = "low"
        self.current_browner_.charging_ = false

        if not self.current_browner_.sliding_ then
            self.current_browner_.speed_.x = -4 * self.current_browner_:get_sprite_normal().x
        end

        local callback = cc.CallFunc:create(function()
            self.current_browner_.stunned_ = false
        end)

        local sequence = nil

        if self.current_browner_.simple_stun_ == nil then
            cc.audio.play_sfx("sounds/sfx_hit.mp3", false)

            local delay = cc.DelayTime:create(self.current_browner_:get_action_duration("hurt"))

            local blink = cc.Blink:create(self.current_browner_:get_action_duration("hurt"), 8)

            local blink_callback = cc.CallFunc:create(function()
                if not self:isVisible() then
                    self:setVisible(true)
                end

                if not self.sprite_:isVisible() then
                    self.sprite_:setVisible(true)
                end

                self.vulnerable_ = true
            end)

             sequence = cc.Sequence:create(delay, callback, blink, blink, blink_callback, nil)

             sequence:setTag(cc.tags.actions.visibility)
             self:stopAllActionsByTag(cc.tags.actions.visibility)
             self:runAction(sequence)

        else
            cc.audio.play_sfx("sounds/sfx_enemyhit.mp3", false)

            local blink = cc.Blink:create(0.2, 4)

            local blink_callback = cc.CallFunc:create(function()
                if not self:isVisible() then
                    self:setVisible(true)
                end

                if not self.sprite_:isVisible() then
                    self.sprite_:setVisible(true)
                end

                self.vulnerable_ = true
            end)

             sequence = cc.Sequence:create(blink, blink_callback, callback, nil)
             sequence:setTag(cc.tags.actions.visibility)
             self.sprite_:stopAllActionsByTag(cc.tags.actions.visibility)
             self.sprite_:runAction(sequence)
     
        end

    end
end

function boss:solve_collisions()

    if self.spawning_ then 
        if not self.current_browner_.skip_intro_ then
            self.current_browner_.speed_.y = -256
            self.kinematic_body_.ignore_block_collisions_ = true
        end
    else
        if not self.spawn_flag_ then
            self.spawn_flag_ = true
            self.current_browner_.speed_.y = 0
            self.kinematic_body_.ignore_block_collisions_ = false
        end
    end

    local collisions = self.kinematic_body_:get_collisions()

    for _, collision in pairs(collisions) do

        local collision_tag = collision:getPhysicsBody():getShapes()[1]:getTag()

        if collision_tag == cc.tags.teleporter then

            if self.spawning_ then
                cc.pause(false)
                self.spawning_ = false
                self.spawn_flag_ = false
                if not self.is_sub_boss_ then
                    self:switch_browner(cc.browners_.boss_.id_)
                else
                    self:switch_browner(cc.browners_.sub_boss_.id_)
                end

                if self.current_browner_.skip_intro_ == nil then
                    self.current_browner_:run_action("intro")
                    self.current_browner_.is_intro_ = true

                    cc.audio.play_sfx("sounds/sfx_teleport1.mp3", false)
                    self.battle_status_ = cc.battle_status_.intro_
                else
                    self.battle_status_ = cc.battle_status_.intro_
                end
            end

        elseif collision_tag == cc.tags.weapon.player then

            if collision.power_ >= 3 and self.health_ <= 0 then
                collision.disposed_ = false
            else
                collision.disposed_ = true
            end

            self:stun(collision.power_)
        end
    end

end

function boss:move()

    self.current_browner_.speed_ = self.kinematic_body_.body_:getVelocity()

    if self.contacts_[cc.kinematic_contact_.down] then
        self.current_browner_.speed_.y = 0

        if not self.current_browner_.on_ground_ and not self.current_browner_.climbing_ then
            self.current_browner_.on_ground_    = true
            self.current_browner_.dash_jumping_ = false
            self.current_browner_.jumping_      = false
            --cc.audio.play_sfx("sounds/sfx_land.mp3", false)
            --print("land")
        end
    end

    if self.contacts_[cc.kinematic_contact_.up] then
        if self.current_browner_.speed_.y > 0 then
            self.current_browner_.speed_.y = -1
        end
    end

    if self.battle_status_ == cc.battle_status_.fighting_ then

        self:update(1/60)

        self:walk()
        self:jump()
        self:dash_jump()

    end

    if self.contacts_[cc.kinematic_contact_.right] then
        if self.current_browner_.speed_.x > 0 then
            self.current_browner_.speed_.x = 0
        end
    elseif self.contacts_[cc.kinematic_contact_.left] then
        if self.current_browner_.speed_.x < 0 then
            self.current_browner_.speed_.x = 0
        end
    end
end

function boss:explode(x_offset)

    local y_offset = 16

    local creation_args = {}
    creation_args.real_position_    = cc.p(self:getPositionX(), self:getPositionY())
    creation_args.type_    = "directional"
    creation_args.sprite_color_ =   cc.c3b(255,255,255)

    local explosion = import("app.objects.gameplay.level.fx.explosion")

    for i = 1, 9 do

        if i ~= 5 then
            creation_args.direction_ =  cc.p(creation_args.real_position_.x + x_offset, creation_args.real_position_.y + y_offset)

            local death_explosion = explosion:create(creation_args)
                                             :setup("gameplay", "level", "fx", "explosion")
                                             :addTo(self:getParent(), 1024)

            death_explosion:build(creation_args)

            self:getParent().animations_[death_explosion] = death_explosion

        end

        if i % 3 == 0 then
            x_offset = -16
            y_offset = y_offset - 16
        else
            x_offset = x_offset + 16
        end

    end
end

function boss:finish(full_callback)
    self.exit_arguments_ = {}
    self.exit_arguments_.demo_browner_id_ = self.demo_browner_id_
    self.exit_arguments_.is_level_complete_ = not full_callback -- means skip music, therefore boss already defeated.

    self.player_.can_move_ = false
    self.player_.on_end_battle_ = true

    for _, v in pairs(cc.browners_) do
        if v.id_ == self.demo_browner_id_ then
            v.acquired_ = true
            cc.levels_[v.id_ - 3].defeated_ = true
        end
    end


    local delay = cc.DelayTime:create(2)

    local audio_callback = cc.CallFunc:create(function()
        ccexp.AudioEngine:stopAll()
        cc.audio.play_sfx("sounds/bgm_boss_victory.mp3", false)
    end)

    local exit_callback = cc.CallFunc:create(function()
        ccexp.AudioEngine:stopAll()
        self.player_:exit(self.exit_arguments_)
    end)

    local sequence

    if full_callback then
        sequence = cc.Sequence:create(delay,
                                      audio_callback,
                                      delay,
                                      delay, exit_callback, nil)
    else
        sequence = cc.Sequence:create(delay,
                                      delay, exit_callback, nil)
    end


    local slot = cc.game.get_default_slot()

    slot[self.type_] = true

    cc.game.save_default_slot(slot)

    self:runAction(sequence)
end

function boss:check_health()
    
    local kill_animation = true

    if self.current_browner_.simple_stun_ ~= nil then
        kill_animation = false
    end

    if self.health_ <= 0 and self.alive_ then
        --cc.pause(true)
        self.battle_status_ = cc.battle_status_.defeated_
        self.current_browner_.speed_.x = 0
        self.current_browner_.speed_.y = 0

        self.current_browner_:deactivate()
        self.health_ = 0
        self.alive_ = false

        self:stopAllActionsByTag(cc.tags.actions.visibility)

        --cc.player_.lives_ = cc.player_.lives_ - 1

        if kill_animation then
            ccexp.AudioEngine:stopAll()
            cc.audio.play_sfx("sounds/sfx_death.mp3", false)
    
            self:finish(true)

            self.sprite_:setVisible(false)


            local explosion_a = cc.CallFunc:create(function()
                self:explode(-16)
            end)

            local delay = cc.DelayTime:create(0.20)

            local explosion_b = cc.CallFunc:create(function()
                self:explode(-12)
            end)

            local sequence = cc.Sequence:create(explosion_a, delay, explosion_b, nil)

            self:runAction(sequence)

        else
            cc.audio.play_sfx("sounds/sfx_death.mp3", false)

            
            local explosion_a = cc.CallFunc:create(function()
                self:explode(-16)
            end)

            local delay = cc.DelayTime:create(0.20)

            local explosion_b = cc.CallFunc:create(function()
                self:explode(-12)
            end)

            local sequence = cc.Sequence:create(explosion_a, delay, explosion_b, nil)

            self:runAction(sequence)

            self:getPhysicsBody():getShapes()[1]:setTag(cc.tags.weapon.none)
            self.sprite_:setVisible(false)

            if self.current_browner_.onDefeated then
                self.current_browner_:onDefeated()
            end
        end
    end

end

function boss:trigger_actions()
    if not self.current_browner_.stunned_ or self.current_browner_.simple_stun_ ~= nil then
        if self.current_browner_.on_ground_ then
            if self.current_browner_.walking_ then
                if self.current_browner_.attacking_ then
                    self.current_browner_:run_action("walkshoot")
                else
                    self.current_browner_:run_action("walk")
                end
            else
                if self.current_browner_.attacking_ then
                    self.current_browner_:run_action("standshoot")
                elseif self.current_browner_.sliding_ then
                    self.current_browner_:run_action("slide")
                elseif self.current_browner_.is_intro_ then
                    self.current_browner_:run_action("intro")
                else
                    self.current_browner_:run_action("stand")
                end
            end
        else
            if self.current_browner_.climbing_ then
                if self.current_browner_.attacking_ then
                    self.current_browner_:run_action("climbshoot")
                else
                    self.current_browner_:run_action("climb")
                end
            else
                if self.current_browner_.attacking_ then
                    self.current_browner_:run_action("jumpshoot")
                else
                    if self.current_browner_.dash_jumping_ then
                        self.current_browner_:run_action("dashjump")
                    elseif self.current_browner_.jumping_ then
                        --print("is jumping")
                        self.current_browner_:run_action("jump")
                    end
                end
            end
        end
    else
        if self.current_browner_.simple_stun_ == nil then
            self.current_browner_:run_action("hurt")
        end
    end

end

function boss:update(dt)
   self.current_browner_:update(dt) 
end

function boss:kinematic_step(dt)
    if cc.game_status_ == cc.GAME_STATUS.RUNNING or self.spawning_ then
        self:compute_position()
        self:move(dt)
    end
end

function boss:kinematic_post_step(dt)

    if cc.game_status_ == cc.GAME_STATUS.RUNNING or self.spawning_ then

        if self.kinematic_body_.body_:getVelocity().x ~= self.current_speed_.x
            or self.kinematic_body_.body_:getVelocity().y ~= self.current_speed_.y then

            self.kinematic_body_.body_:setVelocity(self.current_speed_)

        end

    else
        self.kinematic_body_.body_:setVelocity(cc.p(0, 0))
        self.kinematic_body_.collisions_ = {}
    end

end

function boss:forced_step(dt)
    self:kinematic_step(dt)

    local bbox = self.kinematic_body_:bbox()

    bbox.y = cc.bounds_:top() - cc.bounds_:height() / 2 -- battle offset

    if cc.bounds_:is_rect_inside(bbox) then

        if self.defeated_ then
            cc.is_boss_area_ = false
        else
            cc.is_boss_area_ = true
        end

        if self.battle_status_ == cc.battle_status_.startup_ then

            local defeated = false

            for _, v in pairs(cc.browners_) do
                if v.id_ == self.demo_browner_id_ then
                    if cc.levels_[v.id_ - 3].defeated_ then
                        defeated = true
                    end
                end
            end

            if defeated then
                self.battle_status_ = cc.battle_status_.waiting_

                self:finish(false)
            else
                ccexp.AudioEngine:stopAll()
                cc.audio.play_bgm("sounds/bgm_boss_vineman.mp3", true)

                --self.player_.can_move_ = false
                cc.joypad_.take_inputs_ = false
                self:spawn()
                self.battle_status_ = cc.battle_status_.waiting_
            end

        elseif self.battle_status_ == cc.battle_status_.intro_ then

            if self.current_browner_.skip_intro_ == nil then

                if self.health_bar_ == nil then
                    self.health_bar_ = energy_bar:create()
                                                :setPosition(cc.p(cc.bounds_:width() * 0.5 - 16, cc.bounds_:height() * 0.5 -16))
                                                :addTo(cc.bounds_)     -- node positions are relative to parent's
                else
                    self.health_bar_:setVisible(true)
                end

                local callback = cc.CallFunc:create(function()
                    self.current_browner_.is_intro_ = true
                end)

                local delay = cc.DelayTime:create(self.current_browner_:get_action_duration("intro"))

                local fill_bar = cc.CallFunc:create(function()
                    self:restore_health(28, function()
                        self.current_browner_.is_intro_ = false
                        self.player_.can_move_ = true
                        cc.joypad_.take_inputs_ = true
                        self.battle_status_ = cc.battle_status_.fighting_
                    end)

                end)

                local sequence = cc.Sequence:create(callback, delay, fill_bar, nil)

                self:runAction(sequence)

                self.battle_status_ = cc.battle_status_.waiting_
            else

                local callback = cc.CallFunc:create(function()
                    self.current_browner_.is_intro_ = false
                end)

                local delay = cc.DelayTime:create(1)

                local fill_bar = cc.CallFunc:create(function()
                    self.health_ = self.current_browner_.default_health_
                    self.player_.can_move_ = true
                    cc.joypad_.take_inputs_ = true
                    self.battle_status_ = cc.battle_status_.fighting_
                    cc.pause(false)
                end)

                local sequence = cc.Sequence:create(callback, delay, fill_bar, nil)

                self:runAction(sequence)

                self.battle_status_ = cc.battle_status_.waiting_
            end

        elseif self.battle_status_ == cc.battle_status_.defeated_ then
            if self.current_browner_.skip_intro_ == nil then
                self.health_bar_:removeSelf()
                self.health_bar_ = nil
            end
            self.defeated_ = true
            self.battle_status_ = cc.battle_status_.waiting_
        end
        if self.spawning_ and self.alive_ then
            self:solve_collisions()
        elseif self.alive_ then
            self:solve_collisions()
        end

        if cc.game_status_ == cc.GAME_STATUS.RUNNING then
            if self.spawning_ and self.alive_ then
                self:solve_collisions()
            elseif self.alive_ then

                if cc.game_status_ == cc.GAME_STATUS.RUNNING then
                    self:solve_collisions()
                    if self.battle_status_ == cc.battle_status_.fighting_ then
                        self:attack()
                        self:check_health()
                    end
                end
            end
        end
    else

        if self.battle_status_ ~= cc.battle_status_.startup_ then
            self.battle_status_ = cc.battle_status_.startup_

            if self.health_bar_ ~= nil then
                self.health_bar_:setVisible(false)
            end

            self:setPosition(self.start_position_)

            self.sprite_:setVisible(false)
        end

        if self.sprite_:isVisible() then
            self.sprite_:setVisible(false)
        end

        self.current_browner_.speed_.x = 0
        self.current_browner_.speed_.y = 0
    end

    if self.battle_status_ ~= cc.battle_status_.defeated_ then
        if self.health_bar_ ~= nil then
            self.health_bar_:set_meter(self.health_)
        end
    end

    return self
end

function boss:step(dt)

end

function boss:post_step(dt)

    self.current_speed_ = self.current_browner_.speed_
    self:kinematic_post_step(dt)

    if cc.game_status_ == cc.GAME_STATUS.RUNNING then
        self:trigger_actions()
    end

end

return boss

