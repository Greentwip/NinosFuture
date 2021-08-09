#include "Level.h"
#include "External/tao/json.hpp"
#include "External/tao/json/contrib/traits.hpp"

#include "Display.h"

#include "Entities/Logical.h"
#include "Entities/Door.h"
#include "Entities/Block.h"
#include "Entities/Bounds.h"
#include "Entities/Camera.h"
#include "Entities/Checkpoint.h"
#include "Entities/Player.h"
#include "Entities/Enemy.h"
#include "Entities/Item.h"
#include "Entities/Ladder.h"
#include "Entities/Scroll.h"
#include "Entities/Gui.h"

#include "GameTags.h"

#include "DebugDrawNode.h"

#include "PhysicsWorld.h"

#include "ObjectManager.h"

#include "LandscapeTile.h"

#include "EntityFactory.h"

using namespace windy;

cocos2d::Point calculateTmxPosition(cocos2d::ValueMap tmxObject, cocos2d::experimental::TMXTiledMap* tmxMap) {
    //float tmxBlockY = (tmxMap->getMapSize().height * tmxMap->getTileSize().height) - tmxObject["y"].asFloat();
    float tmxBlockY = tmxObject["y"].asFloat();
    float tmxRealBlockY = tmxBlockY + tmxObject["height"].asFloat() * 0.5f;

    float tmxBlockX = tmxObject["x"].asFloat() + (tmxObject["width"].asFloat() * 0.5f);

    //int top = Display::getInstance().top;

    //return cocos2d::Point(tmxBlockX, top - tmxRealBlockY);
    return cocos2d::Point(tmxBlockX, tmxRealBlockY);
}

