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

    void createDevice(GVULKANInstance& vulkanInstance, const TStringsArray& useDeviceExtensions, VkSurfaceKHR &surface);
    void destroyDevice();

    VkCommandPool createCommandPool();
    
    SwapChainSupportDetails querySwapChainSupport(VkSurfaceKHR& surface);
    VkPhysicalDevice getPhysicalDevice();
    VkDevice getLogicalDevice();
    VkQueue& getPresentQueue();
    VkQueue& getGraphicsQueue();
    TUInt getGraphicsQueueIndex();
    std::vector<TUInt> getQueuesIndexesArray();
    VkPhysicalDeviceProperties getPhysicalDeviceProperties();

    TBool presentationIsEqualToGraphics();

private:
    VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice device);
    VkPhysicalDevice selectPhysicalDevice(GVULKANInstance &vulkanInstance, const TStringsArray& useDeviceExtensions, VkSurfaceKHR &surface);
    VkDevice createLogicalDevice(VkPhysicalDevice device, const TStringsArray& useDeviceExtensions, VkSurfaceKHR &metalSurface);

    std::vector<VkExtensionProperties> collectAvailableExtensions(VkPhysicalDevice device);
    TCharPointersArray collectAvailableExtensionsNames(const std::vector<VkExtensionProperties>& extensionArray, const TStringsArray& useDeviceExtensions);

    TBool checkPhysicalDeviceCapability(VkPhysicalDevice device, const TStringsArray& useDeviceExtensions, VkSurfaceKHR &surface);
    TBool checkPhysicalDeviceExtensionSupport(VkPhysicalDevice device, const TStringsArray& useDeviceExtensions, const std::vector<VkExtensionProperties>& availableExtensions);

    int32_t findGraphicsQueueIndex(const std::vector<VkQueueFamilyProperties>& queuePropertiesArray);
    int32_t findPresentQueueIndex(const std::vector<VkQueueFamilyProperties>& queuePropertiesArray, VkSurfaceKHR& metalSurface);
    void findQueuesIndexes(VkSurfaceKHR& metalSurface);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface);

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
