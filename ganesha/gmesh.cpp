#include "gmesh.h"

namespace spcGaneshaEngine {

GMesh::GMesh(const std::vector<TFloat>& vertexesArray,
             const TIndexArray& indexesArray) : vertexesArray(vertexesArray), indexesArray(indexesArray) {
}

GMesh::~GMesh() {
}

bool GMesh::isDeployed() {
    if (indexesArray.empty() && vertexesArray.empty()) {
        return true;
    }
    
    return false;
}

void GMesh::deploy(GCommandServiceProtocol *commandService,
                   GVULKANDevice& vulkanDevice) {
    vertexesBuffer.createBuffer(vertexesArray.data(),
                                sizeof(TFloat) * vertexesArray.size(),
                                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                true,
                                commandService,
                                vulkanDevice);
    
    indexesBuffer.createBuffer(indexesArray.data(),
                               sizeof(TIndex) * indexesArray.size(),
                               VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                               true,
                               commandService,
                               vulkanDevice);
    vertexesArray.clear();
    indexesArray.clear();
}

void GMesh::destroyMesh(VkDevice device) {
    vertexesBuffer.destroyBuffer(device);
    indexesBuffer.destroyBuffer(device);
}

void GMesh::render(TUInt instancesNumber, VkCommandBuffer renderCommand) {
    VkBuffer vertexBuffers[] = { vertexesBuffer.getBuffer() };
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(renderCommand, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(renderCommand, indexesBuffer.getBuffer(), 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(renderCommand, indexesBuffer.getBufferSize() / sizeof(TIndex), instancesNumber, 0, 0, 0);
}

};
