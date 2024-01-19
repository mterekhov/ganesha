#include "ggraphnode.h"

namespace spcGaneshaEngine {

GGraphNode::GGraphNode(GNode *node, GDescriptorsetServiceProtocol *descriptorsetService, GVULKANDevice& vulkanDevice, VkCommandPool commandPool) : node(node), rts(GMatrix::identityMatrix()) {
    rtsBuffer.createBuffer(static_cast<void *>(&rts),
                           sizeof(rts),
                           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           false,
                           vulkanDevice,
                           commandPool);
    descriptorsetService->attachBufferToDescriptorset(rtsBuffer, 1, vulkanDevice.getLogicalDevice());
}

GGraphNode::~GGraphNode() {
}

void GGraphNode::updateRTS(GMatrix& newRTSMatrix, GVULKANDevice& vulkanDevice) {
    rts = newRTSMatrix;
    rtsBuffer.refreshBuffer(&rts, vulkanDevice);
}

};
