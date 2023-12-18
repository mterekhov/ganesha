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

namespace spcGaneshaEngine {

static const TCharPointersArray khronosValidationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

static const TStringsArray avoidInstanceExtensions = {
    VK_LUNARG_DIRECT_DRIVER_LOADING_EXTENSION_NAME
};

struct UniformBufferObject {
    GMatrix model;
    GMatrix view;
    GMatrix proj;
};

GVULKANAPI::GVULKANAPI() : log("Ganesha"), vulkanInstance(log), vulkanDevice(log), vulkanSwapChain(log), vulkanPipeline(log), vulkanCommands(log) {
    
}

GVULKANAPI::~GVULKANAPI() {
    
}

#pragma mark - GGraphicsAPIProtocol -

void GVULKANAPI::initAPI(void *metalLayer, const uint32_t frameWidth, const uint32_t frameHeight, const GRenderGraph& renderGraph) {
    updateFrameSize = false;

    //  create VULKAN instance
    vulkanInstance.addInstanceExtensionsToAvoid(avoidInstanceExtensions);
    vulkanInstance.createInstance("DOOM", khronosValidationLayers);
    
    //  create surface
    metalSurface = createSurface(metalLayer);
        
    //  creates physicalDevice
    vulkanDevice.createDevice(vulkanInstance, metalSurface);
    vulkanSwapChain.createSwapChain(frameWidth, frameHeight, vulkanDevice, metalSurface);

    createDescriptorSetLayout();
    vulkanPipeline.createPipeline(vulkanDevice, vulkanSwapChain);
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();

    vulkanCommands.createCommands(vulkanDevice, vulkanSwapChain, vulkanPipeline, renderGraph);

    std::vector<Vertex> vertecesArray = renderGraph.getVertecesArray();
    vertecesBuffer.createBuffer(vertecesArray.data(), vertecesArray.size(),
                                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                true,
                                vulkanDevice,
                                vulkanCommands);

    std::vector<uint32_t> indecesArray = renderGraph.getIndecesArray();
    indecesBuffer.createBuffer(indecesArray.data(), indecesArray.size(),
                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                true,
                                vulkanDevice,
                                vulkanCommands);
    
    renderCommands.resize(vulkanSwapChain.framebuffersNumber());
    for (uint32_t i = 0; i < vulkanSwapChain.framebuffersNumber(); i++) {
        renderCommands[i] = vulkanCommands.emptyCommand(vulkanDevice.getLogicalDevice());
    }

    createSemaphores();
}

void GVULKANAPI::destroyAPI() {
    for (size_t i = 0; i < maxFramesInFlight; i++) {
        vkDestroySemaphore(vulkanDevice.getLogicalDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(vulkanDevice.getLogicalDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(vulkanDevice.getLogicalDevice(), inFlightFences[i], nullptr);
    }
    
    vkDestroyCommandPool(vulkanDevice.getLogicalDevice(), vulkanCommands.getCommandPool(), nullptr);
    vulkanPipeline.destroyPipeline(vulkanDevice);
    vulkanSwapChain.destroySwapChain(vulkanDevice);
    
    for (size_t i = 0; i < vulkanSwapChain.framebuffersNumber(); i++) {
        vkDestroyBuffer(vulkanDevice.getLogicalDevice(), uniformBuffers[i], nullptr);
        vkFreeMemory(vulkanDevice.getLogicalDevice(), uniformBuffersMemory[i], nullptr);
    }
    vkDestroyDescriptorPool(vulkanDevice.getLogicalDevice(), descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(vulkanDevice.getLogicalDevice(), descriptorSetLayout, nullptr);
    
    vulkanCommands.destroyCommands(vulkanDevice);    
    vulkanDevice.destroyDevice();
    vkDestroySurfaceKHR(vulkanInstance.getVulkanInstance(), metalSurface, nullptr);
    vulkanInstance.destroyInstance();
}

void GVULKANAPI::frameResized(const float width, const float height) {
    updateFrameSize = true;
    vulkanSwapChain.updateScreenSize(width, height, vulkanDevice, metalSurface);
}

void GVULKANAPI::drawFrame(GRenderGraph& renderGraph) {
    VkResult result = vkWaitForFences(vulkanDevice.getLogicalDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    result = vkAcquireNextImageKHR(vulkanDevice.getLogicalDevice(), vulkanSwapChain.getVulkanSwapChain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(vulkanDevice.getLogicalDevice(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];
    
    updateUniformBuffer(imageIndex);
    
    vkResetCommandBuffer(renderCommands[imageIndex], 0);
    vulkanCommands.recordRenderCommand(renderCommands[imageIndex],
                                       vertecesBuffer.getBuffer(),
                                       indecesBuffer.getBuffer(),
                                       static_cast<uint32_t>(renderGraph.getIndecesArray().size()),
                                       vulkanSwapChain.getFramebuffers()[imageIndex],
                                       vulkanSwapChain,
                                       vulkanPipeline);
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
    result = vkQueuePresentKHR(vulkanDevice.getPresentQueue(), &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || updateFrameSize) {
        updateFrameSize = false;
    } else if (result != VK_SUCCESS) {
        log.error("failed to present frame\n");
    }
    
    currentFrame = (currentFrame + 1) % maxFramesInFlight;
}

void GVULKANAPI::installIsometricView(const TFloat fieldOfView, const TFloat near, const TFloat far) {
    VkExtent2D swapChainExtent = vulkanSwapChain.getExtent();
    TFloat aspect = static_cast<TFloat>(swapChainExtent.width) / static_cast<TFloat>(swapChainExtent.height);
    TFloat size = near * tanf(fieldOfView / 2.0);
    TFloat aspectHeight = aspect * size;
    projectionMatrix = GMatrix::frustum(-aspectHeight, aspectHeight, -size, size, near, far);
}

void GVULKANAPI::installViewMatrix(const GMatrix& newViewMatrix) {
    viewMatrix = newViewMatrix;
}

#pragma mark - Routine -

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

void GVULKANAPI::createDescriptorPool() {
    uint32_t framebuffersNumber = static_cast<uint32_t>(vulkanSwapChain.framebuffersNumber());

    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = framebuffersNumber;
    
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = framebuffersNumber;
    if (vkCreateDescriptorPool(vulkanDevice.getLogicalDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        printf("GaneshaEngine: failed to create descriptor pool\n");
    }
}

void GVULKANAPI::createDescriptorSets() {
    uint32_t framebuffersNumber = static_cast<uint32_t>(vulkanSwapChain.framebuffersNumber());

    std::vector<VkDescriptorSetLayout> layouts(framebuffersNumber, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = framebuffersNumber;
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(framebuffersNumber);
    if (vkAllocateDescriptorSets(vulkanDevice.getLogicalDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        printf("GaneshaEngine: failed to allocate descriptor sets\n");
    }

    for (size_t i = 0; i < framebuffersNumber; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr; // Optional
        descriptorWrite.pTexelBufferView = nullptr; // Optional
        
        vkUpdateDescriptorSets(vulkanDevice.getLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
    }
    
}

void GVULKANAPI::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    
    uniformBuffers.resize(vulkanSwapChain.framebuffersNumber());
    uniformBuffersMemory.resize(vulkanSwapChain.framebuffersNumber());
    
    for (size_t i = 0; i < vulkanSwapChain.framebuffersNumber(); i++) {
        createBuffer(bufferSize, 
                     VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     uniformBuffers[i],
                     uniformBuffersMemory[i]);
    }
}
    
void GVULKANAPI::updateUniformBuffer(uint32_t currentImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    
    UniformBufferObject ubo{};
    ubo.model = GMatrix::rotationZ(time * M_PI_2);

    ubo.view = viewMatrix;
    ubo.proj = projectionMatrix;

    void* data;
    vkMapMemory(vulkanDevice.getLogicalDevice(), uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(vulkanDevice.getLogicalDevice(), uniformBuffersMemory[currentImage]);
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
            printf("GaneshaEngine: failed to create semaphores\n");
            return;
        }
    }
}

#pragma mark - Pipeline -

void GVULKANAPI::createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(vulkanDevice.getLogicalDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        printf("GaneshaEngine: failed to create descriptor set layout\n");
    }
    
}

}
