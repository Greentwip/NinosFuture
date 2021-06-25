require "config"
require "cocos.init"

local device = require("cocos.framework.device")


cc.FileUtils:getInstance():setPopupNotify(false)
cc.FileUtils:getInstance():addSearchPath("src/")
cc.FileUtils:getInstance():addSearchPath("res/")
cc.FileUtils:getInstance():addSearchResolutionsOrder("src/")

local json = require "json"

cc.file = {
exists = function() end,
read = function() end,
read_raw = function() end,
write = function() end,
remove = function() end
}

cc.json = {
decode = function() end,
encode = function() end
}

cc.audio = {
current_track = "",
play_bgm = function() end,
play_sfx = function() end,
set_bgm_volume = function() end,
current_bgm_id_ = -1
}

cc.game = {
slot_ = 1,
read_slot = function() end,
save_slot = function() end,
get_default_slot = function() end,
save_default_slot = function() end
}

cc.bgm_volume_ = 0.5
cc.sfx_volume_ = 0.5

-- see if the file exists
function cc.file.exists(file)
    local writable_path = cc.FileUtils:getInstance():getWritablePath() 

    local file_path = writable_path .. "/" .. file

    local f = io.open(file_path, "rb")
    if f then f:close() end
    return f ~= nil
end

function cc.file.read(file)

    local writable_path = cc.FileUtils:getInstance():getWritablePath() 

    local file_path = writable_path .. "/" .. file

    local f = io.open(file_path, "rb")

    local contents = f:read("*all")
    f:close()
    return contents
end

function cc.file.read_raw(file)
    --local file_path = file
    --print(file_path)

    --local f = io.open(file_path, "rb")

    --local contents = f:read("*all")
    --f:close()


    --return contents

    return cc.FileUtils:getInstance():getStringFromFile(file)
end

function cc.file.remove(file)
    local writable_path = cc.FileUtils:getInstance():getWritablePath() 

    local file_path = writable_path .. "/" .. file

    cc.FileUtils:getInstance():removeFile(file_path)
end

-- Write a string to a file.
function cc.file.write(filename, contents)
    local writable_path = cc.FileUtils:getInstance():getWritablePath() 

    local file_path = writable_path .. "/" .. filename

    local file, err = io.open( file_path, "w+" )
    file:write( contents )
    file:flush()
    file:close()

    --os.remove(file_path)

    --os.rename( file_path .. ".tmp", file_path )
end

function cc.json.encode(table)
    local status = pcall(json.encode, table)

    if status then
        return json.encode(table)
    end

    return nil

end

function cc.json.decode(string)
    local status = pcall(json.decode, string)

    if status then
        return json.decode(string)
    end

    return nil

end

function cc.game.read_slot(slot)
    local contents = cc.file.read("save" .. tostring(slot) .. ".json")
    return cc.json.decode(contents)
end

function cc.game.save_slot(slot, data)
    local encoded = cc.json.encode(data)
    cc.file.write("save" .. tostring(slot) .. ".json", encoded)
end

function cc.game.get_default_slot()
    local contents = cc.file.read("save" .. tostring(cc.game.slot_) .. ".json")
    return cc.json.decode(contents)
end

function cc.game.save_default_slot(data)
    local encoded = cc.json.encode(data)
    cc.file.write("save" .. tostring(cc.game.slot_) .. ".json", encoded)
end


function cc.audio.play_bgm(path, loop)
    cc.audio.current_track = path
    local id = ccexp.AudioEngine:play2d(path, loop, cc.bgm_volume_)
    cc.current_bgm_id_ = id
    return id
end

function cc.audio.play_sfx(path, loop)
    local id = ccexp.AudioEngine:play2d(path, loop, cc.sfx_volume_)
    return id
end

function cc.audio.set_bgm_volume(volume)
    cc.bgm_volume_ = volume
    ccexp.AudioEngine:setVolume(cc.current_bgm_id_, cc.bgm_volume_)
end

local function main()
    --[[ccexp.AudioEngine:preload('sounds/bgm_boss_intro.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_boss_victory.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_boss_vineman.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_gameover.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_get_weapon.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_level_boomerman.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_level_militaryman.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_level_nightman.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_level_sheriffman.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_level_test.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_level_vineman.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_load_screen.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_opening.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_stage_select.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_stage_sheriffman.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_stage_vineman.mp3')
    ccexp.AudioEngine:preload('sounds/bgm_title.mp3')
    ccexp.AudioEngine:preload('sounds/mus_password.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_buster_charging_high.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_buster_charging_mid.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_buster_shoot.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_buster_shoot_high.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_buster_shoot_mid.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_death.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_door.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_enemyhit.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_enemyland1.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_error.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_explosion1.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_getenergy.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_getlife.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_gyroattack.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_hit.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_land.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_land.ogg')
    ccexp.AudioEngine:preload('sounds/sfx_pause.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_ricochet.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_roar.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_select.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_selected.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_splash.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_taban.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_teleport1.mp3')
    ccexp.AudioEngine:preload('sounds/sfx_teleport2.mp3')
    ccexp.AudioEngine:preload('sounds/screens/boss_intro/bgm_boss_intro.mp3')
    ccexp.AudioEngine:preload('sounds/screens/common/belt/sfx_belt_join.mp3')
    ccexp.AudioEngine:preload('sounds/screens/common/belt/sfx_belt_join.ogg')
    ]]

    if cc.file.exists("options.json") then
        local contents = cc.file.read("options.json")

        if cc.json.decode(contents) ~= nil then
            local decoded = cc.json.decode(contents)
            cc.bgm_volume_ = decoded["bgm_volume"]
            cc.sfx_volume_ = decoded["sfx_volume"]
        end
    else
        local decoded = { bgm_volume = cc.bgm_volume_, sfx_volume = cc.sfx_volume_ }

        local string = cc.json.encode(decoded)

        cc.file.write("options.json", string)

    end

    cc.Director:getInstance():setAnimationInterval(1 / 60.0)
    require("app.MyApp"):create():run()

end

local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    print(msg)
end
