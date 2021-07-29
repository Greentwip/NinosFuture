#ifndef __WINDY_ENTITY_FACTORY_H__
#define __WINDY_ENTITY_FACTORY_H__

#include <string>
#include <map>
#include <functional>

#include "cocos2d.h"

namespace windy {
    class Logical;
    class Level;
}

namespace windy {

    template <typename T, typename U>
    class ConcreteEntityFactory
    {
        static Level* levelInstance;

    public:
        static void initialize(Level* level) {
            levelInstance = level;
        }

        static ConcreteEntityFactory<Logical, std::string>& getInstance() {
            static ConcreteEntityFactory<Logical, std::string> instance;
            return instance;
        }

    public:
        template <typename TDerived>
        void registerType(U name)
        {
            _createFuncs[name] = &createFunc<TDerived>;
        }

        T* create(std::string name, const cocos2d::Point& position, const cocos2d::Size& size) {
            typename std::map<U, PCreateFunc>::const_iterator it = _createFuncs.find(name);
            if (it != _createFuncs.end()) {
                return it->second(levelInstance, position, size);
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

        typedef T* (*PCreateFunc)(Level* level, const cocos2d::Point& position, const cocos2d::Size& size);
        std::map<U, PCreateFunc> _createFuncs;
    };

    typedef ConcreteEntityFactory<Logical, std::string> EntityFactory;

    Level* EntityFactory::levelInstance = nullptr;
}

#endif
