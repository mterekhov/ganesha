//
//  gvulkandevice.cpp
//  game
//
//  Created by cipher on 20.05.2022.
//

#include "gvulkancommands.h"

namespace spcGaneshaEngine {

GVULKANCommands::GVULKANCommands(GLog& log) : log(log) {
    
}

GVULKANCommands::~GVULKANCommands() {
    
}

void GVULKANCommands::createCommands(GVULKANDevice& device, GVULKANSwapChain& swapChain, GVULKANPipeline& pipline, const GRenderGraph& renderGraph) {
    commandPool = createCommandPool(device);
}

void GVULKANCommands::recordRenderCommand(VkCommandBuffer& renderCommand,
                                          VkBuffer& vertecesBuffer,
                                          VkBuffer& indicesBuffer,
                                          uint32_t indicesNumber,
                                          VkFramebuffer& framebuffer,
                                          GVULKANSwapChain& swapChain,
                                          GVULKANPipeline& pipeline) {
    VkCommandBufferBeginInfo beginInfo = { };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    
    if (vkBeginCommandBuffer(renderCommand, &beginInfo) != VK_SUCCESS) {
        log.error("failed to begin recording command buffer\n");
    }
    
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain.getRenderPass();
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain.getExtent();
    
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    
    vkCmdBeginRenderPass(renderCommand, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    vkCmdBindPipeline(renderCommand, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getGraphicsPipeline());
    
    VkBuffer vertexBuffers[] = { vertecesBuffer };
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(renderCommand, 0, 1, vertexBuffers, offsets);
    
    vkCmdBindIndexBuffer(renderCommand, indicesBuffer, 0, VK_INDEX_TYPE_UINT16);
    
    vkCmdBindDescriptorSets(renderCommand, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipelineLayout(), 0, 1, &descriptorSets[i], 0, nullptr);
    vkCmdDrawIndexed(renderCommand, indicesNumber, 1, 0, 0, 0);
    
    vkCmdEndRenderPass(renderCommand);
    
    if (vkEndCommandBuffer(renderCommand) != VK_SUCCESS) {
        log.error("failed to record command buffer\n");
    }
}

void GVULKANCommands::destroyCommands(GVULKANDevice& device) {
    vkDestroyCommandPool(device.getLogicalDevice(), commandPool, nullptr);

//    vkDestroyBuffer(device.getLogicalDevice(), vertexBuffer, nullptr);
//    vkFreeMemory(device.getLogicalDevice(), vertexBufferMemory, nullptr);
//
//    vkDestroyBuffer(device.getLogicalDevice(), indexBuffer, nullptr);
//    vkFreeMemory(device.getLogicalDevice(), indexBufferMemory, nullptr);
}

VkCommandPool& GVULKANCommands::getCommandPool() {
    return commandPool;
}

#pragma mark - Routine -

VkCommandPool GVULKANCommands::createCommandPool(GVULKANDevice& device) {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = device.getGraphicsQueueIndex();
    
    VkCommandPool newCommandPool;
    if (vkCreateCommandPool(device.getLogicalDevice(), &poolInfo, nullptr, &newCommandPool) != VK_SUCCESS) {
        log.error("failed to create command pool\n");
    }
    
    return newCommandPool;
}

VkCommandBuffer GVULKANCommands::emptyCommand(const VkDevice& device) {
    VkCommandBufferAllocateInfo allocInfo = { };
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
    
    VkCommandBuffer newCommandBuffer;
    if (vkAllocateCommandBuffers(device, &allocInfo, &newCommandBuffer) != VK_SUCCESS) {
        log.error("failed to create command buffer\n");
    }

    return newCommandBuffer;
}

VkCommandBuffer GVULKANCommands::copyBufferCommand(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, const VkDevice& device) {
    VkCommandBuffer commandBuffer = emptyCommand(device);

    VkCommandBufferBeginInfo beginInfo = { };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    vkEndCommandBuffer(commandBuffer);

    return commandBuffer;
}

void GVULKANCommands::destroyCommandBuffer(VkCommandBuffer& commandBuffer, const VkDevice& device) {
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

//VkBuffer GVULKANCommands::createIndicesBuffer(GVULKANDevice& vulkanDevice, const std::vector<uint32_t>& indices) {
//    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
//    VkBuffer stagingBuffer;
//    VkDeviceMemory stagingBufferMemory;
//    createBuffer(vulkanDevice,
//                 bufferSize,
//                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
//                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//                 stagingBuffer,
//                 stagingBufferMemory);
//    
//    void *data;
//    vkMapMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
//    memcpy(data, indices.data(), (size_t) bufferSize);
//    vkUnmapMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory);
//    
//    createBuffer(vulkanDevice,
//                 bufferSize,
//                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
//                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//                 indexBuffer,
//                 indexBufferMemory);
//    
//    copyBuffer(vulkanDevice, stagingBuffer, indexBuffer, bufferSize);
//    
//    vkDestroyBuffer(vulkanDevice.getLogicalDevice(), stagingBuffer, nullptr);
//    vkFreeMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory, nullptr);
//}
//
//  Uniform staging:
//  VkBufferUsageFlags usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
//  VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,

//  Indices staging:
//  VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT
//  VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
//
//  Indices final:
//  VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
//  VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
//
//  Verteces staging
//  VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT
//  VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
//
//  Verteces final
//  VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
//  VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT

//VkBuffer GVULKANCommands::createVertexBuffer(GVULKANDevice& vulkanDevice, const std::vector<uint32_t>& vertices) {
//    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
//    VkBuffer stagingBuffer;
//    VkDeviceMemory stagingBufferMemory;
//    createBuffer(vulkanDevice,
//                 bufferSize,
//                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
//                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//                 stagingBuffer,
//                 stagingBufferMemory);
//    
//    void *data;
//    vkMapMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
//    memcpy(data, vertices.data(), (size_t) bufferSize);
//    vkUnmapMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory);
//    
//    createBuffer(vulkanDevice,
//                 bufferSize,
//                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
//                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//                 vertexBuffer,
//                 vertexBufferMemory);
//    
//    copyBuffer(vulkanDevice, stagingBuffer, vertexBuffer, bufferSize);
//    
//    vkDestroyBuffer(vulkanDevice.getLogicalDevice(), stagingBuffer, nullptr);
//    vkFreeMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory, nullptr);
//}
//
//void GVULKANCommands::createBuffer(GVULKANDevice& vulkanDevice,
//                                   VkDeviceSize size,
//                                   VkBufferUsageFlags usage,
//                                   VkMemoryPropertyFlags properties,
//                                   VkBuffer& buffer,
//                                   VkDeviceMemory& bufferMemory) {
//    VkBufferCreateInfo bufferInfo = { };
//    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//    bufferInfo.size = size;
//    bufferInfo.usage = usage;
//    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//    if (vkCreateBuffer(vulkanDevice.getLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
//        log.error("failed to create vertex buffer\n");
//    }
//    
//    VkMemoryRequirements memoryRequirements;
//    vkGetBufferMemoryRequirements(vulkanDevice.getLogicalDevice(), buffer, &memoryRequirements);
//    
//    VkMemoryAllocateInfo memoryAllocateInfo = { };
//    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//    memoryAllocateInfo.allocationSize = memoryRequirements.size;
//    memoryAllocateInfo.memoryTypeIndex = findMemoryType(vulkanDevice.getPhysicalDevice(), memoryRequirements.memoryTypeBits, properties);
//    if (vkAllocateMemory(vulkanDevice.getLogicalDevice(), &memoryAllocateInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
//        log.error("failed to allocate vertex buffer memory\n");
//        return;
//    }
//    vkBindBufferMemory(vulkanDevice.getLogicalDevice(), buffer, bufferMemory, 0);
//}
//
//uint32_t GVULKANCommands::findMemoryType(const VkPhysicalDevice& device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
//    VkPhysicalDeviceMemoryProperties memoryProperties;
//    vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);
//    
//    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
//        if ((typeFilter & (1 << i)) &&
//            (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
//            return i;
//        }
//    }
//    
//    return 0;
//}
//
//void GVULKANCommands::copyBuffer(GVULKANDevice& vulkanDevice, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
//    VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
//    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//    commandBufferAllocateInfo.commandPool = commandPool;
//    commandBufferAllocateInfo.commandBufferCount = 1;
//    
//    VkCommandBuffer commandBuffer;
//    vkAllocateCommandBuffers(vulkanDevice.getLogicalDevice(), &commandBufferAllocateInfo, &commandBuffer);
//
//    VkCommandBufferBeginInfo beginInfo{};
//    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
//
//    vkBeginCommandBuffer(commandBuffer, &beginInfo);
//    VkBufferCopy copyRegion{};
//    copyRegion.srcOffset = 0; // Optional
//    copyRegion.dstOffset = 0; // Optional
//    copyRegion.size = size;
//    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
//    vkEndCommandBuffer(commandBuffer);
//
//    VkSubmitInfo submitInfo{};
//    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//    submitInfo.commandBufferCount = 1;
//    submitInfo.pCommandBuffers = &commandBuffer;
//    vkQueueSubmit(vulkanDevice.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
//    vkQueueWaitIdle(vulkanDevice.getGraphicsQueue());
//    
//    vkFreeCommandBuffers(vulkanDevice.getLogicalDevice(), commandPool, 1, &commandBuffer);
//}
//
//std::vector<VkCommandBuffer> GVULKANCommands::createCommandBuffers(GVULKANDevice& device,
//                                                                   GVULKANSwapChain& swapChain,
//                                                                   GVULKANPipeline& pipeline,
//                                                                   VkCommandPool& currentCommandPool,
//                                                                   const GRenderGraph& renderGraph) {
//    uint32_t commandBuffersNumber = static_cast<uint32_t>(swapChain.getFramebuffers().size());
//    std::vector<VkCommandBuffer> newCommandBuffersArray;
//    newCommandBuffersArray.resize(commandBuffersNumber);
//
//    VkCommandBufferAllocateInfo allocInfo{};
//    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//    allocInfo.commandPool = currentCommandPool;
//    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffersNumber);
//
//    if (vkAllocateCommandBuffers(device.getLogicalDevice(), &allocInfo, newCommandBuffersArray.data()) != VK_SUCCESS) {
//        log.error("failed to create command buffer\n");
//    }
//
//    for (size_t i = 0; i < commandBuffersNumber; i++) {
//        VkCommandBufferBeginInfo beginInfo{};
//        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//
//        if (vkBeginCommandBuffer(newCommandBuffersArray[i], &beginInfo) != VK_SUCCESS) {
//            log.error("failed to begin recording command buffer\n");
//        }
//
//        VkRenderPassBeginInfo renderPassInfo{};
//        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//        renderPassInfo.renderPass = swapChain.getRenderPass();
//        renderPassInfo.framebuffer = swapChain.getFramebuffers()[i];
//        renderPassInfo.renderArea.offset = {0, 0};
//        renderPassInfo.renderArea.extent = swapChain.getExtent();
//
//        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
//        renderPassInfo.clearValueCount = 1;
//        renderPassInfo.pClearValues = &clearColor;
//
//        vkCmdBeginRenderPass(newCommandBuffersArray[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
//
//        vkCmdBindPipeline(newCommandBuffersArray[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getGraphicsPipeline());
//
//        VkBuffer vertexBuffers[] = { vertexBuffer };
//        VkDeviceSize offsets[] = {0};
//        vkCmdBindVertexBuffers(newCommandBuffersArray[i], 0, 1, vertexBuffers, offsets);
//
//        vkCmdBindIndexBuffer(newCommandBuffersArray[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);
//
//        vkCmdBindDescriptorSets(newCommandBuffersArray[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipelineLayout(), 0, 1, &descriptorSets[i], 0, nullptr);
//        vkCmdDrawIndexed(newCommandBuffersArray[i], static_cast<uint32_t>(renderGraph.getIndecesArray().size()), 1, 0, 0, 0);
//
//        vkCmdEndRenderPass(newCommandBuffersArray[i]);
//
//        if (vkEndCommandBuffer(newCommandBuffersArray[i]) != VK_SUCCESS) {
//            log.error("failed to record command buffer\n");
//        }
//    }
//
//    return newCommandBuffersArray;
//}

}   //  namespace spcGaneshaEngine
