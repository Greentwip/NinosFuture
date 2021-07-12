#include "Level.h"
#include "External/tao/json.hpp"
#include "External/tao/json/contrib/traits.hpp"

#include "Display.h"

using namespace windy;

cocos2d::Point calculateTmxPosition(cocos2d::ValueMap tmxObject, cocos2d::experimental::TMXTiledMap* tmxMap) {
    float tmxBlockY = (tmxMap->getMapSize().height * tmxMap->getTileSize().height) - tmxObject["y"].asFloat();
    float tmxRealBlockY = tmxBlockY - tmxObject["height"].asFloat() * 0.5f;

    float tmxBlockX = tmxObject["x"].asFloat() + (tmxObject["width"].asFloat() * 0.5f);

    int top = Display::getInstance().top;

    return cocos2d::Point(tmxBlockX, top - tmxRealBlockY);
}

Level* Level::create(const std::string& mapPath, const std::string& mug, windy::Sounds bgm) {

    Level* level = new (std::nothrow) Level();

    if (level) {
        level->mapPath = mapPath;
        level->mug = mug;
        level->bgm = bgm;
    }

    if (level && level->init()) {
        level->autorelease();
        return level;
    }

    CC_SAFE_DELETE(level);
    return nullptr;
}

bool Level::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }


    // Tile layers
    std::string definitionsPath = this->mapPath + "/" + "definitions.json";
    const tao::json::value v = tao::json::from_file(definitionsPath);

    std::vector<tao::json::value> layers = v.at("layers").get_array();

    for (int i = 0; i < layers.size(); ++i) {
        auto layer = layers[i];

        std::string baseName = layer.as<std::string>("basename");
        int order = layer.as<int>("order");

        int tileX = layer.as<int>("tile_x");
        int tileY = layer.as<int>("tile_y");

        std::vector<tao::json::value> tiles = layer.at("tiles").get_array();

        for (int j = 0; j < tiles.size(); ++j) {
            auto tile = tiles[j];

            std::string path = tile.as<std::string>("path");

            int x = tile.as<int>("x");
            int y = tile.as<int>("y");

            std::string spritePath = this->mapPath + "/" + "__slices" + "/" + baseName + "/" + path;

            auto sprite = cocos2d::Sprite::create(spritePath);
            int spriteX = x * tileX;
            int spriteY = y * tileY;

            auto leftTop = Display::getInstance().left_top;
            auto posFinal = cocos2d::Point(leftTop.x + spriteX, leftTop.y + spriteY);

            sprite->setPosition(posFinal);
            sprite->setAnchorPoint(cocos2d::Point(0, 1));
            sprite->getTexture()->setAliasTexParameters();
            this->addChild(sprite);

        }
    }

    std::string tmxPath = this->mapPath + "/" + "level" + "_" + this->mug + ".tmx";

    auto map = cocos2d::experimental::TMXTiledMap::create(tmxPath);
    map->setAnchorPoint(cocos2d::Point(0, 1));

    auto leftTop = Display::getInstance().left_top;

    map->setPosition(leftTop);

    this->addChild(map);

    // Blocks

    cocos2d::TMXObjectGroup* groupArray = map->getObjectGroup("blocks");

    {
        auto& objects = groupArray->getObjects();
        for (auto& obj : objects)
        {
            auto& dictionary = obj.asValueMap();

            auto size = cocos2d::Size(dictionary["width"].asFloat(), dictionary["height"].asFloat());
            auto position = calculateTmxPosition(dictionary, map);

        }
    }

    return true;
}


void Level::onEnter()
{
    Node::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void Level::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Node::onExit();
}

void Level::update(float dt)
{
    
}
