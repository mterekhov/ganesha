#include "gvulkantools.h"

namespace spcGaneshaEngine {

std::vector<uint8_t> GVULKANTools::readFile(const std::string& filename) {
    FILE *file = fopen(filename.c_str(), "rb");
    if (file == 0) {
        return std::vector<uint8_t>();
    }
    
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    std::vector<uint8_t> buffer(fileSize);
    fseek(file, 0L, SEEK_SET);
    fread(buffer.data(), fileSize, 1, file);
    fclose(file);
    
    return buffer;
}

TUInt GVULKANTools::findMemoryType(VkPhysicalDevice device, const TUInt typeFilter, const VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);
    
    for (TUInt i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    
    return 0;
}

VkImageView GVULKANTools::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkDevice device) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView newImageView;
    vkCreateImageView(device, &viewInfo, nullptr, &newImageView);
    return newImageView;
}

VkFormat GVULKANTools::findDepthFormat(GVULKANDevice& vulkanDevice) {
    return findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
                               VK_IMAGE_TILING_OPTIMAL,
                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
                               vulkanDevice);
}

VkFormat GVULKANTools::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, GVULKANDevice& vulkanDevice) {
    for (VkFormat format : candidates) {
        VkFormatProperties props = vulkanDevice.getPhysicalDeviceFormatProperties(format);
        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        
        if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }
    
    throw std::runtime_error("failed to find supported format!");
}

}
