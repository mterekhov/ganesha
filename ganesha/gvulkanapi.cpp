#include <iostream>
#include <set>
#include <array>
#include <math.h>

#include "gvulkanapi.h"
#include "gpoint2d.h"
#include "gcolor.h"
#include "gmatrix.h"
#include "gvulkanbuffer.h"
#include "gvulkantools.h"
#include "gspritemesh.h"

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

GVULKANAPI::GVULKANAPI(const std::string& applicationTitle, void *metalLayer) : applicationTitle(applicationTitle) {
    //  create VULKAN instance
    vulkanInstance.createInstance(applicationTitle.c_str(), khronosValidationLayers, avoidInstanceExtensions);
    
    //  create surface
    metalSurface = createSurface(metalLayer);
    
    //  creates physicalDevice
    vulkanDevice.createDevice(vulkanInstance, useDeviceExtensions, metalSurface);
    
    descriptorService = std::make_shared<GDescriptorsetService>(vulkanDevice);
    descriptorService->init();
    
    commandService = std::make_shared<GCommandService>(vulkanDevice);
    commandService->init();
    
    materialsService = std::make_shared<GMaterialsService>(commandService, vulkanDevice);
    materialsService->init();
    
    shadersService = std::make_shared<GShadersService>(commandService, vulkanDevice);
    shadersService->init();
    
    sceneService = std::make_shared<GSceneService>(descriptorService, commandService, vulkanDevice);
    sceneService->init();
    
    vulkanSwapChain.createSwapChain(metalSurface,
                                    vulkanDevice);
}

GVULKANAPI::~GVULKANAPI() {
    descriptorService->destroy();
    commandService->destroy();
    shadersService->destroy();
}

#pragma mark - GGraphicsAPIProtocol -

void GVULKANAPI::loadScene(GScene& scene) {
    vkDeviceWaitIdle(vulkanDevice.getLogicalDevice());
    
    vulkanPipeline.destroyPipeline(vulkanDevice);

    std::vector<VkPipelineShaderStageCreateInfo> shadersArray = shadersService->getShadersPipelineInfo(scene.fragmentShadersArray, VK_SHADER_STAGE_FRAGMENT_BIT, commandService, vulkanDevice);
    std::vector<VkPipelineShaderStageCreateInfo> shadersArray2 = shadersService->getShadersPipelineInfo(scene.vertexShadersArray, VK_SHADER_STAGE_VERTEX_BIT, commandService, vulkanDevice);
    shadersArray.insert(shadersArray.end(), shadersArray2.begin(), shadersArray2.end());
    vulkanPipeline.createPipeline(vulkanDevice, vulkanSwapChain, shadersArray, descriptorService);

    TUInt framebuffersNumber = static_cast<TUInt>(vulkanSwapChain.framebuffersNumber());
    for (TUInt i = 0; i < framebuffersNumber; i++) {
        //  projection matrix
        GVULKANBuffer newProjectionBuffer;
        ProjectionsBufferObject projectionBufferObject = currentProjectionBufferObject();
        newProjectionBuffer.createBuffer(&projectionBufferObject,
                                         sizeof(ProjectionsBufferObject),
                                         VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                         false,
                                         commandService,
                                         vulkanDevice);
        descriptorService->updateDescriptorset(newProjectionBuffer, 0);
        vulkanProjectionBuffers.push_back(newProjectionBuffer);

        //  render commands
        renderCommands.push_back(commandService->allocateCommandBuffer());
    }

    createSemaphores();
}

void GVULKANAPI::loadGundle(const std::string& gundleFilePath) {
}

