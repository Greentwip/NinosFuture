-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local level_base = import("app.objects.gameplay.control.level_base")

local level  = class("level", level_base)

-- anything related to physics should be created here
function level:onLoad()
    self.super:onLoad()

    self.level_bgm_ = "sounds" .. "/" .. "bgm_level_" .. cc.current_level_.mug_ .. ".mp3"
    self.tmx_map_   = "tilemaps" .. "/" .. cc.current_level_.mug_  .. "/" .. "level" .. "_" .. cc.current_level_.mug_ .. ".tmx"
    self.map_path_  = "tilemaps" .. "/" .. cc.current_level_.mug_
end

return level