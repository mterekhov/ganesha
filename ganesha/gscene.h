#ifndef SPCGANESHAENGINE_GSCENE_H
#define SPCGANESHAENGINE_GSCENE_H

#include "gcamera.h"
#include "gviewport.h"
#include "gshader.h"
#include "gmesh.h"
#include "gscenenode.h"
#include "gvulkanimage.h"

namespace spcGaneshaEngine {

class GScene {
public:
    GScene();
    ~GScene();
    
    GCamera camera;
    GViewport viewport;
    std::vector<std::shared_ptr<GSceneNode>> nodesArray;
    std::vector<std::shared_ptr<GMesh>> meshesArray;
    
    std::vector<std::shared_ptr<GVULKANImage>> materialsArray;
    std::vector<std::shared_ptr<GShader>> vertexShadersArray;
    std::vector<std::shared_ptr<GShader>> fragmentShadersArray;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSCENE_H
