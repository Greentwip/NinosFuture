-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local sprite = import("app.core.graphical.sprite")
local enemy  = import("app.objects.characters.enemies.base.enemy")
local tremor = class("tremor", enemy)
local item    = import("app.objects.gameplay.level.goods.item")

function tremor:onCreate()
    self.default_health_ = 85
    self.power_ = 10

    self.kinematic_body_size_   = cc.size(40.0, 40.0) -- default is cc.size(16.0, 16.0)
    self.kinematic_body_offset_ = cc.p(0.0, 0.0)      -- default is cc.p(0, 0)

    self.weapon_parameters_ = {
        category_ = "gameplay",
        sub_category_ = "level",
        package_ = "weapon",
        cname_ = "tremor_laser"
    }

    self.weapon_ = import("app.objects.weapons.enemies.sheriff.tremor_laser")

    self:init_variables()

end

function tremor:animate(cname)

    local head_action   =  { name = "head",  animation = { name = cname .. "_" .. "head",  forever = false, delay = 0.10 } }

    self.sprite_:load_action(head_action, false)
    self.sprite_:set_animation(cname .. "_" .. "head")
    self.sprite_:run_action("head")
    self.sprite_:reverse_action()


    self.body_sprites_ = {}

    local y_offset = self.sprite_:getPositionY()

    local x_offset = 5

    for i = 1, 5 do
        self.body_sprites_[i] = sprite:create("sprites/characters/enemy/sheriff/tremor/tremor")
                                      :addTo(self, -i)

        y_offset = y_offset - self.body_sprites_[i]:getContentSize().height * 0.75

        self.body_sprites_[i]:setPosition(cc.p(x_offset, y_offset))
    end

    return self
end

function tremor:on_after_init() -- should be called after attached to parent

    self.tail_ = cc.Node:create()
                        :setPosition(self:getPositionX(), self:getPositionY() - 256)
                        :addTo(self:getParent())

    self.tail_parameters_ = {
        category_ = "gameplay",
        sub_category_ = "level",
        package_ = "weapon",
        cname_ = "tremor_tail"
    }

    self.tremor_tail_weapon_ = import("app.objects.weapons.enemies.sheriff.tremor_tail")

    self.tail_.drill_ = self.tremor_tail_weapon_:create()
                                                        :setPosition(cc.p(0, 0))
                                                        :setAnchorPoint(cc.p(0.5, 0))
                                                        :setup(self.tail_parameters_.category_,
                                                                self.tail_parameters_.sub_category_,
                                                                self.tail_parameters_.package_,
                                                                self.tail_parameters_.cname_)
                                                        :init_weapon(self:get_sprite_normal().x, cc.tags.weapon.none)
                                                        :addTo(self.tail_)

    local x_offset = 0
    local y_offset = 0

    for i = 1, 5 do
        local tail_body = sprite:create("sprites/characters/enemy/sheriff/tremor/tremor")
                                :addTo(self.tail_, -i)

        y_offset = y_offset - tail_body:getContentSize().height * 0.75

        tail_body:setPosition(cc.p(x_offset, y_offset))
    end

    self:getPhysicsBody():getShapes()[1]:setTag(cc.tags.weapon.enemy)
end

function tremor:get_sprite_normal()
    local x_normal = -1

    local normal = cc.p(x_normal, 1)
    return normal
end


function tremor:normalize_orientation()
end

function tremor:onRespawn()
    self:init_variables()
    self.start_position_ = cc.p(self:getPositionX(), self:getPositionY())
    self.sprite_:run_action("head")
    self.sprite_:reverse_action()
    self:setVisible(true)
    self.tail_:setVisible(true)
    self.tail_.drill_:setVisible(true)
end

function tremor:onDefeated()
    self:setVisible(false)
    self.tail_:setVisible(false)
    self.tail_.drill_:setVisible(false)

    cc.audio.play_sfx("sounds/sfx_explosion1.mp3", false)


    local item_array = {}

    if cc.unlockables_.items_.head_.acquired_ then
        item_array[1] = "health_big"
    else
        item_array[1] = "head"
    end

    if item_array[1] ~= nil then
        local item_good = item:create()
                              :setup("gameplay", "level", "goods", "item")
                              :setPosition(cc.p(self:getPositionX(), self:getPositionY()))

        item_good:swap(item_array[1], true)

        if item_array[1] == "head" then
            item_good:set_name("head")
        end

        self:getParent():schedule_component(item_good)
    end

    cc.level_controller_.horizontal_doors_[1]:unlock()

