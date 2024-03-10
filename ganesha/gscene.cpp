#include "gscene.h"

namespace spcGaneshaEngine {

GScene::GScene() {
    
}

GScene::~GScene() {
    
}

void GScene::addSprite(const std::string& materialFilePath) {
    spritesMaterialsArray.push_back(materialFilePath);
}

void GScene::addFragmentShader(const std::string& shaderFilePath) {
    fragmetShadersArrray.push_back(shaderFilePath);
}

void GScene::addVertexShader(const std::string& shaderFilePath) {
    vertexShadersArray.push_back(shaderFilePath);
}

const TStringsArray& GScene::getSpritesMaterialsArray() const {
    return spritesMaterialsArray;
}

const TStringsArray& GScene::getVertexShadersArray() const {
    return vertexShadersArray;
}

const TStringsArray& GScene::getFragmetShadersArrray() const {
    return fragmetShadersArrray;
}

}   //  namespace spcGaneshaEngine