Level* Level::create(const std::string& resourcesRootPath,
                     const std::string& tilemapRootPath,
                     const std::string& mug, windy::Sounds bgm) {

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

        EntityFactory::initialize(level);

        EntityFactory::getInstance().registerType<Block>("block");
        EntityFactory::getInstance().registerType<Camera>("camera");
        EntityFactory::getInstance().registerType<Checkpoint>("checkpoint");
        EntityFactory::getInstance().registerType<Bounds>("bounds");
        EntityFactory::getInstance().registerType<Ladder>("ladder");
        EntityFactory::getInstance().registerType<Scroll>("scroll");

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

            //this->tiles.pushBack(LandscapeTile::create(sprite, posFinal, cocos2d::Size(static_cast<float>(tileX), static_cast<float>(tileY))));
        }
    }

    std::string tmxPath = this->tilemapRootPath + "/" + this->mug + "/" + "level" + "_" + this->mug + ".tmx";

    auto map = cocos2d::experimental::TMXTiledMap::create(tmxPath);
    map->setAnchorPoint(cocos2d::Point(0, 1));

    auto leftTop = Display::getInstance().left_top;

    map->setPosition(leftTop);

    //this->addChild(map);

    // Object management
    this->objectManager = ObjectManager::create(this);

    this->addChild(this->objectManager);


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

            auto entryCollisionBox = Logical::getEntryCollisionRectangle<Block>(position, size);
            auto entry = Logical::getEntry(entryCollisionBox, [=]() { return EntityFactory::getInstance().create("block", position, size); });

            this->objectManager->objectEntries.push_back(entry);
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
                auto checkpoint = EntityFactory::getInstance().create("checkpoint", position, size);
                checkpoint->parseBehavior(dictionary);
                this->addChild(checkpoint);

                checkpoints.pushBack(checkpoint);

                if (checkpoint->getTag() == GameTags::Logic::Checkpoint::First) {
                    firstCheckpoint = checkpoint;
                }
            }
            else if (name.compare("ladder") == 0) {

                auto ladder = EntityFactory::getInstance().create("ladder", position, size);
                ladder->parseBehavior(dictionary);
                this->addChild(ladder);
                this->entities.pushBack(ladder);

            }
            else if (name.compare("scroll") == 0) {
                auto scroll = EntityFactory::getInstance().create("scroll", position, size);
                scroll->parseBehavior(dictionary);
                this->addChild(scroll);
                this->entities.pushBack(scroll);
            }

        }
    }

    groupArray = map->getObjectGroup("special");

    {
        auto& objects = groupArray->getObjects();
        for (auto& obj : objects)
        {
            auto& dictionary = obj.asValueMap();

            std::string name = dictionary["name"].asString();
            auto size = cocos2d::Size(dictionary["width"].asFloat(), dictionary["height"].asFloat());
            auto position = calculateTmxPosition(dictionary, map);

            if (name.compare("door") == 0) {
                auto door = EntityFactory::getInstance().create("door", position, size);
                door->parseBehavior(dictionary);
                this->addChild(door);
                this->entities.pushBack(door);
                this->verticalDoors.pushBack(dynamic_cast<Door*>(door));

            }
            else if (name.compare("horizontal_door") == 0) {
                auto door = EntityFactory::getInstance().create("horizontal_door", position, size);
                door->parseBehavior(dictionary);
                this->addChild(door);
                this->entities.pushBack(door);
                this->horizontalDoors.pushBack(dynamic_cast<Door*>(door));
            }
        }

    }

    groupArray = map->getObjectGroup("enemies");

    {
        auto& objects = groupArray->getObjects();
        for (auto& obj : objects)
        {
            auto& dictionary = obj.asValueMap();

            std::string name = dictionary["name"].asString();
            auto size = cocos2d::Size(dictionary["width"].asFloat(), dictionary["height"].asFloat());
            auto position = calculateTmxPosition(dictionary, map);

            if (name.compare("cannon_joe") == 0 || 
                name.compare("lyric") == 0 || 
                name.compare("Subeil") == 0 ||
                name.compare("sumatran") == 0 || 
                name.compare("taban") == 0 || 
                
                name.compare("barrel") == 0 || 
                name.compare("cow") == 0 || 
                name.compare("jetbird") == 0 || 
                name.compare("tremor") == 0) {

                auto entryCollisionBox = EntityFactory::getInstance().getEntryCollisionRectangle(name, position, size);
                auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                    return EntityFactory::getInstance().create(name, position, size);
                    });

                this->objectManager->objectEntries.push_back(entry);

            }
        }

    }

    auto firstCheckpointPosition = firstCheckpoint->getPosition();

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

    this->bounds = dynamic_cast<Bounds*>(EntityFactory::getInstance().create("bounds", firstCheckpointPosition, visibleSize));

    this->addChild(this->bounds, 4096);

    auto playerSize = cocos2d::Size(15, 24);

    this->player = dynamic_cast<Player*>(EntityFactory::getInstance().create("player", firstCheckpointPosition, playerSize));

    this->addChild(this->player, 512);

    this->entities.pushBack(this->player);

    auto cameraSize = cocos2d::Size(8, 16);

    this->camera = dynamic_cast<Camera*>(EntityFactory::getInstance().create("camera", cocos2d::Point(0, 0), cameraSize));

    this->camera->setPositionY(this->camera->collisionBox->size.height * 0.25f);

    this->entities.pushBack(this->camera);

    this->bounds->addChild(this->camera);

    this->camera->normalizeCollisionRectangles();

    this->debugDrawNode = DebugDrawNode::create(this);

    this->addChild(this->debugDrawNode);

    this->debugDrawNode->customEntities.pushBack(this->bounds);

    auto guiSize = cocos2d::Size(16, 16);

    this->gui = dynamic_cast<Gui*>(EntityFactory::getInstance().create("gui", cocos2d::Point(0, 0), guiSize));

    this->bounds->addChild(gui);

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
    /*cocos2d::Vector<LandscapeTile*> potentialNewTiles;
    cocos2d::Vector<LandscapeTile*> discardedTiles;

    auto boundsCollisionBox = this->bounds->collisionBox;

    for (int i = 0; i < this->tiles.size(); ++i) {
        auto tile = this->tiles.at(i);

        auto tileCollisionBox = tile->collisionBox;
        
        // upper left

        auto upperLeftCollisionBox = *boundsCollisionBox;
        auto upperCenterCollisionBox = *boundsCollisionBox;
        auto upperRightCollisionBox = *boundsCollisionBox;

        auto middleLeftCollisionBox = *boundsCollisionBox;
        auto middleCenterCollisionBox = *boundsCollisionBox;
        auto middleRightCollisionBox = *boundsCollisionBox;

        auto bottomLeftCollisionBox = *boundsCollisionBox;
        auto bottomCenterCollisionBox = *boundsCollisionBox;
        auto bottomRightCollisionBox = *boundsCollisionBox;

        // Top row
        upperLeftCollisionBox.origin.x -= boundsCollisionBox->size.width;
        upperLeftCollisionBox.origin.y += boundsCollisionBox->size.height;

        upperCenterCollisionBox.origin.x = boundsCollisionBox->origin.x;
        upperCenterCollisionBox.origin.y += boundsCollisionBox->size.height;

        upperRightCollisionBox.origin.x += boundsCollisionBox->size.width;
        upperRightCollisionBox.origin.y += boundsCollisionBox->size.height;

        // Middle row
        middleLeftCollisionBox.origin.x = boundsCollisionBox->size.width;
        middleLeftCollisionBox.origin.y += boundsCollisionBox->size.height;

        middleLeftCollisionBox.origin.x = boundsCollisionBox->origin.x;
        middleLeftCollisionBox.origin.y += boundsCollisionBox->size.height;

        middleLeftCollisionBox.origin.x = boundsCollisionBox->size.width;
        middleLeftCollisionBox.origin.y += boundsCollisionBox->size.height;

        // Bottom row
        upperLeftCollisionBox.origin.x -= boundsCollisionBox->size.width;
        upperLeftCollisionBox.origin.y -= boundsCollisionBox->size.height;

        upperCenterCollisionBox.origin.x = boundsCollisionBox->origin.x;
        upperCenterCollisionBox.origin.y -= boundsCollisionBox->size.height;

        upperRightCollisionBox.origin.x += boundsCollisionBox->size.width;
        upperRightCollisionBox.origin.y -= boundsCollisionBox->size.height;

        std::vector<cocos2d::Rect> screenCollisionRectangles = {
            upperLeftCollisionBox,
            upperCenterCollisionBox,
            upperRightCollisionBox,

            middleLeftCollisionBox,
            middleCenterCollisionBox,
            middleRightCollisionBox,

            bottomLeftCollisionBox,
            bottomCenterCollisionBox,
            bottomRightCollisionBox
        };

        bool tileCollides = false;
        for (int j = 0; j < screenCollisionRectangles.size(); ++j) {
            auto screenCollisionRectangle = screenCollisionRectangles[j];
            if (screenCollisionRectangle.intersectsRect(*tileCollisionBox)) {
                tileCollides = true;
                break;
            }
        }

        if (tileCollides) {
            potentialNewTiles.pushBack(tile);
        }
        else {
            discardedTiles.pushBack(tile);
        }
    }

    auto children = this->getChildren();
    for (int i = 0; i < discardedTiles.size(); ++i) {
        auto discardedTile = discardedTiles.at(i);

        if (children.find(discardedTile) != children.end()) {
            discardedTile->removeFromParent();
        }
    }


    for (int i = 0; i < potentialNewTiles.size(); ++i) {
        auto potentialNewTile = potentialNewTiles.at(i);

        if (children.find(potentialNewTile) == children.end()) {
            this->addChild(potentialNewTile);
        }
    }*/

}
