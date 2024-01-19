
#ifndef SPCGANESHAENGINE_GVULKANIMAGE_H
#define SPCGANESHAENGINE_GVULKANIMAGE_H

#include <vulkan/vulkan.h>

#include "gvulkandevice.h"
#include "glog.h"
#include "gtga.h"

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
                    VkCommandPool commandPool);
    void destroyImage(VkDevice device);
    
    VkImageView getImageView();
    VkSampler getSampler();
    VkDescriptorSet getDescriptorSet();

private:
    VkExtent2D imageExtent;
    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
    VkSampler sampler;
    
    VkSampler createTextureSampler(GVULKANDevice& device);
    VkCommandBuffer copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkCommandPool commandPool, VkDevice device);
    VkCommandBuffer transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandPool commandPool, VkDevice device);
    void submitCommand(VkCommandBuffer command, VkCommandPool commandPool, GVULKANDevice& vulkanDevice);
    VkCommandBuffer allocateCommandBuffer(VkCommandPool commandPool, VkDevice device);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANBUFFER_H
