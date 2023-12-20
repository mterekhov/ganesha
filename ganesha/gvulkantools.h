#ifndef SPCGANESHAENGINE_GVULKANTOOLS_H
#define SPCGANESHAENGINE_GVULKANTOOLS_H

#include <vulkan/vulkan.h>

#include "ganeshatypes.h"

namespace spcGaneshaEngine {

class GVULKANTools {
public:
    TUInt findMemoryType(VkPhysicalDevice device, const TUInt typeFilter, const VkMemoryPropertyFlags properties);
    VkImageView createImageView(VkImage image, VkFormat format,  VkDevice device);
};

};

#endif
