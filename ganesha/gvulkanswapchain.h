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
    
    void createSwapChain(VkSurfaceKHR& surface, GVULKANDevice& vulkanDevice);
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
    VkFormat imageFormat;
    VkExtent2D extent;
    VkRenderPass renderPass;
    GVULKANImage depthImage;

    std::vector<VkImage> imagesArray;
    std::vector<VkImageView> imageViewsArray;
    std::vector<VkFramebuffer> framebuffersArray;
    
    VkSwapchainKHR createNewSwapChain(const VkExtent2D& initialExtent, const SwapChainSupportDetails& supportDetails, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface);
    void createSwapChain(const VkExtent2D& initialExtent, const TBool shouldCreateRenderPass, VkSurfaceKHR& surface, GVULKANDevice& vulkanDevice);
    std::vector<VkImage> ejectImagesArray(VkDevice device, const VkSwapchainKHR& swapChainSource);
    std::vector<VkImageView> createImageViews(VkDevice logicalDevice, std::vector<VkImage>& swapChainImagesArray);
    std::vector<VkFramebuffer> createFramebuffers(VkDevice device, const std::vector<VkImageView>& useImagesViewArray, VkImageView depthImageView, VkRenderPass useRenderPass, const VkExtent2D& useExtent);
    VkRenderPass createRenderPass(VkFormat format, GVULKANDevice& vulkanDevice);
    VkExtent2D selectSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities, const VkExtent2D& initialExtent);
    VkSurfaceFormatKHR selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR selectSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModesArray);
    void destroySwapChainAndDependency(GVULKANDevice& vulkanDevice);
    void destroyRenderPass(VkDevice device);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANSWAPCHAIN_H
