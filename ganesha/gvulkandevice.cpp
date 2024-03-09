#include <set>
#include <string>
#include <iostream>

#include "gvulkandevice.h"

namespace spcGaneshaEngine {

GVULKANDevice::GVULKANDevice() {
    
}

GVULKANDevice::~GVULKANDevice() {
    
}

void GVULKANDevice::createDevice(GVULKANInstance &vulkanInstance, const TStringsArray& useDeviceExtensions, VkSurfaceKHR &surface) {
    physicalDevice = selectPhysicalDevice(vulkanInstance, useDeviceExtensions, surface);
    logicalDevice = createLogicalDevice(physicalDevice, useDeviceExtensions, surface);
    
    vkGetDeviceQueue(logicalDevice, graphicQueueFamilyIndex, 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, presentQueueFamilyIndex, 0, &presentQueue);
}

void GVULKANDevice::destroyDevice() {
    vkDestroyDevice(logicalDevice, nullptr);
}

TBool GVULKANDevice::presentationIsEqualToGraphics() {
    return graphicQueueFamilyIndex == presentQueueFamilyIndex;
}

VkPhysicalDevice GVULKANDevice::getPhysicalDevice() {
    return physicalDevice;
}

VkDevice GVULKANDevice::getLogicalDevice() {
    return logicalDevice;
}

TUInt GVULKANDevice::getGraphicsQueueIndex() {
    return graphicQueueFamilyIndex;
}

VkQueue& GVULKANDevice::getGraphicsQueue() {
    return graphicsQueue;
}

VkQueue& GVULKANDevice::getPresentQueue() {
    return presentQueue;
}

std::vector<TUInt> GVULKANDevice::getQueuesIndexesArray() {
    return { static_cast<TUInt>(graphicQueueFamilyIndex), static_cast<TUInt>(presentQueueFamilyIndex) };
}

SwapChainSupportDetails GVULKANDevice::querySwapChainSupport(VkSurfaceKHR& surface) {
    return querySwapChainSupport(physicalDevice, surface);
}

VkPhysicalDeviceProperties GVULKANDevice::getPhysicalDeviceProperties() {
    return getPhysicalDeviceProperties(physicalDevice);
}

VkFormatProperties GVULKANDevice::getPhysicalDeviceFormatProperties(VkFormat format) {
    VkFormatProperties formatProperties;
    
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);
    
    return formatProperties;
}

