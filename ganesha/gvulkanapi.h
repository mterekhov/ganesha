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
#include "gmatrix.h"
#include "gvulkanbuffer.h"
#include "gvulkandescriptorsets.h"
#include "gvulkanimage.h"

namespace spcGaneshaEngine {

struct UniformBufferObject {
    GMatrix model;
    GMatrix view;
    GMatrix proj;
};

class GVULKANAPI: public GGraphicsAPIProtocol {
public:
    GVULKANAPI(GLog& log);
    virtual ~GVULKANAPI();

    virtual void initAPI(void *metalLayer, const TUInt frameWidth, const TUInt frameHeight, GRenderGraph& renderGraph);
    virtual void destroyAPI();
    virtual void drawFrame(GRenderGraph& renderGraph);
    virtual void frameResized(const float width, const float height);
    virtual void installIsometricView(const TFloat fieldOfView, const TFloat near, const TFloat far);
    virtual void installViewMatrix(const GMatrix& newViewMatrix);

private:
    GLog& log;
    GVULKANDevice vulkanDevice;
    GVULKANInstance vulkanInstance;
    GVULKANSwapChain vulkanSwapChain;
    GVULKANPipeline vulkanPipeline;
    GVULKANDescriptorsets vulkanDescriptorset;

    GVULKANImage texture;
    std::vector<GVULKANBuffer> vulkanUniformBuffers;
    GVULKANBuffer vertexesBuffer;
    GVULKANBuffer indexesBuffer;

    VkSurfaceKHR metalSurface;
    VkCommandPool commandPool;

    TFloat nearPlane;
    TFloat farPlane;
    TFloat fov;
    GMatrix projectionMatrix;
    GMatrix viewMatrix;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    std::vector<VkCommandBuffer> renderCommands;

    const TUInt maxFramesInFlight = 2;

    void recordRenderCommand(VkCommandBuffer renderCommand,
                             VkBuffer vertexesBuffer,
                             VkBuffer indexesBuffer,
                             const TUInt indexesNumber,
                             VkFramebuffer framebuffer,
                             GVULKANSwapChain& swapChain,
                             GVULKANPipeline& pipeline,
                             VkDescriptorSet descriptorset);
    VkCommandPool createCommandPool(GVULKANDevice& device);
    VkSurfaceKHR createSurface(void *metalLayer);
    void createTextures();
    void createSemaphores();
    UniformBufferObject currentUBO();
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANAPI_H
