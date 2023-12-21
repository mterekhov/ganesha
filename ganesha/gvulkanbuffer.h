#ifndef SPCGANESHAENGINE_GVULKANBUFFER_H
#define SPCGANESHAENGINE_GVULKANBUFFER_H

#include <vulkan/vulkan.h>

#include "gvulkandevice.h"
#include "glog.h"

namespace spcGaneshaEngine {

class GVULKANBuffer {
public:
    GVULKANBuffer(GLog& log);
    ~GVULKANBuffer();

    void createBuffer(const void *data,
                      const VkDeviceSize size,
                      const VkBufferUsageFlags usage,
                      const VkMemoryPropertyFlags properties,
                      const TBool protectAccess,
                      GVULKANDevice& vulkanDevice,
                      VkCommandPool commandPool);
    void destroyBuffer(GVULKANDevice& vulkanDevice);
    void refreshBuffer(const void *data, GVULKANDevice& vulkanDevice);
    VkBuffer getBuffer();
    TUInt getBufferSize();

private:
    GLog& log;
    VkBuffer buffer;
    TUInt bufferSize;
    VkDeviceMemory bufferMemory;
    
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, const VkDeviceSize size, GVULKANDevice& vulkanDevice, VkCommandPool commandPool);

    VkBuffer createBuffer(VkDevice device, const VkDeviceSize size, const VkBufferUsageFlags usage);
    VkDeviceMemory allocateBufferMemory(VkBuffer originalBuffer, const VkMemoryPropertyFlags properties, GVULKANDevice& vulkanDevice);
    TUInt findMemoryType(VkPhysicalDevice device, const TUInt typeFilter, const VkMemoryPropertyFlags properties);
    VkCommandBuffer copyBufferCommand(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool, VkDevice device);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANBUFFER_H
