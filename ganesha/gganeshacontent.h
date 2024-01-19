#ifndef SPCGANESHAENGINE_GGANESHACONTENT_H
#define SPCGANESHAENGINE_GGANESHACONTENT_H

#include "ganeshatypes.h"

namespace spcGaneshaEngine {

class GGaneshaContent {
public:
    GGaneshaContent();
    ~GGaneshaContent();
    
    void addSprite(const std::string& materialFilePath);
    void addFragmentShader(const std::string& shaderFilePath);
    void addVertexShader(const std::string& shaderFilePath);
    
    TStringsArray& getSpritesMaterialsArray();
    TStringsArray& getVertexShadersArray();
    TStringsArray& getFragmetShadersArrray();

private:
    TStringsArray spritesMaterialsArray;
    TStringsArray vertexShadersArray;
    TStringsArray fragmetShadersArrray;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GGANESHACONTENT_H
