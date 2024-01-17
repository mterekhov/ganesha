#include "gganeshacontent.h"

namespace spcGaneshaEngine {

GGaneshaContent::GGaneshaContent() {
    
}

GGaneshaContent::~GGaneshaContent() {
    
}

void GGaneshaContent::addSprite(const std::string& materialFilePath) {
    spritesMaterialsArray.push_back(materialFilePath);
}

void GGaneshaContent::addFragmentShader(const std::string& shaderFilePath) {
    fragmetShadersArrray.push_back(shaderFilePath);
}

void GGaneshaContent::addVertexShader(const std::string& shaderFilePath) {
    vertexShadersArray.push_back(shaderFilePath);
}

TStringsArray& GGaneshaContent::getSpritesMaterialsArray() {
    return spritesMaterialsArray;
}

TStringsArray& GGaneshaContent::getVertexShadersArray() {
    return vertexShadersArray;
}

TStringsArray& GGaneshaContent::getFragmetShadersArrray() {
    return fragmetShadersArrray;
}

}   //  namespace spcGaneshaEngine
