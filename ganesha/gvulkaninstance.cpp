#include "gvulkaninstance.h"

namespace spcGaneshaEngine {

GVULKANInstance::GVULKANInstance() {
    
}

GVULKANInstance::~GVULKANInstance() {
    
}

void GVULKANInstance::createInstance(const std::string& applicationName, const TCharPointersArray& khronosValidationLayers, const TStringsArray& shouldNotUseExtensionsArray) {
    VkApplicationInfo applicationInfo = createApplicationInfo(applicationName);
    TCharPointersArray extensionsNamesArray = collectInstanceExtensionsNames(shouldNotUseExtensionsArray);
    TCharPointersArray availableValidationLayersArray = collectValidationLayers(khronosValidationLayers);

    VkInstanceCreateInfo instanceInfo = createInstanceInfo(applicationInfo, availableValidationLayersArray, extensionsNamesArray);
    auto result = vkCreateInstance(&instanceInfo, nullptr, &vulkanInstance);
    if (result != VK_SUCCESS) {
        GLOG_ERROR("error creating VULKAN instance\n");
    }
    
    for (const auto& name : extensionsNamesArray) {
        delete [] name;
    }

    if (!availableValidationLayersArray.empty()) {
        if (createDebugUtilsMessenger(vulkanInstance, &debugUtilsMessengerInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            GLOG_ERROR("failed to set up debug messenger\n");
        }
    }
}

void GVULKANInstance::destroyInstance() {
    destroyDebugUtilsMessenger(vulkanInstance, debugMessenger, nullptr);
    vkDestroyInstance(vulkanInstance, nullptr);
}

VkInstance GVULKANInstance::getVulkanInstance() {
    return vulkanInstance;
}

#pragma mark - Routine -

TCharPointersArray GVULKANInstance::collectValidationLayers(const TCharPointersArray& layersNamesArray) {
    TCharPointersArray supportedValidationLayersArray;

    TUInt count;
    if (vkEnumerateInstanceLayerProperties(&count, nullptr) != VK_SUCCESS) {
        return supportedValidationLayersArray;
    }
    std::vector<VkLayerProperties> availableLayersArray(count);
    if (vkEnumerateInstanceLayerProperties(&count, availableLayersArray.data()) != VK_SUCCESS) {
        return supportedValidationLayersArray;
    }

    for (const auto& layerProperties : availableLayersArray) {
        for (const auto layerName : layersNamesArray) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                supportedValidationLayersArray.push_back(layerName);
            }
        }
    }
    
    return supportedValidationLayersArray;
}

TCharPointersArray GVULKANInstance::collectInstanceExtensionsNames(const TStringsArray& shouldNotUseExtensionsArray) {
    TUInt count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    std::vector<VkExtensionProperties> instanceExtensionsArray(count);
    vkEnumerateInstanceExtensionProperties(nullptr, &count, instanceExtensionsArray.data());

    TCharPointersArray namesList = TCharPointersArray();
    for (const auto& extension : instanceExtensionsArray) {
        if (!shouldUseInstanceExtension(extension.extensionName, shouldNotUseExtensionsArray)) {
            continue;
        }
        char *newString = new char[VK_MAX_EXTENSION_NAME_SIZE];
        memcpy(newString, extension.extensionName, VK_MAX_EXTENSION_NAME_SIZE);
        namesList.emplace_back(newString);
    }

    return namesList;
}

TBool GVULKANInstance::shouldUseInstanceExtension(const char *instanceExtensionName, const TStringsArray& shouldNotUseExtensionsArray) {
    for (std::string avoidExtension : shouldNotUseExtensionsArray) {
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
    instanceInfo.enabledExtensionCount = static_cast<TUInt>(extensionsNamesArray.size());
    instanceInfo.ppEnabledExtensionNames = extensionsNamesArray.data();
    instanceInfo.pApplicationInfo = &applicationInfo;
    
    if (!availableValidationLayersList.empty()) {
        instanceInfo.enabledLayerCount = static_cast<TUInt>(availableValidationLayersList.size());
        instanceInfo.ppEnabledLayerNames = availableValidationLayersList.data();
        debugUtilsMessengerInfo = createDebugUtilsMessengerInfo();
        instanceInfo.pNext = &debugUtilsMessengerInfo;
    }
    
    return instanceInfo;
}

VkApplicationInfo GVULKANInstance::createApplicationInfo(const std::string& title) {
    VkApplicationInfo newApplicationInfo = {};
    
    TUInt pApiVersion = 0;
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
    GLOG_LAYER("%s\n", pCallbackData->pMessage);

    return VK_FALSE;
}

}
