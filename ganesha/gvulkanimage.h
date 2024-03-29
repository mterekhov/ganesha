
#ifndef SPCGANESHAENGINE_GVULKANIMAGE_H
#define SPCGANESHAENGINE_GVULKANIMAGE_H

#include <vulkan/vulkan.h>

#include "gvulkandevice.h"
#include "gtga.h"
#include "gcommandservice.h"

namespace spcGaneshaEngine {

class GVULKANImage {
public:
    GVULKANImage();
    ~GVULKANImage();

    void createImage(const VkExtent2D& extent,
                     VkFormat format,
                     VkImageAspectFlags aspectFlags,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     GVULKANDevice& vulkanDevice);
    void deployData(GTGA& tgaFile,
                    GVULKANDevice& vulkanDevice,
                    GCommandServiceProtocol *commandService);
    void destroyImage(VkDevice device);
    
    VkImageView getImageView();
    VkSampler getSampler();

private:
    VkExtent2D imageExtent;
    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
    VkSampler sampler;
    
    VkSampler createTextureSampler(GVULKANDevice& device);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, GCommandServiceProtocol *commandService);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, GCommandServiceProtocol *commandService);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANBUFFER_H
