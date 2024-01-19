#ifndef SPCGANESHAENGINE_GGRAPHNODE_H
#define SPCGANESHAENGINE_GGRAPHNODE_H

#include "gnode.h"
#include "gmatrix.h"
#include "gvulkanbuffer.h"
#include "gdescriptorsetservice.h"
#include "gquaternion.h"
#include "gvector.h"

namespace spcGaneshaEngine {

class GGraphNode {
public:
    GGraphNode(GNode *node, GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice, VkCommandPool commandPool);
    ~GGraphNode();
    
    GNode *node;

    void updateRTS(GMatrix& newRTSMatrix, GVULKANDevice& vulkanDevice);

private:
    GMatrix rts;
    GVULKANBuffer rtsBuffer;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GGRAPHNODE_H
