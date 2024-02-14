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
#include "gvulkanimage.h"
#include "gmaterialsservice.h"
#include "gdescriptorsetservice.h"
#include "gcommandservice.h"
#include "gshadersservice.h"
#include "grendergraph.h"

namespace spcGaneshaEngine {

struct ProjectionsBufferObject {
    GMatrix view;
    GMatrix proj;
};

class GVULKANAPI: public GGraphicsAPIProtocol {
public:
    GVULKANAPI();
    virtual ~GVULKANAPI();

    virtual void initAPI(void *metalLayer, const GGaneshaContent& content);
    virtual void destroyAPI();
    virtual void drawFrame();
    virtual void frameResized(const TFloat width, const TFloat height);
    virtual void installIsometricView(const TFloat fieldOfView, const TFloat near, const TFloat far);
    virtual void installViewMatrix(const GMatrix& newViewMatrix);

private:
    GVULKANDevice vulkanDevice;
    GVULKANInstance vulkanInstance;
    GVULKANSwapChain vulkanSwapChain;
    GVULKANPipeline vulkanPipeline;
    GDescriptorsetServiceProtocol *descriptorService;
    GCommandServiceProtocol *commandService;
    GShadersServiceProtocol *shadersService;
    
    VkSurfaceKHR metalSurface;

    TFloat nearPlane;
    TFloat farPlane;
    TFloat fov;
    GMatrix projectionMatrix;
    GMatrix viewMatrix;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    size_t currentFrame = 0;

    std::vector<VkCommandBuffer> renderCommands;
    std::vector<GRenderGraph> renderGraphArray;

    const TUInt maxFramesInFlight = 2;

    void recordRenderCommand(VkCommandBuffer renderCommand,
                             GRenderGraph& renderGraph,
                             VkFramebuffer framebuffer,
                             GVULKANSwapChain& swapChain,
                             GVULKANPipeline& pipeline,
                             VkDescriptorSet descriptorset);
    VkSurfaceKHR createSurface(void *metalLayer);
    void createSemaphores();

    std::vector<GVULKANBuffer> vulkanProjectionBuffers;
    ProjectionsBufferObject currentProjectionBufferObject();
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANAPI_H
