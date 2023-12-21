
#ifndef SPCGANESHAENGINE_GVULKANIMAGE_H
#define SPCGANESHAENGINE_GVULKANIMAGE_H

#include <vulkan/vulkan.h>

#include "gvulkancommands.h"
#include "gvulkandevice.h"
#include "glog.h"
#include "gtga.h"

namespace spcGaneshaEngine {

class GVULKANImage {
public:
    GVULKANImage(GLog& log);
    ~GVULKANImage();

    void createImage(const VkExtent2D& extent,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     GVULKANDevice& vulkanDevice);
    void deployData(GTGA& tgaFile,
                    GVULKANDevice& vulkanDevice,
                    GVULKANCommands& vulkanCommands);
    void destroyImage(GVULKANDevice& vulkanDevice);
    
    VkImageView getImageView();
    VkSampler getSampler();

private:
    GLog& log;
    VkExtent2D imageExtent;
    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
    VkSampler sampler;
    
    VkSampler createTextureSampler(GVULKANDevice& device);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANBUFFER_H
