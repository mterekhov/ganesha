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

#pragma mark - Public -

void GVULKANDevice::createPhysicalDevice(GVULKANInstance &vulkanInstance) {
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(vulkanInstance.getVulkanInstance(), &count, nullptr);
    std::vector<VkPhysicalDevice> physicalDevicesArray(count);
    vkEnumeratePhysicalDevices(vulkanInstance.getVulkanInstance(), &count, physicalDevicesArray.data());
    
    for (const auto& device : physicalDevicesArray) {
        if (checkPhysicalDeviceCapability(device)) {
            physicalDevice = device;
            break;
        }
    }
}

void GVULKANDevice::createLogicalDevice(VkSurfaceKHR &metalSurface) {
    findQueuesIndeces(metalSurface);
    std::set<int32_t> uniqueQueueFamilies = {graphicQueueFamilyIndex, presentQueueFamilyIndex};
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
    
    if (vkCreateDevice(physicalDevice, &logicalDeviceInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        log.error("error creating logical device\n");
    }

    vkGetDeviceQueue(logicalDevice, graphicQueueFamilyIndex, 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, presentQueueFamilyIndex, 0, &presentQueue);
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

void GVULKANDevice::destroyDevice() {
    vkDestroyDevice(logicalDevice, nullptr);
}

#pragma mark - Routine -

bool GVULKANDevice::checkPhysicalDeviceCapability(const VkPhysicalDevice& device) {
    //  Find discrete GPU or integrated in case of Apple M1
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
        return false;
    }
    
    return true;
}

bool GVULKANDevice::checkPhysicalDeviceExtensionSupport(VkPhysicalDevice& device, const TCharPointersArray& extensionsToSupport) {
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