TUInt GVULKANDevice::findMemoryType(const TUInt typeFilter, const VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
    
    for (TUInt i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    
    return 0;
}

VkFormat GVULKANDevice::findDepthImageFormat() {
    std::vector<VkFormat> candidates = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
    VkFormatFeatureFlags features = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
    
    for (VkFormat format : candidates) {
        VkFormatProperties props = getPhysicalDeviceFormatProperties(format);
        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        
        if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }
    
    throw std::runtime_error("failed to find supported format!");
}

#pragma mark - Routine -

VkPhysicalDeviceProperties GVULKANDevice::getPhysicalDeviceProperties(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    
    return deviceProperties;

}

VkPhysicalDevice GVULKANDevice::selectPhysicalDevice(GVULKANInstance &vulkanInstance, const TStringsArray& useDeviceExtensions, VkSurfaceKHR &surface) {
    TUInt count = 0;
    vkEnumeratePhysicalDevices(vulkanInstance.getVulkanInstance(), &count, nullptr);
    std::vector<VkPhysicalDevice> physicalDevicesArray(count);
    vkEnumeratePhysicalDevices(vulkanInstance.getVulkanInstance(), &count, physicalDevicesArray.data());
    
    for (const auto& device : physicalDevicesArray) {
        if (checkPhysicalDeviceCapability(device, useDeviceExtensions, surface)) {
            return device;
        }
    }
    
    return physicalDevicesArray[0];
}

VkDevice GVULKANDevice::createLogicalDevice(VkPhysicalDevice device, const TStringsArray& useDeviceExtensions, VkSurfaceKHR &metalSurface) {
    findQueuesIndexes(metalSurface);
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
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo logicalDeviceInfo = {};
    logicalDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    logicalDeviceInfo.queueCreateInfoCount = static_cast<TUInt>(queueCreateInfosList.size());
    logicalDeviceInfo.pQueueCreateInfos = queueCreateInfosList.data();
    logicalDeviceInfo.pEnabledFeatures = &deviceFeatures;
    
    std::vector<VkExtensionProperties> availableExtensions = collectAvailableExtensions(device);
    TCharPointersArray deviceExtensionsNamesArray = collectAvailableExtensionsNames(availableExtensions, useDeviceExtensions);
    logicalDeviceInfo.enabledExtensionCount = static_cast<TUInt>(deviceExtensionsNamesArray.size());
    logicalDeviceInfo.ppEnabledExtensionNames = deviceExtensionsNamesArray.data();

    VkDevice newDevice;
    if (vkCreateDevice(physicalDevice, &logicalDeviceInfo, nullptr, &newDevice) != VK_SUCCESS) {
        GLOG_ERROR("error creating logical device\n");
    }

    for (const auto& name : deviceExtensionsNamesArray) {
        delete [] name;
    }

    return newDevice;
}

TCharPointersArray GVULKANDevice::collectAvailableExtensionsNames(const std::vector<VkExtensionProperties>& extensionArray, const TStringsArray& useDeviceExtensions) {
    TCharPointersArray namesArray;
    
    for (const auto& extension : extensionArray) {
        for (auto shouldUseExtensionName:useDeviceExtensions) {
            if (strcmp(shouldUseExtensionName.c_str(), extension.extensionName) == 0) {
                char *newString = new char[VK_MAX_EXTENSION_NAME_SIZE];
                memcpy(newString, extension.extensionName, VK_MAX_EXTENSION_NAME_SIZE);
                namesArray.emplace_back(newString);
                continue;
            }
        }
    }

    return namesArray;
}

std::vector<VkExtensionProperties> GVULKANDevice::collectAvailableExtensions(VkPhysicalDevice device) {
    TUInt count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
    
    std::vector<VkExtensionProperties> availableExtensions(count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, availableExtensions.data());

    return availableExtensions;
}

TBool GVULKANDevice::checkPhysicalDeviceCapability(VkPhysicalDevice device, const TStringsArray& useDeviceExtensions, VkSurfaceKHR &surface) {
    VkPhysicalDeviceProperties deviceProperties = getPhysicalDeviceProperties(device);
    if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
        return false;
    }

    std::vector<VkExtensionProperties> availableExtensions = collectAvailableExtensions(device);
    if (!checkPhysicalDeviceExtensionSupport(device, useDeviceExtensions, availableExtensions)) {
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

TBool GVULKANDevice::checkPhysicalDeviceExtensionSupport(VkPhysicalDevice device, const TStringsArray& useDeviceExtensions, const std::vector<VkExtensionProperties>& availableExtensions) {
    std::set<std::string> requiredExtensions(useDeviceExtensions.begin(), useDeviceExtensions.end());
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }
    
    return requiredExtensions.empty();
}

SwapChainSupportDetails GVULKANDevice::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface) {
    SwapChainSupportDetails details = {0};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.surfaceCapabilities);
    
    TUInt count;
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

void GVULKANDevice::findQueuesIndexes(VkSurfaceKHR& metalSurface) {
    TUInt count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamiliesArray(count);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, queueFamiliesArray.data());

    graphicQueueFamilyIndex = findGraphicsQueueIndex(queueFamiliesArray);
    presentQueueFamilyIndex = findPresentQueueIndex(queueFamiliesArray, metalSurface);
}

int32_t GVULKANDevice::findGraphicsQueueIndex(const std::vector<VkQueueFamilyProperties>& queueFamiliesArray) {
    for (TUInt i = 0; i < queueFamiliesArray.size(); i++) {
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
