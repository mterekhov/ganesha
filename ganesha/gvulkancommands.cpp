#include "gvulkancommands.h"

namespace spcGaneshaEngine {

GVULKANCommands::GVULKANCommands(GLog& log) : log(log) {
    
}

GVULKANCommands::~GVULKANCommands() {
    
}

void GVULKANCommands::createCommands(GVULKANDevice& device) {
    commandPool = createCommandPool(device);
}

void GVULKANCommands::recordRenderCommand(VkCommandBuffer& renderCommand,
                                          VkBuffer& vertecesBuffer,
                                          VkBuffer& indicesBuffer,
                                          TUInt indicesNumber,
                                          VkFramebuffer& framebuffer,
                                          GVULKANSwapChain& swapChain,
                                          GVULKANPipeline& pipeline,
                                          VkDescriptorSet& descriptorset) {
    VkCommandBufferBeginInfo beginInfo = { };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    
    if (vkBeginCommandBuffer(renderCommand, &beginInfo) != VK_SUCCESS) {
        log.error("failed to begin recording command buffer\n");
    }
    
    VkExtent2D swapChainExtent = swapChain.getExtent();
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain.getRenderPass();
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChainExtent;
    
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    
    vkCmdBeginRenderPass(renderCommand, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    vkCmdBindPipeline(renderCommand, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getGraphicsPipeline());
    
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<TFloat>(swapChainExtent.width);
    viewport.height = static_cast<TFloat>(swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(renderCommand, 0, 1, &viewport);
    
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(renderCommand, 0, 1, &scissor);
    
    VkBuffer vertexBuffers[] = { vertecesBuffer };
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(renderCommand, 0, 1, vertexBuffers, offsets);
    
    vkCmdBindIndexBuffer(renderCommand, indicesBuffer, 0, VK_INDEX_TYPE_UINT32);
    
    vkCmdBindDescriptorSets(renderCommand, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipelineLayout(), 0, 1, &descriptorset, 0, nullptr);
    vkCmdDrawIndexed(renderCommand, indicesNumber, 1, 0, 0, 0);
    
    vkCmdEndRenderPass(renderCommand);
    
    if (vkEndCommandBuffer(renderCommand) != VK_SUCCESS) {
        log.error("failed to record command buffer\n");
    }
}

void GVULKANCommands::destroyCommands(GVULKANDevice& device) {
    vkDestroyCommandPool(device.getLogicalDevice(), commandPool, nullptr);
}

VkCommandPool& GVULKANCommands::getCommandPool() {
    return commandPool;
}

#pragma mark - Routine -

VkCommandPool GVULKANCommands::createCommandPool(GVULKANDevice& device) {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
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

}   //  namespace spcGaneshaEngine
