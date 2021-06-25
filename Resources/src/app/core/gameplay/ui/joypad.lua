--
-- Created by Victor on 9/22/2015 3:04 AM
--

local joypad = class("joypad", cc.Node)

require("cocos.controller.ControllerConstants")

function joypad:ctor(layout)
    cc.joypad_ = self

    self.take_inputs_ = true

    local platform = device.platform

    local win_debug = false

    if platform == "windows" or platform == "mac" and not win_debug then
        local function onKeyPressed(keycode, event)
            self:onKeypad(keycode, true)
        end

        local function onKeyReleased(keycode, event)
            self:onKeypad(keycode, false)
        end

        self.joy_keys_ = {}

        self.joy_keys_[#self.joy_keys_ + 1] = cc.key_code_.right
        self.joy_keys_[#self.joy_keys_ + 1] = cc.key_code_.left
        self.joy_keys_[#self.joy_keys_ + 1] = cc.key_code_.up
        self.joy_keys_[#self.joy_keys_ + 1] = cc.key_code_.down


        local listener = cc.EventListenerKeyboard:create()
        listener:registerScriptHandler(onKeyPressed, cc.Handler.EVENT_KEYBOARD_PRESSED)
        listener:registerScriptHandler(onKeyReleased, cc.Handler.EVENT_KEYBOARD_RELEASED)
        layout:getScene():getEventDispatcher():addEventListenerWithSceneGraphPriority(listener, layout:getScene())
    elseif platform == "winrt" then
        --print("winrt")

        local director = cc.Director:getInstance()
        local view = director:getOpenGLView()

        view:setCursorVisible(false)

        local function onControllerKeyPressed(controller, keycode)
            self:onControllerPressed(controller, keycode, true)
        end

        local function onControllerKeyReleased(controller, keycode)
            self:onControllerPressed(controller, keycode, false)
        end
        
        local function onAxis(controller, keycode, event)
            
            --print("On Axis")
            --print("Keycode " .. tostring(keycode))
            --print(controller:getKeyStatus(keycode).value)


            --[[
            if keycode == 214 or keycode == 213 then
                print("Left X")
                print(controller:getKeyStatus(keycode).value)
            end

            if keycode == 211 or keycode == 212 then
                print("Left Y")
                print(controller:getKeyStatus(keycode).value)
            end
            ]]

            --print("Left X")
            --print(controller:getKeyStatus(cc.ControllerKey.JOYSTICK_LEFT_X).value)

            --print("Left Y")
            --print(controller:getKeyStatus(cc.ControllerKey.JOYSTICK_LEFT_Y).value)

            --if keycode == cc.ControllerKey.JOYSTICK_LEFT_X or keycode == cc.ControllerKey.JOYSTICK_LEFT_Y then
                self:onAxis(controller:getKeyStatus(cc.ControllerKey.JOYSTICK_LEFT_X).value,
                            controller:getKeyStatus(cc.ControllerKey.JOYSTICK_LEFT_Y).value)
            --end
        end

        self.dead_zone_ = 0.25


        self.joy_keys_ = {}

        self.joy_keys_[#self.joy_keys_ + 1] = cc.key_code_.right
        self.joy_keys_[#self.joy_keys_ + 1] = cc.key_code_.left
        self.joy_keys_[#self.joy_keys_ + 1] = cc.key_code_.up
        self.joy_keys_[#self.joy_keys_ + 1] = cc.key_code_.down

        local controllerListener = cc.EventListenerController:create()
        controllerListener:registerScriptHandler(onControllerKeyPressed, cc.Handler.EVENT_CONTROLLER_KEYDOWN)
        controllerListener:registerScriptHandler(onControllerKeyReleased, cc.Handler.EVENT_CONTROLLER_KEYUP)
        controllerListener:registerScriptHandler(onAxis, cc.Handler.EVENT_CONTROLLER_AXIS)
        layout:getScene():getEventDispatcher():addEventListenerWithSceneGraphPriority(controllerListener, layout:getScene())

        local function onKeyPressed(keycode, event)
            self:onKeypad(keycode, true)
        end

        local function onKeyReleased(keycode, event)
            self:onKeypad(keycode, false)
        end

        local listener = cc.EventListenerKeyboard:create()
        listener:registerScriptHandler(onKeyPressed, cc.Handler.EVENT_KEYBOARD_PRESSED)
        listener:registerScriptHandler(onKeyReleased, cc.Handler.EVENT_KEYBOARD_RELEASED)
        layout:getScene():getEventDispatcher():addEventListenerWithSceneGraphPriority(listener, layout:getScene())

    else
        local tex_path = "sprites/core/joystick"

        self.ring_ = ccui.Button:create()
        self.ring_:setTouchEnabled(false)
        self.ring_:loadTextures(tex_path.."/vjoy_ring.png", tex_path.."/vjoy_ring.png", "")
        self.ring_:addTo(self)

        self.ring_:setPositionX(self.ring_:getPositionX() - 85 / 2)
        self.ring_:setPositionY(self.ring_:getPositionY() + self.ring_:getContentSize().height * 0.5)

        self.circle_ = ccui.Button:create()
        self.circle_:setTouchEnabled(true)
        self.circle_:setSwallowTouches(false)
        self.circle_:loadTextures(tex_path.."/vjoy_circle.png", tex_path.."/vjoy_circle.png", "")
        self.circle_:setPosition(cc.p(self.ring_:getPositionX(),
                                      self.ring_:getPositionY()))
        self.circle_:setPressedActionEnabled(true)
        self.circle_:onTouch(function(event) self:onJoystick(event) end)
        self.circle_:addTo(self)

        self.start_ = ccui.Button:create()
        self.start_:setTouchEnabled(true)
        self.start_:loadTextures(tex_path.."/start_button.png", tex_path.."/start_button.png", "")
        self.start_:setPosition(cc.p(display.right_top.x - 85 * 2 + 85 * 0.5 - self.start_:getContentSize().width * 0.5,
                                     display.right_top.y - self.start_:getContentSize().height))
        self.start_:setPressedActionEnabled(true)
        self.start_:onTouch(function(event) self:onButton(event) end)
        self.start_:addTo(self)

        self.a_ = ccui.Button:create()
        self.a_:setTouchEnabled(true)
        self.a_:loadTextures(tex_path.."/a_button.png", tex_path.."/a_button.png", "")
        self.a_:setPosition(cc.p(display.right_bottom.x - 85 * 2 + 85 * 0.5 - self.start_:getContentSize().width * 0.5,
                                 display.right_bottom.y + self.start_:getContentSize().height * 0.5))
        self.a_:setPressedActionEnabled(true)
        self.a_:onTouch(function(event) self:onButton(event) end)
        self.a_:addTo(self)

        self.b_ = ccui.Button:create()
        self.b_:setTouchEnabled(true)
        self.b_:loadTextures(tex_path.."/b_button.png", tex_path.."/b_button.png", "")
        self.b_:setPosition(cc.p(self.a_:getPositionX(), self.a_:getPositionY() + self.b_:getContentSize().height))
        self.b_:setPressedActionEnabled(true)
        self.b_:onTouch(function(event) self:onButton(event) end)
        self.b_:addTo(self)

        local function onTouchBegan(touch, event)
            self:onTouchBegan(touch, event)
            return true
        end

        local function onTouchMoved(touch, event)
            self:onTouchMoved(touch, event)
        end


        if win_debug then
            local listener = cc.EventListenerMouse:create()
            listener:registerScriptHandler(onTouchBegan, cc.Handler.EVENT_MOUSE_DOWN)
            listener:registerScriptHandler(onTouchMoved, cc.Handler.EVENT_MOUSE_MOVE)
            layout:getScene():getEventDispatcher():addEventListenerWithSceneGraphPriority(listener, layout:getScene())
        else
            local listener = cc.EventListenerTouchOneByOne:create()
            listener:setSwallowTouches(false)
            listener:registerScriptHandler(onTouchBegan, cc.Handler.EVENT_TOUCH_BEGAN)
            listener:registerScriptHandler(onTouchMoved, cc.Handler.EVENT_TOUCH_MOVED)
            layout:getScene():getEventDispatcher():addEventListenerWithSceneGraphPriority(listener, layout:getScene())
        end

        self.moving_joystick_ = false
        self.in_horizontal_range_ = false
        self.in_vertical_range_ = false
        self.can_move_ = false
        self.dead_zone_ = 5
        self.horizontal_range_key_ = cc.key_code_.none
        self.vertical_range_key_ = cc.key_code_.none

        self.joy_keys_ = {}

        self.joy_keys_[#self.joy_keys_ + 1] = cc.key_code_.right
        self.joy_keys_[#self.joy_keys_ + 1] = cc.key_code_.left
        self.joy_keys_[#self.joy_keys_ + 1] = cc.key_code_.up
        self.joy_keys_[#self.joy_keys_ + 1] = cc.key_code_.down

    end
end


function joypad:onControllerPressed(controller, keycode, keydown)
    print("Controller Key pressed")
    print(keycode)


    local key = keycode

    local translated_key
    if key == 1 then
        translated_key = cc.key_code_.start
    elseif key == 64 then
        translated_key = cc.key_code_.up
    elseif key == 128 then
        translated_key = cc.key_code_.down
    elseif key == 256 then
        translated_key = cc.key_code_.left
    elseif key == 512 then
        translated_key = cc.key_code_.right
    elseif key == 4 or key == 16 then
        translated_key = cc.key_code_.a
    elseif key == 8 or key == 32 then
        translated_key = cc.key_code_.b
    elseif key == 1024  then
        translated_key = cc.key_code_.lb
    elseif key == 2048 then
        translated_key = cc.key_code_.rb
    end

    if translated_key ~= nil then
        if keydown then
            cc.delayed_keys_[translated_key].status_  = cc.KEY_STATUS.DOWN
            cc.delayed_keys_[translated_key].pressed_ = true
        else
            cc.delayed_keys_[translated_key].status_ = cc.KEY_STATUS.UP
            cc.delayed_keys_[translated_key].released_ = true
        end

    end        


    if not self.take_inputs_ then
        return
    end

    if translated_key ~= nil then
        if keydown then
            cc.keys_[translated_key].status_  = cc.KEY_STATUS.DOWN
            cc.keys_[translated_key].pressed_ = true
        else
            cc.keys_[translated_key].status_ = cc.KEY_STATUS.UP
            cc.keys_[translated_key].released_ = true
        end

    end        
end


function joypad:onTouchBegan(touch, event)
    self.previous_touch_ = touch:getLocation()
end

function joypad:onTouchMoved(touch, event)

    if self.moving_joystick_ then

        local x_move = touch:getLocation().x - self.previous_touch_.x
        local y_move = touch:getLocation().y - self.previous_touch_.y

        local ring_position = cc.p(self.ring_:getPositionX(), self.ring_:getPositionY())
        local new_position  = cc.p(self.circle_:getPositionX() + x_move, self.circle_:getPositionY() + y_move)

        local circle_distance = cc.pGetDistance(new_position, ring_position)

        if circle_distance <= self.ring_:getContentSize().width * 0.5 then
            self.circle_:setPosition(new_position)
        end

        if circle_distance >= self.dead_zone_ then
            self.can_move_ = true
        else
            self.can_move_ = false
        end

    end

    self.previous_touch_ = touch:getLocation()
end

function joypad:onTouchEnded(touch, event)

end

function joypad:onAxis(axis_x, axis_y)

    if not self.take_inputs_ then
        return
    end

    local circle_distance = cc.pGetDistance(cc.p(axis_x, axis_y), cc.p(0, 0))

    if circle_distance >= self.dead_zone_ then
        self.can_move_ = true
    else
        
        if self.can_move_ then
            self.in_horizontal_range_ = false
            self.in_vertical_range_ = false

            for i = 1, #self.joy_keys_ do
                cc.keys_[self.joy_keys_[i]].status_    = cc.KEY_STATUS.UP
                if cc.keys_[self.joy_keys_[i]].pressed_ then
                    cc.keys_[self.joy_keys_[i]].pressed_ = false
                    cc.keys_[self.joy_keys_[i]].released_ = true
                end
            end
        end

        self.can_move_ = false
    end


    local delta_x = axis_x
    local delta_y = axis_y

    local angle = math.atan2(delta_y, delta_x) * 180 / math.pi
    
    --print("circle_distance")
    --print(circle_distance)

    if self.can_move_ then

        if angle >= -60 and angle <= 60 then
            if not self.in_horizontal_range_ then
                cc.keys_[cc.key_code_.right].status_  = cc.KEY_STATUS.DOWN
                cc.keys_[cc.key_code_.right].pressed_ = true
                self.in_horizontal_range_ = true
                self.horizontal_range_key_ = cc.key_code_.right
            end
        else
            cc.keys_[cc.key_code_.right].status_ = cc.KEY_STATUS.UP
            cc.keys_[cc.key_code_.right].released_ = true
            if self.horizontal_range_key_ == cc.key_code_.right then
                self.in_horizontal_range_ = false
            end
        end

        if math.abs(angle) >= 120 and math.abs(angle) <= 180 then
            if not self.in_horizontal_range_ then
                cc.keys_[cc.key_code_.left].status_  = cc.KEY_STATUS.DOWN
                cc.keys_[cc.key_code_.left].pressed_ = true
                self.in_horizontal_range_ = true
                self.horizontal_range_key_ = cc.key_code_.left
            end
        else
            cc.keys_[cc.key_code_.left].status_ = cc.KEY_STATUS.UP
            cc.keys_[cc.key_code_.left].released_ = true
            if self.horizontal_range_key_ == cc.key_code_.left then
                self.in_horizontal_range_ = false
            end
        end

        if angle >= 30 and angle <= 150 then
            if not self.in_vertical_range_ then
                cc.keys_[cc.key_code_.up].status_  = cc.KEY_STATUS.DOWN
                cc.keys_[cc.key_code_.up].pressed_ = true
                self.in_vertical_range_ = true
                self.vertical_range_key_ = cc.key_code_.up
            end
        else
            cc.keys_[cc.key_code_.up].status_ = cc.KEY_STATUS.UP
            cc.keys_[cc.key_code_.up].released_ = true
            if self.vertical_range_key_ == cc.key_code_.up then
                self.in_vertical_range_ = false
            end
        end

        if angle <= -30 and angle >= -150 then
            if not self.in_vertical_range_ then
                cc.keys_[cc.key_code_.down].status_  = cc.KEY_STATUS.DOWN
                cc.keys_[cc.key_code_.down].pressed_ = true
                self.in_vertical_range_ = true
                self.vertical_range_key_ = cc.key_code_.down
            end
        else
            cc.keys_[cc.key_code_.down].status_ = cc.KEY_STATUS.UP
            cc.keys_[cc.key_code_.down].released_ = true
            if self.vertical_range_key_ == cc.key_code_.down then
                self.in_vertical_range_ = false
            end
        end


    end
end

function joypad:onJoystick(event)
    
    if event.name == "began" then
        self.moving_joystick_ = true
    end

    if self.take_inputs_ then

        if event.name == "moved" then

            local delta_x = self.circle_:getPositionX() - self.ring_:getPositionX()
            local delta_y = self.circle_:getPositionY() - self.ring_:getPositionY()

            local angle = math.atan2(delta_y, delta_x) * 180 / math.pi

            if self.can_move_ then

                if angle >= -60 and angle <= 60 then
                    if not self.in_horizontal_range_ then
                        cc.keys_[cc.key_code_.right].status_  = cc.KEY_STATUS.DOWN
                        cc.keys_[cc.key_code_.right].pressed_ = true
                        self.in_horizontal_range_ = true
                        self.horizontal_range_key_ = cc.key_code_.right
                    end
                else
                    cc.keys_[cc.key_code_.right].status_ = cc.KEY_STATUS.UP
                    cc.keys_[cc.key_code_.right].released_ = true
                    if self.horizontal_range_key_ == cc.key_code_.right then
                        self.in_horizontal_range_ = false
                    end
                end

                if math.abs(angle) >= 120 and math.abs(angle) <= 180 then
                    if not self.in_horizontal_range_ then
                        cc.keys_[cc.key_code_.left].status_  = cc.KEY_STATUS.DOWN
                        cc.keys_[cc.key_code_.left].pressed_ = true
                        self.in_horizontal_range_ = true
                        self.horizontal_range_key_ = cc.key_code_.left
                    end
                else
                    cc.keys_[cc.key_code_.left].status_ = cc.KEY_STATUS.UP
                    cc.keys_[cc.key_code_.left].released_ = true
                    if self.horizontal_range_key_ == cc.key_code_.left then
                        self.in_horizontal_range_ = false
                    end
                end

                if angle >= 30 and angle <= 150 then
                    if not self.in_vertical_range_ then
                        cc.keys_[cc.key_code_.up].status_  = cc.KEY_STATUS.DOWN
                        cc.keys_[cc.key_code_.up].pressed_ = true
                        self.in_vertical_range_ = true
                        self.vertical_range_key_ = cc.key_code_.up
                    end
                else
                    cc.keys_[cc.key_code_.up].status_ = cc.KEY_STATUS.UP
                    cc.keys_[cc.key_code_.up].released_ = true
                    if self.vertical_range_key_ == cc.key_code_.up then
                        self.in_vertical_range_ = false
                    end
                end

                if angle <= -30 and angle >= -150 then
                    if not self.in_vertical_range_ then
                        cc.keys_[cc.key_code_.down].status_  = cc.KEY_STATUS.DOWN
                        cc.keys_[cc.key_code_.down].pressed_ = true
                        self.in_vertical_range_ = true
                        self.vertical_range_key_ = cc.key_code_.down
                    end
                else
                    cc.keys_[cc.key_code_.down].status_ = cc.KEY_STATUS.UP
                    cc.keys_[cc.key_code_.down].released_ = true
                    if self.vertical_range_key_ == cc.key_code_.down then
                        self.in_vertical_range_ = false
                    end
                end


            end
        end
    end

    if event.name == "ended" or event.name == "cancelled" then
        self.moving_joystick_ = false
        self.circle_:setPosition(cc.p(self.ring_:getPositionX(),
                                      self.ring_:getPositionY()))
        self.in_horizontal_range_ = false
        self.in_vertical_range_ = false
        self.can_move_ = false

        for i = 1, #self.joy_keys_ do
            cc.keys_[self.joy_keys_[i]].status_    = cc.KEY_STATUS.UP
            if cc.keys_[self.joy_keys_[i]].pressed_ then
                cc.keys_[self.joy_keys_[i]].pressed_ = false
                cc.keys_[self.joy_keys_[i]].released_ = true
            end
        end

    end
end

function joypad:onButton(event)

    if not self.take_inputs_ then
        return
    end

    --print("Touch")
    
    local translated_key

    if event.target == self.a_ then
        translated_key = cc.key_code_.a
    elseif event.target == self.b_ then
        translated_key = cc.key_code_.b
    elseif event.target == self.start_ then
        translated_key = cc.key_code_.start
    end

    if translated_key ~= nil then
       if event.name == "began" then
           cc.keys_[translated_key].status_  = cc.KEY_STATUS.DOWN
           cc.keys_[translated_key].pressed_ = true
       elseif event.name == "ended" or event.name == "cancelled" then
           cc.keys_[translated_key].status_ = cc.KEY_STATUS.UP
           cc.keys_[translated_key].released_ = true
       end
    end
end


function joypad:onKeypad(keycode, keydown)

    local key = cc.KeyCodeKey[keycode + 1]
    local translated_key

    if key == "KEY_KP_ENTER" or key == "KEY_ENTER" then
        translated_key = cc.key_code_.start
    elseif key == "KEY_UP_ARROW" then
        translated_key = cc.key_code_.up
    elseif key == "KEY_DOWN_ARROW" then
        translated_key = cc.key_code_.down
    elseif key == "KEY_LEFT_ARROW" then
        translated_key = cc.key_code_.left
    elseif key == "KEY_RIGHT_ARROW" then
        translated_key = cc.key_code_.right
    elseif key == "KEY_Z" then
        translated_key = cc.key_code_.a
    elseif key == "KEY_X" then
        translated_key = cc.key_code_.b
    end

    

    if translated_key ~= nil then
        if keydown then
            cc.delayed_keys_[translated_key].status_  = cc.KEY_STATUS.DOWN
            cc.delayed_keys_[translated_key].pressed_ = true
        else
            cc.delayed_keys_[translated_key].status_ = cc.KEY_STATUS.UP
            cc.delayed_keys_[translated_key].released_ = true
        end

    end


    if not self.take_inputs_ then
        return
    end


    if translated_key ~= nil then
        if keydown then
            cc.keys_[translated_key].status_  = cc.KEY_STATUS.DOWN
            cc.keys_[translated_key].pressed_ = true
        else
            cc.keys_[translated_key].status_ = cc.KEY_STATUS.UP
            cc.keys_[translated_key].released_ = true
        end

    end

end

function joypad:step(dt)
    if not self.take_inputs_ then
        return
    end

    for i = 1, #cc.keys_ do
        if cc.keys_[i].pressed_ then
            cc.keys_[i].pressed_ = false
        end

        if cc.keys_[i].released_ then
            cc.keys_[i].released_ = false
        end
    end

    for i = 1, #cc.delayed_keys_ do
        if cc.delayed_keys_[i].pressed_ then
            cc.delayed_keys_[i].pressed_ = false
        end

        if cc.delayed_keys_[i].released_ then
            cc.delayed_keys_[i].released_ = false
        end
    end


    return self
end

return joypad