#ifndef SPCGANESHAENGINE_GSCENE_H
#define SPCGANESHAENGINE_GSCENE_H

#include "ganeshatypes.h"
#include "gmatrix.h"
#include "gviewport.h"
#include "gcameradata.h"
#include "gshader.h"
#include "gscenenode.h"

namespace spcGaneshaEngine {

class GScene {
public:
    GScene();
    ~GScene();
    
    GViewport viewport;
    GCameraData cameraData;
    std::vector<std::shared_ptr<GSceneNode>> nodesArray;
    std::vector<std::shared_ptr<GMesh>> meshesArray;
    
    std::vector<std::shared_ptr<GVULKANImage>> materialsArray;
    std::vector<std::shared_ptr<GShader>> vertexShadersArray;
    std::vector<std::shared_ptr<GShader>> fragmentShadersArray;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSCENE_H
