#include "gspritenode.h"

namespace spcGaneshaEngine {

GSpriteNode::GSpriteNode(GVULKANImage *material, GVULKANDevice& vulkanDevice, GCommandServiceProtocol *commandService) : material(material) {
    const std::vector<TFloat> vertexesArray = {
        -0.5f, -0.3, -0.5f, 2.0f, 0.0f,
        0.5f, -0.3, -0.5f,  0.0f, 0.0,
        0.5f, -0.3, 0.5f,   0.0f, 2.0,
        -0.5f, -0.3, 0.5f,  2.0f, 2.0,
    };
    vertexesBuffer.createBuffer(vertexesArray.data(),
                                sizeof(TFloat) * vertexesArray.size(),
                                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                true,
                                vulkanDevice,
                                commandService);
    
    const TIndexArray indexesArray = {
        2, 1, 0, 0, 3, 2
    };
    indexesBuffer.createBuffer(indexesArray.data(),
                               sizeof(TIndex) * indexesArray.size(),
                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                               true,
                               vulkanDevice,
                               commandService);
}

GSpriteNode::~GSpriteNode() {
}

void GSpriteNode::destroyNode(VkDevice device) {
    vertexesBuffer.destroyBuffer(device);
    indexesBuffer.destroyBuffer(device);
}

void GSpriteNode::render(VkCommandBuffer renderCommand) {
    VkBuffer vertexBuffers[] = { vertexesBuffer.getBuffer() };
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(renderCommand, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(renderCommand, indexesBuffer.getBuffer(), 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(renderCommand, indexesBuffer.getBufferSize() / sizeof(TIndex), 1, 0, 0, 0);
}

};
