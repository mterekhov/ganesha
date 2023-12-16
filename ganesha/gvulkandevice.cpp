//
//  gvulkandevice.cpp
//  game
//
//  Created by cipher on 20.05.2022.
//

#include <set>
#include <string>

#include "gvulkandevice.h"
#include "gvulkaninstance.h"

namespace spcGaneshaEngine {

const TCharPointersArray deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

GVULKANDevice::GVULKANDevice(GLog& log) : log(log) {
    
}

GVULKANDevice::~GVULKANDevice() {
    
}

void GVULKANDevice::createDevice(GVULKANInstance &vulkanInstance, VkSurfaceKHR &surface) {
    physicalDevice = selectPhysicalDevice(vulkanInstance, surface);
    logicalDevice = createLogicalDevice(physicalDevice, surface);
    
    vkGetDeviceQueue(logicalDevice, graphicQueueFamilyIndex, 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, presentQueueFamilyIndex, 0, &presentQueue);
}

VkPhysicalDevice GVULKANDevice::selectPhysicalDevice(GVULKANInstance &vulkanInstance, VkSurfaceKHR &surface) {
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(vulkanInstance.getVulkanInstance(), &count, nullptr);
    std::vector<VkPhysicalDevice> physicalDevicesArray(count);
    vkEnumeratePhysicalDevices(vulkanInstance.getVulkanInstance(), &count, physicalDevicesArray.data());
    
    for (const auto& device : physicalDevicesArray) {
        if (checkPhysicalDeviceCapability(device, surface)) {
            return device;
        }
    }
    
    return physicalDevicesArray[0];
}

VkDevice GVULKANDevice::createLogicalDevice(VkPhysicalDevice& device, VkSurfaceKHR &metalSurface) {
    findQueuesIndeces(metalSurface);
    std::set<int32_t> uniqueQueueFamilies = { graphicQueueFamilyIndex, presentQueueFamilyIndex };
    TFloat queuePriority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfosList;
    for (int32_t queueIndex : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo deviceQueueInfo = {};
        
        deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueInfo.queueFamilyIndex = queueIndex;
        deviceQueueInfo.queueCount = 1;
        deviceQueueInfo.pQueuePriorities = &queuePriority;

        queueCreateInfosList.push_back(deviceQueueInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo logicalDeviceInfo = {};
    logicalDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    logicalDeviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfosList.size());
    logicalDeviceInfo.pQueueCreateInfos = queueCreateInfosList.data();
    logicalDeviceInfo.pEnabledFeatures = &deviceFeatures;
    
    if (checkPhysicalDeviceExtensionSupport(physicalDevice, deviceExtensions)) {
        logicalDeviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        logicalDeviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
    }
    
    VkDevice newDevice;
    if (vkCreateDevice(physicalDevice, &logicalDeviceInfo, nullptr, &newDevice) != VK_SUCCESS) {
        log.error("error creating logical device\n");
    }

    return newDevice;
}

void GVULKANDevice::destroyDevice() {
    vkDestroyDevice(logicalDevice, nullptr);
}

VkCommandPool GVULKANDevice::createCommandPool() {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicQueueFamilyIndex;

    VkCommandPool commandPool;
    if (vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        printf("GaneshaEngine: failed to create command pool\n");
    }
    
    return commandPool;
}

bool GVULKANDevice::presentationIsEqualToGraphics() {
    return graphicQueueFamilyIndex == presentQueueFamilyIndex;
}

VkPhysicalDevice& GVULKANDevice::getPhysicalDevice() {
    return physicalDevice;
}

VkDevice& GVULKANDevice::getLogicalDevice() {
    return logicalDevice;
}

VkQueue& GVULKANDevice::getGraphicsQueue() {
    return graphicsQueue;
}

VkQueue& GVULKANDevice::getPresentQueue() {
    return presentQueue;
}

std::vector<uint32_t> GVULKANDevice::getQueuesIndecesArray() {
    return { static_cast<uint32_t>(graphicQueueFamilyIndex), static_cast<uint32_t>(presentQueueFamilyIndex) };
}

SwapChainSupportDetails GVULKANDevice::querySwapChainSupport(VkSurfaceKHR& surface) {
    return querySwapChainSupport(physicalDevice, surface);
}

#pragma mark - Routine -

SwapChainSupportDetails GVULKANDevice::querySwapChainSupport(const VkPhysicalDevice& device, VkSurfaceKHR& surface) {
    SwapChainSupportDetails details = {0};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.surfaceCapabilities);
    
    uint32_t count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);
    if (count != 0) {
        details.formats.resize(count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, details.formats.data());
    }
    
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);
    if (count != 0) {
        details.presentModes.resize(count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, details.presentModes.data());
    }
    
    return details;
}

bool GVULKANDevice::checkPhysicalDeviceCapability(const VkPhysicalDevice& device, VkSurfaceKHR &surface) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
        return false;
    }

    if (!checkPhysicalDeviceExtensionSupport(device, deviceExtensions)) {
        return false;
    }
    else {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
        if ((swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty())) {
            return false;
        }

    }
    
    return true;
}

bool GVULKANDevice::checkPhysicalDeviceExtensionSupport(const VkPhysicalDevice& device, const TCharPointersArray& extensionsToSupport) {
    uint32_t count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, availableExtensions.data());
    
    std::set<std::string> requiredExtensions(extensionsToSupport.begin(), extensionsToSupport.end());
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }
    
    return requiredExtensions.empty();
}

void GVULKANDevice::findQueuesIndeces(VkSurfaceKHR& metalSurface) {
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamiliesArray(count);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, queueFamiliesArray.data());

    graphicQueueFamilyIndex = findGraphicsQueueIndex(queueFamiliesArray);
    presentQueueFamilyIndex = findPresentQueueIndex(queueFamiliesArray, metalSurface);
}

int32_t GVULKANDevice::findGraphicsQueueIndex(const std::vector<VkQueueFamilyProperties>& queueFamiliesArray) {
    for (uint32_t i = 0; i < queueFamiliesArray.size(); i++) {
        const auto& properties = queueFamiliesArray[i];
        if ((properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
            return i;
        }
    }
    
    return -1;
}

int32_t GVULKANDevice::findPresentQueueIndex(const std::vector<VkQueueFamilyProperties>& queueFamiliesArray, VkSurfaceKHR& metalSurface) {
    for (int32_t familyIndex = 0; familyIndex < queueFamiliesArray.size(); familyIndex++) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, familyIndex, metalSurface, &presentSupport);
        if (presentSupport) {
            return familyIndex;
        }
    }
    
    return -1;
}

}   //  namespace spcGaneshaEngine
