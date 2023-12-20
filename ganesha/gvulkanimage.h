
#ifndef SPCGANESHAENGINE_GVULKANIMAGE_H
#define SPCGANESHAENGINE_GVULKANIMAGE_H

#include <vulkan/vulkan.h>

#include "gvulkancommands.h"
#include "gvulkandevice.h"
#include "glog.h"

namespace spcGaneshaEngine {

class GVULKANImage {
public:
    GVULKANImage(GLog& log);
    ~GVULKANImage();

    void createImage(const std::string filePath,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     GVULKANDevice& vulkanDevice,
                     GVULKANCommands& vulkanCommands);
    void destroyImage(GVULKANDevice& vulkanDevice);

private:
    GLog& log;
    VkImage image;
    VkDeviceMemory imageMemory;
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANBUFFER_H
