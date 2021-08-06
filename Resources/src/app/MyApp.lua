-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local MyApp = class("MyApp", cc.load("mvc").AppBase)

function MyApp:onCreate()
    math.randomseed(os.time())
    self:setup()
end

function MyApp:setup()
    self:initiate()

    --cc.lite_edition_ = true

    
    if device.platform ~= "android" and device.platform ~= "ios" then

        display.setAutoScale({
            autoscale = "SHOW_ALL", 
            width = 256, 
            height = 224
        }, 
        {
            width = display.width, 
            height = display.height
        })

        cc.platform_ = "desktop"
    elseif device.platform == "winrt" then
        display.setAutoScale({
            autoscale = "SHOW_ALL", 
            width = 256, 
            height = 224
        }, 
        {
            width = display.width, 
            height = display.height
        })

        cc.platform_ = "console"
    else
        display.setAutoScale({
            autoscale = "SHOW_ALL", 
            width = 426, 
            height = 224
        }, 
        {
            width = display.width, 
            height = display.height
        })
        cc.platform_ = "mobile"
    end
end

cc.exports.UnanchoredPosition = function(newAnchorPoint, sprite)
    local rect = sprite:getContentSize()
    local oldAnchorPoint = sprite:getAnchorPoint()
    local scaleX = sprite:getScaleX()
    local scaleY = sprite:getScaleY()

    local newPoint = cc.p(rect.width * newAnchorPoint.x * scaleX, rect.height * newAnchorPoint.y * scaleY)
    local oldPoint = cc.p(rect.width * oldAnchorPoint.x * scaleX, rect.height * oldAnchorPoint.y * scaleY)

    local position = cc.p(sprite:getPosition())

    position.x = position.x - oldPoint.x
    position.x = position.x + newPoint.x

    position.y = position.y - oldPoint.y
    position.y = position.y + newPoint.y

    return position;
end

function cc.key_down(key)
    return cc.keys_[key].status_ == cc.KEY_STATUS.DOWN
end

function cc.key_pressed(key)
    return cc.keys_[key].pressed_
end

function cc.pause(freeze)
    if freeze then
        cc.game_status_ = cc.GAME_STATUS.PAUSED
        print('paused')
    else
        cc.game_status_ = cc.GAME_STATUS.RUNNING
        print('running')
    end
end

function MyApp:initiate()

    self:setup_application()
    self:setup_tags()
    self:setup_callbacks()
    self:setup_keyboard()
    self:setup_camera()
    self:setup_player()
    self:setup_enemy()
    self:setup_special()
    self:setup_battle()
    self:setup_items()
    self:setup_levels()
    self:setup_browners()

end

function MyApp:setup_callbacks()

    cc.callbacks_ = {}

    function cc.callbacks_.pre_fill()
        cc.pause(true)
        cc.audio.play_sfx("sounds/sfx_getenergy.mp3", false)
    end

    function cc.callbacks_.post_fill()
        cc.pause(false)
        cc.fill_amount_ = 0
        if cc.fill_object_.resume_actions then
            cc.fill_object_:resume_actions()
        end
        cc.sender_:stopAllActions()
        if cc.on_post_fill_callback_ ~= nil then
           cc:on_post_fill_callback_()
        end
    end

    function cc.callbacks_.fill_health_()
        if cc.fill_amount_ > 0 and cc.fill_object_.health_ <= 28 then
            cc.callbacks_.pre_fill()
            cc.fill_object_.health_ = cc.fill_object_.health_ + 1
            cc.fill_amount_ = cc.fill_amount_ - 1
        else
            cc.callbacks_.post_fill()
        end
    end

    function cc.callbacks_.fill_energy_()
        if cc.fill_object_.energy_ ~= nil then
            if cc.fill_amount_ > 0 and cc.fill_object_.energy_ < 28 then
                cc.callbacks_.pre_fill()
                cc.fill_object_.energy_ = cc.fill_object_.energy_ + 1
                cc.fill_amount_ = cc.fill_amount_ - 1
            else
                cc.callbacks_.post_fill()
            end
        else
            cc.callbacks_.post_fill()
        end
    end

    function cc.callbacks_.energy_fill(sender, object, amount, property, on_post_fill_callback)
        cc.fill_amount_ = amount
        cc.fill_object_ = object

        if cc.fill_object_.pause_actions then
            cc.fill_object_:pause_actions()
        end

        cc.sender_ = sender

        cc.on_post_fill_callback_ = on_post_fill_callback

        local fill_callback = cc.callbacks_.fill_health_

        if property.energy_ then
            fill_callback = cc.callbacks_.fill_energy_
        end

        local delay = cc.DelayTime:create(0.06)
        local fill_callback = cc.CallFunc:create(fill_callback)

        local fill_sequence = cc.Sequence:create(delay, fill_callback, nil)

        local sequence = cc.RepeatForever:create(fill_sequence)

        cc.sender_:runAction(sequence)
    end
