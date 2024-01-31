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

}
