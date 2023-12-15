//
//  gvulkandevice.hpp
//  game
//
//  Created by cipher on 20.05.2022.
//

#ifndef SPCGANESHAENGINE_GVULKANDEVICE_H
#define SPCGANESHAENGINE_GVULKANDEVICE_H

#include <vulkan/vulkan.h>

#include "glog.h"
#include "ganeshatypes.h"
#include "gvulkaninstance.h"

namespace spcGaneshaEngine {

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class GVULKANDevice {
public:
    GVULKANDevice(GLog& log);
    ~GVULKANDevice();

    void selectPhysicalDevice(GVULKANInstance &vulkanInstance, VkSurfaceKHR &surface);
    void createLogicalDevice(VkSurfaceKHR &metalSurface);
    VkCommandPool createCommandPool();
    void destroyLogicalDevice();
    
    SwapChainSupportDetails querySwapChainSupport(VkSurfaceKHR& surface);

    VkPhysicalDevice& getPhysicalDevice();
    VkDevice& getLogicalDevice();
    VkQueue& getGraphicsQueue();
    VkQueue& getPresentQueue();

private:
    SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device, VkSurfaceKHR& surface);
    bool checkPhysicalDeviceCapability(const VkPhysicalDevice& device, VkSurfaceKHR &surface);
    bool checkPhysicalDeviceExtensionSupport(const VkPhysicalDevice& device, const TCharPointersArray& extensionsToSupport);
    int32_t findGraphicsQueueIndex(const std::vector<VkQueueFamilyProperties>& queuePropertiesArray);
    int32_t findPresentQueueIndex(const std::vector<VkQueueFamilyProperties>& queuePropertiesArray, VkSurfaceKHR& metalSurface);
    void findQueuesIndeces(VkSurfaceKHR& metalSurface);
    void setupLogicalDevice();

    GLog& log;

    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;

    VkQueue graphicsQueue;
    int32_t graphicQueueFamilyIndex = -1;

    VkQueue presentQueue;
    int32_t presentQueueFamilyIndex = -1;
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANDEVICE_H