end

function MyApp:setup_application()

    cc.joypad_ = {}
    
    cc.texture_formats_ = {}
    cc.texture_formats_.pvr_ = 0
    cc.texture_formats_.png_ = 1

    cc.texture_format_ = cc.texture_formats_.pvr_


    cc.GAME_STATUS = {}
    cc.GAME_STATUS.PAUSED = 0
    cc.GAME_STATUS.RUNNING = 1

    cc.PAUSE_STATUS = {}
    cc.PAUSE_STATUS.NONE    = 0
    cc.PAUSE_STATUS.SCREEN  = 1

    cc.game_status_  = cc.GAME_STATUS.RUNNING
    cc.pause_status_ = cc.PAUSE_STATUS.NONE

    cc.frames_cache_ = {}
    cc.animations_cache_ = {}

    cc.is_boss_area_ = false
end

function MyApp:setup_tags()

    cc.tags = {}
    cc.tags.none         = -1
    cc.tags.player       = 1
    cc.tags.item         = 2
    cc.tags.enemy        = 3
    cc.tags.block        = 4
    cc.tags.camera       = 5
    cc.tags.scroll       = 6
    cc.tags.check_point  = 7
    cc.tags.teleporter   = 8
    cc.tags.bounds       = 9
    cc.tags.hole         = 10
    cc.tags.door         = 11
    cc.tags.ladder       = 12
    cc.tags.weapon = {}
    cc.tags.weapon.player = 13
    cc.tags.weapon.enemy  = 14
    cc.tags.weapon.none   = 15

    cc.tags.logic = {}
    cc.tags.logic.check_point = {}
    cc.tags.logic.check_point.first_ = 16
    cc.tags.logic.check_point.last_ = 17

    cc.tags.actions = {}
    cc.tags.actions.animation   = 18
    cc.tags.actions.color       = 19

    cc.tags.free_scroll       = 20

    cc.tags.actions.visibility  = 21



    cc.level_status_ = {}
    cc.level_status_.init_ = 1
    cc.level_status_.run_  = 2
end

function MyApp:setup_keyboard()

    cc.KEY_STATUS       = {}
    cc.KEY_STATUS.UP    = 0
    cc.KEY_STATUS.DOWN  = 1

    cc.key_code_ = {}
    cc.key_code_.a      = 1
    cc.key_code_.b      = 2
    cc.key_code_.start  = 3
    cc.key_code_.up     = 4
    cc.key_code_.down   = 5
    cc.key_code_.left   = 6
    cc.key_code_.right  = 7
    cc.key_code_.lb  = 8
    cc.key_code_.rb  = 9


    cc.keys_ = {}

    local key_list = {cc.key_code_.a,
                      cc.key_code_.b,
                      cc.key_code_.start,
                      cc.key_code_.up, 
                      cc.key_code_.down, 
                      cc.key_code_.left, 
                      cc.key_code_.right,
                      cc.key_code_.lb,
                      cc.key_code_.rb}

    for i = 1, #key_list do
        local key = {}
        key.status_ = cc.KEY_STATUS.UP
        key.pressed_ = false
        key.released_ = false

        cc.keys_[i] = key
    end


    cc.delayed_keys_ = {}

    for i = 1, #key_list do
        local key = {}
        key.status_ = cc.KEY_STATUS.UP
        key.pressed_ = false
        key.released_ = false

        cc.delayed_keys_[i] = key
    end


end

function MyApp:setup_camera()
    cc.camera_ = {}
    cc.CAMERA = {}
    cc.CAMERA.MODE = {}
    cc.CAMERA.SCROLL = {}
    cc.CAMERA.SHIFT  = {}

    cc.CAMERA.MODE.SCREEN = 1
    cc.CAMERA.MODE.SCROLL = 2
    cc.CAMERA.MODE.SHIFT  = 3

    cc.CAMERA.SCROLL.UP     = 1
    cc.CAMERA.SCROLL.DOWN   = 2
    cc.CAMERA.SCROLL.LEFT   = 3
    cc.CAMERA.SCROLL.RIGHT  = 4
    cc.CAMERA.SCROLL.MOVING = 5
    cc.CAMERA.SCROLL.NONE   = 6


    cc.CAMERA.SHIFT.UP      = 1
    cc.CAMERA.SHIFT.DOWN    = 2
    cc.CAMERA.SHIFT.LEFT    = 3
    cc.CAMERA.SHIFT.RIGHT   = 4
    cc.CAMERA.SHIFT.NONE    = 5
end

