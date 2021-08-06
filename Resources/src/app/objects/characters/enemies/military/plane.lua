-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local enemy = import("app.objects.characters.enemies.base.enemy")
local mob   = class("plane", enemy)


function mob:onCreate()
    self.super:onCreate()
    self.movement_is_non_blockable_ = true
    self.default_health_ = 2
    self.shooting_  = false
    self.moving_    = false
    self.orientation_set_ = false

    self.initial_speed_ = 0

    self.status_        = cc.enemy_.status_.preparing_ -- it is not ok for enemies to be in checkpoints

end

function mob:animate(cname)

    local fly = {name = "fly",  animation = {name = cname .. "_" .. "fly", forever = true, delay = 0.10} }

    self.sprite_:load_action(fly, false)
    self.sprite_:set_animation(fly.animation.name)

    return self
end

function mob:onRespawn()
    self.shooting_ = false
    self.moving_    = false
    self.orientation_set_ = false
    self.current_speed_.x = 0

    if self.player_:getPositionX() >= self:getPositionX() then
        self.initial_speed_ = self.walk_speed_
    else
        self.initial_speed_ = -self.walk_speed_
    end

end

function mob:flip(x_normal)
    if not self.orientation_set_ then
        if x_normal == 1 and self.current_speed_.x > 0 then
            self.orientation_set_  = true
            self.sprite_:setFlippedX(true)
        elseif x_normal == -1 and self.current_speed_.x < 0 then
            self.orientation_set_  = true
            self.sprite_:setFlippedX(false)
        end
    end

    self.is_flipping_ = false
end

function mob:onscreen()

    if cc.game_status_ == cc.GAME_STATUS.PAUSED then
        self.moving_ = false
    end

    local bbox = self.sprite_:getBoundingBox()
    local real_position = self:convertToWorldSpace(cc.p(bbox.x, bbox.y))

    bbox.x = real_position.x
    bbox.y = real_position.y

    if cc.bounds_:is_point_inside(self.start_position_) then
        if self.status_ == cc.enemy_.status_.active_ then
           self.sprite_:setVisible(true)
           self.status_ = cc.enemy_.status_.fighting_
           self.health_ = self.default_health_

           self:onRespawn()

        elseif self.status_ == cc.enemy_.status_.defeated_ then
            self.sprite_:stopAllActions()
            self:stopAllActions()
            self:onDefeated()
            self.sprite_:setVisible(false)
            self:setPosition(self.start_position_)
            self.status_ = cc.enemy_.status_.inactive_
        end
    elseif self.status_ == cc.enemy_.status_.defeated_ then
            self.sprite_:stopAllActions()
            self:stopAllActions()
            self:onDefeated()
            self.sprite_:setVisible(false)
            self:setPosition(self.start_position_)
            self.status_ = cc.enemy_.status_.inactive_
    end

end

function mob:offscreen()
    if self.status_ == cc.enemy_.status_.fighting_ or self.status_ == cc.enemy_.status_.preparing_ then
        self:stopAllActions()
        self.sprite_:stopAllActions()

        if not cc.bounds_:is_point_inside(self.start_position_) then
            --self.sprite_:setVisible(true)
            self:setPosition(self.start_position_)
            self.status_ = cc.enemy_.status_.active_
        end

    end

    if self.status_ == cc.enemy_.status_.active_  then
        if not cc.bounds_:is_point_inside(self.start_position_) then
            self.sprite_:setVisible(false)
        else
            self.sprite_:setVisible(true)
        end
    end

    if self.status_ == cc.enemy_.status_.inactive_ or self.status_ == cc.enemy_.status_.defeated_ then
        self.status_ = cc.enemy_.status_.preparing_
    end

end

function mob:walk()
    self.current_speed_.x = self.initial_speed_
end

function mob:jump()
    self.current_speed_.y = 0
end

function mob:attack()

    if self.status_ == cc.enemy_.status_.fighting_ and not self.is_flipping_ and not self.shooting_ then
        -- this enemy throws projectiles
    end

end


return mob