end


function tremor:init_variables()
    self.start_position_ = nil
    self.shooting_ = false
    self.moving_ = false
    self.cannon_attack_count_ = 0
    self.attacking_ = false
end

function tremor:on_tail_attack_end() -- prepare to move again

end

function tremor:on_cannon_attack_end() -- prepare tail attack
    self.sprite_:reverse_action()

    self.tail_.drill_.sprite_:run_action("tail_idle")


    --if self.cannon_attack_count_ % 8 == 0 then
    local player_xy = cc.MoveTo:create(1, cc.p(self.player_:getPositionX(), self.player_:getPositionY()))

    local screen_xy = cc.MoveTo:create(1, cc.p(self:getPositionX(), self:getPositionY() - 256))

    local delay = cc.DelayTime:create(self.tail_.drill_.sprite_:get_action_duration("tail_a") * 8)
    local half_delay = cc.DelayTime:create(self.tail_.drill_.sprite_:get_action_duration("tail_a") * 4)

    local sequence = cc.Sequence:create(player_xy,
                                        cc.CallFunc:create(function()
                                            self.tail_.drill_.sprite_:run_action("tail_a")
                                        end),
                                        half_delay,
                                        cc.CallFunc:create(function()
                                            self.tail_.drill_:enable_attack()
                                        end),
                                        cc.CallFunc:create(function()
                                            self.tail_.drill_.sprite_:run_action("tail_b")
                                        end),
                                        delay,
                                        cc.CallFunc:create(function()
                                            self.tail_.drill_:disable_attack()
                                        end),
                                        cc.CallFunc:create(function()
                                            self.tail_.drill_.sprite_:run_action("tail_c")
                                        end),
                                        half_delay,
                                        screen_xy,
                                        nil)

    self.tail_:stopAllActions()
    self.tail_:runAction(sequence)

    local reinit = function()
        self.attacking_ = false
        self.moving_ = false
    end

    local move_to_start  = cc.MoveTo:create(2, self.start_position_)
    local move_delay = cc.DelayTime:create(1)
    local sequence   = cc.Sequence:create(move_to_start, 
                                        move_delay, 
                                        cc.CallFunc:create(reinit), nil)

    self:runAction(sequence)
    --end
end

function tremor:on_move_end() -- prepare cannon attack
    self.attacking_ = true
end

function tremor:walk()

    if not self.moving_ then

        self.moving_ = true
        local move_down  = cc.MoveTo:create(1.5, cc.p(self:getPositionX(), self:getPositionY() - 128))
        local move_up  = cc.MoveTo:create(1.5, cc.p(self:getPositionX(), self:getPositionY() + 24))
        local move_delay = cc.DelayTime:create(1)

        local sequence   = cc.Sequence:create(move_down, 
                                            move_delay, 
                                            move_up, 
                                            move_delay, 
                                            cc.CallFunc:create(self.on_move_end), nil)

        self:runAction(sequence)

    end

end

function tremor:jump()
    self.current_speed_.y = 0 -- to make him float
end

function tremor:attack()

    if self.attacking_ then
        self.attacking_ = false

        local locate_player_y  = cc.MoveTo:create(1, cc.p(self:getPositionX(), self.player_:getPositionY()))

        local animate_attack = cc.CallFunc:create(function()
            self.sprite_:run_action("head")
        end)

        local fire_callback = cc.CallFunc:create(function()

            local offset = cc.p(20, -10)

            local laser = self:fire({
                            sfx = nil,
                            offset = offset,
                            weapon = self.weapon_,
                            parameters = self.weapon_parameters_
                          })

            laser:disable()
            laser:enable()

            laser:setPositionX(laser:getPositionX() + (laser.kinematic_body_size_.width * 0.5 * self:get_sprite_normal().x) - 16)

        end)

        local attack_delay = cc.DelayTime:create(2)

        local attack_end_callback = cc.CallFunc:create(self.on_cannon_attack_end)

        local sequence = cc.Sequence:create(locate_player_y,
                                            animate_attack,
                                            fire_callback,
                                            attack_delay,
                                            attack_end_callback,
                                            nil)

        self:runAction(sequence)

    end
end

--function tremor:step(dt)
--    self:kinematic_step(dt)
--    self.current_speed_.y = 0
--end

return tremor