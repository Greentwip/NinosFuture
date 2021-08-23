#include "SoundTestMenu.h"

#include "cocos/ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


#include "Windy/AudioManager.h"
#include "Windy/Settings.h"

using namespace game;

class SoundTestMenuResources {
public:
    static std::string dataFile;
};

std::string SoundTestMenuResources::dataFile = "sprites/gameplay/screens/sound_test/data.csb";

SoundTestMenu* SoundTestMenu::create() {

    SoundTestMenu* menu = new (std::nothrow) SoundTestMenu();

    if (menu) {
        menu->_root = nullptr;
    }

    if (menu && menu->init()) {
        menu->autorelease();
        return menu;
    }

    CC_SAFE_DELETE(menu);
    return nullptr;
}

bool SoundTestMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    auto root = cocos2d::CSLoader::createNode(SoundTestMenuResources::dataFile);

    addChild(root);

    _root = root;

    auto playButton = dynamic_cast<cocos2d::ui::Button*>(_root->getChildByName("play_button"));
    auto stopButton = dynamic_cast<cocos2d::ui::Button*>(_root->getChildByName("stop_button"));
    auto previousButton = dynamic_cast<cocos2d::ui::Button*>(_root->getChildByName("previous_button"));
    auto nextButton = dynamic_cast<cocos2d::ui::Button*>(_root->getChildByName("next_button"));
    auto saveButton = dynamic_cast<cocos2d::ui::Button*>(_root->getChildByName("save_button"));
    auto slider = dynamic_cast<cocos2d::ui::Slider*>(_root->getChildByName("slider"));
    auto currentSoundText = dynamic_cast<cocos2d::ui::Text*>(_root->getChildByName("current_sound"));
    auto amplitudeText = dynamic_cast<cocos2d::ui::Text*>(_root->getChildByName("amplitude"));

    auto soundKeyMap = windy::AudioManager::getSoundKeyMap();

    _currentTrackId = 0;

    _playing = false;

    windy::AudioManager::stopAll();


    for (int i = static_cast<int>(windy::Sounds::Intro); i <= static_cast<int>(windy::Sounds::NightMan); ++i) {
        _tracks.push_back(soundKeyMap[static_cast<windy::Sounds>(i)]);

        std::string trackString = _tracks.back();
        std::size_t found = trackString.find_last_of("/");
        std::string identifier = trackString.substr(found + 1);

        _trackIdentifiers.push_back(identifier);
    }

    _amplitudes = windy::Settings::read().soundAmplitudeValues;

    currentSoundText->setString(_trackIdentifiers[_currentTrackId]);
    amplitudeText->setString(std::to_string(_amplitudes[_tracks[_currentTrackId]]));

    float amplitude = _amplitudes[_tracks[_currentTrackId]];
    int percent = static_cast<int>((amplitude * 100) / 2.0f);

    slider->setPercent(percent);

    
    playButton->addClickEventListener([=](Ref*)
    {
        windy::AudioManager::stopAll();
        windy::AudioManager::playSfx(static_cast<windy::Sounds>(_currentTrackId));

        _playing = true;
    });

    stopButton->addClickEventListener([=](Ref*)
    {
        windy::AudioManager::stopAll();
        _playing = false;
    });

    nextButton->addClickEventListener([=](Ref*)
    {
        _currentTrackId++;
        if (_currentTrackId > static_cast<int>(windy::Sounds::NightMan)) {
            _currentTrackId = static_cast<int>(windy::Sounds::NightMan);
        }

        currentSoundText->setString(_trackIdentifiers[_currentTrackId]);
        amplitudeText->setString(std::to_string(_amplitudes[_tracks[_currentTrackId]]));

        float amplitude = _amplitudes[_tracks[_currentTrackId]];
        int percent = static_cast<int>((amplitude * 100) / 2.0f);

        slider->setPercent(percent);

        windy::AudioManager::stopAll();
        _playing = false;

    });

    previousButton->addClickEventListener([=](Ref*)
    {
        _currentTrackId--;
        if (_currentTrackId < static_cast<int>(windy::Sounds::Intro)) {
            _currentTrackId = static_cast<int>(windy::Sounds::Intro);
        }

        currentSoundText->setString(_trackIdentifiers[_currentTrackId]);
        amplitudeText->setString(std::to_string(_amplitudes[_tracks[_currentTrackId]]));

        float amplitude = _amplitudes[_tracks[_currentTrackId]];
        int percent = static_cast<int>((amplitude * 100) / 2.0f);
        
        slider->setPercent(percent);

        windy::AudioManager::stopAll();
        _playing = false;

    });

    saveButton->addClickEventListener([=](Ref*)
    {
        windy::Settings::soundAmplitudeValues = _amplitudes;
        windy::Settings::save();
    });

    slider->addEventListener([=](Ref* sender, cocos2d::ui::Slider::EventType type) {
        if (type == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            int percent = slider->getPercent();
            float amplitude = static_cast<float>(percent * 2) / 100.0f;
            _amplitudes[_tracks[_currentTrackId]] = amplitude;
            windy::Settings::soundAmplitudeValues = _amplitudes;

            amplitudeText->setString(std::to_string(amplitude));

            if (_playing) {
                windy::AudioManager::setSfxVolume(amplitude);
            }
        }
    });


    return true;
}

void SoundTestMenu::onEnter()
{
    Node::onEnter();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void SoundTestMenu::onExit()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Node::onExit();
}

void SoundTestMenu::update(float dt)
{


}