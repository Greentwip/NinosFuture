-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local save = import("app.core.gameplay.control.layout_base").create("title")

function save:onLoad()
    -- self variables
    if cc.platform_ == "mobile" then
        self:setPositionX(85)
    end
    
    local root = cc.CSLoader:createNode("sprites/gameplay/screens/save/data.csb")
    root:addTo(self)

    self.slot_1_ = root:getChildByName("file_1")
    self.slot_2_ = root:getChildByName("file_2")
    self.slot_3_ = root:getChildByName("file_3")

    self.delete_button_ = root:getChildByName("delete")
    self.delete_confirm_ = root:getChildByName("confirm")

    self.slot_1_customs_ = root:getChildByName("file_1"):getChildByName("customs")
    self.slot_2_customs_ = root:getChildByName("file_2"):getChildByName("customs")
    self.slot_3_customs_ = root:getChildByName("file_3"):getChildByName("customs")

    self.slot_1_customs_:setOpacity(0)
    self.slot_2_customs_:setOpacity(0)
    self.slot_3_customs_:setOpacity(0)

    self.inactive_color_ = cc.c3b(127, 127, 127)
    self.active_color_ = cc.c3b(255, 255, 255)

    self.slot_1_:setColor(self.active_color_)
    self.slot_2_:setColor(self.inactive_color_)
    self.slot_3_:setColor(self.inactive_color_)

    self.delete_button_:setColor(self.inactive_color_)
    self.delete_confirm_:setVisible(false)
    
    self.selecting_ = 0
    self.deleting_confirmation_ = 1
    self.state_ = self.selecting_
    
    self.active_slot_ = self.slot_1_

    print("populating")

    for i=1, 3 do
        print(tostring(i))
        if self:slot_exists(i) then
            print("exists")
            self:load_slot(i)
            self:populate_slot(i)
        end
    end

    self.triggered_ = false

    if cc.audio.current_track ~= "sounds/bgm_title.mp3" then
        cc.audio.play_bgm("sounds/bgm_title.mp3", true)
    end

end

function save:read_slot(slot)
    local contents = cc.file.read("save" .. tostring(slot) .. ".json")
    return cc.json.decode(contents)
end

function save:color_weapon(custom, weapon_name)
    custom:getChildByName(weapon_name):setColor(self.active_color_)
end

function save:uncolor_weapon(custom, weapon_name)
    custom:getChildByName(weapon_name):setColor(self.inactive_color_)
end

function save:clear_slot(slot)
    local save_slot = nil

    if slot == 1 then
        save_slot = self.slot_1_customs_
    elseif slot == 2 then
        save_slot = self.slot_2_customs_
    else
        save_slot = self.slot_3_customs_
    end

    save_slot:setOpacity(0)

    local e_tank = save_slot:getChildByName("etank"):getChildByName("label")
    local m_tank = save_slot:getChildByName("mtank"):getChildByName("label")
    local lives = save_slot:getChildByName("life"):getChildByName("label")

    e_tank:setString(tostring(0))
    m_tank:setString(tostring(0))
    lives:setString(tostring(0))

    local weapons = save_slot:getChildByName("weapons")

    self:uncolor_weapon(weapons, "helmet")
    self:uncolor_weapon(weapons, "ex_helmet")
    self:uncolor_weapon(weapons, "chest")    
    self:uncolor_weapon(weapons, "fist")
    self:uncolor_weapon(weapons, "boot")
    self:uncolor_weapon(weapons, "freezer")
    self:uncolor_weapon(weapons, "sheriff")
    self:uncolor_weapon(weapons, "boomer")
    self:uncolor_weapon(weapons, "military")
    self:uncolor_weapon(weapons, "vine")
    self:uncolor_weapon(weapons, "shield")
    self:uncolor_weapon(weapons, "night")
    self:uncolor_weapon(weapons, "torch")
    self:uncolor_weapon(weapons, "helmet")
    self:uncolor_weapon(weapons, "ex")
end

