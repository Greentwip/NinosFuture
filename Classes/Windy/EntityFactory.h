#ifndef __WINDY_ENTITY_FACTORY_H__
#define __WINDY_ENTITY_FACTORY_H__

#include <string>
#include <map>
#include <functional>

#include "cocos2d.h"

#include "Entities/Logical.h"

namespace windy {
    class Level;
}

namespace windy {

    class EntityFactory
    {
    protected:
        Level* levelInstance;
        
    public:
        static EntityFactory& getInstance() {
            static EntityFactory instance;
            return instance;
        }

        void initialize(Level* level) {
            this->levelInstance = level;
        }

    private:
        EntityFactory(){
            this->levelInstance = nullptr;
            this->_createFuncs.clear();
            this->_collisionFuncs.clear();
        }

    public:
        template <typename TDerived>
        void registerType(std::string name)
        {
            _createFuncs[name] = &createFunc<TDerived>;
        }

        template <typename TDerived>
        void registerTypeCollisionFunc(std::string name)
        {
            _collisionFuncs[name] = &collisionFunc<TDerived>;
        }


        Logical* create(std::string name, const cocos2d::Point& position, const cocos2d::Size& size) {
            typename std::map<std::string, PCreateFunc>::const_iterator it = _createFuncs.find(name);
            if (it != _createFuncs.end()) {
                return it->second(levelInstance, position, size);
            }
            return nullptr;
        }


        std::shared_ptr<cocos2d::Rect> getEntryCollisionRectangle(std::string name, const cocos2d::Point& position, const cocos2d::Size& size) {
            typename std::map<std::string, PCollisionFunc>::const_iterator it = _collisionFuncs.find(name);
            if (it != _collisionFuncs.end()) {
                return it->second(position, size);
            }
            return nullptr;
        }

        void clear() {
            this->_createFuncs.clear();
            this->_collisionFuncs.clear();
        }

    private:
        template <typename TDerived>
        static Logical* createFunc(Level* level, const cocos2d::Point& position, const cocos2d::Size& size)
        {
            return Logical::create<TDerived>(level, position, size);
        }

        template <typename TDerived>
        static std::shared_ptr<cocos2d::Rect> collisionFunc(const cocos2d::Point& position, const cocos2d::Size& size)
        {
            return TDerived::getEntryCollisionRectangle(position, size);
        }

        typedef Logical* (*PCreateFunc)(Level* level, const cocos2d::Point& position, const cocos2d::Size& size);
        std::map<std::string, PCreateFunc> _createFuncs;

        typedef std::shared_ptr<cocos2d::Rect>(*PCollisionFunc)(const cocos2d::Point& position, const cocos2d::Size& size);
        std::map<std::string, PCollisionFunc> _collisionFuncs;
    };
}

#endif
