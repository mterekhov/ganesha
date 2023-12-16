//
//  gvulkaninstance.cpp
//  ganesha
//
//  Created by Mihail Terekhov on 23.07.2021.
//

#include "gvulkaninstance.h"

namespace spcGaneshaEngine {

GVULKANInstance::GVULKANInstance(GLog& log) : log(log) {
    
}

GVULKANInstance::~GVULKANInstance() {
    
}

void GVULKANInstance::createInstance(const std::string& applicationName, const TCharPointersArray& khronosValidationLayers) {
    VkApplicationInfo applicationInfo = createApplicationInfo(applicationName);
    TCharPointersArray extensionsNamesArray = collectInstanceExtensionsNames();
    TCharPointersArray availableValidationLayersArray = collectValidationLayers(khronosValidationLayers);

    VkInstanceCreateInfo instanceInfo = createInstanceInfo(applicationInfo, availableValidationLayersArray, extensionsNamesArray);
    auto result = vkCreateInstance(&instanceInfo, nullptr, &vulkanInstance);
    if (result != VK_SUCCESS) {
        log.error("error creating VULKAN instance\n");
    }
    
    log.info("extensions:\n");
    for (const auto& name : extensionsNamesArray) {
        log.info("\t%s\n", name);
        delete [] name;
    }

    if (!availableValidationLayersArray.empty()) {
        if (createDebugUtilsMessenger(vulkanInstance, &debugUtilsMessengerInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            log.error("failed to set up debug messenger\n");
        }
    }
}

void GVULKANInstance::destroyInstance() {
    destroyDebugUtilsMessenger(vulkanInstance, debugMessenger, nullptr);
    vkDestroyInstance(vulkanInstance, nullptr);
}

VkInstance& GVULKANInstance::getVulkanInstance() {
    return vulkanInstance;
}

void GVULKANInstance::addInstanceExtensionsToAvoid(const TStringsArray& extensions) {
    removeExtensions.insert(removeExtensions.end(), extensions.begin(), extensions.end());
}

#pragma mark - Routine -

TCharPointersArray GVULKANInstance::collectValidationLayers(const TCharPointersArray& layersNamesArray) {
    TCharPointersArray supportedValidationLayersArray;

    uint32_t count;
    if (vkEnumerateInstanceLayerProperties(&count, nullptr) != VK_SUCCESS) {
        return supportedValidationLayersArray;
    }
    std::vector<VkLayerProperties> availableLayersArray(count);
    if (vkEnumerateInstanceLayerProperties(&count, availableLayersArray.data()) != VK_SUCCESS) {
        return supportedValidationLayersArray;
    }

    log.info("validation layers:\n");
    for (const auto& layerProperties : availableLayersArray) {
        log.info("\t%s\n", layerProperties.layerName);
        for (const auto layerName : layersNamesArray) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                supportedValidationLayersArray.push_back(layerName);
            }
        }
    }
    
    return supportedValidationLayersArray;
}

TCharPointersArray GVULKANInstance::collectInstanceExtensionsNames() {
    TCharPointersArray namesList = TCharPointersArray();
    uint32_t count = 0;
    if (vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr) != VK_SUCCESS) {
        return namesList;
    }
    std::vector<VkExtensionProperties> instanceExtensionsArray(count);
    if (vkEnumerateInstanceExtensionProperties(nullptr, &count, instanceExtensionsArray.data()) != VK_SUCCESS) {
        return namesList;
    }

    //  Collect names
    for (const auto& extension : instanceExtensionsArray) {
        if (!shouldUseInstanceExtension(extension.extensionName)) {
            continue;
        }
        char *newString = new char[VK_MAX_EXTENSION_NAME_SIZE];
        memcpy(newString, extension.extensionName, VK_MAX_EXTENSION_NAME_SIZE);
        namesList.emplace_back(newString);
    }

    return namesList;
}

bool GVULKANInstance::shouldUseInstanceExtension(const char *instanceExtensionName) {
    for (std::string avoidExtension : removeExtensions) {
        if (strcmp(instanceExtensionName, avoidExtension.c_str()) == 0) {
            return false;
        }
    }

    return true;
}

VkInstanceCreateInfo GVULKANInstance::createInstanceInfo(const VkApplicationInfo& applicationInfo,
                                                         const TCharPointersArray& availableValidationLayersList,
                                                         const TCharPointersArray& extensionsNamesArray) {
    VkInstanceCreateInfo instanceInfo = {};
    
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensionsNamesArray.size());
    instanceInfo.ppEnabledExtensionNames = extensionsNamesArray.data();
    instanceInfo.pApplicationInfo = &applicationInfo;
    
    if (!availableValidationLayersList.empty()) {
        instanceInfo.enabledLayerCount = static_cast<uint32_t>(availableValidationLayersList.size());
        instanceInfo.ppEnabledLayerNames = availableValidationLayersList.data();
        debugUtilsMessengerInfo = createDebugUtilsMessengerInfo();
        instanceInfo.pNext = &debugUtilsMessengerInfo;
    }
    
    return instanceInfo;
}

VkApplicationInfo GVULKANInstance::createApplicationInfo(const std::string& title) {
    VkApplicationInfo newApplicationInfo = {};
    
    uint32_t pApiVersion = 0;
    vkEnumerateInstanceVersion(&pApiVersion);
    
    newApplicationInfo.apiVersion = pApiVersion;
    newApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    newApplicationInfo.pApplicationName = title.c_str();
    newApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    newApplicationInfo.pEngineName = "Ganesha";
    newApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    
    return newApplicationInfo;
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
