//
//  gvulkaninstance.cpp
//  ganesha
//
//  Created by Mihail Terekhov on 23.07.2021.
//

#include "gvulkanswapchain.h"

namespace spcGaneshaEngine {

GVULKANSwapChain::GVULKANSwapChain(GLog& log) : log(log) {
}

GVULKANSwapChain::~GVULKANSwapChain() {
}

void GVULKANSwapChain::createSwapChain(const uint32_t frameWidth, const uint32_t frameHeight, GVULKANDevice& vulkanDevice, VkSurfaceKHR& surface) {
    SwapChainSupportDetails supportDetails = vulkanDevice.querySwapChainSupport(surface);
    
    VkSurfaceFormatKHR surfaceFormat = selectSwapSurfaceFormat(supportDetails.formats);
    extent = selectSwapExtent(supportDetails.surfaceCapabilities, frameWidth, frameHeight);
    
    uint32_t count = supportDetails.surfaceCapabilities.minImageCount + 1;
    if (supportDetails.surfaceCapabilities.maxImageCount > 0 && count > supportDetails.surfaceCapabilities.maxImageCount) {
        count = supportDetails.surfaceCapabilities.maxImageCount;
    }
    
    VkSwapchainCreateInfoKHR swapChainInfo = { };
    swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainInfo.surface = surface;
    swapChainInfo.minImageCount = count;
    swapChainInfo.imageFormat = surfaceFormat.format;
    swapChainInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapChainInfo.imageExtent = extent;
    swapChainInfo.imageArrayLayers = 1;
    swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    std::vector<uint32_t> queueFamilyIndexArray;
    if (vulkanDevice.presentationIsEqualToGraphics()) {
        swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainInfo.queueFamilyIndexCount = 0;
        swapChainInfo.pQueueFamilyIndices = nullptr;
    }
    else {
        queueFamilyIndexArray = vulkanDevice.getQueuesIndecesArray();
        swapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndexArray.size());
        swapChainInfo.pQueueFamilyIndices = queueFamilyIndexArray.data();
    }
    swapChainInfo.preTransform = supportDetails.surfaceCapabilities.currentTransform;
    swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainInfo.presentMode = selectSwapPresentMode(supportDetails.presentModes);
    swapChainInfo.clipped = VK_TRUE;
    if (vkCreateSwapchainKHR(vulkanDevice.getLogicalDevice(), &swapChainInfo, nullptr, &swapChain) != VK_SUCCESS) {
        log.error("failed to create swap chain\n");
    }
    
    imageFormat = surfaceFormat.format;
    vkGetSwapchainImagesKHR(vulkanDevice.getLogicalDevice(), swapChain, &count, nullptr);
    imagesArray.resize(count);
    vkGetSwapchainImagesKHR(vulkanDevice.getLogicalDevice(), swapChain, &count, imagesArray.data());
    
    imageViewsArray = createImageViews(vulkanDevice.getLogicalDevice(), imagesArray);
}

void GVULKANSwapChain::destroySwapChain(VkDevice& logicalDevice) {
    for (auto imageView : imageViewsArray) {
        vkDestroyImageView(logicalDevice, imageView, nullptr);
    }
    
    vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
}

std::vector<VkImageView>& GVULKANSwapChain::getImageViewsArray() {
    return imageViewsArray;
}

VkExtent2D GVULKANSwapChain::getExtent() {
    return extent;
}

VkSwapchainKHR& GVULKANSwapChain::getVulkanSwapChain() {
    return swapChain;
}

size_t GVULKANSwapChain::size() {
    return imagesArray.size();
}

VkFormat& GVULKANSwapChain::getImagesFormat() {
    return imageFormat;
}

#pragma mark - Routine -

std::vector<VkImageView> GVULKANSwapChain::createImageViews(VkDevice& logicalDevice, std::vector<VkImage>& swapChainImagesArray) {
    std::vector<VkImageView> newImageViewsArray;
    
    newImageViewsArray.resize(swapChainImagesArray.size());
    for (size_t i = 0; i < swapChainImagesArray.size(); i++) {
        VkImageViewCreateInfo imageViewInfo = { };
        
        imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewInfo.image = swapChainImagesArray[i];
        imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewInfo.format = imageFormat;
        imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewInfo.subresourceRange.baseMipLevel = 0;
        imageViewInfo.subresourceRange.levelCount = 1;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount = 1;
        
        if (vkCreateImageView(logicalDevice, &imageViewInfo, nullptr, &newImageViewsArray[i]) != VK_SUCCESS) {
            log.error("failed to create image view\n");
        }
    }
    
    return newImageViewsArray;
}

VkExtent2D GVULKANSwapChain::selectSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities, const uint32_t frameWidth, const uint32_t frameHeight) {
    VkExtent2D actualExtent = {frameWidth, frameHeight};
    
    if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return surfaceCapabilities.currentExtent;
    } else {
        actualExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, actualExtent.height));
    }
    
    return actualExtent;
}

VkSurfaceFormatKHR GVULKANSwapChain::selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    
    return availableFormats[0];
}

VkPresentModeKHR GVULKANSwapChain::selectSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModesArray) {
    for (const auto& mode : presentModesArray) {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        }
    }
    
    return VK_PRESENT_MODE_FIFO_KHR;
}

}
