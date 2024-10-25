#ifndef SPCGANESHAENGINE_GVULKANSWAPCHAIN_H
#define SPCGANESHAENGINE_GVULKANSWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <vector>

#include "glog.h"
#include "gvulkandevice.h"
#include "gimageservice.h"
#include "gcommandservice.h"

namespace spcGaneshaEngine {

class GVULKANSwapChain {
public:
    GVULKANSwapChain();
    ~GVULKANSwapChain();
    
    void createSwapChain(VkSurfaceKHR& surface, 
                         std::shared_ptr<GCommandServiceProtocol> commandService,
                         std::shared_ptr<GImageServiceProtocol> imageService,
                         GVULKANDevice& vulkanDevice);
    void createSwapChain(const TUInt screenWidth, 
                         const TUInt screenHeight,
                         std::shared_ptr<GCommandServiceProtocol> commandService,
                         std::shared_ptr<GImageServiceProtocol> imageService,
                         VkSurfaceKHR& surface,
                         GVULKANDevice& vulkanDevice);
    void updateScreenSize(const VkExtent2D& newExtent,
                          std::shared_ptr<GCommandServiceProtocol> commandService,
                          std::shared_ptr<GImageServiceProtocol> imageService,
                          VkSurfaceKHR& surface,
                          GVULKANDevice& vulkanDevice);
    void destroySwapChain(std::shared_ptr<GImageServiceProtocol> imageService, GVULKANDevice& vulkanDevice);
    
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
    std::shared_ptr<GVULKANImage> depthImage;

    std::vector<VkImage> imagesArray;
    std::vector<VkImageView> imageViewsArray;
    std::vector<VkFramebuffer> framebuffersArray;
    
    void createSwapChain(const VkExtent2D& initialExtent,
                         const TBool shouldCreateRenderPass,
                         VkSurfaceKHR& surface,
                         std::shared_ptr<GCommandServiceProtocol> commandService,
                         std::shared_ptr<GImageServiceProtocol> imageService,
                         GVULKANDevice& vulkanDevice);
    VkSwapchainKHR createNewSwapChain(const VkExtent2D& initialExtent, 
                                      const SwapChainSupportDetails& supportDetails,
                                      GVULKANDevice& vulkanDevice,
                                      VkSurfaceKHR& surface);
    std::vector<VkImage> ejectImagesArray(const VkSwapchainKHR& swapChainSource, VkDevice device);
    std::vector<VkImageView> createImageViews(std::vector<VkImage>& swapChainImagesArray, VkDevice logicalDevice);
    std::vector<VkFramebuffer> createFramebuffers(const std::vector<VkImageView>& useImagesViewArray,
                                                  VkImageView depthImageView,
                                                  VkRenderPass useRenderPass,
                                                  const VkExtent2D& useExtent,
                                                  VkDevice device);
    VkRenderPass createRenderPass(VkFormat format, GVULKANDevice& vulkanDevice);
    VkExtent2D selectSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities, const VkExtent2D& initialExtent);
    VkSurfaceFormatKHR selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR selectSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModesArray);
    void destroySwapChainAndDependency(std::shared_ptr<GImageServiceProtocol> imageService, GVULKANDevice& vulkanDevice);
    void destroyRenderPass(VkDevice device);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANSWAPCHAIN_H