function save:populate_slot(slot)
    local save_slot = nil

    if slot == 1 then
        save_slot = self.slot_1_customs_
    elseif slot == 2 then
        save_slot = self.slot_2_customs_
    else
        save_slot = self.slot_3_customs_
    end

    save_slot:setOpacity(255)

    local e_tank = save_slot:getChildByName("etank"):getChildByName("label")
    local m_tank = save_slot:getChildByName("mtank"):getChildByName("label")
    local lives = save_slot:getChildByName("life"):getChildByName("label")

    e_tank:setString(tostring(cc.player_.e_tanks_))
    m_tank:setString(tostring(cc.player_.m_tanks_))
    lives:setString(tostring(cc.player_.lives_))

    local weapons = save_slot:getChildByName("weapons")

    if cc.unlockables_.items_.helmet_.acquired_ then
        self:color_weapon(weapons, "helmet")
    end
    if cc.unlockables_.items_.head_.acquired_ then
        self:color_weapon(weapons, "ex_helmet")
    end
    if cc.unlockables_.items_.chest_.acquired_ then
        self:color_weapon(weapons, "chest")
    end

    if cc.unlockables_.items_.fist_.acquired_ then 
        self:color_weapon(weapons, "fist")

    end

    if cc.unlockables_.items_.boot_.acquired_ then 
        self:color_weapon(weapons, "boot")
    end

    if cc.browners_.freezer_.acquired_ then 
        self:color_weapon(weapons, "freezer")

    end

    if cc.browners_.sheriff_.acquired_ then 
        self:color_weapon(weapons, "sheriff")

    end

    if cc.browners_.boomer_.acquired_ then 
        self:color_weapon(weapons, "boomer")

    end

    if cc.browners_.military_.acquired_ then 
        self:color_weapon(weapons, "military")

    end

    if cc.browners_.vine_.acquired_ then 
        self:color_weapon(weapons, "vine")

    end

    if cc.browners_.shield_.acquired_ then 
        self:color_weapon(weapons, "shield")

    end

    if cc.browners_.night_.acquired_ then 
        self:color_weapon(weapons, "night")

    end

    if cc.browners_.torch_.acquired_ then 
        self:color_weapon(weapons, "torch")

    end

    if cc.browners_.helmet_.acquired_ then 
        self:color_weapon(weapons, "helmet")

    end

    if cc.browners_.extreme_.acquired_ then 
        self:color_weapon(weapons, "ex")
    end

end

