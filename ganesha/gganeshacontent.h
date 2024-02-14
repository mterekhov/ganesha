#ifndef SPCGANESHAENGINE_GGANESHACONTENT_H
#define SPCGANESHAENGINE_GGANESHACONTENT_H

#include "ganeshatypes.h"
#include "gmatrix.h"
#include "gviewport.h"
#include "gcameradata.h"

namespace spcGaneshaEngine {

class GGaneshaContent {
public:
    GGaneshaContent();
    ~GGaneshaContent();
    
    void addSprite(const std::string& materialFilePath);
    void addFragmentShader(const std::string& shaderFilePath);
    void addVertexShader(const std::string& shaderFilePath);
    
    const TStringsArray& getSpritesMaterialsArray() const;
    const TStringsArray& getVertexShadersArray() const;
    const TStringsArray& getFragmetShadersArrray() const;

    GViewport viewport;
    GCameraData cameraData;

private:
    TStringsArray spritesMaterialsArray;
    TStringsArray vertexShadersArray;
    TStringsArray fragmetShadersArrray;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GGANESHACONTENT_H
