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
#include "Entities/Boss.h"
#include "Entities/Enemy.h"
#include "Entities/Spikes.h"
#include "Entities/Item.h"
#include "Entities/Ladder.h"
#include "Entities/Scroll.h"
#include "Entities/Passage.h"
#include "Entities/Teleporter.h"
#include "Entities/Gui.h"
#include "Entities/Browner.h"
#include "Entities/LevelController.h"
#include "Entities/Weapon.h"

#include "Sprite.h"

#include "GameTags.h"

#include "DebugDrawNode.h"

#include "PhysicsWorld.h"

#include "ObjectManager.h"

#include "LandscapeTile.h"

#include "EntityFactory.h"

#include "SaveManager.h"

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
#if _DEBUG
        level->debugDrawNode = nullptr;
#endif
        level->physicsWorld = nullptr;
        level->gui = nullptr;
        level->boss = nullptr;

        EntityFactory::getInstance().initialize(level);

        EntityFactory::getInstance().registerType<Block>("block");
        EntityFactory::getInstance().registerType<Camera>("camera");
        EntityFactory::getInstance().registerType<Checkpoint>("checkpoint");
        EntityFactory::getInstance().registerType<Bounds>("bounds");
        EntityFactory::getInstance().registerType<Ladder>("ladder");
        EntityFactory::getInstance().registerType<Scroll>("scroll");
        EntityFactory::getInstance().registerType<Passage>("hole");
        EntityFactory::getInstance().registerType<Teleporter>("teleporter");
        EntityFactory::getInstance().registerType<Spikes>("spikes");
        
        EntityFactory::getInstance().registerTypeCollisionFunc<Spikes>("spikes");

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
        //int order = layer.as<int>("order");

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

    if (groupArray != nullptr)
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

    if (groupArray != nullptr)
    {
        auto& objects = groupArray->getObjects();
        for (auto& obj : objects)
        {
            auto& dictionary = obj.asValueMap();

            std::string name = dictionary["name"].asString();
            auto size = cocos2d::Size(dictionary["width"].asFloat(), dictionary["height"].asFloat());
            auto position = calculateTmxPosition(dictionary, map);

            if (name.compare("checkpoint") == 0) {
                auto entity = EntityFactory::getInstance().create("checkpoint", position, size);
                entity->parseBehavior(dictionary);
                this->addChild(entity);

                checkpoints.pushBack(entity);

                if (entity->getTag() == GameTags::Logic::Checkpoint::First) {
                    firstCheckpoint = entity;
                }

                this->entities.pushBack(entity);
            }
            else if (name.compare("ladder") == 0) {

                auto entity = EntityFactory::getInstance().create("ladder", position, size);
                entity->parseBehavior(dictionary);
                this->addChild(entity);
                this->entities.pushBack(entity);

            }
            else if (name.compare("scroll") == 0) {
                auto entity = EntityFactory::getInstance().create("scroll", position, size);
                entity->parseBehavior(dictionary);
                this->addChild(entity);
                this->entities.pushBack(entity);
            }
            else if (name.compare("teleporter") == 0) {
                auto entity = EntityFactory::getInstance().create("teleporter", position, size);
                entity->parseBehavior(dictionary);
                this->addChild(entity);
                this->entities.pushBack(entity);
            }
            else if (name.compare("hole") == 0) {
                auto entity = EntityFactory::getInstance().create("hole", position, size);
                entity->parseBehavior(dictionary);
                this->addChild(entity);
                this->entities.pushBack(entity);
            }

        }
    }

    groupArray = map->getObjectGroup("special");

    if (groupArray != nullptr)
    {
        auto& objects = groupArray->getObjects();
        for (auto& obj : objects)
        {
            auto& dictionary = obj.asValueMap();

            std::string name = dictionary["name"].asString();
            auto size = cocos2d::Size(dictionary["width"].asFloat(), dictionary["height"].asFloat());
            auto position = calculateTmxPosition(dictionary, map);

            if (name.compare("door") == 0) {
                auto entity = EntityFactory::getInstance().create("door", position, size);
                entity->parseBehavior(dictionary);
                this->addChild(entity);
                this->entities.pushBack(entity);
                this->verticalDoors.pushBack(dynamic_cast<Door*>(entity));

            }
            else if (name.compare("horizontal_door") == 0) {
                auto entity = EntityFactory::getInstance().create("horizontal_door", position, size);
                entity->parseBehavior(dictionary);
                this->addChild(entity);
                this->entities.pushBack(entity);
                this->horizontalDoors.pushBack(dynamic_cast<Door*>(entity));
            }
            else if (name.compare("platform") == 0) {

                auto entity = EntityFactory::getInstance().create("platform", position, size);
                entity->parseBehavior(dictionary);
                this->addChild(entity);
                this->entities.pushBack(entity);
            }
            else if (name.compare("falling_block") == 0) {

                auto entryCollisionBox = EntityFactory::getInstance().getEntryCollisionRectangle(name, position, size);
                auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                    return EntityFactory::getInstance().create(name, position, size, 16);
                });

                this->objectManager->objectEntries.push_back(entry);

            }
            else if (name.compare("water") == 0) {

                auto entryCollisionBox = EntityFactory::getInstance().getEntryCollisionRectangle(name, position, size);
                auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                    return EntityFactory::getInstance().create(name, position, size, 16);
                });

                this->objectManager->objectEntries.push_back(entry);

            }
            else if (name.compare("spikes") == 0) {

                auto entryCollisionBox = EntityFactory::getInstance().getEntryCollisionRectangle(name, position, size);
                auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                    return EntityFactory::getInstance().create(name, position, size, 16);
                    });

                this->objectManager->objectEntries.push_back(entry);

            }


        }

    }


    groupArray = map->getObjectGroup("items");

    if(groupArray != nullptr)
    {
        auto collectibles = SaveManager::readSlot(SaveManager::defaultSlot).collectibles;

        auto& objects = groupArray->getObjects();
        for (auto& obj : objects)
        {
            auto& dictionary = obj.asValueMap();

            std::string name = dictionary["name"].asString();
            auto size = cocos2d::Size(dictionary["width"].asFloat(), dictionary["height"].asFloat());
            auto position = calculateTmxPosition(dictionary, map);

            auto itemName = dictionary.at("name").asString();
            auto itemType = dictionary.at("type").asString();

            bool collected = false;
            for (int i = 0; i < collectibles.size(); ++i) {
                auto collectible = collectibles[i];

                if (collectible.first.compare(itemName) == 0) {
                    if (collectible.second) {
                        collected = true;
                        break;
                    }
                }
            }

            if (collected) {
                continue;
            }

            auto entryCollisionBox = EntityFactory::getInstance().getEntryCollisionRectangle("item", position, size);

            auto entry = Logical::getEntry(entryCollisionBox, [=]() { 

                auto entity = EntityFactory::getInstance().create("item", position, size); 

                dynamic_cast<windy::Item*>(entity)->setup(itemName, itemType, true, true);

                return entity;

            });

            this->objectManager->objectEntries.push_back(entry);

        }

    }

    groupArray = map->getObjectGroup("enemies");

    if (groupArray != nullptr)
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
                name.compare("subeil") == 0 ||
                name.compare("sumatran") == 0 || 
                name.compare("taban") == 0 || 
                
                name.compare("barrel") == 0 || 
                name.compare("cow") == 0 || 
                name.compare("jetbird") == 0 || 
                name.compare("tremor") == 0 ||

                name.compare("roll_runner") == 0 ||
                name.compare("tank") == 0 ||
                name.compare("green_soldier") == 0 ||
                name.compare("red_soldier") == 0 ||
                name.compare("claw_soldier") == 0 ||

                name.compare("backpacker") == 0 ||
                name.compare("batcombat") == 0 ||
                name.compare("falling_skull") == 0 ||
                name.compare("flash_brain") == 0 ||
                name.compare("rip_jumper") == 0 ||
                name.compare("wall_spider") == 0 ||
                name.compare("wall_skull") == 0 ||
                name.compare("robot_worm") == 0) {

                auto entryCollisionBox = EntityFactory::getInstance().getEntryCollisionRectangle(name, position, size);
                auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                    return EntityFactory::getInstance().create(name, position, size);
                    });

                this->objectManager->objectEntries.push_back(entry);

            }
        }

    }


    groupArray = map->getObjectGroup("boss");

    if (groupArray != nullptr)
    {
        auto& objects = groupArray->getObjects();
        for (auto& obj : objects)
        {
            auto& dictionary = obj.asValueMap();

            std::string name = dictionary["name"].asString();
            auto size = cocos2d::Size(dictionary["width"].asFloat(), dictionary["height"].asFloat());
            auto position = calculateTmxPosition(dictionary, map);

            auto bossType = dictionary.at("type").asString();

            if (bossType.compare("sheriff") == 0){
                
                auto entryCollisionBox = EntityFactory::getInstance().getEntryCollisionRectangle(bossType, position, size);
                auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                    return EntityFactory::getInstance().create(bossType, position, size);
                    });

                this->objectManager->objectEntries.push_back(entry);

            } else if (bossType.compare("military") == 0) {

                auto entryCollisionBox = EntityFactory::getInstance().getEntryCollisionRectangle(bossType, position, size);
                auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                    return EntityFactory::getInstance().create(bossType, position, size);
                    });

                this->objectManager->objectEntries.push_back(entry);

            } else if (bossType.compare("vine") == 0) {

                auto entryCollisionBox = EntityFactory::getInstance().getEntryCollisionRectangle(bossType, position, size);
                auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                    return EntityFactory::getInstance().create(bossType, position, size);
                    });

                this->objectManager->objectEntries.push_back(entry);

            } else if (bossType.compare("night") == 0) {

                auto entryCollisionBox = EntityFactory::getInstance().getEntryCollisionRectangle(bossType, position, size);
                auto entry = Logical::getEntry(entryCollisionBox, [=]() {
                    return EntityFactory::getInstance().create(bossType, position, size);
                    });

                this->objectManager->objectEntries.push_back(entry);

            }


        }

    }

    if (firstCheckpoint == nullptr) {
        // must define initial checkpoint
        throw std::exception();
    }

    this->lastCheckpoint = firstCheckpoint;

    auto firstCheckpointPosition = firstCheckpoint->getPosition();

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

    this->bounds = dynamic_cast<Bounds*>(EntityFactory::getInstance().create("bounds", firstCheckpointPosition, visibleSize));

    this->addChild(this->bounds, 4096);

    this->bounds->recomputeCollisionRectangles();


    this->entities.pushBack(this->bounds);

    auto guiSize = cocos2d::Size(16, 16);

    this->gui = dynamic_cast<Gui*>(EntityFactory::getInstance().create("gui", cocos2d::Point(0, 0), guiSize));

    this->bounds->addChild(gui, 1024);


    auto playerSize = cocos2d::Size(15, 24);

    this->player = dynamic_cast<Player*>(EntityFactory::getInstance().create("player", firstCheckpointPosition, playerSize));

    this->addChild(this->player, 512);

    this->entities.pushBack(this->player);

    auto cameraSize = cocos2d::Size(8, 16);

    this->camera = dynamic_cast<Camera*>(EntityFactory::getInstance().create("camera", cocos2d::Point(0, 0), cameraSize, 48));

    this->camera->setPositionY(this->camera->collisionBox->size.height * 0.25f);

    this->entities.pushBack(this->camera);

    this->bounds->addChild(this->camera);

    this->camera->normalizeCollisionRectangles();