void GVULKANAPI::destroyAPI() {
    vkDeviceWaitIdle(vulkanDevice.getLogicalDevice());
    
    vulkanSwapChain.destroySwapChain(vulkanDevice);
    vulkanPipeline.destroyPipeline(vulkanDevice);
    
    for (size_t i = 0; i < vulkanProjectionBuffers.size(); i++) {
        vulkanProjectionBuffers[i].destroyBuffer(vulkanDevice.getLogicalDevice());
    }

    for (size_t i = 0; i < maxFramesInFlight; i++) {
        vkDestroySemaphore(vulkanDevice.getLogicalDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(vulkanDevice.getLogicalDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(vulkanDevice.getLogicalDevice(), inFlightFences[i], nullptr);
    }
        
    for (GScene scene:scenesArray) {
        scene.clear();
    }
    
    descriptorService->destroy();
    commandService->destroy();
    shadersService->destroy();
    
    vulkanDevice.destroyDevice();
    vkDestroySurfaceKHR(vulkanInstance.getVulkanInstance(), metalSurface, nullptr);
    vulkanInstance.destroyInstance();
}

void GVULKANAPI::updateSwapChain(const GViewport& viewport) {
    vulkanSwapChain.updateScreenSize(viewport.width, viewport.height, vulkanDevice, metalSurface);
}

void GVULKANAPI::installIsometricProjection(const GViewport& viewport) {
    VkExtent2D swapChainExtent = vulkanSwapChain.getExtent();
    TFloat aspect = static_cast<TFloat>(swapChainExtent.width) / static_cast<TFloat>(swapChainExtent.height);
    TFloat size = viewport.near * tanf(viewport.fov / 2.0);
    TFloat aspectHeight = aspect * size;
    
    projectionMatrix = GMatrix::frustum(-aspectHeight, aspectHeight, -size, size, viewport.near, viewport.far);
}

void GVULKANAPI::installViewMatrix(const GMatrix& newViewMatrix) {
    viewMatrix = newViewMatrix;
}

void GVULKANAPI::render() {
    vkWaitForFences(vulkanDevice.getLogicalDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    
    //  index of frame buffer in which we are going to render
    TUInt imageIndex;
    vkAcquireNextImageKHR(vulkanDevice.getLogicalDevice(), vulkanSwapChain.getVulkanSwapChain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    ProjectionsBufferObject projectionBufferObject = currentProjectionBufferObject();
    vulkanProjectionBuffers[imageIndex].refreshBuffer(&projectionBufferObject, vulkanDevice.getLogicalDevice());
    
    vkResetFences(vulkanDevice.getLogicalDevice(), 1, &inFlightFences[currentFrame]);
    vkResetCommandBuffer(renderCommands[currentFrame], 0);
    recordRenderCommand(renderCommands[currentFrame],
                        scenesArray[currentFrame],
                        vulkanSwapChain.getFramebuffers()[imageIndex],
                        vulkanSwapChain,
                        vulkanPipeline,
                        descriptorService->getDescriptorset());
    
    std::vector<VkSemaphore> signalSemaphores = { renderFinishedSemaphores[currentFrame] };
    commandService->submitCommandBuffer({ renderCommands[currentFrame] },
                                        { imageAvailableSemaphores[currentFrame] },
                                        signalSemaphores,
                                        inFlightFences[currentFrame],
                                        false,
                                        false);
    
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = signalSemaphores.size();
    presentInfo.pWaitSemaphores = signalSemaphores.data();
    
    VkSwapchainKHR swapChains[] = { vulkanSwapChain.getVulkanSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    vkQueuePresentKHR(vulkanDevice.getPresentQueue(), &presentInfo);
    
    currentFrame = (currentFrame + 1) % maxFramesInFlight;
}

#pragma mark - Routine -

void GVULKANAPI::recordRenderCommand(VkCommandBuffer renderCommand,
                                     GScene& scene,
                                     VkFramebuffer framebuffer,
                                     GVULKANSwapChain& swapChain,
                                     GVULKANPipeline& pipeline,
                                     VkDescriptorSet descriptorset) {
    VkCommandBufferBeginInfo beginInfo = { };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    
    vkBeginCommandBuffer(renderCommand, &beginInfo);
        VkExtent2D swapChainExtent = swapChain.getExtent();
        VkRenderPassBeginInfo renderPassInfo = { };
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

            VkDeviceSize offsets[1] = { 0 };
            vkCmdBindDescriptorSets(renderCommand, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipelineLayout(), 0, 1, &descriptorset, 0, nullptr);
            const std::vector<GSceneNode *>& sceneNodesArray = scene.getNodesArray();
            for (GSceneNode *node:sceneNodesArray) {
                VkBuffer instanceBuffer = node->getInstanceBuffer().getBuffer();
                vkCmdBindVertexBuffers(renderCommand, 1, 1, &instanceBuffer, offsets);
                node->getMesh()->render(1, renderCommand);
            }
        vkCmdEndRenderPass(renderCommand);
    vkEndCommandBuffer(renderCommand);
}

void GVULKANAPI::createSemaphores() {
    imageAvailableSemaphores.resize(maxFramesInFlight);
    renderFinishedSemaphores.resize(maxFramesInFlight);
    inFlightFences.resize(maxFramesInFlight);
    
    VkSemaphoreCreateInfo semaphoreInfo = { };
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkFenceCreateInfo fenceInfo = { };
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    
    for (size_t i = 0; i < maxFramesInFlight; i++) {
        if (vkCreateSemaphore(vulkanDevice.getLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(vulkanDevice.getLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(vulkanDevice.getLogicalDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            GLOG_ERROR("failed to create semaphores\n");
            return;
        }
    }
}

VkSurfaceKHR GVULKANAPI::createSurface(void *metalLayer) {
    VkMetalSurfaceCreateInfoEXT metalSurfaceInfo = {};
    
    metalSurfaceInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
    metalSurfaceInfo.pNext = NULL;
    metalSurfaceInfo.flags = 0;
    metalSurfaceInfo.pLayer = metalLayer;
    
    VkSurfaceKHR newSurface;
    if (vkCreateMetalSurfaceEXT(vulkanInstance.getVulkanInstance(), &metalSurfaceInfo, NULL, &newSurface) != VK_SUCCESS) {
        GLOG_ERROR("error creating metal surface\n");
    }
    
    return newSurface;
}

ProjectionsBufferObject GVULKANAPI::currentProjectionBufferObject() {
    ProjectionsBufferObject projectionBufferObject = {};
    
    projectionBufferObject.view = viewMatrix;
    projectionBufferObject.proj = projectionMatrix;
    
    return projectionBufferObject;
}

}
