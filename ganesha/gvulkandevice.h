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

    void createDevice(GVULKANInstance &vulkanInstance, const TStringsArray& useDeviceExtensions, VkSurfaceKHR &surface);
    void destroyDevice();

    VkCommandPool createCommandPool();
    
    SwapChainSupportDetails querySwapChainSupport(VkSurfaceKHR& surface);
    VkPhysicalDevice& getPhysicalDevice();
    VkDevice& getLogicalDevice();
    VkQueue& getPresentQueue();
    VkQueue& getGraphicsQueue();
    uint32_t getGraphicsQueueIndex();
    std::vector<uint32_t> getQueuesIndecesArray();
    
    bool presentationIsEqualToGraphics();

private:
    VkPhysicalDevice selectPhysicalDevice(GVULKANInstance &vulkanInstance, const TStringsArray& useDeviceExtensions, VkSurfaceKHR &surface);
    VkDevice createLogicalDevice(VkPhysicalDevice& device, const TStringsArray& useDeviceExtensions, VkSurfaceKHR &metalSurface);

    std::vector<VkExtensionProperties> collectAvailableExtensions(const VkPhysicalDevice& device);
    TCharPointersArray collectAvailableExtensionsNames(const std::vector<VkExtensionProperties>& extensionArray, const TStringsArray& useDeviceExtensions);

    bool checkPhysicalDeviceCapability(const VkPhysicalDevice& device, const TStringsArray& useDeviceExtensions, VkSurfaceKHR &surface);
    bool checkPhysicalDeviceExtensionSupport(const VkPhysicalDevice& device, const TStringsArray& useDeviceExtensions, const std::vector<VkExtensionProperties>& availableExtensions);

    int32_t findGraphicsQueueIndex(const std::vector<VkQueueFamilyProperties>& queuePropertiesArray);
    int32_t findPresentQueueIndex(const std::vector<VkQueueFamilyProperties>& queuePropertiesArray, VkSurfaceKHR& metalSurface);
    void findQueuesIndeces(VkSurfaceKHR& metalSurface);
    SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device, VkSurfaceKHR& surface);

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
