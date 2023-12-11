//
//  gvulkaninstance.cpp
//  ganesha
//
//  Created by Mihail Terekhov on 23.07.2021.
//

#include "gvulkaninstance.h"
#include <vulkan/vulkan.h>

namespace spcGaneshaEngine {

static const TCharPointersArray khronosValidationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

void GVULKANInstance::createInstance(const bool useValidationLayers) {
    createNewInstance(useValidationLayers);
    if (useValidationLayers) {
        if (createDebugUtilsMessenger(vulkanInstance, &debugUtilsMessengerInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            printf("GaneshaEngine: failed to set up debug messenger\n");
        }
    }
}

void GVULKANInstance::destroyInstance() {
    destroyDebugUtilsMessenger(vulkanInstance, debugMessenger, nullptr);
    vkDestroyInstance(vulkanInstance, nullptr);
    
    for (const auto& name : extensionsNamesList) {
        delete [] name;
    }
}

VkInstance& GVULKANInstance::getVulkanInstance() {
    return vulkanInstance;
}

#pragma mark - Routine -

void GVULKANInstance::createNewInstance(const bool useValidationLayers) {
    extensionsList = collectInstanceExtensions();
    extensionsNamesList = collectInstanceExtensionsNames(extensionsList);
    availableValidationLayersList = collectValidationLayers(khronosValidationLayers);
    
    VkInstanceCreateInfo instanceInfo = createInstanceInfo(useValidationLayers);
    VkResult error = VK_SUCCESS;
    error = vkCreateInstance(&instanceInfo, nullptr, &vulkanInstance);
    if (error != VK_SUCCESS) {
        printf("GaneshaEngine: error creating VULKAN instance\n");
        return;
    }
}

VkApplicationInfo GVULKANInstance::createApplicationInfo() {
    VkApplicationInfo newApplicationInfo = {};
    
    uint32_t pApiVersion = 0;
    vkEnumerateInstanceVersion(&pApiVersion);
    
    newApplicationInfo.apiVersion = pApiVersion;
    newApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    newApplicationInfo.pApplicationName = "GAME";
    newApplicationInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
    newApplicationInfo.pEngineName = "Ganesha";
    newApplicationInfo.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
    
    return newApplicationInfo;
}

VkInstanceCreateInfo GVULKANInstance::createInstanceInfo(const bool useValidationLayers) {
    VkInstanceCreateInfo instanceInfo = {};
    
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensionsNamesList.size());
    instanceInfo.ppEnabledExtensionNames = extensionsNamesList.data();
    
    applicationInfo = createApplicationInfo();
    instanceInfo.pApplicationInfo = &applicationInfo;
    
    if (useValidationLayers && availableValidationLayersList.size() > 0) {
        instanceInfo.enabledLayerCount = static_cast<uint32_t>(availableValidationLayersList.size());
        instanceInfo.ppEnabledLayerNames = availableValidationLayersList.data();
        debugUtilsMessengerInfo = createDebugUtilsMessengerInfo();
        instanceInfo.pNext = &debugUtilsMessengerInfo;
    }
    
    return instanceInfo;
}

#pragma mark - Validation Layers -

VkDebugUtilsMessengerCreateInfoEXT GVULKANInstance::createDebugUtilsMessengerInfo() {
    VkDebugUtilsMessengerCreateInfoEXT newDebugUtilsMessengerInfo = {};
    
    newDebugUtilsMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    newDebugUtilsMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    newDebugUtilsMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    newDebugUtilsMessengerInfo.pfnUserCallback = debugCallback;
    
    return newDebugUtilsMessengerInfo;
}

VkResult GVULKANInstance::createDebugUtilsMessenger(VkInstance instance,
                                              const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                              const VkAllocationCallbacks* pAllocator,
                                              VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void GVULKANInstance::destroyDebugUtilsMessenger(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL GVULKANInstance::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData) {
    printf("GaneshaEngineLayer: %s\n", pCallbackData->pMessage);
    
    return VK_FALSE;
}

}
