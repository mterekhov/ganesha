#ifndef SPCGANESHAENGINE_GGRAPHNODE_H
#define SPCGANESHAENGINE_GGRAPHNODE_H

#include "gnode.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

class GGraphNode {
public:
    GGraphNode(GNode *node);
    ~GGraphNode();
    
    GMatrix rtsMatrix;
    GNode *node;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GGRAPHNODE_H
