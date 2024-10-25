#include "gscenenode.h"

namespace spcGaneshaEngine {

GSceneNode::GSceneNode(std::shared_ptr<GMesh> mesh,
                       const GVector& newTranslation,
                       const GVector& newRotation,
                       const GVector& newScale,
                       std::shared_ptr<GVULKANImage> newMaterial) : mesh(mesh),
                                                                    translation(newTranslation),
                                                                    rotation(newRotation),
                                                                    scale(newScale),
                                                                    material(newMaterial) {
}

GSceneNode::~GSceneNode() {
}

void GSceneNode::deploy(std::shared_ptr<GCommandServiceProtocol> commandService, GVULKANDevice& vulkanDevice) {
    std::vector<GVector> instancesArray;
    instancesArray.push_back(translation);
    instancesArray.push_back(rotation);
    instancesArray.push_back(scale);
    instanceBuffer.createBuffer(instancesArray.data(),
                                instancesArray.size() * sizeof(GVector),
                                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                true,
                                commandService,
                                vulkanDevice);
}

//GMesh *GSceneNode::getMesh() {
//    return mesh;
//}
//
//void GSceneNode::setupInstance(const GVector& newTranslation, const GVector& newRotation, const GVector& newScale, GVULKANImage *newMaterial) {
//    material = newMaterial;
//    translation = newTranslation;
//    scale = newScale;
//    rotation = newRotation;
//    
//    instanceBuffer.destroyBuffer(vulkanDevice.getLogicalDevice());
//    std::vector<GVector> instancesArray;
//    instancesArray.push_back(translation);
//    instancesArray.push_back(rotation);
//    instancesArray.push_back(scale);
//    instanceBuffer.createBuffer(instancesArray.data(),
//                                instancesArray.size() * sizeof(GVector),
//                                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
//                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//                                true,
//                                commandService,
//                                vulkanDevice);
//}
//
//GVULKANImage *GSceneNode::getMaterial() {
//    return material;
//}
//
//GVULKANBuffer& GSceneNode::getInstanceBuffer() {
//    return instanceBuffer;
//}

};
