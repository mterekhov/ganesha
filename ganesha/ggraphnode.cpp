#include "ggraphnode.h"

namespace spcGaneshaEngine {

GGraphNode::GGraphNode(GNode *node) : node(node), rts(GMatrix::identityMatrix()) {
}

GGraphNode::~GGraphNode() {
}

};