function MyApp:setup_player()

    cc.player_ = {}
    cc.player_.climb_direction_ = {}
    cc.player_.climb_direction_.up_ = 0
    cc.player_.climb_direction_.down_ = 1
    cc.player_.climb_direction_.none_ = 2
    cc.player_.lives_ = 3

    cc.player_.e_tanks_ = 0
    cc.player_.m_tanks_ = 0

    cc.unlockables_ = {}

    cc.unlockables_.items_ = {}

    cc.unlockables_.items_.helmet_ = {id_ = 2, acquired_ = true, name_ = "helmet" }
    cc.unlockables_.items_.head_   = {id_ = 3, acquired_ = false, name_ = "head" }
    cc.unlockables_.items_.chest_  = {id_ = 4, acquired_ = false, name_ = "chest" }
    cc.unlockables_.items_.fist_   = {id_ = 5, acquired_ = false, name_ = "fist" }
    cc.unlockables_.items_.boot_   = {id_ = 6, acquired_ = false, name_ = "boot" }

    cc.unlockables_.helmet_acquired_ = function()
        return cc.unlockables_.items_.helmet_.acquired_
    end

    cc.unlockables_.extreme_acquired_ = function()
        local acquired = true

        for _, unlockable in pairs(cc.unlockables_.items_) do
            if not unlockable.acquired_ then
               acquired = false
            end
        end

        return acquired

    end

    cc.game_options_ = {}

    cc.game_options_.extreme_activated_ = false
    cc.game_options_.helmet_activated_  = false

    cc.kinematic_contact_ = {}
    cc.kinematic_contact_.up    = 1
    cc.kinematic_contact_.down  = 2
    cc.kinematic_contact_.left  = 3
    cc.kinematic_contact_.right = 4

end

function MyApp:setup_enemy()
    cc.boss_ = {}

    cc.enemy_ = {}
    cc.enemy_.status_ = {}

    cc.enemy_.status_.preparing_   = 1
    cc.enemy_.status_.active_   = 2
    cc.enemy_.status_.fighting_ = 3
    cc.enemy_.status_.defeated_ = 4
    cc.enemy_.status_.inactive_ = 5

end

function MyApp:setup_special()
    cc.special_ = {}
    cc.special_.status_ = {}

    cc.special_.status_.on_screen_   = 1
    cc.special_.status_.off_screen_  = 2

end

function MyApp:setup_battle()
    cc.battle_status_ = {}
    cc.battle_status_.waiting_  = 1
    cc.battle_status_.startup_  = 2
    cc.battle_status_.intro_    = 3
    cc.battle_status_.fighting_ = 4
    cc.battle_status_.defeated_ = 5

end