function save:populate_slot_cheat(slot)

    local save_slot = nil

    if slot == 1 then
        save_slot = self.slot_1_customs_
    elseif slot == 2 then
        save_slot = self.slot_2_customs_
    else
        save_slot = self.slot_3_customs_
    end

    save_slot:setOpacity(255)

    local e_tank = save_slot:getChildByName("etank"):getChildByName("label")
    local m_tank = save_slot:getChildByName("mtank"):getChildByName("label")
    local lives = save_slot:getChildByName("life"):getChildByName("label")

    e_tank:setString(tostring(cc.player_.e_tanks_))
    m_tank:setString(tostring(cc.player_.m_tanks_))
    lives:setString(tostring(cc.player_.lives_))

    local weapons = save_slot:getChildByName("weapons")

    if cc.unlockables_.items_.helmet_.acquired_ then
        self:color_weapon(weapons, "helmet")
    end
    if cc.unlockables_.items_.head_.acquired_ then
        self:color_weapon(weapons, "ex_helmet")
    end
    if cc.unlockables_.items_.chest_.acquired_ then
        self:color_weapon(weapons, "chest")
    end

    if cc.unlockables_.items_.fist_.acquired_ then 
        self:color_weapon(weapons, "fist")
    end

    if cc.unlockables_.items_.boot_.acquired_ then 
        self:color_weapon(weapons, "boot")
    end

    if cc.browners_.freezer_.acquired_ then 
        self:color_weapon(weapons, "freezer")

    end

    if cc.browners_.sheriff_.acquired_ then 
        self:color_weapon(weapons, "sheriff")

    end

    if cc.browners_.boomer_.acquired_ then 
        self:color_weapon(weapons, "boomer")

    end

    if cc.browners_.military_.acquired_ then 
        self:color_weapon(weapons, "military")

    end

    if cc.browners_.vine_.acquired_ then 
        self:color_weapon(weapons, "vine")

    end

    if cc.browners_.shield_.acquired_ then 
        self:color_weapon(weapons, "shield")

    end

    if cc.browners_.night_.acquired_ then 
        self:color_weapon(weapons, "night")

    end

    if cc.browners_.torch_.acquired_ then 
        self:color_weapon(weapons, "torch")

    end

    if cc.browners_.helmet_.acquired_ then 
        self:color_weapon(weapons, "helmet")

    end

    if cc.browners_.extreme_.acquired_ then 
        self:color_weapon(weapons, "ex")
    end

    cc.browners_ = {
        teleport_   = {id_ = 1,  acquired_ = true,  pause_item_ = nil},
        violet_     = {id_ = 2,  acquired_ = true,  pause_item_ = "violet"},
        fuzzy_      = {id_ = 3,  acquired_ = true,  pause_item_ = "fuzzy"},
        freezer_    = {id_ = 4,  acquired_ = false, pause_item_ = "freezer"},
        sheriff_    = {id_ = 5,  acquired_ = true, pause_item_ = "sheriff"},
        boomer_     = {id_ = 6,  acquired_ = false, pause_item_ = "boomer"},
        military_   = {id_ = 7,  acquired_ = true, pause_item_ = "military"},
        vine_       = {id_ = 8,  acquired_ = true,  pause_item_ = "vine"},
        shield_     = {id_ = 9,  acquired_ = false, pause_item_ = "shield"},
        night_      = {id_ = 10, acquired_ = true, pause_item_ = "night"},
        torch_      = {id_ = 11, acquired_ = false, pause_item_ = "torch"},
        helmet_     = {id_ = 12, acquired_ = true, pause_item_ = "helmet"},
        extreme_    = {id_ = 13, acquired_ = true, pause_item_ = "ex"},
        boss_       = {id_ = 14, acquired_ = nil, pause_item_ = nil }
    }

    cc.unlockables_.items_.helmet_ = {id_ = 2, acquired_ = true }
    cc.unlockables_.items_.head_   = {id_ = 3, acquired_ = true }
    cc.unlockables_.items_.chest_  = {id_ = 4, acquired_ = true }
    cc.unlockables_.items_.fist_   = {id_ = 5, acquired_ = true }
    cc.unlockables_.items_.boot_   = {id_ = 6, acquired_ = true }


end

function save:load_slot(slot)
    local json = self:read_slot(slot)

    cc.unlockables_.items_.helmet_.acquired_ = true
    cc.unlockables_.items_.head_.acquired_ = json["head"]
    cc.unlockables_.items_.chest_.acquired_ = json["chest"]
    cc.unlockables_.items_.fist_.acquired_ = json["fist"]
    cc.unlockables_.items_.boot_.acquired_ = json["boot"]

    cc.player_.e_tanks_ = json["e"]
    cc.player_.m_tanks_ = json["m"]

    cc.player_.lives_ = json["lives"]

    cc.browners_.freezer_.acquired_ = json["freezer"]
    cc.browners_.sheriff_.acquired_ = json["sheriff"]
    cc.browners_.boomer_.acquired_ = json["boomer"]
    cc.browners_.military_.acquired_ = json["military"]
    cc.browners_.vine_.acquired_ = json["vine"]
    cc.browners_.shield_.acquired_ = json["shield"]
    cc.browners_.night_.acquired_ = json["night"]
    cc.browners_.torch_.acquired_ = json["torch"]

    cc.browners_.helmet_.acquired_ = cc.unlockables_.helmet_acquired_()
    cc.browners_.extreme_.acquired_ = cc.unlockables_.extreme_acquired_()

    cc.levels_[1].defeated_ = cc.browners_.freezer_.acquired_
    cc.levels_[2].defeated_ = cc.browners_.sheriff_.acquired_
    cc.levels_[3].defeated_ = cc.browners_.boomer_.acquired_
    cc.levels_[4].defeated_ = cc.browners_.military_.acquired_
    cc.levels_[5].defeated_ = cc.browners_.vine_.acquired_
    cc.levels_[6].defeated_ = cc.browners_.shield_.acquired_
    cc.levels_[7].defeated_ = cc.browners_.night_.acquired_
    cc.levels_[8].defeated_ = cc.browners_.torch_.acquired_

    cc.game_options_.helmet_activated_ = json["helmet_activated"] or false

    cc.item_.collectibles_ = json["collectible_items"]

