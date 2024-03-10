#include "ggraphnode.h"

namespace spcGaneshaEngine {

GGraphNode::GGraphNode(GMesh *mesh, GDescriptorsetServiceProtocol *descriptorsetService, GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice) : mesh(mesh),
                                                                                                                                                                descriptorsetService(descriptorsetService),
                                                                                                                                                                commandService(commandService),
                                                                                                                                                                vulkanDevice(vulkanDevice) {
}

GGraphNode::~GGraphNode() {
}

TUInt GGraphNode::getInstancesCount() {
    return instancesArray.size() / 3;
}

void GGraphNode::addInstance(const GVector& translation, const GVector& rotation, const GVector& scale) {
    instancesArray.push_back(translation);
    instancesArray.push_back(rotation);
    instancesArray.push_back(scale);

    instanceBuffer.destroyBuffer(vulkanDevice.getLogicalDevice());
    instanceBuffer.createBuffer(instancesArray.data(),
                                instancesArray.size() * sizeof(GVector),
                                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                true,
                                commandService,
                                vulkanDevice);
}

};
