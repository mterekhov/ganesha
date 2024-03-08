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

GVULKANAPI::GVULKANAPI() {
    
}

GVULKANAPI::~GVULKANAPI() {
    delete descriptorService;
    delete shadersService;
    delete commandService;
}

#pragma mark - GGraphicsAPIProtocol -

void GVULKANAPI::initAPI(const std::string& applicationTitle, void *metalLayer, const GGaneshaContent& content) {
    //  create VULKAN instance
    vulkanInstance.createInstance(applicationTitle.c_str(), khronosValidationLayers, avoidInstanceExtensions);
    
    //  create surface
    metalSurface = createSurface(metalLayer);
    
    //  creates physicalDevice
    vulkanDevice.createDevice(vulkanInstance, useDeviceExtensions, metalSurface);
    
    vulkanSwapChain.createSwapChain(content.viewport.width, content.viewport.height, vulkanDevice, metalSurface);
    
    descriptorService = new GDescriptorsetService(vulkanDevice);
    descriptorService->init();
    
    commandService = new GCommandService(vulkanDevice);
    commandService->init();
    
    shadersService = new GShadersService(vulkanDevice);
    shadersService->init();
    const TStringsArray& vertexShadersArray = content.getVertexShadersArray();
    for (TStringsArray::const_iterator iter = vertexShadersArray.begin(); iter != vertexShadersArray.end(); iter++) {
        shadersService->addVertexShader(*iter);
    }
    const TStringsArray& fragmentShadersArray = content.getFragmetShadersArrray();
    for (TStringsArray::const_iterator iter = fragmentShadersArray.begin(); iter != fragmentShadersArray.end(); iter++) {
        shadersService->addFragmentShader(*iter);
    }

    for (TUInt i = 0; i < maxFramesInFlight; i++) {
        GRenderGraph newRenderGraph(commandService);
        newRenderGraph.createGraph(descriptorService, vulkanDevice);
        newRenderGraph.loadContent(content, descriptorService, vulkanDevice);
        renderGraphArray.push_back(newRenderGraph);
    }
    
    std::vector<VkPipelineShaderStageCreateInfo> shadersArray = shadersService->getAllShadersArray();
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
                                         vulkanDevice,
                                         commandService);
        descriptorService->attachBufferToDescriptorset(newProjectionBuffer, 0);
        vulkanProjectionBuffers.push_back(newProjectionBuffer);

        //  render commands
        renderCommands.push_back(commandService->allocateCommandBuffer());
    }

    createSemaphores();
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
        
    for (GRenderGraph graph:renderGraphArray) {
        graph.destroyGraph(vulkanDevice.getLogicalDevice());
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
                        renderGraphArray[currentFrame],
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
                                     GRenderGraph& renderGraph,
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

            vkCmdBindDescriptorSets(renderCommand, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipelineLayout(), 0, 1, &descriptorset, 0, nullptr);
            GVULKANBuffer& modelBuffer = renderGraph.getModelBuffer();
            TUInt counter = 1;
            for (GGraphNode *graphNode:renderGraph.getNodeArray()) {
                graphNode->rts.print();
                modelBuffer.refreshBuffer(&graphNode->rts, vulkanDevice.getLogicalDevice());
                graphNode->mesh->render(renderCommand);
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