#if _DEBUG
    this->debugDrawNode = DebugDrawNode::create(this);

    this->addChild(this->debugDrawNode);

    this->debugDrawNode->customEntities.pushBack(this->bounds);
#endif

    this->levelController =
        dynamic_cast<LevelController*>(EntityFactory::getInstance().create("level_controller", cocos2d::Point(0, 0), cocos2d::Size(16, 16)));

    this->addChild(this->levelController);

    _initialObjectEntries = objectManager->objectEntries;

    startup();

    return true;
}


bool Level::getPaused() {
    return this->isPaused;
}

void Level::setPaused(bool isPaused, bool freezePlayer) {

    this->isPaused = isPaused;

    auto player = this->player;
    auto camera = this->camera;

    for (int i = 0; i < this->entities.size(); ++i) {
        auto entity = this->entities.at(i);

        bool isDoor = entity->getTag() == GameTags::General::Door;

        if (entity == player || entity == camera || isDoor) {
            continue;
        }

        if (isPaused) {
            entity->pause();
        }
        else {
            entity->resume();
        }

        if (entity->getTag() == GameTags::General::Enemy) {
            auto enemy = dynamic_cast<Enemy*>(entity);
            
            if (isPaused) {
                if (enemy->sprite != nullptr) {
                    enemy->sprite->pause();
                }
                
            }
            else {
                if (enemy->sprite != nullptr) {
                    enemy->sprite->resume();
                }
                
            }
            
        }

        if (entity->getTag() == GameTags::Weapon::WeaponEnemy || 
            entity->getTag() == GameTags::Weapon::WeaponPlayer ||
            entity->getTag() == GameTags::Weapon::WeaponNone) {
            auto weapon = dynamic_cast<Weapon*>(entity);

            if (isPaused) {
                weapon->sprite->pause();
            }
            else {
                weapon->sprite->resume();
            }

        }

        if (entity->getTag() == GameTags::General::Item) {
            auto item = dynamic_cast<Item*>(entity);
            if (isPaused) {
                item->sprite->pause();
            }
            else {
                item->sprite->resume();
            }
        }

    }

    if (freezePlayer) {
        if (isPaused) {
            this->player->pause();
            this->player->sprite->pause();
        }
        else {
            this->player->resume();
            this->player->sprite->resume();
        }
    }
}
void Level::resetGameplay() {
    this->bounds->setPosition(this->lastCheckpoint->getPosition());

    float playerOffsetY = this->lastCheckpoint->getPositionY() + this->player->sprite->getContentSize().height + 128;
    this->player->setPosition(cocos2d::Point(this->lastCheckpoint->getPositionX(), playerOffsetY));

    this->bounds->recomputeCollisionRectangles();
    this->camera->recomputeCollisionRectangles();
    this->player->recomputeCollisionRectangles();

    for (int i = 0; i < this->verticalDoors.size(); ++i) {
        auto door = this->verticalDoors.at(i);
        door->reset();
    }

    for (int i = 0; i < this->horizontalDoors.size(); ++i) {
        auto door = this->horizontalDoors.at(i);
        door->reset();
    }

    this->player->onRestart();

    //this->camera->synchronizeWithBounds();

    this->physicsWorld->setUnderwater(false);

    this->setPaused(false, true);

    this->objectManager->resetEntryTable(_initialObjectEntries);
}

void Level::startup() {
    resetGameplay();

    this->levelController->startup();
}

void Level::restart() {
    this->levelController->restart([this]() {
        resetGameplay();
    });
}

void Level::exitLevel() {
    this->levelController->exitLevel();
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
