#include <iostream>
#include <set>
#include <array>
#include <math.h>

#include "gvulkanapi.h"
#include "gbundle.h"
#include "gpoint2d.h"
#include "gcolor.h"
#include "gmatrix.h"
#include "gvulkanbuffer.h"
#include "gvulkantools.h"

namespace spcGaneshaEngine {

static const TCharPointersArray khronosValidationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

static const TStringsArray avoidInstanceExtensions = {
    VK_LUNARG_DIRECT_DRIVER_LOADING_EXTENSION_NAME
};

const TStringsArray useDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    "VK_KHR_portability_subset"
};

GVULKANAPI::GVULKANAPI() : log("Ganesha"),
vulkanInstance(log),
vulkanDevice(log),
vulkanSwapChain(log),
vulkanPipeline(log),
vertexesBuffer(log),
indexesBuffer(log),
vulkanDescriptorset(log),
texture(log) {
    
}

GVULKANAPI::~GVULKANAPI() {
    
}

#pragma mark - GGraphicsAPIProtocol -

void GVULKANAPI::initAPI(void *metalLayer, const TUInt frameWidth, const TUInt frameHeight, const GRenderGraph& renderGraph) {
    //  create VULKAN instance
    vulkanInstance.createInstance("DOOM", khronosValidationLayers, avoidInstanceExtensions);
    
    //  create surface
    metalSurface = createSurface(metalLayer);
    
    //  creates physicalDevice
    vulkanDevice.createDevice(vulkanInstance, useDeviceExtensions, metalSurface);

    vulkanSwapChain.createSwapChain(frameWidth, frameHeight, vulkanDevice, metalSurface);
    commandPool = createCommandPool(vulkanDevice);
    
    TUInt framebuffersNumber = static_cast<TUInt>(vulkanSwapChain.framebuffersNumber());
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    for (TUInt i = 0; i < framebuffersNumber; i++) {
        GVULKANBuffer newBuffer(log);
        UniformBufferObject ubo = currentUBO();
        newBuffer.createBuffer(&ubo,
                               bufferSize,
                               VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               false,
                               vulkanDevice,
                               commandPool);
        vulkanUniformBuffers.push_back(newBuffer);
    }
    createTextures();
    vulkanDescriptorset.createDescriptorsets(vulkanDevice, vulkanUniformBuffers, texture);

    vulkanPipeline.createPipeline(vulkanDevice, vulkanSwapChain, vulkanDescriptorset.getDescriptorsetLayout());
    
    std::vector<Vertex> vertexesArray = renderGraph.getVertexesArray();
    vertexesBuffer.createBuffer(vertexesArray.data(),
                                sizeof(Vertex) * vertexesArray.size(),
                                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                true,
                                vulkanDevice,
                                commandPool);
    
    TIndexArray indexesArray = renderGraph.getIndexesArray();
    indexesBuffer.createBuffer(indexesArray.data(),
                               sizeof(TIndex) * indexesArray.size(),
                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                               true,
                               vulkanDevice,
                               commandPool);
    
    
    for (TUInt i = 0; i < framebuffersNumber; i++) {
        VkCommandBufferAllocateInfo allocInfo = { };
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        
        VkCommandBuffer newCommand;
        vkAllocateCommandBuffers(vulkanDevice.getLogicalDevice(), &allocInfo, &newCommand);
        renderCommands.push_back(newCommand);
    }
    
    createSemaphores();
}

