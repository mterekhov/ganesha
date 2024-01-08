#include "ggraphnode.h"

namespace spcGaneshaEngine {

GGraphNode::GGraphNode(GNode *node) : node(node), rtsMatrix(GMatrix::identityMatrix()) {
    
}

GGraphNode::~GGraphNode() {
}

};
