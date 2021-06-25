-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local special   = import("app.core.physics.kinematic_character").create("falling_block")

function special:onCreate(args)
    self.player_contact_ = false
    self.movement_is_non_blockable_ = true

    self.time_to_fall_ = 1.4
    self.falling_ = false
    self.start_position_ = args.real_position_
    self:setPosition(self.start_position_)
    self.status_ = cc.special_.status_.on_screen_

    self.reappearing_ = false
    self.appeared_ = false
end

function special:animate(cname)
    self.start_bbox_ = self.sprite_:getBoundingBox()
    local real_position = self:convertToWorldSpace(cc.p(self.start_bbox_.x, self.start_bbox_.y))

    self.start_bbox_.x = real_position.x
    self.start_bbox_.y = real_position.y
end

function special:solve_collisions()
    local collisions = self.kinematic_body_:get_collisions()

    for _, collision in pairs(collisions) do
        if collision:getPhysicsBody():getShapes()[1]:getTag() == cc.tags.player then
            if not self.player_contact_ then
                self.player_contact_ = true
                local delay = cc.DelayTime:create(self.time_to_fall_)

                local callback = cc.CallFunc:create(function()
                    self.falling_ = true
                    
                    self.sprite_:stopAllActionsByTag(cc.tags.actions.visibility)

                    if not self:isVisible() then
                        self:setVisible(true)
                    end

                    if not self.sprite_:isVisible() then
                        self.sprite_:setVisible(true)
                    end

                end)

                local sequence = cc.Sequence:create(delay, callback, nil)

                self:runAction(sequence)

            end
        end
    end
end

function special:reappear()

    self:setPosition(self.start_position_)


    local blink = cc.Blink:create(1, 2)

    local blink_callback = cc.CallFunc:create(function()
        if not self:isVisible() then
            self:setVisible(true)
        end

        if not self.sprite_:isVisible() then
            self.sprite_:setVisible(true)
        end

        self.kinematic_body_.current_shape_:setTag(cc.tags.block)

    end)

    local sequence = cc.Sequence:create(blink, blink_callback, nil)

    sequence:setTag(cc.tags.actions.visibility)
    self.sprite_:stopAllActionsByTag(cc.tags.actions.visibility)
    self.sprite_:runAction(sequence)
end

function special:offscreen()

    if self.reappearing_ then
        self.reappearing_ = false
        self:reappear()
    end

    if self.status_ == cc.special_.status_.on_screen_ then
        if not cc.bounds_:is_rect_inside(self.start_bbox_) then
            self.sprite_:setVisible(true)
            self:setPosition(self.start_position_)
            self.status_ = cc.special_.status_.off_screen_
            self.player_contact_ = false
            self.falling_ = false
            self.current_speed_.y = 0
            self.appeared_ = false
        end
    end

    if self.status_ == cc.special_.status_.on_screen_ then
        print("onscreen - offscreen switch")
        self.status_ = cc.special_.status_.off_screen_
        self.player_contact_ = false
        self.falling_ = false
        self.current_speed_.y = 0

        self.sprite_:setVisible(false)

        self.kinematic_body_.current_shape_:setTag(cc.tags.none)


        if self.appeared_ then
            local delay = cc.DelayTime:create(2)
            local callback = cc.CallFunc:create(function()
                self.reappearing_ = true
                self.status_ = cc.special_.status_.on_screen_
                print("reappearing")
            end)

            local sequence = cc.Sequence:create(delay, callback, nil)

            self:runAction(sequence)
        end
    end
end


function special:jump() -- override in children
    self:solve_collisions()

    if not self.falling_ then
        self.current_speed_.y = 0
    end

    local bbox = self.sprite_:getBoundingBox()
    local real_position = self:convertToWorldSpace(cc.p(bbox.x, bbox.y))

    bbox.x = real_position.x
    bbox.y = real_position.y

    if cc.bounds_:is_rect_inside(bbox) then
        if self.status_ == cc.special_.status_.off_screen_ then
            self.status_ = cc.special_.status_.on_screen_
            if not self.appeared_ then
                self.appeared_ = true
            end
        end
    end
end

return special



