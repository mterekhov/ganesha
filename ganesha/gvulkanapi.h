#ifndef SPCGANESHAENGINE_GVULKANAPI_H
#define SPCGANESHAENGINE_GVULKANAPI_H

#include <vector>
#include <string>
#include <vulkan/vulkan.h>

#include "gmatrix.h"
#include "ganeshatypes.h"
#include "gshadersservice.h"
#include "gsceneservice.h"
#include "gimageservice.h"
#include "gdescriptorsetservice.h"
#include "gcommandservice.h"
#include "gvulkaninstance.h"
#include "gvulkandevice.h"
#include "gvulkanswapchain.h"
#include "gvulkanpipeline.h"
#include "gvulkanbuffer.h"
#include "gscene.h"


namespace spcGaneshaEngine {

struct ProjectionsBufferObject {
    GMatrix view;
    GMatrix proj;
};

class GVULKANAPI {
public:
    GVULKANAPI(const std::string& applicationTitle, void *metalLayer);
    virtual ~GVULKANAPI();

    void loadGundle(const std::string& gundleFilePath);
    void destroyAPI();
    void render();
    void updateSwapChain(const TFloat width, const TFloat height);
    void installIsometricProjection(const GViewport& viewport);
    void installViewMatrix(const GMatrix& newViewMatrix);

    std::shared_ptr<GShadersServiceProtocol> shadersService;
    std::shared_ptr<GImageServiceProtocol> imageService;
    std::shared_ptr<GSceneServiceProtocol> sceneService;
    
private:
    std::string applicationTitle;
    GVULKANDevice vulkanDevice;
    GVULKANInstance vulkanInstance;
    GVULKANSwapChain vulkanSwapChain;
    GVULKANPipeline vulkanPipeline;
    
    std::shared_ptr<GDescriptorsetServiceProtocol> descriptorService;
    std::shared_ptr<GCommandServiceProtocol> commandService;
    
    VkSurfaceKHR metalSurface;

    GMatrix projectionMatrix;
    GMatrix viewMatrix;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    size_t currentFrame = 0;

    std::vector<VkCommandBuffer> renderCommands;
    std::vector<GScene> scenesArray;

    const TUInt maxFramesInFlight = 2;

    void recordRenderCommand(VkCommandBuffer renderCommand,
                             GScene& scene,
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
