#include "Level.h"
#include "External/tao/json.hpp"
#include "External/tao/json/contrib/traits.hpp"

#include "Display.h"

#include "Entities/Logical.h"
#include "Entities/Block.h"
#include "Entities/Bounds.h"
#include "Entities/Camera.h"
#include "Entities/Checkpoint.h"
#include "Entities/Player.h"

#include "GameTags.h"

#include "DebugDrawNode.h"

#include "PhysicsWorld.h"

#include "ObjectManager.h"

using namespace windy;


template <typename T, typename U>
class EntityFactory
{
public:
    template <typename TDerived>
    void registerType(U name)
    {
        _createFuncs[name] = &createFunc<TDerived>;
    }

    T* create(std::string name, Level* level, const cocos2d::Point& position, const cocos2d::Size& size) {
        typename std::map<U, PCreateFunc>::const_iterator it = _createFuncs.find(name);
        if (it != _createFuncs.end()) {
            return it->second(level, position, size);
        }
        return nullptr;
    }

    void clear() {
        this->_createFuncs.clear();
    }

private:
    template <typename TDerived>
    static T* createFunc(Level* level, const cocos2d::Point& position, const cocos2d::Size& size)
    {
        return T::create<TDerived>(level, position, size);
    }

    typedef T*(*PCreateFunc)(Level* level, const cocos2d::Point& position, const cocos2d::Size& size);
    std::map<U, PCreateFunc> _createFuncs;
};

static EntityFactory<Logical, std::string> s_entityFactory;

cocos2d::Point calculateTmxPosition(cocos2d::ValueMap tmxObject, cocos2d::experimental::TMXTiledMap* tmxMap) {
    //float tmxBlockY = (tmxMap->getMapSize().height * tmxMap->getTileSize().height) - tmxObject["y"].asFloat();
    float tmxBlockY = tmxObject["y"].asFloat();
    float tmxRealBlockY = tmxBlockY - tmxObject["height"].asFloat() * 0.5f;

    float tmxBlockX = tmxObject["x"].asFloat() + (tmxObject["width"].asFloat() * 0.5f);

    //int top = Display::getInstance().top;

    //return cocos2d::Point(tmxBlockX, top - tmxRealBlockY);
    return cocos2d::Point(tmxBlockX, tmxRealBlockY);
}

