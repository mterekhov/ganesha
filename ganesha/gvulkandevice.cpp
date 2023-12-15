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

#pragma mark - Public -

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

void GVULKANDevice::createPhysicalDevice(GVULKANInstance &vulkanInstance, VkSurfaceKHR &metalSurface) {
    uint32_t devicesCount = 0;
    vkEnumeratePhysicalDevices(vulkanInstance.getVulkanInstance(), &devicesCount, nullptr);
    
    std::vector<VkPhysicalDevice> devicesList(devicesCount);
    vkEnumeratePhysicalDevices(vulkanInstance.getVulkanInstance(), &devicesCount, devicesList.data());
    for (const auto& device : devicesList) {
        if (checkDeviceCapability(device)) {
            physicalDevice = device;
            break;
        }
    }
    
    findQueuesIndeces(metalSurface);
}

void GVULKANDevice::createLogicalDevice() {
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfosList;
    std::set<int32_t> uniqueQueueFamilies = {graphicQueueFamilyIndex, presentQueueFamilyIndex};
    float queuePriority = 1.0f;
    for (int queueIndex : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo deviceQueueInfo = {};
        
        deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueInfo.queueFamilyIndex = queueIndex;
        deviceQueueInfo.queueCount = 1;
        deviceQueueInfo.pQueuePriorities = &queuePriority;

        queueCreateInfosList.push_back(deviceQueueInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfosList.size());
    deviceInfo.pQueueCreateInfos = queueCreateInfosList.data();
    deviceInfo.pEnabledFeatures = &deviceFeatures;
    
    if (checkDeviceExtensionSupport(physicalDevice)) {
        deviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
    }
    
    if (vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        printf("GaneshaEngine: error creating logical device\n");
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

void GVULKANDevice::destroyDevice() {
    vkDestroyDevice(logicalDevice, nullptr);
}

#pragma mark - Routine -

bool GVULKANDevice::checkDeviceCapability(const VkPhysicalDevice& device) {
    //  Find discrete GPU or integrated in case of Apple M1
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
        return false;
    }
    
    return true;
}

bool GVULKANDevice::checkDeviceExtensionSupport(VkPhysicalDevice& device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
    
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }
    
    return requiredExtensions.empty();
}

void GVULKANDevice::findQueuesIndeces(VkSurfaceKHR& metalSurface) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamiliesList(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamiliesList.data());

    for (uint32_t i = 0; i < queueFamiliesList.size(); i++) {
        const auto& queueFamily = queueFamiliesList[i];
        if ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
            graphicQueueFamilyIndex = i;
        }
        
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, metalSurface, &presentSupport);
        if (presentSupport) {
            presentQueueFamilyIndex = i;
        }

        if (presentQueueFamilyIndex > 0 && graphicQueueFamilyIndex > 0) {
            return;
        }
    }
}

}   //  namespace spcGaneshaEngine
