#ifndef SPCGANESHAENGINE_GVULKANSWAPCHAIN_H
#define SPCGANESHAENGINE_GVULKANSWAPCHAIN_H

#include <vulkan/vulkan.h>

#include "glog.h"
#include "gvulkandevice.h"
#include "gvulkanimage.h"

namespace spcGaneshaEngine {

class GVULKANSwapChain {
public:
    GVULKANSwapChain();
    ~GVULKANSwapChain();
    
    void createSwapChain(const TUInt screenWidth, const TUInt screenHeight, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface);
    void updateScreenSize(const TUInt screenWidth, const TUInt screenHeight, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface);
    void destroySwapChain(GVULKANDevice& vulkanDevice);
    
    VkSwapchainKHR getVulkanSwapChain();
    VkRenderPass getRenderPass();
    std::vector<VkFramebuffer>& getFramebuffers();
    std::vector<VkImageView>& getImageViewsArray();
    VkExtent2D getExtent();
    VkFormat getImagesFormat();
    size_t framebuffersNumber();
    
private:
    VkSwapchainKHR swapChain;
    std::vector<VkImage> imagesArray;
    std::vector<VkImageView> imageViewsArray;
    std::vector<VkFramebuffer> framebuffersArray;
    VkFormat imageFormat;
    VkExtent2D extent;
    VkRenderPass renderPass;
    GVULKANImage depthImage;
    
    VkSwapchainKHR createNewSwapChain(const TUInt screenWidth, const TUInt screenHeight, const SwapChainSupportDetails& supportDetails, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface);
    void createSwapChain(const TUInt screenWidth, const TUInt screenHeight, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface, const TBool recreateSwapChain);
    std::vector<VkImage> ejectImagesArray(VkDevice device, const VkSwapchainKHR& swapChainSource);
    std::vector<VkImageView> createImageViews(VkDevice logicalDevice, std::vector<VkImage>& swapChainImagesArray);
    std::vector<VkFramebuffer> createFramebuffers(VkDevice device, const std::vector<VkImageView>& useImagesViewArray, VkImageView depthImageView, VkRenderPass useRenderPass, const VkExtent2D& useExtent);
    VkRenderPass createRenderPass(GVULKANDevice& vulkanDevice, VkFormat format);
    VkExtent2D selectSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities, const TUInt screenWidth, const TUInt screenHeight);
    VkSurfaceFormatKHR selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR selectSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModesArray);
    void destroyExtentDependency(GVULKANDevice& vulkanDevice);
    void destroyRenderPass(VkDevice device);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANSWAPCHAIN_H
