#include "VineBall.h"

#include "Windy/GameTags.h"

using namespace game;

game::Resources& VineBall::getResources() {
    static game::Resources resources{game::ResourceKind::Weapon, "vine_bullet"};
    return resources;
}

VineBall* VineBall::create() {
    VineBall* entity = new (std::nothrow) VineBall();

    if (entity && entity->init()) {
        entity->autorelease();
        return entity;
    }

    CC_SAFE_DELETE(entity);
    return nullptr;
}

std::shared_ptr<cocos2d::Rect> VineBall::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Logical::buildEntryCollisionRectangle<VineBall>(position, size);
}

bool VineBall::init() {
    if (!Node::init()) {
        return false;
    }

    return true;
}

void VineBall::setup() {
    power = 3;
    _onGround = false;

    Logical::composite<VineBall>(this);
    Logical::setup(this->getPosition(), this->collisionRectangles[0]->size);

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("vine_a",            "vine_bullet_a",         false,   0.10f),
        windy::AnimationAction("vine_b",            "vine_bullet_b",         true,   0.10f),
        windy::AnimationAction("vine_c",            "vine_bullet_c",         false,   0.04f),
        windy::AnimationAction("vine_c_reversed",   "vine_bullet_c",         false,   0.04f)
    };
    sprite->appendActionSet(actionSet, false);

    sprite->runAction("vine_c_reversed");
    sprite->reverseAction();

    sprite->stopActions();
                   

    sprite->setAnimation("vine_bullet_b");
    sprite->runAction("vine_b");    
}

void VineBall::onFinished() {
    finishForever();
}

void VineBall::fire(int power, int direction, windy::GameTags::Weapon tag, const cocos2d::Point& weaponSpeed) {

    this->power = power;

    bool flipX = direction == -1 ? true : false;

    if (flipX) {
        speed.x = -weaponSpeed.x;
    }
    else {
        speed.x = weaponSpeed.x;
    }

    speed.y = weaponSpeed.y;


    this->sprite->setFlippedX(flipX);

    float velocity = 4;

    this->setTag(tag);
}

void VineBall::onUpdate(float dt) {
    if (this->contacts[windy::CollisionContact::Down]) {
        if (!_onGround) {
            _onGround = true;

            speed.x = 0;

            auto preCallback = cocos2d::CallFunc::create([this]() {
                this->sprite->runAction("vine_c");

                float widthBackup = this->sprite->getContentSize().width;

                this->sprite->setAnimation("vine_bullet_c");

                
                this->collisionBox->origin.x += widthBackup * 0.5f;

                float newWidth = this->sprite->getContentSize().width * 0.75f;

                this->collisionBox->size.height = this->sprite->getContentSize().height * 0.85f;
                this->collisionBox->size.width = newWidth;
                
                this->collisionBox->origin.x += newWidth * 0.5f * -1;
            });

            auto postCallback = cocos2d::CallFunc::create([this]() {
                this->sprite->runAction("vine_c_reversed");
            });

            auto onEnd = cocos2d::CallFunc::create([this]() {
                this->finishForever();
            });

            auto duration = cocos2d::DelayTime::create(this->sprite->getActionDuration("vine_c"));

            auto sequence = cocos2d::Sequence::create(preCallback, duration, postCallback, duration, onEnd, nullptr);

            runAction(sequence);
        }
    }
}