function MyApp:setup_items()

    cc.item_                 = {}

    cc.item_.collectibles_ = {}

    cc.item_.life_            = {id_ = 1,  string_ = "life"   }

    cc.item_.helmet_          = {id_ = 2,  string_ = "helmet" }
    cc.item_.head_            = {id_ = 3,  string_ = "head"   }
    cc.item_.chest_           = {id_ = 4,  string_ = "chest"  }
    cc.item_.fist_            = {id_ = 5,  string_ = "fist"   }
    cc.item_.boot_            = {id_ = 6,  string_ = "boot"   }

    cc.item_.health_small_    = {id_ = 7,  string_ = "health_small" }
    cc.item_.health_big_      = {id_ = 8,  string_ = "health_big"   }

    cc.item_.energy_small_    = {id_ = 9,  string_ = "energy_small" }
    cc.item_.energy_big_      = {id_ = 10, string_ = "energy_big" }

    cc.item_.e_tank_          = {id_ = 11,  string_ = "e_tank" }
    cc.item_.m_tank_          = {id_ = 12,  string_ = "m_tank" }



    local on_item_acquired = function(player, item)

        if item:get_collectible() then
            cc.item_.collectibles_[#cc.item_.collectibles_ + 1] = item:get_name()
            local slot = cc.game.get_default_slot()
            slot["collectible_items"] = cc.item_.collectibles_
            cc.game.save_default_slot(slot)
        end

        if item.id_ == cc.item_.life_.id_ then

            if cc.player_.lives_ < 9 then
                cc.player_.lives_ = cc.player_.lives_ + 1
            end

            
            local slot = cc.game.get_default_slot()

            slot["lives"] = cc.player_.lives_

            cc.game.save_default_slot(slot)


            cc.audio.play_sfx("sounds/sfx_getlife.mp3", false)

        elseif item.id_ >= cc.item_.helmet_.id_ and item.id_ <= cc.item_.boot_.id_ then
            local this_unlockable = nil
            for _, unlockable in pairs(cc.unlockables_.items_) do
                if item.id_ == unlockable.id_ then
                    this_unlockable = unlockable
                    unlockable.acquired_ = true
                end
            end

            local slot = cc.game.get_default_slot()

            slot[this_unlockable.name_] = true

            if item.id_ == cc.item_.head_.id_ then
                cc.unlockables_.items_.helmet_.acquired_ = true
                cc.browners_.helmet_.acquired_ = true
                slot["helmet"] = true
            end

            local extreme_acquired = true

            for _, unlockable in pairs(cc.unlockables_.items_) do
                if not unlockable.acquired_ then
                    extreme_acquired = false
                end
            end
            
            if extreme_acquired then
                cc.browners_.extreme_.acquired_ = true
            end

            cc.game.save_default_slot(slot)

            cc.audio.play_sfx("sounds/sfx_getlife.mp3", false)

        elseif item.id_ == cc.item_.e_tank_.id_ then
            if cc.player_.e_tanks_ < 9 then
                cc.player_.e_tanks_ = cc.player_.e_tanks_ + 1
            end

            local slot = cc.game.get_default_slot()

            slot["e"] = cc.player_.e_tanks_

            cc.game.save_default_slot(slot)

            cc.audio.play_sfx("sounds/sfx_getlife.mp3", false)

        elseif item.id_ == cc.item_.m_tank_.id_ then
            if cc.player_.m_tanks_ < 9 then
                cc.player_.m_tanks_ = cc.player_.m_tanks_ + 1
            end

            local slot = cc.game.get_default_slot()

            slot["m"] = cc.player_.m_tanks_

            cc.game.save_default_slot(slot)

            cc.audio.play_sfx("sounds/sfx_getlife.mp3", false)

        else
            player:restore_sanity(item)
        end
    end

    for _, item in pairs(cc.item_) do
       item.callback_ = on_item_acquired
    end

end

function MyApp:setup_levels()

    cc.levels_ = {}

    local level_mugs = {}

    level_mugs[1] = "freezerman"
    level_mugs[2] = "sheriffman"
    level_mugs[3] = "boomerman"
    level_mugs[4] = "militaryman"
    level_mugs[5] = "vineman"
    level_mugs[6] = "shieldman"
    level_mugs[7] = "nightman"
    level_mugs[8] = "torchman"
    level_mugs[9] = "test"
    level_mugs[10] = "sheriffmantest"
    level_mugs[11] = "militarymantest"
    level_mugs[12] = "nightmantest"
    level_mugs[13] = "vinemantest"
    

    for i = 1, #level_mugs do
        local level_map = {}
        level_map.mug_ = level_mugs[i]
        level_map.defeated_ = false
        cc.levels_[#cc.levels_ + 1] = level_map
    end

    cc.current_level_ = nil

    cc.demo_ = {}
    cc.demo_.level_      = 1
    cc.demo_.get_weapon_ = 2

    cc.is_level_clear_ = false

end

function MyApp:setup_browners()

    cc.browners_ = {
        teleport_   = {id_ = 1,  acquired_ = true,  pause_item_ = nil},
        violet_     = {id_ = 2,  acquired_ = true,  pause_item_ = "violet"},
        fuzzy_      = {id_ = 3,  acquired_ = false,  pause_item_ = "fuzzy"},
        freezer_    = {id_ = 4,  acquired_ = false, pause_item_ = "freezer"},
        sheriff_    = {id_ = 5,  acquired_ = false, pause_item_ = "sheriff"},
        boomer_     = {id_ = 6,  acquired_ = false, pause_item_ = "boomer"},
        military_   = {id_ = 7,  acquired_ = false, pause_item_ = "military"},
        vine_       = {id_ = 8,  acquired_ = false, pause_item_ = "vine"},
        shield_     = {id_ = 9,  acquired_ = false, pause_item_ = "shield"},
        night_      = {id_ = 10, acquired_ = false, pause_item_ = "night"},
        torch_      = {id_ = 11, acquired_ = false, pause_item_ = "torch"},
        helmet_     = {id_ = 12, acquired_ = false, pause_item_ = "helmet"},
        extreme_    = {id_ = 13, acquired_ = false, pause_item_ = "ex"},
        boss_       = {id_ = 14, acquired_ = nil, pause_item_ = nil },
        sub_boss_   = {id_ = 15, acquired_ = nil, pause_item_ = nil },
        newnightmanboss_ = {id_ = 16,  acquired_ = false,  pause_item_ = nil}
    }

    for _, v in pairs(cc.browners_) do
        if v.id_ >= 4 and v.id_ <= 11 then
           v.level_ = cc.levels_[v.id_ - 3].mug_
           v.energy_ = 28
        end
    end

    cc.browners_.violet_.energy_    = -1
    cc.browners_.helmet_.energy_    = -1
    cc.browners_.fuzzy_.energy_     = 28
    cc.browners_.extreme_.energy_   = 28
end

return MyApp