Level* Level::create(const std::string& resourcesRootPath,
                     const std::string& tilemapRootPath,
                     const std::string& mug, windy::Sounds bgm) {

    s_entityFactory.clear();

    s_entityFactory.registerType<Block>("block");
    s_entityFactory.registerType<Camera>("camera");
    s_entityFactory.registerType<Checkpoint>("checkpoint");
    s_entityFactory.registerType<Bounds>("bounds");
    s_entityFactory.registerType<Player>("player");

    Level* level = new (std::nothrow) Level();

    if (level) {
        level->resourcesRootPath = resourcesRootPath;
        level->tilemapRootPath = tilemapRootPath;
        level->mug = mug;
        level->bgm = bgm;
        level->isPaused = false;
        level->triggeringDoor = nullptr;
        level->bounds = nullptr;
        level->camera = nullptr;
        level->debugDrawNode = nullptr;
        level->physicsWorld = nullptr;
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
    std::string definitionsPath = this->resourcesRootPath + "/" + this->tilemapRootPath + "/" + this->mug + "/" + "definitions.json";
    const tao::json::value v = tao::json::from_file(definitionsPath);

    std::vector<tao::json::value> layers = v.at("layers").get_array();

    for (int i = 0; i < layers.size(); ++i) {
        auto layer = layers[i];

        std::string baseName = layer.as<std::string>("basename");
        int order = layer.as<int>("order");

        int tileX = layer.as<int>("tile_x");
        int tileY = layer.as<int>("tile_y");

        std::vector<tao::json::value> tiles = layer.at("tiles").get_array();

        int maxTileY = 0;

        for (int j = 0; j < tiles.size(); ++j) {
            auto tile = tiles[j];

            int y = tile.as<int>("y");

            if (y > maxTileY) {
                maxTileY = y;
            }
        }

        for (int j = 0; j < tiles.size(); ++j) {
            auto tile = tiles[j];

            std::string path = tile.as<std::string>("path");

            int x = tile.as<int>("x");
            int y = tile.as<int>("y");

            std::string spritePath = this->tilemapRootPath + "/" + this->mug + "/" + "__slices" + "/" + baseName + "/" + path;

            auto sprite = cocos2d::Sprite::create(spritePath);
            int spriteX = x * tileX;
            int spriteY = (maxTileY - y) * tileY;

            auto leftBottom = Display::getInstance().left_bottom;
            auto posFinal = cocos2d::Point(leftBottom.x + spriteX, leftBottom.y + spriteY);

            sprite->setPosition(posFinal);
            sprite->setAnchorPoint(cocos2d::Point(0, 0));
            sprite->getTexture()->setAliasTexParameters();
            this->addChild(sprite);

        }
    }

    std::string tmxPath = this->tilemapRootPath + "/" + this->mug + "/" + "level" + "_" + this->mug + ".tmx";

    auto map = cocos2d::experimental::TMXTiledMap::create(tmxPath);
    map->setAnchorPoint(cocos2d::Point(0, 1));

    auto leftTop = Display::getInstance().left_top;

    map->setPosition(leftTop);

    this->addChild(map);

    // Physics
    physicsWorld = PhysicsWorld::create(this);
    this->addChild(physicsWorld);

    // Blocks

    cocos2d::TMXObjectGroup* groupArray = map->getObjectGroup("blocks");

    {
        auto& objects = groupArray->getObjects();
        for (auto& obj : objects)
        {
            auto& dictionary = obj.asValueMap();

            auto size = cocos2d::Size(dictionary["width"].asFloat(), dictionary["height"].asFloat());
            auto position = calculateTmxPosition(dictionary, map);

            auto block = s_entityFactory.create("block", this, position, size);

            this->addChild(block);

            this->entities.pushBack(block);


        }
    }

    cocos2d::Vector<Logical*> checkpoints;

    Logical* firstCheckpoint = nullptr;

    groupArray = map->getObjectGroup("logic");

    {
        auto& objects = groupArray->getObjects();
        for (auto& obj : objects)
        {
            auto& dictionary = obj.asValueMap();

            std::string name = dictionary["name"].asString();
            auto size = cocos2d::Size(dictionary["width"].asFloat(), dictionary["height"].asFloat());
            auto position = calculateTmxPosition(dictionary, map);

            if (name.compare("checkpoint") == 0) {
                auto checkpoint = s_entityFactory.create("checkpoint", this, position, size);
                checkpoint->parseBehavior(dictionary);
                this->addChild(checkpoint);

                checkpoints.pushBack(checkpoint);

                if (checkpoint->getTag() == GameTags::Logic::Checkpoint::First) {
                    firstCheckpoint = checkpoint;
                }
            }

        }
    }

    auto firstCheckpointPosition = firstCheckpoint->getPosition();

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

    this->bounds = dynamic_cast<Bounds*>(s_entityFactory.create("bounds", this, firstCheckpointPosition, visibleSize));
    
    this->addChild(this->bounds, 4096);

    auto playerSize = cocos2d::Size(15, 24);

    this->player = dynamic_cast<Player*>(s_entityFactory.create("player", this, firstCheckpointPosition, playerSize));

    this->addChild(this->player, 512);

    this->entities.pushBack(this->player);

    auto cameraSize = cocos2d::Size(8, 16);

    this->camera = dynamic_cast<Camera*>(s_entityFactory.create("camera", this, cocos2d::Point(0, 0), cameraSize));

    this->camera->setPositionY(this->camera->collisionBox->size.height * 0.25f);

    this->bounds->addChild(this->camera);

    this->debugDrawNode = DebugDrawNode::create(this);

    this->addChild(this->debugDrawNode);

    this->debugDrawNode->customEntities.pushBack(this->bounds);
    
    this->objectManager = ObjectManager::create(this);

    this->addChild(this->objectManager);

    return true;
}

bool Level::paused() {
    return this->isPaused;
}

void Level::pause(bool isPaused) {
    this->isPaused = isPaused;
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
