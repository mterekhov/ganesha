#ifndef SPCGANESHAENGINE_GVULKANTOOLS_H
#define SPCGANESHAENGINE_GVULKANTOOLS_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace spcGaneshaEngine {

class GVULKANTools {
public:
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkDevice device);
    std::vector<uint8_t> readFile(const std::string& filename);
    
};

};

#endif
