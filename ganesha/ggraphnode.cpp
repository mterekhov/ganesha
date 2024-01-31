#include "ggraphnode.h"

namespace spcGaneshaEngine {

GGraphNode::GGraphNode(GMesh *mesh) : mesh(mesh), rts(GMatrix::identityMatrix()) {
}

GGraphNode::~GGraphNode() {
}

};
