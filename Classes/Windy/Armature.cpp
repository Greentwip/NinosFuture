#include "Armature.h"

#include <regex>
#include <vector>

using namespace windy;

std::map<std::string, cocos2d::ValueMap> s_armaturePlistCache;

std::map<std::string, std::map<std::string, Armature>> s_definitionCache;

void Armature::clearPlistCache() {
    s_armaturePlistCache.clear();
    s_definitionCache.clear();
}

void Armature::cache(const std::string& dataFileName) {
    cocos2d::ValueMap plistDict;

    if (s_armaturePlistCache.find(dataFileName) == s_armaturePlistCache.end()) {
        plistDict = cocos2d::FileUtils::getInstance()->getValueMapFromFile(dataFileName + ".plist");
        s_armaturePlistCache[dataFileName] = plistDict;
    }
    else {
        plistDict = s_armaturePlistCache[dataFileName];
    }

    if (s_definitionCache.find(dataFileName) == s_definitionCache.end()) {

        for (auto bkvp : plistDict["bodies"].asValueMap()) {
            auto bodyName = bkvp.first;
            auto body = bkvp.second.asValueMap();

            auto anchor = Armature::pointFromString(body.at("anchorpoint").asString());

            std::vector<std::shared_ptr<cocos2d::Rect>> collisionRectangles;

            for (auto fixtureValue : body.at("fixtures").asValueVector()) {

                auto fixture = fixtureValue.asValueMap();

                assert(fixture.at("fixture_type").asString().compare("POLYGON") == 0); // Other fixture types are not supported

                auto polygonsVector = fixture.at("polygons").asValueVector();

                assert(polygonsVector.size() == 1); // One polygon per fixture supported

                auto vertexStrings = polygonsVector.at(0).asValueVector();

                assert(vertexStrings.size() == 4); // Only rectangle shaped polygons are supported

                std::vector<cocos2d::Point> vertices;

                for (int i = 0; i < 4; ++i) {
                    std::string vertexString = vertexStrings[i].asString();
                    vertices.push_back(Armature::pointFromString(vertexString));
                }


                float minX = 0.0f;
                float maxX = 0.0f;

                float minY = 0.0f;
                float maxY = 0.0f;

                for (int i = 0; i < vertices.size(); ++i) {
                    auto vertex = vertices[i];

                    if (vertex.x < minX) {
                        minX = vertex.x;
                    }

                    if (vertex.x > maxX) {
                        maxX = vertex.x;
                    }

                    if (vertex.y < minY) {
                        minY = vertex.y;
                    }

                    if (vertex.y > maxY) {
                        maxY = vertex.y;
                    }
                }


                collisionRectangles.push_back(std::make_shared<cocos2d::Rect>(minX, minY, maxX - minX, maxY - minY));

            }

            auto definitionToStore = Armature(anchor, collisionRectangles);

            s_definitionCache[dataFileName][bodyName] = definitionToStore;
        }

    }
}


Armature::Armature() : anchor(cocos2d::Point(0, 0)) {

}

Armature::Armature(cocos2d::Point anchor, std::vector<std::shared_ptr<cocos2d::Rect>> collisionRectangles) {
    this->anchor = anchor;
    this->collisionRectangles = collisionRectangles;
}

Armature::Armature(const std::string& dataFileName) {
    cocos2d::ValueMap plistDict;

    if (s_armaturePlistCache.find(dataFileName) == s_armaturePlistCache.end()) {
        plistDict = cocos2d::FileUtils::getInstance()->getValueMapFromFile(dataFileName + ".plist");
        s_armaturePlistCache[dataFileName] = plistDict;
    }
    else {
        plistDict = s_armaturePlistCache[dataFileName];
    }

    if (s_definitionCache.find(dataFileName) == s_definitionCache.end()) {

        for (auto bkvp : plistDict["bodies"].asValueMap()) {
            auto bodyName = bkvp.first;
            auto body = bkvp.second.asValueMap();

            auto anchor = Armature::pointFromString(body.at("anchorpoint").asString());

            std::vector<std::shared_ptr<cocos2d::Rect>> collisionRectangles;

            for (auto fixtureValue : body.at("fixtures").asValueVector()) {

                auto fixture = fixtureValue.asValueMap();

                assert(fixture.at("fixture_type").asString().compare("POLYGON") == 0); // Other fixture types are not supported

                auto polygonsVector = fixture.at("polygons").asValueVector();

                assert(polygonsVector.size() == 1); // One polygon per fixture supported

                auto vertexStrings = polygonsVector.at(0).asValueVector();

                assert(vertexStrings.size() == 4); // Only rectangle shaped polygons are supported

                std::vector<cocos2d::Point> vertices;

                for (int i = 0; i < 4; ++i) {
                    std::string vertexString = vertexStrings[i].asString();
                    vertices.push_back(Armature::pointFromString(vertexString));
                }


                float minX = 0.0f;
                float maxX = 0.0f;

                float minY = 0.0f;
                float maxY = 0.0f;

                for (int i = 0; i < vertices.size(); ++i) {
                    auto vertex = vertices[i];

                    if (vertex.x < minX) {
                        minX = vertex.x;
                    }

                    if (vertex.x > maxX) {
                        maxX = vertex.x;
                    }

                    if (vertex.y < minY) {
                        minY = vertex.y;
                    }

                    if (vertex.y > maxY) {
                        maxY = vertex.y;
                    }
                }


                collisionRectangles.push_back(std::make_shared<cocos2d::Rect>(minX, minY, maxX - minX, maxY - minY));

            }

            auto definitionToStore = Armature(anchor, collisionRectangles);

            this->definitions[bodyName] = definitionToStore;
        }

        s_definitionCache[dataFileName] = this->definitions;
    }
    else {
        this->definitions = s_definitionCache[dataFileName];
    }

}


cocos2d::Point Armature::pointFromString(const std::string& pointString) {
    std::vector<float> stringPoints;

    std::regex e("(-*[0-9]+.[0-9]+)");

    std::string mutableString = pointString;

    std::regex_token_iterator<std::string::iterator> iter(mutableString.begin(), mutableString.end(), e);
    std::regex_token_iterator<std::string::iterator> end;

    for (; iter != end; ++iter) {
        stringPoints.push_back(static_cast<float>(std::atof(iter->str().c_str())));
    }

    return cocos2d::Point((float)stringPoints[0], (float)stringPoints[1]);
}

Armature::Armature(Armature const& other) {
    this->anchor = other.anchor;
    this->collisionRectangles = other.collisionRectangles;
}

cocos2d::Size Armature::sizeFromString(const std::string& sizeString) {
    auto point = Armature::pointFromString(sizeString);
    return cocos2d::Size(point.x, point.y);
}

Armature Armature::get(const std::string& name) {
    return this->definitions[name];
}

