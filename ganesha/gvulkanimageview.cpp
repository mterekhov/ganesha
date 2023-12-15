//
//  gvulkaninstance.cpp
//  ganesha
//
//  Created by Mihail Terekhov on 23.07.2021.
//

#include "gvulkanimageview.h"

namespace spcGaneshaEngine {

GVULKANImageView::GVULKANImageView(const VkImage& image, const VkFormat& imageFormat, const VkDevice& logicalDevice) : logicalDevice(logicalDevice) {
    imageView = createImageView(image, imageFormat, logicalDevice);
}

GVULKANImageView::~GVULKANImageView() {
    vkDestroyImageView(logicalDevice, imageView, nullptr);
}

VkImageView& GVULKANImageView::getImageView() {
    return imageView;
}

VkImageView GVULKANImageView::createImageView(const VkImage& image, const VkFormat& imageFormat, const VkDevice& device) {
    VkImageViewCreateInfo imageViewInfo{};
    
    imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewInfo.image = image;
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
    
    VkImageView newImageView;
    if (vkCreateImageView(device, &imageViewInfo, nullptr, &newImageView) != VK_SUCCESS) {
//        printf("GaneshaEngine: failed to create image view\n");
    }
    
    return newImageView;
}

}