end

function save:save_slot(slot)

    local settings = {
        helmet = true,
        head = false,
        chest = false,
        fist = false,
        boot = false,

        collectible_items = {},

        e = 0,
        m = 0,
        lives = 3,

        freezer = false,
        sheriff = false,
        boomer = false,
        military = false,
        vine = false,
        shield = false,
        night = false,
        torch = false,
        helmet_activated = false
    }

    local encoded = cc.json.encode(settings)

    cc.file.write("save" .. tostring(slot) .. ".json", encoded)

end

function save:delete_slot(slot)
    cc.file.remove("save" .. tostring(slot) .. ".json")
end

function save:reset_deleting()
    self.active_slot_ = self.slot_1_
    self.active_slot_:setColor(self.active_color_)
    self.slot_3_:setColor(self.inactive_color_)
    self.slot_2_:setColor(self.inactive_color_)
    self.delete_button_:setColor(self.inactive_color_)
    self.delete_confirm_:setVisible(false)
end

function save:slot_exists(slot)
    return cc.file.exists("save" .. tostring(slot) .. ".json")
end

function save:step(dt)

    if self.state_ == self.deleting_confirmation_ then
        if cc.key_pressed(cc.key_code_.up) then
            if self.active_slot_ == self.slot_1_ then
                if self:slot_exists(3) then
                    self.active_slot_ = self.slot_3_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_1_:setColor(self.inactive_color_)
                elseif self:slot_exists(2) then
                    self.active_slot_ = self.slot_2_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_1_:setColor(self.inactive_color_)
                end
            elseif self.active_slot_ == self.slot_2_ then
                if self:slot_exists(1) then
                    self.active_slot_ = self.slot_1_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_2_:setColor(self.inactive_color_)
                elseif self:slot_exists(3) then
                    self.active_slot_ = self.slot_3_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_2_:setColor(self.inactive_color_)
                end
            else
                if self:slot_exists(2) then
                    self.active_slot_ = self.slot_2_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_3_:setColor(self.inactive_color_)
                elseif self:slot_exists(1) then
                    self.active_slot_ = self.slot_1_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_3_:setColor(self.inactive_color_)
                end
            end
            cc.audio.play_sfx("sounds/sfx_select.mp3", false)
        end

        if cc.key_pressed(cc.key_code_.down) then
            if self.active_slot_ == self.slot_1_ then
                if self:slot_exists(2) then
                    self.active_slot_ = self.slot_2_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_1_:setColor(self.inactive_color_)
                elseif self:slot_exists(3) then
                    self.active_slot_ = self.slot_3_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_1_:setColor(self.inactive_color_)
                end
            elseif self.active_slot_ == self.slot_2_ then
                if self:slot_exists(3) then
                    self.active_slot_ = self.slot_3_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_2_:setColor(self.inactive_color_)
                elseif self:slot_exists(1) then
                    self.active_slot_ = self.slot_1_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_2_:setColor(self.inactive_color_)
                end
            else
                if self:slot_exists(1) then
                    self.active_slot_ = self.slot_1_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_3_:setColor(self.inactive_color_)
                elseif self:slot_exists(2) then
                    self.active_slot_ = self.slot_2_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_3_:setColor(self.inactive_color_)
                end
            end
            cc.audio.play_sfx("sounds/sfx_select.mp3", false)
        end

    end

    if self.state_ == self.selecting_ then
        if cc.key_pressed(cc.key_code_.up) then
            if self.active_slot_ == self.slot_1_ then
                if self:slot_exists(1) or self:slot_exists(2) or self:slot_exists(3) then
                    self.active_slot_ = self.delete_button_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_1_:setColor(self.inactive_color_)
                else
                    self.active_slot_ = self.slot_3_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_1_:setColor(self.inactive_color_)
                end
            elseif self.active_slot_ == self.slot_2_ then
                self.active_slot_ = self.slot_1_
                self.active_slot_:setColor(self.active_color_)
                self.slot_2_:setColor(self.inactive_color_)
            elseif self.active_slot_ == self.slot_3_ then
                self.active_slot_ = self.slot_2_
                self.active_slot_:setColor(self.active_color_)
                self.slot_3_:setColor(self.inactive_color_)
            else
                self.active_slot_ = self.slot_3_
                self.active_slot_:setColor(self.active_color_)
                self.delete_button_:setColor(self.inactive_color_)            
            end
            cc.audio.play_sfx("sounds/sfx_select.mp3", false)
        end

        if cc.key_pressed(cc.key_code_.down) then
            if self.active_slot_ == self.slot_1_ then
                self.active_slot_ = self.slot_2_
                self.active_slot_:setColor(self.active_color_)
                self.slot_1_:setColor(self.inactive_color_)
            elseif self.active_slot_ == self.slot_2_ then
                self.active_slot_ = self.slot_3_
                self.active_slot_:setColor(self.active_color_)
                self.slot_2_:setColor(self.inactive_color_)
            elseif self.active_slot_ == self.slot_3_ then
                if self:slot_exists(1) or self:slot_exists(2) or self:slot_exists(3) then
                    self.active_slot_ = self.delete_button_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_3_:setColor(self.inactive_color_)
                else
                    self.active_slot_ = self.slot_1_
                    self.active_slot_:setColor(self.active_color_)
                    self.slot_3_:setColor(self.inactive_color_)            
                end
            else
                self.active_slot_ = self.slot_1_
                self.active_slot_:setColor(self.active_color_)
                self.delete_button_:setColor(self.inactive_color_)            
            end

            cc.audio.play_sfx("sounds/sfx_select.mp3", false)
        end
    end

    if not self.triggered_ then

        if cc.key_pressed(cc.key_code_.a) then

            local delete_state = false

            local selected_slot = 0
            if self.active_slot_ == self.slot_1_ then
                selected_slot = 1
            elseif self.active_slot_ == self.slot_2_ then
                selected_slot = 2
            elseif self.active_slot_ == self.slot_3_ then
                selected_slot = 3
            else
                delete_state = true
            end

            if self.state_ == self.deleting_confirmation_ then
                delete_state = true
            end

            if delete_state then

                    
                if self.state_ == self.selecting_ then    
                    self.delete_confirm_:setOpacity(255)
                    self.delete_confirm_:setVisible(true)
                    self.active_slot_ = self.slot_1_
                    self.active_slot_:setColor(self.active_color_)                
                    self.state_ = self.deleting_confirmation_
                else
                    self.delete_confirm_:setOpacity(0)
                    self:delete_slot(selected_slot)
                    self:clear_slot(selected_slot)
                    self:reset_deleting()
                    self.state_ = self.selecting_
                end
            
            else
                if not self:slot_exists(selected_slot) then
                    self:save_slot(selected_slot)
                end
                
    
                self:load_slot(selected_slot)
                
                if cc.player_.lives_ <= 0 then
                    cc.player_.lives_ = 3
                end
    
                if selected_slot == 3 then
                    self:populate_slot_cheat(selected_slot)
                else
                    self:populate_slot(selected_slot)
                end
    
                cc.game.slot_ = selected_slot
    
                local slot = cc.game.get_default_slot()
    
                slot["lives"] = cc.player_.lives_
            
                cc.game.save_default_slot(slot)
                
                self.triggered_ = true
    
                ccexp.AudioEngine:stopAll()
                cc.audio.play_sfx("sounds/sfx_selected.mp3", false)

                self:getApp()
                :enterScene("screens.stage_select", "FADE", 0.5)
            end

        elseif cc.key_pressed(cc.key_code_.b) then

            if self.state_ == self.deleting_confirmation_ then
                self.state_ = self.selecting_
                self:reset_deleting()
            else
                self.triggered_ = true

                self:getApp()
                :enterScene("screens.title", "FADE", 0.5)    
            end
            
        end
    end

    -- joypad reset
    self:post_step(dt)

    return self
end

return save