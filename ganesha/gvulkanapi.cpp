#include <iostream>
#include <set>
#include <array>
#include <math.h>

#include "gvulkanapi.h"
#include "gbundle.h"
#include "gpoint2d.h"
#include "gcolor.h"
#include "gmatrix.h"
#include "ganeshastubdata.h"

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

GVULKANAPI::GVULKANAPI() : log("Ganesha"), vulkanInstance(log), vulkanDevice(log), vulkanSwapChain(log), vulkanPipeline(log) {
    
}

GVULKANAPI::~GVULKANAPI() {
    
}

#pragma mark - GGraphicsAPIProtocol -

void GVULKANAPI::initAPI(void *metalLayer, const uint32_t frameWidth, const uint32_t frameHeight) {
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
    
    commandPool = vulkanDevice.createCommandPool();
    
    createVertexBuffer();
    createIndicesBuffer();
    createCommandBuffers();
    createSemaphores();
}

void GVULKANAPI::destroyAPI() {
    for (size_t i = 0; i < maxFramesInFlight; i++) {
        vkDestroySemaphore(vulkanDevice.getLogicalDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(vulkanDevice.getLogicalDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(vulkanDevice.getLogicalDevice(), inFlightFences[i], nullptr);
    }
    
    vkDestroyCommandPool(vulkanDevice.getLogicalDevice(), commandPool, nullptr);
    vulkanPipeline.destroyPipeline(vulkanDevice);
    vulkanSwapChain.destroySwapChain(vulkanDevice);
    
    for (size_t i = 0; i < vulkanSwapChain.size(); i++) {
        vkDestroyBuffer(vulkanDevice.getLogicalDevice(), uniformBuffers[i], nullptr);
        vkFreeMemory(vulkanDevice.getLogicalDevice(), uniformBuffersMemory[i], nullptr);
    }
    vkDestroyDescriptorPool(vulkanDevice.getLogicalDevice(), descriptorPool, nullptr);
    
    vkDestroyDescriptorSetLayout(vulkanDevice.getLogicalDevice(), descriptorSetLayout, nullptr);
    vkDestroyBuffer(vulkanDevice.getLogicalDevice(), vertexBuffer, nullptr);
    vkFreeMemory(vulkanDevice.getLogicalDevice(), vertexBufferMemory, nullptr);
    vkDestroyBuffer(vulkanDevice.getLogicalDevice(), indexBuffer, nullptr);
    vkFreeMemory(vulkanDevice.getLogicalDevice(), indexBufferMemory, nullptr);
    
    vulkanDevice.destroyDevice();
    vkDestroySurfaceKHR(vulkanInstance.getVulkanInstance(), metalSurface, nullptr);
    vulkanInstance.destroyInstance();
}

void GVULKANAPI::frameResized(const float width, const float height) {
    updateFrameSize = true;
}

void GVULKANAPI::drawFrame() {
    VkResult result = vkWaitForFences(vulkanDevice.getLogicalDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    result = vkAcquireNextImageKHR(vulkanDevice.getLogicalDevice(), vulkanSwapChain.getVulkanSwapChain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(vulkanDevice.getLogicalDevice(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];
    
    updateUniformBuffer(imageIndex);
    
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;

    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.pWaitDstStageMask = waitStages;
    
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    
    vkResetFences(vulkanDevice.getLogicalDevice(), 1, &inFlightFences[currentFrame]);
    
    if (vkQueueSubmit(vulkanDevice.getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        printf("GaneshaEngine: failed to submit draw command buffer\n");
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
        printf("GaneshaEngine: failed to present frame\n");
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

uint32_t GVULKANAPI::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(vulkanDevice.getPhysicalDevice(), &memoryProperties);
    
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    
    return 0;
}

void GVULKANAPI::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandPool = commandPool;
    commandBufferAllocateInfo.commandBufferCount = 1;
    
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(vulkanDevice.getLogicalDevice(), &commandBufferAllocateInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    vkQueueSubmit(vulkanDevice.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(vulkanDevice.getGraphicsQueue());
    
    vkFreeCommandBuffers(vulkanDevice.getLogicalDevice(), commandPool, 1, &commandBuffer);
}

void GVULKANAPI::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateBuffer(vulkanDevice.getLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        printf("GaneshaEngine: failed to create vertex buffer\n");
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(vulkanDevice.getLogicalDevice(), buffer, &memoryRequirements);
    
    VkMemoryAllocateInfo memoryAllocateInfo{};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, properties);
    if (vkAllocateMemory(vulkanDevice.getLogicalDevice(), &memoryAllocateInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        printf("GaneshaEngine: failed to allocate vertex buffer memory\n");
        return;
    }
    vkBindBufferMemory(vulkanDevice.getLogicalDevice(), buffer, bufferMemory, 0);
}

void GVULKANAPI::createIndicesBuffer() {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer,
                 stagingBufferMemory);
    
    void *data;
    vkMapMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory);

    createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 indexBuffer,
                 indexBufferMemory);
    
    copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(vulkanDevice.getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory, nullptr);
}

void GVULKANAPI::createVertexBuffer() {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer,
                 stagingBufferMemory);
    
    void *data;
    vkMapMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory);

    createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 vertexBuffer,
                 vertexBufferMemory);
    
    copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(vulkanDevice.getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(vulkanDevice.getLogicalDevice(), stagingBufferMemory, nullptr);
}

void GVULKANAPI::createDescriptorPool() {
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(vulkanSwapChain.size());
    
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(vulkanSwapChain.size());
    if (vkCreateDescriptorPool(vulkanDevice.getLogicalDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        printf("GaneshaEngine: failed to create descriptor pool\n");
    }
}

void GVULKANAPI::createDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts(vulkanSwapChain.size(), descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(vulkanSwapChain.size());
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(vulkanSwapChain.size());
    if (vkAllocateDescriptorSets(vulkanDevice.getLogicalDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        printf("GaneshaEngine: failed to allocate descriptor sets\n");
    }

    for (size_t i = 0; i < vulkanSwapChain.size(); i++) {
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
    
    uniformBuffers.resize(vulkanSwapChain.size());
    uniformBuffersMemory.resize(vulkanSwapChain.size());
    
    for (size_t i = 0; i < vulkanSwapChain.size(); i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
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
    imagesInFlight.resize(vulkanSwapChain.size(), VK_NULL_HANDLE);
    
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

#pragma mark - Commands -
    
void GVULKANAPI::createCommandBuffers() {
    commandBuffers.resize(vulkanSwapChain.getFramebuffers().size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(vulkanDevice.getLogicalDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        printf("GaneshaEngine: failed to create command buffer\n");
    }
    
    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        
        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            printf("GaneshaEngine: failed to begin recording command buffer\n");
        }
        
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vulkanSwapChain.getRenderPass();
        renderPassInfo.framebuffer = vulkanSwapChain.getFramebuffers()[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = vulkanSwapChain.getExtent();
        
        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;
        
        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline.getGraphicsPipeline());
        
        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
        
        vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline.getPipelineLayout(), 0, 1, &descriptorSets[i], 0, nullptr);
        vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
        
        vkCmdEndRenderPass(commandBuffers[i]);
        
        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            printf("GaneshaEngine: failed to record command buffer\n");
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
