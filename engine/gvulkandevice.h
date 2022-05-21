//
//  gvulkandevice.hpp
//  game
//
//  Created by cipher on 20.05.2022.
//

#ifndef SPCGANESHAENGINE_GVULKANDEVICE_H
#define SPCGANESHAENGINE_GVULKANDEVICE_H

#include <vulkan/vulkan.h>

namespace spcGaneshaEngine {

class GVULKANInstance;

class GVULKANDevice {
public:
    void createPhysicalDevice(GVULKANInstance &vulkanInstance, VkSurfaceKHR &metalSurface);
    void createLogicalDevice();
    VkCommandPool createCommandPool();
    void destroyDevice();
    
    VkPhysicalDevice& getPhysicalDevice();
    VkDevice& getLogicalDevice();

    VkQueue& getGraphicsQueue();
    VkQueue& getPresentQueue();

private:
    bool checkDeviceCapability(const VkPhysicalDevice& device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice& device);
    void findQueuesIndeces(VkSurfaceKHR& metalSurface);
    void setupLogicalDevice();

    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;

    VkQueue graphicsQueue;
    int32_t graphicQueueFamilyIndex = -1;

    VkQueue presentQueue;
    int32_t presentQueueFamilyIndex = -1;
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANDEVICE_H
