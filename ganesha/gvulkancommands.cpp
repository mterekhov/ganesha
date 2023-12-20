#include "gvulkancommands.h"

namespace spcGaneshaEngine {

GVULKANCommands::GVULKANCommands(GLog& log) : log(log) {
    
}

GVULKANCommands::~GVULKANCommands() {
    
}

void GVULKANCommands::createCommands(GVULKANDevice& device) {
    commandPool = createCommandPool(device);
}

VkCommandBuffer GVULKANCommands::emptyCommand(VkDevice device) {
    VkCommandBufferAllocateInfo allocInfo = { };
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
    
    VkCommandBuffer newCommandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &newCommandBuffer);

    return newCommandBuffer;
}

VkCommandBuffer GVULKANCommands::copyBufferCommand(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device) {
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

VkCommandBuffer GVULKANCommands::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkDevice device) {
    VkCommandBuffer commandBuffer = beginSingleCommand(device);

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = { width, height, 1 };

    vkCmdCopyBufferToImage(commandBuffer,buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    
    endSingleCommand(commandBuffer);
    
    return commandBuffer;
}

void GVULKANCommands::submitCommand(VkCommandBuffer command, GVULKANDevice& vulkanDevice) {
    VkSubmitInfo submitInfo = { };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &command;

    vkQueueSubmit(vulkanDevice.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(vulkanDevice.getGraphicsQueue());

    vkFreeCommandBuffers(vulkanDevice.getLogicalDevice(), commandPool, 1, &command);
}

VkCommandBuffer GVULKANCommands::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkDevice device) {
    VkCommandBuffer commandBuffer = beginSingleCommand(device);

    VkImageMemoryBarrier barrier = { };
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage = 0;
    VkPipelineStageFlags destinationStage = 0;
    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    
    if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }

    vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    
    endSingleCommand(commandBuffer);
    
    return commandBuffer;
}

void GVULKANCommands::recordRenderCommand(VkCommandBuffer renderCommand,
                                          VkBuffer vertexesBuffer,
                                          VkBuffer indexesBuffer,
                                          const TUInt indexesNumber,
                                          VkFramebuffer& framebuffer,
                                          GVULKANSwapChain& swapChain,
                                          GVULKANPipeline& pipeline,
                                          VkDescriptorSet& descriptorset) {
    VkCommandBufferBeginInfo beginInfo = { };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    
    vkBeginCommandBuffer(renderCommand, &beginInfo);
        
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
            
            VkBuffer vertexBuffers[] = { vertexesBuffer };
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(renderCommand, 0, 1, vertexBuffers, offsets);
            
            vkCmdBindIndexBuffer(renderCommand, indexesBuffer, 0, VK_INDEX_TYPE_UINT32);
            
            vkCmdBindDescriptorSets(renderCommand, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipelineLayout(), 0, 1, &descriptorset, 0, nullptr);
            vkCmdDrawIndexed(renderCommand, indexesNumber, 1, 0, 0, 0);
        
        vkCmdEndRenderPass(renderCommand);
        
    vkEndCommandBuffer(renderCommand);
}

void GVULKANCommands::destroyCommands(GVULKANDevice& device) {
    vkDestroyCommandPool(device.getLogicalDevice(), commandPool, nullptr);
}

#pragma mark - Routine -

VkCommandBuffer GVULKANCommands::beginSingleCommand(VkDevice device) {
    VkCommandBufferAllocateInfo allocInfo = { };
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);
    
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    
    return commandBuffer;
}

void GVULKANCommands::endSingleCommand(VkCommandBuffer command) {
    vkEndCommandBuffer(command);
}

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

void GVULKANCommands::destroyCommandBuffer(VkCommandBuffer commandBuffer, VkDevice device) {
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

}   //  namespace spcGaneshaEngine
