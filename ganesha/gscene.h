#ifndef SPCGANESHAENGINE_GSCENE_H
#define SPCGANESHAENGINE_GSCENE_H

#include "ganeshatypes.h"
#include "gmatrix.h"
#include "gviewport.h"
#include "gcameradata.h"
#include "gmesh.h"
#include "gscenenode.h"
#include "gmaterialsservice.h"

namespace spcGaneshaEngine {

class GScene {
public:
    GScene();
    ~GScene();
    
    GViewport viewport;
    GCameraData cameraData;
    std::vector<std::shared_ptr<GSceneNode>> nodesArray;
    std::vector<std::shared_ptr<GMesh>> meshesArray;
    TStringsArray materialsArray;
    TStringsArray vertexShadersArray;
    TStringsArray fragmentShadersArrray;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSCENE_H
