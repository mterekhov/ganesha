#ifndef SPCGANESHAENGINE_GGRAPHNODE_H
#define SPCGANESHAENGINE_GGRAPHNODE_H

#include "gmesh.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

class GGraphNode {
public:
    GGraphNode(GMesh *mesh);
    ~GGraphNode();
    
    GMesh *mesh;
    GMatrix rts;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GGRAPHNODE_H
