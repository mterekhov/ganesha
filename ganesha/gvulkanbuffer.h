#ifndef SPCGANESHAENGINE_GVULKANBUFFER_H
#define SPCGANESHAENGINE_GVULKANBUFFER_H

#include <vulkan/vulkan.h>

#include "gvulkandevice.h"
#include "gcommandservice.h"
#include "ganeshatypes.h"

namespace spcGaneshaEngine {

class GVULKANBuffer {
public:
    GVULKANBuffer();
    ~GVULKANBuffer();

    void createBuffer(const void *data,
                      const VkDeviceSize size,
                      const VkBufferUsageFlags usage,
                      const VkMemoryPropertyFlags properties,
                      const TBool protectAccess,
                      GCommandServiceProtocol *commandService,
                      GVULKANDevice& vulkanDevice);
    void destroyBuffer(VkDevice device);
    
    /// Refresh data which size is the same
    /// - Parameters:
    ///   - data: new data
    ///   - device: VULKAN logical device
    void refreshBuffer(const void *data, VkDevice device);
    
    VkBuffer getBuffer();
    TUInt getBufferSize();

private:
    VkBuffer buffer;
    TUInt bufferSize;
    VkDeviceMemory bufferMemory;
    
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, const VkDeviceSize size, GCommandServiceProtocol *commandService);
    VkBuffer createBuffer(const VkDeviceSize size, const VkBufferUsageFlags usage, VkDevice device);
    VkDeviceMemory allocateBufferMemory(VkBuffer originalBuffer, const VkMemoryPropertyFlags properties, GVULKANDevice& vulkanDevice);
};

};   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANBUFFER_H
