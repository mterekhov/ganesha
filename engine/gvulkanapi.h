#ifndef SPCGANESHAENGINE_GVULKANAPI_H
#define SPCGANESHAENGINE_GVULKANAPI_H

#include <stdio.h>
#include <vector>
#include <vulkan/vulkan.h>

#include "gvulkaninstance.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class GVULKANAPI {
public:
    void createInstance(void *metalLayer, const uint32_t frameWidth, const uint32_t frameHeight);
    void destroyInstance();
    void drawFrame();
    void frameResized(const float width, const float height);
    void installIsometricView(const TFloat fieldOfView, const TFloat near, const TFloat far);
    void installViewMatrix(const GMatrix& newViewMatrix);

private:
    GMatrix projectionMatrix;
    GMatrix viewMatrix;

    bool updateFrameSize;
    GVULKANInstance vulkanInstance;
    VkPhysicalDevice physicalDevice;
    VkSurfaceKHR metalSurface;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;
    
    std::vector<VkCommandBuffer> commandBuffers;
    VkCommandPool commandPool;
    
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkDevice logicalDevice;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
    
    int32_t graphicQueueFamilyIndex = -1;
    int32_t presentQueueFamilyIndex = -1;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t maxFramesInFlight = 2;

    void createDescriptorSetLayout();

    void createSemaphores();
    
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createVertexBuffer();
    void createIndicesBuffer();
    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage);
    void createDescriptorPool();
    void createDescriptorSets();

    void createCommandPool();
    void createCommandBuffers();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createRenderPass();
    void createGraphicsPipeline();
    VkShaderModule createShaderModule(const std::vector<uint8_t>& code);

    void createFramebuffers();
    void createSwapChain();
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    void createImageViews();

    void setupDevice();
    bool checkDeviceExtensionSupport(VkPhysicalDevice& device);
    bool checkDeviceCapability(const VkPhysicalDevice& device);
    
    void setupLogicalDevice();

    void setupSurface(void *metalLayer);
    
    void findQueuesIndeces();
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANAPI_H
