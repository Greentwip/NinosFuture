#include "Sprite.h"
#include "ImageFormat.h"
#include "Settings.h"
#include "Display.h"
#include "AnimationAction.h"
#include "GameTags.h"

#include "SpriteDefinition.h"

#include <vector>
#include <string>
#include <regex>
#include <map>

using namespace windy;

Sprite* Sprite::create(const std::string& spriteName, const cocos2d::Point& anchor) {



    auto dataFileName = spriteName + ".plist";
    std::string imageFileName = "";

    if (Settings::TextureFormat == ImageFormat::PVR) {
        imageFileName = spriteName + ".pvr.ccz";
    }

    auto plistDict = cocos2d::FileUtils::getInstance()->getValueMapFromFile(dataFileName);

    std::map<std::string, std::map<int, std::string> > frames;
    std::vector<std::string> animations;
    std::string currentAnimation = "";

    if(!cocos2d::SpriteFrameCache::getInstance()->isSpriteFramesWithFileLoaded(dataFileName)) {
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(dataFileName, imageFileName);
    }
            

    for(auto kp : plistDict["frames"].asValueMap()){
        auto k = kp.first;

        std::vector<std::string> tokens;

        std::regex e("([_]+)");
            
        std::sregex_token_iterator iter(k.begin(),
            k.end(),
            e,
            -1);
        std::sregex_token_iterator end;

        for (; iter != end; ++iter) {
            tokens.push_back(iter->str());
        }

        std::string animationName = "";

        for (unsigned i = 0; i < tokens.size() - 1; ++i) {
            if (animationName.compare("") != 0) {
                animationName = animationName + "_" + tokens[i];
            }
            else {
                animationName = tokens[i];
            }
        }
            
        if (currentAnimation != animationName) {
            currentAnimation = animationName;
            animations.push_back(currentAnimation);
        }
        

        frames[currentAnimation][atoi(tokens.at(tokens.size() - 1).c_str())] = k;

        //CCLOG("Frames content");
        //CCLOG(tokens.at(tokens.size() - 1).c_str());
        //CCLOG(frames[currentAnimation][atoi(tokens.at(tokens.size() - 1).c_str())].c_str());

    }

    /*CCLOG("KP");
    for (auto kp : frames[animations[0]]) {
        CCLOG(std::to_string(kp.first).c_str());
        CCLOG(kp.second.c_str());
    }

    CCLOG(frames[animations[0]][0].c_str());*/

    cocos2d::SpriteFrame* spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frames[animations[0]][0]);


    Sprite* sprite = new (std::nothrow) Sprite();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();

        sprite->frames = frames;
        sprite->animations = animations;
        sprite->imageIndex = 0;
        sprite->currentAnimation = animations[0];
        sprite->touchRange = 0;
        sprite->getTexture()->setAliasTexParameters();
        sprite->setPosition(Display::getInstance().center);
        sprite->defaultAnchor = anchor;
        sprite->actions.clear();
        sprite->currentAction = nullptr;
        sprite->cycles = 0;
        sprite->definitions.clear();
        sprite->setAnchorPoint(sprite->defaultAnchor);
        return sprite;
    }

    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Sprite::increaseOrLoopFrame() {
    int imageIndex = this->imageIndex + 1;
    if (imageIndex >= this->frames[this->currentAnimation].size()) {
        imageIndex = 0;;
    }
    this->setSpriteFrame(this->frames[this->currentAnimation][imageIndex]);

    this->imageIndex = imageIndex;
}

Sprite* Sprite::setAnimation(const std::string& animationName) {
    if (animationName.compare(this->currentAnimation) != 0) {
        if (this->frames.find(animationName) != this->frames.end()) {
            this->currentAnimation = animationName;
            this->setImageIndex(0);
        }
    }
    return this;
}
    
cocos2d::Vector<cocos2d::SpriteFrame*> Sprite::getAnimationFrames(const std::string animationName) {
    cocos2d::Vector<cocos2d::SpriteFrame*> frames;

    for (auto kv : this->frames[animationName]) {
        frames.pushBack(cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(kv.second));
    }

    return frames;
}
    
int Sprite::getCurrentAnimationNumberOfFrames() {
    return (int)this->frames[this->currentAnimation].size();
}


void Sprite::setImageIndex(int imageIndex) {
    this->imageIndex = imageIndex;
    this->setSpriteFrame(this->frames[this->currentAnimation][imageIndex]);
}

int Sprite::getImageIndex() {
    return this->imageIndex;
}

std::string Sprite::getSpriteFrameName() {
    return this->frames[this->currentAnimation][this->imageIndex];
}
    
bool Sprite::checkTouch(const cocos2d::Point& point) {
    float dx = point.x - this->getPositionX();
    float dy = point.y - this->getPositionY();
    float offset = (float) std::sqrt(dx * dx + dy * dy);
    return offset <= this->touchRange;
}    

