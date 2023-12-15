//
//  gvulkaninstance.hpp
//  ganesha
//
//  Created by Mihail Terekhov on 23.07.2021.
//

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
    
    void createSwapChain(const uint32_t frameWidth, const uint32_t frameHeight, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface);
    void destroySwapChain(VkDevice& logicalDevice);

    std::vector<VkImageView>& getImageViewsArray();
    VkExtent2D getExtent();
    VkSwapchainKHR& getVulkanSwapChain();
    VkFormat& getImagesFormat();
    size_t size();

private:
    GLog& log;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> imagesArray;
    std::vector<VkImageView> imageViewsArray;
    VkFormat imageFormat;
    VkExtent2D extent;

    std::vector<VkImageView> createImageViews(VkDevice& logicalDevice, std::vector<VkImage>& swapChainImagesArray);
    VkExtent2D selectSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities, const uint32_t frameWidth, const uint32_t frameHeight);
    VkSurfaceFormatKHR selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR selectSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModesArray);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANSWAPCHAIN_H
