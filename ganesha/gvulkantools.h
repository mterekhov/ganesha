#ifndef SPCGANESHAENGINE_GVULKANTOOLS_H
#define SPCGANESHAENGINE_GVULKANTOOLS_H

#include <vulkan/vulkan.h>

#include "ganeshatypes.h"
#include "gvulkandevice.h"

namespace spcGaneshaEngine {

class GVULKANTools {
public:
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkDevice device);
    VkFormat findDepthFormat(GVULKANDevice& vulkanDevice);
    std::vector<uint8_t> readFile(const std::string& filename);

private:
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, GVULKANDevice& vulkanDevice);

};

};

#endif