void GVULKANAPI::destroyAPI() {
    vkDeviceWaitIdle(vulkanDevice.getLogicalDevice());
    
    texture.destroyImage(vulkanDevice);
    
    vulkanSwapChain.destroySwapChain(vulkanDevice);
    vulkanPipeline.destroyPipeline(vulkanDevice);
    
    for (size_t i = 0; i < vulkanUniformBuffers.size(); i++) {
        vulkanUniformBuffers[i].destroyBuffer(vulkanDevice);
    }
    
    vulkanDescriptorset.destroyDescriptorsets(vulkanDevice);
    
    indexesBuffer.destroyBuffer(vulkanDevice);
    vertexesBuffer.destroyBuffer(vulkanDevice);
    
    for (size_t i = 0; i < maxFramesInFlight; i++) {
        vkDestroySemaphore(vulkanDevice.getLogicalDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(vulkanDevice.getLogicalDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(vulkanDevice.getLogicalDevice(), inFlightFences[i], nullptr);
    }
    
    vkDestroyCommandPool(vulkanDevice.getLogicalDevice(), commandPool, nullptr);
    vulkanDevice.destroyDevice();
    vkDestroySurfaceKHR(vulkanInstance.getVulkanInstance(), metalSurface, nullptr);
    vulkanInstance.destroyInstance();
}

void GVULKANAPI::frameResized(const float width, const float height) {
    vulkanSwapChain.updateScreenSize(width, height, vulkanDevice, metalSurface);
    installIsometricView(fov, nearPlane, farPlane);
}

void GVULKANAPI::drawFrame(GRenderGraph& renderGraph) {
    VkResult result = vkWaitForFences(vulkanDevice.getLogicalDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    
    TUInt imageIndex;
    result = vkAcquireNextImageKHR(vulkanDevice.getLogicalDevice(), vulkanSwapChain.getVulkanSwapChain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(vulkanDevice.getLogicalDevice(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];
    
    UniformBufferObject ubo = currentUBO();
    vulkanUniformBuffers[imageIndex].refreshBuffer(&ubo, vulkanDevice);
    
    vkResetCommandBuffer(renderCommands[imageIndex], 0);
    recordRenderCommand(renderCommands[imageIndex],
                        vertexesBuffer.getBuffer(),
                        indexesBuffer.getBuffer(),
                        static_cast<TUInt>(renderGraph.getIndexesArray().size()),
                        vulkanSwapChain.getFramebuffers()[imageIndex],
                        vulkanSwapChain,
                        vulkanPipeline,
                        vulkanDescriptorset.getDescriptorsetArray()[imageIndex]);
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.pWaitDstStageMask = waitStages;
    
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &renderCommands[imageIndex];
    
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    
    vkResetFences(vulkanDevice.getLogicalDevice(), 1, &inFlightFences[currentFrame]);
    
    if (vkQueueSubmit(vulkanDevice.getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        log.error("failed to submit draw command buffer\n");
    }
    
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    
    VkSwapchainKHR swapChains[] = { vulkanSwapChain.getVulkanSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    vkQueuePresentKHR(vulkanDevice.getPresentQueue(), &presentInfo);

    currentFrame = (currentFrame + 1) % maxFramesInFlight;
}

void GVULKANAPI::installIsometricView(const TFloat fieldOfView, const TFloat near, const TFloat far) {
    VkExtent2D swapChainExtent = vulkanSwapChain.getExtent();
    TFloat aspect = static_cast<TFloat>(swapChainExtent.width) / static_cast<TFloat>(swapChainExtent.height);
    TFloat size = near * tanf(fieldOfView / 2.0);
    TFloat aspectHeight = aspect * size;
    
    nearPlane = near;
    farPlane = far;
    fov = fieldOfView;
    projectionMatrix = GMatrix::frustum(-aspectHeight, aspectHeight, -size, size, near, far);
}

void GVULKANAPI::installViewMatrix(const GMatrix& newViewMatrix) {
    viewMatrix = newViewMatrix;
}

#pragma mark - Routine -

void GVULKANAPI::recordRenderCommand(VkCommandBuffer renderCommand,
                                          VkBuffer vertexesBuffer,
                                          VkBuffer indexesBuffer,
                                          const TUInt indexesNumber,
                                          VkFramebuffer framebuffer,
                                          GVULKANSwapChain& swapChain,
                                          GVULKANPipeline& pipeline,
                                          VkDescriptorSet descriptorset) {
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
        
        std::array<VkClearValue, 2> clearValues = { };
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};
        
        renderPassInfo.clearValueCount = clearValues.size();
        renderPassInfo.pClearValues = clearValues.data();
        
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

void GVULKANAPI::createSemaphores() {
    imageAvailableSemaphores.resize(maxFramesInFlight);
    renderFinishedSemaphores.resize(maxFramesInFlight);
    inFlightFences.resize(maxFramesInFlight);
    imagesInFlight.resize(vulkanSwapChain.framebuffersNumber(), VK_NULL_HANDLE);
    
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    
    for (size_t i = 0; i < maxFramesInFlight; i++) {
        if (vkCreateSemaphore(vulkanDevice.getLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(vulkanDevice.getLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(vulkanDevice.getLogicalDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            log.error("failed to create semaphores\n");
            return;
        }
    }
}

VkCommandPool GVULKANAPI::createCommandPool(GVULKANDevice& device) {
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

VkSurfaceKHR GVULKANAPI::createSurface(void *metalLayer) {
    VkMetalSurfaceCreateInfoEXT metalSurfaceInfo = {};
    
    metalSurfaceInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
    metalSurfaceInfo.pNext = NULL;
    metalSurfaceInfo.flags = 0;
    metalSurfaceInfo.pLayer = metalLayer;
    
    VkSurfaceKHR newSurface;
    if (vkCreateMetalSurfaceEXT(vulkanInstance.getVulkanInstance(), &metalSurfaceInfo, NULL, &newSurface) != VK_SUCCESS) {
        log.error("error creating metal surface\n");
    }
    
    return newSurface;
}

UniformBufferObject GVULKANAPI::currentUBO() {
    static auto startTime = std::chrono::high_resolution_clock::now();
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    
    UniformBufferObject ubo = {};
    
    ubo.model = GMatrix::rotationY(time * M_PI_2);
    ubo.view = viewMatrix;
    ubo.proj = projectionMatrix;
    
    return ubo;
}

void GVULKANAPI::createTextures() {
    GTGA tgaFile("/Users/cipher/Development/ganesha/resources/MWALL4_1.tga");
    texture.createImage({ tgaFile.getWidth(), tgaFile.getHeight() },
                        VK_FORMAT_R8G8B8A8_SRGB,
                        VK_IMAGE_ASPECT_COLOR_BIT,
                        VK_IMAGE_TILING_OPTIMAL,
                        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                        vulkanDevice);
    texture.deployData(tgaFile,
                       vulkanDevice,
                       commandPool);
}


}