void Sprite::appendAction(AnimationAction action, bool prependAction, std::string baseName) {
    std::string animationName;

    if (prependAction) {
        animationName = action.name + "/" + action.animationName;
    }
    else {
        animationName = action.animationName;
    }

    auto animationFrames = cocos2d::Animation::createWithSpriteFrames(this->getAnimationFrames(animationName), action.delay);

    cocos2d::ActionInterval* animation = nullptr;

    auto animationSequence =
        cocos2d::Sequence::create(cocos2d::Animate::create(animationFrames),
            cocos2d::CallFunc::create([this]() {
                this->cycles = this->cycles + 1;
                }), nullptr);

    if (action.forever) {
        animation = cocos2d::RepeatForever::create(animationSequence);
    }
    else {
        animation = animationSequence;
    }

    animation->setDuration(action.delay * static_cast<float>(this->getAnimationFrames(animationName).size()));

    animation->setTag(GameTags::Actions::Animation);

    std::string animationBaseName = "";

    if (baseName.compare("") != 0) {
        animationBaseName = baseName + "_";
    }

    this->actions.insert(animationBaseName + action.name, animation);
}

void Sprite::appendActionSet(std::vector<AnimationAction> set, bool prependAction, std::string baseName) {
    for (auto action : set) {
        this->appendAction(action, prependAction, baseName);
    }
}

void Sprite::loadDefinitions(const std::string& path) {
    if(cocos2d::FileUtils::getInstance()->isFileExist(path + ".plist")) {
        SpriteDefinition spriteDefinition(path);

        for (auto kp : spriteDefinition.definitions) {
            auto definitionName = kp.first;
            auto definition = kp.second;

            // make shared or plain stack objects
            this->definitions[definitionName] = {
                definition.anchor,
                definition.size
            };
        }
    }

}

SpriteDefinition Sprite::getDefinition(const std::string& name) {
    return this->definitions[name];
}

cocos2d::ActionInterval* Sprite::getCurrentAction() {
    return this->currentAction;
}

cocos2d::ActionInterval* Sprite::getAction(const std::string& name, const std::string& baseName) {
    if (baseName.compare("") != 0) {
        return this->actions.at(baseName + "_" + name);
    }
    else {
        return this->actions.at(name);
    }
}



void Sprite::runAction(const std::string& name, const std::string& baseName) {
    cocos2d::ActionInterval* newAction;
    cocos2d::Point definitionAnchor;

    bool foundDefinitionAnchor = false;
    if (baseName.compare("") != 0) {
        if (this->definitions.find(baseName + "_" + name) != this->definitions.end()) {
            definitionAnchor = this->getDefinition(baseName + "_" + name).anchor;
            foundDefinitionAnchor = true;
        }
        
    }
    else {

        if (this->definitions.find(name) != this->definitions.end()) {
            definitionAnchor = this->getDefinition(name).anchor;
            foundDefinitionAnchor = true;
        }
        
    }

    if (!foundDefinitionAnchor) {
        definitionAnchor = this->defaultAnchor;
    }


    if (baseName.compare("") != 0) {
        newAction = this->getAction(name, baseName);
    }
    else {
        newAction = this->getAction(name);
    }

    if (this->currentAction != newAction) {
        this->currentAction = newAction;
        this->stopAllActionsByTag(GameTags::Actions::Animation);
        if (this->isFlippedX()) {
            definitionAnchor.x = 1.0f - definitionAnchor.x;
        }

        this->setAnchorPoint(definitionAnchor);

        cocos2d::Sprite::runAction(this->currentAction);
    }

    this->currentAction = newAction;
}

float Sprite::getActionDuration(const std::string& name, const std::string& baseName) {
    if (baseName.compare("") != 0) {
        return this->actions.at(baseName + "_" + name)->getDuration();
    }
    else {
        return this->actions.at(name)->getDuration();
    }

}

void Sprite::reverseAction() {
    auto previousAction = this->currentAction;
    previousAction->retain();
    this->currentAction = this->currentAction->reverse();
    this->stopAllActionsByTag(GameTags::Actions::Animation);

    std::shared_ptr<std::string> foundName = std::make_shared<std::string>();

    for (auto it = actions.begin(); it != actions.end();)
    {
        auto element = *it;

        if (element.second == previousAction) {
            *foundName = element.first;
            actions.erase(it++);
            break;
        }
        else {
            (++it);
        }
    }

    this->actions.erase(*foundName);
    this->actions.insert(*foundName, this->currentAction);

    cocos2d::Sprite::runAction(this->currentAction);
    previousAction->release();

}

void Sprite::pauseActions() {
    if (this->getActionByTag(GameTags::Actions::Animation) != nullptr) {
        cocos2d::Sprite::stopAllActionsByTag(GameTags::Actions::Animation);
    }
}

void Sprite::resumeActions() {

    if (this->getActionByTag(GameTags::Actions::Animation) == nullptr) {
        cocos2d::Sprite::runAction(this->currentAction);
    }
}


void Sprite::stopActions() {
    this->currentAction = nullptr;
    cocos2d::Sprite::stopAllActionsByTag(GameTags::Actions::Animation);
}
