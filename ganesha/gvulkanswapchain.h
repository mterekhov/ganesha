#ifndef SPCGANESHAENGINE_GVULKANSWAPCHAIN_H
#define SPCGANESHAENGINE_GVULKANSWAPCHAIN_H

#include <vulkan/vulkan.h>

#include "glog.h"
#include "gvulkandevice.h"

namespace spcGaneshaEngine {

class GVULKANSwapChain {
public:
    GVULKANSwapChain(GLog& log);
    ~GVULKANSwapChain();
    
    void createSwapChain(const uint32_t screenWidth, const uint32_t screenHeight, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface);
    void updateScreenSize(const uint32_t screenWidth, const uint32_t screenHeight, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface);
    void destroySwapChain(GVULKANDevice& device);

    std::vector<VkImageView>& getImageViewsArray();
    VkExtent2D getExtent();
    VkSwapchainKHR& getVulkanSwapChain();
    VkFormat& getImagesFormat();
    VkRenderPass& getRenderPass();
    std::vector<VkFramebuffer>& getFramebuffers();
    size_t framebuffersNumber();

private:
    GLog& log;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> imagesArray;
    std::vector<VkImageView> imageViewsArray;
    std::vector<VkFramebuffer> framebuffersArray;
    VkFormat imageFormat;
    VkExtent2D extent;
    VkRenderPass renderPass;

    VkSwapchainKHR createNewSwapChain(const uint32_t screenWidth, const uint32_t screenHeight, const SwapChainSupportDetails& supportDetails, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface);
    void createSwapChain(const uint32_t screenWidth, const uint32_t screenHeight, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface, const bool recreateSwapChain);
    std::vector<VkImage> ejectImagesArray(const VkDevice& device, const VkSwapchainKHR& swapChainSource);
    std::vector<VkImageView> createImageViews(VkDevice& logicalDevice, std::vector<VkImage>& swapChainImagesArray);
    std::vector<VkFramebuffer> createFramebuffers(VkDevice& device, const std::vector<VkImageView>& useImagesViewArray, const VkRenderPass& useRenderPass, const VkExtent2D& useExtent);
   VkRenderPass createRenderPass(const VkDevice& device, VkFormat format);
    VkExtent2D selectSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities, const uint32_t screenWidth, const uint32_t screenHeight);
    VkSurfaceFormatKHR selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR selectSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModesArray);
    void destroyExtentDependency(const VkDevice& device);
    void destroyRenderPass(const VkDevice& device);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANSWAPCHAIN_H
