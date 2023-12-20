#include "gvulkantools.h"

namespace spcGaneshaEngine {

TUInt GVULKANTools::findMemoryType(VkPhysicalDevice device, const TUInt typeFilter, const VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);
    
    for (TUInt i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    
    return 0;
}

}
