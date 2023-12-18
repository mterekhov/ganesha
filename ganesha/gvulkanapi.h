#ifndef SPCGANESHAENGINE_GVULKANAPI_H
#define SPCGANESHAENGINE_GVULKANAPI_H

#include <stdio.h>
#include <vector>
#include <vulkan/vulkan.h>

#include "ggraphicsapiprotocol.h"
#include "gvulkaninstance.h"
#include "gvulkandevice.h"
#include "gvulkanswapchain.h"
#include "gvulkanpipeline.h"
#include "gvulkancommands.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

/// A LOT OF FUCKING SHIT
class GVULKANAPI: public GGraphicsAPIProtocol {
public:
    GVULKANAPI();
    virtual ~GVULKANAPI();

    virtual void initAPI(void *metalLayer, const uint32_t frameWidth, const uint32_t frameHeight, const GRenderGraph& renderGraph);
    virtual void destroyAPI();
    virtual void drawFrame();
    virtual void frameResized(const float width, const float height);
    virtual void installIsometricView(const TFloat fieldOfView, const TFloat near, const TFloat far);
    virtual void installViewMatrix(const GMatrix& newViewMatrix);

private:
    GLog log;
    GVULKANDevice vulkanDevice;
    GVULKANInstance vulkanInstance;
    GVULKANSwapChain vulkanSwapChain;
    GVULKANPipeline vulkanPipeline;
    GVULKANCommands vulkanCommands;
    
    VkSurfaceKHR metalSurface;

    GMatrix projectionMatrix;
    GMatrix viewMatrix;

    bool updateFrameSize;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    VkDescriptorSetLayout descriptorSetLayout;

    const uint32_t maxFramesInFlight = 2;

    void createSemaphores();
    
    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage);
    void createDescriptorPool();
    void createDescriptorSets();
    void createDescriptorSetLayout();

    VkSurfaceKHR createSurface(void *metalLayer);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANAPI_H
