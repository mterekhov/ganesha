#ifndef SPCGANESHAENGINE_GVULKANBUFFER_H
#define SPCGANESHAENGINE_GVULKANBUFFER_H

#include <vulkan/vulkan.h>

#include "gvulkancommands.h"
#include "gvulkandevice.h"
#include "glog.h"

namespace spcGaneshaEngine {

class GVULKANBuffer {
public:
    GVULKANBuffer(GLog& log);
    ~GVULKANBuffer();

    void createBuffer(const void *data,
                      const VkDeviceSize size,
                      VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties,
                      bool protectAccess,
                      GVULKANDevice& vulkanDevice,
                      GVULKANCommands& vulkanCommands);
    void destroyBuffer(GVULKANDevice& vulkanDevice);
    void refreshBuffer(const void *data, const VkDeviceSize size, GVULKANDevice& vulkanDevice);
    VkBuffer& getBuffer();
    
private:
    GLog& log;
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    
    VkBuffer createBuffer(VkDevice& device, VkDeviceSize size, VkBufferUsageFlags usage);
    VkDeviceMemory allocateBufferMemory(VkBuffer& originalBuffer, VkMemoryPropertyFlags properties, GVULKANDevice& vulkanDevice);
    void copyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, const VkDeviceSize size, GVULKANDevice& vulkanDevice, GVULKANCommands& vulkanCommands);
    uint32_t findMemoryType(const VkPhysicalDevice& device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANBUFFER_H
